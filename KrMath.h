#pragma once

#ifndef __has_include
#define __has_include(x) false
#endif

#if __has_include("KrPlatform.h")
#include "KrPlatform.h"
#else
#include <stdint.h>
#include <stddef.h>

#define Minimum(a, b) (((a) < (b)) ? (a) : (b))
#define Maximum(a, b) (((a) > (b)) ? (a) : (b))
#define Clamp(a, b, v) Minimum(b, Maximum(a, v))

#if defined(_MSC_VER)
#define TriggerBreakpoint() __debugbreak()
#elif ((!defined(__NACL__)) && ((defined(__GNUC__) || defined(__clang__)) && (defined(__i386__) || defined(__x86_64__))))
#define TriggerBreakpoint() __asm__ __volatile__("int $3\n\t")
#elif defined(__386__) && defined(__WATCOMC__)
#define TriggerBreakpoint() _asm { int 0x03 }
#elif defined(HAVE_SIGNAL_H) && !defined(__WATCOMC__)
#define TriggerBreakpoint() raise(SIGTRAP)
#else
#define TriggerBreakpoint() ((int *)0) = 0
#endif

#if defined(__GNUC__)
#define inproc inline static
#else
#define inproc inline
#endif

#endif

#include <float.h>
#include <math.h>

constexpr float PI = 3.1415926535f;
constexpr float PI_INVERSE = 1.0f / PI;
constexpr float TAU = PI / 2;
constexpr float REAL_EPSILON = FLT_EPSILON;
constexpr float REAL_MAX = FLT_MAX;
constexpr float REAL_MIN = FLT_MIN;

#ifndef Assert
#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#define MathAssert(x)           \
        if (!(x))               \
            TriggerBreakpoint();
#else
#define MathAssert(x) 
#endif
#else
#define MathAssert Assert
#endif // !Assert

struct Vec2 {
	union {
		struct { float x, y; };
		float m[2];
	};

	Vec2() {}
	Vec2(float a) : x(a), y(a) {}
	Vec2(float a, float b) : x(a), y(b) {}
};

struct Vec3 {
	union {
		struct { float x, y, z; };
		float m[3];
	};
	Vec3() {}
	Vec3(float a) : x(a), y(a), z(a) {}
	Vec3(float a, float b, float c) : x(a), y(b), z(c) {}
	Vec3(Vec2 ab, float c) : x(ab.x), y(ab.y), z(c) {}
	Vec3(float a, Vec2 cd) : x(a), y(cd.x), z(cd.y) {}
};

struct Vec4 {
	union {
		struct { float x, y, z, w; };
		float m[4];
	};
	Vec4() {}
	Vec4(float a) : x(a), y(a), z(a), w(a) {}
	Vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
	Vec4(Vec2 ab, Vec2 cd): x(ab.x), y(ab.y), z(cd.x), w(cd.y) {}
	Vec4(Vec3 abc, float d): x(abc.x), y(abc.y), z(abc.z), w(d) {}
	Vec4(float a, Vec3 bcd): x(a), y(bcd.x), z(bcd.y), w(bcd.z) {}
};

#define v_xy(v)  Vec2((v).x, (v).y)
#define v_yz(v)  Vec2((v).y, (v).z)
#define v_xyz(v) Vec3((v).x, (v).y, (v).z)
#define v_yzw(v) Vec3((v).y, (v).z, (v).w)

union Mat2 {
	Vec2  rows[2];
	float m[4];
	float m2[2][2];
	inline Mat2() {}
	inline Mat2(Vec2 a, Vec2 b) : rows{ a, b } {}

	inline Mat2(float a) {
		rows[0] = Vec2(a, 0);
		rows[1] = Vec2(0, a);
	}

	static inline Mat2 Identity() { return Mat2(1); }
	static Mat2 Scalar(float x, float y);
	static Mat2 Scalar(Vec2 s);
	static Mat2 Rotation(float angle);
};

union Mat3 {
	Vec3  rows[3];
	float m[9];
	float m2[3][3];
	inline Mat3() {}
	inline Mat3(Vec3 a, Vec3 b, Vec3 c) : rows{ a, b, c } {}

	inline Mat3(float a) {
		rows[0] = Vec3(a, 0, 0);
		rows[1] = Vec3(0, a, 0);
		rows[2] = Vec3(0, 0, a);
	}

