#include "bilinear_sampling.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "least_squares.h"
#include "matrices_algebra.h"
#include "point2d.h"
#include "utils_math.h"

some_pixel_t bilinear_sample(ImageView *img, float x, float y) {
    // Image must be in a "float channel" format
    size_t float_channels = img_float_channels_length(img->image->format);
    assert(float_channels != 0);

    // Splitting fractional and int part of the coordinates
    int   xI = (int)x;
    int   yI = (int)y;
    float xF = x - (float)xI;
    float yF = y - (float)yI;

    int x_plus = xI + 1;
    int y_plus = yI + 1;

    float top_value[float_channels];
    {
        any_pixel_t a = imgv_get_pixel_any(img, xI, yI);
        any_pixel_t b = imgv_get_pixel_any(img, x_plus, yI);
        for (size_t c = 0; c < float_channels; c++) {
            top_value[c] =
                a.float_channels[c] * (1.f - xF) + b.float_channels[c] * xF;
        }
    }
    float bottom_value[float_channels];
    {
        any_pixel_t a = imgv_get_pixel_any(img, xI, y_plus);
        any_pixel_t b = imgv_get_pixel_any(img, x_plus, y_plus);
        for (size_t c = 0; c < float_channels; c++) {
            bottom_value[c] =
                a.float_channels[c] * (1.f - xF) + b.float_channels[c] * xF;
        }
    }
    float final_value[float_channels];
    for (size_t c = 0; c < float_channels; c++) {
        final_value[c] = top_value[c] * (1.f - yF) + bottom_value[c] * yF;
        final_value[c] = clamp(final_value[c], 0.f, 1.f);
    }

    some_pixel_t out = {.format = img->image->format};
    memcpy(
        out.value.float_channels, final_value, sizeof(float) * float_channels
    );
    return out;
}

void bilinear_resize(ImageView *from, ImageView *to) {
    float x_scale = (float)from->width / (float)to->width;
    float y_scale = (float)from->height / (float)to->height;
    for (int y = 0; y < to->height; y++) {
        for (int x = 0; x < to->width; x++) {
            imgv_set_pixel_some(
                to, x, y,
                bilinear_sample(from, (float)x * x_scale, (float)y * y_scale)
            );
        }
    }
}

void bilinear_apply_matrix(ImageView *from, ImageView *to, Matrix(double) m) {
    Matrix(double) mat_a = m_new(double, 3, 1);
    Matrix(double) mat_b = m_new(double, 3, 1);

    Matrix(double) inv = m_new(double, 3, 3);
    m_copy(inv, m);
    mal_inverse(inv);

    printf("InvB:\n");
    m_fprint(inv);

    for (int y = 0; y < to->height; y++) {
        for (int x = 0; x < to->width; x++) {
            m_copy(mat_a, (double[]){(double)x, (double)y, 1.});
            m_mul(mat_a, inv, mat_b);
            double fx = mat_b[0] / mat_b[2];
            double fy = mat_b[1] / mat_b[2];

            imgv_set_pixel_some(
                to, x, y, bilinear_sample(from, (float)fx, (float)fy)
            );
        }
    }

    m_destroy(mat_a);
    m_destroy(mat_b);
    m_destroy(inv);
}

void bilinear_perspective_transmute(
    ImageView *from_img, fquadrilateral_t from_shape, ImageView *to,
    fquadrilateral_t to_shape
) {
    printf(
        "[(%f, %f), (%f, %f), (%f, %f), (%f, %f)]\n", from_shape.a.x,
        from_shape.a.y, from_shape.b.x, from_shape.b.y, from_shape.c.x,
        from_shape.c.y, from_shape.d.x, from_shape.d.y
    );
    printf(
        "[(%f, %f), (%f, %f), (%f, %f), (%f, %f)]\n", to_shape.a.x,
        to_shape.a.y, to_shape.b.x, to_shape.b.y, to_shape.c.x, to_shape.c.y,
        to_shape.d.x, to_shape.d.y
    );

    Matrix(double) a = m_new(double, 8, 8);
    Matrix(double) b = m_new(double, 1, 8);

    fpoint2d_t src[4] = {
        from_shape.a, from_shape.b, from_shape.c, from_shape.d};
    fpoint2d_t dst[4] = {to_shape.a, to_shape.b, to_shape.c, to_shape.d};
    for (size_t i = 0; i < 4; ++i) {
        m_get2(a, 0, i)     = src[i].x;
        m_get2(a, 3, i + 4) = src[i].x;

        m_get2(a, 1, i)     = src[i].y;
        m_get2(a, 4, i + 4) = src[i].y;

        m_get2(a, 2, i)     = 1;
        m_get2(a, 5, i + 4) = 1;

        m_get2(a, 3, i)     = 0;
        m_get2(a, 4, i)     = 0;
        m_get2(a, 5, i)     = 0;
        m_get2(a, 0, i + 4) = 0;
        m_get2(a, 1, i + 4) = 0;
        m_get2(a, 2, i + 4) = 0;

        m_get2(a, 6, i) = -src[i].x * dst[i].x;
        m_get2(a, 7, i) = -src[i].y * dst[i].x;

        m_get2(a, 6, i + 4) = -src[i].x * dst[i].y;
        m_get2(a, 7, i + 4) = -src[i].y * dst[i].y;

        m_get2(b, 0, i)     = dst[i].x;
        m_get2(b, 0, i + 4) = dst[i].y;
    }

    Matrix(double) x = m_new(double, 1, 8);
    la_solve_least_squares(a, b, x);

    Matrix(double) m = m_new(double, 3, 3);
    m_copy(m, (double[]){
        1, 0, 0,
        0, 2, 0,
        0, 0, 1,
    });
    memcpy(m, x, sizeof(double) * 8);

    bilinear_apply_matrix(from_img, to, m);

    m_destroy(a);
    m_destroy(b);
    m_destroy(x);
    m_destroy(m);
}
