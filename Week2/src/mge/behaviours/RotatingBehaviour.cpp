#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/core/GameObject.hpp"

RotatingBehaviour::RotatingBehaviour():AbstractBehaviour()
{
	//ctor
}

RotatingBehaviour::RotatingBehaviour(float speed, glm::vec3 axis) :AbstractBehaviour()
{
	//ctor
	this->speed = speed;
	this->axis = axis;
}


RotatingBehaviour::~RotatingBehaviour()
{
	//dtor
}

void RotatingBehaviour::update(float pStep)
{
    //rotates 45° per second
	_owner->rotate(pStep * glm::radians(speed), axis );
}
