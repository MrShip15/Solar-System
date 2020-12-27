#include "Widget.h"
#include <QtXml/QDomDocument>
#include <QtCore/QFile>
#include <QtCore/QDebug>

const float G = 6.67f * pow(10.f, -11);
const float k = 0.0336f * 149597870.f;
const float sunMass = 1.99f * pow(10.f, 30);

float dist(float xb, float yb, float zb, float xe, float ye, float ze){
    return sqrtf((xe-xb)*(xe - xb) + (ye-yb)*(ye - yb) + (ze-zb)*(ze - zb));
}

Widget::Widget() : m_sunTexture(QOpenGLTexture::Target2D)
{
    setWindowTitle("Solar System");
    connect(&m_cameraRotator, &CameraRotator::requestToRedraw, this, &Widget::onRotated);
    connect(this, &Widget::sizeChanged, &m_cameraRotator, &CameraRotator::onSizeChanged);
}

void Widget::animationLoop()
{
    m_delta = m_elapsedTimer.elapsed() / 1000.f;
    m_elapsedTimer.restart();

    for (int i = 0; i < m_amountOfPlanets; i++) {
        if(flag_reverse == false) {
        m_planets[i].orbitAngle += m_planets[i].speed * m_delta / m_planets[i].orbitRadius * 50.f;
        } else {
          m_planets[i].orbitAngle -= m_planets[i].speed * m_delta / m_planets[i].orbitRadius* 50.f;
        }
        m_planets[i].angle += m_planets[i].angleSpeed * m_delta;
        m_planets[i].new_x();
        m_planets[i].new_z();
    }

    for (int i = 0; i < m_amountOfSatellites; i++) {
        if(flag_reverse == false) {
        m_satellites[i].orbitAngle += m_satellites[i].speed * m_delta/ m_satellites[i].orbitRadius* 50.f;
        } else {
          m_satellites[i].orbitAngle -= m_satellites[i].speed * m_delta/ m_satellites[i].orbitRadius* 50.f;
        }
        m_satellites[i].new_x();
        m_satellites[i].new_z();
    }
    if(flag_visible == true) {
        for (int i = 0; i < m_amountOfAsteroids; i++) {
            m_asteroids[i].accX = 0;
            m_asteroids[i].accY = 0;
            m_asteroids[i].accZ = 0;
            for(int j = 0; j < m_amountOfPlanets; j++){
                float temp_dist = dist(m_asteroids[i].x, m_asteroids[i].y, m_asteroids[i].z, m_planets[j].x, m_planets[j].y, m_planets[j].z);
                float f_g = G*(m_planets[j].mass/(k*k*temp_dist * temp_dist));
                m_asteroids[i].accX += f_g*(m_planets[j].x-m_asteroids[i].x/temp_dist);
                m_asteroids[i].accY += f_g*(m_planets[j].y-m_asteroids[i].y/temp_dist);
                m_asteroids[i].accZ += f_g*(m_planets[j].z-m_asteroids[i].z/temp_dist);
            }
            float temp_dist = dist(m_asteroids[i].x, m_asteroids[i].y, m_asteroids[i].z, 0.f, 0.f, 0.f);
            float f_g = G*(sunMass/(k*k*temp_dist * temp_dist));
            m_asteroids[i].accX += f_g*(0.f-m_asteroids[i].x/temp_dist);
            m_asteroids[i].accY += f_g*(0.f-m_asteroids[i].y/temp_dist);
            m_asteroids[i].accZ += f_g*(0.f-m_asteroids[i].z/temp_dist);
            m_asteroids[i].new_x(flag_reverse);
            m_asteroids[i].new_y(flag_reverse);
            m_asteroids[i].new_z(flag_reverse);
        }
    }
    update();
}

void Widget::onRotated()
{
    m_viewMatrix = m_cameraRotator.getViewMatrix();
    update();
}

