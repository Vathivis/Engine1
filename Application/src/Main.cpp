
#include <Engine1.h>

class ExampleLayer : public Engine1::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void onUpdate() override
	{
		E1_INFO("ExampleLayer::Update");
	}

	void onEvent(Engine1::Event& event) override
	{
		E1_TRACE("{0}", event);
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
	}

	~IPS() {

	}


};


Engine1::Application* Engine1::CreateApplication() {
	return new IPS();
}




