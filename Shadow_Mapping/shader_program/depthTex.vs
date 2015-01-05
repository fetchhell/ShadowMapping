#version 330

/* attributes */
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

/* uniforms */
uniform mat4 u_viewProjection;
uniform	mat4 u_shadowViewProjection;
uniform mat4 u_model;

/* varyings */
out vec2 vTexCoord;
out vec4 vProjectiveCoord;

void main() {

   vec4 projective_coord = u_shadowViewProjection * u_model * vec4(aPosition, 1.0);
   gl_Position = u_viewProjection * vec4(aPosition, 1.0);

   mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
			  0.0, 0.5, 0.0, 0.0,
			  0.0, 0.0, 0.5, 0.0,
			  0.5, 0.5, 0.4999, 1.0);

   vProjectiveCoord = biasMatrix * projective_coord;
   vTexCoord = aTexCoord;
}