void Widget::initializeGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glEnable(GL_DEPTH_TEST);

    m_planetProgram.create();
    m_planetProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/textured_shader.vert");
    m_planetProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/textured_shader.frag");
    m_planetProgram.link();

    m_sunProgram.create();
    m_sunProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/textured_shadeless.vert");
    m_sunProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/textured_shadeless.frag");
    m_sunProgram.link();

    m_amountOfVertices = initVertexBuffers();

    m_planetProgram.bind();
    m_planetProgram.bindAttributeLocation("aPosition", 0);
    m_planetProgram.bindAttributeLocation("aNormal", 1);
    m_planetProgram.bindAttributeLocation("aTexCoord", 2);

    m_sunProgram.bind();
    m_sunProgram.bindAttributeLocation("aPosition", 0);
    m_sunProgram.bindAttributeLocation("aTexCoord", 1);

    m_planetProgram.bind();
    uPlanetMvpMatrixLocation = m_planetProgram.uniformLocation("uMvpMatrix");
    uPlanetModelMatrixLocation = m_planetProgram.uniformLocation("uModelMatrix");
    uPlanetNormalMatrixLocation = m_planetProgram.uniformLocation("uNormalMatrix");
    uPlanetColorLocation = m_planetProgram.uniformLocation("uColor");
    m_planetModelMatrix.translate(-10.f, 0.f, 0.f);
    m_planetModelMatrix.scale(1.f, 1.f, 1.f);
    m_planetNormalMatrix = m_planetModelMatrix.inverted();
    m_planetNormalMatrix = m_planetNormalMatrix.transposed();

//    m_viewMatrix.lookAt(
//                QVector3D(0.f, 90.f, 300.f),
//                QVector3D(0.f, 0.f, 0.f),
//                QVector3D(0.f, 1.f, 0.f));
    m_viewMatrix = m_cameraRotator.getViewMatrix();

    m_sunProgram.bind();
    uSunMvpMatrixLocation = m_sunProgram.uniformLocation("uMvpMatrix");
    m_sunModelMatrix.scale(69.5f, 69.5f, 69.5f);

    m_amountOfPlanets = sizeof(m_planets) / sizeof(m_planets[0]);

    m_sunTexture.create();
    m_sunTexture.setData(QImage(":/Textures/sun.jpg"));
    m_sunTexture.setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    m_sunTexture.setWrapMode(QOpenGLTexture::ClampToEdge);

    m_planets[0].radius = 0.24f;
    m_planets[0].orbitRadius = 90.9f;
    m_planets[0].orbitAngle = 0.f;
    m_planets[0].speed = 47.87f;
    m_planets[0].createTexture(":/Textures/mercury.jpg");
    m_planets[0].mass = 3.3f*pow(10, 23);
    m_planets[0].ecc = 0.206f;

    m_planets[1].radius = 0.61f;
    m_planets[1].orbitRadius = 100.2f;
    m_planets[1].orbitAngle = 0.f;
    m_planets[1].speed = 35.02f;
    m_planets[1].createTexture(":/Textures/vernus.jpg");
    m_planets[1].mass = 4.87f*pow(10, 24);
    m_planets[1].ecc = 0.007f;

    m_planets[2].radius = 0.64f;
    m_planets[2].orbitRadius = 110.f;
    m_planets[2].orbitAngle = 0.f;
    m_planets[2].speed = 29.79f;
    m_planets[2].createTexture(":/Textures/earth.jpg");
    m_planets[2].mass = 5.976f*pow(10,24);
    m_planets[2].ecc = 0.017f;

    m_planets[3].radius = 0.34f;
    m_planets[3].orbitRadius = 140.9f;
    m_planets[3].orbitAngle = 0.f;
    m_planets[3].speed = 24.13f;
    m_planets[3].createTexture(":/Textures/mars.jpg");
    m_planets[3].mass = 6.4f*pow(10,23);
    m_planets[3].ecc = 0.093f;

    m_planets[4].radius = 7.1f;
    m_planets[4].orbitRadius = 180.5f;
    m_planets[4].orbitAngle = 0.f;
    m_planets[4].speed = 13.06f;
    m_planets[4].createTexture(":/Textures/jupiter.jpg");
    m_planets[4].mass = 1.9f*pow(10, 27);
    m_planets[4].ecc =0.048f;

    m_planets[5].radius = 6.f;
    m_planets[5].orbitRadius = 280.f;
    m_planets[5].orbitAngle = 0.f;
    m_planets[5].speed = 9.66f;
    m_planets[5].createTexture(":/Textures/saturn.jpg");
    m_planets[5].mass = 5.68f*pow(10, 26);
    m_planets[5].ecc = 0.056f;

    m_planets[6].radius = 2.6f;
    m_planets[6].orbitRadius = 480.f;
    m_planets[6].orbitAngle = 0.f;
    m_planets[6].speed = 6.80f;
    m_planets[6].createTexture(":/Textures/uranus.jpg");
    m_planets[6].mass = 8.7f*pow(10, 25);
    m_planets[6].ecc = 0.046f;

    m_planets[7].radius = 2.5f;
    m_planets[7].orbitRadius = 680.f;
    m_planets[7].orbitAngle = 0.f;
    m_planets[7].speed = 5.44f;
    m_planets[7].createTexture(":/Textures/neptune.jpg");
    m_planets[7].mass = 1.f*pow(10, 26);
    m_planets[7].ecc = 0.009f;

    m_planets[8].radius = 0.11f;
    m_planets[8].orbitRadius = 880.f;
    m_planets[8].orbitAngle = 0.f;
    m_planets[8].speed = 4.74f;
    m_planets[8].createTexture(":/Textures/pluto.jpg");
    m_planets[8].mass = 1.3f*pow(10,22);
    m_planets[8].ecc = 0.246f;

    m_amountOfSatellites = sizeof(m_satellites) / sizeof(m_satellites[0]);

    m_satellites[0].radius = 0.17f;
    m_satellites[0].orbitRadius = 1.5f;
    m_satellites[0].orbitAngle = 0.f;
    m_satellites[0].speed = 1.023f;
    m_satellites[0].createTexture(":/Textures/moon.jpg");
    m_satellites[0].m_planet = &m_planets[2];

    m_amountOfAsteroids = sizeof(m_asteroids) / sizeof(m_asteroids[0]);

    m_asteroids[0].radius = 0.31f;
    m_asteroids[0].orbitRadius = 0.f;
    m_asteroids[0].orbitAngle = 0.f;
    m_asteroids[0].speed = 100.f;
    m_asteroids[0].createTexture(":/Textures/moon.jpg");
    m_asteroids[0].x = 0.f;
    m_asteroids[0].y = 0.f;
    m_asteroids[0].z = 200.f;

    connect(&m_timer, &QTimer::timeout, this, &Widget::animationLoop);
    m_timer.start(1000.f/60.f);
    m_elapsedTimer.start();
}

