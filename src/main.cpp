#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/utils.hpp"
#include "../include/torus.hpp"
#include "../include/sphere.hpp"
#include "../include/model.hpp"
#include <iostream>
#define numVAOs 1
#define numVBOs 3
#define numModelVOBs 3
#define numSKYVBOs 2

// 创建一个模型
Model myModel = Model("../obj/shuttle.obj");
// 创建一个球体
Sphere mySphere = Sphere();
// 渲染程序
GLuint renderingProgram;
// 纹理贴图
GLuint sun_texture, earth_texture, moon_texture, model_texture, sky_texture;
// 
GLuint mv_Loc, proj_Loc;
// 
glm::mat4 mMat, vMat, mvMat, pMat, rMat, sMat, sunMat, earthMat, moonMat, modelMat;
// 顶点缓冲区
GLuint vao[numVAOs], vbo[numVBOs], model_vbo[numModelVOBs], sky_vbo[numSKYVBOs];
// 相机位置，太阳位置
float sun_Loc[3], camera_Loc[3];
// 观察窗口的宽、高以及宽高比
float aspect;
int width, height;
float Znear = 0.1f, Zfar = 1000.0f; 
glm::vec3 U(1.0f, 0.0f, 0.0f), V(0.0f, 1.0f, 0.0f), N(0.0f, 0.0f, 1.0f);
//
bool w_flag = false, a_flag = false, s_flag = false, d_flag = false; 

void init(GLFWwindow* window);

void display(GLFWwindow* window, double currentTime);

void scroll(GLFWwindow* window, double xoffset, double yoffset);

