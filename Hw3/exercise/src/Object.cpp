#include "Object.h"
#include "GameManager.h"
#include <algorithm>
#include "TextureManager.h"
#include <sstream>

// Object implementation
Object::Object(Vec3<GLfloat> pos, bool gravity, TextureID texture, MaterialID material, std::initializer_list<Object*> children) : transform(pos), gravity(gravity), texture(texture), material(material) {
    ObjectHandler::addObject(this);

    for (Object* child : children) {
        this->addChildren(child);
    }
}

Object::~Object() {
    ObjectHandler::removeObject(this);

    for (Object *o : children)
        delete o;
    children.clear();
}

void Object::invalidateDisplayList() {
    displayList = 0;
}

void Object::optimize() {
    if (displayList != 0) return;

    displayList = glGenLists(1);
    //! Record mode objects arent displayed
    glNewList(displayList, GL_COMPILE); 

    MaterialManager::bind(material);
    TextureManager::bind(texture);

    drawInternal();

    for (Object *o : children) {
        o->draw();
    }

    glEndList();
}

void Object::draw() {
    if (hidden)
        return;

    glPushMatrix();

    glTranslated(transform.position.x, transform.position.y, transform.position.z);
    if (transform.angle != 0.0f) {
        glRotated(transform.angle, transform.rotateAxis.x, transform.rotateAxis.y, transform.rotateAxis.z);
    }
    if (transform.scale.x != 1.0f || transform.scale.y != 1.0f || transform.scale.z != 1.0f) {
        glScaled(transform.scale.x, transform.scale.y, transform.scale.z);
    }

    if (displayList != 0) {
        glCallList(displayList);
    } else {
        if (texture != TextureID::NONE) {
            TextureManager::bind(texture);
        }

        drawInternal();

        if (texture != TextureID::NONE) {
            TextureManager::bind(TextureID::NONE);
        }
        for (Object *o : children)
            o->draw();
    }
    glPopMatrix();
}

void Object::update() {
    if (gravity)
        velocity.y -= GameManager::gravity * GameManager::dt;
    this->transform.position += this->velocity * (GLfloat) GameManager::dt;
}

Object *Object::addChildren(Object *o) {
    ObjectHandler::removeObject(o);
    o->setGravity(false);
    o->setVelocity(Vec3(0.0f, 0.0f, 0.0f));
    children.push_back(o);

    return this;
}

Object* Object::setScale(Vec3<GLfloat> scale) {
    this->transform.scale = scale;
    return this;
}

Object* Object::setScale(GLfloat x, GLfloat y, GLfloat z) {
    this->transform.scale = Vec3<GLfloat>(x, y, z);
    return this;
}

Object* Object::setRotation(GLfloat angle, Vec3<GLfloat> axis) {
    this->transform.angle = angle;
    this->transform.rotateAxis = axis;
    return this;
}

Object* Object::setTexture(TextureID id) {
    this->texture = id;
    displayList = 0;
    return this;
}

bool Object::isHidden() {
    return hidden;
}

Object* Object::setHidden(bool hidden) {
    this->hidden = hidden;
    return this;
}

bool Object::hasGravity() {
    return gravity;
}

Object* Object::setGravity(bool gravity) {
    this->gravity = gravity;
    return this;
}

Vec3<GLfloat>& Object::getPosition() {
    return transform.position;
}

Object* Object::setPosition(Vec3<GLfloat> position) {
    this->transform.position = position;
    return this;
}

Vec3<GLfloat>& Object::getVelocity() {
    return velocity;
}

Object* Object::setVelocity(Vec3<GLfloat> velocity) {
    this->velocity = velocity;
    return this;
}

std::string Object::to_string() {
    return to_string(0);
}

std::string Object::to_string(int depth) {
    std::stringstream ss;
    std::string indent(depth * 2, ' '); 

    ss << indent << "Object {\n";
    
    // Transform / Position
    ss << indent << "  Position: (" << transform.position.x << ", " << transform.position.y << ", " << transform.position.z << ")\n";
    ss << indent << "  Scale: (" << transform.scale.x << ", " << transform.scale.y << ", " << transform.scale.z << ")\n";
    ss << indent << "  Rotation: " << transform.angle << " deg around (" 
       << transform.rotateAxis.x << ", " << transform.rotateAxis.y << ", " << transform.rotateAxis.z << ")\n";
    
    // State properties
    ss << indent << "  Velocity: (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")\n";
    ss << indent << "  Gravity: " << (gravity ? "enabled" : "disabled") << "\n";
    ss << indent << "  Hidden: " << (hidden ? "true" : "false") << "\n";
    ss << indent << "  TextureID: " << static_cast<int>(texture) << "\n";

    // Children recursion
    if (!children.empty()) {
        ss << indent << "  Children [" << children.size() << "]:\n";
        for (Object* child : children) {
            if (child) {
                // Recursive call increasing the depth
                ss << child->to_string(depth + 2);
            }
        }
    }

    ss << indent << "}\n";
    return ss.str();
}

