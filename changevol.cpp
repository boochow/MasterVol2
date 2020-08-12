/*
 * File: changevol.cpp
 *
 * Sound volume controller
 *
 */

#include "usermodfx.h"

static float s_depth;
static uint8_t s_time;

void MODFX_INIT(uint32_t platform, uint32_t api)
{
    s_depth = 1.0f;
    s_time = 0.5f;
}

void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames)
{
    const float * mx = main_xn;
    float * __restrict my = main_yn;
    const float * my_e = my + 2 * frames;

    const float *sx = sub_xn;
    float * __restrict sy = sub_yn;

    float on_off;
    if (s_time < 32) {
        on_off = 0;
    } else if (s_time < 64) {
        on_off = 1.0;
    } else if (s_time < 96) {
        on_off = 1.f / 32 * (s_time - 64);
    } else {
        on_off = 1.f / 32 * (127 - s_time);
    }

    for (; my != my_e; ) {
        *(my++) = *(mx++) * s_depth * on_off;
        *(my++) = *(mx++) * s_depth * on_off;
        *(sy++) = *(sx++) * s_depth * on_off;
        *(sy++) = *(sx++) * s_depth * on_off;
    }
}


void MODFX_PARAM(uint8_t index, int32_t value)
{
    const float valf = q31_to_f32(value);
    switch (index) {
    case k_user_modfx_param_time:
        s_time = clip01f(valf * 0.999) * 128;
        break;
    case k_user_modfx_param_depth:
        s_depth = valf;
        break;
    default:
        break;
  }
}

