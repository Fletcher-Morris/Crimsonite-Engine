#pragma once

#include <memory>
#include <vector>
#include <array>
#include <bitset>
#include <algorithm>
#include <iostream>
#include <typeinfo>

#include "../asset/AssetManager.h"
#include "../core/Time.h"
#include "Transform.h"

//	Forward-declare ECS classes for compiler.
class EcsEntity;
class EcsComponent;
class EcsSystem;

//	Tyhe maximum number of components on any entity.
const int MAX_ENT_COMPONENTS = 32;
inline int GetComponentId()
{
	static int prevComponentId = 0;
	return prevComponentId++;
}
template <typename T>
inline int GetComponentId() noexcept
{
	static int id = GetComponentId();
	return id;
}

//	The template class for all ecs components.
class EcsComponent : public EditorSerializable
{

private:

	//	The unique id assigned to this component.
	int m_ecsComponentId = 0;
	//	The name of the component.
	std::string m_componentName;
	//	Should the component be used?
	bool m_enabled = true;
	//	Should this component be detached from it's entity?
	bool m_detached = false;

public:

	//	Entity reference
	EcsEntity * entity;
	//	Component destructor
	virtual ~EcsComponent() {}
	//	Called when the component is first initialised.
	virtual void OnInit() {}
	//	Called every frame.
	virtual void OnUpdate() {}
	//	Called at fixed intervals.
	virtual void OnFixedUpdate() {}
	//	Called by the renderer.
	virtual void OnRender() {}
	//	Called when the complnent becomes enabled.
	virtual void OnEnable() {}
	//	Called when the component becomes disabled.
	virtual void OnDisable() {}
	//	Returns true if the component is enabled.
	bool IsEnabled() { return m_enabled; }
	//	Enables the component and calls the OnEnable() method.
	void Enable() { if (!m_enabled) { m_enabled = true; OnEnable(); } }
	//	Disables the component and calls the OnDisable() method.
	void Disable() { if (m_enabled) { m_enabled = false; OnDisable(); } }
	//	Set the enabled state of this component.
	void SetEnabled(bool _enable)
	{
		if (_enable) Enable();
		else Disable();
	}
	//	Detach this component from it's entity.
	void DetachFromEntity();
	//	Returns true if this component is detached.
	bool IsDetached() { return m_detached; }
	//	Returns the unique identity assigned to this component.
	int GetEcsComponentId() { return m_ecsComponentId; }
	//	Return the name of this component.
	std::string GetComponentName()
	{
		if (m_componentName == "")
		{
			return typeid(this).name();
		}
		return m_componentName;
	}
	void SetComponentName(std::string _name) { m_componentName = _name; }
	//	Called by the editor.
	std::string GetTypeString() override
	{
		return "EcsComponent";
	}

};

class EcsEntity : public EditorSerializable
{
	
private:

	//	Pointer to the EcsSystem this entity is part of.
	EcsSystem * m_ecsSystem;
	//	The unique id assigned to this entity.
	int m_ecsEntityId;
	//	The name of this entity.
	std::string m_name;
	char m_tempName[128] = "temp name";
	//	The enabled state of this entity.
	bool m_enabled = true;
	bool m_enabledPrev = true;
	void EnabledChangedCheck()
	{
		if (m_enabled != m_enabledPrev)
		{
			SetEnabled(m_enabledPrev);
		}
	}
	//	Should this entity be destroyed upon ECS refresh?
	bool m_doDestroy;
	//	Is this entity immortal?
	bool m_immortal = false;
	//	Should this entity be serialized?
	bool m_serializable = true;
	std::vector<std::unique_ptr<EcsComponent>> m_componentsVector;
	std::array<EcsComponent*, MAX_ENT_COMPONENTS> m_componentsArray;
	std::bitset<MAX_ENT_COMPONENTS> m_componentsBitset;
	int m_componentsCount = 0;

public:

