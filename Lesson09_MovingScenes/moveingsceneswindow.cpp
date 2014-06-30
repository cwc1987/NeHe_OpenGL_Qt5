#include "moveingsceneswindow.h"

const int starCount = 50;
MoveingScenesWindow::MoveingScenesWindow(QWindow *parent) :
    OpenGLWindow(parent), m_program(NULL), m_texture(-1),
    m_twinkle(false), m_zoom(-15.0f), m_tilt(90.0f), m_spin(0.0f)
{
}

MoveingScenesWindow::~MoveingScenesWindow()
{
    glDeleteTextures(1, &m_texture);
}

void MoveingScenesWindow::initialize()
{
    initGeometry();
    loadShader();
    loadGLTexture();

    glClearDepthf(1.0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE,GL_SRC_ALPHA,GL_ONE);
}

void MoveingScenesWindow::render()
{
    m_program->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    for (int i = 0, iend = starCount;i<iend; i++)
    {
        m_modelView.setToIdentity();
        m_modelView.translate(0.0f, 0.0f, m_zoom);
        m_modelView.rotate(m_tilt, 1.0f, 0.0f, 0.0f);
        m_modelView.rotate(m_stars[i].angle, 0.0f, 1.0f, 0.0f);
        m_modelView.translate(m_stars[i].dist, 0.0f, 0.0f);
        m_modelView.rotate(-m_stars[i].angle, 0.0f, 0.0f);
        m_modelView.rotate(-m_tilt, 1.0f, 0.0f, 0.0f);

        GLfloat quadsVertexs[] = {
            1.0, 1.0, 0.0,
           -1.0, 1.0, 0.0,
            1.0,-1.0, 0.0,
           -1.0,-1.0, 0.0

        };
        GLfloat texCoords[] = {
            1.0, 1.0,
            0.0, 1.0,
            1.0, 0.0,
            0.0, 0.0
        };

        m_program->setUniformValue("mvpMatrix", m_projection * m_modelView);
        if (m_twinkle)
        {
            m_program->setUniformValue("starColor", m_stars[(iend-i)-1].r, m_stars[(iend-i)-1].g, m_stars[(iend-i)-1].b, 1.0f);
            m_program->enableAttributeArray(m_posAttr);
            m_program->setAttributeArray(m_posAttr, GL_FLOAT, quadsVertexs, 3);
            m_program->enableAttributeArray(m_texCoordAttr);
            m_program->setAttributeArray(m_texCoordAttr, GL_FLOAT, texCoords, 2);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        m_modelView.rotate(m_spin, 0.0f, 0.0f, 1.0f);
        m_program->setUniformValue("mvpMatrix", m_projection * m_modelView);
        m_program->setUniformValue("starColor", m_stars[i].r, m_stars[i].g, m_stars[i].b, 1.0f);

        m_program->enableAttributeArray(m_posAttr);
        m_program->setAttributeArray(m_posAttr, GL_FLOAT, quadsVertexs, 3);
        m_program->enableAttributeArray(m_texCoordAttr);
        m_program->setAttributeArray(m_texCoordAttr, GL_FLOAT, texCoords, 2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        m_spin+=0.01f;
        m_stars[i].angle+=float(i)/iend;
        m_stars[i].dist-=0.01f;
        if (m_stars[i].dist<0.0f)
        {
            m_stars[i].dist+=5.0f;
            m_stars[i].r=float(qrand()%256)/(float)256;
            m_stars[i].g=float(qrand()%256)/(float)256;
            m_stars[i].b=float(qrand()%256)/(float)256;
        }
    }
    m_program->release();
}

void MoveingScenesWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_T:
        {
            m_twinkle=!m_twinkle;
            break;
        }
        case Qt::Key_Up:
        {
            m_tilt-=0.5f;
            break;
        }
        case Qt::Key_Down:
        {
            m_tilt+=0.5f;
            break;
        }
        case Qt::Key_PageUp:
        {
            m_zoom-=0.2f;
            break;
        }
        case Qt::Key_PageDown:
        {
            m_zoom+=0.2f;
            break;
        }
    }
    OpenGLWindow::keyPressEvent(event);
}

void MoveingScenesWindow::loadGLTexture()
{
    QImage image(":/image/Star.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MoveingScenesWindow::loadShader()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_texCoordAttr = m_program->attributeLocation("texCoordAttr");
}

void MoveingScenesWindow::initGeometry()
{
    for (int i = 0, iend = starCount; i<iend; i++)
    {
        stars star;
        star.angle=0.0f;
        star.dist=(float(i)/iend)*5.0f;
        star.r=float(qrand()%256)/(float)256;
        star.g=float(qrand()%256)/(float)256;
        star.b=float(qrand()%256)/(float)256;
        m_stars.push_back(star);
    }
}
