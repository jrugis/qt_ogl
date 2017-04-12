#include <QCoreApplication>
#include <QDebug>
#include <QExposeEvent>
#include <QString>

#include "glwindow.h"

#define ARR 29.0         // ANIMATE_REFRESH_RATE
#define AP (1000.0/ARR)  // ANIMATE_PERIOD (ms)

GLWindow::GLWindow()
{
  animation_timer = new QTimer(this);
  connect(animation_timer, SIGNAL(timeout()), this, SLOT(animation_tick()));
  animate = true;
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
  animation_timer->start(AP);
  //m_elapsed_time.restart();
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
  //int elapsed_time = m_elapsed_time.elapsed(); // get actual elapsed time
  //m_elapsed_time.restart();
  //m_scene01->animation_tick(elapsed_time);
  m_scene01->animation_tick(ARR);
  QOpenGLWindow::update(); // schedule paint
}

void GLWindow::keyPressEvent(QKeyEvent *e)
{
  if(e->text() == "A") {
    animate ^= 1;
    if(animate) animation_timer->start(AP);
    else animation_timer->stop();
  }
  if(e->text() == "P") {
    m_scene01->m_parallel ^= 1;
    m_scene01->resize(this->width(), this->height());
    this->update();
  }
  if(e->key() == Qt::Key_Escape) {
    QCoreApplication::exit(0);
  }
        else QOpenGLWindow::keyPressEvent(e);
}


