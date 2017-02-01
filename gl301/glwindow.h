#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class GLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
  Q_OBJECT // required for signals and slots

public:
  ~GLWindow();

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void destroyGL();

private:
  QOpenGLBuffer m_buffer;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLShaderProgram *m_shader;
};

#endif // GLWINDOW_H
