#include "AssetManager.h"

AssetManager * AssetManager::m_instance(0);
AssetManager * AssetManager::Instance()
{
	if (!m_instance) m_instance = new AssetManager();
	return m_instance;
}

AssetManager::~AssetManager()
{
	m_instance = 0;
}