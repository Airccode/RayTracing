#include "vec3.h"
vec3& vec3::operator+=(const vec3& v){
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}
vec3& vec3::operator*=(const double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}
vec3& vec3::operator/=(const double t) {
	return *this *= 1 / t;
}
double vec3::length()const { return sqrt(length_squrared()); }
double vec3::length_squrared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }


