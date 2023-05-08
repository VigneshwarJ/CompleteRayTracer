#include "MainApp.h"
#include <imgui/imgui.h>
#include <chrono>
#include <memory>
#include "Image.h"
#include "Camera.h"
#include "Phong.h"
#include "Renderer.h"
#include "CheckerBoardTexture.h"
#include "ObjReader.h"
class MainFrame : public ImguiFrame
{

public:
	MainFrame() : camera(45.0f, 0.1f, 100.0f),
		scene(&camera)
	{
		InitializeLights();
		InitializeMaterials();
		InitializeShapes();
		scene.SkyColor = { 0,0,.5f,1 };

	}
	void InitializeLights()
	{
		glm::vec3 lightPoint1 = glm::vec3(-0.121f, 7.0f, -5.0f);

		Light l1{ lightPoint1, glm::vec3(1, 1, 1) , 1.0};
		scene.pointLights.push_back(l1);
	}
	void InitializeShapes()
	{
		//scene.Shapes.push_back(
		//	std::make_unique<Sphere>(
		//		glm::vec3(-0.121f, 1.5014f, -4.724f),
		//		1.0f,
		//		scene.materials[3].get()));
		//scene.Shapes.push_back(
		//	std::make_unique<Sphere>(
		//		glm::vec3(-1.5f, 0.92f, -3.24f),
		//		.75f,
		//		scene.materials[3].get()));
		std::vector<glm::vec3> floorVertices{
			glm::vec3(1, -0.5f, -10),
			glm::vec3(+1, -0.5f, 50),
			glm::vec3(-10, -0.5f, 50),
			glm::vec3(-10, -0.5f, -10) };

		readObjFiles("bunny.obj", scene.materials[3].get(), scene);
		scene.calculateBoundingBoxVoxels();
		scene.buildkdTree();
		scene.kdTree = true;
		//bunny = false;

		//scene.Shapes.push_back(std::make_unique<Plane>(floorVertices, scene.materials[2].get()));

		std::cout << "total shapes : " << scene.Shapes.size() << std::endl;
	}

	void InitializeMaterials()
	{
		scene.materials.push_back(std::make_unique<Phong>(glm::vec3(0, 0,0),
			glm::vec3(1, 1, 1), 0.1f,01.2f,0.33f, 0.33f, 0.33f, 64.f));
		scene.materials.push_back(std::make_unique<Phong>(glm::vec3(0, 0, 0),
			glm::vec3(1, 1, 1),1.f,0.0f,
			0.33f, 0.33f, 0.33f, 32.f));
		scene.textures.push_back(std::make_unique<CheckerBoardTexture>(glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), 4.2));
		scene.materials.push_back(std::make_unique<Phong>(scene.textures[0].get(),
			glm::vec3(1, 1, 1),
			0.33f, 0.33f, 0.33f, 20.5f));
		scene.materials.push_back(std::make_unique<Phong>(glm::vec3(1, 0, 1),
			glm::vec3(1, 1, 1), 0.f, 0.0f,
			0.33f, 0.33f, 0.33f, 32.f));
	}

	void onRender() override {

		ImGui::Begin("Raytracer");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Render Time %.3f ms/frame (%.1f FPS)", m_LastRenderTime, .001/m_LastRenderTime);
		ImGui::Text("Simple Raytracer");
		
		ImGui::Checkbox("KDTree", &isKdTreeEnabled);
		
		//if(ImGui::Button("Load bunny") && bunny)
		//{
			//readObjFiles("bunny.obj", scene.materials[3].get(), scene);
			//bunny = false;
		//}
		//if (isKdTreeEnabled)
		//{
		//	ImGui::Text("Building Kd Tree");
		//	if (!isKdTreeBuilt)
		//	{
		//		scene.calculateBoundingBoxVoxels();
		//		scene.buildkdTree();
		//		isKdTreeBuilt = true;
		//	}
		//		scene.kdTree = true;
		//	
		//}
		//else
		//{
		//	scene.kdTree = false;
		//}
		ImGui::SliderFloat3("light position", light,-100,100);
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
			ImGui::Image(image->GetDescriptorSet(), 
				{ (float)image->GetWidth(), (float)image->GetHeight() },
				{1,1},{0,0});

		ImGui::End();
	}
	void Render() {


		auto start = std::chrono::steady_clock::now();
		//  Insert the code that will be timed
		
		//scene.kdTree = true;
		scene.pointLights[0].Position.x = light[0];
		scene.pointLights[0].Position.y = light[1];
		scene.pointLights[0].Position.z = light[2];
		camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		renderer.Render(scene,camera);
		auto end = std::chrono::steady_clock::now();

		// Store the time difference between start and end
		auto diff = end - start;
		m_LastRenderTime = std::chrono::duration <float,std::milli>(diff).count();
		
	}
private:
	Renderer renderer;
	Scene scene;
	float light[3] = { 0.121f, 6.0f, -13.0f };;
	Camera camera;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	uint32_t* m_ImageData = nullptr;
	float m_LastRenderTime = 0.0f;
	bool isKdTreeBuilt = false;
	bool isKdTreeEnabled = false;
	bool bunny = true;
};







// Main code
int main(int, char**) {

	MainApp* app = new  MainApp();
	app->AddFrames(new MainFrame());
	app->Render();
	delete app;
	return 0;
}