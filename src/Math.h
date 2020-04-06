#ifndef MATH_H
#define MATH_H
namespace Math 
{
template<typename T>
const T& max(const T& a, const T& b)
{
	return a > b ? a : b;
}

template<typename T>
const T& min(const T& a, const T& b){
	return a < b ? a : b;
}

template<typename T>
const T& clamp(const T& a, const T& b, float x)
{
	return max(a, min(b, x));
}
float lerp(float current, float target, float step);
float sign(float v);
}
#endif //MATH_H
