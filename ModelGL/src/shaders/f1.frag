
#version 330 core

in vec2 textureCoordinates;

out vec4 color;

uniform sampler2D textureDiffuse1;

// placeholder
void main(){
    color = vec4(1.0, 0.5, 0.5, 1.0);//texture(textureDiffuse1, textureCoordinates);
}
