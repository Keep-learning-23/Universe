#ifndef SPHERE_HPP
#define SPHERE_HPP
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#define DEFAULTSPEC 48
#define PI 3.14159

/**
 * @brief 这是一个球类(默认球心为原点，半径为1，可以通过平移和缩放矩阵修改)，存储球上的顶点，法向量以及纹理贴图坐标
 * 
 */

class Sphere {
public:
    Sphere(int spec);
    Sphere();
    ~Sphere();
    std::vector<float> getPositions();          /* 返回球上的顶点坐标 */
    std::vector<float> getCoords();             /* 返回球的纹理坐标 */
    std::vector<float> getNormals();            /* 返回顶点的法向量 */
    int getPositionsSize();
private:
    int spec;                                   /* 球的精度，把球分成spec个分片，每个分片分成spec个小块 */
    std::vector<std::vector<glm::vec3>> points; /* 球上的顶点 */
    void init(int num);
};

#endif