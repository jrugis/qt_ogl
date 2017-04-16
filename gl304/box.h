#ifndef BOX_H
#define BOX_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "vertex.h"

class CBox
{
public:
  CBox(QOpenGLShaderProgram *shader);
  ~CBox();
  void draw();

private:
  QOpenGLShaderProgram *m_shader;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_buffer;
};

#endif // BOX_H
