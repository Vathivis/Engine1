#pragma once

#include "Engine1/Layer.h"

#include "Engine1/Events/ApplicationEvent.h"
#include "Engine1/Events/KeyEvent.h"
#include "Engine1/Events/MouseEvent.h"

namespace Engine1 {

	class ENGINE1_API ImGuiLayer : public Layer {
	private:
		float m_time = 0.0f;

	public:

		ImGuiLayer();
		~ImGuiLayer();


		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;

		void Begin();
		void End();

	private:

	};

}