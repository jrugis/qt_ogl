#ifndef OBJECT_H
#define OBJECT_H

class Scene;

class Object
{
public:
  Object(Scene *parent);
  ~Object();

private:
  Scene *m_scene;
};

#endif // OBJECT_H
