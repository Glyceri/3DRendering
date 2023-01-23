#pragma once
#include <string>
#include "AmberAssignment/config.hpp"
#include "mge/config.hpp"

namespace TypeWisher {
	const int Model = 0;
	const int Texture = 1;
	const int Shader = 2;
	const int Font = 3;
	 
	const std::string GetAuthorString(int author, int type) {
		if (author == 0)
		{
			if (type == Model) return config::AMBER_MODEL_PATH;
			if (type == Texture) return config::AMBER_TEXTURE_PATH;
			if (type == Shader) return config::AMBER_SHADER_PATH;
			if (type == Font) return config::AMBER_SHADER_PATH;
		}
		if (author == 1) 
		{
			if (type == Model) return config::MGE_MODEL_PATH;
			if (type == Texture) return config::MGE_TEXTURE_PATH;
			if (type == Shader) return config::MGE_SHADER_PATH;
			if (type == Font) return config::MGE_SHADER_PATH;
		}
		return "";
	}
}