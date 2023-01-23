#include <iostream>
#include <string>

#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "AmberAssignment/ENUMS/TypeWisher.hpp"
#include "AmberAssignment/ENUMS/AuthorType.hpp"
#include "AmberAssignment/config.hpp"
#include "AmberAssignment/MGEAssignment.hpp"
#include "AmberAssignment/ENUMS/ModelType.hpp"
#include "AmberAssignment/Bhvrs/FollowBehaviour.hpp"
#include "AmberAssignment/Bhvrs/MouseRotateBehaviour.hpp"
#include "AmberAssignment/Materials/MultiTextureMaterial.hpp"
#include "AmberAssignment/Bhvrs/UpDownBehaviour.hpp"
#include "AmberAssignment/Bhvrs/ZoomBehaviour.hpp"
#include "AmberAssignment/Materials/ShadowMaterial.hpp"



//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEAssignment::MGEAssignment() :AbstractGame(), _hud(0)
{
  
}


void MGEAssignment::initialize() {
    //setup the core part
    AbstractGame::initialize();


    //setup the custom part so we can display some text
    std::cout << "Initializing HUD" << std::endl;
    _hud = new DebugHud(_window);
    std::cout << "HUD initialized." << std::endl << std::endl;
}

void MGEAssignment::mouseScrolled(int value)
{
    Recursive(_world, value);
}

void MGEAssignment::Recursive(GameObject* gameObject, int value)
{
    AbstractBehaviour* currBhvr = gameObject->getBehaviour();
    if (currBhvr != nullptr) 
        if (typeid(*currBhvr).name() == typeid(ZoomBehaviour).name()) 
            ((ZoomBehaviour*)currBhvr)->Zoomed(value);
        
    
    for (int i = 0; i < gameObject->getChildCount(); i++) 
        Recursive(gameObject->getChildAt(i), value);
    
}

