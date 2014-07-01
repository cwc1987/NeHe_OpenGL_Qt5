#include "texturemappingwindow.h"

TextureMappingWindow::TextureMappingWindow(QWindow *parent) :
    OpenGLWindow(parent), m_xrot(0.0f), m_yrot(0.0f), m_zrot(0.0f),
    m_texture(-1)
{
}

TextureMappingWindow::~TextureMappingWindow()
{
    glDeleteTextures(1, &m_texture);
}

void TextureMappingWindow::initialize()
{
    initGeometry();
    loadShader();
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
}

void TextureMappingWindow::render()
{
    float float_x, float_y, float_xb, float_yb;
    m_program->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_modelView.setToIdentity();
    m_modelView.translate(0.0f, 0.0f, -12.0f);
    m_modelView.rotate(m_xrot, 1.0f, 0.0f, 0.0f);
    m_modelView.rotate(m_yrot, 0.0f, 1.0f, 0.0f);
    m_modelView.rotate(m_zrot, 0.0f, 0.0f, 1.0f);
    m_program->setUniformValue("mvpMatrix", m_projection * m_modelView);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    for(int x = 0; x < 44; x++ )
    {
        for(int y = 0; y < 44; y++ )
        {
            float_x = float(x)/44.0f;
            float_y = float(y)/44.0f;
            float_xb = float(x+1)/44.0f;
            float_yb = float(y+1)/44.0f;
            GLfloat vertexs[] =
            {
                m_points[x][y][0], m_points[x][y][1], m_points[x][y][2],
                m_points[x+1][y][0], m_points[x+1][y][1], m_points[x+1][y][2],
                m_points[x][y+1][0], m_points[x][y+1][1], m_points[x][y+1][2],
                m_points[x+1][y+1][0], m_points[x+1][y+1][1], m_points[x+1][y+1][2]
            };
            GLfloat texCoords[] =
            {
                float_x, float_y,
                float_xb, float_y,
                float_x, float_yb,
                float_xb, float_yb
            };

            m_program->enableAttributeArray(m_posAttr);
            m_program->setAttributeArray(m_posAttr, GL_FLOAT, vertexs, 3);
            m_program->enableAttributeArray(m_texCoordAttr);
            m_program->setAttributeArray(m_texCoordAttr, GL_FLOAT, texCoords, 2);
            glCullFace(GL_BACK);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glCullFace(GL_FRONT);
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }
    }
    m_program->release();

    for(int y = 0; y < 45; y++ )
    {
        GLfloat hold = m_points[0][y][2];
        for(int x = 0; x < 44; x++)
        {
            m_points[x][y][2] = m_points[x+1][y][2];
        }
        m_points[44][y][2]=hold;
    }
    m_xrot+=0.3f;
    m_yrot+=0.2f;
    m_zrot+=0.4f;
}

void TextureMappingWindow::loadGLTexture()
{
    QImage image(":/image/Tim.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void TextureMappingWindow::loadShader()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_texCoordAttr = m_program->attributeLocation("texCoordAttr");
}

void TextureMappingWindow::initGeometry()
{
    for(int x=0; x<45; x++)
    {
        for(int y=0; y<45; y++)
        {
            m_points[x][y][0]=float((x/5.0f)-4.5f);
            m_points[x][y][1]=float((y/5.0f)-4.5f);
            m_points[x][y][2]=float(qSin((((x/5.0f)*40.0f)/360.0f)*3.141592654*2.0f));
        }
    }
}
