#include "Object.h"
#include "GameManager.h"
#include <algorithm>
#include "TextureManager.h"
#include <sstream>

// Object implementation

Object::Object(Vec3<GLfloat> pos, bool gravity, Color3f color, TextureID texture, MaterialID material, std::initializer_list<Object*> children) : transform(pos), gravity(gravity), color(color), texture(texture), material(material) {
    ObjectHandler::addObject(this);
    ObjectHandler::addTransparent(this);

    for (Object* child : children) {
        this->Object::addChildren(child);
    }
}

Object::~Object() {
    if (displayList != 0) {
        glDeleteLists(displayList, 1);
        displayList = 0;
    }
    
    if (parent != nullptr) 
        parent->removeChildren(this);

    ObjectHandler::removeObject(this);
    ObjectHandler::removeTransparent(this);

    for (Object *o : children) {
        o->parent = nullptr;
        delete o;
    }
    children.clear();
}

void Object::invalidateDisplayList() {
    if (displayList != 0) {
        glDeleteLists(displayList, 1);
        displayList = 0;
    }

    for (Object *o : children) {
        o->invalidateDisplayList();
    }
}

bool Object::hasTransparency() {
    return TextureManager::isTransparent(this->texture);
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
        if (!o->hasTransparency()) 
            o->draw();
    }

    glEndList();
}

void Object::applyParentTransforms() {
    if (parent == nullptr) return;

    parent->applyParentTransforms();

    glTranslated(parent->transform.position.x, parent->transform.position.y, parent->transform.position.z);
    
    //! Ifs are for performance in case it does the matrix mult when all is 0
    if (parent->transform.angle != 0.0f) {
        glRotated(parent->transform.angle, parent->transform.rotateAxis.x, parent->transform.rotateAxis.y, parent->transform.rotateAxis.z);
    }
    
    if (parent->transform.scale.x != 1.0f || parent->transform.scale.y != 1.0f || parent->transform.scale.z != 1.0f) {
        glScaled(parent->transform.scale.x, parent->transform.scale.y, parent->transform.scale.z);
    }
}

void Object::draw() {
    if (hidden)
        return;

    if (staticObject && displayList == 0) {
        optimize();
    }

    glPushMatrix();

    if (hasTransparency() && parent != nullptr) {
        applyParentTransforms();
    }

    glTranslated(getPosition().x, getPosition().y, getPosition().z);
    if (transform.angle != 0.0f) {
        glRotated(transform.angle, transform.rotateAxis.x, transform.rotateAxis.y, transform.rotateAxis.z);
    }
    if (transform.scale.x != 1.0f || transform.scale.y != 1.0f || transform.scale.z != 1.0f) {
        glScaled(transform.scale.x, transform.scale.y, transform.scale.z);
    }

    if (staticObject) {
        glCallList(displayList);
    } else {
        MaterialManager::bind(material);
        TextureManager::bind(texture);

        drawInternal();

        for (Object *o : children) {
            if (!o->hasTransparency()) 
                o->draw();
        }
    }
    
    glPopMatrix();
}

void Object::update() {
    if (gravity)
        velocity.y -= GameManager::gravity * GameManager::dt;
    this->transform.position += this->velocity * (GLfloat) GameManager::dt;
}

Object *Object::addChildren(Object *o) {
    if (!o) return this;

    ObjectHandler::removeObject(o);
    o->setHidden(this->isHidden());
    o->setGravity(false);
    o->setVelocity(Vec3(0.0f, 0.0f, 0.0f));
    o->parent = this;
    children.push_back(o);

    return this;
}

Object *Object::removeChildren(Object *o) {
    if (!o) return this;

    auto it = std::find(children.begin(), children.end(), o);
    if (it != children.end())
        children.erase(it);
    o->parent = nullptr;
    o->setGravity(gravity);
    o->setVelocity(velocity);
    ObjectHandler::addObject(o);

    return this;
}

Vec3<GLfloat> Object::getWorldPosition() {
    if (parent == nullptr) {
        return transform.position;
    }
    // Recursive Step: Parent's World Position + My Local Position
    return parent->getWorldPosition() + transform.position;
}

Object* Object::setScale(Vec3<GLfloat> scale) {
    if (this->transform.scale == scale) return this;
    this->transform.scale = scale;
    if (staticObject && (displayList != 0)) invalidateDisplayList();
    return this;
}

Object* Object::setScale(GLfloat x, GLfloat y, GLfloat z) {
    if (this->transform.scale == Vec3(x, y, z)) return this;
    this->transform.scale = Vec3<GLfloat>(x, y, z);
    if (staticObject && (displayList != 0)) invalidateDisplayList();
    return this;
}

Object* Object::setRotation(GLfloat angle, Vec3<GLfloat> axis) {
    if (this->transform.angle == angle && this->transform.rotateAxis == axis) return this;

    this->transform.angle = angle;
    this->transform.rotateAxis = axis;
    if (staticObject && (displayList != 0)) invalidateDisplayList();
    return this;
}

Object* Object::setTexture(TextureID id, bool recurse) {
    TextureID old = texture;
    this->texture = id;
    
    if (TextureManager::isTransparent(old) && !TextureManager::isTransparent(id))
        ObjectHandler::removeTransparent(this);
    else if (!TextureManager::isTransparent(old) && TextureManager::isTransparent(id))
        ObjectHandler::addTransparent(this);
    
    if (!recurse)
        return this;

    for (Object* o : children)
        o->setTexture(id, recurse);
    if (staticObject && (displayList != 0)) invalidateDisplayList();
    return this;
}