void Widget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_vertexPositionBuffer.bind();
    m_planetProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_planetProgram.enableAttributeArray(0);
    m_normalBuffer.bind();
    m_planetProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3);
    m_planetProgram.enableAttributeArray(1);
    m_texCoordBuffer.bind();
    m_planetProgram.setAttributeBuffer(2, GL_FLOAT, 0, 2);
    m_planetProgram.enableAttributeArray(2);

    for (int i = 0; i < m_amountOfPlanets; i++) {
        m_planetModelMatrix.setToIdentity();
        m_planetModelMatrix.translate(m_planets[i].x, 0.f, m_planets[i].z);
        m_planetModelMatrix.rotate(m_planets[i].angle, QVector3D(0.f, 1.f, 0.f));
        //        m_planetModelMatrix.scale(m_planets[i].radius * 2.f, m_planets[i].radius * 2, 1.f);
        m_planetModelMatrix.scale(m_planets[i].radius * 2.f);
        m_planetMvpMatrix = m_projMatrix * m_viewMatrix * m_planetModelMatrix;
        m_planetProgram.bind();
        m_planetProgram.setUniformValue(uPlanetMvpMatrixLocation, m_planetMvpMatrix);
        m_planetProgram.setUniformValue(uPlanetModelMatrixLocation, m_planetModelMatrix);
        m_planetProgram.setUniformValue(uPlanetNormalMatrixLocation, m_planetNormalMatrix);
//        m_planetProgram.setUniformValue(uPlanetColorLocation, m_planets[i].color);
        m_planets[i].texture.bind();
        glDrawArrays(GL_TRIANGLES, 0, m_amountOfVertices);
    }

    for (int i = 0; i < m_amountOfSatellites; i++) {
        m_planetModelMatrix.setToIdentity();
        m_planetModelMatrix.translate(m_satellites[i].x, 0.f, m_satellites[i].z);
        //        m_planetModelMatrix.scale(m_satellites[i].radius * 2.f, m_satellites[i].radius * 2, 1.f);
        m_planetModelMatrix.scale(m_satellites[i].radius * 2.f);
        m_planetMvpMatrix = m_projMatrix * m_viewMatrix * m_planetModelMatrix;
        m_planetProgram.bind();
        m_planetProgram.setUniformValue(uPlanetMvpMatrixLocation, m_planetMvpMatrix);
        m_planetProgram.setUniformValue(uPlanetModelMatrixLocation, m_planetModelMatrix);
        m_planetProgram.setUniformValue(uPlanetNormalMatrixLocation, m_planetNormalMatrix);
//        m_planetProgram.setUniformValue(uPlanetColorLocation, m_satellites[i].color);
        m_satellites[i].texture.bind();
        glDrawArrays(GL_TRIANGLES, 0, m_amountOfVertices);
    }

    if(flag_visible == true){
        for (int i = 0; i < m_amountOfAsteroids; i++) {
            m_planetModelMatrix.setToIdentity();
            m_planetModelMatrix.translate(m_asteroids[i].x, m_asteroids[i].y, m_asteroids[i].z);
            //        m_planetModelMatrix.scale(m_asteroids[i].radius * 2.f, m_asteroids[i].radius * 2, 1.f);
            m_planetModelMatrix.scale(m_asteroids[i].radius * 2.f);
            m_planetMvpMatrix = m_projMatrix * m_viewMatrix * m_planetModelMatrix;
            m_planetProgram.bind();
            m_planetProgram.setUniformValue(uPlanetMvpMatrixLocation, m_planetMvpMatrix);
            m_planetProgram.setUniformValue(uPlanetModelMatrixLocation, m_planetModelMatrix);
            m_planetProgram.setUniformValue(uPlanetNormalMatrixLocation, m_planetNormalMatrix);
//            m_planetProgram.setUniformValue(uPlanetColorLocation, m_asteroids[i].color);
            m_asteroids[i].texture.bind();
            glDrawArrays(GL_TRIANGLES, 0, m_amountOfVertices);
        }
    }
    m_sunMvpMatrix = m_projMatrix * m_viewMatrix * m_sunModelMatrix;
    m_sunProgram.bind();
    m_sunProgram.setUniformValue(uSunMvpMatrixLocation, m_sunMvpMatrix);
    m_vertexPositionBuffer.bind();
    m_sunProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_sunProgram.enableAttributeArray(0);
    m_texCoordBuffer.bind();
    m_planetProgram.setAttributeBuffer(1, GL_FLOAT, 0, 2);
    m_planetProgram.enableAttributeArray(1);
    m_sunTexture.bind();

    glDrawArrays(GL_TRIANGLES, 0, m_amountOfVertices);
}

