#ifndef SHADOWMATERIAL_HPP
#define SHADOWMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"
#include "AmberAssignment/config.hpp"
#include "mge/core/Camera.hpp"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class ShadowMaterial : public AbstractMaterial
{
public:
    ShadowMaterial();
    virtual ~ShadowMaterial();

    virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

protected:
private:
    static ShaderProgram* _shader;
    static void _lazyInitializeShader();

    //in this example we cache all identifiers for uniforms & attributes
    static GLint _uMVPMatrix;
    static GLint _aVertex;
    static GLint _aNormal;
    static GLint _aUV;

    static const int LIGHTCOUNT = 32;

    ShadowMaterial(const ShadowMaterial&);
    ShadowMaterial& operator=(const ShadowMaterial&);

};

#endif // TEXTUREMATERIAL_HPP
