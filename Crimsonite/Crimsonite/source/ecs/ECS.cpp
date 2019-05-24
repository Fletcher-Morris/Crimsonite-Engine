#include "ECS.h"
#include "Components.h"
#include "ComponentNames.h"
#include "../editor/Editor.h"

void EcsEntity::SetName(std::string _name)
{
	bool uniqueNameFound = false;
	int entityCounter = 1;
	std::string tempName = _name;

	while (uniqueNameFound == false)
	{
		if (EcsEntity * ent = m_ecsSystem->FindEntity(tempName))
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

void EcsEntity::Destroy()
{
	if (m_immortal)
	{
		std::cout << "Cannot destroy immortal entity '" << m_name << "'!" << std::endl;
		return;
	}
	std::cout << "Destroyed entity : " << m_name << std::endl;
	m_ecsSystem->AlertEntityDestruction();
	m_doDestroy = true;
}

void EcsEntity::SetParent(Transform * _parent)
{
	transform.SetParent(_parent, false);
	if(_parent != NULL)
	std::cout << "Set parent of entity '" << m_name << "'." << std::endl;
}

void EcsEntity::SetParent(EcsEntity * _entity)
{
	transform.SetParent(&_entity->transform, false);
	if(_entity != NULL)
	std::cout << "Parented entity '" << m_name << "' to '" << _entity->GetName() << "'." << std::endl;
}

void EcsEntity::Unparent()
{
	transform.Unparent(false);
	std::cout << "Unparented entity '" << m_name << "'." << std::endl;
}

void EcsEntity::Deserialize(std::vector<std::string> _data)
{
	if (m_serializable == false) return;
	SetName(_data[0]);
	SetEnabled(_data[1] == "true" ? true : false);
	MakeImmortal(_data[2] == "true" ? true : false);
	transform.Deserialize(_data);
}

std::string EcsEntity::Serialize()
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
	serialized += "\nval ";
	if (m_ecsSystem->FindEntityByTransform(transform.GetParent()) != NULL)
	{
		serialized += m_ecsSystem->FindEntityByTransform(transform.GetParent())->GetName();
	}
	else
	{
		serialized += "NO_PARENT";
	}
	serialized += "\nEndEntity";
	for (int i = 0; i < m_componentsVector.size(); i++)
	{
		if (!m_componentsVector.at(i)->IsDetached())
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
	}
	serialized += "\n";
	return serialized;
}

void EcsEntity::Update()
{
	if (m_doDestroy) return;
	EnabledChangedCheck();
	if (m_enabled == false) return;
	for (int i = 0; i < m_componentsCount; i++)
	{
		if (m_componentsVector[i]->IsEnabled())
			m_componentsVector[i]->OnUpdate();
	}
}

void EcsEntity::FixedUpdate()
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

void EcsEntity::Render()
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

void EcsEntity::DrawEditorProperties()
{
	if (IsDestroyed()) return;

	ImGui::Text("%s", m_name.c_str());
	ImGui::InputText("", m_tempName, 128);
	ImGui::SameLine();
	if (ImGui::Button("Rename"))
	{
		SetName((std::string)m_tempName);
	}
	std::string enableString = "ENABLE";
	if (m_enabled) enableString = "DISABLE";
	if (ImGui::Button(enableString.c_str()))
	{
		SetEnabled(!m_enabled);
	}
	if (!m_immortal)
	{
		ImGui::SameLine();
		if (ImGui::Button("DESTROY"))
		{
			Destroy();
		}
	}
	std::string currentParentName = "no parent";
	if (transform.GetParent() != NULL)
	{
		EcsEntity * parentEntity = m_ecsSystem->FindEntityByTransform(transform.GetParent());
		if (parentEntity != NULL)
		{
			currentParentName = parentEntity->GetName();
		}
	}
	if (m_name != "EditorCamera")
	{
		if (ImGui::BeginCombo("Parent", currentParentName.c_str()))
		{
			if (ImGui::Button("no parent"))
			{
				Unparent();
			}
			for (int i = 0; i < m_ecsSystem->EntityCount(); i++)
			{
				bool showEntity = true;
				EcsEntity * ent = &*m_ecsSystem->entities[i];
				if (ent->IsDestroyed() == true) showEntity = false;
				if (ent == this) showEntity = false;
				if (ent->transform.GetParent() == GetTransformRef()) showEntity = false;
				std::string entName = ent->GetName();
				if (entName == "EditorCamera") showEntity = false;;

				if (showEntity)
				{
					bool isSelected = (currentParentName == entName);
					if (ImGui::Button(entName.c_str()))
					{
						SetParent(ent);
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}
		if (transform.GetParent() != NULL)
		{
			ImGui::SameLine();
			if (ImGui::Button("Go To"))
			{
				Editor::GetEditor()->SelectEditorObject(m_ecsSystem->FindEntityByTransform(transform.GetParent()));
			}
		}
	}
	ImGui::Separator();
	ImGui::NewLine();
	transform.DrawEditorProperties();
	for (int i = 0; i < m_componentsCount; i++)
	{
		if (!m_componentsVector[i]->IsDetached())
		{
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();
			bool componentEnabled = m_componentsVector[i]->IsEnabled();
			ImGui::Checkbox(m_componentsVector[i]->GetComponentName().c_str(), &componentEnabled);
			m_componentsVector[i]->SetEnabled(componentEnabled);
			ImGui::SameLine();
			if (ImGui::Button(("Detach " + m_componentsVector[i]->GetComponentName()).c_str()))
			{
				m_componentsVector[i]->DetachFromEntity();
				Editor::GetEditor()->SaveScene();
				Editor::GetEditor()->ReloadScene();
			}
			ImGui::NewLine();
			m_componentsVector[i]->DrawEditorProperties();
		}
	}
	ImGui::NewLine();
	ImGui::Separator();
	std::vector<std::string> compNames = GetComponentNames();
	std::string newConponentName = compNames[0];
	if (ImGui::BeginCombo("Add Component", "Rotator"))
	{

		for (int i = 0; i < compNames.size(); i++)
		{

			if (ImGui::Button(compNames[i].c_str()))
			{

				AttachComponentWithName(this, compNames[i]);
			}
		}
		ImGui::EndCombo();
	}
}

void EcsEntity::SetEnabled(bool _enable)
{
	m_enabled = _enable;
	m_enabledPrev = _enable;
	for (int i = 0; i < m_componentsCount; i++)
	{
		if (_enable == true) m_componentsVector[i]->OnEnable();
		else m_componentsVector[i]->OnDisable();
	}
}

EcsEntity & EcsSystem::NewEntity(std::string _entityName)
{
	return NewEntity(_entityName, NULL);
}

EcsEntity & EcsSystem::NewEntity(std::string _entityName, Transform * _parent)
{
	if (_entityName == "") _entityName = "New Entity";
	EcsEntity * newEntity = new EcsEntity(this, _entityName, _parent);
	std::unique_ptr<EcsEntity>entPtr{ newEntity };
	entities.emplace_back(std::move(entPtr));
	m_newestEntity = newEntity;
	m_totalEntityCount++;
	m_liveEntityCount++;
	return *newEntity;
}

EcsEntity * EcsSystem::FindEntity(std::string _entityName)
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

EcsEntity * EcsSystem::FindEntityByTransform(Transform * _transform)
{
	if (_transform != NULL)
	{
		for (int i = 0; i < m_totalEntityCount; i++)
		{
			if (entities[i]->IsDestroyed() == false)
			{
				if (&entities[i]->transform == _transform)
				{
					return &*entities[i];
				}
			}
		}
	}
	return NULL;
}

void EcsSystem::Refresh()
{
	entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<EcsEntity> &ent) {return ent->IsDestroyed(); }), std::end(entities));
	m_totalEntityCount = entities.size();
	m_liveEntityCount = m_totalEntityCount;
	m_destroyedEntityCount = 0;
}

std::string EcsSystem::Serialize()
{
	std::string serialized = "";
	int capacity = 0;
	int attempts = 0;
	std::vector<EcsEntity*> serializedEntities;

	{
		for (int i = 0; i < EntityCount(); i++)
		{
			EcsEntity * ent = &*entities[i];
			if (ent->IsDestroyed() == false)
			{
				if (ent->Serializable())
				{
					capacity++;
				}
			}
		}
	}

	while (serializedEntities.size() < capacity && attempts < capacity * 2)
	{
		for (int i = 0; i < EntityCount(); i++)
		{
			EcsEntity * ent = &*entities[i];
			if (ent->IsDestroyed() == false)
			{
				if (ent->Serializable())
				{
					bool allreadySerialized = false;
					for (int k = 0; k < serializedEntities.size(); k++)
					{
						if (serializedEntities[k] == ent)
						{
							allreadySerialized = true;

							break;
						}

					}
					if (allreadySerialized == false)
					{

						bool parentAllreadySerialized = false;
						Transform * parentTransform = ent->transform.GetParent();
						if (parentTransform != NULL)
						{
							EcsEntity * parent = FindEntityByTransform(ent->transform.GetParent());

							for (int j = 0; j < serializedEntities.size(); j++)
							{
								if (serializedEntities[j] == parent)
								{
									parentAllreadySerialized = true;
									break;
								}
							}
						}
						else
						{
							parentAllreadySerialized = true;
						}
						if (parentAllreadySerialized)
						{
							serialized += ent->Serialize();
							serialized += "\n";
							serializedEntities.push_back(ent);
						}
						else
						{
							attempts++;
						}
					}
				}
			}
		}
	}

	return serialized;
}

void EcsSystem::DeserializeEntity(std::vector<std::string> _serializedComponent)
{
	if (_serializedComponent.size() <= 12)
	{
		NewEntity(_serializedComponent[0]);
	}
	else if (_serializedComponent[12] == "NO_PARENT")
	{
		NewEntity(_serializedComponent[0]);
	}
	else
	{
		EcsEntity * foundParent = FindEntity(_serializedComponent[12]);
		if (foundParent != NULL)
		{
			NewEntity(_serializedComponent[0], &foundParent->transform);
		}
		else
		{
			NewEntity(_serializedComponent[0]);
		}
	}
	m_newestEntity->Deserialize(_serializedComponent);
}

void EcsComponent::DetachFromEntity()
{
	m_detached = true;
	Disable();
}
