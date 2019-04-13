#pragma once

#include <vector>
#include <string>
#include "../external/imgui/imgui.h"

class EditorSerializable
{

public:

	//	Set the serializable values of the class when the scene deserializes.
	virtual void Deserialize(std::vector<std::string> _data) {}
	//	Get the serializable properties of the class.
	virtual std::string Serialize() { return std::string(); }

	//	Used for drawing editor properties and tools.
	virtual void DrawEditorProperties() {}
};