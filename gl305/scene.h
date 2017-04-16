#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLShaderProgram>

#include "plot.h"

class QOpenGLShaderProgram;

class CScene
{
public:
  CScene();
  ~CScene();
  void animation_tick(int elapsed_time);
  void draw();
  void set_canvas_size(int width, int height);
  void reset();

private:
  int width, height;   // opengl canvas size
  float angle;         // range +-180 degrees
  float tilt;          // range +-90 degrees
  float offset;

  QOpenGLShaderProgram *shader01;
  QMatrix4x4 camera2view, world2camera, model2world;
  int u_camera2view, u_world2camera, u_model2world;

  CPlot *plot01;

  void set_projection();
};

#endif // SCENE_H
