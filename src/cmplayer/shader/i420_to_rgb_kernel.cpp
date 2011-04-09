#include "i420_to_rgb.hpp"

DEC_COMMON_VARS();
PARAM param1 = program.local[1];
PARAM param2 = program.local[2];
PARAM param3 = program.local[3];
TEMP t1, t2, t3;
ALIAS ycc = t3;
ALIAS yuv = t1;
ALIAS rgb = t2;
GET_YCbCr_KERNEL_APPLIED(ycc, t1, t2);
ycc.x = ycc.x - coef_r.a;
ycc.y = ycc.y - coef_g.a;
ycc.z = ycc.z - coef_b.a;
SET_COLOR_PROP(yuv, ycc);
YUV_TO_RGB_SAT(rgb, yuv);
rgb = rgb * param1;
output = param1.w + rgb;
output.a = 1.0;
