#version 330 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D boxTexture;
uniform sampler2D faceTexture;

void main()
{
	FragColor = mix(texture(boxTexture, texCoord), texture(faceTexture,texCoord), 0.2) * vec4(ourColor, 1.0f);
}