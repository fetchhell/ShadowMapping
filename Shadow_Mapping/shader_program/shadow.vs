#version 330

/* attributes */
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

/* uniforms */
uniform	mat4 u_shadowViewProjection;
uniform mat4 u_model;

void main() {

   gl_Position = u_shadowViewProjection * u_model * vec4(aPosition, 1.0);
}