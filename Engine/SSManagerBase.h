#pragma once

class ENGINE_API SSManagerBase : public Singleton<SSManagerBase>
{
public:

	SSManagerBase();

	static void Initialize();

protected:
	static std::vector<SSManagerBase*> mManagerList;
};
