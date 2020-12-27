#ifndef TEST_PLANET_H
#define TEST_PLANET_H

#include <QObject>
#include <QTest>
#include "Planet.h"
#include "satellite.h"
class test_Planet : public QObject
{
    Q_OBJECT
public:
    explicit test_Planet(QObject *parent = nullptr);

signals:
private slots:
    void test_new();
};

#endif // TEST_PLANET_H
