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
	//	Can there be more than one of this component on a sngle entity?
	bool m_uniquePerEntity = false;

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
	//	Returns true if the component is entity-unique.
	bool IsUnique() { return m_uniquePerEntity; }
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

};

class EcsEntity : public EditorSerializable
{
	
private:

	//	Pointer toe the EcsSystem this entity is part of.
	EcsSystem * m_system;
	//	The unique id assigned to this entity.
	int m_ecsEntityId;
	//	The name of this entity.
	std::string m_name;
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

	EcsEntity(EcsSystem * _system) { m_system = _system; };
	EcsEntity(EcsSystem * _system, std::string _entityName) { m_system = _system; m_name = _entityName; };
	EcsEntity(std::string _entityName, EcsSystem * _system) { m_system = _system; m_name = _entityName; };

	//	Set the name of this entity.
	void SetName(std::string _name) { m_name = _name; }
	//	Return the name of this entity.
	std::string GetName() { return m_name; }
	//	Return true of this entity is enabled.
	bool IsEnabled() { return m_enabled; }
	//	Return true if this entity is marked as destroyed.
	bool IsDestroyed() { return m_doDestroy; }
	//	Mark this entity for destruction.
	void Destroy()
	{
		if (m_immortal)
		{
			std::cout << "Cannot destroy immortal entity '" << m_name << "'!" << std::endl;
			return;
		}
		std::cout << "Destroyed entity : " << m_name << std::endl;
		/*m_system->AlertEntityDestruction();*/
		m_doDestroy = true;
	}
	//	Make this entity immortal.
	void MakeImmortal(bool _immortal) { m_immortal = _immortal; }
	//	Prevent this entity from being serialized.
	void SetSerializable(bool _serializable) { m_serializable = _serializable; }
	//	Is this entity serializable?
	bool Serializable() { return m_serializable; }
	//	Return the unique identity assigned to this entity.
	int GetEcsEntityId() { return m_ecsEntityId; }
	//	Return the EcsSystem this entity is part of.
	EcsSystem * GetEcsSystem() { return m_system; }

	//	The Transform of the entity.
	Transform transform;
	//	Return a reference to the entity's Transform.
	Transform * GetTransformRef() { return &transform; }

	//	Return the number of components on this entity.
	int ComponentCount() { return m_componentsArray.size(); }

	//	Called when loading the scene.
	void Deserialize(std::vector<std::string> _data) override
	{
		if (m_serializable == false) return;
		SetName(_data[0]);
		SetEnabled(_data[1] == "true" ? true : false);
		MakeImmortal(_data[2] == "true" ? true : false);
		transform.Deserialize(_data);
	}
	std::string Serialize() override
	{
		if (m_serializable == false) return std::string();

		std::string serialized = "BeginEntity";
		serialized += "\nval ";
		serialized += GetName();
		serialized += "\nval ";
		serialized += IsEnabled() == true ? "true" : "false";
		serialized += "\nval ";
		serialized += m_immortal == true ? "true" : "false";
		serialized += "\n";
		serialized += transform.Serialize();
		serialized += "\nEndEntity";
		for (int i = 0; i < m_componentsVector.size(); i++)
		{
			serialized += "\n";
			serialized += "BeginComponent ";
			serialized += m_componentsVector.at(i)->GetComponentName();
			serialized += "\nval ";
			serialized += m_componentsVector.at(i)->IsEnabled() == true ? "true" : "false";
			serialized += "\n";
			serialized += m_componentsVector.at(i)->Serialize();
			serialized += "\nEndComponent";
		}
		serialized += "\n";
		return serialized;
	}

	//	Called every frame.
	void Update()
	{
		if (m_doDestroy) return;
		EnabledChangedCheck();
		if (m_enabled == false) return;
		for (int i = 0; i < m_componentsCount; i++)
		{
			if(m_componentsVector[i]->IsEnabled())
			m_componentsVector[i]->OnUpdate();
		}
	}

	//	Called at fixed intervals.
	void FixedUpdate()
	{
		if (m_doDestroy) return;
		EnabledChangedCheck();
		if (m_enabled == false) return;
		for (int i = 0; i < m_componentsCount; i++)
		{
			if (m_componentsVector[i]->IsEnabled())
			m_componentsVector[i]->OnFixedUpdate();
		}
	}

	//	Called by the renderer.
	void Render()
	{
		if (m_doDestroy) return;
		EnabledChangedCheck();
		if (m_enabled == false) return;
		for (int i = 0; i < m_componentsCount; i++)
		{
			if (m_componentsVector[i]->IsEnabled())
			m_componentsVector[i]->OnRender();
		}
	}


