#version 420 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 vertexPos;
in vec2 textCoord;
uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
void main(){
    //FragColor = mix(texture(ourTexture, textCoord), texture(ourTexture2, textCoord), 0.2);
    FragColor = vec4(ourColor, 1.0f);
    // if the third argument of mix() is 0 then it will show first texture, and if 1.0 then it will show second texture.
}
