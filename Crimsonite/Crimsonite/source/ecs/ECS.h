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

	EcsSystem * m_system;
	int m_ecsEntityId;
	std::string m_name;
	bool m_enabled;
	bool m_doDestroy;

	std::vector<std::unique_ptr<EcsComponent>> m_componentsVector;
	std::array<EcsComponent*, MAX_ENT_COMPONENTS> m_componentsArray;
	std::bitset<MAX_ENT_COMPONENTS> m_componentsBitset;

public:

	EcsEntity(EcsSystem * _system) { m_system = _system; };
	EcsEntity(EcsSystem * _system, std::string _entityName) { m_system = _system; m_name = _entityName; };
	EcsEntity(std::string _entityName, EcsSystem * _system) { m_system = _system; m_name = _entityName; };

	void SetName(std::string _name) { m_name = _name; }
	std::string GetName() { return m_name; }
	bool IsEnabled() { return m_enabled; }
	bool IsDestroyed() { return m_doDestroy; }
	void Destroy() { std::cout << "Destroyed entity : " << m_name << std::endl; m_system->AlertEntityDestruction(); m_doDestroy = true; }
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

class EcsSystem
{

private:

	//	The most recently created entity.
	EcsEntity * m_newestEntity;
	//	The total number of entities (both live or destroyed).
	int m_totalEntityCount;
	//	The total number of live (not destroyed) entities.
	int m_liveEntityCount;
	//	The total number of destroyed (not live) entities.
	int m_destroyedEntityCount;

public:

	EcsSystem() { Refresh(); }

	//	A vector of all created entities.
	std::vector<std::unique_ptr<EcsEntity>> entities;

	//	Create a new entity with a given name.
	EcsEntity& NewEntity(std::string _entityName)
	{
		EcsEntity * newEntity = new EcsEntity(this, _entityName);
		std::unique_ptr<EcsEntity>entPtr{ newEntity };
		entities.emplace_back(std::move(entPtr));
		std::cout << "Created new entity : " << _entityName << std::endl;
		m_newestEntity = newEntity;
	}

	//	Find an entity with a given name.
	EcsEntity & FindEntity(std::string _entityName)
	{
		auto it = std::find_if(entities.begin(), entities.end(), [&_entityName](EcsEntity & ent) {return ent.GetName() == _entityName; });
		if (it != entities.end()) { return *entities[std::distance(entities.begin(), it)]; }
	}

	//	Destroy a given entity.
	void DestroyEntity(EcsEntity & _deleteEntity) { _deleteEntity.Destroy(); }
	//	Destroy an entity with a given name.
	void DestroyEntity(std::string _entityName) { DestroyEntity(FindEntity(_entityName)); }
	//	Alert the system of a deleted entity.
	void AlertEntityDestruction()
	{
		m_destroyedEntityCount++;
		if (m_liveEntityCount > 0) m_liveEntityCount--;
	}

	//	Clear out all destroyed entities.
	void Refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<EcsEntity> &ent) {return ent->IsDestroyed(); }), std::end(entities));
		m_totalEntityCount = entities.size();
		m_liveEntityCount = m_totalEntityCount;
		m_destroyedEntityCount = 0;
	}
};