#pragma once
#include "GraphicsDevice.h"
#include <string>

namespace GatesEngine
{
	class Texture
	{
	private:
		GraphicsDevice* gDevice;
	protected:
		ID3D12Resource* texBuffer;
		int srvNumber;
	public:
		Texture();
		virtual ~Texture();
		void Load(const std::string& filename,GraphicsDevice* gDevice);
		virtual void Set(int descIndex);
	};
}