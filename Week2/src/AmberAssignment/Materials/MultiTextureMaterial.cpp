#include "glm.hpp"

#include "MultiTextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"
#include "AmberAssignment/config.hpp"

ShaderProgram* MultiTextureMaterial::_shader = NULL;

GLint MultiTextureMaterial::_uMVPMatrix = 0;
GLint MultiTextureMaterial::_uHeightmap = 0;
GLint MultiTextureMaterial::_uSplatmap = 0;

GLint MultiTextureMaterial::_aVertex = 0;
GLint MultiTextureMaterial::_aNormal = 0;
GLint MultiTextureMaterial::_aUV = 0;

GLint MultiTextureMaterial::_uDiffuses[];

GLint MultiTextureMaterial::_directionalLightColour[];
GLint MultiTextureMaterial::_lampStrength[];
GLint MultiTextureMaterial::_lightPos[];
GLint MultiTextureMaterial::_lampType[];
GLint MultiTextureMaterial::_lightDir[];
GLint MultiTextureMaterial::_ExtraAttributes[];

GLint MultiTextureMaterial::_camPos = 0;
GLint MultiTextureMaterial::_shinyness = 0;

MultiTextureMaterial::MultiTextureMaterial(Texture* heightmap, Texture* splatmap, Texture* diffuseMaps[DIFFUSECOUNT]) :_heightmap(heightmap), _splatmap(splatmap) {
    for (int i = 0; i < DIFFUSECOUNT; i++) {
        _diffuses[i] = diffuseMaps[i];
    }
    _lazyInitializeShader();
}

MultiTextureMaterial::~MultiTextureMaterial() {}

void MultiTextureMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::AMBER_SHADER_PATH + "textureTerrain.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::AMBER_SHADER_PATH + "textureTerrain.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
        _uHeightmap = _shader->getUniformLocation("heightMap");
        _uSplatmap = _shader->getUniformLocation("splatMap");

        for (int i = 0; i < DIFFUSECOUNT; i++) {
            std::string ref = "diffuseMaps[" + std::to_string(i) + "]";
            _uDiffuses[i] = _shader->getUniformLocation(ref);
        }

        for (int i = 0; i < LIGHTCOUNT; i++) {
            std::string def = "lightElements[" + std::to_string(i) + "].";
            _directionalLightColour[i] = _shader->getUniformLocation(def + "lightColour");
            _lampStrength[i] = _shader->getUniformLocation(def + "lightPower");
            _lightPos[i] = _shader->getUniformLocation(def + "lightPosition");
            _lampType[i] = _shader->getUniformLocation(def + "lightType");
            _lightDir[i] = _shader->getUniformLocation(def + "lightDirection");

            _ExtraAttributes[i] = _shader->getUniformLocation(def + "extraAttributes");
        }

        _camPos = _shader->getUniformLocation("camPos");
        _shinyness = _shader->getUniformLocation("shinyness");

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV = _shader->getAttribLocation("uv");
    }
}

void MultiTextureMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_heightmap) return;

    _shader->use();

    glUniform1i(_shinyness, AbstractMaterial::getShiny());
   
    if (pWorld->getLightCount() > 0) {
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
        }
    }

    //setup texture slot 0
    int curTexture = GL_TEXTURE0;
    glActiveTexture(curTexture++);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _heightmap->getId());
    glActiveTexture(curTexture++);
    glBindTexture(GL_TEXTURE_2D, _splatmap->getId());
    glUniform1i(_uHeightmap, 0);
    glUniform1i(_uSplatmap, 1);
    for (int i = 0; i < DIFFUSECOUNT; i++) {
        glActiveTexture(curTexture++);
        glBindTexture(GL_TEXTURE_2D, _diffuses[i]->getId());
        glUniform1i(_uDiffuses[i], 2 + i);
    }
    //tell the shader the texture slot for the diffuse texture is slot 0



    //pass in a precalculate mvp matrix (see texture material for the opposite)
    glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
    glm::mat4 model = pModelMatrix;
    glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    glUniformMatrix4fv(_shader->getUniformLocation("modelMat"), 1, GL_FALSE, glm::value_ptr(model));
    glm::vec3 cPos = pWorld->getMainCamera()->getWorldPosition();
    glUniform3f(_camPos, cPos.x, cPos.y, cPos.z);
    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
