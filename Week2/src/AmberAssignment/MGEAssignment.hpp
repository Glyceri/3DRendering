#ifndef MGEASSIGNMENT_HPP
#define MGEASSIGNMENT_HPP

#include "mge/core/AbstractGame.hpp"
#include <mge/materials/AbstractMaterial.hpp>
#include <mge/core/Light.hpp>

class DebugHud;

/**
 * An example subclass of AbstractGame showing how we can setup a scene.
 */
class MGEAssignment : public AbstractGame
{
	//PUBLIC FUNCTIONS

public:
	MGEAssignment();
	virtual ~MGEAssignment();

	//override initialize so we can add a DebugHud
	virtual void initialize();

	virtual void mouseScrolled(int value);
	void Recursive(GameObject* gameObject, int value);

protected:
	//override so we can construct the actual scene
	virtual void _initializeScene();
	void _initializeSceneOLD(); //This is used so that I can keep using this script
	//override render to render the hud as well.
	virtual void _render(bool renderAsShadows);

private:
	DebugHud* _hud;                   //hud display

	void _updateHud();

	MGEAssignment(const MGEAssignment&);
	MGEAssignment& operator=(const MGEAssignment&);
	
	Light* CreateLight(Mesh* lightMesh, AbstractMaterial* lightMaterial, int lightType);

	Mesh* InitializeMesh(int author, std::string fileName, int modelType);
};

#endif // MGEAssignment_HPP
