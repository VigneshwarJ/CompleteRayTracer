#include "MainApp.h"
#include <imgui/imgui.h>
#include <memory>
#include "Image.h"
#include "Renderer.h"
#include "Camera.h"
#include "Phong.h"
class MainFrame : public ImguiFrame
{

public:
	MainFrame() : camera(45.0f, 0.1f, 100.0f)
	{
		InitializeLights();
		InitializeMaterials();
		InitializeShapes();
		scene.camera = &camera;
	}
	void InitializeLights()
	{
		glm::vec3 lightPoint1 = glm::vec3(-0.121f, 7.0f, -5.0f);

		Light l1{ lightPoint1, glm::vec3(1, 1, 1) , 1.0};
		scene.pointLights.push_back(l1);
	}
	void InitializeShapes()
	{
		scene.Shapes.push_back(
			std::make_unique<Sphere>(
				glm::vec3(-0.121f, 1.5014f, -4.724f),
				1.0f,
				scene.materials[0].get()));
		scene.Shapes.push_back(
			std::make_unique<Sphere>(
				glm::vec3(-1.5f, 0.92f, -3.94f),
				.75f,
				scene.materials[1].get()));
		std::vector<glm::vec3> floorVertices{
			glm::vec3(1, -0.5f, -10),
			glm::vec3(+1, -0.5f, 100),
			glm::vec3(-10, -0.5f, 100),
			glm::vec3(-10, -0.5f, -10) };


		scene.Shapes.push_back(std::make_unique<Plane>(floorVertices, scene.materials[2].get()));
	}

	void InitializeMaterials()
	{
		scene.materials.push_back(std::make_unique<Phong>(glm::vec3(0, 1, 0),
			glm::vec3(1, 1, 1), 0.33f, 0.33f, 0.33f, 20.5f));
		scene.materials.push_back(std::make_unique<Phong>(glm::vec3(.5, .5, .5),
			glm::vec3(1, 1, 1),
			0.33f, 0.33f, 0.33f, 20.5f));
		scene.materials.push_back(std::make_unique<Phong>(glm::vec3(1, 0, 0),
			glm::vec3(1, 1, 1),
			0.33f, 0.33f, 0.33f, 20.5f));
	}

	void onRender() override {

		ImGui::Begin("Raytracer");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Simple Raytracer");
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::SliderFloat3("light position", color,-100,100);
		ImGui::End();
		ImGui::Begin("Viewport");
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		auto image = renderer.GetFinalImage();

		if (image)
			ImGui::Image(image->GetDescriptorSet(), 
				{ (float)image->GetWidth(), (float)image->GetHeight() },
				{1,1},{0,0});

		ImGui::End();
		//Render();
		//ImGui::PopStyleVar();
		Render();
	}
	void Render() {
		scene.pointLights[0].Position.x = color[0];
		scene.pointLights[0].Position.y = color[1];
		scene.pointLights[0].Position.z = color[2];
		camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		renderer.Render(scene,camera);
	}
private:
	Renderer renderer;
	Scene scene;
	float color[3] = { 0.121f, 6.0f, -13.0f };;
	Camera camera;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	uint32_t* m_ImageData = nullptr;
	float m_LastRenderTime = 0.0f;
};



// Main code
int main(int, char**) {

	MainApp* app = new  MainApp();
	app->AddFrames(new MainFrame());
	app->Render();
	delete app;
	return 0;
}