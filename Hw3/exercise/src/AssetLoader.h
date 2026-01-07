#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include "Object.h"
#include "TextureEnums.h" 
#include "utilities.h" 

class AssetLoader {
public:
    static Object* load(const Vec3<float>& spawnPos, const std::string& filename) {
        
        Object* anchor = new Object(spawnPos, false); 

        std::stack<Object*> hierarchyStack; 
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return anchor; 
        }

        std::string line;
        Object* currentObj = nullptr;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; 

            std::stringstream ss(line);
            std::string cmd;
            ss >> cmd;
            
            if (cmd.empty()) continue; 

            bool objectCreated = false;

            float x, y, z; 
            
            if (cmd == "OBJECT") {
                ss >> x >> y >> z;
                currentObj = new Object(Vec3<float>(x, y, z), false); 
                objectCreated = true;
            }
            else if (cmd == "SPHERE") {
                ss >> x >> y >> z;
                currentObj = new Sphere(x, y, z, false);
                objectCreated = true;
            }
            else if (cmd == "CUBOID") {
                float w, h, l; 
                ss >> x >> y >> z >> w >> h >> l;
                currentObj = new Cuboid(x, y, z, w, h, l, false);
                objectCreated = true;
            }

            // --- LINKING ---
            if (objectCreated && currentObj) {
                if (hierarchyStack.empty()) {
                    anchor->addChildren(currentObj);
                } else {
                    hierarchyStack.top()->addChildren(currentObj);
                }
            }

            // --- PROPERTIES ---
            else if (currentObj) {
                if (cmd == "TEXTURE") {
                    std::string texName; ss >> texName;
                    currentObj->setTexture(resolveTexture(texName));
                }
                else if (cmd == "ROTATION") {
                    float angle, rx, ry, rz;
                    ss >> angle >> rx >> ry >> rz;
                    currentObj->setRotation(angle, Vec3<float>(rx, ry, rz));
                }
                else if (cmd == "GRAVITY") {
                    std::string val; ss >> val;
                    currentObj->setGravity(val == "true");
                }
                else if (cmd == "TEX_CONFIG") {
                    std::string modeStr;
                    ss >> modeStr;
                    if (modeStr == "CUSTOM") {
                        float u, v; ss >> u >> v;
                        currentObj->setTextureConfig(TextureConfig(TextureMode::REPEAT_CUSTOM, u, v));
                    } else {
                        currentObj->setTextureConfig(TextureConfig(TextureMode::REPEAT_FIT));
                    }
                }
                else if (cmd == "BEGIN") {
                    hierarchyStack.push(currentObj);
                }
                else if (cmd == "END") {
                    if (!hierarchyStack.empty()) {
                        currentObj = hierarchyStack.top(); 
                        hierarchyStack.pop();
                    }
                }
            }
        }
        
        return anchor;
    }

private:
    static TextureID resolveTexture(const std::string& name) {
        if (name == "WINDOW") return TextureID::WINDOW;
        if (name == "IRON")   return TextureID::IRON;
        if (name == "GRASS")  return TextureID::GRASS;
        if (name == "WOOD")   return TextureID::WOOD;
        return TextureID::NONE;
    }
};

#endif