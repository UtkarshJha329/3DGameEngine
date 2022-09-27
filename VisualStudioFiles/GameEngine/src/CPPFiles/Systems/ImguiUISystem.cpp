#include "../../HelpHeaders/Systems/ImguiUISystem.h"

void ImguiUISystem::PreAwake() {
}

void ImguiUISystem::Awake() {
	InitialiseImGUI();
	EditorGuiVars::Inst()->SetTextures();
}

void ImguiUISystem::Start() {

}

void ImguiUISystem::Update() {
	PrepareImguiFrame();
	DrawGUI();
}

void ImguiUISystem::LateUpdate() {}

void ImguiUISystem::Clear() {

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

void ImguiUISystem::InitialiseImGUI() {

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(WindowProperties::instance->window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void ImguiUISystem::PrepareImguiFrame() {

	Tabler* curSceneTabler = &(Scene::curScene->sceneTabler);

	bool showDemoWindow = true;
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&showDemoWindow);

		ImGui::DockSpaceOverViewport();

		{

			ImGui::Begin("Entities");
			
			/*for (int i = 0; i < curSceneTabler->entity_Set.sparse_Index.size(); i++)
			{
				if(curSceneTabler->entity_Set.entity_Alive[i])
				{
					int index = curSceneTabler->entity_Set.sparse_Index[i];
					int entity = curSceneTabler->entity_Set.dense_Entities[index];
					if (ImGui::Selectable(std::to_string(entity).c_str())) {
						lastSelectedEntity = entity;
					}
				}
			}*/



			DrawEntityTree(0, &lastSelectedEntity);

			/*{

				unsigned int newEntity = curScene->sceneTabler.entity_Set.NewEntity(true);

				auto parchiSet = curScene->GetCompSparseSet<ParentChild>();
				parchiSet->Add(newEntity, true);

				ParentChild* curParChiZero = curScene->GetCompOfEntity<ParentChild>(0);
				ParentChild* curParChi = curScene->GetCompOfEntity<ParentChild>(newEntity);

				curParChiZero->childrenEntity.push_back(newEntity);
				curParChi->parEntity = 0;
				curParChi->currentEntity = newEntity;

			}*/

			ImGui::End();


		}

		{
			ImGui::Begin("Inspector");


			if (lastSelectedEntity >= 0
				&& lastSelectedEntity <= curSceneTabler->entity_Set.sparse_Index.size()
				&& curSceneTabler->entity_Set.entity_Alive[lastSelectedEntity])
			{
				ImGui::Text(std::to_string(lastSelectedEntity).c_str());
				DrawComponentsOfEntity(lastSelectedEntity);
			}

			ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			ImGui::Selectable("Add Component", &RandomProps::createNewEntitySelectable);
			if(RandomProps::createNewEntitySelectable){

				ImGui::ListBoxHeader("Components");

				for (int i = 0; i < curScene->sceneTabler.allCmpsSSets.size(); i++)
				{
					if (ImGui::Selectable(curScene->sceneTabler.allCmpsSSets[i]->compName.c_str())) {
						curScene->sceneTabler.allCmpsSSets[i]->Add(lastSelectedEntity, true);
					}
				}

				ImGui::ListBoxFooter();
			}

			ImGui::End();
		}


		#pragma region TexturesUI
		{
			{
				ImGui::Begin("Textures");

				CompSparseSet<MeshData>* meshSet = curScene->GetCompSparseSet<MeshData>();
				for (int i = 0; i < meshSet->DenseSize(); i++)
				{
					if (meshSet->denseTArray[i].meshEntities.size() > 0) {

						std::string texTitle = "Textures : " + std::to_string(i);
						if (ImGui::TreeNodeEx(texTitle.c_str())) {
							for (int j = 0; j < meshSet->denseTArray[i].meshEntities.size(); j++)
							{
								std::string label = "mesh : " + std::to_string(meshSet->denseTArray[i].meshEntities[j]);
								if (ImGui::TreeNodeEx(label.c_str())) {

									MeshData* curMesh = curScene->GetCompOfEntity<MeshData>(meshSet->denseTArray[i].meshEntities[j]);
									for (int k = 0; k < curMesh->textureIndex.size(); k++)
									{
										Texture* text = CommonMeshData::Instance()->loadedTextures[curMesh->textureIndex[k]];
										std::string textureName = text->texturePath.substr(text->texturePath.find_last_of('/') + 1, text->texturePath.size());
										UIfyTexture(textureName, text->ID);
									}
									ImGui::TreePop();
								}
							}
							ImGui::TreePop();

						}
					}
				}

				ImGui::End();
			}
		}
		#pragma endregion

		{
			CompSparseSet<CameraComponent>* camSet = curScene->GetCompSparseSet<CameraComponent>();

			unsigned int denseTIndex = camSet->sparse_Indecies[CameraComponent::mainCameraEntity];
			CameraComponent* mainCam = &camSet->denseTArray[denseTIndex];

			#pragma region ScenePanel
			{
				ImGui::Begin("Scene");
				auto viewportOffset = ImGui::GetCursorPos();

				auto windowSize = ImGui::GetWindowSize();
				ImVec2 minBound = ImGui::GetWindowPos();
				minBound.x += viewportOffset.x;
				minBound.y += viewportOffset.y;

				ImVec2 maxBound = { minBound.x + windowSize.x , minBound.y + windowSize.y };
				glm::vec2 m_ViewportBounds[2];
				m_ViewportBounds[0] = { minBound.x, minBound.y };
				m_ViewportBounds[1] = { maxBound.x, maxBound.y };

				auto [mx, my] = ImGui::GetMousePos();
				mx -= m_ViewportBounds[0].x;
				my -= m_ViewportBounds[0].y;

				glm::vec2 s_viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
				
				my = s_viewportSize.y - my;

				int mouseX = (int)mx;
				int mouseY = (int)my;

				//std::cout << mouseX << " : " << mouseY << std::endl;
				CameraComponent* mainCam = curScene->GetCompOfEntity<CameraComponent>(CameraComponent::mainCameraEntity);

				if (ImGui::IsMouseDown(0)
					&& mouseX >= 0 
					&& mouseY >= 0 
					&& mouseX < (int)s_viewportSize.x 
					&& mouseY < (int)s_viewportSize.y)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, mainCam->FBO);
					glReadBuffer(GL_COLOR_ATTACHMENT2);
					glReadPixels(mouseX, mouseY, 1, 1, GL_RED_INTEGER, GL_INT, &mainCam->entityFromTexture);
					//glGetError();
					std::cout << "Mouse over entity : " << mainCam->entityFromTexture << std::endl;
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					lastSelectedEntity = mainCam->entityFromTexture;
				}


				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				if (WindowProperties::instance->viewportSize.x != viewportSize.x
					|| WindowProperties::instance->viewportSize.y != viewportSize.y) {

					WindowProperties::instance->viewportSize = { viewportSize.x, viewportSize.y };
					WindowProperties::instance->viewportResized = true;
				}

				ImGui::Image((void*)mainCam->renderTextureID, { viewportSize.x, viewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::End();
			}
			#pragma endregion

			#pragma region Content Browser
			
			{
				ImGui::Begin("Assets");

				if (EditorGuiVars::Inst()->contentBrowserCurrentPath != RandomProps::assetsDirectoryPath) {

					if (ImGui::Button("<-")) {
						EditorGuiVars::Inst()->contentBrowserCurrentPath = EditorGuiVars::Inst()->contentBrowserCurrentPath.parent_path();
					}

				}

				
				float cellSize = EditorGuiVars::Inst()->contentBrowserthumbnailSize + EditorGuiVars::Inst()->contentBrowserpadding;

				float panelWidth = ImGui::GetContentRegionAvail().x;

				int numColumns = panelWidth / cellSize;
				if (numColumns < 1)
					numColumns = 1;
				ImGui::Columns(numColumns, 0, false);

				for (auto& directoryEntry : std::filesystem::directory_iterator(EditorGuiVars::Inst()->contentBrowserCurrentPath)) {

					auto path = directoryEntry.path();
					auto relativePath = std::filesystem::relative(path, EditorGuiVars::Inst()->contentBrowserCurrentPath);
					std::string relativePathString = relativePath.string();

					/*ImGui::Button("ICON", { EditorGuiVars::Inst()->contentBrowserthumbnailSize, EditorGuiVars::Inst()->contentBrowserthumbnailSize });
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						if (directoryEntry.is_directory()) {
							EditorGuiVars::Inst()->contentBrowserCurrentPath /= directoryEntry.path().filename();
						}

					}
					ImGui::Text(relativePathString.c_str());*/
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
					if (directoryEntry.is_directory()) {
						ImGui::ImageButton((ImTextureID)EditorGuiVars::Inst()->contentBrowserFolderIcon->ID,
							{ EditorGuiVars::Inst()->contentBrowserthumbnailSize, EditorGuiVars::Inst()->contentBrowserthumbnailSize });
						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							EditorGuiVars::Inst()->contentBrowserCurrentPath /= directoryEntry.path().filename();
						}
					}
					else {
						ImGui::ImageButton((ImTextureID)EditorGuiVars::Inst()->contentBrowserFileIcon->ID, { EditorGuiVars::Inst()->contentBrowserthumbnailSize, EditorGuiVars::Inst()->contentBrowserthumbnailSize });
					}
					ImGui::TextWrapped(relativePathString.c_str());
					ImGui::PopStyleColor();

					//if (directoryEntry.is_directory()) {
					//	if (ImGui::Button("Folder", { thumbnailSize, thumbnailSize })) {
					//		RandomProps::contentBrowserCurrentPath /= directoryEntry.path().filename();
					//	}
					//	ImGui::Text(relativePathString.c_str());
					//}
					//else {
					//	ImGui::Button("File", { thumbnailSize, thumbnailSize });
					//	ImGui::Text(relativePathString.c_str());
					//}

					ImGui::NextColumn();

				}

				ImGui::Columns(1);

				ImGui::SliderFloat("Thumbnail Size", &EditorGuiVars::Inst()->contentBrowserthumbnailSize, 64, 256);
				ImGui::SliderFloat("Padding", &EditorGuiVars::Inst()->contentBrowserpadding, 0, 32);

				ImGui::End();
			}

			#pragma endregion

			//{
			//	ImGui::Begin("ColourTexture");

			//	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			//	if (WindowProperties::instance->viewportSize.x != viewportSize.x
			//		|| WindowProperties::instance->viewportSize.y != viewportSize.y) {

			//		WindowProperties::instance->viewportSize = { viewportSize.x, viewportSize.y };
			//		WindowProperties::instance->viewportResized = true;
			//	}
			//	//std::cout << ImGui::GetMousePos().x << " : " << ImGui::GetMousePos().y << std::endl;
			//	
			//	ImGui::Image((void*)mainCam->entityColourTextureID, { viewportSize.x, viewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
			//	ImGui::End();
			//}
		}
	}
}

