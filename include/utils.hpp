#ifndef UTILS_HPP
#define UTILS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <exception>
#include <fstream>
#include <iostream>

namespace utils {
    void printShaderLog(GLuint shader);

    void printProgramLog(GLuint program);

    bool checkOpenGLError();

    GLuint createShaderProgram(const char* vShaderFile, const char* fShaderFile);

    std::string readShaderSource(const char* filename);

    GLuint loadTexture(const char* filename);

    // 黄金材质
    float* goldAmbient();

    float* goldDiffuse();

    float* goldSpecular();

    float goldShininess();

    // 白银材质
    float* silverAmbient();

    float* silverDiffuse();

    float* silverSpecular();

    float silverShininess();

    // 铜材质
    float* bronzeAmbient();

    float* bronzeDiffuse();

    float* bronzeSpecular();

    float bronzeShininess();

    // 立方体
    std::vector<float> cubePositions();
    std::vector<float> cubeCoords();
}

#endif