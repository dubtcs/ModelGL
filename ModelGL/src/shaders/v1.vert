
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textCoord;

uniform mat4 toWorldSpace;
uniform mat4 toViewSpace;
uniform mat4 toClipSpace;

out vec2 textureCoordinates;

// placeholder
void main(){
    gl_Position = toClipSpace * toViewSpace * toWorldSpace * vec4(position, 1.0);
    textureCoordinates = textCoord;
}
