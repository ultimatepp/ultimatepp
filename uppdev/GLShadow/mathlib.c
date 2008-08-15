/*	3d math lib
 *
 *		written by Alexander Zaprjagaev
 *		frustum@public.tsu.ru
 */

#include <math.h>
#include "mathlib.h"

float v_length(const float *v) {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float v_normalize(const float *v,float *out) {
	float length,ilength;
	length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if(length == 0) {
		v_clear(out);
		return 0;
	}
	ilength = 1.0 / length;
	v_scale(v,ilength,out);
	return length;
}

void v_cross(const float *v1,const float *v2,float *out) {
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void v_translate(const float *m,float *out) {
	out[0] = m[12];
	out[1] = m[13];
	out[2] = m[14];
}

void v_transform(const float *v,const float *m,float *out) {
	float v1[3];
	v_copy(v,v1);
	out[0] = v1[0] * m[0] + v1[1] * m[4] + v1[2] * m[8] + m[12];
	out[1] = v1[0] * m[1] + v1[1] * m[5] + v1[2] * m[9] + m[13];
	out[2] = v1[0] * m[2] + v1[1] * m[6] + v1[2] * m[10] + m[14];
}

void v_transform_normal(const float *v,const float *m,float *out) {
	float v1[3];
	v_copy(v,v1);
	out[0] = v1[0] * m[0] + v1[1] * m[4] + v1[2] * m[8];
	out[1] = v1[0] * m[1] + v1[1] * m[5] + v1[2] * m[9];
	out[2] = v1[0] * m[2] + v1[1] * m[6] + v1[2] * m[10];
}

void m_identity(float *m) {
	m[0] = 1; m[4] = 0; m[8] = 0; m[12] = 0;
	m[1] = 0; m[5] = 1; m[9] = 0; m[13] = 0;
	m[2] = 0; m[6] = 0; m[10] = 1; m[14] = 0;
	m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1;
}

void m_multiply(const float *m1,const float *m2,float *out) {
	out[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	out[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	out[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	out[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
	out[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	out[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	out[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	out[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
	out[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	out[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	out[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	out[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
	out[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	out[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	out[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	out[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
}

int m_inverse(const float *m,float *out) {
	float   det;
	det = m[0] * m[5] * m[10];
	det += m[4] * m[9] * m[2];
	det += m[8] * m[1] * m[6];
	det -= m[8] * m[5] * m[2];
	det -= m[4] * m[1] * m[10];
	det -= m[0] * m[9] * m[6];
	if(det * det < 1e-6) return -1;
	det = 1.0 / det;
	out[0] =    (m[5] * m[10] - m[9] * m[6]) * det;
	out[1] =  - (m[1] * m[10] - m[9] * m[2]) * det;
	out[2] =    (m[1] * m[6] -  m[5] * m[2]) * det;
	out[3] = 0.0;
	out[4] =  - (m[4] * m[10] - m[8] * m[6]) * det;
	out[5] =    (m[0] * m[10] - m[8] * m[2]) * det;
	out[6] =  - (m[0] * m[6] -  m[4] * m[2]) * det;
	out[7] = 0.0;
	out[8] =    (m[4] * m[9] -  m[8] * m[5]) * det;
	out[9] =  - (m[0] * m[9] -  m[8] * m[1]) * det;
	out[10] =   (m[0] * m[5] -  m[4] * m[1]) * det;
	out[11] = 0.0;
	out[12] = - (m[12] * out[0] + m[13] * out[4] + m[14] * out[8]);
	out[13] = - (m[12] * out[1] + m[13] * out[5] + m[14] * out[9]);
	out[14] = - (m[12] * out[2] + m[13] * out[6] + m[14] * out[10]);
	out[15] = 1.0;
	return 0;
}

void m_transpose(const float *m,float *out) {
	out[0] = m[0]; out[4] = m[1]; out[8] = m[2]; out[12] = m[3];
	out[1] = m[4]; out[5] = m[5]; out[9] = m[6]; out[13] = m[7];
	out[2] = m[8]; out[6] = m[9]; out[10] = m[10]; out[14] = m[11];
	out[3] = m[12]; out[7] = m[13]; out[11] = m[14]; out[15] = m[15];
}

void m_rotation_x(float angle,float *out) {
	float rad = angle * DEG2RAD;
	float Cos = cos(rad);
	float Sin = sin(rad);
	out[0] = 1.0; out[4] = 0.0; out[8] = 0.0; out[12] = 0.0;
	out[1] = 0.0; out[5] = Cos; out[9] = -Sin; out[13] = 0.0;
	out[2] = 0.0; out[6] = Sin; out[10] = Cos; out[14] = 0.0;
	out[3] = 0.0; out[7] = 0.0; out[11] = 0.0; out[15] = 1.0;
}

void m_rotation_y(float angle,float *out) {
	float rad = angle * DEG2RAD;
	float Cos = cos(rad);
	float Sin = sin(rad);
	out[0] = Cos; out[4] = 0.0; out[8] = Sin; out[12] = 0.0;
	out[1] = 0.0; out[5] = 1.0; out[9] = 0.0; out[13] = 0.0;
	out[2] = -Sin; out[6] = 0.0; out[10] = Cos; out[14] = 0.0;
	out[3] = 0.0; out[7] = 0.0; out[11] = 0.0; out[15] = 1.0;
}

void m_rotation_z(float angle,float *out) {
	float rad = angle * DEG2RAD;
	float Cos = cos(rad);
	float Sin = sin(rad);
	out[0] = Cos; out[4] = -Sin; out[8] = 0.0; out[12] = 0.0;
	out[1] = Sin; out[5] = Cos; out[9] = 0.0; out[13] = 0.0;
	out[2] = 0.0; out[6] = 0.0; out[10] = 1.0; out[14] = 0.0;
	out[3] = 0.0; out[7] = 0.0; out[11] = 0.0; out[15] = 1.0;
}

void m_translate(const float *v,float *out) {
	out[0] = 1.0; out[4] = 0.0; out[8] = 0.0; out[12] = v[0];
	out[1] = 0.0; out[5] = 1.0; out[9] = 0.0; out[13] = v[1];
	out[2] = 0.0; out[6] = 0.0; out[10] = 1.0; out[14] = v[2];
	out[3] = 0.0; out[7] = 0.0; out[11] = 0.0; out[15] = 1.0;
}

void m_look_at(const float *eye,const float *dir,const float *up,float *out) {
	float x[3],y[3],z[3],ieye[3],m1[16],m2[16];
	v_sub(eye,dir,z);
	v_normalize(z,z);
	v_cross(up,z,x);
	v_cross(z,x,y);
	v_normalize(x,x);
	v_normalize(y,y);
	m1[0] = x[0]; m1[4] = x[1]; m1[8] = x[2]; m1[12] = 0.0;
	m1[1] = y[0]; m1[5] = y[1]; m1[9] = y[2]; m1[13] = 0.0;
	m1[2] = z[0]; m1[6] = z[1]; m1[10] = z[2]; m1[14] = 0.0;
	m1[3] = 0.0; m1[7] = 0.0; m1[11] = 0.0; m1[15] = 1.0;
	v_scale(eye,-1,ieye);
	m_translate(ieye,m2);
	m_multiply(m1,m2,out);
}

void m_shadow(const float *plane,const float *light,float *out) {
	float dot;
	dot = plane[0] * light[0] + plane[1] * light[1] +
		  plane[2] * light[2] + plane[3] * light[3];
	out[0] = -light[0] * plane[0] + dot;
	out[1] = -light[1] * plane[0];
	out[2] = -light[2] * plane[0];
	out[3] = -light[3] * plane[0];
	out[4] = -light[0] * plane[1];
	out[5] = -light[1] * plane[1] + dot;
	out[6] = -light[2] * plane[1];
	out[7] = -light[3] * plane[1];
	out[8] = -light[0] * plane[2];
	out[9] = -light[1] * plane[2];
	out[10] = -light[2] * plane[2] + dot;
	out[11] = -light[3] * plane[2];
	out[12] = -light[0] * plane[3];
	out[13] = -light[1] * plane[3];
	out[14] = -light[2] * plane[3];
	out[15] = -light[3] * plane[3] + dot;
}

void q_set(const float *dir,float angle,float *out) {
	float sinangle,length,ilength;
	length = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
	if(length != 0.0) {
		ilength = 1.0 / length;
		angle *= DEG2RAD / 2.0;
		sinangle = sin(angle);
		out[0] = dir[0] * ilength * sinangle;
		out[1] = dir[1] * ilength * sinangle;
		out[2] = dir[2] * ilength * sinangle;
		out[3] = cos(angle);
		return;
	}
	out[0] = out[1] = out[2] = 0.0;
	out[3] = 1.0;
}

void q_copy(const float *q,float *out) {
	out[0] = q[0];
	out[1] = q[1];
	out[2] = q[2];
	out[3] = q[3];
}

void q_slerp(const float *q1,const float *q2,float t,float *out) {
	float omega,cosomega,sinomega,k1,k2,q[4];
	cosomega = q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];
	if(cosomega < 0.0) {
		cosomega = -cosomega;
		q[0] = -q2[0];
		q[1] = -q2[1];
		q[2] = -q2[2];
		q[3] = -q2[3];
	} else {
		q[0] = q2[0];
		q[1] = q2[1];
		q[2] = q2[2];
		q[3] = q2[3];
	}
	if(1.0 - cosomega > 1e-6) {
		omega = acos(cosomega);
		sinomega = sin(omega);
		k1 = sin((1.0 - t) * omega) / sinomega;
		k2 = sin(t * omega) / sinomega;
	} else {
		k1 = 1.0 - t;
		k2 = t;
	}
	out[0] = q1[0] * k1 + q[0] * k2;
	out[1] = q1[1] * k1 + q[1] * k2;
	out[2] = q1[2] * k1 + q[2] * k2;
	out[3] = q1[3] * k1 + q[3] * k2;
}

void q_multiply(const float *q1,const float *q2,float *out) {
	out[0] = q1[3] * q2[0] + q1[0] * q2[0] + q1[1] * q2[2] - q1[2] * q2[1];
	out[1] = q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2];
	out[2] = q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0];
	out[3] = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
}

void q_to_matrix(const float *q,float *out) {
	float x2,y2,z2,xx,yy,zz,xy,yz,xz,wx,wy,wz;
	x2 = q[0] + q[0];
	y2 = q[1] + q[1];
	z2 = q[2] + q[2];
	xx = q[0] * x2;
	yy = q[1] * y2;
	zz = q[2] * z2;
	xy = q[0] * y2;
	yz = q[1] * z2;
	xz = q[2] * x2;
	wx = q[3] * x2;
	wy = q[3] * y2;
	wz = q[3] * z2;
	out[0] = 1.0 - (yy + zz);
	out[1] = xy + wz;
	out[2] = xz - wy;
	out[3] = 0.0;
	out[4] = xy - wz;
	out[5] = 1.0 - (xx + zz);
	out[6] = yz + wx;
	out[7] = 0.0;
	out[8] = xz + wy;
	out[9] = yz - wx;
	out[10] = 1.0 - (xx + yy);
	out[11] = 0.0;
	out[12] = 0.0;
	out[13] = 0.0;
	out[14] = 0.0;
	out[15] = 1.0;
}
