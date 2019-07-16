#pragma once

#include "Engine1/Core.h"
#include "Engine1/Events/Event.h"

namespace Engine1 {

	/*
		virtualni vrstva, slouzi jako zaklad pro realne vrstvy
	*/
	class ENGINE1_API Layer {
	protected:
		std::string m_debugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& GetName() const { return m_debugName; }
	};

}