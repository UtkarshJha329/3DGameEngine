#include "../../HelpHeaders/Systems/MeshRenderingSystem.h"


void MeshRenderingSystem::UnorderedInit()
{
	archetype.priorityType = HAS_ALL_COMP;
	archetype.compSparseIdsNdPriority.push_back(MeshData::compSetIndex);
	archetype.compSparseIdsNdPriority.push_back(Transform::compSetIndex);
	archetype.compSparseIdsNdPriority.push_back(Shader::compSetIndex);

	systemName = typeid(MeshRenderingSystem).name();
}

void MeshRenderingSystem::Awake()
{
}

void MeshRenderingSystem::Start()
{
}

void MeshRenderingSystem::Update()
{
	CompSparseSet<CameraComponent>* curCam = curScene->GetCompSparseSet<CameraComponent>();

	unsigned int denseTIndex = curCam->sparse_Indecies[CameraComponent::mainCameraEntity];
	CameraComponent* mainCam = &curCam->denseTArray[denseTIndex];

	//DrawMeshFrontDepthPass(mainCam);
	//DrawMeshBackDepthPass(mainCam);
	DrawMeshesPassOne(mainCam);
}

void MeshRenderingSystem::LateUpdate()
{
}

void MeshRenderingSystem::Clear()
{
	for (unsigned int i = 0; i < CommonMeshData::Instance()->loadedTextures.size(); i++)
	{
		glDeleteTextures(1, &CommonMeshData::Instance()->loadedTextures[i]->ID);
	}

	std::vector<Texture*>().swap(CommonMeshData::Instance()->loadedTextures);
}

