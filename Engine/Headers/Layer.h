#pragma once

// 그룹핑 된 객체들을 보관한다.
#include "GameObject.h"

BEGIN(Engine)

class CComponent;
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	CGameObject* Get_ObjectPointer(_uint iIndex);
	CComponent* Get_ComponentPointer(const _tchar* pComponentTag, _uint iIndex);
	list<CGameObject*>*	Get_ObjectList() { return &m_ObjectList; }
public:
	HRESULT Ready_Layer();
	HRESULT Add_Object(CGameObject* pGameObject);
	_int Update_Layer(_double TimeDelta);
	_int Late_Update_Layer(_double TimeDelta);
	HRESULT	Clear_DeadObject();
	_int Get_Size() { return m_ObjectList.size(); }

private:
	list<CGameObject*>			m_ObjectList;
	typedef list<CGameObject*>	OBJECTLIST;
public:
	static CLayer* Create();
	virtual void Free();
};

END