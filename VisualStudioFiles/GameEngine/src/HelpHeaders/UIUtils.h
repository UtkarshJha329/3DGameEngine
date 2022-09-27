#pragma once

#pragma once

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../ImGUIStuff/imgui.h"
#include "../ImGUIStuff/imgui_impl_glfw.h"
#include "../ImGUIStuff/imgui_impl_opengl3.h"
#include "../ImGUIStuff/imgui_stdlib.h"

#include "../ImGUIStuff/imgui_internal.h"

#include "RandomProps.h"

#define UIfyVec3(...) UIfyGlmVec3(Split(#__VA_ARGS__, ',').data(),__VA_ARGS__)
#define UIfyString(...) UIfyStdString(Split(#__VA_ARGS__, ',').data(),__VA_ARGS__)
#define UIfyBool(...) UIfyDefBool(Split(#__VA_ARGS__, ',').data(),__VA_ARGS__)
#define UIfyFloat(...) UIfyDefFloat(Split(#__VA_ARGS__, ',').data(),__VA_ARGS__)
#define UIfyInt(...) UIfyDefInt(Split(#__VA_ARGS__, ',').data(),__VA_ARGS__)
#define UIfyCombo(label, value, ...) UIfyDefCombo(label, Split(#__VA_ARGS__, ','), value)

/*
#define UIF(dt, ...) SerializeArgs(dt, Split(#__VA_ARGS__, ','),__VA_ARGS__)
#define UIFvecs(...) SerializeGlmVec3(Split(#__VA_ARGS__, ','),__VA_ARGS__)
#define UIFstrs(...) SerializeString(Split(#__VA_ARGS__, ','),__VA_ARGS__)


enum class DelimiterType {
	NONE,
	TAB_PER_WRITE,
	NEW_LINE,
	SPACE_PER_WRITE,
	COMMA_PER_WRITE
};


#pragma region Serialization

template<typename ...Args>
static void SerializeArgs(DelimiterType dt, std::vector<std::string> varNamesVec, Args... args) {

	std::string* varName = varNamesVec.data();

	for (auto p : { args... }) {

		SerializeArgs(dt, varName, p);
		varName++;
	}
}

template<typename ...Args>
static void SerializeArgs(DelimiterType dt, std::string* varName, Args... args) {

	if (varName) {
		RandomProps::curSaveStr += *varName + " : ";
	}

	for (auto p : { args... }) {

		RandomProps::curSaveStr += std::to_string(p);

		if (dt == DelimiterType::SPACE_PER_WRITE) {
			RandomProps::curSaveStr += " ";
		}
		else if (dt == DelimiterType::COMMA_PER_WRITE) {
			RandomProps::curSaveStr += ",";
		}
		else if (dt == DelimiterType::TAB_PER_WRITE) {
			RandomProps::curSaveStr += "\t";
		}
		else if (dt == DelimiterType::NEW_LINE) {
			RandomProps::curSaveStr += "\n";
		}
	}
}

static void SerializeGlmVec3(std::string* varName, glm::vec3 sVec) {

	SerializeArgs(DelimiterType::SPACE_PER_WRITE, varName, sVec.x, sVec.y, sVec.z);
	RandomProps::curSaveStr += "\n";
}

static void SerializeString(std::string* varName, std::string str) {

	RandomProps::curSaveStr += *varName + " : ";

	RandomProps::curSaveStr += str;

	RandomProps::curSaveStr += "\n";
}

template<typename... Vecs>
static void SerializeGlmVec3(std::vector<std::string> varNamesVec, Vecs... vecs) {

	std::string* varNames = varNamesVec.data();

	for (auto vec : { vecs... })
	{
		SerializeGlmVec3(varNames, vec);
		varNames++;
	}
}

template<typename ...Strs>
static void SerializeString(std::vector<std::string> varNamesVec, Strs... strs) {

	std::string* varNames = varNamesVec.data();

	for (auto str : { strs... })
	{
		SerializeString(varNames, str);
		varNames++;
	}
}

#pragma endregion

#pragma region DeSerialze

template<typename ...Args>
static void DeSerializeArgs(std::vector<std::string> varNamesVec, Args... args) {

	std::string* varName = varNamesVec.data();

	for (auto p : { args... }) {

		DeSerializeArgs(varName, p);
		varName++;
	}
}

template<typename ...Args>
static void DeSerializeArgs(std::string* varName, Args... args) {

	std::string serializedString = "";
	std::getline(RandomProps::curInFileStream, serializedString);

	std::vector<std::string> strs = Split(serializedString, ' ');

	if (std::strcmp(strs[0].c_str(), varName->c_str()) != 0) {

		return;
	}

	int varIndex = 2;

	for (auto p : { args... }) {

		std::stringstream curVar(strs[varIndex]);
		curVar >> *p;

		varIndex++;
	}
}

#pragma endregion
*/