	//	Called by the editor.
	void DrawEditorProperties() override
	{
		ImGui::Text("%s", m_name.c_str());
		std::string enableString = "ENABLE";
		if(m_enabled) enableString = "DISABLE";
		if (ImGui::Button(enableString.c_str()))
		{
			SetEnabled(!m_enabled);
		}
		ImGui::SameLine();
		if (ImGui::Button("DESTROY"))
		{
			Destroy();
		}
		ImGui::Separator();
		ImGui::NewLine();
		transform.DrawEditorProperties();
		for (int i = 0; i < m_componentsCount; i++)
		{
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();
			ImGui::Text("%s", m_componentsVector[i]->GetComponentName().c_str());
			bool componentEnabled = m_componentsVector[i]->IsEnabled();
			ImGui::Checkbox("Enabled", &componentEnabled);
			m_componentsVector[i]->SetEnabled(componentEnabled);
			ImGui::NewLine();
			m_componentsVector[i]->DrawEditorProperties();
		}
	}

	void SetEnabled(bool _enable)
	{
		m_enabled = _enable;
		m_enabledPrev = _enable;
		for (int i = 0; i < m_componentsCount; i++)
		{
			if(_enable == true) m_componentsVector[i]->OnEnable();
			else m_componentsVector[i]->OnDisable();
		}
	}

	//	Check if this entity has a specific component attached.
	template<typename T> bool HasComponent()
	{
		return m_componentsBitset[GetComponentId<T>];
	}

	//	Add a component to this entity and return a reference.
	template<typename T, typename... args>
	T& AttachComponent(args&&... _args)
	{
		T* newComponent(new T(std::forward<args>(_args)...));
		newComponent->entity = this;
		std::unique_ptr<EcsComponent> uniquePtr{ newComponent };
		m_componentsVector.emplace_back(std::move(uniquePtr));
		m_componentsArray[GetComponentId<T>()] = newComponent;
		m_componentsBitset[GetComponentId<T>()] = true;
		m_componentsCount++;
		std::string nameStr = typeid(T).name();
		nameStr.erase(0, 6);
		newComponent->SetComponentName(nameStr);
		newComponent->OnInit();
		return *newComponent;
	}

	//	Return a reference to a component on this entity.
	template<typename T> T& GetComponent()
	{
		auto ptr(m_componentsArray[GetComponentId<T>()]);
		return *static_cast<T*>(ptr);
	}

	//	Return a reference to a component on this entity with a given ID.
	template<typename T> T& GetComponentWithId(int _id)
	{
		auto ptr(m_componentsVector.at(_id));
		return *static_cast<T*>(ptr);
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
	EcsEntity& NewEntity(std::string _entityName)
	{
		bool nameFound = false;
		int entityCounter = 1;
		std::string tempName = _entityName;
		while (nameFound == false)
		{
			if (FindEntity(tempName))
			{
				entityCounter++;
				tempName = _entityName + " (" + std::to_string(entityCounter) + ")";

			}
			else
			{
				nameFound = true;
			}
		}
		_entityName = tempName;
		EcsEntity * newEntity = new EcsEntity(this, _entityName);
		std::unique_ptr<EcsEntity>entPtr{ newEntity };
		entities.emplace_back(std::move(entPtr));
		m_newestEntity = newEntity;
		m_totalEntityCount++;
		m_liveEntityCount++;
		return *newEntity;
	}

	//	Find an entity with a given name.
	EcsEntity * FindEntity(std::string _entityName)
	{
		//	MAKE THIS FASTER
		for (int i = 0; i < m_totalEntityCount; i++)
		{
			if (entities[i]->IsDestroyed() == false)
			{
				if (entities[i]->GetName() == _entityName)
				{
					return &*entities[i];
				}
			}
		}
		return NULL;
	}

	template<typename T>
	std::vector<EcsEntity*> FindEntitiesWithComponent()
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

	template<typename T> std::vector<T*> GetAllComponentsOfType()
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

	EcsEntity * GetEntityById(int _id)
	{
		return &*entities[_id];
	}

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
	void Refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<EcsEntity> &ent) {return ent->IsDestroyed(); }), std::end(entities));
		m_totalEntityCount = entities.size();
		m_liveEntityCount = m_totalEntityCount;
		m_destroyedEntityCount = 0;
	}

	std::string Serialize() override
	{
		std::string serialized = "";
		for (int i = 0; i < EntityCount(); i++)
		{
			EcsEntity * ent = &*entities[i];
			if (ent->IsDestroyed() == false)
			{
				if(ent->Serializable())
				serialized += ent->Serialize();
				serialized += "\n";
			}
		}
		return serialized;
	}

	void DeserializeEntity(std::vector<std::string> _serializedComponent)
	{
		NewEntity(_serializedComponent[0]);
		m_newestEntity->Deserialize(_serializedComponent);
	}
	void DeserializeComponent(std::vector<std::string> _serializedComponent)
	{
		
	}
};