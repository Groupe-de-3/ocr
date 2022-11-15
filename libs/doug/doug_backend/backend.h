#pragma once

#include "point2d.h"

typedef struct {
    const char *title;
    ipoint2d_t position;
    ipoint2d_t size;
    ipoint2d_t min_size;
    ipoint2d_t max_size;
} DougWindowProperties;

typedef struct DougBackendConnection DougBackendConnection;
typedef struct DougBackendWindow DougBackendWindow;

DougBackendConnection *doug_backend_connect(void);
void doug_backend_disconnect(DougBackendConnection *conn);

DougBackendWindow *doug_backend_window_new(
    DougBackendConnection *connection,
    DougWindowProperties properties
);

DougBackendConnection *doug_backend_window_get_connection(
    DougBackendWindow *window
);

ipoint2d_t doug_backend_window_get_size(DougBackendWindow *window);
ipoint2d_t doug_backend_window_get_min_size(DougBackendWindow *window);
ipoint2d_t doug_backend_window_get_max_size(DougBackendWindow *window);

void doug_backend_window_set_size(DougBackendWindow *window, ipoint2d_t size);
void doug_backend_window_set_min_size(DougBackendWindow *window, ipoint2d_t min_size);
void doug_backend_window_set_max_size(DougBackendWindow *window, ipoint2d_t min_size);
void doug_backend_window_set_title(DougBackendWindow *window, const char *title);

void doug_backend_window_destroy(DougBackendWindow *window);
