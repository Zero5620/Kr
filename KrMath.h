#pragma once
#include "KrBase.h"
#include <float.h>
#include <math.h>

constexpr float PI            = 3.1415926535f;
constexpr float PI_INVERSE    = 1.0f / PI;
constexpr float TAU           = PI / 2;
constexpr float REAL_EPSILON  = FLT_EPSILON;
constexpr float REAL_MAX      = FLT_MAX;
constexpr float REAL_MIN      = FLT_MIN;
constexpr float REAL_INFINITY = INFINITY;

union Vec2 {
    struct
    {
        float x, y;
    };
    float m[2];
    Vec2()
    {
    }
    Vec2(float a) : x(a), y(a)
    {
    }
    Vec2(float a, float b) : x(a), y(b)
    {
    }
    float &operator()(uint32_t index)
    {
        Assert(index < 2);
        return ((float *)this)[index];
    }
};

union Vec3 {
    struct
    {
        float x, y, z;
    };
    struct
    {
        Vec2  xy;
        float _z;
    };
    struct
    {
        float _x;
        Vec2  yz;
    };
    float m[3];
    Vec3()
    {
    }
    Vec3(float a) : x(a), y(a), z(a)
    {
    }
    Vec3(float a, float b, float c) : x(a), y(b), z(c)
    {
    }
    Vec3(Vec2 ab, float c) : xy(ab), _z(c)
    {
    }
    Vec3(float a, Vec2 cd) : _x(a), yz(cd)
    {
    }
    float &operator()(uint32_t index)
    {
        Assert(index < 3);
        return ((float *)this)[index];
    }
};

union Vec4 {
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        Vec2 xy, zw;
    };
    struct
    {
        Vec3  xyz;
        float _w;
    };
    struct
    {
        float _x;
        Vec3  yzw;
    };
    float m[4];
    Vec4()
    {
    }
    Vec4(float a) : x(a), y(a), z(a), w(a)
    {
    }
    Vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d)
    {
    }
    Vec4(Vec2 ab, Vec2 cd) : xy(ab), zw(cd)
    {
    }
    Vec4(Vec3 abc, float d) : xyz(abc), _w(d)
    {
    }
    Vec4(float a, Vec3 bcd) : _x(a), yzw(bcd)
    {
    }
    float &operator()(uint32_t index)
    {
        Assert(index < 4);
        return ((float *)this)[index];
    }
};

union Mat2 {
    Vec2  rows[2];
    float m[4];
    float m2[2][2];
    inline Mat2()
    {
    }
    inline Mat2(Vec2 a, Vec2 b) : rows{a, b}
    {
    }
    float &operator()(uint32_t i, uint32_t j)
    {
        Assert(i < 2 && j < 2);
        return m2[i][j];
    }
};

union Mat3 {
    Vec3  rows[3];
    float m[9];
    float m2[3][3];
    inline Mat3()
    {
    }
    inline Mat3(Vec3 a, Vec3 b, Vec3 c) : rows{a, b, c}
    {
    }
    float &operator()(uint32_t i, uint32_t j)
    {
        Assert(i < 3 && j < 3);
        return m2[i][j];
    }
};

union Mat4 {
    Vec4  rows[4];
    float m[16];
    float m2[4][4];
    inline Mat4()
    {
    }
    inline Mat4(Vec4 a, Vec4 b, Vec4 c, Vec4 d) : rows{a, b, c, d}
    {
    }
    inline float &operator()(uint32_t i, uint32_t j)
    {
        Assert(i < 4 && j < 4);
        return m2[i][j];
    }
};

union Quat {
    float m[4];
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float i, j, k, real;
    };
    struct
    {
        Vec4 v4;
    };
    Quat()
    {
    }
    Quat(Vec4 v) : v4(v)
    {
    }
    Quat(float b, float c, float d, float a)
    {
        m[0] = b;
        m[1] = c;
        m[2] = d;
        m[3] = a;
    }
    float &operator()(uint32_t index)
    {
        Assert(index < 4);
        return ((float *)this)[index];
    }
};

