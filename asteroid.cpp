#include "asteroid.h"

Asteroid::Asteroid():Planet()
  , speedX(0.f)
  , speedY(0.f)
  , speedZ(0.f)
  , accX(0.f)
  , accY(0.f)
  , accZ(0.f)
{

}
void Asteroid::new_x(bool flag)
{
    if(flag == false) {
        x = x + speedX + 0.5 * accX;
        speedX = speedX + accX;
    } else {
        speedX = speedX - accX;
        x = x - speedX - 0.5 * accX;
    }
}

void Asteroid::new_y(bool flag)
{
    if(flag == false) {
        y = y + speedY + 0.5 * accY;
        speedY = speedY + accY;
    } else {
        speedY = speedY - accY;
        y = y - speedY - 0.5 * accY;
    }
}

void Asteroid::new_z(bool flag)
{
    if(flag == false) {
        z = z + speedZ + 0.5 * accZ;
        speedZ = speedZ + accZ;
    } else {
        speedZ = speedZ - accZ;
        z = z - speedZ - 0.5 * accZ;
    }
}
