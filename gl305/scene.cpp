#include <functional>

#include "constants.h"
#include "scene.h"

CScene::CScene(char* ipaddr, char* port)
{
  dark_background = true;
  toggle_background();

  shader01 = new QOpenGLShaderProgram();
  shader01->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/001.vert");
  shader01->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/001.frag");
  shader01->link();
  shader01->bind();
  u_camera2view = shader01->uniformLocation("camera2view");
  u_world2camera = shader01->uniformLocation("world2camera");
  u_model2world = shader01->uniformLocation("model2world");

  plot01 = new CPlotStrip(ipaddr, port, shader01);
  reset();
}

CScene::~CScene()
{
  delete plot01;
  shader01->release();
  delete shader01;
}

void CScene::animation_tick(int elapsed_time)
{
  plot01->plot_tick();
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
  model2world.scale(MODEL2WORLD_SCALE);
  model2world.translate(MODEL2WORLD_TRANSLATE);
  shader01->setUniformValue(u_model2world, model2world);
  plot01->draw();
}

void CScene::move(bool increase, double amount)
{
  plot01->move(increase, amount);
}

void CScene::moveto(double target)
{
  plot01->moveto(target);
}

void CScene::toggle_transform()
{
  plot01->toggle_transform();
}

void CScene::plot_range(bool increase)
{
  plot01->range(increase);
}

void CScene::reset()
{
  angle = 0.0f;    // for positioning the world in camera space
  tilt = 0.0f;     //
  offset = -5.0f;  //
  plot01->reset();
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
  camera2view.perspective(CAMERA2VIEW_PERSPECTIVE);
  shader01->setUniformValue(u_camera2view, camera2view);
}

void CScene::toggle_background()
{
  dark_background ^= 1;
  if(dark_background) glClearColor(BLACK3, 1.0f);
  else glClearColor(WHITE3, 1.0f);
}

void CScene::toggle_show_vals()
{
  plot01->toggle_show_vals();
}
