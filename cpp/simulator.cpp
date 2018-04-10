#include "simulator.h"

namespace simulator {

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
    if(t < 0)
        t = 0;
    if(t > 1)
        t = 1;
    return (1 - t) * surfaceDensity;
}

double atmosphere::dynamicPressure(double speed, double height){
    return (0.5) * densityAtAltitude(height) * speed * speed;
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
    double drag = rkt.dragCoefficient;
    double area = rkt.crossSectionalArea;

    //Create initial stage
    timeslice start;
    start.position = vec3(0,body.surfaceRadius,0); //Start on surface
    start.time = timestamp;
    start.mass = rkt.mass(timestamp);
    start.gravityForce = body.Fg(start.mass, 0);
    start.thrustForce = 0;
    start.dragForce = 0;
    start.altitude = 0;
    start.dynamicPressure = 0;

    rkt.body.reset();
    rkt.body.transform.position = start.position;

    timeline.push_back(start);

    //Loop until simulation end 
    //End occurs if rocket doesn't launch for 30seconds or rocket has hit the ground after launching
    while((!launched && timestamp < 30) || (launched && !grounded)){
        timestamp += timestep;

        timeslice slice;
        slice.time = timestamp;

        //Setup
        vec3 p = timeline.back().position;
        vec3 v = timeline.back().velocity;
        vec3 norm = p.normal();
        double m = rkt.mass(timestamp);
        double speed = v.magnitude();

        slice.altitude = p.magnitude() - body.surfaceRadius;
        slice.gravityForce = body.Fg(m, slice.altitude);
        slice.dynamicPressure = body.atmo.dynamicPressure(speed, slice.altitude);
        slice.dragForce = body.atmo.Fd(speed, slice.altitude, drag, area);
        slice.thrustForce = rkt.motor.thrust.evaluate(timestamp);

        //Do phys
        //Force of gravity (changes with altitude)
        vec3 Fg = (norm * -1) * slice.gravityForce;
        //Force of thrust (changes with time)
        vec3 Ft = norm * slice.thrustForce;
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
        launched = launched || slice.velocity.magnitude() > 0;
        grounded = !launched || slice.altitude < 0;
    }
}

void formatOutput(std::string filename, std::vector<timeslice>& timeline){
    std::ofstream out(filename.c_str(), std::ofstream::out);
    //Output statistics
    out << "#Summary Statistics" << std::endl;
    double maxtime = 0; 
    double mintime = 0;
    double maxalt = 0;
    double minalt = 0;
    double addalt = 0;
    double maxspeed = 0;
    double minspeed = 0;
    double addspeed = 0;
    double minq = 0;
    double maxq = 0;
    double addq = 0;
    for(int i = 0; i < timeline.size(); i++){
        simulator::timeslice& slice = timeline[i];

        if(slice.time < mintime)
            mintime = slice.time;
        if(slice.time > maxtime)
            maxtime = slice.time;
        
        if(slice.altitude < minalt)
            minalt = slice.altitude;
        if(slice.altitude > maxalt)
            maxalt = slice.altitude;
        addalt += slice.altitude;

        double speed = slice.velocity.magnitude();
        if(speed < minspeed)
            minspeed = speed;
        if(speed > maxspeed)
            maxspeed = speed;
        addspeed += speed;

        if(slice.dynamicPressure < minq)
            minq = slice.dynamicPressure;
        if(slice.dynamicPressure > maxq)
            maxq = slice.dynamicPressure;
        addq += slice.dynamicPressure;
    }
    out << "Quantity, Min, Max, Avg" << std::endl;
    out << "Flight time (s), " << mintime << ", " << maxtime << std::endl;
    out << "Altitude (m), " << minalt << ", " << maxalt << ", " << (addalt / timeline.size()) << std::endl;
    out << "Speed (m/s), " << minspeed << ", " << maxspeed << ", " << (addspeed / timeline.size()) << std::endl;
    out << "Dynamic Pressure, " << minq << ", " << maxq << ", " << (addq / timeline.size()) << std::endl;
    out << std::endl;

    //Output timeline
    out << "#Timeline" << std::endl;
    out << "time, mass, altitude, x, y, z, vx, vy, vz, Fg, Fd, Ft" << std::endl;
    for(int i = 0; i < timeline.size(); i++){
        simulator::timeslice& slice = timeline[i];
        out << slice.time << ", " << slice.mass << ", " << slice.altitude, ",";
        out << slice.position.x << ", " << slice.position.y << ", " << slice.position.z << ", ";
        out << slice.velocity.x << ", " << slice.velocity.y << ", " << slice.velocity.z << ", ";
        out << slice.gravityForce << ", " << slice.dragForce << ", " << slice.thrustForce;
        out << std::endl;
    }
    out.close();
}

}