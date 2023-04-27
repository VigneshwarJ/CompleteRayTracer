#include "Phong.h"
#include "Shapes.h"

Phong::Phong(glm::vec3 objectColor, 
	glm::vec3 specColor, 
	float ambient, 
	float diffuse, 
	float specular, 
	float specHighlight):
	C_o(objectColor),
	C_s(specColor),
	k_a(ambient),
	k_d(diffuse),
	k_s(specular),
	k_e(specHighlight)
{

}

glm::vec4 Phong::GetColor(RayHitInfo record,const Scene& scene)
{
	glm::vec3 diffuseColor = { 0,0,0 };
	glm::vec3 specularColor = { 0,0,0 };
	glm::vec3 ambientColor = C_o * k_a;;
	glm::vec3 lightDir;
	glm::vec3 viewDir;
	glm::vec3 shadowPosition;
	glm::vec3 halfway;
	float spec = 0;
	float NdotL =0;
	float diffuse = 0.0f;
	viewDir = -record.ray.Direction;
	//viewDir =  glm::normalize(scene.camera->GetDirection());
	for (int i = 0; i < scene.pointLights.size(); i++) {
		const Light& pointLight = scene.pointLights[i];

		glm::vec3 normal = record.WorldNormal;
		lightDir = glm::normalize( pointLight.Position - record.WorldPosition);

		// blinn phong
		//halfway = glm::reflect(-lightDir, -normal);
		halfway = glm::reflect(-lightDir , -normal);
	
		// blinn

		glm::vec3 shadowRayorig = record.WorldPosition + (lightDir * 1.0f);
		Ray shadowRay{ shadowRayorig, lightDir };
		RayHitInfo shadowRec;

		if (scene.Intersects(shadowRay, shadowRec)) {

			continue;
		}

		// calculate diffuse lighting 
		NdotL = glm::max(glm::dot(normal, lightDir), 0.0f);
		 diffuse = pointLight.intensity * NdotL;
		 diffuseColor += (C_o * pointLight.color * diffuse * k_d);
		//calculate specular highlight
		spec = glm::pow(glm::max(glm::dot(viewDir, halfway), 0.f), k_e);
	    specularColor += (C_s* pointLight.color * pointLight.intensity * spec * k_s);
	}

	return glm::vec4(  ambientColor+ diffuseColor + specularColor, 1.0);
	//return glm::vec4(spec);
}
