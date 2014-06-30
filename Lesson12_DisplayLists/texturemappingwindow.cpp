#include "texturemappingwindow.h"

struct VertexData
{
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
};

QVector4D LightAmbient = QVector4D(0.5f, 0.5f, 0.5f, 1.0f);
QVector4D LightDiffuse = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
QVector4D LightPosition = QVector4D(0.0f, 0.0f, 2.0f, 1.0f);
QVector4D LightModelAmbient = QVector4D(0.2, 0.2, 0.2, 1.0);
QVector4D MaterialDiffuse = QVector4D(0.8f, 0.8f, 0.8f, 1.0f);
QVector4D MaterialAmbient = QVector4D(0.2f, 0.2f, 0.2f, 1.0f);

static GLfloat boxcol[5][4]=
{
    {1.0f,0.0f,0.0f,1.0},
    {1.0f,0.5f,0.0f,1.0},
    {1.0f,1.0f,0.0f,1.0},
    {0.0f,1.0f,0.0f,1.0},
    {0.0f,1.0f,1.0f,1.0}
};

static GLfloat topcol[5][4]=
{
    {0.5f,0.0f,0.0f,1.0},
    {0.5f,0.25f,0.0f,1.0},
    {0.5f,0.5f,0.0f,1.0},
    {0.0f,0.5f,0.0f,1.0},
    {0.0f,0.5f,0.5f,1.0}
};

TextureMappingWindow::TextureMappingWindow(QWindow *parent) :
    OpenGLWindow(parent), m_texture(-1), m_xrot(0.0f), m_yrot(0.0f)
{
}

TextureMappingWindow::~TextureMappingWindow()
{
    glDeleteTextures(1, &m_texture);
    glDeleteBuffers(3, &m_vboIds[0]);
}

