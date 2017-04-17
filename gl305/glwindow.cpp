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
  if(e->text() == "A") {
    animate ^= 1;
    if(animate) animation_timer->start(AP);
    else animation_timer->stop();
  }
  if(e->text() == "B") {
    m_scene01->toggle_background();
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->key() == Qt::Key_Left) {
    m_scene01->move_source(-1, 0);
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->key() == Qt::Key_Right) {
    m_scene01->move_source(1, 0);
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->key() == Qt::Key_Up) {
    m_scene01->move_source(0, 1);
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->key() == Qt::Key_Down) {
    m_scene01->move_source(0, -1);
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->text() == "R") {
    animate = false;
    animation_timer->stop();
    m_scene01->reset();
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->key() == Qt::Key_Escape) {
    QCoreApplication::exit(0);
  }
  else QOpenGLWindow::keyPressEvent(e);
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