void Widget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_projMatrix.setToIdentity();
    m_projMatrix.perspective(60.f, (float) w / h, 0.1f, 2000.f);
    emit sizeChanged(w, h);
}

int Widget::initVertexBuffers()
{
    DaeData data = getDaeData(":/Models/Sphere.dae");

    QStringList dataVertPosList = data.vertices.split(" ");
    float dataVertPosArray[dataVertPosList.length()];
    for (int i = 0; i < dataVertPosList.length(); i++) {
        dataVertPosArray[i] = dataVertPosList[i].toFloat();
    }

    QStringList dataNormalList = data.normals.split(" ");
    float dataNormalArray[dataNormalList.length()];
    for (int i = 0; i < dataNormalList.length(); i++) {
        dataNormalArray[i] = dataNormalList[i].toFloat();
    }

    QStringList dataTexCoordList = data.texCoords.split(" ");
    float dataTexCoordArray[dataTexCoordList.length()];
    for (int i = 0; i < dataTexCoordList.length(); i++) {
        dataTexCoordArray[i] = dataTexCoordList[i].toFloat();
    }

    QStringList dataIndexList = data.indices.split(" ");
    float dataIndexArray[dataIndexList.length()];
    for (int i = 0; i < dataIndexList.length(); i++) {
        dataIndexArray[i] = dataIndexList[i].toFloat();
    }

    int numAttributes = 3;
    float vertPositions[3 * dataIndexList.length() / numAttributes];
    float normals[3 * dataIndexList.length() / numAttributes];
    float texCoords[2 * dataIndexList.length() / numAttributes];
    int vi = 0, ni = 0, ti = 0;
    for (int i = 0; i < dataIndexList.length(); i += numAttributes) {
        int vertPosIndex = dataIndexArray[i + 0];
        // qDebug() << "x: " << dataVertPosArray[vertPosIndex * 3 + 0];
        // qDebug() << "y: " << dataVertPosArray[vertPosIndex * 3 + 1];
        // qDebug() << "z: " << dataVertPosArray[vertPosIndex * 3 + 2];
        vertPositions[vi++] = dataVertPosArray[vertPosIndex * 3 + 0];
        vertPositions[vi++] = dataVertPosArray[vertPosIndex * 3 + 1];
        vertPositions[vi++] = dataVertPosArray[vertPosIndex * 3 + 2];

        int normalIndex = dataIndexArray[i + 1];
        normals[ni++] = dataNormalArray[normalIndex * 3 + 0];
        normals[ni++] = dataNormalArray[normalIndex * 3 + 1];
        normals[ni++] = dataNormalArray[normalIndex * 3 + 2];

        int texCoordIndex = dataIndexArray[i + 2];
        texCoords[ti++] = dataTexCoordArray[texCoordIndex * 2 + 0];
        texCoords[ti++] = dataTexCoordArray[texCoordIndex * 2 + 1];
    }

    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.bind();
    m_vertexPositionBuffer.allocate(vertPositions, sizeof(vertPositions));

    m_normalBuffer.create();
    m_normalBuffer.bind();
    m_normalBuffer.allocate(normals, sizeof(normals));

    m_texCoordBuffer.create();
    m_texCoordBuffer.bind();
    m_texCoordBuffer.allocate(texCoords, sizeof(texCoords));

    return dataIndexList.length() / numAttributes;
}

