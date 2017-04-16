#include <functional>

#include "vertex.h"
#include "scene.h"

#define ROTSP 50.0f // animation rotation speed (milli-seconds per degree)

CScene::CScene()
{
  //glEnable(GL_MULTISAMPLE);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  //glFrontFace(GL_CCW);
  //glEnable(GL_LINE_SMOOTH);
  //glEnable(GL_POINT_SMOOTH);

  m_parallel = true;
  m_background = false;
  toggle_background();

  m_shader = new QOpenGLShaderProgram();
  m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/pcwc_.vert");
  m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/pcwc_.frag");
  m_shader->link();
  m_shader->bind();
  u_world2camera = m_shader->uniformLocation("world2camera");
  u_camera2view = m_shader->uniformLocation("camera2view");

  m_angle = 0.0f;    // for positioning the world in camera space
  m_tilt = 0.0f;     //
  m_offset = -4.0f;  //

  //m_box01 = new CBox(m_shader);

  m_plot01 = new CPlot(m_shader);
  m_show_plot = true;
}
CScene::~CScene()
{
  m_shader->release();
  //delete m_box01;
  delete m_plot01;
  delete m_shader;
}

void CScene::animation_tick(int elapsed_time)
{
  m_angle += elapsed_time/ROTSP;
  if(m_angle > 180.0f) m_angle -= 360.0f;
  if(m_angle < -180.0f) m_angle += 360.0f;
}
void CScene::draw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  m_world2camera.setToIdentity();
  m_world2camera.translate(0.0f, 0.0f, m_offset);
  m_world2camera.rotate(m_tilt, 1.0f, 0.0f, 0.0f);
  m_world2camera.rotate(m_angle, 0.0f, 1.0f, 0.0f);
  m_shader->setUniformValue(u_world2camera, m_world2camera);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //glLineWidth(2);
  //m_box01->draw();
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if(m_show_plot){
    m_plot01->draw();
  }
}
void CScene::set_canvas_size(int width, int height)
{
  m_width = width;
  m_height = height;
  set_projection();
}
void CScene::set_background(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
}
void CScene::set_projection()
{
  float wh = static_cast<float>(m_width) / m_height;
  m_camera2view.setToIdentity();
  if(m_parallel)
    m_camera2view.ortho(-wh, wh, -1.0f, 1.0f,  10.0f, -10.0f);
  else
    m_camera2view.perspective(30.0f, wh, 0.0f, 10.0f);
  m_shader->setUniformValue(u_camera2view, m_camera2view);
}
void CScene::toggle_background()
{
  m_background ^= 1;
  if(m_background) set_background(1.0f, 1.0f, 1.0f, 1.0f); // white
  else set_background(0.0f, 0.0f, 0.0f, 1.0f);             // black
}
void CScene::toggle_plot()
{
  m_show_plot ^= 1;
}

void CScene::toggle_projection()
{
  m_parallel ^= 1;
  set_projection();
}
