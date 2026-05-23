#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

struct Light
{
    vec2 position;
    float lightRadius;
};

uniform Light lights[3];

vec4 calculateColour(Light light, vec2 fragCoord)
{
    vec2 toCentre = fragCoord - light.position;
    float dist2 = dot(toCentre, toCentre);

    float inner = 0.2 * light.lightRadius;
    float alpha = smoothstep(inner, light.lightRadius, dist2);


    return vec4(0.0, 0.0, 0.0, alpha);
}

void main()
{
    float alpha = 1.0;
    for (int i = 0; i < 3; i++)
    {
        vec4 colour = calculateColour(lights[i], gl_FragCoord.xy);
        alpha *= colour.w;
    }

    vec3 rgb = normalize(finalColor.xyz);

    finalColor = vec4(rgb, alpha);
}
