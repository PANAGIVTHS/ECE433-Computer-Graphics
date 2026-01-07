#ifndef UTILITIES_H
#define UTILITIES_H
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>

inline float toRadians(float degrees) {
    return degrees * (M_PI / 180);
}

enum Direction {
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

typedef struct {
    float red, green, blue;
} Color3f;

template <typename T>
class Vec3
{
public:
    T x, y, z;
    
    Vec3(T x = 0, T y = 0, T z = 0): x(x), y(y), z(z) {}

    Vec3& normalize() {
        T length = sqrt(x*x + y*y + z*z);
        if (length == 0)
            return *this;

        *this /= length;
        return *this;
    }

    Vec3<T> cross(const Vec3<T>& other) {
        return Vec3<T>(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vec3& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vec3& operator/=(T scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
};

template <typename T>
Vec3<T> operator+(Vec3<T> lhs, const Vec3<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
Vec3<T> operator*(Vec3<T> vec, T scalar) {
    vec *= scalar;
    return vec;
}

template <typename T>
Vec3<T> operator/(Vec3<T> vec, T scalar) {
    vec /= scalar;
    return vec;
}

struct Transform {
    Vec3<GLfloat> position;
    Vec3<GLfloat> scale;
    Vec3<GLfloat> rotateAxis;
    GLfloat angle;

    Transform(Vec3<GLfloat> pos = Vec3<GLfloat>(0,0,0)) 
        : position(pos), scale(1.0f, 1.0f, 1.0f), rotateAxis(0.0f, 1.0f, 0.0f), angle(0.0f) {}
};

#endif