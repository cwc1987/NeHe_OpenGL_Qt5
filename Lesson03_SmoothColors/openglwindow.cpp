#include "openglwindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent) :
    QWindow(parent),
    m_update_pending(false),
    m_animating(false),
    m_context(NULL),
    m_show_full_screen(false)
{
    setSurfaceType(QWindow::OpenGLSurface);
    resize(640, 480);
}

OpenGLWindow::~OpenGLWindow()
{

}

void OpenGLWindow::render()
{

}

void OpenGLWindow::initialize()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void OpenGLWindow::resizeGL(int w, int h)
{
    if(h == 0)
    {
        h = 1;
    }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    qgluperspective(45.0f, (GLfloat)w/(GLfloat)h,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLWindow::qgluperspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat m[4][4];
    GLfloat sine;
    GLfloat cotangent;
    GLfloat deltaZ;
    GLfloat radians = fovy / 2 * 3.14 / 180;

    deltaZ = zFar - zNear;
    sine = qSin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
    {
        return;
    }
    cotangent = qCos(radians) / sine;

    qgluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1;
    m[3][2] = -2 * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixf(&m[0][0]);
}

void OpenGLWindow::qgluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

void OpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;
    if(animating)
    {
        renderLater();
    }
}

void OpenGLWindow::renderLater()
{
    if (!m_update_pending)
    {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void OpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context)
    {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();
        needsInitialize = true;
    }

    m_context->makeCurrent(this);
    if (needsInitialize)
    {
        initializeOpenGLFunctions();
        initialize();
        const qreal retinaScale = devicePixelRatio();
        resizeGL(width()*retinaScale, height()*retinaScale);
    }
    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type())
    {
        case QEvent::UpdateRequest:
            m_update_pending = false;
            renderNow();
            return true;
        default:
            return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    if (isExposed())
    {
        renderNow();
    }
    QWindow::exposeEvent(event);
}

void OpenGLWindow::resizeEvent(QResizeEvent *event)
{
    int w = event->size().width();
    int h = event->size().height();
    const qreal retinaScale = devicePixelRatio();
    resizeGL(w*retinaScale, h*retinaScale);
    renderNow();
    QWindow::resizeEvent(event);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_F1:
        {
            m_show_full_screen = !m_show_full_screen;
            if(m_show_full_screen)
            {
                showFullScreen();
            }
            else
            {
                showNormal();
            }
            break;
        }
        case Qt::Key_Escape:
        {
            qApp->exit();
            break;
        }
    }
    QWindow::keyPressEvent(event);
}
