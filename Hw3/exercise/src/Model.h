#ifndef MODEL_H
#define MODEL_H

#include "Object.h"
#include <vector>
#include <string>

class Model : public Object {
public:
    Model(std::string filename, Vec3<float> pos, bool gravity = DEFAULT_GRAVITY, 
          Color3f color = DEFAULT_COLOR, TextureID texture = DEFAULT_TEXTURE, 
          MaterialID material = DEFAULT_MATERIAL);
    
private:
    // We override optimize because we do it automatically in the constructor
    // and we don't have data to re-optimize later.
    void drawInternal() override; // Will basically be empty or error handling
    
    void loadAndCompile(const std::string& filename);
};

#endif