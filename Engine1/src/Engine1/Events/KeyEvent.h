#pragma once


#include "Event.h"

namespace Engine1 {

	/*
		zakaldni trida pro praci s klavesou
		abstraktni trida
	*/
	class ENGINE1_API KeyEvent : public Event {
	protected:
		KeyEvent(int keycode) : m_keyCode(keycode) {}

		int m_keyCode;

	public:
		inline int getKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	};

	/*
		trida pro zmacknuti klavesy a drzeni
	*/
	class ENGINE1_API KeyPressedEvent : public KeyEvent {
	private:
		int m_repeatCount;

	public:
		KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {}

		inline int getRepeatCount() const { return m_repeatCount; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	/*
		trida pro pusteni klavesy
	*/
	class ENGINE1_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}