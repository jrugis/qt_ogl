#version 330
layout(location = 0) in vec3 position;

uniform mat4 model2world;
uniform mat4 world2camera;
uniform mat4 camera2view;

void main(void)
{
  gl_Position = camera2view * world2camera * model2world * vec4(position, 1.0);
}