#define ExpandVec2(v) (v).x, (v).y
#define ExpandVec3(v) (v).x, (v).y, (v).z
#define ExpandVec4(v) (v).x, (v).y, (v).z, (v).w
#define Fmt_Vec2 "(%f, %f)"
#define Fmt_Vec3 "(%f, %f, %f)"
#define Fmt_Vec4 "(%f, %f, %f, %f)"

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

INLINE_PROCEDURE Vec2 Arm(float angle)
{
    return Vec2(MathCos(angle), MathSin(angle));
}
INLINE_PROCEDURE Vec2 ArmInverse(float angle)
{
    return Vec2(MathSin(angle), MathCos(angle));
}

INLINE_PROCEDURE Vec2 MinimumVec(Vec2 a, Vec2 b)
{
    return Vec2{Minimum(a.x, b.x), Minimum(a.y, b.y)};
}
INLINE_PROCEDURE Vec2 MaximumVec(Vec2 a, Vec2 b)
{
    return Vec2{Maximum(a.x, b.x), Maximum(a.y, b.y)};
}
INLINE_PROCEDURE Vec3 MinimumVec(Vec3 a, Vec3 b)
{
    return Vec3{Minimum(a.x, b.x), Minimum(a.y, b.y), Minimum(a.z, b.z)};
}
INLINE_PROCEDURE Vec3 MaximumVec(Vec3 a, Vec3 b)
{
    return Vec3{Maximum(a.x, b.x), Maximum(a.y, b.y), Maximum(a.z, b.z)};
}
INLINE_PROCEDURE Vec4 MinimumVec(Vec4 a, Vec4 b)
{
    return Vec4{Minimum(a.x, b.x), Minimum(a.y, b.y), Minimum(a.z, b.z), Minimum(a.w, b.w)};
}
INLINE_PROCEDURE Vec4 MaximumVec(Vec4 a, Vec4 b)
{
    return Vec4{Maximum(a.x, b.x), Maximum(a.y, b.y), Maximum(a.z, b.z), Maximum(a.w, b.w)};
}

INLINE_PROCEDURE bool IsNull(Vec2 a)
{
    return MathAbsolute(a.x) < REAL_EPSILON && MathAbsolute(a.y) < REAL_EPSILON;
}
INLINE_PROCEDURE bool IsNull(Vec3 a)
{
    return MathAbsolute(a.x) < REAL_EPSILON && MathAbsolute(a.y) < REAL_EPSILON && MathAbsolute(a.z) < REAL_EPSILON;
}
INLINE_PROCEDURE bool IsNull(Vec4 a)
{
    return MathAbsolute(a.x) < REAL_EPSILON && MathAbsolute(a.y) < REAL_EPSILON && MathAbsolute(a.z) < REAL_EPSILON &&
           MathAbsolute(a.w) < REAL_EPSILON;
}

