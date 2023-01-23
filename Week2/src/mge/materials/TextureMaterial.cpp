#include "glm.hpp"

#include "TextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"
#include "AmberAssignment/config.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/AbstractGame.hpp"

ShaderProgram* TextureMaterial::_shader = NULL;

GLint TextureMaterial::_uMVPMatrix = 0;
GLint TextureMaterial::_uDiffuseTexture = 0;

GLint TextureMaterial::_aVertex = 0;
GLint TextureMaterial::_aNormal = 0;
GLint TextureMaterial::_aUV = 0;

GLuint TextureMaterial::shadowMap = 0;

GLint TextureMaterial::_directionalLightColour[];
GLint TextureMaterial::_lampStrength[];
GLint TextureMaterial::_lightPos[];
GLint TextureMaterial::_lampType[];
GLint TextureMaterial::_lightDir[];
GLint TextureMaterial::_ExtraAttributes[];
GLint TextureMaterial::lightProj = 0;

GLint TextureMaterial::_camPos = 0;
GLint TextureMaterial::_shinyness = 0;


TextureMaterial::TextureMaterial(Texture * pDiffuseTexture):_diffuseTexture(pDiffuseTexture) {
    _lazyInitializeShader();
}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::AMBER_SHADER_PATH+"texture.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::AMBER_SHADER_PATH +"texture.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
        _uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

        shadowMap = _shader->getUniformLocation("shadowMap");

        lightProj = _shader->getUniformLocation("lightMat");

        for (int i = 0; i < LIGHTCOUNT; i++) {
            std::string def = "lightElements[" + std::to_string(i) + "].";
            _directionalLightColour[i] = _shader->getUniformLocation(def + "lightColour");
            _lampStrength[i] = _shader->getUniformLocation(def + "lightPower");
            _lightPos[i] = _shader->getUniformLocation(def + "lightPosition");
            _lampType[i] = _shader->getUniformLocation(def+"lightType");
            _lightDir[i] = _shader->getUniformLocation(def + "lightDirection");
            //_lightProj[i] = _shader->getUniformLocation(def + "lightProj");

            _ExtraAttributes[i] = _shader->getUniformLocation(def + "extraAttributes");
        }


        _camPos = _shader->getUniformLocation("camPos");
        _shinyness = _shader->getUniformLocation("shinyness");

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV =     _shader->getAttribLocation("uv");
    }
}

void TextureMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void TextureMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture) return;

    _shader->use();

    //glm::mat4 depthProjectionMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f);
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, 0.1f, 1000.0f);

    glUniform1i(_shinyness, AbstractMaterial::getShiny());
    //Print the number of lights in the scene and the position of the first light.
    //It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
    if (pWorld->getLightCount() > 0) {
        int pointCounter = 0;
        for (int i = 0; i < LIGHTCOUNT; i++) {
            if (i >= pWorld->getLightCount()) {
                glUniform1i(_lampType[i], -1);
                continue;
            }
            Light* cLight = pWorld->getLightAt(i);
            glm::vec3 lc = cLight->getLightColour();
            glm::vec3 lp = cLight->getWorldPosition();
            glm::vec4 back = cLight->getWorldTransform()[2];
            glm::vec3 forw = -glm::vec3(back.x, back.y, back.z);
            glUniform3f(_directionalLightColour[i], lc.x, lc.y, lc.z);
            glUniform1f(_lampStrength[i], cLight->getLightStrength());
            glUniform3f(_lightPos[i], lp.x, lp.y, lp.z);
            glUniform1i(_lampType[i], cLight->getLightType());
            glUniform3f(_lightDir[i], forw.x, forw.y, forw.z);
            glUniform3f(_ExtraAttributes[i], cLight->extraAttrib1, cLight->extraAttrib2, cLight->extraAttrib3);

            glm::mat4 depthViewMatrix = glm::lookAt(pWorld->getLightAt(i)->getWorldPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * pModelMatrix;

            if (cLight->getLightType() == LightType::Point && pointCounter == 0) {
                glUniformMatrix4fv(lightProj, 1, GL_FALSE, glm::value_ptr(depthMVP));
                pointCounter++;
            }
        }
        //std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getWorldPosition() << std::endl;
    }

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    //tell the shader the texture slot for the diffuse texture is slot 0
    glUniform1i (_uDiffuseTexture, 0);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, AbstractGame::depthMap);
    glUniform1i(shadowMap, 1);


    //pass in a precalculate mvp matrix (see texture material for the opposite)
    glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
    glm::mat4 model = pModelMatrix;
    glUniformMatrix4fv ( _uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    glUniformMatrix4fv (_shader->getUniformLocation("modelMat"), 1, GL_FALSE, glm::value_ptr(model));
    glm::vec3 cPos = pWorld->getMainCamera()->getWorldPosition();
    glUniform3f(_camPos, cPos.x, cPos.y, cPos.z);
    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
