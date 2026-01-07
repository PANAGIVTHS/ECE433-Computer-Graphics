#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include "Object.h"
#include "TextureEnums.h" // Ensure this is included for TextureMode/Config

class LevelLoader {
public:
    static std::vector<Object*> load(const std::string& filename) {
        std::vector<Object*> roots;
        std::stack<Object*> hierarchyStack; 
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return roots;
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

            // --- OBJECT CREATION ---
            if (cmd == "OBJECT") {
                float x, y, z; ss >> x >> y >> z;
                currentObj = new Object(Vec3<float>(x, y, z), false); 
                objectCreated = true;
            }
            else if (cmd == "SPHERE") {
                float x, y, z; ss >> x >> y >> z;
                currentObj = new Sphere(x, y, z, false);
                objectCreated = true;
            }
            else if (cmd == "CUBOID") {
                float x, y, z, w, h, l; 
                ss >> x >> y >> z >> w >> h >> l;
                currentObj = new Cuboid(x, y, z, w, h, l, false);
                objectCreated = true;
            }

            // --- LINKING ---
            if (objectCreated && currentObj) {
                if (hierarchyStack.empty()) {
                    roots.push_back(currentObj);
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
                    // Expects: ROTATION <angle> <x> <y> <z>
                    float angle, x, y, z;
                    ss >> angle >> x >> y >> z;
                    currentObj->setRotation(angle, Vec3<float>(x, y, z));
                }
                else if (cmd == "GRAVITY") {
                    std::string val; ss >> val;
                    currentObj->setGravity(val == "true");
                }
                // NEW: Texture Config Support
                else if (cmd == "TEX_CONFIG") {
                    std::string modeStr;
                    ss >> modeStr;

                    if (modeStr == "CUSTOM") {
                        float u, v;
                        ss >> u >> v;
                        currentObj->setTextureConfig(TextureConfig(TextureMode::REPEAT_CUSTOM, u, v));
                    } else {
                        // Default to FIT if mode is FIT or unknown
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
                else {
                    std::cerr << "WARNING: Unknown command '" << cmd << "' in level file. "
                            << "Previous object might be corrupted!" << std::endl;
                }
            }
        }
        
        return roots;
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