#ifndef PLANET_H
#define PLANET_H

#include <QtGui/QOpenGLTexture>
#include <QtCore/QtMath>

class Planet
{
public:
//    Planet();
//    Planet(float radius, float orbitRadius, float orbitAngle
//           , QColor color, float planetSpeed, float planetMass, float ecc);

    Planet(float radius = 1.0f, float orbitRadius = 5.f, float orbitAngle = 0.f,
           float planetSpeed = 5.f, float planetMass = 0.f, float ecc = 0.f);

    float x;
    float y;
    float z;
    float radius;
    float orbitRadius;
    float orbitAngle;
    float speed;
    float mass;
    float ecc;
    QOpenGLTexture texture;
    float angle = 0;
    float angleSpeed = 50.f;

    void createTexture(QString path);
    virtual void new_x();
    virtual void new_z();
};

#endif // PLANET_H
