#include "texturemappingwindow.h"

TextureMappingWindow::TextureMappingWindow(QWindow *parent) :
    OpenGLWindow(parent), m_xrot(0.0f), m_yrot(0.0f), m_zrot(0.0f), m_texture(-1)
{
}

TextureMappingWindow::~TextureMappingWindow()
{
    glDeleteTextures(1, &m_texture);
}

void TextureMappingWindow::initialize()
{
    loadShader();
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void TextureMappingWindow::render()
{
    m_program->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_modelview.setToIdentity();
    m_modelview.translate(0.0f, 0.0f, -5.0f);
    m_modelview.rotate(m_xrot, 1.0, 0.0, 0.0);
    m_modelview.rotate(m_yrot, 0.0, 1.0, 0.0);
    m_modelview.rotate(m_zrot, 0.0, 0.0, 1.0);
    m_program->setUniformValue("mvp_matrix", m_projection * m_modelview);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    GLuint textcoord = m_program->attributeLocation("a_texcoord");
    glBegin(GL_QUADS);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f,  1.0f);

            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);

            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f(-1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f( 1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f, -1.0f);

            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f,  1.0f);

            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);

            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
    m_program->release();
    m_xrot+=0.3f;
    m_yrot+=0.2f;
    m_zrot+=0.4f;
}

void TextureMappingWindow::loadGLTexture()
{
    QImage image(":/image/NeHe.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextureMappingWindow::loadShader()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
}
