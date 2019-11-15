#pragma once

#include "Engine1/Core/Core.h"
#include "Engine1/Core/Layer.h"

#include <vector>

namespace Engine1 {

	/*
		wrapper trida kolem vektoru m_layers pro lepsi manipulaci
		slouzi k managovani vrstev
	*/
	class LayerStack {
	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	public:
		LayerStack() = default;
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	};

}