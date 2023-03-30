#include "MainApp.h"
#include <imgui/imgui.h>
#include <memory>
#include "Image.h"
#include "Renderer.h"
#include "Camera.h"
class MainFrame : public ImguiFrame
{

public:
	MainFrame() : camera(45.0f, 0.1f, 100.0f)
	{
		materials.push_back(Material(glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			1));
		materials.push_back(Material(glm::vec3(.5, .5, .5),
			glm::vec3(0, 1, 0),
			1));
		materials.push_back(Material(glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			1));
		std::vector<glm::vec3> floorVertices{ 
			glm::vec3(0.85f, -0.5f, 1), 
			glm::vec3(+1, -0.5f, 10), 
			glm::vec3(-2, -0.5f, 10), 
			glm::vec3(-2, -0.5f, 1) };
		shapes.push_back(std::make_unique<Sphere>(glm::vec3(-0.121f, 1.5014f, -4.724f), 1.0f, &materials[0]));
		shapes.push_back(std::make_unique<Sphere>(glm::vec3(1.25f, 0.92f, -3.94f), .75f, &materials[1]));
		shapes.push_back(std::make_unique<Plane>(floorVertices,&materials[2]));
	}

	void onRender() override {

		ImGui::Begin("Raytracer");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Simple Raytracer");
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();
		ImGui::Begin("Viewport");
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		auto image = renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() });

		ImGui::End();
		//ImGui::PopStyleVar();
		//Render();
	}
	void Render() {
		camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		renderer.Render(shapes,camera);
	}
private:
	Renderer renderer;
	std::vector<std::unique_ptr<Shape>> shapes;
    std::vector<Material> materials;
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