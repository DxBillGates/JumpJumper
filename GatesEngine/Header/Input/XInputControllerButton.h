#pragma once

namespace GatesEngine
{
	enum class XInputControllerButton
	{
		XINPUT_UP = 0x0001,     //0
		XINPUT_DOWN = 0x0002,	//1
		XINPUT_LEFT = 0x0004,	//2
		XINPUT_RIGHT = 0x0008,	//3
		XINPUT_START = 0x0010,	//4
		XINPUT_BACK = 0x0020,	//5
		XINPUT_LSTICK = 0x0040, //6
		XINPUT_RSTICK = 0x0080, //7
		XINPUT_LB = 0x0100,	    //8
		XINPUT_RB = 0x0200,	    //9
		XINPUT_A = 0x1000,		//10
		XINPUT_B = 0x2000,		//11
		XINPUT_X = 0x4000,		//12
		XINPUT_Y = 0x8000,		//13
		XINPUT_LT = -1,		    //14
		XINPUT_RT = -2,		    //15
	};
}
