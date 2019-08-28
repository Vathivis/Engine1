#pragma once
#include "E1pch.h"
#include "Engine1/Core.h"

namespace Engine1 {

	// Eventy v Engine1 ted blokuji, to znamena ze pokud prijde event
	// zacne se ihned resit a musi byt zpracovan
	// do budoucna je lepsi strategie eventy davat do bufferu
	// a zpracovavat je v EventHandle casti update faze enginu

	//typy eventu
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	//kategorie eventu - soubory eventu pro mys, klavesnici ...
	//bit shifting kvuli moznosti vice kategorii zaroven - KeyPressed je keyboard a input event ...
	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	//makro pro dopsani virtualnich funkci k eventum - usetreni mista
#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

	//stejne makro ale pro kategorie
#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	/*
		zakladni trida pro eventy od ktere dedi dalsi typy eventu
	*/
	class ENGINE1_API Event {
	public:
		bool handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;	//pozdeji pouze debug ne release
		virtual int getCategoryFlags() const = 0;	//debug
		virtual std::string toString() const { return getName(); }		//debug

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;	//0 = zadna kategorie
		}
	};

	/*
		trida na posilani eventu podle jejich typu
		event ziskame jako referenci -> vytvorime EventDispatcher s timto eventem jako argument
		muzeme pote volat funkce zavisle na tomto eventu
	*/
	class EventDispatcher {
	public:


		EventDispatcher(Event& event) : m_event(event) {}

		template<typename T, typename F>
		bool dispatch(const F& func) {
			if (m_event.getEventType() == T::getStaticType()) {
				m_event.handled = func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

	//vystupni operator pro logger
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}
