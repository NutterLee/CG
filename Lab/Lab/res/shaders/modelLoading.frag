#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse;

void main( )
{
    //color = vec4( texture( texture_diffuse, TexCoords ));
	color = vec4(1.0f, 0.5f, 0.2f,1.0f);
}