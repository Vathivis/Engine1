#include "E1pch.h"
#include "Renderer.h"

namespace Engine1 {

	void Renderer::beginScene()	{

	}

	void Renderer::endScene() {

	}

	void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray) {
		vertexArray->bind();
		RenderCommand::DrawIndexed(vertexArray);

	}

}

