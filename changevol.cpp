/*
 * File: changevol.cpp
 *
 * Sound volume controller
 *
 */

#include "userdelfx.h"

static float s_depth;
static uint8_t s_time;

void DELFX_INIT(uint32_t platform, uint32_t api)
{
    s_depth = 1.0f;
    s_time = 0.5f;
}

void DELFX_PROCESS(float *main_xn, 
                   uint32_t frames)
{
    float * __restrict my = main_xn;
    const float * my_e = my + 2 * frames;

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
        *(my) = *(my) * s_depth * on_off;
        my++;
    }
}


void DELFX_PARAM(uint8_t index, int32_t value)
{
    const float valf = q31_to_f32(value);
    switch (index) {
    case k_user_delfx_param_time:
        s_time = clip01f(valf * 0.999) * 128;
        break;
    case k_user_delfx_param_depth:
        s_depth = valf;
        break;
    case k_user_delfx_param_shift_depth:
        break;
    default:
        break;
  }
}

