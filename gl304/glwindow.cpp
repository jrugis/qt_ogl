#include <QCoreApplication>
#include <QDebug>
#include <QExposeEvent>
#include <QString>

#include "glwindow.h"

#define ARR 29.0         // ANIMATE_REFRESH_RATE
#define AP (1000.0/ARR)  // ANIMATE_PERIOD (ms)

GLWindow::GLWindow()
{
  animate = false;
  animation_timer = new QTimer(this);
  connect(animation_timer, SIGNAL(timeout()), this, SLOT(animation_tick()));
}

GLWindow::~GLWindow()
{
  delete animation_timer;
  delete m_scene01;
}

void GLWindow::initializeGL()
{
  initializeOpenGLFunctions();
  qDebug() << (const char*)glGetString(GL_RENDERER);
  qDebug() << "OpenGL" << (const char*)glGetString(GL_VERSION);
  qDebug() << "GLSL" << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

  m_scene01 = new CScene();
}

void GLWindow::keyPressEvent(QKeyEvent *e)
{
  if(e->text() == "1") {
    m_scene01->toggle_plot();
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->text() == "A") {
    animate ^= 1;
    if(animate) animation_timer->start(AP);
    else animation_timer->stop();
  }
  if(e->text() == "B") {
    m_scene01->toggle_background();
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->text() == "P") {
    m_scene01->toggle_projection();
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->key() == Qt::Key_Escape) {
    QCoreApplication::exit(0);
  }
  else QOpenGLWindow::keyPressEvent(e);
}

void GLWindow::mouseMoveEvent(QMouseEvent *event)
{
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  if(event->buttons() & Qt::LeftButton) {
    rotate_angle += dx * RDS;
    if(rotate_angle > ROTATE_MAX) rotate_angle -= 360.0;
    if(rotate_angle < ROTATE_MIN) rotate_angle += 360.0;

    tilt_angle += dy * RDS;
    if(tilt_angle > TILT_MAX) tilt_angle = TILT_MAX;
    if(tilt_angle < TILT_MIN) tilt_angle = TILT_MIN;
    updateGL();
  }
  lastPos = event->pos();
}

void GLWindow::paintGL()
{
  m_scene01->draw();
}

void GLWindow::resizeGL(int width, int height)
{
  m_scene01->set_canvas_size(width, height);
}

void GLWindow::animation_tick()
{
  m_scene01->animation_tick(ARR);
  QOpenGLWindow::update(); // schedule paint
}



