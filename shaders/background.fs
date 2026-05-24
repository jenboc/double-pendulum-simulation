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

vec3 ambientLight = vec3(0.15);

float calculateAttenuation(vec2 fragPos, Light light)
{
    vec2 displacement = fragPos - light.position;
    float dist = dot(displacement, displacement);

    float inner = 0.01 * light.radius;
    float outer = light.radius;

    return 1.0 - smoothstep(inner, outer, dist);
}

vec3 computeLighting(vec2 fragPos)
{
    vec3 accum = vec3(0.0);

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        float atten = calculateAttenuation(fragPos, lights[i]);
        accum += atten * lights[i].colour;
    }
    
    return accum;
}

void main()
{
    vec2 fragPos = gl_FragCoord.xy;
    vec3 lighting = ambientLight + computeLighting(fragPos);

    finalColor = vec4(clamp(lighting, 0.0, 1.0), fragColor.a);
}
