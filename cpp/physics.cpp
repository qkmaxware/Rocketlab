#include "physics.h"

namespace physics {

rigidbody::rigidbody(): mass(1){}

void rigidbody::applyForce(const la::vec3& f){
    this->netforce = this->netforce + f;
}

void rigidbody::applyTorque(const la::vec3& t){
    this->nettorque = this->nettorque + t;
}

void rigidbody::applyForceAtPoint(const la::vec3& f, const la::vec3& p){
    la::vec3& lp = la::vec3(p.x, p.y, p.z);
    la::vec3 t = lp.cross(f);
    applyTorque(t);
    applyForce(f);
}

void rigidbody::step(double timestep){
    //Linear movement
    //F = ma
    la::vec3 a = netforce / mass;
    netforce = la::vec3();

    //Vf = vi + a * t
    this->velocity = this->velocity + a * timestep;

    //D = s * t
    this->transform.position = this->transform.position + this->velocity * timestep;

    //Angular movement
    //la::vec3 alpha = inverseMomentOfInertia * nettorque;
    //nettorque = vec3();

    //Vf = vi + alpha * t
    //this->angularVelocity = this->angularVelocity + alpha * timestep;

    //this->transform.rotation = this->transform.rotation * quat();
}

void rigidbody::reset(){
    transform.position = la::vec3();
    transform.rotation = la::quat();
    velocity = la::vec3();
    angularVelocity = la::vec3();

    netforce = la::vec3();
    nettorque = la::vec3();
}

}