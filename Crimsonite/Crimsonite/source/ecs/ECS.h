#pragma once

#include <memory>
#include <vector>
#include <array>
#include <bitset>
#include <algorithm>
#include <iostream>

class EcsEntity;
class EcsComponent;

const int MAX_ENT_COMPONENTS = 32;

inline int GetComponentId()
{
	static int prevComponentId = 0;
	return prevComponentId++;
}


//	The template class for all ecs components.
class EcsComponent
{

private:

	int m_ecsComponentId = 0;
	bool m_enabled = true;
	bool m_uniquePerEntity = false;

public:

	//	Entity reference
	virtual ~EcsComponent() {}

	virtual void OnInit() {}
	virtual void OnUpdate() {}
	virtual void OnFixedUpdate() {}
	virtual void OnRender() {}
	virtual void OnEnable() {}
	virtual void OnDisable() {}

	bool IsEnabled() { return m_enabled; }
	void Enable() { if (!m_enabled) { m_enabled = true; OnEnable(); } }
	void Enable() { if (m_enabled) { m_enabled = true; OnDisable(); } }
	bool IsUnique() { return m_uniquePerEntity; }
	int GetEcsComponentId() { return m_ecsComponentId; }

};