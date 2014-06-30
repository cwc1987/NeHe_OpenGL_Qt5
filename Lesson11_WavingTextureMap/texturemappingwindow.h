#ifndef TEXTUREMAPPINGWINDOW_H
#define TEXTUREMAPPINGWINDOW_H

#include "openglwindow.h"
#include <QOpenGLShaderProgram>
#include <QtMath>

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

    void initGeometry();
private:
    QOpenGLShaderProgram *m_program;
    GLuint m_posAttr;
    GLuint m_texCoordAttr;

    GLfloat m_xrot;
    GLfloat m_yrot;
    GLfloat m_zrot;
    GLuint m_texture;
    float m_points[45][45][3];
};

#endif // TEXTUREMAPPINGWINDOW_H
