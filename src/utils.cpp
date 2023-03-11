#include "../include/utils.hpp"

void utils::printShaderLog(GLuint shader) {
    int len = 0;
    int charWrittn = 0;
    char* log = nullptr;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = (char*)malloc(len+1);
        glGetShaderInfoLog(shader, len, &charWrittn, log);
        log[charWrittn] = '\0';
        printf("Shader Info Log: %s\n", log);
        free(log);
    }
}

void utils::printProgramLog(GLuint program) {
    int len = 0;
    int charWrittn = 0;
    char* log = nullptr;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = (char*)malloc(len+1);
        glGetProgramInfoLog(program, len, &charWrittn, log);
        log[charWrittn] = '\0';
        printf("Program Info Log: %s\n", log);
        free(log);
    }
}

bool utils::checkOpenGLError() {
    bool foundError = false;
    int glError = glGetError();
    while (glError != GL_NO_ERROR)
    {
        printf("glError: %d\n", glError);
        foundError = true;
        glError = glGetError();
    }
    return foundError;
}

GLuint utils::createShaderProgram(const char* vShaderFile, const char* fShaderFile) {
    std::string vshaderSource = readShaderSource(vShaderFile);
    std::string fshaderSource = readShaderSource(fShaderFile);
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vshaderSource_str = vshaderSource.c_str();
    const char* fshaderSource_str = fshaderSource.c_str();
    glShaderSource(vShader, 1, &vshaderSource_str, nullptr);
    glShaderSource(fShader, 1, &fshaderSource_str, nullptr);
    glCompileShader(vShader);
    // 捕获编译着色器时的错误
    bool foundError = checkOpenGLError();
    GLint vertCompile;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompile);
    if(vertCompile != 1) {
        printf("vertex shader compilation failed\n");
        printShaderLog(vShader);
    }
    glCompileShader(fShader);
    // 捕获编译着色器时的错误
    foundError = checkOpenGLError();
    GLint fragCompile;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompile);
    if(fragCompile != 1) {
        printf("fragment shader compilation failed\n");
        printShaderLog(fShader);
    }
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    // 捕获链接着色器时的错误
    foundError = checkOpenGLError();
    GLint linked;
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if(linked != 1) {
        printf("vfProgram linked failed\n");
        printProgramLog(vfProgram);
    }
    return vfProgram;
}

std::string utils::readShaderSource(const char* filename) {
    std::ifstream ifs(filename);
    std::string content;
    std::string line;
    while(std::getline(ifs, line)) {
        content += line;
        content.push_back('\n');
    }
    ifs.close();
    return content;
}

GLuint utils::loadTexture(const char* filename) {
    GLuint textureID;
    textureID = SOIL_load_OGL_texture(filename,
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if(textureID == 0) {
        printf("can't find texture file\n");
        throw std::exception();
    }
    return textureID;
}
// 黄金材质
float* utils::goldAmbient() {
    static float a[4] = {0.24725f, 0.1995f, 0.0745f, 1.0f};
    return a;
}

float* utils::goldDiffuse() {
    static float a[4] = {0.7516f, 0.6065f, 0.2265f, 1.0f};
    return a;
}

float* utils::goldSpecular() {
    static float a[4] = {0.6238f, 0.5559f, 0.3661f, 1.0f};
    return a;
}

float utils::goldShininess() {
    return 51.2f;
}

// 白银材质
float* utils::silverAmbient() {
    static float a[4] = {0.1923f, 0.1923f, 0.1923f, 1.0f};
    return a;
}

float* utils::silverDiffuse() {
    static float a[4] = {0.5075f, 0.5075f, 0.5075f, 1.0f};
    return a;
}

float* utils::silverSpecular() {
    static float a[4] = {0.5083f, 0.5083f, 0.5083f, 1.0f};
    return a;
}

float utils::silverShininess() {
    return 51.2;
}

// 铜材质
float* utils::bronzeAmbient() {
    static float a[4] = {0.2125f, 0.1275f, 0.0540f, 1.0f};
    return a;
}

float* utils::bronzeDiffuse() {
    static float a[4] = {0.7140, 0.4284f, 0.1814f, 1.0f};
    return a;
}

float* utils::bronzeSpecular() {
    static float a[4] = {0.3936, 0.2719f, 0.1667f, 1.0f};
    return a;
}

float utils::bronzeShininess() {
    return 25.6;
}

std::vector<float> utils::cubePositions() {
    std::vector<float> positions = {
        -1.0f,  1.0f,   -1.0f,  -1.0f,  -1.0f,  -1.0f,  1.0f,   -1.0f,  -1.0f,
         1.0f, -1.0f,   -1.0f,   1.0f,   1.0f,  -1.0f, -1.0f,    1.0f,  -1.0f,
         1.0f, -1.0f,   -1.0f,   1.0f,  -1.0f,   1.0f,  1.0f,    1.0f,  -1.0f,
         1.0f, -1.0f,    1.0f,   1.0f,   1.0f,   1.0f,  1.0f,    1.0f,  -1.0f,
         1.0f, -1.0f,    1.0f,  -1.0f,  -1.0f,   1.0f,  1.0f,    1.0f,   1.0f,
        -1.0f, -1.0f,    1.0f,  -1.0f,   1.0f,   1.0f,  1.0f,    1.0f,   1.0f,
        -1.0f, -1.0f,    1.0f,  -1.0f,  -1.0f,  -1.0f, -1.0f,    1.0f,   1.0f,
        -1.0f, -1.0f,   -1.0f,  -1.0f,   1.0f,  -1.0f, -1.0f,    1.0f,   1.0f,
        -1.0f, -1.0f,    1.0f,   1.0f,  -1.0f,   1.0f,  1.0f,   -1.0f,  -1.0f,
         1.0f, -1.0f,   -1.0f,  -1.0f,  -1.0f,  -1.0f, -1.0f,   -1.0f,   1.0f,
        -1.0f,  1.0f,   -1.0f,   1.0f,   1.0f,  -1.0f,  1.0f,    1.0f,   1.0f,
         1.0f,  1.0f,    1.0f,  -1.0f,   1.0f,   1.0f, -1.0f,    1.0f,  -1.0f
    };
    return positions;
}

std::vector<float> utils::cubeCoords() {
    std::vector<float> coords = {
        1.0f, 0.66f, 1.0f, 0.33f, 0.75f, 0.33f,
        0.75f, 0.33f, 0.75f, 0.66f, 1.00f, 0.66f,
        0.75f, 0.33f, 0.50f, 0.33f, 0.75f, 0.66f,
        0.50f, 0.33f, 0.50f, 0.66f, 0.75f, 0.66f,
        0.50f, 0.33f, 0.25f, 0.33f, 0.50f, 0.66f,
        0.25f, 0.33f, 0.25f, 0.66f, 0.50f, 0.66f,
        0.25f, 0.33f, 0.00f, 0.33f, 0.25f, 0.66f,
        0.00f, 0.33f, 0.00f, 0.66f, 0.25f, 0.66f,
        0.25f, 0.33f, 0.50f, 0.33f, 0.50f, 0.00f,
        0.50f, 0.00f, 0.25f, 0.00f, 0.25f, 0.33f,
        0.25f, 1.00f, 0.50f, 1.00f, 0.50f, 0.66f,
        0.50f, 0.66f, 0.25f, 0.66f, 0.25f, 1.00f
    };
    return coords;
}