void TextureMappingWindow::initialize()
{
    initGeometry();
    loadShader();
    loadGLTexture();
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepthf(1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
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

    for (int yloop=1;yloop<6;yloop++)
    {
        for (int xloop=0;xloop<yloop;xloop++)
        {
            m_modelView.setToIdentity();
            m_modelView.translate(1.4+(float(xloop)*2.8) - (float(yloop)*1.4),
                                  ((6.0-float(yloop))*2.4) - 7.0,
                                  -20.0);
            m_modelView.rotate(45.0-(2.0*yloop)+m_xrot, 1.0, 0.0, 0.0);
            m_modelView.rotate(45.0+m_yrot, 0.0, 1.0, 0.0);
            m_program->setUniformValue("mvMatrix", m_modelView);
            m_program->setUniformValue("mvpMatrix", m_projection * m_modelView);

            quintptr offset = 0;
            glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
            m_program->enableAttributeArray(m_posAttr);
            glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

            offset += sizeof(QVector3D);
            m_program->enableAttributeArray(m_normalAttr);
            glVertexAttribPointer(m_normalAttr, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

            offset += sizeof(QVector3D);
            m_program->enableAttributeArray(m_texCoordAttr);
            glVertexAttribPointer(m_texCoordAttr, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

            glBindTexture(GL_TEXTURE_2D, m_texture);
            m_program->setUniformValue("matrialColor",
                                       boxcol[yloop-1][0],
                                       boxcol[yloop-1][1],
                                       boxcol[yloop-1][2],
                                       boxcol[yloop-1][3]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
            glDrawElements(GL_TRIANGLE_STRIP, 29, GL_UNSIGNED_SHORT, 0);

            m_program->setUniformValue("matrialColor",
                                       topcol[yloop-1][0],
                                       topcol[yloop-1][1],
                                       topcol[yloop-1][2],
                                       topcol[yloop-1][3]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[2]);
            glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
        }
    }
    m_program->release();
}

void TextureMappingWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
        {
            m_xrot-=0.2f;
            break;
        }
        case Qt::Key_Down:
        {
            m_xrot+=0.2f;
            break;
        }
        case Qt::Key_Left:
        {
            m_yrot-=0.2f;
            break;
        }
        case Qt::Key_Right:
        {
            m_yrot+=0.2f;
            break;
        }
    }
    renderNow();
    OpenGLWindow::keyPressEvent(event);
}

void TextureMappingWindow::loadGLTexture()
{
    QImage image(":/image/Cube.bmp");
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
    m_posAttr = m_program->attributeLocation("posAttr");
    m_texCoordAttr = m_program->attributeLocation("texCoordAttr");
    m_normalAttr = m_program->attributeLocation("normalAttr");
}

void TextureMappingWindow::initGeometry()
{
    glGenBuffers(3, &m_vboIds[0]);
    VertexData vertices[] =
    {
        // Vertex data for face 0
        {QVector3D(-1.0, -1.0,  1.0), QVector3D(0.0, 0.0, 1.0), QVector2D(0.0, 0.0)},  // v0
        {QVector3D( 1.0, -1.0,  1.0), QVector3D(0.0, 0.0, 1.0), QVector2D(1.0, 0.0)}, // v1
        {QVector3D(-1.0,  1.0,  1.0), QVector3D(0.0, 0.0, 1.0), QVector2D(0.0, 1.0)},  // v2
        {QVector3D( 1.0,  1.0,  1.0), QVector3D(0.0, 0.0, 1.0), QVector2D(1.0, 1.0)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0, -1.0,  1.0), QVector3D(1.0, 0.0, 0.0), QVector2D(0.0, 0.0)}, // v4
        {QVector3D( 1.0, -1.0, -1.0), QVector3D(1.0, 0.0, 0.0), QVector2D(1.0, 0.0)}, // v5
        {QVector3D( 1.0,  1.0,  1.0), QVector3D(1.0, 0.0, 0.0), QVector2D(0.0, 1.0)},  // v6
        {QVector3D( 1.0,  1.0, -1.0), QVector3D(1.0, 0.0, 0.0), QVector2D(1.0, 1.0)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0, -1.0, -1.0), QVector3D(0.0, 0.0, -1.0), QVector2D(0.0, 0.0)}, // v8
        {QVector3D(-1.0, -1.0, -1.0), QVector3D(0.0, 0.0, -1.0), QVector2D(1.0, 0.0)},  // v9
        {QVector3D( 1.0,  1.0, -1.0), QVector3D(0.0, 0.0, -1.0), QVector2D(0.0, 1.0)}, // v10
        {QVector3D(-1.0,  1.0, -1.0), QVector3D(0.0, 0.0, -1.0), QVector2D(1.0, 1.0)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0, -1.0, -1.0), QVector3D(-1.0, 0.0, 0.0), QVector2D(0.0, 0.0)}, // v12
        {QVector3D(-1.0, -1.0,  1.0), QVector3D(-1.0, 0.0, 0.0), QVector2D(1.0, 0.0)},  // v13
        {QVector3D(-1.0,  1.0, -1.0), QVector3D(-1.0, 0.0, 0.0), QVector2D(0.0, 1.0)}, // v14
        {QVector3D(-1.0,  1.0,  1.0), QVector3D(-1.0, 0.0, 0.0), QVector2D(1.0, 1.0)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0, -1.0, -1.0), QVector3D(0.0, -1.0, 0.0), QVector2D(0.0, 0.0)}, // v16
        {QVector3D( 1.0, -1.0, -1.0), QVector3D(0.0, -1.0, 0.0), QVector2D(1.0, 0.0)}, // v17
        {QVector3D(-1.0, -1.0,  1.0), QVector3D(0.0, -1.0, 0.0), QVector2D(0.0, 1.0)}, // v18
        {QVector3D( 1.0, -1.0,  1.0), QVector3D(0.0, -1.0, 0.0), QVector2D(1.0, 1.0)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0,  1.0,  1.0), QVector3D(0.0, 1.0, 0.0), QVector2D(0.0, 0.0)}, // v20
        {QVector3D( 1.0,  1.0,  1.0), QVector3D(0.0, 1.0, 0.0), QVector2D(1.0, 0.0)}, // v21
        {QVector3D(-1.0,  1.0, -1.0), QVector3D(0.0, 1.0, 0.0), QVector2D(0.0, 1.0)}, // v22
        {QVector3D( 1.0,  1.0, -1.0), QVector3D(0.0, 1.0, 0.0), QVector2D(1.0, 1.0)}  // v23
    };

    GLushort bodyIndices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
    };

    GLushort topIndices[] = {
        20, 21, 22, 23          // Face 5 - triangle strip (v20, v21, v22, v23)
    };

    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(VertexData), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 29 * sizeof(GLushort), bodyIndices, GL_STATIC_DRAW);

    // Transfer index data to VBO 2
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), topIndices, GL_STATIC_DRAW);
}
