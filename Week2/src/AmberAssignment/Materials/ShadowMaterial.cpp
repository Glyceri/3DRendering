#include "ShadowMaterial.hpp"

ShaderProgram* ShadowMaterial::_shader = NULL;
GLint ShadowMaterial::_uMVPMatrix = 0;
GLint ShadowMaterial::_aVertex = 0;
GLint ShadowMaterial::_aNormal = 0;
GLint ShadowMaterial::_aUV = 0;

ShadowMaterial::ShadowMaterial()
{
	_lazyInitializeShader();
}

ShadowMaterial::~ShadowMaterial()
{
}



void ShadowMaterial::_lazyInitializeShader()
{
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::AMBER_SHADER_PATH + "depth.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::AMBER_SHADER_PATH + "depth.fs");
		_shader->finalize();


		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");

		_uMVPMatrix = _shader->getUniformLocation("depthMVP");
	}
}


void ShadowMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix)
{
	_shader->use();

	//glm::mat4 depthProjectionMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f);
	glm::mat4 depthProjectionMatrix  = glm::ortho<float>(-10, 10, -10, 10, 0.1f, 1000.0f);

    if (pWorld->getLightCount() > 0) {
		int pointCounter = 0;
        for (int i = 0; i < LIGHTCOUNT; i++) {
            if (i >= pWorld->getLightCount()) {
                continue;
            }
			Light* l = pWorld->getLightAt(i);

			glm::vec3 lightInvDir = l->getWorldTransform()[2];
			glm::vec3 forw = -glm::vec3(lightInvDir.x, lightInvDir.y, lightInvDir.z);
			glm::mat4 depthViewMatrix = glm::lookAt(pWorld->getLightAt(i)->getWorldPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			
			glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * pModelMatrix;
			if (l->getLightType() == LightType::Point && pointCounter == 0) {
				glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(depthMVP));
				pointCounter++;
			}
        }
       
    }

	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}