#include "polygonwindow.h"

PolygonWindow::PolygonWindow(QWindow *parent) :
    OpenGLWindow(parent), m_program(NULL), m_rtri(0.0f), m_rquad(0.0f)
{
}

void PolygonWindow::initialize()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
}

void PolygonWindow::render()
{
    m_program->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_modelview.setToIdentity();
    m_modelview.translate(-1.5f, 0.0f, -6.0f);
    m_modelview.rotate(m_rtri, 0.0f, 1.0f, 0.0f);
    m_program->setUniformValue("mvp_matrix", m_projection * m_modelview);
    GLuint color = m_program->attributeLocation("colAttr");
    glBegin(GL_TRIANGLES);

        glVertexAttrib3f(color, 1.0f, 0.0f, 0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertexAttrib3f(color, 0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glVertexAttrib3f(color, 0.0f, 0.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);

        glVertexAttrib3f(color, 1.0f, 0.0f, 0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertexAttrib3f(color, 0.0f, 0.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glVertexAttrib3f(color, 0.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, -1.0f);

        glVertexAttrib3f(color, 1.0f, 0.0f, 0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertexAttrib3f(color, 0.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, -1.0f);
        glVertexAttrib3f(color, 0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, -1.0f);

        glVertexAttrib3f(color, 1.0f, 0.0f, 0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertexAttrib3f(color, 0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertexAttrib3f(color, 0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);

    glEnd();

    m_modelview.setToIdentity();
    m_modelview.translate(1.5f, 0.0f, -7.0f);
    m_modelview.rotate(m_rquad, 1.0f, 1.0f, 1.0f);
    m_program->setUniformValue("mvp_matrix", m_projection * m_modelview);
    glBegin(GL_QUADS);

        glVertexAttrib3f(color, 0.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);

        glVertexAttrib3f(color, 1.0f, 0.5f, 0.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f( 1.0f,-1.0f,-1.0f);

        glVertexAttrib3f(color, 1.0f, 0.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);

        glVertexAttrib3f(color, 1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f( 1.0f, 1.0f,-1.0f);

        glVertexAttrib3f(color, 0.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);

        glVertexAttrib3f(color, 1.0f, 0.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f,-1.0f);
    glEnd();
    m_program->release();
    m_rtri+=0.2f;
    m_rquad-=0.15f;
}
