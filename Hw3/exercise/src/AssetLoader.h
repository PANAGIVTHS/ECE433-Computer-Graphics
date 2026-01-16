#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>          // [ADDED] For variable storage
#include <cmath>        // [ADDED] For math operations
#include <algorithm>    // [ADDED] For string manipulation
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
        std::map<std::string, float> variables; // [ADDED] Store variables here

        std::stack<Object*> hierarchyStack; 
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return anchor; 
        }

        std::string line;
        Object* currentObj = anchor;
        bool rootObjectDefined = false;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; 

            std::stringstream ss(line);
            std::string cmd;
            ss >> cmd;
            
            if (cmd.empty()) continue; 

            bool objectCreated = false;

            // [CHANGE] Removed local 'float x,y,z' declarations here to use getNextFloat() where needed

            if (cmd == "OBJECT") {
                // [CHANGE] Use getNextFloat to support math/vars
                float x = getNextFloat(ss, variables);
                float y = getNextFloat(ss, variables);
                float z = getNextFloat(ss, variables);

                if (!rootObjectDefined) {
                    anchor->setPosition(offsetPos + Vec3<float>(x, y, z));
                    rootObjectDefined = true;
                    continue;
                } else {
                    currentObj = new Object(Vec3<float>(x, y, z), false); 
                    objectCreated = true;
                }
            }
            else if (cmd == "SPHERE") {
                float x = getNextFloat(ss, variables);
                float y = getNextFloat(ss, variables);
                float z = getNextFloat(ss, variables);
                currentObj = new Sphere(x, y, z, false);
                objectCreated = true;
            }
            else if (cmd == "CUBOID") {
                // [CHANGE] Support vars for all dimensions
                float x = getNextFloat(ss, variables);
                float y = getNextFloat(ss, variables);
                float z = getNextFloat(ss, variables);
                float w = getNextFloat(ss, variables);
                float h = getNextFloat(ss, variables);
                float l = getNextFloat(ss, variables);
                currentObj = new Cuboid(x, y, z, w, h, l, false);
                objectCreated = true;
            }
            else if (cmd == "CUBE") {
                float x = getNextFloat(ss, variables);
                float y = getNextFloat(ss, variables);
                float z = getNextFloat(ss, variables);
                float w = getNextFloat(ss, variables);
                float h = getNextFloat(ss, variables);
                float l = getNextFloat(ss, variables);
                currentObj = new Cube(x, y, z, w, h, l, false);
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
                    float angle = getNextFloat(ss, variables);
                    float rx = getNextFloat(ss, variables);
                    float ry = getNextFloat(ss, variables);
                    float rz = getNextFloat(ss, variables);
                    currentObj->setRotation(angle, Vec3<float>(rx, ry, rz));
                }
                else if (cmd == "SCALE") {
                    float sx = getNextFloat(ss, variables);
                    float sy = getNextFloat(ss, variables);
                    float sz = getNextFloat(ss, variables);
                    currentObj->setScale(Vec3<float>(sx, sy, sz)); 
                }
                else if (cmd == "GRAVITY") {
                    std::string val; ss >> val;
                    currentObj->setGravity(val == "true");
                }
                else if (cmd == "TEX_CONFIG") {
                    std::string modeStr;
                    ss >> modeStr;
                    if (modeStr == "CUSTOM") {
                        float u = getNextFloat(ss, variables);
                        float v = getNextFloat(ss, variables);
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
                    } else {
                        currentObj = anchor;
                    }
                }
                // [ADDED] Variable Assignment Logic: checks if unknown command is a variable definition
                else {
                    // Peek to see if the next token is "="
                    std::string op; 
                    std::streampos oldPos = ss.tellg(); // Save position
                    ss >> op;
                    
                    if (op == "=") {
                        // It is an assignment: VAR = EXPRESSION
                        std::string expression;
                        std::getline(ss, expression); // Read rest of line
                        variables[cmd] = evaluateExpression(expression, variables);
                    } else {
                        // Not an assignment, reset stream (though unnecessary if we just continue)
                         ss.seekg(oldPos);
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
        if (name == "LEAVES") return TextureID::LEAVES; // [FIXED] Removed extra space in string literal
        return TextureID::NONE;
    }

    // [ADDED] Helper to read a token from stream and evaluate it as math
    static float getNextFloat(std::stringstream& ss, std::map<std::string, float>& vars) {
        std::string token;
        ss >> token;
        if (token.empty()) return 0.0f;
        return evaluateExpression(token, vars);
    }

    // [ADDED] Simple Recursive Descent Parser for Math (+, -, *, /, parens)
    static float evaluateExpression(std::string expr, std::map<std::string, float>& vars) {
        // Remove spaces
        expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
        const char* p = expr.c_str();

        struct Parser {
            const char*& p;
            std::map<std::string, float>& vars;

            float parseExpression() {
                float lhs = parseTerm();
                while (*p == '+' || *p == '-') {
                    char op = *p++;
                    float rhs = parseTerm();
                    if (op == '+') lhs += rhs;
                    else lhs -= rhs;
                }
                return lhs;
            }

            float parseTerm() {
                float lhs = parseFactor();
                while (*p == '*' || *p == '/') {
                    char op = *p++;
                    float rhs = parseFactor();
                    if (op == '*') lhs *= rhs;
                    else if (rhs != 0) lhs /= rhs; 
                }
                return lhs;
            }

            float parseFactor() {
                if (*p == '(') {
                    p++; // consume '('
                    float val = parseExpression();
                    p++; // consume ')'
                    return val;
                }
                if (isdigit(*p) || *p == '-' || *p == '.') {
                    char* end;
                    float val = strtof(p, &end);
                    p = end;
                    return val;
                }
                if (isalpha(*p)) {
                    std::string varName;
                    while (isalnum(*p) || *p == '_') varName += *p++;
                    return vars[varName]; // Returns 0.0 if not found (default map behavior)
                }
                return 0.0f;
            }
        };

        Parser parser{p, vars};
        return parser.parseExpression();
    }
};

#endif