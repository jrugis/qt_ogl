
#include "vertex.h"
#include "scene.h"

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
  // face 1 (front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
  // face 2 (back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
  // face 3 (top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
  // face 4 (bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
  // face 5 (left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
  // face 6 (right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
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

Scene::Scene()
{
  m_object01 = new Object(this);
  m_transform.translate(0.0f, 0.0f, -5.0f);
}

Scene::~Scene()
{
  m_vao.destroy();
  m_buffer.destroy();
  delete m_shader;
  delete m_object01;
}

void Scene::initialize()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_CULL_FACE);

  // create shader
  m_shader = new QOpenGLShaderProgram();
  m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
  m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
  m_shader->link();
  m_shader->bind();

  // cache uniform locations
  u_modelToWorld = m_shader->uniformLocation("modelToWorld");
  u_worldToView = m_shader->uniformLocation("worldToView");

  // create buffer
  m_buffer.create();
  m_buffer.bind();
  m_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_buffer.allocate(sg_verts, sizeof(sg_verts));

  // create vertex array object
  m_vao.create();
  m_vao.bind();
  m_shader->enableAttributeArray(0);
  m_shader->enableAttributeArray(1);
  m_shader->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
  m_shader->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

  // release (unbind) all
  m_vao.release();
  m_buffer.release();
  m_shader->release();
}

void Scene::resize(int width, int height)
{
  m_projection.setToIdentity();
  m_projection.perspective(30.0f, width / float(height), 0.0f, 100.0f);
}

void Scene::paint()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // render using shaders
  m_shader->bind();
  m_shader->setUniformValue(u_worldToView, m_projection);
  m_shader->setUniformValue(u_modelToWorld, m_transform.toMatrix());
  m_vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_verts) / sizeof(sg_verts[0]));
  m_vao.release();
  m_shader->release();
}

void Scene::update() // locked to vsync
{
  m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
}
