#include "KrMath.h"

Mat2 IdentityMat2()
{
    Mat2 m;
    m.rows[0] = V2(1.0f, 0.0f);
    m.rows[1] = V2(0.0f, 1.0f);
    return m;
}

float Determinant(const Mat2 &mat)
{
    return mat.m2[0][0] * mat.m2[1][1] - mat.m2[0][1] * mat.m2[1][0];
}

Mat2 Inverse(const Mat2 &mat)
{
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

Mat2 Transpose(const Mat2 &m)
{
    Mat2 t;
    t.m2[0][0] = m.m2[0][0];
    t.m2[0][1] = m.m2[1][0];
    t.m2[1][0] = m.m2[0][1];
    t.m2[1][1] = m.m2[1][1];
    return t;
}

Mat3 IdentityMat3()
{
    Mat3 m;
    m.rows[0] = V3(1.0f, 0.0f, 0.0f);
    m.rows[1] = V3(0.0f, 1.0f, 0.0f);
    m.rows[2] = V3(0.0f, 0.0f, 1.0f);
    return m;
}

float Determinant(const Mat3 &mat)
{
    float det = mat.m2[0][0] * (mat.m2[1][1] * mat.m2[2][2] - mat.m2[2][1] * mat.m2[1][2]) +
                mat.m2[0][1] * (mat.m2[1][2] * mat.m2[2][0] - mat.m2[1][0] * mat.m2[2][2]) +
                mat.m2[0][2] * (mat.m2[1][0] * mat.m2[2][1] - mat.m2[2][0] * mat.m2[1][1]);
    return det;
}

Mat3 Inverse(const Mat3 &mat)
{
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

Mat3 Transpose(const Mat3 &m)
{
    Mat3 res;
    res.rows[0] = V3(m.m2[0][0], m.m2[1][0], m.m2[2][0]);
    res.rows[1] = V3(m.m2[0][1], m.m2[1][1], m.m2[2][1]);
    res.rows[2] = V3(m.m2[0][2], m.m2[1][2], m.m2[2][2]);
    return res;
}

Mat4 IdentityMat4()
{
    Mat4 m;
    m.rows[0] = V4(1.0f, 0.0f, 0.0f, 0.0f);
    m.rows[1] = V4(0.0f, 1.0f, 0.0f, 0.0f);
    m.rows[2] = V4(0.0f, 0.0f, 1.0f, 0.0f);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

float Determinant(const Mat4 &mat)
{
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

Mat4 Inverse(const Mat4 &mat)
{
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

Mat4 Transpose(const Mat4 &m)
{
    Mat4 res;
    res.rows[0] = V4(m.m2[0][0], m.m2[1][0], m.m2[2][0], m.m2[3][0]);
    res.rows[1] = V4(m.m2[0][1], m.m2[1][1], m.m2[2][1], m.m2[3][1]);
    res.rows[2] = V4(m.m2[0][2], m.m2[1][2], m.m2[2][2], m.m2[3][2]);
    res.rows[3] = V4(m.m2[0][3], m.m2[1][3], m.m2[2][3], m.m2[3][3]);
    return res;
}

//
//
//

Mat2 operator*(const Mat2 &left, const Mat2 &right)
{
    Mat2 res;
    Mat2 tras = Transpose(right);

    res.m2[0][0] = DotProduct(left.rows[0], tras.rows[0]);
    res.m2[0][1] = DotProduct(left.rows[0], tras.rows[1]);
    res.m2[1][0] = DotProduct(left.rows[1], tras.rows[0]);
    res.m2[1][1] = DotProduct(left.rows[1], tras.rows[1]);

    return res;
}

Vec2 operator*(const Mat2 &mat, Vec2 vec)
{
    Vec2 res;
    res.m[0] = DotProduct(vec, mat.rows[0]);
    res.m[1] = DotProduct(vec, mat.rows[1]);
    return res;
}

Vec2 operator*(Vec2 vec, const Mat2 &mat)
{
    Vec2 res;
    res.m[0] = DotProduct(vec, V2(mat.m2[0][0], mat.m2[1][0]));
    res.m[1] = DotProduct(vec, V2(mat.m2[0][1], mat.m2[1][1]));
    return res;
}

Mat3 operator*(const Mat3 &left, const Mat3 &right)
{
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

Vec3 operator*(const Mat3 &mat, Vec3 vec)
{
    Vec3 res;
    res.m[0] = DotProduct(vec, mat.rows[0]);
    res.m[1] = DotProduct(vec, mat.rows[1]);
    res.m[2] = DotProduct(vec, mat.rows[2]);
    return res;
}

Mat4 operator*(const Mat4 &left, const Mat4 &right)
{
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

Vec4 operator*(const Mat4 &mat, Vec4 vec)
{
    Vec4 res;
    res.m[0] = DotProduct(vec, mat.rows[0]);
    res.m[1] = DotProduct(vec, mat.rows[1]);
    res.m[2] = DotProduct(vec, mat.rows[2]);
    res.m[3] = DotProduct(vec, mat.rows[3]);
    return res;
}

//
//
//

Mat2 ScalarMat2(float x, float y)
{
    Mat2 m;
    m.rows[0] = V2(x, 0.0f);
    m.rows[1] = V2(0.0f, y);
    return m;
}

Mat2 ScalarMat2(Vec2 s)
{
    Mat2 m;
    m.rows[0] = V2(s.x, 0.0f);
    m.rows[1] = V2(0.0f, s.y);
    return m;
}

Mat2 RotationMat2(float angle)
{
    float c = MathCos(angle);
    float s = MathSin(angle);

    Mat2 mat;
    mat.rows[0] = V2(c, -s);
    mat.rows[1] = V2(s, c);
    return mat;
}

Mat3 ScalarMat3(float S_1, float S_2)
{
    Mat3 m;
    m.rows[0] = V3(S_1, 0.0f, 0.0f);
    m.rows[1] = V3(0.0f, S_2, 0.0f);
    m.rows[2] = V3(0.0f, 0.0f, 1.0f);
    return m;
}

Mat3 ScalarMat3(Vec2 s)
{
    return ScalarMat3(s.x, s.y);
}

Mat3 TranslationMat3(float T_x, float T_y)
{
    Mat3 m;
    m.rows[0] = V3(1.0f, 0.0f, T_x);
    m.rows[1] = V3(0.0f, 1.0f, T_y);
    m.rows[2] = V3(0.0f, 0.0f, 1.0f);
    return m;
}

Mat3 TranslationMat3(Vec2 t)
{
    return TranslationMat3(t.x, t.y);
}

Mat3 RotationMat3(float angle)
{
    Mat3 m;
    float c = MathCos(angle);
    float s = MathSin(angle);
    m.rows[0] = V3(c, -s, 0.0f);
    m.rows[1] = V3(s, c, 0.0f);
    m.rows[2] = V3(0.0f, 0.0f, 1.0f);
    return m;
}

Mat3 LookAt(Vec2 from, Vec2 to, Vec2 forward)
{
    Vec2 dir = Normalize(to - from);
    float cos_theta = DotProduct(dir, forward);
    float sin_theta = MathSquareRoot(1.0f - cos_theta * cos_theta);

    Mat3 m;
    m.rows[0] = V3(cos_theta, -sin_theta, from.x);
    m.rows[1] = V3(sin_theta, cos_theta, from.y);
    m.rows[2] = V3(0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 ScalarMat4(float S_1, float S_2, float S_3)
{
    Mat4 m;
    m.rows[0] = V4(S_1, 0.0f, 0.0f, 0.0f);
    m.rows[1] = V4(0.0f, S_2, 0.0f, 0.0f);
    m.rows[2] = V4(0.0f, 0.0f, S_3, 0.0f);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 ScalarMat4(Vec3 s)
{
    return ScalarMat4(s.x, s.y, s.z);
}

Mat4 TranslationMat4(float T_x, float T_y, float T_z)
{
    Mat4 m;
    m.rows[0] = V4(1.0f, 0.0f, 0.0f, T_x);
    m.rows[1] = V4(0.0f, 1.0f, 0.0f, T_y);
    m.rows[2] = V4(0.0f, 0.0f, 1.0f, T_z);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 TranslationMat4(Vec3 t)
{
    return TranslationMat4(t.x, t.y, t.z);
}

Mat4 RotationX(float angle)
{
    Mat4 m;
    m.rows[0] = V4(1.0f, 0.0f, 0.0f, 0.0f);
    m.rows[1] = V4(0.0f, MathCos(angle), -MathSin(angle), 0.0f);
    m.rows[2] = V4(0.0f, MathSin(angle), MathCos(angle), 0.0f);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 RotationY(float angle)
{
    Mat4 m;
    m.rows[0] = V4(MathCos(angle), 0.0f, -MathSin(angle), 0.0f);
    m.rows[1] = V4(0.0f, 1.0f, 0.0f, 0.0f);
    m.rows[2] = V4(MathSin(angle), 0.0f, MathCos(angle), 0.0f);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 RotationZ(float angle)
{
    Mat4 m;
    m.rows[0] = V4(MathCos(angle), -MathSin(angle), 0.0f, 0.0f);
    m.rows[1] = V4(MathSin(angle), MathCos(angle), 0.0f, 0.0f);
    m.rows[2] = V4(0.0f, 0.0f, 1.0f, 0.0f);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 RotationMat4(float x, float y, float z, float angle)
{
    float c = MathCos(angle);
    float s = MathSin(angle);

    float x2 = x * x;
    float xy = x * y;
    float zx = x * z;
    float y2 = y * y;
    float yz = y * z;
    float z2 = z * z;

    Mat4 m;
    m.rows[0] = V4(c + x2 * (1 - c), xy * (1 - c) - z * s, zx * (1 - c) + y * s, 0.0f);
    m.rows[1] = V4(xy * (1 - c) + z * s, c + y2 * (1 - c), yz * (1 - c) - x * s, 0.0f);
    m.rows[2] = V4(zx * (1 - c) - y * s, yz * (1 - c) + x * s, c + z2 * (1 - c), 0.0f);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 RotationMat4(Vec3 axis, float angle)
{
    return RotationMat4(axis.x, axis.y, axis.z, angle);
}

Mat4 LookAt(Vec3 from, Vec3 to, Vec3 world_up)
{
    Vec3 forward = Normalize(from - to);
    Vec3 right = Normalize(CrossProduct(world_up, forward));
    Vec3 up = CrossProduct(right, forward);

    Mat4 lookat;
    lookat.rows[0] = V4(right.x, up.x, forward.x, -from.x);
    lookat.rows[1] = V4(right.y, up.y, forward.y, -from.y);
    lookat.rows[2] = V4(right.z, up.z, forward.z, -from.z);
    lookat.rows[3] = V4(0, 0, 0, 1);

    return lookat;
}

Mat4 LookAtDirection(Vec3 dir, Vec3 world_up)
{
    Assert(!IsNull(dir));

    Vec3 forward = dir;
    Vec3 right = Normalize(CrossProduct(world_up, forward));
    Vec3 up = CrossProduct(right, forward);

    Mat4 lookat;
    lookat.rows[0] = V4(right.x, up.x, forward.x, 0);
    lookat.rows[1] = V4(right.y, up.y, forward.y, 0);
    lookat.rows[2] = V4(right.z, up.z, forward.z, 0);
    lookat.rows[3] = V4(0, 0, 0, 1);

    return lookat;
}

Mat4 OrthographicProjectionLH(float l, float r, float t, float b, float n, float f)
{
    float iwidth = 1 / (r - l);
    float iheight = 1 / (t - b);
    float range = 1 / (f - n);

    Mat4 m;
    m.rows[0] = V4(2 * iwidth, 0.0f, 0.0f, -(l + r) * iwidth);
    m.rows[1] = V4(0.0f, 2 * iheight, 0.0f, -(t + b) * iheight);
    m.rows[2] = V4(0.0f, 0.0f, range, -n * range);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 OrthographicProjectionRH(float l, float r, float t, float b, float n, float f)
{
    float iwidth = 1 / (r - l);
    float iheight = 1 / (t - b);
    float range = 1 / (f - n);

    Mat4 m;
    m.rows[0] = V4(2 * iwidth, 0.0f, 0.0f, -(l + r) * iwidth);
    m.rows[1] = V4(0.0f, 2 * iheight, 0.0f, -(t + b) * iheight);
    m.rows[2] = V4(0.0f, 0.0f, range, n * range);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 PerspectiveProjectionLH(float fov, float aspect_ratio, float n, float f)
{
    float height = 1.0f / MathTan(fov * 0.5f);
    float width = height / aspect_ratio;
    float range = f / (f - n);

    Mat4 m;
    m.rows[0] = V4(width, 0.0f, 0.0f, 0.0f);
    m.rows[1] = V4(0.0f, height, 0.0f, 0.0f);
    m.rows[2] = V4(0.0f, 0.0f, range, -range * n);
    m.rows[3] = V4(0.0f, 0.0f, 1.0f, 0.0f);
    return m;
}

Mat4 PerspectiveProjectionRH(float fov, float aspect_ratio, float n, float f)
{
    float height = 1.0f / MathTan(fov * 0.5f);
    float width = height / aspect_ratio;
    float range = f / (f - n);

    Mat4 m;
    m.rows[0] = V4(width, 0.0f, 0.0f, 0.0f);
    m.rows[1] = V4(0.0f, height, 0.0f, 0.0f);
    m.rows[2] = V4(0.0f, 0.0f, range, range * n);
    m.rows[3] = V4(0.0f, 0.0f, -1.0f, 0.0f);
    return m;
}

Mat4 OrthographicProjectionExRangeRH(float l, float r, float t, float b, float n, float f)
{
    float iwidth = 1 / (r - l);
    float iheight = 1 / (t - b);
    float range = 1 / (f - n);

    Mat4 m;
    m.rows[0] = V4(2 * iwidth, 0.0f, 0.0f, -(l + r) * iwidth);
    m.rows[1] = V4(0.0f, 2 * iheight, 0.0f, -(t + b) * iheight);
    m.rows[2] = V4(0.0f, 0.0f, -2 * range, -(f + n) * range);
    m.rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    return m;
}

Mat4 PerspectiveProjectionExRangeRH(float fov, float aspect_ratio, float n, float f)
{
    float height = 1.0f / MathTan(fov * 0.5f);
    float width = height / aspect_ratio;
    float range = 1 / (f - n);

    Mat4 m;
    m.rows[0] = V4(width, 0.0f, 0.0f, 0.0f);
    m.rows[1] = V4(0.0f, height, 0.0f, 0.0f);
    m.rows[2] = V4(0.0f, 0.0f, -(f + n) * range, -2.0f * f * n * range);
    m.rows[3] = V4(0.0f, 0.0f, -1.0f, 0.0f);
    return m;
}

Vec3 GetRight(const Mat4 &m)
{
    Vec3 v;
    v.x = m.m2[0][0];
    v.y = m.m2[1][0];
    v.z = m.m2[2][0];
    return v;
}

Vec3 GetUp(const Mat4 &m)
{
    Vec3 v;
    v.x = m.m2[0][2];
    v.y = m.m2[1][2];
    v.z = m.m2[2][2];
    return v;
}

Vec3 GetForward(const Mat4 &m)
{
    Vec3 v;
    v.x = m.m2[0][1];
    v.y = m.m2[1][1];
    v.z = m.m2[2][1];
    return v;
}

Mat2 ToMat2(const Mat3 &mat)
{
    Mat2 result;
    result.rows[0] = mat.rows[0].xy;
    result.rows[1] = mat.rows[1].xy;
    return result;
}

Mat3 ToMat3(const Mat2 &mat)
{
    Mat3 result;
    result.rows[0] = V3(mat.rows[0], 0);
    result.rows[1] = V3(mat.rows[1], 0);
    result.rows[2] = V3(0, 0, 1);
    return result;
}

Mat3 ToMat3(const Mat4 &mat)
{
    Mat3 result;
    result.rows[0] = mat.rows[0].xyz;
    result.rows[1] = mat.rows[1].xyz;
    result.rows[2] = mat.rows[2].xyz;
    return result;
}

Mat4 ToMat4(const Mat3 &mat)
{
    Mat4 result;
    result.rows[0] = V4(mat.rows[0], 0);
    result.rows[1] = V4(mat.rows[1], 0);
    result.rows[2] = V4(mat.rows[2], 0);
    result.rows[3] = V4(0, 0, 0, 1);
    return result;
}

//
//
//

Quat IdentityQuat()
{
    return Quaternion(0, 0, 0, 1);
}

float DotProduct(Quat q1, Quat q2)
{
    return q1.real * q2.real + q1.i * q2.i + q1.j * q2.j + q1.k * q2.k;
}

float Length(Quat q)
{
    return MathSquareRoot(DotProduct(q, q));
}

Quat Normalize(Quat q)
{
    float len = Length(q);
    Assert(len != 0);
    return q * (1.0f / len);
}

Quat Conjugate(Quat q)
{
    return Quaternion(-q.i, -q.j, -q.k, q.real);
}

Quat operator*(Quat q1, Quat q2)
{
    float a = q1.w;
    float b = q1.x;
    float c = q1.y;
    float d = q1.z;

    float e = q2.w;
    float f = q2.x;
    float g = q2.y;
    float h = q2.z;

    Quat res;
    res.w = a * e - b * f - c * g - d * h;
    res.x = a * f + b * e + c * h - d * g;
    res.y = a * g - b * h + c * e + d * f;
    res.z = a * h + b * g - c * f + d * e;
    return res;
}

Vec3 Rotate(Quat q, Vec3 v)
{
    Quat p = Quaternion(v.x, v.y, v.z, 0);
    Quat qc = Conjugate(q);
    Quat res = (q * p * qc);
    return V3(res.x, res.y, res.z);
}

Quat QuatFromAngleAxis(Vec3 axis, float angle)
{
    float r = MathCos(angle * 0.5f);
    float s = MathSin(angle * 0.5f);
    float i = s * axis.x;
    float j = s * axis.y;
    float k = s * axis.z;
    return Quaternion(i, j, k, r);
}

Quat QuatFromAngleAxisNormalized(Vec3 axis, float angle)
{
    return QuatFromAngleAxis(Normalize(axis), angle);
}

void GetAngleAxis(Quat q, float *angle, Vec3 *axis)
{
    float len = MathSquareRoot(q.i * q.i + q.j * q.j + q.k * q.k);
    if (len)
    {
        *angle = 2.0f * MathArcTan2(len, q.real);
        len = 1.0f / len;
        axis->x = q.i * len;
        axis->y = q.j * len;
        axis->z = q.k * len;
    }
    else
    {
        // degenerate case, unit quaternion
        *angle = 0;
        *axis = V3(0, 0, 1);
    }
}

Vec3 GetAxis(Quat q)
{
    Vec3 axis;
    float angle;
    GetAngleAxis(q, &angle, &axis);
    return axis;
}

float GetAngle(Quat q)
{
    Vec3 axis;
    float angle;
    GetAngleAxis(q, &angle, &axis);
    return angle;
}

Quat QuatFromMat4(const Mat4 &m)
{
    Quat q;
    float trace = m.m2[0][0] + m.m2[1][1] + m.m2[2][2];
    if (trace > 0.0f)
    {
        float s = 0.5f / MathSquareRoot(trace + 1.0f);
        q.w = 0.25f / s;
        q.x = (m.m2[2][1] - m.m2[1][2]) * s;
        q.y = (m.m2[0][2] - m.m2[2][0]) * s;
        q.z = (m.m2[1][0] - m.m2[0][1]) * s;
    }
    else
    {
        if (m.m2[0][0] > m.m2[1][1] && m.m2[0][0] > m.m2[2][2])
        {
            float s = 2.0f * MathSquareRoot(1.0f + m.m2[0][0] - m.m2[1][1] - m.m2[2][2]);
            q.w = (m.m2[2][1] - m.m2[1][2]) / s;
            q.x = 0.25f * s;
            q.y = (m.m2[0][1] + m.m2[1][0]) / s;
            q.z = (m.m2[0][2] + m.m2[2][0]) / s;
        }
        else if (m.m2[1][1] > m.m2[2][2])
        {
            float s = 2.0f * MathSquareRoot(1.0f + m.m2[1][1] - m.m2[0][0] - m.m2[2][2]);
            q.w = (m.m2[0][2] - m.m2[2][0]) / s;
            q.x = (m.m2[0][1] + m.m2[1][0]) / s;
            q.y = 0.25f * s;
            q.z = (m.m2[1][2] + m.m2[2][1]) / s;
        }
        else
        {
            float s = 2.0f * MathSquareRoot(1.0f + m.m2[2][2] - m.m2[0][0] - m.m2[1][1]);
            q.w = (m.m2[1][0] - m.m2[0][1]) / s;
            q.x = (m.m2[0][2] + m.m2[2][0]) / s;
            q.y = (m.m2[1][2] + m.m2[2][1]) / s;
            q.z = 0.25f * s;
        }
    }
    return Normalize(q);
}

Quat QuatFromMat4Nomalized(const Mat4 &m)
{
    Mat4 nm;
    nm.rows[0] = V4(Normalize(m.rows[0].xyz), m.rows[0].w);
    nm.rows[1] = V4(Normalize(m.rows[1].xyz), m.rows[1].w);
    nm.rows[2] = V4(Normalize(m.rows[2].xyz), m.rows[2].w);
    nm.rows[3] = V4(Normalize(m.rows[3].xyz), m.rows[3].w);
    return QuatFromMat4(nm);
}

Mat4 GetMat4(Quat q)
{
    float i = q.i;
    float j = q.j;
    float k = q.k;
    float r = q.real;

    float ii = i * i;
    float jj = j * j;
    float kk = k * k;

    float ij = i * j;
    float jk = j * k;
    float kr = k * r;
    float jr = j * r;
    float ir = i * r;
    float ik = i * k;

    Mat4 m;

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

Vec3 GetForward(Quat q)
{
    Vec3 up;
    up.x = 2 * (q.i * q.k + q.j * q.real);
    up.y = 2 * (q.j * q.k - q.i * q.real);
    up.z = 1 - 2 * (q.i * q.i + q.j * q.j);
    return Normalize(up);
}

Vec3 GetRight(Quat q)
{
    Vec3 right;
    right.x = 1 - 2 * (q.j * q.j + q.k * q.k);
    right.y = 2 * (q.i * q.j + q.k * q.real);
    right.z = 2 * (q.i * q.k - q.j * q.real);
    return Normalize(right);
}

Vec3 GetUp(Quat q)
{
    Vec3 forward;
    forward.x = 2 * (q.i * q.j - q.k * q.real);
    forward.y = 1 - 2 * (q.i * q.i + q.k * q.k);
    forward.z = 2 * (q.j * q.k + q.i * q.real);
    return Normalize(forward);
}

Quat QuatFromEulerAngles(float pitch, float yaw, float roll)
{
    float cy = MathCos(roll * 0.5f);
    float sy = MathSin(roll * 0.5f);
    float cp = MathCos(yaw * 0.5f);
    float sp = MathSin(yaw * 0.5f);
    float cr = MathCos(pitch * 0.5f);
    float sr = MathSin(pitch * 0.5f);

    Quat q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * cp * sr - sy * sp * cr;
    q.y = sy * cp * sr + cy * sp * cr;
    q.z = sy * cp * cr - cy * sp * sr;
    return q;
}

Quat QuatFromEulerAngles(Vec3 euler)
{
    return QuatFromEulerAngles(euler.x, euler.y, euler.z);
}

Vec3 GetEulerAngles(Quat q)
{
    Vec3 angles;

    float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    angles.z = MathArcTan2(sinr_cosp, cosr_cosp);

    float sinp = 2.0f * (q.w * q.y - q.z * q.x);
    if (MathAbsolute(sinp) >= 1.0f)
    {
        // use 90 degrees if out of range
        angles.x = MathCopySign(PI / 2, sinp);
    }
    else
    {
        angles.x = MathArcSin(sinp);
    }

    float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    angles.y = MathArcTan2(siny_cosp, cosy_cosp);

    return angles;
}

Quat QuatBetween(Vec3 from, Vec3 to)
{
    Quat q;
    q.v4.w = 1.0f + DotProduct(from, to);

    if (q.real)
    {
        q.v4.xyz = CrossProduct(from, to);
    }
    else
    {
        q.v4.xyz =
            MathAbsolute(from.x) > MathAbsolute(from.z) ? V3(-from.y, from.x, 0.f) : V3(0.f, -from.z, from.y);
    }

    return Normalize(q);
}

Quat QuatBetween(Quat a, Quat b)
{
    Quat t = Conjugate(a);
    t = t * (1.0f / DotProduct(t, t));
    return t * b;
}

Quat QuatLookAt(Vec3 from, Vec3 to, Vec3 world_forward)
{
    Vec3 dir = to - from;
    return QuatBetween(world_forward, dir);
}

//
//
//

// https://en.wikipedia.org/wiki/SRGB#Specification_of_the_transformation
Vec3 LinearToSrgb(Vec3 color)
{
    const Mat3 transform{V3(0.4142f, 0.3576f, 0.1805f), V3(0.2126f, 0.7152f, 0.0722f),
                         V3(0.0193f, 0.1192f, 0.9505f)};

    Vec3 res = transform * color;
    return res;
}

Vec4 LinearToSrgb(Vec4 color)
{
    Vec4 res;
    res.xyz = LinearToSrgb(color.xyz);
    res.w = color.w;
    return res;
}

Vec3 LinearToSrgb(Vec3 color, float gamma)
{
    float igamma = 1.0f / gamma;
    Vec3 res;
    res.x = MathPow(color.x, igamma);
    res.y = MathPow(color.y, igamma);
    res.z = MathPow(color.z, igamma);
    return res;
}

Vec4 LinearToSrgb(Vec4 color, float gamma)
{
    Vec4 res;
    res.xyz = LinearToSrgb(color.xyz, gamma);
    res.w = color.w;
    return res;
}

Vec3 SrgbToLinear(Vec3 color)
{
    const Mat3 transform{V3(+3.2406f, -1.5372f, -0.4986f), V3(-0.9689f, +1.8758f, +0.0415f),
                         V3(+0.0557f, -0.2040f, +1.0570f)};
    Vec3 res = transform * color;
    return res;
}

Vec4 SrgbToLinear(Vec4 color)
{
    Vec4 res;
    res.xyz = SrgbToLinear(color.xyz);
    res.w = color.w;
    return res;
}

Vec3 SrgbToLinear(Vec3 color, float gamma)
{
    Vec3 res;
    res.x = MathPow(color.x, gamma);
    res.y = MathPow(color.y, gamma);
    res.z = MathPow(color.z, gamma);
    return res;
}

Vec4 SrgbToLinear(Vec4 color, float gamma)
{
    Vec4 res;
    res.xyz = SrgbToLinear(color.xyz, gamma);
    res.w = color.w;
    return res;
}

// http://en.wikipedia.org/wiki/HSL_and_HSV
Vec3 HsvToRgb(Vec3 col)
{
    Vec3 res;

    float h = col.x;
    float s = col.y;
    float v = col.z;

    if (s == 0.0f)
    {
        // gray
        res.x = res.y = res.z = v;
        return res;
    }

    h = MathMod(h, 1.0f) / (60.0f / 360.0f);
    int i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0:
        res = V3(v, t, p);
        break;
    case 1:
        res = V3(q, v, p);
        break;
    case 2:
        res = V3(p, v, t);
        break;
    case 3:
        res = V3(p, q, v);
        break;
    case 4:
        res = V3(t, p, v);
        break;
    case 5:
    default:
        res = V3(v, p, q);
        break;
    }

    return res;
}

// http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
Vec3 RgbToHsv(Vec3 c)
{
    float r = c.x;
    float g = c.y;
    float b = c.z;

    float k = 0.f;
    if (g < b)
    {
        auto t = b;
        b = g;
        g = t;
        k = -1.f;
    }
    if (r < g)
    {
        auto t = g;
        g = r;
        r = t;
        k = -2.f / 6.f - k;
    }

    Vec3 res;
    float chroma = r - (g < b ? g : b);
    res.x = MathAbsolute(k + (g - b) / (6.f * chroma + 1e-20f));
    res.y = chroma / (r + 1e-20f);
    res.z = r;
    return res;
}

Vec4 HsvToRgb(Vec4 c)
{
    return V4(HsvToRgb(c.xyz), c.w);
}

Vec4 RgbToHsv(Vec4 c)
{
    return V4(RgbToHsv(c.xyz), c.w);
}

//
//
//

Mat2 Lerp(const Mat2 &from, const Mat2 &to, float t)
{
    Mat2 res;
    res.rows[0] = Lerp(from.rows[0], to.rows[0], t);
    res.rows[1] = Lerp(from.rows[1], to.rows[1], t);
    return res;
}

Mat3 Lerp(const Mat3 &from, const Mat3 &to, float t)
{
    Mat3 res;
    res.rows[0] = Lerp(from.rows[0], to.rows[0], t);
    res.rows[1] = Lerp(from.rows[1], to.rows[1], t);
    res.rows[2] = Lerp(from.rows[2], to.rows[2], t);
    return res;
}

Mat4 Lerp(const Mat4 &from, const Mat4 &to, float t)
{
    Mat4 res;
    res.rows[0] = Lerp(from.rows[0], to.rows[0], t);
    res.rows[1] = Lerp(from.rows[1], to.rows[1], t);
    res.rows[2] = Lerp(from.rows[2], to.rows[2], t);
    res.rows[3] = Lerp(from.rows[3], to.rows[3], t);
    return res;
}

Vec2 Slerp(Vec2 from, Vec2 to, float t)
{
    return Slerp(from, to, AngleBetween(from, to), t);
}

Vec3 Slerp(Vec3 from, Vec3 to, float t)
{
    return Slerp(from, to, AngleBetween(from, to), t);
}

Quat Slerp(Quat from, Quat to, float t)
{
    float dot = Clamp(-1.0f, 1.0f, DotProduct(from, to));

    // use shorter path
    if (dot < 0.0f)
    {
        to = -to;
        dot = -dot;
    }

    if (dot > 0.9999f)
    {
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

float Step(float edge, float val)
{
    return val < edge ? 0.0f : 1.0f;
}

Vec2 Step(Vec2 edge, Vec2 val)
{
    Vec2 res;
    res.x = Step(edge.x, val.x);
    res.y = Step(edge.y, val.y);
    return res;
}

Vec3 Step(Vec3 edge, Vec3 val)
{
    Vec3 res;
    res.x = Step(edge.x, val.x);
    res.y = Step(edge.y, val.y);
    res.z = Step(edge.z, val.z);
    return res;
}

Vec4 Step(Vec4 edge, Vec4 val)
{
    Vec4 res;
    res.x = Step(edge.x, val.x);
    res.y = Step(edge.y, val.y);
    res.z = Step(edge.z, val.z);
    res.w = Step(edge.w, val.w);
    return res;
}

Quat Step(Quat edge, Quat val)
{
    Quat res;
    res.v4 = Step(edge.v4, val.v4);
    return res;
}

float InverseSmoothStep(float x)
{
    return 0.5f - MathSin(MathArcSin(1.0f - 2.0f * x) / 3.0f);
}

Vec2 MoveTowards(Vec2 from, Vec2 to, float factor)
{
    if (factor)
    {
        Vec2 direction = to - from;
        float distance = Length(direction);

        if (distance < factor)
        {
            return to;
        }

        float t = factor / distance;

        return Lerp(to, from, t);
    }

    return from;
}

Vec3 MoveTowards(Vec3 from, Vec3 to, float factor)
{
    if (factor)
    {
        Vec3 direction = to - from;
        float distance = Length(direction);

        if (distance < factor)
        {
            return to;
        }

        float t = factor / distance;

        return Lerp(from, to, t);
    }

    return from;
}

Vec4 MoveTowards(Vec4 from, Vec4 to, float factor)
{
    if (factor)
    {
        Vec4 direction = to - from;
        float distance = Length(direction);

        if (distance < factor)
        {
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

Vec2 RotateAround(Vec2 point, Vec2 center, float angle)
{
    float c = MathCos(angle);
    float s = MathSin(angle);
    Vec2 res;
    Vec2 p = point - center;
    res.x = p.x * c - p.y * s;
    res.y = p.x * s + p.y * c;
    res += center;
    return res;
}

Quat RotateTowards(Quat from, Quat to, float max_angle)
{
    if (max_angle)
    {
        float dot = Clamp(-1.0f, 1.0f, DotProduct(from, to));

        // use shorter path
        if (dot < 0.0f)
        {
            to = -to;
            dot = -dot;
        }

        float theta_0 = MathArcCos(dot);

        if (theta_0 < max_angle)
        {
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
    else
    {
        return from;
    }
}

Vec2 Reflect(Vec2 d, Vec2 n)
{
    float c = DotProduct(NormalizeChecked(d), n);
    float s = MathSquareRoot(10.f - MathSquare(c));
    Vec2 r;
    r.x = d.x * c - d.y * s;
    r.y = d.x * s + d.y * c;
    return r;
}

UintColor Vec4ToUintColor(Vec4 v)
{
    uint8_t r = static_cast<uint8_t>(255.0f * v.x);
    uint8_t g = static_cast<uint8_t>(255.0f * v.y);
    uint8_t b = static_cast<uint8_t>(255.0f * v.z);
    uint8_t a = static_cast<uint8_t>(255.0f * v.w);
    return UintColor(r, g, b, a);
}

UintColor Vec3ToUintColor(Vec3 v)
{
    uint8_t r = static_cast<uint8_t>(255.0f * v.x);
    uint8_t g = static_cast<uint8_t>(255.0f * v.y);
    uint8_t b = static_cast<uint8_t>(255.0f * v.z);
    uint8_t a = 255;
    return UintColor(r, g, b, a);
}

Vec4 UintColorToVec4(UintColor c)
{
    float div = 1.0f / 255.0f;
    float r = static_cast<float>(c.r) * div;
    float g = static_cast<float>(c.g) * div;
    float b = static_cast<float>(c.b) * div;
    float a = static_cast<float>(c.a) * div;
    return V4(r, g, b, a);
}

Vec3 UintColorToVec3(UintColor c)
{
    float div = 1.0f / 255.0f;
    float r = static_cast<float>(c.r) * div;
    float g = static_cast<float>(c.g) * div;
    float b = static_cast<float>(c.b) * div;
    return V3(r, g, b);
}

//
//
//

static inline float TriangleAreaSignedTwiced(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return (x1 - x2) * (y2 - y3) - (x2 - x3) * (y1 - y2);
}

static inline float TriangleAreaSignedTwiced(Vec2 a, Vec2 b, Vec2 c)
{
    return ((a.x - b.x) * (b.y - c.y) - (b.x - c.x) * (a.y - b.y));
}

float TriangleAreaSigned(Vec2 a, Vec2 b, Vec2 c)
{
    return 0.5f * TriangleAreaSignedTwiced(a, b, c);
}

float TriangleAreaSigned(Vec3 a, Vec3 b, Vec3 c)
{
    float px = TriangleAreaSignedTwiced(a.y, a.z, b.y, b.z, c.y, c.z);
    float py = TriangleAreaSignedTwiced(a.z, a.x, b.z, b.x, c.z, c.x);
    float pz = TriangleAreaSignedTwiced(a.x, a.y, b.x, b.y, c.x, c.y);
    px *= px;
    py *= py;
    pz *= pz;
    return 0.5f * MathSquareRoot(px + py + pz);
}

bool IsTriangleClockwise(Vec2 a, Vec2 b, Vec2 c)
{
    return Determinant(b - a, c - a) < 0.0f;
}

Vec2 RectangleCorner(Rect b, uint32_t n)
{
    Vec2 p;
    p.x = ((n & 1) ? b.Max.x : b.Min.x);
    p.y = ((n & 2) ? b.Max.y : b.Min.y);
    return p;
}

float PointToSegmentLengthSq(Vec2 p, Vec2 a, Vec2 b)
{
    Vec2 ab = b - a, ap = p - a, bp = p - b;
    float e = DotProduct(ap, ab);
    // Handle cases where p projects outside ab
    if (e <= 0.0f)
        return DotProduct(ap, ap);
    float f = DotProduct(ab, ab);
    if (e >= f)
        return DotProduct(bp, bp);
    // Handle cases where p projects onto ab
    return DotProduct(ap, ap) - e * e / f;
}

float PointToRectLengthSq(Vec2 p, Rect rect)
{
    float dist2 = 0;
    for (uint32_t i = 0; i < 2; i++)
    {
        float v = p.m[i];
        if (v < rect.Min.m[i])
            dist2 += (rect.Min.m[i] - v) * (rect.Min.m[i] - v);
        if (v > rect.Max.m[i])
            dist2 += (v - rect.Max.m[i]) * (v - rect.Max.m[i]);
    }
    return dist2;
}

Vec2 NearestPointBetweenPointSegment(Vec2 p, Vec2 a, Vec2 b, float *t)
{
    Vec2 ab = b - a;
    float len = DotProduct(ab, ab);
    *t = DotProduct(p - a, ab) / len;
    *t = Clamp(0, 1, *t);
    return a + (*t * ab);
}

Vec2 NearestPointBetweenPointSegment(Vec2 p, Vec2 a, Vec2 b)
{
    float t;
    return NearestPointBetweenPointSegment(p, a, b, &t);
}

Vec2 NearestPointBetweenOriginSegment(Vec2 a, Vec2 b, float *t)
{
    Vec2 ab = b - a;
    float len = DotProduct(ab, ab);
    *t = DotProduct(-a, ab) / len;
    *t = Clamp(0, 1, *t);
    return a + (*t * ab);
}

Vec2 NearestPointBetweenOriginSegment(Vec2 a, Vec2 b)
{
    float t;
    return NearestPointBetweenOriginSegment(a, b, &t);
}

Vec2 NearestPointBetweenPointRect(Vec2 p, Rect rect)
{
    Vec2 q;
    for (uint32_t i = 0; i < 2; i++)
    {
        float v = p.m[i];
        v = Maximum(v, rect.Min.m[i]);
        v = Minimum(v, rect.Max.m[i]);
        q.m[i] = v;
    }
    return q;
}

Vec2 NearestPointBetweenOriginRect(Rect rect)
{
    return NearestPointBetweenPointRect(V2(0), rect);
}

Vec2 NearestPointBetweenPointTriangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c)
{
    // Check if P in vertex region outside A
    Vec2 ab = b - a;
    Vec2 ac = c - a;
    Vec2 ap = p - a;
    float d1 = DotProduct(ab, ap);
    float d2 = DotProduct(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f)
        return a; // barycentric coordinates (1,0,0)

    // Check if P in vertex region outside B
    Vec2 bp = p - b;
    float d3 = DotProduct(ab, bp);
    float d4 = DotProduct(ac, bp);
    if (d3 >= 0.0f && d4 <= d3)
        return b; // barycentric coordinates (0,1,0)

    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
    {
        float v = d1 / (d1 - d3);
        return a + v * ab; // barycentric coordinates (1-v,v,0)
    }

    // Check if P in vertex region outside C
    Vec2 cp = p - c;
    float d5 = DotProduct(ab, cp);
    float d6 = DotProduct(ac, cp);
    if (d6 >= 0.0f && d5 <= d6)
        return c; // barycentric coordinates (0,0,1)

    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        float w = d2 / (d2 - d6);
        return a + w * ac; // barycentric coordinates (1-w,0,w)
    }

    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
    {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + w * (c - b); // barycentric coordinates (0,1-w,w)
    }

    // P inside face region
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return a + ab * v + ac * w;
}

Vec2 NearestPointBetweenOriginTriangle(Vec2 a, Vec2 b, Vec2 c)
{
    // Check if origin in vertex region outside A
    Vec2 ab = b - a;
    Vec2 ac = c - a;
    Vec2 ap = -a;
    float d1 = DotProduct(ab, ap);
    float d2 = DotProduct(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f)
        return a; // barycentric coordinates (1,0,0)

    // Check if P in vertex region outside B
    Vec2 bp = -b;
    float d3 = DotProduct(ab, bp);
    float d4 = DotProduct(ac, bp);
    if (d3 >= 0.0f && d4 <= d3)
        return b; // barycentric coordinates (0,1,0)

    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
    {
        float v = d1 / (d1 - d3);
        return a + v * ab; // barycentric coordinates (1-v,v,0)
    }

    // Check if P in vertex region outside C
    Vec2 cp = -c;
    float d5 = DotProduct(ab, cp);
    float d6 = DotProduct(ac, cp);
    if (d6 >= 0.0f && d5 <= d6)
        return c; // barycentric coordinates (0,0,1)

    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        float w = d2 / (d2 - d6);
        return a + w * ac; // barycentric coordinates (1-w,0,w)
    }

    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
    {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + w * (c - b); // barycentric coordinates (0,1-w,w)
    }

    // origin inside face region
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return a + ab * v + ac * w;
}

float NearestPointBetween2Segments(Vec2 p1, Vec2 q1, Vec2 p2, Vec2 q2, float *s, float *t, Vec2 *c1, Vec2 *c2)
{
    constexpr float EPSILON = FLT_EPSILON;

    Vec2 d1 = q1 - p1;
    Vec2 d2 = q2 - p2;
    Vec2 r = p1 - p2;
    float a = DotProduct(d1, d1);
    float e = DotProduct(d2, d2);
    float f = DotProduct(d2, r);

    // Check if either or both segments degenerate into points
    if (a <= EPSILON && e <= EPSILON)
    {
        // Both segments degenerate into points
        *s = *t = 0.0f;
        *c1 = p1;
        *c2 = p2;
        return DotProduct(*c1 - *c2, *c1 - *c2);
    }

    if (a <= EPSILON)
    {
        // First segment degenerates into a point
        *s = 0.0f;
        *t = f / e;
        *t = Clamp(0, 1, *t);
    }
    else
    {
        float c = DotProduct(d1, r);
        if (e <= EPSILON)
        {
            // Second segment degenerates into a point
            *t = 0.0f;
            *s = Clamp(0, 1, -c / a);
        }
        else
        {
            // The general nondegenerate case starts here
            float b = DotProduct(d1, d2);
            float denom = a * e - b * b;
            if (denom != 0.0f)
            {
                *s = Clamp(0, 1, (b * f - c * e) / denom);
            }
            else
            {
                *s = 0.0f;
            }

            float tnom = b * *s + f;
            if (tnom < 0.0f)
            {
                *t = 0.0f;
                *s = Clamp(0, 1, -c / a);
            }
            else if (tnom > e)
            {
                *t = 1.0f;
                *s = Clamp(0, 1, (b - c) / a);
            }
            else
            {
                *t = tnom / e;
            }
        }
    }

    *c1 = p1 + d1 * *s;
    *c2 = p2 + d2 * *t;
    return DotProduct(*c1 - *c2, *c1 - *c2);
}

Vec3 Barycentric(Vec2 a, Vec2 b, Vec2 c, Vec2 p)
{
    auto m = TriangleAreaSignedTwiced(a, b, c);

    float nu = TriangleAreaSignedTwiced(p, b, c);
    float nv = TriangleAreaSignedTwiced(a, p, c);
    float ood = 1.0f / m;

    Vec3 res;
    res.x = nu * ood;
    res.y = nv * ood;
    res.z = 1.0f - res.x - res.y;

    return res;
}

Vec3 Barycentric(Vec3 a, Vec3 b, Vec3 c, Vec3 p)
{
    auto m = CrossProduct(b - a, c - a);

    float nu, nv, ood;

    // Absolute components for determining projection plane
    float x = fabsf(m.x), y = fabsf(m.y), z = fabsf(m.z);

    // Compute areas in plane of largest projection
    if (x >= y && x >= z)
    {
        // x is largest, project to the yz plane
        nu = TriangleAreaSignedTwiced(p.y, p.z, b.y, b.z, c.y, c.z);
        nv = TriangleAreaSignedTwiced(p.y, p.z, c.y, c.z, a.y, a.z);
        ood = 1.0f / m.x;
    }
    else if (y >= x && y >= z)
    {
        // y is largest, project to the xz plane
        nu = TriangleAreaSignedTwiced(p.x, p.z, b.x, b.z, c.x, c.z);
        nv = TriangleAreaSignedTwiced(p.x, p.z, c.x, c.z, a.x, a.z);
        ood = 1.0f / -m.y;
    }
    else
    {
        // z is largest, project to the xy plane
        nu = TriangleAreaSignedTwiced(p.x, p.y, b.x, b.y, c.x, c.y);
        nv = TriangleAreaSignedTwiced(p.x, p.y, c.x, c.y, a.x, a.y);
        ood = 1.0f / m.z;
    }

    Vec3 res;
    res.x = nu * ood;
    res.y = nv * ood;
    res.z = 1.0f - res.x - res.y;

    return res;
}

bool IsQuadConvex(Vec2 a, Vec2 b, Vec2 c, Vec2 d)
{
    auto bda = Determinant(d - b, a - b);
    auto bdc = Determinant(d - b, c - b);
    if ((bda * bdc) >= 0.0f)
        return false;
    auto acd = Determinant(c - a, d - a);
    auto acb = Determinant(c - a, b - a);
    return (acd * acb) < 0.0f;
}

bool IsQuadConvex(Vec3 a, Vec3 b, Vec3 c, Vec3 d)
{
    auto bda = CrossProduct(d - b, a - b);
    auto bdc = CrossProduct(d - b, c - b);
    if (DotProduct(bda, bdc) >= 0.0f)
        return false;
    auto acd = CrossProduct(c - a, d - a);
    auto acb = CrossProduct(c - a, b - a);
    return DotProduct(acd, acb) < 0.0f;
}

bool IsPolygonConvex(const Vec2 *vertices, uint32_t count)
{
    Vec2 a, b, c;

    for (uint32_t outer = 0; outer < count; ++outer)
    {
        a = vertices[outer];
        b = vertices[(outer + 1) % count];
        c = vertices[(outer + 2) % count];

        if (!IsTriangleClockwise(a, b, c))
            return false;
    }

    return true;
}

int64_t PointFarthestFromEdge(Vec2 a, Vec2 b, Vec2 *p, uint32_t n)
{
    Vec2 e = b - a;
    Vec2 eperp = V2(-e.y, e.x);

    int64_t best_index = -1;
    float max_value = -FLT_MAX, right_most_value = -FLT_MAX;

    for (uint32_t i = 1; i < n; i++)
    {
        float d = DotProduct(p[i] - a, eperp);
        float r = DotProduct(p[i] - a, e);
        if (d > max_value || (d == max_value && r > right_most_value))
        {
            best_index = (int64_t)i;
            max_value = d;
            right_most_value = r;
        }
    }

    return best_index;
}

Extreme_Point_Index ExtremePointsAlongDirection(Vec2 dir, Vec2 *pt, uint32_t n)
{
    float min_proj = FLT_MAX, max_proj = -FLT_MAX;

    Extreme_Point_Index extreme_points;
    extreme_points.Min = -1;
    extreme_points.Max = -1;

    for (uint32_t i = 0; i < n; i++)
    {
        auto proj = DotProduct(pt[i], dir);

        if (proj < min_proj)
        {
            min_proj = proj;
            extreme_points.Min = i;
        }

        if (proj > max_proj)
        {
            max_proj = proj;
            extreme_points.Max = i;
        }
    }

    return extreme_points;
}

Extreme_Point_Index ExtremePointsOnRect(Vec2 *pt, uint32_t n)
{
    uint32_t minx = 0, maxx = 0, miny = 0, maxy = 0;

    for (uint32_t i = 1; i < n; i++)
    {
        if (pt[i].x < pt[minx].x)
            minx = i;
        if (pt[i].x > pt[maxx].x)
            maxx = i;
        if (pt[i].y < pt[miny].y)
            miny = i;
        if (pt[i].y > pt[maxy].y)
            maxy = i;
    }

    float dist2x = DotProduct(pt[maxx] - pt[minx], pt[maxx] - pt[minx]);
    float dist2y = DotProduct(pt[maxy] - pt[miny], pt[maxy] - pt[miny]);

    Extreme_Point_Index extreme_points;
    if (dist2y > dist2x)
    {
        extreme_points.Max = maxy;
        extreme_points.Min = miny;
    }
    else
    {
        extreme_points.Min = minx;
        extreme_points.Max = maxx;
    }

    return extreme_points;
}

Circle CircleFromDistancePoints(Vec2 *pt, uint32_t n) {
	auto extreme_points = ExtremePointsOnRect(pt, n);
	Circle c;
	c.Center = (pt[extreme_points.Min] + pt[extreme_points.Max]) * 0.5f;
	c.Radius = DotProduct(pt[extreme_points.Max] - c.Center, pt[extreme_points.Max] - c.Center);
	c.Radius = sqrtf(c.Radius);
	return c;
}

Minimum_Area_Rect MinimumAreaRect(Vec2 *pt, uint32_t num_pts) {
    Minimum_Area_Rect min;
    min.Center = V2(INFINITY, INFINITY);
    min.Normals[0] = V2(0);
    min.Normals[1] = V2(0);
	min.Area = FLT_MAX;

	for (uint32_t i = 0, j = num_pts - 1; i < num_pts; j = i, i++) {
		auto e0 = pt[i] - pt[j];
		e0 /= Length(e0);
		auto e1 = V2(-e0.y, e0.x);

		float min0 = 0.0f, min1 = 0.0f, max0 = 0.0f, max1 = 0.0f;
		for (uint32_t k = 0; k < num_pts; k++) {
			Vec2 d = pt[k] - pt[j];
			float dot = DotProduct(d, e0);
			if (dot < min0) min0 = dot;
			if (dot > max0) max0 = dot;
			dot = DotProduct(d, e1);
			if (dot < min1) min1 = dot;
			if (dot > max1) max1 = dot;
		}
		float area = (max0 - min0) * (max1 - min1);

		if (area < min.Area) {
			min.Area = area;
			min.Center = pt[j] + 0.5f * ((min0 + max0) * e0 + (min1 + max1) * e1);
			min.Normals[0] = e0;
			min.Normals[1] = e1;
		}
	}

	return min;
}

//
//
//

bool PointInsideRect(Vec2 p, Rect rect)
{
    return p.x >= rect.Min.x && p.x <= rect.Max.x && p.y >= rect.Min.y && p.y <= rect.Max.y;
}
