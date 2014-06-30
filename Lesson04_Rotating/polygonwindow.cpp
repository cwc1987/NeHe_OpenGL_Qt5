#include "polygonwindow.h"

PolygonWindow::PolygonWindow(QWindow *parent) :
    OpenGLWindow(parent), m_program(NULL), m_rtri(0.0f), m_rquad(0.0f)
{
}

void PolygonWindow::initialize()
{
    initGeometry();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
}

void PolygonWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();

    m_modelview.setToIdentity();
    m_modelview.translate(-1.5f, 0.0f, -6.0f);
    m_modelview.rotate(m_rtri, 0.0f, 1.0f, 0.0f);
    m_program->setUniformValue("mvpMatrix", m_projection * m_modelview);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_program->enableAttributeArray(m_posAttr);
    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    m_program->enableAttributeArray(m_colAttr);
    m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 0, 3);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_modelview.setToIdentity();
    m_modelview.translate(1.5f, 0.0f, -6.0f);
    m_modelview.rotate(m_rquad, 1.0f, 0.0f, 0.0f);
    m_program->setUniformValue("mvpMatrix", m_projection * m_modelview);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
    m_program->enableAttributeArray(m_posAttr);
    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[3]);
    m_program->enableAttributeArray(m_colAttr);
    m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();
    m_rtri+=0.2f;
    m_rquad-=0.15f;
}

void PolygonWindow::initGeometry()
{
    glGenBuffers(4, &m_vboIds[0]);
    GLfloat triangleVertices[] = {
        0.0f, 1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f,
        1.0f,-1.0f, 0.0f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    GLfloat triangleColors[] = {
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColors), triangleColors, GL_STATIC_DRAW);

    GLfloat quadVertices[] = {
        1.0f, 1.0f, 0.0f,
       -1.0f, 1.0f, 0.0f,
        1.0f,-1.0f, 0.0f,
       -1.0f,-1.0f, 0.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    GLfloat quadColors[] = {
        0.5f,0.5f,1.0f,
        0.5f,0.5f,1.0f,
        0.5f,0.5f,1.0f,
        0.5f,0.5f,1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadColors), quadColors, GL_STATIC_DRAW);
}
