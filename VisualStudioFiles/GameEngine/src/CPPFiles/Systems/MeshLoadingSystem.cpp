#include "../../HelpHeaders/Systems/MeshLoadingSystem.h"

MeshLoadingSystem::MeshLoadingSystem() {

}

void MeshLoadingSystem::UnorderedInit() {

	archetype.priorityType = HAS_ALL_COMP;
	archetype.compSparseIdsNdPriority.push_back(MeshData::compSetIndex);
	archetype.compSparseIdsNdPriority.push_back(Shader::compSetIndex);

	systemName = typeid(MeshLoadingSystem).name();
}

void MeshLoadingSystem::PreAwake() {

}

void MeshLoadingSystem::Awake()
{


	std::string curFolder = RandomProps::curFolderPath;

	CompSparseSet<MeshData>* curMDSet = curScene->GetCompSparseSet<MeshData>();
	CompSparseSet<Shader>* curShadSet = curScene->GetCompSparseSet<Shader>();
	CompSparseSet<ParentChild>* curParChiSet = curScene->GetCompSparseSet<ParentChild>();


	for (int i = 0; i < curSmallestSetSize; i++)
	{
		MeshData* curMesh = &curMDSet->denseTArray[i];
		//unsigned int curEntity = Tabler::Comp_Set<MeshData>.denseIndexArray[i];
		Shader* curShader = &curShadSet->denseTArray[i];
		//InitShader(curShader, (curFolder + "ObjectShaders/TexturedLit/TexturedLit.vert").c_str(), (curFolder + "ObjectShaders/TexturedLit/TexturedLit.frag").c_str());
		InitShader(curShader, "Assets/ObjectShaders/TexturedLit/TexturedLit.vert", "Assets/ObjectShaders/TexturedLit/TexturedLit.frag");
		curShader->shadNum = ShaderNumber::TexturedLit;

		LoadModel(curMDSet->denseTArray[i].pairedEntity, curShader);
	}

	RandomProps::particleEntity = curScene->sceneTabler.entity_Set.NewEntity(false);

	curMDSet->Add(RandomProps::particleEntity, false);
	curShadSet->Add(RandomProps::particleEntity, false);
	curParChiSet->Add(RandomProps::particleEntity, false);

	Shader* curShader = curScene->GetCompOfEntity<Shader>(RandomProps::particleEntity);
	MeshData* curMD = curScene->GetCompOfEntity<MeshData>(RandomProps::particleEntity);
	ParentChild* curParChi = curScene->GetCompOfEntity<ParentChild>(RandomProps::particleEntity);
	ParentChild* originParChi = curScene->GetCompOfEntity<ParentChild>(0);
	
	curParChi->currentEntity = RandomProps::particleEntity;
	curParChi->parEntity = RandomProps::particleEntity;
	originParChi->childrenEntity.push_back(RandomProps::particleEntity);


	curMD->meshPath = "Assets/Models/SimplePlane/SimplePlane.gltf";

	InitShader(curShader, "Assets/ObjectShaders/TexturedLit/TexturedLit.vert", "Assets/ObjectShaders/TexturedLit/TexturedLit.frag");
	curShader->shadNum = ShaderNumber::TexturedLit;

	Particle::meshEntity = RandomProps::particleEntity;
	Particle::shaderEntity = RandomProps::particleEntity;

	LoadModel(RandomProps::particleEntity, curShader);

	auto particleGroupSet = curScene->GetCompSparseSet<ParticleGroup>();
	for (int i = 0; i < particleGroupSet->DenseSize(); i++)
	{
		std::cout << particleGroupSet->denseTArray[i].texturePath << std::endl;
		particleGroupSet->denseTArray[i].textureIndex = LoadParticleTextureToCommonMeshData(particleGroupSet->denseTArray[i].texturePath);
	}

}

void MeshLoadingSystem::Start()
{
}

void MeshLoadingSystem::Update()
{
}

void MeshLoadingSystem::LateUpdate()
{
}

void MeshLoadingSystem::Clear()
{
	CompSparseSet<MeshData>* curMD = curScene->GetCompSparseSet<MeshData>();

	for (int i = 0; i < curSmallestSetSize; i++)
	{
		FreeStuff(&curMD->denseTArray[i]);
	}
}

void MeshLoadingSystem::GenerateABOs(MeshData* meshData, GLsizeiptr bufferSize, Shader* curShader)
{
	glGenVertexArrays(1, &(meshData->VAO));
	glGenBuffers(1, &(meshData->VBO));
	glGenBuffers(1, &(meshData->EBO));

	glBindVertexArray(meshData->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, meshData->VBO);

	glBufferData(GL_ARRAY_BUFFER, bufferSize, meshData->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indecies.size() * sizeof(unsigned int), meshData->indecies.data(), GL_STATIC_DRAW);

	// vertex attributes
	for (int i = 0; i < meshData->stdNumVAAs; i++)
	{
		glVertexAttribPointer(meshData->stdVAAData[i].index,
			meshData->stdVAAData[i].sizeOfVertex,
			meshData->stdVAAData[i].type,
			meshData->stdVAAData[i].normalized,
			meshData->stdVAAData[i].stride,
			meshData->stdVAAData[i].offset);
		glEnableVertexAttribArray(meshData->stdVAAData[i].index);
	}

	glBindVertexArray(0);
}

