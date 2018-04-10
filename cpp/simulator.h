#ifndef _ROCKETCORE_SIMULATOR_H
#define _ROCKETCORE_SIMULATOR_H

#include <vector>
#include <math.h>
#include <fstream>

#include "linearalgebra.h"
#include "physics.h"

using namespace la;

namespace simulator {

//Represents thrust vs time
struct thrustcurve {
    int points;
    double* time;
    double* thrust;
    double evaluate(double t);
};

//Represents engine parameters
struct engine {
    double totalMass;
    double propellantMass;
    double ignitionDelay;
    thrustcurve thrust;

    double mass(double t);
};

//Represents the rocket
struct rocket {
    double rocketMass;
    double dragCoefficient;
    double crossSectionalArea;
    engine motor;
    physics::rigidbody body;

    double mass(double t);
};

//Represents a planet's atmosphere
struct atmosphere {
    double surfaceDensity;
    double atmosphericRadius;
    //Fd = (1/2) * MassDensity * FlowVelocity^2 * CoefficientOfDrag * SurfaceArea
    double densityAtAltitude(double height);
    double dynamicPressure(double speed, double height);
    double Fd(double speed, double height, double dragCoefficient, double surfaceArea);
};

//Represents the planet being launched from
struct planet {
    double surfaceAcceleration;
    double surfaceRadius;
    atmosphere atmo;
    double Ag(double height);
    double Fg(double bodyMass, double height);
};

//Respresents instananeous moment in time during the simulation
struct timeslice {
    double time;
    double mass;
    double gravityForce;
    double dragForce;
    double thrustForce;
    double dynamicPressure;
    double altitude;
    vec3 position;
    vec3 velocity;
};

void simulate(double timestep, rocket rkt, planet body, std::vector<timeslice>& timeline);

void formatOutput(std::string filename, std::vector<timeslice>& timeline);

}

#endif