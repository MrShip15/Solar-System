#ifndef ASTEROID_H
#define ASTEROID_H

#include "Planet.h"

class Asteroid : public Planet
{
public:
    Asteroid();

    float speedX;
    float speedY;
    float speedZ;
    float accX;
    float accY;
    float accZ;

    void new_x(bool flag);
    void new_y(bool flag);
    void new_z(bool flag);
};
#endif // ASTEROID_H
