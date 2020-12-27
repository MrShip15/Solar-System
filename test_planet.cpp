#include "test_planet.h"

test_Planet::test_Planet(QObject *parent) : QObject(parent)
{

}
void test_Planet::test_new() {
    Planet planet;
    QCOMPARE(int(planet.x), 0);
    planet.new_x();
    QCOMPARE(int(planet.x), 5);
    QCOMPARE(int(planet.z), 0);
    planet.orbitAngle = 90.f;
    planet.new_z();
    QCOMPARE(int(planet.z), 5);
    Satellite satellite;
    satellite.m_planet = &planet;
    satellite.new_x();
    satellite.new_z();
    QCOMPARE(int(satellite.x), 10);
    QCOMPARE(int(satellite.z), 5);
}
