#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords));
	vec3 specularMapColor = vec3(texture(material.specular, TexCoords));
	vec3 emissiveMapColor = vec3(texture(material.emission, TexCoords));

	vec3 ambient =  light.ambient * diffuseMapColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * diffuseMapColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * specularMapColor;
	
	vec3 result = ambient + diffuse + specular;

	if (specularMapColor == vec3(0.0f))
		result += emissiveMapColor;

	FragColor = vec4(result, 1.0f);
}