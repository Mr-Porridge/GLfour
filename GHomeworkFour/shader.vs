#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

//uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    //gl_Position = transform * vec4(position, 1.0f);
	// 注意乘法要从右向左读

    gl_Position = projection * view * model * vec4(position, 1.0f);
	TexCoords = vec2(texCoords.x, 1.0f-texCoords.y);
};
