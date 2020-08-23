#pragma once


class SSRenderingObject
{
public:
	SSRenderingObject(class SSGameObject* pGameObject);

protected:
	SSGameObject* mpGameObject = nullptr;
};