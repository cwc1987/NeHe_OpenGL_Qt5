#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QPainter>
#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include <QCoreApplication>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QEvent>
#include <QResizeEvent>
#include <GL/glu.h>
#include <QKeyEvent>

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    void setAnimating(bool animating);
public slots:
    void renderLater();

    void renderNow();
protected:
    bool event(QEvent *event);

    void exposeEvent(QExposeEvent *event);

    void resizeEvent(QResizeEvent * event);

    void keyPressEvent(QKeyEvent * event);

    virtual void render();

    virtual void initialize();

    virtual void resizeGL(int w, int h);
private:
    bool m_update_pending;
    bool m_animating;
    QOpenGLContext *m_context;
    bool m_show_full_screen;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // OPENGLWINDOW_H
