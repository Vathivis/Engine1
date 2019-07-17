#include <Engine1.h>


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




