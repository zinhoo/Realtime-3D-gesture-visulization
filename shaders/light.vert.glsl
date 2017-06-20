#version 330 core

layout(location = 0) in vec4 vertex_modelspace;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture;

uniform mat4 myprojection_matrix;
uniform mat4 myview_matrix;
uniform mat3 normal_matrix;
uniform mat4 mymodel_matrix;




out vec2 mytexture;

void main() {

	gl_Position = myprojection_matrix * myview_matrix * mymodel_matrix * vertex_modelspace;

	mytexture = vertex_texture;

}
