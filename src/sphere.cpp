#include "../include/sphere.hpp"

Sphere::Sphere() : spec(DEFAULTSPEC) {
    init(DEFAULTSPEC);
}

Sphere::Sphere(int spec) : spec(spec) {
    init(spec);
}

Sphere::~Sphere() {

}

std::vector<float> Sphere::getPositions() {
    std::vector<float> positions;
    for(int i = 0;i < spec;++i) {
        for(int j = 0;j < spec;++j) {
            /* 对于每一个小块，要把它分成两个三角形（左下和右上两个三角形） */
            /* 左下三角形的三个顶点 */
            // (i, j)
            positions.push_back(points[i][j].x);
            positions.push_back(points[i][j].y);
            positions.push_back(points[i][j].z);
            // (i+1, j)
            positions.push_back(points[i+1][j].x);
            positions.push_back(points[i+1][j].y);
            positions.push_back(points[i+1][j].z);
            // (i, j+1)
            positions.push_back(points[i][j+1].x);
            positions.push_back(points[i][j+1].y);
            positions.push_back(points[i][j+1].z);
            /* 右上三角形的三个顶点 */
            // (i+1, j)
            positions.push_back(points[i+1][j].x);
            positions.push_back(points[i+1][j].y);
            positions.push_back(points[i+1][j].z);
            // (i+1, j+1)
            positions.push_back(points[i+1][j+1].x);
            positions.push_back(points[i+1][j+1].y);
            positions.push_back(points[i+1][j+1].z);
            // (i, j+1)
            positions.push_back(points[i][j+1].x);
            positions.push_back(points[i][j+1].y);
            positions.push_back(points[i][j+1].z);
        }
    }
    return positions;
}

std::vector<float> Sphere::getCoords() {
    std::vector<float> coords;
    /* 和positions是对应的 */
    for(int i = 0;i < spec;++i) {
        for(int j = 0;j < spec;++j) {
            /* 对于每一个小块，要把它分成两个三角形（左下和右上两个三角形） */
            /* 左下三角形的三个顶点 */
            // (i, j)
            coords.push_back(float(i)/float(spec));
            coords.push_back(float(j)/float(spec));
            // (i+1, j)
            coords.push_back(float(i+1)/float(spec));
            coords.push_back(float(j)/float(spec));
            // (i, j+1)
            coords.push_back(float(i)/float(spec));
            coords.push_back(float(j+1)/float(spec));
            /* 右上三角形的三个顶点 */
            // (i+1, j)
            coords.push_back(float(i+1)/float(spec));
            coords.push_back(float(j)/float(spec));
            // (i+1, j+1)
            coords.push_back(float(i+1)/float(spec));
            coords.push_back(float(j+1)/float(spec));
            // (i, j+1)
            coords.push_back(float(i)/float(spec));
            coords.push_back(float(j+1)/float(spec));
        }
    }
    return coords;
}

std::vector<float> Sphere::getNormals() {
    /* 对于球心在原点的球来说，法向量就是点的坐标 */
    std::vector<float> normals;
    for(int i = 0;i < spec;++i) {
        for(int j = 0;j < spec;++j) {
            /* 对于每一个小块，要把它分成两个三角形（左下和右上两个三角形） */
            /* 左下三角形的三个顶点 */
            // (i, j)
            normals.push_back(points[i][j].x);
            normals.push_back(points[i][j].y);
            normals.push_back(points[i][j].z);
            // (i+1, j)
            normals.push_back(points[i+1][j].x);
            normals.push_back(points[i+1][j].y);
            normals.push_back(points[i+1][j].z);
            // (i, j+1)
            normals.push_back(points[i][j+1].x);
            normals.push_back(points[i][j+1].y);
            normals.push_back(points[i][j+1].z);
            /* 右上三角形的三个顶点 */
            // (i+1, j)
            normals.push_back(points[i+1][j].x);
            normals.push_back(points[i+1][j].y);
            normals.push_back(points[i+1][j].z);
            // (i+1, j+1)
            normals.push_back(points[i+1][j+1].x);
            normals.push_back(points[i+1][j+1].y);
            normals.push_back(points[i+1][j+1].z);
            // (i, j+1)
            normals.push_back(points[i][j+1].x);
            normals.push_back(points[i][j+1].y);
            normals.push_back(points[i][j+1].z);
        }
    }
    return normals;
}

void Sphere::init(int num) {
    points = std::vector<std::vector<glm::vec3>>(spec+1, std::vector<glm::vec3>(spec+1));
    float x, y, z;
    /* 从下往上开始分片 */
    float alpha = PI, theta;
    for(int i = 0;i <= spec;++i) {
        theta = 0;
        for(int j = 0;j <= spec;++j) {
            /* 利用极坐标计算出x,y,z */
            z = cos(alpha);
            x = sin(alpha)*cos(theta);
            y = sin(alpha)*sin(theta);
            /* 因为opengl采用的是右手系，将y和z坐标互换 */
            points[i][j] = glm::vec3(x, z, y);
            theta += 2*PI/float(spec);
        }
        alpha -= PI/float(spec);
    }
}

int Sphere::getPositionsSize() {
    return spec * spec * 6;
}