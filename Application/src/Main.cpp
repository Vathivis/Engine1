#include <Engine1.h>

#include "imgui/imgui.h"

class ExampleLayer : public Engine1::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void onUpdate() override
	{
		if (Engine1::Input::isKeyPressed(E1_KEY_TAB))
			E1_TRACE("Tab key is pressed (poll)!");
	}

	virtual void onImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
		
		
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

class Sandbox : public Engine1::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Engine1::Application* Engine1::CreateApplication()
{
	return new Sandbox();
}