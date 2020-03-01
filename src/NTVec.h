#ifndef NT_VEC_H
#define NT_VEC_H
#include <cmath>
struct NTVec
{
    float x, y;
    NTVec() : x(0.0f), y(0.0f) {}

    NTVec(float _x, float _y) : x(_x), y(_y) {}

    float len() const { return std::sqrt(x * x + y * y); }

    NTVec& operator+=(const NTVec& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    NTVec& operator-=(const NTVec& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    NTVec& operator*=(float k)
    {
        x *= k;
        y *= k;
        return *this;
    }

    static const NTVec UP;
    static const NTVec DOWN;
    static const NTVec LEFT;
    static const NTVec RIGHT;
};

inline NTVec operator+(const NTVec& lhs, const NTVec& rhs)
{
    return NTVec(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline NTVec operator-(const NTVec& lhs, const NTVec& rhs)
{
    return NTVec(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline NTVec operator*(const NTVec& lhs, float rhs)
{
    return NTVec(lhs.x * rhs, lhs.y * rhs);
}

inline NTVec operator*(float lhs, const NTVec& rhs)
{
    return NTVec(lhs * rhs.x, lhs * rhs.y);
}

inline NTVec operator/(const NTVec& lhs, float rhs)
{
    return NTVec(lhs.x / rhs, lhs.y / rhs);
}

inline float NTDot(const NTVec& lhs, const NTVec& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

#endif // NT_VEC_H
