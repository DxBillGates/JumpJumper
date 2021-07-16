#pragma once

class Stage
{
protected:
	int needKillCount;
public:
	Stage();
	virtual ~Stage();
	int GetNeedKillCount();
};
