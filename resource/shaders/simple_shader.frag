#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;

uniform vec3 objColor;
uniform vec3 viewPos;

void main()
{    
    vec3 lightColor = vec3(0.7, 0.7, 0.7);
    vec3 lightPos = vec3(4.0, 3.0, 5.0);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * vec3(objColor);
    FragColor = vec4(result, 1.0);
}
