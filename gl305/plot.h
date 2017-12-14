#ifndef PLOT_H
#define PLOT_H

#define PLOT_RANGE 10
#define PLOT_ZERO 5.0, 5.0

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <complex>

class CPlot
{
public:
  CPlot(QOpenGLShaderProgram *shader);
  virtual ~CPlot();
  virtual void draw() = 0;                     // pure virtual
  virtual void range(bool increase) = 0;       //
  virtual void reset() = 0;                    //
  virtual void toggle_show_vals() = 0;         //

protected:
  QOpenGLShaderProgram *shader;
  int u_color;
  QOpenGLBuffer vbo_axis, vbo_data;
  QOpenGLVertexArrayObject vao_axis, vao_data;

  int plot_range;
  std::complex<double> plot_zero;
  double plot_scale;
  std::complex<double> plot_offset;

  double source_v;
  bool show_vals;

  virtual void calc() = 0;         // pure virtual
  virtual void setup_axis() = 0;   //
  virtual void setup_data() = 0;   //
};

#endif // PLOT_H
