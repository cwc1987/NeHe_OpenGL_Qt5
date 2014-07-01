#ifndef FONTWINDOW_H
#define FONTWINDOW_H

#include <QOpenGLShaderProgram>
#include <QApplication>
#include <freetype2/ft2build.h>
#include <freetype2/freetype.h>
#include <QtMath>
#include "openglwindow.h"

class FontWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    explicit FontWindow(QWindow *parent = 0);
    ~FontWindow();
protected:
    void initialize();
private:
    void initFreeType();

    void render();
private:
    void loadShader();

    void renderText(const char *text, float x, float y, float sx, float sy);
private:
    QOpenGLShaderProgram *m_program;

    FT_Face m_ftFace;
    GLfloat	m_cnt1;
    GLfloat m_cnt2;
};

#endif // FONTWINDOW_H
