#include "ECS.h"
#include "Components.h"
#include "ComponentNames.h"

void EcsEntity::SetName(std::string _name)
{
	bool uniqueNameFound = false;
	int entityCounter = 1;
	std::string tempName = _name;

	while (uniqueNameFound == false)
	{
		if (EcsEntity * ent = m_system->FindEntity(tempName))
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
	m_system->AlertEntityDestruction();
	m_doDestroy = true;
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
		bool componentEnabled = m_componentsVector[i]->IsEnabled();
		ImGui::Checkbox((m_componentsVector[i]->GetComponentName()).c_str(), &componentEnabled);
		m_componentsVector[i]->SetEnabled(componentEnabled);
		ImGui::NewLine();
		m_componentsVector[i]->DrawEditorProperties();
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
	if (_entityName == "") _entityName = "New Entity";
	EcsEntity * newEntity = new EcsEntity(this, _entityName);
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
	for (int i = 0; i < EntityCount(); i++)
	{
		EcsEntity * ent = &*entities[i];
		if (ent->IsDestroyed() == false)
		{
			if (ent->Serializable())
				serialized += ent->Serialize();
			serialized += "\n";
		}
	}
	return serialized;
}

void EcsSystem::DeserializeEntity(std::vector<std::string> _serializedComponent)
{
	NewEntity(_serializedComponent[0]);
	m_newestEntity->Deserialize(_serializedComponent);
}

void EcsSystem::DeserializeComponent(std::vector<std::string> _serializedComponent)
{
}