	EcsEntity(EcsSystem * _system) { m_ecsSystem = _system; };
	EcsEntity(EcsSystem * _system, std::string _entityName) { m_ecsSystem = _system; SetName(_entityName);};
	EcsEntity(EcsSystem * _system, std::string _entityName, Transform * _parent) { m_ecsSystem = _system; SetName(_entityName); transform.SetParent(_parent); };
	//	Set the name of this entity.
	void SetName(std::string _name);
	//	Return the name of this entity.
	std::string GetName() { return m_name; }
	//	Return true of this entity is enabled.
	bool IsEnabled() { return m_enabled; }
	//	Return true if this entity is marked as destroyed.
	bool IsDestroyed() { return m_doDestroy; }
	//	Mark this entity for destruction.
	void Destroy();
	//	Make this entity immortal.
	void MakeImmortal(bool _immortal) { m_immortal = _immortal; }
	//	Prevent this entity from being serialized.
	void SetSerializable(bool _serializable) { m_serializable = _serializable; }
	//	Is this entity serializable?
	bool Serializable() { return m_serializable; }
	//	Return the unique identity assigned to this entity.
	int GetEcsEntityId() { return m_ecsEntityId; }
	//	Return the EcsSystem this entity is part of.
	EcsSystem * GetEcsSystem() { return m_ecsSystem; }
	//	The Transform of the entity.
	Transform transform;
	//	Return a reference to the entity's Transform.
	Transform * GetTransformRef() { return &transform; }
	//	Set the Entity's Transform's parent.
	void SetParent(Transform * _parent);
	void SetParent(EcsEntity * _entity);
	//	Unparent the Entity.
	void Unparent();
	//	Return the number of components on this entity.
	int ComponentCount() { return m_componentsArray.size(); }
	//	Called when loading the scene.
	void Deserialize(std::vector<std::string> _data) override;
	//	Called when loading the scene.
	std::string Serialize() override;
	//	Called every frame.
	void Update();
	//	Called at fixed intervals.
	void FixedUpdate();
	//	Called by the renderer.
	void Render();
	//	Called by the editor.
	void DrawEditorProperties() override;
	//	Called by the editor.
	std::string GetTypeString() override
	{
		return "EcsEntity";
	}
	//	Set the enabled state of the entity.
	void SetEnabled(bool _enable);
	//	Check if this entity has a specific component attached.
	template<typename T> bool HasComponent()
	{
		//return m_componentsBitset[GetComponentId<T>];
		if (&GetComponent<T>() != NULL) return true;
		return false;
	}
	//	Add a component to this entity and return a reference.
	template<typename T, typename... args>
	T * AttachComponent(args&&... _args);
	//	Return a reference to a component on this entity.
	template<typename T> T& GetComponent()
	{
		return *static_cast<T*>(m_componentsArray[GetComponentId<T>()]);
	}
	//	Return a reference to a component on this entity with a given ID.
	template<typename T> T& GetComponentWithId(int _id)
	{
		return *static_cast<T*>(m_componentsVector.at(_id));
	}
};

class EcsSystem : public EditorSerializable
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
	int EntityCount() { return m_totalEntityCount; }
	EcsEntity * NewestEntity() { return m_newestEntity; }
	//	Create a new entity with a given name.
	EcsEntity& NewEntity(std::string _entityName);
	//	Create a new entity with a given name and parent.
	EcsEntity& NewEntity(std::string _entityName, Transform * _parent);
	//	Find an entity with a given name.
	EcsEntity * FindEntity(std::string _entityName);
	template<typename T>
	std::vector<EcsEntity*> FindEntitiesWithComponent();
	template<typename T> std::vector<T*> GetAllComponentsOfType();
	EcsEntity * GetEntityById(int _id) { return &*entities[_id]; }
	EcsEntity * FindEntityByTransform(Transform * _transform);
	//	Destroy a given entity.
	void DestroyEntity(EcsEntity & _deleteEntity) { _deleteEntity.Destroy(); }
	//	Destroy an entity with a given name.
	void DestroyEntity(std::string _entityName) { FindEntity(_entityName)->Destroy(); }
	//	Alert the system of a deleted entity.
	void AlertEntityDestruction()
	{
		m_destroyedEntityCount++;
		if (m_liveEntityCount > 0) m_liveEntityCount--;
	}
	//	Clear out all destroyed entities and reset counters.
	void Refresh();
	std::string Serialize() override;
	//	Called by the editor.
	std::string GetTypeString() override
	{
		return "EcsSystem";
	}
	void DeserializeEntity(std::vector<std::string> _serializedComponent);
	void DeserializeComponent(std::vector<std::string> _serializedComponent);
};

template<typename T, typename ...args>
inline T * EcsEntity::AttachComponent(args && ..._args)
{
	if (HasComponent<T>()) return NULL;
	T * newComponent(new T(std::forward<args>(_args)...));
	std::unique_ptr<EcsComponent> uniquePtr{ newComponent };
	m_componentsVector.emplace_back(std::move(uniquePtr));
	m_componentsArray[GetComponentId<T>()] = newComponent;
	m_componentsBitset[GetComponentId<T>()] = true;
	m_componentsCount++;
	std::string nameStr = typeid(T).name();
	nameStr.erase(0, 6);
	newComponent->entity = this;
	newComponent->SetComponentName(nameStr);
	newComponent->OnInit();
	return newComponent;
}

template<typename T>
inline std::vector<EcsEntity*> EcsSystem::FindEntitiesWithComponent()
{
	std::vector<EcsEntity*> foundEntities;
	for (int i = 0; i < m_totalEntityCount; i++)
	{
		if (entities[i]->HasComponent<T>())
		{
			foundEntities.push_back(&*entities[i]);
		}
	}
	return foundEntities;
}

template<typename T>
inline std::vector<T*> EcsSystem::GetAllComponentsOfType()
{
	std::vector<T*> foundComponents;
	for (int i = 0; i < m_totalEntityCount; i++)
	{
		T* component = &entities[i]->GetComponent<T>();
		if (component)
		{
			foundComponents.push_back(component);
		}
	}
	return foundComponents;
}
