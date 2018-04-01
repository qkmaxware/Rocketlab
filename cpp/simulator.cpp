#include "simulator.h"

namespace simulator {

//Vec3 implementation
vec3::vec3(): x(0), y(0), z(0){}

vec3::vec3(double ix, double iy, double iz):x(ix), y(iy), z(iz){}

double vec3::sqrMagnitude(){
    return x*x + y*y + z*z;
}

double vec3::magnitude(){
    return sqrt(this->sqrMagnitude());
}

vec3 vec3::normal(){
    double m = this->magnitude();
    if(m == 0)
        return vec3();
    return vec3(this->x / m, this-> y / m, this->z / m);
}

vec3 vec3::operator+(const vec3& other){
    return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

vec3 vec3::operator-(const vec3& other){
    return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

vec3 vec3::operator*(const double scalar){
    return vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}

vec3 vec3::operator/(const double scalar){
    return vec3(this->x / scalar, this->y / scalar, this->z / scalar);
}

//Thrust curve implementation
double thrustcurve::evaluate(double t){
    //Return thrust = 0 if not on the thrust curve
    if(t < time[0])
        return 0;
    if(t > time[points - 1])
        return 0;

    //Find the endpoints around time t
    int start = 0;
    int end = 0;
    for(int i = 0; i < points; i++){
        if(time[i] >= t){
            end = i;
            break;
        }
        if(time[i] < t){
            start = i;
        }
    }

    //Interpolate
    double l = (t - time[start]) / (time[end] - time[start]);
    double ts = thrust[start]; double te = thrust[end];
    double tru = (1 - l) * (ts) + (l) * (te);

    return tru;
}

//Engine implementation
double engine::mass(double t){
    double drymass = (totalMass - propellantMass);
    if(drymass < 0)
        drymass = 0;

    if(t < thrust.time[0])
        return totalMass;
    if(t > thrust.time[thrust.points - 1])
        return drymass; 
    
    double l = (t - thrust.time[0]) / (thrust.time[thrust.points - 1] - thrust.time[0]);
    if(l > 1)
        l = 1;
    if(l < 0)
        l = 0;

    //Linearly interpolate between full engine and empty
    return (1 - l) * (totalMass) + (l) * (drymass);
}

//Atmosphere implementation
double atmosphere::densityAtAltitude(double height){
    double t = height/atmosphericRadius;
    return (1 - t) * surfaceDensity;
}

double atmosphere::dynamicPressure(double speed, double height){
    return (1/2) * densityAtAltitude(height) * speed * speed;
}

double atmosphere::Fd(double speed, double height, double dragCoefficient, double surfaceArea){
    return dynamicPressure(speed, height) * dragCoefficient * surfaceArea;
}

//Rocket implementation
double rocket::mass(double t){
    return rocketMass + motor.mass(t);
}

//Planet implementation
double planet::Ag(double height){
    double ratio = (1 + (height / surfaceRadius));
    return surfaceAcceleration / (ratio * ratio);
}

double planet::Fg(double bodyMass, double height){
    return bodyMass * Ag(height);
}

//----------------------------------
// Run simulation
//----------------------------------
void simulate(double timestep, rocket rkt, planet body, std::vector<timeslice>& timeline){
    //Init counters
    bool launched = false;
    bool grounded = true;
    double timestamp = 0;
    double drag = 0;
    double area = 0;
    vec3 down = vec3(0, -1, 0);
    vec3 up = vec3(0, 1 , 0);

    //Create initial stage
    timeslice start;
    start.time = timestamp;
    start.mass = rkt.mass(timestamp);
    start.gravityForce = body.Fg(start.mass, 0);
    start.thrustForce = 0;
    start.dragForce = 0;

    timeline.push_back(start);

    //Loop until simulation end 
    //End occurs if rocket doesn't launch for 30seconds or rocket has hit the ground after launching
    while((!launched && timestamp < 30) || (launched && !grounded)){
        timestamp += timestep;

        timeslice slice;
        slice.time = timestamp;

        //Setup
        double m = rkt.mass(timestamp);
        vec3 p = timeline.back().position;
        vec3 v = timeline.back().velocity;

        slice.gravityForce = body.Fg(m, p.y);
        slice.dragForce = body.atmo.Fd(v.magnitude(), p.y, drag, area);
        slice.thrustForce = rkt.motor.thrust.evaluate(timestamp);

        //Do phys
        //Force of gravity (changes with altitude)
        vec3 Fg = down * slice.gravityForce;
        //Force of thrust (changes with time)
        vec3 Ft = up * slice.thrustForce;
        //Force of drag (varies with altitude, and speed, and cross-section)
        vec3 dir = v.normal();
        vec3 Fd = (dir) * -slice.dragForce;

        //Total force
        vec3 F = (Fg + Fd) + Ft;
        //Acceleration vector
        vec3 a = F / m;
        //New velocity
        vec3 vf = v + a * timestep;

        if(!launched && vf.y < 0)
            vf.y = 0;

        //Distance travelled
        vec3 d = vf * timestep;
        vec3 np = p + d;
        if(!launched)
            np = p;

        //Assign values
        slice.position = np;
        slice.velocity = vf;
        slice.mass = m;

        //Add to timeline
        timeline.push_back(slice);

        //Cleanup flags
        launched = slice.velocity.magnitude() > 0;
        grounded = !launched || slice.position.y < 0;
    }
}
    
}