#include "vertex.h"
#include "scene.h"

#define ROTSP 50.0f // animation rotation speed (milli-seconds per degree)

CScene::CScene()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  //glFrontFace(GL_CCW);
  glEnable(GL_LINE_SMOOTH);

  m_parallel = false;

  m_shader = new QOpenGLShaderProgram();
  m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
  m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
  m_shader->link();
  m_shader->bind();

  u_world = m_shader->uniformLocation("world");
  u_view = m_shader->uniformLocation("view");

  m_transform.translate(0.0f, 0.0f, -4.0f);
  m_box01 = new CBox(m_shader);
}

CScene::~CScene()
{
  m_shader->release();
  delete m_box01;
  delete m_shader;
}

void CScene::resize(int width, int height)
{
  float wh = width / float(height);
  m_projection.setToIdentity();
  if(m_parallel)
    m_projection.ortho(-wh, wh, -1.0f, 1.0f,  10.0f, -10.0f);
  else
    m_projection.perspective(30.0f, width / float(height), 0.0f, 100.0f);
  m_shader->setUniformValue(u_view, m_projection);
}

void CScene::draw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  m_shader->setUniformValue(u_world, m_transform.toMatrix());

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(2);
  m_box01->draw();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CScene::animation_tick(int elapsed_time)
{
  m_transform.rotate(elapsed_time / ROTSP, QVector3D(0.4f, 0.3f, 0.3f));
}
