#pragma once
namespace GatesEngine
{
#define COM_RELEASE(p){if((p))p->Release();(p)=nullptr;}
}
