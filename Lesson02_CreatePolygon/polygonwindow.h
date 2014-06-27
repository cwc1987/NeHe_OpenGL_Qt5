#ifndef POLYGONWINDOW_H
#define POLYGONWINDOW_H

#include <QOpenGLShaderProgram>
#include <QGLShader>
#include <QScreen>
#include <QDebug>
#include "openglwindow.h"

class PolygonWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    explicit PolygonWindow(QWindow *parent = 0);
protected:
    void initialize();
    void render();
private:
    void initGeometry();
private:
    QOpenGLShaderProgram *m_program;
    GLuint m_posAttr;

    GLuint vboIds[2];
};

#endif // POLYGONWINDOW_H
