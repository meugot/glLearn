#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 atextCoord;
// uniform float xOffset;
out vec3 ourColor;
out vec3 vertexPos;
out vec2 textCoord;
uniform mat4 transMat;
void main(){
    gl_Position = transMat * vec4(aPos, 1.0);
    vertexPos = aPos;
    ourColor = aColor;
    textCoord = atextCoord;
}
