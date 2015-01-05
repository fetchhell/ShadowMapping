#version 330

#extension GL_EXT_shadow_samplers : require

out vec4 FragColor;

/* varyings */
in vec2 vTexCoord;
in vec4 vProjectiveCoord;

/* uniforms */
uniform sampler2D shadowmap;

float getlinearDepth(float value) {
    float zNear = 2.0;
    float zFar  = 10000.0;

    return (2 * zNear) / (zFar + zNear - value * (zFar - zNear));
}

void main() {

   float color = texture2D(shadowmap, vTexCoord).x;
   color =  getlinearDepth(color);

   FragColor = vec4(color, color, color, 1.0);
}