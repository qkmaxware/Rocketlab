#ifndef _ROCKETCORE_LINEARALGEBRA_H
#define _ROCKETCORE_LINEARALGEBRA_H

#include <math.h>

namespace la{

struct vec3 {
    double x;
    double y;
    double z;

    vec3();
    vec3(double ix, double iy, double iz);

    double sqrMagnitude();
    double magnitude();
    vec3 normal();

    double dot(const vec3& other);
    vec3 cross(const vec3& other);

    double operator[](int i);
    vec3 operator+(const vec3& other);
    vec3 operator-(const vec3& other);
    vec3 operator*(const double scalar);
    vec3 operator/(const double scalar);
};

struct quat {
    double x;
    double y;
    double z;
    double w;

    quat();
    quat(double x, double y = 0, double z = 0, double w = 0);

    double sqrMagnitude();
    double magnitude();
    quat conjugate();
    quat normal();

    double operator[](int i);
    quat operator+(const quat& other);
    quat operator-(const quat& other);
    quat operator*(const quat& other);
    quat operator/(const quat& other);
    quat operator*(const double scalar);
    quat operator/(const double scalar);

    vec3 operator*(const vec3& vec);
};

struct affine {
    quat rotation;
    vec3 position;
               
    vec3 right();
    vec3 forward();
    vec3 up();
               
    vec3 transformPoint(const vec3& other);
    vec3 transformDirection(const vec3& other);

};


}

#endif