#include <Engine1.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

class ExampleLayer : public Engine1::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void onUpdate() override
	{
		if (Engine1::Input::isKeyPressed(E1_KEY_TAB)) {
			E1_INFO("Tab key pressed (poll)");
		}
	}

	void onEvent(Engine1::Event& event) override
	{
		if (event.getEventType() == Engine1::EventType::KeyPressed)
		{
			auto& e = (Engine1::KeyPressedEvent&)event;
			if (e.getKeyCode() == E1_KEY_TAB)
				E1_TRACE("Tab key is pressed (event)!");
			E1_TRACE("{0}", (char)e.getKeyCode());
		}
	}

};

/*
	trida pro hlavni instanci programu pod engine Engine1
*/
class IPS : public Engine1::Application {
private:


public:

	IPS() {
		pushLayer(new ExampleLayer());
		pushOverlay(new Engine1::ImGuiLayer());
	}

	~IPS() {

	}


};


Engine1::Application* Engine1::CreateApplication() {
	return new IPS();
}




