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