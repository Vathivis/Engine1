#pragma once

#include "Engine1/Input.h"

namespace Engine1 {

	class WindowsInput : public Input {
	public:



	protected:

		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> getMousePositionImpl() const override;
		virtual float getMouseXImpl() const override;
		virtual float getMouseYImpl() const override;
		

	};


}

