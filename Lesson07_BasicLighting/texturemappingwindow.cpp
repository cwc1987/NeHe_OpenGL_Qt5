#include "texturemappingwindow.h"

QVector4D LightAmbient = QVector4D(0.5f, 0.5f, 0.5f, 1.0f);
QVector4D LightDiffuse = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
QVector4D LightPosition = QVector4D(0.0f, 0.0f, 2.0f, 1.0f);
QVector4D LightModelAmbient = QVector4D(0.2, 0.2, 0.2, 1.0);
QVector4D MaterialDiffuse = QVector4D(0.8f, 0.8f, 0.8f, 1.0f);
QVector4D MaterialAmbient = QVector4D(0.2f, 0.2f, 0.2f, 1.0f);

TextureMappingWindow::TextureMappingWindow(QWindow *parent) :
    OpenGLWindow(parent), m_xrot(0.0f), m_yrot(0.0f), m_xspeed(0.0f),
    m_yspeed(0.0f), m_z(-5.0f), m_light(false), m_filter(0)
{
}

TextureMappingWindow::~TextureMappingWindow()
{
    glDeleteTextures(3, &m_texture[0]);
}

void TextureMappingWindow::initialize()
{
    initGeometry();
    loadShader();
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void TextureMappingWindow::render()
{
    m_program->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->setUniformValue("lightPos", LightPosition);
    m_program->setUniformValue("lightDiffuse", LightDiffuse);
    m_program->setUniformValue("lightAmbient", LightAmbient);
    m_program->setUniformValue("lightModelAmbient", LightModelAmbient);
    m_program->setUniformValue("materialDiffuse", MaterialDiffuse);
    m_program->setUniformValue("materialAmbient", MaterialAmbient);

    m_modelView.setToIdentity();
    m_modelView.translate(0.0f, 0.0f, m_z);
    m_modelView.rotate(m_xrot, 1.0, 0.0, 0.0);
    m_modelView.rotate(m_yrot, 0.0, 1.0, 0.0);

    m_program->setUniformValue("mvMatrix", m_modelView);
    m_program->setUniformValue("mvpMatrix", m_projection * m_modelView);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_program->enableAttributeArray(m_posAttr);
    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    m_program->enableAttributeArray(m_texCoordAttr);
    m_program->setAttributeArray(m_texCoordAttr, GL_FLOAT, 0, 2);
    glBindTexture(GL_TEXTURE_2D, m_texture[m_filter]);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
    m_program->enableAttributeArray(m_normalAttr);
    m_program->setAttributeArray(m_normalAttr, GL_FLOAT, 0, 3);
    glDrawArrays(GL_QUADS, 0, 34);
    m_program->release();
    m_xrot+=m_xspeed;
    m_yrot+=m_yspeed;
}

void TextureMappingWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_L:
        {
            m_light = !m_light;
            break;
        }
        case Qt::Key_F:
        {
            m_filter+=1;
            if(m_filter > 2)
            {
                m_filter = 0;
            }
            break;
        }
        case Qt::Key_PageUp:
        {
            m_z-=0.2f;
            break;
        }
        case Qt::Key_PageDown:
        {
            m_z+=0.2f;
            break;
        }
        case Qt::Key_Up:
        {
            m_xspeed-=0.01f;
            break;
        }
        case Qt::Key_Down:
        {
            m_xspeed+=0.01f;
            break;
        }
        case Qt::Key_Right:
        {
            m_yspeed+=0.01f;
            break;
        }
        case Qt::Key_Left:
        {
            m_yspeed-=0.01f;
            break;
        }
    }
    OpenGLWindow::keyPressEvent(event);
}

void TextureMappingWindow::loadGLTexture()
{
    QImage image(":/image/Crate.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();

    glGenTextures(3, &m_texture[0]);
    glBindTexture(GL_TEXTURE_2D, m_texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(),
                 image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

    glBindTexture(GL_TEXTURE_2D, m_texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(),
                 image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

    glBindTexture(GL_TEXTURE_2D, m_texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void TextureMappingWindow::loadShader()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_texCoordAttr = m_program->attributeLocation("texCoordAttr");
    m_normalAttr = m_program->attributeLocation("normalAttr");
}

void TextureMappingWindow::initGeometry()
{
    glGenBuffers(3, &m_vboIds[0]);
    GLfloat quadVertices[] = {
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    GLfloat quadTexCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoords), quadTexCoords, GL_STATIC_DRAW);

    GLfloat quadNormals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadNormals), quadNormals, GL_STATIC_DRAW);
}