	static inline Mat3 Identity() { return Mat3(1); }
	static Mat3 Scalar(float S_1, float S_2);
	static Mat3 Scalar(Vec2 s);
	static Mat3 Translation(float T_x, float T_y);
	static Mat3 Translation(Vec2 t);
	static Mat3 Rotation(float angle);
	static Mat3 LookAt(Vec2 from, Vec2 to, Vec2 forward);
};

union Mat4 {
	Vec4  rows[4];
	float m[16];
	float m2[4][4];
	inline Mat4() {}
	inline Mat4(Vec4 a, Vec4 b, Vec4 c, Vec4 d) : rows{ a, b, c, d } {}

	inline Mat4(float a) {
		rows[0] = Vec4(a, 0, 0, 0);
		rows[1] = Vec4(0, a, 0, 0);
		rows[2] = Vec4(0, 0, a, 0);
		rows[3] = Vec4(0, 0, 0, a);
	}

	static inline Mat4 Identity() { return Mat4(1); }
	static Mat4 LookAtDirection(Vec3 dir, Vec3 world_up);
	static Mat4 Scalar(float S_1, float S_2, float S_3);
	static Mat4 Scalar(Vec3 s);
	static Mat4 Translation(float T_x, float T_y, float T_z);
	static Mat4 Translation(Vec3 t);
	static Mat4 RotationX(float angle);
	static Mat4 RotationY(float angle);
	static Mat4 RotationZ(float angle);
	static Mat4 Rotation(float x, float y, float z, float angle);
	static Mat4 Rotation(Vec3 axis, float angle);
	static Mat4 LookAt(Vec3 from, Vec3 to, Vec3 world_up);
	static Mat4 OrthographicProjectionRH(float l, float r, float t, float b, float n, float f);
	static Mat4 OrthographicProjectionLH(float l, float r, float t, float b, float n, float f);
	static Mat4 PerspectiveProjectionRH(float fov, float aspect_ratio, float n, float f);
	static Mat4 PerspectiveProjectionLH(float fov, float aspect_ratio, float n, float f);
};

union Quat {
	Vec4  v4;
	float m[4];
	Quat() {}
	Quat(Vec4 v) {
		m[0] = v.x;
		m[1] = v.y;
		m[2] = v.z;
		m[3] = v.w;
	}
	Quat(float b, float c, float d, float a) {
		m[0] = b;
		m[1] = c;
		m[2] = d;
		m[3] = a;
	}

	static Quat Identity();
	static Quat FromAngleAxis(Vec3 axis, float angle);
	static Quat FromAngleAxisNormalized(Vec3 axis, float angle);
	static Quat FromMat4(const Mat4 &m);
	static Quat FromMat4Nomalized(const Mat4 &m);
	static Quat FromEulerAngles(float pitch, float yaw, float roll);
	static Quat FromEulerAngles(Vec3 euler);
	static Quat Between(Vec3 from, Vec3 to);
	static Quat Between(Quat a, Quat b);
	static Quat LookAt(Vec3 from, Vec3 to, Vec3 world_forward);
};

#define FmtVec2 "[%f, %f]"
#define FmtVec3 "[%f, %f, %f]"
#define FmtVec4 "[%f, %f, %f, %f]"

#define ExpandVec2(v) (v).x, (v).y
#define ExpandVec3(v) (v).x, (v).y, (v).z
#define ExpandVec4(v) (v).x, (v).y, (v).z, (v).w

#define DegreesToRadians(deg) ((deg) * (PI / 180))
#define RadiansToDegrees(rad) ((rad) * (180 / PI))

#define MathAbsolute(x) fabsf(x)
#define MathSin(x) sinf(x)
#define MathCos(x) cosf(x)
#define MathTan(x) tanf(x)
#define MathArcSin(x) asinf(x)
#define MathArcCos(x) acosf(x)
#define MathArcTan2(y, x) atan2f(y, x)
#define MathSquareRoot(x) sqrtf(x)
#define MathPow(x, y) powf(x, y)
#define MathCopySign(x, y) copysignf(x, y)
#define MathMod(x, y) fmodf(x, y)
#define MathSquare(x) ((x) * (x))
#define MathFloor(x) floorf(x)

