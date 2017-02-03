#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

#include "transform3d.h"

class QExposeEvent;
class QOpenGLShaderProgram;

class GLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
  Q_OBJECT // required for signals and slots

public:
  GLWindow();
  ~GLWindow();

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void exposeEvent(QExposeEvent *ev);

protected slots:
  void destroyGL();
  void update();

private:
  // OpenGL State Information
  QOpenGLBuffer m_buffer;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLShaderProgram *m_shader;

  // Shader Information
  int u_modelToWorld;
  int u_worldToView;
  QMatrix4x4 m_projection;
  Transform3D m_transform;

  // Fix for Windows
  QRegion m_cachedRegion;
};

#endif // GLWINDOW_H
