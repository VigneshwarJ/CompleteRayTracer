#include "MainApp.h"
#include <imgui/imgui.h>
#include <memory>
#include "Image.h"
class MainFrame : public ImguiFrame
{

public:
	void onRender() override {

		ImGui::Begin("Raytracer");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Simple Raytracer");
		if (ImGui::Button("Render"))
			Render();
		ImGui::End();
		ImGui::Begin("Viewport");
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });

		ImGui::End();
		//ImGui::PopStyleVar();
		Render();
	}

	void Render()
	{
		

		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight())
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++)
		{
			m_ImageData[i] += i;
			m_ImageData[i] |= 0xff000000;
		}

		m_Image->SetData(m_ImageData);


	}
private:
	std::shared_ptr<Image> m_Image;
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