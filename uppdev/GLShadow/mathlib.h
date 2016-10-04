/*	3d math lib
 *
 *		written by Alexander Zaprjagaev
 *		frustum@public.tsu.ru
 */

#ifndef __MATHLIB_H__
#define __MATHLIB_H__

#define PI 3.14159265358979323846
#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)

#define v_set(x,y,z,v) { (v)[0] = x; (v)[1] = y; (v)[2] = z; }
#define v_clear(v) { (v)[0] = (v)[1] = (v)[2] = 0; }
#define v_copy(a,b) { (b)[0] = (a)[0]; (b)[1] = (a)[1]; (b)[2] = (a)[2]; }
#define v_scale(a,b,c) { \
	(c)[0] = (a)[0] * (b); \
	(c)[1] = (a)[1] * (b); \
	(c)[2] = (a)[2] * (b); \
}
#define v_add(a,b,c) { \
	(c)[0] = (a)[0] + (b)[0]; \
	(c)[1] = (a)[1] + (b)[1]; \
	(c)[2] = (a)[2] + (b)[2]; \
}
#define v_sub(a,b,c) { \
	(c)[0] = (a)[0] - (b)[0]; \
	(c)[1] = (a)[1] - (b)[1]; \
	(c)[2] = (a)[2] - (b)[2]; \
}
#define v_dot(a,b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])

float v_length(const float *v);
float v_normalize(const float *v,float *out);
void v_cross(const float *v1,const float *v2,float *out);
void v_translate(const float *m,float *out);
void v_transform(const float *v,const float *m,float *out);
void v_transform_normal(const float *v,const float *m,float *out);
void m_identity(float *m);
void m_multiply(const float *m1,const float *m2,float *out);
int m_inverse(const float *m,float *out);
void m_transpose(const float *m,float *out);
void m_rotation_x(float angle,float *out);
void m_rotation_y(float angle,float *out);
void m_rotation_z(float angle,float *out);
void m_translate(const float *v,float *out);
void m_look_at(const float *eye,const float *dir,const float *up,float *out);
void m_shadow(const float *plane,const float *light,float *out);
void q_set(const float *dir,float angle,float *out);
void q_copy(const float *q,float *out);
void q_slerp(const float *q1,const float *q2,float t,float *out);
void q_multiply(const float *q1,const float *q2,float *out);
void q_to_matrix(const float *q,float *out);

#endif /* __MATHLIB_H__ */