inproc float WrapFloat(float min, float a, float max) {
	float range = max - min;
	float offset = a - min;
	float result = (offset - (MathFloor(offset / range) * range) + min);
	return result;
}

inproc Vec2 Arm(float angle) {
	return Vec2(MathCos(angle), MathSin(angle));
}
inproc Vec2 ArmInverse(float angle) {
	return Vec2(MathSin(angle), MathCos(angle));
}

inproc Vec2 MinimumVec(Vec2 a, Vec2 b) {
	return Vec2{ Minimum(a.x, b.x), Minimum(a.y, b.y) };
}
inproc Vec2 MaximumVec(Vec2 a, Vec2 b) {
	return Vec2{ Maximum(a.x, b.x), Maximum(a.y, b.y) };
}
inproc Vec3 MinimumVec(Vec3 a, Vec3 b) {
	return Vec3{ Minimum(a.x, b.x), Minimum(a.y, b.y), Minimum(a.z, b.z) };
}
inproc Vec3 MaximumVec(Vec3 a, Vec3 b) {
	return Vec3{ Maximum(a.x, b.x), Maximum(a.y, b.y), Maximum(a.z, b.z) };
}
inproc Vec4 MinimumVec(Vec4 a, Vec4 b) {
	return Vec4{ Minimum(a.x, b.x), Minimum(a.y, b.y), Minimum(a.z, b.z), Minimum(a.w, b.w) };
}
inproc Vec4 MaximumVec(Vec4 a, Vec4 b) {
	return Vec4{ Maximum(a.x, b.x), Maximum(a.y, b.y), Maximum(a.z, b.z), Maximum(a.w, b.w) };
}

inproc bool IsNull(Vec2 a) {
	return MathAbsolute(a.x) < REAL_EPSILON && MathAbsolute(a.y) < REAL_EPSILON;
}
inproc bool IsNull(Vec3 a) {
	return MathAbsolute(a.x) < REAL_EPSILON && MathAbsolute(a.y) < REAL_EPSILON && MathAbsolute(a.z) < REAL_EPSILON;
}
inproc bool IsNull(Vec4 a) {
	return MathAbsolute(a.x) < REAL_EPSILON && MathAbsolute(a.y) < REAL_EPSILON && MathAbsolute(a.z) < REAL_EPSILON &&
		MathAbsolute(a.w) < REAL_EPSILON;
}

