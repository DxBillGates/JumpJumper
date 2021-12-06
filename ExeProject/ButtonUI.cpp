#include "ButtonUI.h"

const GatesEngine::Math::Vector2& ButtonUI::GetScale()
{
    return scale;
}

const GatesEngine::Math::Vector2& ButtonUI::GetPos()
{
    return pos;
}

bool ButtonUI::GetIsCenter()
{
    return isCenter;
}

bool ButtonUI::GetIsSelect()
{
    return isSelect;
}

void ButtonUI::SetScale(const Vector2& setscale)
{
    scale = setscale;
}

void ButtonUI::SetPos(const Vector2& setpos)
{
    pos = setpos;
}

void ButtonUI::SetIsCenterFlag(bool flag)
{
    isCenter = flag;
}

void ButtonUI::SetIsSelect(bool flag)
{
    isSelect = flag;
}
