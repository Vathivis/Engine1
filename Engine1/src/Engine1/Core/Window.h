#pragma once

#include "E1pch.h"

#include "Engine1/Core/Core.h"
#include "Engine1/Events/Event.h"

namespace Engine1 {

	//defaultni vlastnosti
	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Engine1", unsigned int width = 1280, unsigned int height = 720)
			: title(title), width(width), height(height) {}
	};

	// Interface representing a desktop system based Window, multiplatform
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		//platform dependent - implemented per platform
		static Window* create(const WindowProps& props = WindowProps());
	};

}