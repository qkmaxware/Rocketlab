#ifndef _ROCKETCORE_PHYSICS_H
#define _ROCKETCORE_PHYSICS_H

#include "linearalgebra.h"

namespace physics {

class rigidbody {
    private:
        la::vec3 netforce;
        la::vec3 nettorque;

    public:
        double mass;
        la::mat3 I;
        la::affine transform;
        la::vec3 velocity;
        la::vec3 angularVelocity;

        rigidbody();

        void applyForce(const la::vec3& f);
        void applyTorque(const la::vec3& t);
        void applyForceAtPoint(const la::vec3& f, const la::vec3& p);

        void step(double timestep);
        void reset();
};
    
}

#endif