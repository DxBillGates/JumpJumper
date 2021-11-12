#pragma once
#include "Component.h"

namespace GatesEngine
{
	class Behaviour : public Component
	{
	private:
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Behaviour();

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Behaviour();
	};
}
