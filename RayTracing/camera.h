#ifndef CAMERA_H
#define CAMERA_H
#include "rtweekend.h"
#include "vec3.h"
class camera {
public:
	camera() {
		auto aspect_ration = 16.0 / 9.0;
		auto viewport_height = 2.0;
		auto viewport_width = aspect_ration * viewport_height;
		auto focal_length = 1.0;

		origin = point3(0, 0, 0);
		horizontal = vec3(viewport_width, 0.0, 0.0);
		vertical = vec3(0.0, viewport_height, 0.0);

		lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
		
	}
	camera(double vfov, double aspect_ratio) {
		auto theta = degTorad(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		auto focal_length = 1.0;
		origin = point3(0, 0, 0);
		horizontal = vec3(viewport_width, 0.0, 0.0);
		vertical = vec3(0.0, viewport_height, 0.0);

		lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
	}
	camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio) {
		auto theta = degTorad(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		auto w = normal(lookfrom - lookat);
		auto u = normal(cross(vup, w));
		auto v = cross(w, u);

		origin = lookfrom;
		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
	}
	camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio,double aperture,double focus_dist) {
		auto theta = degTorad(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		w = normal(lookfrom - lookat);
		u = normal(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
		lens_radius = aperture / 2;
	}
	ray get_ray(double s, double t)const {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}
	
private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens_radius;
};
#endif