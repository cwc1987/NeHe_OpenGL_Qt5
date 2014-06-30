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
    void initGeometry();
private:
    QOpenGLShaderProgram *m_program;
    GLuint m_posAttr;
    GLuint m_texCoordAttr;
    GLuint m_normalAttr;

    GLuint m_vboIds[3];
    GLuint m_texture;
    GLfloat m_xrot;
    GLfloat m_yrot;
};

#endif // TEXTUREMAPPINGWINDOW_H
