#pragma once

#include "doug_backend.h"
#include "point2d.h"

typedef struct {
    DougBackendWindow *back_window;
} DougWindow;

DougWindow doug_window_new(const char *title, ipoint2d_t position, ipoint2d_t size);
void doug_window_destroy(DougWindow window);

void doug_window_show(DougWindow *window);
void doug_window_hide(DougWindow *window);

void doug_window_set_size(DougWindow *window, ipoint2d_t size);
void doug_window_set_position(DougWindow *window, ipoint2d_t pos);
void doug_window_set_max_size(DougWindow *window, ipoint2d_t max_size);
void doug_window_set_min_size(DougWindow *window, ipoint2d_t min_size);
