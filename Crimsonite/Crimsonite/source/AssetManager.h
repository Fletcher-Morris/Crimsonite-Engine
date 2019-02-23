#pragma once

namespace Crimsonite
{
	class AssetManager
	{
	public:
		//	For accessing the singleton instance of the asseet manager class.
		static AssetManager* Instance();
		//	Virtual destructor for the aset manager class.
		virtual ~AssetManager();
	};
}