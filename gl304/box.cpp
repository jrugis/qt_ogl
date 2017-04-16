#include "box.h"

//****************************************************************************************
// front verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )
// back verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )
// colored cube
static const Vertex sg_verts[] = {
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL, // face 1 (front)
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR, // face 2 (back)
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL, // face 3 (top)
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL, // face 4 (bottom)
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL, // face 5 (left)
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR, // face 6 (right)
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};
#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR
#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR
//****************************************************************************************

CBox::CBox(QOpenGLShaderProgram *shader)
{
  m_shader = shader;

  m_buffer.create();
  m_buffer.bind();
  m_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_buffer.allocate(sg_verts, sizeof(sg_verts));

  m_vao.create();
  m_vao.bind();
  m_shader->enableAttributeArray(0);
  m_shader->enableAttributeArray(1);
  m_shader->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
  m_shader->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
  m_vao.release();

  m_buffer.release();
}

CBox::~CBox()
{
  m_vao.destroy();
  m_buffer.destroy();
}

void CBox::draw()
{
  m_vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_verts) / sizeof(sg_verts[0]));
  m_vao.release();
}
