#include <QDebug>

#include "constants.h"
#include "plot.h"

CPlot::CPlot(QOpenGLShaderProgram *s)
{
  shader = s;
  u_color = shader->uniformLocation("color");
  plot_range = PLOT_RANGE;
  plot_zero = std::complex<double>(PLOT_ZERO);

  setup_axis();
  setup_data();
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
  std::complex<double> s[5];
  for(int i=0; i<n_source_verts; i++)
    s[i] = source[i] + source_translate;

  // source->destination mapping
  n_dest_verts = n_source_verts;
  std::complex<double> d[5];
  for(int i=0; i<n_source_verts; i++)
    d[i] = std::pow(s[i], 2);
    //d[i] = std::sin(s[i]);

  // destination range check
  show_dest = true;
  double re = d[0].real(); // hide out-of-range?
  if((re < re_lo) or (re > re_hi)) show_dest = false;
  double im = d[0].imag(); // hide out-of-range?
  if((im < im_lo) or (im > im_hi)) show_dest = false;

  // transform and push to vbo
  for(int i=0; i<n_source_verts; i++){
    s[i] = s[i] * plot_scale + plot_offset;
    d[i] = d[i] * plot_scale + plot_offset;
  }
  QVector3D buf[16]; // as line segment vertices
  for(int i=0; i<4; i++){
    buf[2*i]   = QVector3D(s[0].real(),   s[0].imag(),   0.0f);
    buf[2*i+1] = QVector3D(s[i+1].real(), s[i+1].imag(), 0.0f);
    buf[2*i+8] = QVector3D(d[0].real(),   d[0].imag(),   0.0f);
    buf[2*i+9] = QVector3D(d[i+1].real(), d[i+1].imag(), 0.0f);
  }
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
  glDrawArrays(GL_LINES, 2*(plot_range+1)-2, 4); // top, left
  glDrawArrays(GL_LINES, 4*(plot_range+1)-2, 2); // right
  // horizontal and vertical grid lines
  shader->setUniformValue(u_color, QVector4D(VERY_LIGHT_GRAY, 1.0f));
  glDrawArrays(GL_LINES, 2, 2*(plot_range-1));
  glDrawArrays(GL_LINES, 2*(plot_range+1)+2, 2*(plot_range-1));
  vao_axis.release();

  // draw data: source
  vao_data.bind();
  shader->setUniformValue(u_color, QVector4D(GREEN, 1.0f));
  glPointSize(3.0f);
  glDrawArrays(GL_LINES, 0, 8);  // lines
  glDrawArrays(GL_POINTS, 1, 1); // points

  // draw data: destination
  if(show_dest){
    shader->setUniformValue(u_color, QVector4D(BLUE, 1.0f));
    glPointSize(3.0f);
    glDrawArrays(GL_LINES, 8, 8); // lines
    glDrawArrays(GL_POINTS, 9, 1); // points
  }
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

void CPlot::range(bool increase)
{
  if(increase){
    plot_range += 2;
    plot_zero += std::complex<double>(1.0, 1.0);
  }
  else{
    plot_range -= 2;
    plot_zero -= std::complex<double>(1.0, 1.0);
  }
  vao_axis.destroy();
  vbo_axis.destroy();
  setup_axis();
  move_source(0, 0);
}

void CPlot::setup_axis()
{
  // axis vertices
  QVector3D *axis_verts = new QVector3D[4*(plot_range+1)];
  const float pr_float = static_cast<float>(plot_range); // convenience typecast
  for(int i=0; i<plot_range+1; i++){
    // horizontal lines
    axis_verts[2*i]            = QVector3D(0.0f, i/pr_float, 0.0f);
    axis_verts[2*i+1]          = QVector3D(1.0f, i/pr_float, 0.0f);
    // vertical lines
    axis_verts[2*(plot_range+1)+2*i]   = QVector3D(i/pr_float, 0.0f, 0.0f);
    axis_verts[2*(plot_range+1)+2*i+1] = QVector3D(i/pr_float, 1.0f, 0.0f);
  }

  // axis vbo, vao
  vbo_axis.create();  // vbo
  vbo_axis.bind();
  vbo_axis.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_axis.allocate(axis_verts, 4*(plot_range+1) * sizeof(axis_verts[0]));
  vao_axis.create();  // vao
  vao_axis.bind();
  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  vao_axis.release();
  vbo_axis.release();
  delete[] axis_verts;

  // set plot scale and calculate clamp limits
  plot_scale = 1.0 / plot_range;
  plot_offset = plot_zero * plot_scale;
  re_lo = -plot_offset.real() / plot_scale;
  re_hi = (1.0 - plot_offset.real()) / plot_scale;
  im_lo = -plot_offset.imag() / plot_scale;
  im_hi = (1.0 - plot_offset.imag()) / plot_scale;
}

void CPlot::setup_data()
{
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

  // source data: centered cross shape
  source[0] = std::complex<double>( 0.0,  0.0);
  source[1] = std::complex<double>( 0.1,  0.0);
  source[2] = std::complex<double>( 0.0,  0.1);
  source[3] = std::complex<double>(-0.1,  0.0);
  source[4] = std::complex<double>( 0.0, -0.1);
  n_dest_verts = n_source_verts = 5;
}

void CPlot::reset()
{
  source_translate = std::complex<double>(0.0, 0.0);
  move_source(0, 0); // force clamp check
  calc();
}
