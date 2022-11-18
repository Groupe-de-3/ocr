#include "doug_backend.h"
#include "utils.h"
#include "xcccm.h"

#include "xcb/xcb.h"
#include "xcb/xcb_icccm.h"

#include "vec.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <err.h>

struct DougBackendConnection {
    xcb_connection_t *conn;
    DougBackendWindow **windows_vec;
};
struct DougBackendWindow {
    DougBackendConnection *conn;
    xcb_window_t window;
};

DougBackendConnection *doug_backend_connect() {
    DougBackendConnection *c = malloc(sizeof(DougBackendConnection));
    c->conn = xcb_connect(NULL, NULL);
    c->windows_vec = vec_new(DougBackendWindow*);
    if (c->conn == NULL)
        errx(1, "Could not connect with X server");
    return c;
}
void doug_backend_disconnect(DougBackendConnection *conn) {
    for (size_t i = 0; i < vec_size(conn->windows_vec); i++)
        doug_backend_window_destroy(conn->windows_vec[i], false);

    xcb_disconnect(conn->conn);
    vec_destroy(conn->windows_vec);
    free(conn);
}

DougBackendWindow *doug_backend_window_new(
    DougBackendConnection *connection,
    DougWindowProperties props
) {
    xcb_connection_t *conn = connection->conn;

    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(
        connection->conn
    )).data;
    uint32_t values[] = {screen->black_pixel, 0};
    
    xcb_window_t window = xcb_generate_id(connection->conn);
    values[0]           = screen->white_pixel;
    values[1]           = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
    assert(props.size.x >= 0);
    assert(props.size.y >= 0);

    xcb_create_window(
        conn, screen->root_depth, window, screen->root,
        (short)props.position.x, (short)props.position.y,
        (unsigned short)props.size.x, (unsigned short)props.size.y,
        1, XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual, XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, values
    );
    
    xcb_icccm_wm_hints_t hints;
    xcb_icccm_wm_hints_set_normal(&hints);
    xcb_icccm_set_wm_hints(conn, window, &hints);

    xcb_size_hints_t size_hints = { 0 };
    xcb_icccm_size_hints_set_position(
        &size_hints, true,
        props.position.x, props.position.y
    );
    xcb_icccm_size_hints_set_size(
        &size_hints, true,
        props.size.x, props.size.y
    );
    xcb_icccm_size_hints_set_min_size(
        &size_hints,
        props.min_size.x, props.min_size.y
    );
    xcb_icccm_size_hints_set_max_size(
        &size_hints,
        props.max_size.x, props.max_size.y
    );
    xcb_icccm_set_wm_normal_hints(conn, window, &size_hints);
    
    // TODO: Verify what the format number should be
    xcb_icccm_set_wm_name(
        conn, window, doug_natom(conn, "STRING"), 0,
        (uint32_t)strlen(props.title), props.title
    );
    
    DougBackendWindow *dbw = malloc(sizeof(DougBackendWindow));
    dbw->conn = connection;
    dbw->window = window;
    *vec_push(&connection->windows_vec, DougBackendWindow*) = dbw;
    return dbw;
}

void doug_backend_window_destroy(
    DougBackendWindow *window,
    bool disconnect_if_possible
) {
    // Find and remove this window from the connection's list
    for (size_t i = 0; i < vec_size(window->conn->windows_vec); i++) {
        if (window->conn->windows_vec[i] != window)
            continue;
        vec_swap_remove(window->conn->windows_vec, i);
        break;
    }
    xcb_destroy_window(window->conn->conn, window->window);
    
    if (disconnect_if_possible && vec_size(window->conn->windows_vec) == 0)
        doug_backend_disconnect(window->conn);

    free(window);
}

void doug_backend_window_show(DougBackendWindow *window) {
    xcb_map_window(window->conn->conn, window->window);
    xcb_flush(window->conn->conn);
}

