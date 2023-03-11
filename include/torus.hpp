#ifndef TORUS_HPP
#define TORUS_HPP
#include <vector>
#include <glm/glm.hpp>
#define DEFAULTSPEC 48
#define PI 3.14159
/**
 * @brief 默认是一个环心在原点，环的内径为radius（>=1.0），环的外径为1.0的环面
 * 
 */

class Torus {
public:
    Torus(float radius, int spec);
    Torus(float radius);
    ~Torus();
    std::vector<float> getPositions();
    std::vector<float> getCoords();
    std::vector<float> getNormals();
private:
    int spec;
    float radius;
    void init(int spec);
    std::vector<std::vector<glm::vec3>> points;
    std::vector<std::vector<glm::vec3>> normals;
};

#endif