DaeData Widget::getDaeData(QString path)
{
    DaeData daeData;
    QDomDocument xmlDoc;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file.";
    }
    xmlDoc.setContent(&f);
    f.close();
    QDomElement root = xmlDoc.documentElement();
    QDomElement daeElem = root.firstChildElement();
    while(!daeElem.isNull()) {
        if (daeElem.tagName() == "library_geometries") {
            QDomElement geomElem = daeElem.firstChildElement();
            if (geomElem.tagName() == "geometry") {
                QDomElement meshElem = geomElem.firstChildElement();
                if (meshElem.tagName() == "mesh") {
                    QDomElement meshChildElem = meshElem.firstChildElement();
                    while(!meshChildElem.isNull()) {
                        if (meshChildElem.attribute("id").endsWith("-mesh-positions")) {
                            QDomElement floatArrayElem = meshChildElem.firstChildElement();
                            QString strPositions = floatArrayElem.firstChild().toText().data();
                            daeData.vertices = strPositions;
                        }
                        if (meshChildElem.attribute("id").endsWith("-mesh-normals")) {
                            QDomElement floatArrayElem = meshChildElem.firstChildElement();
                            QString strNormals = floatArrayElem.firstChild().toText().data();
                            daeData.normals = strNormals;
                        }
                        if (meshChildElem.attribute("id").endsWith("-mesh-map-0")) {
                            QDomElement floatArrayElem = meshChildElem.firstChildElement();
                            QString strTexCoords = floatArrayElem.firstChild().toText().data();
                            daeData.texCoords = strTexCoords;
                        }
                        if (meshChildElem.tagName() == "triangles" || meshChildElem.tagName() == "polylist") {
                            QDomElement trianglesChildElem = meshChildElem.firstChildElement();
                            while(!trianglesChildElem.isNull()) {
                                if (trianglesChildElem.tagName() == "p") {
                                    QString strAllIndices = trianglesChildElem.firstChild().toText().data();
                                    daeData.indices = strAllIndices;
                                }
                                trianglesChildElem = trianglesChildElem.nextSiblingElement();
                            }
                        }
                        meshChildElem = meshChildElem.nextSiblingElement();
                    }
                }
            }
        }
        daeElem = daeElem.nextSiblingElement();
    }
    return daeData;
}
void Widget::mousePressEvent (QMouseEvent *event)
{
    switch(event->button())
    {
        case Qt::LeftButton:
            if(flag_reverse == true){
                flag_reverse = false;
            } else {
                flag_reverse = true;
            }
            break;
        case Qt::MiddleButton:
            m_cameraRotator.doMousePress(event->x(), event->y());
            break;
        default:
            break;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_cameraRotator.doMouseMove(event->x(), event->y());
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_cameraRotator.doMouseRelease();
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    flag_visible = true;
}

void Widget::wheelEvent(QWheelEvent *event)
{
    m_cameraRotator.doMouseWheel(event->angleDelta().y());
}
