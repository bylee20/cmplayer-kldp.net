#ifndef I420_TO_RGB_HPP
#define I420_TO_RGB_HPP

#define DEC_COMMON_VARS() \
ATTRIB coord = fragment.texcoord[0];\
OUTPUT output = result.color;\
PARAM param0 = program.local[0];\
PARAM coef_r = {1.16438356,  0.0,          1.59602679,  0.0625};\
PARAM coef_g = {1.16438356, -0.391762290, -0.812967647, 0.5};\
PARAM coef_b = {1.16438356,  2.01723214,   0.0,         0.5};

#define GET_YCbCr(var, c) \
var.x = TEX(c, texture[1], 2D);\
var.y = var.x;\
var.x = TEX(c, texture[2], 2D);\
var.z = var.x;\
var.x = TEX(c, texture[0], 2D);

#define GET_YUV(var, c) \
var.x = TEX(c, texture[1], 2D);\
var.y = var.x - coef_g.a;\
var.x = TEX(c, texture[2], 2D);\
var.z = var.x - coef_b.a;\
var.x = TEX(c, texture[0], 2D);\
var.x = var.x - coef_r.a;

#define SET_COLOR_PROP(yuv, yuv_in) \
yuv.x = yuv_in.x;\
yuv.zw = yuv_in.xxyz * param0.z;\
yuv_in.zw = yuv_in.xxzy * param0.w;\
yuv.y = yuv.z + yuv_in.z;\
yuv.z = yuv.w - yuv_in.w;\
yuv = yuv * param0.xyyz;\
yuv.x = yuv.x + coord.z;

#define YUV_TO_RGB(rgb, yuv) \
rgb.r = DP3(yuv, coef_r);\
rgb.g = DP3(yuv, coef_g);\
rgb.b = DP3(yuv, coef_b);

#define YUV_TO_RGB_SAT(rgb, yuv) \
rgb.r = DP3_SAT(yuv, coef_r);\
rgb.g = DP3_SAT(yuv, coef_g);\
rgb.b = DP3_SAT(yuv, coef_b);

#define GET_YCbCr_AND_ADD_TO(ycc, temp, center, dxdy)\
ycc.w = ycc.x;\
temp.zw = ycc.xxyz;\
temp.xy = center + dxdy;\
GET_YCbCr(ycc, temp);\
ycc.w = ycc.w + ycc.x;\
temp.zw = temp + ycc.wxyz;\
temp.xy = center - dxdy;\
GET_YCbCr(ycc, temp);\
ycc.x = ycc.w + ycc;\
ycc.yz = temp.xzwy + ycc;

#define GET_YCbCr_KERNEL_APPLIED(ycc, tmp1, tmp2)\
tmp1 = 0.0;\
GET_YCbCr_AND_ADD_TO(tmp1, ycc, coord, param2.wyxx);\
GET_YCbCr_AND_ADD_TO(tmp1, ycc, coord, param2.xwxx);\
tmp1 = tmp1 * param3.y;\
tmp2 = 0.0;\
GET_YCbCr_AND_ADD_TO(tmp2, ycc, coord, param2.zyxx);\
GET_YCbCr_AND_ADD_TO(tmp2, ycc, coord, param2.xyxx);\
tmp2 = tmp2 * param3.z;\
GET_YCbCr(ycc, coord);\
ycc = ycc * param3.x;\
ycc = ycc + tmp1;\
ycc = ycc + tmp2;

#endif
