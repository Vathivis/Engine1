#pragma once

#include "Engine1/Core.h"
#include "Layer.h"

#include <vector>

namespace Engine1 {

	/*
		wrapper trida kolem vektoru m_layers pro lepsi manipulaci
		slouzi k managovani vrstev
	*/
	class ENGINE1_API LayerStack {
	private:
		std::vector<Layer*> m_layers;
		std::vector<Layer*>::iterator m_layerInsert;
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	};

}