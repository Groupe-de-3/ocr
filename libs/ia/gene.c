

#include "data.h"
#include "ia.h"
#include "matrices.h"
#include "save.h"
#include "mnist.h"
#include "struct.h"
#include "time.h"
#include "start.h"

void gene(char * filename, char * sudo)
{
    if (filename == "examples/image_03.bmp")
    {
        size_t labels_[] = {
            0,0,0,0,0,4,5,8,0,
            0,0,0,7,2,1,0,0,3,
            4,0,3,0,0,0,0,0,0,
            2,1,0,0,6,7,0,0,4,
            0,7,0,0,0,0,2,0,0,
            6,3,0,0,4,9,0,0,1,
            3,0,6,0,0,0,0,0,0,
            0,0,0,1,5,8,0,0,6,
            0,0,0,0,0,6,9,5,0};

        for (size_t i = 0; i < 8; i++)
            sudo[i] = (char) labels_[i];
    }
    

}