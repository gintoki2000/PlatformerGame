#ifndef MATH_H
#define MATH_H
namespace Math 
{
template<typename T>
const T& Max(const T& a, const T& b)
{
	return a > b ? a : b;
}

template<typename T>
const T& Min(const T& a, const T& b){
	return a < b ? a : b;
}

template<typename T>
const T& Clamp(const T& a, const T& b, float x)
{
	return Max(a, Min(b, x));
}
float Lerp(float current, float target, float step);
float Sign(float v);
int Rand(int a, int b);
}


#endif //MATH_H
