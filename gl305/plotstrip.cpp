#define register // used by pari but deprecated in c++11
#include <pari/pari.h>
#undef register

#include "constants.h"
#include "plotstrip.h"

CPlotStrip::CPlotStrip(QOpenGLShaderProgram *shader):CPlot(shader)
{
  setup_axis();
  setup_data();
  reset();
}

CPlotStrip::~CPlotStrip()
{
  delete[] source;
}

void CPlotStrip::calc()
{
  // transform the source
  std::complex<double> *s;
  s = new std::complex<double>[n_source_verts];
  for(int i=0; i<n_source_verts; i++)
    s[i] = source[i] + source_translate;

  // apply source -> destination mapping
  n_dest_verts = n_source_verts;
  std::complex<double> *d;
  d = new std::complex<double>[n_dest_verts];
  pari_sp psp = avma; // pari stack pointer
  GEN x(cgetc(DEFAULTPREC)), z(cgetc(DEFAULTPREC)); // pari variables
  for(int i=0; i<n_source_verts; i++){
    if(s[i] == std::complex<double>(1.0, 0.0)) return; // avoid zeta pole
    x = gadd(dbltor(s[i].real()), gmul(dbltor(s[i].imag()), gen_I()));
    z = gzeta(x, DEFAULTPREC); // pari zeta function
    d[i] = std::complex<double>(gtodouble(greal(z)), gtodouble(gimag(z)));
  }
  avma = psp; // clear the pari stack
  if(show_vals) qDebug() << "s = (" << s[0].real() << "," << s[0].imag()
          << ")   d = (" << d[0].real() << "," << d[0].imag() << ")";

  //for(int i=0; i<n_dest_verts; i++)
  //  d[i] -= d[n_dest_verts-1];

  // transform and push to vbo
  for(int i=0; i<n_source_verts; i++){
    s[i] = s[i] * plot_scale + plot_offset;
    d[i] = d[i] * plot_scale + plot_offset;
  }

  QVector3D *buf;
  buf = new QVector3D[2*n_source_verts];
  for(int i=0; i<n_source_verts; i++){
    buf[i]    = QVector3D(s[i].real(),   s[i].imag(),   0.0f);
    buf[i+n_source_verts] = QVector3D(d[i].real(),   d[i].imag(),   0.0f);
  }
  vbo_data.bind();
  vbo_data.write(0, buf, 2*n_source_verts*sizeof(QVector3D));
  vbo_data.release();
  delete[] buf;
  delete[] d;
  delete[] s;
}

void CPlotStrip::draw()
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

  // draw data: destination
  glPointSize(3.0f);
  shader->setUniformValue(u_color, QVector4D(BLUE, 1.0f));
  glDrawArrays(GL_POINTS, n_source_verts, n_dest_verts); // points
  glDrawArrays(GL_LINE_STRIP, n_source_verts, n_dest_verts);  // lines
  shader->setUniformValue(u_color, QVector4D(RED, 1.0f));
  glDrawArrays(GL_POINTS, n_source_verts+n_dest_verts/2, 1); // point
  vao_data.release();
}

void CPlotStrip::move_source(int x, int y)
{
  const float scale = 0.002f / plot_scale;
  source_translate += std::complex<double>(x * scale, y * scale);
  calc();
}

void CPlotStrip::range(bool increase)
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

void CPlotStrip::reset()
{
  source_translate = std::complex<double>(0.0, 0.0);
  //source_translate = std::complex<double>(0.0, 7005.0);
  move_source(0, 0); // force clamp check
  calc();
}

void CPlotStrip::setup_axis()
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

void CPlotStrip::setup_data()
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

  n_source_verts = N_SPIRAL_VERTS;
  source = new std::complex<double>[n_source_verts];
  for(int i=0; i<n_source_verts; i++)
    source[i] = std::complex<double>(1.00*i/(n_source_verts-1), 0.0);
}

void CPlotStrip::toggle_show_vals()
{
  show_vals ^= 1;
  calc();
}
