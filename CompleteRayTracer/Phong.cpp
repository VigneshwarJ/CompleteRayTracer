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
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 lightDir;
	glm::vec3 viewDir;
	float NdotL =0;
	for (int i = 0; i < scene.pointLights.size(); i++) {
		const Light& pointLight = scene.pointLights[i];

		glm::vec3 normal = record.WorldNormal;
		lightDir = glm::normalize( pointLight.Position - record.WorldPosition);
		 viewDir = glm::normalize(scene.camera->GetPosition() - record.WorldPosition);

		// blinn phong
		glm::vec3 halfway = glm::normalize(lightDir + viewDir);

		// blinn


		float distance = (pointLight.Position - record.WorldPosition).length();

		//glm::vec3 shadowRayorig = record.WorldPosition + (lightDir * 0.001f);
		//Ray shadowRay{ shadowRayorig, lightDir };
		//RayHitInfo shadowRec;

		//if (scene.Intersects(shadowRay, shadowRec)) {
		//	continue;
		//}
		// calculate diffuse lighting 
		NdotL = glm::max(glm::dot(normal, lightDir), 0.0f);
		float diffuse = /*pointLight.intensity **/ NdotL;
		diffuseColor += (C_o * pointLight.color * diffuse * k_d);
		//specularColor = C_o * pointLight.color * diffuse;
		//calculate specular highlight
		float spec = pow(glm::max(glm::dot(normal,halfway), 0.0f), k_e);
	    specularColor = specularColor + (pointLight.color * pointLight.intensity * spec * k_s);
	}

	return glm::vec4(diffuseColor+specularColor,1.0);
}
