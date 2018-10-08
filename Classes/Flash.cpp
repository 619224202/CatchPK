#include "Flash.h"
#include "baseRes.h"
#include "CSArmatureDataManager.h"


extern CFlashControl* g_pHero;
using namespace cs;
using namespace cocos2d;

CFlashControl::CFlashControl()
{
	m_pHeroLayer	= NULL;
	m_unNowWeapon	= 0;
}

CFlashControl::~CFlashControl()
{
	//BatchNodeManager::sharedBatchNodeManager()->removeBatchNodes("HeroSprite");
	//SAFEDELETE(m_pHeroLayer);
}

CHero* CFlashControl::CreateHero(CCLayer* _pFather)
{
	m_pHeroLayer = new CHero;
	m_pHeroLayer->autorelease();
	m_pHeroLayer->m_pFather = _pFather;
	m_pHeroLayer->InitMonster();
	return m_pHeroLayer;
}

void CFlashControl::ReCreateHero()
{
	m_pHeroLayer->CreateHero();
}

Armature* CFlashControl::CreateMonster(MonsterInfomation* _pMonsterInfo)
{
	int type = _pMonsterInfo->type;
#ifdef GameTypeC
	if ( type == 8 )
		type = 5;
#endif
	Armature* pArmature = Armature::create(g_sMonsterName[type].c_str());
	pArmature->setPosition(_pMonsterInfo->x, _pMonsterInfo->y);
	m_pHeroLayer->SetMonster(pArmature);
	return pArmature;
}

//Armature* CFlashControl::CreateMonsterBlood(float _x, float _y)
//{
//	Armature* pArmature = Armature::create("xueji1dz");
//	pArmature->getAnimation()->playByIndex(0, 0, 50);
//	pArmature->setPosition(_x, _y);
//	m_pHeroLayer->SetMonsterBlood(pArmature);
//	return pArmature;
//}

void CFlashControl::Fire(int _type, float _toward)
{
	m_fFireToward = _toward;
	m_pHeroLayer->GetFire()->setScaleX(_toward);
	m_pHeroLayer->GetFire()->getAnimation()->playByIndex(_type,0,10);
	//for ( int i = 0; i < d_iRotateFireNum; i++ )
	//{
	//	m_pHeroLayer->GetFire()->getBone(d_sRotateFire[i].c_str())->setRotation(-m_pHeroLayer->m_fRotation);
	//}
}

void CFlashControl::Combat(int _type)
{
	m_pHeroLayer->m_unLastAction = _type;
	m_pHeroLayer->GetArmature()->getAnimation()->stop();

	int actionTime = 40;
	if ( _type == 26 || _type == 36 )
		actionTime = 30;
	m_pHeroLayer->GetArmature()->getAnimation()->playByIndex(_type-1, 0, actionTime);
}

bool CFlashControl::DeleteHero()
{
	if ( m_pHeroLayer )
	{
		//ArmatureDataManager::sharedArmatureDataManager()->removeAll();
		//m_pHeroLayer->unscheduleUpdate();
		//delete m_pHeroLayer;
		//m_pHeroLayer = NULL;
		return true;
	}
	return false;
}
