#include <QDebug>
#include <QExposeEvent>
#include <QString>

#include "glwindow.h"

#define ARR 20.0         // ANIMATE_REFRESH_RATE
#define AP (1000.0/ARR)  // ANIMATE_PERIOD (ms)

GLWindow::GLWindow()
{
  animation_timer = new QTimer(this);
  connect(animation_timer, SIGNAL(timeout()), this, SLOT(animation_tick()));
  m_scene01 = new Scene();
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

  m_scene01->initialize();
  animation_timer->start(AP);
  m_elapsed_time.restart();
}

void GLWindow::resizeGL(int width, int height)
{
  m_scene01->resize(width, height);
}

void GLWindow::paintGL()
{
  m_scene01->draw();
}

void GLWindow::animation_tick()
{
  int elapsed_time = m_elapsed_time.elapsed(); // get actual elapsed time
  m_elapsed_time.restart();
  m_scene01->animation_tick(elapsed_time);
  QOpenGLWindow::update(); // schedule paint
}
