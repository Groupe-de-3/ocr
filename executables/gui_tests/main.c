#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "doug.h"
#include "point2d.h"

int main(void) {
    DougWindow window = doug_window_new(
        "My Window",
        ipoint2d(10, 10),
        ipoint2d(100, 100)
    );
    doug_window_show(&window);
    
    sleep(10);
    
    doug_window_hide(&window);

    sleep(2);
    
    doug_window_destroy(window);
}
