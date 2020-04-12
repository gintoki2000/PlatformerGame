#ifndef VEC_H
#define VEC_H
#include <cmath>
struct Vec2
{
    float x, y;
    Vec2() : x(0.0f), y(0.0f) {}

    Vec2(float _x, float _y) : x(_x), y(_y) {}

    float len() const { return std::sqrt(x * x + y * y); }

    Vec2& operator+=(const Vec2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2& operator*=(float k)
    {
        x *= k;
        y *= k;
        return *this;
    }

    static const Vec2 UP;
    static const Vec2 DOWN;
    static const Vec2 LEFT;
    static const Vec2 RIGHT;
};

inline Vec2 operator+(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline Vec2 operator*(const Vec2& lhs, float rhs)
{
    return Vec2(lhs.x * rhs, lhs.y * rhs);
}

inline Vec2 operator*(float lhs, const Vec2& rhs)
{
    return Vec2(lhs * rhs.x, lhs * rhs.y);
}

inline Vec2 operator/(const Vec2& lhs, float rhs)
{
    return Vec2(lhs.x / rhs, lhs.y / rhs);
}

inline float Dot(const Vec2& lhs, const Vec2& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

#endif // VEC_H
