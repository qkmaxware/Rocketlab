#include "linearalgebra.h"

namespace la {

//Vec3 implementation
vec3::vec3(): x(0), y(0), z(0){}

vec3::vec3(double ix, double iy, double iz):x(ix), y(iy), z(iz){}

double vec3::sqrMagnitude() {
    return x*x + y*y + z*z;
}

double vec3::magnitude() { 
    return sqrt(this->sqrMagnitude());
}

vec3 vec3::normal() {
    double m = this->magnitude();
    if(m == 0)
        return vec3();
    return vec3(this->x / m, this-> y / m, this->z / m);
}

double vec3::dot(const vec3& other) {
    return x*other.x + y*other.y + z*other.z;
}

vec3 vec3::cross(const vec3& other) {
    return vec3(
        y * other.z - z * other.y,
        z * other.z - x * other.z,
        x * other.y - y * other.x
    );
}

double vec3::operator[](int i) {
    switch(i % 3){
        case 1:
            return y;
        case 2:
            return z;
        default:
            return x;
    }
}

vec3 vec3::operator+(const vec3& other) {
    return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

vec3 vec3::operator-(const vec3& other) {
    return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

vec3 vec3::operator*(const double scalar) {
    return vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}

vec3 vec3::operator/(const double scalar) {
    return vec3(this->x / scalar, this->y / scalar, this->z / scalar);
}

//Quaternion implementation
quat::quat(): x(0), y(0), z(0), w(1){}

quat::quat(double xi, double yi, double zi, double wi): x(xi), y(yi), z(zi), w(wi){} 

double quat::sqrMagnitude(){
    return x*x + y*y + z*z + w*w;
}

double quat::magnitude(){
    return sqrt(sqrMagnitude());
}

quat quat::conjugate(){
    return quat(x, -y, -z, -w);
}

quat quat::normal(){
    double m = magnitude();
    return quat(x / m, y / m, z / m, w / m);
}

double quat::operator[](int i){
    switch(i % 4){
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        default:
            return x;
    }
}

quat quat::operator+(const quat& other){
    return quat(x + other.x, y + other.y, z + other.z, w + other.w);
}

quat quat::operator-(const quat& other){
    return quat(x - other.x, y - other.y, z - other.z, w - other.w);
}

quat quat::operator*(const quat& other){
    return quat(
        x*other.x - y*other.y - z*other.z - w*other.w,
        y*other.x + x*other.y + z*other.w - w*other.z,
        x*other.z - y*other.w + z*other.x + w*other.y,
        x*other.w + y*other.z - z*other.y + w*other.x
    );
}

quat quat::operator/(const quat& other){
    return quat(
        x*other.x - y*-other.y - z*-other.z - w*-other.w,
        y*other.x + x*-other.y + z*-other.w - w*-other.z,
        x*-other.z - y*-other.w + z*other.x + w*-other.y,
        x*-other.w + y*-other.z - z*-other.y + w*other.x
    );
}

quat quat::operator*(const double scalar){
    return quat(x * scalar, y * scalar, z * scalar, w * scalar);
}

quat quat::operator/(const double scalar){
    return quat(x / scalar, y / scalar, z / scalar, w / scalar);
}

vec3 quat::operator*(const vec3& vec){
    double num = x * 2;
    double num2 = y * 2;
    double num3 = z * 2;
    double num4 = x * num;
    double num5 = y * num2;
    double num6 = z * num3;
    double num7 = x * num2;
    double num8 = x * num3;
    double num9 = y * num3;
    double num10 = w * num;
    double num11 = w * num2;
    double num12 = w * num3;
               
    //https://www.mathworks.com/help/aeroblks/quaternionrotation.html
    return vec3(
        (1 - (num5 + num6)) * vec.x + (num7 - num12) * vec.y + (num8 + num11) * vec.z,
        (num7 + num12) * vec.x + (1 - (num4 + num6)) * vec.y + (num9 - num10) * vec.z,
        (num8 - num11) * vec.x + (num9 + num10) * vec.y + (1 - (num4 + num5)) * vec.z
    );
}

//Affine transformation implementation
vec3 affine::right(){
    return rotation * (vec3(1, 0, 0));
}
 
vec3 affine::forward(){
    return rotation * (vec3(0, 0, 1));
}
 
vec3 affine::up(){
    return rotation * (vec3(0, 1, 0));
}
 
vec3 affine::transformPoint(const vec3& other){
    vec3 a = position + other;
    return rotation * a;
}

vec3 affine::transformDirection(const vec3& other){
    return rotation * (other);
}

mat3::mat3() {
    //Clear matrix
    for(int i = 0; i < 9; i++){
        values[i] = 0;
    }
}

double mat3::determinate() {
    mat3& m = *this;
    return (m(0,0) * m(1,1) * m(2,2) + m(0,1) * m(1,2) * m(2,0) + m(0,2) * m(1,0) * m(2,1)) - 
    (m(0,1) * m(1,0) * m(2,2) + m(0,0) * m(1,2) * m(2,1) + m(0,2) * m(1,1) * m(2,0));
}

mat3 mat3::inverse() {
    double idet = 1.0 / (this->determinate());
    mat3& m = *this;
    
    mat3 r;
    r(0,0) = (m(1,1) * m(2,2) - m(1,2) * m(2,1))/idet;
    r(0,1) = (m(0,2) * m(2,1) - m(0,1) * m(2,2))/idet;
    r(0,2) = (m(0,1) * m(1,2) - m(0,2) * m(1,1))/idet;

    r(1,0) = (m(1,2) * m(2,0) - m(1,0) * m(2,2))/idet;
    r(1,1) = (m(0,0) * m(2,2) - m(0,2) * m(2,0))/idet;
    r(1,2) = (m(0,2) * m(1,0) - m(0,0) * m(1,2))/idet;

    r(2,0) = (m(1,0) * m(2,0) - m(1,1) * m(2,2))/idet;
    r(2,1) = (m(0,1) * m(2,0) - m(0,0) * m(2,1))/idet;
    r(2,2) = (m(0,0) * m(1,1) - m(0,1) * m(1,0))/idet;
    return r;
}

vec3 mat3::operator*(const vec3& v) {
   mat3& m = *this;
   return vec3(
        m(0,0) * v.x + m(0,1) * v.y + m(0,2) * v.z,
        m(1,0) * v.x + m(1,1) * v.y + m(1,2) * v.z,
        m(2,0) * v.x + m(2,1) * v.y + m(2,2) * v.z
    );
}

double& mat3::operator()(unsigned int r, unsigned int c) {
    return values[c + r * 3];
}

double mat3::operator()(unsigned int r, unsigned int c) const {
    return values[c + r * 3];
}

mat3 mat3::operator+(const mat3& other) {
    mat3 r;
    for(int i = 0; i < 9; i++){
        r.values[i] = values[i] + other.values[i];
    }
    return r;
}

mat3 mat3::operator-(const mat3& other) {
    mat3 r;
    for(int i = 0; i < 9; i++){
        r.values[i] = values[i] - other.values[i];
    }
    return r;
}

mat3 mat3::operator*(double scalar){
    mat3 r;
    for(int i = 0; i < 9; i++){
        r.values[i] = values[i] * scalar;
    }
    return r;
}

mat3 mat3::operator/(double scalar){
    mat3 r;
    for(int i = 0; i < 9; i++){
        r.values[i] = values[i] / scalar;
    }
    return r;
}

}