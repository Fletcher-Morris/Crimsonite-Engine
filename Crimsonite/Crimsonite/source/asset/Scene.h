#pragma once

#include <string>

class Scene
{

public:

	void Serialize();
	std::string GetSerializedString();

private:

	std::string m_serializedString;


	std::string m_name;

};