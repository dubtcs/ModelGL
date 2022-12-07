
#version 330 core

in vec2 textureCoordinates;

out vec4 color;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;

// placeholder
void main(){
    color = vec4(vec3(gl_FragCoord.z), 1.0);
}