// ObjectHandler implementation

std::vector<Object *> ObjectHandler::objects;

void ObjectHandler::addObject(Object *o) {
    ObjectHandler::objects.push_back(o);
}

void ObjectHandler::removeObject(Object *o) {
    std::vector<Object *>::iterator it = std::find(objects.begin(), objects.end(), o);
    if (it != objects.end()) {
        std::iter_swap(it, objects.end() - 1);
        objects.pop_back();
    }
}

std::vector<Object *> &ObjectHandler::getObjects() {
    return objects;
}

void ObjectHandler::invalidateListAll() {
    for (Object *o : objects) {
        o->invalidateDisplayList();
    }
}

void ObjectHandler::clear() {
    while(!objects.empty()) {
        delete objects.back();
    }
}

// Object subclasses implementation

// Cuboid
void Cuboid::drawInternal() {
    glColor3f(1.0f, 1.0f, 1.0f);

    Vec3<float> front(0.0f, 0.0f, 1.0f), back(0.0f, 0.0f, -1.0f);
    Vec3<float> right(1.0f, 0.0f, 0.0f), left(-1.0f, 0.0f, 0.0f);
    Vec3<float> up(0.0f, 1.0f, 0.0f), down(0.0f, -1.0f, 0.0f);

    float w = 0.5f;
    float h = 0.5f;
    float l = 0.5f;

    float width  = transform.scale.x;
    float height = transform.scale.y;
    float length = transform.scale.z;

    float uFront = 1.0f, vFront = 1.0f;
    float uSide  = 1.0f, vSide  = 1.0f;
    float uTop   = 1.0f, vTop   = 1.0f;

    if (texConfig.mode == TextureMode::REPEAT_FIT) {
        uFront = width;  vFront = height;
        uSide  = length; vSide  = height;
        uTop   = width;  vTop   = length;
    } else if (texConfig.mode == TextureMode::REPEAT_CUSTOM) {
        // Access values from the struct
        uFront = uSide = uTop = texConfig.uMax;
        vFront = vSide = vTop = texConfig.vMax;
    }

    //! Maybe not clean enough
    TextureManager::drawQuadTex(
        Vec3<GLfloat>(-w, -h,  l), Vec3<GLfloat>( w, -h,  l), 
        Vec3<GLfloat>( w,  h,  l), Vec3<GLfloat>(-w,  h,  l), front, uFront, vFront);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>( w, -h, -l), Vec3<GLfloat>(-w, -h, -l), 
        Vec3<GLfloat>(-w,  h, -l), Vec3<GLfloat>( w,  h, -l), back, uFront, vFront);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>( w, -h, -l), Vec3<GLfloat>( w, -h,  l), 
        Vec3<GLfloat>( w,  h,  l), Vec3<GLfloat>( w,  h, -l), right, uSide, vSide);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>(-w, -h,  l), Vec3<GLfloat>(-w, -h, -l), 
        Vec3<GLfloat>(-w,  h, -l), Vec3<GLfloat>(-w,  h,  l), left, uSide, vSide);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>(-w,  h,  l), Vec3<GLfloat>( w,  h,  l), 
        Vec3<GLfloat>( w,  h, -l), Vec3<GLfloat>(-w,  h, -l), up, uTop, vTop);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>(-w, -h, -l), Vec3<GLfloat>( w, -h, -l), 
        Vec3<GLfloat>( w, -h,  l), Vec3<GLfloat>(-w, -h,  l), down, uTop, vTop);
}

void Cube::drawInternal() {
    glColor3f(1.0f, 0.0f, 0.0f);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    glutSolidCube(1.0f);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
}

// Sphere
void Sphere::drawInternal() {
    glColor3f(1.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    glutSolidSphere(1.0f, 20, 20);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
}
