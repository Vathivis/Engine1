#pragma once

#include "Engine1/Window.h"

#include "Engine1/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Engine1 {

	class WindowsWindow : public Window {
	private:
		GLFWwindow* m_window;
		GraphicsContext* m_context;	

		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool VSync;

			EventCallbackFn eventCallback;
		};

		WindowData m_data;

	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;

		inline unsigned int getWidth() const override { return m_data.width; }
		inline unsigned int getHeight() const override { return m_data.height; }

		// Window attributes
		inline void setEventCallback(const EventCallbackFn& callback) override { m_data.eventCallback = callback; }
		void setVSync(bool enabled = false) override;
		bool isVSync() const override;

		inline virtual void* getNativeWindow() const override { return m_window; }

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();

	};

}
