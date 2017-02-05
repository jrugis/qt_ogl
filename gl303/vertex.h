#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

class Vertex
{
public:
  // constructors
  //Vertex();
  //explicit Vertex(const QVector3D &position);
  Vertex(const QVector3D &position, const QVector3D &color);

  // accessors / mutators
  //const QVector3D& position() const;
  //const QVector3D& color() const;
  //void setPosition(const QVector3D& position);
  //void setColor(const QVector3D& color);

  // ogl shader helpers
  static const int PositionTupleSize = 3;
  static const int ColorTupleSize = 3;
  static int positionOffset();
  static int colorOffset();
  static int stride();

private:
  QVector3D m_position;
  QVector3D m_color;
};

Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE); // can be memcpy'd

// ******* inline implementation *******

// constructors
//inline Vertex::Vertex() {}
//inline Vertex::Vertex(const QVector3D &position) : m_position(position) {}
inline Vertex::Vertex(const QVector3D &position, const QVector3D &color) : m_position(position), m_color(color) {}

// accessors / mutators
//inline const QVector3D& Vertex::position() const {return m_position;}
//inline const QVector3D& Vertex::color() const {return m_color;}
//void inline Vertex::setPosition(const QVector3D& position) {m_position = position;}
//void inline Vertex::setColor(const QVector3D& color) {m_color = color;}

// ogl shader helpers
inline int Vertex::positionOffset() {return offsetof(Vertex, m_position);}
inline int Vertex::colorOffset() {return offsetof(Vertex, m_color);}
inline int Vertex::stride() {return sizeof(Vertex);}

#endif // VERTEX_H
