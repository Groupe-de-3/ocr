#include "doug.h"

#include <stdbool.h>

#include "doug_backend.h"

DougWindow
doug_window_new(const char *title, ipoint2d_t position, ipoint2d_t size) {
    DougBackendConnection *connection  = doug_backend_connect();
    DougBackendWindow     *back_window = doug_backend_window_new(
            connection,
            (DougWindowProperties){
                .title    = title,
                .position = position,
                .size     = size,
                .min_size = ipoint2d(0, 0),
                .max_size = ipoint2d(0, 0),
        }
        );

    DougWindow window = (DougWindow){
        .back_window = back_window,
    };
    return window;
}

void doug_window_destroy(DougWindow window) {
    doug_backend_window_destroy(window.back_window, true);
}
