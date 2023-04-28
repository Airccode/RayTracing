#ifndef COLOR_H
#define COLOR_H
#include"vec3.h"
#include<iostream>
#include "rtweekend.h"

using color = vec3;		//RGB color

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(r * scale);
	g = sqrt(g * scale);
	b = sqrt(b * scale);
	//减小RGB的值，抬高亮度
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
//全局函数应该放在cpp，否则会让多cpp文件引用这个头文件导致多次定义函数.也可以用内联
#endif

