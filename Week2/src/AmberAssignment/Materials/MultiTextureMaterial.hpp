#ifndef MULTITEXTUREMATERIAL_HPP
#define MULTITEXTUREMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"


class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
const int DIFFUSECOUNT = 4;

class MultiTextureMaterial : public AbstractMaterial
{
public:
    MultiTextureMaterial(Texture* heightmap, Texture* splatmap, Texture* diffuseMaps[DIFFUSECOUNT]);
    virtual ~MultiTextureMaterial();

    virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

protected:
private:
    static ShaderProgram* _shader;
    static void _lazyInitializeShader();

    static const int LIGHTCOUNT = 32;

    //in this example we cache all identifiers for uniforms & attributes
    static GLint _uMVPMatrix;
    static GLint _uHeightmap;
    static GLint _uSplatmap;

    static GLint _aVertex;
    static GLint _aNormal;
    static GLint _aUV;

    Texture* _heightmap;
    Texture* _splatmap;
    Texture* _diffuses[DIFFUSECOUNT];

    static GLint _uDiffuses[DIFFUSECOUNT];


    static GLint _camPos;
    static GLint _shinyness;

    static GLint _directionalLightColour[LIGHTCOUNT];
    static GLint _lampStrength[LIGHTCOUNT];
    static GLint _lightPos[LIGHTCOUNT];
    static GLint _lampType[LIGHTCOUNT];
    static GLint _lightDir[LIGHTCOUNT];
    static GLint _ExtraAttributes[LIGHTCOUNT];

    MultiTextureMaterial(const MultiTextureMaterial&);
    MultiTextureMaterial& operator=(const MultiTextureMaterial&);

};

#endif // TEXTUREMATERIAL_HPP
