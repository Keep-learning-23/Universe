#include "../include/model.hpp"

Model::Model(const char* model) {
    modelLoader.load(model);
}

Model::~Model() {

}

std::vector<float> Model::getPositions() {
    std::vector<float> positions;
    for(int i = 0;i < modelLoader.f.size();++i) {
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].x)-1].x);
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].x)-1].y);
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].x)-1].z);
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].y)-1].x);
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].y)-1].y);
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].y)-1].z);
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].z)-1].x);
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].z)-1].y);
        positions.push_back(modelLoader.v[(modelLoader.f[i][0].z)-1].z);
    }
    return positions;
}

std::vector<float> Model::getCoords() {
    std::vector<float> coords;
    for(int i = 0;i < modelLoader.f.size();++i) {
        coords.push_back(modelLoader.vt[(modelLoader.f[i][1].x)-1].s);
        coords.push_back(modelLoader.vt[(modelLoader.f[i][1].x)-1].t);
        coords.push_back(modelLoader.vt[(modelLoader.f[i][1].y)-1].s);
        coords.push_back(modelLoader.vt[(modelLoader.f[i][1].y)-1].t);
        coords.push_back(modelLoader.vt[(modelLoader.f[i][1].z)-1].s);
        coords.push_back(modelLoader.vt[(modelLoader.f[i][1].z)-1].t);
    }
    return coords;
}

std::vector<float> Model::getNormals() {
    std::vector<float> normals;
    for(int i = 0;i < modelLoader.f.size();++i) {
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].x)-1].x);
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].x)-1].y);
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].x)-1].z);
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].y)-1].x);
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].y)-1].y);
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].y)-1].z);
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].z)-1].x);
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].z)-1].y);
        normals.push_back(modelLoader.vn[(modelLoader.f[i][2].z)-1].z);
    }
    return normals;
}

int Model::getPositionsSize() {
    return modelLoader.f.size()*3;
}

ModelLoader::ModelLoader() {

}

ModelLoader::~ModelLoader() {
    
}

void ModelLoader::load(const char* model) {
    model_file = model;
    v.clear();
    vt.clear();
    vn.clear();
    f.clear();
    prase_model();
}

void ModelLoader::prase_model() {
    std::ifstream ifs(model_file);
    std::string line;
    std::string flag;
    float x, y, z;
    float s, t;
    std::string s1, s2, s3;
    std::vector<int> vec1, vec2, vec3;
    while (std::getline(ifs, line))
    {
        if(line.empty() || line[0] == '#') {
            continue;
        }
        std::stringstream ss(line);
        if(ss >> flag) {
            if(flag == "v") {
                /* 这一行表示顶点坐标 */
                if(ss >> x >> y >> z) {
                    v.push_back(glm::vec3(x, y, z));
                }else{
                    throw std::exception();
                }
            }else if(flag == "vt") {
                /* 这一行表示纹理坐标 */
                if(ss >> s >> t) {
                    vt.push_back(glm::vec2(s, t));
                }else{
                    throw std::exception();
                }
            }else if(flag == "vn") {
                if(ss >> x >> y >> z) {
                    vn.push_back(glm::vec3(x, y, z));
                }else{
                    throw std::exception();
                }
            }else if(flag == "f") {
                if(ss >> s1 >> s2 >> s3) {
                    vec1 = prase(s1);
                    vec2 = prase(s2);
                    vec3 = prase(s3);
                    f.push_back({glm::vec3(vec1[0], vec2[0], vec3[0]), 
                                 glm::vec3(vec1[1], vec2[1], vec3[1]), 
                                 glm::vec3(vec1[2], vec2[2], vec3[2])});
                }else{
                    throw std::exception();
                }
            }else{

            }
        }else{
            continue;
        }
    }
}

std::vector<int> ModelLoader::prase(const std::string& s) {
    std::vector<int> vec;
    int index = 0;
    for(int i = 0;i < s.size();++i) {
        if(s[i] == '/') {
            vec.push_back(index);
            index = 0;
        }else{
            index = index*10+s[i]-'0';
            if(i == s.size()-1) {
                vec.push_back(index);
            }
        }
    }
    return vec;
}