inproc Vec2 operator+(Vec2 a, Vec2 b) {
	return Vec2(a.x + b.x, a.y + b.y);
}
inproc Vec3 operator+(Vec3 a, Vec3 b) {
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inproc Vec4 operator+(Vec4 a, Vec4 b) {
	return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
inproc Vec2 operator-(Vec2 a, Vec2 b) {
	return Vec2(a.x - b.x, a.y - b.y);
}
inproc Vec3 operator-(Vec3 a, Vec3 b) {
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inproc Vec4 operator-(Vec4 a, Vec4 b) {
	return Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
inproc Vec2 operator*(float s, Vec2 v) {
	return Vec2(s * v.x, s * v.y);
}
inproc Vec2 operator*(Vec2 v, float s) {
	return s * v;
}
inproc Vec3 operator*(float s, Vec3 v) {
	return Vec3(s * v.x, s * v.y, s * v.z);
}
inproc Vec3 operator*(Vec3 v, float s) {
	return s * v;
}
inproc Vec4 operator*(float s, Vec4 v) {
	return Vec4(s * v.x, s * v.y, s * v.z, s * v.w);
}
inproc Vec4 operator*(Vec4 v, float s) {
	return s * v;
}
inproc Vec2 operator/(Vec2 v, float s) {
	s = 1.0f / s;
	return Vec2(v.x * s, v.y * s);
}
inproc Vec3 operator/(Vec3 v, float s) {
	s = 1.0f / s;
	return Vec3(v.x * s, v.y * s, v.z * s);
}
inproc Vec4 operator/(Vec4 v, float s) {
	return v * 1.0f / s;
}
inproc Vec2 operator*(Vec2 l, Vec2 r) {
	return Vec2(l.x * r.x, l.y * r.y);
}
inproc Vec3 operator*(Vec3 l, Vec3 r) {
	return Vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}
inproc Vec4 operator*(Vec4 l, Vec4 r) {
	return Vec4(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w);
}
inproc Vec2 operator/(Vec2 l, Vec2 r) {
	return Vec2(l.x / r.x, l.y / r.y);
}
inproc Vec3 operator/(Vec3 l, Vec3 r) {
	return Vec3(l.x / r.x, l.y / r.y, l.z / r.z);
}
inproc Vec4 operator/(Vec4 l, Vec4 r) {
	return Vec4(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w);
}

inproc Vec2 operator-(const Vec2 &v) {
	return Vec2(-v.x, -v.y);
}
inproc Vec3 operator-(const Vec3 &v) {
	return Vec3(-v.x, -v.y, -v.z);
}
inproc Vec4 operator-(const Vec4 &v) {
	return Vec4(-v.x, -v.y, -v.z, -v.w);
}
inproc Vec2 &operator+=(Vec2 &a, Vec2 b) {
	a = a + b;
	return a;
}
inproc Vec3 &operator+=(Vec3 &a, Vec3 b) {
	a = a + b;
	return a;
}
inproc Vec4 &operator+=(Vec4 &a, Vec4 b) {
	a = a + b;
	return a;
}
inproc Vec2 &operator-=(Vec2 &a, Vec2 b) {
	a = a - b;
	return a;
}
inproc Vec3 &operator-=(Vec3 &a, Vec3 b) {
	a = a - b;
	return a;
}
inproc Vec4 &operator-=(Vec4 &a, Vec4 b) {
	a = a - b;
	return a;
}
inproc Vec2 &operator*=(Vec2 &t, float s) {
	t = t * s;
	return t;
}
inproc Vec3 &operator*=(Vec3 &t, float s) {
	t = t * s;
	return t;
}
inproc Vec4 &operator*=(Vec4 &t, float s) {
	t = t * s;
	return t;
}
inproc Vec2 &operator/=(Vec2 &t, float s) {
	t = t / s;
	return t;
}
inproc Vec3 &operator/=(Vec3 &t, float s) {
	t = t / s;
	return t;
}
inproc Vec4 &operator/=(Vec4 &t, float s) {
	t = t / s;
	return t;
}
inproc Vec2 &operator*=(Vec2 &t, Vec2 s) {
	t = t * s;
	return t;
}
inproc Vec3 &operator*=(Vec3 &t, Vec3 s) {
	t = t * s;
	return t;
}
inproc Vec4 &operator*=(Vec4 &t, Vec4 s) {
	t = t * s;
	return t;
}
inproc Vec2 &operator/=(Vec2 &t, Vec2 s) {
	t = t / s;
	return t;
}
inproc Vec3 &operator/=(Vec3 &t, Vec3 s) {
	t = t / s;
	return t;
}
inproc Vec4 &operator/=(Vec4 &t, Vec4 s) {
	t = t / s;
	return t;
}

inproc float DotProduct(Vec2 a, Vec2 b) {
	return a.x * b.x + a.y * b.y;
}
inproc float DotProduct(Vec3 a, Vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
inproc float DotProduct(Vec4 a, Vec4 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
inproc float Determinant(Vec2 a, Vec2 b) {
	return (a.x * b.y) - (a.y * b.x);
}
inproc Vec3 CrossProduct(Vec2 a, Vec2 b) {
	Vec3 res;
	res.x = 0;
	res.y = 0;
	res.z = (a.x * b.y) - (a.y * b.x);
	return res;
}
inproc Vec3 CrossProduct(Vec3 a, Vec3 b) {
	Vec3 res;
	res.x = (a.y * b.z) - (a.z * b.y);
	res.y = (a.z * b.x) - (a.x * b.z);
	res.z = (a.x * b.y) - (a.y * b.x);
	return res;
}
inproc Vec2 TripleProduct(Vec2 a, Vec2 b, Vec2 c) {
	float det = Determinant(a, b);
	Vec2  res;
	res.x = -c.y * det;
	res.y = c.x * det;
	return res;
}
inproc Vec3 TripleProduct(Vec3 a, Vec3 b, Vec3 c) {
	return CrossProduct(CrossProduct(a, b), c);
}

inproc float LengthSq(Vec2 v) {
	return DotProduct(v, v);
}
inproc float LengthSq(Vec3 v) {
	return DotProduct(v, v);
}
inproc float LengthSq(Vec4 v) {
	return DotProduct(v, v);
}
inproc float Length(Vec2 v) {
	return MathSquareRoot(DotProduct(v, v));
}
inproc float Length(Vec3 v) {
	return MathSquareRoot(DotProduct(v, v));
}
inproc float Length(Vec4 v) {
	return MathSquareRoot(DotProduct(v, v));
}
inproc float Distance(float a, float b) {
	return b - a;
}
inproc float Distance(Vec2 a, Vec2 b) {
	return Length(b - a);
}
inproc float Distance(Vec3 a, Vec3 b) {
	return Length(b - a);
}
inproc float Distance(Vec4 a, Vec4 b) {
	return Length(b - a);
}

inproc Vec2 NormalizeChecked(Vec2 v) {
	Vec2  res(0);
	float len = Length(v);
	if (len != 0)
		res = v / len;
	return res;
}
inproc Vec3 NormalizeChecked(Vec3 v) {
	Vec3  res(0);
	float len = Length(v);
	if (len != 0)
		res = v / len;
	return res;
}
inproc Vec4 NormalizeChecked(Vec4 v) {
	Vec4  res(0);
	float len = Length(v);
	if (len != 0)
		res = v * (1.0f / len);
	return res;
}
inproc Vec2 Normalize(Vec2 v) {
	Vec2  res(0);
	float len = Length(v);
	MathAssert(len != 0);
	res = v / len;
	return res;
}
inproc Vec3 Normalize(Vec3 v) {
	Vec3  res(0);
	float len = Length(v);
	MathAssert(len != 0);
	res = v / len;
	return res;
}
inproc Vec4 Normalize(Vec4 v) {
	Vec4  res(0);
	float len = Length(v);
	MathAssert(len != 0);
	res = v * (1.0f / len);
	return res;
}
inproc Vec2 PerpendicularVector(Vec2 a, Vec2 b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	return Normalize(Vec2(-dy, dx));
}

inproc float AngleBetween(Vec2 a, Vec2 b) {
	float dot = Clamp(-1.0f, 1.0f, DotProduct(a, b));
	return MathCos(dot);
}
inproc float AngleBetween(Vec3 a, Vec3 b) {
	float dot = Clamp(-1.0f, 1.0f, DotProduct(a, b));
	return MathCos(dot);
}
inproc float AngleBetweenNormalized(Vec2 a, Vec2 b) {
	a = Normalize(a);
	b = Normalize(b);
	return AngleBetween(a, b);
}
inproc float AngleBetweenNormalized(Vec3 a, Vec3 b) {
	a = Normalize(a);
	b = Normalize(b);
	return AngleBetween(a, b);
}
inproc float SignedAngleBetween(Vec2 a, Vec2 b) {
	float dot = Clamp(-1.0f, 1.0f, DotProduct(a, b));
	float angle = MathCos(dot);
	float cross = a.x * b.y - a.y * b.x;
	if (cross < 0) {
		angle = -angle;
	}
	return angle;
}
inproc float SignedAngleBetween(Vec3 a, Vec3 b, Vec3 n) {
	float dot = Clamp(-1.0f, 1.0f, DotProduct(a, b));
	float angle = MathCos(dot);
	Vec3  cross = CrossProduct(a, b);
	if (DotProduct(n, cross) < 0) {
		angle = -angle;
	}
	return angle;
}
inproc float SignedAngleBetweenNormalized(Vec2 a, Vec2 b) {
	a = Normalize(a);
	b = Normalize(b);
	return SignedAngleBetween(a, b);
}
inproc float SignedAngleBetweenNormalized(Vec3 a, Vec3 b, Vec3 n) {
	a = Normalize(a);
	b = Normalize(b);
	n = Normalize(n);
	return SignedAngleBetween(a, b, n);
}

inproc Vec2 ClampVec(Vec2 min, Vec2 max, Vec2 v) {
	v.x = Clamp(min.x, max.x, v.x);
	v.y = Clamp(min.y, max.y, v.y);
	return v;
}
inproc Vec3 ClampVec(Vec3 min, Vec3 max, Vec3 v) {
	v.x = Clamp(min.x, max.x, v.x);
	v.y = Clamp(min.y, max.y, v.y);
	v.z = Clamp(min.z, max.z, v.z);
	return v;
}
inproc Vec4 ClampVec(Vec4 min, Vec4 max, Vec4 v) {
	v.x = Clamp(min.x, max.x, v.x);
	v.y = Clamp(min.y, max.y, v.y);
	v.z = Clamp(min.z, max.z, v.z);
	v.w = Clamp(min.w, max.w, v.w);
	return v;
}

float        Determinant(const Mat2 &mat);
Mat2         Inverse(const Mat2 &mat);
Mat2         Transpose(const Mat2 &m);
float        Determinant(const Mat3 &mat);
Mat3         Inverse(const Mat3 &mat);
Mat3         Transpose(const Mat3 &m);
float        Determinant(const Mat4 &mat);
Mat4         Inverse(const Mat4 &mat);
Mat4         Transpose(const Mat4 &m);

Mat2         operator*(const Mat2 &left, const Mat2 &right);
Vec2         operator*(const Mat2 &mat, Vec2 vec);
Vec2         operator*(Vec2 vec, const Mat2 &mat);
Mat3         operator*(const Mat3 &left, const Mat3 &right);
Vec3         operator*(const Mat3 &mat, Vec3 vec);
Mat4         operator*(const Mat4 &left, const Mat4 &right);
Vec4         operator*(const Mat4 &mat, Vec4 vec);

inproc Mat2 &operator*=(Mat2 &t, Mat2 &o) {
	t = t * o;
	return t;
}
inproc Mat3 &operator*=(Mat3 &t, Mat3 &o) {
	t = t * o;
	return t;
}
inproc Mat4 &operator*=(Mat4 &t, Mat4 &o) {
	t = t * o;
	return t;
}

//
//
//

Vec3        GetRight(const Mat4 &m);
Vec3        GetUp(const Mat4 &m);
Vec3        GetForward(const Mat4 &m);
Mat2        ToMat2(const Mat3 &mat);
Mat3        ToMat3(const Mat2 &mat);
Mat3        ToMat3(const Mat4 &mat);
Mat4        ToMat4(const Mat3 &mat);

inproc Quat operator-(Quat &q) {
	return Quat(-q.v4);
}
inproc Quat operator-(Quat r1, Quat r2) {
	return Quat(r1.v4 - r2.v4);
}
inproc Quat operator+(Quat r1, Quat r2) {
	return Quat(r1.v4 + r2.v4);
}
inproc Quat operator*(Quat q, float s) {
	return Quat(q.v4 * s);
}
inproc Quat operator*(float s, Quat q) {
	return Quat(q.v4 * s);
}

float       DotProduct(Quat q1, Quat q2);
float       Length(Quat q);
Quat        Normalize(Quat q);
Quat        Conjugate(Quat q);
Quat        operator*(Quat q1, Quat q2);
Vec3        Rotate(Quat q, Vec3 v);
inproc Vec3 operator*(Quat q, Vec3 v) {
	return Rotate(q, v);
}

void  GetAngleAxis(Quat q, float *angle, Vec3 *axis);
Vec3  GetAxis(Quat q);
float GetAngle(Quat q);
Mat4  GetMat4(Quat q);
Vec3  GetForward(Quat q);
Vec3  GetRight(Quat q);
Vec3  GetUp(Quat q);
Vec3  GetEulerAngles(Quat q);

Vec3 LinearToSrgb(Vec3 color);
Vec4 LinearToSrgb(Vec4 color);
Vec3 LinearToSrgb(Vec3 color, float gamma);
Vec4 LinearToSrgb(Vec4 color, float gamma);
Vec3 SrgbToLinear(Vec3 color);
Vec4 SrgbToLinear(Vec4 color);
Vec3 SrgbToLinear(Vec3 color, float gamma);
Vec4 SrgbToLinear(Vec4 color, float gamma);

Vec3 HsvToRgb(Vec3 c);
Vec3 RgbToHsv(Vec3 c);
Vec4 HsvToRgb(Vec4 c);
Vec4 RgbToHsv(Vec4 c);

template <typename Type> Type Lerp(Type from, Type to, float t) {
	return (1 - t) * from + t * to;
}

template <typename Type> Type BezierQuadratic(Type a, Type b, Type c, float t) {
	float mt = 1 - t;
	float w1 = mt * mt;
	float w2 = 2 * mt * t;
	float w3 = t * t;
	return w1 * a + w2 * b + w3 * c;
}

template <typename Type> Type BezierCubic(Type a, Type b, Type c, Type d, float t) {
	float mt = 1.0f - t;
	float w1 = mt * mt * mt;
	float w2 = 3 * mt * mt * t;
	float w3 = 3 * mt * t * t;
	float w4 = t * t * t;
	return w1 * a + w2 * b + w3 * c + w4 * d;
}

template <typename Type> void BuildBezierQuadratic(Type a, Type b, Type c, Type *points, int segments) {
	for (int seg_index = 0; seg_index <= segments; ++seg_index) {
		float t = (float)seg_index / (float)segments;
		auto  np = BezierQuadratic(a, b, c, t);
		points[seg_index] = np;
	}
}

template <typename Type> void BuildBezierCubic(Type a, Type b, Type c, Type d, Type *points, int segments) {
	for (int seg_index = 0; seg_index <= segments; ++seg_index) {
		float t = (float)seg_index / (float)segments;
		auto  np = BezierCubic(a, b, c, d, t);
		points[seg_index] = np;
	}
}

template <typename Type> Type Slerp(Type from, Type to, float angle, float t) {
	float s = MathSin(angle);
	float ts = MathSin(angle * t);
	float mts = MathSin(angle * (1 - t));
	return (mts * from + ts * to) * (1.0f / s);
}

Mat2  Lerp(const Mat2 &from, const Mat2 &to, float t);
Mat3  Lerp(const Mat3 &from, const Mat3 &to, float t);
Mat4  Lerp(const Mat4 &from, const Mat4 &to, float t);
Vec2  Slerp(Vec2 from, Vec2 to, float t);
Vec3  Slerp(Vec3 from, Vec3 to, float t);
Quat  Slerp(Quat from, Quat to, float t);
float Step(float edge, float val);
Vec2  Step(Vec2 edge, Vec2 val);
Vec3  Step(Vec3 edge, Vec3 val);
Vec4  Step(Vec4 edge, Vec4 val);
Quat  Step(Quat edge, Quat val);

template <typename T> float SmoothStepZeroChecked(T a, T b, T v) {
	float div_distance = Distance(a, b);
	if (div_distance) {
		auto x = Clamp(0.0f, 1.0f, Distance(a, v) / div_distance);
		return x * x * (3 - 2 * x);
	}
	return 1;
}

template <typename T> float SmoothStep(T a, T b, T v) {
	auto x = Clamp(0.0f, 1.0f, Distance(a, v) / Distance(a, b));
	return x * x * (3 - 2 * x);
}

float InverseSmoothStep(float x);

template <typename T> T MapRange(T in_a, T in_b, T out_a, T out_b, T v) {
	return (out_b - out_a) / (in_b - in_a) * (v - in_a) + out_a;
}
template <typename T> T Map01(T in_a, T in_b, T v) {
	return MapRange(in_a, in_b, T(0), T(1), v);
}

Vec2 MoveTowards(Vec2 from, Vec2 to, float factor);
Vec3 MoveTowards(Vec3 from, Vec3 to, float factor);
Vec4 MoveTowards(Vec4 from, Vec4 to, float factor);
Vec2 RotateAround(Vec2 point, Vec2 center, float angle);
Quat RotateTowards(Quat from, Quat to, float max_angle);
Vec2 Reflect(Vec2 d, Vec2 n);

struct Packed_Color {
	uint8_t r, g, b, a;
	Packed_Color() {}
	Packed_Color(uint8_t x, uint8_t y, uint8_t z, uint8_t w) : r(x), g(y), b(z), a(w) {}
};

Packed_Color PackColor(Vec4 v);
Packed_Color PackColor(Vec3 v);
Vec4      UnpackColor4(Packed_Color c);
Vec3      UnpackColor3(Packed_Color c);

struct Rect {
	Vec2 min;
	Vec2 max;

	Rect() {
	}
	Rect(Vec2 _min, Vec2 _max) : min(_min), max(_max) {
	}
	Rect(float min_x, float min_y, float max_x, float max_y) : min(min_x, min_y), max(max_x, max_y) {
	}
};
