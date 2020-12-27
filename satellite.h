#ifndef SATELLITE_H
#define SATELLITE_H

#include "Planet.h"

class Satellite : public Planet
{
public:
    Satellite();
    Satellite(float radius, float orbitRadius, float orbitAngle
              , float planetSpeed, float planetMass, Planet* planet);

    Planet* m_planet;

    void new_x();
    void new_z();
};

#endif // SATELLITE_H
