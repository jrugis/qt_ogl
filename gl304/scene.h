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

private:
  QOpenGLShaderProgram *m_shader;
  int u_worldToView;

  int u_modelToWorld;
  QMatrix4x4 m_projection;
  Transform3D m_transform;
  QVector3D m_scale;
  QQuaternion m_rotation;
  QVector3D m_translation;

  CBox *m_box01;
};

#endif // SCENE_H