INLINE_PROCEDURE Vec2 operator+(Vec2 a, Vec2 b)
{
    return Vec2(a.x + b.x, a.y + b.y);
}
INLINE_PROCEDURE Vec3 operator+(Vec3 a, Vec3 b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
INLINE_PROCEDURE Vec4 operator+(Vec4 a, Vec4 b)
{
    return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
INLINE_PROCEDURE Vec2 operator-(Vec2 a, Vec2 b)
{
    return Vec2(a.x - b.x, a.y - b.y);
}
INLINE_PROCEDURE Vec3 operator-(Vec3 a, Vec3 b)
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
INLINE_PROCEDURE Vec4 operator-(Vec4 a, Vec4 b)
{
    return Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
INLINE_PROCEDURE Vec2 operator*(float s, Vec2 v)
{
    return Vec2(s * v.x, s * v.y);
}
INLINE_PROCEDURE Vec2 operator*(Vec2 v, float s)
{
    return s * v;
}
INLINE_PROCEDURE Vec3 operator*(float s, Vec3 v)
{
    return Vec3(s * v.x, s * v.y, s * v.z);
}
INLINE_PROCEDURE Vec3 operator*(Vec3 v, float s)
{
    return s * v;
}
INLINE_PROCEDURE Vec4 operator*(float s, Vec4 v)
{
    return Vec4(s * v.x, s * v.y, s * v.z, s * v.w);
}
INLINE_PROCEDURE Vec4 operator*(Vec4 v, float s)
{
    return s * v;
}
INLINE_PROCEDURE Vec2 operator/(Vec2 v, float s)
{
    s = 1.0f / s;
    return Vec2(v.x * s, v.y * s);
}
INLINE_PROCEDURE Vec3 operator/(Vec3 v, float s)
{
    s = 1.0f / s;
    return Vec3(v.x * s, v.y * s, v.z * s);
}
INLINE_PROCEDURE Vec4 operator/(Vec4 v, float s)
{
    return v * 1.0f / s;
}
INLINE_PROCEDURE Vec2 operator*(Vec2 l, Vec2 r)
{
    return Vec2(l.x * r.x, l.y * r.y);
}
INLINE_PROCEDURE Vec3 operator*(Vec3 l, Vec3 r)
{
    return Vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}
INLINE_PROCEDURE Vec4 operator*(Vec4 l, Vec4 r)
{
    return Vec4(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w);
}
INLINE_PROCEDURE Vec2 operator/(Vec2 l, Vec2 r)
{
    return Vec2(l.x / r.x, l.y / r.y);
}
INLINE_PROCEDURE Vec3 operator/(Vec3 l, Vec3 r)
{
    return Vec3(l.x / r.x, l.y / r.y, l.z / r.z);
}
INLINE_PROCEDURE Vec4 operator/(Vec4 l, Vec4 r)
{
    return Vec4(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w);
}

INLINE_PROCEDURE Vec2 operator-(const Vec2 &v)
{
    return Vec2(-v.x, -v.y);
}
INLINE_PROCEDURE Vec3 operator-(const Vec3 &v)
{
    return Vec3(-v.x, -v.y, -v.z);
}
INLINE_PROCEDURE Vec4 operator-(const Vec4 &v)
{
    return Vec4(-v.x, -v.y, -v.z, -v.w);
}
INLINE_PROCEDURE Vec2 &operator+=(Vec2 &a, Vec2 b)
{
    a = a + b;
    return a;
}
INLINE_PROCEDURE Vec3 &operator+=(Vec3 &a, Vec3 b)
{
    a = a + b;
    return a;
}
INLINE_PROCEDURE Vec4 &operator+=(Vec4 &a, Vec4 b)
{
    a = a + b;
    return a;
}
INLINE_PROCEDURE Vec2 &operator-=(Vec2 &a, Vec2 b)
{
    a = a - b;
    return a;
}
INLINE_PROCEDURE Vec3 &operator-=(Vec3 &a, Vec3 b)
{
    a = a - b;
    return a;
}
INLINE_PROCEDURE Vec4 &operator-=(Vec4 &a, Vec4 b)
{
    a = a - b;
    return a;
}
INLINE_PROCEDURE Vec2 &operator*=(Vec2 &t, float s)
{
    t = t * s;
    return t;
}
INLINE_PROCEDURE Vec3 &operator*=(Vec3 &t, float s)
{
    t = t * s;
    return t;
}
INLINE_PROCEDURE Vec4 &operator*=(Vec4 &t, float s)
{
    t = t * s;
    return t;
}
INLINE_PROCEDURE Vec2 &operator/=(Vec2 &t, float s)
{
    t = t / s;
    return t;
}
INLINE_PROCEDURE Vec3 &operator/=(Vec3 &t, float s)
{
    t = t / s;
    return t;
}
INLINE_PROCEDURE Vec4 &operator/=(Vec4 &t, float s)
{
    t = t / s;
    return t;
}
INLINE_PROCEDURE Vec2 &operator*=(Vec2 &t, Vec2 s)
{
    t = t * s;
    return t;
}
INLINE_PROCEDURE Vec3 &operator*=(Vec3 &t, Vec3 s)
{
    t = t * s;
    return t;
}
INLINE_PROCEDURE Vec4 &operator*=(Vec4 &t, Vec4 s)
{
    t = t * s;
    return t;
}
INLINE_PROCEDURE Vec2 &operator/=(Vec2 &t, Vec2 s)
{
    t = t / s;
    return t;
}
INLINE_PROCEDURE Vec3 &operator/=(Vec3 &t, Vec3 s)
{
    t = t / s;
    return t;
}
INLINE_PROCEDURE Vec4 &operator/=(Vec4 &t, Vec4 s)
{
    t = t / s;
    return t;
}

INLINE_PROCEDURE float DotProduct(Vec2 a, Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}
INLINE_PROCEDURE float DotProduct(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
INLINE_PROCEDURE float DotProduct(Vec4 a, Vec4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
INLINE_PROCEDURE float Determinant(Vec2 a, Vec2 b)
{
    return (a.x * b.y) - (a.y * b.x);
}
INLINE_PROCEDURE Vec3 CrossProduct(Vec2 a, Vec2 b)
{
    Vec3 res;
    res.x = 0;
    res.y = 0;
    res.z = (a.x * b.y) - (a.y * b.x);
    return res;
}
INLINE_PROCEDURE Vec3 CrossProduct(Vec3 a, Vec3 b)
{
    Vec3 res;
    res.x = (a.y * b.z) - (a.z * b.y);
    res.y = (a.z * b.x) - (a.x * b.z);
    res.z = (a.x * b.y) - (a.y * b.x);
    return res;
}
INLINE_PROCEDURE Vec2 TripleProduct(Vec2 a, Vec2 b, Vec2 c)
{
    float det = Determinant(a, b);
    Vec2  res;
    res.x = -c.y * det;
    res.y = c.x * det;
    return res;
}
INLINE_PROCEDURE Vec3 TripleProduct(Vec3 a, Vec3 b, Vec3 c)
{
    return CrossProduct(CrossProduct(a, b), c);
}

INLINE_PROCEDURE float LengthSq(Vec2 v)
{
    return DotProduct(v, v);
}
INLINE_PROCEDURE float LengthSq(Vec3 v)
{
    return DotProduct(v, v);
}
INLINE_PROCEDURE float LengthSq(Vec4 v)
{
    return DotProduct(v, v);
}
INLINE_PROCEDURE float Length(Vec2 v)
{
    return MathSquareRoot(DotProduct(v, v));
}
INLINE_PROCEDURE float Length(Vec3 v)
{
    return MathSquareRoot(DotProduct(v, v));
}
INLINE_PROCEDURE float Length(Vec4 v)
{
    return MathSquareRoot(DotProduct(v, v));
}
INLINE_PROCEDURE float Distance(float a, float b)
{
    return b - a;
}
INLINE_PROCEDURE float Distance(Vec2 a, Vec2 b)
{
    return Length(b - a);
}
INLINE_PROCEDURE float Distance(Vec3 a, Vec3 b)
{
    return Length(b - a);
}
INLINE_PROCEDURE float Distance(Vec4 a, Vec4 b)
{
    return Length(b - a);
}

INLINE_PROCEDURE Vec2 NormalizeChecked(Vec2 v)
{
    Vec2  res = {};
    float len = Length(v);
    if (len != 0)
        res = v / len;
    return res;
}
INLINE_PROCEDURE Vec3 NormalizeChecked(Vec3 v)
{
    Vec3  res = {};
    float len = Length(v);
    if (len != 0)
        res = v / len;
    return res;
}
INLINE_PROCEDURE Vec4 NormalizeChecked(Vec4 v)
{
    Vec4  res = {};
    float len = Length(v);
    if (len != 0)
        res = v * (1.0f / len);
    return res;
}
INLINE_PROCEDURE Vec2 Normalize(Vec2 v)
{
    Vec2  res = {};
    float len = Length(v);
    Assert(len != 0);
    res = v / len;
    return res;
}
INLINE_PROCEDURE Vec3 Normalize(Vec3 v)
{
    Vec3  res = {};
    float len = Length(v);
    Assert(len != 0);
    res = v / len;
    return res;
}
INLINE_PROCEDURE Vec4 Normalize(Vec4 v)
{
    Vec4  res = {};
    float len = Length(v);
    Assert(len != 0);
    res = v * (1.0f / len);
    return res;
}
INLINE_PROCEDURE Vec2 PerpendicularVector(Vec2 a, Vec2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return Normalize(Vec2(-dy, dx));
}

INLINE_PROCEDURE float AngleBetween(Vec2 a, Vec2 b)
{
    float dot = Clamp(-1.0f, 1.0f, DotProduct(a, b));
    return MathCos(dot);
}
INLINE_PROCEDURE float AngleBetween(Vec3 a, Vec3 b)
{
    float dot = Clamp(-1.0f, 1.0f, DotProduct(a, b));
    return MathCos(dot);
}
INLINE_PROCEDURE float AngleBetweenNormalized(Vec2 a, Vec2 b)
{
    a = Normalize(a);
    b = Normalize(b);
    return AngleBetween(a, b);
}
INLINE_PROCEDURE float AngleBetweenNormalized(Vec3 a, Vec3 b)
{
    a = Normalize(a);
    b = Normalize(b);
    return AngleBetween(a, b);
}
INLINE_PROCEDURE float SignedAngleBetween(Vec2 a, Vec2 b)
{
    float dot   = Clamp(-1.0f, 1.0f, DotProduct(a, b));
    float angle = MathCos(dot);
    float cross = a.x * b.y - a.y * b.x;
    if (cross < 0)
    {
        angle = -angle;
    }
    return angle;
}
INLINE_PROCEDURE float SignedAngleBetween(Vec3 a, Vec3 b, Vec3 n)
{
    float dot   = Clamp(-1.0f, 1.0f, DotProduct(a, b));
    float angle = MathCos(dot);
    Vec3  cross = CrossProduct(a, b);
    if (DotProduct(n, cross) < 0)
    {
        angle = -angle;
    }
    return angle;
}
INLINE_PROCEDURE float SignedAngleBetweenNormalized(Vec2 a, Vec2 b)
{
    a = Normalize(a);
    b = Normalize(b);
    return SignedAngleBetween(a, b);
}
INLINE_PROCEDURE float SignedAngleBetweenNormalized(Vec3 a, Vec3 b, Vec3 n)
{
    a = Normalize(a);
    b = Normalize(b);
    n = Normalize(n);
    return SignedAngleBetween(a, b, n);
}

INLINE_PROCEDURE Vec2 ClampVec(Vec2 min, Vec2 max, Vec2 v)
{
    v.x = Clamp(min.x, max.x, v.x);
    v.y = Clamp(min.y, max.y, v.y);
    return v;
}
INLINE_PROCEDURE Vec3 ClampVec(Vec3 min, Vec3 max, Vec3 v)
{
    v.x = Clamp(min.x, max.x, v.x);
    v.y = Clamp(min.y, max.y, v.y);
    v.z = Clamp(min.z, max.z, v.z);
    return v;
}
INLINE_PROCEDURE Vec4 ClampVec(Vec4 min, Vec4 max, Vec4 v)
{
    v.x = Clamp(min.x, max.x, v.x);
    v.y = Clamp(min.y, max.y, v.y);
    v.z = Clamp(min.z, max.z, v.z);
    v.w = Clamp(min.w, max.w, v.w);
    return v;
}

Mat2         IdentityMat2();
float        Determinant(const Mat2 &mat);
Mat2         Inverse(const Mat2 &mat);
Mat2         Transpose(const Mat2 &m);
Mat3         IdentityMat3();
float        Determinant(const Mat3 &mat);
Mat3         Inverse(const Mat3 &mat);
Mat3         Transpose(const Mat3 &m);
Mat4         IdentityMat4();
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

INLINE_PROCEDURE Mat2 &operator*=(Mat2 &t, Mat2 &o)
{
    t = t * o;
    return t;
}
INLINE_PROCEDURE Mat3 &operator*=(Mat3 &t, Mat3 &o)
{
    t = t * o;
    return t;
}
INLINE_PROCEDURE Mat4 &operator*=(Mat4 &t, Mat4 &o)
{
    t = t * o;
    return t;
}

//
//
//

Mat2        ScalarMat2(float x, float y);
Mat2        ScalarMat2(Vec2 s);
Mat2        RotationMat2(float angle);
Mat3        ScalarMat3(float S_1, float S_2);
Mat3        ScalarMat3(Vec2 s);
Mat3        TranslationMat3(float T_x, float T_y);
Mat3        TranslationMat3(Vec2 t);
Mat3        RotationMat3(float angle);
Mat3        LookAt(Vec2 from, Vec2 to, Vec2 forward);
Mat4        LookAtDirection(Vec3 dir, Vec3 world_up);
Mat4        ScalarMat4(float S_1, float S_2, float S_3);
Mat4        ScalarMat4(Vec3 s);
Mat4        TranslationMat4(float T_x, float T_y, float T_z);
Mat4        TranslationMat4(Vec3 t);
Mat4        RotationX(float angle);
Mat4        RotationY(float angle);
Mat4        RotationZ(float angle);
Mat4        RotationMat4(float x, float y, float z, float angle);
Mat4        RotationMat4(Vec3 axis, float angle);
Mat4        LookAt(Vec3 from, Vec3 to, Vec3 world_up);
Mat4        OrthographicProjectionLH(float l, float r, float t, float b, float n, float f);
Mat4        OrthographicProjectionRH(float l, float r, float t, float b, float n, float f);
Mat4        PerspectiveProjectionLH(float fov, float aspect_ratio, float n, float f);
Mat4        PerspectiveProjectionRH(float fov, float aspect_ratio, float n, float f);
Mat4        OrthographicProjectionExRangeRH(float l, float r, float t, float b, float n, float f);
Mat4        PerspectiveProjectionExRangeRH(float fov, float aspect_ratio, float n, float f);
Vec3        GetRight(const Mat4 &m);
Vec3        GetUp(const Mat4 &m);
Vec3        GetForward(const Mat4 &m);
Mat2        ToMat2(const Mat3 &mat);
Mat3        ToMat3(const Mat2 &mat);
Mat3        ToMat3(const Mat4 &mat);
Mat4        ToMat4(const Mat3 &mat);

INLINE_PROCEDURE Quat operator-(Quat &q)
{
    return Quat(-q.v4);
}
INLINE_PROCEDURE Quat operator-(Quat r1, Quat r2)
{
    return Quat(r1.v4 - r2.v4);
}
INLINE_PROCEDURE Quat operator+(Quat r1, Quat r2)
{
    return Quat(r1.v4 + r2.v4);
}
INLINE_PROCEDURE Quat operator*(Quat q, float s)
{
    return Quat(q.v4 * s);
}
INLINE_PROCEDURE Quat operator*(float s, Quat q)
{
    return Quat(q.v4 * s);
}

Quat        IdentityQuat();
float       DotProduct(Quat q1, Quat q2);
float       Length(Quat q);
Quat        Normalize(Quat q);
Quat        Conjugate(Quat q);
Quat        operator*(Quat q1, Quat q2);
Vec3        Rotate(Quat q, Vec3 v);
INLINE_PROCEDURE Vec3 operator*(Quat q, Vec3 v)
{
    return Rotate(q, v);
}
Quat                          QuatFromAngleAxis(Vec3 axis, float angle);
Quat                          QuatFromAngleAxisNormalized(Vec3 axis, float angle);
void                          GetAngleAxis(Quat q, float *angle, Vec3 *axis);
Vec3                          GetAxis(Quat q);
float                         GetAngle(Quat q);
Quat                          QuatFromMat4(const Mat4 &m);
Quat                          QuatFromMat4Nomalized(const Mat4 &m);
Mat4                          GetMat4(Quat q);
Vec3                          GetForward(Quat q);
Vec3                          GetRight(Quat q);
Vec3                          GetUp(Quat q);
Quat                          QuatFromEulerAngles(float pitch, float yaw, float roll);
Quat                          QuatFromEulerAngles(Vec3 euler);
Vec3                          GetEulerAngles(Quat q);
Quat                          QuatBetween(Vec3 from, Vec3 to);
Quat                          QuatBetween(Quat a, Quat b);
Quat                          QuatLookAt(Vec3 from, Vec3 to, Vec3 world_forward);

Vec3                          LinearToSrgb(Vec3 color);
Vec4                          LinearToSrgb(Vec4 color);
Vec3                          LinearToSrgb(Vec3 color, float gamma);
Vec4                          LinearToSrgb(Vec4 color, float gamma);
Vec3                          SrgbToLinear(Vec3 color);
Vec4                          SrgbToLinear(Vec4 color);
Vec3                          SrgbToLinear(Vec3 color, float gamma);
Vec4                          SrgbToLinear(Vec4 color, float gamma);

Vec3                          HsvToRgb(Vec3 c);
Vec3                          RgbToHsv(Vec3 c);
Vec4                          HsvToRgb(Vec4 c);
Vec4                          RgbToHsv(Vec4 c);

template <typename Type> Type Lerp(Type from, Type to, float t)
{
    return (1 - t) * from + t * to;
}

template <typename Type> Type BezierQuadratic(Type a, Type b, Type c, float t)
{
    float mt = 1 - t;
    float w1 = mt * mt;
    float w2 = 2 * mt * t;
    float w3 = t * t;
    return w1 * a + w2 * b + w3 * c;
}

template <typename Type> Type BezierCubic(Type a, Type b, Type c, Type d, float t)
{
    float mt = 1.0f - t;
    float w1 = mt * mt * mt;
    float w2 = 3 * mt * mt * t;
    float w3 = 3 * mt * t * t;
    float w4 = t * t * t;
    return w1 * a + w2 * b + w3 * c + w4 * d;
}

template <typename Type> Type Slerp(Type from, Type to, float angle, float t)
{
    float s   = MathSin(angle);
    float ts  = MathSin(angle * t);
    float mts = MathSin(angle * (1 - t));
    return (mts * from + ts * to) * (1.0f / s);
}

Mat2                        Lerp(const Mat2 &from, const Mat2 &to, float t);
Mat3                        Lerp(const Mat3 &from, const Mat3 &to, float t);
Mat4                        Lerp(const Mat4 &from, const Mat4 &to, float t);
Vec2                        Slerp(Vec2 from, Vec2 to, float t);
Vec3                        Slerp(Vec3 from, Vec3 to, float t);
Quat                        Slerp(Quat from, Quat to, float t);
float                       Step(float edge, float val);
Vec2                        Step(Vec2 edge, Vec2 val);
Vec3                        Step(Vec3 edge, Vec3 val);
Vec4                        Step(Vec4 edge, Vec4 val);
Quat                        Step(Quat edge, Quat val);

template <typename T> float SmoothStepZeroChecked(T a, T b, T v)
{
    float div_distance = Distance(a, b);
    if (div_distance)
    {
        auto x = Clamp(0.0f, 1.0f, Distance(a, v) / div_distance);
        return x * x * (3 - 2 * x);
    }
    return 1;
}

template <typename T> float SmoothStep(T a, T b, T v)
{
    auto x = Clamp(0.0f, 1.0f, Distance(a, v) / Distance(a, b));
    return x * x * (3 - 2 * x);
}

float                   InverseSmoothStep(float x);

template <typename T> T MapRange(T in_a, T in_b, T out_a, T out_b, T v)
{
    return (out_b - out_a) / (in_b - in_a) * (v - in_a) + out_a;
}
template <typename T> T Map01(T in_a, T in_b, T v)
{
    return MapRange(in_a, in_b, T(0), T(1), v);
}

Vec2 MoveTowards(Vec2 from, Vec2 to, float factor);
Vec3 MoveTowards(Vec3 from, Vec3 to, float factor);
Vec4 MoveTowards(Vec4 from, Vec4 to, float factor);
Vec2 RotateAround(Vec2 point, Vec2 center, float angle);
Quat RotateTowards(Quat from, Quat to, float max_angle);
Vec2 Reflect(Vec2 d, Vec2 n);

union UintColor {
    struct
    {
        uint8_t r, g, b, a;
    };
    uint8_t channels[4];

    UintColor()
    {
    }
    UintColor(uint8_t x, uint8_t y, uint8_t z, uint8_t w) : r(x), g(y), b(z), a(w)
    {
    }
};

UintColor Vec4ToUintColor(Vec4 v);
UintColor Vec3ToUintColor(Vec3 v);
Vec4      UintColorToVec4(UintColor c);
Vec3      UintColorToVec3(UintColor c);

struct Rect
{
    Vec2 Min;
    Vec2 Max;

    Rect()
    {
    }
    Rect(Vec2 min, Vec2 max) : Min(min), Max(max)
    {
    }
    Rect(float min_x, float min_y, float max_x, float max_y) : Min(min_x, min_y), Max(max_x, max_y)
    {
    }
};

//
//
//

template <typename T>
inline T Integrate(const T &x, float h, const T &f) {
	return x + h * f;
}

template <typename T, typename Function>
inline T Integrate(const T &x, float t, float h, Function f) {
	return x + h * f(t, x);
}

template <typename T, typename Function>
inline T IntegrateRK4(const T &x, float t, float h, Function f) {
	T k1 = h * f(t, x);
	T k2 = h * f(t + 0.5f * h, x + 0.5f * k1);
	T k3 = h * f(t + 0.5f * h, x + 0.5f * k2);
	T k4 = h * f(t + h, x + k3);
	return x + (k1 + 2.0f * (k2 + k3) + k4) / 6.0f;
}

//
//
//

float TriangleAreaSigned(Vec2 a, Vec2 b, Vec2 c);
float TriangleAreaSigned(Vec3 a, Vec3 b, Vec3 c);
bool IsTriangleClockwise(Vec2 a, Vec2 b, Vec2 c);
Vec2 RectangleCorner(Rect b, uint32_t n); // n=00,01,10,11 for 4 different corners

float PointToSegmentLengthSq(Vec2 p, Vec2 a, Vec2 b);
float PointToRectLengthSq(Vec2 p, Rect rect);

INLINE_PROCEDURE float PointToSegmentLength(Vec2 p, Vec2 a, Vec2 b)
{ 
    return MathSquareRoot(PointToSegmentLengthSq(p, a, b));
}

INLINE_PROCEDURE float PointToRectLength(Vec2 p, Rect rect)
{ 
    return MathSquareRoot(PointToRectLengthSq(p, rect));
}

Vec2 NearestPointBetweenPointSegment(Vec2 p, Vec2 a, Vec2 b, float *t);
Vec2 NearestPointBetweenPointSegment(Vec2 p, Vec2 a, Vec2 b);
Vec2 NearestPointBetweenOriginSegment(Vec2 a, Vec2 b, float *t);
Vec2 NearestPointBetweenOriginSegment(Vec2 a, Vec2 b);
Vec2 NearestPointBetweenPointRect(Vec2 a, Rect rect);
Vec2 NearestPointBetweenOriginRect(Rect rect);
Vec2 NearestPointBetweenPointTriangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c);
Vec2 NearestPointBetweenOriginTriangle(Vec2 a, Vec2 b, Vec2 c);

float NearestPointBetween2Segments(Vec2 p1, Vec2 q1, Vec2 p2, Vec2 q2, float *s, float *t, Vec2 *c1, Vec2 *c2);

Vec3 Barycentric(Vec2 a, Vec2 b, Vec2 c, Vec2 p);
Vec3 Barycentric(Vec3 a, Vec3 b, Vec3 c, Vec3 p);

bool IsQuadConvex(Vec2 a, Vec2 b, Vec2 c, Vec2 d);
bool IsQuadConvex(Vec3 a, Vec3 b, Vec3 c, Vec3 d);
bool IsPolygonConvex(const Vec2 *vertices, uint32_t count);

int64_t PointFarthestFromEdge(Vec2 a, Vec2 b, Vec2 *p, uint32_t n);

struct Extreme_Point_Index
{
    uint32_t Min, Max;
};
Extreme_Point_Index ExtremePointsAlongDirection(Vec2 dir, Vec2 *pt, uint32_t n);
Extreme_Point_Index ExtremePointsOnRect(Vec2 *pt, uint32_t n);

//
//
//

bool PointInsideRect(Vec2 p, Rect rect);
