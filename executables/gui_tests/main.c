#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_util.h>
#include <xcb/xproto.h>

xcb_atom_t intern_atom(xcb_connection_t *conn, const char *at) {
    xcb_generic_error_t     *err;
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(
        conn, xcb_intern_atom(conn, false, (uint16_t)strlen(at), at), &err
    );
    if (err != NULL)
        errx(1, "No non ono");
    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}

char *get_atom_name(xcb_connection_t *conn, xcb_atom_t atom) {
    xcb_generic_error_t       *err;
    xcb_get_atom_name_reply_t *reply =
        xcb_get_atom_name_reply(conn, xcb_get_atom_name(conn, atom), &err);
    if (err != NULL)
        errx(1, "No non ono");
    char *name = xcb_get_atom_name_name(reply);
    free(reply);
    return name;
}

void print_wm_desktop(xcb_connection_t *conn, xcb_window_t window) {
    xcb_get_property_cookie_t cookie = xcb_get_property(
        conn, false, window, intern_atom(conn, "_NET_WM_DESKTOP"), 0, 0, 2
    );
    xcb_get_property_reply_t *reply =
        xcb_get_property_reply(conn, cookie, NULL);

    uint32_t *value = xcb_get_property_value(reply);

    printf("WM_DESKTOP = %u\n", *value);
    free(reply);
}

int main(void) {
    xcb_connection_t *conn = xcb_connect(NULL, NULL);
    if (conn == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

    xcb_gcontext_t gctx     = xcb_generate_id(conn);
    uint32_t       values[] = {screen->black_pixel, 0};
    xcb_create_gc(
        conn, gctx, screen->root, XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES,
        values
    );

    xcb_window_t window = xcb_generate_id(conn);
    values[0]           = screen->white_pixel;
    values[1]           = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
    xcb_create_window(
        conn, screen->root_depth, window, screen->root, 10, 10, 100, 100, 1,
        XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
        XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, values
    );

    xcb_size_hints_t size_hints = {0};
    xcb_icccm_size_hints_set_min_size(&size_hints, 100, 100);
    xcb_change_property(
        conn, XCB_PROP_MODE_REPLACE, window,
        intern_atom(conn, "WM_NORMAL_HINTS"),
        intern_atom(conn, "WM_SIZE_HINTS"), 32, sizeof(xcb_size_hints_t) / 4,
        &size_hints
    );

    xcb_icccm_wm_hints_t hints = {0};
    xcb_icccm_wm_hints_set_normal(&hints);
    xcb_change_property(
        conn, XCB_PROP_MODE_REPLACE, window, intern_atom(conn, "WM_HINTS"),
        intern_atom(conn, "WM_HINTS"), 32, sizeof(xcb_icccm_wm_hints_t) / 4,
        &size_hints
    );

    xcb_map_window(conn, window);
    xcb_flush(conn);

    bool done = false;
    while (!done) {
        xcb_generic_event_t *e = xcb_wait_for_event(conn);

        if (e == NULL) {
            int err = xcb_connection_has_error(conn);
            if (err > 0)
                errx(1, "Connection with the X server closed\n");
            else {
                printf("A");
                continue;
            }
        }

        switch (e->response_type) {
        case XCB_EXPOSE:
            {
                printf("Expose\n");
                xcb_rectangle_t r = {20, 20, 60, 60};
                xcb_poly_fill_rectangle(conn, window, gctx, 1, &r);
                xcb_flush(conn);
                break;
            }
        case XCB_KEY_PRESS:
            {
                xcb_key_press_event_t *kpe = (void *)e;
                if (kpe->detail == 9) {
                    done = true;
                    break;
                }

                printf("Pressed %d\n", kpe->detail);
                break;
            }
        }

        free(e);
    }

    /*
    int   prop_length;
    Atom *props = XListProperties(display, window, &prop_length);
    for (int i = 0; i < prop_length; i++) {
        char *name = XGetAtomName(display, props[i]);
        printf("[%02d] %s\n", i, name);
        XFree(name);
    }
    XFree(props);
    */

    //print_wm_desktop(conn, window);

    // close connection to the server
    xcb_disconnect(conn);

    return 0;
}
