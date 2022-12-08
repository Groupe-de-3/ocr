#pragma once
#include "image_view.h"
#include "matrices.h"

typedef struct PipelineIntermediaryResult {
    Image sudoku_img;
} PipelineIntermediaryResult;

typedef struct PipelineSecondaryResult {
    Image result;
    Matrix(char) sudoku;
    Matrix(_Bool) sudoku_mask;
} PipelineSecondaryResult;

PipelineIntermediaryResult pipeline_primary(ImageView *image);
PipelineSecondaryResult pipeline_seconday(PipelineIntermediaryResult *prslt);
