#version 310 es
precision mediump float; 
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Light
{
    vec3 position; 
    vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
};
//Define number of spotlights
#define NR_SPOTLIGHTS 2

uniform Light light[NR_SPOTLIGHTS];
//uniform Light light;
uniform sampler2D texture_diffuse1;

uniform vec3 viewPos; 
uniform vec3 objectColor;

vec3 CalcSpotlight(Light light)
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(light.direction));
    if(theta > light.cutOff)
    {
        
        //Apply light.ambient for when have several ligths in world
        //Ambient
        vec3 ambient = light.ambient;

        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * light.diffuse;
        
        // specular
    // float specularStrength = 0.4;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * light.specular;
        //vec3 specular = specularStrength * spec * lightColor;  

        //SpotLight
        //float theta = dot(lightDir, normalize(-light.direction));
    
        //Attenuation (Point Light)
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
        //ambient  *= attenuation; 
        //diffuse   *= attenuation;
        //specular *= attenuation;
            
        vec3 texColor = texture(texture_diffuse1, TexCoords).rgb;
        vec3 result = (ambient + diffuse + specular) * texColor;
       // FragColor = vec4(result, 1.0);
       return result;
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
       // FragColor = vec4(light[0].ambient * texture(texture_diffuse1, TexCoords).rgb, 1.0);
       return (light.ambient * texture(texture_diffuse1, TexCoords).rgb);
    }
}

void main()
{    
    vec3 result = vec3(0.f);
    for(int i = 0; i < NR_SPOTLIGHTS; i++)
    {
        result += CalcSpotlight(light[i]);
    }
    FragColor = vec4(result, 1.f);
}