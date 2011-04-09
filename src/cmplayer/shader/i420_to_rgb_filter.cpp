#include "i420_to_rgb.hpp"

DEC_COMMON_VARS();
PARAM param1 = program.local[1];
TEMP yuv, rgb;
GET_YUV(rgb, coord);
SET_COLOR_PROP(yuv, rgb);
YUV_TO_RGB_SAT(rgb, yuv);
rgb = rgb * param1;
output = param1.w + rgb;
output.a = 1.0;
