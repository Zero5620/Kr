#include "KrMath.h"

Mat2 Mat2::Scalar(float x, float y) {
	Mat2 m;
	m.rows[0] = Vec2(x, 0.0f);
	m.rows[1] = Vec2(0.0f, y);
	return m;
}

Mat2 Mat2::Scalar(Vec2 s) {
	Mat2 m;
	m.rows[0] = Vec2(s.x, 0.0f);
	m.rows[1] = Vec2(0.0f, s.y);
	return m;
}

Mat2 Mat2::Rotation(float angle) {
	float c = MathCos(angle);
	float s = MathSin(angle);

	Mat2  mat;
	mat.rows[0] = Vec2(c, -s);
	mat.rows[1] = Vec2(s, c);
	return mat;
}

Mat3 Mat3::Scalar(float S_1, float S_2) {
	Mat3 m;
	m.rows[0] = Vec3(S_1, 0.0f, 0.0f);
	m.rows[1] = Vec3(0.0f, S_2, 0.0f);
	m.rows[2] = Vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat3 Mat3::Scalar(Vec2 s) {
	return Scalar(s.x, s.y);
}

Mat3 Mat3::Translation(float T_x, float T_y) {
	Mat3 m;
	m.rows[0] = Vec3(1.0f, 0.0f, T_x);
	m.rows[1] = Vec3(0.0f, 1.0f, T_y);
	m.rows[2] = Vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat3 Mat3::Translation(Vec2 t) {
	return Translation(t.x, t.y);
}

Mat3 Mat3::Rotation(float angle) {
	Mat3  m;
	float c = MathCos(angle);
	float s = MathSin(angle);
	m.rows[0] = Vec3(c, -s, 0.0f);
	m.rows[1] = Vec3(s, c, 0.0f);
	m.rows[2] = Vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat3 Mat3::LookAt(Vec2 from, Vec2 to, Vec2 forward) {
	Vec2  dir = Normalize(to - from);
	float cos_theta = DotProduct(dir, forward);
	float sin_theta = MathSquareRoot(1.0f - cos_theta * cos_theta);

	Mat3  m;
	m.rows[0] = Vec3(cos_theta, -sin_theta, from.x);
	m.rows[1] = Vec3(sin_theta, cos_theta, from.y);
	m.rows[2] = Vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::Scalar(float S_1, float S_2, float S_3) {
	Mat4 m;
	m.rows[0] = Vec4(S_1, 0.0f, 0.0f, 0.0f);
	m.rows[1] = Vec4(0.0f, S_2, 0.0f, 0.0f);
	m.rows[2] = Vec4(0.0f, 0.0f, S_3, 0.0f);
	m.rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::Scalar(Vec3 s) {
	return Scalar(s.x, s.y, s.z);
}

Mat4 Mat4::Translation(float T_x, float T_y, float T_z) {
	Mat4 m;
	m.rows[0] = Vec4(1.0f, 0.0f, 0.0f, T_x);
	m.rows[1] = Vec4(0.0f, 1.0f, 0.0f, T_y);
	m.rows[2] = Vec4(0.0f, 0.0f, 1.0f, T_z);
	m.rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::Translation(Vec3 t) {
	return Translation(t.x, t.y, t.z);
}

Mat4 Mat4::RotationX(float angle) {
	Mat4 m;
	m.rows[0] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	m.rows[1] = Vec4(0.0f, MathCos(angle), -MathSin(angle), 0.0f);
	m.rows[2] = Vec4(0.0f, MathSin(angle), MathCos(angle), 0.0f);
	m.rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::RotationY(float angle) {
	Mat4 m;
	m.rows[0] = Vec4(MathCos(angle), 0.0f, MathSin(angle), 0.0f);
	m.rows[1] = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
	m.rows[2] = Vec4(-MathSin(angle), 0.0f, MathCos(angle), 0.0f);
	m.rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::RotationZ(float angle) {
	Mat4 m;
	m.rows[0] = Vec4(MathCos(angle), -MathSin(angle), 0.0f, 0.0f);
	m.rows[1] = Vec4(MathSin(angle), MathCos(angle), 0.0f, 0.0f);
	m.rows[2] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
	m.rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::Rotation(float x, float y, float z, float angle) {
	float c = MathCos(angle);
	float s = MathSin(angle);

	float x2 = x * x;
	float xy = x * y;
	float zx = x * z;
	float y2 = y * y;
	float yz = y * z;
	float z2 = z * z;

	Mat4  m;
	m.rows[0] = Vec4(c + x2 * (1 - c), xy * (1 - c) - z * s, zx * (1 - c) + y * s, 0.0f);
	m.rows[1] = Vec4(xy * (1 - c) + z * s, c + y2 * (1 - c), yz * (1 - c) - x * s, 0.0f);
	m.rows[2] = Vec4(zx * (1 - c) - y * s, yz * (1 - c) + x * s, c + z2 * (1 - c), 0.0f);
	m.rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::Rotation(Vec3 axis, float angle) {
	return Rotation(axis.x, axis.y, axis.z, angle);
}

Mat4 Mat4::LookAt(Vec3 from, Vec3 to, Vec3 world_up) {
	Vec3 forward = Normalize(from - to);
	Vec3 right = Normalize(CrossProduct(world_up, forward));
	Vec3 up = CrossProduct(right, forward);

	Mat4 lookat;
	lookat.rows[0] = Vec4(right.x, up.x, forward.x, -from.x);
	lookat.rows[1] = Vec4(right.y, up.y, forward.y, -from.y);
	lookat.rows[2] = Vec4(right.z, up.z, forward.z, -from.z);
	lookat.rows[3] = Vec4(0, 0, 0, 1);

	return lookat;
}

Mat4 Mat4::LookAtDirection(Vec3 dir, Vec3 world_up) {
	Assert(!IsNull(dir));

	Vec3 forward = dir;
	Vec3 right = Normalize(CrossProduct(world_up, forward));
	Vec3 up = CrossProduct(right, forward);

	Mat4 lookat;
	lookat.rows[0] = Vec4(right.x, up.x, forward.x, 0);
	lookat.rows[1] = Vec4(right.y, up.y, forward.y, 0);
	lookat.rows[2] = Vec4(right.z, up.z, forward.z, 0);
	lookat.rows[3] = Vec4(0, 0, 0, 1);

	return lookat;
}

Mat4 Mat4::OrthographicProjectionRH(float l, float r, float t, float b, float n, float f) {
	float iwidth = 1 / (r - l);
	float iheight = 1 / (t - b);
	float range = 1 / (n - f);

	Mat4  m;
	m.rows[0] = Vec4(2 * iwidth, 0.0f, 0.0f, -(l + r) * iwidth);
	m.rows[1] = Vec4(0.0f, 2 * iheight, 0.0f, -(t + b) * iheight);
	m.rows[2] = Vec4(0.0f, 0.0f, range, -n * range);
	m.rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::OrthographicProjectionLH(float l, float r, float t, float b, float n, float f) {
	float iwidth = 1 / (r - l);
	float iheight = 1 / (t - b);
	float range = 1 / (f - n);

	Mat4  m;
	m.rows[0] = Vec4(2 * iwidth, 0.0f, 0.0f, -(l + r) * iwidth);
	m.rows[1] = Vec4(0.0f, 2 * iheight, 0.0f, -(t + b) * iheight);
	m.rows[2] = Vec4(0.0f, 0.0f, range, -n * range);
	m.rows[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 Mat4::PerspectiveProjectionRH(float fov, float aspect_ratio, float n, float f) {
	float height = 1.0f / MathTan(fov * 0.5f);
	float width = height / aspect_ratio;
	float range = 1 / (n - f);

	Mat4  m;
	m.rows[0] = Vec4(width, 0.0f, 0.0f, 0.0f);
	m.rows[1] = Vec4(0.0f, height, 0.0f, 0.0f);
	m.rows[2] = Vec4(0.0f, 0.0f, f * range, -1.0f * f * n * range);
	m.rows[3] = Vec4(0.0f, 0.0f, -1.0f, 0.0f);
	return m;
}

Mat4 Mat4::PerspectiveProjectionLH(float fov, float aspect_ratio, float n, float f) {
	float height = 1.0f / MathTan(fov * 0.5f);
	float width = height / aspect_ratio;
	float range = 1 / (f - n);

	Mat4  m;
	m.rows[0] = Vec4(width, 0.0f, 0.0f, 0.0f);
	m.rows[1] = Vec4(0.0f, height, 0.0f, 0.0f);
	m.rows[2] = Vec4(0.0f, 0.0f, f * range, -1.0f * f * n * range);
	m.rows[3] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
	return m;
}

float Determinant(const Mat2 &mat) {
	return mat.m2[0][0] * mat.m2[1][1] - mat.m2[0][1] * mat.m2[1][0];
}

Mat2 Inverse(const Mat2 &mat) {
	float det = mat.m2[0][0] * mat.m2[1][1] - mat.m2[0][1] * mat.m2[1][0];
	det /= det;
	Mat2 res;
	res.m2[0][0] = mat.m2[1][1];
	res.m2[0][1] = -mat.m2[0][1];
	res.m2[1][0] = -mat.m2[1][0];
	res.m2[1][1] = mat.m2[0][0];
	res.m[0] *= det;
	res.m[1] *= det;
	res.m[2] *= det;
	res.m[3] *= det;
	return res;
}

Mat2 Transpose(const Mat2 &m) {
	Mat2 t;
	t.m2[0][0] = m.m2[0][0];
	t.m2[0][1] = m.m2[1][0];
	t.m2[1][0] = m.m2[0][1];
	t.m2[1][1] = m.m2[1][1];
	return t;
}

float Determinant(const Mat3 &mat) {
	float det = mat.m2[0][0] * (mat.m2[1][1] * mat.m2[2][2] - mat.m2[2][1] * mat.m2[1][2]) +
		mat.m2[0][1] * (mat.m2[1][2] * mat.m2[2][0] - mat.m2[1][0] * mat.m2[2][2]) +
		mat.m2[0][2] * (mat.m2[1][0] * mat.m2[2][1] - mat.m2[2][0] * mat.m2[1][1]);
	return det;
}

Mat3 Inverse(const Mat3 &mat) {
	Mat3 result;
	result.m2[0][0] = mat.m2[1][1] * mat.m2[2][2] - mat.m2[2][1] * mat.m2[1][2];
	result.m2[0][1] = mat.m2[0][2] * mat.m2[2][1] - mat.m2[0][1] * mat.m2[2][2];
	result.m2[0][2] = mat.m2[0][1] * mat.m2[1][2] - mat.m2[0][2] * mat.m2[1][1];
	result.m2[1][0] = mat.m2[1][2] * mat.m2[2][0] - mat.m2[1][0] * mat.m2[2][2];
	result.m2[1][1] = mat.m2[0][0] * mat.m2[2][2] - mat.m2[0][2] * mat.m2[2][0];
	result.m2[1][2] = mat.m2[1][0] * mat.m2[0][2] - mat.m2[0][0] * mat.m2[1][2];
	result.m2[2][0] = mat.m2[1][0] * mat.m2[2][1] - mat.m2[2][0] * mat.m2[1][1];
	result.m2[2][1] = mat.m2[2][0] * mat.m2[0][1] - mat.m2[0][0] * mat.m2[2][1];
	result.m2[2][2] = mat.m2[0][0] * mat.m2[1][1] - mat.m2[1][0] * mat.m2[0][1];

	float det = mat.m2[0][0] * result.m2[0][0] + mat.m2[0][1] * result.m2[1][0] + mat.m2[0][2] * result.m2[2][0];
	det /= det;
	for (int i = 0; i < 3; i++)
		result.rows[i] = result.rows[i] * det;
	return result;
}

Mat3 Transpose(const Mat3 &m) {
	Mat3 res;
	res.rows[0] = Vec3(m.m2[0][0], m.m2[1][0], m.m2[2][0]);
	res.rows[1] = Vec3(m.m2[0][1], m.m2[1][1], m.m2[2][1]);
	res.rows[2] = Vec3(m.m2[0][2], m.m2[1][2], m.m2[2][2]);
	return res;
}

float Determinant(const Mat4 &mat) {
	float m0 = mat.m[5] * mat.m[10] * mat.m[15] - mat.m[5] * mat.m[11] * mat.m[14] - mat.m[9] * mat.m[6] * mat.m[15] +
		mat.m[9] * mat.m[7] * mat.m[14] + mat.m[13] * mat.m[6] * mat.m[11] - mat.m[13] * mat.m[7] * mat.m[10];

	float m4 = -mat.m[4] * mat.m[10] * mat.m[15] + mat.m[4] * mat.m[11] * mat.m[14] + mat.m[8] * mat.m[6] * mat.m[15] -
		mat.m[8] * mat.m[7] * mat.m[14] - mat.m[12] * mat.m[6] * mat.m[11] + mat.m[12] * mat.m[7] * mat.m[10];

	float m8 = mat.m[4] * mat.m[9] * mat.m[15] - mat.m[4] * mat.m[11] * mat.m[13] - mat.m[8] * mat.m[5] * mat.m[15] +
		mat.m[8] * mat.m[7] * mat.m[13] + mat.m[12] * mat.m[5] * mat.m[11] - mat.m[12] * mat.m[7] * mat.m[9];

	float m12 = -mat.m[4] * mat.m[9] * mat.m[14] + mat.m[4] * mat.m[10] * mat.m[13] + mat.m[8] * mat.m[5] * mat.m[14] -
		mat.m[8] * mat.m[6] * mat.m[13] - mat.m[12] * mat.m[5] * mat.m[10] + mat.m[12] * mat.m[6] * mat.m[9];

	float det = mat.m[0] * m0 + mat.m[1] * m4 + mat.m[2] * m8 + mat.m[3] * m12;
	return det;
}

Mat4 Inverse(const Mat4 &mat) {
	Mat4 result;

	result.m[0] = mat.m[5] * mat.m[10] * mat.m[15] - mat.m[5] * mat.m[11] * mat.m[14] -
		mat.m[9] * mat.m[6] * mat.m[15] + mat.m[9] * mat.m[7] * mat.m[14] + mat.m[13] * mat.m[6] * mat.m[11] -
		mat.m[13] * mat.m[7] * mat.m[10];

	result.m[4] = -mat.m[4] * mat.m[10] * mat.m[15] + mat.m[4] * mat.m[11] * mat.m[14] +
		mat.m[8] * mat.m[6] * mat.m[15] - mat.m[8] * mat.m[7] * mat.m[14] - mat.m[12] * mat.m[6] * mat.m[11] +
		mat.m[12] * mat.m[7] * mat.m[10];

	result.m[8] = mat.m[4] * mat.m[9] * mat.m[15] - mat.m[4] * mat.m[11] * mat.m[13] - mat.m[8] * mat.m[5] * mat.m[15] +
		mat.m[8] * mat.m[7] * mat.m[13] + mat.m[12] * mat.m[5] * mat.m[11] - mat.m[12] * mat.m[7] * mat.m[9];

	result.m[12] = -mat.m[4] * mat.m[9] * mat.m[14] + mat.m[4] * mat.m[10] * mat.m[13] +
		mat.m[8] * mat.m[5] * mat.m[14] - mat.m[8] * mat.m[6] * mat.m[13] -
		mat.m[12] * mat.m[5] * mat.m[10] + mat.m[12] * mat.m[6] * mat.m[9];

	result.m[1] = -mat.m[1] * mat.m[10] * mat.m[15] + mat.m[1] * mat.m[11] * mat.m[14] +
		mat.m[9] * mat.m[2] * mat.m[15] - mat.m[9] * mat.m[3] * mat.m[14] - mat.m[13] * mat.m[2] * mat.m[11] +
		mat.m[13] * mat.m[3] * mat.m[10];

	result.m[5] = mat.m[0] * mat.m[10] * mat.m[15] - mat.m[0] * mat.m[11] * mat.m[14] -
		mat.m[8] * mat.m[2] * mat.m[15] + mat.m[8] * mat.m[3] * mat.m[14] + mat.m[12] * mat.m[2] * mat.m[11] -
		mat.m[12] * mat.m[3] * mat.m[10];

	result.m[9] = -mat.m[0] * mat.m[9] * mat.m[15] + mat.m[0] * mat.m[11] * mat.m[13] +
		mat.m[8] * mat.m[1] * mat.m[15] - mat.m[8] * mat.m[3] * mat.m[13] - mat.m[12] * mat.m[1] * mat.m[11] +
		mat.m[12] * mat.m[3] * mat.m[9];

	result.m[13] = mat.m[0] * mat.m[9] * mat.m[14] - mat.m[0] * mat.m[10] * mat.m[13] -
		mat.m[8] * mat.m[1] * mat.m[14] + mat.m[8] * mat.m[2] * mat.m[13] +
		mat.m[12] * mat.m[1] * mat.m[10] - mat.m[12] * mat.m[2] * mat.m[9];

	result.m[2] = mat.m[1] * mat.m[6] * mat.m[15] - mat.m[1] * mat.m[7] * mat.m[14] - mat.m[5] * mat.m[2] * mat.m[15] +
		mat.m[5] * mat.m[3] * mat.m[14] + mat.m[13] * mat.m[2] * mat.m[7] - mat.m[13] * mat.m[3] * mat.m[6];

	result.m[6] = -mat.m[0] * mat.m[6] * mat.m[15] + mat.m[0] * mat.m[7] * mat.m[14] + mat.m[4] * mat.m[2] * mat.m[15] -
		mat.m[4] * mat.m[3] * mat.m[14] - mat.m[12] * mat.m[2] * mat.m[7] + mat.m[12] * mat.m[3] * mat.m[6];

	result.m[10] = mat.m[0] * mat.m[5] * mat.m[15] - mat.m[0] * mat.m[7] * mat.m[13] - mat.m[4] * mat.m[1] * mat.m[15] +
		mat.m[4] * mat.m[3] * mat.m[13] + mat.m[12] * mat.m[1] * mat.m[7] - mat.m[12] * mat.m[3] * mat.m[5];

	result.m[14] = -mat.m[0] * mat.m[5] * mat.m[14] + mat.m[0] * mat.m[6] * mat.m[13] +
		mat.m[4] * mat.m[1] * mat.m[14] - mat.m[4] * mat.m[2] * mat.m[13] - mat.m[12] * mat.m[1] * mat.m[6] +
		mat.m[12] * mat.m[2] * mat.m[5];

	result.m[3] = -mat.m[1] * mat.m[6] * mat.m[11] + mat.m[1] * mat.m[7] * mat.m[10] + mat.m[5] * mat.m[2] * mat.m[11] -
		mat.m[5] * mat.m[3] * mat.m[10] - mat.m[9] * mat.m[2] * mat.m[7] + mat.m[9] * mat.m[3] * mat.m[6];

	result.m[7] = mat.m[0] * mat.m[6] * mat.m[11] - mat.m[0] * mat.m[7] * mat.m[10] - mat.m[4] * mat.m[2] * mat.m[11] +
		mat.m[4] * mat.m[3] * mat.m[10] + mat.m[8] * mat.m[2] * mat.m[7] - mat.m[8] * mat.m[3] * mat.m[6];

	result.m[11] = -mat.m[0] * mat.m[5] * mat.m[11] + mat.m[0] * mat.m[7] * mat.m[9] + mat.m[4] * mat.m[1] * mat.m[11] -
		mat.m[4] * mat.m[3] * mat.m[9] - mat.m[8] * mat.m[1] * mat.m[7] + mat.m[8] * mat.m[3] * mat.m[5];

	result.m[15] = mat.m[0] * mat.m[5] * mat.m[10] - mat.m[0] * mat.m[6] * mat.m[9] - mat.m[4] * mat.m[1] * mat.m[10] +
		mat.m[4] * mat.m[2] * mat.m[9] + mat.m[8] * mat.m[1] * mat.m[6] - mat.m[8] * mat.m[2] * mat.m[5];

	float det = mat.m[0] * result.m[0] + mat.m[1] * result.m[4] + mat.m[2] * result.m[8] + mat.m[3] * result.m[12];
	det = 1.0f / det;
	for (int i = 0; i < 4; i++)
		result.rows[i] = result.rows[i] * det;
	return result;
}

Mat4 Transpose(const Mat4 &m) {
	Mat4 res;
	res.rows[0] = Vec4(m.m2[0][0], m.m2[1][0], m.m2[2][0], m.m2[3][0]);
	res.rows[1] = Vec4(m.m2[0][1], m.m2[1][1], m.m2[2][1], m.m2[3][1]);
	res.rows[2] = Vec4(m.m2[0][2], m.m2[1][2], m.m2[2][2], m.m2[3][2]);
	res.rows[3] = Vec4(m.m2[0][3], m.m2[1][3], m.m2[2][3], m.m2[3][3]);
	return res;
}

//
//
//

Mat2 operator*(const Mat2 &left, const Mat2 &right) {
	Mat2 res;
	Mat2 tras = Transpose(right);

	res.m2[0][0] = DotProduct(left.rows[0], tras.rows[0]);
	res.m2[0][1] = DotProduct(left.rows[0], tras.rows[1]);
	res.m2[1][0] = DotProduct(left.rows[1], tras.rows[0]);
	res.m2[1][1] = DotProduct(left.rows[1], tras.rows[1]);

	return res;
}

Vec2 operator*(const Mat2 &mat, Vec2 vec) {
	Vec2 res;
	res[0] = DotProduct(vec, mat.rows[0]);
	res[1] = DotProduct(vec, mat.rows[1]);
	return res;
}

Vec2 operator*(Vec2 vec, const Mat2 &mat) {
	Vec2 res;
	res[0] = DotProduct(vec, Vec2(mat.m2[0][0], mat.m2[1][0]));
	res[1] = DotProduct(vec, Vec2(mat.m2[0][1], mat.m2[1][1]));
	return res;
}

Mat3 operator*(const Mat3 &left, const Mat3 &right) {
	Mat3 res;
	Mat3 tras = Transpose(right);

	res.m2[0][0] = DotProduct(left.rows[0], tras.rows[0]);
	res.m2[0][1] = DotProduct(left.rows[0], tras.rows[1]);
	res.m2[0][2] = DotProduct(left.rows[0], tras.rows[2]);

	res.m2[1][0] = DotProduct(left.rows[1], tras.rows[0]);
	res.m2[1][1] = DotProduct(left.rows[1], tras.rows[1]);
	res.m2[1][2] = DotProduct(left.rows[1], tras.rows[2]);

	res.m2[2][0] = DotProduct(left.rows[2], tras.rows[0]);
	res.m2[2][1] = DotProduct(left.rows[2], tras.rows[1]);
	res.m2[2][2] = DotProduct(left.rows[2], tras.rows[2]);

	return res;
}

Vec3 operator*(const Mat3 &mat, Vec3 vec) {
	Vec3 res;
	res[0] = DotProduct(vec, mat.rows[0]);
	res[1] = DotProduct(vec, mat.rows[1]);
	res[2] = DotProduct(vec, mat.rows[2]);
	return res;
}

Mat4 operator*(const Mat4 &left, const Mat4 &right) {
	Mat4 res;
	Mat4 tras = Transpose(right);

	res.m2[0][0] = DotProduct(left.rows[0], tras.rows[0]);
	res.m2[0][1] = DotProduct(left.rows[0], tras.rows[1]);
	res.m2[0][2] = DotProduct(left.rows[0], tras.rows[2]);
	res.m2[0][3] = DotProduct(left.rows[0], tras.rows[3]);

	res.m2[1][0] = DotProduct(left.rows[1], tras.rows[0]);
	res.m2[1][1] = DotProduct(left.rows[1], tras.rows[1]);
	res.m2[1][2] = DotProduct(left.rows[1], tras.rows[2]);
	res.m2[1][3] = DotProduct(left.rows[1], tras.rows[3]);

	res.m2[2][0] = DotProduct(left.rows[2], tras.rows[0]);
	res.m2[2][1] = DotProduct(left.rows[2], tras.rows[1]);
	res.m2[2][2] = DotProduct(left.rows[2], tras.rows[2]);
	res.m2[2][3] = DotProduct(left.rows[2], tras.rows[3]);

	res.m2[3][0] = DotProduct(left.rows[3], tras.rows[0]);
	res.m2[3][1] = DotProduct(left.rows[3], tras.rows[1]);
	res.m2[3][2] = DotProduct(left.rows[3], tras.rows[2]);
	res.m2[3][3] = DotProduct(left.rows[3], tras.rows[3]);

	return res;
}

Vec4 operator*(const Mat4 &mat, Vec4 vec) {
	Vec4 res;
	res[0] = DotProduct(vec, mat.rows[0]);
	res[1] = DotProduct(vec, mat.rows[1]);
	res[2] = DotProduct(vec, mat.rows[2]);
	res[3] = DotProduct(vec, mat.rows[3]);
	return res;
}

//
//
//

Vec3 GetRight(const Mat4 &m) {
	Vec3 v;
	v.x = m.m2[0][0];
	v.y = m.m2[1][0];
	v.z = m.m2[2][0];
	return v;
}

Vec3 GetUp(const Mat4 &m) {
	Vec3 v;
	v.x = m.m2[0][2];
	v.y = m.m2[1][2];
	v.z = m.m2[2][2];
	return v;
}

Vec3 GetForward(const Mat4 &m) {
	Vec3 v;
	v.x = m.m2[0][1];
	v.y = m.m2[1][1];
	v.z = m.m2[2][1];
	return v;
}

Mat2 ToMat2(const Mat3 &mat) {
	Mat2 result;
	result.rows[0] = Vec2(mat.rows[0].x, mat.rows[0].y);
	result.rows[1] = Vec2(mat.rows[1].x, mat.rows[1].y);
	return result;
}

Mat3 ToMat3(const Mat2 &mat) {
	Mat3 result;
	result.rows[0] = Vec3(mat.rows[0], 0);
	result.rows[1] = Vec3(mat.rows[1], 0);
	result.rows[2] = Vec3(0, 0, 1);
	return result;
}

Mat3 ToMat3(const Mat4 &mat) {
	Mat3 result;
	result.rows[0] = Vec3(mat.rows[0].x, mat.rows[0].y, mat.rows[0].z);
	result.rows[1] = Vec3(mat.rows[1].x, mat.rows[1].y, mat.rows[1].z);
	result.rows[2] = Vec3(mat.rows[2].x, mat.rows[2].y, mat.rows[2].z);
	return result;
}

Mat4 ToMat4(const Mat3 &mat) {
	Mat4 result;
	result.rows[0] = Vec4(mat.rows[0], 0);
	result.rows[1] = Vec4(mat.rows[1], 0);
	result.rows[2] = Vec4(mat.rows[2], 0);
	result.rows[3] = Vec4(0, 0, 0, 1);
	return result;
}

//
//
//

Quat IdentityQuat() {
	return Quat(0, 0, 0, 1);
}

float DotProduct(Quat q1, Quat q2) {
	return DotProduct(q1.v4, q2.v4);
}

float Length(Quat q) {
	return MathSquareRoot(DotProduct(q, q));
}

Quat Normalize(Quat q) {
	float len = Length(q);
	Assert(len != 0);
	return q * (1.0f / len);
}

Quat Conjugate(Quat q) {
	return Quat(-q.m[0], -q.m[1], -q.m[2], q.m[3]);
}

Quat operator*(Quat q1, Quat q2) {
	float a = q1.v4.w;
	float b = q1.v4.x;
	float c = q1.v4.y;
	float d = q1.v4.z;

	float e = q2.v4.w;
	float f = q2.v4.x;
	float g = q2.v4.y;
	float h = q2.v4.z;

	Quat  res;
	res.v4.w = a * e - b * f - c * g - d * h;
	res.v4.x = a * f + b * e + c * h - d * g;
	res.v4.y = a * g - b * h + c * e + d * f;
	res.v4.z = a * h + b * g - c * f + d * e;
	return res;
}

Vec3 Rotate(Quat q, Vec3 v) {
	Quat p = Quat(v.x, v.y, v.z, 0);
	Quat qc = Conjugate(q);
	Quat res = (q * p * qc);
	return Vec3(res.v4.x, res.v4.y, res.v4.z);
}

Quat QuatFromAngleAxis(Vec3 axis, float angle) {
	float r = MathCos(angle * 0.5f);
	float s = MathSin(angle * 0.5f);
	float i = s * axis.x;
	float j = s * axis.y;
	float k = s * axis.z;
	return Quat(i, j, k, r);
}

Quat QuatFromAngleAxisNormalized(Vec3 axis, float angle) {
	return QuatFromAngleAxis(Normalize(axis), angle);
}

void GetAngleAxis(Quat q, float *angle, Vec3 *axis) {
	float len = MathSquareRoot(q.m[0] * q.m[0] + q.m[1] * q.m[1] + q.m[2] * q.m[2]);
	if (len) {
		*angle = 2.0f * MathArcTan2(len, q.m[3]);
		len = 1.0f / len;
		axis->x = q.m[0] * len;
		axis->y = q.m[1] * len;
		axis->z = q.m[2] * len;
	}
	else {
		// degenerate case, unit quaternion
		*angle = 0;
		*axis = Vec3(0, 0, 1);
	}
}

Vec3 GetAxis(Quat q) {
	Vec3  axis;
	float angle;
	GetAngleAxis(q, &angle, &axis);
	return axis;
}

float GetAngle(Quat q) {
	Vec3  axis;
	float angle;
	GetAngleAxis(q, &angle, &axis);
	return angle;
}

Quat QuatFromMat4(const Mat4 &m) {
	Quat  q;
	float trace = m.m2[0][0] + m.m2[1][1] + m.m2[2][2];
	if (trace > 0.0f) {
		float s = 0.5f / MathSquareRoot(trace + 1.0f);
		q.v4.w = 0.25f / s;
		q.v4.x = (m.m2[2][1] - m.m2[1][2]) * s;
		q.v4.y = (m.m2[0][2] - m.m2[2][0]) * s;
		q.v4.z = (m.m2[1][0] - m.m2[0][1]) * s;
	}
	else {
		if (m.m2[0][0] > m.m2[1][1] && m.m2[0][0] > m.m2[2][2]) {
			float s = 2.0f * MathSquareRoot(1.0f + m.m2[0][0] - m.m2[1][1] - m.m2[2][2]);
			q.v4.w = (m.m2[2][1] - m.m2[1][2]) / s;
			q.v4.x = 0.25f * s;
			q.v4.y = (m.m2[0][1] + m.m2[1][0]) / s;
			q.v4.z = (m.m2[0][2] + m.m2[2][0]) / s;
		}
		else if (m.m2[1][1] > m.m2[2][2]) {
			float s = 2.0f * MathSquareRoot(1.0f + m.m2[1][1] - m.m2[0][0] - m.m2[2][2]);
			q.v4.w = (m.m2[0][2] - m.m2[2][0]) / s;
			q.v4.x = (m.m2[0][1] + m.m2[1][0]) / s;
			q.v4.y = 0.25f * s;
			q.v4.z = (m.m2[1][2] + m.m2[2][1]) / s;
		}
		else {
			float s = 2.0f * MathSquareRoot(1.0f + m.m2[2][2] - m.m2[0][0] - m.m2[1][1]);
			q.v4.w = (m.m2[1][0] - m.m2[0][1]) / s;
			q.v4.x = (m.m2[0][2] + m.m2[2][0]) / s;
			q.v4.y = (m.m2[1][2] + m.m2[2][1]) / s;
			q.v4.z = 0.25f * s;
		}
	}
	return Normalize(q);
}

Quat QuatFromMat4Nomalized(const Mat4 &m) {
	Mat4 nm;
	nm.rows[0] = Vec4(Normalize(VecXYZ(m.rows[0])), m.rows[0].w);
	nm.rows[1] = Vec4(Normalize(VecXYZ(m.rows[1])), m.rows[1].w);
	nm.rows[2] = Vec4(Normalize(VecXYZ(m.rows[2])), m.rows[2].w);
	nm.rows[3] = Vec4(Normalize(VecXYZ(m.rows[3])), m.rows[3].w);
	return QuatFromMat4(nm);
}

Mat4 GetMat4(Quat q) {
	float i = q.m[0];
	float j = q.m[1];
	float k = q.m[2];
	float r = q.m[3];

	float ii = i * i;
	float jj = j * j;
	float kk = k * k;

	float ij = i * j;
	float jk = j * k;
	float kr = k * r;
	float jr = j * r;
	float ir = i * r;
	float ik = i * k;

	Mat4  m;

	m.m2[0][0] = 1 - 2 * (jj + kk);
	m.m2[0][1] = 2 * (ij - kr);
	m.m2[0][2] = 2 * (ik + jr);
	m.m2[0][3] = 0;

	m.m2[1][0] = 2 * (ij + kr);
	m.m2[1][1] = 1 - 2 * (ii + kk);
	m.m2[1][2] = 2 * (jk - ir);
	m.m2[1][3] = 0;

	m.m2[2][0] = 2 * (ik - jr);
	m.m2[2][1] = 2 * (jk + ir);
	m.m2[2][2] = 1 - 2 * (ii + jj);
	m.m2[2][3] = 0;

	m.m2[3][0] = 0;
	m.m2[3][1] = 0;
	m.m2[3][2] = 0;
	m.m2[3][3] = 1;

	return m;
}

Vec3 GetForward(Quat q) {
	Vec3 up;
	up.x = 2 * (q.m[0] * q.m[2] + q.m[1] * q.m[3]);
	up.y = 2 * (q.m[1] * q.m[2] - q.m[0] * q.m[3]);
	up.z = 1 - 2 * (q.m[0] * q.m[0] + q.m[1] * q.m[1]);
	return Normalize(up);
}

Vec3 GetRight(Quat q) {
	Vec3 right;
	right.x = 1 - 2 * (q.m[1] * q.m[1] + q.m[2] * q.m[2]);
	right.y = 2 * (q.m[0] * q.m[1] + q.m[2] * q.m[3]);
	right.z = 2 * (q.m[0] * q.m[2] - q.m[1] * q.m[3]);
	return Normalize(right);
}

Vec3 GetUp(Quat q) {
	Vec3 forward;
	forward.x = 2 * (q.m[0] * q.m[1] - q.m[2] * q.m[3]);
	forward.y = 1 - 2 * (q.m[0] * q.m[0] + q.m[2] * q.m[2]);
	forward.z = 2 * (q.m[1] * q.m[2] + q.m[0] * q.m[3]);
	return Normalize(forward);
}

Quat QuatFromEulerAngles(float pitch, float yaw, float roll) {
	float cy = MathCos(roll * 0.5f);
	float sy = MathSin(roll * 0.5f);
	float cp = MathCos(yaw * 0.5f);
	float sp = MathSin(yaw * 0.5f);
	float cr = MathCos(pitch * 0.5f);
	float sr = MathSin(pitch * 0.5f);

	Quat  q;
	q.v4.w = cy * cp * cr + sy * sp * sr;
	q.v4.x = cy * cp * sr - sy * sp * cr;
	q.v4.y = sy * cp * sr + cy * sp * cr;
	q.v4.z = sy * cp * cr - cy * sp * sr;
	return q;
}

Quat QuatFromEulerAngles(Vec3 euler) {
	return QuatFromEulerAngles(euler.x, euler.y, euler.z);
}

Vec3 GetEulerAngles(Quat q) {
	Vec3  angles;

	float sinr_cosp = 2.0f * (q.v4.w * q.v4.x + q.v4.y * q.v4.z);
	float cosr_cosp = 1.0f - 2.0f * (q.v4.x * q.v4.x + q.v4.y * q.v4.y);
	angles.z = MathArcTan2(sinr_cosp, cosr_cosp);

	float sinp = 2.0f * (q.v4.w * q.v4.y - q.v4.z * q.v4.x);
	if (MathAbsolute(sinp) >= 1.0f) {
		// use 90 degrees if out of range
		angles.x = MathCopySign(PI / 2, sinp);
	}
	else {
		angles.x = MathArcSin(sinp);
	}

	float siny_cosp = 2.0f * (q.v4.w * q.v4.z + q.v4.x * q.v4.y);
	float cosy_cosp = 1.0f - 2.0f * (q.v4.y * q.v4.y + q.v4.z * q.v4.z);
	angles.y = MathArcTan2(siny_cosp, cosy_cosp);

	return angles;
}

Quat QuatBetween(Vec3 from, Vec3 to) {
	Quat q;
	float w = 1.0f + DotProduct(from, to);

	if (q.m[3]) {
		q.v4 = Vec4(CrossProduct(from, to), w);
	}
	else {
		Vec3 xyz = MathAbsolute(from.x) > MathAbsolute(from.z) ? Vec3(-from.y, from.x, 0.f) : Vec3(0.f, -from.z, from.y);
		q.v4 = Vec4(xyz, w);
	}

	return Normalize(q);
}

Quat QuatBetween(Quat a, Quat b) {
	Quat t = Conjugate(a);
	t = t * (1.0f / DotProduct(t, t));
	return t * b;
}

Quat QuatLookAt(Vec3 from, Vec3 to, Vec3 world_forward) {
	Vec3 dir = to - from;
	return QuatBetween(world_forward, dir);
}

//
//
//

// https://en.wikipedia.org/wiki/SRGB#Specification_of_the_transformation
Vec3 LinearToSrgb(Vec3 color) {
	const Mat3 transform{ Vec3(0.4142f, 0.3576f, 0.1805f), Vec3(0.2126f, 0.7152f, 0.0722f),
						 Vec3(0.0193f, 0.1192f, 0.9505f) };

	Vec3       res = transform * color;
	return res;
}

Vec4 LinearToSrgb(Vec4 color) {
	Vec4 res = Vec4(LinearToSrgb(VecXYZ(color)), color.w);
	return res;
}

Vec3 LinearToSrgb(Vec3 color, float gamma) {
	float igamma = 1.0f / gamma;
	Vec3  res;
	res.x = MathPow(color.x, igamma);
	res.y = MathPow(color.y, igamma);
	res.z = MathPow(color.z, igamma);
	return res;
}

Vec4 LinearToSrgb(Vec4 color, float gamma) {
	Vec4 res = Vec4(LinearToSrgb(VecXYZ(color), gamma), color.w);
	return res;
}

Vec3 SrgbToLinear(Vec3 color) {
	const Mat3 transform{ Vec3(+3.2406f, -1.5372f, -0.4986f), Vec3(-0.9689f, +1.8758f, +0.0415f),
						 Vec3(+0.0557f, -0.2040f, +1.0570f) };
	Vec3       res = transform * color;
	return res;
}

Vec4 SrgbToLinear(Vec4 color) {
	Vec4 res = Vec4(SrgbToLinear(VecXYZ(color)), color.w);
	return res;
}

Vec3 SrgbToLinear(Vec3 color, float gamma) {
	Vec3 res;
	res.x = MathPow(color.x, gamma);
	res.y = MathPow(color.y, gamma);
	res.z = MathPow(color.z, gamma);
	return res;
}

Vec4 SrgbToLinear(Vec4 color, float gamma) {
	Vec4 res = Vec4(SrgbToLinear(VecXYZ(color), gamma), color.w);
	return res;
}

// http://en.wikipedia.org/wiki/HSL_and_HSV
Vec3 HsvToRgb(Vec3 col) {
	Vec3  res;

	float h = col.x;
	float s = col.y;
	float v = col.z;

	if (s == 0.0f) {
		// gray
		res.x = res.y = res.z = v;
		return res;
	}

	h = MathMod(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i) {
	case 0:
		res = Vec3(v, t, p);
		break;
	case 1:
		res = Vec3(q, v, p);
		break;
	case 2:
		res = Vec3(p, v, t);
		break;
	case 3:
		res = Vec3(p, q, v);
		break;
	case 4:
		res = Vec3(t, p, v);
		break;
	case 5:
	default:
		res = Vec3(v, p, q);
		break;
	}

	return res;
}

// http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
Vec3 RgbToHsv(Vec3 c) {
	float r = c.x;
	float g = c.y;
	float b = c.z;

	float k = 0.f;
	if (g < b) {
		auto t = b;
		b = g;
		g = t;
		k = -1.f;
	}
	if (r < g) {
		auto t = g;
		g = r;
		r = t;
		k = -2.f / 6.f - k;
	}

	Vec3  res;
	float chroma = r - (g < b ? g : b);
	res.x = MathAbsolute(k + (g - b) / (6.f * chroma + 1e-20f));
	res.y = chroma / (r + 1e-20f);
	res.z = r;
	return res;
}

Vec4 HsvToRgb(Vec4 c) {
	return Vec4(HsvToRgb(VecXYZ(c)), c.w);
}

Vec4 RgbToHsv(Vec4 c) {
	return Vec4(RgbToHsv(VecXYZ(c)), c.w);
}

//
//
//

Mat2 Lerp(const Mat2 &from, const Mat2 &to, float t) {
	Mat2 res;
	res.rows[0] = Lerp(from.rows[0], to.rows[0], t);
	res.rows[1] = Lerp(from.rows[1], to.rows[1], t);
	return res;
}

Mat3 Lerp(const Mat3 &from, const Mat3 &to, float t) {
	Mat3 res;
	res.rows[0] = Lerp(from.rows[0], to.rows[0], t);
	res.rows[1] = Lerp(from.rows[1], to.rows[1], t);
	res.rows[2] = Lerp(from.rows[2], to.rows[2], t);
	return res;
}

Mat4 Lerp(const Mat4 &from, const Mat4 &to, float t) {
	Mat4 res;
	res.rows[0] = Lerp(from.rows[0], to.rows[0], t);
	res.rows[1] = Lerp(from.rows[1], to.rows[1], t);
	res.rows[2] = Lerp(from.rows[2], to.rows[2], t);
	res.rows[3] = Lerp(from.rows[3], to.rows[3], t);
	return res;
}

Vec2 Slerp(Vec2 from, Vec2 to, float t) {
	return Slerp(from, to, AngleBetween(from, to), t);
}

Vec3 Slerp(Vec3 from, Vec3 to, float t) {
	return Slerp(from, to, AngleBetween(from, to), t);
}

Quat Slerp(Quat from, Quat to, float t) {
	float dot = Clamp(-1.0f, 1.0f, DotProduct(from, to));

	// use shorter path
	if (dot < 0.0f) {
		to = -to;
		dot = -dot;
	}

	if (dot > 0.9999f) {
		Quat result = Lerp(from, to, t);
		return Normalize(result);
	}

	float theta_0 = MathArcCos(dot);
	float theta = theta_0 * t;
	float sin_theta = MathSin(theta);
	float sin_theta_0 = MathSin(theta_0);

	float s0 = MathCos(theta) - dot * sin_theta / sin_theta_0;
	float s1 = sin_theta / sin_theta_0;

	return (s0 * from) + (s1 * to);
}

float Step(float edge, float val) {
	return val < edge ? 0.0f : 1.0f;
}

Vec2 Step(Vec2 edge, Vec2 val) {
	Vec2 res;
	res.x = Step(edge.x, val.x);
	res.y = Step(edge.y, val.y);
	return res;
}

Vec3 Step(Vec3 edge, Vec3 val) {
	Vec3 res;
	res.x = Step(edge.x, val.x);
	res.y = Step(edge.y, val.y);
	res.z = Step(edge.z, val.z);
	return res;
}

Vec4 Step(Vec4 edge, Vec4 val) {
	Vec4 res;
	res.x = Step(edge.x, val.x);
	res.y = Step(edge.y, val.y);
	res.z = Step(edge.z, val.z);
	res.w = Step(edge.w, val.w);
	return res;
}

Quat Step(Quat edge, Quat val) {
	Quat res;
	res.v4 = Step(edge.v4, val.v4);
	return res;
}

float InverseSmoothStep(float x) {
	return 0.5f - MathSin(MathArcSin(1.0f - 2.0f * x) / 3.0f);
}

Vec2 MoveTowards(Vec2 from, Vec2 to, float factor) {
	if (factor) {
		Vec2  direction = to - from;
		float distance = Length(direction);

		if (distance < factor) {
			return to;
		}

		float t = factor / distance;

		return Lerp(to, from, t);
	}

	return from;
}

Vec3 MoveTowards(Vec3 from, Vec3 to, float factor) {
	if (factor) {
		Vec3  direction = to - from;
		float distance = Length(direction);

		if (distance < factor) {
			return to;
		}

		float t = factor / distance;

		return Lerp(from, to, t);
	}

	return from;
}

Vec4 MoveTowards(Vec4 from, Vec4 to, float factor) {
	if (factor) {
		Vec4  direction = to - from;
		float distance = Length(direction);

		if (distance < factor) {
			return to;
		}

		float t = factor / distance;

		return Lerp(to, from, t);
	}

	return from;
}

//
//
//

Vec2 RotateAround(Vec2 point, Vec2 center, float angle) {
	float c = MathCos(angle);
	float s = MathSin(angle);
	Vec2  res;
	Vec2  p = point - center;
	res.x = p.x * c - p.y * s;
	res.y = p.x * s + p.y * c;
	res += center;
	return res;
}

Quat RotateTowards(Quat from, Quat to, float max_angle) {
	if (max_angle) {
		float dot = Clamp(-1.0f, 1.0f, DotProduct(from, to));

		// use shorter path
		if (dot < 0.0f) {
			to = -to;
			dot = -dot;
		}

		float theta_0 = MathArcCos(dot);

		if (theta_0 < max_angle) {
			return to;
		}

		float t = max_angle / theta_0;

		theta_0 = max_angle;
		float theta = theta_0 * t;
		float sin_theta = MathSin(theta);
		float sin_theta_0 = MathSin(theta_0);

		float s0 = MathCos(theta) - dot * sin_theta / sin_theta_0;
		float s1 = sin_theta / sin_theta_0;

		return (s0 * from) + (s1 * to);
	}
	else {
		return from;
	}
}

Vec2 Reflect(Vec2 d, Vec2 n) {
	float c = DotProduct(NormalizeChecked(d), n);
	float s = MathSquareRoot(10.f - MathSquare(c));
	Vec2  r;
	r.x = d.x * c - d.y * s;
	r.y = d.x * s + d.y * c;
	return r;
}

UintColor Vec4ToUintColor(Vec4 v) {
	uint8_t r = static_cast<uint8_t>(255.0f * v.x);
	uint8_t g = static_cast<uint8_t>(255.0f * v.y);
	uint8_t b = static_cast<uint8_t>(255.0f * v.z);
	uint8_t a = static_cast<uint8_t>(255.0f * v.w);
	return UintColor(r, g, b, a);
}

UintColor Vec3ToUintColor(Vec3 v) {
	uint8_t r = static_cast<uint8_t>(255.0f * v.x);
	uint8_t g = static_cast<uint8_t>(255.0f * v.y);
	uint8_t b = static_cast<uint8_t>(255.0f * v.z);
	uint8_t a = 255;
	return UintColor(r, g, b, a);
}

Vec4 UintColorToVec4(UintColor c) {
	float div = 1.0f / 255.0f;
	float r = static_cast<float>(c.r) * div;
	float g = static_cast<float>(c.g) * div;
	float b = static_cast<float>(c.b) * div;
	float a = static_cast<float>(c.a) * div;
	return Vec4(r, g, b, a);
}

Vec3 UintColorToVec3(UintColor c) {
	float div = 1.0f / 255.0f;
	float r = static_cast<float>(c.r) * div;
	float g = static_cast<float>(c.g) * div;
	float b = static_cast<float>(c.b) * div;
	return Vec3(r, g, b);
}
