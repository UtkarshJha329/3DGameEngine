#include "../HelpHeaders/Components/MeshData.h"

VAA_Data MeshData::stdVAAData[] = {
			{	0, 3, GL_FLOAT, GL_FALSE, MeshData::stride, (void*)0						},	// VERTEX COORDINATES
			{	1, 3, GL_FLOAT, GL_FALSE, MeshData::stride, (void*)(3 * sizeof(float))		},  // VERTEX NORMALS
			{	2, 2, GL_FLOAT, GL_FALSE, MeshData::stride, (void*)(6 * sizeof(float))		},	// TEXTURE COORDINATES
			{	3, 3, GL_FLOAT, GL_FALSE, MeshData::stride, (void*)(8 * sizeof(float))		},   // VERTEX TANGENTS
			{	4, 3, GL_FLOAT, GL_FALSE, MeshData::stride, (void*)(11 * sizeof(float))		}   // VERTEX BI-TANGENTS
};


unsigned int MeshData::stdNumVAAs = 5;
int MeshData::stride = 14 * sizeof(float);

//std::vector<Texture*> MeshData::loadedTextures;

unsigned int MeshData::mainCameraEntity = 1;

void MeshData::SerializeComponent()
{
	SFstrs(meshPath);
}

void MeshData::DeSerializeComponent()
{
	DSFstrs(&meshPath);
	//DSFVecIntsT(1, vertices);
}

void MeshData::UIfyComponent()
{
	UIfyString(meshPath);
	UIfyString(meshName);
}
