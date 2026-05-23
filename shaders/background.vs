#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;

out vec2 fragTexCoord;
out vec4 fragColor;

vec2 screenSize = vec2(800, 800);

void main()
{
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    vec2 pos = vertexPosition.xy / screenSize * 2.0 - 1.0;

    gl_Position = vec4(pos.x, -pos.y, 0.0, 1.0);
}
