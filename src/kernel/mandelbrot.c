/*
    Copyright 2019-2021 Moseley Instruments
    N.A. Moseley
    All rights reserved

    Mandelbrot test

*/

#include "mandelbrot.h"

uint32_t mandel_iter(const float c_re, const float c_im, uint32_t maxiter)
{
    // z = z^2 + c
    //
    // (a+jb)*(a+jb) = (a^2 - b^2) + j(2ab)
    //
    //
    //
    uint32_t iter = 0;
    float zre = c_re;
    float zim = c_im;

    float zre2;
    float re2,im2;
    while(iter < maxiter)
    {        
        re2 = zre*zre;
        im2 = zim*zim;
        if ((re2 + im2) > 4.0f)
        {
            return iter;
        }

        zre2 = re2 - im2 + c_re;
        zim = 2.0f*zre*zim + c_im;
        zre = zre2;

        iter++;
    }
    return iter;
}
