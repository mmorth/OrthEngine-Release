#version 330 core

// Maximum number of point lights to store
#define MAX_POINT_LIGHTS 8

// Struct for material properties
struct Material {
    // texture maps
    sampler2DArray diffuseMapArray;
    sampler2D specularMap;

    // specular shininess
    float shininess;
}; 

// Struct for directional light
struct DirectionalLight {
    vec3 direction;

    // Phong shading
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Struct for point light
struct PointLight {
    vec3 position;

    // attenuation constants
    float constant;
    float linear;
    float quadratic;

    // Phong shading
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Struct for spot light
struct SpotLight {
    vec3 position;
    vec3 direction;

    // cutoff cos values
    float innerCutOff;
    float outerCutOff;
  
    // attenuation constants
    float constant;
    float linear;
    float quadratic;

    // Phong shading
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Input variables
in vec3 FragPos; // interpolated fragment position
in vec3 Normal; // normal of the fragment
in vec2 TexCoords; // interpolated fragment texture coordinates
in float TextureArrayIndex;

// Output fragment color
out vec4 FragColor;

// Uniform variables
uniform int numPointLights; // number of point lights (will not render more than MAX_POINT_LIGHTS)
uniform PointLight pointLights[MAX_POINT_LIGHTS]; // array of point lights
uniform vec3 viewPos; // the position of the camera (e.g. view eye)
uniform DirectionalLight directionalLight; // directional light properties
uniform SpotLight spotLight; // spot light properties
uniform Material material; // material properties

// Light function prototypes
vec3 ComputeDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection);
vec3 ComputePointLight(PointLight pointLight, vec3 normal, vec3 viewDirection, vec3 fragPos);
vec3 ComputeSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDirection, vec3 fragPos);

// Main function that computes the fragment's color by summing all the light sources
void main()
{
    // discard fragments with sufficiently low alpha
    vec4 texColor = texture(material.diffuseMapArray, vec3(TexCoords, float(TextureArrayIndex)));
    if(texColor.a < 0.1)
        discard;

    // get unit normal and view direction
    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    // compute amount of each light present
    vec3 lightResult = ComputeDirectionalLight(directionalLight, norm, viewDirection);
    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        if (i < numPointLights)
            lightResult += ComputePointLight(pointLights[i], norm, viewDirection, FragPos);
    }
    lightResult += ComputeSpotLight(spotLight, norm, viewDirection, FragPos);

    // output fragment color
    FragColor = vec4(lightResult, texColor.a);

    if (TextureArrayIndex < -0.5)
        FragColor = vec4(1.0);
}

// Computes the amount of directional light
vec3 ComputeDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection)
{
    // directionalLight.direction points from the light to the fragment. For dot product, we want it from the fragment to the light since normal is from the fragment
    vec3 unitLightDirection = normalize(-directionalLight.direction);
    
    // Phong shading
    float diffuseShading = max(dot(unitLightDirection, normal), 0.0);
    vec3 lightReflection = reflect(-unitLightDirection, normal);
    float specularShading = pow(max(dot(lightReflection, viewDirection), 0.0), material.shininess);

    // compute directional light shading by combining results
    vec3 ambient = directionalLight.ambient * vec3(texture(material.diffuseMapArray, vec3(TexCoords, float(TextureArrayIndex))));
    vec3 diffuse = directionalLight.diffuse * diffuseShading * vec3(texture(material.diffuseMapArray, vec3(TexCoords, float(TextureArrayIndex))));
    vec3 specular = directionalLight.specular * specularShading * vec3(texture(material.specularMap, TexCoords));

    return ambient + diffuse + specular;
}

// Computes the amount of point light
vec3 ComputePointLight(PointLight pointLight, vec3 normal, vec3 viewDirection, vec3 fragPos)
{
    // compute unit light direction to be the direction of the fragment position to the light source
    vec3 unitLightDirection = normalize(pointLight.position - fragPos);
    
    // Phong shading
    float diffuseShading = max(dot(unitLightDirection, normal), 0.0);
    vec3 lightReflection = reflect(-unitLightDirection, normal);
    float specularShading = pow(max(dot(lightReflection, viewDirection), 0.0), material.shininess);

    // attenuation
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
    
    // compute point light shading by combining results
    vec3 ambient = attenuation * pointLight.ambient * vec3(texture(material.diffuseMapArray, vec3(TexCoords, float(TextureArrayIndex))));
    vec3 diffuse = attenuation * pointLight.diffuse * diffuseShading * vec3(texture(material.diffuseMapArray, vec3(TexCoords, float(TextureArrayIndex))));
    vec3 specular = attenuation * pointLight.specular * specularShading * vec3(texture(material.specularMap, TexCoords));

    return ambient + diffuse + specular;
}

// Computes the amount of spot light
vec3 ComputeSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDirection, vec3 fragPos)
{
    // compute unit light direction to be the direction of the fragment position to the light source
    vec3 unitLightDirection = normalize(spotLight.position - fragPos);
    
    // Phong shading
    float diffuseShading = max(dot(unitLightDirection, normal), 0.0);
    vec3 lightReflection = reflect(-unitLightDirection, normal);
    float specularShading = pow(max(dot(lightReflection, viewDirection), 0.0), material.shininess);

    // attenuation
    float distance = length(spotLight.position - fragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));  

    // spotlight intensity
    float theta = dot(unitLightDirection, normalize(-spotLight.direction)); 
    float epsilon = spotLight.innerCutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    // compute point light shading by combining results
    vec3 ambient = attenuation * intensity * spotLight.ambient * vec3(texture(material.diffuseMapArray, vec3(TexCoords, float(TextureArrayIndex))));
    vec3 diffuse = attenuation * intensity * spotLight.diffuse * diffuseShading * vec3(texture(material.diffuseMapArray, vec3(TexCoords, float(TextureArrayIndex))));
    vec3 specular = attenuation * intensity * spotLight.specular * specularShading * vec3(texture(material.specularMap, TexCoords));

    return ambient + diffuse + specular;
}