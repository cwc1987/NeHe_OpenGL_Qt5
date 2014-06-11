#include "polygonwindow.h"

static const char *triangleVertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *triangleFFragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

static const char *rectVertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

PolygonWindow::PolygonWindow(QWindow *parent) :
    OpenGLWindow(parent)
{
}

void PolygonWindow::initialize()
{
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, triangleVertexShaderSource);
    //m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, rectVertexShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void PolygonWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] =
    {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();
}
