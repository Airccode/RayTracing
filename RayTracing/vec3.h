
#ifndef VEC3_H
#define VEC3_H


#include<cmath>
#include<iostream>
#include "rtweekend.h"
class vec3
{
	//Variables and Methods
public:
	vec3():e{0,0,0}{}
	vec3(double e0,double e1,double e2):e{e0,e1,e2}{}
	double x()const { return e[0]; }
	double y()const { return e[1]; }
	double z()const { return e[2]; }
	vec3 operator-()const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i)const { return e[i]; }
	double& operator[](int i) { return e[i]; }
	vec3& operator+=(const vec3& v);
	vec3& operator*=(const double t);
	vec3& operator/=(const double t);
	double length()const;
	double length_squrared()const;
	static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}
	static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

public:
	double e[3];


};
using point3 = vec3;	//3D point alias
using color = vec3;		//RGB color

//Utility Function
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(double t, const vec3& v) {//不能用*=，因为vec3 &v为const
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}
inline vec3 operator/(const vec3& v, double t) {
	return (1 / t) * v;
}
//dot
inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
inline vec3 normal(const vec3& v) {
	return v / v.length();
}
inline vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
	} while (p.length_squrared() >= 1.0);
	return p;
}
inline vec3 random_unit_vector() {
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	//(r * cosa)^2 + (r * sina)^2 + z^2 = 1
	//更为均匀生成反射光线方向
	return vec3(r * cos(a), r * sin(a), z);
}
inline vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0)
		return in_unit_sphere;
	return -in_unit_sphere;
}
inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = dot(-uv, n);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squrared())) * n;
	return r_out_perp + r_out_parallel;
}
inline vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squrared() >= 1)	continue;
		return p;
	}
}
#endif

