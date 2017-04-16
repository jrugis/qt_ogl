#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec4 vColor;

uniform mat4 world2camera;
uniform mat4 camera2view;

void main()
{
  gl_Position = camera2view * world2camera * vec4(position, 1.0);
  vColor = vec4(color, 1.0f);
  //vColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
