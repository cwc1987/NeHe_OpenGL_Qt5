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
    GLuint m_posAttr;
    GLuint m_matrixUniform;
    GLuint m_colAttr;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // POLYGONWINDOW_H
