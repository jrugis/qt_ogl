#ifndef PLOT_H
#define PLOT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <complex>

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

  double plot_scale;
  std::complex<double> plot_offset;
  double re_lo, re_hi, im_lo, im_hi; // clamp limits

  std::complex<double> source;
  std::complex<double> source_translate;
  int n_source_verts, n_dest_verts;

  void calc();
};

#endif // PLOT_H