void MGEAssignment::_initializeScene() {

    AbstractMaterial* blackMaterial = new TextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + "black.png"));
    Mesh* planeMeshDefault = InitializeMesh(AuthorType::MGE, "plane", ModelType::OBJ);
    Mesh* sphereMeshS = InitializeMesh(AuthorType::MGE, "sphere_smooth", ModelType::OBJ);
    AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 0));
    AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
    Mesh* monke_Mesh = InitializeMesh(AuthorType::MGE, "suzanna_smooth", ModelType::OBJ);
    Mesh* monkeHat_Mesh = InitializeMesh(AuthorType::MGE, "cone_smooth", ModelType::OBJ);
    AbstractMaterial* hatMaterial = new ColorMaterial(glm::vec3(1, 0, 0));
    AbstractMaterial* bricksMaterial = new TextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + "SHIRT_BC.png"));
    AbstractMaterial* bananaMaterial = new TextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + "Banana_02_DIFF.png"));
    Mesh* bananaMesh = InitializeMesh(AuthorType::Amber, "Banana_02_MESH_TRIS", ModelType::OBJ);

    Camera* camera = new Camera("camera", glm::vec3(0, 0, 7));
    camera->setBehaviour(new ZoomBehaviour());

    GameObject* cameraHolder = new GameObject("Camera Holder", glm::vec3(0, 4.5f, 0));
    GameObject* cameraHolderMover = new GameObject("Camera Holder Mover");
    cameraHolder->setBehaviour(new MouseRotateBehaviour());

    _world->add(camera);
    _world->setMainCamera(camera);
    _world->add(cameraHolderMover);
    cameraHolderMover->add(cameraHolder);
    cameraHolder->add(camera);


    Mesh* terrainMesh = InitializeMesh(AuthorType::Amber, "plane_8192", ModelType::OBJ);
    Texture* textures[] = {
            Texture::load(config::AMBER_TEXTURE_PATH + "terrain/diffuse1.jpg"),
            Texture::load(config::AMBER_TEXTURE_PATH + "terrain/diffuse2.jpg"),
            Texture::load(config::AMBER_TEXTURE_PATH + "terrain/diffuse3.jpg"),
            Texture::load(config::AMBER_TEXTURE_PATH + "terrain/diffuse4.jpg")
    };
    AbstractMaterial* terrainMaterial = new MultiTextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + "terrain/heightmap5.png"), Texture::load(config::AMBER_TEXTURE_PATH + "terrain/splatmap.png"), textures);

    GameObject* terrain = new GameObject("Terrain!");
    terrain->setMesh(terrainMesh);
    terrain->setMaterial(terrainMaterial);
    terrain->scale(glm::vec3(50, 50, 50));
    terrainMaterial->setShiny(10);
    _world->add(terrain);
    terrain->rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));
    terrain->setBehaviour(new RotatingBehaviour(10.0f, glm::vec3(0, 1, 0)));

    
    
    Light* light = CreateLight(sphereMeshS, lightMaterial, LightType::Spot);
    light->setLocalPosition(glm::vec3(0, 0.5f, 0.0f));
    light->setLightColour(glm::vec3(0.9f, 0.9f, 0.9f));
    //light->setBehaviour(new UpDownBehaviour());
    light->setBehaviour(new RotatingBehaviour()); 
    

    Light* light2 = CreateLight(sphereMeshS, lightMaterial, LightType::Directional);
    light2->setLocalPosition(glm::vec3(0, 0, 0));
    light2->setLightColour(glm::vec3(1.0f, 1.0f, 1.0f));
    light2->setLightStrength(1.0f);
    
    //light2->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
    light2->rotate(glm::radians(20.0f), glm::vec3(1, 0, 0));
    light2->setBehaviour(new RotatingBehaviour(-12.0f, glm::vec3(1, 0, 0)));

    GameObject* lightDirection = new GameObject("HI!");
    lightDirection->setMesh(sphereMeshS);
    lightDirection->setMaterial(runicStoneMaterial);
    lightDirection->setLocalPosition(glm::vec3(0, 0, -1));

    light2->add(lightDirection);
    
    Light* light3 = CreateLight(sphereMeshS, lightMaterial, LightType::Spot);
    light3->setLocalPosition(glm::vec3(-5, 5, 4.9f));
    light3->setLightColour(glm::vec3(1, 0, 1));
    light3->rotate(glm::radians(-45.0f), glm::vec3(0, 1, 0));
    light3->extraAttrib1 = 0.92f;
    light3->extraAttrib2 = 0.90f;
    light3->extraAttrib3 = 100.0f;

    Light* light4 = CreateLight(sphereMeshS, lightMaterial, LightType::Spot);
    light4->setLocalPosition(glm::vec3(5, 5, 4.9f));
    light4->setLightColour(glm::vec3(0, 0, 1));
    light4->rotate(glm::radians(45.0f), glm::vec3(0, 1, 0));
    light4->extraAttrib1 = 0.92f;
    light4->extraAttrib2 = 0.90f;
    light4->extraAttrib3 = 100.0f;
    
    Light* light5 = CreateLight(sphereMeshS, lightMaterial, LightType::Point);
    light5->setLocalPosition(glm::vec3(0, 2, 3));
    light5->setLightColour(glm::vec3(0, 1, 0));
    light5->setLightStrength(1.0f);
    light5->setBehaviour(new UpDownBehaviour()); 
    
    Light* ambLight = CreateLight(sphereMeshS, lightMaterial, LightType::Ambient);
    ambLight->setLightColour(glm::vec3(1,1,1));
    ambLight->setLightStrength(0.1f);
    /*
    GameObject* jillMain = new GameObject("JillMain", glm::vec3(0, 0, 1.0f));
    _world->add(jillMain);
    std::vector<Mesh*> jillMeshTwoPointO = Mesh::loadMulti(config::AMBER_MODEL_PATH + "JILLBASE.obj");

    int counter = 0;
    for (auto it = jillMeshTwoPointO.begin(); it != jillMeshTwoPointO.end(); ++it) {
        Mesh* mesh = jillMeshTwoPointO[counter];

        GameObject* newJillObj = new GameObject("JILLPART");
        newJillObj->setMesh(mesh);
        AbstractMaterial* curMat = new TextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + mesh->preferredFile));
        //AbstractMaterial* curMat = new ShadowMaterial();
        curMat->setShiny(1);
        newJillObj->setMaterial(curMat);
        newJillObj->scale(glm::vec3(3, 3, 3));
        jillMain->add(newJillObj);

        counter++;
    }
    cameraHolderMover->setBehaviour(new FollowBehaviour(jillMain));
    jillMain->setBehaviour(new KeysBehaviour());*/

    
     //MONKE!
  GameObject* monke_GameObject = new GameObject("Monke", glm::vec3(0, 2, 0));
  monke_GameObject->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
  monke_GameObject->setMesh(monke_Mesh);
  monke_GameObject->setMaterial(runicStoneMaterial);
  monke_GameObject->setBehaviour(new KeysBehaviour(25, 90.0f));

  _world->add(monke_GameObject);

  const int monkeyCount = 4;
  GameObject* monkeHat_GameObjects[monkeyCount];

  for (int i = 0; i < monkeyCount; i++)
      monkeHat_GameObjects[i] = new GameObject("Monke Hat " + std::to_string(i + 1), glm::vec3(0, 1, 0));

  for (int i = 0; i < monkeyCount; i++) {
      monkeHat_GameObjects[i]->scale(glm::vec3(1, 0.5f, 1));
      monkeHat_GameObjects[i]->setMesh(monkeHat_Mesh);
      monkeHat_GameObjects[i]->setMaterial(hatMaterial);
      monkeHat_GameObjects[i]->setLocalPosition(glm::vec3(0, 1 + (i * .5f), 0));
      monke_GameObject->add(monkeHat_GameObjects[i]);
  }

  cameraHolderMover->setBehaviour(new FollowBehaviour(monke_GameObject));

  GameObject* banana = new GameObject("Banana", glm::vec3(0, 0.8f, 0));
  banana->setMesh(bananaMesh);
  banana->setMaterial(bananaMaterial);
  banana->scale(glm::vec3(20, 20, 20));
  banana->rotate(glm::radians(90.0f), glm::vec3(0, 0, 1));
  banana->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
  monke_GameObject->add(banana);

  
    
    GameObject* backWall = new GameObject("backWall", glm::vec3(0, 5, -5));
    backWall->scale(glm::vec3(5, 5, 5));
    backWall->setMesh(planeMeshDefault);
    backWall->setMaterial(blackMaterial);
    backWall->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
    backWall->rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));

    _world->add(backWall);
    
}

