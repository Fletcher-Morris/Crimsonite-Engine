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

class EcsEntity
{
	
private:

	int m_ecsEntityId;
	std::string m_name;
	bool m_enabled;
	bool m_doDestroy;

	std::vector<std::unique_ptr<EcsComponent>> m_componentsVector;
	std::array<EcsComponent*, MAX_ENT_COMPONENTS> m_componentsArray;
	std::bitset<MAX_ENT_COMPONENTS> m_componentsBitset;

public:

	void SetName(std::string _name) { m_name = _name; }
	std::string GetName() { return m_name; }
	bool IsEnabled() { return m_enabled; }
	bool IsDestroyed() { return m_doDestroy; }
	void Destroy() { m_doDestroy = true; }
	int GetEcsEntityId() { return m_ecsEntityId; }

	void Update();
	void FixedUpdate();
	void Render();


	//	Check if an entity has a specific component attached.
	template<typename T> bool HasComponent()
	{
		return m_componentsBitset[GetComponentId<T>];
	}

	//	Add a component to an entity and return a reference.
	template<typename T, typename... args>
	T& AttachComponent(args&&... _args)
	{
		T* newComponent(new T(std::forward<args>(_args)...));
		newComponent->entity = this;
		std::unique_ptr<EcsComponent> uniquePtr{ newComponent };
		m_componentsVector.emplace_back(std::move(uniquePtr));
		m_componentsArray[GetComponentId<T>()] = newComponent;
		m_componentsBitset[GetComponentId<T>()] = true;
		newComponent->OnInit();
		return *newComponent;
	}

	//	Return a reference to a component on an entity.
	template<typename T> T& GetComponent()
	{
		auto ptr(m_componentsArray[GetComponentId<T>()]);
		return *static_cast<T*>(ptr);
	}

};