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

private:
  QOpenGLShaderProgram *shader;
  int u_color;
  QOpenGLBuffer vbo_axis, vbo_data;
  QOpenGLVertexArrayObject vao_axis, vao_data;
  int n_data_verts;
};

#endif // PLOT_H
