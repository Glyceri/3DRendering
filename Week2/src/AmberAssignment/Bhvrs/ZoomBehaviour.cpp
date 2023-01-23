#include "ZoomBehaviour.hpp"
#include "SFML/Window/Keyboard.hpp"

ZoomBehaviour::ZoomBehaviour()
{
}

ZoomBehaviour::~ZoomBehaviour()
{
}

void ZoomBehaviour::update(float pStep)
{
	float speed = 20.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		speed = 200.0f;

	if (zoomedThisTick != 0) {
		glm::vec3 lp = _owner->getLocalPosition();
		lp.z += -zoomedThisTick * pStep * speed;
		_owner->setLocalPosition(lp);
	}

	zoomedThisTick = 0;
}

void ZoomBehaviour::Zoomed(int value)
{
	zoomedThisTick += value;

}
