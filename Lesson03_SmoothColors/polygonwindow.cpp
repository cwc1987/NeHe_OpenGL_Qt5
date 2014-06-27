#include "polygonwindow.h"

PolygonWindow::PolygonWindow(QWindow *parent) :
    OpenGLWindow(parent), m_program(NULL)
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();
    m_modelview.setToIdentity();
    m_modelview.translate(-1.5f, 0.0f, -6.0f);
    m_program->setUniformValue("mvp_matrix", m_projection * m_modelview);
    GLuint color = m_program->attributeLocation("colAttr");
    glBegin(GL_TRIANGLES);
        glVertexAttrib3f(color, 1.0f, 0.0f, 0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertexAttrib3f(color, 0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
        glVertexAttrib3f(color, 0.0f, 0.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
    glEnd();
    m_modelview.translate(3.0f, 0.0f, 0.0f);
    m_program->setUniformValue("mvp_matrix", m_projection * m_modelview);
    glVertexAttrib3f(color, 0.5f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
    glEnd();
    m_program->release();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();
    glLoadIdentity();
    glEnableVertexAttribArray(0);

    glTranslatef(-1.5f, 0.0f, -6.0f);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glTranslatef(3.0f, 0.0f, -0.0f);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(0);
    m_program->release();
}

void PolygonWindow::initVAO()
{
    glGenBuffers(2, &m_vertexbuffer[0]);

    GLfloat triangleVertices[] =
    {
        0.0f, 1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f,
         1.0f,-1.0f, 0.0f
    };
    GLfloat triangleColors[] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    GLfloat quadVertices[] =
    {
        -1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
         1.0f,-1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
}
