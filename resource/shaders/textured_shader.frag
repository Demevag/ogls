#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform vec3 viewPos;

void main()
{    
    vec3 lightColor = vec3(0.7, 0.7, 0.7);
    vec3 lightPos = vec3(2.0, 2.0, 2.0);
    vec3 lightDiffuse = lightColor;
    vec3 lightAmbient = vec3(0.1, 0.1, 0.1); //lightDiffuse * vec3(0.4, 0.4, 0.4);
    vec3 lightSpec = vec3(1.0, 1.0, 1.0);

    vec4 objColor = texture(texture_diffuse1, TexCoords);
    vec4 objSpec = texture(texture_specular1, TexCoords);
    vec4 objNormal = texture(texture_normal1, TexCoords);
    //vec4 objColor = vec4(0.5, 0.5, 0.5, 1.0);   

    // ambient
    float ambientStrength = 0.4;
    vec3 ambient = lightAmbient * vec3(objColor);
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * (diff * vec3(objColor));
            
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = lightSpec * (spec * vec3(objSpec));  
        
    vec3 result = (ambient + diffuse + specular) ;
    FragColor = vec4(result, 1.0);
}
