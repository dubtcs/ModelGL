
#version 330 core

in vec2 textureCoordinates;

out vec4 color;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;

// placeholder
void main(){
    color = texture(textureDiffuse1, textureCoordinates);
}
