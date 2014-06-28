#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QPainter>
#include <QWindow>
#include <QOpenGLFunctions>
#include <QCoreApplication>
#include <QEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QTextStream>
#include <QMatrix4x4>

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

    QMatrix4x4 m_modelView;

    QMatrix4x4 m_projection;
private:
    bool m_update_pending;
    bool m_animating;
    QOpenGLContext *m_context;
    bool m_show_full_screen;
};

#endif // OPENGLWINDOW_H