void doug_backend_window_hide(DougBackendWindow *window) {
    xcb_unmap_window(window->conn->conn, window->window);
    xcb_flush(window->conn->conn);
}

DougBackendConnection *doug_backend_window_get_connection(
    DougBackendWindow *window
) {
    return window->conn;
}

static uint8_t get_size_hints(DougBackendWindow *window, xcb_size_hints_t *hints) {
    return xcb_icccm_get_wm_normal_hints_reply(
        window->conn->conn,
        xcb_icccm_get_wm_normal_hints_unchecked(window->conn->conn, window->window),
        hints, NULL
    );
}

ipoint2d_t doug_backend_window_get_size(DougBackendWindow *window) {
    xcb_size_hints_t hints;
    uint8_t result = get_size_hints(window, &hints); 
    if (result != 0 || (hints.flags & XCB_ICCCM_SIZE_HINT_P_SIZE) == 0)
        return ipoint2d(-1, -1);
    return ipoint2d(hints.width, hints.height);
}
ipoint2d_t doug_backend_window_get_min_size(DougBackendWindow *window) {
    xcb_size_hints_t hints;
    uint8_t result = get_size_hints(window, &hints); 
    if (result != 0 || (hints.flags & XCB_ICCCM_SIZE_HINT_P_MIN_SIZE) == 0)
        return ipoint2d(-1, -1);
    return ipoint2d(hints.min_width, hints.min_height);
}
ipoint2d_t doug_backend_window_get_max_size(DougBackendWindow *window) {
    xcb_size_hints_t hints;
    uint8_t result = get_size_hints(window, &hints); 
    if (result != 0 || (hints.flags & XCB_ICCCM_SIZE_HINT_P_MAX_SIZE) == 0)
        return ipoint2d(-1, -1);
    return ipoint2d(hints.max_width, hints.max_height);
}

void doug_backend_window_set_size(DougBackendWindow *window, ipoint2d_t size) {
    xcb_size_hints_t hints;
    uint8_t result = get_size_hints(window, &hints); 
    if (result != 0)
        hints = (xcb_size_hints_t) { 0 };
    
    xcb_icccm_size_hints_set_size(
        &hints, true,
        size.x, size.y
    );
    xcb_icccm_set_wm_normal_hints(window->conn->conn, window->window, &hints);
}
void doug_backend_window_set_position(DougBackendWindow *window, ipoint2d_t pos) {
    xcb_size_hints_t hints;
    uint8_t result = get_size_hints(window, &hints); 
    if (result != 0)
        hints = (xcb_size_hints_t) { 0 };
    
    xcb_icccm_size_hints_set_position(
        &hints, true,
        pos.x, pos.y
    );
    xcb_icccm_set_wm_normal_hints(window->conn->conn, window->window, &hints);
}
void doug_backend_window_set_min_size(DougBackendWindow *window, ipoint2d_t min_size) {
    xcb_size_hints_t hints;
    uint8_t result = get_size_hints(window, &hints); 
    if (result != 0)
        hints = (xcb_size_hints_t) { 0 };
    
    xcb_icccm_size_hints_set_min_size(
        &hints,
        min_size.x, min_size.y
    );
    xcb_icccm_set_wm_normal_hints(window->conn->conn, window->window, &hints);
}
void doug_backend_window_set_max_size(DougBackendWindow *window, ipoint2d_t max_size) {
    xcb_size_hints_t hints;
    uint8_t result = get_size_hints(window, &hints); 
    if (result != 0)
        hints = (xcb_size_hints_t) { 0 };
    
    xcb_icccm_size_hints_set_max_size(
        &hints,
        max_size.x, max_size.y
    );
    xcb_icccm_set_wm_normal_hints(window->conn->conn, window->window, &hints);
}
void doug_backend_window_set_title(DougBackendWindow *window, const char *title) {
    xcb_icccm_set_wm_name(
        window->conn->conn, window->window, doug_natom(window->conn->conn, "STRING"), 0,
        (uint32_t)strlen(title), title
    );
}
