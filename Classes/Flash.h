#ifndef _FLASH_H_
#define _FLASH_H_

#include "Hero.h"

struct MonsterInfomation
{
	cocos2d::CCSprite* pAction;
	cocos2d::CCSprite* pLand;
	cocos2d::CCSprite* pBloodBoard;
	cocos2d::CCProgressTimer* pBloodBar;
	cocos2d::CCSprite* pFire;
	cs::Armature*	pSprite;
	cs::Armature*	pBlood;
	cs::Armature*	pSprite1;
	cs::Armature*	pSprite2;
	cs::Armature*	pSprite3;
	int				type;
	float			cx;
	float			cy;
	float			x;
	float			y;
	float			w;
	float			h;
	float			attackDistance;
	float			SpeedX;
	float			SpeedY;
	int				hp;
	int				hpMax;
	int				lastAction;
	int				toward;
	int				getback;
	int				bloodTime;
	bool			alive;
	bool			bHold;
	bool			canDeadOnec;
	bool			willRelife;
	bool			secondLife;
	bool			bRock;
	int				rockTime;
	int				deadTime;
	int				liveTime;
	int				attackTime;
	int				holdTime;
	int				CoolTime1;
	int				CoolTime2;
	int				CoolTime3;
	int				CoolTime4;
	int				CoolTime5;
	int				CoolTime6;
	int				CoolTime7;
	int				hp1;
	int				hp2;
	int				hp3;
	int				ActionType;
	int				EffectTime1;
	int				EffectID;
	bool			bPlant;
	int				bulletIndex;
	bool			callback1;
	bool			callback2;
	bool			callback3;
	bool			callback4;
	bool			firedead;
	int				mode;
	bool			canAttack;
	bool			bSprite1;
	bool			bSprite2;
	bool			bSprite3;

	MonsterInfomation()
	{
		pAction		= NULL;
		pLand		= NULL;
		pSprite		= NULL;
		pBlood		= NULL;
		pBloodBoard = NULL;
		pSprite1	= NULL;
		pSprite2	= NULL;
		pSprite3	= NULL;
		pFire		= NULL;
		getback		= 0;
		bloodTime	= 0;
		alive		= true;
		bHold		= true;
		canDeadOnec	= false;
		willRelife	= false;
		secondLife	= false;
		bPlant		= false;
		bRock		= false;
		firedead	= false;
		SpeedY		= 0.0f;
		rockTime	= 100;
		liveTime	= 20;
		deadTime	= 100;
		holdTime	= 0;
		lastAction	= 4;
		toward		= -1;
		CoolTime1	= 1;
		CoolTime2	= 0;
		CoolTime3	= 0;
		EffectTime1	= 0;
		EffectID	= -1;
		bulletIndex = -1;
		mode		= 0;
		canAttack	= true;
		bSprite1	= true;
		bSprite2	= true;
		bSprite3	= true;
	}
	
	~MonsterInfomation()
	{
		if ( pAction )
			pAction->removeFromParentAndCleanup(true);
		if ( pLand )
			pLand->removeFromParentAndCleanup(true);
		if ( pBlood )
			pBlood->removeFromParentAndCleanup(true);
		if ( pFire )
			pFire->removeFromParentAndCleanup(true);
	}

};

class CFlashControl : public cocos2d::CCLayer
{
public:
	CFlashControl();
	~CFlashControl();

	void Fire(int _type, float _toward);
	void Combat(int _type);

	bool DeleteHero();
	CHero* CreateHero(cocos2d::CCLayer* _pFather);
	void ReCreateHero();
	cs::Armature* CreateMonster(MonsterInfomation* _pMonsterInfo);
	//cs::Armature* CreateMonsterBlood(float _x, float _y);

	int				m_unNowWeapon;
	float			m_fFireToward;
	int				m_iHeroType;
private:
	int				m_unMoney;
	int				m_unMedal;
	CHero*			m_pHeroLayer;
	cs::Armature*	m_pFire;
};

#endif
