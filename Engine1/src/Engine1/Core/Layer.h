#pragma once

#include "Engine1/Core/Core.h"
#include "Engine1/Core/Timestep.h"
#include "Engine1/Events/Event.h"

namespace Engine1 {

	/*
		virtualni vrstva, slouzi jako zaklad pro realne vrstvy
	*/
	class Layer {
	protected:
		std::string m_debugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& GetName() const { return m_debugName; }
	};

}