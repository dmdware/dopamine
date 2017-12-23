



#include "../sys/utils.h"
#include "shader.h"
#include "../sys/inc.h"
#include "../sys/syswin.h"
#include "../sys/debug.h"
#include "../sys/utils.h"

glshader g_shader[SHADERS];
signed char g_cursh = 0;

const char *SHTEXT[SHADERS][4]
=
{
	/* SH_ORTHO */
	{
"SH_ORTHO v",
"SH_ORTHO f",
"														\r\n"
"#version 120											\r\n"\
"														\r\n"\
"														\r\n"\
"uniform float width;									\r\n"\
"uniform float height;									\r\n"\
"														\r\n"\
"void main(void)										\r\n"\
"{														\r\n"\
"	gl_Position = vec4(gl_Vertex.x * 2.0 / width - 1.0,	\r\n"\
"		gl_Vertex.y * -2.0 / height + 1.0,				\r\n"\
"		0.5,											\r\n"\
"		1.0);											\r\n"\
"														\r\n"\
"	gl_TexCoord[0] = gl_MultiTexCoord0;					\r\n"\
"}														\r\n"\
"														\r\n",

"														\r\n"\
"#version 120											\r\n"\
"														\r\n"\
"uniform vec4 color;									\r\n"\
"														\r\n"\
"uniform sampler2D texture0;							\r\n"\
"														\r\n"\
"void main(void)										\r\n"\
"{														\r\n"\
"	gl_FragColor = color * texture2D(texture0, gl_TexCoord[0].xy);\r\n"\
"}														\r\n"\
"														\r\n"
	},

	/* SH_COLOR2D */
	{
"SH_COLOR2D v",
"SH_COLOR2D f",
"														\r\n"\
"#version 120											\r\n"\
"														\r\n"\
"														\r\n"\
"uniform float width;									\r\n"\
"uniform float height;									\r\n"\
"														\r\n"\
"														\r\n"\
"void main(void)										\r\n"\
"{														\r\n"\
"	gl_Position = vec4(gl_Vertex.x * 2.0 / width - 1.0,	\r\n"\
"		gl_Vertex.y * -2.0 / height + 1.0,				\r\n"\
"		0.5,											\r\n"\
"		1.0);											\r\n"\
"														\r\n"\
"}														\r\n"\
"														\r\n",
"														\r\n"\
"#version 120											\r\n"\
"														\r\n"\
"uniform vec4 color;									\r\n"\
"														\r\n"\
"														\r\n"\
"void main(void)										\r\n"\
"{														\r\n"\
"	gl_FragColor = color;								\r\n"\
"}														\r\n"\
"														\r\n"\
	},
	/* SH_COLOR3D */
{
"SH_COLOR3D v",
"SH_COLOR3D f",
""
"#version 120\r\n"
""
"uniform mat4 mvp;\r\n"
""
"uniform vec4 color;\r\n"
""
"void main(void)\r\n"
"{\r\n"
"	gl_Position = mvp * gl_Vertex;\r\n"
"	//gl_Position.w = 1;\r\n"
"	//gl_Position.z = - gl_Position.z;"
"}\r\n"
"",
""
"#version 120\r\n"
""
"uniform vec4 color;"
""
"void main(void)"
"{"
"	gl_FragColor = color;"
"	gl_FragColor = vec4(1,1,1,1);"
"}"
""
},
/* SH_E */
{
"SH_E v",
"SH_E f",
"#version 120\r\n"
"varying vec3 outpos;"
"uniform vec3 cornera, cornerb, cornerc, cornerd;"
"uniform vec3 camcen;"
"uniform mat4 mvp;"
""
"void main(void)"
"{"
"	outpos = gl_Vertex.xyz;"
"	gl_Position = gl_Vertex; gl_Position.w = 1;"
"}",
"#version 120											\r\n"\
"uniform sampler2D texture0;\r\n"
"uniform sampler2D h0;\r\n"
"varying vec3 outpos;\r\n"
"uniform vec3 cornera, cornerb, cornerc, cornerd;\r\n"
"uniform vec3 camcen;\r\n"
"uniform vec3 ep;\r\n"
"uniform vec3 mp;\r\n"
"uniform vec3 sp;\r\n"
"uniform vec3 view;\r\n"
"uniform vec3 right;\r\n"
"uniform vec3 up;\r\n"
"uniform float width;\r\n"
"uniform float height;\r\n"
"vec3 rot3f(vec3 v, float sa, float ca, vec3 a)\r\n"
"{\r\n"
"	vec3 newv;\r\n"
"	//float costheta, sintheta;\r\n"
"	//costheta = (float)cos(rad);\r\n"
"	//sintheta = (float)sin(rad);\r\n"
"	\r\n"
"	newv.x = (ca + (1 - ca) * a.x * a.x)		 *v.x;\r\n"
"	newv.x += ((1 - ca) * a.x * a.y - a.z * sa)	 *v.y;\r\n"
"	newv.x += ((1 - ca) * a.x * a.z + a.y * sa)	 *v.z;\r\n"
"	\r\n"
"	newv.y = ((1 - ca) * a.x * a.y + a.z * sa)	 *v.x;\r\n"
"	newv.y += (ca + (1 - ca) * a.y * a.y)		 *v.y;\r\n"
"	newv.y += ((1 - ca) * a.y * a.z - a.x * sa)	 *v.z;\r\n"
"\r\n"
"	newv.z = ((1 - ca) * a.x * a.z - a.y * sa)	 *v.x;\r\n"
"	newv.z += ((1 - ca) * a.y * a.z + a.x * sa)	 *v.y;\r\n"
"	newv.z += (ca + (1 - ca) * a.z * a.z)		 *v.z;\r\n"
"\r\n"
"	return newv;\r\n"
"}\r\n"
"vec3 refract(vec3 from, vec3 dp, vec3 cent, float i)\r\n"
"{\r\n"
//"	return dp;\r\n"
#if 0
"	vec3 gv = cent - from;\r\n"
"	float g = length(gv) * 2.0 * 6371000.0;\r\n"
"	gv = gv / length(gv);\r\n"
//"	vec3 ev = gv * sqrt(2 * 6.67 * (1.0/100000000000.0) * 5.972 * 1000000000000000000000000.0 / length(gv * 6371000.0));\r\n"
"	vec3 ev = gv * sqrt(2 * 6.67 * (1.0/100000000000.0) * 5.972 * 1000000000000000000000000.0 / length(g));\r\n"
"	g = 1.0 / (g*g);\r\n"
"	//dp = dp / g;\r\n"
//"	vec3 to = ( dp * 3000000.0 - gv * ev ) / 300000.0;\r\n"
"	vec3 to = ( dp * 2.0 * 6371000.0 * 3000000.0 + g * ev ) / 3000000.0;\r\n"
"	return to - from;\r\n"
#endif
"	//return ((from+dp-cent)/1.00001)-from+cent;\r\n"
"	vec3 gv = cent-from;\r\n"
"	float lgv = length(gv);"
"	vec3 ngv = gv / lgv;\r\n"
"	vec3 mgv = mp-from;\r\n"
"	vec3 sgv = sp-from;\r\n"
"	vec3 mngv = mgv / length(mgv);\r\n"
"	vec3 sngv = sgv / length(sgv);\r\n"
#if 0
"	//gl_FragColor = vec4(ngv, 1); gl_FragDepth = 0.5;\r\n"
"	vec3 ndp = dp / length(dp);\r\n"
"	vec3 ra = cross(ndp, ngv);\r\n"
"	//gl_FragColor = vec4(ra, 1); gl_FragDepth = 0.5;\r\n"
"	float ca = dot(ndp, ngv);\r\n"
"	float bd = length(cent-from);\r\n"
"	float ad = length(cent-from-dp);\r\n"
"	//gl_FragColor = vec4(vec3(1,ad,2)/length(vec3(1,ad,2)), 1); gl_FragDepth = 0.5;\r\n"
"	float sa = bd*bd/(ad*ad);\r\n"
"	//gl_FragColor = vec4(vec3(1,sa,2)*1/length(vec3(1,sa,2)), 1); gl_FragDepth = 0.5;\r\n"
"	float sap = sqrt(1 - ca*ca);\r\n"
"	//gl_FragColor = vec4(vec3(1,sap,2)*1/length(vec3(1,sap,2)), 1); gl_FragDepth = 0.5;\r\n"
//"	sa = (bd>ad)*1/(sa*ca) + (bd<=ad)*sa/ca;\r\n"
"	sa = max(0,bd-ad)/abs(bd-ad)*1/(sa*sap) + max(0,ad-bd)/abs(ad-bd)*sa/sap;\r\n"
"	//gl_FragColor = vec4(vec3(0,sa,0)*0.5/(min(0,length(vec3(0,sa,1)))+1), 1); gl_FragDepth = 0.5;\r\n"
"	sa = sap * - sqrt(1 - min(1,sa*sa)) + ca * - sa;\r\n"
"	sa = sa * 0.001;\r\n"
"	//sa = - sap * sqrt(1 - min(1,sa*sa)) + ca * sa;\r\n"
"	//gl_FragColor = vec4(vec3(0,sa,0)*-1/length(vec3(0,sa,0)), 1); gl_FragDepth = 0.5;\r\n"
"	ca = sqrt(1 - min(1,sa*sa));\r\n"
"	//gl_FragColor = vec4(vec3(1,ca,2)/length(vec3(1,ca,2)), 1); gl_FragDepth = 0.5;\r\n"
#endif
"	float p = ((0.5/150.0)/0.5*6371)/300000.0;"
#if 0
"	dp = rot3f(dp, sa, ca, ra);// * (ad*ad)/(bd*bd);\r\n"
"	//gl_FragColor = vec4(dp*-1/length(dp)*1, 1); gl_FragDepth = 0.5;\r\n"
"	//gl_FragColor = vec4((1.0/(ad*ad))/(1.0/(bd*bd))*vec3(0.5,0.5,0.5),1);\r\n"
"	//gl_FragColor = vec4(1.0/(ad-0.5)*vec3(0.0,1,1)+vec3(1,0,0),1);\r\n"
"	//gl_FragColor = vec4(vec3(1,sa,2)/length(vec3(1,sa,2)), 1); gl_FragDepth = 0.5;\r\n"
#endif
//"	float p = ((0.5/150.0)/0.5*6371*109)/300000.0;"
//"	return (cent-from-dp)/length(cent-from-dp)*sqrt(2 * 6.67 * 10000 * 5.9722 / (length(from+dp-cent)/0.5*6371))*(0.5/6371)*p+dp;\r\n"
//"	return ( ngv*sqrt(2 * 6.67 * 100000000 * 5.9722 / (1*length(from+dp-cent)/0.5*6371))*(0.5/6371)*p+dp );\r\n"
"	vec3 nv = ( //sngv*sqrt(2 * 6.67 * 10000000000.0 * 1.989 / (1*length(from+dp-sgv)/0.5*6371)) + \r\n"
"	ngv*sqrt(2 * 6.67 * 10000 * 5.9722 / (1*length(from+dp-gv)/0.5*6371)) + \r\n"
"	mngv*sqrt(2 * 6.67 * 100 * 7.34767309 / (1*length(from+dp-mgv)/0.5*6371)) )*(0.5/6371)*p+dp;\r\n"
"	float pp = (lgv - (0.5 - 0.1)) / ((90.0 - i) * length(nv));\r\n"
"	return nv * pp;\r\n"
//"	return ( ngv*sqrt(2 * 6.67 * 1000000 * 7.34767309 / (1*length(from+dp-cent)/0.5*6371))*(0.5/1737)*p+dp );// * s;\r\n"
//"	return (cent-from-dp)/length(cent-from-dp)*sqrt(2 * 6.67 * 10000 * 333000 * 5.9722 / (1*length(from+dp-cent)/0.5*6371*109))*(0.5/6371/109)*p+dp;\r\n"
"}\r\n"
"vec4 trylook(vec3 from, vec3 to, vec3 cent)\r\n"
"{\r\n"
"	float fd = length(cent - from);\r\n"
"	float td = length(cent - to);\r\n"
"	if(/* fd >= 0.9 && td <= 1.09 && */ fd >= td && td <= 1.0/2.0 /* && td >= 1.0/2.0 */)\r\n"
"	{\r\n"
"		vec4 color;\r\n"
"		vec2 tc;\r\n"
"		from = from - cent;\r\n"
"		to = to - cent;\r\n"
"		vec3 ft = (from + to) / 2.0; ft = to;\r\n"
"		tc.y = - 0.5 + ft.y - 0;\r\n"
"		tc.x = atan(ft.z, ft.x)/2/3.14159;\r\n"
"		//tc = vec2(0.5,0.5);\r\n"
"		color = texture2D(texture0, tc);\r\n"
"		//color.r = color.w = 1;\r\n"
"		//color.g = fd;\r\n"
"		return color;\r\n"
"	}\r\n"
"	return vec4(0,0,0,0);\r\n"
"}\r\n"
"void main (void)\r\n"
"{\r\n"
"	//gl_FragColor = vec4(1,1,1,1);\r\n"
"	//return;\r\n"
"	vec3 cent; // cent = vec3(cornera.xyz + cornerc.xyz) / 2.0;\r\n"
"	cent = ep;\r\n"
"	//cent.z *= -1;\r\n"
"	vec3 to = outpos.x * right * width / height + outpos.y * up + view + camcen;\r\n"
"	vec3 from = camcen;\r\n"
"	vec3 dp = to - from;\r\n"
"	dp = dp / length(dp*150);\r\n"
"	float i; float f = 0;\r\n"
"	vec4 outcolor = vec4(0,0,0,0);\r\n"
"	for(i=0; i<90.0; i+=1)\r\n"
"	{\r\n"
"		dp = refract(from, dp, cent, i);\r\n"
"		vec4 color = trylook(from, from + dp, cent);\r\n"
"		//color.w = color.r = 1;"
"		//if(color.w > 0.0)\r\n"
"		//{\r\n"
"		f = f + (1.0 - outcolor.w) * length(dp) / (0.5 * 12000000);"
"		outcolor = outcolor + color * max(0, color.w - outcolor.w);\r\n"
"		//gl_FragColor = color;\r\n"
"		//gl_FragColor = vec4(outpos * -1, 1);"
"		//gl_FragDepth = float(i) / 90.0.0;\r\n"
"		//return;\r\n"
"		//}\r\n"
"		from = from + dp;\r\n"
"		dp = dp / length(dp*150);\r\n"
"	}\r\n"
"	gl_FragColor = outcolor; gl_FragDepth = f;\r\n"
"	//gl_FragColor = vec4(1,1,1,1);\r\n"
"}"
},
/* SH_M */
{
"SH_M v",
"SH_M f",
"#version 120\r\n"
"varying vec3 outpos;"
"uniform vec3 cornera, cornerb, cornerc, cornerd;"
"uniform vec3 camcen;"
"uniform mat4 mvp;"
""
"void main(void)"
"{"
"	outpos = gl_Vertex.xyz;"
"	gl_Position = gl_Vertex; gl_Position.w = 1;"
"}",
"#version 120											\r\n"\
"uniform sampler2D texture0;\r\n"
"uniform sampler2D h0;\r\n"
"varying vec3 outpos;\r\n"
"uniform vec3 cornera, cornerb, cornerc, cornerd;\r\n"
"uniform vec3 camcen;\r\n"
"uniform vec3 ep;\r\n"
"uniform vec3 mp;\r\n"
"uniform vec3 sp;\r\n"
"uniform float rx;\r\n"
"uniform vec3 view;\r\n"
"uniform vec3 right;\r\n"
"uniform vec3 up;\r\n"
"uniform float width;\r\n"
"uniform float height;\r\n"
"vec3 rot3f(vec3 v, float sa, float ca, vec3 a)\r\n"
"{\r\n"
"	vec3 newv;\r\n"
"	\r\n"
"	newv.x = (ca + (1 - ca) * a.x * a.x)		 *v.x;\r\n"
"	newv.x += ((1 - ca) * a.x * a.y - a.z * sa)	 *v.y;\r\n"
"	newv.x += ((1 - ca) * a.x * a.z + a.y * sa)	 *v.z;\r\n"
"	\r\n"
"	newv.y = ((1 - ca) * a.x * a.y + a.z * sa)	 *v.x;\r\n"
"	newv.y += (ca + (1 - ca) * a.y * a.y)		 *v.y;\r\n"
"	newv.y += ((1 - ca) * a.y * a.z - a.x * sa)	 *v.z;\r\n"
"\r\n"
"	newv.z = ((1 - ca) * a.x * a.z - a.y * sa)	 *v.x;\r\n"
"	newv.z += ((1 - ca) * a.y * a.z + a.x * sa)	 *v.y;\r\n"
"	newv.z += (ca + (1 - ca) * a.z * a.z)		 *v.z;\r\n"
"\r\n"
"	return newv;\r\n"
"}\r\n"
"vec3 refract(vec3 from, vec3 dp, vec3 cent, float i)\r\n"
"{\r\n"
//"	return dp;\r\n"
#if 0
"	vec3 gv = cent - from;\r\n"
"	float g = length(gv) * 2.0 * 6371000.0;\r\n"
"	gv = gv / length(gv);\r\n"
//"	vec3 ev = gv * sqrt(2 * 6.67 * (1.0/100000000000.0) * 5.972 * 1000000000000000000000000.0 / length(gv * 6371000.0));\r\n"
"	vec3 ev = gv * sqrt(2 * 6.67 * (1.0/100000000000.0) * 5.972 * 1000000000000000000000000.0 / length(g));\r\n"
"	g = 1.0 / (g*g);\r\n"
"	//dp = dp / g;\r\n"
//"	vec3 to = ( dp * 3000000.0 - gv * ev ) / 300000.0;\r\n"
"	vec3 to = ( dp * 2.0 * 6371000.0 * 3000000.0 + g * ev ) / 3000000.0;\r\n"
"	return to - from;\r\n"
#endif
"	//return ((from+dp-cent)/1.00001)-from+cent;\r\n"
"	vec3 gv = cent-from;\r\n"
"	float lgv = length(gv);\r\n"
"	vec3 ngv = gv / lgv;\r\n"
"	vec3 egv = ep-from;\r\n"
"	vec3 sgv = sp-from;\r\n"
"	vec3 engv = egv / length(egv);\r\n"
"	vec3 sngv = sgv / length(sgv);\r\n"
#if 0
"	//gl_FragColor = vec4(ngv, 1); gl_FragDepth = 0.5;\r\n"
"	vec3 ndp = dp / length(dp);\r\n"
"	vec3 ra = cross(ndp, ngv);\r\n"
"	//gl_FragColor = vec4(ra, 1); gl_FragDepth = 0.5;\r\n"
"	float ca = dot(ndp, ngv);\r\n"
"	float bd = length(cent-from);\r\n"
"	float ad = length(cent-from-dp);\r\n"
"	//gl_FragColor = vec4(vec3(1,ad,2)/length(vec3(1,ad,2)), 1); gl_FragDepth = 0.5;\r\n"
"	float sa = bd*bd/(ad*ad);\r\n"
"	//gl_FragColor = vec4(vec3(1,sa,2)*1/length(vec3(1,sa,2)), 1); gl_FragDepth = 0.5;\r\n"
"	float sap = sqrt(1 - ca*ca);\r\n"
"	//gl_FragColor = vec4(vec3(1,sap,2)*1/length(vec3(1,sap,2)), 1); gl_FragDepth = 0.5;\r\n"
//"	sa = (bd>ad)*1/(sa*ca) + (bd<=ad)*sa/ca;\r\n"
"	sa = max(0,bd-ad)/abs(bd-ad)*1/(sa*sap) + max(0,ad-bd)/abs(ad-bd)*sa/sap;\r\n"
"	//gl_FragColor = vec4(vec3(0,sa,0)*0.5/(min(0,length(vec3(0,sa,1)))+1), 1); gl_FragDepth = 0.5;\r\n"
"	sa = sap * - sqrt(1 - min(1,sa*sa)) + ca * - sa;\r\n"
"	sa = sa * 0.001;\r\n"
"	//sa = - sap * sqrt(1 - min(1,sa*sa)) + ca * sa;\r\n"
"	//gl_FragColor = vec4(vec3(0,sa,0)*-1/length(vec3(0,sa,0)), 1); gl_FragDepth = 0.5;\r\n"
"	ca = sqrt(1 - min(1,sa*sa));\r\n"
"	//gl_FragColor = vec4(vec3(1,ca,2)/length(vec3(1,ca,2)), 1); gl_FragDepth = 0.5;\r\n"
#endif
"	//float gl = length(gv);	float s = 1.0 / (gl*gl);\r\n"
"	float p = ((0.5/150.0)/0.5*6371)/300000.0;"
#if 0
"	dp = rot3f(dp, sa, ca, ra);// * (ad*ad)/(bd*bd);\r\n"
"	//gl_FragColor = vec4(dp*-1/length(dp)*1, 1); gl_FragDepth = 0.5;\r\n"
"	//gl_FragColor = vec4((1.0/(ad*ad))/(1.0/(bd*bd))*vec3(0.5,0.5,0.5),1);\r\n"
"	//gl_FragColor = vec4(1.0/(ad-0.5)*vec3(0.0,1,1)+vec3(1,0,0),1);\r\n"
"	//gl_FragColor = vec4(vec3(1,sa,2)/length(vec3(1,sa,2)), 1); gl_FragDepth = 0.5;\r\n"
#endif
"	//return ( ngv*sqrt(2 * 6.67 * 1000000 * 7.34767309 / (1*length(from+dp-cent)/0.5*6371))*(0.5/6371)*p+dp );// * s;\r\n"
"	vec3 nv = ( //sngv*sqrt(2 * 6.67 * 10000000000.0 * 1.989 / (1*length(from+dp-sgv)/0.5*6371)) + \r\n"
"	engv*sqrt(2 * 6.67 * 10000 * 5.9722 / (1*length(from+dp-egv)/0.5*6371)) + \r\n"
"	ngv*sqrt(2 * 6.67 * 100 * 7.34767309 / (1*length(from+dp-gv)/0.5*6371)) )*(0.5/6371)*p+dp;\r\n"
"	float pp = (lgv - (0.5 - 0.1)*1737/6371) / ((90.0 - i) * length(nv));\r\n"
"	return nv * pp;\r\n"
"}\r\n"
"vec4 trylook(vec3 from, vec3 to, vec3 cent)\r\n"
"{\r\n"
"	float fd = length(cent - from);\r\n"
"	float td = length(cent - to);\r\n"
"	if(/* fd >= 0.9 && td <= 1.09 && */ fd >= td && td <= 1.0/2.0*0.272641 /* && fd >= 1.0/2.0*0.272641 */ )\r\n"
"	{\r\n"
"		vec4 color;\r\n"
"		vec2 tc;\r\n"
"		from = from - cent;\r\n"
"		to = to - cent;\r\n"
"		vec3 ft = (from + to) / 2.0; ft = (to - cent);\r\n"
"		tc.y = - 0.5 + ft.y / (6371.0/1737) - 0;\r\n"
"		tc.x = atan(ft.z, ft.x)/(2.0*3.14159) + rx;\r\n"
"		//tc.y *= (6371.0/1737); tc.x *= (6371.0/1737); \r\n"
"		color = texture2D(texture0, tc);\r\n"
"		//return vec4(1,1,1,0)/2 * length(to - cent) + vec4(0,0,0,1);	\r\n"
"		return color;\r\n"
"	}\r\n"
"	return vec4(0,0,0,0);\r\n"
"}\r\n"
"void main (void)\r\n"
"{\r\n"
"	vec3 cent; // cent = vec3(cornera.xyz + cornerc.xyz) / 2.0;\r\n"
"	cent = mp;\r\n"
"	vec3 to = outpos.x * right * width / height + outpos.y * up + view + camcen;\r\n"
"	vec3 from = camcen;\r\n"
"	vec3 dp = to - from;\r\n"
"	dp = dp / length(dp*150);\r\n"
"	float i; float f = 0;\r\n"
"	vec4 outcolor = vec4(0,0,0,0);\r\n"
"	for(i=0; i<90.0; i+=1)\r\n"
"	{\r\n"
"		dp = refract(from, dp, cent, i);\r\n"
"		vec4 color = trylook(from, from + dp, cent);\r\n"
"		f = f + (1.0 - outcolor.w) * length(dp) / (0.5 * 12000000);"
"		outcolor = outcolor + color * max(0, color.w - outcolor.w);\r\n"
"		from = from + dp;\r\n"
"		dp = dp / length(dp*150);\r\n"
"	}\r\n"
"	gl_FragColor = outcolor; gl_FragDepth = f; //gl_FragColor = vec4(1,1,1,1);\r\n"
"}"
},
/* SH_S */
{
"SH_S v",
"SH_S f",
"#version 120\r\n"
"varying vec3 outpos;"
"uniform vec3 cornera, cornerb, cornerc, cornerd;"
"uniform vec3 camcen;"
"uniform mat4 mvp;"
""
"void main(void)"
"{"
"	outpos = gl_Vertex.xyz;"
"	gl_Position = gl_Vertex; gl_Position.w = 1;"
"}",
"#version 120											\r\n"\
"uniform sampler2D texture0;\r\n"
"uniform sampler2D h0;\r\n"
"varying vec3 outpos;\r\n"
"uniform vec3 cornera, cornerb, cornerc, cornerd;\r\n"
"uniform vec3 camcen;\r\n"
"uniform vec3 ep;\r\n"
"uniform vec3 mp;\r\n"
"uniform vec3 sp;\r\n"
"uniform float rx;\r\n"
"uniform vec3 view;\r\n"
"uniform vec3 right;\r\n"
"uniform vec3 up;\r\n"
"uniform float width;\r\n"
"uniform float height;\r\n"
"vec3 rot3f(vec3 v, float sa, float ca, vec3 a)\r\n"
"{\r\n"
"	vec3 newv;\r\n"
"	\r\n"
"	newv.x = (ca + (1 - ca) * a.x * a.x)		 *v.x;\r\n"
"	newv.x += ((1 - ca) * a.x * a.y - a.z * sa)	 *v.y;\r\n"
"	newv.x += ((1 - ca) * a.x * a.z + a.y * sa)	 *v.z;\r\n"
"	\r\n"
"	newv.y = ((1 - ca) * a.x * a.y + a.z * sa)	 *v.x;\r\n"
"	newv.y += (ca + (1 - ca) * a.y * a.y)		 *v.y;\r\n"
"	newv.y += ((1 - ca) * a.y * a.z - a.x * sa)	 *v.z;\r\n"
"\r\n"
"	newv.z = ((1 - ca) * a.x * a.z - a.y * sa)	 *v.x;\r\n"
"	newv.z += ((1 - ca) * a.y * a.z + a.x * sa)	 *v.y;\r\n"
"	newv.z += (ca + (1 - ca) * a.z * a.z)		 *v.z;\r\n"
"\r\n"
"	return newv;\r\n"
"}\r\n"
"vec3 refract(vec3 from, vec3 dp, vec3 cent, float i)\r\n"
"{\r\n"
"	vec3 gv = cent-from;\r\n"
"	float lgv = length(gv);\r\n"
"	vec3 ngv = gv / lgv;\r\n"
"	vec3 egv = ep-from;\r\n"
"	vec3 mgv = mp-from;\r\n"
"	vec3 engv = egv / length(egv);\r\n"
"	vec3 mngv = mgv / length(mgv);\r\n"
"	//float gl = length(gv);	float s = 1.0 / (gl*gl);\r\n"
"	float p = ((0.5/150.0)/0.5*6371)/300000.0;"
"	//return ( ngv*sqrt(2 * 6.67 * 1000000 * 7.34767309 / (1*length(from+dp-cent)/0.5*6371))*(0.5/6371)*p+dp );// * s;\r\n"
"	vec3 nv = ( ngv*sqrt(2 * 6.67 * 10000000000.0 * 1.989 / (1*length(from+dp-gv)/0.5*6371)) + \r\n"
"	engv*sqrt(2 * 6.67 * 10000 * 5.9722 / (1*length(from+dp-egv)/0.5*6371)) + \r\n"
"	mngv*sqrt(2 * 6.67 * 100 * 7.34767309 / (1*length(from+dp-mgv)/0.5*6371)) )*(0.5/6371)*p+dp;\r\n"
"	float pp = (lgv - (0.5 - 0.4)*695700/6371) / ((90.0 - i) * length(nv));\r\n"
"	return nv * pp;\r\n"
"}\r\n"
"vec4 trylook(vec3 from, vec3 to, vec3 cent)\r\n"
"{\r\n"
"	float fd = length(cent - from);\r\n"
"	float td = length(cent - to);\r\n"
"	if(/* fd >= 0.9 && td <= 1.09 && */ fd >= td && td <= 1.0/2.0*695700/6371 /* && fd >= 1.0/2.0*0.272641 */ )\r\n"
"	{\r\n"
"		vec4 color;\r\n"
"		vec2 tc;\r\n"
"		from = from - cent;\r\n"
"		to = to - cent;\r\n"
"		vec3 ft = (from + to) / 2.0; ft = (to - cent);\r\n"
"		tc.y = - 0.5 + ft.y / 1;//(6371.0/695700) - 0;\r\n"
"		tc.x = atan(ft.z, ft.x)/(2.0*3.14159) + rx;\r\n"
"		color = texture2D(texture0, tc);\r\n"
"		return color;\r\n"
"	}\r\n"
"	return vec4(0,0,0,0);\r\n"
"}\r\n"
"void main (void)\r\n"
"{\r\n"
"	vec3 cent; // cent = vec3(cornera.xyz + cornerc.xyz) / 2.0;\r\n"
"	cent = sp;\r\n"
"	vec3 to = outpos.x * right * width / height + outpos.y * up + view + camcen;\r\n"
"	vec3 from = camcen;\r\n"
"	vec3 dp = to - from;\r\n"
"	dp = dp / length(dp*150);\r\n"
"	float i; float f = 0;\r\n"
"	vec4 outcolor = vec4(0,0,0,0);\r\n"
"	for(i=0; i<90.0; i+=1)\r\n"
"	{\r\n"
"		dp = refract(from, dp, cent, i);\r\n"
"		vec4 color = trylook(from, from + dp, cent);\r\n"
"		f = f + (1.0 - outcolor.w) * length(dp) / (0.5 * 12000000);"
"		outcolor = outcolor + color * max(0, color.w - outcolor.w);\r\n"
"		from = from + dp;\r\n"
"		dp = dp / length(dp*150);\r\n"
"	}\r\n"
"	gl_FragColor = outcolor; gl_FragDepth = f; //gl_FragColor = vec4(1,1,1,1);\r\n"
"}"
}
};

