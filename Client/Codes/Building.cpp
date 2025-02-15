#include "stdafx.h"
#include "..\Headers\Building.h"
#include "Management.h"
#include "PickingMgr.h"
#include "StageUIMgr.h"
#include "FontManager.h"

CBuilding::CBuilding(PDIRECT3DDEVICE9 pGraphic_Device)
	: CInteractable(pGraphic_Device)
{

}

CBuilding::CBuilding(const CBuilding & rhs)
	: CInteractable(rhs)
{

}

HRESULT CBuilding::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CBuilding::Ready_GameObject(void * pArg)
{

	if (FAILED(Add_Component()))
		return E_FAIL;


	return S_OK;
}

_int CBuilding::Update_GameObject(_double TimeDelta)
{
	
	return _int();
}

_int CBuilding::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;

	return _int();
}

HRESULT CBuilding::Render_GameObject()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(PASS_SKYBOX);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

	return S_OK;
}

void CBuilding::Get_Damage(_int iDmg)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement) return;
	m_tagStat.iHp -= iDmg * pManagement->Get_TimeDelta(L"Timer_60");
	if (m_tagStat.iHp <= 0)
	{
		m_bDead = 1;
		OnDead();
	}
}

void CBuilding::Interact()
{
	CStageUIMgr::Get_Instance()->Set_StateToPanel_Building(m_tagStat);
}

void CBuilding::Display_Stats()
{
	_float fOffX = 0.f;
	_float fOffY = 10.f;
	_tchar szChar[128];

	wsprintf(szChar, L"HP \t :%d / %d", m_tagStat.iHp,_int(m_tagStat.iMaxHP.GetValue()));
	CFontManager::Get_Instance()->Draw_Font(_float3(g_iWinSizeX - 200.f + fOffX, 100.f + fOffY, 0.f), _float3(1.2f, 1.2f, 1.2f), szChar, D3DXCOLOR(0.25f, 0.f, 0.f, 1.f), 700);

	wsprintf(szChar, L"Resell \t :%d", _int(m_tagStat.iPrice * 0.5f));
	CFontManager::Get_Instance()->Draw_Font(_float3(g_iWinSizeX - 200.f + fOffX, 100.f + fOffY + 25.f, 0.f), _float3(1.2f, 1.2f, 1.2f), szChar, D3DXCOLOR(0.25f, 0.f, 0.f, 1.f), 700);
}

void CBuilding::Upgrade()
{
}

_bool CBuilding::Picking(POINT _ViewPortPoint, _float3 * _vHitPos)
{
	return m_pVIBufferCom->Picking(_ViewPortPoint, g_hWnd, m_pTransformCom->Get_WorldMatrix(), _vHitPos);
}



HRESULT CBuilding::Add_Component()
{
	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_SkyBox", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Cube", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Cube", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	m_pTransformCom->SetUp_Scale(_float3(5.f, 5.f, 5.f));
	return S_OK;
}

HRESULT CBuilding::SetUp_ConstantTable()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));


	if (FAILED(m_pTextureCom->Set_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;



	Safe_Release(pManagement);

	return S_OK;
}

void CBuilding::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CInteractable::Free();
}