//build the game _world
void MGEAssignment::_initializeSceneOLD()
{
    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = InitializeMesh(AuthorType::MGE, "plane", ModelType::OBJ);
    Mesh* cubeMeshF = InitializeMesh(AuthorType::MGE, "cube_flat", ModelType::OBJ);
    Mesh* sphereMeshS = InitializeMesh(AuthorType::MGE, "sphere_smooth", ModelType::OBJ);

    Mesh* monke_Mesh = InitializeMesh(AuthorType::MGE, "suzanna_smooth", ModelType::OBJ);
    Mesh* monkeHat_Mesh = InitializeMesh(AuthorType::MGE, "cone_smooth", ModelType::OBJ);

    Mesh* bananaMesh = InitializeMesh(AuthorType::Amber, "Banana_02_MESH_TRIS", ModelType::OBJ);
    //Mesh* jillMesh = InitializeMesh(AuthorType::Amber, "JILLBASE", ModelType::OBJ);

    //MATERIALS

    //create some materials to display the cube, the plane and the light
    AbstractMaterial* blackMaterial = new TextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + "black.png"));
    AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(1, 1, 0));
    AbstractMaterial* hatMaterial = new ColorMaterial(glm::vec3(1, 0, 0));
    AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));

    AbstractMaterial* bricksMaterial = new TextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + "SHIRT_BC.png"));
    AbstractMaterial* bananaMaterial = new TextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + "Banana_02_DIFF.png"));

    //SCENE SETUP
  //add camera first (it will be updated last)
    Camera* camera = new Camera("camera", glm::vec3(0, 0, 7));
    //camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));

    camera->setBehaviour(new ZoomBehaviour());

    GameObject* cameraHolder = new GameObject("Camera Holder", glm::vec3(0, 4.5f, 0));
    GameObject* cameraHolderMover = new GameObject("Camera Holder Mover");
    cameraHolder->setBehaviour(new MouseRotateBehaviour());

    _world->add(camera);
    _world->setMainCamera(camera);
    _world->add(cameraHolderMover);
    cameraHolderMover->add(cameraHolder);
    cameraHolder->add(camera);

    //add the floor
    GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
    plane->scale(glm::vec3(5, 5, 5));
    plane->setMesh(planeMeshDefault);
    plane->setMaterial(runicStoneMaterial);
    _world->add(plane);
    


    GameObject* jillMain = new GameObject("JillMain", glm::vec3(0, 0, 1.0f));
    _world->add(jillMain);
    std::vector<Mesh*> jillMeshTwoPointO = Mesh::loadMulti(config::AMBER_MODEL_PATH + "JILLBASE.obj");

    int counter = 0;
    for (auto it = jillMeshTwoPointO.begin(); it != jillMeshTwoPointO.end(); ++it) {
        Mesh* mesh = jillMeshTwoPointO[counter];
       
            GameObject* newJillObj = new GameObject("JILLPART");
            newJillObj->setMesh(mesh);
            AbstractMaterial* curMat = new TextureMaterial(Texture::load(config::AMBER_TEXTURE_PATH + mesh->preferredFile));
            //AbstractMaterial* curMat = new ShadowMaterial();
            curMat->setShiny(100);
            newJillObj->setMaterial(curMat);
            newJillObj->scale(glm::vec3(3, 3, 3));
            jillMain->add(newJillObj);
        
        counter++;
    }
    cameraHolderMover->setBehaviour(new FollowBehaviour(jillMain));
    jillMain->setBehaviour(new KeysBehaviour());

  

   /* //MONKE!
    GameObject* monke_GameObject = new GameObject("Monke", glm::vec3(0, 2, 0));
    monke_GameObject->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
    monke_GameObject->setMesh(monke_Mesh);
    monke_GameObject->setMaterial(bricksMaterial);
    monke_GameObject->setBehaviour(new KeysBehaviour(25, 90.0f));

    _world->add(monke_GameObject);

    const int monkeyCount = 4;
    GameObject* monkeHat_GameObjects[monkeyCount];
    
    for (int i = 0; i < monkeyCount; i++) 
        monkeHat_GameObjects[i] = new GameObject("Monke Hat " + std::to_string(i + 1), glm::vec3(0, 1, 0));
    
    for (int i = 0; i < monkeyCount; i++) {
        monkeHat_GameObjects[i]->scale(glm::vec3(1, 0.5f, 1));
        monkeHat_GameObjects[i]->setMesh(monkeHat_Mesh);
        monkeHat_GameObjects[i]->setMaterial(hatMaterial);
        monkeHat_GameObjects[i]->setLocalPosition(glm::vec3(0, 1 + (i * .5f), 0));
        monke_GameObject->add(monkeHat_GameObjects[i]);
    }

    cameraHolderMover->setBehaviour(new FollowBehaviour(monke_GameObject));

    GameObject* banana = new GameObject("Banana", glm::vec3(0, 0.8f, 0));
    banana->setMesh(bananaMesh);
    banana->setMaterial(bananaMaterial);
    banana->scale(glm::vec3(20, 20, 20));
    banana->rotate(glm::radians(90.0f), glm::vec3(0, 0, 1));
    banana->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
    monke_GameObject->add(banana);*/
    

    Light* light = CreateLight(sphereMeshS, lightMaterial, LightType::Spot);
    light->setLocalPosition(glm::vec3(0, 4, 4.0f));
    light->setLightColour(glm::vec3(0.9f, 0.9f, 0.9f));
    //light->setBehaviour(new UpDownBehaviour());
    light->setBehaviour(new RotatingBehaviour());

    Light* light2 = CreateLight(sphereMeshS, lightMaterial, LightType::Directional);
   light2->setLocalPosition(glm::vec3(0, 10, 10));
    light2->setLightStrength(0.5f);
    light2->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));

    Light* light3 = CreateLight(sphereMeshS, lightMaterial, LightType::Spot);
    light3->setLocalPosition(glm::vec3(-5, 5, 4.9f));
    light3->setLightColour(glm::vec3(1, 0, 1));
    light3->rotate(glm::radians(-45.0f), glm::vec3(0, 1, 0));
    light3->extraAttrib1 = 0.92f;
    light3->extraAttrib2 = 0.90f;
    light3->extraAttrib3 = 100.0f;

    Light* light4 = CreateLight(sphereMeshS, lightMaterial, LightType::Spot);
    light4->setLocalPosition(glm::vec3(5, 5, 4.9f));
    light4->setLightColour(glm::vec3(0, 0, 1));
    light4->rotate(glm::radians(45.0f), glm::vec3(0, 1, 0));
    light4->extraAttrib1 = 0.92f;
    light4->extraAttrib2 = 0.90f;
    light4->extraAttrib3 = 100.0f;

    Light* light5 = CreateLight(sphereMeshS, lightMaterial, LightType::Point);
    light5->setLocalPosition(glm::vec3(0, 1, 3));
    light5->setLightColour(glm::vec3(0, 1, 0));
    light5->setLightStrength(0.4f);
    light5->setBehaviour(new UpDownBehaviour());

    GameObject* lightDirection = new GameObject("HI!");
    lightDirection->setMesh(sphereMeshS);
    lightDirection->setMaterial(runicStoneMaterial);
    lightDirection->setLocalPosition(glm::vec3(0, 0, -1));

    light->add(lightDirection); 

    GameObject* shadowCatcher = new GameObject("Cube", glm::vec3(0.7f, 1.0f, -1.5f));
    shadowCatcher->scale(glm::vec3(0.8f, 0.8f, 0.8f));
    shadowCatcher->setMesh(cubeMeshF);
    shadowCatcher->setMaterial(bricksMaterial);;
    _world->add(shadowCatcher);

    GameObject* backWall = new GameObject("backWall", glm::vec3(0, 5, -5));
    backWall->scale(glm::vec3(5, 5, 5));
    backWall->setMesh(planeMeshDefault);
    backWall->setMaterial(blackMaterial);
    backWall->rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
    backWall->rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));

    _world->add(backWall);
}

