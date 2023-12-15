#pragma once


class SSRenderable
{
public:
	virtual void AddRenderCommand(std::vector<class SSRenderCmdBase*>& InList) = 0;
};