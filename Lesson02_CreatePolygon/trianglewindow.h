#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <GL/gl.h>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include "openglwindow.h"

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow(QWindow *parent = 0);

    void initialize();
    void render();
private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // TRIANGLEWINDOW_H
