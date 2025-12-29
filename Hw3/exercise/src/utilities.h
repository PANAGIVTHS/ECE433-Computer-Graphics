#ifndef UTILITIES_H
#define UTILITIES_H
#include <math.h>
using namespace std;

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
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(T x = 0, T y = 0, T z = 0): x(x), y(y), z(z) {}

    Vec3& normalize() {
        T length = sqrt(x*x + y*y + z*z);
        if (length == 0)
            return *this;

        *this /= length;
        return *this;
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

#endif