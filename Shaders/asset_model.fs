#version 430 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;  

uniform vec3 viewPos;
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

float near = 1.0; 
float far  = 40.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{   
    vec3 normal = vec3(texture(texture_normal1, TexCoords));
    vec3 lightDir = normalize(-lightPos);
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 ambient = lightColor * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = lightColor * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = lightColor * spec * vec3(texture(texture_specular1, TexCoords));

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
  //  float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
  //  FragColor = vec4(vec3(depth), 1.0);
}
