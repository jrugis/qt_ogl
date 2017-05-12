#include <string>
#include <QDebug>

#include "plot.h"

CPlot::CPlot(QOpenGLShaderProgram *s)
{
  shader = s;
  u_color = shader->uniformLocation("color");
  plot_range = PLOT_RANGE;
  plot_zero = std::complex<double>(PLOT_ZERO);
  show_vals = false;
}

CPlot::~CPlot()
{
  vao_axis.destroy(); vbo_axis.destroy();
  vao_data.destroy(); vbo_data.destroy();
}
