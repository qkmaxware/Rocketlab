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
    return {x / m, y / m, z / m};
}

vec3 vec3::operator+(const vec3& other){
    return {x + other.x, y + other.y, z + other.z};
}

vec3 vec3::operator-(const vec3& other){
    return {x - other.x, y - other.y, z - other.z};
}

vec3 vec3::operator*(const double scalar){
    return {x * scalar, y * scalar, z * scalar};
}

vec3 vec3::operator/(const double scalar){
    return {x / scalar, y / scalar, z / scalar};
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
    if(t < thrust.time[0])
        return totalMass;
    if(t > thrust.time[thrust.points - 1])
        return (totalMass - propellantMass);
    
    double l = (t - thrust.time[0]) / (thrust.time[thrust.points - 1] - thrust.time[0]);

    //Linearly interpolate between full engine and empty
    return (1 - l) * (totalMass) + (l) * (totalMass - propellantMass);
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
    vec3 down = vec3(0, -1, 0);
    vec3 up = vec3(0, 1 , 0);

    //Create initial stage
    timeslice start;
    start.time = timestamp;

    timeline.push_back(start);

    //Loop until simulation end 
    //End occurs if rocket doesn't launch for 30seconds or rocket has hit the ground after launching
    while((!launched && timestamp < 30) || (launched && grounded)){
        timestamp += timestep;

        timeslice slice;
        slice.time = timestamp;

        //Setup
        double m = rkt.mass(timestamp);
        vec3 p = timeline.back().position;

        //Do phys
        vec3 Fg = down * body.Fg(m, p.y);
        vec3 Ft = up * rkt.motor.thrust.evaluate(timestamp);

        vec3 F = Fg + Ft;
        vec3 a = F / m;
        vec3 vf = timeline.back().velocity + a * timestep;

        //If I havent launched yet, I can't move below y = 0
        if(grounded && vf.y < 0)
            vf.y = 0;

        vec3 d = vf * timestep;

        //Assign values
        slice.position = p + d;
        slice.velocity = vf;

        //Add to timeline
        timeline.push_back(slice);

        //Cleanup flags
        launched = slice.velocity.y > 0;
        grounded = slice.position.y < 0;
    }
}
    
}