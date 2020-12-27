#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLTexture>
#include <QtGui/QMatrix4x4>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtCore/QTimer>
#include <QtCore/QElapsedTimer>
#include "Planet.h"
#include "satellite.h"
#include "asteroid.h"
#include "CameraRotator.h"

struct DaeData {
    QString vertices;
    QString normals;
    QString texCoords;
    QString indices;
};

class Widget : public QOpenGLWidget
{
    Q_OBJECT
signals:
    void sizeChanged(float w, float h);
public:
    Widget();
private:
    QOpenGLShaderProgram m_planetProgram;
    QOpenGLShaderProgram m_sunProgram;
    QOpenGLBuffer m_vertexPositionBuffer;
    QOpenGLBuffer m_normalBuffer;
    QOpenGLBuffer m_texCoordBuffer;
    int m_amountOfVertices;
    QMatrix4x4 m_planetMvpMatrix;
    QMatrix4x4 m_planetModelMatrix;
    QMatrix4x4 m_planetNormalMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projMatrix;
    int uPlanetMvpMatrixLocation;
    int uPlanetModelMatrixLocation;
    int uPlanetNormalMatrixLocation;
    int uPlanetColorLocation;
    int uSunMvpMatrixLocation;
    QMatrix4x4 m_sunMvpMatrix;
    QMatrix4x4 m_sunModelMatrix;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    float m_delta;
    float m_x = 0;
    float m_z = 0;
    float m_angle = 0;
    bool flag_reverse = false;
    bool flag_visible = false;
    int m_amountOfPlanets;
    Planet m_planets[9];
    int m_amountOfSatellites;
    Satellite m_satellites[1];
    int m_amountOfAsteroids;
    Asteroid m_asteroids[1];
    CameraRotator m_cameraRotator;
    QOpenGLTexture m_sunTexture;

private slots:
    void animationLoop();
    void onRotated();
private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent (QMouseEvent *event) override;
    void mouseMoveEvent (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    int initVertexBuffers();
    DaeData getDaeData(QString path);
};
#endif // WIDGET_H
