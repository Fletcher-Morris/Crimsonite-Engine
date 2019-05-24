#pragma once

#include <vector>
#include "Components.h"

//	Return the names of created Components.
static std::vector<std::string> GetComponentNames()
{
	std::vector<std::string> names;
	names.push_back("Camera");
	names.push_back("MeshRenderer");
	names.push_back("Rotator");
	return names;
}

//	Attach a Component with a given name to a specific Entity.
static void AttachComponentWithName(EcsEntity * _entity, std::string _componentName)
{
	if (_componentName == "Camera")
	{
		_entity->AttachComponent<Camera>();
	}
	else if (_componentName == "MeshRenderer")
	{
		_entity->AttachComponent<MeshRenderer>();
	}
	else if (_componentName == "Rotator")
	{
		_entity->AttachComponent<Rotator>();
	}
}