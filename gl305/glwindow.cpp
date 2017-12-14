#include <QCoreApplication>
#include <QDebug>
#include <QExposeEvent>
#include <QString>

#include "glwindow.h"

GLWindow::GLWindow(char* ip, char* np)
{
  ipaddr = ip;
  port = np;
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

  m_scene01 = new CScene(ipaddr, port);
}

void GLWindow::keyPressEvent(QKeyEvent *e)
{
  if(e->text() == "?") {
    qDebug() << "<up>   move up   by 0.01";
    qDebug() << "<down> move down by 0.01";
    qDebug() << "+  increase range";
    qDebug() << "-  decrease range";
    qDebug() << "1  move to 7005";
    qDebug() << "2  move to 71732.83";
    qDebug() << "a  animate";
    qDebug() << "c  color scheme";
    qDebug() << "m  move down by 100";
    qDebug() << "n  move down by 1";
    qDebug() << "r  reset";
    qDebug() << "t  transform";
    qDebug() << "v  display values";
    qDebug() << "x  exit";
    qDebug() << "M  move up by 100";
    qDebug() << "N  move up by 1";
  }
  if(e->text() == "+") {
    m_scene01->plot_range(true);
    QOpenGLWindow::update(); // schedule paint
  }
  if(e->text() == "-") {
    m_scene01->plot_range(false);
    QOpenGLWindow::update();
  }
  if(e->text() == "a") {
    animate ^= 1;
    if(animate){
      animation_timer->start(ANIMATE_PERIOD);
      elapsed_time.restart();
    }
    else animation_timer->stop();
  }
  if(e->text() == "m") {
    m_scene01->move(false, 100);
    QOpenGLWindow::update();
  }
  if(e->text() == "M") {
    m_scene01->move(true, 100);
    QOpenGLWindow::update();
  }
  if(e->text() == "n") {
    m_scene01->move(false, 1);
    QOpenGLWindow::update();
  }
  if(e->text() == "N") {
    m_scene01->move(true, 1);
    QOpenGLWindow::update();
  }
  if(e->text() == "t") {
    m_scene01->toggle_transform();
    QOpenGLWindow::update();
  }
  if(e->text() == "1") {
    m_scene01->moveto(7005.0);
    QOpenGLWindow::update();
  }
  if(e->text() == "2") {
    m_scene01->moveto(71732.83);
    QOpenGLWindow::update();
  }
  if(e->text() == "c") {
    m_scene01->toggle_background();
    QOpenGLWindow::update();
  }
  if(e->text() == "r") {
    animate = false;
    animation_timer->stop();
    m_scene01->reset();
    QOpenGLWindow::update();
  }
  if(e->text() == "v") {
    m_scene01->toggle_show_vals();
  }
  if(e->text() == "x") {
    QCoreApplication::exit(0);
  }
  if(e->key() == Qt::Key_Up) {
    m_scene01->move(true, 0.01);
    QOpenGLWindow::update();
  }
  if(e->key() == Qt::Key_Down) {
    m_scene01->move(false, 0.01);
    QOpenGLWindow::update();
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
  m_scene01->animation_tick(elapsed_time.elapsed());
  elapsed_time.restart();
  QOpenGLWindow::update(); // schedule paint
}
