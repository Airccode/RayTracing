#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include"rtweekend.h"
#include"ray.h"
#include"hittable.h"
class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)const = 0;
};
//lamberitian reflect
class lambertian :public material {
public:
	lambertian(const color& a):albedo(a){}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
		vec3 scatter_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
public:
	color albedo;
};
//mirrored light reflection
class metal :public material {
public:
	//metal(const color& a):albedo(a){}
	metal(const color& a,double f):albedo(a),fuzz(f < 1 ? f : 1){}
	//r_in为入射光线, scattered为散射光线, attenuation 意思为衰减量，实际为各通道的反射率

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(normal(r_in.direction()), rec.normal);
		vec3 x = fuzz * random_in_unit_sphere();
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
public:
	color albedo;
	double fuzz;
};

class dielectric :public material {
public:
	dielectric(double ri):ref_idx(ri){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)const override {
		attenuation = color(1.0, 1.0, 1.0);
		//交点在外部rec.front_face == true
		double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;
		vec3 unit_direction = normal(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		if (etai_over_etat * sin_theta > 1.0) {
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}
		double reflect_prob = schlick(cos_theta, etai_over_etat);
		if (random_double() < reflect_prob) {
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.p, reflected);
			return true;
		}
		vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
		scattered = ray(rec.p, refracted);
		return true;
	}
public:
	double ref_idx;
private:
	static double schlick(double cosine, double ref_idx) {
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 *= r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
#endif