void shinit(glshader* s)
{
	s->program = 0;
	s->vertshader = 0;
	s->fragshader = 0;
}

void shfree(glshader* s)
{
	if(s->vertshader)
	{
		glDetachShader(s->program, s->vertshader);
		glDeleteShader(s->vertshader);
		s->vertshader = 0;
	}

	if(s->fragshader)
	{
		glDetachShader(s->program, s->fragshader);
		glDeleteShader(s->fragshader);
		s->fragshader = 0;
	}

	if(s->program)
	{
		glDeleteProgram(s->program);
		s->program = 0;
	}
}

GLint shgu(glshader* s, const char* strvar)
{
	if(!s->program)
		return -1;
	return glGetUniformLocation(s->program, strvar);
}

GLint shga(glshader* s, const char* strvar)
{
	if(!s->program)
		return -1;
	return glGetAttribLocation(s->program, strvar);
}

void shmu(glshader* s, int slot, const char* variable)
{
	s->slot[slot] = shgu(s, variable);
}

void shma(glshader* s, int slot, const char* variable)
{
	s->slot[slot] = shga(s, variable);
}

void glver(int* major, int* minor)
{
	/* for all versions */
	char* ver;

	char vermaj[6];
	char vermin[6];

	int i;
	int j;

	ver = (char*)glGetString(GL_VERSION); /* ver = "3.2.0" */

	for(i=0; i<6; i++)
	{
		if(ver[i] != '.')
			vermaj[i] = ver[i];
		else
		{
			vermaj[i] = '\0';
			i++;
			break;
		}
	}

	for(j=0; j<6; j++, i++)
	{
		if(ver[i] != '.')
			vermin[j] = ver[i];
		else
		{
			vermin[j] = '\0';
			i++;
			break;
		}
	}

	sscanf(vermaj, "%d", major);
	sscanf(vermin, "%d", minor);

	ver = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void inglsl()
{
	int major, minor;
	GLenum glewError;
	char* glexts;

#ifndef PLAT_MAC
	glewExperimental = GL_TRUE;
	glewError = glewInit();
	if( glewError != GLEW_OK )
	{
		errm("Error initializing GLEW!", (const char*)glewGetErrorString( glewError ));
		return;
	}
#endif

#ifdef __glew_h__
	if( !GLEW_VERSION_1_4 )
	{
		errm("Error", "OpenGL 1.4 not supported!\n" );
		goto quit;
	}
#endif

#ifndef PLAT_MOBILE
	/* glGetString deprecated after 3.0 */
	glexts = (char*)glGetString(GL_EXTENSIONS);

	fprintf(g_applog, glexts);

#if !defined( PLAT_MAC ) && !defined( PLAT_IOS )
#ifdef GLDEBUG
	if(!strstr(glexts, "GL_ARB_debug_output"))
	{
		fprintf(g_applog, "GL_ARB_debug_output extension not supported\r\n");
	}
	else
	{
		glDebugMessageCallback(&glmess, 0);

		CHECKGL();
	}
#endif
#endif

	if(!strstr(glexts, "GL_ARB_shader_objects"))
	{
		errm("Error", "GL_ARB_shader_objects extension not supported!");
		goto quit;
	}

	if(!strstr(glexts, "GL_ARB_shading_language_100"))
	{
		errm("Error", "GL_ARB_shading_language_100 extension not supported!");
		goto quit;
	}
#endif

	glver(&major, &minor);

#ifndef PLAT_MOBILE
	if(major < 1 || ( major == 1 && minor < 4 ))
	{
		errm("Error", "OpenGL 1.4 is not supported!");
		goto quit;
	}
#endif

	loadsh(SH_ORTHO, SHTEXT[SH_ORTHO][0], SHTEXT[SH_ORTHO][1], 
		SHTEXT[SH_ORTHO][2], SHTEXT[SH_ORTHO][3], 
		dtrue, dfalse);
	loadsh(SH_COLOR2D, SHTEXT[SH_COLOR2D][0], SHTEXT[SH_COLOR2D][1], 
		SHTEXT[SH_COLOR2D][2], SHTEXT[SH_COLOR2D][3], 
		dfalse, dfalse);
	loadsh(SH_COLOR3D, SHTEXT[SH_COLOR3D][0], SHTEXT[SH_COLOR3D][1], 
		SHTEXT[SH_COLOR3D][2], SHTEXT[SH_COLOR3D][3], 
		dfalse, dfalse);
	loadsh(SH_E, SHTEXT[SH_E][0], SHTEXT[SH_E][1],
		SHTEXT[SH_E][2], SHTEXT[SH_E][3],
		dfalse, dfalse);
	loadsh(SH_M, SHTEXT[SH_M][0], SHTEXT[SH_M][1],
		SHTEXT[SH_M][2], SHTEXT[SH_M][3],
		dfalse, dfalse);
	loadsh(SH_S, SHTEXT[SH_S][0], SHTEXT[SH_S][1],
		SHTEXT[SH_S][2], SHTEXT[SH_S][3],
		dfalse, dfalse);

	return;

quit:
	g_quit = dtrue;
}

void loadsh(int shader, 
			const char* namev,
			const char* namef,
			const char* strv, 
			const char* strf, 
			dbool hastexc, 
			dbool hasnorm)
{
	glshader* s;
	GLint loglen;
	GLchar* log;
	GLint status;

	s = g_shader+shader;

	if(s->vertshader || s->fragshader || s->program)
		shfree(s);

	s->hastexc = hastexc;
	s->hasnorm = hasnorm;
	s->vertshader = glCreateShader(GL_VERTEX_SHADER);
	s->fragshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(s->vertshader, 1, &strv, NULL);
	glShaderSource(s->fragshader, 1, &strf, NULL);

	glCompileShader(s->vertshader);

	glGetShaderiv(s->vertshader, GL_INFO_LOG_LENGTH, &loglen);
	if(loglen > 0)
	{
		log = (GLchar*)malloc(loglen);

		if(NOMEM(log))
			return;

		glGetShaderInfoLog(s->vertshader, loglen, &loglen, log);
		fprintf(g_applog, "shader %s compile log: %s\r\n", namev, log);
		fflush(g_applog);
		free(log);
	}

	glCompileShader(s->fragshader);
	glGetShaderiv(s->fragshader, GL_INFO_LOG_LENGTH, &loglen);
	if(loglen > 0)
	{
		log = (GLchar*)malloc(loglen);

		if(NOMEM(log))
			return;

		glGetShaderInfoLog(s->fragshader, loglen, &loglen, log);
		fprintf(g_applog, "shader %s compile log: %s\r\n", namef, log);
		fflush(g_applog);
		free(log);
	}

	s->program = glCreateProgram();
	glAttachShader(s->program, s->vertshader);
	glAttachShader(s->program, s->fragshader);
	glLinkProgram(s->program);

	fprintf(g_applog, "Program %s / %s :", namev, namef);
	fflush(g_applog);

	glGetProgramiv(s->program, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
	{
		log = (GLchar*)malloc(loglen);
		glGetProgramInfoLog(s->program, loglen, &loglen, log);
		fprintf(g_applog, "Program link log: %s\r\n", log);
		fflush(g_applog);
		free(log);
	}

	glGetProgramiv(s->program, GL_LINK_STATUS, &status);
	if (status == 0)
	{
		fprintf(g_applog, "link status 0\r\n");
		fflush(g_applog);
	}
	else
	{
		fprintf(g_applog, "link status ok, program=%d\r\n", s->program);
		fflush(g_applog);
	}

	fprintf(g_applog, "\r\n");
	fflush(g_applog);

	shma(s, SSLOT_POSITION, "position");
	shma(s, SSLOT_NORMAL, "normalin");
	shma(s, SSLOT_TEXCOORD0, "texcoordin0");
	shmu(s, SSLOT_TEXTURE0, "texture0");
	shmu(s, SSLOT_COLOR, "color");
	shmu(s, SSLOT_WIDTH, "width");
	shmu(s, SSLOT_HEIGHT, "height");
	shmu(s, SSLOT_MVP, "mvp");
	shmu(s, SSLOT_CAMCEN, "camcen");
	shmu(s, SSLOT_CORNERA, "cornera");
	shmu(s, SSLOT_CORNERB, "cornerb");
	shmu(s, SSLOT_CORNERC, "cornerc");
	shmu(s, SSLOT_CORNERD, "cornerd");
	shmu(s, SSLOT_EP, "ep");
	shmu(s, SSLOT_MP, "mp");
	shmu(s, SSLOT_RX, "rx");
	shmu(s, SSLOT_VIEW, "view");
	shmu(s, SSLOT_RIGHT, "right");
	shmu(s, SSLOT_UP, "up");
	shmu(s, SSLOT_SP, "sp");
}

void usesh(int shader)
{
	glshader* s;

	CHECKGL();
	g_cursh = shader;

	s = g_shader+g_cursh;

	glUseProgram(s->program);
	CHECKGL();

#ifdef PLAT_GL14
	/* opengl 1.4 way */
	glEnableClientState(GL_VERTEX_ARRAY);
	if(s->hastexc)	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if(s->hasnorm)	glEnableClientState(GL_NORMAL_ARRAY);
#endif
}

void endsh()
{
	glshader* s;
	CHECKGL();

	if(g_cursh < 0)
		return;

	s = g_shader+g_cursh;

#ifdef PLAT_GL14
	//opengl 1.4 way
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
#endif

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0);

	g_cursh = -1;
}
