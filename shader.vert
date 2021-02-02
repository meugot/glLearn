#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
uniform float xOffset;
out vec3 ourColor;
out vec3 vertexPos;
void main(){
    gl_Position = vec4(aPos.x+xOffset, aPos.y, aPos.z, 1.0);
    vertexPos = aPos;
    ourColor = aColor;
}
