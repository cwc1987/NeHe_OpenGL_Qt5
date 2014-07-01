#include "fontwindow.h"

FontWindow::FontWindow(QWindow *parent) :
    OpenGLWindow(parent), m_program(NULL),
    m_cnt1(0.0f), m_cnt2(0.0f)
{
    initFreeType();
}

FontWindow::~FontWindow()
{
}

void FontWindow::initialize()
{
    loadShader();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void FontWindow::initFreeType()
{
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
    {
        qDebug() << "could not init free type library.";
        return;
    }
    QString fontPath = QApplication::applicationDirPath() + "/Test.ttf";
    if(FT_New_Face(ft, fontPath.toLatin1().data(), 0, &m_ftFace))
    {
        qDebug() << "Could not open font.";
        return;
    }
}

void FontWindow::render()
{
    m_program->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_modelView.setToIdentity();
    m_modelView.translate(0.0f, 0.0f, -3.0f);
    QVector4D color(1.0f*float(qCos(m_cnt1)),
                    1.0f*float(qSin(m_cnt2)),
                    1.0f-0.5f*float(qCos(m_cnt1+m_cnt2)),
                    1.0f);
    m_program->setUniformValue("color", color);
    m_program->setUniformValue("mvpMatrix", m_projection * m_modelView);

    const qreal retinaScale = devicePixelRatio();
    resizeGL(width()*retinaScale, height()*retinaScale);

    float sx = 2.0 / width()*retinaScale;
    float sy = 2.0 / height()*retinaScale;
    FT_Set_Pixel_Sizes(m_ftFace, 0, 48);

    float mx = float(cos(m_cnt1));
    float my = float(sin(m_cnt2));
    QString text = "Active OpenGL Text With NeHe - " + QString::number(m_cnt1, 'f', 2);
    renderText(text.toLatin1().data(),
                mx - 0.5, my, sx, sy);
    m_program->release();

    m_cnt1+=0.051f;
    m_cnt2+=0.005f;
}

void FontWindow::loadShader()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertshader.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragshader.glsl");
    m_program->link();
}

void FontWindow::renderText(const char *text, float x, float y, float sx, float sy)
{
    const char *p;
    FT_GlyphSlot ftGlyph = m_ftFace->glyph;
    GLuint coordAttr = m_program->attributeLocation("coord");
    m_program->enableAttributeArray(coordAttr);

    GLuint ftTex;
    glGenTextures(1, &ftTex);
    glBindTexture(GL_TEXTURE_2D, ftTex);

    for(p = text; *p; p++)
    {
        if(FT_Load_Char(m_ftFace, *p, FT_LOAD_RENDER))
        {
            qDebug() << "Could not load character " << *p;
            continue;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ftGlyph->bitmap.width,
                     ftGlyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                     ftGlyph->bitmap.buffer);

        float x2 = x + ftGlyph->bitmap_left * sx;
        float y2 = -y - ftGlyph->bitmap_top * sy;
        float w = ftGlyph->bitmap.width * sx;
        float h = ftGlyph->bitmap.rows * sy;

        GLfloat box[4][4] =
        {
            {x2,     -y2    , 0, 0},
            {x2 + w, -y2    , 1, 0},
            {x2,     -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        m_program->setAttributeArray(coordAttr, GL_FLOAT, box, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x += (ftGlyph->advance.x >> 6) * sx;
        y += (ftGlyph->advance.y >> 6) * sy;
    }

    glDeleteTextures(1, &ftTex);
}
