#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void print_wm_desktop(Display *display, Window window) {
    Atom          return_type;
    int           return_format;
    unsigned long remaining = 5;
    unsigned long items     = 1;
    long         *data;
    XGetWindowProperty(
        display, window, XInternAtom(display, "_NET_WM_DESKTOP", false), 0, 1,
        false, AnyPropertyType, &return_type, &return_format, &items,
        &remaining, (void *)&data
    );
    printf("WM_DESKTOP = %lx", *data);
    {
        char *name = XGetAtomName(display, return_type);
        printf(" as %s (%d * (%lu + %lu))\n", name, return_format, items, remaining);
        XFree(name);
    }
    XFree(data);
}

int main(void) {
    char *msg = "Hello, World!";

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int s = DefaultScreen(display);

    Window window = XCreateSimpleWindow(
        display, RootWindow(display, s), 10, 10, 200, 200, 1,
        BlackPixel(display, s), WhitePixel(display, s)
    );

    XSelectInput(display, window, ExposureMask | KeyPressMask);

    XSizeHints hints = {
        .flags      = PMinSize | PMaxSize,
        .min_height = 200,
        .min_width  = 500,
        .max_height = 300,
        .max_width  = 600,
    };
    XSetWMNormalHints(display, window, &hints);

    XMapWindow(display, window);

    XEvent event;
    for (;;) {
        XNextEvent(display, &event);

        // draw or redraw the window
        if (event.type == Expose) {
            XFillRectangle(
                display, window, DefaultGC(display, s), 20, 20, 10, 10
            );
            XDrawString(
                display, window, DefaultGC(display, s), 50, 50, msg,
                (int)strlen(msg)
            );
        }

        // exit on key press
        if (event.type == KeyPress) {
            if (event.xkey.keycode == 9)
                break;
            else if (event.xkey.keycode == 33) {
                print_wm_desktop(display, window);

                XEvent xevent;
                xevent.xclient = (XClientMessageEvent){
                    .type = ClientMessage,
                    .message_type =
                        XInternAtom(display, "_NET_WM_DESKTOP", false),
                    .window    = window,
                    .format    = 32,
                    .data.l[0] = 0,
                };
                XSendEvent(
                    display, RootWindow(display, s), false, 0, &xevent
                );
            }
        }
    }

    int   prop_length;
    Atom *props = XListProperties(display, window, &prop_length);
    for (int i = 0; i < prop_length; i++) {
        char *name = XGetAtomName(display, props[i]);
        printf("[%02d] %s\n", i, name);
        XFree(name);
    }
    XFree(props);

    print_wm_desktop(display, window);

    // close connection to the server
    XCloseDisplay(display);

    return 0;
}
