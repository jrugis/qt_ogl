#include <QDebug>
#include <QExposeEvent>
#include <QString>

#include "glwindow.h"

GLWindow::GLWindow()
{
  m_scene01 = new Scene();
}

GLWindow::~GLWindow()
{
  delete m_scene01;
}

void GLWindow::initializeGL()
{
  initializeOpenGLFunctions();
  qDebug() << (const char*)glGetString(GL_RENDERER);
  qDebug() << "OpenGL" << (const char*)glGetString(GL_VERSION);
  qDebug() << "GLSL" << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  m_scene01->initialize();
}

void GLWindow::resizeGL(int width, int height)
{
  m_scene01->resize(width, height);
}

void GLWindow::paintGL()
{
  m_scene01->paint();
}

void GLWindow::update() // locked to vsync
{
  m_scene01->update();
  QOpenGLWindow::update();
}
