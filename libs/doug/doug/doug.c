#include "doug.h"
#include "doug_backend.h"

#include <stdbool.h>

DougWindow doug_window_new(
    const char *title,
    ipoint2d_t position, 
    ipoint2d_t size
) {
    DougBackendConnection *connection = doug_backend_connect();
    DougBackendWindow *back_window = doug_backend_window_new(connection, (DougWindowProperties) {
        .title = title,
        .position = position,
        .size = size,
        .min_size = ipoint2d(0, 0),
        .max_size = ipoint2d(0, 0),
    });
    
    DougWindow window = (DougWindow) {
        .back_window = back_window,
    };
    return window;
}

void doug_window_destroy(DougWindow window) {
    doug_backend_window_destroy(window.back_window, true);
}

void doug_window_show(DougWindow *window) {
    doug_backend_window_show(window->back_window);
}
void doug_window_hide(DougWindow *window) {
    doug_backend_window_hide(window->back_window);
}

void doug_window_set_size(DougWindow *window, ipoint2d_t size) {
    doug_backend_window_set_size(window->back_window, size);
}
void doug_window_set_position(DougWindow *window, ipoint2d_t pos) {
    doug_backend_window_set_position(window->back_window, pos);
}
void doug_window_set_max_size(DougWindow *window, ipoint2d_t max_size) {
    doug_backend_window_set_max_size(window->back_window, max_size);
}
void doug_window_set_min_size(DougWindow *window, ipoint2d_t min_size) {
    doug_backend_window_set_min_size(window->back_window, min_size);
}
