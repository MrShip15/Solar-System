#include "CameraRotator.h"
#include <QtCore/QtGlobal>
#include <QtCore/QtMath>
#include <QtCore/QDebug>

CameraRotator::CameraRotator(float viewDistance, float rotX, float rotY)
    : m_viewDistance(viewDistance)
    , m_rotX(rotX)
    , m_rotY(rotY)
{

}

void CameraRotator::doMousePress(float x, float y)
{
    if (m_moving)
    {
        return;
    }
    m_moving = true;
    m_prevX = x;
    m_prevY = y;
}

void CameraRotator::doMouseMove(float x, float y)
{
    if (!m_moving)
    {
        return;
    }
    float newRotX = m_rotX + m_degreesPerPixelX * (y - m_prevY);
    const float newRotY = m_rotY + m_degreesPerPixelY * (x - m_prevX);
    newRotX = qMax(-m_xLimit, qMin(m_xLimit, newRotX));
    m_prevX = x;
    m_prevY = y;
    if (newRotX != m_rotX || newRotY != m_rotY)
    {
        m_rotX = newRotX;
        m_rotY = newRotY;
        emit requestToRedraw();
    }
}

void CameraRotator::doMouseRelease()
{
    if (!m_moving)
    {
        return;
    }
    m_moving = false;
}

void CameraRotator::doMouseWheel(float delta)
{
    m_viewDistance += -delta / 10.f;
    emit requestToRedraw();
}

QMatrix4x4 CameraRotator::getViewMatrix()
{
    QMatrix4x4 mat;

    const float cosX = qCos(m_rotX / 180.f * M_PI);
    const float sinX = qSin(m_rotX / 180.f * M_PI);
    const float cosY = qCos(m_rotY / 180.f * M_PI);
    const float sinY = qSin(m_rotY / 180.f * M_PI);

    mat.setColumn(0, QVector4D(cosY, sinX * sinY, -cosX * sinY, 0.f));
    mat.setColumn(1, QVector4D(0.f, cosX, sinX, 0.f));
    mat.setColumn(2, QVector4D(sinY, -sinX * cosY, cosX * cosY, 0.f));
    mat.setColumn(3, QVector4D(0.f, 0.f, -m_viewDistance, 1.f));

    return mat;
}

void CameraRotator::onSizeChanged(float width, float height)
{
    m_degreesPerPixelX = 90.f / width;
    m_degreesPerPixelY = 180.f / height;
}
