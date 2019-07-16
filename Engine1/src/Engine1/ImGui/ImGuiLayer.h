#pragma once

#include "Engine1/Layer.h"

namespace Engine1 {

	class ENGINE1_API ImGuiLayer : public Layer {
	private:
		float m_time = 0.0f;

	public:

		ImGuiLayer();
		~ImGuiLayer();


		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(Event& event);

	};

}