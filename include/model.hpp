#ifndef MODEL_HPP
#define MODEL_HPP
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <glm/glm.hpp>

class Model;
class ModelLoader;

class ModelLoader {
    friend Model;
public:
    ModelLoader();
    ~ModelLoader();
    void load(const char* model);
private:
    void prase_model();
    std::vector<int> prase(const std::string& s);
    std::string model_file;
    std::vector<glm::vec3> v;
    std::vector<glm::vec2> vt;
    std::vector<glm::vec3> vn;
    std::vector<std::vector<glm::vec3>> f;
};

class Model {
public:
    Model(const char* model);
    ~Model();
    std::vector<float> getPositions();
    std::vector<float> getCoords();
    std::vector<float> getNormals();
    int getPositionsSize();
private:
    ModelLoader modelLoader;
};

#endif