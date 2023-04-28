// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <windows.h>
#include"rtweekend.h"
#include"color.h"
#include"hittable_list.h"
#include"sphere.h"
#include"camera.h"
#include"material.h"


hittable_list random_scene() {
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));//ground
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                if (choose_mat < 0.8) {
                    //diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    //metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    //glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5),0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    return world;
}

//linear interpolation
color ray_color(const ray& r,const hittable& world,int depth) {
    hit_record rec;
    if (depth <= 0)
        return color(0, 0, 0);
    if (world.hit(r, 0.001, inf, rec)) {
        ray scattered;
        color attenuation;
        //�������ɢ������뷨�����н�С��90
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        //����90�������߱������գ����غ�ɫ
        return color(0, 0, 0);
        //0.5������ɴֲڳ̶ȣ���������ϵ�����ݹ����Խ�����չ���Խ��
    }
    vec3 dir = normal(r.direction());
    //����y���ӽ��䣬+1.0��Ϊ�˳�ʼt��Ϊ0����ζ�Ų��Ǵ��׽��䵽(0.5,0.7,1.0)
    double t = 0.5 * (dir.y() + 1.0);
    //(0.5,0.7,1.0) to (1.0,1.0,1.0) interpolation
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

//Ŀǰ��Ŀ��Camera��Ϊ���Դ����ray
int main()
{
    remove("image.ppm");
    std::ofstream out("image.ppm");

    //Image
    const double aspect_ratio = 3.0 / 2.0;
    const int image_width = 480;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int sample_per_pixel = 50;
    const int max_depth = 50;
    std::cout << "����" << image_height * image_width << "������" << std::endl;
    //World
    hittable_list world = random_scene();
    //world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    //world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    //auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    //auto material_left = make_shared<dielectric>(1.5);
    //auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    /*
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8),0.3);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2),1.0);
    auto material_up = make_shared<lambertian>(color(0.9375, 0.25390625, 0.33203125));
    */
    //world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0),100.0, material_ground));
    //world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    //world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
    //world.add(make_shared<sphere>(point3(0.0, 1.0, -1.0), 0.5, material_up));
    
    //Camera,Screen space
    point3 lookfrom(12, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
   
    //Render
    int t = 0;
    out << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; j--) {
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            //��[i,j]�����أ�ȡ[i+random(),j+random()]��Ӧ���ߵķ�����Ϊ��ɫ��ȡƽ��
            for (int s = 0; s < sample_per_pixel; s++) {
                double u = double(i + random_double()) / (image_width - 1);
                double v = double(j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world,max_depth);
            }
            write_color(out, pixel_color, sample_per_pixel);
            if(t % 10000 == 0)
                std::cout << "��" << t <<"����" << std::endl;
            t++;
        }
    }

}
//��Ҫ����96w�����أ�ÿ1w�������������һ�Σ�����ʱ��ϳ�
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
