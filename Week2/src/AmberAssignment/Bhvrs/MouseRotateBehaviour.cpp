#include "MouseRotateBehaviour.hpp"

MouseRotateBehaviour::MouseRotateBehaviour() : AbstractBehaviour()
{
}

MouseRotateBehaviour::~MouseRotateBehaviour()
{
}

void MouseRotateBehaviour::update(float pStep)
{
	HandleSensAdjust(pStep);
	HandleMouseRot(pStep);
}

void MouseRotateBehaviour::HandleMouseRot(float pStep) {
	sf::Vector2i moved = lastMousePos - sf::Mouse::getPosition();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		//glm::mat4 matrix = _owner->getWorldTransform();
		_owner->getParent()->rotate(moved.x * pStep * sensitivity, glm::vec3(0, 1, 0));
		_owner->rotate(moved.y * pStep * 0.1f, glm::vec3(1, 0, 0));

		glm::mat4 m = _owner->getTransform();
		float x = atan2f(-m[1][2], m[2][2]);
		x = glm::degrees(x);
		if (x < -45 || x > 45)
			_owner->rotate(-moved.y * pStep * 0.1f, glm::vec3(1, 0, 0));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		_owner->translate(glm::vec3(0, 0, -moved.y * pStep * 0.3f));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		_owner->translate(glm::vec3(moved.x * pStep * 0.3f, 0, 0));
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		_owner->translate(glm::vec3(0, -moved.y * pStep * 0.3f,0));
	}
	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		_owner->setLocalPosition(glm::vec3(0, 0, 0));
	lastMousePos = sf::Mouse::getPosition();
}

void MouseRotateBehaviour::HandleSensAdjust(float pStep)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) sensitivity += 0.1f * pStep;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen)) sensitivity -= 0.1f * pStep;
}
