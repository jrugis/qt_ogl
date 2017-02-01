#include <QDebug>
#include <QOpenGLShaderProgram>

#include "glwindow.h"
#include "vertex.h"

// colored triangle
static const Vertex tri_verts[] = {
  Vertex(QVector3D( 0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)),
  Vertex(QVector3D( 0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)),
  Vertex(QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f))
};

GLWindow::~GLWindow()
{
  makeCurrent();
  destroyGL();
}

void GLWindow::initializeGL()
{
  initializeOpenGLFunctions();
  qDebug() << (const char*)glGetString(GL_RENDERER);
  qDebug() << "OpenGL" << (const char*)glGetString(GL_VERSION);
  qDebug() << "GLSL" << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // create shader
  m_shader = new QOpenGLShaderProgram();
  m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
  m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
  m_shader->link();
  m_shader->bind();

  // create buffer
  m_buffer.create();
  m_buffer.bind();
  m_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_buffer.allocate(tri_verts, sizeof(tri_verts));

  // create vertex array object
  m_vao.create();
  m_vao.bind();
  m_shader->enableAttributeArray(0);
  m_shader->enableAttributeArray(1);
  m_shader->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
  m_shader->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

  // release (unbind) all
  m_vao.release();
  m_buffer.release();
  m_shader->release();
}

void GLWindow::resizeGL(int width, int height)
{
  (void)width;
  (void)height;
}

void GLWindow::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // render using shaders
  m_shader->bind();
  m_vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, sizeof(tri_verts) / sizeof(tri_verts[0]));
  m_vao.release();
  m_shader->release();
}

void GLWindow::destroyGL()
{
  m_vao.destroy();
  m_buffer.destroy();
  delete m_shader;
}
