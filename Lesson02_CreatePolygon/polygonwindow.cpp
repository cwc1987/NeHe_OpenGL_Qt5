#include "polygonwindow.h"

PolygonWindow::PolygonWindow(QWindow *parent) :
    OpenGLWindow(parent), m_program(NULL), m_frame(0)
{
}

void PolygonWindow::initialize()
{
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    qDebug() << "GL Vendor:" << QString::fromLocal8Bit((char*)vendor);
    qDebug() << "GL Renderer:" << QString::fromLocal8Bit((char*)renderer);
    qDebug() << "GL Version:" << QString::fromLocal8Bit((char*)version);
    qDebug() << "GLSL Version:" << QString::fromLocal8Bit((char*)glslVersion);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//    m_program = new QOpenGLShaderProgram(this);
//    QString triangleVertSource = loadShaderFile(":/shader/triangle.vert");
//    QString triangleFragSource = loadShaderFile(":/shader/triangle.frag");
//    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, triangleVertSource);
//    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, triangleFragSource);
//    m_program->link();
//    m_posAttr = m_program->attributeLocation("posAttr");
//    m_colAttr = m_program->attributeLocation("colAttr");
//    m_matrixUniform = m_program->uniformLocation("matrix");
}

void PolygonWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕及深度缓存

    glLoadIdentity();							// 重置当前的模型观察矩阵
    glTranslatef(-1.5f,0.0f,-6.0f);						// 左移 1.5 单位，并移入屏幕 6.0
    glBegin(GL_TRIANGLES);							// 绘制三角形

        glVertex3f( 0.0f, 1.0f, 0.0f);					// 上顶点

        glVertex3f(-1.0f,-1.0f, 0.0f);					// 左下

        glVertex3f( 1.0f,-1.0f, 0.0f);					// 右下

    glEnd();								// 三角形绘制结束
    glTranslatef(3.0f,0.0f,0.0f);						// 右移3单位
    glBegin(GL_QUADS);							//  绘制正方形

        glVertex3f(-1.0f, 1.0f, 0.0f);					// 左上

        glVertex3f( 1.0f, 1.0f, 0.0f);					// 右上

        glVertex3f( 1.0f,-1.0f, 0.0f);					// 左下

        glVertex3f(-1.0f,-1.0f, 0.0f);					// 右下

    glEnd();								// 正方形绘制结束


//    const qreal retinaScale = devicePixelRatio();
//    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

//    glClear(GL_COLOR_BUFFER_BIT);

//    m_program->bind();

//    QMatrix4x4 matrix;
//    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
//    matrix.translate(0, 0, -2);
//    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

//    m_program->setUniformValue(m_matrixUniform, matrix);

//    GLfloat vertices[] =
//    {
//        0.0f, 0.707f,
//        -0.5f, -0.5f,
//        0.5f, -0.5f
//    };

//    GLfloat colors[] =
//    {
//        1.0f, 0.0f, 0.0f,
//        0.0f, 1.0f, 0.0f,
//        0.0f, 0.0f, 1.0f
//    };

//    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
//    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);

//    glDrawArrays(GL_TRIANGLES, 0, 3);

//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(0);

//    m_program->release();

//    ++m_frame;
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    m_program->bind();
//    glLoadIdentity();

//    GLfloat vertices[] =
//    {
//        0.0f, 1.0f, 0.0f,
//        -1.0f, -1.0f, 0.0f,
//        1.0f, -1.0f, 0.0f
//    };
//    glVertexAttribPointer();

//    GLfloat colors[] =
//    {
//        1.0f, 0.0f, 0.0f,
//        0.0f, 1.0f, 0.0f,
//        0.0f, 0.0f, 1.0f
//    };

}