Object* Object::setColor(Color3f color, bool recurse) {
    this->color = color;
    if (!recurse)
        return this;

    for (Object* o : children)
        o->setColor(color, recurse);
    if (staticObject && (displayList != 0)) invalidateDisplayList();
    return this;
}

Object* Object::setMaterial(MaterialID id, bool recurse) {
    this->material = id;
    if (!recurse)
        return this;

    for (Object* o : children)
        o->setMaterial(id, recurse);
    if (staticObject && (displayList != 0)) invalidateDisplayList();
    return this;
}

bool Object::isHidden() {
    return hidden;
}

Object* Object::setHidden(bool hidden) {
    this->hidden = hidden;
    if (staticObject && (displayList != 0)) invalidateDisplayList();

    for (Object *o : children)
        o->setHidden(hidden);

    return this;
}

bool Object::hasGravity() {
    return gravity;
}

Object* Object::setGravity(bool gravity) {
    this->gravity = gravity;
    return this;
}

Vec3<GLfloat> Object::getPosition() const{
    return transform.position;
}

Object* Object::setPosition(Vec3<GLfloat> position) {
    if (this->transform.position == position) return this;
    this->transform.position = position;
    if (staticObject && (displayList != 0)) invalidateDisplayList();
    return this;
}

bool Object::isStatic() {
    return staticObject;
}

Object* Object::setStatic(bool staticObject) {
    this->staticObject = staticObject;
    if ((staticObject == false) && (displayList != 0)) {
        glDeleteLists(displayList, 1);
        displayList = 0;
    }

    return this;
}

Vec3<GLfloat>& Object::getVelocity() {
    return velocity;
}

Object* Object::setVelocity(Vec3<GLfloat> velocity) {
    this->velocity = velocity;
    return this;
}

std::vector<Object *>& Object::getChildren() {
    return children;
}

std::string Object::toString() {
    return toString(0);
}

std::string Object::toString(int depth) {
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
                ss << child->toString(depth + 2);
            }
        }
    }

    ss << indent << "}\n";
    return ss.str();
}

// ObjectHandler implementation

std::vector<Object *> ObjectHandler::objects;
std::vector<Object *> ObjectHandler::transObjects;

void ObjectHandler::addObject(Object *o) {
    ObjectHandler::objects.push_back(o);
}

void ObjectHandler::addTransparent(Object *o) {
    if (!o->hasTransparency())
        return;
    
    auto itTrans = std::find(transObjects.begin(), transObjects.end(), o);
    if (itTrans == transObjects.end())
        ObjectHandler::transObjects.push_back(o);
}

void ObjectHandler::removeObject(Object *o) {
    std::vector<Object *>::iterator it = std::find(objects.begin(), objects.end(), o);
    if (it != objects.end()) {
        std::iter_swap(it, objects.end() - 1);
        objects.pop_back();
    }
}

void ObjectHandler::removeTransparent(Object *o) {
    auto itTrans = std::find(transObjects.begin(), transObjects.end(), o);
    if (itTrans != transObjects.end()) {
        std::iter_swap(itTrans, transObjects.end() - 1);
        transObjects.pop_back();
    }
}

std::vector<Object *> &ObjectHandler::getTransObjects() { 
    return transObjects;
}

std::vector<Object *> &ObjectHandler::getObjects() {
    return objects;
}

void ObjectHandler::invalidateDisplayListAll() {
    for (Object *o : objects) {
        o->invalidateDisplayList();
    }
}

void ObjectHandler::clear() {
    while(!objects.empty()) {
        delete objects.back();
    }

    objects.clear();
    transObjects.clear();
}

// Object subclasses implementation

// Cuboid
void Cuboid::drawInternal() {
    glColor3f(color.red, color.green, color.blue);

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
        Vec3<GLfloat>( w,  h,  l), Vec3<GLfloat>(-w,  h,  l), front, uFront, vFront, subdivisions);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>( w, -h, -l), Vec3<GLfloat>(-w, -h, -l), 
        Vec3<GLfloat>(-w,  h, -l), Vec3<GLfloat>( w,  h, -l), back, uFront, vFront, subdivisions);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>( w, -h, -l), Vec3<GLfloat>( w, -h,  l), 
        Vec3<GLfloat>( w,  h,  l), Vec3<GLfloat>( w,  h, -l), right, uSide, vSide, subdivisions);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>(-w, -h,  l), Vec3<GLfloat>(-w, -h, -l), 
        Vec3<GLfloat>(-w,  h, -l), Vec3<GLfloat>(-w,  h,  l), left, uSide, vSide, subdivisions);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>(-w,  h,  l), Vec3<GLfloat>( w,  h,  l), 
        Vec3<GLfloat>( w,  h, -l), Vec3<GLfloat>(-w,  h, -l), up, uTop, vTop, subdivisions);

    TextureManager::drawQuadTex(
        Vec3<GLfloat>(-w, -h, -l), Vec3<GLfloat>( w, -h, -l), 
        Vec3<GLfloat>( w, -h,  l), Vec3<GLfloat>(-w, -h,  l), down, uTop, vTop, subdivisions);
}

void Cube::drawInternal() {
    glColor3f(color.red, color.green, color.blue);

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
    glColor3f(color.red, color.green, color.blue);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    glutSolidSphere(1.0f, 20, 20);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
}
