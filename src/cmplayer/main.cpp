#include "application.hpp"
#include <QtCore/QDebug>
#include <QtCore/QMetaType>
#include <QtCore/QDateTime>
#include "fragmentprogram.hpp"

#define SHADER_GET_YUV(var, c) \
"TEX "var".x, "c", texture[1], 2D;"\
"MOV "var".y, "var".x;"\
"TEX "var".x, "c", texture[2], 2D;"\
"MOV "var".z, "var".x;"\
"TEX "var".x, "c", texture[0], 2D;"

#define SHADER_SET_COLOR_PROP(yuv_i, yuv_o) \
"SUB "yuv_o".x, "yuv_i".x, coef_r.a;"\
"SUB "yuv_i".y, "yuv_i".y, coef_g.a;"\
"SUB "yuv_i".z, "yuv_i".z, coef_b.a;"\
"MUL "yuv_o".yz, "yuv_i", param0.z;"\
"MUL "yuv_i".yz, "yuv_i".xzyw, param0.w;"\
"ADD "yuv_o".y, "yuv_o", "yuv_i";"\
"SUB "yuv_o".z, "yuv_o", "yuv_i";"\
"MUL "yuv_o", "yuv_o", param0.xyyz;"\
"ADD "yuv_o".x, "yuv_o", param1.x;"

#define SHADER_GET_YUV_AND_SET_COLOR_PROP(yuv)\
"TEX "yuv".x, fragment.texcoord[0], texture[1], 2D;"\
"SUB "yuv".z, "yuv".x, coef_g.a;"\
"TEX "yuv".x, fragment.texcoord[0], texture[2], 2D;"\
"SUB "yuv".w, "yuv".x, coef_b.a;"\
"MUL "yuv".x, "yuv".z, param0.z;"\
"MUL "yuv".y, "yuv".w, param0.w;"\
"ADD "yuv".y, "yuv".x, "yuv".y;"\
"MUL "yuv".x, "yuv".z, param0.w;"\
"MUL "yuv".w, "yuv".w, param0.z;"\
"SUB "yuv".z, "yuv".w, "yuv".x;"\
"TEX "yuv".x, fragment.texcoord[0], texture[0], 2D;"\
"SUB "yuv".x, "yuv", coef_r.a;"\
"MUL "yuv", "yuv", param0.xyyz;"\
"ADD "yuv".x, "yuv", param1.x;"

#define SHADER_HEADER \
"PARAM param0 = program.local[0];"\
"PARAM param1 = program.local[1];"\
"PARAM coef_r = {1.16438356,  0.0,          1.59602679,  0.0625};"\
"PARAM coef_g = {1.16438356, -0.391762290, -0.812967647, 0.5};"\
"PARAM coef_b = {1.16438356,  2.01723214,   0.0,         0.5};"

#define SHADER_YUV_TO_RGB(yuv, rgb) \
"DP3 "rgb".r, "yuv", coef_r;"\
"DP3 "rgb".g, "yuv", coef_g;"\
"DP3 "rgb".b, "yuv", coef_b;"\
"MOV "rgb".a, 1.0;"

static const char *i420ToRgb =
SHADER_HEADER
"TEMP yuv;"
"ATTRIB coord = fragment.texcoord[0];"
SHADER_GET_YUV_AND_SET_COLOR_PROP("yuv")
SHADER_YUV_TO_RGB("yuv", "result.color")
"END";

#include "i420_to_rgb.hpp";

static const char *testcode =
"ATTRIB coord=1;"
"TEMP yuv, rgb;"
"yuv = rgb + 1;"
"yuv += 1;"
"rgb = DP3(yuv, rgb);";

int main(int argc, char **argv) {
	Application app(argc, argv);
	const int ret = app.exec();
	return ret;
}
