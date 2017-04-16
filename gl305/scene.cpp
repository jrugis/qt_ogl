#include <functional>

#include "scene.h"

#define ROTSP 50.0f // animation rotation speed (milli-seconds per degree)

CScene::CScene()
{
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  reset();

  shader01 = new QOpenGLShaderProgram();
  shader01->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/001.vert");
  shader01->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/001.frag");
  shader01->link();
  shader01->bind();
  u_camera2view = shader01->uniformLocation("camera2view");
  u_world2camera = shader01->uniformLocation("world2camera");
  u_model2world = shader01->uniformLocation("model2world");

  plot01 = new CPlot(shader01);
}

CScene::~CScene()
{
  delete plot01;
  shader01->release();
  delete shader01;
}

void CScene::animation_tick(int elapsed_time)
{
  angle += elapsed_time/ROTSP;
  if(angle > 180.0f) angle -= 360.0f;
  if(angle < -180.0f) angle += 360.0f;
}

void CScene::draw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  world2camera.setToIdentity();
  world2camera.translate(0.0f, 0.0f, offset);
  world2camera.rotate(tilt, 1.0f, 0.0f, 0.0f);
  world2camera.rotate(angle, 0.0f, 1.0f, 0.0f);
  shader01->setUniformValue(u_world2camera, world2camera);

  model2world.setToIdentity();
  model2world.scale(1.5f);
  model2world.translate(-0.5f, -0.5f, 0.0f);
  shader01->setUniformValue(u_model2world, model2world);
  plot01->draw();
}

void CScene::reset()
{
  angle = 0.0f;    // for positioning the world in camera space
  tilt = 0.0f;     //
  offset = -5.0f;  //
}

void CScene::set_canvas_size(int w, int h)
{
  width = w;
  height = h;
  set_projection();
}

void CScene::set_projection()
{
  float wh = static_cast<float>(width) / height;
  camera2view.setToIdentity();
  camera2view.perspective(20.0f, wh, 0.0f, 10.0f);
  shader01->setUniformValue(u_camera2view, camera2view);
}
