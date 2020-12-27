#ifndef CAMERAROTATOR_H
#define CAMERAROTATOR_H

#include <QtGui/QMatrix4x4>
#include <QtCore/QObject>

class CameraRotator : public QObject
{
    Q_OBJECT
public slots:
    void onSizeChanged(float width, float height);
signals:
    void requestToRedraw();
private:
    float m_viewDistance;
    float m_rotX;
    float m_rotY;
    const float m_xLimit = 85.f;
    bool m_moving = false;
    float m_degreesPerPixelX;
    float m_degreesPerPixelY;
    float m_prevX;
    float m_prevY;
public:
    CameraRotator(float viewDistance = 300.f, float rotX = 0.f, float rotY = 0.f);
    virtual ~CameraRotator() {};

    void doMousePress(float x, float y);
    void doMouseMove(float x, float y);
    void doMouseRelease();
    void doMouseWheel(float delta);

    QMatrix4x4 getViewMatrix();
};

#endif // CAMERAROTATOR_H
