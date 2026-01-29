#include "Model.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Helper struct to keep track of indices temporarily
struct VertexIndex {
    int vIdx = -1;
    int tIdx = -1;
    int nIdx = -1;
};

Model::Model(std::string filename, Vec3<float> pos, bool gravity, Color3f color, TextureID texture, MaterialID material)
    : Object(pos, gravity, color, texture, material) {
    this->modelPath = filename;
}

void Model::drawInternal() {
    loadAndCompile(modelPath);
}

void Model::loadAndCompile(const std::string& filename) {
    std::cout << "[Model] Loading: " << filename << "... ";

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "FAILED! File not found." << std::endl;
        return;
    }
    std::cout << "Success!" << std::endl;

    // Temporary storage for data
    std::vector<Vec3<float>> temp_positions;
    std::vector<Vec3<float>> temp_texCoords;
    std::vector<Vec3<float>> temp_normals;

    // Apply color settings (Texture/Material are handled by the Object wrapper)
    glColor3f(color.red, color.green, color.blue);

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            float x, y, z; ss >> x >> y >> z;
            temp_positions.push_back(Vec3<float>(x, y, z));
        }
        else if (prefix == "vt") {
            float u, v; ss >> u >> v;
            temp_texCoords.push_back(Vec3<float>(u, v, 0));
        }
        else if (prefix == "vn") {
            float x, y, z; ss >> x >> y >> z;
            temp_normals.push_back(Vec3<float>(x, y, z));
        }
        else if (prefix == "f") {
            std::vector<VertexIndex> faceVerts;
            std::string segment;
            
            while (ss >> segment) {
                VertexIndex idx;
                std::stringstream segmentSS(segment);
                std::string valStr;

                // Position Index
                std::getline(segmentSS, valStr, '/');
                if (!valStr.empty()) idx.vIdx = std::stoi(valStr) - 1;

                // Texture Index
                std::getline(segmentSS, valStr, '/');
                if (!valStr.empty()) idx.tIdx = std::stoi(valStr) - 1;

                // Normal Index
                std::getline(segmentSS, valStr, '/');
                if (!valStr.empty()) idx.nIdx = std::stoi(valStr) - 1;

                faceVerts.push_back(idx);
            }

            glBegin(GL_TRIANGLE_FAN); 

            for (size_t k = 0; k < faceVerts.size(); k++) {
                VertexIndex* v = &faceVerts[k];

                // Send Normal
                if (v->nIdx >= 0 && v->nIdx < (int)temp_normals.size()) {
                    Vec3<float>& n = temp_normals[v->nIdx];
                    glNormal3f(n.x, n.y, n.z);
                }
                // Send UV
                if (v->tIdx >= 0 && v->tIdx < (int)temp_texCoords.size()) {
                    Vec3<float>& t = temp_texCoords[v->tIdx];
                    glTexCoord2f(t.x, 1.0f - t.y);
                }
                // Send Position
                if (v->vIdx >= 0 && v->vIdx < (int)temp_positions.size()) {
                    Vec3<float>& p = temp_positions[v->vIdx];
                    glVertex3f(p.x, p.y, p.z);
                }
            }
            glEnd(); 
        }
    }
}