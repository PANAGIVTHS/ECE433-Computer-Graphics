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
    std::string modelPath;
    void drawInternal() override;
    void loadAndCompile(const std::string& filename);
};

#endif