void MeshLoadingSystem::LoadModel(unsigned int curMeshEntity, Shader* curShader)
{
	Assimp::Importer importer;
	MeshData* curMesh = curScene->GetCompOfEntity<MeshData>(curMeshEntity);
	const aiScene* scene = importer.ReadFile(curMesh->meshPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {

		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	curMesh->meshDataDir = curMesh->meshPath.substr(0, curMesh->meshPath.find_last_of('/'));
	ProcessNode(scene, curMeshEntity, curMesh->meshDataDir, curShader);
	
	
	//LoadMeshPreservingHierarchy(scene, scene->mRootNode, curMesh, curMesh->meshDataDir, curShader, curMesh->pairedEntity);
}

void MeshLoadingSystem::ProcessNode(const aiScene* scene, unsigned int curMeshEntity, std::string curMeshDir, Shader* curShader)
{
	//auto transSet = curScene->GetCompSparseSet<Transform>();
	//auto shaderSet = curScene->GetCompSparseSet<Shader>();
	auto parChi = curScene->GetCompSparseSet<ParentChild>();
	auto meshSet = curScene->GetCompSparseSet<MeshData>();
	//std::cout << "FIRST : " << curMeshDir << std::endl;

	ParentChild* parent = curScene->GetCompOfEntity<ParentChild>(curMeshEntity);
	parent->childrenEntity.clear();

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		unsigned int meshEntityID = curScene->sceneTabler.entity_Set.NewEntity(false);
		meshSet->Add(meshEntityID, false);
		parChi->Add(meshEntityID, false);

		ParentChild* child = curScene->GetCompOfEntity<ParentChild>(meshEntityID);
		child->parEntity = curShader->pairedEntity;
		child->currentEntity = meshEntityID;
		child->codedHierarchy = 1;
		ParentChild* parent = curScene->GetCompOfEntity<ParentChild>(curMeshEntity);
		parent->childrenEntity.push_back(meshEntityID);
		parent->codedHierarchy = 1;
		//transSet->Add(meshEntityID);
		//shaderSet->Add(meshEntityID);

		aiMesh* mesh = scene->mMeshes[i];

		MeshData* curMesh = curScene->GetCompOfEntity<MeshData>(curMeshEntity);
		curMesh->meshEntities.push_back(meshEntityID);
		//std::cout << "SECOND : " << curMeshDir << std::endl;
		//std::cout << "Pushed new mesh entity : " << meshEntityID << std::endl;

		ProcessMesh(mesh, scene, meshEntityID, curShader, curMeshDir);
	}
}

//
//void MeshLoadingSystem::ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData* meshN, Shader* curShader, std::string meshDataDir) {
//
//	//meshN->vertices.reserve(mesh->mNumVertices * 11);
//	#define vertexArray meshN->vertices
//	//std::vector<float> vertexArray;
//
//	int texCoordIndex = -1;
//	bool found = false;
//	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
//	{
//		if ((*mesh).HasTextureCoords(i)) {
//			texCoordIndex = i;
//			found = true;
//			break;
//		}
//	}
//
//	for (int i = 0; i < (*mesh).mNumVertices; i++)
//	{
//		vertexArray.push_back((*mesh).mVertices[i].x);
//		vertexArray.push_back((*mesh).mVertices[i].y);
//		vertexArray.push_back((*mesh).mVertices[i].z);
//
//		if ((*mesh).HasNormals()) {
//			vertexArray.push_back((*mesh).mNormals[i].x);
//			vertexArray.push_back((*mesh).mNormals[i].y);
//			vertexArray.push_back((*mesh).mNormals[i].z);
//		}
//		else {
//			vertexArray.push_back(0.0f);
//			vertexArray.push_back(0.0f);
//			vertexArray.push_back(0.0f);
//		}
//
//		if (found) {
//			vertexArray.push_back((*mesh).mTextureCoords[texCoordIndex][i].x);
//
//			//DOIN 1 - y THIS WAS SOO0000000000000000000000000000000 SHIT.
//			vertexArray.push_back(1.0f - (*mesh).mTextureCoords[texCoordIndex][i].y);
//		}
//		else {
//			vertexArray.push_back(0.0f);
//			vertexArray.push_back(0.0f);
//		}
//		
//		if ((*mesh).HasTangentsAndBitangents()) {
//			//std::cout << "HAS TANGENT AND BITANGENTS" << std::endl;
//			vertexArray.push_back((*mesh).mTangents[i].x);
//			vertexArray.push_back((*mesh).mTangents[i].y);
//			vertexArray.push_back((*mesh).mTangents[i].z);
//		
//			vertexArray.emplace_back((*mesh).mBitangents[i].x);
//			vertexArray.emplace_back((*mesh).mBitangents[i].y);
//			vertexArray.emplace_back((*mesh).mBitangents[i].z);
//		}
//	}
//
//	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//		meshN->indecies.reserve(meshN->indecies.size() + face.mNumIndices);
//		meshN->indecies.insert(meshN->indecies.end(), face.mIndices, (face.mIndices + face.mNumIndices));
//	}
//
//	if (mesh->mMaterialIndex >= 0)
//	{
//		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//
//		std::vector<unsigned int> diffTex = LoadMaterialTextures(material, aiTextureType_DIFFUSE, diffuse, meshDataDir);
//
//		std::vector<unsigned int> specTex = LoadMaterialTextures(material, aiTextureType_SPECULAR, specular, meshDataDir);
//		
//		std::vector<unsigned int> normTex = LoadMaterialTextures(material, aiTextureType_HEIGHT, normal, meshDataDir);
//
//		meshN->textureIndex.reserve(diffTex.size() + specTex.size() + normTex.size());
//		meshN->textureIndex.insert(meshN->textureIndex.end(), diffTex.begin(), diffTex.end());
//		meshN->textureIndex.insert(meshN->textureIndex.end(), specTex.begin(), specTex.end());
//		meshN->textureIndex.insert(meshN->textureIndex.end(), normTex.begin(), normTex.end());
//
//	}
//
//	//meshN->vertices = vertexArray.data();
//	meshN->numVertices = vertexArray.size();
//
//	MeshLoadingSystem::GenerateABOs(meshN, MeshData::stride * (*mesh).mNumVertices, curShader);
//}

void MeshLoadingSystem::ProcessMesh(aiMesh* mesh, const aiScene* scene, unsigned int meshEntity, Shader* curShader, std::string meshDataDir) {

	MeshData* meshN = curScene->GetCompOfEntity<MeshData>(meshEntity);
	//meshN->vertices.reserve(mesh->mNumVertices * 11);
	#define vertexArray meshN->vertices
	//std::vector<float> vertexArray;

	//std::cout << "Max : " << mesh->mAABB.mMax.x << " : " << mesh->mAABB.mMax.y << " : " << mesh->mAABB.mMax.z << std::endl;
	//std::cout << "Min : " << mesh->mAABB.mMin.x << " : " << mesh->mAABB.mMin.y << " : " << mesh->mAABB.mMin.z << std::endl;
	
	meshN->aabb.max.x = mesh->mAABB.mMax.x;
	meshN->aabb.max.y = mesh->mAABB.mMax.y;
	meshN->aabb.max.z = mesh->mAABB.mMax.z;
	meshN->aabb.min.x = mesh->mAABB.mMin.x;
	meshN->aabb.min.y = mesh->mAABB.mMin.y;
	meshN->aabb.min.z = mesh->mAABB.mMin.z;

	int texCoordIndex = -1;
	bool found = false;
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
	{
		if ((*mesh).HasTextureCoords(i)) {
			texCoordIndex = i;
			found = true;
			break;
		}
	}

	for (int i = 0; i < (*mesh).mNumVertices; i++)
	{
		vertexArray.push_back((*mesh).mVertices[i].x);
		vertexArray.push_back((*mesh).mVertices[i].y);
		vertexArray.push_back((*mesh).mVertices[i].z);

		if ((*mesh).HasNormals()) {
			vertexArray.push_back((*mesh).mNormals[i].x);
			vertexArray.push_back((*mesh).mNormals[i].y);
			vertexArray.push_back((*mesh).mNormals[i].z);
		}
		else {
			vertexArray.push_back(0.0f);
			vertexArray.push_back(0.0f);
			vertexArray.push_back(0.0f);
		}

		if (found) {
			vertexArray.push_back((*mesh).mTextureCoords[texCoordIndex][i].x);

			//DOIN 1 - y THIS WAS SOO0000000000000000000000000000000 SHIT.
			vertexArray.push_back(1.0f - (*mesh).mTextureCoords[texCoordIndex][i].y);
		}
		else {
			vertexArray.push_back(0.0f);
			vertexArray.push_back(0.0f);
		}

		if ((*mesh).HasTangentsAndBitangents()) {
			//std::cout << "HAS TANGENT AND BITANGENTS" << std::endl;
			vertexArray.push_back((*mesh).mTangents[i].x);
			vertexArray.push_back((*mesh).mTangents[i].y);
			vertexArray.push_back((*mesh).mTangents[i].z);

			vertexArray.emplace_back((*mesh).mBitangents[i].x);
			vertexArray.emplace_back((*mesh).mBitangents[i].y);
			vertexArray.emplace_back((*mesh).mBitangents[i].z);
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		meshN->indecies.reserve(meshN->indecies.size() + face.mNumIndices);
		meshN->indecies.insert(meshN->indecies.end(), face.mIndices, (face.mIndices + face.mNumIndices));
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<unsigned int> diffTex = LoadMaterialTextures(material, aiTextureType_DIFFUSE, diffuse, meshDataDir);

		std::vector<unsigned int> specTex = LoadMaterialTextures(material, aiTextureType_SPECULAR, specular, meshDataDir);

		std::vector<unsigned int> normTex = LoadMaterialTextures(material, aiTextureType_HEIGHT, normal, meshDataDir);

		meshN->textureIndex.reserve(diffTex.size() + specTex.size() + normTex.size());
		meshN->textureIndex.insert(meshN->textureIndex.end(), diffTex.begin(), diffTex.end());
		meshN->textureIndex.insert(meshN->textureIndex.end(), specTex.begin(), specTex.end());
		meshN->textureIndex.insert(meshN->textureIndex.end(), normTex.begin(), normTex.end());

	}

	//meshN->vertices = vertexArray.data();
	meshN->numVertices = vertexArray.size();

	MeshLoadingSystem::GenerateABOs(meshN, MeshData::stride * (*mesh).mNumVertices, curShader);

	//std::cout << mesh->mName.C_Str() << std::endl;
	meshN->meshName = mesh->mName.C_Str();
}


std::vector<unsigned int> MeshLoadingSystem::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName, std::string curMeshDataDir)
{
	std::vector<unsigned int> texIndex;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string path = curMeshDataDir;
		path.append("/").append(str.C_Str());
		bool skip = false;
		for (unsigned int j = 0; j < CommonMeshData::Instance()->loadedTextures.size(); j++)
		{
			if (std::strcmp(CommonMeshData::Instance()->loadedTextures[j]->texturePath.c_str(), path.c_str()) == 0)
			{
				//std::cout << "SAME OLD TEXTURE: " << str.C_Str() << std::endl;
				texIndex.push_back(j);
				//textures.push_back(MeshData::loadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			//if (typeName == TextureType::diffuse) {
			//	std::cout << "NEW TEXTURE: " << str.C_Str() << "\t";
			//}
			//std::cout << str.C_Str() << std::endl;
			Texture* tex = Texture::TextureFromFile(path, GL_TEXTURE_2D, TEXTURE_FLIP_VERTICAL);
			tex->type = typeName;

			//GET THE LOADED TEXTURE SIZE AND THEN PUSH SO NO NEED FOR - 1
			texIndex.push_back(CommonMeshData::Instance()->loadedTextures.size());
			CommonMeshData::Instance()->loadedTextures.push_back(tex);
		}
	}
	return texIndex;
}

int MeshLoadingSystem::LoadParticleTextureToCommonMeshData(std::string path)
{
	int texIndex = -1;
	bool createNewTexture = true;
	for (unsigned int j = 0; j < CommonMeshData::Instance()->loadedTextures.size(); j++)
	{
		if (std::strcmp(CommonMeshData::Instance()->loadedTextures[j]->texturePath.c_str(), path.c_str()) == 0)
		{
			texIndex = j;
			createNewTexture = false;
			break;
		}
	}
	if (createNewTexture)
	{
		Texture* tex = Texture::TextureFromFile(path, GL_TEXTURE_2D, TEXTURE_FLIP_VERTICAL);
		tex->type = TextureType::diffuse;

		//GET THE LOADED TEXTURE SIZE AND THEN PUSH SO NO NEED FOR - 1
		texIndex = CommonMeshData::Instance()->loadedTextures.size();
		CommonMeshData::Instance()->loadedTextures.push_back(tex);
	}
	return texIndex;
}

void MeshLoadingSystem::FreeStuff(MeshData* curMeshData)
{
	std::vector<unsigned int>().swap(curMeshData->textureIndex);
	std::vector<unsigned int>().swap(curMeshData->indecies);

	std::string().swap(curMeshData->meshDataDir);
	std::string().swap(curMeshData->meshPath);


	if (!curMeshData->meshEntities.empty()) {

		for (unsigned int i = 0; i < curMeshData->meshEntities.size(); i++)
		{
			MeshData* curMesh = curScene->GetCompOfEntity<MeshData>(curMeshData->meshEntities[i]);

			glDeleteBuffers(1, &(curMesh->EBO));
			glDeleteBuffers(1, &(curMesh->VBO));
		}
		std::vector<unsigned int>().swap(curMeshData->meshEntities);
	}
}

