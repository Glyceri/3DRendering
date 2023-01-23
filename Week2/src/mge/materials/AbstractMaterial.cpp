#include "mge/materials/AbstractMaterial.hpp"

AbstractMaterial::AbstractMaterial()
{
    //ctor
}

AbstractMaterial::~AbstractMaterial()
{
    //dtor
}

void AbstractMaterial::setShiny(int val)
{
    shinyness = val;
}

int AbstractMaterial::getShiny()
{
    return shinyness;
}

