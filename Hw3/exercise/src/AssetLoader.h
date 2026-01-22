#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include "Object.h"
#include "TextureEnums.h" 
#include "utilities.h" 

class AssetLoader {
public:
    static Object* load(const std::string& filename) {
        return load(filename, Vec3(0.0f, 0.0f, 0.0f));
    }

    static Object* load(const std::string& filename, const Vec3<float>& offsetPos) {
        
        Object* anchor = new Object(offsetPos, false);

        std::stack<Object*> hierarchyStack; 
        std::stack<std::string> typeStack;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return anchor; 
        }

        std::string line;
        Object* currentObj = anchor;
        std::string currentObjType = "Object";
        bool rootObjectDefined = false;

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
                if (!rootObjectDefined) {
                    anchor->setPosition(offsetPos + Vec3<float>(x, y, z));
                    rootObjectDefined = true;
                    continue;
                } else {
                    currentObj = new Object(Vec3<float>(x, y, z), false); 
                    currentObjType = "Object";
                    objectCreated = true;
                }
            }
            else if (cmd == "SPHERE") {
                ss >> x >> y >> z;
                currentObj = new Sphere(x, y, z, false);
                currentObjType = "Sphere";
                objectCreated = true;
            }
            else if (cmd == "CUBOID") {
                float w, h, l; 
                ss >> x >> y >> z >> w >> h >> l;
                currentObj = new Cuboid(x, y, z, w, h, l, false);
                currentObjType = "Cuboid";
                objectCreated = true;
            }
            else if (cmd == "CUBE") {
                float w, h, l; 
                ss >> x >> y >> z >> w >> h >> l;
                currentObj = new Cube(x, y, z, w, h, l, false);
                currentObjType = "Cube";
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
                    currentObj->setTexture(resolveTexture(texName), false);
                }
                else if (cmd == "ROTATION") {
                    float angle, rx, ry, rz;
                    ss >> angle >> rx >> ry >> rz;
                    currentObj->setRotation(angle, Vec3<float>(rx, ry, rz));
                }
                else if (cmd == "SCALE") {
                    float sx, sy, sz;
                    ss >> sx >> sy >> sz;
                    currentObj->setScale(Vec3<float>(sx, sy, sz)); 
                }
                else if (cmd == "GRAVITY") {
                    std::string val; ss >> val;
                    currentObj->setGravity(val == "true");
                }
                else if (cmd == "SET_SUBDIVS") {
                    int sub; ss >> sub;
                    if (currentObjType == "Cuboid") {
                        static_cast<Cuboid*>(currentObj)->setSubdivisions(sub);
                    } else {
                        throw std::runtime_error("Object of type " + currentObjType + " doenst support SET_SUBDIVS directive");
                    }
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
                    typeStack.push(currentObjType);
                }
                else if (cmd == "END") {
                    if (!hierarchyStack.empty()) {
                        currentObj = hierarchyStack.top(); 
                        hierarchyStack.pop();
                        currentObjType = typeStack.top();
                        typeStack.pop();
                    } else {
                        currentObj = anchor;
                        currentObjType = "Object";
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
        if (name == "LEAVES")   return TextureID::LEAVES;
        if (name == "STONE")   return TextureID::STONE;
        return TextureID::NONE;
    }
};

#endif