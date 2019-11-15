#pragma once

#include "E1pch.h"


namespace Engine1 {

	class GraphicsContext {
	private:



	public:

		virtual void init() = 0;
		virtual void swapBuffers() = 0;

		static scope<GraphicsContext> create(void* window);


	};




}