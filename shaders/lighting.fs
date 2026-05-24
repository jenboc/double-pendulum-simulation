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

vec3 ambientLight = vec3(0.15);

float calculateAttenuation(vec2 fragPos, Light light)
{
    vec2 displacement = fragPos - light.position;
    float dist = dot(displacement, displacement);

    float inner = 0.01 * light.radius;
    float outer = light.radius;

    return 1.0 - smoothstep(inner, outer, dist);
}

vec4 computeLighting(vec2 fragPos)
{
    float totalLight = 1.0;
    vec3 totalColour = vec3(0.0);

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        float atten = calculateAttenuation(fragPos, lights[i]);
        totalLight *= 0.8 * (1 - atten);
        totalColour += atten * lights[i].colour;
    }
        
    return vec4(totalColour, totalLight);
}

void main()
{
    vec2 fragPos = gl_FragCoord.xy;
    vec4 computedLight = computeLighting(fragPos);
    vec4 lighting = vec4(ambientLight * computedLight.xyz, computedLight.w);

    vec4 sceneColour = texture(sceneTexture, fragTexCoord);
    finalColor = lighting;
}
