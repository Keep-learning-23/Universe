#include "../include/torus.hpp"

Torus::Torus(float radius, int spec) : spec(spec), radius(radius){
    init(spec);
}

Torus::Torus(float radius) : radius(radius), spec(DEFAULTSPEC){
    init(spec);
}

Torus::~Torus() {

}

void Torus::init(int spec) {
    points = std::vector<std::vector<glm::vec3>>(spec+1, std::vector<glm::vec3>(spec+1));
    normals = std::vector<std::vector<glm::vec3>>(spec+1, std::vector<glm::vec3>(spec+1));
    float x, y, z;
    float rx, ry, rz;
    float alpha = 0.0f, theta = 0.0f;
    for(int i = 0;i <= spec;++i) {
        rx = radius*cos(alpha);
        ry = radius*sin(alpha);
        rz = 0;
        theta = 0.0f;
        for(int j = 0;j <= spec;++j) {
            x = cos(theta)*cos(alpha) + rx;
            y = cos(theta)*sin(alpha) + ry;
            z = sin(theta) + rz;
            points[i][j] = glm::vec3(x, z, y);
            normals[i][j] = glm::vec3(x-rx, y-ry, z-rz);
            theta += 2*PI/spec;
        }
        alpha += 2*PI/spec;
    }
}

std::vector<float> Torus::getPositions() {
    std::vector<float> positions;
    for(int i = 0;i < spec;++i) {
        for(int j = 0;j < spec;++j) {
            // (i, j)
            positions.push_back(points[i][j].x);
            positions.push_back(points[i][j].y);
            positions.push_back(points[i][j].z);
            // (i, j+1)
            positions.push_back(points[i][j+1].x);
            positions.push_back(points[i][j+1].y);
            positions.push_back(points[i][j+1].z);
            // (i+1, j)
            positions.push_back(points[i+1][j].x);
            positions.push_back(points[i+1][j].y);
            positions.push_back(points[i+1][j].z);
            // (i, j+1)
            positions.push_back(points[i][j+1].x);
            positions.push_back(points[i][j+1].y);
            positions.push_back(points[i][j+1].z);
            // (i+1, j+1)
            positions.push_back(points[i+1][j+1].x);
            positions.push_back(points[i+1][j+1].y);
            positions.push_back(points[i+1][j+1].z);
            // (i+1, j)
            positions.push_back(points[i+1][j].x);
            positions.push_back(points[i+1][j].y);
            positions.push_back(points[i+1][j].z);
        }
    }
    return positions;
}

std::vector<float> Torus::getCoords() {
    std::vector<float> coords;
    for(int i = 0;i < spec;++i) {
        for(int j = 0;j < spec;++j) {
            // (i, j)
            coords.push_back(float(i)/float(spec));
            coords.push_back(float(j)/float(spec));
            // (i, j+1)
            coords.push_back(float(i)/float(spec));
            coords.push_back(float(j+1)/float(spec));
            // (i+1, j)
            coords.push_back(float(i+1)/float(spec));
            coords.push_back(float(j)/float(spec));
            // (i, j+1)
            coords.push_back(float(i)/float(spec));
            coords.push_back(float(j+1)/float(spec));
            // (i+1, j+1)
            coords.push_back(float(i+1)/float(spec));
            coords.push_back(float(j+1)/float(spec));
            // (i+1, j)
            coords.push_back(float(i+1)/float(spec));
            coords.push_back(float(j)/float(spec));
        }
    }
    return coords;
}

std::vector<float> Torus::getNormals() {
    std::vector<float> normals;
    for(int i = 0;i < spec;++i) {
        for(int j = 0;j < spec;++j) {
            // (i, j)
            normals.push_back(this->normals[i][j].x);
            normals.push_back(this->normals[i][j].y);
            normals.push_back(this->normals[i][j].z);
            // (i, j+1)
            normals.push_back(this->normals[i][j+1].x);
            normals.push_back(this->normals[i][j+1].y);
            normals.push_back(this->normals[i][j+1].z);
            // (i+1, j)
            normals.push_back(this->normals[i+1][j].x);
            normals.push_back(this->normals[i+1][j].y);
            normals.push_back(this->normals[i+1][j].z);
            // (i, j+1)
            normals.push_back(this->normals[i][j+1].x);
            normals.push_back(this->normals[i][j+1].y);
            normals.push_back(this->normals[i][j+1].z);
            // (i+1, j+1)
            normals.push_back(this->normals[i+1][j+1].x);
            normals.push_back(this->normals[i+1][j+1].y);
            normals.push_back(this->normals[i+1][j+1].z);
            // (i+1, j)
            normals.push_back(this->normals[i+1][j].x);
            normals.push_back(this->normals[i+1][j].y);
            normals.push_back(this->normals[i+1][j].z);
        }
    }
    return normals;
}