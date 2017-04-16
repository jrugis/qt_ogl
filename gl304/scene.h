#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLShaderProgram>

#include "box.h"
#include "plot.h"

class QOpenGLShaderProgram;
//class CBox;

class CScene
{
public:
  CScene();
  ~CScene();
  void animation_tick(int elapsed_time);
  void draw();
  void set_canvas_size(int width, int height);
  void toggle_background();
  void toggle_plot();
  void toggle_projection();

private:
  int u_camera2view, u_world2camera;
  int m_width, m_height;
  bool m_parallel, m_background, m_show_plot;
  float m_angle; // range +-180 degrees
  float m_tilt;  // range +-90 degrees
  float m_offset;
  QMatrix4x4 m_camera2view, m_world2camera;
  QOpenGLShaderProgram *m_shader;
  //CBox *m_box01;
  CPlot *m_plot01;

  void set_background(float r, float g, float b, float alpha);
  void set_projection();
};

#endif // SCENE_H
