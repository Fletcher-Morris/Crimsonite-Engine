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

void Material::SetShader(std::string _shader)
{
	m_shader = AssetManager::Instance()->GetShader(_shader);
}

void Material::SetColor(glm::vec3 _color)
{
}
