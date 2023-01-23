#include <string>

namespace ModelType {
	const int OBJ = 0; 
	const int FBX = 1; 
	const std::string TypeName(int modelType) {
		if(modelType == 0) return ".obj";
		if(modelType == 1) return ".fbx";
		return "";
	} 
}