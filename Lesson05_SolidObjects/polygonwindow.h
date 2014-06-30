#ifndef POLYGONWINDOW_H
#define POLYGONWINDOW_H

#include <QOpenGLShaderProgram>
#include "openglwindow.h"

class PolygonWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    explicit PolygonWindow(QWindow *parent = 0);
    ~PolygonWindow();
protected:
    void initialize();
    void render();
private:
    void initGeometry();
private:
    QOpenGLShaderProgram *m_program;
    GLuint m_colAttr;
    GLuint m_posAttr;
    GLuint m_vboIds[4];

    GLfloat	m_rtri;
    GLfloat	m_rquad;
};

#endif // POLYGONWINDOW_H
