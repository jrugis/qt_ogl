#ifndef PLOT_H
#define PLOT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "vertex.h"

//****************************************************************************************
// plot axes
#define VERTEX_0 Vertex(QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f))
#define VERTEX_X Vertex(QVector3D(1.0f,0.0f,0.0f), QVector3D(0.0f,0.0f,0.0f))
#define VERTEX_Y Vertex(QVector3D(0.0f,1.0f,0.0f), QVector3D(0.0f,0.0f,0.0f))
#define VERTEX_Z Vertex(QVector3D(0.0f,0.0f,1.0f), QVector3D(0.0f,0.0f,0.0f))
static const Vertex ax_verts[] = {
  VERTEX_0, VERTEX_X, // x axis
  VERTEX_0, VERTEX_Y, // y axis
  VERTEX_0, VERTEX_Z  // y axis
};
#undef VERTEX_0
#undef VERTEX_X
#undef VERTEX_Y
#undef VERTEX_Z
// plot points
#define POINT_0 Vertex(QVector3D( 0.0f, 0.0f,-0.5f), QVector3D(1.0f,0.0f,0.0f ) )
#define POINT_1 Vertex(QVector3D( 0.1f, 0.0f,-0.5f), QVector3D(0.0f,1.0f,0.0f ) )
static const Vertex pnt_verts[] = {
  POINT_0, POINT_1
};
#undef POINT_0
#undef POINT_1
//****************************************************************************************

class CPlot
{
public:
  CPlot(QOpenGLShaderProgram *shader);
  ~CPlot();
  void draw();

private:
  QOpenGLShaderProgram *m_shader;
  QOpenGLBuffer m_buffer_grid, m_buffer_data;
  QOpenGLVertexArrayObject m_vao_grid, m_vao_data;
};

#endif // PLOT_H
