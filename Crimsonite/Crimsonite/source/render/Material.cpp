#include "Material.h"

#include "../asset/AssetManager.h"

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
	m_shader = AssetManager::Instance()->GetShader(_shader);
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
	SetTextureProperty(_propertyName, AssetManager::Instance()->GetTexture(_textureName));
}

void Material::SetMainTex(std::string _textureName)
{
	SetMainTex(AssetManager::Instance()->GetTexture(_textureName));
}

void Material::SetMainTex(Texture * _texture)
{
	SetTextureProperty("MainTex", _texture);
}

void Material::ReservePropertyName(std::string _propertyName)
{
	if (_propertyName == "") return;
	if (GetReservedPropertyIndex(_propertyName) == -1)
	{
		m_reservedPropertyNames.push_back(_propertyName);
	}
}

int Material::GetReservedPropertyIndex(std::string _propertyName)
{
	if (_propertyName == "") return -1;
	for (int i = 0; i < m_reservedPropertyNames.size(); i++)
	{
		if (m_reservedPropertyNames[i] == _propertyName)
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
