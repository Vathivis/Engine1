#pragma once

#include "Engine1.h"


class Sandbox2D : public Engine1::Layer{
private:
	//camera
	Engine1::OrthographicCameraController m_cameraController;

	//temp
	Engine1::ref<Engine1::VertexArray> m_squareVA;
	Engine1::ref<Engine1::Shader> m_flatColorShader;

	Engine1::ref<Engine1::Texture2D> m_checkerboardTexture;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };




public:
	Sandbox2D();
	~Sandbox2D() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(Engine1::Timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(Engine1::Event& event) override;

};