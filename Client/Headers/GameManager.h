#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLevel;
class CCube;
class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager)
private:
	explicit CGameManager();
	virtual ~CGameManager() = default;

public:
	void	SetWaveStart(_bool IsStart) { m_bGameStart = IsStart; if (IsStart) { OnGameStart();} else { OnGameEnd(); ++m_iWave;} }

public:
	_int	Update();
public:
	_bool	IsGameStart() { return m_bGameStart; }
	

	HRESULT OnGameStart();
	HRESULT OnGameEnd();

	_int	Get_Gold() { return m_iGold; }
	_int	Get_Wave() { return m_iWave; }
	void	Add_Gold(_int amount) { m_iGold += amount; }
	void	Sub_Gold(_int amount) { m_iGold -= amount; }
private:
	_int		m_iGold = 0;
	_bool		m_bGameStart = false;
	_int		m_iWave = 0;
public:
	virtual void Free() override;
};

END