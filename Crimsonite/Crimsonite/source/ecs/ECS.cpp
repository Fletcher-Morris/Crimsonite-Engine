#include "ECS.h"

void EcsEntity::SetName(std::string _name)
{
	bool uniqueNameFound = false;
	int entityCounter = 1;
	std::string tempName = _name;

	while (uniqueNameFound == false)
	{
		if (EcsEntity * ent = GetEcsSystem()->FindEntity(tempName))
		{
			if (ent != this)
			{
				entityCounter++;
				tempName = _name + " (" + std::to_string(entityCounter) + ")";
			}
			else
			{
				uniqueNameFound = true;
			}
		}
		else { uniqueNameFound = true; }
	}
#pragma warning(push)
#pragma warning(disable: 4996)
	m_name = tempName; std::strcpy(m_tempName, tempName.c_str());
#pragma warning(pop)
}