void MeshRenderingSystem::DrawMeshesPassOne(CameraComponent* curCam) {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, curCam->FBO);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	CompSparseSet<MeshData>* curMDS = curScene->GetCompSparseSet<MeshData>();
	CompSparseSet<Shader>* shadSet = curScene->GetCompSparseSet<Shader>();
	CompSparseSet<Transform>* transSet = curScene->GetCompSparseSet<Transform>();
	//std::cout << curSmallestSetSize << ", " << Tabler::Comp_Set<MeshData>.denseTArray.size() << std::endl;
	for (int i = 0; i < curSmallestSetSize; i++)
	{
		UseShader(&shadSet->denseTArray[i]);
		setUFloat(&shadSet->denseTArray[i], "totalEntities", (float)curScene->sceneTabler.entity_Set.sparse_Index.size());
		//glActiveTexture(GL_TEXTURE0);
		//setUInt(&curShad->denseTArray[i], "depthTexFront", 0);
		//glBindTexture(GL_TEXTURE_2D, curCam->depthTextureFrontID);
		//
		//glActiveTexture(GL_TEXTURE1);
		//setUInt(&curShad->denseTArray[i], "depthTexBack", 1);
		//glBindTexture(GL_TEXTURE_2D, curCam->depthTextureBackID);


		DrawMeshTotal(&curMDS->denseTArray[i], &shadSet->denseTArray[i], &transSet->denseTArray[i]);
	}

	auto particleGroupSet = curScene->GetCompSparseSet<ParticleGroup>();
	Transform* curCameraTrans = curScene->GetCompOfEntity<Transform>(CameraComponent::mainCameraEntity);

	for (unsigned i = 0; i < particleGroupSet->DenseSize(); i++)
	{
		for (unsigned int j = 0; j < particleGroupSet->denseTArray[i].particleEntities.size(); j++)
		{
			int curParticleEntity = particleGroupSet->denseTArray[i].particleEntities[j];
			Particle* curParticle = curScene->GetCompOfEntity<Particle>(curParticleEntity);

			if (curParticle->particleAlive) {

				unsigned int curShaderEntity = curParticle->shaderEntity;

				Shader* curShader = curScene->GetCompOfEntity<Shader>(curShaderEntity);
				Transform* curTransform = curScene->GetCompOfEntity<Transform>(curParticleEntity);;

				//std::cout << "DRAWING PARTICLE : " << i << std::endl;

				unsigned int curMeshDataEntity = curParticle->meshEntity;
				MeshData* curMD = curScene->GetCompOfEntity<MeshData>(curMeshDataEntity);

				UseShader(curShader);

				glm::quat lookAtQuat = glm::quatLookAt(glm::normalize(curTransform->position - curCameraTrans->position), curCameraTrans->up);
				glm::mat4 trans = WorldTransform(&curTransform->CalculateTransformMatr(lookAtQuat), curParticleEntity);

				setMat4(curShader, "transform", trans);
				//curTransform->PrintTransformMat();

				if (particleGroupSet->denseTArray[i].textureIndex == -1) {
					DrawMeshTotal(curMD, curShader, curTransform, particleGroupSet->denseTArray[i].pairedEntity);
				}
				else {
					DrawParticleTotal(curMD, curShader, particleGroupSet->denseTArray[i].textureIndex, particleGroupSet->denseTArray[i].pairedEntity);
				}
			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

glm::mat4 MeshRenderingSystem::WorldTransform(glm::mat4* childTrans, unsigned int childEntityID)
{
	auto parentChild = curScene->GetCompSparseSet<ParentChild>();
	auto transSet = curScene->GetCompSparseSet<Transform>();

	//ParentChild* parChi = &parentChild->denseTArray[parentChild->sparse_Indecies[curEntity]];
	ParentChild* parChi = &(parentChild->denseTArray[parentChild->sparse_Indecies[childEntityID]]);
	bool hasParent = parChi->parEntity != childEntityID;

	glm::mat4* parentTrans = &curScene->GetCompOfEntity<Transform>(parChi->parEntity)->CalculateTransformMatr();

	if (hasParent) {
		glm::mat4 trans = ((WorldTransform(parentTrans, parChi->parEntity)) * (*childTrans));
		return trans;
	}
	else {
		return *childTrans;
	}


	//return nullptr;
}

void MeshRenderingSystem::DrawMeshElements(MeshData* meshdata, Shader *parShader)
{
	UseShader(parShader);
	BindTexturesWithIndecies(meshdata, parShader);
	glBindVertexArray((meshdata->VAO));
	//std::cout << meshdata->indecies.size() << std::endl;
	UseShader(parShader);
	//std::cout << parShader->ID << std::endl;
	glDrawElements(GL_TRIANGLES, meshdata->indecies.size(), GL_UNSIGNED_INT, 0);
	//std::cout << "SIZE IN BYTES : " << meshdata->indecies.size() * sizeof(float) << std::endl;
	glBindVertexArray(0);
}

void MeshRenderingSystem::DrawMeshElementsBindingGivenTextureIndex(MeshData* meshdata, Shader* parShader, int textureIndex)
{
	UseShader(parShader);

	glActiveTexture(GL_TEXTURE2);
	setUInt(parShader, std::string("mat.diffuseTex1"), 2);
	glBindTexture(GL_TEXTURE_2D, CommonMeshData::Instance()->loadedTextures[textureIndex]->ID);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray((meshdata->VAO));
	//std::cout << meshdata->indecies.size() << std::endl;
	UseShader(parShader);
	//std::cout << parShader->ID << std::endl;
	glDrawElements(GL_TRIANGLES, meshdata->indecies.size(), GL_UNSIGNED_INT, 0);
	//std::cout << "SIZE IN BYTES : " << meshdata->indecies.size() * sizeof(float) << std::endl;
	glBindVertexArray(0);
}


void MeshRenderingSystem::DrawMeshElementsPlain(MeshData* meshdata, Shader* parShader)
{
	UseShader(parShader);
	glBindVertexArray((meshdata->VAO));
	//std::cout << meshdata->indecies.size() << std::endl;
	UseShader(parShader);
	glDrawElements(GL_TRIANGLES, meshdata->indecies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshRenderingSystem::BindTexturesWithIndecies(MeshData *curMeshData, Shader *parShader)
{
	unsigned int diffuseCount = 1;
	unsigned int specularCount = 1;
	unsigned int normalCount = 1;
	if (curMeshData->textureIndex.data()) {
		for (unsigned int i = 0; i < curMeshData->textureIndex.size(); i++)
		{
			glActiveTexture(GL_TEXTURE2 + i);
			int index = curMeshData->textureIndex[i];
			if (CommonMeshData::Instance()->loadedTextures[index]->type == diffuse) {
				setUInt(parShader, std::string("mat.diffuseTex" + std::to_string(diffuseCount)).c_str(), i + 2);
				//std::cout << std::string("mat.diffuseTex" + std::to_string(diffuseCount)).c_str() << std::endl;
				diffuseCount++;
			}
			else if (CommonMeshData::Instance()->loadedTextures[index]->type == specular) {
				setUInt(parShader, std::string("mat.specularTex" + std::to_string(specularCount)).c_str(), i + 2);
				specularCount++;
			}
			else if (CommonMeshData::Instance()->loadedTextures[index]->type == normal) {
				setUInt(parShader, std::string("mat.normalTex" + std::to_string(normalCount)).c_str(), i + 2);
				normalCount++;
			}

			//std::cout << curMeshData->shader->ID << std::endl;
			//std::cout << MeshData::loadedTextures[index].texturePath << std::endl;
			glBindTexture(GL_TEXTURE_2D, CommonMeshData::Instance()->loadedTextures[index]->ID);
		}
		glActiveTexture(GL_TEXTURE0);
	}
}

void MeshRenderingSystem::DrawMeshTotal(MeshData* curMeshData, Shader* curShader, Transform* curTrans, int entity)
{
	//std::cout << MeshData::loadedTextures.size() << std::endl;
	//std::cout << curMeshData->meshes.size() << std::endl;
	for (unsigned int i = 0; i < curMeshData->meshEntities.size(); i++)
	{
		MeshData* curMesh = curScene->GetCompOfEntity<MeshData>(curMeshData->meshEntities[i]);

		if (entity == -1) {
			setUFloat(curShader, "curEntity", (float)curMesh->pairedEntity);
		}
		else {
			setUFloat(curShader, "curEntity", (float)entity);
		}
		DrawMeshElements(curMesh, curShader);
	}
}

void MeshRenderingSystem::DrawParticleTotal(MeshData* curMeshData, Shader* curShader, int textureIndex, int entity)
{
	//std::cout << MeshData::loadedTextures.size() << std::endl;
	//std::cout << curMeshData->meshes.size() << std::endl;
	for (unsigned int i = 0; i < curMeshData->meshEntities.size(); i++)
	{
		MeshData* curMesh = curScene->GetCompOfEntity<MeshData>(curMeshData->meshEntities[i]);
		if (entity == -1) {
			setUFloat(curShader, "curEntity", (float)curMesh->pairedEntity);
		}
		else {
			setUFloat(curShader, "curEntity", (float)entity);
		}
		DrawMeshElementsBindingGivenTextureIndex(curMesh, curShader, textureIndex);
	}
}

void MeshRenderingSystem::DrawMeshTotalPlain(MeshData* curMeshData, Shader* curShader)
{
	//std::cout << MeshData::loadedTextures.size() << std::endl;
	//std::cout << curMeshData->meshes.size() << std::endl;
	for (unsigned int i = 0; i < curMeshData->meshEntities.size(); i++)
	{
		MeshData* curMesh = curScene->GetCompOfEntity<MeshData>(curMeshData->meshEntities[i]);
		DrawMeshElementsPlain(curMesh, curShader);
	}
}

