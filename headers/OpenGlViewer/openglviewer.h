#ifndef OPENGLVIEWER_H
#define OPENGLVIEWER_H


#include <QGLWidget>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <math.h>
#include <QDebug>
#include <QFile>
#include <GL/GLU.h>
//#include <GLFW/glfw3.h>
//#include <GL/glfw3.h>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
namespace Ui {
class OpenGlViewer;
}

class OpenGlViewer : public QGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGlViewer(  QWidget *parent = nullptr);
    ~OpenGlViewer() override;

    void setLight(bool value);

signals:
    void setDistanceInLabel(QString textDistance);
public slots:
    void setShowGrid(bool value);
    void setShowFaces(bool value);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;  // object roration
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;  // object scale
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private:

    void InitMaxOrigin();

    void updateDrawVertex();

private:
    const uint COUNT_ALIGN_CYCLES=5;
    const double ERROR_ALIGN=0.021f;

    const GLclampf BACKGROUND_COLOR[3]={0.709,0.588, 0.921}; //White background



private:

    GLfloat * drawVertex;   //vertex that contain Vertex shader
    uint sizeDrawVertex=0;  //size all of vertex that need to draw
    uint sizeDrawVertexFirstObject=0;   //size of first mesh that need to draw

    GLint GPUobjectColor;
    GLint GPUtransformMatrix;
    GLint GPUprojectionMatrix;
    GLint GPUlightColor;

    GLint GPUlightPosition;

    GLuint VBO, VAO;
    GLuint shaderProgram;

    QOpenGLExtraFunctions *f;


     QString distanceInfo;

    QVector2D mousePressPosition;       //save mouse position
    // QVector3D rotationAxis;

    QQuaternion rotation;
    QMatrix4x4 m_transform;
    QMatrix4x4 m_projection;
    qreal aspect;


    QMatrix4x4 matrix;

    float minMaxXYZ[6];//minX maxX minY maxY minZ maxZ

    float x_pos, y_pos, rotate_y, rotate_x;  // rotate values
    float translateX, translateY;
    float rotationSpeed=3.0f;
    float translateSpeed=0.3f;

    double ratioWidthHeight;

    float scaleSpeed=0.001;// current scale (zoom in\zoom out)

    bool cameraMove=false;

    Ui::OpenGlViewer *ui;

    GLfloat light_diffuse[3];
    GLfloat light_ambient[4];
    GLfloat light_position[4];

    GLfloat light_position2[4];

    int maxOrigin=-100000;  // max value of coords x/y/z
    int orthoCoefficient=3;
    float scaleWheel = 25;

    bool isDrawGrid=false;
    bool isDrawFaces=true;
    bool isLight=false;

};

#endif  // OPENGLVIEWER_H
