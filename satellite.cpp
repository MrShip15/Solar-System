#include "satellite.h"

Satellite::Satellite(): Planet(1.f, 5.f, 0.f, 5.f, 0.f, 0.f), m_planet(nullptr)
{

}

Satellite::Satellite(float radius, float orbitRadius, float orbitAngle, float planetSpeed,float planetMass, Planet* planet):Planet(radius, orbitRadius, orbitAngle, planetSpeed, planetMass, 0.f)
  ,m_planet(planet)
{

}
void Satellite::new_x() {
    x = m_planet->x + orbitRadius * qCos(qDegreesToRadians(orbitAngle)) - orbitRadius * ecc;
}
void Satellite::new_z() {
    z = m_planet->z + orbitRadius * sqrt(1 - ecc*ecc) * qSin(qDegreesToRadians(orbitAngle));
}
