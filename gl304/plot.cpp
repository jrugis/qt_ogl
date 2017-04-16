#include "plot.h"

CPlot::CPlot(QOpenGLShaderProgram *shader)
{
  m_shader = shader;

  // axis buffer
  m_buffer_grid.create();
  m_buffer_grid.bind();
  m_buffer_grid.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_buffer_grid.allocate(ax_verts, sizeof(ax_verts));
  m_vao_grid.create();
  m_vao_grid.bind();
  m_shader->enableAttributeArray(0);
  m_shader->enableAttributeArray(1);
  m_shader->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
  m_shader->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
  m_vao_grid.release();
  m_buffer_grid.release();

  // data buffer
  m_buffer_data.create();
  m_buffer_data.bind();
  m_buffer_data.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  m_buffer_data.allocate(pnt_verts, sizeof(pnt_verts));
  m_vao_data.create();
  m_vao_data.bind();
  m_shader->enableAttributeArray(0);
  m_shader->enableAttributeArray(1);
  m_shader->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
  m_shader->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
  m_vao_data.release();
  m_buffer_data.release();
}

CPlot::~CPlot()
{
  m_vao_grid.destroy();
  m_buffer_grid.destroy();
  m_buffer_data.destroy();
}

void CPlot::draw()
{
  m_buffer_data.bind();
  Vertex temp = Vertex(QVector3D(-0.2f, -0.2f, -0.5f), QVector3D( 0.0f, 0.0f, 0.0f ));
  m_buffer_data.write(0, &temp, sizeof(temp));
  m_buffer_data.release();

  m_vao_grid.bind();
  //glLineWidth(10.0f);
  glDrawArrays(GL_LINES, 0, sizeof(ax_verts) / sizeof(ax_verts[0]));
  m_vao_grid.release();

  m_vao_data.bind();
  glPointSize(3.0f);
  glDrawArrays(GL_POINTS, 0, sizeof(pnt_verts) / sizeof(pnt_verts[0]));
  glDrawArrays(GL_LINES, 0, sizeof(pnt_verts) / sizeof(pnt_verts[0]));
  m_vao_data.release();
}
