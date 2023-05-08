#include "Phong.h"
#include "Shapes.h"
#include "Scene.h"

Phong::Phong(glm::vec3 objectColor, 
	glm::vec3 specColor, float reflective, float transperency,
	float ambient, 
	float diffuse, 
	float specular, 
	float specHighlight):
	C_o(objectColor),
	C_s(specColor),
	k_r(reflective),
	k_a(ambient),
	k_d(diffuse),
	k_s(specular),
	k_e(specHighlight),
	texture(nullptr)
{
	ReflectiveConst = k_r;
	TransperancyConst = transperency;
}

Phong::Phong(Texture* texture, glm::vec3 specColor, float ambient, float diffuse, float specular, float specHighlight):
	texture(texture),
	C_s(specColor),
	
	k_a(ambient),
	k_d(diffuse),
	k_s(specular),
	k_e(specHighlight),
	C_o({1.0f,1.0f,1.0f})
{
	
}

glm::vec4 Phong::GetColor(RayHitInfo record,const Scene& scene)
{
	glm::vec3 diffuseColor = { 0,0,0 };
	glm::vec3 specularColor = { 0,0,0 };
	glm::vec3 ambientColor = C_o * k_a;;
	glm::vec3 shadowDir;
	glm::vec3 viewDir;
	glm::vec3 shadowPosition;
	glm::vec3 halfway;
	float spec = 0;
	float NdotL =0;
	float diffuse = 0.0f;
	viewDir = -record.ray.Direction;
	if (texture)
	{
		glm::vec2 uv = record.hitObject->getUV(record.WorldPosition);
		C_o = texture->value(uv.x, uv.y);
	}
	//viewDir =  glm::normalize(scene.camera->GetDirection());
	for (int i = 0; i < scene.pointLights.size(); i++) {
		const Light& pointLight = scene.pointLights[i];

		glm::vec3 normal = record.WorldNormal;
		shadowDir = glm::normalize( pointLight.Position - record.WorldPosition);

		// blinn phong
		//halfway = glm::reflect(-lightDir, -normal);
		halfway = glm::reflect(-shadowDir , normal);
	
		// blinn

		glm::vec3 shadowRayorig = record.WorldPosition + (shadowDir * 1.0f);
		Ray shadowRay{ shadowRayorig, shadowDir };
		RayHitInfo shadowRec;

		if (scene.Intersects(shadowRay, shadowRec)) {

			continue;
		}

		// calculate diffuse lighting 
		NdotL = glm::max(glm::dot(normal, -shadowDir), 0.0f);
		diffuse = pointLight.intensity * NdotL;
		diffuseColor += (C_o * pointLight.color * diffuse * k_d);
		//calculate specular highlight
		spec = glm::pow(glm::max(glm::dot(viewDir, halfway), 0.f), k_e);
	    specularColor += (C_s* pointLight.color * pointLight.intensity * spec * k_s);
	}

	return glm::vec4(  ambientColor+ diffuseColor + specularColor, 1.0);
}
