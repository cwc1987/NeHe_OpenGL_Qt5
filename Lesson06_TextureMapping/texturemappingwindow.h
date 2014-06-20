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
private:
    void loadGLTexture();

    void loadShader();
private:
    QOpenGLShaderProgram *m_program;
    GLfloat m_xrot;
    GLfloat m_yrot;
    GLfloat m_zrot;
    GLuint m_texture;
};

#endif // TEXTUREMAPPINGWINDOW_H
