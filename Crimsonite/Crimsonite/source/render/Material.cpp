#include "Material.h"

#include "../asset/AssetManager.h"
#include "../external/imgui/imgui.h"

Material::Material()
{
	SetName("New Material");
}

Material::Material(std::string _name)
{
	SetName(_name);
}

void Material::UpdateShaderProperties()
{
	for (auto const&[key, value] : boolProperties)
	{
		m_shader->SetBool(key, value);
	}
	for (auto const&[key, value] : intProperties)
	{
		m_shader->SetInt(key, value);
	}
	for (auto const&[key, value] : floatProperties)
	{
		m_shader->SetFloat(key, value);
	}
	for (auto const&[key, value] : vec2Properties)
	{
		m_shader->SetVector2(key, value);
	}
	for (auto const&[key, value] : vec3Properties)
	{
		m_shader->SetVector3(key, value);
	}
	for (auto const&[key, value] : vec4Properties)
	{
		m_shader->SetVector4(key, value);
	}
	for (auto const&[key, value] : textureProperties)
	{
		m_shader->SetTexture(key, value);
	}

	m_shader->SetColor(m_color);

}

void Material::SetShader(std::string _shader)
{
	m_shader = AssetManager::GetShader(_shader);
}

void Material::SetColor(glm::vec3 _color)
{
	m_color.x = _color.x;
	m_color.y = _color.y;
	m_color.z = _color.z;
	m_color.w = 1.0f;
}

void Material::SetColor(glm::vec4 _color)
{
	m_color.x = _color.x;
	m_color.y = _color.y;
	m_color.z = _color.z;
	m_color.w = _color.w;
}

void Material::SetTextureProperty(std::string _propertyName, Texture * _texture)
{
	textureProperties[_propertyName] = _texture;
}

void Material::SetTextureProperty(std::string _propertyName, std::string _textureName)
{
	SetTextureProperty(_propertyName, AssetManager::GetTexture(_textureName));
}

void Material::SetMainTex(std::string _textureName)
{
	SetMainTex(AssetManager::GetTexture(_textureName));
}

void Material::SetMainTex(Texture * _texture)
{
	SetTextureProperty("MainTex", _texture);
}

void Material::ReserveTexturePropertyName(std::string _propertyName)
{
	if (_propertyName == "") return;
	if (GetReservedPropertyIndex(&m_reservedTexturePropertyNames, _propertyName) == -1)
	{
		m_reservedTexturePropertyNames.push_back(_propertyName);
	}
}

int Material::GetReservedPropertyIndex(std::vector<std::string> * _reservedPopertyArray, std::string _propertyName)
{
	if (_propertyName == "") return -1;
	for (int i = 0; i < m_reservedPropertyNames.size(); i++)
	{
		if (_reservedPopertyArray->at(i) == _propertyName)
		{
			return i;
		}
	}

	return -1;
}

std::string Material::GetReservedPropertyName(int _index)
{
	if (m_reservedPropertyNames.size() > _index)
	{
		return m_reservedPropertyNames[_index];
	}
	return "";
}

void Material::DrawEditorProperties()
{
	ImGui::Text("Name : '%s'", GetName().c_str());
	ImGui::Text("Shader : '%s'", GetShader()->GetName().c_str());
	float col[4];
	col[0] = GetColor().r;
	col[1] = GetColor().g;
	col[2] = GetColor().b;
	col[3] = GetColor().a;
	ImGui::ColorEdit4("Main Color", col);
	SetColor(glm::vec4{ col[0], col[1], col[2], col[3] });
	std::vector<std::string> texturePropertyNames = GetReservedTextureNames();
	for (int t = 0; t < texturePropertyNames.size(); t++)
	{
		std::string reservedTexName = texturePropertyNames.at(t);
		Texture * currentTex = textureProperties[reservedTexName];
		if (currentTex == NULL)
		{
			currentTex = AssetManager::GetTexture("error");
		}
		std::string currentTexName = currentTex->GetName();
		if (ImGui::BeginCombo(reservedTexName.c_str(), currentTexName.c_str()))
		{
			int r = 0;
			for (int i = 0; i < AssetManager::TextureCount(); i++)
			{
				std::string foundTexName = AssetManager::GetTexture(i)->GetName();
				bool isSelected = (currentTexName == foundTexName);
				if (r < 6)
				{
					ImGui::SameLine();
					r++;
				}
				else
				{
					r = 0;
				}
				if (ImGui::ImageButton((GLuint*)AssetManager::GetTexture(i)->TextureId, ImVec2(50.0f, 50.0f), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128)))
				{
					SetTextureProperty(reservedTexName, foundTexName);
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

}