void ImguiUISystem::DrawGUI() {

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

}

void ImguiUISystem::DrawComponentsOfEntity(unsigned int entity) {

	Tabler* curSceneTabler = &(Scene::curScene->sceneTabler);

	for (unsigned int i = 0; i < curSceneTabler->allCmpsSSets.size(); i++)
	{
		if (curSceneTabler->allCmpsSSets[i]->EntityHasComp(entity)) {

			if (ImGui::CollapsingHeader((curSceneTabler->allCmpsSSets)[i]->compName.c_str())) {

				(curSceneTabler->allCmpsSSets)[i]->UIfyComp(entity);
			}

		}
	}

}

void ImguiUISystem::DrawEntityTree(unsigned int entity, unsigned int* lastSelectedEntity)
{
	CompSparseSet<ParentChild>* parChi = curScene->GetCompSparseSet<ParentChild>();

	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	bool opened = ImGui::TreeNodeEx(std::to_string(entity).c_str(), base_flags);
	
	if (ImGui::IsItemClicked())
	{
		*lastSelectedEntity = entity;
	}

	if (opened)
	{
		ParentChild* curParChi = &parChi->denseTArray[parChi->sparse_Indecies[entity]];
		for (int i = 0; i < curParChi->childrenEntity.size(); i++)
		{
			ParentChild* childParChi = &parChi->denseTArray[parChi->sparse_Indecies[curParChi->childrenEntity[i]]];

			if (childParChi->childrenEntity.size() == 0) {
				ImGuiTreeNodeFlags node_flags = base_flags;
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

				ImGui::TreeNodeEx(std::to_string(childParChi->currentEntity).c_str(), node_flags);
				if (ImGui::IsItemClicked())
				{
					*lastSelectedEntity = childParChi->currentEntity;
				}
			}
			else {
				DrawEntityTree(childParChi->currentEntity, lastSelectedEntity);
			}
		}

		ImGui::TreePop();
	}
}
