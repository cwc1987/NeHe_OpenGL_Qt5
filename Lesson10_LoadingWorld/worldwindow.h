#ifndef WORLDWINDOW_H
#define WORLDWINDOW_H

#include <QOpenGLShaderProgram>
#include <QtMath>
#include "openglwindow.h"

class WorldWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    typedef struct tagVERTEX
    {
        float x, y, z;
        float u, v;
    }VERTEX;

    typedef struct tagTRIANGLE
    {
        VERTEX vertex[3];
    }TRIANGLE;

    typedef struct tagSECTOR
    {
        int numtriangles;
        TRIANGLE* triangle;
    }SECTOR;

    explicit WorldWindow(QWindow *parent = 0);
    ~WorldWindow();
protected:
    void initialize();
    void render();
    void keyPressEvent(QKeyEvent *event);
private:
    void setupWorld();
    void loadShader();
    void loadGLTexture();
private:
    QOpenGLShaderProgram *m_program;
    GLuint m_posAttr;
    GLuint m_texCoordAttr;

    bool m_blend;
    GLfloat m_yrot;
    GLfloat m_xpos;
    GLfloat m_zpos;
    GLfloat m_heading;
    GLfloat m_walkbias;
    GLfloat m_walkbiasangle;
    GLfloat m_lookupdown;

    SECTOR m_sector;
    GLuint	m_filter;
    GLuint	m_texture[3];
};

#endif // WORLDWINDOW_H
