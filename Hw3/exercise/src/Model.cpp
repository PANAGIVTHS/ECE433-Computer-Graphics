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
    // Debug
    if (displayList == 0) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glutWireCube(1.0f);
    }

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

    // Apply settings
    glDisable(GL_CULL_FACE); // Draw both sides of faces
    glColor3f(color.red, color.green, color.blue);

    glBegin(GL_TRIANGLES);

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
            // 1. Read ALL vertices of the face into a list first
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

            // 2. Triangulate (Fan Method)
            // If face has 3 verts (Triangle): Loop runs once (0, 1, 2)
            // If face has 4 verts (Quad): Loop runs twice (0, 1, 2) and (0, 2, 3)
            for (size_t i = 1; i < faceVerts.size() - 1; ++i) {
                VertexIndex* tri[3] = { &faceVerts[0], &faceVerts[i], &faceVerts[i + 1] };

                for (int k = 0; k < 3; k++) {
                    VertexIndex* v = tri[k];

                    // Send Normal
                    if (v->nIdx >= 0 && v->nIdx < (int)temp_normals.size()) {
                        Vec3<float>& n = temp_normals[v->nIdx];
                        glNormal3f(n.x, n.y, n.z);
                    }
                    // Send UV
                    if (v->tIdx >= 0 && v->tIdx < (int)temp_texCoords.size()) {
                        Vec3<float>& t = temp_texCoords[v->tIdx];
                        glTexCoord2f(t.x, t.y);
                    }
                    // Send Position
                    if (v->vIdx >= 0 && v->vIdx < (int)temp_positions.size()) {
                        Vec3<float>& p = temp_positions[v->vIdx];
                        glVertex3f(p.x, p.y, p.z);
                    }
                }
            }
        }
    }
    glEnd();
}