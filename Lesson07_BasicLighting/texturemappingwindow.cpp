#include "texturemappingwindow.h"

GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightPosition[]= {0.0f, 0.0f, 2.0f, 1.0f};

TextureMappingWindow::TextureMappingWindow(QWindow *parent) :
    OpenGLWindow(parent), m_xrot(0.0f), m_yrot(0.0f), m_z(-5.0f),
    m_xspeed(0.0f), m_yspeed(0.0f), m_filter(0), m_light(true)
{
}

TextureMappingWindow::~TextureMappingWindow()
{
    glDeleteTextures(3, &m_texture[0]);
}

void TextureMappingWindow::initialize()
{
    loadShader();
    loadGLTexture();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
//    if(!m_light)
//    {
//        glDisable(GL_LIGHTING);
//    }
//    else
//    {
//        glEnable(GL_LIGHTING);
//    }
}

void TextureMappingWindow::render()
{
    m_program->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,m_z);
    glRotatef(m_xrot,1.0f,0.0f,0.0f);
    glRotatef(m_yrot,0.0f,1.0f,0.0f);
    glBindTexture(GL_TEXTURE_2D, m_texture[m_filter]);
    GLuint textcoord = m_program->attributeLocation("a_texcoord");
    glBegin(GL_QUADS);
            glNormal3f( 0.0f, 0.0f, 1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f,  1.0f);

            glNormal3f( 0.0f, 0.0f,-1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);

            glNormal3f( 0.0f, 1.0f, 0.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f(-1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f( 1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f, -1.0f);

            glNormal3f( 0.0f,-1.0f, 0.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f,  1.0f);

            glNormal3f( 1.0f, 0.0f, 0.0f);
            glVertexAttrib2f(textcoord, 1.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 1.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f, -1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f,  1.0f);
            glVertexAttrib2f(textcoord, 0.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);

            glNormal3f(-1.0f, 0.0f, 0.0f);
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
            if(!m_light)
            {
                glDisable(GL_LIGHTING);
            }
            else
            {
                glEnable(GL_LIGHTING);
            }
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

    glBindTexture(GL_TEXTURE_2D, m_texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

    glBindTexture(GL_TEXTURE_2D, m_texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.width(), image.height(),
                      GL_RGB, GL_UNSIGNED_BYTE, image.bits());
}

void TextureMappingWindow::loadShader()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
}
