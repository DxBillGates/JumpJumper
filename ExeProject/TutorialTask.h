#pragma once
class TutorialTask
{
private:
protected:
	bool preClearFlag;
	bool clearFlag;
	bool errorTaskFlag;
	int clearTaskCount;
protected:
	void ErrorTaskCheck();
	void IncrementClearTaskCount();
public:
	TutorialTask();
	virtual ~TutorialTask();
	virtual void Initialize();

	/// <summary>
	/// Updateึเษ^XNฎน๐ช๐Lq
	/// </summary>
	virtual void Update();

	bool GetClearFlag();
	bool IsClearTask();
};

