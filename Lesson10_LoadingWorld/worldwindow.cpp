#include "worldwindow.h"

const float piover180 = 0.0174532925f;

void readStr(QTextStream *stream, QString &string)
{
    do
    {
        string = stream->readLine();
    } while (string[0] == '/' || string[0] == '\n' || string.isEmpty());
}

WorldWindow::WorldWindow(QWindow *parent) :
    OpenGLWindow(parent), m_program(NULL), m_blend(false),
    m_yrot(0.0f), m_xpos(0.0f), m_zpos(0.0f), m_heading(0.0f),
    m_walkbias(0.0f), m_walkbiasangle(0.0f), m_lookupdown(0.0f),
    m_filter(0)
{
}

WorldWindow::~WorldWindow()
{
    glDeleteTextures(3, &m_texture[0]);
}

void WorldWindow::initialize()
{
    loadShader();
    loadGLTexture();
    setupWorld();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE,GL_SRC_ALPHA,GL_ONE);
}

void WorldWindow::render()
{
    m_program->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_modelView.setToIdentity();

    GLfloat xtrans = -m_xpos;
    GLfloat ztrans = -m_zpos;
    GLfloat ytrans = -m_walkbias-0.25f;
    GLfloat sceneroty = 360.0f - m_yrot;
    int numtriangles;
    m_modelView.rotate(m_lookupdown, 1.0f, 0, 0);
    m_modelView.rotate(sceneroty, 0, 1.0f, 0);
    m_modelView.translate(xtrans, ytrans, ztrans);
    numtriangles = m_sector.numtriangles;

    glBindTexture(GL_TEXTURE_2D, m_texture[m_filter]);
    m_program->setUniformValue("mvpMatrix", m_projection * m_modelView);
    for (int loop_m = 0; loop_m < numtriangles; loop_m++)
    {
        GLfloat triangleVertexs[] = {
            m_sector.triangle[loop_m].vertex[0].x,
            m_sector.triangle[loop_m].vertex[0].y,
            m_sector.triangle[loop_m].vertex[0].z,
            m_sector.triangle[loop_m].vertex[1].x,
            m_sector.triangle[loop_m].vertex[1].y,
            m_sector.triangle[loop_m].vertex[1].z,
            m_sector.triangle[loop_m].vertex[2].x,
            m_sector.triangle[loop_m].vertex[2].y,
            m_sector.triangle[loop_m].vertex[2].z,
        };

        GLfloat triangleTexCoords[] = {
            m_sector.triangle[loop_m].vertex[0].u,
            m_sector.triangle[loop_m].vertex[0].v,
            m_sector.triangle[loop_m].vertex[1].u,
            m_sector.triangle[loop_m].vertex[1].v,
            m_sector.triangle[loop_m].vertex[2].u,
            m_sector.triangle[loop_m].vertex[2].v,
        };

        m_program->enableAttributeArray(m_posAttr);
        m_program->setAttributeArray(m_posAttr, GL_FLOAT, triangleVertexs, 3);
        m_program->enableAttributeArray(m_texCoordAttr);
        m_program->setAttributeArray(m_texCoordAttr, GL_FLOAT, triangleTexCoords, 2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    m_program->release();
}

void WorldWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_PageUp:
        {
            m_lookupdown-=1.0f;
            break;
        }
        case Qt::Key_PageDown:
        {
            m_lookupdown+=1.0f;
            break;
        }
        case Qt::Key_Right:
        {
            m_heading -=1.0f;
            m_yrot = m_heading;
            break;
        }
        case Qt::Key_Left:
        {
            m_heading += 1.0f;
            m_yrot = m_heading;
            break;
        }
        case Qt::Key_Up:
        {
            m_xpos -= (float)sin(m_heading*piover180) * 0.05f;
            m_zpos -= (float)cos(m_heading*piover180) * 0.05f;
            if (m_walkbiasangle >= 359.0f)
            {
                m_walkbiasangle = 0.0f;
            }
            else
            {
                 m_walkbiasangle+= 10;
            }
            m_walkbias = (float)qSin(m_walkbiasangle * piover180)/20.0f;
            break;
        }
        case Qt::Key_Down:
        {
            m_xpos += (float)sin(m_heading * piover180) * 0.05f;
            m_zpos += (float)cos(m_heading * piover180) * 0.05f;
            if (m_walkbiasangle <= 1.0f)
            {
                m_walkbiasangle = 359.0f;
            }
            else
            {
                m_walkbiasangle-= 10;
            }
            m_walkbias = (float)sin(m_walkbiasangle * piover180)/20.0f;
            break;
        }
        case Qt::Key_B:
        {
            m_blend = !m_blend;
            if (m_blend)
            {
                glEnable(GL_BLEND);
                glDisable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_BLEND);
                glEnable(GL_DEPTH_TEST);
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
    }
    renderNow();
    OpenGLWindow::keyPressEvent(event);
}

void WorldWindow::setupWorld()
{
    QFile file(":/World.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString oneline;
    int numtriangles;
    float x, y, z, u, v;
    readStr(&stream, oneline);
    sscanf(oneline.toLatin1().data(), "NUMPOLLIES %d\n", &numtriangles);

    m_sector.triangle = new TRIANGLE[numtriangles];
    m_sector.numtriangles = numtriangles;
    for (int triloop = 0; triloop < numtriangles; triloop++)
    {
        for (int vertloop = 0; vertloop < 3; vertloop++)
        {
            readStr(&stream, oneline);
            sscanf(oneline.toLatin1().data(), "%f %f %f %f %f", &x, &y, &z, &u, &v);
            m_sector.triangle[triloop].vertex[vertloop].x = x;
            m_sector.triangle[triloop].vertex[vertloop].y = y;
            m_sector.triangle[triloop].vertex[vertloop].z = z;
            m_sector.triangle[triloop].vertex[vertloop].u = u;
            m_sector.triangle[triloop].vertex[vertloop].v = v;
        }
    }
    file.close();
}

void WorldWindow::loadShader()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_texCoordAttr = m_program->attributeLocation("texCoordAttr");
}

void WorldWindow::loadGLTexture()
{
    QImage image(":/image/Crate.bmp");
    image = image.convertToFormat(QImage::Format_RGB888);
    image = image.mirrored();

    glGenTextures(3, &m_texture[0]);

    glBindTexture(GL_TEXTURE_2D, m_texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, m_texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, m_texture[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
}
