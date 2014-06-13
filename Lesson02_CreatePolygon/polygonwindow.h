#ifndef POLYGONWINDOW_H
#define POLYGONWINDOW_H

#include <QOpenGLShaderProgram>
#include <QScreen>
#include "openglwindow.h"

class PolygonWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    explicit PolygonWindow(QWindow *parent = 0);

    void initialize();
    void render();
protected:
    void resizeEvent(QResizeEvent *event);
private:
    GLuint m_posAttr;
    GLuint m_matrixUniform;
    GLuint m_colAttr;

    QOpenGLShaderProgram *m_program;
};

#endif // POLYGONWINDOW_H
