#pragma once

#include <string>

class Scene
{

public:

	Scene(std::string _scenePath);

	void Serialize();
	std::string GetSerializedString();

	std::string GetName() { return m_name; }
	void SetName(std::string _sceneName) { m_name = _sceneName; }

private:

	std::string m_serializedString;


	std::string m_name;

};