#include <QCoreApplication>
#include <QDebug>
#include <QExposeEvent>
#include <QString>

#include "constants.h"
#include "glwindow.h"

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
  if(e->text() == "+") {
    m_scene01->plot_range(true);
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->text() == "-") {
    m_scene01->plot_range(false);
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->text() == "a") {
    animate ^= 1;
    if(animate){
      animation_timer->start(ANIMATE_PERIOD);
      elapsed_time.restart();
    }
    else animation_timer->stop();
  }
  if(e->text() == "b") {
    m_scene01->toggle_background();
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->text() == "r") {
    animate = false;
    animation_timer->stop();
    m_scene01->reset();
    QOpenGLWindow::update(); // schedule paint
  }
  //if(e->key() == Qt::Key_Escape) {
  if(e->text() == "x") {
    QCoreApplication::exit(0);
  }
  else QOpenGLWindow::keyPressEvent(e);
}

void GLWindow::mouseMoveEvent(QMouseEvent *event)
{
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  if(event->buttons() & Qt::LeftButton){
    if(event->modifiers().testFlag(Qt::ShiftModifier)){
      if(event->modifiers().testFlag(Qt::ControlModifier))
        m_scene01->move_source(0, -dy);
      else m_scene01->move_source(dx, 0);
    }
    else m_scene01->move_source(dx, -dy);
    QOpenGLWindow::update(); // schedule paint
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
  m_scene01->animation_tick(elapsed_time.elapsed());
  elapsed_time.restart();
  QOpenGLWindow::update(); // schedule paint
}
