#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "object.h"
#include "transform3d.h"

class QOpenGLShaderProgram;

class Scene
{
public:
  Scene();
  ~Scene();
  void initialize();
  void resize(int width, int height);
  void draw();
  void animation_tick(int elapsed_time);

private:
  Object *m_object01;

  // OpenGL State Information
  QOpenGLBuffer m_buffer;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLShaderProgram *m_shader;

  // Shader Information
  int u_modelToWorld;
  int u_worldToView;
  QMatrix4x4 m_projection;
  Transform3D m_transform;
  QVector3D m_scale;
  QQuaternion m_rotation;
  QVector3D m_translation;
};

#endif // SCENE_H
