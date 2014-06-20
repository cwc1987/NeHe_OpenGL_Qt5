#ifndef TEXTUREMAPPINGWINDOW_H
#define TEXTUREMAPPINGWINDOW_H

#include "openglwindow.h"
#include <QOpenGLShaderProgram>

class TextureMappingWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    explicit TextureMappingWindow(QWindow *parent = 0);
    ~TextureMappingWindow();
protected:
    void initialize();
    void render();
    void keyPressEvent(QKeyEvent *event);
private:
    void loadGLTexture();

    void loadShader();
private:
    QOpenGLShaderProgram *m_program;
    GLfloat m_xrot;
    GLfloat m_yrot;
    GLfloat m_z;
    GLfloat m_xspeed;
    GLfloat m_yspeed;
    GLuint m_texture[3];
    GLuint m_filter;
    bool m_light;
};

#endif // TEXTUREMAPPINGWINDOW_H
