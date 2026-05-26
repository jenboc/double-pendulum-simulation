#version 330

#define NUM_LIGHTS 2

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

struct Light
{
    vec2 position;
    vec3 colour;
    float radius;
};

uniform Light lights[NUM_LIGHTS];
uniform sampler2D sceneTexture;

vec3 fog = vec3(0.5);

float calculateAttenuation(vec2 fragPos, Light light)
{
    vec2 displacement = fragPos - light.position;
    float dist = length(displacement);

    float inner = 0.01 * light.radius;
    float outer = light.radius;

    return 1.0 - smoothstep(inner, outer, dist);
}

vec3 computeLighting(vec2 fragPos)
{
    float totalLight = 1.0;
    vec3 totalColour = vec3(0.0);

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        float atten = calculateAttenuation(fragPos, lights[i]);
        totalColour += atten * lights[i].colour;
    }
        
    return totalColour;
}

void main()
{
    vec2 fragPos = gl_FragCoord.xy;
    vec3 computedLight = computeLighting(fragPos);

    vec2 texCoordFlipped = vec2(fragTexCoord.x, 1 - fragTexCoord.y);
    vec4 sceneColour = texture(sceneTexture, texCoordFlipped);
    vec3 colour = clamp((fog * sceneColour.xyz) + computedLight, 0.0, 1.0);
    finalColor = vec4(colour, sceneColour.w);
}
