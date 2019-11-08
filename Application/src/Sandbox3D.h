#pragma once

#include "Engine1.h"


class Sandbox3D : public Engine1::Layer {
private:

	//temp
	Engine1::ref<Engine1::VertexArray> m_cubeVA;
	Engine1::ref<Engine1::Shader> m_flatColorShader;

	glm::vec4 m_cubeColor = { 0.2f, 0.3f, 0.8f, 1.0f };

public:
	Sandbox3D();
	~Sandbox3D() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(Engine1::Timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(Engine1::Event& event) override;

};