void pressKey(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char* argv[]) {
    if(!glfwInit()) {
        std::cout << "glfw init failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GL_MAJOR_VERSION, 4);
    glfwWindowHint(GL_MINOR_VERSION, 3);
    GLFWwindow* window = glfwCreateWindow(1400, 1400, "Universe", nullptr, nullptr);
    if(!window) {
        std::cout << "window init failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if(glewInit() != GLEW_OK) {
        std::cout << "glew init failure" << std::endl;
        exit(EXIT_FAILURE);
    }

    init(window);

    while (!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_FAILURE);
}

void init(GLFWwindow* window) {
    glfwSetKeyCallback(window, pressKey);
    glfwSetScrollCallback(window, scroll);
    const char* vShader = "../glsl/vShader.glsl";
    const char* fShader = "../glsl/fShader.glsl";
    renderingProgram = utils::createShaderProgram(vShader, fShader);
    const char* sun = "../jpg/sun.jpg";
    const char* earth = "../jpg/earth.jpg";
    const char* moon = "../jpg/moon.jpg";
    const char* shuttle = "../jpg/shuttle.jpg";
    const char* sky = "../jpg/sky.jpg";
    sun_texture = utils::loadTexture(sun);
    earth_texture = utils::loadTexture(earth);
    moon_texture = utils::loadTexture(moon);
    model_texture = utils::loadTexture(shuttle);
    sky_texture = utils::loadTexture(sky);
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);
    glGenBuffers(numModelVOBs, model_vbo);

    std::vector<float> positions = mySphere.getPositions();
    std::vector<float> coords = mySphere.getCoords();
    std::vector<float> normals = mySphere.getNormals();
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(float), &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, coords.size()*sizeof(float), &coords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float), &normals[0], GL_STATIC_DRAW);
    
    std::vector<float> model_positions = myModel.getPositions();
    std::vector<float> model_coords = myModel.getCoords();
    std::vector<float> model_normals = myModel.getNormals();
    glBindBuffer(GL_ARRAY_BUFFER, model_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, model_positions.size()*sizeof(float), &model_positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, model_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, model_coords.size()*sizeof(float), &model_coords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, model_vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, model_normals.size()*sizeof(float), &model_normals[0], GL_STATIC_DRAW);
    std::vector<float> cubePositions = utils::cubePositions();
    std::vector<float> cubeCoords = utils::cubeCoords();
    
    glGenBuffers(numSKYVBOs, sky_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sky_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, cubePositions.size()*sizeof(float), &cubePositions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, sky_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, cubeCoords.size()*sizeof(float), &cubeCoords[0], GL_STATIC_DRAW);
    sun_Loc[0] = 0.0f, sun_Loc[1] = 0.0f, sun_Loc[2] = 0.0f;
    camera_Loc[0] = 0.0f, camera_Loc[1] = 0.0f, camera_Loc[2] = 12.0f;
}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(renderingProgram);

    if(w_flag) {
        U = glm::vec3(glm::rotate(glm::mat4(1.0), -0.01f, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(U, 1.0));
        V = glm::vec3(glm::rotate(glm::mat4(1.0), -0.01f, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(V, 1.0));
        N = glm::vec3(glm::rotate(glm::mat4(1.0), -0.01f, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(N, 1.0));
    }
    if(s_flag) {
        U = glm::vec3(glm::rotate(glm::mat4(1.0), 0.01f, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(U, 1.0));
        V = glm::vec3(glm::rotate(glm::mat4(1.0), 0.01f, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(V, 1.0));
        N = glm::vec3(glm::rotate(glm::mat4(1.0), 0.01f, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(N, 1.0));
    }
    if(a_flag) {
        U = glm::vec3(glm::rotate(glm::mat4(1.0), -0.01f, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(U, 1.0));
        V = glm::vec3(glm::rotate(glm::mat4(1.0), -0.01f, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(V, 1.0));
        N = glm::vec3(glm::rotate(glm::mat4(1.0), -0.01f, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(N, 1.0));
    }
    if(d_flag) {
        U = glm::vec3(glm::rotate(glm::mat4(1.0), 0.01f, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(U, 1.0));
        V = glm::vec3(glm::rotate(glm::mat4(1.0), 0.01f, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(V, 1.0));
        N = glm::vec3(glm::rotate(glm::mat4(1.0), 0.01f, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(N, 1.0));
    }

    glfwGetFramebufferSize(window, &width, &height);
    aspect = float(width) / float(height);
    pMat = glm::perspective(1.0472f, aspect, Znear, Zfar);

    rMat = glm::mat4(glm::vec4(U, 0.0f), glm::vec4(V, 0.0f), glm::vec4(N, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    
    vMat = rMat*glm::translate(glm::mat4(1.0f), glm::vec3(-camera_Loc[0], -camera_Loc[1], -camera_Loc[2]));
    /* --------------------------------------------------------------------- */
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(camera_Loc[0], camera_Loc[1], camera_Loc[2]));
    mvMat = vMat*mMat;
    mv_Loc = mv_Loc = glGetUniformLocation(renderingProgram, "mv_matrix");
    proj_Loc = glGetUniformLocation(renderingProgram, "proj_matrix");
    glUniformMatrix4fv(mv_Loc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(proj_Loc, 1, GL_FALSE, glm::value_ptr(pMat));

    glBindBuffer(GL_ARRAY_BUFFER, sky_vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, sky_vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sky_texture);

    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_DEPTH_TEST);
    /* --------------------------------------------------------------------- */
    sunMat = glm::translate(glm::mat4(1.0f), glm::vec3(sun_Loc[0], sun_Loc[1], sun_Loc[2]));
    rMat = glm::rotate(glm::mat4(1.0f), float(currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
    sMat = glm::scale(glm::mat4(1.0f), glm::vec3(3.0, 3.0, 3.0));
    mvMat = vMat * sunMat * rMat * sMat;
    mv_Loc = glGetUniformLocation(renderingProgram, "mv_matrix");
    proj_Loc = glGetUniformLocation(renderingProgram, "proj_matrix");
    glUniformMatrix4fv(mv_Loc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(proj_Loc, 1, GL_FALSE, glm::value_ptr(pMat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sun_texture);

    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getPositionsSize());
    /* --------------------------------------------------------------------- */
    earthMat = sunMat* glm::translate(glm::mat4(1.0f), glm::vec3(cos(currentTime)*5.0, 0.0f, sin(currentTime)*5.0));
    rMat = glm::rotate(glm::mat4(1.0f), float(currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
    sMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0));
    mvMat = vMat * earthMat * rMat * sMat;
    glfwGetFramebufferSize(window, &width, &height);
    aspect = float(width) / float(height);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    mv_Loc = glGetUniformLocation(renderingProgram, "mv_matrix");
    proj_Loc = glGetUniformLocation(renderingProgram, "proj_matrix");
    glUniformMatrix4fv(mv_Loc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(proj_Loc, 1, GL_FALSE, glm::value_ptr(pMat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earth_texture);

    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getPositionsSize());
    /* --------------------------------------------------------------------- */
    moonMat = earthMat * glm::translate(glm::mat4(1.0f), glm::vec3(cos(currentTime)*2.0, sin(currentTime)*2.0, 0.0));
    rMat = glm::rotate(glm::mat4(1.0f), float(currentTime), glm::vec3(0.0f, 1.0f, 0.0f));
    sMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5));
    mvMat = vMat * moonMat * rMat * sMat;
    glfwGetFramebufferSize(window, &width, &height);
    aspect = float(width) / float(height);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    mv_Loc = glGetUniformLocation(renderingProgram, "mv_matrix");
    proj_Loc = glGetUniformLocation(renderingProgram, "proj_matrix");
    glUniformMatrix4fv(mv_Loc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(proj_Loc, 1, GL_FALSE, glm::value_ptr(pMat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moon_texture);

    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getPositionsSize());
    /* -------------------------------------------------------------------- */
    modelMat = sunMat * glm::translate(glm::mat4(1.0f), glm::vec3(cos(currentTime+0.25)*8.0, 0.0f, sin(currentTime+0.25)*8.0));
    rMat = glm::rotate(glm::mat4(1.0f), float(-currentTime+PI/2), glm::vec3(0.0f, 1.0f, 0.0f));
    sMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5));
    mvMat = vMat * modelMat * rMat * sMat;
    glfwGetFramebufferSize(window, &width, &height);
    aspect = float(width) / float(height);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    mv_Loc = glGetUniformLocation(renderingProgram, "mv_matrix");
    proj_Loc = glGetUniformLocation(renderingProgram, "proj_matrix");
    glUniformMatrix4fv(mv_Loc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(proj_Loc, 1, GL_FALSE, glm::value_ptr(pMat));

    glBindBuffer(GL_ARRAY_BUFFER, model_vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, model_vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, model_vbo[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model_texture);

    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, myModel.getPositionsSize());
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
    /* 鼠标滚动滚轮事件 */
    if(yoffset == 1) {
        camera_Loc[2]--;
    }else if(yoffset == -1) {
        camera_Loc[2]++;
    }
}

void pressKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == 'W' || key == 'w') {
        if(action == GLFW_PRESS) {
            w_flag = true;
        }else if(action == GLFW_RELEASE) {
            w_flag = false;
        }
    }else if(key == 'A' || key == 'a') {
        if(action == GLFW_PRESS) {
            a_flag = true;
        }else if(action == GLFW_RELEASE) {
            a_flag = false;
        }
    }else if(key == 'S' || key == 's') {
        if(action == GLFW_PRESS) {
            s_flag = true;
        }else if(action == GLFW_RELEASE) {
            s_flag = false;
        }
    }else if(key == 'D' || key == 'd') {
        if(action == GLFW_PRESS) {
            d_flag = true;
        }else if(action == GLFW_RELEASE) {
            d_flag = false;
        }
    }
}