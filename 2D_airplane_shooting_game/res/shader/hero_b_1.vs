#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform float posXOffset;
uniform float posYOffset;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = vec4(aPos.x + posXOffset, aPos.y + posYOffset, aPos.z, 1.0);
	//gl_Position = projection * view * model * vec4(aPos, 1.0);

	gl_Position = model * vec4(aPos, 1.0);

	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}