void MGEAssignment::_render(bool renderAsShadows) {
    //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    AbstractGame::_render(renderAsShadows);
    if(!renderAsShadows)
    _updateHud();
}

void MGEAssignment::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}


Light* MGEAssignment::CreateLight(Mesh* lightMesh, AbstractMaterial* lightMaterial, int lightType)
{
    Light* light = new Light("Light: " + std::to_string(_world->getLightCount()), glm::vec3(0,0,0));
    light->scale(glm::vec3(0.045f, 0.045f, 0.045f));
    light->setMesh(lightMesh);
    light->setMaterial(lightMaterial);
    light->setLightStrength(1);
    light->setLightColour(glm::vec3(1, 1, 1));
    light->setLightType(lightType);
    _world->add(light);
    if (lightType == LightType::Spot)
    {
        light->extraAttrib1 = 0.92f;
        light->extraAttrib2 = 0.88f;
        light->extraAttrib3 = 15.0f;
    }
    return light;
}

Mesh* MGEAssignment::InitializeMesh(int author,std::string fileName, int modelType)
{
    return Mesh::load(TypeWisher::GetAuthorString(author, TypeWisher::Model) + fileName + ModelType::TypeName(modelType));
}

MGEAssignment::~MGEAssignment()
{
    //dtor
}
