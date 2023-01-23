#include "UpDownBehaviour.hpp"

UpDownBehaviour::UpDownBehaviour()
{
}

UpDownBehaviour::~UpDownBehaviour()
{
}

void UpDownBehaviour::update(float pStep)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) _owner->translate(glm::vec3(0, -50, 0) * pStep);
		else												  _owner->translate(glm::vec3(0, 50, 0) * pStep);
		
	
}
