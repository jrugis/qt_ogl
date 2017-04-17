#ifndef PLOT_H
#define PLOT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#define MAX_DATA_VERTS 100

class CPlot
{
public:
  CPlot(QOpenGLShaderProgram *shader);
  ~CPlot();
  void draw();
  void move_source(int x, int y);
  void reset();

private:
  QOpenGLShaderProgram *shader;
  int u_color;
  QOpenGLBuffer vbo_axis, vbo_data;
  QOpenGLVertexArrayObject vao_axis, vao_data;
  int n_data_verts;
  QVector3D source;
  QVector3D source_translate;

  void calc();
};

#endif // PLOT_H
