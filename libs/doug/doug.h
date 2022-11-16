#pragma once

#include "doug_backend.h"
#include "point2d.h"

typedef struct {
    DougBackendWindow *back_window;
} DougWindow;

DougWindow
     doug_window_new(const char *title, ipoint2d_t position, ipoint2d_t size);
void doug_window_destroy(DougWindow window);
