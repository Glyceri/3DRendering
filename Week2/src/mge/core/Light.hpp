#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include "AmberAssignment/ENUMS/LightType.hpp"

/**
 * Exercise for the student: implement the Light class...
 * Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
 */
class Light : public GameObject
{
private:
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	float lightStrength = 0.5f;
	int lightType = LightType::Point;

	public:
		glm::vec3 getLightColour();
		void setLightColour(glm::vec3 value);
		float getLightStrength();
		void setLightStrength(float value);
		int getLightType();
		void setLightType(int type);

		Light(const std::string& aName = nullptr, const glm::vec3& aPosition = glm::vec3( 2.0f, 10.0f, 5.0f ) );
		virtual ~Light();

        //override set parent to register/deregister light...
        virtual void _setWorldRecursively (World* pWorld) override;


		float extraAttrib1 = 1.0f;
		float extraAttrib2 = 0.0f;
		float extraAttrib3 = 0.1f;
};

#endif // LIGHT_HPP
