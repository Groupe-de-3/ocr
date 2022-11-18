#pragma once

#include "point2d.h"
#include <stdint.h>

typedef unsigned int doug_backend_keycode_t;

typedef struct {
    _Bool is_down;
    ipoint2d_t mouse_pos;
    doug_backend_keycode_t keycode;
} DougBackendKeyboardEvent;

typedef struct {
    _Bool is_down;
    ipoint2d_t mouse_pos;
} DougBackendMouseMoveEvent;

typedef struct {
    _Bool is_down;
    
} DougBackendMouseButtonEvent;

enum DougBackendEventType {
    DOUG_BACKEND_KEYBOARD_EVENT,
    DOUG_BACKEND_MOUSE_MOVE_EVENT,
    DOUG_BACKEND_MOUSE_BUTTON_EVENT,
};

typedef struct {
    enum DougBackendEventType type;
    union {
        DougBackendKeyboardEvent keyboard;
        DougBackendMouseMoveEvent mouse_move;
    } event;
} DougBackendEvent;

