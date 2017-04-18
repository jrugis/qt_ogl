#include <QDebug>

#include "constants.h"
#include "plot.h"

CPlot::CPlot(QOpenGLShaderProgram *s)
{
  shader = s;
  u_color = shader->uniformLocation("color");

  // axis vertices
  const int pr_int = static_cast<int>(PLOT_RANGE)+1;
  const float pr_float = static_cast<float>(PLOT_RANGE);
  QVector3D axis_verts[4*(pr_int+1)];
  for(int i=0; i<pr_int; i++){
    // horizontal lines
    axis_verts[2*i]            = QVector3D(0.0f, i/pr_float, 0.0f);
    axis_verts[2*i+1]          = QVector3D(1.0f, i/pr_float, 0.0f);
    // vertical lines
    axis_verts[2*pr_int+2*i]   = QVector3D(i/pr_float, 0.0f, 0.0f);
    axis_verts[2*pr_int+2*i+1] = QVector3D(i/pr_float, 1.0f, 0.0f);
  }

  // axis vbo, vao
  vbo_axis.create();  // vbo
  vbo_axis.bind();
  vbo_axis.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_axis.allocate(axis_verts, sizeof(axis_verts));
  vao_axis.create();  // vao
  vao_axis.bind();
  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  vao_axis.release();
  vbo_axis.release();

  // data vbo, vao
  const QVector3D data_verts[MAX_DATA_VERTS] = {};
  vbo_data.create();  // vbo
  vbo_data.bind();
  vbo_data.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  vbo_data.allocate(data_verts, sizeof(data_verts));
  vao_data.create();  // vao
  vao_data.bind();
  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  vao_data.release();
  vbo_data.release();

  // source data
  source = std::complex<double>(0.0, 0.0);
  n_dest_verts = n_source_verts = 1;

  // set plot scale and calculate clamp limits
  plot_scale = 1.0 / PLOT_RANGE;
  plot_offset = std::complex<double>(PLOT_ZERO) / PLOT_RANGE;
  re_lo = -plot_offset.real() / plot_scale;
  re_hi = (1.0 - plot_offset.real()) / plot_scale;
  im_lo = -plot_offset.imag() / plot_scale;
  im_hi = (1.0 - plot_offset.imag()) / plot_scale;

  reset();
}

CPlot::~CPlot()
{
  vao_axis.destroy(); vbo_axis.destroy();
  vao_data.destroy(); vbo_data.destroy();
}

void CPlot::calc()
{
  // transform the source
  std::complex<double> s, d;
  s = source + source_translate;

  // source->destination mapping
  d = std::pow(s, 2);
  //d = std::sin(s);

  // destination range check
  n_dest_verts = 1;
  double re = d.real(); // hide out-of-range?
  if((re < re_lo) or (re > re_hi)) n_dest_verts = 0;
  double im = d.imag(); // hide out-of-range?
  if((im < im_lo) or (im > im_hi)) n_dest_verts = 0;

  // transform and push to vbo
  s = s * plot_scale + plot_offset;
  d = d * plot_scale + plot_offset;
  QVector3D buf[] ={
    QVector3D(s.real(), s.imag(), 0.0f),
    QVector3D(d.real(), d.imag(), 0.0f)
  };
  vbo_data.bind();
  vbo_data.write(0, &buf, sizeof(buf));
  vbo_data.release();
}

void CPlot::draw()
{
  //glLineWidth(12.0f);   // not supported on mac air
  //glEnable(GL_LINE_SMOOTH);
  //glDisable(GL_LINE_SMOOTH);

  // draw axis
  vao_axis.bind();
  shader->setUniformValue(u_color, QVector4D(MED_GRAY, 1.0f));
  glDrawArrays(GL_LINES, 0, 2);                  // bottom
  glDrawArrays(GL_LINES, 2*(PLOT_RANGE+1)-2, 4); // top, left
  glDrawArrays(GL_LINES, 4*(PLOT_RANGE+1)-2, 2); // right
  // horizontal and vertical grid lines
  shader->setUniformValue(u_color, QVector4D(VERY_LIGHT_GRAY, 1.0f));
  glDrawArrays(GL_LINES, 2, 2*(PLOT_RANGE-1));
  glDrawArrays(GL_LINES, 2*(PLOT_RANGE+1)+2, 2*(PLOT_RANGE-1));
  vao_axis.release();

  // draw data: source
  vao_data.bind();
  shader->setUniformValue(u_color, QVector4D(GREEN, 1.0f));
  glPointSize(6.0f);
  glDrawArrays(GL_POINTS, 0, n_source_verts);     // points
  glDrawArrays(GL_LINE_STRIP, 0, n_source_verts); // lines

  // draw data: destination
  shader->setUniformValue(u_color, QVector4D(BLUE, 1.0f));
  glPointSize(6.0f);
  glDrawArrays(GL_POINTS, n_source_verts, n_dest_verts);     // points
  glDrawArrays(GL_LINE_STRIP, n_source_verts, n_dest_verts); // lines
  vao_data.release();
}

void CPlot::move_source(int x, int y)
{
  const float scale = 0.002f / plot_scale;
  source_translate += std::complex<double>(x * scale, y * scale);
  double re = source_translate.real(); // clamp real part
  re = re > re_hi ? re_hi : re;
  re = re < re_lo ? re_lo : re;
  double im = source_translate.imag(); // clamp imaginary part
  im = im > im_hi ? im_hi : im;
  im = im < im_lo ? im_lo : im;
  source_translate = std::complex<double>(re, im);
  calc();
}

void CPlot::reset()
{
  source_translate = std::complex<double>(0.0, 0.0);
  move_source(0, 0); // force clamp check
  calc();
}
