#pragma once

#include <vector>
#include <string>
#include "../external/imgui/imgui.h"

class EditorSerializable
{
public:
	virtual void Deserialize(std::vector<std::string> _data) {}
	virtual std::string Serialize() {}
	virtual void DrawEditorProperties() {}
};