#ifndef MOUSEROTATEBEHAVIOUR_HPP
#define MOUSEROTATEBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "mge/core/GameObject.hpp"
#include <math.h> 

class MouseRotateBehaviour : public AbstractBehaviour {
private:
	sf::Vector2i lastMousePos;

	void HandleMouseRot(float pStep);
	void HandleSensAdjust(float pStep);
public:
	MouseRotateBehaviour();
	virtual ~MouseRotateBehaviour();
	virtual void update(float pStep);

	float sensitivity = 0.1f;
};

#endif //MOUSEROTATEBEHAVIOUR