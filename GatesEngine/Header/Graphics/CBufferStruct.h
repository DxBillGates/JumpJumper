#pragma once
#include "..\..\Header\Math\Math.h"
namespace GatesEngine
{
	struct B0
	{
		Math::Matrix4x4 modelMatrix;
	};

	struct B1
	{
		Math::Vector4 ambient;
		Math::Vector4 diffuse;
		Math::Vector4 specular;
	};

	struct B2
	{
		Math::Matrix4x4 viewMatrix;
		Math::Matrix4x4 projMatrix;
		Math::Vector4 cameraPos;
		Math::Matrix4x4 billboard;
	};

	struct B3
	{
		Math::Vector4 worldLightDir;
		Math::Vector4 lightColor;
	};
}
