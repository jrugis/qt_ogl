#ifndef SCENE_H
#define SCENE_H

#define CAMERA2VIEW_PERSPECTIVE 20.0f, wh, 0.0f, 10.0f
#define MODEL2WORLD_SCALE 1.5f
#define MODEL2WORLD_TRANSLATE -0.5f, -0.5f, 0.0f

#define ANIMATE_REFRESH_RATE 29.0
#define ANIMATE_PERIOD (1000.0 / ANIMATE_REFRESH_RATE)
#define ANIMATE_MS_DEGREE 50.0

#include <QOpenGLShaderProgram>

#include "plotstrip.h"

class QOpenGLShaderProgram;

class CScene
{
public:
  CScene(char* ipaddr, char* port);
  ~CScene();
  void animation_tick(int elapsed_time);
  void draw();
  void move(bool increase, double amount);
  void moveto(double target);
  void toggle_transform();
  void plot_range(bool increase);
  void reset();
  void set_canvas_size(int width, int height);
  void toggle_background();
  void toggle_show_vals();

private:
  int width, height;   // opengl canvas size
  float angle;         // range +-180 degrees
  float tilt;          // range +-90 degrees
  float offset;
  bool dark_background;

  QOpenGLShaderProgram *shader01;
  QMatrix4x4 camera2view, world2camera, model2world;
  int u_camera2view, u_world2camera, u_model2world;

  CPlotStrip *plot01;

  void set_projection();
};

#endif // SCENE_H
