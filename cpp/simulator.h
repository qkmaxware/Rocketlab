#ifndef _ROCKETCORE_SIMULATOR_H
#define _ROCKETCORE_SIMULATOR_H

#include <vector>
#include <math.h>

namespace simulator {

struct vec3 {
    double x;
    double y;
    double z;

    vec3();
    vec3(double ix, double iy, double iz);

    double sqrMagnitude();
    double magnitude();
    vec3 normal();

    vec3 operator+(const vec3& other);
    vec3 operator-(const vec3& other);
    vec3 operator*(const double scalar);
    vec3 operator/(const double scalar);
};

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
    engine motor;

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
    vec3 position;
    vec3 velocity;
};

void simulate(double timestep, rocket rkt, planet body, std::vector<timeslice>& timeline);

}

#endif