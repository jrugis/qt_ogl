#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLShaderProgram>

#include "transform3d.h"
#include "box.h"

class QOpenGLShaderProgram;
class CBox;

class CScene
{
public:
  CScene();
  ~CScene();
  void resize(int width, int height);
  void draw();
  void animation_tick(int elapsed_time);

  bool m_parallel;

private:
  int u_view;
  int u_world;
  QOpenGLShaderProgram *m_shader;
  QMatrix4x4 m_projection;
  Transform3D m_transform;
  //QVector3D m_scale;
  //QQuaternion m_rotation;
  //QVector3D m_translation;

  CBox *m_box01;
};

#endif // SCENE_H