static void DrawVec3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

static void DrawTextField(const std::string& label, std::string& value, float columnWidth = 100.0f) {

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::InputText("##X", &value);

	ImGui::Columns(1);

	ImGui::PopID();
}

static void DrawCheckBox(const std::string& label, bool& value, float columnWidth = 100.0f) {

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::Checkbox("##X", &value);

	ImGui::Columns(1);

	ImGui::PopID();
}

static void DrawInputFloat(const std::string& label, float& value, float columnWidth = 100.0f) {

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::InputFloat("##X", &value);

	ImGui::Columns(1);

	ImGui::PopID();
}

static void DrawInputInt(const std::string& label, int& value, float columnWidth = 100.0f) {

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::InputInt("##X", &value);

	ImGui::Columns(1);

	ImGui::PopID();
}

static void DrawCombo(const std::string& label, int& curItem, const char& itemsList, float columnWidth = 100.0f) {

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::Combo("##X", &curItem, &itemsList);

	ImGui::Columns(1);

	ImGui::PopID();
}

#pragma region Vec3

static void UIfyGlmVec3(std::string* varName, glm::vec3& sVec) {

	DrawVec3(*varName, sVec);
}

template<typename... Vecs>
static void UIfyGlmVec3(std::string* varNames, glm::vec3& vec, Vecs&... vecs) {


	UIfyGlmVec3(varNames, vec);

	varNames++;
	UIfyGlmVec3(varNames, vecs...);

}

#pragma endregion

#pragma region String

static void UIfyStdString(std::string* varName, std::string& value) {

	DrawTextField(*varName, value);
}

template<typename... Vals>
static void UIfyStdString(std::string* varNames, std::string& value, Vals&... vals) {


	UIfyStdString(varNames, value);

	varNames++;
	UIfyStdString(varNames, vals...);

}

#pragma endregion

#pragma region Bool

static void UIfyDefBool(std::string* varName, bool& value) {

	DrawCheckBox(*varName, value);
}

template<typename... Vals>
static void UIfyDefBool(std::string* varNames, bool& value, Vals&... vals) {


	UIfyDefBool(varNames, value);

	varNames++;
	UIfyDefBool(varNames, vals...);

}

#pragma endregion

#pragma region Float

static void UIfyDefFloat(std::string* varName, float& value) {

	DrawInputFloat(*varName, value);
}

template<typename... Vals>
static void UIfyDefFloat(std::string* varNames, float& value, Vals&... vals) {


	UIfyDefFloat(varNames, value);

	varNames++;
	UIfyDefFloat(varNames, vals...);

}
#pragma endregion

#pragma region Int

static void UIfyDefInt(std::string* varName, int& value) {

	DrawInputInt(*varName, value);
}

template<typename... Vals>
static void UIfyDefInt(std::string* varNames, int& value, Vals&... vals) {


	UIfyDefInt(varNames, value);

	varNames++;
	UIfyDefInt(varNames, vals...);

}

#pragma endregion

#pragma region Combo

static void UIfyDefCombo(std::string label, std::vector<std::string> varNames, int& value) {

	std::string comboStr = "";

	for (int i = 0; i < varNames.size(); i++)
	{
		comboStr += (varNames[i] + '\0');
	}

	comboStr += '\0';

	DrawCombo(label, value, *comboStr.c_str());
}
#pragma endregion

#pragma region Image

static void UIfyTexture(std::string& textureName, unsigned int textureID, float x = 50.0f, float y = 50.0f) {
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(textureName.c_str());

	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, 100.0f);

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 0.0f, 0.0f, 0.5f); // 50% opaque red
	ImGui::Image((ImTextureID)textureID, ImVec2(x, y), uv_min, uv_max, tint_col, border_col);

	ImGui::NextColumn();

	ImGui::Text(textureName.c_str());
	ImGui::Columns(1);

	ImGui::PopID();
	//ImGui::End();
}

#pragma endregion
