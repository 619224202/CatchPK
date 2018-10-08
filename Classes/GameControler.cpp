#include "GameControler.h"
#include "baseRes.h"
#include "MonsterInfo.h"
#include "AppDelegate.h"
#include "MainMenu.h"
#include "ControlerMenu.h"
#include "PauseMenu.h"
#include "GameoverMenu.h"
#include "GameoverBtnMenu.h"
#include "GameBossLoading.h"
#include "GameLoading.h"
#include "Common.h"
#include "GameStartMenu.h"
#include "GameSkillLayer.h"
#include "SimpleAudioEngine.h"
#include "GameStart.h"
#include "AchieveAdd.h"
#include "BossBox.h"
#include "GetItemLayer.h"
#include "ChoseHero.h"
#include "ShopMenu.h"
#include "PayService.h"
#include "FirstLogin.h"

extern CFlashControl* g_pHero;
using namespace cocos2d;
using namespace cocos2d::extension;

#define MAINFRAME		60
#define PIXELPERMETER	20
#define BOSS_WIN_TIME	450
#define BOSS3_NEED_DAMAGE	5000
#define BOSS4_NEED_DAMAGE	15000
#define BOSS5_NEED_DAMAGE	11000
#define GRENADE_DAMAGE	500

#define d_LocateLimit	73, 28

#define d_eWT_FireWeapon		8
#define d_eWT_FireW				173
#define d_eWT_FireH				165
#define d_eWT_FireDamage		100
#define d_eWT_FireCoolTime		14
#define d_eWT_FireFixX			225
#define d_eWT_FireFixY			292

#define d_TagWeaponUse			200
#define d_TagHealBoard			219
#define d_TagHealBoxHave		220
#define d_TagHealBoxUnhave		225
#define d_iShowLocateSkillTag	198

#define d_fHealEffectToward		33.0f
#define d_LocateWeaponUse		71, 415
#define d_LocateHealX			665
#define d_LocateHealY			447
#define d_LocateHealBoard		665, 447
#define d_LocateHeartbeat		93, 35
#define d_sizeHeartbeat			156, 43
#define d_LocateHealBox(i)		593+i*52, 447
#define d_iGrenadeWH1			238, 159
#define d_iGrenadeWH2			81, 99
#define d_iEggWH				29, 35
#define d_iEggBomgWH			81, 99
#define d_fGrenadeBombYFix		60.0
#define d_iHealEffectWH			226, 235
#define d_fHealEffectFixY		20
#define d_fShowLocateMoney		219, 441
#define d_fShowLocateMinute		214, 396
#define d_fShowLocateSecond		243, 396
#define d_fShowLocateGrenade	60, 327
#define d_fShowLocateBullet		197, 374
#define d_fShowLocateHP			174, 410

#define d_sizeThunder1			182, 480
#define d_sizeThunder2			314, 480
#define d_sizeThunder3			900, 510
#define d_sizeThunder4			105, 115
#define d_sizePlant1			314, 480
#define d_sizePlant2			450, 450
#define d_sizePlant3			226, 224
#define d_sizeGetItem			66, 113
#define d_sizeRock				117, 98
#define d_sizeRockAim			121, 56
#define d_sizeRock2				470, 151
#define d_sizeHead				71, 69
#define d_locationHead			35.5, 34.5

#define d_locationBoss2Head1	-53, 62
#define d_locationBoss2Head2	-10, 66
#define d_locationBoss2Head3	20, 66

#define lenght_PoisonGasW		30
#define lenght_PoisonGasH		270

#define Height_JUMP		240
#define Time_DropsDist	150
#define Time_DropsStay	100
#define Time_PoisonGas	150

//#define CHANGE_CONTRAL

static int WeaponPicIndex[] =
{
	3,
	2,
	7,
};

static int StayTime[] =
{
	105, 105, 120,
};

static int VIPLocationY[] =
{
	100,
	0,
	90,
	100,
};

static int keynum[] =
{
	5, 7, 7,
};

static int guideMap[] =
{
	0, 900, 1000, 1200, 1500, 1800, 2500, 3000, 4000, 4800,
};

static int itemPercent[] =
{
	5, 5, 35, 50, 55, 60, 65, 85, 95, 100,
};

static int moneyNum[] =
{
	5, 5, 10, 10, 10, 10, 15, 15, 15, 15,
	20, 20,
};

float rotation = 0;

CGameControler* CGameControler::s_pGameControler = NULL;

CGameControler::CGameControler()
{
	m_bShakeStop	= false;
	m_bExitGame		= false;
	m_bFirstDown	= false;
	m_bGuideWeaponGet = false;
	m_bGuidePause	= false;
	m_bSellOpen		= false;
	m_bBuyMedal		= false;
	m_bPerfect		= true;
	m_bUnFire		= true;
	m_pScene		= NULL;
	m_pHero			= NULL;
	m_pCBackgroud	= NULL;
	m_pCFrontgroud	= NULL;
	m_pHealEffect1	= NULL;
	m_pHealEffect2	= NULL;
	m_pHealParticle	= NULL;
	m_pGetItemEffect= NULL;
	m_pHeadShake	= NULL;
	m_pWinMark		= NULL;
	m_pFireSprite	= NULL;
	m_pSpriteBuffer1= NULL;
	m_pSpriteBuffer2= NULL;
	m_pSpriteBuffer3= NULL;
	m_pFireGas		= NULL;
	m_pBoss6Skill	= NULL;
	m_pParitcleInPrison = NULL;
	m_pSkill1Particle = NULL;
	m_bAimMonster	= false;
	m_bBombedMovie	= false;
	m_bSkill		= false;
	m_bOver			= false;
	m_bOverMove		= false;
	m_bDead			= false;
	m_bBossView		= false;
	m_bPause		= false;
	m_bBoss			= false;
	m_bBossPause	= false;
	m_bWin			= false;
	m_bBossWin		= false;
	m_bBossBox		= false;
	m_bStartMovie	= false;
	m_bSkillPause	= false;
	m_bDamaged		= false;
	m_bCreateMonster= 0;
	m_bMonsterCry	= false;
	m_bComboShow	= false;
	m_bLimitStart	= false;
	m_bCanPause		= false;
	m_bEndPress		= true;
	m_bWeaponNeedChange	= false;
	m_iMapChanged	= false;
	bInChangeWeapon = false;
	m_bGuideBuyVip = false;
	m_bGuideBuyVipIndex = 0;
	m_fJumpSpeedInit= 14.1f;
	m_iJumpCanTimes	= 2;
	m_iHoldTime		= 0;
	m_iYDist		= 0;
	m_iYDist2		= 0;
	m_iGameMinute	= 0;
	m_iGameSecond	= 0;
	m_iSkill		= 0;
	m_iLevelType	= 0;
	m_iFireTime		= 0;
	m_iGrenadeTime	= 0;
	m_iBulletTime	= 0;
	m_iWave			= 0;
	m_iWaveThis		= 0;
	m_iCombo		= 0;
	m_iComboTime	= 0;
	m_iActionIndex	= 0;
	m_iNeedKill		= 0;
	m_iGuideMap		= 0;
	m_iGuideBtn		= 0;
	m_iGuideNow		= 0;
	m_iGuideAction	= 1;
	m_iGuideTalk	= 1;
	m_iGuideTime	= 0;
	m_iWaveDistTime	= 100;
	m_iBulletIndex	= 0;
	m_iLimitDigit	= 0;
	m_iAllStar		= 0;
	m_iComboMax		= 0;
	m_iVIPHp		= 100;
	m_unMoveEventID	= 0;
	m_iMapIndex		= 0;
	m_iVIPLastAction= -1;
	m_iBossLimitX	= -1;
	m_iPrisonTime	= -1;
	m_iFrozeTime	= -1;
	m_iBloodLessTime	= 0;
	m_iVIPAttackedTime	= 0;
	m_iJumpActionType	= 0;
	m_iSkillFullTime[0] = -1;
	m_iSkillFullTime[1] = -1;
	m_timeBeAttackedEffect = 0;
	m_iKeyNum		= 0;
	m_pKeyTip		= NULL;

	m_iStateHP		= 100;
	m_iStateNumKill = 0;
	m_iStateNumMedal= 0;
	m_iStateNumGold	= 0;
	m_iMoneyTo		= 0;
	m_iStateGold	= AppDelegate::s_Money;
	m_iStateSkill[0]= AppDelegate::s_SkillEnergy[0];
	m_iStateSkill[1]= AppDelegate::s_SkillEnergy[1];
	m_iGrenadeNum	= AppDelegate::s_GrenadeNum;
	m_iHealBox		= AppDelegate::s_Healbox;
	m_iSkillCanUse	= AppDelegate::m_Sell2;
	m_iStateBullet	= new int[d_iWeaponNum];
	for ( int i = 0; i < d_iWeaponNum; i++ )
	{
		m_iStateBullet[i] = AppDelegate::s_BulletNum[i];
	}

	m_fHeroLocateY	= 340.0;
	m_fHeroToward	= 1.0;
	m_fHeroSpeedX	= 0.0;
	m_fHeroSpeedY	= 0.0;
	m_iHeroJumps	= 0;
	m_iMoveDigit	= 0;
	m_iShakeTime	= 0;
	m_iShakeLocate	= 0;

	m_ewtNowWeapon	= eWT_Pistol;
	g_pHero->m_unNowWeapon = m_ewtNowWeapon;
	m_bAttack		= false;
	m_iRamp			= 0;
	m_iAttackDigit	= 0;
	m_iAttackCoolTime = 0;

	m_iStar			= 0;

	m_pGrenade		= NULL;
	m_pArrayThunder = NULL;
	m_pRelife		= NULL;
	m_iPlantTime	= 0;
	m_pArrayBlood	= new cocos2d::CCArray;

	isInRelife = false;
	relifeIndex = 0;
	isInRecharge = false;
	rechargeIndex = 0;
	isBuji = false;

	m_pMoveActionArray = new int[d_iActionNum];
	memset(m_pMoveActionArray, 0, sizeof(int)*d_iActionNum );

	if ( AppDelegate::s_HeroType == 0 )
		m_fHeroSpeedXRes= 4.3f;
	else if ( AppDelegate::s_HeroType == 1 )
		m_fHeroSpeedXRes= 4.3f;
	else
		m_fHeroSpeedXRes= d_fHeroSpeedX;
}

CGameControler::~CGameControler()
{
	s_pGameControler = NULL;
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	SAFEDELETEARRAY(m_pMoveActionArray);
	SAFEDELETEARRAY(m_iStateBullet);
	m_pArrayBlood->removeAllObjects();
	delete m_pArrayBlood;
	if ( m_pArrayThunder )
	{
		m_pArrayThunder->removeAllObjects();
		m_pArrayThunder->release();
	}
	VECTORCLEAN(m_vecAttackingObjectArray, AttackingObject);
	VECTORCLEAN(m_vecShellsArray, Shells);
	VECTORCLEAN(m_vecMonsterArray, MonsterInfomation);
	VECTORCLEAN(m_vecItemObejctArray, ItemObject);
	VECTORCLEAN(m_vecChopperArray, Chopper);
	VECTORCLEAN(m_vecBoxDownArray, BoxDown);
	for ( int i = 50; i < 54; i++ )
	{
		removeChildByTag(i);
	}
}

void CGameControler::keyBackClicked()
{
	if(isInRecharge){
		GetRemove(this);
		return;
	}
	if(isBuji){
		Sell2Back(this);
		return;
	}
	if(m_bGuideBuyVip){
		GuideBack();
		return;
	}
	if(isInRelife){
		Sell3Back(this);
		return;
	}
	if ( m_bPause || m_iSkill || m_bWin || m_bDead || m_bLimitStart || m_bExitGame || m_bSellOpen || m_bBuyMedal 
		|| getChildByTag(197) )
		return;
	if ( m_iLevelType == 6 )
		OnExitGame();
	else
		PauseGame();
	//AppDelegate::SaveAll();
	//CCDirector::sharedDirector()->end();
	//exit(0);
}

CGameControler* CGameControler::NewGameControler(cocos2d::CCScene* _pScene, int _unLevel, int _difficult)
{
	//CCScene* pScene = CCScene::create();

	CGameControler* pLayer = new CGameControler();
	s_pGameControler = pLayer;
	pLayer->init();
	pLayer->autorelease();
	pLayer->m_pScene	= _pScene;
	pLayer->m_unLevel	= _unLevel;
	pLayer->m_iDifficult= _difficult;

	SetScale(pLayer);
	_pScene->addChild(pLayer, 1, 1);
	if ( pLayer->m_unLevel > 54 )
	{
		pLayer->m_bBoss;
		pLayer->m_iLevelType = 7;
	}
	else if ( pLayer->m_unLevel < 0 )
		pLayer->m_iLevelType = 6;
	else if ( pLayer->m_unLevel > 48 )
		pLayer->m_iLevelType = 4;
	else if ( pLayer->m_unLevel == 7 || pLayer->m_unLevel == 13 )
		pLayer->m_iLevelType = 1;
	else if ( pLayer->m_unLevel == 2 || pLayer->m_unLevel == 15 || pLayer->m_unLevel == 19 )
		pLayer->m_iLevelType = 2;
	else if ( pLayer->m_unLevel == 9 || pLayer->m_unLevel == 21 )
		pLayer->m_iLevelType = 3;
	else if ( pLayer->m_unLevel%6 == 5 || pLayer->m_unLevel == 30 )
		pLayer->m_iLevelType = 5;
	else
		pLayer->m_iLevelType = 0; 	

	pLayer->m_pCFrontgroud = CFrontground::Create(_unLevel);
	pLayer->m_iMapNum = pLayer->m_pCFrontgroud->GetMapNums();

	if ( pLayer->m_iLevelType == 6 )
	{
		pLayer->m_pBG = CCSprite::create("Tile/beijing2.png");
		pLayer->m_pBG->setAnchorPoint(ccp(0.5f,0));
		pLayer->m_pBG->setPosition(ccp(400, 0));
		CCLayer* pBGLayer = new CCLayer();
		pBGLayer->autorelease();
		pBGLayer->addChild(pLayer->m_pBG);
		SetScale(pBGLayer);
		_pScene->addChild(pBGLayer,0);
		pLayer->m_ewtNowWeapon	= eWT_Knife;
		g_pHero->m_unNowWeapon = pLayer->m_ewtNowWeapon;
	}
	else
	{
		pLayer->m_pBG = CCSprite::create(g_sBGPath[_unLevel].c_str());
		pLayer->m_pBG->setAnchorPoint(ccp(0.5f,0));
		pLayer->m_pBG->setPosition(ccp(400, 0));
		CCLayer* pBGLayer = new CCLayer();
		pBGLayer->autorelease();
		pBGLayer->addChild(pLayer->m_pBG);
		SetScale(pBGLayer);
		_pScene->addChild(pBGLayer,0);
		pLayer->m_iGuideMap = pLayer->m_iMapNum*d_fMapSizeX;
	}

	pLayer->m_pCBackgroud = CBackground::Create(_unLevel);
	pLayer->m_pCBackgroud->setPosition(0, 0);
	SetScale(pLayer->m_pCBackgroud);
	_pScene->addChild(pLayer->m_pCBackgroud);

	pLayer->m_pCFrontgroud->setPosition(0, 0);
	SetScale(pLayer->m_pCFrontgroud);
	_pScene->addChild(pLayer->m_pCFrontgroud,0);

	if ( pLayer->m_iLevelType == 6 )
	{
		pLayer->m_pCMidground = CCTMXTiledMap::create("Tile/yindaojg.tmx");
		pLayer->m_pCMidground->setPosition( ccp(0, 0) );
		//SetScale(pLayer->m_pCMidground);
		pLayer->m_pCFrontgroud->addChild(pLayer->m_pCMidground, -1);
		pLayer->m_iMapMax = 1;
	}
	else
	{
#ifdef Old_Map
		pLayer->m_pCMidground = CCTMXTiledMap::create(g_sNGPath[_unLevel].c_str());
#else
		char buffer[255];
		sprintf(buffer, "Tile/%d_0jg.tmx", _unLevel);
		pLayer->m_pCMidground = CCTMXTiledMap::create(buffer);
#endif
		pLayer->m_pCMidground->setPosition(ccp(0, 0));
		//SetScale(pLayer->m_pCMidground);
		pLayer->m_pCFrontgroud->addChild(pLayer->m_pCMidground, -1);
		pLayer->m_iMapMax = g_unMapNum[_unLevel];
	}

	pLayer->m_pHero = g_pHero->CreateHero(pLayer);
	pLayer->m_pHero->setPosition(ccp(0.0f,0.0f));
	pLayer->m_pHero->GetArmature()->setPosition(pLayer->m_fHeroLocateX, d_fHeroCenterY);
	pLayer->m_pCFrontgroud->addChild(pLayer->m_pHero, 4);

	if ( pLayer->m_iLevelType == 2 )
	{//保护关
		CCSprite* pSprite = common::CreateAnimation("Animation/vip/1.png", ccp(pLayer->m_iMapNum/2.0f*800.0f+3, VIPLocationY[pLayer->m_unLevel/6]+2), 
			ccp(0.5f, 0.5f), "Animation/vip/", 6, CCRect(0,0,166,59), true);
		pLayer->m_pCFrontgroud->GetTileMap()->addChild(pSprite, 2);

		pLayer->m_pVIP = pLayer->m_pHero->CreateVIP();
		pLayer->m_pVIP->setPosition( ccp(pLayer->m_iMapNum/2.0f*800.0f, VIPLocationY[pLayer->m_unLevel/6]) );
		pLayer->m_pCFrontgroud->GetTileMap()->addChild(pLayer->m_pVIP, 2);

		//血条
		pLayer->m_pVipHPBoard = CCSprite::create("Monster/xue1.png");
		pLayer->m_pVipHPBoard->setPosition( ccp(pLayer->m_iMapNum/2.0f*800.0f,VIPLocationY[pLayer->m_unLevel/6]+100) );
		pLayer->m_pVipHPBoard->setAnchorPoint( ccp(0.5f, 0.5f) );
		pLayer->m_pVipHPBoard->setVisible(false);
		pLayer->m_pCFrontgroud->GetTileMap()->addChild(pLayer->m_pVipHPBoard, 2);

		CCSprite* pBar = CCSprite::create("Monster/xue2.png");
		pLayer->m_pHpBar = CCProgressTimer::create(pBar);
		pLayer->m_pHpBar->setPosition( ccp(0,0) );
		pLayer->m_pHpBar->setAnchorPoint( ccp(0.0f, 0.0f) );
		pLayer->m_pHpBar->setType(kCCProgressTimerTypeBar);//
		pLayer->m_pHpBar->setMidpoint(ccp(0, 0.5));
		pLayer->m_pHpBar->setBarChangeRate(ccp(1, 0));
		pLayer->m_pHpBar->setPercentage(100); 
		//pLayer->m_pHpBar->setVisible(false);
		pLayer->m_pVipHPBoard->addChild(pLayer->m_pHpBar);
		pLayer->m_pVipHPBoard->setVisible(false);

		pLayer->m_pEnemyTipL = CCSprite::create("Role/enemytip.png");
		pLayer->m_pEnemyTipL->setPositionX(50);
		pLayer->m_pEnemyTipL->setVisible(false);
		pLayer->m_pEnemyTipL->setScaleX(-1.0f);
		pLayer->addChild(pLayer->m_pEnemyTipL, 4);

		pLayer->m_pEnemyTipR = CCSprite::create("Role/enemytip.png");
		pLayer->m_pEnemyTipR->setPositionX(750);
		pLayer->m_pEnemyTipR->setVisible(false);
		pLayer->addChild(pLayer->m_pEnemyTipR, 4);

		pLayer->m_pVipTip1 = CCSprite::create("Role/viptip1.png");
		pLayer->m_pVipTip1->setPositionY(300);
		pLayer->m_pVipTip1->setVisible(false);
		pLayer->addChild(pLayer->m_pVipTip1, 4);
		CCFadeIn* pAction1 = CCFadeIn::create(0.1f);
		CCFadeOut* pAction2 = CCFadeOut::create(0.1f);
		pLayer->m_pVipTip1->runAction(CCRepeatForever::create(CCSequence::create(pAction1, pAction2, NULL)));

		pLayer->m_pVipTip2 = CCSprite::create("Role/viptip2.png");
		pLayer->m_pVipTip2->setPositionY(300);
		pLayer->m_pVipTip2->setVisible(false);
		pLayer->addChild(pLayer->m_pVipTip2, 4);
		pAction1 = CCFadeIn::create(0.1f);
		pAction2 = CCFadeOut::create(0.1f);
		pLayer->m_pVipTip2->runAction(CCRepeatForever::create(CCSequence::create(pAction1, pAction2, NULL)));
	}

	//武器初始化
	pLayer->m_iWeaponUse[0] = AppDelegate::s_WeaponUse[0];
	pLayer->m_iWeaponUse[1] = AppDelegate::s_WeaponUse[1];
	if ( AppDelegate::s_HeroType != 2 ) 
	{
		pLayer->m_iBulletW[0] = 135;
		pLayer->m_iBulletH[0] = 60;
	}
	else
	{
		pLayer->m_iJumpCanTimes = 3;
		pLayer->m_fJumpSpeedInit= 11.8f;
		pLayer->m_iBulletW[0] = 300;
		pLayer->m_iBulletH[0] = 30;
	}
	if ( AppDelegate::s_WeaponUse[0] == 2 )
		pLayer->m_sCutSound = "MS/Sound/EfCut.mp3";
	else if ( AppDelegate::s_WeaponUse[0] == 6 )
		pLayer->m_sCutSound = "MS4/5Cut.mp3";
	else if ( AppDelegate::s_WeaponUse[0] == 13 )
		pLayer->m_sCutSound = "MS4/8Cut.mp3";
	if ( AppDelegate::s_WeaponUse[1] == 12 )
		pLayer->m_iFireTime = 1;
	for ( int i = 0; i < 2; i++ )
	{
		pLayer->m_iWeaponDamage[i] = AppDelegate::WeaponDamage(AppDelegate::s_WeaponUse[i]);
		pLayer->m_iWeaponSpeed[i] = AppDelegate::WeaponSpeed(AppDelegate::s_WeaponUse[i]);
	}

	if ( 6 == pLayer->m_iLevelType )
		pLayer->m_iWeaponDamage[0] *= 2;

	//if ( AppDelegate::s_HeroType == 2 )
	//	pLayer->m_iWeaponSpeed[1] = 50;
	for ( int i = 0; i < 1; i++ )
	{
		switch ( AppDelegate::s_WeaponUse[i+1] )
		{
		case 3:
			pLayer->m_iFireIndex[i] = 6;
			break;
		case 4:
			pLayer->m_iFireIndex[i] = 5;
			break;
		case 5:
			pLayer->m_iFireIndex[i] = 4;
			break;
		case 7:
			pLayer->m_iFireIndex[i] = 0;
			break;
		case 8:
			pLayer->m_iFireIndex[i] = 2;
			break;
		case 9:
			pLayer->m_iFireIndex[i] = 1;
			break;
		case 10:
			pLayer->m_iFireIndex[i] = 3;
			break;
		case 11:
			pLayer->m_iFireIndex[i] = 7;
			break;
		case 12:
			pLayer->m_iFireIndex[i] = -1;
			pLayer->m_iBulletW[i+1] = 300;
			pLayer->m_iBulletH[i+1] = 130;
			break;
		case 16:
		case 17:
			pLayer->m_iFireIndex[i] = 8;
			break;
		}
		if ( pLayer->m_iFireIndex[i] != -1 )
		{
			pLayer->m_iBulletW[i+1] = 10;
			pLayer->m_iBulletH[i+1] = 60;
		}
		pLayer->m_bStrike[i]	= false;
		pLayer->m_bShells[i]	= false;
		pLayer->m_iBulletNum[i] = AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[i+1]];
		if ( AppDelegate::s_HeroType == 0 )
		{
			pLayer->m_iFireX[i] = g_fFireX[AppDelegate::s_WeaponUse[i+1]];
			pLayer->m_iFireY[i] = g_fFireY[AppDelegate::s_WeaponUse[i+1]];
		}
		else if ( AppDelegate::s_HeroType == 1 )
		{
			pLayer->m_iFireX[i] = g_fFireX2[AppDelegate::s_WeaponUse[i+1]];
			pLayer->m_iFireY[i] = g_fFireY2[AppDelegate::s_WeaponUse[i+1]];
		}
		if ( AppDelegate::s_HeroType == 2 )
		{
			pLayer->m_iFireX[i] = g_fFireX3;
			pLayer->m_iFireY[i] = g_fFireY3;
		}

		if ( AppDelegate::s_HeroType == 2 )
		{
			pLayer->m_iActionType1[i] = 12;
			pLayer->m_iActionType2[i] = 6;
		}
		else switch ( AppDelegate::s_WeaponUse[i+1] )
		{
		case 3:
		case 8:
		case 16:
		case 17:
			pLayer->m_iActionType1[i] = 17;
			pLayer->m_iActionType2[i] = 6;
			break;
		case 4:
		case 5:
		case 7:
		case 10:
			pLayer->m_iActionType1[i] = 18;
			pLayer->m_iActionType2[i] = 20;
			break;
		case 9:
		case 11:
		case 12://火枪
			pLayer->m_iActionType1[i] = 19;
			pLayer->m_iActionType2[i] = 21;
			break;
		}
		//pLayer->m_ptBullet[i] = NULL;
		pLayer->m_sBulletPath = "";
		if ( AppDelegate::s_WeaponUse[i+1] != 8 && AppDelegate::s_WeaponUse[i+1] != 12 )
		{
			char bulletName[255];
			sprintf(bulletName, "bullet/%d/3.png", AppDelegate::s_WeaponUse[i+1]/*, AppDelegate::s_WeaponLevel[AppDelegate::s_WeaponUse[i+1]]+1*/);
			//pLayer->m_ptBullet[i] = CCTextureCache::sharedTextureCache()->addImage(bulletName);
			//pLayer->m_ptBullet[i]->retain();
			//CCSize size = pLayer->m_ptBullet[i]->getContentSize();
			//pLayer->m_iBulletW[i+1] = size.width/2;
			//pLayer->m_iBulletH[i+1] = size.height/2;
			pLayer->m_sBulletPath = bulletName;
			pLayer->m_iBulletW[i+1] = 20;
			pLayer->m_iBulletH[i+1] = 5;
		}
		else
		{
			pLayer->m_iBulletH[i+1] = 120;
		}
		if ( AppDelegate::s_WeaponUse[i+1] == 8 || AppDelegate::s_WeaponUse[i+1] == 10 || AppDelegate::s_WeaponUse[i+1] == 11 || AppDelegate::s_WeaponUse[i+1] == 12 )
			pLayer->m_bStrike[i] = true;
		if ( AppDelegate::s_WeaponUse[i+1] == 10 || AppDelegate::s_WeaponUse[i+1] == 12 || AppDelegate::s_WeaponUse[i+1] == 16 || AppDelegate::s_WeaponUse[i+1] == 17 )
			pLayer->m_bShells[i] = true;
	}

	pLayer->m_pHero->ChangeWeapon(AppDelegate::s_WeaponUse[0]);
	pLayer->m_pHero->ChangeWeapon(AppDelegate::s_WeaponUse[1]);
	pLayer->m_pHero->RunAction(1);
	pLayer->m_pHero->HeroAim(0, 1);

	switch ( AppDelegate::s_WeaponUse[0] )
	{
	case 2:
	case 14:
	case 15:
		pLayer->m_iKnifeAction[0] = 8;
		pLayer->m_iKnifeAction[1] = 9;
		pLayer->m_iKnifeAction[2] = 10;
		pLayer->m_iKnifeAction[3] = 11;
		pLayer->m_iKnifeAction[4] = 12;
		pLayer->m_iKnifeAction[5] = 14;
		pLayer->m_iKnifeAction[6] = 16;
		break;
	case 6:
		for ( int i = 0; i < 7; i++ )
		{
			pLayer->m_iKnifeAction[i] = 22 + i;
		}
		break;
	case 13:
		for ( int i = 0; i < 7; i++ )
		{
			pLayer->m_iKnifeAction[i] = 29 + i;
		}
		break;
	}

	pLayer->m_ptBox = CCTextureCache::sharedTextureCache()->addImage("Tile/xiangzi.png");
	pLayer->m_ptBox->retain();
	if ( AppDelegate::s_WeaponUse[1] == 8 )
		pLayer->m_ptShell = CCTextureCache::sharedTextureCache()->addImage("shortgunshell.png");
	else
		pLayer->m_ptShell = CCTextureCache::sharedTextureCache()->addImage("shells.png");
	pLayer->m_ptShell->retain();
	common::AddTextureCacheArray(pLayer->m_ptDropsA, "Animation/drops/a", 5);
	common::AddTextureCacheArray(pLayer->m_ptDropsB, "Animation/drops/b", 6);
	common::AddTextureCacheArray(pLayer->m_ptDropsT, "Animation/drops/tou", 5);
#ifndef GameTypeC
	if ( AppDelegate::s_HeroType == 0 )
	{
		common::AddTextureCacheArray(pLayer->m_ptHead, "Animation/head/lan", 7);
		common::AddTextureCacheArray(pLayer->m_ptHeadShake, "Animation/head/hong", 4);
	}
	else if ( AppDelegate::s_HeroType == 1 )
	{
		common::AddTextureCacheArray(pLayer->m_ptHead, "Animation/head2/lan", 7);
		common::AddTextureCacheArray(pLayer->m_ptHeadShake, "Animation/head2/hong", 4);
	}
	else if ( AppDelegate::s_HeroType == 2 )
	{
		common::AddTextureCacheArray(pLayer->m_ptHead, "Animation/head3/lan", 7);
		common::AddTextureCacheArray(pLayer->m_ptHeadShake, "Animation/head3/hong", 4);
	}
#endif

	return pLayer;
}

void CGameControler::GameLoadingSecond()
{
	CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ccbControlMenu", ccbControlMenuLoader::loader());
	cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	m_pMenuNode = ccbReader->readNodeGraphFromFile("6.ccbi"/*, this*/);
	m_pMenuNode->setPosition(ccp(0,0));
	//SetScale(m_pMenuNode);
	((ccbControlMenu*)m_pMenuNode)->setAnimationManager(ccbReader->getAnimationManager(), m_unLevel);
	ccbReader->release();
	addChild(m_pMenuNode, 4, 50);

#ifndef GameTypeC
	for ( int i = 0; i < 4; i++ )
#else
	for ( int i = 0; i < 1; i++ )
#endif
	{
		char buffer[255];
		sprintf(buffer, "Animation/head/di%d.png", i+1);
		CCSprite* pSprite = CCSprite::create(buffer);
		pSprite->setPosition( CCPoint(d_LocateWeaponUse) );
		addChild(pSprite, 4, d_TagWeaponUse+i);

		if ( AppDelegate::s_HeroType == 0 )
			sprintf(buffer, "Animation/head/%d.png", i+1);
		else if ( AppDelegate::s_HeroType == 1 )
			sprintf(buffer, "Animation/head2/%d.png", i+1);
		else if ( AppDelegate::s_HeroType == 2 )
			sprintf(buffer, "Animation/head3/%d.png", i+1);
		CCSprite* pSprite1 = CCSprite::create(buffer);
		pSprite1->setPosition( CCPoint(d_locationHead) );
		pSprite1->setAnchorPoint(ccp(0, 0));
		pSprite->addChild(pSprite1);

#ifndef GameTypeC
		pSprite->setVisible(false);
		CCSprite* pSprite2 = CCSprite::createWithTexture(m_ptHead[0]);
		pSprite2->setPosition( CCPoint(d_locationHead) );
		pSprite2->setAnchorPoint(ccp(0, 0));
		CCAnimate* pAnimate = CCAnimate::create( common::CreateAnimationFrame(m_ptHead, 7, 0.05f, false) );
		pSprite2->runAction( CCRepeatForever::create(CCSequence::create(
			pAnimate, pAnimate, pAnimate, CCDelayTime::create(5.0f), NULL)) );
		pSprite->addChild(pSprite2);
#endif
	}

#ifndef GameTypeC
	m_pHeadShake = CCSprite::createWithTexture(m_ptHeadShake[0]);
	m_pHeadShake->setPosition( CCPoint(d_LocateWeaponUse) );
	addChild(m_pHeadShake, 4);
	m_pHeadShake->setVisible(false);
#endif

	//武器切换
	//for ( int i = 2; i < 16; i++ )
	//{
	//	char buffer[5];
	//	sprintf(buffer, "%d", i);
	//	std::string str = "tu4/weapon/";
	//	str += buffer;
	//	str += ".png";
	//	CCSprite* pSprite = CCSprite::create(str.c_str());
	//	pSprite->setPosition( CCPoint(d_LocateWeaponUse) );
	//	//pSprite->setScale(0.6f);
	//	pSprite->setVisible(false);
	//	addChild(pSprite, 4, d_TagWeaponUse+i);
	//}
	//getChildByTag(d_TagWeaponUse+m_iWeaponUse[m_ewtNowWeapon])->setVisible(true);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary1 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary1->registerCCNodeLoader("ccbPauseMenu", ccbPauseMenuLoader::loader());
	cocos2d::extension::CCBReader* ccbReader1 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary1);
	m_pPauseNode = ccbReader1->readNodeGraphFromFile("7.ccbi"/*, this*/);
	m_pPauseNode->setPosition(ccp(0,0));
	//SetScale(m_pPauseNode);
	((ccbPauseMenu*)m_pPauseNode)->setAnimationManager(ccbReader1->getAnimationManager(), this);
	ccbReader1->release();
	addChild(m_pPauseNode, 21, 51);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary2 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary2->registerCCNodeLoader("ccbGameoverMenu", ccbGameoverMenuLoader::loader());
	cocos2d::extension::CCBReader* ccbReader2 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary2);
	m_pGameoverNode = ccbReader2->readNodeGraphFromFile("8.ccbi"/*, this*/);
	m_pGameoverNode->setPosition(ccp(0,0));
	//SetScale(m_pGameoverNode);
	((ccbGameoverMenu*)m_pGameoverNode)->setAnimationManager(ccbReader2->getAnimationManager(), m_unLevel);
	((ccbGameoverMenu*)m_pGameoverNode)->SetGame(this);
	ccbReader2->release();
	addChild(m_pGameoverNode, 22, 52);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary3 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary3->registerCCNodeLoader("ccbGameoverBtnMenu", ccbGameoverBtnMenuLoader::loader());
	cocos2d::extension::CCBReader* ccbReader3 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary3);
	m_pGameoverBtnNode = ccbReader3->readNodeGraphFromFile("9.ccbi"/*, this*/);
	m_pGameoverBtnNode->setPosition(ccp(0,0));
	//SetScale(m_pGameoverBtnNode);
	((ccbGameoverBtnMenu*)m_pGameoverBtnNode)->setAnimationManager(ccbReader3->getAnimationManager(), this);
	ccbReader3->release();
	addChild(m_pGameoverBtnNode, 23, 53);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary4 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary4->registerCCNodeLoader("ccbSkillLayer", ccbSkillLayerLoader::loader());
	cocos2d::extension::CCBReader* ccbReader4 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary4);
	m_pSkillNode = ccbReader4->readNodeGraphFromFile("19.ccbi"/*, this*/);
	m_pSkillNode->setVisible(false);
	m_pSkillNode->setPosition(ccp(0,0));
	//SetScale(m_pSkillNode);
	((ccbSkillLayer*)m_pSkillNode)->setAnimationManager(ccbReader4->getAnimationManager(), this);
	ccbReader4->release();
	addChild(m_pSkillNode, 17, 54);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary5 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary5->registerCCNodeLoader("ccbGameStart", ccbGameStartLoader::loader());
	cocos2d::extension::CCBReader* ccbReader5 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary5);
	m_pGameStart = ccbReader5->readNodeGraphFromFile("14.ccbi"/*, this*/);
	m_pGameStart->setPosition(ccp(0,0));
	m_pGameStart->setVisible(false);
	//SetScale(pGameStart);
	((ccbGameStart*)m_pGameStart)->setAnimationManager(ccbReader5->getAnimationManager(), this);
	ccbReader5->release();
	addChild(m_pGameStart, 18, 55);
	//m_pGameStart->setVisible(false);

	if ( 2 != m_iLevelType && m_unLevel < 55 )
	{
		m_fHeroLocateX	= d_fHeroWidth*2.0;
		m_fRelativeX	= m_fHeroLocateX-10;
	}
	else if ( m_unLevel > 54 )
	{
		m_fHeroLocateX = d_fHeroWidth*2.0;
		m_fRelativeX	= m_fHeroLocateX;
	}
	else
	{
		m_fHeroLocateX = m_iMapNum/2.0f*800;
		m_fRelativeX = 800/2.0f;
	}
	if ( m_unLevel < 49 && m_unLevel >= 0 )
	{
		for ( int i = 0; i < 3; i++ )
		{
			m_structQuestion.index[i] = data_qustioninfo_type[i+m_unLevel*3];
			m_structQuestion.type[data_qustioninfo_type[i+m_unLevel*3]-1] = 1;
			m_structQuestion.num[data_qustioninfo_type[i+m_unLevel*3]-1] = data_qustioninfo_num[i+m_unLevel*3];
			m_structQuestion.id[data_qustioninfo_type[i+m_unLevel*3]-1] = data_qustioninfo_id[i+m_unLevel*3];
		}
	}

	{
		CCMenuItem* pItem = CCMenuItemImage::create("tu4/buyboard.png", "tu4/buyboard.png", this, menu_selector(CGameControler::Sell4_1));
		pItem->setPosition(ccp(747, 270));
		m_pSullpyBtn = CCMenu::create(pItem, NULL);
		m_pSullpyBtn->setPosition(ccp(0,0));
		m_pSullpyBtn->setVisible(false);
		addChild(m_pSullpyBtn, 5);

		m_pSullpyHealbox = CCSprite::create("tu3/qiang/0.png");
		m_pSullpyHealbox->setPosition(ccp(770, 281));
		m_pSullpyHealbox->setVisible(false);
		addChild(m_pSullpyHealbox, 6);

		char buffer[255];
		sprintf(buffer, "tu3/bullet/%d.png", AppDelegate::s_WeaponUse[1]);
		m_pSullpyBullet = CCSprite::create(buffer);
		m_pSullpyBullet->setPosition(ccp(770, 275));
		m_pSullpyBullet->setScale(0.60f);
		m_pSullpyBullet->setVisible(false);
		addChild(m_pSullpyBullet, 7);

		m_pSullpyBoard = CCSprite::create("tu4/goumai.png");
		m_pSullpyBoard->setPosition(ccp(737,246));
		m_pSullpyBoard->setVisible(false);
		addChild(m_pSullpyBoard, 8);
	}

	char buffer[250];
	if ( 6 == m_iLevelType )
		sprintf(buffer,"MS/Music/level2/1.mp3");
	else if ( m_unLevel == 30 || m_unLevel == 60 )
		sprintf(buffer,"MS/Music/level1/1.mp3");
	else if ( m_unLevel < 49 )
		sprintf(buffer,"MS/Music/level%d/%d.mp3", m_unLevel/6+1, m_unLevel%6%2+1);
	else if ( m_unLevel < 55 )
		sprintf(buffer,"MS/Music/level%d/%d.mp3", m_unLevel-48, 1);
	else
		sprintf(buffer,"MS/Music/level%d/%d.mp3", m_unLevel-54, 1);
	AppDelegate::AudioPlayBgm(buffer);
	if ( m_unLevel > 48 )
	{
		int level = m_unLevel - 49;
		for ( int i = 0; i < 3; i++ )
		{
			bool ok = true;
			while ( ok )
			{
				ok = false;
				m_iKeyIndex[i] = rand()%keynum[level]+1;
				for ( int j = 0; j < i; j++ )
				{
					if ( m_iKeyIndex[j] == m_iKeyIndex[i] )
						ok = true;
				}
			}
		}
	}
}

void CGameControler::PlaneFly()
{
	CCNodeLoaderLibrary* ccNodeLoaderLibrary6 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary6->registerCCNodeLoader("ccbStartGame", ccbStartGameLoader::loader());
	cocos2d::extension::CCBReader* ccbReader6 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary6);
	ccbStartGame* pStartGame = (ccbStartGame*)ccbReader6->readNodeGraphFromFile("24.ccbi"/*, this*/);
	pStartGame->setAnimationManager(ccbReader6->getAnimationManager(), this);
	ccbReader6->release();
	m_pCFrontgroud->addChild(pStartGame, -1, 56);

	if ( 6 == m_iLevelType )
		m_pHero->RunAction(m_iKnifeAction[0]);

	if ( 4 == m_iLevelType )
		m_iYDist = g_iHideLevelY[m_unLevel-49];
	//if ( m_unLevel%7 != 4 )
		pStartGame->Appear(CCPoint(0, 200+m_iYDist), CCPoint(400, 400));
	//else
		//pStartGame->Appear(CCPoint(300, 600), CCPoint(500, 200));
}

void CGameControler::HeroStart()
{
	m_fHeroLocateX = m_pHero->GetArmature()->getPositionX();
	m_fRelativeX = m_fHeroLocateX;
	if ( m_fRelativeX > 400 )
		m_fRelativeX = 400;
	m_fHeroLocateY = m_pHero->GetArmature()->getPositionY();
	SetLocation();
	schedule(schedule_selector(CGameControler::TimerStart), 0/*1.0f/MAINFRAME*/);
}

void CGameControler::GameStart()
{
	if ( m_unLevel > 54 )
		m_pHero->GetArmature()->setPosition(ccp(200, 105));
	else if ( m_unLevel > 29 )
		m_pHero->GetArmature()->setPosition(ccp(80, 110+m_iYDist));
	else if ( m_unLevel >= 0 )
		unschedule(schedule_selector(CGameControler::TimerStart));
	else
		m_pHero->GetArmature()->setPosition(ccp(200, 105));
	if ( m_iLevelType == 1 )
	{
		m_iLevelType = 1;
		m_bLimitStart = true;
		m_pLimitBoard = CCSprite::create("tu4/limitT.png");
		m_pLimitBoard->setPosition(ccp(400, 240));
		addChild(m_pLimitBoard, 5);
		m_iLimitTimeNeed = StayTime[m_unLevel/6];
		common::ShowNumber(m_pLimitBoard, 888, 38, 65, d_LocateLimit, "tu4/limitN2.png", 889);
	}
	else if ( m_iLevelType == 3 )
	{
		m_bLimitStart = true;
		m_pLimitBoard = CCSprite::create("tu4/limitM.png");
		m_pLimitBoard->setPosition(ccp(400, 240));
		addChild(m_pLimitBoard, 5);
		common::ShowNumber(m_pLimitBoard, 888, 38, 65, d_LocateLimit, "tu4/limitN2.png", 889);
	}
	else
		NPCTalkStart();

	CCSprite* pHealBoard = CCSprite::create("tu4/di.png");
	pHealBoard->setPosition( CCPoint(d_LocateHealBoard) );
	addChild(pHealBoard, 5);
	CCSprite* pHeartbeat = common::CreateAnimation("Animation/heartbeat/1.png", CCPoint(d_LocateHeartbeat), ccp(0.5f, 0.5f),
		"Animation/heartbeat/", 6, CCRect(0,0,d_sizeHeartbeat), true);
	pHealBoard->addChild(pHeartbeat, 5);
	for ( int i = 0; i < d_iHealBoxMax; i++ )
	{
		CCSprite* pSprite1 = CCSprite::create("tu4/xuebao.png");
		pSprite1->setPosition( CCPoint(d_LocateHealBox(i)) );
		addChild(pSprite1, 5, d_TagHealBoxHave+i);
		CCSprite* pSprite2 = CCSprite::create("tu4/xuebao2.png");
		pSprite2->setPosition( CCPoint(d_LocateHealBox(i)) );
		addChild(pSprite2, 5, d_TagHealBoxUnhave+i);
	}
	ShowHealBox();
	//for ( int i = 0; i < QuestionTypeNum; i++ )
	//{
		//m_structQuestion.type[i] = 0;
		//m_structQuestion.num[i] = 0;
		//m_structQuestion.num[i] = 0;
	m_bPause = false;
	schedule(schedule_selector(CGameControler::Timer));
	//if ( m_iLevelType != 1 && m_iLevelType != 3 )
	//	((ccbGameStart*)getChildByTag(55))->Appear();
	((ccbControlMenu*)m_pMenuNode)->Appear();
	if ( 6 == m_iLevelType )
		((ccbControlMenu*)m_pMenuNode)->SetType(0);

	if ( m_iMapMax == 1 && m_unLevel < 30 )
	{
		if ( !(m_iLevelType == 1 ||
			m_iLevelType == 2 ||
			m_iLevelType == 3 ||
			m_unLevel%6 == 5) && m_unLevel < 49 && m_unLevel >= 0 )
		{
			m_pWinMark = CCSprite::create("Animation/winmark/1.png");
			m_pWinMark->setPosition(ccp(d_fMapSizeX*m_iMapNum-240, 300));
			m_pWinMark->setAnchorPoint(ccp(0.0f, 0.5f));
			CCRepeatForever* pAction1 = CCRepeatForever::create(CCAnimate::create(
				common::CreateAnimationFrame("Animation/winmark/", 2, CCRect(0,0,147,63))));
			CCRepeatForever* pAction2 = CCRepeatForever::create(CCSequence::create(
				CCMoveBy::create(0.2f,ccp(60.0,0)), CCMoveBy::create(1.0f,ccp(-60.0,0)), NULL));
			m_pWinMark->runAction(pAction1);
			m_pWinMark->runAction(pAction2);
			m_pCFrontgroud->GetTileMap()->addChild(m_pWinMark, 5);
			m_pWinMark->setVisible(false);
		}
	}

	m_pWeaponMark1 = CCSprite::create("tu4/change1.png");
	m_pWeaponMark1->setPosition(ccp(g_fButtonX[2],g_fButtonY[2]));
	m_pWeaponMark1->setVisible(false);
	addChild(m_pWeaponMark1,9);
	m_pWeaponMark2 = CCSprite::create("tu4/change2.png");
	m_pWeaponMark2->setPosition(ccp(g_fButtonX[2],g_fButtonY[2]));
	addChild(m_pWeaponMark2,9);
	if ( 6 == m_iLevelType )
	{
		m_pWeaponMark2->setVisible(false);
		m_pWeaponMark1->setVisible(true);
	}

	m_pSkillDigit = CCSprite::create("tu4/shuzidi.png");
	m_pSkillDigit->setPosition( ccp(500, 446) );
	addChild(m_pSkillDigit, 9);
	common::ShowNumber(m_pSkillDigit, m_iSkillCanUse+1, 11, 15, 20, 8, "tu4/suzi.png");
	if ( m_iSkillCanUse == 0 )
		m_pSkillDigit->setVisible(false);

	CCSprite* pSprite = CCSprite::create("tu4/board.png");
	pSprite->setPosition( ccp(414, 455) );
	addChild(pSprite, 10);
	for ( int i = 0; i < d_iActionNum; i++ )
	{
		if ( i == 4 )
			continue;
		CCSprite* pBtn =  CCSprite::create(g_sButtonPath[i].c_str());
		pBtn->setAnchorPoint(ccp(0.5f,0.5f));
		pBtn->setPosition(ccp(g_fButtonX[i],g_fButtonY[i]));
		addChild(pBtn,10, 20+i);
		if ( (m_iGrenadeNum == 0 && i == 6)
			|| (m_iStateSkill[0] < 100 && i == 5) )
			pBtn->setVisible(false);
		CCSprite* pBtnPress = CCSprite::create(g_sButtonPressPath[i].c_str());
		pBtnPress->setPosition(ccp(g_fButtonX[i],g_fButtonY[i]));
		pBtnPress->setVisible(false);
		addChild(pBtnPress,10, 40+i);
		if ( i == d_iActionNum-1 )
		{
			char path[255];
			sprintf(path, "tu4/skill%d.png", i-d_iActionNum+2);
			CCSprite* pSprite = CCSprite::create(path);
			m_pSkillBar[i-d_iActionNum+1] = CCProgressTimer::create(pSprite);
			m_pSkillBar[i-d_iActionNum+1]->setAnchorPoint( ccp(0.5f,0.5f) );
			m_pSkillBar[i-d_iActionNum+1]->setPosition( ccp(35,33) );
			m_pSkillBar[i-d_iActionNum+1]->setType(kCCProgressTimerTypeBar);//
			m_pSkillBar[i-d_iActionNum+1]->setMidpoint(ccp(0.5, 0));
			m_pSkillBar[i-d_iActionNum+1]->setBarChangeRate(ccp(0, 1));
			m_pSkillBar[i-d_iActionNum+1]->setPercentage(0); 
			pBtn->addChild(m_pSkillBar[i-d_iActionNum+1], 0, 888);

			sprintf(path, "Animation/skill/rope%d/1.png", i-d_iActionNum+2);
			m_pRope[i-d_iActionNum+1] = CCSprite::create(path);
			//if ( i-d_iActionNum+2 == 1 )
				m_pRope[i-d_iActionNum+1]->setPosition( ccp(77, -2) );
			//else if ( i-d_iActionNum+2 == 2 )
			//	m_pRope[i-d_iActionNum+1]->setPosition( ccp(72, 9) );
			pBtn->addChild(m_pRope[i-d_iActionNum+1], 1);
		}
	}
	if ( m_iStateSkill[0] == 100 )
	{
		CCSprite* pSprite = common::CreateAnimation("tu4/skill/1.png", ccp(g_fButtonX[5],g_fButtonY[5]), ccp(0.5f,0.5f),
			"tu4/skill/", 6, CCRect(0,0,89,74), true);
		addChild(pSprite,9, 39);
	}
	ButtonPressCheck();

#ifndef GameTypeC
	m_pBeAttackedEffect = CCSprite::create("xue.png");
	m_pBeAttackedEffect->setPosition(ccp(0.0f, 0.0f));
	m_pBeAttackedEffect->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pBeAttackedEffect->setOpacity(0);
	addChild(m_pBeAttackedEffect, 20);
#endif

	m_iPressCount = 0;
	setTouchEnabled(true);
	setKeypadEnabled(true);  

	//if ( m_iLevelType == 2 )
	//{
	//	CCSprite* pTipBoard = CCSprite::create("Tile/tishi/meizi.png");
	//	pTipBoard->setAnchorPoint(ccp(0,0));
	//	pTipBoard->setPosition(ccp(450, 290));
	//	addChild(pTipBoard, 10, 197);

	//	{
	//		CCSprite* pSprite = common::CreateAnimation("Tile/tishi/1.png", ccp(50, 67), ccp(0.5f, 0.5f),
	//			"Tile/tishi/", 4, CCRect(0,0,32,32), true, false, 0.1f);
	//		pTipBoard->addChild(pSprite);
	//	}

	//	CCAnimate* pAction1 = NULL;
	//	CCSprite* pSprite = NULL;
	//	CCDelayTime* pDelay = CCDelayTime::create(3.0f);
	//	CCCallFunc* pFunc = CCCallFunc::create(this, callfunc_selector(CGameControler::NPCTalk));
	//	if ( m_iLevelType == 1 )
	//	{
	//		pSprite = CCSprite::create("Tile/tishi/1/1.png");
	//		pAction1 = CCAnimate::create(common::CreateAnimationFrame("Tile/tishi/1/", 2, CCRect(0,0,209,70), 3.0f));
	//		pSprite->runAction(CCSequence::create(pFunc, pDelay, pFunc));
	//	}
	//	else if ( m_iLevelType == 2 )
	//	{
	//		pSprite = CCSprite::create("Tile/tishi/2/1.png");
	//		pAction1 = CCAnimate::create(common::CreateAnimationFrame("Tile/tishi/2/", 3, CCRect(0,0,209,70), 3.0f));
	//		pSprite->runAction(CCSequence::create(pFunc, pDelay, pFunc, pDelay, pFunc));
	//	}
	//	else if ( m_iLevelType == 3 )
	//	{
	//		pSprite = CCSprite::create("Tile/tishi/3/1.png");
	//		pAction1 = CCAnimate::create(common::CreateAnimationFrame("Tile/tishi/3/", 2, CCRect(0,0,209,70), 3.0f));
	//		pSprite->runAction(CCSequence::create(pFunc, pDelay, pFunc));
	//	}
	//	pSprite->setAnchorPoint(ccp(0,0));
	//	pSprite->setPosition(ccp(17, 11));
	//	CCDelayTime* pAction2 = CCDelayTime::create(3.0f);
	//	CCCallFunc* pAction3 = CCCallFunc::create(this, callfunc_selector(CGameControler::TipOver));
	//	pSprite->runAction(CCSequence::create(pAction1, pAction2, pAction3, NULL));
	//	pTipBoard->addChild(pSprite);
	//}

#ifndef PayGOCTwo
	if ( AppDelegate::m_Sell1 == 0 )
	{
		Sell2();
	}
#endif
}

bool CGameControler::init()
{
	//m_cSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	this->setKeypadEnabled(true);
#endif

	return true;
}

void CGameControler::keyEnterClicked(){
	if(keyPressed){
		return;
	}
	if(isInRecharge){
		if(rechargeIndex==0){
			GetMedalBtn(this);
		}else if(rechargeIndex==1){
			GetMedalBtn(this);
		}
		return;
	}
	if(isBuji){
		Sell4Resume(this);
		return;
	}
	if(m_bGuideBuyVip){
		switch(m_bGuideBuyVipIndex){
		case 0:
			GuideBuy();
			break;
		case 1:
			GuideBack();
			break;
		}
		return;
	}
	if(isInRelife){
		if(relifeIndex==0){
			Sell3Resume2(this);
		}else{
			Sell3Resume1(this);
		}
		return;
	}
	//CCLog("Enter keyCode=%d",keyCode);
	if ( (m_iGuideTalk == 6 || m_iGuideTalk == 7) && getChildByTag(196) )
	{
		if ( 4 == m_iGuideNow )
			m_iGuideMap = guideMap[5]+800;
		m_iGuideAction = m_iGuideNow+1;
		GuideResume();
	}
	//CCLog("确定键%d,%d",m_bAttack,m_iGrenadeTime);
	if ( m_bAttack || m_iGrenadeTime > 0 ){
		return;
	}
	m_bAttack		= true;
	m_pMoveActionArray[2] = 1;

	keyPressed = true;
	keyCode = kTypeEnterClicked;

}

void CGameControler::keyEnterReleased(){
	keyPressed = false;
	keyCode = 0;
	m_bAttack	= false;
	m_iAttackDigit	= 0;
	m_pMoveActionArray[2]	= 0;

}

void CGameControler::keyArrowReleased(int arrow){
	
	bool bMoveL = false;
	bool bMoveR = false;
	if ( m_pMoveActionArray[0] )
		bMoveL = true;
	if ( m_pMoveActionArray[1] )
		bMoveR = true;
	m_iMoveDigit			= 0;
	m_pMoveActionArray[0]	= 0;
	m_pMoveActionArray[1]	= 0;
	m_fHeroSpeedX			= 0;
	if ( !m_pMoveActionArray[0] && bMoveL )
		bMoveL = false;
	if ( !m_pMoveActionArray[1] && bMoveR )
		bMoveR = false;
}

void CGameControler::keyArrowClicked(int arrow){
	if(isInRecharge){
		if(arrow==kTypeLeftArrowClicked||arrow==kTypeRightArrowClicked){
			if(rechargeIndex==0){
				rechargeIndex = 1;
				getChildByTag(1001)->setPosition(ccp(556,174));
			}else if(rechargeIndex==1){
				rechargeIndex = 0;
				getChildByTag(1001)->setPosition(ccp(243,174));
			}
		}
		return;
	}

	if(m_bGuideBuyVip){
		if(arrow==kTypeDownArrowClicked||arrow==kTypeUpArrowClicked){
			if(m_bGuideBuyVipIndex==0){
				m_bGuideBuyVipIndex = 1;
				getChildByTag(1001)->setPosition(ccp(400,35));
			}else{
				m_bGuideBuyVipIndex = 0;
				getChildByTag(1001)->setPosition(ccp(673,411));
			}
		}
		return;
	}

	if(isInRelife){
		if(arrow==kTypeLeftArrowClicked||arrow==kTypeRightArrowClicked){
			if(relifeIndex==0){
				relifeIndex = 1;
				getChildByTag(1001)->setPosition(ccp(560,74));
			}else{
				relifeIndex = 0;
				getChildByTag(1001)->setPosition(ccp(250,74));
			}
		}
		return;
	}
	//CCLog("arrow keyCode=%d",keyCode);
#ifdef CHANGE_CONTRAL
	switch(arrow){
	case kTypeLeftArrowClicked:
		heroUseBomb();
		break;
	case kTypeRightArrowClicked:
		heroJump();
		break;
	case kTypeDownArrowClicked:
		MoveCheck(1, 1);
		break;
	case kTypeUpArrowClicked:
		MoveCheck(0, 1);
		break;
	}
#else
	switch(arrow){
	case kTypeDownArrowClicked:
		heroUseBomb();
		break;
	case kTypeUpArrowClicked:
		heroJump();
		break;
	case kTypeRightArrowClicked:
		MoveCheck(1, 1);
		break;
	case kTypeLeftArrowClicked:
		MoveCheck(0, 1);
		break;
	}
#endif
	keyPressed = true;
	keyCode = arrow;
}

void CGameControler::keyNumClicked(int keyNum){
	switch(keyNum){
	case kTypeNum0:
		//keyBackClicked();
		if(isInRecharge){
			GetRemove(this);
			return;
		}
		if(isBuji){
			Sell2Back(this);
			return;
		}
		if(m_bGuideBuyVip){
			GuideBack();
			return;
		}
		if(isInRelife){
			Sell3Back(this);
			return;
		}
		
		//CCLog("guideTalk=%d",m_iGuideTalk);
		if(m_iGuideTalk==12){
				PauseGame();
				return;
		}
		if ( m_bPause || m_iSkill || m_bWin || m_bDead || m_bLimitStart || m_bExitGame || m_bSellOpen || m_bBuyMedal 
		|| getChildByTag(197) ){
				return;
		}
			
		PauseGame();
		break;
	case kTypeNum1:
		if(!bInChangeWeapon){
			heroChangeWeapon();
		}
		break;
	case kTypeNum2:
		CheckToSkill();
		break;
	case kTypeNum8:
		
	break;
	}
	//CCLog("按下了数字键:%d",keyNum);
}

void CGameControler::keyNumReleased(int keyNum){
	switch(keyNum){
	case kTypeNum1:
		bInChangeWeapon = false;
		break;
	
	}
}

void CGameControler::heroJump(){
	if ( m_iGrenadeTime > 0 || ( m_ewtNowWeapon == eWT_Knife && m_iAttackCoolTime > 0 ))
		return;
	if ( m_iHoldTime > 0 )
		return;
	if ( (m_iGuideTalk == 4 || m_iGuideTalk == 5) && getChildByTag(196) )
	{
		m_iGuideMap = guideMap[m_iGuideAction]+800;
		m_iGuideAction = m_iGuideNow+1;
		GuideResume();
	}
	if ( !m_iHeroJumps )
	{
		AppDelegate::AudioPlayEffect("MS/Sound/EfJump.mp3");
		m_fHeroSpeedY = d_fHeroJump;
		m_iHeroJumps = 1;
		m_iJumpActionType = 2;
	}
	else if ( m_iHeroJumps == 1 )
	{
		AppDelegate::AudioPlayEffect("MS/Sound/EfJump.mp3");
		m_iHeroJumps ++;
		m_fHeroSpeedY		= d_fHeroJump;
		m_iJumpActionType	= 2;
	}
	else if ( m_iHeroJumps == 2 && m_iJumpCanTimes > 2 )
	{
		AppDelegate::AudioPlayEffect("MS/Sound/EfJump.mp3");
		m_iHeroJumps ++;
		m_fHeroSpeedY		= m_fJumpSpeedInit;
		m_iJumpActionType	= 2;
	}
	m_pMoveActionArray[3] = 20;
}

void CGameControler::heroChangeWeapon(){
	bInChangeWeapon = true;
	bool change = false;
	if ( m_ewtNowWeapon == eWT_Knife )
	{
			
		m_pWeaponMark1->setVisible(false);
		m_pWeaponMark2->setVisible(true);
		m_pWeaponMark2->setRotation(0);
		m_ewtNowWeapon = eWT_Pistol;
		change = true;
			
	}else{
		m_pWeaponMark2->setVisible(false);
		m_pWeaponMark1->setVisible(true);
		m_pWeaponMark1->setRotation(0);
		m_ewtNowWeapon = eWT_Knife;
		change = true;
	}
	if ( change )
	{
		if ( m_ewtNowWeapon != eWT_Knife && ( m_iHeroJumps < 2 || m_fHeroSpeedY < 2.0f ))
		{
			m_pHero->ChangeWeapon(m_iWeaponUse[0]);
			m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
			if ( m_iWeaponUse[1] == 12 && !m_bBossWin )
				m_iFireTime = 1;
		}else{
			m_iFireTime = 0;
			removeChildByTag(805);
		}
		if ( m_iLevelType == 1 || m_iLevelType == 3 )
		{
			if ( m_ewtNowWeapon == eWT_Knife )
				((ccbControlMenu*)m_pMenuNode)->SetType(2);
			else
				((ccbControlMenu*)m_pMenuNode)->SetType(3);
		}
		else
		{
			if ( m_ewtNowWeapon == eWT_Knife )
				((ccbControlMenu*)m_pMenuNode)->SetType(0);
			else
				((ccbControlMenu*)m_pMenuNode)->SetType(1);
		}
		g_pHero->m_unNowWeapon = m_ewtNowWeapon;
	}
	if ( m_iGuideTalk == 8 && getChildByTag(196) )
	{
		removeChildByTag(193);
		GuideResume();
		GuideTalk(9);
	}

}

void CGameControler::heroUseBomb(){
	if ( m_iHoldTime > 0 )
		return;
	if ( m_iGrenadeNum < 1 || m_pGrenade || m_iGrenadeTime > 0  )
		return;
	if ( m_iGuideTalk == 10 && getChildByTag(196) )
	{
		m_iGuideAction = 9;
		GuideResume();
	}
	m_iGrenadeNum--;
	if ( m_iGrenadeNum == 0 )
	{
		getChildByTag(26)->setVisible(false);
		getChildByTag(46)->setVisible(false);
	}
	AchieveAdd::Add(8);
	m_bAttack = false;
	m_pMoveActionArray[6] = 20;
	if ( 0 == m_fHeroSpeedY && 0 == m_iHeroJumps )
	{
		m_iGrenadeTime = 35;
		m_pHero->m_unLastAction = -1;
		switch( m_ewtNowWeapon )
		{
		case eWT_Knife:
			m_pHero->RunAction(m_iKnifeAction[6]);
			break;
		default:
			m_pHero->RunAction(15);
			break;
		}
	}
	else
	{
		GrenadeCreate();
	}
}

void CGameControler::heroUseSkill(){
	if ( m_iHoldTime > 0 )
		return;
	if ( m_iStateSkill[0] < 100 || m_pArrayThunder || m_iSkill  ) //|| m_bBossWin
		return;
	{
		if ( m_iGuideTalk == 11 && getChildByTag(196) )
		{
			m_iGuideMap = guideMap[9]+800;
			GuideResume();
		}
		if ( getChildByTag(39) )
			getChildByTag(39)->removeFromParentAndCleanup(true);
		getChildByTag(25)->setVisible(false);
		if ( AppDelegate::s_HeroType == 0 )
			AchieveAdd::Add(41);
		else if ( AppDelegate::s_HeroType == 1 )
			AchieveAdd::Add(45);
		else if ( AppDelegate::s_HeroType == 2 )
			AchieveAdd::Add(49);
		m_iSkill = 1;
		m_bPause = true;
		m_pHero->PauseAction();
		m_iStateSkill[0] = 0;
		if ( m_pRope[0] )
		{
			m_pRope[0]->stopAllActions();
			common::RunNewAnimation(m_pRope[0], "Animation/skill/rope1/", 5, CCRect(0,0,52,43));
		}
		m_pSkillDown[0] = CCSprite::create("tu4/skill1.png");
		m_pSkillDown[0]->setPosition( ccp(g_fButtonX[8], g_fButtonY[8]) );
		addChild(m_pSkillDown[0], 10);
		m_pSkillBar[0]->removeFromParentAndCleanup(true);
		m_pSkillBar[0] = NULL;

#ifdef GameTypeC
		SkillStart();
#else
		CCMoveBy* pAction1 = CCMoveBy::create(0.5f, ccp(0, -480/2));
		CCAnimate* pAction2 = CCAnimate::create( common::CreateAnimationFrame(
			"Animation/skill/3/bao", 3, CCRect(0,0,381,283), 0.1f) );
		m_pSkillDown[0]->runAction( CCSequence::create(
			pAction1, pAction2, CCCallFunc::create(this, callfunc_selector(CGameControler::SkillStart)), NULL) );
#endif
	}
}

void CGameControler::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	m_iPressCount++;
	if ( m_bPause || m_iSkill || m_bWin || m_bDead || m_bLimitStart || m_bSellOpen || m_bExitGame )
		return;
	//if ( m_iLevelType != 1 && m_iLevelType != 3 /*&& !m_bStartMovie*/ )
	//	return;
	CCSetIterator iter = pTouches->begin();
	int digit = 1;
	for (; iter != pTouches->end(); iter++/*, digit++*/)
	{
		CCPoint location = ((CCTouch*)(*iter))->getLocation();
		digit = (*iter)->m_uID;

		e_ActionType iActionType = eAT_NULL;
		for ( int i = 0; i < d_iActionNum; i++ )
		{
			if ( 6 == m_iLevelType )
			{
				if ( i == 4 )
					continue;
				bool vis = true;
				if ( getChildByTag(40+i) )
					vis = getChildByTag(40+i)->isVisible();
				if ( !getChildByTag(20+i)->isVisible() && !vis )
					continue;
			}
			if ( fabs(location.x - g_fButtonX[i]*Scale_X) < g_fButtonW[i]*Scale_X
				&& fabs(location.y - g_fButtonY[i]*Scale_Y) < g_fButtonH[i]*Scale_Y )
			{
				if ( i == 2 )
				{
					if ( (location.x - g_fButtonX[i]*Scale_X)*(location.x - g_fButtonX[i]*Scale_X) +
						(location.y - g_fButtonY[i]*Scale_Y)*(location.y - g_fButtonY[i]*Scale_Y) > 
						g_fButtonW[i]*Scale_X*g_fButtonW[i]*Scale_X )
						continue;
				}
				iActionType = (e_ActionType)i;
				break;
			}
		}
		switch ( iActionType )
		{
		case eAT_MoveLeft:
			MoveCheck(0, digit);
			break;
		case eAT_MoveRight:
			MoveCheck(1, digit);
			break;
		case eAT_Jump:
			if ( m_iGrenadeTime > 0 || ( m_ewtNowWeapon == eWT_Knife && m_iAttackCoolTime > 0 ))
				break;
			if ( m_iHoldTime > 0 )
				return;
			if ( (m_iGuideTalk == 4 || m_iGuideTalk == 5) && getChildByTag(196) )
			{
				m_iGuideMap = guideMap[m_iGuideAction]+800;
				m_iGuideAction = m_iGuideNow+1;
				GuideResume();
			}
			if ( !m_iHeroJumps )
			{
				AppDelegate::AudioPlayEffect("MS/Sound/EfJump.mp3");
				m_fHeroSpeedY = d_fHeroJump;
				m_iHeroJumps = 1;
				m_iJumpActionType = 2;
			}
			else if ( m_iHeroJumps == 1 )
			{
				AppDelegate::AudioPlayEffect("MS/Sound/EfJump.mp3");
				m_iHeroJumps ++;
				m_fHeroSpeedY		= d_fHeroJump;
				m_iJumpActionType	= 2;
			}
			else if ( m_iHeroJumps == 2 && m_iJumpCanTimes > 2 )
			{
				AppDelegate::AudioPlayEffect("MS/Sound/EfJump.mp3");
				m_iHeroJumps ++;
				m_fHeroSpeedY		= m_fJumpSpeedInit;
				m_iJumpActionType	= 2;
			}
			m_pMoveActionArray[3] = 20;
			break;
		case eAT_Attack:
			if ( (m_iGuideTalk == 6 || m_iGuideTalk == 7) && getChildByTag(196) )
			{
				if ( 4 == m_iGuideNow )
					m_iGuideMap = guideMap[5]+800;
				m_iGuideAction = m_iGuideNow+1;
				GuideResume();
			}
			if ( m_bAttack || m_iGrenadeTime > 0 )
				break;
			m_bAttack		= true;
			m_pMoveActionArray[2] = digit;
			break;
		case eAT_ChangeWeapon:
			if ( m_iStateHP < 100 && m_iHealBox > 0 )
			{
				AppDelegate::AudioPlayEffect("MS2/heal.mp3");
				if ( m_pHealEffect1 )
				{
					m_pHealEffect1->stopAllActions();
					m_pHealEffect1->removeFromParent();
				}
				if ( m_pHealEffect2 )
				{
					m_pHealEffect2->stopAllActions();
					m_pHealEffect2->removeFromParent();
				}
				if ( m_structQuestion.type[1] )
				{
					m_structQuestion.num[1]--;
				}
				m_pHealEffect1 = CCSprite::create("Animation/heal/hou/1.png");
				m_pHealEffect1->setPosition(ccp(m_fHeroLocateX, m_fHeroLocateY));
				m_pHealEffect1->setAnchorPoint(ccp(0.59f, 0.18f));
				m_pCFrontgroud->GetTileMap()->addChild(m_pHealEffect1, 3);
				char str[100] = {0};
				CCArray* animFrames1 = CCArray::createWithCapacity(10);
				for(int i = 1; i < 10; i++) 
				{
					sprintf(str,"Animation/heal/hou/%d.png", i);
					CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iHealEffectWH));
					animFrames1->addObject(frame);
				}
				CCAnimation* pAnimation1 = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
				CCSequence* pSequence1 = CCSequence::create(
					CCAnimate::create(pAnimation1), 
					CCCallFunc::create(this, callfunc_selector(CGameControler::HealEffectOver1)),
					NULL);
				m_pHealEffect1->runAction( pSequence1 );
				m_pHealEffect1->setPositionX(d_fHealEffectToward*m_fHeroToward-d_fHealEffectToward);
				m_pHealEffect1->setScaleX(m_fHeroToward);

				m_pHealEffect2 = CCSprite::create("Animation/heal/qian/1.png");
				//m_pHealEffect2->setPosition(ccp(m_fHeroLocateX, d_fHealEffectFixY+m_fHeroLocateY));
				m_pCFrontgroud->GetTileMap()->addChild(m_pHealEffect2, 4);
				CCArray* animFrames2 = CCArray::createWithCapacity(13);
				for(int i = 1; i < 12; i++) 
				{
					sprintf(str,"Animation/heal/qian/%d.png", i);
					CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iHealEffectWH));
					animFrames2->addObject(frame);
				}
				CCAnimation* pAnimation2 = CCAnimation::createWithSpriteFrames(animFrames2, d_fStarMovieTime);
				CCSequence* pSequence2 = CCSequence::create(
					CCAnimate::create(pAnimation2), 
					CCCallFunc::create(this, callfunc_selector(CGameControler::HealEffectOver2)),
					NULL);
				m_pHealEffect2->runAction( pSequence2 );
				//m_pHealEffect2->setPositionX(d_fHealEffectToward*m_fHeroToward-d_fHealEffectToward);
				m_pHealEffect2->setScaleX(m_fHeroToward);

				m_iHealBox--;
				m_iStateHP = 100;
				m_pMoveActionArray[5] = 20;

				if ( m_iPrisonTime > 0 )
				{
					m_iPrisonTime = 0;
				}
				ShowHealBox();
			}
			//if ( m_iAttackCoolTime || m_bWin )
			//	break;
			////getChildByTag(d_TagWeaponUse+m_iWeaponUse[m_ewtNowWeapon])->setVisible(false);武器切换显示
			//m_ewtNowWeapon = (e_WeaponType)(m_ewtNowWeapon + 1);
			//if ( m_ewtNowWeapon > eWT_Pistol )
			//{
			//	m_ewtNowWeapon = eWT_Knife;
			//}
			//g_pHero->m_unNowWeapon = m_ewtNowWeapon;
			////getChildByTag(d_TagWeaponUse+m_iWeaponUse[m_ewtNowWeapon])->setVisible(true);武器切换显示
			//m_pMoveActionArray[4] = 20;
			//if ( m_ewtNowWeapon != eWT_Knife && ( m_iHeroJumps != 2 || m_fHeroSpeedY < 2.0f ))
			//{
			//	m_pHero->ChangeWeapon(m_iWeaponUse[0]);
			//	m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
			//	if ( m_iWeaponUse[1] == 12 )
			//		m_iFireTime = 1;
			//}
			//else
			//{
			//	m_iFireTime = 0;
			//	removeChildByTag(805);
			//}
			//if ( m_iLevelType == 1 || m_iLevelType == 3 )
			//{
			//	if ( m_ewtNowWeapon == eWT_Knife )
			//		((ccbControlMenu*)m_pMenuNode)->SetType(2);
			//	else
			//		((ccbControlMenu*)m_pMenuNode)->SetType(3);
			//}
			//else
			//{
			//	if ( m_ewtNowWeapon == eWT_Knife )
			//		((ccbControlMenu*)m_pMenuNode)->SetType(0);
			//	else
			//		((ccbControlMenu*)m_pMenuNode)->SetType(1);
			//}
			break;
		case eAT_BOMB:
			if ( m_iHoldTime > 0 )
				return;
			if ( m_iGrenadeNum < 1 || m_pGrenade || m_iGrenadeTime > 0  )
				return;
			if ( m_iGuideTalk == 10 && getChildByTag(196) )
			{
				m_iGuideAction = 9;
				GuideResume();
			}
			m_iGrenadeNum--;
			if ( m_iGrenadeNum == 0 )
			{
				getChildByTag(26)->setVisible(false);
				getChildByTag(46)->setVisible(false);
			}
			AchieveAdd::Add(8);
			m_bAttack = false;
			m_pMoveActionArray[6] = 20;
			if ( 0 == m_fHeroSpeedY && 0 == m_iHeroJumps )
			{
				m_iGrenadeTime = 35;
				m_pHero->m_unLastAction = -1;
				switch( m_ewtNowWeapon )
				{
				case eWT_Knife:
					m_pHero->RunAction(m_iKnifeAction[6]);
					break;
				default:
					m_pHero->RunAction(15);
					break;
				}
			}
			else
			{
				GrenadeCreate();
				//m_pGrenade = CCSprite::create("Animation/grenade/0.png");
				//m_iGrenadeRotate	= 0;
				//m_iGrenadeToward	= m_fHeroToward;
				//if ( AppDelegate::s_HeroType != 2 )
				//{
				//	m_fGrenadeLocateX	= m_fHeroLocateX;
				//	m_fGrenadeLocateY	= m_fHeroLocateY + 58;
				//}
				//else
				//{
				//	m_fGrenadeLocateX	= m_fHeroLocateX + 86*m_fHeroToward;
				//	m_fGrenadeLocateY	= m_fHeroLocateY + 96;
				//}
				//m_fGrenadeSpeedX	= d_fHeroSpeedX*2*m_fHeroToward;
				//m_fGrenadeSpeedY	= d_fHeroSpeedX;
				//m_pGrenade->setPosition( ccp(m_fGrenadeLocateX, m_fGrenadeLocateY) );
				//m_pCFrontgroud->GetTileMap()->addChild(m_pGrenade, 3);
			}
			break;
		case eAT_PAUSE:
			PauseGame();
			break;
		case eAT_SKILL1:
			if ( m_iStateSkill[0] < 100 )
			{
				Sell5();
			}
		case eAT_SKILL:
			if ( m_iHoldTime > 0 )
				return;
			if ( m_iStateSkill[0] < 100 || m_pArrayThunder || m_iSkill || m_bBossWin )
				return;
			{
				if ( m_iGuideTalk == 11 && getChildByTag(196) )
				{
					m_iGuideMap = guideMap[9]+800;
					GuideResume();
				}
				if ( getChildByTag(39) )
					getChildByTag(39)->removeFromParentAndCleanup(true);
				getChildByTag(25)->setVisible(false);
				if ( AppDelegate::s_HeroType == 0 )
					AchieveAdd::Add(41);
				else if ( AppDelegate::s_HeroType == 1 )
					AchieveAdd::Add(45);
				else if ( AppDelegate::s_HeroType == 2 )
					AchieveAdd::Add(49);
				m_iSkill = 1;
				m_bPause = true;
				m_pHero->PauseAction();
				m_iStateSkill[0] = 0;
				if ( m_pRope[0] )
				{
					m_pRope[0]->stopAllActions();
					common::RunNewAnimation(m_pRope[0], "Animation/skill/rope1/", 5, CCRect(0,0,52,43));
				}
				m_pSkillDown[0] = CCSprite::create("tu4/skill1.png");
				m_pSkillDown[0]->setPosition( ccp(g_fButtonX[8], g_fButtonY[8]) );
				addChild(m_pSkillDown[0], 10);
				m_pSkillBar[0]->removeFromParentAndCleanup(true);
				m_pSkillBar[0] = NULL;

#ifdef GameTypeC
				SkillStart();
#else
				CCMoveBy* pAction1 = CCMoveBy::create(0.5f, ccp(0, -480/2));
				CCAnimate* pAction2 = CCAnimate::create( common::CreateAnimationFrame(
					"Animation/skill/3/bao", 3, CCRect(0,0,381,283), 0.1f) );
				m_pSkillDown[0]->runAction( CCSequence::create(
					pAction1, pAction2, CCCallFunc::create(this, callfunc_selector(CGameControler::SkillStart)), NULL) );
#endif
			}
			break;
		//case eAT_SKILL2:
		//	if ( m_iStateSkill[1] < 100 || m_pArrayThunder || m_iSkill )
		//		return;
		//	{
		//		if ( g_pHero->m_iHeroType == 1 )
		//			AchieveAdd::Add(41);
		//		else
		//			AchieveAdd::Add(49);
		//		m_iSkill = 2;
		//		m_bPause = true;
		//		m_pHero->PauseAction();
		//		m_iStateSkill[1] = 0;
		//		m_pRope[1]->stopAllActions();
		//		common::RunNewAnimation(m_pRope[1], "Animation/skill/rope2/", 5, CCRect(0,0,42,61));
		//		m_pSkillDown[1] = CCSprite::create("tu4/skill2.png");
		//		m_pSkillDown[1]->setPosition( ccp(g_fButtonX[9], g_fButtonY[9]) );
		//		addChild(m_pSkillDown[1], 10);
		//		m_pSkillBar[1]->removeFromParentAndCleanup(true);
		//		m_pSkillBar[1] = NULL;

		//		CCMoveBy* pAction1 = CCMoveBy::create(0.5f, ccp(0, -480/2));
		//		m_pSkillDown[1]->runAction( CCSequence::create(
		//			pAction1, CCCallFunc::create(this, callfunc_selector(CGameControler::SkillAction1)), CCDelayTime::create(0.5f), 
		//			CCCallFunc::create(this, callfunc_selector(CGameControler::SkillStart)), NULL) );
		//	}
		//	break;
		default:
			break;
		}
		if ( iActionType == eAT_NULL && !m_bBossWin )
		{
			if ( 6 == m_iLevelType )
			{
				if ( m_iGuideBtn < 4 )
					continue;
			}
			if ( (location.x - g_fButtonX[2]*Scale_X)*(location.x - g_fButtonX[2]*Scale_X) +
				(location.y - g_fButtonY[2]*Scale_Y)*(location.y - g_fButtonY[2]*Scale_Y) < 
				(g_fButtonW[2]+70.0f)*Scale_X*(g_fButtonW[2]+70.0f)*Scale_X )
			{
				if ( location.x < g_fButtonX[2]*Scale_X )
				{
					if ( (m_ewtNowWeapon == eWT_Knife && fabs(location.y - (g_fButtonY[2]+50.0f)*Scale_Y) < 90.0f*Scale_Y) ||
						(m_ewtNowWeapon == eWT_Pistol && fabs(location.y - (g_fButtonY[2]-30.0f)*Scale_Y) < 70.0f*Scale_Y) )
					{
						m_unMoveEventID = digit;
						m_pointMoveEvent = location;					
						if ( m_iGuideTalk == 8 && getChildByTag(196) )
						{
							removeChildByTag(193);
							GuideResume();
							GuideTalk(9);
						}
					}
				}
			}
		}
	}
}

void CGameControler::CheckToSkill(){
	if ( m_iHoldTime > 0 )
		return;
	if ( m_iStateSkill[0] < 100 || m_pArrayThunder || m_iSkill || m_bBossWin )
		return;
	{
		if ( m_iGuideTalk == 11 && getChildByTag(196) )
		{
			m_iGuideMap = guideMap[9]+800;
			GuideResume();
		}
		if ( getChildByTag(39) )
			getChildByTag(39)->removeFromParentAndCleanup(true);
		getChildByTag(25)->setVisible(false);
		if ( AppDelegate::s_HeroType == 0 )
			AchieveAdd::Add(41);
		else if ( AppDelegate::s_HeroType == 1 )
			AchieveAdd::Add(45);
		else if ( AppDelegate::s_HeroType == 2 )
			AchieveAdd::Add(49);
		m_iSkill = 1;
		m_bPause = true;
		m_pHero->PauseAction();
		m_iStateSkill[0] = 0;
		if ( m_pRope[0] )
		{
			m_pRope[0]->stopAllActions();
			common::RunNewAnimation(m_pRope[0], "Animation/skill/rope1/", 5, CCRect(0,0,52,43));
		}
		m_pSkillDown[0] = CCSprite::create("tu4/skill1.png");
		m_pSkillDown[0]->setPosition( ccp(g_fButtonX[8], g_fButtonY[8]) );
		addChild(m_pSkillDown[0], 10);
		m_pSkillBar[0]->removeFromParentAndCleanup(true);
		m_pSkillBar[0] = NULL;

#ifdef GameTypeC
		SkillStart();
#else
		CCMoveBy* pAction1 = CCMoveBy::create(0.5f, ccp(0, -480/2));
		CCAnimate* pAction2 = CCAnimate::create( common::CreateAnimationFrame(
			"Animation/skill/3/bao", 3, CCRect(0,0,381,283), 0.1f) );
		m_pSkillDown[0]->runAction( CCSequence::create(
			pAction1, pAction2, CCCallFunc::create(this, callfunc_selector(CGameControler::SkillStart)), NULL) );
#endif
	}
}

void CGameControler::SkillInit()
{

}



void CGameControler::SkillAction1()
{
	m_pSkill1Particle = CCParticleSystemQuad::create("Animation/skill/2/skill2.plist");
	m_pSkill1Particle->setPosition( ccp(g_fButtonX[9], g_fButtonY[9]-480/2) );
	addChild(m_pSkill1Particle, 10);
}

void CGameControler::SkillStart()
{
	if ( m_pSkill1Particle )
	{
		m_pSkill1Particle->removeFromParentAndCleanup(true);
		m_pSkill1Particle = NULL;
	}
	m_pSkillDown[0]->removeFromParentAndCleanup(true);
	switch ( AppDelegate::s_HeroType )
	{
	case 0:
		((ccbSkillLayer*)m_pSkillNode)->Appear();
		break;
	case 1:
		((ccbSkillLayer*)m_pSkillNode)->Appear2();
		break;
	case 2:
		((ccbSkillLayer*)m_pSkillNode)->Appear3();
		break;
	}
	m_pSkillNode->setVisible(true);
	if ( m_pFireSprite )
		m_pFireSprite->pauseSchedulerAndActions();
}

void CGameControler::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if ( m_bLimitStart || m_bSellOpen || m_bExitGame )
		return;
	bool bNoChange = true;
	{
		CCSetIterator iter = pTouches->begin();
		int digit = 1;
		for (; iter != pTouches->end(); iter++/*, digit++*/)
		{
			digit = (*iter)->m_uID;
			if ( digit == m_unMoveEventID )
			{
				CCPoint location = ((CCTouch*)(*iter))->getLocation();
				float rotate = (location.y - m_pointMoveEvent.y)/2.0f;
				m_pointMoveEvent = location;
				if ( m_ewtNowWeapon == eWT_Knife )
				{
					rotate += m_pWeaponMark1->getRotation();
					if ( rotate > 0 )
						rotate = 0;
					else if ( rotate < -60 )
						rotate = -60;
					m_pWeaponMark1->setRotation(rotate);
				}
				else
				{
					rotate += m_pWeaponMark2->getRotation();
					if ( rotate > 60 )
						rotate = 60;
					else if ( rotate < 0 )
						rotate = 0;
					m_pWeaponMark2->setRotation(rotate);
				}
				bNoChange = false;
				break;
			}
		}
	}
	if ( bNoChange )
	{
		bool bMoveL = false;
		bool bMoveR = false;
		if ( m_pMoveActionArray[0] )
			bMoveL = true;
		if ( m_pMoveActionArray[1] )
			bMoveR = true;

		//m_pMoveActionArray[0]	= 0;
		//m_pMoveActionArray[1]	= 0;
		//m_bAttack				= false;
		for ( int i = 0; i < 3; i++ )
		{
			if ( 6 == m_iLevelType )
			{
				if ( !getChildByTag(20+i)->isVisible() )
					continue;
			}
			bool press = false;
			bool out = false;
			int digit = 0;
			CCSetIterator iter = pTouches->begin();
			for (; iter != pTouches->end(); iter++/*, digit++*/)
			{
				CCPoint location = ((CCTouch*)(*iter))->getLocation();
				if ( fabs(location.x - g_fButtonX[i]*Scale_X) < g_fButtonW[i]*Scale_X
					&& fabs(location.y - g_fButtonY[i]*Scale_Y) < g_fButtonH[i]*Scale_Y )
				{
					if ( i == 2 )
					{
						if ( (location.x - g_fButtonX[i]*Scale_X)*(location.x - g_fButtonX[i]*Scale_X) +
							(location.y - g_fButtonY[i]*Scale_Y)*(location.y - g_fButtonY[i]*Scale_Y) > 
							g_fButtonW[i]*Scale_X*g_fButtonW[i]*Scale_X )
						{
							out = true;
							continue;
						}
					}
					press = true;
					digit = (*iter)->m_uID;
				}
				else if ( fabs(location.x - g_fButtonX[i]*Scale_X) < (g_fButtonW[i]+250)*Scale_X
					&& fabs(location.y - g_fButtonY[i]*Scale_Y) < (g_fButtonH[i]+480)*Scale_Y )
				{
					out = true;
				}
			}
			if ( press )
			{
				switch ( i )
				{
				case 0:
				case 1:
					m_pMoveActionArray[i] = digit;
					m_pMoveActionArray[1-i] = 0;
					if ( m_pMoveActionArray[0] && bMoveL )
						bMoveL = false;
					if ( m_pMoveActionArray[1] && bMoveR )
						bMoveR = false;
					break;
				case 2:
					m_bAttack = true;
					m_pMoveActionArray[2] = digit;
					break;
				}
			}
			else if ( out )
			{
				switch ( i )
				{
				case 0:
				case 1:
					m_pMoveActionArray[i] = 0;
					break;
				case 2:
					m_bAttack = false;
					m_pMoveActionArray[2] = 0;
					break;
				}
			}
		}
		if ( (!bMoveL || !bMoveR) && m_iAttackCoolTime > 0 )
			m_iJumpActionType = 3;
	}
}

void CGameControler::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	DIS_SELF(m_iPressCount);
	//if ( m_bPause )
	//	return;
	bool bMoveL = false;
	bool bMoveR = false;
	if ( m_pMoveActionArray[0] )
		bMoveL = true;
	if ( m_pMoveActionArray[1] )
		bMoveR = true;
	int digit = 0;
	CCSetIterator iter = pTouches->begin();
	for (; iter != pTouches->end(); iter++/*, digit++*/)
	{
		CCPoint location = ((CCTouch*)(*iter))->getLocation();
		digit = (*iter)->m_uID;
		if ( digit == m_unMoveEventID && !m_bBossWin )
		{
			m_unMoveEventID = 0;
			bool change = false;
			if ( m_ewtNowWeapon == eWT_Knife )
			{
				if ( m_pWeaponMark1->getRotation() < -10 && m_iHoldTime == 0 )
				{
					m_pWeaponMark1->setVisible(false);
					m_pWeaponMark2->setVisible(true);
					m_pWeaponMark2->setRotation(0);
					m_ewtNowWeapon = eWT_Pistol;
	//				if ( m_iGuideTalk == 8 && getChildByTag(196) )
	//				{
	//					removeChildByTag(193);
	//					GuideResume();
	//					GuideTalk(9);
	///*					removeChildByTag(193);
	//					CCSprite* pTipBoard = common::CreateAnimation("guide/shang1.png", ccp(645,110), ccp(0.5f, 0.5f),
	//						"guide/shang", 2, CCRect(0,0,137,149), true, false, 0.3f);
	//					addChild(pTipBoard, 31, 193);*/
	//				}
	//				else
						change = true;
				}
				else
				{
					m_pWeaponMark1->setRotation(0);
				}
			}
			else
			{
				if ( m_pWeaponMark2->getRotation() > 10 && m_iHoldTime == 0 )
				{
					m_pWeaponMark2->setVisible(false);
					m_pWeaponMark1->setVisible(true);
					m_pWeaponMark1->setRotation(0);
					m_ewtNowWeapon = eWT_Knife;
					//if ( m_iGuideTalk == 8 && getChildByTag(196) )
					//{
					//	removeChildByTag(193);
					//	GuideResume();
					//	GuideTalk(9);
					//}
					//else
						change = true;
				}
				else
				{
					m_pWeaponMark2->setRotation(0);
				}
			}
			if ( change )
			{
				if ( m_ewtNowWeapon != eWT_Knife && ( m_iHeroJumps < 2 || m_fHeroSpeedY < 2.0f ))
				{
					m_pHero->ChangeWeapon(m_iWeaponUse[0]);
					m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
					if ( m_iWeaponUse[1] == 12 && !m_bBossWin )
						m_iFireTime = 1;
				}
				else
				{
					m_iFireTime = 0;
					removeChildByTag(805);
				}
				if ( m_iLevelType == 1 || m_iLevelType == 3 )
				{
					if ( m_ewtNowWeapon == eWT_Knife )
						((ccbControlMenu*)m_pMenuNode)->SetType(2);
					else
						((ccbControlMenu*)m_pMenuNode)->SetType(3);
				}
				else
				{
					if ( m_ewtNowWeapon == eWT_Knife )
						((ccbControlMenu*)m_pMenuNode)->SetType(0);
					else
						((ccbControlMenu*)m_pMenuNode)->SetType(1);
				}
				g_pHero->m_unNowWeapon = m_ewtNowWeapon;
			}
		}
		//else
		{
			e_ActionType iActionType = eAT_NULL;
			for ( int i = 0; i < d_iActionNum; i++ )
			{
				if ( fabs(location.x - g_fButtonX[i]*Scale_X) < g_fButtonW[i]*Scale_X
					&& fabs(location.y - g_fButtonY[i]*Scale_Y) < g_fButtonH[i]*Scale_Y
					 /*digit == m_pMoveActionArray[i]*/ )
				{
					iActionType = (e_ActionType)i;
					m_pMoveActionArray[i] = 0;
					break;
				}
			}
			switch ( iActionType )
			{
			case eAT_MoveLeft:
			case eAT_MoveRight:
				m_iMoveDigit			= 0;
				m_pMoveActionArray[0]	= 0;
				m_pMoveActionArray[1]	= 0;
				m_fHeroSpeedX			= 0;
				if ( !m_pMoveActionArray[0] && bMoveL )
					bMoveL = false;
				if ( !m_pMoveActionArray[1] && bMoveR )
					bMoveR = false;
				break;
			case eAT_Attack:
				m_bAttack		= false;
				m_iAttackDigit	= 0;
				m_pMoveActionArray[2]	= 0;
				break;
			default:
				break;
			}
		}
	}
	if ( m_iPressCount == 0 )
	{
		m_iMoveDigit			= 0;
		m_pMoveActionArray[0]	= 0;
		m_pMoveActionArray[1]	= 0;
		m_fHeroSpeedX			= 0;
		if ( !m_pMoveActionArray[0] && bMoveL )
			bMoveL = false;
		if ( !m_pMoveActionArray[1] && bMoveR )
			bMoveR = false;
		m_bAttack		= false;
		m_iAttackDigit	= 0;
		m_pMoveActionArray[2]	= 0;
	}
	if ( (!bMoveL || !bMoveR) && m_iAttackCoolTime > 0 )
		m_iJumpActionType = 3;
}

void CGameControler::HealEffectOver1()
{
	m_pHealEffect1->removeFromParent();
	m_pHealEffect1 = NULL;
}

void CGameControler::HealEffectOver2()
{
	m_pHealEffect2->removeFromParent();
	m_pHealEffect2 = NULL;

	if ( m_pHealParticle )
		m_pHealParticle->removeFromParent();

	m_pHealParticle = CCParticleSystemQuad::create("Animation/heal/jiaxue.plist");
	m_pHealParticle->setPosition( ccp(m_fHeroLocateX, m_fHeroLocateY+d_fHeroHeight) );
	m_pCFrontgroud->GetTileMap()->addChild(m_pHealParticle, 6);
	scheduleOnce(schedule_selector(CGameControler::HealParticleOver), 1.5f);
}

void CGameControler::HealParticleOver( float _t )
{
	if ( !m_pHealParticle )
		return;

	m_pHealParticle->removeFromParent();
	m_pHealParticle = NULL;
}

void CGameControler::ButtonPressCheck()
{
	if ( 6 == m_iLevelType )
	{
		for ( int i = 0; i < 8; i++ )
		{
			if ( i == 5 || i == 4 )
				continue;
			if ( i == 6 && m_iGrenadeNum == 0 )
				continue;
			bool bShow = true;
			bool bOpen = false;
			switch ( i )
			{
			case 0:
			case 1:
				if ( m_iGuideBtn > 0 )
					bOpen = true;
				break;
			case 2:
				if ( m_iGuideBtn > 2 )
					bOpen = true;
				break;
			case 3:
				if ( m_iGuideBtn > 1 )
					bOpen = true;
				break;
			case 6:
				if ( m_iGuideBtn > 4 )
					bOpen = true;
				break;
			case 7:
				if ( m_iGuideBtn > 6 )
					bOpen = true;
				break;
			}
			if ( i < 2 )
			{
				if ( m_pMoveActionArray[i] )
					bShow = false;
			}
			else if ( i == 2 )
			{
				if ( m_bAttack )
					bShow = false;
			}
			else if ( m_pMoveActionArray[i] > 0 )
			{
				m_pMoveActionArray[i]--;
				bShow = false;
			}
			getChildByTag(20+i)->setVisible(bShow&bOpen);
			getChildByTag(40+i)->setVisible(!bShow&bOpen);
		}
		if ( m_iGuideBtn < 4 )
		{
			m_pWeaponMark1->setVisible(false);
			m_pWeaponMark2->setVisible(false);
		}
		if ( m_iGuideBtn < 6 )
		{
			getChildByTag(25)->setVisible(false);
		}
		else
		{
			if ( m_iStateSkill[0] >= 100 )
				getChildByTag(25)->setVisible(true);
		}
	}
	else for ( int i = 0; i < d_iActionNum; i++ )
	{
		if ( i == 5 || i == 4 )
			continue;
		if ( i == 6 && m_iGrenadeNum == 0 )
			continue;
		bool bShow = true;
		if ( i < 2 )
		{
			if ( m_pMoveActionArray[i] )
				bShow = false;
		}
		else if ( i == 2 )
		{
			if ( m_bAttack )
				bShow = false;
		}
		else if ( m_pMoveActionArray[i] > 0 )
		{
			m_pMoveActionArray[i]--;
			bShow = false;
		}
		getChildByTag(20+i)->setVisible(bShow);
		getChildByTag(40+i)->setVisible(!bShow);
	}
}

template<class T>
bool CGameControler::SetVisible( T* _p, float _x, float _y)
{
	if ( fabs(_x - m_fHeroLocateX + m_fRelativeX - 400) < 420.0f && fabs(_y - m_fHeroLocateY) < 480.0f )
	{
		_p->setVisible(true);
		return true;
	}
	else if ( ( fabs(_x - m_fHeroLocateX + m_fRelativeX - 400) < 800.0f && fabs( _y - m_fHeroLocateY ) < 600.0f  )
		|| 2 == m_iLevelType )
	{
		_p->setVisible(false);
		return true;
	}
	else
	{
		_p->setVisible(false);
		return false;
	}
}

void CGameControler::MoveCheck(int _un, int _digit)
{
	if ( !(m_pMoveActionArray[0] | m_pMoveActionArray[1]) )
	{
		m_pMoveActionArray[_un]	= _digit;
		//int	a = _un*2-1;
		//int b = 0;
	}
	if ( m_iGuideTalk == 3 && getChildByTag(196) )
	{
		m_iGuideMap = guideMap[1]+800;
		m_iGuideAction = m_iGuideNow+1;
		GuideResume();
	}
}

void CGameControler::TimerStart( float _dt )
{
	if ( m_fHeroSpeedY > 0 && CheckInWall( m_fHeroLocateX, m_fHeroLocateY+m_fHeroSpeedY, d_fHeroCrashCX, d_fHeroHeight) )
		m_fHeroSpeedY	= 0.0;
	if ( m_fHeroSpeedY > 0 || !CheckInWall( m_fHeroLocateX, m_fHeroLocateY-0.8, d_fHeroCrashCX, d_fHeroHeight ) )
	{
		if ( CheckInWall( m_fHeroLocateX, m_fHeroLocateY-15.0, d_fHeroCrashCX, d_fHeroHeight ) )
		{
			m_iHeroJumps	= 0;
			m_fHeroSpeedY	= 0.0;
			m_bDrop = false;
		}
		else
			m_bDrop = true;
		m_fHeroSpeedY -= d_fHeroGravity;
		if ( m_fHeroSpeedY < -d_fHeroJump*2 )
			m_fHeroSpeedY = -d_fHeroJump*2;
	}
	else
	{
		m_iHeroJumps	= 0;
		m_fHeroSpeedY	= 0.0;
		m_bDrop			= false;
	}
	if ( !m_bDrop && !m_bFirstDown )
	{
		m_bFirstDown = true;
		m_iShakeTime = 10;

		CCSprite* pSprite = common::CreateAnimation("Animation/boss1skill2/1.png", ccp(m_fHeroLocateX, m_fHeroLocateY), ccp(0.56f, 0.31f),
			"Animation/boss1skill2/", 6, CCRect(0,0,220,181), false, false, 0.1f, 
			CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
		pSprite->setScaleX(m_fHeroToward);
		m_pCFrontgroud->GetTileMap()->addChild(pSprite, 4);
	}
	m_fHeroLocateY += m_fHeroSpeedY;
	CheckCrash(m_fHeroLocateX, m_fHeroLocateY, d_fHeroCrashCX, d_fHeroHeight, d_fHeroCenterY);
	SetLocation();
	HeroAction();
	if ( m_iShakeTime > 0 )
	{
		m_iShakeTime--;
		if ( m_iShakeTime%2 == 0 )
		{
			if ( !m_iShakeTime )
				m_pCFrontgroud->GetTileMap()->setPositionX(0);
			else
			{
				if ( m_iShakeLocate >= 0 )
					m_iShakeLocate = -rand()%10;
				else
					m_iShakeLocate =rand()%10;
				m_pCFrontgroud->GetTileMap()->setPositionX(m_iShakeLocate);
			}
		}
	}
}

void CGameControler::Timer(float _dt)
{
	//////////////
	/****暂停****/
	//////////////
	if ( m_bPause || m_bGuidePause || m_bSellOpen || m_bExitGame /*|| m_bShakeStop*/ )
		return;

	ShakeWindow();
	ShowScore();

	if ( m_bSkillPause )
		return;
	if ( SkillCheck() )
		return;

	//////////////////
	/****BOSS移动****/
	//////////////////
	if ( m_bBossPause )
	{
		float x = d_fMapSizeX * (m_iMapNum-1.00f);
		if ( m_fHeroLocateX - m_fRelativeX < x )
		{
			if ( fabs(m_fHeroLocateX - m_fRelativeX - x) < 3.0f )
				m_fRelativeX = m_fHeroLocateX - x;
			else
				m_fRelativeX -= 3.0f;
			SetLocation();
		}
		return;
	}

	//////////////////
	/****过关移动****/
	//////////////////
	if ( m_bBossWin )
	{
		BossBox();
		m_iWinTime--;
		if ( m_bBossBox )
		{
			Win(0.0f);
		}
	}
	else if ( m_bWin && m_iWinTime > 0 )
	{
		m_iWinTime--;
		if ( 0 == m_iWinTime )
			Win(0.0f);
	}

	if ( m_bOver && m_bOverMove )
	{
		if ( m_pFireSprite )
		{
			m_pFireSprite->removeFromParentAndCleanup(true);
			m_pFireSprite = NULL;
		}
		m_fHeroLocateX += m_fHeroSpeedXRes;
		
		m_pHero->GetArmature()->setPosition(m_fHeroLocateX, m_fHeroLocateY);
		m_pHero->GetArmature()->setScale(1);
		if ( m_ewtNowWeapon == eWT_Knife )
			m_pHero->RunAction(m_iKnifeAction[1]);
		else
			m_pHero->RunAction(2);
		WinCheck();
		return;
	}
	//////////////////
	/****游戏时间****/
	//////////////////
	SupplyCheck();
	if ( !m_bDead && !m_bLimitStart )
	{
		m_iGameSecond ++;
		if ( m_iGameSecond == 60*MAINFRAME )
		{
			m_iGameSecond = 0;
			m_iGameMinute ++;
		}
		EffectTime();
		SkillTimer();

		if ( m_iLevelType == 1 || m_iLevelType == 3 )
			LimitUpdata();

		//////////////////
		/****按键检测****/
		//////////////////
		ButtonPressCheck();
	}
	else if ( m_bLimitStart )
	{
		LimitInit();
		//if ( LimitInit() )
		//{
		//	m_bLimitStart = false;
		//	m_pLimitBoard->setScale(1.5f);
		//	CCMoveTo* pAction1 = CCMoveTo::create(0.3f, ccp(230, 428));
		//	CCScaleTo* pAction2 = CCScaleTo::create(0.3f, 0.65f);
		//	m_pLimitBoard->runAction(pAction1);
		//	m_pLimitBoard->runAction(pAction2);
		//}
	}
	//////////////
	/****重力****/
	//////////////
	m_bIsMoveObject = false;
	m_bInSand		= false;
	m_iUnderAttacked= 1;
	if ( m_fHeroSpeedY > 0 && CheckInWall( m_fHeroLocateX, m_fHeroLocateY+m_fHeroSpeedY, d_fHeroCrashCX, d_fHeroHeight) )
		m_fHeroSpeedY	= 0.0;

	bool check = !CheckInWall( m_fHeroLocateX, m_fHeroLocateY-0.8, d_fHeroCrashCX, d_fHeroHeight );
	if ( m_fHeroSpeedY > 0 || check )
	{
		if ( CheckInWall( m_fHeroLocateX, m_fHeroLocateY-15.0, d_fHeroCrashCX, d_fHeroHeight ) )
		{
			m_bDrop = false;
		}
		else if ( !m_bInSand )
		{
			m_bIsMoveObject = true;
			m_bDrop = true;
		}
		else
			m_bDrop = false;
		m_fHeroSpeedY -= d_fHeroGravity;
		if ( m_fHeroSpeedY < -24.0f )
			m_fHeroSpeedY = -24.0f;
	}
	else
	{
		if ( !m_bIsMoveObject && m_fHeroSpeedY == 0 )
		{
			m_fOnLandLocateX = m_fHeroLocateX;
			m_fOnLandLocateY = m_fHeroLocateY;
		}
		if ( m_fHeroSpeedY < 0.0f )
			m_iJumpActionType = 1;
		if ( m_iHeroJumps )
		{
			CCSprite* pSprite = common::CreateAnimation("Animation/JumpOver/1.png", ccp(m_fHeroLocateX, m_fHeroLocateY), ccp(0.56f, 0.1f),
				"Animation/JumpOver/", 5, CCRect(0,0,90,89), false, false, 0.1f, 
				CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
			pSprite->setScaleX(m_fHeroToward);
			m_pHero->addChild(pSprite, 4);
		}
		m_bDrop			= false;
		m_iHeroJumps	= 0;
		m_fHeroSpeedY	= 0.0;
		WinCheck();
		GuideCheck();
	}

	//////////////////////
	/****场景道具运动****/
	//////////////////////
	m_bEffect1 = false;
	m_bEffect2 = false;
	m_bEffect3 = false;
	m_bEffect4 = false;
	m_bEffect5 = false;
	m_bEffect6 = false;
	m_bPrisonWater = false;
	if ( m_iPrisonTime > 0 )
	{
		if ( m_iPrisonTime%10 == 0 )
			m_iStateHP--;
		m_iPrisonTime--;
		CreateInPrisonPlist();
	}
	bool bMoveType = 0;
	GrenadeRun();
	ObjectRun();
	if ( !m_bIsMoveObject )
	{
		m_fAliveLocateX = m_fOnLandLocateX;
		m_fAliveLocateY = m_fOnLandLocateY;
	}
	m_fHeroSpeedX = 0;
	if ( !m_bDead && !m_bLimitStart )
	{
		m_bBoxBrokenEffect = true;
		m_bBeAttackedEffect = true;
		m_bMonsterCry = true;
		AttackRun();
		ItemCheck();
		if ( !m_bWin && !m_bBossWin )
		{
			MonsterFresh();
			MonsterRun();
			MonsterAttackRun();
		}
		AttackCheck();
		CreateCombo();
		//////////////
		/****运动****/
		//////////////
		if ( m_iGrenadeTime > 0 )
		{
			m_iGrenadeTime--;
			if ( ( m_iGrenadeTime == 26 && AppDelegate::s_HeroType != 2 ) ||
				( m_iGrenadeTime == 10 && AppDelegate::s_HeroType == 2 ) )
			{
				GrenadeCreate();
			}
		}
		else if ( !m_iHeroJumps && m_iAttackCoolTime > 0 && 
			m_ewtNowWeapon == eWT_Knife )
			m_fHeroSpeedX = 0;
		else if ( m_iHoldTime == 0 /*m_ewtNowWeapon != eWT_Knife || ( m_ewtNowWeapon == eWT_Knife && !m_iAttackCoolTime )*/ )
		{
			if ( m_iFrozeTime > 0 )
			{
				m_iUnderAttacked = 3.0f;
				m_iFrozeTime--;
			}
			else if ( AppDelegate::s_HeroType == 2 )
				m_iUnderAttacked = 1.0f;
			if ( m_pMoveActionArray[0] && m_fRelativeX > d_fCantMove )
			{ 
				m_fHeroSpeedX = -m_fHeroSpeedXRes/m_iUnderAttacked;
				if ( m_fRelativeX > 800/2 || m_fHeroLocateX <= d_fMapSizeX / 2 )
					bMoveType = true;
			}
			else if ( m_pMoveActionArray[1] && m_fRelativeX < d_fMapSizeX - d_fCantMove )
			{
				m_fHeroSpeedX = m_fHeroSpeedXRes/m_iUnderAttacked;
				if ( m_fRelativeX < 800/2 || m_fHeroLocateX >= m_iGuideMap - 400 )
					bMoveType = true;
			}
			else
				m_fHeroSpeedX = 0;
			if ( m_pMoveActionArray[0] )
				m_fHeroToward = -1;
			else if ( m_pMoveActionArray[1] )
				m_fHeroToward = 1;
		}
	}

	//////////////
	/****移动****/
	//////////////
	m_fAimLocateX	= m_fHeroLocateX + 100;
	if ( m_ewtNowWeapon != eWT_Knife )
	{
		float aimy;
		if ( m_iRamp )
			aimy = d_iGraviTime*m_iRamp;
		else
			aimy = d_iGraviTime*m_fHeroSpeedY;
		m_fAimLocateY	= m_fHeroLocateY - aimy ;
	}
	else
		m_fAimLocateY	= m_fHeroLocateY;
	m_fHeroLastX = m_fHeroLocateX;
	m_fHeroLastY = m_fHeroLocateY;
	m_fHeroLocateY += m_fHeroSpeedY;
	if ( m_iHoldTime > 6 )
	{
		if ( m_fHeroToward == 1 && m_fRelativeX > 80 ){
			m_fHeroSpeedX = -8.0f;
			
		}else if ( m_fHeroToward == -1 && m_fRelativeX < 720 ){
			m_fHeroSpeedX = 8.0f;
			
		}
		bMoveType = true;
	}
	//int pz_speed = m_fHeroSpeedX;
	if(m_iHeroJumps){
		m_fHeroSpeedX = m_fHeroSpeedX*2;
	}
	if ( !CheckCrash(m_fHeroLocateX+m_fHeroSpeedX, m_fHeroLocateY, d_fHeroCrashCX, d_fHeroHeight, d_fHeroCenterY) && m_fHeroSpeedX )
	{
		if ( m_pHealEffect1 )
		{
			//m_pHealEffect1->setPositionX(d_fHealEffectToward*m_fHeroToward-d_fHealEffectToward);
			//m_pHealEffect2->setPositionX(d_fHealEffectToward*m_fHeroToward-d_fHealEffectToward);
			m_pHealEffect1->setScaleX(m_fHeroToward);
			m_pHealEffect2->setScaleX(m_fHeroToward);
		}

		m_fHeroLocateX	+= m_fHeroSpeedX;
		if ( bMoveType || m_bBoss )
			m_fRelativeX += m_fHeroSpeedX;
	}
	if ( m_pHealParticle )
	{
		float x = m_fHeroLastX - m_fHeroLocateX;
		float y = m_fHeroLastY - m_fHeroLocateY;
		
		m_pHealParticle->setPosition(CCPoint(m_fHeroLocateX+10*x, m_fHeroLocateY+d_fHeroHeight+10*y));
		
	}
	if ( m_pGetItemEffect )
		m_pGetItemEffect->setPosition( ccp(m_fHeroLocateX, m_fHeroLocateY) );

	//////////////
	/****攻击****/

	//////////////
	if ( m_bAttack ){
		CreateAttack();
	}

	//////////////
	/****最终****/
	//////////////
	NeedKillShow();
	SetLocation();
	if ( !m_bDead )
	{
		if ( m_iAttackCoolTime >= 0 )
			m_iAttackCoolTime --;
		if ( 2 == m_iLevelType )
			VIPCheck();
		HeroAim();
		HeroAction();
		DeadCheck();
		if ( m_iFrozeTime > 0 )
			m_pHero->GetArmature()->setColor(ccc3(0,0,200));
		else if ( m_iPrisonTime > 0 || m_bPrisonWater )
			m_pHero->GetArmature()->setColor(ccc3(0,200,0));
		else
			m_pHero->GetArmature()->setColor(ccc3(255,255,255));
	}
	keyPressed = false;
	keyCode = 0;
}

void CGameControler::SetLocation()
{
	if ( m_iBossLimitX > 0 )
		if ( m_iBossLimitX < m_fHeroLocateX )
		{
			float dist = m_fHeroLocateX - m_iBossLimitX;
			m_fHeroLocateX = m_iBossLimitX;
			m_fRelativeX -= dist;
		}
	m_pHero->GetArmature()->setPosition(m_fHeroLocateX, m_fHeroLocateY);
	if ( m_bDead )
		return;
	if ( m_pFireSprite )
	{
		m_pFireObject->toward	= m_fHeroToward;
		m_pFireSprite->setScaleX(m_fHeroToward);
		//m_pFireObject->rotation	= m_pHero->GetRotation();
		//m_pFireObject->pSprite->setRotation( -m_pFireObject->rotation * m_pFireObject->toward );
		//m_pFireObject->rotation = (float)(m_pFireObject->rotation * PI_FIX);
		m_pFireObject->x		= m_fHeroLocateX + ( d_fWeaponCenterX+m_iFireX[m_ewtNowWeapon-1]/**cosf(m_pFireObject->rotation)*/ )*m_fHeroToward;
		{
			float heightFix = 0.0;
			if ( m_iHeroJumps > 0 || m_fHeroSpeedY < -4.0 )
				heightFix = d_fJumpHigher;
			else if ( (m_pMoveActionArray[0] || m_pMoveActionArray[1]) && AppDelegate::s_HeroType == 0 )
				heightFix = d_fRunHigher;
			if ( AppDelegate::s_HeroType != 2 )
				m_pFireObject->y	= m_fHeroLocateY+m_iFireY[m_ewtNowWeapon-1]+/*m_iFireY[m_ewtNowWeapon-1]*sinf(pAttackingObject->rotation)*/ + heightFix;
			else
				m_pFireObject->y	= m_fHeroLocateY+g_fFireY3 - 28;
		}
		m_pFireSprite->setPosition( ccp(m_pFireObject->x, m_pFireObject->y) );
	}
	m_pHero->GetArmature()->setScaleX(m_fHeroToward);
	m_pCBackgroud->setPositionX(Scale_X*(int((m_fRelativeX-m_fHeroLocateX)/2)%1600));
	//m_pCMidground->setPositionX(Scale_X*(m_fRelativeX-m_fHeroLocateX)/**3/(m_iMapNum-1)*/);
	m_pCFrontgroud->setPositionX(Scale_X*(m_fRelativeX-m_fHeroLocateX));
	if ( m_pHealEffect1 )
		m_pHealEffect1->setPosition(CCPoint(m_fHeroLocateX, d_fHealEffectFixY+m_fHeroLocateY));
	if ( m_pHealEffect2 )
		m_pHealEffect2->setPosition(CCPoint(m_fHeroLocateX, d_fHealEffectFixY+m_fHeroLocateY));
	if ( m_pRelife )
		m_pRelife->setPosition(ccp(m_fHeroLocateX, m_fHeroLocateY));

	if ( m_bBoss )
	{
		if ( m_unLevel != 30 )
		{
			m_pCBackgroud->setPositionY(0);
			m_pCFrontgroud->setPositionY(0);
			m_iYFix = 0;
		}
		else
		{
			m_pCBackgroud->setPositionY(-Scale_Y*3360);
			m_pCFrontgroud->setPositionY(-Scale_Y*3360);
		}
	}
	else if ( m_fHeroLocateY > Height_JUMP + m_iYDist )
	{
		m_pCBackgroud->setPositionY(Scale_Y*(Height_JUMP + m_iYDist - m_fHeroLocateY)/4.0f);
		//m_pCMidground->setPositionY(Scale_Y*(Height_JUMP - m_fHeroLocateY));
		m_pCFrontgroud->setPositionY(Scale_Y*(Height_JUMP - m_fHeroLocateY));
		m_iYFix = Height_JUMP - m_fHeroLocateY;
	}
	else if ( m_fHeroLocateY > m_iYDist )
	{
		m_pBG->setVisible(true);
		m_pCMidground->setVisible(true);
		m_pCBackgroud->setPositionY(0);
		//m_pCMidground->setPositionY(-Scale_Y*m_iYDist);
		m_pCFrontgroud->setPositionY(-Scale_Y*m_iYDist);
		m_iYFix = -m_iYDist;
	}
	else if ( m_unLevel > 48 )
	{
		if ( m_fHeroLocateY > m_iYDist - 350 )
		{
			m_pBG->setVisible(false);
			m_pCBackgroud->GetTileMap()->setVisible(false);
			m_pCMidground->setVisible(false);
			m_pCFrontgroud->setPositionY(Scale_Y*(480-m_iYDist));
			m_iYFix = 480-m_iYDist;
		}
		else if ( m_fHeroLocateY > 130 )
		{
			m_pCFrontgroud->setPositionY(Scale_Y*(130-m_fHeroLocateY));
			m_iYFix = 130-m_fHeroLocateY;
		}
		else
		{
			m_pCFrontgroud->setPositionY(0);
			m_iYFix = 0;
		}
	}

	if ( m_pWinMark )
	{
		if ( m_fHeroLocateX - m_fRelativeX < d_fMapSizeX*(m_iMapNum-1)-200 )
			m_pWinMark->setVisible(false);
		else
			m_pWinMark->setVisible(true);
	}
}

void CGameControler::ShakeWindow()
{
	if ( !m_iShakeTime || m_bPause || m_bSellOpen || m_bBossWin )
		return;
	m_iShakeTime--;
	if ( m_iShakeTime%2 == 0 )
	{
		if ( !m_iShakeTime )
		{
			m_pCMidground->setPositionX(0);
			m_pCFrontgroud->GetTileMap()->setPositionX(0);
		}
		else
		{
			if ( m_iShakeLocate >= 0 )
				m_iShakeLocate = -rand()%10;
			else
				m_iShakeLocate =rand()%10;
			m_pCMidground->setPositionX(m_iShakeLocate);
			m_pCFrontgroud->GetTileMap()->setPositionX(m_iShakeLocate);
		}
	}
}

bool CGameControler::CheckCrash(float _fHeroX, float& _fHeroY, float _w, float _h, float _cy, bool _bHero)
{
	bool flag = false;
	float fMax = -100.0;
	CCArray* pArray = NULL;

	//////////////////
	/****运动物体****/
	//////////////////
	float fYMove = 0;
	MapObject* pObjectArray = m_pCFrontgroud->GetMapObjects();
	int unObjectNum = m_pCFrontgroud->GetObjectNum();
	for ( int i = 0; i < unObjectNum; i++ )
	{
		MapObject* pMapObject = &pObjectArray[i];
		if ( /*pMapObject->unKind != 0 && pMapObject->unKind != 2 && pMapObject->unKind != 3 && pMapObject->unKind != 4 && 
			 pMapObject->unKind != 6 && pMapObject->unKind != 9 && pMapObject->unKind != 10 && pMapObject->unKind != 11 &&
			 pMapObject->unKind != 13*/ 
			 !pMapObject->bCheck )
			continue;
		if ( _fHeroX+_w <= pMapObject->x || _fHeroX-_w >= pMapObject->x+pMapObject->w  )
			continue;
		//if ( pMapObject->unKind == 9 || pMapObject->unKind == 10 || pMapObject->unKind == 0 )
		//{
		//	if ( !pMapObject->argb )
		//		continue;
		//}
		if ( RectInRect( _fHeroY, _h, pMapObject->y, pMapObject->h ) )
		{
			if ( pMapObject->unKind == 13 )
				m_bIsMoveObject = true;
			else if ( pMapObject->unKind == 19 )
				return true;
			fMax = pMapObject->y + pMapObject->h + _cy + d_fHeroGravity ;
			break;
		}
	}

	//////////////
	/****三角****/
	//////////////
	m_iRamp = 0;
	if ( fMax < 0.0 )
	{
		pArray = m_pCFrontgroud->GetCrashTriaArray();
		for ( unsigned int i = 0; i < pArray->count(); i++ )
		{
			CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
			float x		= pObject->valueForKey("x")->floatValue();
			float cx	= pObject->valueForKey("width")->floatValue();
			if ( _fHeroX+_w <= x || _fHeroX-_w >= x+cx  )
				continue;

			int unSide = pObject->valueForKey("poid")->uintValue();
			float distX;
			if ( unSide)
				distX	= x + cx - _fHeroX ;
			else
				distX	= _fHeroX - x;
			float y		= pObject->valueForKey("y")->floatValue()-2;
			float cy	= pObject->valueForKey("height")->floatValue();
			float distY = distX * cy / cx;
			float compY	= _fHeroY - y;
			if ( RectInRect( compY, _h, 0, distY ) )
			{
				float buffer = y + distY + _cy + d_fHeroGravity;
				fMax = buffer > fMax ? buffer : fMax;
				if ( m_fHeroSpeedY < -3 )
					m_iRamp = 0;
				else
					m_iRamp = 1;
				break;
			}
			else if ( compY - distY < d_fRampHeight )
				if ( m_pMoveActionArray[0] || m_pMoveActionArray[1] )
					m_iRamp	= -1;
				else
					m_iRamp = 0;
		}
	}

	//////////////
	/****方形****/
	//////////////
	if ( fMax < 0.0 )
	{
		pArray = m_pCFrontgroud->GetCrashRectArray();
		for ( unsigned int i = 0; i < pArray->count(); i++ )
		{
			CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
			float x = pObject->valueForKey("x")->floatValue();
			float w = pObject->valueForKey("width")->floatValue();
			if ( _fHeroX+_w <= x || _fHeroX-_w >= x+w  )
				continue;

			float y = pObject->valueForKey("y")->floatValue();
			float h = pObject->valueForKey("height")->floatValue();
			if ( RectInRect( _fHeroY, _h, y, h ) )
			{
				float buffer = y + h + _cy + d_fHeroGravity;
				fMax = buffer > fMax ? buffer : fMax;
				break;
			}
		}
	}

	if ( fMax > 0.0 )
	{
		if ( (fMax > _fHeroY && fMax-_fHeroY < d_fSpan /*&& _fHeroY > 0*/) || !_bHero )
		{
			_fHeroY	= fMax;
		}
		else
			flag = true;
	}

	return flag;
}

bool CGameControler::CheckInWall(float _x, float _y, float _w, float _h)
{
	//////////////////
	/****运动物体****/
	//////////////////
	float fYMove = 0;
	MapObject* pObjectArray = m_pCFrontgroud->GetMapObjects();
	int unObjectNum = m_pCFrontgroud->GetObjectNum();
	for ( int i = 0; i < unObjectNum; i++ )
	{
		MapObject* pMapObject = &pObjectArray[i];
		if ( !pMapObject->bCheck && pMapObject->unKind != 19 )
			continue;
		if ( _x+_w < pMapObject->x || _x-_w > pMapObject->x+pMapObject->w  )
			continue;
		if ( RectInRect(_y, _h, pMapObject->y, pMapObject->h) )
		{
			m_bIsMoveObject = true;
			//if ( pMapObject->unKind == 13 )
			//	m_bIsMoveObject = true;
			if ( pMapObject->unKind != 19 )
				return true;
			else
			{
				if ( m_fHeroLocateY < 30 )
				{
					m_iHeroJumps = m_iJumpCanTimes;
				}
				else if ( m_fHeroSpeedY < 0 )
				{
					m_fHeroSpeedY = 0;
					m_iHeroJumps = 0;
					m_iUnderAttacked = 2;
					m_bInSand = true;
				}
				return false;
			}
		}
	}

	//////////////
	/****方形****/
	//////////////
	CCArray* pArray = NULL;
	pArray = m_pCFrontgroud->GetCrashRectArray();
	for ( unsigned int i = 0; i < pArray->count(); i++ )
	{
		CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
		float x = pObject->valueForKey("x")->floatValue();
		float cx = pObject->valueForKey("width")->floatValue();
		if ( _x+_w <= x || _x-_w >= x+cx  )
			continue;

		float y = pObject->valueForKey("y")->floatValue();
		float cy = pObject->valueForKey("height")->floatValue();
		if ( RectInRect(_y, _h, y, cy)
			/*(_y/ *-_h* / >= y && _y/ *-_h* / <= y+cy)
			|| (_y+_h >= y && _y+_h <= y+cy) 
			|| ( y > _y/ *-_h* / && y < _y+_h ) 
			|| ( y+cy > _y/ *-_h* / && y+cy < _y+_h )*/ )
		{
			return true;
		}
	}

	//////////////
	/****三角****/
	//////////////
	pArray = m_pCFrontgroud->GetCrashTriaArray();
	for ( unsigned int i = 0; i < pArray->count(); i++ )
	{
		CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
		float x		= pObject->valueForKey("x")->floatValue();
		float cx	= pObject->valueForKey("width")->floatValue();
		if ( _x+_w <= x || _x-_w >= x+cx  )
			continue;

		int unSide = pObject->valueForKey("poid")->uintValue();
		float distX;
		if ( unSide)
			distX	= x + cx - _x ;
		else
			distX	= _x - x;
		float y		= pObject->valueForKey("y")->floatValue();
		float cy	= pObject->valueForKey("height")->floatValue();
		float distY = distX * cy / cx;
		float compY	= _y - y;
		if ( RectInRect( compY, _h, 0, distY )
			/*(compY/ *-_h* / >= 0 && compY/ *-_h* / <= distY)
			|| (compY+_h >= 0 && compY+_h <= distY) 
			|| ( 0 > _y/ *-_h* / && 0 < _y+_h ) 
			|| ( distY > _y/ *-_h* / && distY < _y+_h )*/ )
		{
			return true;
		}
	}

	return false;
}

void CGameControler::ObjectRun()
{
	//////////////////
	/****场景物品****/
	//////////////////
	MapObject* pObjectArray = m_pCFrontgroud->GetMapObjects();
	int unObjectNum = m_pCFrontgroud->GetObjectNum();
	for ( int i = 0; i < unObjectNum; i++ )
	{
		MapObject* pMapObject = &pObjectArray[i];
		if ( !pMapObject->bActive )
		{
			if ( m_fHeroLocateX > pMapObject->x - d_fActiveLength && m_fHeroLocateX < pMapObject->x + d_fActiveLength &&
				m_fHeroLocateY > pMapObject->y - 600 && m_fHeroLocateY < pMapObject->y + 600 )
			{
				switch ( pMapObject->unKind )
				{
				case 0:
					if ( ( pMapObject->argi1 == m_iActionIndex && !pMapObject->argb1 )
						|| pMapObject->bCheck == true )
					{
						if ( pMapObject->argi2 == 0 && m_iNeedKill == 0 )
						{
							pMapObject->bCheck = false;
						}
						else
						{
							pMapObject->bActive = true;
							pMapObject->bCheck	= true;
							pMapObject->argb1	= true;
							pMapObject->argb2	= false;

							pMapObject->pSprite = CCSprite::create("Tile/z0.png");
							pMapObject->pSprite->setAnchorPoint(ccp(0.0f, 0.0f));
							pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
							m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);

							pMapObject->pSprite1 = CCSprite::create("Tile/z1.png");
							pMapObject->pSprite1->setAnchorPoint(ccp(0.0f, 0.0f));
							pMapObject->pSprite1->setPosition(ccp(pMapObject->x, pMapObject->y+50));
							m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite1, 2);
							CCMoveBy* pAction1 = CCMoveBy::create(1.5f, ccp(0, 200));
							CCMoveBy* pAction2 = CCMoveBy::create(1.5f, ccp(0, -200));
							pMapObject->pSprite1->runAction(CCRepeatForever::create(CCSequence::create(pAction1, pAction2, NULL)));

							if ( pMapObject->argi1%2 == 1 )
							{
								pMapObject->pSprite->setAnchorPoint(ccp(1.0f, 0.0f));
								pMapObject->pSprite1->setAnchorPoint(ccp(1.0f, 0.0f));
								pMapObject->pSprite->setScaleX(-1);
								pMapObject->pSprite1->setScaleX(-1);
							}

							//if ( pMapObject->pArmature )
							//	pMapObject->pArmature->setVisible(true);
							//else
							//{
							//	pMapObject->pArmature = cs::Armature::create("zhedang");
							//	pMapObject->pArmature->setAnchorPoint( ccp(0.0f, 0.08f) );
							//	pMapObject->pArmature->setPosition( ccp(pMapObject->x, pMapObject->y) );
							//	//if ( pMapObject->iToward == 0 )
							//	//{
							//	//	pMapObject->pArmature->setScaleX(-1);
							//	//	pMapObject->pArmature->setPositionX(pMapObject->x+pMapObject->w);
							//	//}
							//	m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pArmature, 2);
							//}
							//pMapObject->pArmature->getAnimation()->playByIndex(0, 0, 50);
						}
					}
					break;
				case 1:
					pMapObject->bActive = true;
					pMapObject->pSprite = CCSprite::createWithTexture(m_ptBox);
					pMapObject->pSprite->setAnchorPoint(ccp(0.5f, 0.6f));
					pMapObject->pSprite->setPosition(ccp(pMapObject->x+pMapObject->w/2, pMapObject->y+pMapObject->h/2+d_fStairFix/2));
					m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					break;
				case 4:
					pMapObject->bActive = true;
					pMapObject->pSprite = CCSprite::create("Tile/a3.png");
					pMapObject->pSprite->setPosition( ccp(pMapObject->x, pMapObject->y) );
					pMapObject->pSprite->setAnchorPoint( ccp(0.0f, 0.0f) );
					m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					break;
				case 5:
					pMapObject->bActive = true;
					if ( m_unLevel/6 == 0 || m_unLevel == 30 )
						pMapObject->pSprite = CCSprite::create("Tile/a4.png");
					else if ( m_unLevel/6 == 1 )
						pMapObject->pSprite = CCSprite::create("Tile/b4.png");
					pMapObject->pSprite->setPosition( ccp(pMapObject->x, pMapObject->y) );
					pMapObject->pSprite->setAnchorPoint( ccp(0.0f, 0.0f) );
					m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 4);
					break;
				case 2:
				case 3:
				case 6:
					pMapObject->bActive = true;
					pMapObject->pSprite = CCSprite::create(pMapObject->sPath.c_str());
					pMapObject->pSprite->setAnchorPoint(ccp(0,0));
					pMapObject->pSprite->setPosition(ccp(pMapObject->x/*+pMapObject->w/2*/, pMapObject->y/*+pMapObject->h/2+d_fStairFix/2*/));
					m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					break;
				case 7:
					pMapObject->argi2++;
					if ( pMapObject->argi2 > Time_DropsDist )
					{
						pMapObject->bActive = true;
						pMapObject->pSprite = CCSprite::createWithTexture(m_ptDropsT[pMapObject->argi1]);
						pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
						pMapObject->pSprite->setAnchorPoint( ccp(0.5f, 0.1f) );
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					}
					if ( !pMapObject->pSprite1 )
					{
						pMapObject->pSprite1 = CCSprite::create("Animation/drops/di.png");
						pMapObject->pSprite1->setAnchorPoint( ccp(0.5f, 0.6f) );
						pMapObject->pSprite1->setPosition(ccp(pMapObject->x, pMapObject->arg1));
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite1, 2);
					}
					break;
				case 8:
					pMapObject->argi2++;
					if ( pMapObject->argi2 > Time_PoisonGas )
					{
						AppDelegate::AudioPlayEffect("MS/Sound/EfGas.mp3");
						pMapObject->bActive = true;
						pMapObject->argb	= false;
						pMapObject->pSprite = CCSprite::create("Animation/poisongas/1.png");
						pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
						pMapObject->pSprite->setAnchorPoint( ccp(0.5f, 0.12f) );
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 4);
						pMapObject->pSprite->runAction(CCSequence::create(
							CCAnimate::create(common::CreateAnimationFrame("Animation/poisongas/", 7, CCRect(0, 0, 201, 342))) ,
							CCCallFuncND::create(this, callfuncND_selector(CGameControler::CallBackGasOver), pMapObject), 
							NULL));
					}
					break;
				case 9:
					if ( pMapObject->argb )
					{
						pMapObject->bActive = true;
						pMapObject->pSprite = CCSprite::create(g_sDefencePath[pMapObject->argi1].c_str());
						pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
						pMapObject->pSprite->setAnchorPoint( ccp(0.0f, 0.05f) );
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 5);

						pMapObject->pSprite1 = CCSprite::create("Monster/xue1.png");
						pMapObject->pSprite1->setPosition( ccp(pMapObject->x+50, pMapObject->y + pMapObject->h + 70.0) );
						pMapObject->pSprite1->setAnchorPoint( ccp(0.5f, 0.5f) );
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite1, 5);
						CCSprite* pBar = CCSprite::create("Monster/xue2.png");
						CCProgressTimer* pProgress = CCProgressTimer::create(pBar);
						pProgress->setPosition( ccp(0,0) );
						pProgress->setAnchorPoint( ccp(0.0f, 0.0f) );
						pProgress->setType(kCCProgressTimerTypeBar);//
						pProgress->setMidpoint(ccp(0, 0.5));
						pProgress->setBarChangeRate(ccp(1, 0));
						pProgress->setPercentage(100); 
						pMapObject->pSprite1->addChild(pProgress, 0, 1);
					}
					break;
				case 10:
					if ( pMapObject->argb )
					{
						pMapObject->bActive = true;
						pMapObject->pSprite = CCSprite::create(g_sWallPath[pMapObject->argi1].c_str());
						pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
						pMapObject->pSprite->setAnchorPoint( ccp(0.0f, 0.05f) );
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					}
					if ( !pMapObject->pSprite1 )
					{
						pMapObject->pSprite1 = CCSprite::create("Animation/walltip/1.png");
						pMapObject->pSprite1->setAnchorPoint( ccp(0.5f, 0.6f) );
						pMapObject->pSprite1->setPosition(ccp(pMapObject->x-50, pMapObject->y+150));
						common::RunNewAnimation(pMapObject->pSprite1, "Animation/walltip/", 5, CCRect(0,0,103,93), true);
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite1, 2);
					}
					break;
				case 11:
					pMapObject->bActive = true;
					pMapObject->pSprite = CCSprite::create(g_sDownStairPath[pMapObject->argi1].c_str());
					pMapObject->pSprite->setAnchorPoint(ccp(0.0f, 0.0f));
					pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
					m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					break;
				case 12:
					pMapObject->bActive = true;
					if ( pMapObject->pArmature )
						pMapObject->pArmature->setVisible(true);
					else
					{
						pMapObject->pArmature = cs::Armature::create("shirenhua");
						pMapObject->pArmature->setAnchorPoint( ccp(0.5f, 0.08f) );
						pMapObject->pArmature->setPosition( ccp(pMapObject->x, pMapObject->y) );
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pArmature, 2);
					}
					break;
				case 13:
					pMapObject->bActive = true;
					{
						pMapObject->pSprite = CCSprite::create("Tile/c1.png");
						pMapObject->pSprite->setAnchorPoint(ccp(0.0f,0.0f));
						pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					}
					break;
				case 14:
					pMapObject->bActive = true;
					{
						pMapObject->pSprite = CCSprite::create("Animation/oilcan/you1.png");
						pMapObject->pSprite->setAnchorPoint(ccp(0.5f,0.0f));
						pMapObject->pSprite->setScale(0.8f);
						pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					}
					break;
				case 15:
					pMapObject->bActive = true;
					{
						bool show = false;
						for ( int j = 0; j < 3; j++ )
						{
							if ( pMapObject->argi1 == m_iKeyIndex[j] )
							{
								show = true;
								break;
							}
						}
						if ( show )
						{//KEY
							pMapObject->pSprite = CCSprite::create("Tile/key.png");
							pMapObject->pSprite->setAnchorPoint(ccp(0.0f,0.0f));
							pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
							m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
							CCMoveBy* pAction1 = CCMoveBy::create(1.0f, ccp(0, 50));
							CCMoveBy* pAction2 = CCMoveBy::create(1.0f, ccp(0, -50));
							CCDelayTime* pDelay = CCDelayTime::create(0.3f);
							pMapObject->pSprite->runAction(CCRepeatForever::create(CCSequence::create(
								pAction1, pDelay, pAction2, pDelay, NULL)));
						}
					}
					break;
				case 16:
					pMapObject->bActive = true;
					{
						if ( pMapObject->bCheck )
						{
							pMapObject->pSprite = CCSprite::create("Tile/men1.png");
							pMapObject->pSprite->setAnchorPoint(ccp(0,0));
							pMapObject->pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
							m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
						}
						else
						{
							pMapObject->pSprite = CCSprite::create("Tile/men2.png");
							pMapObject->pSprite->setAnchorPoint(ccp(0,0));
							pMapObject->pSprite->setPosition(ccp(pMapObject->x+100, pMapObject->y));
							m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
						}
					}
					break;
				case 17:
					pMapObject->bActive = true;
					if ( AppDelegate::s_LevelKey[m_unLevel] == 0 )
					{
						pMapObject->pSprite = CCSprite::create("Tile/xun.png");
						pMapObject->pSprite->setScale(0.4f);
						pMapObject->pSprite->setAnchorPoint(ccp(0.5f, 0.6f));
						pMapObject->pSprite->setPosition(ccp(pMapObject->x+pMapObject->w/2, pMapObject->y+pMapObject->h/2+d_fStairFix/2));
						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pSprite, 2);
					}
					break;
				case 18://鱼
					pMapObject->bActive = true;
					if ( pMapObject->pArmature )
						pMapObject->pArmature->setVisible(true);
					else
					{
						pMapObject->pArmature = cs::Armature::create("js12dz");
						pMapObject->pArmature->setScaleY(0.7f);
						pMapObject->pArmature->setScaleX(-0.7f);
						pMapObject->pArmature->setAnchorPoint(ccp(0.5f, 0.6f));
						pMapObject->pArmature->setPosition(ccp(pMapObject->x, pMapObject->y));
						pMapObject->pArmature->getAnimation()->playByIndex(0, 0, 30);

						pMapObject->arg1 = pMapObject->x;
						pMapObject->arg2 = pMapObject->y;
						pMapObject->argi1 = 100;
						pMapObject->arg3 = -4;
						pMapObject->arg4 = 8.8f;
						pMapObject->argb1 = false;
						pMapObject->argb3 = true;

						m_pCFrontgroud->GetTileMap()->addChild(pMapObject->pArmature, 3);
					}
					break;
				case 19://
					break;
				}
			}
		}
		else
		{
			if ( pMapObject->pSprite1 )
			{
				if ( m_fHeroLocateX < pMapObject->x - d_fReleaseLength && m_fHeroLocateX > pMapObject->x + d_fReleaseLength )
				{
					pMapObject->pSprite1->removeFromParentAndCleanup(true);
					pMapObject->pSprite1 = NULL;
				}
			}
			if ( !pMapObject->pSprite && !pMapObject->pArmature )
				continue;
			if ( m_fHeroLocateX < pMapObject->x - d_fReleaseLength && m_fHeroLocateX > pMapObject->x + d_fReleaseLength )
			{
				pMapObject->bActive = false;
				if ( pMapObject->pSprite )
				{
					pMapObject->pSprite->removeFromParentAndCleanup(true);
					pMapObject->pSprite = NULL;
				}
				if ( pMapObject->pArmature )
				{
					pMapObject->pArmature->setVisible(false);
					pMapObject->pArmature->getAnimation()->stop();
					if ( m_iNeedKill > 0 && pMapObject->argi2 == 0 )
					{
						pMapObject->argb2 = true;
					}
				}
			}
			else
			{
				switch ( pMapObject->unKind )
				{
				case 0://限制门
					{
						if ( fabs(pMapObject->x - m_fHeroLocateX) < 300 && m_iActionIndex == pMapObject->argi1 && pMapObject->argi2 == 1 )
						{
							pMapObject->argb2 = true;
							m_iActionIndex++;
							m_iNeedKill = pMapObject->argi3;
						}
						else if ( m_iNeedKill > 0 && pMapObject->argi2 == 0 )
						{
							pMapObject->argb2 = true;
						}
						if ( m_iNeedKill == 0 && pMapObject->argb2 )
						{
							pMapObject->bCheck = false;
							pMapObject->pSprite->removeFromParent();
							pMapObject->pSprite = NULL;

							pMapObject->pSprite1->removeFromParentAndCleanup(true);
							pMapObject->pSprite1 = NULL;
						}
					}
					break;
				case 1://箱子与攻击碰撞
					{
						std::vector<AttackingObject*>::iterator it;
						if ( m_vecAttackingObjectArray.size() )
							it = m_vecAttackingObjectArray.begin();
						while ( m_vecAttackingObjectArray.size() )
						{
							bool bStop		= false;
							bool bStopFor	= false;
							std::vector<AttackingObject*>::iterator next;
							if ( it+1 == m_vecAttackingObjectArray.end() )
								bStop = true;
							else
								next = it+1;
							if ( pMapObject->pSprite )
							{
								if ( (*it)->type == eWT_Grenade )
								{
									if ( ((*it)->x - pMapObject->x)*((*it)->x - pMapObject->x) +
										((*it)->y - pMapObject->y)*((*it)->y - pMapObject->y) < d_fGrenadeRange*d_fGrenadeRange )
									{
										AppDelegate::AudioPlayEffect("MS3/BoxBroken.mp3");
										pMapObject->pSprite->removeFromParent();
										pMapObject->pSprite = NULL;
										CreateItem(pMapObject->x, pMapObject->y, pMapObject->argi3);
										CCParticleSystemQuad* pParticle = CCParticleSystemQuad::create("Tile/boxBroken.plist");
										pParticle->setPosition( ccp(pMapObject->x+pMapObject->w/2, pMapObject->y+pMapObject->h+40) );
										pParticle->runAction( CCSequence::create(CCDelayTime::create(1.0f), 
											CCCallFuncND::create(this, callfuncND_selector(CGameControler::CallBackBoxDestroy), pParticle), NULL) );
										m_pCFrontgroud->GetTileMap()->addChild(pParticle, 2);
									}
								}
								else if ( ( (*it)->time == 10 && (*it)->type != eWT_Knife ) || ( (*it)->type == eWT_Knife && 
									( (*it)->time < 21 && m_iWeaponUse[0] == 2 && AppDelegate::s_HeroType == 0 ) ||
									( (*it)->time < 18 && m_iWeaponUse[0] == 14 && AppDelegate::s_HeroType == 1 ) ||
									( (*it)->time < 20 && (*it)->time > 7 && AppDelegate::s_HeroType == 2 ) ||
									( (*it)->time < 21 && m_iWeaponUse[0] == 6 ) || 
									( (*it)->time < 17 && m_iWeaponUse[0] == 13 )) )
								{
									float x;
									float y;
									if ( (*it)->type != eWT_Knife && (*it)->type != eWT_Fire )
									{
										x = (*it)->x - (*it)->w/2;
										y = (*it)->y - (*it)->h/2;
									}
									else if ( (*it)->type == eWT_Fire )
									{
										x = (*it)->x;
										y = (*it)->y;
										if ( m_fHeroToward == -1 )
											x -= (*it)->w;
									}
									else
									{
										x = m_fHeroLocateX;
										y = m_fHeroLocateY;
										if ( m_fHeroToward == -1 )
											x -= (*it)->w;
									}
									if ( RectInRect( x, (*it)->w, pMapObject->x, pMapObject->w ) 
										&& RectInRect( y, (*it)->h, pMapObject->y, pMapObject->h+50.0f ) )
									{//刷道具
										AppDelegate::AudioPlayEffect("MS3/BoxBroken.mp3");
										pMapObject->pSprite->removeFromParent();
										pMapObject->pSprite = NULL;
										CreateItem(pMapObject->x, pMapObject->y, pMapObject->argi3);
										CCParticleSystemQuad* pParticle = CCParticleSystemQuad::create("Tile/boxBroken.plist");
										pParticle->setPosition( ccp(pMapObject->x+pMapObject->w/2, pMapObject->y+pMapObject->h/2+40) );
										pParticle->runAction( CCSequence::create(CCDelayTime::create(1.0f), 
											CCCallFuncND::create(this, callfuncND_selector(CGameControler::CallBackBoxDestroy), pParticle), NULL) );
										m_pCFrontgroud->GetTileMap()->addChild(pParticle, 2);
										if ( (*it)->type == eWT_Pistol || (*it)->type == eWT_M4A1 )
										{
											if ( (*it)->isFirst && m_iWeaponUse[1] == 9 )
											{
												char str[100] = {0};
												CCArray* animFrames1 = CCArray::createWithCapacity(7);
												for(int i = 1; i < 7; i++) 
												{
													sprintf(str,"Animation/grenade/d%d.png", i);
													CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iGrenadeWH2));
													animFrames1->addObject(frame);
												}
												CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
												CCSequence* pSequence = CCSequence::create(
													CCAnimate::create(pAnimation), 
													CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)),
													NULL);
												CCSprite* pBomb = CCSprite::create("Animation/grenade/d1.png");
												pBomb->setPosition(ccp((*it)->x, (*it)->y));
												pBomb->runAction(pSequence);
												m_pCFrontgroud->GetTileMap()->addChild(pBomb, 5);

												AppDelegate::AudioPlayEffect("MS/Weapon/grenade.mp3");

												CCSprite* pSprite = CCSprite::create("bullet/9/1.png");
												pSprite->setAnchorPoint(ccp(0, 0.5f));
												pSprite->setPosition(ccp((*it)->x, (*it)->y));
												pSprite->setRotation(180);
												pSprite->setScale((*it)->toward);
												m_pCFrontgroud->GetTileMap()->addChild(pSprite, 9);
												pSprite->runAction(CCSequence::create(CCMoveBy::create(0.6f, ccp(-50*(*it)->toward, 0)),
													CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));

												CCSprite* pSprite1 = CCSprite::create("bullet/9/1.png");
												pSprite1->setAnchorPoint(ccp(0, 0.5f));
												pSprite1->setPosition(ccp((*it)->x, (*it)->y));
												pSprite1->setRotation(240);
												pSprite1->setScale((*it)->toward);
												m_pCFrontgroud->GetTileMap()->addChild(pSprite1, 9);
												pSprite1->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-30*(*it)->toward, 40*(*it)->toward)),
													CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));

												CCSprite* pSprite2 = CCSprite::create("bullet/9/1.png");
												pSprite2->setAnchorPoint(ccp(0, 0.5f));
												pSprite2->setPosition(ccp((*it)->x, (*it)->y));
												pSprite2->setRotation(120);
												pSprite2->setScale((*it)->toward);
												m_pCFrontgroud->GetTileMap()->addChild(pSprite2, 9);
												pSprite2->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-30*(*it)->toward, -40*(*it)->toward)),
													CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));
											}
										}
										if ( ((*it)->type == eWT_Pistol || (*it)->type == eWT_M4A1) && (*it)->pSprite && !m_bStrike[0] )
										{
											REMOVE_SELF((*it)->pSprite);
											DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
											bStopFor = true;
											if ( m_vecAttackingObjectArray.size() )
												it = m_vecAttackingObjectArray.begin();
										}
									}
								}
							}
							if ( !bStop )
							{
								if ( !bStopFor )
									it = next;
							}
							else
								break;
						}
					}
					break;
				case 2://左右移动
					pMapObject->x += pMapObject->arg3*pMapObject->iToward;
					pMapObject->pSprite->setPositionX( pMapObject->x );
					if ( m_fHeroLocateX >= pMapObject->x-10 && m_fHeroLocateX <= pMapObject->x+pMapObject->w+10 )
					{
						if ( RectInRect(m_fHeroLocateY, d_fHeroHeight, pMapObject->y, pMapObject->h+6) && m_iHeroJumps == 0 )
						{
							m_bIsMoveObject = true;
							m_fHeroLocateX += pMapObject->arg3*pMapObject->iToward;
							//if ( m_fHeroLocateX <= pMapObject->x || m_fHeroLocateX >= pMapObject->x+pMapObject->w )
							//{
							//	if ( m_iHeroJumps == 0 )
							//		m_fHeroLocateY -= 4.0f;
							//}
							if ( m_fHeroLocateY < pMapObject->y + pMapObject->h+6 && m_iHeroJumps == 0 )
								m_fHeroLocateY -= 4.0f;
						}
					}
					if ( pMapObject->x <= pMapObject->arg1 )
						pMapObject->iToward = 1;
					else if ( pMapObject->x >= pMapObject->arg2 )
						pMapObject->iToward = -1;
					break;
				case 3://上下移动
					if ( m_fHeroLocateX >= pMapObject->x && m_fHeroLocateX <= pMapObject->x + pMapObject->w )
					{
						if ( m_fHeroLocateY - pMapObject->y > pMapObject->h && m_fHeroLocateY - pMapObject->y - pMapObject->h <= d_fHeroGravity + 6 && m_iHeroJumps == 0 )
						{
							m_bIsMoveObject = true;
							m_fHeroLocateY += pMapObject->arg3 * pMapObject->iToward;
						}
						else if ( pMapObject->iToward == -1 )
						{
							if ( m_fHeroLocateY < pMapObject->y + pMapObject->h && m_fHeroLocateY+d_fHeroHeight > pMapObject->y )
								m_fHeroLocateY += pMapObject->arg3 * pMapObject->iToward;
						}
					}
					pMapObject->y += pMapObject->arg3 * pMapObject->iToward;
					pMapObject->pSprite->setPositionY( pMapObject->y );
					if ( pMapObject->y <= pMapObject->arg1 )
						pMapObject->iToward = 1;
					else if ( pMapObject->y >= pMapObject->arg2 )
						pMapObject->iToward = -1;
					break;
				case 4://掉落
					if ( fabs(m_fHeroLocateX - pMapObject->x - pMapObject->w/2) < 60.0f + d_fHeroWidth &&
						fabs(m_fHeroLocateY - pMapObject->y - pMapObject->h) < 20.0f )
					{
						m_bIsMoveObject = true;
						if ( !pMapObject->argb )
						{
							pMapObject->argb = true;
							pMapObject->pSprite->setPositionX(pMapObject->x - 3.0f);
							CCMoveBy* pAction1 = CCMoveBy::create(0.05f, ccp(6.0f, 0.0f));
							CCMoveBy* pAction2 = CCMoveBy::create(0.05f, ccp(-6.0f, 0.0f));
							pMapObject->pSprite->runAction( CCRepeatForever::create(
								CCSequence::create(pAction1, pAction2,NULL) ) );
						}
					}
					if ( pMapObject->argb )
					{
						if ( pMapObject->argi1 <= d_iLandStayTime )
							pMapObject->argi1++;
						if ( pMapObject->argi1 == d_iLandStayTime )
						{
							pMapObject->pSprite->stopAllActions();
						}
						else if ( pMapObject->argi1 > d_iLandStayTime )
						{
							pMapObject->y -= 15.0f;
							pMapObject->pSprite->setPosition( ccp(pMapObject->x, pMapObject->y) );
							if ( pMapObject->y < -250 - pMapObject->h )
							{
								pMapObject->pSprite->removeFromParentAndCleanup(true);
								pMapObject->pSprite = NULL;
							}
						}
					}
					break;
				case 5://地刺
					if ( pMapObject->argi1 > 0 )
						pMapObject->argi1--;
					if ( fabs(m_fHeroLocateX - pMapObject->x - pMapObject->w/2) < pMapObject->w/2 + d_fHeroWidth/2 &&
						fabs(m_fHeroLocateY - pMapObject->y - pMapObject->h/2) < pMapObject->h/2 )
					{
						m_iUnderAttacked = 2;
						if ( pMapObject->argi1 == 0 )
						{
							BeAttack(d_iGressDamage);
							pMapObject->argi1 = d_iGressDist;
						}
					}
					break;
				case 6://上下滚动
					if ( m_fHeroLocateX >= pMapObject->x && m_fHeroLocateX <= pMapObject->x + pMapObject->w )
					{
						if ( m_fHeroLocateY - pMapObject->y > pMapObject->h && m_fHeroLocateY - pMapObject->y - pMapObject->h <= d_fHeroGravity + 6 && m_iHeroJumps == 0 )
						{
							m_bIsMoveObject = true;
							m_fHeroLocateY += pMapObject->arg3 * pMapObject->iToward;
						}
					}
					pMapObject->y += pMapObject->arg3 * pMapObject->iToward;
					if ( 480/2 + pMapObject->iToward*480/2 < pMapObject->iToward*pMapObject->y )
						pMapObject->y =480/2 - pMapObject->iToward*480/2;
					pMapObject->pSprite->setPositionY( pMapObject->y );
					break;
				case 7://水滴
					if ( !pMapObject->argb )
					{
						if ( pMapObject->argi2 < Time_DropsDist + Time_DropsStay )
						{
							pMapObject->argi2++;
							if ( pMapObject->argi2 == Time_DropsDist + Time_DropsStay )
							{
								AppDelegate::AudioPlayEffect("MS/Sound/EfDrops.mp3");
								common::RunNewAnimation(pMapObject->pSprite, m_ptDropsA, 5, false, false, 0.30f);
							}
						}
						else
						{
							pMapObject->y -= d_fHeroGravity*10;
							pMapObject->pSprite->setPositionY(pMapObject->y);
							if ( fabs(pMapObject->x - m_fHeroLocateX ) < d_fHeroWidth/2 && 
								pMapObject->y > m_fHeroLocateY && pMapObject->y < m_fHeroLocateY+d_fHeroHeight )
							{
								pMapObject->pSprite->removeFromParentAndCleanup(true);
								pMapObject->pSprite = NULL;
								pMapObject->bActive = false;
								pMapObject->argi2	= 0;
								pMapObject->y		= pMapObject->arg2;
								BeAttack(d_iDropsDamage);
							}
							else if ( pMapObject->y < pMapObject->arg1 )
							{
								pMapObject->y = pMapObject->arg1;
								pMapObject->pSprite->setPositionY(pMapObject->y);
								pMapObject->pSprite->stopAllActions();
								pMapObject->argb = true;
								common::RunNewAnimation(pMapObject->pSprite, m_ptDropsB, 6, false, false, 0.05f,
									CCCallFuncND::create(this, callfuncND_selector(CGameControler::CallBackDropOver), pMapObject) );
							}
							else if ( pMapObject->y < -10 )
							{
								pMapObject->pSprite->removeFromParentAndCleanup(true);
								pMapObject->pSprite = NULL;
								pMapObject->bActive = false;
								pMapObject->argb	= false;
								pMapObject->argi2	= 0;
								pMapObject->y		= pMapObject->arg2;
							}
						}
					}
					break;
				case 8://毒气
					if ( !pMapObject->argb )
					{
						if ( fabs(m_fHeroLocateX - pMapObject->x) < d_fHeroWidth + lenght_PoisonGasW )
						{
							if ( RectInRect(m_fHeroLocateY, d_fHeroHeight, pMapObject->y, lenght_PoisonGasH) )
							{
								pMapObject->argb = true;
								BeAttack(d_iPoisonGasDamage);
							}
						}
					}
					break;
				case 9://护栏
					{
						pMapObject->argi2 = 0;
						if ( pMapObject->arg1 <= 0.0f )
						{
							AppDelegate::AudioPlayEffect("MS2/wallbroken.mp3");
							pMapObject->bCheck = false;
							CCParticleSystemQuad* pParticle1 = CCParticleSystemQuad::create("Tile/a5_1.plist");
							CCParticleSystemQuad* pParticle2 = CCParticleSystemQuad::create("Tile/a5_2.plist");
							pParticle1->setAnchorPoint(ccp(0.5f, 0.0f));
							pParticle2->setAnchorPoint(ccp(0.5f, 0.0f));
							pParticle1->setPosition(ccp(pMapObject->x+pMapObject->w/2, pMapObject->y));
							pParticle2->setPosition(ccp(pMapObject->x+pMapObject->w/2, pMapObject->y));
							pParticle1->runAction(CCSequence::create(CCDelayTime::create(1.5f), 
								CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
							pParticle2->runAction(CCSequence::create(CCDelayTime::create(1.5f), 
								CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
							pMapObject->pSprite->getParent()->addChild(pParticle1, 3);
							pMapObject->pSprite->getParent()->addChild(pParticle2, 3);
							pMapObject->pSprite->removeFromParentAndCleanup(true);
							pMapObject->pSprite = NULL;
							if ( pMapObject->pSprite1 )
							{
								pMapObject->pSprite1->removeFromParentAndCleanup(true);
								pMapObject->pSprite1 = NULL;
							}
						}
						else if ( pMapObject->pSprite1 )
						{
							((CCProgressTimer*)(pMapObject->pSprite1->getChildByTag(1)))->setPercentage(pMapObject->arg1/2);
						}
					}
					break;
				case 10://障碍
					{
						std::vector<AttackingObject*>::iterator it;
						if ( m_vecAttackingObjectArray.size() )
							it = m_vecAttackingObjectArray.begin();
						while ( m_vecAttackingObjectArray.size() )
						{
							bool bStop		= false;
							bool bStopFor	= false;
							if ( !pMapObject->pSprite )
								break;
							std::vector<AttackingObject*>::iterator next;
							if ( it+1 == m_vecAttackingObjectArray.end() )
								bStop = true;
							else
								next = it+1;
							if ( (*it)->type == eWT_Grenade )
							{
								if ( (*it)->x - pMapObject->x < d_fGrenadeRange )
								{
									AppDelegate::AudioPlayEffect("MS2/wallbroken.mp3");
									pMapObject->bCheck = false;
									char buffer1[255];
									char buffer2[255];
									pMapObject->argi2 = 0;
									sprintf(buffer1, "Tile/%d_1.plist", pMapObject->argi1-1);
									sprintf(buffer2, "Tile/%d_2.plist", pMapObject->argi1-1);
									CCParticleSystemQuad* pParticle1 = CCParticleSystemQuad::create(buffer1);
									CCParticleSystemQuad* pParticle2 = CCParticleSystemQuad::create(buffer2);
									pParticle1->setPosition(ccp(pMapObject->x+80, pMapObject->y+50));
									pParticle2->setPosition(ccp(pMapObject->x+80, pMapObject->y+50));
									pParticle1->setAnchorPoint(ccp(0.5f, 0.0f));
									pParticle2->setAnchorPoint(ccp(0.5f, 0.0f));
									pParticle1->runAction(CCSequence::create(CCDelayTime::create(1.5f), 
										CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
									pParticle2->runAction(CCSequence::create(CCDelayTime::create(1.5f), 
										CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
									pMapObject->pSprite->getParent()->addChild(pParticle1, 3);
									pMapObject->pSprite->getParent()->addChild(pParticle2, 3);
									pMapObject->pSprite->removeFromParent();
									pMapObject->pSprite = NULL;
									pMapObject->pSprite1->removeFromParentAndCleanup(true);
									pMapObject->pSprite1 = NULL;
								}
							}
							else if ( (*it)->time == 10 )
							{
								float x;
								float y;
								if ( (*it)->type != eWT_Knife )
								{
									x = (*it)->x - (*it)->w/2;
									y = (*it)->y - (*it)->h/2;
								}
								else
								{
									x = m_fHeroLocateX;
									y = m_fHeroLocateY;
									if ( m_fHeroToward == -1 )
										x -= (*it)->w;
								}
								if ( RectInRect( x, (*it)->w, pMapObject->x, pMapObject->w ) 
									&& RectInRect( y, (*it)->h, pMapObject->y, pMapObject->h+50.0f ) )
								{
									if ( (*it)->type == eWT_Knife )
									{
										pMapObject->argi2 -= (*it)->damage;
										if ( pMapObject->argi2 <= 0 )
										{
											AppDelegate::AudioPlayEffect("MS2/wallbroken.mp3");
											pMapObject->bCheck = false;
											pMapObject->argb = false;
											pMapObject->argi2 = 0;
											char buffer1[255];
											char buffer2[255];
											sprintf(buffer1, "Tile/%d_1.plist", pMapObject->argi1-1);
											sprintf(buffer2, "Tile/%d_2.plist", pMapObject->argi1-1);
											CCParticleSystemQuad* pParticle1 = CCParticleSystemQuad::create(buffer1);
											CCParticleSystemQuad* pParticle2 = CCParticleSystemQuad::create(buffer2);
											pParticle1->setPosition(ccp(pMapObject->x+80, pMapObject->y+50));
											pParticle2->setPosition(ccp(pMapObject->x+80, pMapObject->y+50));
											pParticle1->setAnchorPoint(ccp(0.5f, 0.0f));
											pParticle2->setAnchorPoint(ccp(0.5f, 0.0f));
											pParticle1->runAction(CCSequence::create(CCDelayTime::create(1.5f), 
												CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
											pParticle2->runAction(CCSequence::create(CCDelayTime::create(1.5f), 
												CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
											pMapObject->pSprite->getParent()->addChild(pParticle1, 3);
											pMapObject->pSprite->getParent()->addChild(pParticle2, 3);
											pMapObject->pSprite->removeFromParentAndCleanup(true);
											pMapObject->pSprite = NULL;
											pMapObject->pSprite1->removeFromParentAndCleanup(true);
											pMapObject->pSprite1 = NULL;
										}
										else
										{
											CCMoveBy* pAction1 = CCMoveBy::create(0.05f, ccp(-3, 0));
											CCMoveBy* pAction2 = CCMoveBy::create(0.05f, ccp(6, 0));
											CCMoveBy* pAction3 = CCMoveBy::create(0.05f, ccp(-6, 0));
											pMapObject->pSprite->stopAllActions();
											pMapObject->pSprite->setPositionX(pMapObject->x);
											pMapObject->pSprite->runAction( CCSequence::create(pAction1, pAction2, pAction3, pAction2, pAction3, pAction2, pAction1, NULL));
										}
									}
									else if ( (*it)->type == eWT_Pistol )
									{
										REMOVE_SELF((*it)->pSprite);
										DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
										bStopFor = true;
										if ( m_vecAttackingObjectArray.size() )
											it = m_vecAttackingObjectArray.begin();
									}
								}
							}
							if ( !bStop )
							{
								if ( !bStopFor )
									it = next;
							}
							else
								break;
						}
					}
					break;
				case 11://下落一定距离
					if ( !pMapObject->argb )
					{
						if ( fabs(m_fHeroLocateX - pMapObject->x - pMapObject->w/2) < 60.0f + d_fHeroWidth &&
							fabs(m_fHeroLocateY - pMapObject->y - pMapObject->h - 2.5f) < 2.5f )
						{
							m_bIsMoveObject = true;
							pMapObject->argb = true;
						}
					}
					if ( pMapObject->argb )
					{
						if ( pMapObject->y > pMapObject->argi2 )
						{
							if ( m_fHeroLocateY - pMapObject->y > pMapObject->h && m_fHeroLocateY - pMapObject->y - pMapObject->h <= d_fHeroGravity && m_iHeroJumps == 0 )
							{
								m_bIsMoveObject = true;
								m_fHeroLocateY -= 5.0f;
							}
							pMapObject->y -= 5.0f;
							if ( pMapObject->y < pMapObject->argi2 )
								pMapObject->y = pMapObject->argi2;
							pMapObject->pSprite->setPositionY( pMapObject->y );
						}
					}
					break;
				case 12://食人花
					if ( pMapObject->argi3 != 0 )
						pMapObject->argi3 --;
					if ( fabs(m_fHeroLocateX - pMapObject->x) < 100.0f )
					{
						if ( pMapObject->argi3 == 0 )
						{
							pMapObject->argi3 = -1;
							pMapObject->pArmature->getAnimation()->playByIndex(0, 0, 80);
						}
					}
					if ( pMapObject->argi3 < 0 )
					{
						if ( pMapObject->x > m_fHeroLocateX )
							pMapObject->pArmature->setScaleX(-1.0f);
						else
							pMapObject->pArmature->setScaleX(1.0f);
						if ( pMapObject->argi3 == -30 )
						{
							if ( fabs(m_fHeroLocateX - pMapObject->x) < 100.0f && 
								fabs(m_fHeroLocateY + d_fHeroHeight/2 - pMapObject->y - 40) < 80.0f )
								BeAttack(20);
						}
						else if ( pMapObject->argi3 == -85 )
						{
							//pMapObject->pArmature->getAnimation()->stop();
							//pMapObject->pArmature->getAnimation()->update(0);
							pMapObject->argi3 = 100;
						}
					}
					break;
				case 13://沉浮的水桶
					if ( fabs(m_fHeroLocateX - pMapObject->x - pMapObject->w/2) < 60.0f + d_fHeroWidth &&
						fabs(m_fHeroLocateY - pMapObject->y - pMapObject->h - 2.5f) < 2.5f )
					{
						m_bIsMoveObject = true;
						if ( pMapObject->arg1 < 40.0f )
						{
							pMapObject->arg1	+= 1.0f;
							pMapObject->y		-= 1.0f;
							m_fHeroLocateY		-= 1.0f;
						}
						if ( pMapObject->arg1 > 25.0f )
						{
							m_bPrisonWater = true;
							if ( pMapObject->argi1 == 0 )
								m_iStateHP -= 1;
							CreateInPrisonPlist();
							pMapObject->argi1 = (pMapObject->argi1+1)%10;
						}
					}
					else if ( pMapObject->arg1 > 0.0f )
					{
						pMapObject->arg1	-= 1.0f;
						pMapObject->y		+= 1.0f;
					}
					pMapObject->pSprite->setPositionY(pMapObject->y);
					break;
				case 14://打击用油桶
					{
						std::vector<AttackingObject*>::iterator it;
						if ( m_vecAttackingObjectArray.size() )
							it = m_vecAttackingObjectArray.begin();
						while ( m_vecAttackingObjectArray.size() )
						{
							bool bStop		= false;
							bool bStopFor	= false;
							std::vector<AttackingObject*>::iterator next;
							if ( it+1 == m_vecAttackingObjectArray.end() )
								bStop = true;
							else
								next = it+1;
							if ( pMapObject->pSprite )
							{
								if ( (*it)->type == eWT_Grenade )
								{
									if ( ((*it)->x - pMapObject->x)*((*it)->x - pMapObject->x) +
										((*it)->y - pMapObject->y-pMapObject->h/2)*((*it)->y - pMapObject->y-pMapObject->h/2) < 
										d_fGrenadeRange*d_fGrenadeRange + pMapObject->w/2*pMapObject->w/2 + pMapObject->h/2*pMapObject->h/2 )
									{
										CCSprite* pSprite = CCSprite::create("Animation/oilcan/you1.png");
										pSprite->setScale(0.8f);
										pSprite->setAnchorPoint(ccp(0.5f,0));
										pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
										CCAnimate* pAction1 = CCAnimate::create(common::CreateAnimationFrame(
											"Animation/oilcan/you", 2, CCRect(0,0,47,84), 0.3f, true));
										CCAnimate* pAction2 = CCAnimate::create(common::CreateAnimationFrame(
											"Animation/oilcan/you", 2, CCRect(0,0,47,84), 0.2f, true));
										CCAnimate* pAction3 = CCAnimate::create(common::CreateAnimationFrame(
											"Animation/oilcan/you", 2, CCRect(0,0,47,84), 0.1f, true));
										pSprite->runAction(CCSequence::create(pAction1, pAction1, pAction2, pAction2, pAction3, pAction3,
											CCCallFuncN::create(this, callfuncN_selector(CGameControler::OilcanBomb)), NULL));
										pMapObject->pSprite->getParent()->addChild(pSprite, pMapObject->pSprite->getZOrder());
										
										pMapObject->pSprite->removeFromParent();
										pMapObject->pSprite = NULL;
									}
								}
								else if ( (*it)->time == 10 || 
									((*it)->type == eWT_Knife && ((*it)->time == 32 || 
									( (*it)->time < 32 && (m_iWeaponUse[0] == 6 || m_iWeaponUse[0] == 13) ) ) )
									)
								{
									float x;
									float y;
									if ( (*it)->type != eWT_Knife && (*it)->type != eWT_Fire )
									{
										x = (*it)->x - (*it)->w/2;
										y = (*it)->y - (*it)->h/2;
									}
									else if ( (*it)->type == eWT_Fire )
									{
										x = (*it)->x;
										y = (*it)->y;
										if ( m_fHeroToward == -1 )
											x -= (*it)->w;
									}
									else
									{
										x = m_fHeroLocateX;
										y = m_fHeroLocateY;
										if ( m_fHeroToward == -1 )
											x -= (*it)->w;
									}
									if ( RectInRect( x, (*it)->w, pMapObject->x-pMapObject->w/2, pMapObject->w ) 
										&& RectInRect( y, (*it)->h, pMapObject->y, pMapObject->h+50.0f ) )
									{
										CCSprite* pSprite = CCSprite::create("Animation/oilcan/you1.png");
										pSprite->setAnchorPoint(ccp(0.5f,0));
										pSprite->setScale(0.8f);
										pSprite->setPosition(ccp(pMapObject->x, pMapObject->y));
										CCAnimate* pAction1 = CCAnimate::create(common::CreateAnimationFrame(
											"Animation/oilcan/you", 2, CCRect(0,0,47,84), 0.3f, true));
										CCAnimate* pAction2 = CCAnimate::create(common::CreateAnimationFrame(
											"Animation/oilcan/you", 2, CCRect(0,0,47,84), 0.2f, true));
										CCAnimate* pAction3 = CCAnimate::create(common::CreateAnimationFrame(
											"Animation/oilcan/you", 2, CCRect(0,0,47,84), 0.1f, true));
										pSprite->runAction(CCSequence::create(pAction1, pAction1, pAction2, pAction2, pAction3, pAction3,
											CCCallFuncN::create(this, callfuncN_selector(CGameControler::OilcanBomb)), NULL));
										pMapObject->pSprite->getParent()->addChild(pSprite, pMapObject->pSprite->getZOrder());
										pMapObject->pSprite->removeFromParent();
										pMapObject->pSprite = NULL;
										if ( ((*it)->type == eWT_Pistol || (*it)->type == eWT_M4A1) && (*it)->pSprite && !m_bStrike[0] )
										{
											REMOVE_SELF((*it)->pSprite);
											DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
											bStopFor = true;
											if ( m_vecAttackingObjectArray.size() )
												it = m_vecAttackingObjectArray.begin();
										}
									}
								}
							}
							if ( !bStop )
							{
								if ( !bStopFor )
									it = next;
							}
							else
								break;
						}
					}
					break;
				case 15://钥匙
					{
						if ( RectInRect( m_fHeroLocateX-d_fHeroWidth/2, d_fHeroWidth, pMapObject->x+30, pMapObject->w ) 
							&& RectInRect( m_fHeroLocateY, d_fHeroHeight, pMapObject->y+20, pMapObject->h ) )
						{//KEYFLY
							m_iKeyNum++;
							AppDelegate::AudioPlayEffect("MS2/getKey.mp3");
							CCSprite* pSprite = CCSprite::create("Tile/key.png");
							pSprite->setPosition(ccp(400,240));
							pSprite->setScale(3.0f);
							CCScaleTo* pAction1 = CCScaleTo::create(1.0f, 0.5f);
							CCMoveTo* pAction2 = CCMoveTo::create(1.0f, ccp(30*m_iKeyNum ,300));
							addChild(pSprite, 9);
							pSprite->runAction(pAction1);
							pSprite->runAction(pAction2);
							pMapObject->pSprite->removeFromParentAndCleanup(true);
							pMapObject->pSprite = NULL;
						}
					}
					break;
				case 16://铁门
					{
						if ( pMapObject->bCheck )
						{
							if ( RectInRect( m_fHeroLocateX, d_fHeroWidth, pMapObject->x, pMapObject->w ) 
								&& RectInRect( m_fHeroLocateY, d_fHeroHeight, pMapObject->y, pMapObject->h ) )
							{
								if ( m_iKeyNum == 3 )
								{
									AppDelegate::AudioPlayEffect("MS2/opendoor.mp3");
									CCSprite* pSprite = CCSprite::create("Tile/men2.png");
									pSprite->setScaleX(-1);
									pSprite->setAnchorPoint(ccp(0,0));
									pSprite->setPosition(ccp(pMapObject->pSprite->getPositionX()+100, pMapObject->pSprite->getPositionY()));
									m_pCFrontgroud->GetTileMap()->addChild(pSprite, 4);
									pMapObject->pSprite->removeFromParent();
									pMapObject->pSprite = NULL;
									pMapObject->bCheck = false;
								}
								else
								{
									if ( !m_pKeyTip )
									{
										m_pKeyTip = CCSprite::create("Tile/keytip.png");
										m_pKeyTip->setPosition(ccp(400,240));
										addChild(m_pKeyTip, 18);
									}
									m_pKeyTip->stopAllActions();
									m_pKeyTip->runAction(CCSequence::create(CCDelayTime::create(1.0f), CCFadeOut::create(1.0f),
										CCCallFunc::create(this, callfunc_selector(CGameControler::KeyTip)), NULL));
								}
							}
						}
					}
					break;
				case 17://骷髅
					{
						if ( RectInRect( m_fHeroLocateX-d_fHeroWidth/2, d_fHeroWidth, pMapObject->x, pMapObject->w ) 
							&& RectInRect( m_fHeroLocateY, d_fHeroHeight, pMapObject->y, pMapObject->h ) )
						{
							AchieveAdd::Add(53);
							AppDelegate::s_LevelKey[m_unLevel] = 1;
							AppDelegate::AudioPlayEffect("MS2/getKey.mp3");
							CCSprite* pSprite = CCSprite::create("Tile/xun.png");
							pSprite->setPosition(ccp(400,240));
							pSprite->setScale(2.0f);
							CCScaleTo* pAction1 = CCScaleTo::create(1.0f, 0.2f);
							CCMoveTo* pAction2 = CCMoveTo::create(1.0f, ccp(30 ,300));
							addChild(pSprite, 9);
							pSprite->runAction(pAction1);
							pSprite->runAction(pAction2);
							pMapObject->pSprite->removeFromParentAndCleanup(true);
							pMapObject->pSprite = NULL;
						}
					}
					break;
				case 18://鱼
					if ( pMapObject->argi1 > 0 )
					{
						pMapObject->argi1--;
					}
					else if ( pMapObject->argb1 )
					{
						if ( pMapObject->argi3 > 0 )
						{
							pMapObject->argi3 --;
						}
						else
						{
							pMapObject->argb1 = false;
							pMapObject->pArmature->getAnimation()->playByIndex(0, 0, 30);
							pMapObject->x = pMapObject->arg1;
							pMapObject->y = pMapObject->arg2;
							pMapObject->argi1 = 1000;
							pMapObject->arg3 = -5;
							pMapObject->arg4 = 8.8f;
							pMapObject->argb3 = true;
							pMapObject->pArmature->setPosition(ccp(pMapObject->x, pMapObject->y));
						}
					}
					else
					{
						pMapObject->x += pMapObject->arg3;
						pMapObject->y += pMapObject->arg4;
						pMapObject->arg4 -= 0.2f;

						if ( pMapObject->y < 0 )
						{
							pMapObject->x = pMapObject->arg1;
							pMapObject->y = pMapObject->arg2;
							pMapObject->argi1 = 300;
							pMapObject->arg3 = -5;
							pMapObject->arg4 = 8.8f;
							pMapObject->argb3 = true;
						}
						else
						{
							if ( pMapObject->argb3 )
							{
								if ( fabs(m_fHeroLocateX - pMapObject->x) < 50.0f && 
									fabs(m_fHeroLocateY + d_fHeroHeight/2 - pMapObject->y - 35) < 70.0f )
								{
									BeAttack(20);
									pMapObject->argb3 = false;
								}
							}

							std::vector<AttackingObject*>::iterator it;
							if ( m_vecAttackingObjectArray.size() )
								it = m_vecAttackingObjectArray.begin();
							while ( m_vecAttackingObjectArray.size() )
							{
								bool bStop		= false;
								bool bStopFor	= false;
								std::vector<AttackingObject*>::iterator next;
								if ( it+1 == m_vecAttackingObjectArray.end() )
									bStop = true;
								else
									next = it+1;
								if ( (*it)->type == eWT_Grenade )
								{
									if ( ((*it)->x - pMapObject->x)*((*it)->x - pMapObject->x) +
										((*it)->y - pMapObject->y-pMapObject->h/2)*((*it)->y - pMapObject->y-pMapObject->h/2) < 
										d_fGrenadeRange*d_fGrenadeRange + pMapObject->w/2*pMapObject->w/2 + pMapObject->h/2*pMapObject->h/2 )
									{
										pMapObject->pArmature->getAnimation()->playByIndex(1, 0, 30);
										pMapObject->argb1 = true;
										pMapObject->argi3 = 20;
									}
								}
								else if ( (*it)->time == 10 || 
									((*it)->type == eWT_Knife && ((*it)->time == 32 || 
									( (*it)->time < 32 && (m_iWeaponUse[0] == 6 || m_iWeaponUse[0] == 13) ) ) )
									)
								{
									float x;
									float y;
									if ( (*it)->type != eWT_Knife && (*it)->type != eWT_Fire )
									{
										x = (*it)->x - (*it)->w/2;
										y = (*it)->y - (*it)->h/2;
									}
									else if ( (*it)->type == eWT_Fire )
									{
										x = (*it)->x;
										y = (*it)->y;
										if ( m_fHeroToward == -1 )
											x -= (*it)->w;
									}
									else
									{
										x = m_fHeroLocateX;
										y = m_fHeroLocateY;
										if ( m_fHeroToward == -1 )
											x -= (*it)->w;
									}
									if ( RectInRect( x, (*it)->w, pMapObject->x-pMapObject->w/2, pMapObject->w ) 
										&& RectInRect( y, (*it)->h, pMapObject->y-pMapObject->h/2, pMapObject->h ) )
									{
										pMapObject->pArmature->getAnimation()->playByIndex(1, 0, 30);
										pMapObject->argb1 = true;
										pMapObject->argi3 = 30;
									}
								}
								if ( !bStop )
								{
									if ( !bStopFor )
										it = next;
								}
								else
									break;
							}
						}
						pMapObject->pArmature->setPosition(ccp(pMapObject->x, pMapObject->y));
					}
					break;
				case 19://
					break;
				}
			}
		}
	}

	//////////////
	/****弹壳****/
	//////////////
	while ( m_vecShellsArray.size() )
	{
		bool bStop = true;
		std::vector<Shells*>::iterator it = m_vecShellsArray.begin();
		for ( ; it != m_vecShellsArray.end(); it++ )
		{
			(*it)->life --;
			if ( (*it)->life <= 0 )
			{
				(*it)->pSprite->removeFromParent();
				DELETEITERATORFROMVECTOR(m_vecShellsArray, it);
				bStop = false;
				break;
			}
			else if ( (*it)->vx != 0.0 )
			{
				(*it)->x	+= (*it)->vx;
				(*it)->y	+= (*it)->vy;
				(*it)->vy	-= 0.8f;
				if ( (*it)->vy < -15.0f )
					(*it)->vy = -15.0f;
				if ( CheckInWall( (*it)->x-6.0, (*it)->y, 6.0, 6.0 ) )
				{
					(*it)->vy = -(*it)->vy*2/3;
					if ( fabs( (*it)->vy ) < 5.0f )
					{
						(*it)->vx		= 0.0;
						(*it)->rotate	= 0.0;
					}
					else
						AppDelegate::AudioPlayEffect("MS/Sound/ShellDrop.mp3");
				}
				else
					(*it)->rotate -= 30;
				(*it)->pSprite->setRotation( (*it)->rotate );
				(*it)->pSprite->setPosition( ccp((*it)->x, (*it)->y) );
			}
		}
		if ( bStop )
			break;
	}
}

void CGameControler::CallBackDropOver(cocos2d::CCNode* pNode, void* data)
{
	MapObject* pMapObject = (MapObject*)data;
	pMapObject->pSprite->removeFromParentAndCleanup(true);
	pMapObject->pSprite = NULL;
	pMapObject->bActive = false;
	pMapObject->argb	= false;
	pMapObject->argi2	= 0;
	pMapObject->y		= pMapObject->arg2;
}

void CGameControler::CallBackBoxDestroy( cocos2d::CCNode* pNode, void* data )
{
	((CCParticleSystemQuad*)data)->removeFromParentAndCleanup(true);
}

void CGameControler::CallBackGasOver( cocos2d::CCNode* pNode, void* data )
{
	MapObject* pMapObject = (MapObject*)data;
	pMapObject->pSprite->removeFromParentAndCleanup(true);
	pMapObject->pSprite = NULL;
	pMapObject->bActive = false;
	pMapObject->argi2	= 0;
}

void CGameControler::ItemCheck()
{
	while ( m_vecItemObejctArray.size() )
	{
		bool bStop = true;
		std::vector<ItemObject*>::iterator it = m_vecItemObejctArray.begin();
		for (; it != m_vecItemObejctArray.end(); it++ )
		{
			(*it)->time++;
			if ( fabs((*it)->x - m_fHeroLocateX) < d_fHeroWidth && fabs((*it)->y - m_fHeroLocateY) < d_fHeroHeight )
			{//捡到道具
				CCActionInterval* pAction = NULL;
				//bool pick = true;
				if ( (*it)->type > 29 )
				{
					int index = (*it)->type - 30;
					if ( index == AppDelegate::s_WeaponUse[1] )
					{
						m_iBulletNum[0] += g_iWeaponBulletOnce[index];
						if ( m_iBulletNum[0] > 999 )
							m_iBulletNum[0] = 999;
					}
					else
					{
						AppDelegate::s_BulletNum[index] += g_iWeaponBulletOnce[index];
						if ( AppDelegate::s_BulletNum[index] > 999 )
							AppDelegate::s_BulletNum[index] = 999;
					}
					pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateBullet));
				}
				else {
					switch ( (*it)->type )
					{
						case 0://
							//if ( m_iBulletNum[0] == 999 )
							//{
							//	pick = false;
							//	break;
							//}
							m_iBulletNum[0] += 15;
							if ( m_iBulletNum[0] > 999 )
								m_iBulletNum[0] = 999;
							pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateBullet));
							break;
						case 1://
							//if ( m_iBulletNum[0] == 999 )
							//{
							//	pick = false;
							//	break;
							//}
							m_iBulletNum[0] += 30;
							if ( m_iBulletNum[0] > 999 )
								m_iBulletNum[0] = 999;
							pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateBullet));
							break;
						case 2://小钱
							//if ( m_iMoneyTo + m_iStateGold == 99999 )
							//{
							//	pick = false;
							//	break;
							//}
							pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateMoney));
							GetMoney((*it)->num);
							break;
						case 3://中钱
							//if ( m_iMoneyTo + m_iStateGold == 99999 )
							//{
							//	pick = false;
							//	break;
							//}
							pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateMoney));
							GetMoney(d_iMoneyN);
							break;
						case 4://大钱
							//if ( m_iMoneyTo + m_iStateGold == 99999 )
							//{
							//	pick = false;
							//	break;
							//}
							pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateMoney));
							GetMoney(d_iMoneyL);
							break;
							//m_iStateSkill ++;
							break;
						case 6://
							//if ( m_iGrenadeNum == 10 )
							//{
							//	pick = false;
							//	break;
							//}
							pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateGrenade));
							if ( m_iGrenadeNum == 0 )
								getChildByTag(26)->setVisible(false);
							m_iGrenadeNum ++;
							if ( m_iGrenadeNum > d_iGrenadeMax )
								m_iGrenadeNum = d_iGrenadeMax;
							break;
						case 7://
							//if ( m_iStateHP == 100 )
							//{
							//	pick = false;
							//	break;
							//}
							pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateHP));
							m_iStateHP += 30;
							if ( m_iStateHP > 100 )
								m_iStateHP = 100;
							break;
						case 8://
							//if ( m_iStateHP == 100 )
							//{
							//	pick = false;
							//	break;
							//}
							pAction = CCMoveTo::create(0.5, CCPoint(d_fShowLocateHP));
							m_iStateHP += 60;
							if ( m_iStateHP > 100 )
								m_iStateHP = 100;
							break;
						case 9://
							//if ( m_iStateHP == 100 && m_iHealBox == d_iHealBoxMax )
							//{
							//	pick = false;
							//	break;
							//}
							{
								int a = m_iHealBox < 3 ? m_iHealBox : 2;
								pAction = CCMoveTo::create(0.5, CCPoint(d_LocateHealBox(a)));
								m_iHealBox ++;
								if ( m_iHealBox > d_iHealBoxMax )
								{
									m_iHealBox = d_iHealBoxMax;
									m_iStateHP = 100;
								}
							}
							break;
						case 5:
						case 10://红技能
							//if ( m_iStateSkill[0] == 100 )
							//{
							//	pick = false;
							//	break;
							//}
							pAction = CCMoveTo::create(0.5, CCPoint(383, 440));
							break;
						case 11://蓝技能
							{
								GuideTalk(8);
								m_bGuideWeaponGet = true;
							}
							pAction = CCMoveTo::create(0.5, CCPoint(50, 440));
							break;
						case 20:
						case 21:
						case 22:
						case 23:
						case 24:
						case 25:
							if ( AppDelegate::s_Hero3Array[(*it)->type-20] == 0 )
							{
								AppDelegate::s_Hero3Array[(*it)->type-20] = 1;
								AppDelegate::s_Hero3 --;
							}
							pAction = CCScaleTo::create(0.5, 10.0f);
							scheduleOnce(schedule_selector(CGameControler::GuideGoToChoseHero), 0.6f);
							break;
						default:
							break;
						}
				}
				//if ( !pick )
				//	continue;
				if ( !m_pGetItemEffect )
				{
					m_pGetItemEffect = common::CreateAnimation("Animation/GetItem/1.png", ccp(0, d_fHeroHeight), ccp(0.5f, 0)
						,"Animation/GetItem/", 6, CCRect(0, 0, d_sizeGetItem), 
						false, false, 0.1f, CCCallFunc::create(this, callfunc_selector(CGameControler::GetItemEffectOver)) );
					m_pGetItemEffect->setPosition( ccp(m_fHeroLocateX, m_fHeroLocateY) );
					m_pCFrontgroud->GetTileMap()->addChild(m_pGetItemEffect, 5);
				}
				m_pCFrontgroud->GetTileMap()->removeChild((*it)->pSprite1);
				if ( (*it)->pSprite2 )
				{
					(*it)->pSprite2->removeFromParent();
					(*it)->pSprite2 = NULL;
				}
				if ( 1 || (*it)->type < 20 || (*it)->type > 29 )
				{
					if ( 10 != (*it)->type && 5 != (*it)->type )
					{
						char buffer[25];
						if ( 11 == (*it)->type )
							sprintf(buffer,"tu3/qiang2/3.png");
						else if ( 29 < (*it)->type )
							sprintf(buffer,"item/bullet/%d.png", (*it)->type-30);
						else if ( 19 < (*it)->type)
							sprintf(buffer,"tu16/item/35.png");
						else
							sprintf(buffer,"item/item%d.png",(*it)->type+1);
						CCSprite* pSprite = CCSprite::create(buffer);
						pSprite->setPosition( ccp((*it)->x-m_fHeroLocateX+m_fRelativeX, (*it)->y+m_iYFix) );
						addChild(pSprite, 5);
						pSprite->runAction( CCSequence::create(pAction, CCCallFuncND::create(this, callfuncND_selector(CGameControler::CallBackRemoveSelf), pSprite), NULL) );
					}
					else
					{
						char buffer[255];
						sprintf(buffer, "item/%d/particle.plist", 0);
						CCParticleSystemQuad* pParticle = CCParticleSystemQuad::create(buffer);
						pParticle->setPosition( ccp((*it)->x-m_fHeroLocateX+m_fRelativeX, ((*it)->y+m_iYFix)) );
						pParticle->setAnchorPoint(ccp(0.5f, 0.0f));
						pParticle->setTag((*it)->type);
						addChild(pParticle, 2);
						pParticle->runAction( CCSequence::create(pAction, CCCallFuncND::create(this, callfuncND_selector(CGameControler::CallBackRemoveSelfP), pParticle), NULL) );

#ifndef GameTypeC
						sprintf(buffer, "item/%d/", 0);
						std::string path = (std::string)buffer + (std::string)"1.png";
						CCSprite* pSprite = common::CreateAnimation(path.c_str(), ccp((*it)->x, (*it)->y), ccp(0.5f, 0.5f),
							buffer, 7, CCRect(0,0,88,92), false, false, 0.1f, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)) );
						m_pCFrontgroud->GetTileMap()->addChild(pSprite, 3);
#endif
					}
				}

				DELETEITERATORFROMVECTOR(m_vecItemObejctArray, it);
				bStop = false;
				break;
			}
			if ( (*it)->time > 570 && 6 != m_iLevelType )
			{
				m_pCFrontgroud->GetTileMap()->removeChild((*it)->pSprite1);
				if ( (*it)->pSprite2 )
				{
					(*it)->pSprite2->removeFromParent();
					(*it)->pSprite2 = NULL;
				}
				DELETEITERATORFROMVECTOR(m_vecItemObejctArray, it);
				bStop = false;
				break;
			}
			if ( (*it)->pSprite2 )
			{
				bool bShow = false;
				if ( (*it)->time < 450 )
				{
					if ( !(((*it)->time/30)%2) )
						bShow = true;
				}
				else
					if ( !(((*it)->time/15)%2) )
						bShow = true;
				(*it)->pSprite2->setPositionY( (*it)->y + abs(10-(*it)->time/5%20) );
				(*it)->pSprite1->setVisible(bShow);
				(*it)->pSprite2->setVisible(!bShow);
			}
			(*it)->pSprite1->setPositionY( (*it)->y + abs(10-(*it)->time/5%20) );
		}
		if ( bStop )
			break;
	}
}

void CGameControler::CallBackRemoveSelf( CCNode* _pNode )
{
	_pNode->stopAllActions();
	_pNode->removeFromParent();
}

void CGameControler::CallBackRemoveSelf( CCNode* _pNode, void* data )
{
	CCSprite* pObject = (CCSprite*)data;
	if ( fabs(pObject->getPositionX()-d_LocateHealX) < 80.0f &&
		fabs(pObject->getPositionY()-d_LocateHealY) < 10.0f )
	{
		_pNode->stopAllActions();
		_pNode->setScale(2.0f);
		CCScaleTo* pAction1 = CCScaleTo::create(0.3f, 1.0f);
		_pNode->runAction( CCSequence::create(pAction1, CCDelayTime::create(1.0f),
			CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf2)), NULL) );
#ifndef GameTypeC
		CCSprite* pSprite = common::CreateAnimation("Animation/gethealbox/1.png", ccp(33, 21), ccp(0.5f, 0.5f),
			"Animation/gethealbox/", 5, CCRect(0, 0, 181, 127), false, false, 0.1f, 
			CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
		_pNode->addChild(pSprite, -1);
#endif
	}
	else
	{
		_pNode->removeFromParentAndCleanup(true);
	}
}

void CGameControler::CallBackRemoveSelfRelife()
{
	if ( m_pRelife )
	{
		m_pRelife->stopAllActions();
		m_pRelife->removeFromParent();
		m_pRelife = NULL;
	}
}

void CGameControler::CallBackRemoveSelfP( cocos2d::CCNode* _pNode, void* data )
{//技能拾取
	CCParticleSystemQuad* pParticle = (CCParticleSystemQuad*)data;
	CCProgressTimer* pNode;
	CCNode* pNode1;
	pNode1 = getChildByTag(28);
	pNode = (CCProgressTimer*)pNode1->getChildByTag(888);
	if ( m_iStateSkill[0] < 100 )
	{
		if ( pParticle->getTag() < g_iItemNum )
		{
			if ( m_iLevelType != 6 )
				m_iStateSkill[0] += 10;
			else
				m_iStateSkill[0] += 50;
		}
		else
			m_iStateSkill[0] += 5;
		SkillFullCheck();
	}
	//else
	//{
	//	pNode1 = getChildByTag(29);
	//	pNode = (CCProgressTimer*)pNode1->getChildByTag(888);
	//	if ( m_iStateSkill[1] < 100 )
	//	{
	//		m_iStateSkill[1] += 34;
	//		if ( m_iStateSkill[1] >= 100 )
	//		{
	//			m_iSkillFullTime[1] = 0;
	//			m_iStateSkill[1] = 100;
	//			CCAnimate* pAction = CCAnimate::create( common::CreateAnimationFrame("Animation/skill/3/", 4, CCRect(0,0,76,93)) );
	//			CCSprite* pSprite = CCSprite::create("Animation/skill/3/1.png");
	//			pSprite->setPosition( ccp(492, 440) );
	//			pSprite->runAction( CCSequence::create(pAction, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL) );
	//			addChild(pSprite, 18);
	//		}
	//	}
	//}
	pParticle->removeFromParentAndCleanup(true);

	if ( pNode )
	{
		CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 1.5f);
		CCScaleTo* pAction2 = CCScaleTo::create(0.1f, 1.0f);
		pNode->stopAllActions();
		pNode->runAction( CCSequence::create( pAction1, pAction2, NULL) );
	}

	CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 1.5f);
	CCScaleTo* pAction2 = CCScaleTo::create(0.1f, 1.0f);
	pNode1->stopAllActions();
	pNode1->runAction( CCSequence::create( pAction1, pAction2, NULL) );
}

void CGameControler::CallBackRemoveSelf2( CCNode* _pNode )
{
	ShowHealBox();
	_pNode->removeFromParentAndCleanup(true);
}

void CGameControler::MonsterFresh()
{
	if ( 2 != m_iLevelType || m_pCFrontgroud->m_vecObjectArray.size() || m_vecMonsterArray.size() )
		return;
	
	if ( m_iWaveDistTime > 0 )
	{
		m_iWaveDistTime--;
		return;
	}
	if ( m_iWave == m_iWaveThis )
	{
		m_iWave++;
		m_iWaveDistTime = 250;
	}
	else
	{
		CCArray* pArray = m_pCFrontgroud->m_pMonsterArray;
		for ( unsigned int i = 0; i < pArray->count(); i++ )
		{//本波刷怪
			CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
			if ( m_iWave != pObject->valueForKey("wave")->intValue() )
				continue;
			int num = pObject->valueForKey("num")->intValue();
			int time = 0;
			for ( int j = 0; j < num; j++ )
			{
				MonsterObject* pMonsterInfo = new MonsterObject;
				pMonsterInfo->y		= pObject->valueForKey("y")->floatValue();
#ifdef GameTypeC
				int id = pObject->valueForKey("id")->uintValue()%100 - 1;
				id = id%2;
				pMonsterInfo->kind = id;
#else
				pMonsterInfo->kind	= pObject->valueForKey("id")->uintValue()%100 - 1;
#endif
				pMonsterInfo->time	= time + rand()%100 + 50;
				pMonsterInfo->range = pObject->valueForKey("toward")->intValue();
				pMonsterInfo->num	= 0;
				pMonsterInfo->index = 0;
				m_pCFrontgroud->m_vecObjectArray.push_back(pMonsterInfo);
				time = pMonsterInfo->time;
			}
		}
		m_iWaveThis = m_iWave;
		if ( m_iWaveThis <= g_unWaveNum[m_unLevel/6] )
			((ccbGameStart*)m_pGameStart)->Wave(m_iWave);
		//((ccbGameStart*)getChildByTag(55))->Appear();
	}
}

void CGameControler::MonsterRun()
{
	if ( 6 == m_iLevelType )
		if ( 9 == m_iGuideNow )
			return;
	//////////////////
	/****怪物生成****/
	//////////////////
	if ( m_pCFrontgroud->m_vecObjectArray.size() )
	{
		for ( std::vector<MonsterObject*>::iterator it = m_pCFrontgroud->m_vecObjectArray.begin();
			it != m_pCFrontgroud->m_vecObjectArray.end(); it++ )
		{
			if ( 2 != m_iLevelType )
			{
				if ( (*it)->x > m_fHeroLocateX - m_fRelativeX + 800 )
					continue;
				if ( (*it)->x < m_fHeroLocateX - m_fRelativeX )
					continue;
				if ( fabs((*it)->y - m_fHeroLocateY) > 300 )
					continue;
			}

			if ( (*it)->kind == 17 || (*it)->kind == 14 || (*it)->kind == 13 
				|| (*it)->kind == 12 || (*it)->kind == 8 || (*it)->kind == 5 )
			{
				if ( m_fHeroLocateX - m_fRelativeX < d_fMapSizeX * (m_iMapNum-1.40f) )
					continue;
			}

			if ( m_unLevel == 30 && (*it)->kind == 17 )
			{
				if ( m_fHeroLocateY < m_pCFrontgroud->GetTileMap()->getMapSize().height*m_pCFrontgroud->GetTileMap()->getTileSize().height-480 )
					continue;
			}

			if ( m_iActionIndex != (*it)->index )
				continue;
			if ( (*it)->time > 0 )
				(*it)->time--;
			else
			{
				MonsterInfomation* pMonsterInfo = new MonsterInfomation;
				pMonsterInfo->type	= (*it)->kind;
				if ( 2 != m_iLevelType )
				{
					pMonsterInfo->x	= (*it)->x;
					if ( (*it)->range )
						pMonsterInfo->x += rand()%(*it)->range - (*it)->range/2;
					if ( pMonsterInfo->x > m_fHeroLocateX - m_fRelativeX + 800 - 50 )
						pMonsterInfo->x = m_fHeroLocateX - m_fRelativeX + 800 - 50;
					else if ( pMonsterInfo->x < m_fHeroLocateX - m_fRelativeX + 50 )
						pMonsterInfo->x = m_fHeroLocateX - m_fRelativeX + 50;
				}
				else
				{
					if ( (*it)->range == 0 )
						pMonsterInfo->x	= -40.0f;
					else
						pMonsterInfo->x	= m_iMapNum*800 + 40.0f;
				}
				pMonsterInfo->y		= (*it)->y;
				pMonsterInfo->cx	= pMonsterInfo->x;
				pMonsterInfo->cy	= pMonsterInfo->y;
				pMonsterInfo->w		= g_fMonsterWidth[(*it)->kind];
				pMonsterInfo->h		= g_fMonsterHeight[(*it)->kind];
				pMonsterInfo->hp	= g_iMonsterHp[(*it)->kind];
				pMonsterInfo->hpMax = pMonsterInfo->hp;
				if ( pMonsterInfo->type != 5 && pMonsterInfo->type != 8 && pMonsterInfo->type != 12 
					&& pMonsterInfo->type != 13 && pMonsterInfo->type != 14 && pMonsterInfo->type != 17 )
				{//普通血条
					pMonsterInfo->pBloodBoard = CCSprite::create("Monster/xue1.png");
					pMonsterInfo->pBloodBoard->setPosition( ccp(pMonsterInfo->x, pMonsterInfo->y + pMonsterInfo->h + 50.0) );
					pMonsterInfo->pBloodBoard->setAnchorPoint( ccp(0.5f, 0.5f) );
					pMonsterInfo->pBloodBoard->setVisible(false);
					m_pCFrontgroud->GetTileMap()->addChild(pMonsterInfo->pBloodBoard, 3);

					CCSprite* pBar = CCSprite::create("Monster/xue2.png");
					pMonsterInfo->pBloodBar = CCProgressTimer::create(pBar);
					pMonsterInfo->pBloodBar->setPosition( ccp(0,0) );
					pMonsterInfo->pBloodBar->setAnchorPoint( ccp(0.0f, 0.0f) );
					pMonsterInfo->pBloodBar->setType(kCCProgressTimerTypeBar);//
					pMonsterInfo->pBloodBar->setMidpoint(ccp(0, 0.5));
					pMonsterInfo->pBloodBar->setBarChangeRate(ccp(1, 0));
					pMonsterInfo->pBloodBar->setPercentage(100); 
					//pMonsterInfo->pBloodBar->setVisible(false);
					pMonsterInfo->pBloodBoard->addChild(pMonsterInfo->pBloodBar);
				}
				else
				{//BOSS血条
					m_pBossBloodBoard = CCSprite::create("Monster/shang.png");
					m_pBossBloodBoard->setPosition(ccp(521, 400));
					addChild(m_pBossBloodBoard, 0, 19);

					CCSprite* pSprite = CCSprite::create("Monster/xia.png");
					pSprite->setPosition(ccp(231, 20));
					m_pBossBloodBoard->addChild(pSprite, -2);

					CCSprite* pBar = CCSprite::create("Monster/zhong.png");
					m_pBossBloodProgress = CCProgressTimer::create(pBar);
					m_pBossBloodProgress->setPosition( ccp(0,0) );
					m_pBossBloodProgress->setAnchorPoint( ccp(0.0f, 0.0f) );
					m_pBossBloodProgress->setType(kCCProgressTimerTypeBar);//
					m_pBossBloodProgress->setMidpoint(ccp(0, 0.5));
					m_pBossBloodProgress->setBarChangeRate(ccp(1, 0));
					m_pBossBloodProgress->setPercentage(100); 
					m_pBossBloodBoard->addChild(m_pBossBloodProgress, -1);
				}
				pMonsterInfo->liveTime		= g_iMonsterAppearTime[(*it)->kind];
				pMonsterInfo->canDeadOnec	= g_bMonsterCanDeadOnec[(*it)->kind];
				pMonsterInfo->attackTime	= 0;
				pMonsterInfo->lastAction	= g_iMonsterBorn[pMonsterInfo->type];
				if ( (*it)->kind == 4 )
				{//金毛,陨石
					pMonsterInfo->SpeedY = -1.5f;
					pMonsterInfo->bRock = true;
					pMonsterInfo->pLand = common::CreateAnimation("Animation/rock/c1.png", 
						ccp(pMonsterInfo->x-(*it)->dist/2, pMonsterInfo->y-(*it)->dist), ccp(0.5f, 0.5f), 
						"Animation/rock/c", 4, CCRect(0,0,d_sizeRockAim), true );
					m_pCFrontgroud->GetTileMap()->addChild(pMonsterInfo->pLand, 2);
				}
				else
				{
					if ( pMonsterInfo->type == 5 )
					{
						pMonsterInfo->CoolTime1 = 200;
						pMonsterInfo->CoolTime2 = 200;
						pMonsterInfo->CoolTime3 = 1;
					}
					else if ( pMonsterInfo->type == 12 )
					{
						pMonsterInfo->canAttack = false;
						pMonsterInfo->CoolTime1 = BOSS3_NEED_DAMAGE;
						pMonsterInfo->CoolTime2 = 200;
						pMonsterInfo->CoolTime3 = 0;
						pMonsterInfo->CoolTime4 = rand()%3+3;
						pMonsterInfo->CoolTime6 = pMonsterInfo->y;
					}
					else if ( pMonsterInfo->type == 13 )
					{
						pMonsterInfo->CoolTime1 = 400;
						pMonsterInfo->CoolTime2 = 270;
						pMonsterInfo->CoolTime3 = 100;
						pMonsterInfo->CoolTime4 = 100;
						pMonsterInfo->CoolTime5 = 0;
						pMonsterInfo->CoolTime6 = 0;
					}
					else if ( (*it)->kind == 14 )
					{
						m_iBossLimitX = pMonsterInfo->x - 195.0f;
						pMonsterInfo->CoolTime1 = 0;
						pMonsterInfo->CoolTime2 = 400;
						pMonsterInfo->CoolTime3 = 200;
						pMonsterInfo->CoolTime4 = 0;
						pMonsterInfo->CoolTime5 = BOSS5_NEED_DAMAGE;
						pMonsterInfo->CoolTime6 = 100;
					}
					else if ( (*it)->kind == 17 )
					{//BOSS6
						pMonsterInfo->CoolTime1 = 1000;
						pMonsterInfo->CoolTime2 = 200;
						pMonsterInfo->CoolTime3 = 800;
						pMonsterInfo->CoolTime4 = 2500;
						pMonsterInfo->CoolTime5 = 0;
						pMonsterInfo->CoolTime6 = 0;

						pMonsterInfo->pSprite1 = cs::Armature::create("jsboss6");
						pMonsterInfo->pSprite1->setPosition(pMonsterInfo->x+20, pMonsterInfo->y+170);
						pMonsterInfo->pSprite1->setOpacity(128);
						pMonsterInfo->pSprite1->setScale(0.8f);
						pMonsterInfo->pSprite1->setVisible(false);
						m_pHero->SetMonster(pMonsterInfo->pSprite1);

						pMonsterInfo->pSprite2 = cs::Armature::create("jsboss6");
						pMonsterInfo->pSprite2->setPosition(pMonsterInfo->x-720, pMonsterInfo->y+170);
						pMonsterInfo->pSprite2->setOpacity(128);
						pMonsterInfo->pSprite2->setScaleY(0.8f);
						pMonsterInfo->pSprite2->setScaleX(-0.8f);
						pMonsterInfo->pSprite2->setVisible(false);
						m_pHero->SetMonster(pMonsterInfo->pSprite2);
					}
					if ( pMonsterInfo->type == 8 )
					{//BOSS2的多头
						m_iBossLimitX = pMonsterInfo->x - 10.0f;
						pMonsterInfo->ActionType = 0;
						pMonsterInfo->CoolTime1 = 100;
						pMonsterInfo->CoolTime2 = 110;
						pMonsterInfo->CoolTime3 = 120;
						pMonsterInfo->CoolTime4 = 510;
						pMonsterInfo->CoolTime5 = 500;
						pMonsterInfo->CoolTime6 = 490;
						pMonsterInfo->hp  = 1;
						pMonsterInfo->hp1 = g_iMonsterHp[(*it)->kind];
						pMonsterInfo->hp2 = g_iMonsterHp[(*it)->kind];
						pMonsterInfo->hp3 = g_iMonsterHp[(*it)->kind];
						pMonsterInfo->hpMax = g_iMonsterHp[(*it)->kind];

						pMonsterInfo->pSprite3 = cs::Armature::create("jsboss2_hou");
						pMonsterInfo->pSprite3->setPosition(pMonsterInfo->x + d_locationBoss2Head3 + pMonsterInfo->y);
						pMonsterInfo->pSprite3->getAnimation()->playByIndex(0, 0, 60);
						m_pHero->SetMonster(pMonsterInfo->pSprite3);

						pMonsterInfo->pSprite2 = cs::Armature::create("jsboss2_zhu");
						pMonsterInfo->pSprite2->setPosition(pMonsterInfo->x + d_locationBoss2Head2 + pMonsterInfo->y);
						pMonsterInfo->pSprite2->getAnimation()->playByIndex(0, 0, 60);
						m_pHero->SetMonster(pMonsterInfo->pSprite2);

						pMonsterInfo->pSprite1 = cs::Armature::create("jsboss2_du");
						pMonsterInfo->pSprite1->setPosition(pMonsterInfo->x + d_locationBoss2Head1 + pMonsterInfo->y);
						pMonsterInfo->pSprite1->getAnimation()->playByIndex(0, 0, 60);
						m_pHero->SetMonster(pMonsterInfo->pSprite1);

						pMonsterInfo->pSprite = g_pHero->CreateMonster(pMonsterInfo);
					}
					else
					{
						pMonsterInfo->pSprite = g_pHero->CreateMonster(pMonsterInfo);
						pMonsterInfo->pSprite->getAnimation()->playByIndex(pMonsterInfo->lastAction, 0, g_iMonsterFrame[pMonsterInfo->type]);
					}
					if ( pMonsterInfo->x > m_fHeroLocateX )
						pMonsterInfo->pSprite->setScaleX(-1);
				}
				m_vecMonsterArray.push_back(pMonsterInfo);
				if ( 0 == (*it)->num )
				{
					delete *it;
					m_pCFrontgroud->m_vecObjectArray.erase(it);
				}
				else
				{
					if ( 0 < (*it)->num  )
						(*it)->num --;
					if ( (*it)->timemax == 0 )
						(*it)->time = 0;
					else if ( 2 != m_iLevelType )
						(*it)->time = 50*(rand()%((*it)->timemax - (*it)->timemin) + (*it)->timemin);
				}
				break;
			}
		}
	}

	//////////////////////
	/****技能生成怪物****/
	//////////////////////
	if ( m_bCreateMonster )
	{
		if ( m_bCreateMonster == 5 )
		{
			int num = rand()%3 + 3;
			for ( int i = 0; i < num; i++ )
			{
				MonsterInfomation* pMonsterInfo = new MonsterInfomation;
#ifdef GameTypeC
				pMonsterInfo->type	= 0;
				pMonsterInfo->y		= 100;
#else
				pMonsterInfo->type	= 4;
				pMonsterInfo->y		= 480;
#endif
				pMonsterInfo->x		= rand()%400+200+800*(m_iMapNum-1);
				pMonsterInfo->cx	= pMonsterInfo->x;
				pMonsterInfo->cy	= pMonsterInfo->y;
				pMonsterInfo->w		= g_fMonsterWidth[pMonsterInfo->type];
				pMonsterInfo->h		= g_fMonsterHeight[pMonsterInfo->type];
				pMonsterInfo->hp	= g_iMonsterHp[pMonsterInfo->type];
				pMonsterInfo->liveTime		= g_iMonsterAppearTime[pMonsterInfo->type];
				pMonsterInfo->canDeadOnec	= false;
				pMonsterInfo->attackTime	= 0;
				pMonsterInfo->lastAction	= g_iMonsterBorn[pMonsterInfo->type];

				pMonsterInfo->SpeedY = -1.5f;
				pMonsterInfo->bRock = true;
				pMonsterInfo->pLand = common::CreateAnimation("Animation/rock/c1.png", 
					ccp(pMonsterInfo->x-185, pMonsterInfo->y-370), ccp(0.5f, 0.5f), 
					"Animation/rock/c", 4, CCRect(0,0,d_sizeRockAim), true );
				m_pCFrontgroud->GetTileMap()->addChild(pMonsterInfo->pLand, 2);
				m_vecMonsterArray.push_back(pMonsterInfo);

				pMonsterInfo->pBloodBoard = CCSprite::create("Monster/xue1.png");
				pMonsterInfo->pBloodBoard->setPosition( ccp(pMonsterInfo->x, pMonsterInfo->y + pMonsterInfo->h + 50.0) );
				pMonsterInfo->pBloodBoard->setAnchorPoint( ccp(0.5f, 0.5f) );
				pMonsterInfo->pBloodBoard->setVisible(false);
				m_pCFrontgroud->GetTileMap()->addChild(pMonsterInfo->pBloodBoard, 3);

				CCSprite* pBar = CCSprite::create("Monster/xue2.png");
				pMonsterInfo->pBloodBar = CCProgressTimer::create(pBar);
				pMonsterInfo->pBloodBar->setPosition( ccp(0,0) );
				pMonsterInfo->pBloodBar->setAnchorPoint( ccp(0.0f, 0.0f) );
				pMonsterInfo->pBloodBar->setType(kCCProgressTimerTypeBar);//
				pMonsterInfo->pBloodBar->setMidpoint(ccp(0, 0.5));
				pMonsterInfo->pBloodBar->setBarChangeRate(ccp(1, 0));
				pMonsterInfo->pBloodBar->setPercentage(100); 
				//pMonsterInfo->pBloodBar->setVisible(false);
				pMonsterInfo->pBloodBoard->addChild(pMonsterInfo->pBloodBar);
			}
		}
		else
		{
			MonsterInfomation* pMonsterInfo = new MonsterInfomation;
			pMonsterInfo->type	= m_bCreateMonster - 1;
			pMonsterInfo->x		= m_iBossCreateX;
			pMonsterInfo->y		= m_iBossCreateY;
			pMonsterInfo->cx	= pMonsterInfo->x;
			pMonsterInfo->cy	= pMonsterInfo->y;
			pMonsterInfo->w		= g_fMonsterWidth[pMonsterInfo->type];
			if ( pMonsterInfo->type == 16 )
			{
				pMonsterInfo->h = 0;
				pMonsterInfo->CoolTime1 = 100;
				pMonsterInfo->CoolTime2 = 400;
				pMonsterInfo->CoolTime3 = 300;
				pMonsterInfo->CoolTime4 = 0;
				pMonsterInfo->CoolTime5 = 0;
				pMonsterInfo->CoolTime6 = 50;
				pMonsterInfo->CoolTime7 = rand()%200;
			}
			else
				pMonsterInfo->h	= g_fMonsterHeight[pMonsterInfo->type];
			pMonsterInfo->hp	= g_iMonsterHp[pMonsterInfo->type];
			pMonsterInfo->liveTime		= g_iMonsterAppearTime[pMonsterInfo->type];
			pMonsterInfo->canDeadOnec	= g_bMonsterCanDeadOnec[pMonsterInfo->type];
			pMonsterInfo->attackTime	= 0;
			pMonsterInfo->lastAction	= g_iMonsterBorn[pMonsterInfo->type];
			pMonsterInfo->pSprite = g_pHero->CreateMonster(pMonsterInfo);
			pMonsterInfo->pSprite->setScaleX(-1.0f);
			if ( pMonsterInfo->type != 16 )
				pMonsterInfo->pSprite->getAnimation()->playByIndex(pMonsterInfo->lastAction, 0, g_iMonsterFrame[pMonsterInfo->type]);

			pMonsterInfo->pBloodBoard = CCSprite::create("Monster/xue1.png");
			pMonsterInfo->pBloodBoard->setPosition( ccp(pMonsterInfo->x, pMonsterInfo->y + pMonsterInfo->h + 50.0) );
			pMonsterInfo->pBloodBoard->setAnchorPoint( ccp(0.5f, 0.5f) );
			pMonsterInfo->pBloodBoard->setVisible(false);
			m_pCFrontgroud->GetTileMap()->addChild(pMonsterInfo->pBloodBoard, 3);

			CCSprite* pBar = CCSprite::create("Monster/xue2.png");
			pMonsterInfo->pBloodBar = CCProgressTimer::create(pBar);
			pMonsterInfo->pBloodBar->setPosition( ccp(0,0) );
			pMonsterInfo->pBloodBar->setAnchorPoint( ccp(0.0f, 0.0f) );
			pMonsterInfo->pBloodBar->setType(kCCProgressTimerTypeBar);//
			pMonsterInfo->pBloodBar->setMidpoint(ccp(0, 0.5));
			pMonsterInfo->pBloodBar->setBarChangeRate(ccp(1, 0));
			pMonsterInfo->pBloodBar->setPercentage(100); 
			//pMonsterInfo->pBloodBar->setVisible(false);
			pMonsterInfo->pBloodBoard->addChild(pMonsterInfo->pBloodBar);

			m_vecMonsterArray.push_back(pMonsterInfo);
		}
		m_bCreateMonster = 0;
	}

	//////////////////
	/****怪物行为****/
	//////////////////
	m_bAttacked			= false;
	m_bAimMonster		= false;
	float dist			= 500.0;
	if ( !m_vecMonsterArray.size() )
		return;
	for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
		it != m_vecMonsterArray.end(); it++ )
	{
		//////////////
		/****陨石****/
		//////////////
		if ( (*it)->type == 4 && (*it)->bRock )
		{
			if ( (*it)->rockTime > 0 )
			{
				(*it)->rockTime--;
				if ( 0 == (*it)->rockTime )
				{
					AppDelegate::AudioPlayEffect("MS/Sound/EfRock.mp3");
					(*it)->pAction = CCSprite::create("Animation/rock/1.png");
					(*it)->pAction->setScale(0.2f);
					common::RunNewAnimation((*it)->pAction, "Animation/rock/", 4, CCRect(0,0,d_sizeRock), true);
					(*it)->pAction->runAction( CCScaleTo::create(1.0f, 1.2f) );
					m_pCFrontgroud->GetTileMap()->addChild((*it)->pAction, 2);
					(*it)->pLand->stopAllActions();
					common::RunNewAnimation((*it)->pLand, "Animation/rock/b", 3, CCRect(0,0,d_sizeRockAim), true, true, 0.20f);
				}
			}
			else
			{
				(*it)->SpeedY	-= d_fHeroGravity;
				(*it)->y		+= (*it)->SpeedY;
				(*it)->x		+= (*it)->SpeedY/2;
				if ( fabs((*it)->x - m_fHeroLocateX) < 10+d_fHeroWidth 
					&& RectInRect((*it)->y, (*it)->h, m_fHeroLocateY, d_fHeroHeight) )
				{
					BeAttack(g_iMonsterDamage[(*it)->type]);
					(*it)->pAction->removeFromParentAndCleanup(true);
					(*it)->pLand->removeFromParentAndCleanup(true);
					(*it)->pAction = NULL;
					(*it)->pLand = NULL;
					delete *it;
					m_vecMonsterArray.erase(it);
					break;
				}
				else if ( CheckInWall((*it)->x, (*it)->y, d_fGrenadeW, d_fGrenadeH) )
				{
					MonsterMoveCheck1((*it)->x, (*it)->y);
					(*it)->pAction->setPosition( ccp((*it)->x, (*it)->y) );
					(*it)->bRock = false;
					(*it)->pAction->removeFromParentAndCleanup(true);
					(*it)->pLand->removeFromParentAndCleanup(true);
					(*it)->pAction = NULL;
					(*it)->pLand = NULL;
					(*it)->pAction = common::CreateAnimation("Animation/rock/a1.png", ccp((*it)->x, (*it)->y), ccp(0.5f,0.3f), 
						"Animation/rock/a", 7, CCRect(0,0,d_sizeRock2), false, false, 0.1f, 
						CCCallFuncND::create(this, callfuncND_selector(CGameControler::CallBackRemoveSelfAction), &((*it)->pAction)));
					m_pCFrontgroud->GetTileMap()->addChild((*it)->pAction, 3);
					(*it)->pSprite = g_pHero->CreateMonster(*it);
					(*it)->pSprite->getAnimation()->playByIndex((*it)->lastAction, 0, g_iMonsterFrame[(*it)->type]);
					if ( (*it)->x > m_fHeroLocateX )
						(*it)->pSprite->setScaleX(-1);
					return;
				}
				else
					(*it)->pAction->setPosition( ccp((*it)->x, (*it)->y) );
			}
			continue;
		}
		//////////////
		/****死亡****/
		//////////////
		if ( !(*it)->pSprite )
		{
			delete *it;
			m_vecMonsterArray.erase(it);
			break;
		}
		if ( !(*it)->alive || m_bBossWin )
		{
			if ( (*it)->firedead )
			{
				CCSprite* pSprite = CCSprite::create("Animation/FireDead/hui1.png");
				CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("Animation/FireDead/hui", 7, CCRect(0,0,95,132)));
				pSprite->setAnchorPoint(ccp(0.5f,0.05f));
				pSprite->setPosition(ccp((*it)->x, (*it)->y));
				pSprite->runAction(CCSequence::create(pAction, CCDelayTime::create(0.5f), 
					CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
				(*it)->pSprite->getParent()->addChild(pSprite, (*it)->pSprite->getZOrder()-1);

				m_pHero->RemoveMonster((*it)->pSprite);
				if ( (*it)->pSprite1 )
				{
					m_pHero->RemoveMonster((*it)->pSprite1);
					(*it)->pSprite1 = NULL;
				}
				if ( (*it)->pSprite2 )
				{
					m_pHero->RemoveMonster((*it)->pSprite2);
					(*it)->pSprite2 = NULL;
				}
				if ( (*it)->pSprite3 )
				{
					m_pHero->RemoveMonster((*it)->pSprite3);
					(*it)->pSprite3 = NULL;
				}
				delete *it;
				m_vecMonsterArray.erase(it);
				break;
			}
			else if ( (*it)->deadTime > 0 || m_bBossWin )
			{
				if ( 6 == (*it)->type )
				{
					(*it)->SpeedY	-= d_fHeroGravity;
					(*it)->y		+= (*it)->SpeedY;
					(*it)->pSprite->setPositionY((*it)->y);
				}

				(*it)->deadTime--;
				if ( (*it)->deadTime == 0 || m_bBossWin )
				{
					///////怪死亡变身////////
					if ( (*it)->willRelife )
					{
						(*it)->deadTime = 100;
						(*it)->alive = true;
						(*it)->willRelife = false;
						(*it)->secondLife = true;
						(*it)->lastAction = -1;
						(*it)->hp = g_iMonsterHp[(*it)->type]/2;
					}
					else
					{
						//m_pHero->RemoveMonsterBlood();
						m_pHero->RemoveMonster((*it)->pSprite);
						if ( (*it)->pSprite1 )
						{
							m_pHero->RemoveMonster((*it)->pSprite1);
							(*it)->pSprite1 = NULL;
						}
						if ( (*it)->pSprite2 )
						{
							m_pHero->RemoveMonster((*it)->pSprite2);
							(*it)->pSprite2 = NULL;
						}
						if ( (*it)->pSprite3 )
						{
							m_pHero->RemoveMonster((*it)->pSprite3);
							(*it)->pSprite3 = NULL;
						}
						delete *it;
						m_vecMonsterArray.erase(it);
						break;
					}
				}
			}
			else if ( (*it)->deadTime == -1 )
			{
				if ( (*it)->type == 0 || (*it)->type == 1 || (*it)->type == 2 || 
					(*it)->type == 3 || (*it)->type == 4 || (*it)->type == 9 || 
					(*it)->type == 10 || (*it)->type == 12 )
				{
					int n = 5;
					int index = 0;
					int array[10] = {0,0,0,0,0,0,0,0,0,0,};
					{
						Chopper* pChopper = new Chopper;
						char buffer[255];
						sprintf(buffer, "Monster/head/%d.png", (*it)->type);
						pChopper->pSprite = CCSprite::create(buffer);
						pChopper->type = -1;
						pChopper->x = (*it)->x;
						pChopper->y = (*it)->y;
						pChopper->vx = rand()%10-5.0;
						pChopper->vy = rand()%5+10.0;
						pChopper->toward = pChopper->vx <= 0 ? -1 : 1;
						pChopper->va = d_fHeroGravity;
						pChopper->damage = 20;
						pChopper->time = 5;
						pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
						pChopper->pSprite->setVisible(false);
						m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
						m_vecChopperArray.push_back(pChopper);
					}
					while ( n > 0 )
					{
						int num = rand()%5+1;
						index = (index + num)%9;
						if ( array[index] == 0 )
						{
							array[index] = 1;
							n--;
							char buffer[255];
							sprintf(buffer, "Monster/body/%d.png", index+1);
							Chopper* pChopper = new Chopper;
							pChopper->pSprite = CCSprite::create(buffer);
							pChopper->type = -1;
							pChopper->x = (*it)->x;
							pChopper->y = (*it)->y;
							pChopper->vx = rand()%10-5.0;
							pChopper->vy = rand()%5+10.0;
							pChopper->toward = pChopper->vx <= 0 ? -1 : 1;
							pChopper->va = d_fHeroGravity;
							pChopper->damage = 20;
							pChopper->time = 5;
							pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
							pChopper->pSprite->setVisible(false);
							m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
							m_vecChopperArray.push_back(pChopper);
						}
					}
				}

				CCSprite* pSprite = common::CreateAnimation("Animation/MonsterDaed/1.png", ccp((*it)->x, (*it)->y), ccp(0.44f, 0.2f),
					"Animation/MonsterDaed/", 6, CCRect(0,0,188,163), false, false, 0.1f, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
				(*it)->pSprite->getParent()->addChild(pSprite, (*it)->pSprite->getZOrder());
				m_pHero->RemoveMonster((*it)->pSprite);
				if ( (*it)->pSprite1 )
				{
					m_pHero->RemoveMonster((*it)->pSprite1);
					(*it)->pSprite1 = NULL;
				}
				if ( (*it)->pSprite2 )
				{
					m_pHero->RemoveMonster((*it)->pSprite2);
					(*it)->pSprite2 = NULL;
				}
				if ( (*it)->pSprite3 )
				{
					m_pHero->RemoveMonster((*it)->pSprite3);
					(*it)->pSprite3 = NULL;
				}
				delete *it;
				m_vecMonsterArray.erase(it);
				break;
			}
			continue;
		}
		//////////////
		/****活动****/
		//////////////
		if ( !SetVisible( (*it)->pSprite, (*it)->x, (*it)->y ) )
		{
			if ( (*it)->pBloodBoard )
				(*it)->pBloodBoard->setVisible(false);
			continue;
		}
		else if ( (*it)->hp != g_iMonsterHp[(*it)->type] )
		{
			if ( (*it)->pBloodBoard )
				(*it)->pBloodBoard->setVisible(true);
		}
		DIS_SELF( (*it)->bloodTime );
		if ( 0 == (*it)->bloodTime && (*it)->pBloodBoard )
			(*it)->pBloodBoard->setVisible(false);
		//////////////
		/****出生****/
		//////////////
		if ( (*it)->liveTime > 0 )
		{
			(*it)->liveTime--;
			continue;
		}
		//////////////
		/****瞄准****/
		//////////////
		if ( m_ewtNowWeapon != eWT_Knife )
		{
			float distBuffer = (*it)->x - m_fHeroLocateX;
			if ( distBuffer*m_fHeroToward > 0 && fabs(distBuffer) < dist )
			{
				float a = atanf( ((*it)->y/*+(*it)->h/2*/-m_fHeroLocateY/*-m_iFireY[0]*/) / fabs((*it)->x-m_fHeroLocateX/*-m_iFireX[0]*m_fHeroToward*/) );
				if ( fabs(a/PI_FIX) < 30 )
				{
					/*bool bBetweenWall = false;
					float fToward = 1;
					float ax, ay, bx;
					if ( distBuffer < 0 )
					{
					ax = (*it)->x;
					ay = (*it)->y + (*it)->h;
					bx = m_fHeroLocateX;
					fToward = -1;
					}
					else
					{
					ax = m_fHeroLocateX;
					ay = m_fHeroLocateY + d_fHeroHeight;
					bx = (*it)->x;
					}
					ax += 20.0f*cos(a);
					ay += 20.0f*sin(a)*fToward;
					while ( ax < bx )
					{
					if ( CheckInWall(ax, ay, 5.0f, 5.0f) )
					{
					bBetweenWall = true;
					break;
					}
					ax += 10.0f*cos(a);
					ay += 10.0f*sin(a)*fToward;
					}
					if ( !bBetweenWall )
					{*/
						m_bAimMonster	= true;
						m_fAimRotation	= a;
						dist = abs(distBuffer);
					//}
				}
			}
		}
		//////////////
		/****硬直****/
		//////////////
		if ( (*it)->bPlant )
		{
			(*it)->pSprite->getAnimation()->pause();
			continue;
		}
		if ( (*it)->holdTime > 0 )
		{
			(*it)->holdTime--;
			continue;
		}
		////////////////
		/****怪行为****/
		////////////////
		DIS_SELF( (*it)->EffectTime1 );
		if ( 0 == (*it)->EffectTime1 )
		{
			(*it)->EffectTime1 = rand()%400 + 500;
			AppDelegate::AudioStopEffect((*it)->EffectID);
			switch ( (*it)->type )
			{
			case 1://男
			case 3:
			case 4:
			case 10:
			case 11:
				(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/male/standby.mp3");
				break;
			case 0://女
			case 2:
			case 9:
				//(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/female/standby.mp3");
				break;
			case 5://1号BOSS
				(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/boss1/standby.mp3");
				break;
			case 7://狗
				(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/dog/standby.mp3");
				break;
			case 6://鹰
				(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/dog/attack.mp3");
				break;
			}
		}

		switch ( (*it)->type )
		{
		case 0://普通女僵尸
		case 2:
		case 3:
		case 4:
		case 7:
		case 9:
		case 11:
			if ( 2 != m_iLevelType )
			{
				int index = (*it)->type != 11 ? 0 : 1;
				float dist = m_bBoss ? 1600 : g_fMonsterAttackDist[(*it)->type];
				if ( ( fabs((*it)->y-m_fHeroLocateY) > d_fHeroHeight 
					|| (fabs((*it)->x-m_fHeroLocateX) > g_fMonsterView && !m_bBoss ) )
					&& (*it)->attackTime >= 0 )
						MonsterMove(*it, index, true);
				else if ( !(*it)->getback && fabs((*it)->x - m_fHeroLocateX) < dist /*&& !(*it)->bPlant*/ )
				{
					if ( (*it)->x > m_fHeroLocateX )
						(*it)->toward = -1;
					else
						(*it)->toward = 1;
					if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight 
						&& fabs((*it)->x-m_fHeroLocateX) < d_fHeroWidth )
					{
						m_iUnderAttacked = 2;
						if ( (*it)->attackTime == 0 )
						{
							AppDelegate::AudioStopEffect((*it)->EffectID);
							switch ( (*it)->type )
							{
							case 1://男
							case 3:
							case 4:
							case 10:
							case 11:
								(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/male/attack.mp3");
								break;
							case 0://女
							case 2:
							case 9:
								(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/female/attack.mp3");
								break;
							case 7://狗
								(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/dog/attack.mp3");
								break;
							}
							(*it)->lastAction = index+1;
							(*it)->attackTime = -1;
							if ( (*it)->secondLife )
								(*it)->pSprite->getAnimation()->playByIndex(g_iMonsterAttackSecond[(*it)->type], 0, g_iMonsterFrame[(*it)->type]/2, 0);
							else
								(*it)->pSprite->getAnimation()->playByIndex((*it)->lastAction, 0, g_iMonsterFrame[(*it)->type]/2, 0);
						}
						else if ( (*it)->attackTime == -12 )
						{
							if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2 
								&& fabs((*it)->x-m_fHeroLocateX) < d_fHeroWidth )
								BeAttack(g_iMonsterDamage[(*it)->type]);
						}
					}
					else if ( (*it)->attackTime >= 0 && fabs((*it)->x-m_fHeroLocateX) > 20.0f )
						MonsterMove(*it, index);
				}
				else
				{
					if ( (*it)->x > m_fHeroLocateX )
						(*it)->toward = -1;
					else
						(*it)->toward = 1;
					MonsterMove(*it, index, true);
					if ( fabs((*it)->y-m_fHeroLocateY) < 20.0f )
						(*it)->getback = 0;
				}
			}
			else
			{
				if ( (*it)->x > m_iMapNum/2.0*800 )
					(*it)->toward = -1;
				else
					(*it)->toward = 1;
				int index = (*it)->type != 11 ? 0 : 1;
				if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2
					&& fabs((*it)->x - m_fHeroLocateX) < d_fHeroWidth  )
				{
					m_iUnderAttacked = 2;
					if ( (*it)->attackTime == 0 )
					{
						AppDelegate::AudioStopEffect((*it)->EffectID);
						switch ( (*it)->type )
						{
						case 1://男
						case 3:
						case 4:
						case 10:
						case 11:
							(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/male/attack.mp3");
							break;
						case 0://女
						case 2:
						case 9:
							(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/female/attack.mp3");
							break;
						case 7://狗
							(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/dog/attack.mp3");
							break;
						}
						(*it)->lastAction = index+1;
						(*it)->attackTime = -1;
						if ( (*it)->secondLife )
							(*it)->pSprite->getAnimation()->playByIndex(g_iMonsterAttackSecond[(*it)->type], 0, g_iMonsterFrame[(*it)->type]/2, 0);
						else
							(*it)->pSprite->getAnimation()->playByIndex((*it)->lastAction, 0, g_iMonsterFrame[(*it)->type]/2, 0);
					}
					else if ( (*it)->attackTime == -12 )
					{
						if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2 
							&& fabs((*it)->x-m_fHeroLocateX) < d_fHeroWidth )
							BeAttack(g_iMonsterDamage[(*it)->type]);
					}
				}
				else if ( fabs((*it)->x - m_iMapNum/2.0*800) < d_fHeroWidth )
				{//打女人
					if ( (*it)->attackTime == 0 )
					{
						(*it)->lastAction = index+1;
						(*it)->attackTime = -1;
						m_iVIPHp -= g_iMonsterDamage[(*it)->type];
						m_pVIP->getAnimation()->playByIndex(1, 0, 30, 0);
						m_iVIPLastAction = 1;
						m_iVIPAttackedTime = 50;

						if ( fabs(m_fHeroLocateX - m_iMapNum/2.0*800) >= 0 )
						{
							m_iVipTipTime = 50;
							m_pVipTip1->setVisible(true);
							m_pVipTip2->setVisible(true);
							int x1 = 200;
							int x2 = 80;
							int toward = -1;
							if ( m_fHeroLocateX < m_iMapNum/2.0*800 )
							{
								x1 = 600;
								x2 = 720;
								toward = 1;
							}
							m_pVipTip1->setPositionX(x1);
							m_pVipTip2->setPositionX(x2);
							m_pVipTip2->setScale(toward);
						}

						if ( (*it)->secondLife )
							(*it)->pSprite->getAnimation()->playByIndex(g_iMonsterAttackSecond[(*it)->type], 0, g_iMonsterFrame[(*it)->type], 0);
						else
							(*it)->pSprite->getAnimation()->playByIndex((*it)->lastAction, 0, g_iMonsterFrame[(*it)->type], 0);
					}
				}
				else if ( (*it)->attackTime >= 0 )
				{
					//打墙
					bool bWall = false;
					MapObject* pObjectArray = m_pCFrontgroud->GetMapObjects();
					int unObjectNum = m_pCFrontgroud->GetObjectNum();
					for ( int i = 0; i < unObjectNum; i++ )
					{
						MapObject* pMapObject = &pObjectArray[i];
						if ( pMapObject->arg1 > 0.0f )
						{
							if ( fabs((*it)->x - pMapObject->x - pMapObject->w/2) < (*it)->w/2 + 40 )
							{
								bWall = true;
								if ( (*it)->attackTime == 0 )
								{
									(*it)->lastAction = index+1;
									(*it)->attackTime = -1;
									if ( (*it)->secondLife )
										(*it)->pSprite->getAnimation()->playByIndex(g_iMonsterAttackSecond[(*it)->type], 0, g_iMonsterFrame[(*it)->type], 0);
									else
										(*it)->pSprite->getAnimation()->playByIndex((*it)->lastAction, 0, g_iMonsterFrame[(*it)->type], 0);
									pMapObject->arg1 -= g_iMonsterDamage[(*it)->type];
									if ( pMapObject->argi2 == 0 )
									{
										pMapObject->argi2 = 1;
										CCMoveBy* pAction1 = CCMoveBy::create(0.05f, ccp(-2, 0));
										CCMoveBy* pAction2 = CCMoveBy::create(0.05f, ccp(4, 0));
										CCMoveBy* pAction3 = CCMoveBy::create(0.05f, ccp(-4, 0));
										pMapObject->pSprite->stopAllActions();
										pMapObject->pSprite->setPositionX(pMapObject->x);
										pMapObject->pSprite->runAction( CCSequence::create(pAction1, pAction2, pAction3, pAction2, pAction3, pAction2, pAction1, NULL));
									}
								}
								break;
							}
						}
					}
					//移动
					if ( !bWall )
					{
						(*it)->x += (*it)->toward*g_fMonsterSpeed[(*it)->type];
						(*it)->pSprite->setPositionX((*it)->x);
						if ( index != (*it)->lastAction )
						{
							(*it)->lastAction = index;
							if ( (*it)->secondLife )
								(*it)->pSprite->getAnimation()->playByIndex(g_iMonsterMoveSecond[(*it)->type], 0, g_iMonsterFrame[(*it)->type]);
							else
								(*it)->pSprite->getAnimation()->playByIndex(index, 0, g_iMonsterFrame[(*it)->type]);
						}
					}
				}
			}
			break;
		case 1://菜刀男
			if ( (*it)->x > m_fHeroLocateX )
				(*it)->toward = -1;
			else
				(*it)->toward = 1;
			if ( 6 == m_iLevelType )
				break;
			if ( fabs((*it)->x - m_fHeroLocateX) < g_fMonsterAttackDist[(*it)->type] /*&& !(*it)->bPlant*/ )
			{
				if ( (fabs((*it)->x-m_fHeroLocateX) > d_fHeroWidth+(*it)->w/2 
					|| fabs((*it)->y-m_fHeroLocateY) > d_fHeroHeight/2) )
				{
					if ( (*it)->attackTime == 0 && fabs((*it)->y-m_fHeroLocateY) < 300)
					{
						(*it)->attackTime = -1;
						//(*it)->attackTime = g_iMonsterAttackTime[(*it)->type];
						(*it)->pSprite->getAnimation()->playByIndex(1, 0, 50, 0);
						Chopper* pChopper = new Chopper;
						pChopper->pSprite = CCSprite::create("caidao.png");
						pChopper->type = (*it)->type;
						pChopper->damage = g_iMonsterDamage[(*it)->type];
						pChopper->x = (*it)->x - (*it)->toward*(*it)->w ;
						pChopper->y = (*it)->y + g_fMonsterHeight[(*it)->type]*1.2;
						pChopper->w = 40;
						pChopper->h = 5;
						pChopper->time = 14;
						pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
						pChopper->pSprite->setVisible(false);
						//pChopper->toward = (*it)->toward;
						m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
						m_vecChopperArray.push_back(pChopper);
					}
				}
				else
				{
					m_iUnderAttacked = 2;
					if ( (*it)->attackTime == 0 )
					{
						AppDelegate::AudioStopEffect((*it)->EffectID);
						(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/male/attack.mp3");
						(*it)->attackTime = -1;
						//(*it)->attackTime = g_iMonsterAttackTime[(*it)->type];
						(*it)->pSprite->getAnimation()->playByIndex(2, 0, 50, 0);
					}
					else if ( (*it)->attackTime == -12 )
					{
						if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2 
							&& fabs((*it)->x-m_fHeroLocateX) < d_fHeroWidth + (*it)->w/2 )
							BeAttack(g_iMonsterDamage[(*it)->type]);
					}
				}
			}
			break;
		case 6://鸟
			if ( fabs((*it)->x-m_fHeroLocateX) < 15.0f && (*it)->attackTime <= 0 )
			{
				if ( (*it)->attackTime == 0 )
				{
					AppDelegate::AudioStopEffect((*it)->EffectID);
					(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/hawk/attack.mp3");
					Chopper* pChopper = new Chopper;
					pChopper->pSprite = CCSprite::create("Animation/grenade/dan1.png");
					pChopper->type = (*it)->type;
					pChopper->damage = g_iMonsterDamage[(*it)->type];
					pChopper->x = (*it)->x;
					pChopper->y = (*it)->y;
					pChopper->w = 12;
					pChopper->h = 20;
					pChopper->time = 16;
					pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
					pChopper->pSprite->setVisible(false);
					m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
					m_vecChopperArray.push_back(pChopper);

					(*it)->lastAction = 1;
					(*it)->attackTime = -1;
					(*it)->pSprite->getAnimation()->playByIndex(1, 0, 50, 0);
				}
			}
			else if ( (*it)->attackTime >= 0 )
			{
				//if ( (*it)->attackTime > 0 )
				//	(*it)->attackTime--;
				if ( m_iLevelType == 2 )
				{
					if ( (*it)->x < - 50 || (*it)->x > 1650 )
						(*it)->toward = -(*it)->toward;
				}
				else
				{
					if ( fabs((*it)->x - (*it)->cx) > g_fMonsterAttackDist[(*it)->type] )
						(*it)->toward = -(*it)->toward;
				}
				(*it)->x += (*it)->toward*g_fMonsterSpeed[(*it)->type];
				(*it)->pSprite->setPositionX((*it)->x);
				if ( (*it)->lastAction > 0 )
				{
					(*it)->lastAction = 0;
					(*it)->pSprite->getAnimation()->playByIndex(0, 0, 50);
				}
			}
			break;
		case 10://机枪男
			if ( (*it)->x > m_fHeroLocateX )
				(*it)->toward = -1;
			else
				(*it)->toward = 1;
			(*it)->CoolTime1--;
			//if ( fabs((*it)->x-m_fHeroLocateX) < 10.0f && (*it)->CoolTime1 > 0 )
			//{
			//	(*it)->CoolTime1 = 0;
			//}
			if ( (*it)->CoolTime1 > 0 )
			{
				MonsterMove(*it, 1);
			}
			else if ( fabs((*it)->x - m_fHeroLocateX) > 600 )
				continue;
			else if ( (*it)->CoolTime1 > -100 ) 
			{
				if ( (*it)->CoolTime1 == 0 )
				{
					(*it)->lastAction = 0;
					(*it)->pSprite->getAnimation()->playByIndex(0, 0, 60, 1);
				}
			}
			else if ( (*it)->CoolTime1 > -160 )
			{
				if ( (*it)->CoolTime1 == -100 )
				{
					(*it)->lastAction = 2;
					(*it)->pSprite->getAnimation()->playByIndex(2, 0, 60);
				}
				else if ( (*it)->CoolTime1 == -108 )
				{
					//(*it)->pSprite->getAnimation()->getMovementCount()
					Chopper* pChopper = new Chopper;
					pChopper->pSprite = CCSprite::create("bullet/5/3.png");
					pChopper->pSprite->setAnchorPoint(ccp(0.75f,0.5f));
					pChopper->type = (*it)->type;
					pChopper->damage = g_iMonsterDamage[(*it)->type];
					pChopper->rotate = 0;
					pChopper->x = (*it)->x + (*it)->toward*54 ;
					pChopper->y = (*it)->y + 68;
					pChopper->w = 10;
					pChopper->h = 10;
					pChopper->vx= 5*(*it)->toward;
					pChopper->vy= 0;
					pChopper->time = 0;
					pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
					pChopper->toward = (*it)->toward;
					pChopper->pSprite->setScaleX(pChopper->toward);
					m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
					m_vecChopperArray.push_back(pChopper);
				}
				else if ( (*it)->CoolTime1 == -120 )
				{
					Chopper* pChopper = new Chopper;
					pChopper->pSprite = CCSprite::create("bullet/5/3.png");
					pChopper->pSprite->setAnchorPoint(ccp(0.75f,0.5f));
					pChopper->type = (*it)->type;
					pChopper->damage = g_iMonsterDamage[(*it)->type];
					pChopper->rotate = -15;
					pChopper->x = (*it)->x + (*it)->toward*34 ;
					pChopper->y = (*it)->y + 56;
					pChopper->w = 10;
					pChopper->h = 10;
					pChopper->vx= 5*(*it)->toward;
					pChopper->vx*=cos(pChopper->rotate*PI_FIX);
					pChopper->vy= 5*sin(pChopper->rotate*PI_FIX);
					pChopper->time = 0;
					pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
					pChopper->toward = (*it)->toward;
					pChopper->pSprite->setScaleX(pChopper->toward);
					pChopper->pSprite->setRotation( 15*(*it)->toward );
					m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
					m_vecChopperArray.push_back(pChopper);
				}
				else if ( (*it)->CoolTime1 == -132 )
				{
					Chopper* pChopper = new Chopper;
					pChopper->pSprite = CCSprite::create("bullet/5/3.png");
					pChopper->pSprite->setAnchorPoint(ccp(0.75f,0.5f));
					pChopper->type = (*it)->type;
					pChopper->damage = g_iMonsterDamage[(*it)->type];
					pChopper->rotate = 0;
					pChopper->x = (*it)->x + (*it)->toward*54 ;
					pChopper->y = (*it)->y + 68;
					pChopper->w = 10;
					pChopper->h = 10;
					pChopper->vx= 5*(*it)->toward;
					pChopper->vy= 0;
					pChopper->time = 0;
					pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
					pChopper->toward = (*it)->toward;
					pChopper->pSprite->setScaleX(pChopper->toward);
					m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
					m_vecChopperArray.push_back(pChopper);
				}
				else if ( (*it)->CoolTime1 == -144 )
				{
					Chopper* pChopper = new Chopper;
					pChopper->pSprite = CCSprite::create("bullet/5/3.png");
					pChopper->pSprite->setAnchorPoint(ccp(0.75f,0.5f));
					pChopper->type = (*it)->type;
					pChopper->damage = g_iMonsterDamage[(*it)->type];
					pChopper->rotate = 30;
					pChopper->x = (*it)->x + (*it)->toward*10 ;
					pChopper->y = (*it)->y + 87;
					pChopper->w = 10;
					pChopper->h = 10;
					pChopper->vx= 5*(*it)->toward;
					pChopper->vx*=cos(pChopper->rotate*PI_FIX);
					pChopper->vy= 5*sin(pChopper->rotate*PI_FIX);
					pChopper->time = 0;
					pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
					pChopper->toward = (*it)->toward;
					pChopper->pSprite->setScaleX(pChopper->toward);
					pChopper->pSprite->setRotation( -30*(*it)->toward );
					m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
					m_vecChopperArray.push_back(pChopper);
				}
			}
			else
				(*it)->CoolTime1 = 150;
			break;
		case 15://精英怪
			{
				if ( !(*it)->getback && fabs((*it)->x - m_fHeroLocateX)<g_fMonsterAttackDist[(*it)->type] /*&& !(*it)->bPlant*/ )
				{
					if ( (*it)->x > m_fHeroLocateX )
						(*it)->toward = -1;
					else
						(*it)->toward = 1;
					if ( (*it)->attackTime > 0 && fabs((*it)->x-m_fHeroLocateX) > 70.0f )
					{
						MonsterMove(*it, 0);
					}
					else if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight 
						&& fabs((*it)->x-m_fHeroLocateX) < 180 )
					{
						m_iUnderAttacked = 2;
						if ( (*it)->attackTime == 0 )
						{
							AppDelegate::AudioStopEffect((*it)->EffectID);
							(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/male/attack.mp3");

							(*it)->lastAction = 2;
							(*it)->attackTime = -1;
							(*it)->pSprite->getAnimation()->playByIndex(2, 0, 50, 0);
						}
						else if ( (*it)->attackTime == -12 )
						{
							if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2 
								&& fabs((*it)->x-m_fHeroLocateX) < 250 )
								BeAttack(g_iMonsterDamage[(*it)->type]);
						}
					}
					else if ( (*it)->attackTime >= 0 && fabs((*it)->x-m_fHeroLocateX) > 70.0f )
					{
						MonsterMove(*it, 0);
					}
				}
				else
				{
					if ( (*it)->x > m_fHeroLocateX )
						(*it)->toward = -1;
					else
						(*it)->toward = 1;
					MonsterMove(*it, 0, true);
					if ( fabs((*it)->y-m_fHeroLocateY) < 20.0f )
						(*it)->getback = 0;
				}
			}
			break;
		case 16://飞机
			{
				MonsterInfomation* pMonster = *it;
				bool action = false;
				if ( pMonster->x > m_fHeroLocateX )
					pMonster->toward = -1;
				else
					pMonster->toward = 1;

				if ( pMonster->hp > 160 )
				{
					if ( fabs(pMonster->x - m_fHeroLocateX) > 200+pMonster->CoolTime7 && pMonster->CoolTime3 > 0 )
					{
						action = true;
						pMonster->x += pMonster->toward * g_fMonsterSpeed[pMonster->type];
						if ( pMonster->lastAction != 1 )
						{
							pMonster->lastAction = 1;
							pMonster->pSprite->getAnimation()->playByIndex(1, 0, 40);
						}
					}
				}

				if ( pMonster->CoolTime1 > 0 )
				{
					pMonster->y += 2;
					pMonster->CoolTime1 -= 2;
					if ( pMonster->CoolTime1 < 0 )
						pMonster->CoolTime1 = 0;
					pMonster->pSprite->setScaleX(pMonster->toward);
				}
				else
				{
					if ( pMonster->hp > 160 )
					{
						if ( pMonster->y > pMonster->CoolTime2 )
						{
							pMonster->y -= 1;
							if ( pMonster->y <= pMonster->CoolTime2 )
								pMonster->y = pMonster->CoolTime2;
						}
						else if ( pMonster->y < pMonster->CoolTime2 )
						{
							pMonster->y += 1;
							if ( pMonster->y >= pMonster->CoolTime2 )
								pMonster->y = pMonster->CoolTime2;
						}
						else
						{
							pMonster->CoolTime2 = 250 +rand()%50;
						}
						if ( pMonster->CoolTime3 > 0 )
							pMonster->CoolTime3 --;
						if ( pMonster->CoolTime3 <= 0 )
						{
							action = true;
							if ( pMonster->CoolTime3 == 0 )
							{
								pMonster->lastAction = 2;
								pMonster->pSprite->getAnimation()->playByIndex(2, 0, 40);
							}
							else if ( pMonster->CoolTime3 == -10 )
							{
								Chopper* pChopper = new Chopper;
								pChopper->pSprite = CCSprite::create("bullet/5/1.png");
								pChopper->type = 14;
								pChopper->damage = 5;
								pChopper->bomb	 = true;
								pChopper->rotate = 0;
								pChopper->active = true;
								pChopper->x = pMonster->x;
								pChopper->y = pMonster->y;
								float k = ( pMonster->y - m_fHeroLocateY - 40 )/( pMonster->x - m_fHeroLocateX );
								float x = sqrtf(36/(1+k*k));
								float y = -fabs(k*x);
								if ( pMonster->y < m_fHeroLocateY )
									y = -y;
								pChopper->pSprite->setRotation(atanf(y/x)*pMonster->toward);
								pChopper->pSprite->setScaleX(pMonster->toward);
								pChopper->vx= x*pMonster->toward;
								pChopper->vy= y;
								pChopper->w = 10;
								pChopper->h = 10;
								pChopper->va= 0;
								pChopper->time = 0;
								pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
								m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
								m_vecChopperArray.push_back(pChopper);
							}
							else if ( pMonster->CoolTime3 == -30 )
							{
								pMonster->CoolTime3 = 180 +rand()%200;
							}
							pMonster->CoolTime3 --;
						}
						pMonster->pSprite->setScaleX(pMonster->toward);
					}
					else
					{
						action = true;
						if ( pMonster->CoolTime6 > 0 )
						{
							pMonster->CoolTime6--;
							if ( pMonster->CoolTime6 == 49 )
							{
								pMonster->pSprite->getAnimation()->playByIndex(3, 0, 30);
							}
							else if ( pMonster->CoolTime6 == 0 )
							{
								pMonster->pSprite->getAnimation()->playByIndex(4, 0, 30);
								pMonster->CoolTime4 = m_fHeroLocateX;
								pMonster->CoolTime5 = m_fHeroLocateY;

								float k = ( pMonster->y - m_fHeroLocateY - 40 )/( pMonster->x - m_fHeroLocateX );
								float x = sqrtf(100/(1+k*k));
								float y = fabs(k*x);
								if ( pMonster->y > m_fHeroLocateY )
									y = -y;
								pMonster->pSprite->setRotation(atanf(y/x)*pMonster->toward);
								pMonster->SpeedX= x*pMonster->toward;
								pMonster->SpeedY= y;
								pMonster->pSprite->setScaleX(pMonster->toward);
							}
						}
						else
						{
							if ( (fabs(pMonster->x-m_fHeroLocateX) < 70 && fabs(pMonster->y-m_fHeroLocateY) < 50)
								|| (fabs(pMonster->x-pMonster->CoolTime4) < 40 && fabs(pMonster->y-pMonster->CoolTime5) < 30) )
							{
								MonsterBeAttack(pMonster, 200, 1);
							}
							pMonster->x += pMonster->SpeedX;
							pMonster->y += pMonster->SpeedY;
							if ( !SetVisible(pMonster->pSprite, pMonster->x, pMonster->y) )
							{
								MonsterBeAttack(pMonster, 200, 1);
							}
						}
					}
				}

				if ( !action )
				{
					if ( pMonster->lastAction != 0 )
					{
						pMonster->lastAction = 0;
						pMonster->pSprite->getAnimation()->playByIndex(0, 0, 30);
					}
				}

				pMonster->pSprite->setPosition(ccp(pMonster->x, pMonster->y));
			}
			break;
		case 5://BOSS1
			if ( (*it)->CoolTime1 > 0 )
				(*it)->CoolTime1--;
			if ( (*it)->CoolTime3 > 0 )
				(*it)->CoolTime3--;
			if ( (*it)->CoolTime3 <= 0 && (*it)->CoolTime1 >=0 )
			{
				(*it)->CoolTime3--;
				if ( (*it)->CoolTime3 == -1 )
				{
					(*it)->lastAction = 1;
					(*it)->pSprite->getAnimation()->playByIndex(1, 0, 30, 0);
				}
				else if ( (*it)->CoolTime3 == -10 )
				{
					if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight
						&& fabs((*it)->x-m_fHeroLocateX-(*it)->w/2*(*it)->toward) < d_fHeroWidth )
						BeAttack(g_iMonsterDamage[(*it)->type]);
					m_iShakeTime = 40;
					//CCMoveBy* pAction1 = CCMoveBy::create(0.03f, ccp(0.02f, 10.0f));
					//CCMoveBy* pAction2 = CCMoveBy::create(0.03f, ccp(0.02f, -10.0f));
					////m_pScene->setPositionY(m_pCFrontgroud->GetTileMap()->getPositionY()-5.0f);
					//m_pScene->runAction( CCRepeatForever::create(CCSequence::create(pAction1, pAction2, NULL)));
					CCSprite* pSprite = common::CreateAnimation("Animation/boss1skill2/1.png", ccp((*it)->x+(*it)->w/2*(*it)->toward, (*it)->y), ccp(0.5f, 0.2f),
						"Animation/boss1skill2/", 6, CCRect(0,0,220,181), false, false, 0.1f, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
					m_pCFrontgroud->GetTileMap()->addChild(pSprite,3);
				}
				else if ( (*it)->CoolTime3 == -20 )
					m_bCreateMonster = 5;
				else if ( (*it)->CoolTime3 == -45 )
				{
					(*it)->CoolTime3 = 5*(*it)->CoolTime2;
					//m_pScene->stopAllActions();
					//m_pScene->setPositionY(0.0f);
				}
			}
			else if ( (fabs((*it)->x-m_fHeroLocateX) > 100.0f && (*it)->attackTime >= 0) ||
				( !(fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2 && fabs((*it)->x-m_fHeroLocateX) < d_fHeroWidth) && (*it)->CoolTime1 <= 0) )
			{
				if ( (*it)->CoolTime1 <= 0 )
				{
					(*it)->CoolTime1 --;
					int t = 0;
					if ( (*it)->x > m_fHeroLocateX )
						t = -1;
					else if ( (*it)->x < m_fHeroLocateX )
						t = 1;
					(*it)->bHold = false;
					if ( (*it)->CoolTime1 == -1 )
						(*it)->pSprite->getAnimation()->playByIndex(3, 0, 30, 0);
					else if ( (*it)->CoolTime1 == -20 )
						(*it)->pSprite->getAnimation()->playByIndex(4, 0, 30, 1);
					if ( fabs((*it)->x - m_fHeroLocateX) > 10.0f )
						MonsterMoveCommon(*it, false, 7.0f);
					if ( (*it)->CoolTime1 < -20 && (*it)->CoolTime1 > -25 )
					{
						if ( t == -(*it)->toward )
							(*it)->CoolTime1 = -26;
						else
							(*it)->CoolTime1 = -23;
					}
					if ( (*it)->CoolTime1 < -85 )
					{
						(*it)->CoolTime1 = (*it)->CoolTime2;
						(*it)->attackTime = -20;
						(*it)->lastAction = 5;
						(*it)->pSprite->getAnimation()->playByIndex(0, 0, 30);
					}
				}
				else if ( fabs((*it)->x-m_fHeroLocateX) > 10.0f )
				{
					if ( (*it)->x > m_fHeroLocateX )
						(*it)->toward = -1;
					else
						(*it)->toward = 1;
					(*it)->bHold = true;
					//MonsterMoveCommon(*it);
					MonsterMove(*it);
				}
			}
			else
			{
				if ( (*it)->CoolTime1 >= 0 )
				{
					if ( (*it)->x > m_fHeroLocateX )
						(*it)->toward = -1;
					else
						(*it)->toward = 1;
					if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2 
						&& fabs((*it)->x-m_fHeroLocateX) < d_fHeroWidth )
					{
						m_iUnderAttacked = 2;
						if ( (*it)->attackTime == 0 )
						{
							AppDelegate::AudioStopEffect((*it)->EffectID);
							(*it)->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/boss1/attack.mp3");
							(*it)->attackTime = -1;
							(*it)->lastAction = 1;
							(*it)->pSprite->getAnimation()->playByIndex(2, 0, 50, 0);
						}
						else if ( (*it)->attackTime == -30 )
						{
							if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2 
								&& fabs((*it)->x-m_fHeroLocateX) < (*it)->w + d_fHeroWidth )
								BeAttack(g_iMonsterDamage[(*it)->type]);
						}
					}
					else if ( (*it)->attackTime >= 0 )
					{
						(*it)->bHold = true;
						//MonsterMoveCommon(*it);
						MonsterMove(*it);
					}
				}
				else
				{
					if ( fabs((*it)->y-m_fHeroLocateY) < d_fHeroHeight/2 
						&& fabs((*it)->x-m_fHeroLocateX) < d_fHeroWidth )
					{
						m_iUnderAttacked = 2;
						(*it)->CoolTime1 = (*it)->CoolTime2;
						(*it)->attackTime = -20;
						(*it)->lastAction = 5;
						(*it)->pSprite->getAnimation()->playByIndex(5, 0, 30, 0);
						BeAttack(20);
						m_iHoldTime	= 50;
						m_pHero->RunAction(14-m_ewtNowWeapon, 30);
						if ( (*it)->x > m_fHeroLocateX )
							m_fHeroToward = 1;
						else
							m_fHeroToward = -1;
						m_fHeroSpeedY = 4.0f;
					}
					else
					{
						if ( (*it)->attackTime >= 0 )
						{
							(*it)->bHold = false;
							if ( fabs((*it)->x - m_fHeroLocateX) > 10.0f )
								MonsterMoveCommon(*it, false, 3.5f);
						}
					}
				}
			}
			if ( (*it)->x < m_fHeroLocateX-m_fRelativeX + 50.0f )
				(*it)->x = m_fHeroLocateX-m_fRelativeX + 50.0f;
			else if ( (*it)->x > m_fHeroLocateX-m_fRelativeX + S_WINSIZE_W - 50.0f )
				(*it)->x = m_fHeroLocateX-m_fRelativeX + S_WINSIZE_W - 50.0f;
			break;
		case 8://BOSS2
			if ( (*it)->x > m_fHeroLocateX )
				(*it)->toward = -1;
			else 
				(*it)->toward = 1;
			if ( (*it)->bSprite1 )
			{
				(*it)->pSprite1->setScaleX((*it)->toward);
				(*it)->pSprite1->setPosition((*it)->x + (*it)->toward*d_locationBoss2Head1 + (*it)->y);
			}
			if ( (*it)->bSprite2 )
			{
				(*it)->pSprite2->setScaleX((*it)->toward);
				(*it)->pSprite2->setPosition((*it)->x + (*it)->toward*d_locationBoss2Head2 + (*it)->y);
			}
			if ( (*it)->bSprite3 )
			{
				(*it)->pSprite3->setScaleX((*it)->toward);
				(*it)->pSprite3->setPosition((*it)->x + (*it)->toward*d_locationBoss2Head3 + (*it)->y);
			}

			if ( (*it)->ActionType == 0 )
			{
				if ( (*it)->bSprite1 )
				{
					(*it)->CoolTime1 --;
					(*it)->CoolTime4 --;
				}
				if ( (*it)->bSprite2 )
				{
					(*it)->CoolTime2 --;
					(*it)->CoolTime5 --;
				}
				if ( (*it)->bSprite3 )
				{
					(*it)->CoolTime3 --;
					(*it)->CoolTime6 --;
				}
			}
			if ( (*it)->CoolTime1 <= 0 )
			{//技能2-1
				if ( (*it)->CoolTime1 == 0 )
				{
					if ( fabs(m_fHeroLocateX - (*it)->x) < 150 )
					{
						(*it)->ActionType = 1;
						(*it)->pSprite1->getAnimation()->playByIndex(1, 0, 30);
					}
					else
					{
						(*it)->ActionType = 2;
						(*it)->pSprite1->getAnimation()->playByIndex(4, 0, 30);
					}
				}
				else if ( (*it)->CoolTime1 == -18 )
				{
					if ( (*it)->ActionType == 1 )
					{
						if ( fabs(m_fHeroLocateX - (*it)->x) < 150 && fabs(m_fHeroLocateY - (*it)->y) < 120 )
						{
							BeAttack(15);
						}
					}
					else if ( (*it)->ActionType == 2 )
					{
						Chopper* pChopper = new Chopper;
						pChopper->pSprite = common::CreateAnimation("Animation/boss2skill1/a1.png", ccp(0,0), ccp(0.5f,0.5f),
							"Animation/boss2skill1/a", 4, CCRect(0,0,68,61), true);
						pChopper->type = (*it)->type;
						pChopper->damage = 20;
						pChopper->rotate = 0;
						pChopper->active = true;
						pChopper->x = m_fHeroLocateX;
						pChopper->y = 500;
						pChopper->w = 30;
						pChopper->h = 30;
						pChopper->vx= 0;
						pChopper->vy= 0;
						pChopper->va= d_fHeroGravity;
						pChopper->time = 0;
						pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
						m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
						m_vecChopperArray.push_back(pChopper);
					}
				}
				(*it)->CoolTime1--;
				if ( (*it)->CoolTime1 < -40 )
				{
					(*it)->ActionType = 0;
					if ( (*it)->pSprite1 )
						(*it)->pSprite1->getAnimation()->playByIndex(0, 0, 60);
					(*it)->CoolTime1 = 70+rand()%30;
				}
			}
			else if ( (*it)->CoolTime2 <= 0 )
			{//技能2-2
				if ( (*it)->CoolTime2 == 0 )
				{
					if ( fabs(m_fHeroLocateX - (*it)->x) < 150 )
					{
						(*it)->ActionType = 1;
						(*it)->pSprite2->getAnimation()->playByIndex(1, 0, 30);
					}
					else
					{
						(*it)->ActionType = 2;
						(*it)->pSprite2->getAnimation()->playByIndex(4, 0, 30);
					}
				}
				else if ( (*it)->CoolTime2 == -18 )
				{
					if ( (*it)->ActionType == 1 )
					{
						if ( fabs(m_fHeroLocateX - (*it)->x) < 150 && fabs(m_fHeroLocateY - (*it)->y) < 120 )
						{
							BeAttack(15);
						}
					}
					else if ( (*it)->ActionType == 2 )
					{
						Chopper* pChopper = new Chopper;
						pChopper->pSprite = common::CreateAnimation("Animation/boss2skill1/a1.png", ccp(0,0), ccp(0.5f,0.5f),
							"Animation/boss2skill1/a", 4, CCRect(0,0,68,61), true);
						pChopper->type = (*it)->type;
						pChopper->damage = 20;
						pChopper->rotate = 0;
						pChopper->active = true;
						pChopper->x = m_fHeroLocateX;
						pChopper->y = 500;
						pChopper->w = 30;
						pChopper->h = 30;
						pChopper->vx= 0;
						pChopper->vy= 0;
						pChopper->va= d_fHeroGravity;
						pChopper->time = 0;
						pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
						m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
						m_vecChopperArray.push_back(pChopper);
					}
				}
				(*it)->CoolTime2--;
				if ( (*it)->CoolTime2 < -40 )
				{
					(*it)->ActionType = 0;
					if ( (*it)->pSprite2 )
						(*it)->pSprite2->getAnimation()->playByIndex(0, 0, 60);
					(*it)->CoolTime2 = 70+rand()%50;
				}
			}
			else if ( (*it)->CoolTime3 <= 0 )
			{//技能2-3
				if ( (*it)->CoolTime3 == 0 )
				{
					if ( fabs(m_fHeroLocateX - (*it)->x) < 150 )
					{
						(*it)->ActionType = 1;
						(*it)->pSprite3->getAnimation()->playByIndex(1, 0, 30);
					}
					else
					{
						(*it)->ActionType = 2;
						(*it)->pSprite3->getAnimation()->playByIndex(4, 0, 30);
					}
				}
				else if ( (*it)->CoolTime3 == -18 )
				{
					if ( (*it)->ActionType == 1 )
					{
						if ( fabs(m_fHeroLocateX - (*it)->x) < 150 && fabs(m_fHeroLocateY - (*it)->y) < 120 )
						{
							BeAttack(15);
						}
					}
					else if ( (*it)->ActionType == 2 )
					{
						Chopper* pChopper = new Chopper;
						pChopper->pSprite = common::CreateAnimation("Animation/boss2skill1/a1.png", ccp(0,0), ccp(0.5f,0.5f),
							"Animation/boss2skill1/a", 4, CCRect(0,0,68,61), true);
						pChopper->type = (*it)->type;
						pChopper->damage = 20;
						pChopper->rotate = 0;
						pChopper->active = true;
						pChopper->x = m_fHeroLocateX;
						pChopper->y = 500;
						pChopper->w = 30;
						pChopper->h = 30;
						pChopper->vx= 0;
						pChopper->vy= 0;
						pChopper->va= d_fHeroGravity;
						pChopper->time = 0;
						pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
						m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
						m_vecChopperArray.push_back(pChopper);
					}
				}
				(*it)->CoolTime3--;
				if ( (*it)->CoolTime3 < -40 )
				{
					(*it)->ActionType = 0;
					if ( (*it)->pSprite3 )
						(*it)->pSprite3->getAnimation()->playByIndex(0, 0, 60);
					(*it)->CoolTime3 = 80+rand()%60;
				}
			}
			else if ( (*it)->CoolTime4 <= 0 )
			{//技能2-4
				if ( (*it)->CoolTime4 == 0 )
				{
					(*it)->ActionType = 1;
					(*it)->pSprite1->getAnimation()->playByIndex(2, 0, 60);
				}
				else if ( (*it)->CoolTime4 == -20 )
				{
					(*it)->pSprite1->getAnimation()->playByIndex(3, 0, 60);
					m_pSpriteBuffer1 = common::CreateAnimation("Animation/boss2skill1/1/1.png", ccp((*it)->x-470, (*it)->y+53),
						ccp(0.06f, 0.21f), "Animation/boss2skill1/1/", 7, CCRect(0,0,500,249), true);
					m_pSpriteBuffer1->setScaleY(1.50f);
					m_pSpriteBuffer1->setFlipX(true);
					m_pCFrontgroud->GetTileMap()->addChild(m_pSpriteBuffer1, 3);
				}
				else if ( (*it)->CoolTime4 < -20  && (*it)->ActionType == 1 )
				{
					if ( (*it)->x - m_fHeroLocateX > 140 && (*it)->x - m_fHeroLocateX < 410 )
					{//被冰喷到
						(*it)->ActionType = 2;
						m_iFrozeTime = 450;
					}
				}
				(*it)->CoolTime4--;
				if ( (*it)->CoolTime4 < -80 )
				{
					(*it)->ActionType = 0;
					if ( m_pSpriteBuffer1 )
					{
						m_pSpriteBuffer1->removeFromParentAndCleanup(true);
						m_pSpriteBuffer1 = NULL;
					}
					if ( (*it)->pSprite1 )
						(*it)->pSprite1->getAnimation()->playByIndex(0, 0, 60);
					(*it)->CoolTime4 = 300+rand()%50;
				}
			}
			else if ( (*it)->CoolTime5 <= 0 )
			{//技能2-5
				if ( (*it)->CoolTime5 == 0 )
				{
					(*it)->ActionType = 1;
					(*it)->pSprite2->getAnimation()->playByIndex(2, 0, 60);
				}
				else if ( (*it)->CoolTime5 == -20 )
				{
					(*it)->pSprite2->getAnimation()->playByIndex(3, 0, 60);
					m_pSpriteBuffer2 = common::CreateAnimation("Animation/boss2skill1/2/1.png", ccp((*it)->x-421, (*it)->y+60),
						ccp(0.06f, 0.21f), "Animation/boss2skill1/2/", 7, CCRect(0,0,500,249), true);
					m_pSpriteBuffer2->setScaleX(0.85f);
					m_pSpriteBuffer2->setScaleY(1.50f);
					m_pSpriteBuffer2->setFlipX(true);
					m_pCFrontgroud->GetTileMap()->addChild(m_pSpriteBuffer2, 3);
				}
				else if ( (*it)->CoolTime5 < -20 && (*it)->ActionType == 1 )
				{
					if ( (*it)->x - m_fHeroLocateX > 140 && (*it)->x - m_fHeroLocateX < 410 )
					{//被毒喷到
						(*it)->ActionType = 2;
						m_iPrisonTime = 250;
					}
				}
				(*it)->CoolTime5--;
				if ( (*it)->CoolTime5 < -80 )
				{
					(*it)->ActionType = 0;
					if ( m_pSpriteBuffer2 )
					{
						m_pSpriteBuffer2->removeFromParentAndCleanup(true);
						m_pSpriteBuffer2 = NULL;
					}
					if ( (*it)->pSprite2 )
						(*it)->pSprite2->getAnimation()->playByIndex(0, 0, 60);
					(*it)->CoolTime5 = 350+rand()%50;
				}
			}
			else if ( (*it)->CoolTime6 <= 0 )
			{//技能2-6
				if ( (*it)->CoolTime6 == 0 )
				{
					(*it)->ActionType = 1;
					(*it)->pSprite3->getAnimation()->playByIndex(2, 0, 60);
				}
				else if ( (*it)->CoolTime6 == -20 )
				{
					(*it)->pSprite3->getAnimation()->playByIndex(3, 0, 60);
					m_pSpriteBuffer3 = common::CreateAnimation("Animation/fire/1.png", ccp((*it)->x-485, (*it)->y+93),
						ccp(0.0f, 0.14f), "Animation/fire/", 7, CCRect(0,0,313,133), true);
					m_pSpriteBuffer3->setScaleX(1.20f);
					m_pSpriteBuffer3->setScaleY(2.00f);
					m_pSpriteBuffer3->setFlipX(true);
					m_pCFrontgroud->GetTileMap()->addChild(m_pSpriteBuffer3, 3);
				}
				else if ( (*it)->CoolTime5 < -20 && (*it)->ActionType == 1 )
				{
					if ( (*it)->x - m_fHeroLocateX > 140 && (*it)->x - m_fHeroLocateX < 410 )
					{//被火喷到
						(*it)->ActionType = 2;
						BeAttack(20);
					}
				}
				(*it)->CoolTime6--;
				if ( (*it)->CoolTime6 < -80 )
				{
					(*it)->ActionType = 0;
					if ( m_pSpriteBuffer3 )
					{
						m_pSpriteBuffer3->removeFromParentAndCleanup(true);
						m_pSpriteBuffer3 = NULL;
					}
					if ( (*it)->pSprite3 )
						(*it)->pSprite3->getAnimation()->playByIndex(0, 0, 60);
					(*it)->CoolTime6 = 400+rand()%50;
				}
			}
			break;
		case 12://BOSS3
			if ( (*it)->x > m_fHeroLocateX )
				(*it)->toward = -1;
			else 
				(*it)->toward = 1;
			(*it)->CoolTime2 --;
			if ( (*it)->CoolTime2 <= 0 )
			{
				if ( (*it)->mode == 0 )
				{//人
					if ( (*it)->CoolTime2 == 0 )
					{
						if ( (*it)->CoolTime1 < 0 )
						{
							(*it)->CoolTime3 = 2;
							(*it)->canAttack = false;
							(*it)->pSprite->getAnimation()->playByIndex(12, 0, 30);
						}
						else if ( (*it)->CoolTime4 > 0 && (*it)->CoolTime3 == 0)
						{
							(*it)->canAttack = true;
							(*it)->CoolTime4--;
							(*it)->pSprite->getAnimation()->playByIndex(5, 0, 30);
						}
						else if ( (*it)->CoolTime3 == 0 )
						{
							(*it)->CoolTime3 = 1;
							(*it)->canAttack = true;
							(*it)->pSprite->getAnimation()->playByIndex(6, 0, 30);
						}
						else if ( (*it)->CoolTime3 == 1 )
						{
							(*it)->CoolTime2 = 1;
							(*it)->CoolTime3 = 0;
							(*it)->CoolTime4 = rand()%3+3;
							//(*it)->canAttack = false;
							//(*it)->pSprite->getAnimation()->playByIndex(4, 0, 30, 1);
						}
					}
					else switch ((*it)->CoolTime3)
					{
					case 0://光球
						if ( (*it)->CoolTime2 == -20 )
						{//创建球
							Chopper* pChopper = new Chopper;
							pChopper->pSprite = common::CreateAnimation("Animation/boss3skill1/1.png", ccp(0,0), ccp(0.5f,0.5f),
								"Animation/boss3skill1/", 4, CCRect(0,0,102,91));
							pChopper->type = (*it)->type;
							pChopper->active = false;
							pChopper->damage = g_iMonsterDamage[(*it)->type];
							pChopper->time = 5;
							pChopper->x = (*it)->x+40*(*it)->toward;
							pChopper->y = (*it)->y+146;
							pChopper->w = 40;
							pChopper->h = 40;
							pChopper->va= 0;
							pChopper->pSprite->setVisible(false);
							pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
							m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
							m_vecChopperArray.push_back(pChopper);
						}
						else if ( (*it)->CoolTime2 == -70 )
						{
							if ( (*it)->CoolTime4 == 0 )
							{
								(*it)->CoolTime2 = 100;
								(*it)->canAttack = false;
								(*it)->pSprite->getAnimation()->playByIndex(4, 0, 30, 1);
							}
							else
								(*it)->CoolTime2 = 1;
						}
						break;
					case 1://地上手
						if ( (*it)->CoolTime2 == - 20 )
						{
							(*it)->CoolTime4 = rand()%3+3;
							(*it)->pSprite->getAnimation()->playByIndex(7, 0, 30, 1);
						}
						else if ( (*it)->CoolTime2 < -20 )
						{
							if ( (*it)->CoolTime4 > 0 && ((*it)->CoolTime2+20)%40 == 0 )
							{
								(*it)->CoolTime4--;
								Chopper* pChopper = new Chopper;
								pChopper->pSprite = common::CreateAnimation("Animation/boss3skill2/1.png", ccp(0,0), ccp(0.5f,0.2f),
									"Animation/boss3skill2/", 13, CCRect(0,0,150,161));
								pChopper->type = (*it)->type+1;
								pChopper->damage = g_iMonsterDamage[(*it)->type];
								pChopper->bomb	 = true;
								pChopper->rotate = 0;
								pChopper->active = true;
								pChopper->x = m_fHeroLocateX;
								pChopper->y = (*it)->CoolTime6;
								pChopper->w = 90;
								pChopper->h = 100;
								pChopper->vx= 0;
								pChopper->vy= 0;
								pChopper->va= 0;
								pChopper->time = 0;
								pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
								m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
								m_vecChopperArray.push_back(pChopper);
							}
							else if ( (*it)->CoolTime4 == 0 )
							{
								(*it)->CoolTime2 = 100;
								(*it)->canAttack = false;
								(*it)->pSprite->getAnimation()->playByIndex(4, 0, 30, 1);
							}
						}
						break;
					case 2://变身蜘蛛
						if ( (*it)->CoolTime2 == -25 )
						{
							(*it)->pSprite->getAnimation()->playByIndex(10, 0, 30, 1);
						}
						if ( (*it)->CoolTime2 > -100 && (*it)->CoolTime2 < -25 && (*it)->y < (*it)->CoolTime6 + 480 )
						{
							(*it)->y += 10;
							if ( (*it)->y >= (*it)->CoolTime6 + 480 || (*it)->CoolTime2 == -99 )
							{
								(*it)->pSprite->getAnimation()->playByIndex(0, 0, 30, 1);
							}
						}
						else if ( (*it)->CoolTime2 < -100 )
						{
							if ( (*it)->y > (*it)->CoolTime6 )
								(*it)->y -= 10;
							else
							{//人初始化
								(*it)->y = (*it)->CoolTime6;
								(*it)->mode = 1;
								(*it)->CoolTime2 = 100;
								(*it)->CoolTime4 = 2;
							}
						}
						break;
					}
				}
				else
				{//蜘蛛
					if ( (*it)->CoolTime2 == 0 )
					{
						if ( (*it)->CoolTime4 > 0 )
						{
							(*it)->CoolTime1 = 0;
							(*it)->CoolTime3 = 0;
							(*it)->CoolTime4 --;
							(*it)->pSprite->getAnimation()->playByIndex(1, 0, 30);
						}
						else
						{
							(*it)->CoolTime3 = 1;
							(*it)->pSprite->getAnimation()->playByIndex(11, 0, 30);
						}
					}
					else switch ( (*it)->CoolTime3 )
					{
					case 0://砸
						if ( (*it)->CoolTime2 == -70 )
						{
							(*it)->x = m_fHeroLocateX;
						}
						else if ( (*it)->CoolTime2 < -70 )
						{
							if ( (*it)->y > (*it)->CoolTime6 )
								(*it)->y -= 10;
							else
							{
								(*it)->CoolTime2 = 50;
								(*it)->y = (*it)->CoolTime6;
								//(*it)->pSprite->getAnimation()->playByIndex(0, 0, 30, 1);
							}
							if ( (*it)->y < (*it)->CoolTime6 + 100 && (*it)->CoolTime1 == 0 )
							{
								(*it)->CoolTime1 = 1;
								(*it)->pSprite->getAnimation()->playByIndex(3, 0, 30);
							}
							if ( !(*it)->bRock )
							{
								if ( fabs((*it)->x - m_fHeroLocateX) < 100 &&
									fabs((*it)->y - m_fHeroLocateY) < (*it)->h/2 + d_fHeroHeight/2 )
								{
									(*it)->bRock = true;
									BeAttack(g_iMonsterDamage[(*it)->type]);
								}
							}
						}
						else if ( (*it)->CoolTime2 == -20 )
						{
							(*it)->y += 10;
							(*it)->CoolTime1 = 0;
							(*it)->bRock = false;
							(*it)->pSprite->getAnimation()->playByIndex(2, 0, 30, 1);
						}
						else if ( (*it)->CoolTime2 < -20 && (*it)->y < (*it)->CoolTime6 + 600 )
						{
							(*it)->y += 10;
						}
						break;
					case 1:
						//变身人
						if ( (*it)->CoolTime2 == -25 )
						{
							(*it)->pSprite->getAnimation()->playByIndex(10, 0, 30, 1);
						}
						if ( (*it)->CoolTime2 > -100 && (*it)->CoolTime2 < -25 && (*it)->y < (*it)->CoolTime6 + 480 )
						{
							(*it)->y += 10;
							if ( (*it)->y >= (*it)->CoolTime6 + 480 || (*it)->CoolTime2 == -99 )
							{
								(*it)->pSprite->getAnimation()->playByIndex(4, 0, 30, 1);
							}
						}
						else if ( (*it)->CoolTime2 < -100 )
						{
							if ( (*it)->y > (*it)->CoolTime6 )
								(*it)->y -= 10;
							else
							{//人初始化
								(*it)->y = (*it)->CoolTime6;
								(*it)->mode = 0;
								(*it)->CoolTime1 = BOSS3_NEED_DAMAGE;//变身需要的血量
								(*it)->CoolTime2 = 200;
								(*it)->CoolTime3 = 0;
								(*it)->CoolTime4 = rand()%3+3;
							}
						}
						break;
					}
				}
			}
			(*it)->pSprite->setScaleX((*it)->toward);
			(*it)->pSprite->setPosition(ccp((*it)->x, (*it)->y));
			break;
		case 13://BOSS4
			{
				MonsterInfomation* pMonster = *it;
				if ( pMonster->x > m_fHeroLocateX )
					pMonster->toward = -1;
				else 
					pMonster->toward = 1;

				if ( pMonster->CoolTime1 <= 0 )
				{
					if ( pMonster->CoolTime1 == 0 )
					{
						pMonster->CoolTime6 = -1;
						pMonster->CoolTime5 = rand()%2;
						if ( pMonster->CoolTime5 == 0 )
						{
							pMonster->CoolTime5 = 100;
							pMonster->lastAction = 4;
						}
						else
						{
							pMonster->CoolTime5 = 40;
							pMonster->lastAction = 5;
						}
						pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 60);
					}
					else if ( pMonster->CoolTime1 < -35 && pMonster->CoolTime1 > -45 && 
						fabs(pMonster->y + pMonster->CoolTime5 - m_fHeroLocateY) < 80 && pMonster->CoolTime6 == -1 )
					{
						pMonster->CoolTime6 = 0;
						BeAttack(50);
					}
					else if ( pMonster->CoolTime1 < -80 )
					{
						pMonster->CoolTime6 = 0;
						pMonster->CoolTime1 = 400;
					}
					pMonster->CoolTime1--;
				}
				else if ( pMonster->CoolTime2 <= 0 )
				{
					if ( pMonster->CoolTime2 == 0 )
					{
						if ( pMonster->CoolTime6 == 0 )
							pMonster->CoolTime6 = 2 + rand()%3;
						else
							pMonster->CoolTime6 --;

						if ( pMonster->hp > BOSS4_NEED_DAMAGE )
							pMonster->lastAction = 6;
						else
							pMonster->lastAction = 10;
						pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 30);
					}
					else if ( pMonster->CoolTime2 == -10 )
					{
						{
							Chopper* pChopper = new Chopper;
							pChopper->pSprite = CCSprite::create("bullet/10/1.png");
							pChopper->type = (*it)->type+1;
							pChopper->damage = 10;
							pChopper->bomb	 = true;
							pChopper->rotate = 0;
							pChopper->active = true;
							if ( pMonster->hp > BOSS4_NEED_DAMAGE )
							{
								pChopper->x = pMonster->x+129*pMonster->toward;
								pChopper->y = pMonster->y+113;
								pChopper->vx= 7.3f*pMonster->toward;
								pChopper->vy= -2.3f;
							}
							else
							{
								pChopper->x = pMonster->x+138*pMonster->toward;
								pChopper->y = pMonster->y+90;
								pChopper->vx= 8*pMonster->toward;
								pChopper->vy= 0;
							}
							pChopper->w = 10;
							pChopper->h = 10;
							pChopper->va= 0;
							pChopper->time = 0;
							pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
							m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
							m_vecChopperArray.push_back(pChopper);
						}
						{
							Chopper* pChopper = new Chopper;
							pChopper->pSprite = CCSprite::create("bullet/10/1.png");
							pChopper->type = (*it)->type+1;
							pChopper->damage = 10;
							pChopper->bomb	 = true;
							pChopper->rotate = 0;
							pChopper->active = true;
							if ( pMonster->hp > BOSS4_NEED_DAMAGE )
							{
								pChopper->x = pMonster->x+148*pMonster->toward;
								pChopper->y = pMonster->y+124;
								pChopper->vx= 7.3f*pMonster->toward;
								pChopper->vy= -2.3f;
							}
							else
							{
								pChopper->x = pMonster->x+107*pMonster->toward;
								pChopper->y = pMonster->y+80;
								pChopper->vx= 8*pMonster->toward;
								pChopper->vy= 0;
							}
							pChopper->w = 10;
							pChopper->h = 10;
							pChopper->va= 0;
							pChopper->time = 0;
							pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
							m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
							m_vecChopperArray.push_back(pChopper);
						}
					}
					else if ( pMonster->CoolTime2 == -30 )
					{
						if ( pMonster->CoolTime6 != 0 )
							pMonster->CoolTime2 = 1;
						else
							pMonster->CoolTime2 = 250;
					}
					pMonster->CoolTime2--;
				}
				else
				{
					if ( pMonster->hp > BOSS4_NEED_DAMAGE )
					{
						if ( pMonster->CoolTime1 > 0 )
							pMonster->CoolTime1 --;
						if ( pMonster->CoolTime3 > 0 )
							pMonster->CoolTime3 --;
					}
					if ( pMonster->CoolTime2 > 0 )
						pMonster->CoolTime2 --;
					if ( pMonster->CoolTime4 != 0 )
						pMonster->CoolTime4 --;

					if ( pMonster->CoolTime4 < 0 )
					{//近战持
						if ( pMonster->CoolTime4 == -20 && fabs(pMonster->x - m_fHeroLocateX) < 140 )
						{
							BeAttack(30);
						}
						else if ( pMonster->CoolTime4 < -40 )
						{
							pMonster->CoolTime4 = 200;
						}
					}
					else if ( pMonster->CoolTime4 == 0 && fabs(pMonster->x - m_fHeroLocateX) < 140 )
					{//近战始
						pMonster->CoolTime4 = -1;
						if ( pMonster->hp > BOSS4_NEED_DAMAGE )
							pMonster->lastAction = 3;
						else
							pMonster->lastAction = 11;
						pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 30);
					}
					else if ( pMonster->CoolTime3 <= 0 && pMonster->hp > BOSS4_NEED_DAMAGE )
					{//步行
						if ( fabs(pMonster->x - m_fHeroLocateX) > 50.0f )
							pMonster->x += pMonster->toward * g_fMonsterSpeed[pMonster->type];

						if ( pMonster->lastAction != 2 )
						{
							pMonster->lastAction = 2;
							pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 30);
						}
						if ( pMonster->CoolTime3 < -100 )
						{
							pMonster->CoolTime3 = 200;
						}
					}
					else
					{//待机
						int action = 0;
						if ( pMonster->hp > BOSS4_NEED_DAMAGE )
							action = 1;
						else
							action = 9;
						if ( pMonster->lastAction != action )
						{
							pMonster->lastAction = action;
							pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 30);
						}
					}
				}
				(*it)->pSprite->setScaleX(pMonster->toward);
				(*it)->pSprite->setPosition(ccp((*it)->x, (*it)->y));
			}
			break;
		case 14://BOSS5
			{
				MonsterInfomation* pMonster = *it;
				if ( m_pHero->MonsterCount() != 1 && pMonster->CoolTime1 == -1 )
				{
					if ( pMonster->CoolTime1 == -1 )
					{
						if ( pMonster->lastAction != 1 )
						{
							pMonster->lastAction = 1;
							pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 30);
						}
					}
					continue;
				}
				else if ( pMonster->CoolTime1 == -1 )
				{
					pMonster->CoolTime1 = 0;
					pMonster->canAttack = true;
				}
				if ( pMonster->CoolTime1 > 0 )
				{
					pMonster->canAttack = false;
					if ( pMonster->CoolTime6 == 70 )
					{
						if ( pMonster->CoolTime1 < 10 )
							pMonster->lastAction = 3;
						else
							pMonster->lastAction = pMonster->CoolTime1-6;
						pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 30);
					}
					else if ( pMonster->CoolTime6 == 60 )
					{//5招怪
						if ( pMonster->CoolTime1 < 9 )
						{
							int a = rand()%3;
							switch ( a )
							{
							case 0:
								m_bCreateMonster	= 3;
								break;
							case 1:
								m_bCreateMonster	= 4;
								break;
							case 2:
								m_bCreateMonster	= 8;
								break;
							}
							m_iBossCreateX		= pMonster->x-170;
							m_iBossCreateY		= pMonster->y;
						}
						else if ( pMonster->CoolTime1 == 9 )
						{
							m_bCreateMonster	= 16;
							m_iBossCreateX		= pMonster->x-170;
							m_iBossCreateY		= pMonster->y;
						}
						else
						{
							m_bCreateMonster	= 17;
							if ( pMonster->CoolTime1 == 10 )
							{
								m_iBossCreateX		= pMonster->x-48;
								m_iBossCreateY		= pMonster->y+125;
							}
							else if ( pMonster->CoolTime1 == 11 )
							{
								m_iBossCreateX		= pMonster->x+17;
								m_iBossCreateY		= pMonster->y+113;
							}
							else if ( pMonster->CoolTime1 == 12 )
							{
								m_iBossCreateX		= pMonster->x-57;
								m_iBossCreateY		= pMonster->y+97;
							}
							else if ( pMonster->CoolTime1 == 13 )
							{
								m_iBossCreateX		= pMonster->x+20;
								m_iBossCreateY		= pMonster->y+140;
							}
							else if ( pMonster->CoolTime1 == 14 )
							{
								m_iBossCreateX		= pMonster->x-12;
								m_iBossCreateY		= pMonster->y+143;
							}
						}
					}
					else if ( pMonster->CoolTime6 == 0 )
					{
						pMonster->CoolTime6 = 71;
						pMonster->CoolTime1++;
						if ( pMonster->CoolTime1 == 15 )
						{
							pMonster->CoolTime1 = -1;
							pMonster->CoolTime5 = BOSS5_NEED_DAMAGE;
						}
					}

					pMonster->CoolTime6--;
				}
				else
				{
					if ( pMonster->CoolTime3 <= 0 )
					{
						if ( pMonster->CoolTime4 == 0 )
						{
							pMonster->CoolTime4 = rand()%2+2;
						}
						if ( pMonster->CoolTime3 == 0 )
						{
							pMonster->callback1 = true;
							pMonster->lastAction = 11;
							pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 60);
						}
						else if ( pMonster->CoolTime3 < -29 && pMonster->CoolTime3 > -32 && pMonster->callback1 
							&& fabs(pMonster->y + 80 - m_fHeroLocateY - 40) < 42 )
						{
							pMonster->callback1 = false;
							BeAttack(30);
						}
						else if ( pMonster->CoolTime3 == -150 )
						{
							pMonster->CoolTime4--;
							if ( pMonster->CoolTime4 )
								pMonster->CoolTime3 = 1;
							else
								pMonster->CoolTime3 = 300;
						}
						pMonster->CoolTime3--;
					}
					else
					{
						if ( pMonster->CoolTime2 > 0 )
							pMonster->CoolTime2--;
						if ( pMonster->CoolTime2 < 0 )
						{
							if ( pMonster->CoolTime2 == -30 && fabs(pMonster->x - m_fHeroLocateX) < 350 && fabs(pMonster->y - m_fHeroLocateY) < 100 )
							{
								BeAttack(30);
							}
							else if ( pMonster->CoolTime2 < -60 )
							{
								pMonster->CoolTime2 = 400;
							}
							pMonster->CoolTime2--;
						}
						else if ( fabs(pMonster->x - m_fHeroLocateX) < 320 && pMonster->CoolTime2 == 0 )
						{
							pMonster->CoolTime2 = -1;
							pMonster->lastAction = 2;
							pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 60);
						}
						else if ( pMonster->CoolTime5 < 0 )
						{
							pMonster->CoolTime1 = 1;
						}
						else
						{
							if ( pMonster->CoolTime3 > 0 )
								pMonster->CoolTime3--;

							if ( pMonster->lastAction != 1 )
							{
								pMonster->lastAction = 1;
								pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 30);
							}
						}
					}
				}
			}
			break;
		case 17://BOSS6
			{
				MonsterInfomation* pMonster = *it;
				if ( pMonster->x > m_fHeroLocateX )
					pMonster->toward = -1;
				else 
					pMonster->toward = 1;
				if ( pMonster->CoolTime6 > 0 )
				{//分身行为
					pMonster->pSprite1->setScaleX(-1);
					pMonster->pSprite2->setScaleX(1);
					if ( pMonster->CoolTime6 == 1000 )
					{
						pMonster->pSprite1->setVisible(true);
						pMonster->pSprite2->setVisible(true);
						pMonster->pSprite1->getAnimation()->playByIndex(9, 0, 50);
						pMonster->pSprite2->getAnimation()->playByIndex(9, 0, 50);
					}
					else if ( pMonster->CoolTime6 == 1 )
					{
						pMonster->pSprite1->setVisible(false);
						pMonster->pSprite2->setVisible(false);
					}
					else if ( pMonster->CoolTime6%100 == 60 )
					{
						pMonster->pSprite1->getAnimation()->playByIndex(10, 0, 30);
						pMonster->pSprite2->getAnimation()->playByIndex(10, 0, 30);
					}
					else if ( pMonster->CoolTime6%100 == 0 )
					{
						pMonster->pSprite1->getAnimation()->playByIndex(11, 0, 30);
						pMonster->pSprite2->getAnimation()->playByIndex(11, 0, 30);
					}
					else if ( pMonster->CoolTime6%100 == 85 || pMonster->CoolTime6%100 == 80 )
					{
						{
							Chopper* pChopper = new Chopper;
							pChopper->pSprite = CCSprite::create("bullet/10/1.png");
							pChopper->type = 14;
							pChopper->damage = 10;
							pChopper->bomb	 = true;
							pChopper->rotate = 0;
							pChopper->active = true;
							pChopper->x = pMonster->cx-658;
							pChopper->y = pMonster->y+255;
							pChopper->vx= 7.8f;
							pChopper->vy= -2.3f;
							pChopper->w = 10;
							pChopper->h = 10;
							pChopper->va= 0;
							pChopper->time = 0;
							pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
							m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
							m_vecChopperArray.push_back(pChopper);
						}
						{
							Chopper* pChopper = new Chopper;
							pChopper->pSprite = CCSprite::create("bullet/10/1.png");
							pChopper->type = 14;
							pChopper->damage = 10;
							pChopper->bomb	 = true;
							pChopper->rotate = 0;
							pChopper->active = true;
							pChopper->x = pMonster->cx-42;
							pChopper->y = pMonster->y+255;
							pChopper->vx= -7.8f;
							pChopper->vy= -2.3f;
							pChopper->w = 10;
							pChopper->h = 10;
							pChopper->va= 0;
							pChopper->time = 0;
							pChopper->pSprite->setPosition( ccp(pChopper->x, pChopper->y) );
							m_pCFrontgroud->GetTileMap()->addChild(pChopper->pSprite, 3);
							m_vecChopperArray.push_back(pChopper);
						}
					}
					pMonster->CoolTime6--;
				}

				if ( pMonster->CoolTime1 <= 0 )
				{//放电
					if ( pMonster->CoolTime1 == 0 )
					{
						pMonster->lastAction = -1;
						pMonster->pSprite->getAnimation()->playByIndex(2, 0, 50);
					}
					else if ( pMonster->CoolTime1 == -60 )
					{
						if ( pMonster->CoolTime5 == 0 )
						{
							pMonster->CoolTime5 = rand()%5+3;
							pMonster->lastAction = -1;
							pMonster->pSprite->getAnimation()->playByIndex(3, 0, 30);
						}
						pMonster->CoolTime5 --;
						pMonster->CoolTime7 = 0;
						if ( pMonster->CoolTime5 == 0 )
						{
							if ( m_pBoss6Skill )
							{
								m_pBoss6Skill->stopAllActions();
								m_pBoss6Skill->removeFromParentAndCleanup(true);
								m_pBoss6Skill = NULL;
							}
							pMonster->CoolTime1 = 800;
						}
						else
							pMonster->CoolTime1 = 0;
					}
					else if ( pMonster->CoolTime5 != 0 && pMonster->CoolTime7 == 0 )
					{
						if ( !m_pBoss6Skill )
						{
							m_pBoss6Skill = common::CreateAnimation("Animation/boss6skill/1.png", ccp((m_iMapNum-0.5f)*800.0f, pMonster->pSprite->getPositionY()),
								ccp(0.5f,0.5f), "Animation/boss6skill/", 2, CCRect(0,0,800,108), true);
							pMonster->pSprite->getParent()->addChild(m_pBoss6Skill, pMonster->pSprite->getZOrder()-1);
						}
						if ( fabs(m_fHeroLocateY - pMonster->y) < 20 )
						{
							pMonster->CoolTime7 = 1;
							BeAttack(10);
						}
					}

					pMonster->CoolTime1 --;
				}
				else if ( pMonster->CoolTime3 <= 0 )
				{
					if ( pMonster->CoolTime3 == 0 )
					{
						pMonster->CoolTime7 = 0;
						pMonster->lastAction = -1;
						pMonster->pSprite->getAnimation()->playByIndex(5, 0, 40);
					}
					else if ( pMonster->CoolTime3 > -50 && pMonster->CoolTime3 < -20 && pMonster->CoolTime7 == 0 )
					{
						if ( fabs(m_fHeroLocateY - pMonster->y) < 150 && fabs(m_fHeroLocateX - pMonster->x) < 240 )
						{
							pMonster->CoolTime7 = 1;
							BeAttack(50);
						}
					}
					else if ( pMonster->CoolTime3 < -50 )
					{
						pMonster->CoolTime3 = 600;
					}
					pMonster->CoolTime3--;
				}
				else if ( pMonster->CoolTime4 <= 0 )
				{//分身准备
					if ( pMonster->CoolTime4 == 0 )
					{
						pMonster->lastAction = -1;
						pMonster->pSprite->getAnimation()->playByIndex(8, 0, 40);
						pMonster->hp += 5000;
						if ( pMonster->hp > pMonster->hpMax )
							pMonster->hp = pMonster->hpMax;
						m_pBossBloodProgress->setPercentage(1+pMonster->hp*92.0f/((float)pMonster->hpMax));
					}
					else if ( pMonster->CoolTime4 == -30 )
						pMonster->CoolTime6 = 1000;
					else if ( pMonster->CoolTime4 < -45 )
						pMonster->CoolTime4 = 2400;

					pMonster->CoolTime4--;
				}
				else if ( pMonster->CoolTime2 < 0 )
				{
					if ( pMonster->CoolTime2 < -30 && pMonster->CoolTime2 > -40 && pMonster->CoolTime7 == 0 &&
						fabs(pMonster->x - m_fHeroLocateX) < 130 && fabs(pMonster->y - m_fHeroLocateY) < 100 )
					{
						pMonster->CoolTime7 = 1;
						BeAttack(30);
					}
					else if ( pMonster->CoolTime2 < -60 )
					{
						pMonster->CoolTime2 = 200;
					}
					pMonster->CoolTime2--;
				}
				else if ( fabs(pMonster->x - m_fHeroLocateX) < 120 && pMonster->CoolTime2 == 0 )
				{
					pMonster->CoolTime2 = -1;
					pMonster->CoolTime7 = 0;
					pMonster->lastAction = -1;
					pMonster->pSprite->getAnimation()->playByIndex(4, 0, 60);
				}
				else
				{
					DIS_SELF(pMonster->CoolTime1);
					DIS_SELF(pMonster->CoolTime2);
					DIS_SELF(pMonster->CoolTime3);
					DIS_SELF(pMonster->CoolTime4);
					if ( fabs(pMonster->x - m_fHeroLocateX) > 70 )
					{
						pMonster->x += g_fMonsterSpeed[pMonster->type]*pMonster->toward;
						if ( pMonster->lastAction != 2 )
						{
							pMonster->lastAction = 2;
							pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 50);
						}
					}
					else if ( pMonster->lastAction != 1 )
					{
						pMonster->lastAction = 1;
						pMonster->pSprite->getAnimation()->playByIndex(pMonster->lastAction-1, 0, 50);
					}
				}
				pMonster->pSprite->setScaleX(pMonster->toward);
				pMonster->pSprite->setPosition(ccp(pMonster->x, pMonster->y));
			}
			break;
		}
		if ( (*it)->attackTime != 0 )
			(*it)->attackTime--;
		if ( (*it)->attackTime < -30 )
			(*it)->attackTime = g_iMonsterAttackTime[(*it)->type];
		(*it)->pSprite->setScaleX((*it)->toward);
		if ( (*it)->pBloodBoard )
			(*it)->pBloodBoard->setPosition( ccp((*it)->x, (*it)->y+(*it)->h+50.0f) );
		if ( (*it)->pFire )
			(*it)->pFire->setPosition(ccp((*it)->x, (*it)->y));
	}
}

void CGameControler::MonsterAttackRun()
{
	while ( m_vecChopperArray.size() )
	{
		bool bStop = true;
		std::vector<Chopper*>::iterator it = m_vecChopperArray.begin();
		for ( ; it != m_vecChopperArray.end(); it++ )
		{
			if ( (*it)->time > 0 )
			{//出现前的准备时间
				(*it)->time --;
				if ( (*it)->time == 0 )
				{
					(*it)->active = true;
					(*it)->pSprite->setVisible(true);
					switch ( (*it)->type )
					{
					case 1:
						{
							AppDelegate::AudioPlayEffect("MS/Monster/male/chopper.mp3");
							float h = m_fHeroLocateY + d_fHeroHeight + 100.0f - (*it)->y - g_fMonsterHeight[(*it)->type]*1.2;
							if ( h < 100.0)
								h = 100.0;
							float t1 = sqrt(h*2/d_fHeroGravity);
							(*it)->vy = t1*d_fHeroGravity;
							float t2 = sqrt( 2*(h+(*it)->y+g_fMonsterHeight[(*it)->type]*1.2-m_fHeroLocateY - d_fHeroHeight)/d_fHeroGravity );
							(*it)->vx = (m_fHeroLocateX - (*it)->x) / (t1 + t2);
							(*it)->va = d_fHeroGravity;
						}
						break;
					case 6:
						{
							(*it)->vx = 0;
							(*it)->vy = 2*d_fHeroGravity;
							(*it)->va = d_fHeroGravity;
						}
						break;
					case 12:
						{
							if ( (*it)->x > m_fHeroLocateX )
								(*it)->toward = -1;
							else
								(*it)->toward = 1;
							float dist = (*it)->x - m_fHeroLocateX;
							if ( fabs(dist) < 0.5f  )
								dist = 1.0f;
							float rotate = atanf(((*it)->y-m_fHeroLocateY-d_fHeroHeight/2)/dist)*(*it)->toward;
							(*it)->vx = 10*cosf(rotate)*(*it)->toward;
							(*it)->vy = 10*sinf(rotate);
						}
						break;
					}
				}
			}
			else if ( 13 == (*it)->type )
			{//手抓
				(*it)->time --;
				if ( (*it)->time < -36 && (*it)->time > -52 && (*it)->bomb )
				{
					if ( RectInRect((*it)->x-(*it)->w/2.0, (*it)->w, m_fHeroLocateX-d_fHeroWidth/2, d_fHeroWidth)
						&& RectInRect((*it)->y, (*it)->h, m_fHeroLocateY, d_fHeroHeight) )
					{
						(*it)->bomb = false;
						BeAttack((*it)->damage);
					}
				}
				else if ( (*it)->time < -90 )
				{
					(*it)->pSprite->removeFromParent();
					DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
					bStop = false;
					break;
				}
			}
			else if ( -1 == (*it)->type )
			{
				if ( (*it)->damage == 0 || (*it)->y < 0 )
					(*it)->time --;
				(*it)->rotate += (*it)->toward*(*it)->damage;
				(*it)->x	+= (*it)->vx;
				(*it)->y	+= (*it)->vy;
				(*it)->vy	-= (*it)->va;
				if ( (*it)->vy < -10.0 )
					(*it)->vy = -10.0;
				if ( CheckInWall((*it)->x, (*it)->y, 12.0f, 18.0f) )
				{
					(*it)->vx	= (*it)->vx/3;
					(*it)->y	-= (*it)->vy;
					(*it)->vy	= -(*it)->vy*2/3;
					if ( (*it)->vy < 5.0f )
					{
						(*it)->vx	= 0;
						(*it)->vy	= 0;
						(*it)->va	= 0;
						(*it)->damage = 0;
					}
				}
				(*it)->pSprite->setRotation( (*it)->rotate );
				(*it)->pSprite->setPosition( ccp((*it)->x, (*it)->y) );
				if ( (*it)->time < -15 )
				{
					(*it)->pSprite->removeFromParent();
					(*it)->pSprite = NULL;
					DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
					bStop = false;
					break;
				}
			}
			else if ( 6 == (*it)->type && ((*it)->bomb || (*it)->touchEarth) )
			{//鸡蛋
				(*it)->time --;
				if ( (*it)->time < -(*it)->clearTime && (*it)->bomb )
				{
					(*it)->pSprite->removeFromParent();
					DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
					bStop = false;
					break;
				}
				else if ( (*it)->time < -100 && (*it)->touchEarth )
				{
					(*it)->bomb = true;
					char str[100] = {0};
					CCArray* animFrames1 = CCArray::createWithCapacity(9);
					for(int i = 2; i < 5; i++) 
					{
						sprintf(str,"Animation/grenade/dan%d.png", i);
						CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iEggWH));
						animFrames1->addObject(frame);
					}
					for(int i = 1; i < 7; i++) 
					{
						sprintf(str,"Animation/grenade/d%d.png", i);
						CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iEggBomgWH));
						animFrames1->addObject(frame);
					}
					CCAnimation* pAnimation1 = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
					(*it)->pSprite->runAction( CCAnimate::create(pAnimation1) );
					//(*it)->pSprite->setScale(0.5f);
					//(*it)->pSprite->setAnchorPoint(ccp(0.5f, 0.5f));
					(*it)->clearTime = 50;
					(*it)->time = -1;
				}
			}
			else if ( (*it)->type == 10 || (*it)->type == 12 || (*it)->type == 14 )
			{//子弹
				(*it)->time --;
				(*it)->x += (*it)->vx;
				(*it)->y += (*it)->vy;
				if ( (*it)->time < -160 )
				{
					(*it)->pSprite->removeFromParent();
					DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
					bStop = false;
					break;
				}
				else if ( fabs((*it)->x - m_fHeroLocateX) > 900 || fabs((*it)->y- m_fHeroLocateY) > 480 )
				{
					(*it)->pSprite->removeFromParent();
					DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
					bStop = false;
					break;
				}
				else if ( RectInRect((*it)->x-(*it)->w/2.0, (*it)->w, m_fHeroLocateX-d_fHeroWidth/2, d_fHeroWidth)
					&& RectInRect((*it)->y, (*it)->h, m_fHeroLocateY, d_fHeroHeight) )
				{
					BeAttack((*it)->damage);
					(*it)->pSprite->removeFromParent();
					DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
					bStop = false;
					break;
				}
				else if ( CheckInWall( (*it)->x, (*it)->y, (*it)->w, (*it)->h ) )
				{
					(*it)->pSprite->removeFromParent();
					DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
					bStop = false;
					break;
				}
				(*it)->pSprite->setPosition( ccp((*it)->x, (*it)->y) );
			}
			else if ( (*it)->active && !(*it)->bomb )
			{//菜刀
				(*it)->x	+= (*it)->vx;
				(*it)->y	+= (*it)->vy;
				(*it)->vy	-= (*it)->va;
				if ( (*it)->vy < -10.0 )
					(*it)->vy = -10.0;
				if ( RectInRect((*it)->x-(*it)->w/2.0, (*it)->w, m_fHeroLocateX-d_fHeroWidth/2, d_fHeroWidth)
					&& RectInRect((*it)->y, (*it)->h, m_fHeroLocateY, d_fHeroHeight) )
				{
					BeAttack((*it)->damage);
					if ( 6 == (*it)->type )
					{
						(*it)->bomb = true;
						char str[100] = {0};
						CCArray* animFrames1 = CCArray::createWithCapacity(6);
						for(int i = 1; i < 7; i++) 
						{
							sprintf(str,"Animation/grenade/d%d.png", i);
							CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iEggBomgWH));
							animFrames1->addObject(frame);
						}
						CCAnimation* pAnimation1 = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
						(*it)->pSprite->runAction( CCAnimate::create(pAnimation1) );
						//(*it)->pSprite->setScale(0.5f);
						//(*it)->pSprite->setAnchorPoint(ccp(0.5f, 0.05f));
						(*it)->time = -1;
						(*it)->clearTime = 35;
					}
					else
					{
						(*it)->pSprite->removeFromParent();
						DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
						bStop = false;
						break;
					}
				}
				else if ( CheckInWall( (*it)->x, (*it)->y, 26.0, 18.0 ) )
				{
					if ( 1 == (*it)->type )
					{
						(*it)->pSprite->removeFromParent();
						DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
						bStop = false;
						break;
					}
					else if ( 8 == (*it)->type )
					{
						CCSprite* pSprite = common::CreateAnimation("Animation/boss2skill1/b1.png", ccp((*it)->x, (*it)->y),
							ccp(0.5f, 0.24f), "Animation/boss2skill1/b", 6, CCRect(0,0,179,144), false, false, 0.1f,
							CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
						m_pCFrontgroud->GetTileMap()->addChild(pSprite, 3);
						(*it)->pSprite->removeFromParent();
						DELETEITERATORFROMVECTOR(m_vecChopperArray, it);
						bStop = false;
						break;
					}
					else if ( 6 == (*it)->type )
					{
						CCArray* pArray = m_pCFrontgroud->GetCrashTriaArray();
						for ( unsigned int i = 0; i < pArray->count(); i++ )
						{
							CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
							float x		= pObject->valueForKey("x")->floatValue();
							float cx	= pObject->valueForKey("width")->floatValue();
							if ( (*it)->x+(*it)->w < x || (*it)->x-(*it)->w > x+cx )
								continue;
							float toward = -1.00f;
							int unSide = pObject->valueForKey("poid")->uintValue();
							float distX;
							if ( unSide)
							{
								distX	= x + cx - (*it)->x ;
								toward	= 1.00f;
							}
							else
								distX	= (*it)->x - x;
							float y		= pObject->valueForKey("y")->floatValue();
							float cy	= pObject->valueForKey("height")->floatValue();
							float distY = distX * cy / cx;
							float compY	= (*it)->y - y;
							if ( compY < 0 || compY  > distY+10.0f )
								continue;

							(*it)->vx = fabs((*it)->vy * cx / cy)*toward;
							
							break;
						}

						(*it)->vx	= (*it)->vx/3;
						(*it)->y	-= (*it)->vy;
						(*it)->pSprite->setPositionY((*it)->y);
						(*it)->vy	= -(*it)->vy*2/3;
						if ( (*it)->vy < 3.5f )
						{
							(*it)->vx	= 0;
							(*it)->vy	= 0;
							(*it)->va	= 0;
							if ( !(*it)->touchEarth )
							{
								(*it)->time = -1;
								(*it)->touchEarth = true;
							}
						}
					}
					break;
				}
				else
				{
					if ( 1 == (*it)->type )
						(*it)->rotate -= 30;
				}
				(*it)->pSprite->setRotation( (*it)->rotate );
				(*it)->pSprite->setPosition( ccp((*it)->x, (*it)->y) );
			}//结束
		}
		if ( bStop )
			break;
	}
}

void CGameControler::MonsterMove(MonsterInfomation* _pMonster, int _index, bool _bMove)
{
	if ( _pMonster->lastAction != _index )
	{
		_pMonster->lastAction = _index;
		if ( _pMonster->secondLife )
			_pMonster->pSprite->getAnimation()->playByIndex(g_iMonsterMoveSecond[_pMonster->type], 0, g_iMonsterFrame[_pMonster->type]);
		else
			_pMonster->pSprite->getAnimation()->playByIndex(_index, 0, g_iMonsterFrame[_pMonster->type]);
	}
	if ( fabs(_pMonster->x - m_fHeroLocateX) < 10.0f && 
		_pMonster->type == 5 )
		return;
	if ( (_bMove || _pMonster->getback) && !(_pMonster->secondLife) && !m_bBoss )
	{
		if ( _pMonster->x - _pMonster->cx > 200 )
			_pMonster->getback = -1;
		else if ( _pMonster->cx - _pMonster->x > 200 )
			_pMonster->getback = 1;
		if ( _pMonster->getback )
		{
			_pMonster->toward = _pMonster->getback;
			if ( (_pMonster->x > _pMonster->cx && _pMonster->getback == 1) 
				|| ( _pMonster->x < _pMonster->cx && _pMonster->getback == -1) )
				_pMonster->getback = 0;
		}
	}
	if ( !MonsterMoveCommon(_pMonster, _bMove) ) 
	{
		if ( !_pMonster->getback )
			if ( _pMonster->x - _pMonster->cx > 200 )
				_pMonster->getback = -1;
			else if ( _pMonster->cx - _pMonster->x > 200 )
				_pMonster->getback = 1;
			else
				_pMonster->getback = - _pMonster->toward;
	}
}

bool CGameControler::MonsterMoveCommon( MonsterInfomation* _pMonster ,bool _bMove, float _speed)
{
	//if ( _pMonster->bPlant )
	//	return true;
	_speed = _speed > 1.0f ? _speed : g_fMonsterSpeed[_pMonster->type];
	float x = _pMonster->x + _pMonster->toward*_speed;
	if ( fabs(x - m_fHeroLocateX) <= _pMonster->toward*g_fMonsterSpeed[_pMonster->type] 
		&& (x - m_fHeroLocateX)*_pMonster->toward >= 0 
		&& !_bMove )
		x = m_fHeroLocateX;
	if ( MonsterMoveCheck1(x, _pMonster->y) )
	{
		//float ydown = _pMonster->y - 5.0f;
		//if ( MonsterMoveCheck2(x, ydown) )
		//{
			_pMonster->x = x;
			_pMonster->pSprite->setPosition(_pMonster->x, _pMonster->y);
			return true;
		//}
		//else 
		//	return false;
	}
	return false;
}

bool CGameControler::MonsterBeAttack(MonsterInfomation* _pMonster, int _damage , int _type)
{
	if ( _pMonster->type == 8 )
	{//二号BOSS被打
		int hp = 0;
		CCTintTo* pColor1 = CCTintTo::create(0.2f, 240, 10, 10);
		CCTintTo* pColor2 =  CCTintTo::create(0.2f, 255, 255, 255);
		if ( _pMonster->bSprite3 )
		{
			_pMonster->hp1 -= _damage;
			hp = _pMonster->hp1;
			if ( hp <= 0 )
			{
				if ( _pMonster->CoolTime3 <= 0 )
				{
					_pMonster->ActionType = 0;
					_pMonster->CoolTime3 = 1000;
				}
				if ( _pMonster->CoolTime6 <= 0 )
				{
					_pMonster->ActionType = 0;
					_pMonster->CoolTime6 = 1000;
				}
				_pMonster->ActionType = 0;
				AppDelegate::AudioPlayEffect("MS3/BOSS2dead.mp3");
				_pMonster->pSprite3->getAnimation()->playByIndex(6, 0, 30);
				hp = _pMonster->hpMax;
				_pMonster->bSprite3 = false;
				if ( m_pSpriteBuffer3 )
				{
					m_pSpriteBuffer3->removeFromParentAndCleanup(true);
					m_pSpriteBuffer3 = NULL;
				}
			}
			else
			{
				_pMonster->pSprite3->stopAllActions();
				_pMonster->pSprite3->runAction(CCSequence::create(pColor1, pColor2, NULL));
				//	_pMonster->pSprite3->getAnimation()->playByIndex(5, 0, 30);
			}
		}
		else if ( _pMonster->bSprite2 )
		{
			_pMonster->hp2 -= _damage;
			hp = _pMonster->hp2;
			if ( hp <= 0 )
			{
				if ( _pMonster->CoolTime2 <= 0 )
				{
					_pMonster->ActionType = 0;
					_pMonster->CoolTime2 = 1000;
				}
				if ( _pMonster->CoolTime5 <= 0 )
				{
					_pMonster->ActionType = 0;
					_pMonster->CoolTime5 = 1000;
				}
				AppDelegate::AudioPlayEffect("MS3/BOSS2dead.mp3");
				_pMonster->pSprite2->getAnimation()->playByIndex(6, 0, 30);
				hp = _pMonster->hpMax;
				_pMonster->bSprite2 = false;
				if ( m_pSpriteBuffer2 )
				{
					m_pSpriteBuffer2->removeFromParentAndCleanup(true);
					m_pSpriteBuffer2 = NULL;
				}
			}
			else
			{
				_pMonster->pSprite2->stopAllActions();
				_pMonster->pSprite2->runAction(CCSequence::create(pColor1, pColor2, NULL));
				//	_pMonster->pSprite2->getAnimation()->playByIndex(5, 0, 30);
			}
		}
		else if ( _pMonster->bSprite1 )
		{
			_pMonster->hp3 -= _damage;
			hp = _pMonster->hp3;
			if ( hp <= 0 )
			{
				if ( _pMonster->CoolTime1 <= 0 )
				{
					_pMonster->ActionType = 0;
					_pMonster->CoolTime1 = 1000;
				}
				if ( _pMonster->CoolTime4 <= 0 )
				{
					_pMonster->ActionType = 0;
					_pMonster->CoolTime4 = 1000;
				}
				AppDelegate::AudioPlayEffect("MS3/BOSS2dead.mp3");
				_pMonster->pSprite1->getAnimation()->playByIndex(6, 0, 30);
				_pMonster->bSprite1 = false;
				if ( m_pSpriteBuffer1 )
				{
					m_pSpriteBuffer1->removeFromParentAndCleanup(true);
					m_pSpriteBuffer1 = NULL;
				}
				_pMonster->alive = false;
				m_iBossLimitX = -1.0f;
				BossBoxStart(_pMonster->x, _pMonster->y);
				return true;
			}
			else
			{
				_pMonster->pSprite1->stopAllActions();
				_pMonster->pSprite1->runAction(CCSequence::create(pColor1, pColor2, NULL));
				//	_pMonster->pSprite1->getAnimation()->playByIndex(5, 0, 30);
			}
		}
		//if ( _pMonster->pSprite )
		//{
		//	CCTintTo* pColor3 = CCTintTo::create(0.2f, 240, 10, 10);
		//	CCTintTo* pColor4 =  CCTintTo::create(0.2f, 255, 255, 255);
		//	_pMonster->pSprite->stopAllActions();
		//	_pMonster->pSprite->runAction(CCSequence::create(pColor3, pColor4, NULL));
		//}
		m_pBossBloodProgress->setPercentage(1+hp*95.0f/((float)_pMonster->hpMax));
		return false;
	}

	_pMonster->hp -= _damage;
	AppDelegate::AudioStopEffect(_pMonster->EffectID);

	if ( m_bMonsterCry && 0 == _type )
	{
		m_bMonsterCry = false;
		CCSprite* pSprite = CCSprite::create("AttackEffect.png");
		pSprite->setScaleY(0.8f);
		pSprite->setScaleX(-0.8f*_pMonster->toward);
		pSprite->setPosition( ccp(_pMonster->x-30*_pMonster->toward, _pMonster->y+_pMonster->h/2) );
		CCMoveBy* pAction = CCMoveBy::create(0.2f, ccp(-50*_pMonster->toward, 0.0) );
		pSprite->runAction( CCSequence::create(pAction, CCDelayTime::create(0.5f), 
			CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL) );
		m_pCFrontgroud->GetTileMap()->addChild(pSprite, 4);
	}

	if ( _pMonster->hp <= 0 && _pMonster->pSprite )
	{
		if ( m_structQuestion.type[2] )
		{
			if ( !m_structQuestion.id[2] || m_structQuestion.id[2] == _pMonster->type+1 )
			{
				m_structQuestion.num[2]--;
				if ( m_structQuestion.num[2] == 0 )
				{
					m_iAllStar++;
					m_structQuestion.complate[2] = 1;
				}
			}
		}
		else if ( m_structQuestion.type[6] && _type == 0 )
		{
			if ( !m_structQuestion.id[6] || m_structQuestion.id[6] == _pMonster->type+1 )
			{
				m_structQuestion.num[6]--;
				if ( m_structQuestion.num[6] == 0 )
				{
					m_iAllStar++;
					m_structQuestion.complate[6] = 1;
				}
			}
		}
		switch ( _pMonster->type )
		{
		case 1://男
		case 10:
			if ( !m_bEffect1 )
			{
				m_bEffect1 = true;
				AppDelegate::AudioPlayEffect("MS3/1_10Dead.mp3");
			}
			break;
		case 4:
		case 11:
			if ( !m_bEffect2 )
			{
				m_bEffect2 = true;
				AppDelegate::AudioPlayEffect("MS/Monster/male/dead.mp3");
			}
			break;
		case 3://胖子
			if ( !m_bEffect1 )
			{
				m_bEffect1 = true;
				AppDelegate::AudioPlayEffect("MS3/4Dead.mp3");
			}
			break;
		case 0://女
			if ( !m_bEffect2 )
			{
				m_bEffect2 = true;
				AppDelegate::AudioPlayEffect("MS4/1Dead.mp3");
			}
			break;
		case 2:
		case 9:
			if ( !m_bEffect3 )
			{
				m_bEffect3 = true;
				AppDelegate::AudioPlayEffect("MS/Monster/female/dead.mp3");
			}
			break;
		case 5://1号BOSS
			AppDelegate::AudioPlayEffect("MS3/BOSS1dead.mp3");
			break;
		case 8://2号BOSS
		case 12://3号BOSS
			AppDelegate::AudioPlayEffect("MS/Monster/boss1/dead.mp3");
			break;
		case 7://狗
			if ( !m_bEffect3 )
			{
				m_bEffect3 = true;
				AppDelegate::AudioPlayEffect("MS/Monster/dog/dead.mp3");
			}
			break;
		case 6://鹰
			if ( !m_bEffect3 )
			{
				m_bEffect3 = true;
				AppDelegate::AudioPlayEffect("MS/Monster/hawk/dead.mp3");
			}
			break;
		}
		bool bDead = false;
		if ( 6 != _pMonster->type && 16 != _pMonster->type && 1 != m_iLevelType && 3 != m_iLevelType && 6 != m_iLevelType && 7 != m_iLevelType )
		{
			int money = 5;
			if ( _pMonster->type < 12 )
				money = moneyNum[_pMonster->type];
			CreateItem(_pMonster->x, _pMonster->y, 3, money);
		}
		_pMonster->alive = false;
		if ( _type == 1 && m_iWeaponUse[1] == 12 && 
			_pMonster->type != 5 && _pMonster->type != 8 && _pMonster->type != 12 && _pMonster->type != 6 )
		{
			_pMonster->firedead = true;
			if ( _pMonster->pBloodBoard )
			{
				_pMonster->pBloodBoard->removeFromParent();
				_pMonster->pBloodBoard = NULL;
			}
			bDead = true;
		}
		else
		{
			int ran = rand()%100;
			if ( ran < 50 && _pMonster->canDeadOnec )
			{
				if ( _pMonster->pBloodBoard )
				{
					_pMonster->pBloodBar->setPercentage(50);
					_pMonster->pSprite->getAnimation()->playByIndex(g_iMonsterRelife[_pMonster->type], 0, 30);
					_pMonster->pBloodBoard->setVisible(false);
				}
				_pMonster->bloodTime = 0;
				_pMonster->willRelife = true;
				_pMonster->canDeadOnec = false;
			}
			else if ( _pMonster->secondLife )
			{
				bDead = true;
				if ( _pMonster->pBloodBoard )
				{
					_pMonster->pBloodBoard->removeFromParent();
					_pMonster->pBloodBoard = NULL;
				}
				_pMonster->pSprite->getAnimation()->playByIndex(g_iMonsterDeadSecond[_pMonster->type],0,30);
				if ( (m_iWeaponUse[0] != 2 && _type == 0) || _type != 0 )
				{
					_pMonster->deadTime = -1;
				}
			}
			else
			{
				bDead = true;
				if ( _pMonster->pBloodBoard )
				{
					_pMonster->pBloodBoard->removeFromParent();
					_pMonster->pBloodBoard = NULL;
				}
				switch ( _type )
				{
				case 0:
					_pMonster->lastAction = g_iMonsterDeadKnife[_pMonster->type];
					if ( m_iWeaponUse[0] != 2 && _pMonster->type != 5 && _pMonster->type != 12 &&
						_pMonster->type != 13 && _pMonster->type != 14 && _pMonster->type != 17 )
					{
						_pMonster->deadTime = -1;
					}
					break;
				case 1:
					if ( _pMonster->type != 5 && _pMonster->type != 12 &&
						_pMonster->type != 13 && _pMonster->type != 14 && _pMonster->type != 17 )
						_pMonster->deadTime = -1;
					_pMonster->lastAction = g_iMonsterDeadGun[_pMonster->type];
					break;
				case 2:
					if ( _pMonster->type != 5 && _pMonster->type != 12 &&
						_pMonster->type != 13 && _pMonster->type != 14 && _pMonster->type != 17 )
						_pMonster->deadTime = -1;
					_pMonster->lastAction = g_iMonsterDeadGun[_pMonster->type];
					break;
				}
				if ( _pMonster->type == 16 )
					_pMonster->deadTime = 1;
				if ( _pMonster->lastAction >=0 && _pMonster->lastAction < 13 )
					_pMonster->pSprite->getAnimation()->playByIndex(_pMonster->lastAction,0,30);
			}
		}
		if ( bDead )
		{
			if ( 0 == _type )
			{
				m_iCombo++;
				if ( m_iComboMax < m_iCombo )
				{
					m_iComboMax = m_iCombo;
					AchieveAdd::Add(56, m_iComboMax);
				}
				m_iComboTime = 250;
				m_bComboShow = false;
				AchieveAdd::Add(1);
			}
			else if ( 1 == _type )
				AchieveAdd::Add(5);
			m_iStateNumKill++;
			if ( m_iNeedKill >0 )
			{
				m_iNeedKill--;
				if ( m_iNeedKill == 0 )
				{
					removeChildByTag(348);
					CCSprite* pSprite = CCSprite::create("Tile/qingli.png");
					pSprite->setAnchorPoint(ccp(0.5f,0.5f));
					pSprite->setScale(0.8f);
					pSprite->setPosition(ccp(400,350));
					CCFadeIn* pAction1 = CCFadeIn::create(0.2f);
					CCFadeOut* pAction2= CCFadeOut::create(0.2f);
					pSprite->runAction(CCSequence::create(pAction1, pAction2, pAction1, pAction2, pAction1, pAction2,
						CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
					addChild(pSprite, 10, 348);
				}
			}
		}
		if ( 5 == _pMonster->type || 8 == _pMonster->type || 12 == _pMonster->type || 13 == _pMonster->type 
			|| 14 == _pMonster->type || 17 == _pMonster->type )
		{//BOSS死亡
				BossBoxStart(_pMonster->x, _pMonster->y);
				return false;
		}
		else if ( 16 == _pMonster->type )
		{
			if ( fabs(_pMonster->x-m_fHeroLocateX) < 100 && fabs(_pMonster->y-m_fHeroLocateY) < 70 )
			{
				BeAttack(30);
			}
			CCSprite* pSprite = common::CreateAnimation("Animation/grenade/d1.png", ccp(_pMonster->x, _pMonster->y), ccp(0.5f, 0.5f),
				"Animation/grenade/d", 6, CCRect(0, 0, 81, 99), false, false, 0.1f, 
				CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
			_pMonster->pSprite->getParent()->addChild(pSprite, _pMonster->pSprite->getZOrder());
			_pMonster->pSprite->setVisible(false);
		}
		return true;
	}
	else if ( _pMonster->bHold && 5 != _pMonster->type && 12 != _pMonster->type && 13 != _pMonster->type 
		&& 14 != _pMonster->type && 17 != _pMonster->type )
	{
		if ( !_pMonster->pSprite )
			return false;
		switch ( _pMonster->type )
		{
		case 1://男
		case 4:
		case 10:
		case 11:
			if ( !m_bEffect4 )
			{
				m_bEffect4 = true;
				_pMonster->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/male/beAttacked.mp3");
			}
			break;
		case 3://胖子
			if ( !m_bEffect4 )
			{
				m_bEffect4 = true;
				AppDelegate::AudioPlayEffect("MS3/4BeAttack.mp3");
			}
			break;
		case 0://女
			if ( !m_bEffect5 )
			{
				m_bEffect5 = true;
				AppDelegate::AudioPlayEffect("MS4/1BeAttack.mp3");
			}
			break;
		case 2:
			if ( !m_bEffect5 )
			{
				m_bEffect5 = true;
				AppDelegate::AudioPlayEffect("MS4/3BeAttack.mp3");
			}
			break;
		case 9:
			if ( !m_bEffect5 )
			{
				m_bEffect5 = true;
				_pMonster->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/female/beAttacked.mp3");
			}
			break;
		case 5://1号BOSS
		case 8:
		case 12:
			_pMonster->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/boss1/beAttacked.mp3");
			break;
		case 7://狗
			if ( !m_bEffect6 )
			{
				m_bEffect6 = true;
				_pMonster->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/dog/beAttacked.mp3");
			}
			break;
		case 6://鹰
			if ( !m_bEffect6 )
			{
				m_bEffect6 = true;
				_pMonster->EffectID = AppDelegate::AudioPlayEffect("MS/Monster/hawk/beAttacked.mp3");
			}
			break;
		}
		if ( _pMonster->pBloodBoard )
		{
			_pMonster->bloodTime = 75;
			_pMonster->pBloodBoard->setVisible(true);
			_pMonster->pBloodBar->setPercentage(_pMonster->hp*100/g_iMonsterHp[_pMonster->type]);
		}
		_pMonster->lastAction = g_iMonsterHold[_pMonster->type];
		_pMonster->holdTime = g_iMonsterHoldTime[_pMonster->type];
		if ( !_pMonster->bPlant && _pMonster->lastAction >= 0 && _pMonster->pSprite )
		{
			if ( _pMonster->secondLife )
				_pMonster->pSprite->getAnimation()->playByIndex(g_iMonsterHoldSecond[_pMonster->type], 0, 30);
			else
				_pMonster->pSprite->getAnimation()->playByIndex(_pMonster->lastAction,0,30);
		}
	}
	else if ( _pMonster->type == 5 || _pMonster->type == 12 || _pMonster->type == 13 
		|| _pMonster->type == 14 || _pMonster->type == 17 )
	{
		if ( m_pBossBloodProgress )
			m_pBossBloodProgress->setPercentage(1+_pMonster->hp*92.0f/((float)_pMonster->hpMax));
		if ( _pMonster->type == 12 )
			_pMonster->CoolTime1 -= _damage;
		if ( _pMonster->type == 14 )
			_pMonster->CoolTime5 -= _damage;

		CCTintTo* pColor1 = CCTintTo::create(0.2f, 240, 10, 10);
		CCTintTo* pColor2 =  CCTintTo::create(0.2f, 255, 255, 255);
		_pMonster->pSprite->stopAllActions();
		_pMonster->pSprite->runAction(CCSequence::create(pColor1, pColor2, NULL));
	}
#ifndef GameTypeC
	if ( _type == 1 && m_iWeaponUse[1] == 12 )
	{
		if ( !_pMonster->pFire )
		{
			_pMonster->pFire = CCSprite::create("Animation/beFire/1.png");
			_pMonster->pFire->setAnchorPoint(ccp(0.5f, 0));
			_pMonster->pFire->setPosition(_pMonster->pSprite->getPosition());
			_pMonster->pSprite->getParent()->addChild(_pMonster->pFire, _pMonster->pSprite->getZOrder());
		}
		CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("Animation/beFire/", 5, CCRect(0,0,145,168)));
		_pMonster->pFire->setVisible(true);
		_pMonster->pFire->stopAllActions();
		_pMonster->pFire->runAction(CCSequence::create(pAction, CCCallFuncN::create(this, callfuncN_selector(CGameControler::MonsterBeFire)), NULL));
	}
#endif

	return false;
}

bool CGameControler::MonsterMoveCheck1(float _x, float &_y)
{
	bool flag = false;
	float fMax = -100.0;
	CCArray* pArray = NULL;
	
	////////////////////
	///****运动物体****/
	////////////////////
	//float fYMove = 0;
	//MapObject* pObjectArray = m_pCFrontgroud->GetMapObjects();
	//int unObjectNum = m_pCFrontgroud->GetObjectNum();
	//for ( int i = 0; i < unObjectNum; i++ )
	//{
	//	MapObject* pMapObject = &pObjectArray[i];
	//	if ( !pMapObject->bCheck )
	//		continue;
	//	if ( _x < pMapObject->x || _x > pMapObject->x+pMapObject->w  )
	//		continue;
	//	//if ( pMapObject->unKind == 9 || pMapObject->unKind == 10 || pMapObject->unKind == 0 )
	//	//{
	//	//	if ( !pMapObject->argb )
	//	//		continue;
	//	//}
	//	if ( RectInRect( _y, 20, pMapObject->y, pMapObject->h ) )
	//		fMax = pMapObject->y + pMapObject->h + d_fHeroGravity ;
	//}

	//////////////
	/****方形****/
	//////////////
	if ( fMax < 0.0 )
	{
		pArray = m_pCFrontgroud->GetCrashRectArray();
		for ( unsigned int i = 0; i < pArray->count(); i++ )
		{
			CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
			float x = pObject->valueForKey("x")->floatValue();
			float w = pObject->valueForKey("width")->floatValue();
			if ( _x < x || _x > x+w  )
				continue;

			float y = pObject->valueForKey("y")->floatValue();
			float h = pObject->valueForKey("height")->floatValue();
			if ( RectInRect( _y-20, 40, y, h ) )
			{
				flag = true;
				if ( RectInRect( _y, 40, y, h ) )
				{
					fMax = y+h+d_fHeroGravity;
				}
				break;
			}
		}
	}

	//////////////
	/****三角****/
	//////////////
	if ( fMax < 0.0 )
	{
		pArray = m_pCFrontgroud->GetCrashTriaArray();
		for ( unsigned int i = 0; i < pArray->count(); i++ )
		{
			CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
			float x		= pObject->valueForKey("x")->floatValue();
			float cx	= pObject->valueForKey("width")->floatValue();
			if ( _x < x || _x > x+cx  )
				continue;

			int unSide = pObject->valueForKey("poid")->uintValue();
			float distX;
			if ( unSide)
				distX	= x + cx - _x ;
			else
				distX	= _x - x;
			float y		= pObject->valueForKey("y")->floatValue();
			float cy	= pObject->valueForKey("height")->floatValue();
			float distY = distX * cy / cx;
			//if ( fabs(_y-10-y-distY) < d_fSpan+cy )
			//{
			//	flag = true;
				if ( fabs(_y-y-distY) < d_fSpan )
				{
					flag = true;
					fMax = y+distY+d_fHeroGravity;
					break;
				}
			//}
		}
	}

	if ( fMax > 0.0 )
	{
		if ( /*fMax >= _y &&*/ fMax-_y < d_fSpan )
		{
			_y	= fMax;
		}
		else
			flag = false;
	}
	return flag;
}

bool CGameControler::MonsterMoveCheck2(float _x, float _y)
{
	CCArray* pArray = NULL;
	////////////////////
	///****运动物体****/
	////////////////////
	//float fYMove = 0;
	//MapObject* pObjectArray = m_pCFrontgroud->GetMapObjects();
	//int unObjectNum = m_pCFrontgroud->GetObjectNum();
	//for ( int i = 0; i < unObjectNum; i++ )
	//{
	//	MapObject* pMapObject = &pObjectArray[i];
	//	if ( !pMapObject->bCheck )
	//		continue;
	//	if ( _x < pMapObject->x || _x > pMapObject->x+pMapObject->w  )
	//		continue;
	//	//if ( pMapObject->unKind == 9 || pMapObject->unKind == 10 || pMapObject->unKind == 0 )
	//	//{
	//	//	if ( !pMapObject->argb )
	//	//		continue;
	//	//}
	//	if ( RectInRect( _y, 20, pMapObject->y, pMapObject->h ) )
	//		return true;
	//}

	//////////////
	/****方形****/
	//////////////
	pArray = m_pCFrontgroud->GetCrashRectArray();
	for ( unsigned int i = 0; i < pArray->count(); i++ )
	{
		CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
		float x = pObject->valueForKey("x")->floatValue();
		float w = pObject->valueForKey("width")->floatValue();
		if ( _x < x || _x > x+w  )
			continue;

		float y = pObject->valueForKey("y")->floatValue();
		float h = pObject->valueForKey("height")->floatValue();
		if ( RectInRect( _y, 40, y, h ) )
		{
			return true;
		}
	}

	//////////////
	/****三角****/
	//////////////
	pArray = m_pCFrontgroud->GetCrashTriaArray();
	for ( unsigned int i = 0; i < pArray->count(); i++ )
	{
		CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
		float x		= pObject->valueForKey("x")->floatValue();
		float cx	= pObject->valueForKey("width")->floatValue();
		if ( _x < x || _x > x+cx  )
			continue;
		return true;

		//int unSide = pObject->valueForKey("poid")->uintValue();
		//float distX;
		//if ( unSide)
		//	distX	= x + cx - _x ;
		//else
		//	distX	= _x - x;
		//float y		= pObject->valueForKey("y")->floatValue();
		//float cy	= pObject->valueForKey("h")->floatValue();
		//float distY = distX * cy / cx;
		//if ( fabs(_y-y-distY) < d_fSpan+cy )
		//{
		//	return true;
		//}
	}
	return false;
}

void CGameControler::CreateAttack()
{
	if ( m_iHoldTime > 0 || m_iGuideNow == 10 )
		return;
	if ( m_iAttackCoolTime == -1 )
		m_iTaijimanAttack = 0;
	if ( m_iAttackCoolTime > 0 || m_bDead )
		return;
	if ( m_ewtNowWeapon > 0 )
	{
		if ( m_iBulletNum[m_ewtNowWeapon-1] <= 0 )
		{
			m_iAttackCoolTime = 10;
			AppDelegate::AudioPlayEffect("MS4/noBullet.mp3");
			//进入买子弹
			Sell4_1(this);
			return;
		}
		else
			m_iBulletNum[m_ewtNowWeapon-1]--;
	}

	if ( m_iFireTime && m_pFireSprite )
	{
		m_pFireObject->life = 10;
		m_pFireObject->index= m_iBulletIndex;
		m_iAttackCoolTime	= m_iWeaponSpeed[m_ewtNowWeapon];
		m_iBulletIndex++;
		AppDelegate::AudioPlayEffect("MS2/firegun.mp3");
	}
	else
	{
		AttackingObject* pAttackingObject	= new AttackingObject;
		m_vecAttackingObjectArray.push_back(pAttackingObject);
		pAttackingObject->toward= m_fHeroToward;
		pAttackingObject->index	= m_iBulletIndex;
		m_iBulletIndex++;
		if ( m_iFireTime )
		{
			AppDelegate::AudioPlayEffect("MS2/firegun.mp3");
			pAttackingObject->type		= eWT_Fire;
			m_iAttackCoolTime			= m_iWeaponSpeed[m_ewtNowWeapon];
			m_pFireObject				= pAttackingObject;
			m_pFireObject->time			= 10;
			pAttackingObject->life		= 10;
			pAttackingObject->speed		= 0;
			pAttackingObject->pSprite	= common::CreateAnimation("Animation/fire/1.png", CCPoint(0, 0), CCPoint(0.02f, 0.14f),
				"Animation/fire/", 7, CCRect(0, 0, 313, 133), true);
			m_pCFrontgroud->GetTileMap()->addChild(pAttackingObject->pSprite, 3);
			pAttackingObject->pSprite->setScaleX(pAttackingObject->toward);
			pAttackingObject->w			= 270;
			pAttackingObject->h			= 90;
			pAttackingObject->damage	= m_iWeaponDamage[m_ewtNowWeapon];
			pAttackingObject->index		= m_iBulletIndex;
			m_iBulletIndex++;
			//pAttackingObject->rotation	= m_pHero->GetRotation();
			//pAttackingObject->pSprite->setRotation( -pAttackingObject->rotation * pAttackingObject->toward );
			//pAttackingObject->rotation  = (float)(pAttackingObject->rotation * PI_FIX);
			pAttackingObject->x			= m_fHeroLocateX + ( d_fWeaponCenterX+m_iFireX[m_ewtNowWeapon-1]/**cosf(pAttackingObject->rotation)*/ )*m_fHeroToward;

			float heightFix = 0.0;
			if ( m_iHeroJumps > 0 || m_fHeroSpeedY < -4.0 )
				heightFix = d_fJumpHigher;
			else if ( ( m_pMoveActionArray[0] || m_pMoveActionArray[1] ) && AppDelegate::s_HeroType == 0 )
				heightFix = d_fRunHigher;
			if ( AppDelegate::s_HeroType != 2 )
				pAttackingObject->y		= m_fHeroLocateY+m_iFireY[m_ewtNowWeapon-1]+/*m_iFireY[m_ewtNowWeapon-1]*sinf(pAttackingObject->rotation)*/ + heightFix;
			else
				pAttackingObject->y		= m_fHeroLocateY+g_fFireY3 - 28;

			pAttackingObject->pSprite->setPosition( ccp(pAttackingObject->x, pAttackingObject->y) );
			m_pFireSprite = pAttackingObject->pSprite;
		}
		else
		{
			pAttackingObject->w		= m_iBulletW[m_ewtNowWeapon];
			pAttackingObject->h		= m_iBulletH[m_ewtNowWeapon];
			pAttackingObject->type	= m_ewtNowWeapon;
			pAttackingObject->damage= m_iWeaponDamage[m_ewtNowWeapon];
			m_iAttackCoolTime		= m_iWeaponSpeed[m_ewtNowWeapon];
			switch ( m_ewtNowWeapon )
			{
			case eWT_Knife:
#ifdef Old_Version
				if ( AppDelegate::s_HeroType == 1 && m_iWeaponUse[0] == 14 )
				{
					switch ( m_iTaijimanAttack )
					{
					case 0:
						g_pHero->Combat(m_iKnifeAction[4]);
						break;
					case 1:
						g_pHero->Combat(36);
						break;
					case 2:
						g_pHero->Combat(37);
						break;
					}
					m_iTaijimanAttack++;
					if ( m_iTaijimanAttack > 2 )
						m_iTaijimanAttack = 0;
				}
				else if ( AppDelegate::s_HeroType == 2 )
#else
				if ( AppDelegate::s_HeroType == 2 )
#endif
				{
					if ( AppDelegate::s_WeaponLevel[15] == 0 )
						g_pHero->Combat(12);
					else
					{
						g_pHero->Combat(16+AppDelegate::s_WeaponLevel[15]);
					}
				}
				else
					g_pHero->Combat(m_iKnifeAction[4]);
				AppDelegate::AudioPlayEffect(m_sCutSound.c_str());
				pAttackingObject->time	= m_iWeaponSpeed[m_ewtNowWeapon];
				pAttackingObject->x		= m_fHeroLocateX + pAttackingObject->w*m_fHeroToward/2;
				pAttackingObject->y		= m_fHeroLocateY + d_fWeaponCenterY;
				break;
			case eWT_Pistol:
			case eWT_M4A1:
				m_bUnFire = false;
				g_pHero->Fire(m_iFireIndex[m_ewtNowWeapon-1], m_fHeroToward);
				char name[255];
				sprintf(name, "MS/Weapon/%d.mp3", m_iWeaponUse[m_ewtNowWeapon]);
				AppDelegate::AudioPlayEffect(name);
				pAttackingObject->time		= 10;
				pAttackingObject->life		= 20;
				pAttackingObject->speed		= g_fWeaponStartSpeed[m_ewtNowWeapon];
				pAttackingObject->rotation	= m_pHero->GetRotation();
				pAttackingObject->rotation  = (float)(pAttackingObject->rotation * PI_FIX);
				pAttackingObject->x			= m_fHeroLocateX + ( d_fWeaponCenterX+m_iFireX[m_ewtNowWeapon-1]*cosf(pAttackingObject->rotation) )*m_fHeroToward;
				{
					float heightFix = 0.0;
					if ( m_iHeroJumps > 0 || m_fHeroSpeedY < -4.0 )
						heightFix = d_fJumpHigher;
					else if ( (m_pMoveActionArray[0] || m_pMoveActionArray[1]) && AppDelegate::s_HeroType == 0 )
						heightFix = d_fRunHigher;
					pAttackingObject->y		= m_fHeroLocateY + m_iFireY[m_ewtNowWeapon-1]+m_iFireX[m_ewtNowWeapon-1]*sinf(pAttackingObject->rotation) + heightFix;
				}
				pAttackingObject->vx = pAttackingObject->speed*cosf(pAttackingObject->rotation)*pAttackingObject->toward;
				pAttackingObject->vy = pAttackingObject->speed*sinf(pAttackingObject->rotation);
				//if ( m_ptBullet[m_ewtNowWeapon-1] )
				if ( m_sBulletPath != "" )
				{
					//pAttackingObject->pSprite = CCSprite::createWithTexture(m_ptBullet[m_ewtNowWeapon-1]);
					pAttackingObject->pSprite = CCSprite::create(m_sBulletPath.c_str());
					pAttackingObject->pSprite->setScaleX(pAttackingObject->toward);
					pAttackingObject->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
					pAttackingObject->pSprite->setPosition( ccp(pAttackingObject->x, pAttackingObject->y) );
					m_pCFrontgroud->GetTileMap()->addChild(pAttackingObject->pSprite, 3);
					pAttackingObject->pSprite->setRotation( -pAttackingObject->rotation * pAttackingObject->toward /(PI_FIX));
				}
				if ( m_iWeaponUse[1] < 12 )
				{
					if ( !m_pFireGas )
					{
						m_pFireGas = CCSprite::create("yan1.png");
						m_pFireGas->setScale(1.3f);
						m_pFireGas->setPosition(ccp(pAttackingObject->x-15*m_fHeroToward,pAttackingObject->y));
						m_pFireGas->setAnchorPoint(ccp(0.5f,0.1f));
						m_pCFrontgroud->GetTileMap()->addChild(m_pFireGas);
						CCFadeOut* pAction = CCFadeOut::create(0.5f);
						CCMoveBy* pAction1 = CCMoveBy::create(0.5f, ccp(0, 30));
						m_pFireGas->runAction(CCSequence::create(pAction, NULL));
						m_pFireGas->runAction(CCSequence::create(pAction1, 
							CCCallFunc::create(this, callfunc_selector(CGameControler::CallBackGas)), NULL));
					}
					if ( m_iWeaponUse[1] == 10 )
						pAttackingObject->vy += (rand()%10/10.0f - 0.5f);
					else if ( m_iWeaponUse[1] == 5 )
					{
						AttackingObject* pAttackingObject1 = new AttackingObject;
						m_vecAttackingObjectArray.push_back(pAttackingObject1);
						pAttackingObject1->w		= m_iBulletW[m_ewtNowWeapon];
						pAttackingObject1->h		= m_iBulletH[m_ewtNowWeapon];
						pAttackingObject1->type		= m_ewtNowWeapon;
						pAttackingObject1->damage	= m_iWeaponDamage[m_ewtNowWeapon];
						pAttackingObject1->toward	= m_fHeroToward;
						pAttackingObject1->index	= m_iBulletIndex;
						m_iBulletIndex++;
						pAttackingObject1->time		= 10;
						pAttackingObject1->life		= 20;
						pAttackingObject1->speed	= g_fWeaponStartSpeed[m_ewtNowWeapon];
						pAttackingObject1->rotation	= m_pHero->GetRotation() + 20;
						pAttackingObject1->rotation = (float)(pAttackingObject1->rotation * PI_FIX);
						pAttackingObject1->x		= m_fHeroLocateX + ( d_fWeaponCenterX+m_iFireX[m_ewtNowWeapon-1]*cosf(pAttackingObject->rotation) )*m_fHeroToward;
						{
							float heightFix = 0.0;
							if ( m_iHeroJumps > 0 || m_fHeroSpeedY < -4.0 )
								heightFix = d_fJumpHigher;
							else if ( m_pMoveActionArray[0] || m_pMoveActionArray[1] )
								heightFix = d_fRunHigher;
							pAttackingObject1->y		= m_fHeroLocateY + m_iFireY[m_ewtNowWeapon-1]+m_iFireX[m_ewtNowWeapon-1]*sinf(pAttackingObject->rotation) + heightFix;
						}
						pAttackingObject1->vx = pAttackingObject1->speed*cosf(pAttackingObject1->rotation)*pAttackingObject1->toward;
						pAttackingObject1->vy = pAttackingObject1->speed*sinf(pAttackingObject1->rotation);
						//pAttackingObject1->pSprite = CCSprite::createWithTexture(m_ptBullet[m_ewtNowWeapon-1]);
						pAttackingObject1->pSprite = CCSprite::create(m_sBulletPath.c_str());
						pAttackingObject1->pSprite->setScaleX(pAttackingObject1->toward);
						pAttackingObject1->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
						pAttackingObject1->pSprite->setPosition( ccp(pAttackingObject1->x, pAttackingObject1->y) );
						m_pCFrontgroud->GetTileMap()->addChild(pAttackingObject1->pSprite, 3);
						pAttackingObject1->pSprite->setRotation( -pAttackingObject1->rotation * pAttackingObject1->toward /(PI_FIX));

						AttackingObject* pAttackingObject2 = new AttackingObject;
						m_vecAttackingObjectArray.push_back(pAttackingObject2);
						pAttackingObject2->toward	= m_fHeroToward;
						pAttackingObject2->index	= m_iBulletIndex;
						pAttackingObject2->w		= m_iBulletW[m_ewtNowWeapon];
						pAttackingObject2->h		= m_iBulletH[m_ewtNowWeapon];
						pAttackingObject2->type	= m_ewtNowWeapon;
						pAttackingObject2->damage= m_iWeaponDamage[m_ewtNowWeapon];
						m_iBulletIndex++;
						pAttackingObject2->time		= 10;
						pAttackingObject2->life		= 20;
						pAttackingObject2->speed	= g_fWeaponStartSpeed[m_ewtNowWeapon];
						pAttackingObject2->rotation	= m_pHero->GetRotation() - 20;
						pAttackingObject2->rotation = (float)(pAttackingObject2->rotation * PI_FIX);
						pAttackingObject2->x		= m_fHeroLocateX + ( d_fWeaponCenterX+m_iFireX[m_ewtNowWeapon-1]*cosf(pAttackingObject->rotation) )*m_fHeroToward;
						{
							float heightFix = 0.0;
							if ( m_iHeroJumps > 0 || m_fHeroSpeedY < -4.0 )
								heightFix = d_fJumpHigher;
							else if ( m_pMoveActionArray[0] || m_pMoveActionArray[1] )
								heightFix = d_fRunHigher;
							pAttackingObject2->y		= m_fHeroLocateY + m_iFireY[m_ewtNowWeapon-1]+m_iFireX[m_ewtNowWeapon-1]*sinf(pAttackingObject->rotation) + heightFix;
						}
						pAttackingObject2->vx = pAttackingObject2->speed*cosf(pAttackingObject2->rotation)*pAttackingObject->toward;
						pAttackingObject2->vy = pAttackingObject2->speed*sinf(pAttackingObject2->rotation);
						//pAttackingObject2->pSprite = CCSprite::createWithTexture(m_ptBullet[m_ewtNowWeapon-1]);
						pAttackingObject2->pSprite = CCSprite::create(m_sBulletPath.c_str());
						pAttackingObject2->pSprite->setScaleX(pAttackingObject2->toward);
						pAttackingObject2->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
						pAttackingObject2->pSprite->setPosition( ccp(pAttackingObject2->x, pAttackingObject2->y) );
						m_pCFrontgroud->GetTileMap()->addChild(pAttackingObject2->pSprite, 3);
						pAttackingObject2->pSprite->setRotation( -pAttackingObject2->rotation * pAttackingObject2->toward /(PI_FIX));
					}
				}
				else if ( m_iWeaponUse[1] == 16 )
				{
					m_iBulletNum[0] -= 2;
					if ( m_iBulletNum[0] < 0 )
						m_iBulletNum[0] = 0;
					AttackingObject* pAttackingObject1 = new AttackingObject;
					m_vecAttackingObjectArray.push_back(pAttackingObject1);
					pAttackingObject1->w		= m_iBulletW[m_ewtNowWeapon];
					pAttackingObject1->h		= m_iBulletH[m_ewtNowWeapon];
					pAttackingObject1->type		= m_ewtNowWeapon;
					pAttackingObject1->damage	= m_iWeaponDamage[m_ewtNowWeapon];
					pAttackingObject1->toward	= pAttackingObject->toward;
					pAttackingObject1->index	= m_iBulletIndex;
					m_iBulletIndex++;
					pAttackingObject1->time		= 10;
					pAttackingObject1->life		= 20;
					pAttackingObject1->speed	= g_fWeaponStartSpeed[m_ewtNowWeapon];
					pAttackingObject1->rotation	= pAttackingObject->rotation;
					pAttackingObject1->x		= pAttackingObject->x - 10.0f*m_fHeroToward*sin(pAttackingObject1->rotation);
					pAttackingObject1->y		= pAttackingObject->y - 10.0f*cos(pAttackingObject1->rotation);
					pAttackingObject1->vx		= pAttackingObject->vx;
					pAttackingObject1->vy		= pAttackingObject->vy;
					//pAttackingObject1->pSprite = CCSprite::createWithTexture(m_ptBullet[m_ewtNowWeapon-1]);
					pAttackingObject1->pSprite = CCSprite::create(m_sBulletPath.c_str());
					pAttackingObject1->pSprite->setScaleX(pAttackingObject1->toward);
					pAttackingObject1->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
					pAttackingObject1->pSprite->setPosition( ccp(pAttackingObject1->x, pAttackingObject1->y) );
					m_pCFrontgroud->GetTileMap()->addChild(pAttackingObject1->pSprite, 3);
					pAttackingObject1->pSprite->setRotation( -pAttackingObject1->rotation * pAttackingObject1->toward /(PI_FIX));

					AttackingObject* pAttackingObject2 = new AttackingObject;
					m_vecAttackingObjectArray.push_back(pAttackingObject2);
					pAttackingObject2->toward	= m_fHeroToward;
					pAttackingObject2->index	= m_iBulletIndex;
					pAttackingObject2->w		= m_iBulletW[m_ewtNowWeapon];
					pAttackingObject2->h		= m_iBulletH[m_ewtNowWeapon];
					pAttackingObject2->type	= m_ewtNowWeapon;
					pAttackingObject2->damage= m_iWeaponDamage[m_ewtNowWeapon];
					m_iBulletIndex++;
					pAttackingObject2->time		= 10;
					pAttackingObject2->life		= 20;
					pAttackingObject2->speed	= g_fWeaponStartSpeed[m_ewtNowWeapon];
					pAttackingObject2->rotation	= pAttackingObject->rotation;
					pAttackingObject2->x		= pAttackingObject->x + 10.0f*m_fHeroToward*sin(pAttackingObject1->rotation);
					pAttackingObject2->y		= pAttackingObject->y + 10.0f*cos(pAttackingObject1->rotation);
					pAttackingObject2->vx		= pAttackingObject->vx;
					pAttackingObject2->vy		= pAttackingObject->vy;
					//pAttackingObject2->pSprite = CCSprite::createWithTexture(m_ptBullet[m_ewtNowWeapon-1]);
					pAttackingObject2->pSprite = CCSprite::create(m_sBulletPath.c_str());
					pAttackingObject2->pSprite->setScaleX(pAttackingObject2->toward);
					pAttackingObject2->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
					pAttackingObject2->pSprite->setPosition( ccp(pAttackingObject2->x, pAttackingObject2->y) );
					m_pCFrontgroud->GetTileMap()->addChild(pAttackingObject2->pSprite, 3);
					pAttackingObject2->pSprite->setRotation( -pAttackingObject2->rotation * pAttackingObject2->toward /(PI_FIX));
				}
				break;
			}
			if ( m_iBulletTime > 0 )
				return;
			switch ( m_ewtNowWeapon )
			{
			case eWT_Knife:
				return;
			}
			if ( m_bShells[m_ewtNowWeapon-1] )
				return;
			switch ( m_ewtNowWeapon )
			{
			case eWT_Pistol:
			case eWT_M4A1:
				{
					m_iBulletTime	= d_iShellsCooltime;
					Shells* pShells = new Shells;
					pShells->pSprite = new CCSprite;
					pShells->pSprite->initWithTexture(m_ptShell);
					pShells->pSprite->autorelease();
					//pShells->pSprite->setScale(0.5f);
					pShells->x	= m_fHeroLocateX + m_fHeroToward*d_fShellsX;
					pShells->y	= m_fHeroLocateY + d_fShellsY;
					pShells->vx = -m_fHeroToward * d_fShellsVX; 
					pShells->vy = d_fShellsVY;
					pShells->rotate = 0.0;
					pShells->pSprite->setScaleX(-m_fHeroToward);
					pShells->pSprite->setPosition( ccp(pShells->x, pShells->y) );
					m_pCFrontgroud->GetTileMap()->addChild(pShells->pSprite, 3);
					m_vecShellsArray.push_back(pShells);
				}
				break;
			}
		}
	}
}

void CGameControler::AttackRun()
{
	////////////////////////
	/****攻击与怪墙检测****/
	////////////////////////
	if ( m_vecAttackingObjectArray.size() )
	{
		std::vector<AttackingObject*>::iterator it = m_vecAttackingObjectArray.begin();
		for (;it != m_vecAttackingObjectArray.end(); it++ )
		{
			switch ( (*it)->type )
			{
			case eWT_Knife:
				(*it)->time --;
				break;
			case eWT_Pistol:
			case eWT_M4A1:
				(*it)->x += (*it)->vx;
				(*it)->y += (*it)->vy;
				if ( (*it)->pSprite )
					(*it)->pSprite->setPosition( ccp((*it)->x, (*it)->y) );
				if ( m_iWeaponUse[(*it)->type] == 8 )
					(*it)->life--;
				if ( m_iWeaponUse[1] == 10 )
				{
					float r = (*it)->pSprite->getRotation();
					r += 10;
					(*it)->pSprite->setRotation(r);
				}
				break;
			case eWT_Fire:
				(*it)->life--;
				break;
			}
		}
	}
}

void CGameControler::AttackCheck()
{
	if ( m_vecAttackingObjectArray.size() )
	{
		while ( m_vecAttackingObjectArray.size() )
		{
			bool bStop		= true;
			bool bStopFor	= false;
			std::vector<AttackingObject*>::iterator it = m_vecAttackingObjectArray.begin();
			for (;it != m_vecAttackingObjectArray.end(); it++ )
			{
				if ( 0 == (*it)->life )
				{
					if ( (*it)->pSprite )
						(*it)->pSprite->stopAllActions();
					if ( m_pFireObject == (*it) )
						m_pFireSprite = NULL;
					REMOVE_SELF((*it)->pSprite);
					DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
					bStop		= false;
					bStopFor	= true;
				}
				else switch ( (*it)->type )
				{
				case eWT_Grenade:
					if ( !m_bBossWin )
					{
						for ( std::vector<MonsterInfomation*>::iterator ppMonster = m_vecMonsterArray.begin();
							ppMonster != m_vecMonsterArray.end(); ppMonster++ )
						{
							if ( !(*ppMonster)->alive || ((*ppMonster)->type == 4 && (*ppMonster)->bRock) )
								continue;
							if ( ((*ppMonster)->x - (*it)->x)*((*ppMonster)->x - (*it)->x) +
								((*ppMonster)->y - (*it)->y)*((*ppMonster)->y - (*it)->y) < d_fGrenadeRange*d_fGrenadeRange
								&& (*ppMonster)->canAttack )
								MonsterBeAttack(*ppMonster, (*it)->damage, 2);
							if ( !m_vecMonsterArray.size() )
							{
								break;
							}
						}
					}
					DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
					bStop		= false;
					bStopFor	= true;
					break;
				case eWT_Knife:
				case eWT_Fire:
					if ( !m_bBossWin )
					{
						if ( (*it)->type == eWT_Fire ||
							( (*it)->time < 21 && m_iWeaponUse[0] == 2 && AppDelegate::s_HeroType == 0 && m_iHoldTime == 0 ) ||
							( (*it)->time < 18 && m_iWeaponUse[0] == 14 && AppDelegate::s_HeroType == 1 && m_iHoldTime == 0 ) ||
							( (*it)->time < 20 && (*it)->time > 7 && AppDelegate::s_HeroType == 2 && m_iHoldTime == 0 ) ||
							( (*it)->time < 21 && m_iWeaponUse[0] == 6 ) || 
							( (*it)->time < 17 && m_iWeaponUse[0] == 13 ) )
						{
							int type = 0;
							if ( (*it)->type == eWT_Fire )
								type = 1;
							int height = 0;
							if ( m_iWeaponUse[0] == 6 || m_iWeaponUse[0] == 13 )
								height = 50;
							if ( m_vecMonsterArray.size() )
							{
								//bool shake = false;
								bool bRemove = false;
								bool bEffect = true;
								for ( std::vector<MonsterInfomation*>::iterator ppMonster = m_vecMonsterArray.begin();
									ppMonster != m_vecMonsterArray.end(); ppMonster++ )
								{
									if ( (*ppMonster)->bulletIndex >= (*it)->index )
										continue;
									(*ppMonster)->bulletIndex = (*it)->index;
									if ( !(*ppMonster)->alive  || ((*ppMonster)->type == 4 && (*ppMonster)->bRock) )
										continue;

									float x = m_fHeroLocateX;
									if ( (*it)->type == eWT_Fire )
										x = (*it)->x;
									if ( m_fHeroToward == -1 )
										x -= (*it)->w;
									int y = m_fHeroLocateY;
									if ( AppDelegate::s_HeroType == 2 )
										y += 50;
									if ( RectInRect(x, (*it)->w, (*ppMonster)->x-(*ppMonster)->w, 2*(*ppMonster)->w) 
										&& RectInRect(y, (*it)->h, (*ppMonster)->y, (*ppMonster)->h+height) )
									{
										if ( bEffect && (*it)->type != eWT_Fire )
										{
											AppDelegate::AudioPlayEffect("MS/Sound/EfMonsterBeCutted.mp3");
											bEffect = false;
										}
										if ( (*ppMonster)->canAttack )
										{
											//shake = true;
											MonsterBeAttack(*ppMonster, (*it)->damage, type);
											if ( !m_vecMonsterArray.size() )
											{
												break;
											}
										}
										//if ( MonsterBeAttack(*ppMonster, (*it)->damage, 0) )
											//bRemove				= true;
									}
								}
								//if ( shake )
								//{
								//	if ( (*it)->type == eWT_Knife /*&& (*it)->time == 8*/ )
								//	{
								//		m_bShakeStop = true;
								//		m_pHero->PauseAction();
								//		scheduleOnce(schedule_selector(CGameControler::ShakeTime), 0.15f);
								//	}
								//}
								//if ( bRemove )
								//{
								//	REMOVE_SELF((*it)->pSprite);
								//	DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
								//	bStop				= false;
								//	bStopFor			= true;
								//}
							}
						}
					}
					if ( (*it)->time == 0 )
					{
						REMOVE_SELF((*it)->pSprite);
						DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
						bStop		= false;
						bStopFor	= true;
					}
					break;
				case eWT_Pistol:
				case eWT_M4A1:
					if ( (*it)->wall )
					{
						if ( CheckInWall( (*it)->x, (*it)->y, 6.0, 6.0 ) 
							|| (*it)->x - m_fHeroLocateX + m_fRelativeX < 0.0 
							|| (*it)->x - m_fHeroLocateX + m_fRelativeX > 800 )
						{
							bStop		= false;
							bStopFor	= true;
							if ( (*it)->isFirst && m_iWeaponUse[1] == 9 )
							{
								char str[100] = {0};
								CCArray* animFrames1 = CCArray::createWithCapacity(7);
								for(int i = 1; i < 7; i++) 
								{
									sprintf(str,"Animation/grenade/d%d.png", i);
									CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iGrenadeWH2));
									animFrames1->addObject(frame);
								}
								CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
								CCSequence* pSequence = CCSequence::create(
									CCAnimate::create(pAnimation), 
									CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)),
									NULL);
								CCSprite* pBomb = CCSprite::create("Animation/grenade/d1.png");
								pBomb->setPosition(ccp((*it)->x, (*it)->y));
								pBomb->runAction(pSequence);
								m_pCFrontgroud->GetTileMap()->addChild(pBomb, 5);
								
								AppDelegate::AudioPlayEffect("MS/Weapon/grenade.mp3");

								CCSprite* pSprite = CCSprite::create("bullet/9/1.png");
								pSprite->setAnchorPoint(ccp(0, 0.5f));
								pSprite->setPosition(ccp((*it)->x, (*it)->y));
								pSprite->setRotation(180);
								pSprite->setScale((*it)->toward);
								m_pCFrontgroud->GetTileMap()->addChild(pSprite, 9);
								pSprite->runAction(CCSequence::create(CCMoveBy::create(0.6f, ccp(-50*(*it)->toward, 0)),
									CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));

								CCSprite* pSprite1 = CCSprite::create("bullet/9/1.png");
								pSprite1->setAnchorPoint(ccp(0, 0.5f));
								pSprite1->setPosition(ccp((*it)->x, (*it)->y));
								pSprite1->setRotation(240);
								pSprite1->setScale((*it)->toward);
								m_pCFrontgroud->GetTileMap()->addChild(pSprite1, 9);
								pSprite1->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-30*(*it)->toward, 40*(*it)->toward)),
									CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));

								CCSprite* pSprite2 = CCSprite::create("bullet/9/1.png");
								pSprite2->setAnchorPoint(ccp(0, 0.5f));
								pSprite2->setPosition(ccp((*it)->x, (*it)->y));
								pSprite2->setRotation(120);
								pSprite2->setScale((*it)->toward);
								m_pCFrontgroud->GetTileMap()->addChild(pSprite2, 9);
								pSprite2->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-30*(*it)->toward, -40*(*it)->toward)),
									CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));
							}
							REMOVE_SELF((*it)->pSprite);
							DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
							break;
						}
					}
					else if ( (*it)->x - m_fHeroLocateX + m_fRelativeX < 0.0 
						|| (*it)->x - m_fHeroLocateX + m_fRelativeX > 800  )
					{
						REMOVE_SELF((*it)->pSprite);
						DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
						bStop		= false;
						bStopFor	= true;
						break;
					}
					if ( m_vecMonsterArray.size() )
					{
						for ( std::vector<MonsterInfomation*>::iterator ppMonster = m_vecMonsterArray.begin();
							ppMonster != m_vecMonsterArray.end(); ppMonster++ )
						{
							if ( !(*ppMonster)->alive  || ((*ppMonster)->type == 4 && (*ppMonster)->bRock) )
								continue;
							if ( RectInRect((*it)->x-3, 6, (*ppMonster)->x-(*ppMonster)->w, 2*(*ppMonster)->w) 
								&& RectInRect((*it)->y-3, 6, (*ppMonster)->y, (*ppMonster)->h) )
							{
								if ( (*ppMonster)->bulletIndex >= (*it)->index && m_iWeaponUse[1] != 9 && m_iWeaponUse[1] != 16 )
									continue;
								(*ppMonster)->bulletIndex = (*it)->index;
								if ( (*ppMonster)->canAttack )
								{
									MonsterBeAttack(*ppMonster, (*it)->damage, 1);
									int iToward = -1.0f;
									if ( (*it)->x < m_fHeroLocateX )
										iToward = 1.0f;
									CCSprite* pSprite = common::CreateAnimation("Animation/MonsterBeShotted/1.png", ccp((*it)->x, (*it)->y),
										ccp(0.2f, 0.5f), "Animation/MonsterBeShotted/", 5, CCRect(0,0,142,90), false, false, 0.1f,
										CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
									pSprite->setScaleX(iToward);
									m_pCFrontgroud->GetTileMap()->addChild(pSprite, 4);
									if ( !m_vecMonsterArray.size() )
									{
										break;
									}
								}

									//(*ppMonster)->pBlood = g_pHero->CreateMonsterBlood((*it)->x, (*it)->y);
								if ( /*eWT_Fire != (*it)->type && */!m_bStrike[(*it)->type-1] && (*it)->pSprite )
								{
									if ( (*it)->isFirst && m_iWeaponUse[1] == 9 )
									{
										char str[100] = {0};
										CCArray* animFrames1 = CCArray::createWithCapacity(7);
										for(int i = 1; i < 7; i++) 
										{
											sprintf(str,"Animation/grenade/d%d.png", i);
											CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iGrenadeWH2));
											animFrames1->addObject(frame);
										}
										CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
										CCSequence* pSequence = CCSequence::create(
											CCAnimate::create(pAnimation), 
											CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)),
											NULL);
										CCSprite* pBomb = CCSprite::create("Animation/grenade/d1.png");
										pBomb->setPosition(ccp((*it)->x, (*it)->y));
										pBomb->runAction(pSequence);
										m_pCFrontgroud->GetTileMap()->addChild(pBomb, 5);

										AppDelegate::AudioPlayEffect("MS/Weapon/grenade.mp3");

										CCSprite* pSprite = CCSprite::create("bullet/9/1.png");
										pSprite->setAnchorPoint(ccp(0, 0.5f));
										pSprite->setPosition(ccp((*it)->x, (*it)->y));
										pSprite->setRotation(180);
										pSprite->setScale((*it)->toward);
										m_pCFrontgroud->GetTileMap()->addChild(pSprite, 9);
										pSprite->runAction(CCSequence::create(CCMoveBy::create(0.6f, ccp(-50*(*it)->toward, 0)),
											CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));

										CCSprite* pSprite1 = CCSprite::create("bullet/9/1.png");
										pSprite1->setAnchorPoint(ccp(0, 0.5f));
										pSprite1->setPosition(ccp((*it)->x, (*it)->y));
										pSprite1->setRotation(240);
										pSprite1->setScale((*it)->toward);
										m_pCFrontgroud->GetTileMap()->addChild(pSprite1, 9);
										pSprite1->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-30*(*it)->toward, 40*(*it)->toward)),
											CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));

										CCSprite* pSprite2 = CCSprite::create("bullet/9/1.png");
										pSprite2->setAnchorPoint(ccp(0, 0.5f));
										pSprite2->setPosition(ccp((*it)->x, (*it)->y));
										pSprite2->setRotation(120);
										pSprite2->setScale((*it)->toward);
										m_pCFrontgroud->GetTileMap()->addChild(pSprite2, 9);
										pSprite2->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-30*(*it)->toward, -40*(*it)->toward)),
											CCCallFuncN::create(this, callfuncN_selector(CGameControler::BulletAimMonster)), NULL));
									}
									REMOVE_SELF((*it)->pSprite);
									DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
									bStop				= false;
									bStopFor			= true;
									break;
								}
							}
						}
					}
					break;
				}
				if ( bStopFor )
					break;
			}
			if ( bStop )
				break;
		}
	}
}

void CGameControler::BulletAimMonster(CCNode* _pNode)
{
	AttackingObject* pAttackingObject	= new AttackingObject;
	m_vecAttackingObjectArray.push_back(pAttackingObject);
	pAttackingObject->index	= m_iBulletIndex;
	m_iBulletIndex++;
	pAttackingObject->isFirst	= 0;
	pAttackingObject->wall	= false;
	pAttackingObject->w		= m_iBulletW[m_ewtNowWeapon];
	pAttackingObject->h		= m_iBulletH[m_ewtNowWeapon];
	pAttackingObject->type	= m_ewtNowWeapon;
	pAttackingObject->damage= m_iWeaponDamage[m_ewtNowWeapon];
	pAttackingObject->time		= 10;
	pAttackingObject->life		= 20;
	pAttackingObject->speed		= g_fWeaponStartSpeed[m_ewtNowWeapon];
	pAttackingObject->x		= _pNode->getPositionX();
	pAttackingObject->y		= _pNode->getPositionY();
	pAttackingObject->pSprite = CCSprite::create("bullet/9/1.png");
	pAttackingObject->pSprite->setAnchorPoint(ccp(0, 0.5f));
	pAttackingObject->pSprite->setPosition( ccp(pAttackingObject->x, pAttackingObject->y) );
	m_pCFrontgroud->GetTileMap()->addChild(pAttackingObject->pSprite, 10);

	MonsterInfomation* pMonster = NULL;
	int time = rand()%4;
	for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
		it != m_vecMonsterArray.end(); it++ )
	{
		if ( !(*it)->alive )
			continue;
		if ( fabs((*it)->x - m_fHeroLocateX + m_fRelativeX - 400) < 420.0f && fabs((*it)->y - m_fHeroLocateY) < 480.0f )
		{
			pMonster = *it;
			if ( !time )
				break;
			time--;
		}
	}

	if ( pMonster )
	{
		float toward = 1.0f;
		if ( _pNode->getPositionX() > pMonster->x )
			toward = -1.0f;
		pAttackingObject->toward	= toward;
		float dist = pMonster->x - _pNode->getPositionX();
		if ( fabs(dist) < 0.5f  )
			dist = 1.0f;
		float rotate = atanf((pMonster->y+pMonster->h/2-_pNode->getPositionY())/dist)*toward;
		pAttackingObject->rotation  = rotate;
		pAttackingObject->vx = pAttackingObject->speed*cosf(pAttackingObject->rotation)*pAttackingObject->toward;
		pAttackingObject->vy = pAttackingObject->speed*sinf(pAttackingObject->rotation);
		pAttackingObject->pSprite->setRotation( -pAttackingObject->rotation * pAttackingObject->toward /(PI_FIX));
		pAttackingObject->pSprite->setScaleX(toward);
	}
	else
	{
		float toward = 1.0f;
		if ( _pNode->getRotation() != 0 )
			toward = -1.0f;
		pAttackingObject->vx = pAttackingObject->speed*toward;
		pAttackingObject->vy = 0;
		pAttackingObject->pSprite->setScaleX(toward);
	}
	_pNode->removeFromParentAndCleanup(true);
}

void CGameControler::GrenadeCreate()
{
	if ( AppDelegate::s_HeroType != 2 )
		m_pGrenade = CCSprite::create("Animation/grenade/0.png");
	else
		m_pGrenade = CCSprite::create("Animation/grenade/grenade2.png");
	m_iGrenadeRotate	= 0;
	m_iGrenadeToward	= m_fHeroToward;
	if ( AppDelegate::s_HeroType != 2 )
	{
		m_fGrenadeLocateX	= m_fHeroLocateX;
		m_fGrenadeLocateY	= m_fHeroLocateY + d_fHeroHeight*2/3;
	}
	else
	{
		m_fGrenadeLocateX	= m_fHeroLocateX + 56*m_fHeroToward;
		m_fGrenadeLocateY	= m_fHeroLocateY + 96;
	}
	if ( AppDelegate::s_HeroType != 2 )
		m_fGrenadeSpeedX	= d_fHeroSpeedX*2*m_fHeroToward;
	else
		m_fGrenadeSpeedX	= 9.0f*m_fHeroToward;
	m_fGrenadeSpeedY	= d_fHeroSpeedX;
	m_pGrenade->setPosition( ccp(m_fGrenadeLocateX, m_fGrenadeLocateY) );
	m_pCFrontgroud->GetTileMap()->addChild(m_pGrenade, 3);
}

void CGameControler::GrenadeRun()
{
	if ( !m_pGrenade || m_bBombedMovie )
		return;
	m_fGrenadeLocateX	+= m_fGrenadeSpeedX;
	m_fGrenadeLocateY	+= m_fGrenadeSpeedY;
	if ( AppDelegate::s_HeroType != 2 )
		m_fGrenadeSpeedY	-= d_fHeroGravity;
	else
		m_fGrenadeSpeedY	-= 0.6f;
	if ( AppDelegate::s_HeroType != 2 )
		m_iGrenadeRotate	+= 15*m_iGrenadeToward;
	else
	{
		m_iGrenadeRotate = -atanf(m_fGrenadeSpeedY/m_fGrenadeSpeedX)/PI_FIX;
	}

	if ( m_fGrenadeLocateY < -100 )
	{
		m_pCFrontgroud->GetTileMap()->removeChild(m_pGrenade);
		m_pGrenade = NULL;
		return;
	}

	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			if ( !(*it)->alive )
				continue;
			if ( RectInRect(m_fGrenadeLocateX-20, 40, (*it)->x - (*it)->w/2, (*it)->w ) )
			{
				if ( RectInRect(m_fGrenadeLocateY-20, 40, (*it)->y, (*it)->h ) )
				{
					AppDelegate::AudioPlayEffect("MS/Weapon/grenade.mp3");
					GrenadeBomb1();
					return;
				}
			}
		}
	}
	if ( CheckInWall(m_fGrenadeLocateX, m_fGrenadeLocateY-d_fGrenadeH/2, d_fGrenadeW, d_fGrenadeH) )
	{
		AppDelegate::AudioPlayEffect("MS/Weapon/grenade.mp3");
		GrenadeBomb();
		return;
	}
	m_pGrenade->setPosition( ccp(m_fGrenadeLocateX, m_fGrenadeLocateY) );
	m_pGrenade->setScaleX(m_iGrenadeToward);
	m_pGrenade->setRotation(m_iGrenadeRotate);
}

void CGameControler::GrenadeBomb()
{
	m_iShakeTime	= 31;
	m_iShakeLocate	= 10;
	m_bBombedMovie	= true;
	m_pGrenade->setRotation(0);

	char str[100] = {0};
	CCArray* animFrames1 = CCArray::createWithCapacity(7);
	for(int i = 1; i < 7; i++) 
	{
		sprintf(str,"Animation/grenade/%d.png", i);
		CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iGrenadeWH1));
		animFrames1->addObject(frame);
	}
	CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
	CCSequence* pSequence = CCSequence::create(
		CCAnimate::create(pAnimation), 
		CCCallFunc::create(this, callfunc_selector(CGameControler::GrenadeOver)),
		NULL);
	m_pGrenade->runAction( pSequence );
	m_pGrenade->setAnchorPoint(ccp(0.5f, 0.05f));

	AttackingObject* pAttackingObject = new AttackingObject;
	pAttackingObject->type	= eWT_Grenade;
	pAttackingObject->w		= d_fGrenadeRange;
	pAttackingObject->x		= m_fGrenadeLocateX;
	pAttackingObject->y		= m_fGrenadeLocateY;
	pAttackingObject->damage= 250;
	m_vecAttackingObjectArray.push_back(pAttackingObject);
}

void CGameControler::GrenadeBomb1()
{
	m_iShakeTime	= 31;
	m_iShakeLocate	= 10;
	m_bBombedMovie	= true;
	m_pGrenade->setRotation(0);

	char str[100] = {0};
	CCArray* animFrames1 = CCArray::createWithCapacity(7);
	for(int i = 1; i < 7; i++) 
	{
		sprintf(str,"Animation/grenade/d%d.png", i);
		CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iGrenadeWH2));
		animFrames1->addObject(frame);
	}
	CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
	CCSequence* pSequence = CCSequence::create(
		CCAnimate::create(pAnimation), 
		CCCallFunc::create(this, callfunc_selector(CGameControler::GrenadeOver)),
		NULL);
	m_pGrenade->runAction( pSequence );
	m_pGrenade->setAnchorPoint(ccp(0.5f, 0.05f));

	AttackingObject* pAttackingObject = new AttackingObject;
	pAttackingObject->type	= eWT_Grenade;
	pAttackingObject->w		= d_fGrenadeRange;
	pAttackingObject->x		= m_fGrenadeLocateX;
	pAttackingObject->y		= m_fGrenadeLocateY;
	pAttackingObject->damage= GRENADE_DAMAGE;
	m_vecAttackingObjectArray.push_back(pAttackingObject);
}

void CGameControler::GrenadeOver()
{
	m_bBombedMovie = false;
	m_pGrenade->stopAllActions();
	m_pGrenade->removeFromParentAndCleanup(true);
	m_pGrenade = NULL;
}

void CGameControler::CreateItem(float& _x, float& _y, int _type /*= -1*/, int _num)
{
	//if ( _type < 0 && m_bBoxBrokenEffect )
	//{
	//	AppDelegate::AudioPlayEffect("MS/Sound/EfMonsterBeCutted.mp3");
	//	m_bBoxBrokenEffect = false;
	//}
	//if ( m_bBossWin )
	//	_type = 4;
	bool skill = false;
	if ( _type == 3 )
	{
		int random = rand()%100;
		if ( random < 10 )
		{
			int a = 0;
		}
		else if ( random < 15 )
		{
			_type = 6;
			skill = true;
		}
		else
			_type = -5;
	}
	if ( _type < -2 )
		return;
	ItemObject* pItemObject = new ItemObject;
	pItemObject->x = _x;
	pItemObject->y = _y;
	if ( _type > 0 )
	{
		pItemObject->type = _type-1;
	}
	else
	{
		int random = rand()%100;
		int type = 0;
		for (; type < 10; type++)
		{
			if ( random < itemPercent[type] )
				break;
		}
		pItemObject->type = type;
	}
	if ( pItemObject->type == 2 )
		pItemObject->num = _num;
	if ( 6 == m_iLevelType )
	{
		if ( m_iGuideNow < 20 && m_iGuideNow > 13 )
		{
			m_iBossBoxNeed --;
			if ( m_iBossBoxNeed <= 0 )
			{
				m_iGuideNow = 20;
				pItemObject->type = 20;
			}
		}
	}
	//if ( 5 == pItemObject->type && !skill )
	//{
	//	pItemObject->type--;
	//}
	if ( 5 == pItemObject->type && skill )
	{
		int type = rand()%2;
		//if ( type == 0 || _type != -1 )
		pItemObject->type = g_iItemNum /*+ type*/;
		//else
		//	pItemObject->type = 5;
		char buffer[255];
		sprintf(buffer, "item/%d/jing.png", 0);
		pItemObject->pSprite1 = CCSprite::create(buffer);
	}
	else if ( pItemObject->type < g_iItemNum )
	{
		char path1[255], path2[255];
		if ( pItemObject->type > 1 )
		{
			sprintf(path1, "item/item%d.png", pItemObject->type+1);
			sprintf(path2, "item/item%d_1.png", pItemObject->type+1);
		}
		else
		{
			int t = rand()%17+1;
			int index = 0;
			while ( t != 0 )
			{
				index ++;
				index = index%17;
				if ( ccbShopMenu::WeaponType(index) == 3 && AppDelegate::s_WeaponOwn[index] == 1 )
					t --;
			}
			pItemObject->type = 30 + index;
			sprintf(path1, "item/bullet/%d.png", index);
			sprintf(path2, "item/bullet/%d-1.png", index);
		}
		pItemObject->pSprite1 = CCSprite::create(path1);
		pItemObject->pSprite2 = CCSprite::create(path2);
		m_pCFrontgroud->GetTileMap()->addChild(pItemObject->pSprite2, 3);
		pItemObject->pSprite2->setPosition(ccp(pItemObject->x, pItemObject->y));
	}
	else if ( pItemObject->type < 20 )
	{
		pItemObject->pSprite1 = CCSprite::create("tu3/qiang2/3.png");
		pItemObject->pSprite1->setScale(0.5f);
		pItemObject->type++;
	}
	else
	{
		pItemObject->pSprite1 = CCSprite::create("tu16/item/35.png");
		pItemObject->pSprite1->setScale(0.5f);
	}
	m_pCFrontgroud->GetTileMap()->addChild(pItemObject->pSprite1, 3);
	pItemObject->pSprite1->setPosition(ccp(pItemObject->x, pItemObject->y));
	m_vecItemObejctArray.push_back(pItemObject);
}

bool CGameControler::RectInRect(float len1, float wid1, float len2, float wid2)
{
	if ( ( len1 /*- wid1*/ >= len2 && len1 /*- wid1*/ <= len2 + wid2 )
		|| ( len1 + wid1 >= len2 && len1 + wid1 <= len2 + wid2 )
		|| ( len2 >= len1 /*- wid1*/ && len2 <= len1 + wid1 )
		|| ( len2 + wid2 >= len1 /*- wid1*/ && len2 + wid2 <= len1 + wid1 ) )
		return true;
	return false;
}

void CGameControler::HeroAction()
{
	if ( m_bBossPause )
		return;
	if ( m_iBulletTime > 0 )
		m_iBulletTime --;
	if ( m_iHoldTime > 0 )
	{
		m_iHoldTime --;
		if ( m_iHoldTime > 6 )
			return;
	}
	//e_WeaponType type = m_ewtNowWeapon;
	switch ( m_ewtNowWeapon )
	{
	case eWT_Knife:
		if ( m_iAttackCoolTime <= 0 )
		{
			if ( ( !m_iHeroJumps && m_bDrop )
				|| ( m_iHeroJumps && m_fHeroSpeedY < 2.0f ) )
				return m_pHero->RunAction(m_iKnifeAction[3]);
			if ( m_iHeroJumps > 1 )
			{
				m_pHero->RunAction(5,15);
				m_pHero->ChangeWeapon(m_iWeaponUse[0]);
				return ;
			}
			else if ( m_iHeroJumps )
				return m_pHero->RunAction(m_iKnifeAction[2]);
			else if ( m_iGrenadeTime == 0 )
			{
				if ( m_pMoveActionArray[0] || m_pMoveActionArray[1] )
					return m_pHero->RunAction(m_iKnifeAction[1]);
				else
					return m_pHero->RunAction(m_iKnifeAction[0]);
			}
		}
		break;
	case eWT_Pistol:
	case eWT_M4A1:
		if ( m_iAttackCoolTime > 0 && m_iJumpActionType != 1 )
		{
			if ( m_iHeroJumps || ( m_fHeroSpeedY < -4.0f && !m_bInSand ) )
			{
				if ( m_iWeaponSpeed[m_ewtNowWeapon] -1 == m_iAttackCoolTime )
				{
					m_pHero->m_unLastAction = -1;
					if ( m_iFireTime )
						m_pHero->RunAction(3,11);
					else
						m_pHero->RunAction(7,11);
					if ( m_bWeaponNeedChange )
					{
						m_bWeaponNeedChange = false;
						m_pHero->ChangeWeapon(m_iWeaponUse[0]);
						m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
					}
					return;
				}
				else if ( m_iJumpActionType == 2 )
				{
					if ( m_iHeroJumps > 1 )
					{
						m_bWeaponNeedChange = true;
						return m_pHero->RunAction(5,15);
					}
					else
					{
						if ( m_fHeroSpeedY > 0 )
							m_pHero->RunAction(3);
						else
							m_pHero->RunAction(4);
						if ( m_bWeaponNeedChange )
						{
							m_bWeaponNeedChange = false;
							m_pHero->ChangeWeapon(m_iWeaponUse[0]);
							m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
						}
						return;
					}
				}
				else if ( m_fHeroSpeedY < 0.0f )
				{
					if ( m_bWeaponNeedChange )
					{
						m_bWeaponNeedChange = false;
						m_pHero->ChangeWeapon(m_iWeaponUse[0]);
						m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
					}
					return m_pHero->RunAction(4);
				}
			}
			else 
			{
				if ( m_bWeaponNeedChange )
				{
					m_bWeaponNeedChange = false;
					m_pHero->ChangeWeapon(m_iWeaponUse[0]);
					m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
				}
				if ( m_pMoveActionArray[0] || m_pMoveActionArray[1] )
				{
					//if ( m_iWeaponSpeed[m_ewtNowWeapon] - d_eWT_FireCoolTime > m_iAttackCoolTime )
					if ( AppDelegate::s_HeroType != 2 )
						return m_pHero->RunAction(2);
					else
						return m_pHero->RunAction(20);
					//else
						//return m_pHero->RunAction(m_iActionType1[m_ewtNowWeapon-1]);
				}
				else if ( m_iJumpActionType == 3 )
				{
					if ( AppDelegate::s_HeroType != 2 )
					{
						return m_pHero->RunAction(1);
					}
				}
				else if ( m_iWeaponSpeed[m_ewtNowWeapon] -1 == m_iAttackCoolTime )
				{
					if ( AppDelegate::s_HeroType != 2 )
					{
						m_pHero->m_unLastAction = -1;
						return m_pHero->RunAction(m_iActionType2[m_ewtNowWeapon-1],11);
					}
					else
						return m_pHero->RunAction(m_iActionType2[m_ewtNowWeapon-1]);
				}
			}
		}
		else
		{
			m_iJumpActionType = 0;
			if ( ( !m_iHeroJumps && m_bDrop )
				|| ( m_iHeroJumps && m_fHeroSpeedY < 2.0 ) )
			{
				m_pHero->RunAction(4);
				if ( m_bWeaponNeedChange )
				{
					m_bWeaponNeedChange = false;
					m_pHero->ChangeWeapon(m_iWeaponUse[0]);
					m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
				}
				return ;
			}
			if ( m_iHeroJumps > 1 )
			{
				m_bWeaponNeedChange = true;
				return m_pHero->RunAction(5,15);
			}
			else if ( m_iHeroJumps )
			{
				m_pHero->RunAction(3);
				if ( m_bWeaponNeedChange )
				{
					m_bWeaponNeedChange = false;
					m_pHero->ChangeWeapon(m_iWeaponUse[0]);
					m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
				}
				return;
			}
			else if ( m_iGrenadeTime == 0 )
			{ 
				if ( m_bWeaponNeedChange )
				{
					m_bWeaponNeedChange = false;
					m_pHero->ChangeWeapon(m_iWeaponUse[0]);
					m_pHero->ChangeWeapon(m_iWeaponUse[m_ewtNowWeapon]);
				}
				if ( m_pMoveActionArray[0] || m_pMoveActionArray[1] )
				{
					if ( AppDelegate::s_HeroType != 2 )
						return m_pHero->RunAction(2);
					else
						return m_pHero->RunAction(20);
				}
				else if ( m_iGrenadeTime == 0 && !m_bAttack && m_iAttackCoolTime <= 0 )
				{
					//if ( AppDelegate::s_HeroType != 2 )
						return m_pHero->RunAction(1);
					//else
					//	return m_pHero->RunAction(m_iActionType2[m_ewtNowWeapon-1]);
				}
			}
		}
		break;
	}
}

void CGameControler::HeroAim()
{
	if ( m_ewtNowWeapon == eWT_Knife )
		return;

	float rotation = atanf( (m_fAimLocateY-m_fHeroLocateY) / (m_fAimLocateX-m_fHeroLocateX) );
	if ( m_iFireTime )
	{
		rotation = 0.0f;
	}
	else if ( m_bAimMonster )
	{
		rotation = m_fAimRotation;
	}
	return m_pHero->HeroAim(rotation/PI_FIX, m_fHeroToward);
}

void CGameControler::WinCheck()
{//胜利
	if ( m_bWin )
		return;
	if ( m_unLevel > 48 && m_unLevel < 55 )
	{
		if ( !m_bBoss && m_fHeroLocateX - m_fRelativeX > d_fMapSizeX * (m_iMapNum-1.40f) 
			&& m_fHeroLocateY < 480 )
		{
			m_bBoss			= true;
			m_bBossPause	= true;
			m_pHero->PauseAction();
			SAFEDELETEARRAY(m_pCFrontgroud->m_pObjectArray);
			m_pCFrontgroud->m_unObjectArrayNum = 0;

			CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			ccNodeLoaderLibrary->registerCCNodeLoader("ccbGameBossMenu", ccbGameBossMenuLoader::loader());
			cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			CCNode* pNode = ccbReader->readNodeGraphFromFile("12.ccbi"/*, this*/);
			pNode->setPosition(ccp(0,0));
			addChild(pNode, 10, 798);
			ccbReader->getAnimationManager()->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
			scheduleOnce(schedule_selector(CGameControler::BossMovie), 3.0f);
		}
	}
	else if ( m_unLevel == 30 )
	{
		if ( !m_bBoss && m_fHeroLocateX - m_fRelativeX > d_fMapSizeX * (m_iMapNum-1.40f) 
			&& m_fHeroLocateY > m_pCFrontgroud->GetTileMap()->getMapSize().height*m_pCFrontgroud->GetTileMap()->getTileSize().height-480 )
		{
			m_bBoss			= true;
			m_bBossPause	= true;
			m_pHero->PauseAction();
			SAFEDELETEARRAY(m_pCFrontgroud->m_pObjectArray);
			m_pCFrontgroud->m_unObjectArrayNum = 0;

			CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			ccNodeLoaderLibrary->registerCCNodeLoader("ccbGameBossMenu", ccbGameBossMenuLoader::loader());
			cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			CCNode* pNode = ccbReader->readNodeGraphFromFile("12.ccbi"/*, this*/);
			pNode->setPosition(ccp(0,0));
			addChild(pNode, 10, 798);
			ccbReader->getAnimationManager()->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
			scheduleOnce(schedule_selector(CGameControler::BossMovie), 3.0f);
		}
	}
	else switch ( m_iLevelType )
	{
	case 1:
		if ( m_iLimitDigit > 0 )
			break;
		m_bWin = true;
		m_iWinTime = 10;
		break;
	case 2:
		if ( m_iWave > 3 )
		{
			m_bWin = true;
			m_iWinTime = 100;
		}
		break;
	case 5:
	case 6:
		if ( m_iMapIndex+1 < m_iMapMax )
		{
			if ( !m_bOver && m_fHeroLocateX > d_fMapSizeX * m_iMapNum-5*d_fCantMove )
			{
				m_bOver = true;
				m_bOverMove = true;
				m_pHero->PauseMonster();
			}
			else if ( m_fHeroLocateX > d_fMapSizeX * m_iMapNum )
			{
				SwitchMapStart();
			}
		}
		else if ( !m_bBoss && m_fHeroLocateX - m_fRelativeX > d_fMapSizeX * (m_iMapNum-1.40f) )
		{
			m_bBoss			= true;
			m_bBossPause	= true;
			m_pHero->PauseAction();
			SAFEDELETEARRAY(m_pCFrontgroud->m_pObjectArray);
			m_pCFrontgroud->m_unObjectArrayNum = 0;

			CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			ccNodeLoaderLibrary->registerCCNodeLoader("ccbGameBossMenu", ccbGameBossMenuLoader::loader());
			cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			CCNode* pNode = ccbReader->readNodeGraphFromFile("12.ccbi"/*, this*/);
			pNode->setPosition(ccp(0,0));
			addChild(pNode, 10, 798);
			ccbReader->getAnimationManager()->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
			scheduleOnce(schedule_selector(CGameControler::BossMovie), 3.0f);
		}
		break;
	case 7:
		break;
	default:
		if ( !m_bOver && m_fHeroLocateX > d_fMapSizeX * m_iMapNum-5*d_fCantMove )
		{
			m_bOver = true;
			m_bOverMove = true;
			m_pHero->PauseMonster();
		}
		else if ( m_fHeroLocateX > d_fMapSizeX * m_iMapNum )
		{
			if ( SwitchMapStart() )
				break;
			m_bWin = true;
			m_iWinTime = 100;
		}
		break;
	}
}

void CGameControler::BossMovie( float _t )
{
	m_bBossPause = false;	
	//m_vecMonsterArray.clear();
	VECTORCLEAN(m_vecMonsterArray, MonsterInfomation);
	removeChildByTag(798);
	m_pHero->ResumeAction();
	if ( m_iGuideNow == 11 )
	{
		GuideTalk(12, 1);
		m_iGuideNow = 12;
	}
}

void CGameControler::Win(float _dt)
{
	if ( 6 == m_iLevelType )
	{
		AppDelegate::s_FirstLogin = 2;
		AppDelegate::SaveGuide();
		AppDelegate::SaveStatus();
		AppDelegate::ChangeScene( ccbChoseHero::CreateScene() );
		return;
	}

	if ( m_bPerfect )
		AchieveAdd::Add(17, 0, m_unLevel);
	if ( m_bUnFire )
		AchieveAdd::Add(22, 0, m_unLevel);
	if ( AppDelegate::s_HeroType == 1 )
		AchieveAdd::Add(26, 0, m_unLevel);
	else if ( AppDelegate::s_HeroType == 2 )
		AchieveAdd::Add(28, 0, m_unLevel);

	if ( m_unLevel%6 != 5 )
	{
		if ( m_structQuestion.type[0] )
		{
			if ( m_iGameMinute*60 + m_iGameSecond/MAINFRAME <= m_structQuestion.num[0] )
			{
				m_iAllStar++;
				m_structQuestion.complate[0] = 1;
			}
		}
		if ( m_structQuestion.type[1] )
		{
			if ( 0 < m_structQuestion.num[1] )
			{ 
				m_iAllStar++;
				m_structQuestion.complate[1] = 1;
			}
		}
		if ( m_structQuestion.type[3] )
		{
			m_iAllStar++;
			m_structQuestion.complate[3] = 1;
		}
		if ( m_structQuestion.type[4] )
		{
			if ( m_structQuestion.complate[4] )
			{
				m_iAllStar++;
			}
		}
		if ( m_structQuestion.type[5] )
		{
			if ( m_iVIPHp >= m_structQuestion.num[5] )
			{
				m_iAllStar++;
				m_structQuestion.complate[5] = 1;
			}
		}
		if ( AppDelegate::s_LevelStar[m_unLevel] < m_iAllStar )
			AppDelegate::s_LevelStar[m_unLevel] = m_iAllStar;
	}

	m_bPause = true;
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	addChild(pCCLayerColor, 16, 10);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary6 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary6->registerCCNodeLoader("ccbGetItemLayer", ccbGetItemLayerLoader::loader());
	cocos2d::extension::CCBReader* ccbReader6 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary6);
	m_pGetItemCCB = ccbReader6->readNodeGraphFromFile("25.ccbi"/*, this*/);
	m_pGetItemCCB->setVisible(false);
	m_pGetItemCCB->setPosition(ccp(0,0));
	//SetScale(m_pSkillNode);
	((ccbGetItemLayer*)m_pGetItemCCB)->setAnimationManager(ccbReader6->getAnimationManager(), this);
	ccbReader6->release();
	addChild(m_pGetItemCCB, 25, 56);

	((ccbGameoverMenu*)m_pGameoverNode)->Win( 100*m_fHeroLocateX/d_fMapSizeX/(m_iMapNum) );
}

void CGameControler::ShowStar()
{
	if ( m_unLevel%6 != 5 && m_unLevel < 49 )
	{
		//int iStarAll = 3;//现得星星数
		scheduleOnce(schedule_selector(CGameControler::ShowOverBtn), 0.60f+m_iAllStar*0.70f);
		if ( m_iAllStar > 0 )
			scheduleOnce(schedule_selector(CGameControler::ShowStarMovie1), 0.30f);
		if ( m_iAllStar > 1 )
			scheduleOnce(schedule_selector(CGameControler::ShowStarMovie2), 1.00f);
		if ( m_iAllStar > 2 )
			scheduleOnce(schedule_selector(CGameControler::ShowStarMovie3), 1.70f);
	}
	else
	{
#ifndef GameTypeC
		scheduleOnce(schedule_selector(CGameControler::ShowOverBtn), 1.30f);
		scheduleOnce(schedule_selector(CGameControler::ShowStarBoss), 0.30f);
#else
		scheduleOnce(schedule_selector(CGameControler::ShowOverBtn), 1.00f);
#endif
	}
}

void CGameControler::ShowStarMovie1( float _dt )
{
	AppDelegate::AudioPlayEffect("MS3/star.mp3");
#ifndef GameTypeC
	CCSprite* pStar = CCSprite::create("tu5/xingxing/3.png");
	pStar->setScale(8.0f);
	CCActionInterval* actionTo1 = CCScaleTo::create(0.2f, 1.0f);
	CCActionInterval* actionTo2 = CCMoveTo::create(0.2f, ccp(d_fStarLocateX1, d_fStarLocateY1));
	CCAnimate* pAnimate = CCAnimate::create( 
		common::CreateAnimationFrame("tu5/xingxing/", 12, CCRect(0,0,154,140), 0.1f) );
	pStar->setPosition(ccp(d_fStarLocateX, d_fStarLocateY));
	pStar->runAction( CCSequence::create(
		CCSpawn::create(actionTo1, actionTo2, NULL),
		pAnimate, NULL) );
#else
	CCSprite* pStar = CCSprite::create("tu5/xingxing/12.png");
	pStar->setScale(8.0f);
	CCActionInterval* actionTo1 = CCScaleTo::create(0.2f, 1.0f);
	CCActionInterval* actionTo2 = CCMoveTo::create(0.2f, ccp(d_fStarLocateX1, d_fStarLocateY1));
	pStar->setPosition(ccp(d_fStarLocateX, d_fStarLocateY));
	pStar->runAction( CCSpawn::create(actionTo1, actionTo2, NULL) );
#endif
	addChild(pStar,24);
	m_iStar++;
}

void CGameControler::ShowStarMovie2( float _dt )
{
	AppDelegate::AudioPlayEffect("MS3/star.mp3");
#ifndef GameTypeC
	CCSprite* pStar = CCSprite::create("tu5/xingxing/3.png");
	pStar->setScale(8.0f);
	CCActionInterval* actionTo1 = CCScaleTo::create(0.2f, 1.0f);
	CCActionInterval* actionTo2 = CCMoveTo::create(0.2f, ccp(d_fStarLocateX2, d_fStarLocateY2));
	CCAnimate* pAnimate = CCAnimate::create( 
		common::CreateAnimationFrame("tu5/xingxing/", 12, CCRect(0,0,154,140), 0.1f) );
	pStar->setPosition(ccp(d_fStarLocateX, d_fStarLocateY));
	pStar->runAction( CCSequence::create(
		CCSpawn::create(actionTo1, actionTo2, NULL),
		pAnimate, NULL) );
#else
	CCSprite* pStar = CCSprite::create("tu5/xingxing/12.png");
	pStar->setScale(8.0f);
	CCActionInterval* actionTo1 = CCScaleTo::create(0.2f, 1.0f);
	CCActionInterval* actionTo2 = CCMoveTo::create(0.2f, ccp(d_fStarLocateX2, d_fStarLocateY2));
	pStar->setPosition(ccp(d_fStarLocateX, d_fStarLocateY));
	pStar->runAction( CCSpawn::create(actionTo1, actionTo2, NULL) );
#endif
	addChild(pStar,24);
	m_iStar++;
}

void CGameControler::ShowStarMovie3( float _dt )
{
	AppDelegate::AudioPlayEffect("MS3/star.mp3");
#ifndef GameTypeC
	CCSprite* pStar = CCSprite::create("tu5/xingxing/3.png");
	pStar->setScale(8.0f);
	CCActionInterval* actionTo1 = CCScaleTo::create(0.2f, 1.0f);
	CCActionInterval* actionTo2 = CCMoveTo::create(0.2f, ccp(d_fStarLocateX3, d_fStarLocateY3));
	CCAnimate* pAnimate = CCAnimate::create( 
		common::CreateAnimationFrame("tu5/xingxing/", 12, CCRect(0,0,154,140), 0.1f) );
	pStar->setPosition(ccp(d_fStarLocateX, d_fStarLocateY));
	pStar->runAction( CCSequence::create(
		CCSpawn::create(actionTo1, actionTo2, NULL),
		pAnimate, NULL) );
#else
	CCSprite* pStar = CCSprite::create("tu5/xingxing/12.png");
	pStar->setScale(8.0f);
	CCActionInterval* actionTo1 = CCScaleTo::create(0.2f, 1.0f);
	CCActionInterval* actionTo2 = CCMoveTo::create(0.2f, ccp(d_fStarLocateX3, d_fStarLocateY3));
	pStar->setPosition(ccp(d_fStarLocateX, d_fStarLocateY));
	pStar->runAction( CCSpawn::create(actionTo1, actionTo2, NULL) );
#endif
	addChild(pStar,24);
	m_iStar++;
}

void CGameControler::ShowStarBoss( float _dt )
{
	AppDelegate::AudioPlayEffect("MS3/star.mp3");
#ifndef GameTypeC
	CCSprite* pStar = CCSprite::create("tu5/boss/ku1.png");
	pStar->setScale(8.0f);
	CCActionInterval* actionTo1 = CCScaleTo::create(0.2f, 1.0f);
	CCActionInterval* actionTo2 = CCMoveTo::create(0.2f, ccp(d_fStarLocateX2, d_fStarLocateY2-30));
	CCAnimate* pAnimate = CCAnimate::create( 
		common::CreateAnimationFrame("tu5/boss/ku", 2, CCRect(0,0,129,123), 0.1f) );
	pStar->setPosition(ccp(d_fStarLocateX, d_fStarLocateY));
	pStar->runAction( CCSequence::create(
		CCSpawn::create(actionTo1, actionTo2, NULL), CCCallFunc::create(this, callfunc_selector(CGameControler::ShowStarBoss1)),
		pAnimate, NULL) );
	addChild(pStar,25);
#endif
}

void CGameControler::ShowStarBoss1()
{
#ifndef GameTypeC
	CCSprite* pSprite = CCSprite::create("tu5/boss/1.png");
	pSprite->setScale(1.5f);
	pSprite->setPosition(ccp(d_fStarLocateX2, d_fStarLocateY2-30));
	pSprite->setAnchorPoint(ccp(0.46f, 0.5f));
	CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("tu5/boss/", 8, CCRect(0,0,361,235)));
	pSprite->runAction(CCSequence::create(pAction, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
	addChild(pSprite,24);
#endif
}

void CGameControler::ShowOverBtn( float _dt )
{
	if ( ((ccbGameoverMenu*)m_pGameoverNode)->m_bWin )
	{
		((ccbGameoverBtnMenu*)m_pGameoverBtnNode)->Win();
		((ccbGameoverMenu*)m_pGameoverNode)->StartBtn();
		((ccbGetItemLayer*)m_pGetItemCCB)->Appear1(1);
	}
	else
	{
		CCLog("GameLose-------------------------------------------");
		((ccbGameoverBtnMenu*)m_pGameoverBtnNode)->Lose();
		((ccbGetItemLayer*)m_pGetItemCCB)->Appear2(0);
	}
}

void CGameControler::DeadCheck()
{//失败
	if ( m_bDead )
		return;
	if ( m_iStateHP > 100 )
		m_iStateHP = 100;
	((ccbControlMenu*)m_pMenuNode)->ShowBlood(m_iStateHP);
	if ( m_iBloodLessTime > 0 )
		m_iBloodLessTime--;
	if ( m_iStateHP <=0 && m_iHealBox > 0 )
	{
		AppDelegate::AudioPlayEffect("MS2/heal.mp3");
		if ( m_pHealEffect1 )
		{
			m_pHealEffect1->stopAllActions();
			m_pCFrontgroud->GetTileMap()->removeChild(m_pHealEffect1);
		}
		if ( m_pHealEffect2 )
		{
			m_pHealEffect2->stopAllActions();
			m_pCFrontgroud->GetTileMap()->removeChild(m_pHealEffect2);
		}
		if ( m_structQuestion.type[1] )
		{
			m_structQuestion.num[1]--;
		}
		m_pHealEffect1 = CCSprite::create("Animation/heal/hou/1.png");
		m_pHealEffect1->setPosition(ccp(m_fHeroLocateX, m_fHeroLocateY));
		m_pHealEffect1->setAnchorPoint(ccp(0.59f, 0.18f));
		m_pCFrontgroud->GetTileMap()->addChild(m_pHealEffect1, 3);
		char str[100] = {0};
		CCArray* animFrames1 = CCArray::createWithCapacity(10);
		for(int i = 1; i < 10; i++) 
		{
			sprintf(str,"Animation/heal/hou/%d.png", i);
			CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iHealEffectWH));
			animFrames1->addObject(frame);
		}
		CCAnimation* pAnimation1 = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
		CCSequence* pSequence1 = CCSequence::create(
			CCAnimate::create(pAnimation1), 
			CCCallFunc::create(this, callfunc_selector(CGameControler::HealEffectOver1)),
			NULL);
		m_pHealEffect1->runAction( pSequence1 );
		m_pHealEffect1->setPositionX(d_fHealEffectToward*m_fHeroToward-d_fHealEffectToward);
		m_pHealEffect1->setScaleX(m_fHeroToward);

		m_pHealEffect2 = CCSprite::create("Animation/heal/qian/1.png");
		//m_pHealEffect2->setPosition(ccp(m_fHeroLocateX, d_fHealEffectFixY+m_fHeroLocateY));
		m_pCFrontgroud->GetTileMap()->addChild(m_pHealEffect2, 4);
		CCArray* animFrames2 = CCArray::createWithCapacity(13);
		for(int i = 1; i < 12; i++) 
		{
			sprintf(str,"Animation/heal/qian/%d.png", i);
			CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, d_iHealEffectWH));
			animFrames2->addObject(frame);
		}
		CCAnimation* pAnimation2 = CCAnimation::createWithSpriteFrames(animFrames2, d_fStarMovieTime);
		CCSequence* pSequence2 = CCSequence::create(
			CCAnimate::create(pAnimation2), 
			CCCallFunc::create(this, callfunc_selector(CGameControler::HealEffectOver2)),
			NULL);
		m_pHealEffect2->runAction( pSequence2 );
		//m_pHealEffect2->setPositionX(d_fHealEffectToward*m_fHeroToward-d_fHealEffectToward);
		m_pHealEffect2->setScaleX(m_fHeroToward);

		if ( 6 != m_iLevelType || m_iHealBox > 1 )
			m_iHealBox--;
		m_iStateHP = 100;
		m_pMoveActionArray[5] = 20;

		if ( m_iPrisonTime > 0 )
		{
			m_iPrisonTime = 0;
		}
		ShowHealBox();
	}
	else if ( m_fHeroLocateY < -200 || m_iStateHP < 0 )
	{
#ifndef GameTypeC
		Sell3();
#else
		AppDelegate::AudioPlayEffect("MS/Sound/EfHeroDead.mp3");
		//m_bPause = true;
		m_bDead = true;
		m_pHero->GetArmature()->setZOrder(10);
		switch (m_ewtNowWeapon)
		{
		case eWT_Knife:
			m_pHero->RunAction(m_iKnifeAction[5]);
			break;
		default:
			m_pHero->RunAction(13);
			break;
		}
		m_pHero->PauseMonster();
		scheduleOnce(schedule_selector(CGameControler::Lose), 0);
#endif
	}
	else if ( m_iVIPHp <= 0 )
	{
		m_bPause = true;
		m_bDead = true;
		m_pHero->PauseMonster();
		scheduleOnce(schedule_selector(CGameControler::Lose), 0);
		m_pVIP->getAnimation()->playByIndex(2, 0, 30);
		m_iVIPAttackedTime = -1;
	}
	else
	{
		int state = 0;
		if ( m_iStateHP < d_iBloodLess )
		{
			if ( 0 == m_iBloodLessTime )
			{
				AppDelegate::AudioPlayEffect("MS/Sound/EfBloodLess.mp3");
				m_iBloodLessTime = 70 + m_iStateHP;
			}
			state = 3;
		}
#ifndef GameTypeC
		else if ( m_iStateHP < d_iBloodNormal )
			state = 2;
		else if ( m_iStateHP < 100 )
			state = 1;
		for ( int i = 0; i < 4; i++ )
		{
			bool bShow = false;
			if ( i == state && !m_bDamaged )
				bShow = true;
			getChildByTag(d_TagWeaponUse+i)->setVisible(bShow);
		}
#endif
	}
}

void CGameControler::Lose(float _dt)
{
	CCNodeLoaderLibrary* ccNodeLoaderLibrary6 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary6->registerCCNodeLoader("ccbGetItemLayer", ccbGetItemLayerLoader::loader());
	cocos2d::extension::CCBReader* ccbReader6 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary6);
	m_pGetItemCCB = ccbReader6->readNodeGraphFromFile("25.ccbi"/*, this*/);
	m_pGetItemCCB->setVisible(false);
	m_pGetItemCCB->setPosition(ccp(0,0));
	//SetScale(m_pSkillNode);
	((ccbGetItemLayer*)m_pGetItemCCB)->setAnimationManager(ccbReader6->getAnimationManager(), this);
	ccbReader6->release();
	addChild(m_pGetItemCCB, 25, 56);
	
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	addChild(pCCLayerColor, 16, 10);
	((ccbGameoverMenu*)m_pGameoverNode)->Lose( 100*m_fHeroLocateX/d_fMapSizeX/(m_iMapNum) );
}

void CGameControler::GameOver(float _dt)
{
	//unschedule(schedule_selector(CGameControler::Timer));
	//CleanAll();
	if ( m_unLevel < 30 )
	{
		int level = m_unLevel/6;
		AppDelegate::ChangeScene(CMainMenu::StartMenu(level, m_iDifficult));
	}
	else
	{
		AppDelegate::ChangeScene(CMainMenu::StartMenu());
	}
}

bool CGameControler::BeAttack(int _damage)
{
	bool flag = false;
	if ( 0 == m_timeBeAttackedEffect )
	{
		AppDelegate::AudioPlayEffect("MS/Sound/EfBeAttacked2.mp3");
		m_timeBeAttackedEffect = 300;
		m_bBeAttackedEffect = false;
		flag = true;
	}
	else if ( m_bBeAttackedEffect )
	{
		AppDelegate::AudioPlayEffect("MS/Sound/EfBeAttacked.mp3");
		m_bBeAttackedEffect = false;
		flag = true;
	}

	if ( flag )
	{
		m_bDamaged = true;
#ifndef GameTypeC
		CCFadeIn* pAction1 = CCFadeIn::create(0.1f);
		CCFadeOut* pAction2 = CCFadeOut::create(0.1f);
		m_pBeAttackedEffect->stopAllActions();
		m_pBeAttackedEffect->runAction(CCSequence::create(pAction1, pAction2, pAction1, pAction2, NULL));

		m_pHeadShake->stopAllActions();
		m_pHeadShake->setVisible(true);
		CCAnimate* pAnimate = CCAnimate::create( common::CreateAnimationFrame(m_ptHeadShake, 4, 0.05f) );
		m_pHeadShake->runAction( CCSequence::create(pAnimate, pAnimate, CCCallFunc::create(this, callfunc_selector(CGameControler::HeadShakeOver)), NULL));
#endif
	}

	m_bPerfect = false;
	m_iStateHP -= _damage;
	m_bComboShow = false;
	m_iCombo = 0;

	int a = rand()%2 + 1;
	char buffer[20];
	sprintf(buffer, "tu4/blood%d.png", a);
	int x = rand()%50 - 25;
	int y = rand()%50;
	int rotate = rand()%360;
	int b = rand()%50;
	float scale = ((float)b)/100 + 0.5f;
	CCSprite* pSprite = CCSprite::create(buffer);
	pSprite->setRotation(rotate);
	pSprite->setScale(scale);
	pSprite->setPosition( ccp(m_fHeroLocateX+x, m_fHeroLocateY+d_fHeroHeight+y) );
	m_pCFrontgroud->GetTileMap()->addChild(pSprite, 5);
	m_pArrayBlood->addObject(pSprite);
	CCActionInterval* pAction = CCFadeOut::create(2.0f);
	pSprite->runAction(CCSequence::create(pAction, 
		CCCallFunc::create(this, callfunc_selector(CGameControler::BloodRemove)), NULL));
	
	return false;
}

void CGameControler::HeadShakeOver()
{
	m_bDamaged = false;
	m_pHeadShake->stopAllActions();
	m_pHeadShake->setVisible(false);
}

void CGameControler::BloodRemove()
{
	CCSprite* pSprite = (CCSprite*)m_pArrayBlood->objectAtIndex(0);
	pSprite->removeFromParentAndCleanup(true);
	m_pArrayBlood->removeObjectAtIndex(0);
}

void CGameControler::ShowScore()
{
	if ( m_ewtNowWeapon > 0 )
		common::ShowNumber(this, m_iBulletNum[m_ewtNowWeapon-1], 11, 15, 171, 348, "tu4/suzi.png", 805, 10);
	else if ( getChildByTag(805) )
		getChildByTag(805)->setVisible(false);
	if ( m_bPause )
		return;
	if ( m_iSkillCanUse == 0 )
	{
		m_pSkillDigit->setVisible(false);
	}
	else
	{
		m_pSkillDigit->setVisible(true);
		common::ShowNumberUpdate(m_pSkillDigit, m_iSkillCanUse+1);
	}
	//common::ShowNumber(this, m_iStateHP/*AppDelegate::s_Money*/, 18, 23, 705, 430, "tu5/suzi.png");
	if ( 1 != m_iLevelType && 3 != m_iLevelType )
	{
		if ( m_iMoneyTo > m_iStateNumGold )
			m_iStateNumGold++;
		float scale = 1.0f;
		int y = 0;
		if ( AppDelegate::s_Money + m_iStateNumGold > 99999 )
		{
			scale = 0.8f;
			y = 3;
		}
		common::ShowNumber(this, AppDelegate::s_Money + m_iStateNumGold, 11, 15, d_fShowLocateMoney+y, "tu4/suzi.png", 801, 10, scale);
		common::ShowNumber(this, m_iGameMinute, 11, 15, d_fShowLocateMinute, "tu4/suzi.png", 802, 10, 1, true);
		common::ShowNumber(this, m_iGameSecond/MAINFRAME, 11, 15, d_fShowLocateSecond, "tu4/suzi.png", 803, 10, 1, true);
	}
	common::ShowNumber(this, m_iGrenadeNum, 11, 15, d_fShowLocateGrenade, "tu4/suzi.png", 804, 10);
}

void CGameControler::ShowHealBox()
{
	for ( int i = 0; i < d_iHealBoxMax; i++ )
	{
		bool bShow = false;
		if ( i < m_iHealBox )
			bShow = true;
		CCNode* pSprite1 = getChildByTag(d_TagHealBoxHave+i);
		pSprite1->setVisible(bShow);
		CCNode* pSprite2 = getChildByTag(d_TagHealBoxUnhave+i);
		pSprite2->setVisible(!bShow);
	}
}

bool CGameControler::SkillCheck()
{
	if ( m_iStateSkill[0] == 100 )
	{
		m_iSkillFullTime[0]++;
		if ( m_iSkillFullTime[0] > 200 )
		{
			m_iSkillFullTime[0] = 0;
			CCNode* pNode1 = getChildByTag(28);
			CCProgressTimer* pNode = (CCProgressTimer*)pNode1->getChildByTag(888);
			if ( pNode )
			{
				CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 1.1f);
				CCScaleTo* pAction2 = CCScaleTo::create(0.1f, 1.0f);
				pNode->stopAllActions();
				pNode->runAction( CCSequence::create( pAction1, pAction2, pAction1, pAction2, pAction1, pAction2, NULL) );

				pAction1 = CCScaleTo::create(0.1f, 1.1f);
				pAction2 = CCScaleTo::create(0.1f, 1.0f);
				pNode1->stopAllActions();
				pNode1->runAction( CCSequence::create( pAction1, pAction2, pAction1, pAction2, pAction1, pAction2, NULL) );
			}

			CCAnimate* pAction = CCAnimate::create( common::CreateAnimationFrame("Animation/skill/3/", 4, CCRect(0,0,76,93)) );
			CCSprite* pSprite = CCSprite::create("Animation/skill/3/1.png");
			pSprite->setPosition( ccp(383, 440) );
			pSprite->runAction( CCSequence::create(pAction, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL) );
			addChild(pSprite, 9);
		}
	}
	//if ( m_iStateSkill[1] == 100 )
	//{
	//	m_iSkillFullTime[1]++;
	//	if ( m_iSkillFullTime[1] > 200 )
	//	{
	//		m_iSkillFullTime[1] = 0;

	//		CCNode* pNode1 = getChildByTag(29);
	//		CCProgressTimer* pNode = (CCProgressTimer*)pNode1->getChildByTag(888);
	//		if ( pNode )
	//		{
	//			CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 1.1f);
	//			CCScaleTo* pAction2 = CCScaleTo::create(0.1f, 1.0f);
	//			pNode->stopAllActions();
	//			pNode->runAction( CCSequence::create( pAction1, pAction2, pAction1, pAction2, pAction1, pAction2, NULL) );

	//			pAction1 = CCScaleTo::create(0.1f, 1.1f);
	//			pAction2 = CCScaleTo::create(0.1f, 1.0f);
	//			pNode1->stopAllActions();
	//			pNode1->runAction( CCSequence::create( pAction1, pAction2, pAction1, pAction2, pAction1, pAction2, NULL) );
	//		}

	//		CCAnimate* pAction = CCAnimate::create( common::CreateAnimationFrame("Animation/skill/3/", 4, CCRect(0,0,76,93)) );
	//		CCSprite* pSprite = CCSprite::create("Animation/skill/3/1.png");
	//		pSprite->setPosition( ccp(492, 440) );
	//		pSprite->runAction( CCSequence::create(pAction, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL) );
	//		addChild(pSprite, 9);
	//	}
	//}
	if ( m_pSkillBar[0] )
		m_pSkillBar[0]->setPercentage(m_iStateSkill[0]);
	//if ( m_pSkillBar[1] )
	//	m_pSkillBar[1]->setPercentage(m_iStateSkill[1]);

	if ( !m_iSkill /*|| 1 *//*&& 0*/)
		return false;

	m_bSkillPause = true;
	switch ( AppDelegate::s_HeroType )
	{
	case 0:
		{
			AppDelegate::AudioPlayEffect("MS4/skillstart.mp3");
			m_pThunder1 = CCSprite::create("Animation/skill/1/huo/a1.png");
			m_pThunder1->setAnchorPoint(ccp(0.45f, 0.1f));
			m_pThunder1->setPosition( CCPoint(m_fHeroLocateX, m_fHeroLocateY) );
			CCAnimate* pAction1 = CCAnimate::create( common::CreateAnimationFrame(
				"Animation/skill/1/huo/a", 2, CCRect(0,0,319,548), 0.1f) );
			m_pThunder1->runAction( pAction1 );
			m_pCFrontgroud->GetTileMap()->addChild(m_pThunder1, 4);

			scheduleOnce(schedule_selector(CGameControler::SkillThunderSecond), 0.3f);
		}
		break;
	case 1:
		{
			AppDelegate::AudioPlayEffect("MS4/skillstart.mp3");
			m_pThunder2 = common::CreateAnimation("Animation/skill/2/b1.png", CCPoint(m_fHeroLocateX, m_fHeroLocateY), 
				CCPoint(0.5f, 0.2f), "Animation/skill/2/b", 9, CCRect(0, 0, 232, 266), false, false, 0.05f);
			m_pCFrontgroud->GetTileMap()->addChild(m_pThunder2, 3);

			scheduleOnce(schedule_selector(CGameControler::SkillPlantSecond), 0.5f);
		}
		break;
	case 2:
		{
			m_pHero->GetArmature()->getAnimation()->playByIndex(20, 0, 40, 0);
			scheduleOnce(schedule_selector(CGameControler::SkillBotSecond), 0.5f);
		}
		break;
	}
	return true;
}

void CGameControler::SkillBotSecond( float _t )
{
	m_iShakeTime	= 150;
	m_iShakeLocate	= 10;
	m_pHero->m_unLastAction = 21;
	m_pHero->GetArmature()->getAnimation()->playByIndex(20, 0, 40, 0);
	m_pThunder2 = common::CreateAnimation("Animation/skill/4/1.png", CCPoint(m_fHeroLocateX+400-m_fRelativeX, m_fHeroLocateY), 
		CCPoint(0.5f, 0.2f), "Animation/skill/4/", 10, CCRect(0, 0, 800, 480), false, false, 0.1f);
	m_pCFrontgroud->GetTileMap()->addChild(m_pThunder2, 4);
	scheduleOnce(schedule_selector(CGameControler::ShillBotOver), 1.1f);
}

void CGameControler::ShillBotThird( float _t )
{
	m_pHero->GetArmature()->getAnimation()->playByIndex(20, 0, 40, 0);
	m_pThunder2 = common::CreateAnimation("Animation/skill/4/1.png", CCPoint(m_fHeroLocateX, m_fHeroLocateY), 
		CCPoint(0.5f, 0.2f), "Animation/skill/4/", 10, CCRect(0, 0, 800, 480), false, false, 0.15f);
	m_pCFrontgroud->GetTileMap()->addChild(m_pThunder2, 4);
	scheduleOnce(schedule_selector(CGameControler::ShillBotOver), 1.6f);
}

void CGameControler::ShillBotOver( float _t )
{
	if ( m_pRope[0] )
	{
		m_pRope[0]->stopAllActions();
		CCPoint point = m_pRope[0]->getPosition();
		CCNode* pNode = m_pRope[0]->getParent();
		CCNode* removeNode = m_pRope[0];
		m_pRope[0] = CCSprite::create("Animation/skill/rope1/1.png");
		m_pRope[0]->setPosition(point);
		pNode->addChild(m_pRope[0], 1);
		removeNode->removeFromParentAndCleanup(true);
	}
	if ( m_pThunder2 )
	{
		m_pThunder2->removeFromParentAndCleanup(true);
		m_pThunder2 = NULL;
	}

	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			if ( !(*it)->alive || ((*it)->type == 4 && (*it)->bRock) )
				continue;
			if ( !(*it)->pSprite )
				continue;
			if ( !SetVisible( (*it)->pSprite, (*it)->x, (*it)->y ) )
				continue;
			//if ( (*it)->type == 5 || (*it)->type == 8 || (*it)->type == 12 )
			//	continue;

			MonsterBeAttack(*it, d_iThunderDamage, 2);
			if ( !m_vecMonsterArray.size() )
			{
				break;
			}
		}
	}

	m_iSkill = 0;
	if ( m_iSkillCanUse > 0 )
	{
		m_iSkillCanUse --;
		m_iStateSkill[0] = 100;
	}
	else
		m_iStateSkill[0] = 0;
	m_bSkillPause = false;
	m_pHero->ResumeHero();
	if ( m_pFireSprite )
		m_pFireSprite->resumeSchedulerAndActions();

	CCSprite* pSprite = CCSprite::create("tu4/skill1.png");
	m_pSkillBar[0] = CCProgressTimer::create(pSprite);
	m_pSkillBar[0]->setAnchorPoint( ccp(0,0) );
	m_pSkillBar[0]->setPosition( ccp(0,0) );
	m_pSkillBar[0]->setType(kCCProgressTimerTypeBar);//
	m_pSkillBar[0]->setMidpoint(ccp(0.5, 0));
	m_pSkillBar[0]->setBarChangeRate(ccp(0, 1));
	m_pSkillBar[0]->setPercentage(0); 
	getChildByTag(28)->addChild(m_pSkillBar[0]);
	SkillFullCheck();
}

void CGameControler::SkillThunderSecond( float _t )
{
	m_iShakeTime	= 150;
	m_iShakeLocate	= 10;
	AppDelegate::AudioPlayEffect("MS4/skillcontinue.mp3");
	
	if ( m_pThunder1 )
	{
		m_pThunder1->stopAllActions();
		CCAnimate* pAction = CCAnimate::create( common::CreateAnimationFrame(
			"Animation/skill/1/huo/", 3, CCRect(0,0,319,548), 0.1f) );
		m_pThunder1->runAction( CCRepeatForever::create(pAction) );
	}

	m_pSkillMan = CCSprite::create("Animation/skill/1/ren1.png");
	if ( m_pSkillMan )
	{
		m_pSkillMan->setScaleX(m_fHeroToward);
		m_pSkillMan->setPosition( CCPoint(m_fHeroLocateX, m_fHeroLocateY) );
		m_pSkillMan->setAnchorPoint( ccp(0.55f, 0.15f) );
		m_pCFrontgroud->GetTileMap()->addChild(m_pSkillMan, 4);
		CCAnimate* pAction1 = CCAnimate::create( common::CreateAnimationFrame(
			"Animation/skill/1/ren", 4, CCRect(0,0,110,128), 0.1f) );
		m_pSkillMan->runAction( pAction1 );
	}

	m_pHero->ResumeMonster();
	m_pArrayThunder = CCArray::create();
	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			if ( !(*it)->alive || ((*it)->type == 4 && (*it)->bRock) )
				continue;
			if ( !(*it)->pSprite )
				continue;
			if ( !SetVisible( (*it)->pSprite, (*it)->x, (*it)->y ) )
				continue;

			CCSprite* pSprite = common::CreateAnimation("Animation/skill/1/dian1.png", CCPoint((*it)->x, (*it)->y), CCPoint(0.5, 0),
				"Animation/skill/1/dian", 3, CCRect(0, 0, d_sizeThunder1), true );
			(*it)->pSprite->getParent()->addChild(pSprite, (*it)->pSprite->getZOrder());
			m_pArrayThunder->addObject(pSprite);

			if ( (*it)->type == 5 || (*it)->type == 8 || (*it)->type == 12 )
				continue;
			if ( (*it)->secondLife )
				(*it)->pSprite->getAnimation()->playByIndex(g_iMonsterHoldSecond[(*it)->type], 0, 10, 1);
			else if ( g_iMonsterHold[(*it)->type] > 0 )
				(*it)->pSprite->getAnimation()->playByIndex(g_iMonsterHold[(*it)->type], 0, 10, 1);
		}
	}
	m_pArrayThunder->retain();

	scheduleOnce(schedule_selector(CGameControler::SkillThunderThird), 0.4f);
}

void CGameControler::SkillThunderThird( float _t )
{
	if ( m_pSkillMan )
	{
		m_pSkillMan->stopAllActions();
		CCAnimate* pAction = CCAnimate::create( common::CreateAnimationFrame(
			"Animation/skill/1/", 2, CCRect(0,0,110,128), 0.1f) );
		m_pSkillMan->runAction( CCRepeatForever::create(pAction) );
	}

	scheduleOnce(schedule_selector(CGameControler::SkillThunderOver), 2.0f);
}

void CGameControler::SkillThunderOver( float _t )
{
	if ( m_pThunder1 )
	{
		m_pThunder1->stopAllActions();
		CCAnimate* pAction1 = CCAnimate::create( common::CreateAnimationFrame(
			"Animation/skill/1/huo/b", 3, CCRect(0,0,319,548), 0.1f) );
		m_pThunder1->runAction( CCSequence::create(pAction1, CCCallFuncN::create(
			this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL) );
		m_pThunder1 = NULL;
	}

	if ( m_pRope[0] )
	{
		m_pRope[0]->stopAllActions();
		CCPoint point = m_pRope[0]->getPosition();
		CCNode* pNode = m_pRope[0]->getParent();
		CCNode* removeNode = m_pRope[0];
		m_pRope[0] = CCSprite::create("Animation/skill/rope1/1.png");
		m_pRope[0]->setPosition(point);
		pNode->addChild(m_pRope[0], 1);
		removeNode->removeFromParentAndCleanup(true);
	}

	if ( m_pSkillMan )
	{
		m_pSkillMan->removeFromParentAndCleanup(true);
		m_pSkillMan = NULL;
	}
	CCObject* pObject = NULL;
	CCARRAY_FOREACH( m_pArrayThunder, pObject )
		((CCSprite*)pObject)->removeFromParentAndCleanup(true);
	if ( m_pArrayThunder )
	{
		m_pArrayThunder->removeAllObjects();
		m_pArrayThunder->release();
		m_pArrayThunder = NULL;
	}

	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			if ( !(*it)->alive || ((*it)->type == 4 && (*it)->bRock) )
				continue;
			if ( !(*it)->pSprite )
				continue;
			if ( !SetVisible( (*it)->pSprite, (*it)->x, (*it)->y ) )
				continue;
			//if ( (*it)->type == 5 || (*it)->type == 8 || (*it)->type == 12 )
			//	continue;

			MonsterBeAttack(*it, d_iThunderDamage, 1);
			if ( !m_vecMonsterArray.size() )
			{
				break;
			}
		}
	}

	m_iSkill = 0;
	if ( m_iSkillCanUse > 0 )
	{
		m_iSkillCanUse --;
		m_iStateSkill[0] = 100;
	}
	else
		m_iStateSkill[0] = 0;
	m_bSkillPause = false;
	m_pHero->ResumeHero();
	if ( m_pFireSprite )
		m_pFireSprite->resumeSchedulerAndActions();

	CCSprite* pSprite = CCSprite::create("tu4/skill1.png");
	m_pSkillBar[0] = CCProgressTimer::create(pSprite);
	m_pSkillBar[0]->setAnchorPoint( ccp(0,0) );
	m_pSkillBar[0]->setPosition( ccp(0,0) );
	m_pSkillBar[0]->setType(kCCProgressTimerTypeBar);//
	m_pSkillBar[0]->setMidpoint(ccp(0.5, 0));
	m_pSkillBar[0]->setBarChangeRate(ccp(0, 1));
	m_pSkillBar[0]->setPercentage(m_iStateSkill[0]); 
	getChildByTag(28)->addChild(m_pSkillBar[0]);
	SkillFullCheck();
}

void CGameControler::SkillPlantSecond( float _t )
{
	m_iShakeTime	= 150;
	m_iShakeLocate	= 10;
	AppDelegate::AudioPlayEffect("MS4/skillcontinue.mp3");

	if ( m_pThunder2 )
	{
		m_pThunder2->stopAllActions();
		CCAnimate* pAction = CCAnimate::create( common::CreateAnimationFrame(
			"Animation/skill/2/c", 5, CCRect(0,0,232,266), 0.1f) );
		m_pThunder2->runAction( CCRepeatForever::create(pAction) );
	}

	m_pThunder1 = common::CreateAnimation("Animation/skill/2/a1.png", CCPoint(m_fHeroLocateX, m_fHeroLocateY), CCPoint(0.5f, 0.05f),
		"Animation/skill/2/a", 3, CCRect(0,0,194,480), true);
	m_pCFrontgroud->GetTileMap()->addChild(m_pThunder1, 4);

	m_pSkillMan = CCSprite::create("Animation/skill/2/ren1.png");
	m_pSkillMan->setScaleX(m_fHeroToward);
	m_pSkillMan->setPosition( CCPoint(m_fHeroLocateX, m_fHeroLocateY) );
	m_pSkillMan->setAnchorPoint( ccp(0.55f, 0.15f) );
	m_pCFrontgroud->GetTileMap()->addChild(m_pSkillMan, 4);
	CCAnimate* pAction1 = CCAnimate::create( common::CreateAnimationFrame(
		"Animation/skill/2/ren", 2, CCRect(0,0,101,118), 0.1f, true));
	m_pSkillMan->runAction( pAction1 );

	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			if ( !(*it)->alive || ((*it)->type == 4 && (*it)->bRock) )
				continue;
			if ( !(*it)->pSprite )
				continue;
			if ( !SetVisible( (*it)->pSprite, (*it)->x, (*it)->y ) )
				continue;

			(*it)->pSprite->getAnimation()->pause();
			CCSprite* pSprite = common::CreateAnimation("Animation/skill/2/b1.png", CCPoint((*it)->x, (*it)->y), 
				CCPoint(0.5f, 0.2f), "Animation/skill/2/b", 9, CCRect(0, 0, 232, 266), false, false, 0.1f, 
				CCCallFuncN::create(this, callfuncN_selector(CGameControler::SkillPlantCallback)));
			pSprite->setScale(0.7f);
			(*it)->pSprite->getParent()->addChild(pSprite, (*it)->pSprite->getZOrder());
		}
	}

	scheduleOnce(schedule_selector(CGameControler::SkillPlantThird), 1.2f);
}

void CGameControler::SkillPlantThird( float _t )
{
	if ( m_pRope[0] )
	{
		m_pRope[0]->stopAllActions();
		CCPoint point = m_pRope[0]->getPosition();
		CCNode* pNode = m_pRope[0]->getParent();
		CCNode* removeNode = m_pRope[0];
		m_pRope[0] = CCSprite::create("Animation/skill/rope1/1.png");
		m_pRope[0]->setPosition(point);
		pNode->addChild(m_pRope[0], 1);
		removeNode->removeFromParentAndCleanup(true);
	}

	m_pThunder2->stopAllActions();
	m_pThunder2->removeFromParentAndCleanup(true);
	m_pThunder1->stopAllActions();
	m_pThunder1->removeFromParentAndCleanup(true);
	m_pSkillMan->stopAllActions();
	m_pSkillMan->removeFromParentAndCleanup(true);

	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			if ( !(*it)->alive || ((*it)->type == 4 && (*it)->bRock) )
				continue;
			if ( !(*it)->pSprite )
				continue;
			if ( !SetVisible( (*it)->pSprite, (*it)->x, (*it)->y ) )
				continue;

			MonsterBeAttack(*it, d_iThunderDamage, 1);
			if ( !m_vecMonsterArray.size() )
			{
				break;
			}
		}
	}

	m_iSkill = 0;
	if ( m_iSkillCanUse > 0 )
	{
		m_iSkillCanUse --;
		m_iStateSkill[0] = 100;
	}
	else
		m_iStateSkill[0] = 0;
	m_bSkillPause = false;
	m_pHero->ResumeHero();
	if ( m_pFireSprite )
		m_pFireSprite->resumeSchedulerAndActions();

	CCSprite* pSprite = CCSprite::create("tu4/skill1.png");
	m_pSkillBar[0] = CCProgressTimer::create(pSprite);
	m_pSkillBar[0]->setAnchorPoint( ccp(0,0) );
	m_pSkillBar[0]->setPosition( ccp(0,0) );
	m_pSkillBar[0]->setType(kCCProgressTimerTypeBar);//
	m_pSkillBar[0]->setMidpoint(ccp(0.5, 0));
	m_pSkillBar[0]->setBarChangeRate(ccp(0, 1));
	m_pSkillBar[0]->setPercentage(0); 
	getChildByTag(28)->addChild(m_pSkillBar[0]);
	SkillFullCheck();
}

void CGameControler::SkillPlantCallback( cocos2d::CCNode* _pNode )
{
	CCSprite* pSprite = (CCSprite*)_pNode;
	pSprite->stopAllActions();
	
	CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("Animation/skill/2/d", 9, CCRect(0,0,126,206),
		0.05f));
	CCCallFuncN* pCallback = CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf));
	pSprite->setScale(1.4f);
	pSprite->runAction(CCSequence::create(pAction, pCallback, NULL));
	pSprite->setZOrder(pSprite->getZOrder()+2);
}

void CGameControler::SkillPlantOver( float _t )
{

	//CCARRAY_FOREACH( m_pArrayThunder, pObject )
	//{
	//	((CCSprite*)pObject)->removeFromParent();
	//	break;
	//}
	//((CCSprite*)(*m_pArrayThunder->data->arr))->removeFromParent();
	//m_pArrayThunder->removeObjectAtIndex(0);

	m_pRope[1]->stopAllActions();
	CCPoint point = m_pRope[1]->getPosition();
	CCNode* pNode = m_pRope[1]->getParent();
	m_pRope[1]->removeFromParentAndCleanup(true);
	m_pRope[1] = CCSprite::create("Animation/skill/rope2/1.png");
	m_pRope[1]->setPosition(point);
	pNode->addChild(m_pRope[1], 1);

	m_pSkillMan->removeFromParentAndCleanup(true);
	m_pSkillMan = NULL;

	m_iSkill = 0;
	m_bSkillPause = false;
	m_iPlantTime = 500;
	m_pHero->ResumeAction();

	CCSprite* pSprite = CCSprite::create("tu4/skill2.png");
	m_pSkillBar[1] = CCProgressTimer::create(pSprite);
	m_pSkillBar[1]->setAnchorPoint( ccp(0,0) );
	m_pSkillBar[1]->setPosition( ccp(0,0) );
	m_pSkillBar[1]->setType(kCCProgressTimerTypeBar);//
	m_pSkillBar[1]->setMidpoint(ccp(0.5, 0));
	m_pSkillBar[1]->setBarChangeRate(ccp(0, 1));
	m_pSkillBar[1]->setPercentage(0); 
	getChildByTag(29)->addChild(m_pSkillBar[1]);
}

void CGameControler::SkillTimer()
{
	if ( m_iPlantTime > 0 )
	{
		m_iPlantTime --;
		if ( !m_iPlantTime )
		{
			CCObject* pObject = NULL;
			CCARRAY_FOREACH( m_pArrayThunder, pObject )
				((CCSprite*)pObject)->removeFromParentAndCleanup(true);
			//m_pArrayThunder->release();

			//m_pArrayThunder = CCArray::create();
			for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
				it != m_vecMonsterArray.end(); it++ )
			{
				if ( !(*it)->alive || !(*it)->bPlant )
					continue;
				if ( !(*it)->pSprite )
					continue;
				if ( (*it)->type == 12 )
					continue;

				(*it)->pSprite->getAnimation()->resume();
				(*it)->bPlant = false;
				CCSprite* pSprite = common::CreateAnimation("Animation/skill/2/suidiao/1.png", CCPoint((*it)->x, (*it)->y), 
					CCPoint(0.47f, 0.20f), "Animation/skill/2/suidiao/", 6, CCRect(0, 0, d_sizePlant3), false, false, 0.1f,
					CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
				m_pCFrontgroud->GetTileMap()->addChild(pSprite, 5);
				//m_pArrayThunder->addObject(pSprite);
			}
			m_pArrayThunder->retain();

			scheduleOnce(schedule_selector(CGameControler::SkillPlantDisappear), 0.8f);
		}
	}
}

void CGameControler::SkillPlantDisappear( float _t )
{
	CCObject* pObject = NULL;
	CCARRAY_FOREACH( m_pArrayThunder, pObject )
		((CCSprite*)pObject)->removeFromParentAndCleanup(true);
	m_pArrayThunder->release();
	m_pArrayThunder = NULL;
}

void CGameControler::GetItemEffectOver()
{
	m_pGetItemEffect->removeFromParent();
	m_pGetItemEffect = NULL;
}

void CGameControler::BossShakeOver()
{
	m_pCFrontgroud->GetTileMap()->setPositionY(m_pCFrontgroud->GetTileMap()->getPositionY()+5.0f);
}
	
void CGameControler::VIPCheck()
{
	DIS_SELF(m_iVIPAttackedTime);
	if ( 0 == m_iVIPAttackedTime && 0 != m_iVIPLastAction )
	{
		m_pVIP->getAnimation()->playByIndex(0, 0, 30, 1);
		m_iVIPLastAction = 0;
	}
	if ( m_iVipTipTime > 0 )
	{
		m_iVipTipTime --;
		if ( m_iVipTipTime == 0 )
		{
			m_pVipTip1->setVisible(false);
			m_pVipTip2->setVisible(false);
		}
	}

	if ( m_iVIPHp != 100 )
	{
		m_pVipHPBoard->setVisible(true);
		if ( m_iVIPHp < 0 )
			m_iVIPHp = 0;
		m_pHpBar->setPercentage(m_iVIPHp);
	}

	float xl = 0;
	float xr = 0;
	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			float* pInt = NULL;
			if ( (*it)->x < m_fHeroLocateX )
				pInt = &xl;
			else
				pInt = &xr;
			float dist = fabs((*it)->x - m_fHeroLocateX);
			if ( *pInt == 0 || *pInt > dist )
				*pInt = dist;
		}
	}
	if ( xl > 400 )
	{
		float y = 140+ (xl-400.0)/1200.0*300.0f;
		if ( y > 300.0 )
			y = 300.0;
		m_pEnemyTipL->setVisible(true);
		m_pEnemyTipL->setPositionY(y);
	}
	else
		m_pEnemyTipL->setVisible(false);
	if ( xr > 400 )
	{
		float y = 140+ (xr-400.0)/1200.0*300.0f;
		if ( y > 300.0 )
			y = 300.0;
		m_pEnemyTipR->setVisible(true);
		m_pEnemyTipR->setPositionY(y);
	}
	else
		m_pEnemyTipR->setVisible(false);
}

void CGameControler::BossBox()
{
	int yDist = 120;
	int yInit = 480;
	if ( m_unLevel > 55 )
		yDist = 110;
	else if ( m_unLevel == 54 )
		yDist = 100;
	else if ( m_unLevel == 30 )
	{
		yDist = 3460;
		yInit = 3840;
	}
	else if ( m_unLevel/6 > 0 )
		yDist = 120;
	if ( m_vecBoxDownArray.size() )
	{
		while ( m_vecBoxDownArray.size() )
		{
			bool bStop		= true;
			std::vector<BoxDown*>::iterator it = m_vecBoxDownArray.begin();
			for (;it != m_vecBoxDownArray.end(); it++ )
			{
				(*it)->y -= 10.0f;
				(*it)->pSprite->setPositionY((*it)->y);
				if ( (*it)->y < yDist )
				{
					m_pCFrontgroud->m_pObjectArray[m_iBoxIndex].unKind	= 1;
					m_pCFrontgroud->m_pObjectArray[m_iBoxIndex].x		= (*it)->x;
					m_pCFrontgroud->m_pObjectArray[m_iBoxIndex].y		= (*it)->y;
					m_pCFrontgroud->m_pObjectArray[m_iBoxIndex].sPath	= g_sBoxPath[1];
					m_pCFrontgroud->m_pObjectArray[m_iBoxIndex].arg1	= 0;
					m_pCFrontgroud->m_pObjectArray[m_iBoxIndex].w		= 68;
					m_pCFrontgroud->m_pObjectArray[m_iBoxIndex].h		= 50;
					m_iBoxIndex++;
					m_pCFrontgroud->m_unObjectArrayNum = m_iBoxIndex;

					(*it)->pSprite->removeFromParent();
					delete *it;
					m_vecBoxDownArray.erase(it);
					bStop		= false;
					break;
				}
			}
			if ( bStop )
				break;
		}
	}
	if ( m_iWinTime <= BOSS_WIN_TIME && m_iWinTime >= BOSS_WIN_TIME-150 && 0 == m_iWinTime%10 )
	{
		if ( m_iWinTime == BOSS_WIN_TIME-120 )
		{
			m_pCFrontgroud->GetTileMap()->removeChildByTag(932);
			if ( m_iLevelType != 6 )
			{
				CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				ccNodeLoaderLibrary->registerCCNodeLoader("ccbBossBox", ccbBossBoxLoader::loader());
				cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
				CCNode* pNode = ccbReader->readNodeGraphFromFile("26.ccbi"/*, this*/);
				pNode->setPosition(ccp(-60,80));
				pNode->setScale(0.9f);
				((ccbBossBox*)pNode)->setAnimationManager(ccbReader->getAnimationManager(), this);
				ccbReader->release();
				addChild(pNode, 4, 50);
			}
		}
		BoxDown* p = new BoxDown;
		p->x = rand()%((int)800) + (m_iMapNum-1)*800;
		p->y = yInit;
		p->pSprite = CCSprite::create("Tile/xiangzi.png");
		p->pSprite->setPosition( ccp(p->x, p->y) );
		m_pCFrontgroud->GetTileMap()->addChild(p->pSprite);
		m_vecBoxDownArray.push_back(p);
	}
}

void CGameControler::EffectTime()
{
	DIS_SELF(m_timeBeAttackedEffect);
}

void CGameControler::CreateCombo()
{
	if ( m_iComboTime > 0 )
	{
		m_iComboTime--;
		if ( m_iComboTime == 0 )
		{
			m_iCombo = 0;
			m_bComboShow = false;
		}
	}
	if ( m_bComboShow )
		return;
	if ( m_iCombo > 1 )
	{
		m_bComboShow = true;
		CCSprite* pSprite = CCSprite::create("lianji.png");
		pSprite->setAnchorPoint(ccp(0.5f,0.5f));
		pSprite->setPosition( ccp(m_fHeroLocateX+30*m_fHeroToward, m_fHeroLocateY+150) );
		m_pHero->getParent()->addChild(pSprite, m_pHero->getZOrder()+3);
		int i = 0;
		if ( m_iCombo > 9 )
			i = 1;
		else if ( m_iCombo > 99 )
			i = 2;
		CCSprite* pSpriteWord = CCSprite::create("lianjiWord.png");
		pSpriteWord->setPosition(ccp(42, 80));
		pSpriteWord->setAnchorPoint(ccp(0,0.5f));
		pSprite->addChild(pSpriteWord);
		common::ShowNumber(pSprite, m_iCombo, 29, 40, 44/*-29*i*/, 80, "lianjinum.png");
		pSprite->getChildByTag(888)->setAnchorPoint(ccp(1.0f,0.5f));

		//pSprite->setScale(0.8f);
		//CCScaleTo* pAction2 = CCScaleTo::create(0.1f, 1.0f);
		pSprite->runAction( CCSequence::create(CCDelayTime::create(0.8f), /*pAction2, CCDelayTime::create(0.5f),*/
			CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL) );

		CCFadeIn* pAction = CCFadeIn::create(0.2f);
		CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 0.8f);
		pSpriteWord->setScale(1.2f);
		pSpriteWord->runAction( CCSequence::create(pAction, pAction1, NULL) );
		pAction = CCFadeIn::create(0.2f);
		pAction1 = CCScaleTo::create(0.1f, 0.8f);
		pSprite->getChildByTag(888)->setScale(1.2f);
		pSprite->getChildByTag(888)->runAction( CCSequence::create(pAction, pAction1, NULL) );
	}
}

void CGameControler::NeedKillShow()
{
	if ( m_iNeedKill )
	{
		if ( !getChildByTag(348) )
		{
			CCSprite* pSprite = CCSprite::create("Tile/wuran.png");
			pSprite->setAnchorPoint(ccp(0.5f,0.5f));
			pSprite->setScale(0.85f);
			pSprite->setPosition(ccp(100,256));
			addChild(pSprite, 10, 348);
			if ( m_iNeedKill > 9 )
				common::ShowNumber(pSprite, m_iNeedKill, 24, 28, 108, 27, "Tile/suzi2.png");
			else
				common::ShowNumber(pSprite, m_iNeedKill, 24, 28, 122, 27, "Tile/suzi2.png");
		}
		else
		{
			if ( m_iNeedKill > 9 )
				common::ShowNumber(getChildByTag(348), m_iNeedKill, 24, 28, 108, 27, "Tile/suzi2.png");
			else
				common::ShowNumber(getChildByTag(348), m_iNeedKill, 24, 28, 122, 27, "Tile/suzi2.png");
		}
	}
}

bool CGameControler::LimitInit()
{
	bool ok = false;
	m_iLimitDigit += 2;
	switch ( m_iLevelType )
	{
	case 1:
		if ( m_iLimitDigit >= m_iLimitTimeNeed )
			ok = true;
		break;
	case 3:
		if ( m_iLimitDigit >= (m_iMapNum*800-m_fHeroLocateX-5*d_fCantMove)/PIXELPERMETER )
		{
			m_iLimitDigit = (m_iMapNum*800-m_fHeroLocateX-5*d_fCantMove)/PIXELPERMETER;
			ok = true;
		}
		break;
	}
	int locatex = 0;
	if ( m_iLimitDigit < 10 )
		locatex = 78;
	else if ( m_iLimitDigit < 100 )
		locatex = 38;
	common::ShowNumber(m_pLimitBoard, m_iLimitDigit, 38, 65, locatex + d_LocateLimit, "tu4/limitN1.png");
	if ( ok )
	{
		NPCTalkStart();
		m_bLimitStart = false;
		m_pLimitBoard->setScale(1.5f);
		CCMoveTo* pAction1 = CCMoveTo::create(0.3f, ccp(230, 428));
		CCScaleTo* pAction2 = CCScaleTo::create(0.3f, 0.65f);
		m_pLimitBoard->runAction(pAction1);
		m_pLimitBoard->runAction(pAction2);
	}
	//	CCSprite* pTipBoard = CCSprite::create("Tile/tishi/meizi.png");
	//	pTipBoard->setAnchorPoint(ccp(0,0));
	//	pTipBoard->setPosition(ccp(450, 290));
	//	addChild(pTipBoard, 10, 197);

	//	{
	//		CCSprite* pSprite = common::CreateAnimation("Tile/tishi/1.png", ccp(50, 67), ccp(0.5f, 0.5f),
	//			"Tile/tishi/", 4, CCRect(0,0,32,32), true, false, 0.1f);
	//		pTipBoard->addChild(pSprite);
	//	}

	//	CCAnimate* pAction1 = NULL;
	//	CCSprite* pSprite = NULL;
	//	if ( m_iLevelType == 1 )
	//	{
	//		pSprite = CCSprite::create("Tile/tishi/1/1.png");
	//		pAction1 = CCAnimate::create(common::CreateAnimationFrame("Tile/tishi/1/", 2, CCRect(0,0,209,70), 3.0f));
	//	}
	//	else if ( m_iLevelType == 3 )
	//	{
	//		pSprite = CCSprite::create("Tile/tishi/3/1.png");
	//		pAction1 = CCAnimate::create(common::CreateAnimationFrame("Tile/tishi/3/", 2, CCRect(0,0,209,70), 3.0f));
	//	}
	//	pSprite->setAnchorPoint(ccp(0,0));
	//	pSprite->setPosition(ccp(17, 11));
	//	CCDelayTime* pAction2 = CCDelayTime::create(3.0f);
	//	CCCallFunc* pAction3 = CCCallFunc::create(this, callfunc_selector(CGameControler::TipOver));
	//	pSprite->runAction(CCSequence::create(pAction1, pAction2, pAction3, NULL));
	//	pTipBoard->addChild(pSprite);
	//}
	return ok;
}

void CGameControler::LimitUpdata()
{
	switch ( m_iLevelType )
	{
	case 1:
		m_iLimitDigit = m_iLimitTimeNeed - m_iGameMinute*60 - m_iGameSecond/MAINFRAME;
		break;
	case 3:
		m_iLimitDigit = (m_iMapNum*800-m_fHeroLocateX-5*d_fCantMove)/PIXELPERMETER;
		break;
	}
	if ( m_iLimitDigit < 0 )
		m_iLimitDigit = 0;
	int locatex = 0;
	if ( m_iLimitDigit < 10 )
		locatex = 78;
	else if ( m_iLimitDigit < 100 )
		locatex = 38;
	common::ShowNumber(m_pLimitBoard, m_iLimitDigit, 38, 65, locatex + d_LocateLimit, "tu4/limitN1.png");
}

void CGameControler::CallBackGas()
{
	m_pFireGas->removeFromParentAndCleanup(true);
	m_pFireGas = NULL;
}

void CGameControler::CallBackInPrison()
{
	m_pParitcleInPrison->removeFromParentAndCleanup(true);
	m_pParitcleInPrison = NULL;
}

void CGameControler::CreateInPrisonPlist()
{
	if ( !m_pParitcleInPrison )
	{
		m_pParitcleInPrison = CCParticleSystemQuad::create("inPrison.plist");
		m_pParitcleInPrison->setPosition(ccp(m_fHeroLocateX, m_fHeroLocateY+80));
		m_pParitcleInPrison->setAnchorPoint(ccp(0.5f,0.5f));
		m_pParitcleInPrison->setScale(2.0f);
		m_pCFrontgroud->GetTileMap()->addChild(m_pParitcleInPrison, 4);
		CCMoveBy* pAction = CCMoveBy::create(0.8f, ccp(0, 60));
		m_pParitcleInPrison->runAction(CCSequence::create(pAction, CCDelayTime::create(0.2f), 
			CCCallFunc::create(this, callfunc_selector(CGameControler::CallBackInPrison)), NULL));
	}
}

void CGameControler::GetMoney( int _num )
{
	m_iMoneyTo += _num*(1+AppDelegate::s_VIP);
	if ( m_structQuestion.type[4] )
	{
		m_structQuestion.num[4] -= _num*(1+AppDelegate::s_VIP);
		if ( m_structQuestion.num[4] <= 0 )
			m_structQuestion.complate[4] = 1;
	}
	//if ( m_iMoneyTo + m_iStateGold > 99999 )
	//	m_iMoneyTo = 99999 - m_iStateGold;
}

void CGameControler::OilcanBomb(cocos2d::CCNode* _pNode)
{
	AttackingObject* pAttackingObject = new AttackingObject;
	pAttackingObject->type	= eWT_Grenade;
	pAttackingObject->w		= 150;
	pAttackingObject->x		= _pNode->getPositionX();
	pAttackingObject->y		= _pNode->getPositionY();
	pAttackingObject->damage= 250;
	m_vecAttackingObjectArray.push_back(pAttackingObject);

	CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame(
		"Animation/oilcan/", 8, CCRect(0,0,115,181), 0.1f));
	_pNode->stopAllActions();
	_pNode->setAnchorPoint(ccp(0.5f, 0.1f));
	_pNode->runAction(CCSequence::create(pAction, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL));
}

void CGameControler::BossBoxStart(float _x, float _y)
{
	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			//if ( (*it)->type == 5 || (*it)->type == 8 || (*it)->type == 12 )
			//	continue;
			if ( (*it)->pSprite )
			{
				m_pHero->RemoveMonster((*it)->pSprite);
				(*it)->pSprite = NULL;
			}
			if ( (*it)->pSprite1 )
			{
				m_pHero->RemoveMonster((*it)->pSprite1);
				(*it)->pSprite1 = NULL;
			}
			if ( (*it)->pSprite2 )
			{
				m_pHero->RemoveMonster((*it)->pSprite2);
				(*it)->pSprite2 = NULL;
			}
			if ( (*it)->pSprite3 )
			{
				m_pHero->RemoveMonster((*it)->pSprite3);
				(*it)->pSprite3 = NULL;
			}
		}
		VECTORCLEAN(m_vecMonsterArray, MonsterInfomation);
	}
	if ( m_vecChopperArray.size() )
	{
		for ( std::vector<Chopper*>::iterator it = m_vecChopperArray.begin();
			it != m_vecChopperArray.end(); it++ )
		{
			if ( (*it)->type == -1 )
			{
				if ( (*it)->pSprite )
				{
					(*it)->pSprite->removeFromParent();
				}
			}
		}
		VECTORCLEAN(m_vecChopperArray, Chopper);
	}
	if ( 6 == m_iLevelType )
	{
		AppDelegate::s_Hero3Array[0] = 1;
		AppDelegate::s_Hero3 --;
	//	GuideTalk(14);
		m_iGuideNow = 14;
		m_iBossBoxNeed = 12;
	}

	m_iFireTime = 0;
	//if ( m_pFireSprite )
	//{
	//	m_pFireSprite->removeAllChildrenWithCleanup(true);
	//	m_pFireSprite = NULL;
	//}
	m_iUnderAttacked = 1;
	m_iAttackCoolTime = 0;
	m_ewtNowWeapon = eWT_Knife;
	((ccbControlMenu*)m_pMenuNode)->SetType(0);
	m_bBossWin = true;
	m_bBossBox = false;
	m_iWinTime = BOSS_WIN_TIME;
	if ( m_pCFrontgroud->m_pObjectArray )
		delete[] m_pCFrontgroud->m_pObjectArray;
	m_pCFrontgroud->m_pObjectArray = new MapObject[255];
	m_iBoxIndex = 0;
	CCParticleSystem* pParticle = CCParticleSystemQuad::create("Monster/BossReward.plist");
	pParticle->setPosition( ccp(_x, _y) );
	m_pCFrontgroud->GetTileMap()->addChild(pParticle, 2, 932);
	m_pBossBloodProgress->setPercentage(0);

	//CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	//ccNodeLoaderLibrary->registerCCNodeLoader("ccbBossBox", ccbBossBoxLoader::loader());
	//cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	//CCNode* pNode = ccbReader->readNodeGraphFromFile("26.ccbi"/*, this*/);
	//pNode->setPosition(ccp(0,0));
	//((ccbBossBox*)pNode)->setAnimationManager(ccbReader->getAnimationManager(), this);
	//ccbReader->release();
	//addChild(pNode, 4, 50);
}

void CGameControler::MonsterBeFire( cocos2d::CCNode* _pNode )
{
	_pNode->setVisible(false);
}

void CGameControler::KeyTip()
{
	m_pKeyTip->removeFromParentAndCleanup(true);
	m_pKeyTip = NULL;
}

void CGameControler::TipOver()
{
	getChildByTag(197)->removeFromParentAndCleanup(true);
	if ( getChildByTag(198) )
		getChildByTag(198)->removeFromParentAndCleanup(true);
}

void CGameControler::NPCTalk(float _t)
{
	AppDelegate::AudioPlayEffect("MS3/NPCTalk.mp3");
	m_iNPCTalkTime--;

	if ( m_iNPCTalkTime == 0 )
		unschedule(schedule_selector(CGameControler::NPCTalk));
	else
	{
		if ( getChildByTag(198) )
			getChildByTag(198)->removeFromParentAndCleanup(true);
		CCSprite* pNPC = common::CreateAnimation("guide/zhihui1.png", ccp(660, 250), ccp(0.0f, 0.0f),
			"guide/zhihui", 4, CCRect(0,0,138,158), false);
		addChild(pNPC, 11, 198);
	}
}

void CGameControler::NPCTalkStart()
{
	m_bCanPause = true;
	if ( m_unLevel != 2 && m_unLevel != 7 && m_unLevel != 9 )
		return;

	//CCSprite* pTipBoard = CCSprite::create("Tile/tishi/meizi.png");
	CCSprite* pTipBoard = CCSprite::create("guide/dikuang.png");
	pTipBoard->setAnchorPoint(ccp(0,0));
	pTipBoard->setPosition(ccp(400, 240));
	addChild(pTipBoard, 10, 197);

	CCSprite* pNPC = common::CreateAnimation("guide/zhihui1.png", ccp(660, 250), ccp(0.0f, 0.0f),
		"guide/zhihui", 4, CCRect(0,0,138,158), false);
	addChild(pNPC, 11, 198);

	{
		CCSprite* pSprite = common::CreateAnimation("Tile/tishi/1.png", ccp(50, 67), ccp(0.5f, 0.5f),
			"Tile/tishi/", 4, CCRect(0,0,32,32), true, false, 0.1f);
		pTipBoard->addChild(pSprite);
	}

	CCAnimate* pAction1 = NULL;
	CCSprite* pSprite = NULL;
	if ( m_iLevelType == 1 )
	{
		pSprite = CCSprite::create("Tile/tishi/1/1.png");
		pAction1 = CCAnimate::create(common::CreateAnimationFrame("Tile/tishi/1/", 2, CCRect(0,0,209,70), 3.0f));
		m_iNPCTalkTime = 2;
		schedule(schedule_selector(CGameControler::NPCTalk), 3.0f);
	}
	else if ( m_iLevelType == 2 )
	{
		pSprite = CCSprite::create("Tile/tishi/2/1.png");
		pAction1 = CCAnimate::create(common::CreateAnimationFrame("Tile/tishi/2/", 3, CCRect(0,0,209,70), 3.0f));
		m_iNPCTalkTime = 3;
		schedule(schedule_selector(CGameControler::NPCTalk), 3.0f);
		
	}
	else if ( m_iLevelType == 3 )
	{
		pSprite = CCSprite::create("Tile/tishi/3/1.png");
		pAction1 = CCAnimate::create(common::CreateAnimationFrame("Tile/tishi/3/", 2, CCRect(0,0,209,70), 3.0f));
		m_iNPCTalkTime = 2;
		schedule(schedule_selector(CGameControler::NPCTalk), 3.0f);
	}
	pSprite->setAnchorPoint(ccp(0,0));
	pSprite->setPosition(ccp(17, 11));
	CCDelayTime* pAction2 = CCDelayTime::create(3.0f);
	CCCallFunc* pAction3 = CCCallFunc::create(this, callfunc_selector(CGameControler::TipOver));
	pSprite->runAction(CCSequence::create(pAction1, pAction2, pAction3, NULL));
	pTipBoard->addChild(pSprite);
}

void CGameControler::PauseGame(int _type)
{
	if ( m_bSkillPause || !m_bCanPause || m_bBossWin || m_bPause || m_bExitGame || m_bSellOpen 
		|| ( getChildByTag(197) && m_iLevelType != 6 )  )
		return;
	if ( m_iGuideTalk == 12 && getChildByTag(196) )
	{
		m_iGuideAction = 13;
		GuideResume();
	}
	else if ( m_iLevelType == 6 && m_iGuideAction < 14 )
	{
		AppDelegate::AudioResume();
		return;
	}
	pauseSchedulerAndActions();
	m_pHero->PauseAction();
	m_bPause = true;
	m_bAttack = false;
	m_pMoveActionArray[0] = 0;
	m_pMoveActionArray[1] = 0;
	if ( !getChildByTag(1044) )
	{
		CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
		addChild(pCCLayerColor, 17, 1044);
		((ccbPauseMenu*)m_pPauseNode)->Appear(_type);
	}
}

void CGameControler::OnExitGame( cocos2d::CCObject* sender )
{
	if ( m_bExitGame )
		return;
	pauseSchedulerAndActions();
	m_pHero->PauseAction();
	m_bExitGame = true;
	//m_bAttack = false;
	//m_pMoveActionArray[0] = 0;
	//m_pMoveActionArray[1] = 0;
#ifdef Plat_91
	scheduleOnce(schedule_selector(ccbLoginMenu::ExitProtecter), 2.0f);
	PayService::exitGame();
#else
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	pCCLayerColor->setAnchorPoint(ccp(0,0));
	pCCLayerColor->setPosition(ccp(0,0));
	addChild(pCCLayerColor, 49, 1478);

	CCSprite* pBoard = CCSprite::create("tu7/exit/tuichu.png");
	pBoard->setPosition(ccp(400, 240));
	addChild(pBoard, 50, 1479);

	CCMenuItemImage* pYes = CCMenuItemImage::create( 
		"tu7/exit/shi.png", "tu7/exit/shi2.png", this, menu_selector(CGameControler::ExitYes));
	CCMenuItemImage* pNo = CCMenuItemImage::create( 
		"tu7/exit/fou.png", "tu7/exit/fou2.png", this, menu_selector(CGameControler::ExitNo));
	pYes->setPosition(ccp(127, 54));
	pNo->setPosition(ccp(301, 54));
	CCMenu* pMenu = CCMenu::create(pYes, pNo, NULL);
	pMenu->setPosition(ccp(0,0));
	pBoard->addChild(pMenu);
#endif
}

void CGameControler::ExitYes( cocos2d::CCObject* sender )
{
	AppDelegate::SaveAll();
#ifdef Plat_Telecom
	PayService::exitGame();
#endif
	exit(0);
}

void CGameControler::ExitNo( cocos2d::CCObject* sender )
{
	m_bExitGame = false;
	removeChildByTag(1478);
	removeChildByTag(1479);
	resumeSchedulerAndActions();
	m_pHero->ResumeAction();
}

void CGameControler::GuideTalk( int _index ,int _type)
{
	int yfix = 0;
	if ( _type == 1 )
	{
		yfix = -100;
	}
	if ( getChildByTag(197) )
		getChildByTag(197)->removeFromParentAndCleanup(true);
	m_iGuideTalk = _index;
	CCSprite* pTipBoard = CCSprite::create("guide/dikuang.png");
	pTipBoard->setAnchorPoint(ccp(0,0));
	pTipBoard->setPosition(ccp(400, 240+yfix));
	addChild(pTipBoard, 30, 197);

	if ( getChildByTag(198) )
		getChildByTag(198)->removeFromParentAndCleanup(true);
	CCSprite* pNPC = common::CreateAnimation("guide/zhihui1.png", ccp(660, 250+yfix), ccp(0.0f, 0.0f),
		"guide/zhihui", 4, CCRect(0,0,138,158), false);
	addChild(pNPC, 30, 198);

	CCSprite* pSprite = common::CreateAnimation("Tile/tishi/1.png", ccp(444, 302+yfix), ccp(0.5f, 0.5f),
		"Tile/tishi/", 4, CCRect(0,0,32,32), true, false, 0.1f);
	addChild(pSprite, 30, 195);

	if ( getChildByTag(199) )
		getChildByTag(199)->removeFromParentAndCleanup(true);
	char buffer[255];
	sprintf(buffer, "guide/%d.png", _index);
	CCSprite* pTip = CCSprite::create(buffer);
	pTip->setAnchorPoint(ccp(0,0));
	pTip->setPosition(ccp(420, 257+yfix));
	addChild(pTip, 30, 199);

	schedule(schedule_selector(CGameControler::GuideTalkLogic), 2.0f);

	if ( m_iGuideTalk == 3 )
	{
		CCSprite* pSprite = common::CreateAnimation("guide/san1.png", ccp(700, 120), ccp(0.5f, 0.8f),
			"guide/san", 6, CCRect(0,0,179,80), true);
		m_pCFrontgroud->GetTileMap()->addChild(pSprite, 30, 194);
		CCSprite* pSprite1 = common::CreateAnimation("guide/fangxiang1.png", ccp(700, 120), ccp(0.5f, 0.0f),
			"guide/fangxiang", 2, CCRect(0,0,51,99), true, false, 0.5f);
		m_pCFrontgroud->GetTileMap()->addChild(pSprite1, 30, 193);
	}
	else if ( m_iGuideTalk == 4 )
	{
		CCSprite* pSprite = common::CreateAnimation("guide/san1.png", ccp(1080, 210), ccp(0.5f, 0.8f),
			"guide/san", 6, CCRect(0,0,179,80), true);
		m_pCFrontgroud->GetTileMap()->addChild(pSprite, 30, 194);
		CCSprite* pSprite1 = common::CreateAnimation("guide/fangxiang1.png", ccp(1080, 210), ccp(0.5f, 0.0f),
			"guide/fangxiang", 2, CCRect(0,0,51,99), true, false, 0.5f);
		m_pCFrontgroud->GetTileMap()->addChild(pSprite1, 30, 193);
	}
	else if ( m_iGuideTalk == 5 )
	{
		CCSprite* pSprite = common::CreateAnimation("guide/san1.png", ccp(1250, 370), ccp(0.5f, 0.8f),
			"guide/san", 6, CCRect(0,0,179,80), true);
		m_pCFrontgroud->GetTileMap()->addChild(pSprite, 30, 194);
		CCSprite* pSprite1 = common::CreateAnimation("guide/fangxiang1.png", ccp(1250, 370), ccp(0.5f, 0.0f),
			"guide/fangxiang", 2, CCRect(0,0,51,99), true, false, 0.5f);
		m_pCFrontgroud->GetTileMap()->addChild(pSprite1, 30, 193);
	}
}

void CGameControler::GuideCheck()
{
	if ( 6 != m_iLevelType )
		return;
	if ( m_iGuideNow != m_iGuideAction )
	{
		switch( m_iGuideNow )
		{
		case 0:
			GuideTalk(1);
			m_iGuideNow = m_iGuideAction;
			break;
		case 1:
			if ( m_pCFrontgroud->GetTileMap()->getChildByTag(194) )
			{
				if ( m_fHeroLocateX >= 700 )
				{
					m_pCFrontgroud->GetTileMap()->getChildByTag(193)->removeFromParentAndCleanup(true);
					m_pCFrontgroud->GetTileMap()->getChildByTag(194)->removeFromParentAndCleanup(true);
					CCSprite* pSprite = common::CreateAnimation("guide/san1.png", ccp(920, 120), ccp(0.5f, 0.8f),
						"guide/san", 6, CCRect(0,0,179,80), true);
					m_pCFrontgroud->GetTileMap()->addChild(pSprite, 30, 191);
					CCSprite* pSprite1 = common::CreateAnimation("guide/fangxiang1.png", ccp(920, 120), ccp(0.5f, 0.0f),
						"guide/fangxiang", 2, CCRect(0,0,51,99), true, false, 0.5f);
					m_pCFrontgroud->GetTileMap()->addChild(pSprite1, 30, 192);
				}
			}
			if ( m_fHeroLocateX >= 920 )
			{
				if ( m_pCFrontgroud->GetTileMap()->getChildByTag(192) )
				{
					m_pCFrontgroud->GetTileMap()->getChildByTag(192)->removeFromParentAndCleanup(true);
					m_pCFrontgroud->GetTileMap()->getChildByTag(191)->removeFromParentAndCleanup(true);
				}
				GuideTalk(m_iGuideNow+3);
				m_iGuideBtn = 2;
				ButtonPressCheck();
				GuideShield(2);
				m_iGuideNow = m_iGuideAction;
			}
			break;
		case 2:
			if ( m_fHeroLocateX >= 1080 )
			{
				if ( m_pCFrontgroud->GetTileMap()->getChildByTag(194) )
				{
					m_pCFrontgroud->GetTileMap()->getChildByTag(193)->removeFromParentAndCleanup(true);
					m_pCFrontgroud->GetTileMap()->getChildByTag(194)->removeFromParentAndCleanup(true);
				}
				GuideTalk(m_iGuideNow+3);
				m_iGuideBtn = 2;
				ButtonPressCheck();
				GuideShield(2);
				m_iGuideNow = m_iGuideAction;
			}
			break;
		case 3:
			if ( m_pCFrontgroud->GetTileMap()->getChildByTag(194) )
			{
				if ( m_fHeroLocateX >= 1200 )
				{
					m_pCFrontgroud->GetTileMap()->getChildByTag(193)->removeFromParentAndCleanup(true);
					m_pCFrontgroud->GetTileMap()->getChildByTag(194)->removeFromParentAndCleanup(true);
				}
			}
			if ( m_fHeroLocateX >= m_iGuideMap-700 )
			{
				GuideTalk(m_iGuideNow+3);
				m_iGuideBtn = 3;
				ButtonPressCheck();
				GuideShield(3);
				m_iGuideNow = 4;
			}
			break;
		case 4:
			m_iGuideNow = 5;
			break;
		case 5:
			if ( !m_vecMonsterArray.size() && m_iGuideAction == 6 && m_fHeroLocateX >= m_iGuideMap-520 )
			{
				m_iGuideMap = guideMap[6]+800;
				m_iGuideNow = 6;
				m_iGuideAction = 7;
			}
			break;
		case 6:
			if ( m_iGuideAction == 7 && m_iGrenadeNum > 0 && m_bGuideWeaponGet )
			{
				m_iGuideMap = guideMap[7]+800;
				m_iGuideNow = 7;
				m_iGuideAction = 8;
			}
			break;
		case 7:
			if ( m_fHeroLocateX >= m_iGuideMap-490 && m_vecMonsterArray.size() )
			{
				GuideTalk(10);
				m_iGuideBtn = 5;
				ButtonPressCheck();
				GuideShield(5);
				m_iGuideNow = 8;
			}
			break;
		case 8:
			if ( !m_vecMonsterArray.size() )
			{
				m_iGuideMap = guideMap[8]+800;
				m_iGuideNow = 9;
				m_iGuideAction = 10;
			}
			break;
		case 9:
			if ( m_iStateSkill[0] >= 100 )
			{
				m_iGuideNow = 10;
				m_iGuideAction = 12;
			}
			break;
		case 10:
			if ( m_vecMonsterArray.size() > 7 )
			{
				GuideTalk(11);
				m_iGuideBtn = 6;
				ButtonPressCheck();
				GuideShield(6);
				m_iGuideNow = 11;
			}
			break;
		case 11:
			//if ( m_fHeroLocateX >= m_iGuideMap-400 )
			//{
			//	GuideTalk(12, 1);
			//	m_iGuideNow = 12;
			//}
			break;
		case 12:
			if ( m_iHealBox < 3 )
			{
				GuideTalk(13);
				m_iGuideNow = 13;
			}
			break;
		}
	}
	else if ( m_iGuideNow == 5 && m_iGuideAction == 5 )
	{
		if ( m_fHeroLocateX >= m_iGuideMap-800 )
		{
			GuideTalk(7);
			GuideShield(3);
		}
	}
}

void CGameControler::GuideTalkLogic( float _t )
{
	unschedule(schedule_selector(CGameControler::GuideTalkLogic));
	switch ( m_iGuideTalk )
	{
	case 9:
	case 15:
		if ( getChildByTag(195) )
			getChildByTag(195)->removeFromParentAndCleanup(true);
		if ( getChildByTag(197) )
			getChildByTag(197)->removeFromParentAndCleanup(true);
		if ( getChildByTag(198) )
			getChildByTag(198)->removeFromParentAndCleanup(true);
		if ( getChildByTag(199) )
			getChildByTag(199)->removeFromParentAndCleanup(true);
		break;
	case 1:
	case 2:
	case 14:
		if ( getChildByTag(195) )
			getChildByTag(195)->removeFromParentAndCleanup(true);
		if ( getChildByTag(197) )
			getChildByTag(197)->removeFromParentAndCleanup(true);
		if ( getChildByTag(198) )
			getChildByTag(198)->removeFromParentAndCleanup(true);
		if ( getChildByTag(199) )
			getChildByTag(199)->removeFromParentAndCleanup(true);
		GuideTalk(m_iGuideTalk+1);
		break;
	case 3:
		m_iGuideBtn = 1;
		ButtonPressCheck();
		GuideShield(1);
		break;
	case 4:
	case 5:
		//m_iGuideBtn = 2;
		//ButtonPressCheck();
		//GuideShield(2);
		break;
	case 6:
		//m_iGuideBtn = 3;
		//ButtonPressCheck();
		//GuideShield(3);
		break;
	case 8:
		{
			CCSprite* pTipBoard = common::CreateAnimation("guide/xia1.png", ccp(652,93), ccp(0.5f, 0.5f),
				"guide/xia", 2, CCRect(0,0,118,177), true, false, 0.3f);
			addChild(pTipBoard, 31, 193);
		}
		m_iGuideBtn = 4;
		m_pWeaponMark1->setVisible(true);
		ButtonPressCheck();
		GuideShield(4);
		break;
	case 10:
		//m_iGuideBtn = 5;
		//ButtonPressCheck();
		//GuideShield(5);
		break;
	case 11:
		//m_iGuideBtn = 6;
		//ButtonPressCheck();
		//GuideShield(6);
		break;
	case 12:
		{
			CCSprite* pTipBoard = common::CreateAnimation("guide/a1.png", ccp(670,380), ccp(0.5f, 0.5f),
				"guide/a", 2, CCRect(0,0,60,60), true, false, 0.3f);
			addChild(pTipBoard, 31, 193);
		}
		m_iGuideBtn = 7;
		ButtonPressCheck();
		GuideShield(7);
		break;
	case 13:
		{
			CCNodeLoaderLibrary* ccNodeLoaderLibrary6 = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			ccNodeLoaderLibrary6->registerCCNodeLoader("ccbStartGame", ccbStartGameLoader::loader());
			cocos2d::extension::CCBReader* ccbReader6 = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary6);
			ccbStartGame* pStartGame = (ccbStartGame*)ccbReader6->readNodeGraphFromFile("24.ccbi"/*, this*/);
			pStartGame->setAnimationManager(ccbReader6->getAnimationManager(), this);
			ccbReader6->release();
			m_pCFrontgroud->addChild(pStartGame, -1, 56);

			pStartGame->Appear(CCPoint(4910, 70), CCPoint(400, 300), 1);
		}
		break;
	}
}

void CGameControler::GuideShield( int _index )
{
	m_bGuidePause = true;
	pauseSchedulerAndActions();
	m_pHero->PauseAction();
	char buffer[255];
	sprintf(buffer, "guide/t%d.png", _index);
	CCSprite* pTipBoard = CCSprite::create(buffer);
	pTipBoard->setAnchorPoint(ccp(0,0));
	pTipBoard->setPosition(ccp(0,0));
	addChild(pTipBoard, 29, 196);
}

void CGameControler::GuideResume(cocos2d::CCObject *pSender)
{
	m_bGuidePause = false;
	removeChildByTag(196);
	if ( getChildByTag(193) )
		getChildByTag(193)->removeFromParentAndCleanup(true);
	if ( getChildByTag(194) )
		getChildByTag(194)->removeFromParentAndCleanup(true);
	if ( getChildByTag(195) )
		getChildByTag(195)->removeFromParentAndCleanup(true);
	if ( getChildByTag(197) )
		getChildByTag(197)->removeFromParentAndCleanup(true);
	if ( getChildByTag(198) )
		getChildByTag(198)->removeFromParentAndCleanup(true);
	if ( getChildByTag(199) )
		getChildByTag(199)->removeFromParentAndCleanup(true);
	resumeSchedulerAndActions();
	m_pHero->ResumeAction();
}

void CGameControler::GuideVIP()
{
	if ( getChildByTag(195) )
		getChildByTag(195)->removeFromParentAndCleanup(true);
	if ( getChildByTag(197) )
		getChildByTag(197)->removeFromParentAndCleanup(true);
	if ( getChildByTag(198) )
		getChildByTag(198)->removeFromParentAndCleanup(true);
	if ( getChildByTag(199) )
		getChildByTag(199)->removeFromParentAndCleanup(true);

#ifdef Plat_MM
	GuideBack();
	return;
#else
#ifdef Plat_Telecom_Demo
	GuideBuy();
	return;
#endif
	{
		m_bGuidePause = true;
		m_bSellOpen = true;
		m_bGuideBuyVip = true;
		CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
		//pCCLayerColor->setPosition(ccp(0,0));
		//SetScale(pCCLayerColor);
		addChild(pCCLayerColor, 29, 195);

		CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(CGameControler::GuideBack));
		pItem2->setPosition(ccp(673,411));
		pItem2->setScale(0.8f);
		CCMenu* pMenu1 = CCMenu::create(pItem2, NULL);
		pMenu1->setPosition(ccp(0,0));
		addChild(pMenu1, 31, 196);

		CCSprite* pSprite1 = CCSprite::create("sell/di2.png");
		pSprite1->setPosition(ccp(400, 240));
		addChild(pSprite1, 31, 199);
		CCSprite* pSprite2 = CCSprite::create("sell/guide/tu.png");
		pSprite2->setPosition(ccp(400, 240));
		addChild(pSprite2, 31, 198);

		CCMenuItem* pItem1 = CCMenuItemImage::create("sell/huode3-1.png", "sell/huode3-2.png", this, menu_selector(CGameControler::GuideBuy));
		pItem1->setPosition(ccp(400,64));
		CCMenu* pMenu = CCMenu::create(pItem1, NULL);
		pMenu->setPosition(ccp(0,0));
		addChild(pMenu, 32, 197);

		if(!getChildByTag(1001)){
			CCSprite* handelSprite = ccbShopMenu::createHandelCreator();
			addChild(handelSprite,80,1001);
		}
		getChildByTag(1001)->setVisible(true);
		getChildByTag(1001)->setPosition(ccp(400,35));
		m_bGuideBuyVipIndex = 0;
	}
#endif
}

void CGameControler::GuideBuy( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(1);
	}
	else
	{
#ifdef Plat_Telecom_Demo
		AppDelegate::m_Sell1		= 1;
		AppDelegate::s_WeaponOwn[6] = 1;
		AppDelegate::s_WeaponOwn[9] = 1;
		if ( AppDelegate::s_HeroType == 0 )
		{
			AppDelegate::s_WeaponUse[0] = 6;
			AppDelegate::s_WeaponUse[1] = 9;
		}
		AppDelegate::s_BulletNum[9] += 100;
		if ( AppDelegate::s_BulletNum[9] > 999 )
			AppDelegate::s_BulletNum[9] = 999;
		AppDelegate::s_Healbox = 3;
		AppDelegate::s_GrenadeNum = 10;
		AppDelegate::s_Money += 1000;
		AppDelegate::SaveStatus();
#endif
		GuideBack();
	}
}

void CGameControler::GuideBack( cocos2d::CCObject *pSender /*= NULL*/ )
{
	m_bSellOpen = false;
	m_bGuideBuyVip = false;
	getChildByTag(1001)->setVisible(false);
	m_iWeaponDamage[0] = 500;
	m_iWeaponDamage[1] = 200;
	m_iWeaponUse[0] = 6;
	m_iWeaponUse[1] = 9;
	m_sCutSound = "MS4/5Cut.mp3";
	m_iWeaponSpeed[1] = AppDelegate::WeaponSpeed(9);
	m_iFireIndex[0] = 1;
	m_iFireX[0] = g_fFireX[9];
	m_iFireY[0] = g_fFireY[9];
	m_iActionType1[0] = 19;
	m_iActionType2[0] = 21;
	for ( int i = 0; i < 7; i++ )
	{
		m_iKnifeAction[i] = 22 + i;
	}
	//m_ptBullet[0]->release();
	char bulletName[255];
	sprintf(bulletName, "bullet/%d/3.png", 9);
	//m_ptBullet[0] = CCTextureCache::sharedTextureCache()->addImage(bulletName);
	//m_ptBullet[0]->retain();
	m_sBulletPath = bulletName;
	if ( m_ewtNowWeapon != eWT_Knife && ( m_iHeroJumps < 2 || m_fHeroSpeedY < 2.0f ))
	{
		m_pHero->ChangeWeapon(m_iWeaponUse[0]);
		m_pHero->ChangeWeapon(m_iWeaponUse[1]);
		if ( m_iWeaponUse[1] == 12 && !m_bBossWin )
			m_iFireTime = 1;
	}
	{
		//GuideResume();

		m_bGuidePause = false;
		removeChildByTag(196);
		if ( getChildByTag(193) )
			getChildByTag(193)->removeFromParentAndCleanup(true);
		if ( getChildByTag(194) )
			getChildByTag(194)->removeFromParentAndCleanup(true);
		if ( getChildByTag(195) )
			getChildByTag(195)->removeFromParentAndCleanup(true);
		if ( getChildByTag(197) )
			getChildByTag(197)->removeFromParentAndCleanup(true);
		if ( getChildByTag(198) )
			getChildByTag(198)->removeFromParentAndCleanup(true);
		if ( getChildByTag(199) )
			getChildByTag(199)->removeFromParentAndCleanup(true);
		if ( getChildByTag(1001) )
			getChildByTag(1001)->removeFromParentAndCleanup(true);
	}
}

void CGameControler::Sell2()
{
#ifdef Plat_Telecom_Demo
	return;
#endif
	m_bSellOpen = true;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	//pCCLayerColor->setPosition(ccp(0,0));
	//SetScale(pCCLayerColor);
	addChild(pCCLayerColor, 29, 194);

	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(CGameControler::Sell2Back));
	pItem2->setPosition(ccp(680,411));
	pItem2->setScale(0.8f);
	CCMenu* pMenu1 = CCMenu::create(pItem2, NULL);
	pMenu1->setPosition(ccp(0,0));
	addChild(pMenu1, 31, 196);

	CCSprite* pSprite1 = CCSprite::create("sell/di2.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 31, 199);
	CCSprite* pSprite2 = CCSprite::create("sell/supply/tu.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 31, 198);

	CCMenuItem* pItem1 = CCMenuItemImage::create("sell/huode3-1.png", "sell/huode3-2.png", this, menu_selector(CGameControler::Sell2Resume));
	pItem1->setPosition(ccp(400,60));
	CCMenu* pMenu = CCMenu::create(pItem1, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 32, 197);
}

void CGameControler::Sell2Resume( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(2);
	}
	else
	{
		m_iHealBox = 3;
		m_iBulletNum[0] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
		if ( m_iBulletNum[0] > 999 )
			m_iBulletNum[0] = 999;
		m_iGrenadeNum = 10;
		m_iSkillCanUse = 5;
		if ( m_iStateSkill[0] < 100 )
		{
			m_iSkillCanUse = 4;
			m_iStateSkill[0] = 100;
		}
		//AppDelegate::m_Sell1 = 1;
		//AppDelegate::m_Sell2 = m_iSkillCanUse;
		//AppDelegate::s_GrenadeNum = 10;
		//AppDelegate::s_Healbox = 3;
		//AppDelegate::s_Money += 2000;
		//AppDelegate::SaveStatus();
		SkillFullCheck();
		ShowHealBox();
		ShowScore();
		//DeadCheck();
		Sell2Back();
	}
}

void CGameControler::Sell2Back( cocos2d::CCObject *pSender /*= NULL*/ )
{
	m_bSellOpen = false;
	isBuji = false;
	if ( getChildByTag(194) )
		getChildByTag(194)->removeFromParentAndCleanup(true);
	if ( getChildByTag(196) )
		getChildByTag(196)->removeFromParentAndCleanup(true);
	if ( getChildByTag(195) )
		getChildByTag(195)->removeFromParentAndCleanup(true);
	if ( getChildByTag(197) )
		getChildByTag(197)->removeFromParentAndCleanup(true);
	if ( getChildByTag(198) )
		getChildByTag(198)->removeFromParentAndCleanup(true);
	if ( getChildByTag(199) )
		getChildByTag(199)->removeFromParentAndCleanup(true);
	if (getChildByTag(1001))
		getChildByTag(1001)->removeFromParentAndCleanup(true);
}

void CGameControler::SkillFullCheck()
{
	if ( m_iStateSkill[0] >= 100 )
	{
		m_iSkillFullTime[0] = 0;
		m_iStateSkill[0] = 100;
#ifndef GameTypeC
		CCAnimate* pAction = CCAnimate::create( common::CreateAnimationFrame("Animation/skill/3/", 4, CCRect(0,0,76,93)) );
		CCSprite* pSprite = CCSprite::create("Animation/skill/3/1.png");
		pSprite->setPosition( ccp(383, 440) );
		pSprite->runAction( CCSequence::create(pAction, CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)), NULL) );
		addChild(pSprite, 11);
		if ( !getChildByTag(39) )
		{
			CCSprite* pSprite = common::CreateAnimation("tu4/skill/1.png", ccp(g_fButtonX[5],g_fButtonY[5]), ccp(0.5f,0.5f),
				"tu4/skill/", 6, CCRect(0,0,89,74), true);
			addChild(pSprite, 9, 39);
		}
#endif
		getChildByTag(25)->setVisible(true);
	}
}

void CGameControler::Sell3()
{
	m_bSellOpen = true;
#ifndef PayGOCTwo
	if ( AppDelegate::m_Sell1 == 0 )
	{
		CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
		//pCCLayerColor->setPosition(ccp(0,0));
		//SetScale(pCCLayerColor);
		addChild(pCCLayerColor, 29, 194);

		CCMenuItem* pItem1 = CCMenuItemImage::create("sell/huode2-1.png", "sell/huode2-2.png", this, menu_selector(CGameControler::Sell3Resume));
		CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(CGameControler::Sell3Back));
		pItem1->setPosition(ccp(725,195));
		pItem2->setPosition(ccp(728,437));
		CCMenu* pMenu = CCMenu::create(pItem1, pItem2, NULL);
		pMenu->setPosition(ccp(0,0));
		addChild(pMenu, 31, 197);

		CCSprite* pSprite1 = CCSprite::create("sell/di1.png");
		pSprite1->setPosition(ccp(400, 240));
		addChild(pSprite1, 31, 199);
		CCSprite* pSprite4 = CCSprite::create("sell/7/tu.png");
		pSprite4->setPosition(ccp(400, 240));
		addChild(pSprite4, 31, 198);
		CCSprite* pSprite2 = common::CreateAnimation("sell/vip/meizi1.png", ccp(245, 185), ccp(0.5f, 0.5f),
			"sell/vip/meizi", 2, CCRect(0,0,282,284), true);
		addChild(pSprite2, 31, 196);
		CCSprite* pSprite3 = CCSprite::create("sell/vip/dang.png");
		pSprite3->setPosition(ccp(283, 66));
		addChild(pSprite3, 31, 195);
	}
	else
#endif
#ifndef Pay_GOC_Can
	if ( AppDelegate::m_Sell1 )
#endif
	{
		CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
		//pCCLayerColor->setPosition(ccp(0,0));
		//SetScale(pCCLayerColor);
		addChild(pCCLayerColor, 29, 194);

		CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(CGameControler::Sell3Back));
		pItem2->setPosition(ccp(680,411));
		pItem2->setScale(0.8f);
		CCMenu* pMenu1 = CCMenu::create(pItem2, NULL);
		pMenu1->setPosition(ccp(0,0));
		addChild(pMenu1, 31, 196);

		CCSprite* pSprite1 = CCSprite::create("sell/di2.png");
		pSprite1->setPosition(ccp(400, 240));
		addChild(pSprite1, 31, 199);
		CCSprite* pSprite2 = CCSprite::create("sell/8/tu.png");
		pSprite2->setPosition(ccp(400, 240));
		addChild(pSprite2, 31, 198);

		CCMenuItem* pItem1 = CCMenuItemImage::create("sell/8/chongsheng.png", "sell/8/chongsheng2.png", this, menu_selector(CGameControler::Sell3Resume2));
		CCMenuItem* pItem3 = CCMenuItemImage::create("sell/8/fuhuo.png", "sell/8/fuhuo2.png", this, menu_selector(CGameControler::Sell3Resume1));
		pItem1->setPosition(ccp(250,94));
		pItem3->setPosition(ccp(560,94));
		CCMenu* pMenu = CCMenu::create(pItem1, pItem3, NULL);
		pMenu->setPosition(ccp(0,0));
		addChild(pMenu, 32, 197);

		isInRelife = true;
		relifeIndex = 0;
		if(!getChildByTag(1001)){
			CCSprite* handelSprite = ccbShopMenu::createHandelCreator();
			addChild(handelSprite,80,1001);
		}
		getChildByTag(1001)->setPosition(ccp(250,74));
		getChildByTag(1001)->setVisible(true);
	}
#ifndef Pay_GOC_Can
	else
	{
		Sell3Back();
	}
#endif
}

void CGameControler::Sell3Resume( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(3);
	}
	else
	{
		m_iBulletNum[0] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
		if ( m_iBulletNum[0] > 999 )
			m_iBulletNum[0] = 999;
		m_iGrenadeNum = 10;
		//AppDelegate::m_Sell1 = 1;
		//AppDelegate::s_Healbox = 3;
		//AppDelegate::s_GrenadeNum = 10;
		//AppDelegate::s_Money += 1000;
		//AppDelegate::SaveStatus();
		Relife();
		ShowHealBox();
		ShowScore();
		DeadCheck();
		Sell2Back();
	}
}

void CGameControler::Sell3Back( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_bBuyMedal )
		return;
	Sell2Back();
	isInRelife = false;
	AppDelegate::AudioPlayEffect("MS/Sound/EfHeroDead.mp3");
	//m_bPause = true;
	m_bDead = true;
	m_pHero->GetArmature()->setZOrder(10);
	switch (m_ewtNowWeapon)
	{
	case eWT_Knife:
		m_pHero->RunAction(m_iKnifeAction[5]);
		break;
	default:
		m_pHero->RunAction(13);
		break;
	}
	m_pHero->PauseMonster();
	scheduleOnce(schedule_selector(CGameControler::Lose), 0);
}

void CGameControler::Sell3Resume1( cocos2d::CCObject *pSender /*= NULL*/ )
{
#ifndef Plat_Telecom_Demo
	if ( pSender )
	{
		PayService::pay(10);
	}
	else
#endif
	{
#ifdef Plat_Telecom_Demo
		AppDelegate::s_GrenadeNum = 10;
		AppDelegate::s_Healbox = 3;
#endif
		m_iBulletNum[0] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
		if ( m_iBulletNum[0] > 999 )
			m_iBulletNum[0] = 999;
		m_iGrenadeNum = 10;
		m_iHealBox = 3;
		isInRelife = false;
		Relife();
		ShowHealBox();
		ShowScore();
		DeadCheck();
		Sell2Back();
	}
}

void CGameControler::Sell3Resume2( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( AppDelegate::s_Medal < 20 )
	{
#ifndef NoGetMoney
		GetMedal();
#endif
	}
	else
	{
		AppDelegate::s_Medal -= 20;
		AppDelegate::SaveMedal();
		isInRelife = false;
		Relife();
		ShowScore();
		DeadCheck();
		Sell2Back();
	}
}

void CGameControler::RelifeMovie( float _t )
{
	CallBackRemoveSelfRelife();
	m_pRelife = common::CreateAnimation("Animation/relife/1.png", ccp(m_fHeroLocateX, m_fHeroLocateY), ccp(0.5f, 0.13f),
		"Animation/relife/", 13, CCRect(0,0,325,480), false, false, 0.1f, 
		CCCallFunc::create(this, callfunc_selector(CGameControler::CallBackRemoveSelfRelife)));
	m_pHero->GetArmature()->getParent()->addChild(m_pRelife, m_pHero->GetArmature()->getZOrder()-1);

	CCSprite* pSprite = common::CreateAnimation("Animation/skill/4/1.png", CCPoint(m_fHeroLocateX+400-m_fRelativeX, m_fHeroLocateY), 
		CCPoint(0.5f, 0.2f), "Animation/skill/4/", 10, CCRect(0, 0, 800, 480), false, false, 0.1f,
		CCCallFuncN::create(this, callfuncN_selector(CGameControler::CallBackRemoveSelf)));
	m_pHero->GetArmature()->getParent()->addChild(pSprite, 4);
}

void CGameControler::Relife()
{
	m_iShakeTime = 20;
	m_iStateHP = 100;
	m_iHoldTime= 0;
	m_fHeroLocateX = m_fAliveLocateX;
	m_fHeroLocateY = m_fAliveLocateY;
	if ( !m_bBoss )
	{
		if ( m_fHeroLocateX < 400 )
		{
			m_fRelativeX = m_fHeroLocateX;
		}
		else if ( m_fHeroLocateX > m_iMapNum*800 - 400 )
		{
			m_fRelativeX = m_fHeroLocateX + 800 - m_iMapNum*800;
		}
		else
			m_fRelativeX = 400;
	}
	m_fHeroSpeedY = 0;

	scheduleOnce(schedule_selector(CGameControler::RelifeMovie), 0.1f);

	if ( m_vecMonsterArray.size() )
	{
		for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
			it != m_vecMonsterArray.end(); it++ )
		{
			if ( !(*it)->alive || ((*it)->type == 4 && (*it)->bRock) )
				continue;
			if ( !(*it)->pSprite )
				continue;
			if ( !SetVisible( (*it)->pSprite, (*it)->x, (*it)->y ) )
				continue;
			if ( (*it)->hp < 0 || (*it)->hp > 200000 )
				continue;

			MonsterBeAttack(*it, 500, 2);
			if ( !m_vecMonsterArray.size() )
			{
				break;
			}
		}
	}
}

void CGameControler::Sell4_1(cocos2d::CCObject *pSender)
{
	if ( m_bPause || m_bSellOpen || m_bWin || m_bDead || m_bBuyMedal )
		return;
	Sell4(pSender);
}

void CGameControler::Sell4(cocos2d::CCObject *pSender)
{
	if ( m_bSellOpen )
		return;
#ifdef Plat_Telecom_Demo
	if ( !m_bSellOpen )
		Sell4Resume();
	return;
#else
	if ( 6 == m_iLevelType )
	{
		Sell4Resume();
		return;
	}
#endif
	m_bSellOpen = true;
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	//pCCLayerColor->setPosition(ccp(0,0));
	//SetScale(pCCLayerColor);
	addChild(pCCLayerColor, 29, 194);

	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(CGameControler::Sell2Back));
	pItem2->setPosition(ccp(630,370));
	pItem2->setScale(0.8f);
	CCMenu* pMenu1 = CCMenu::create(pItem2, NULL);
	pMenu1->setPosition(ccp(0,0));
	addChild(pMenu1, 31, 196);

	CCSprite* pSprite1 = CCSprite::create("sell/di3.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 31, 199);
	CCSprite* pSprite2 = CCSprite::create("sell/10/tu.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 31, 198);

	CCMenuItem* pItem1 = CCMenuItemImage::create("sell/10/queding.png", "sell/10/queding2.png", this, menu_selector(CGameControler::Sell4Resume));
	pItem1->setPosition(ccp(400,100));
	CCMenu* pMenu = CCMenu::create(pItem1, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 32, 197);

	isBuji = true;
	if(!getChildByTag(1001)){
		CCSprite* handelSprite = ccbShopMenu::createHandelCreator();
		addChild(handelSprite,80,1001);
	}
	getChildByTag(1001)->setPosition(ccp(400,80));
	getChildByTag(1001)->setVisible(true);
	
}

void CGameControler::Sell4Resume( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(8);
	}
	else
	{
		//AppDelegate::s_GrenadeNum = 10;
		//AppDelegate::s_Healbox = 3;

		m_iStateHP = 100;
		m_iHealBox = 3;
		isBuji = false;
		m_iBulletNum[0] += 3*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
		if ( m_iBulletNum[0] > 999 )
			m_iBulletNum[0] = 999;
		m_iGrenadeNum = 10;
		ShowHealBox();
		ShowScore();
		DeadCheck();
		Sell2Back();
		((ccbPauseMenu*)m_pPauseNode)->OnBack();

	}
}

void CGameControler::CallBackRemoveSelfAction( cocos2d::CCNode* _pNode, void* data )
{
	_pNode->removeFromParentAndCleanup(true);
	*((CCSprite**)data) = NULL;
}

void CGameControler::Sell5()
{
#ifdef Plat_Telecom_Demo
	return;
#endif
	m_bSellOpen = true;
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	//pCCLayerColor->setPosition(ccp(0,0));
	//SetScale(pCCLayerColor);
	addChild(pCCLayerColor, 29, 194);

	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(CGameControler::Sell2Back));
	pItem2->setPosition(ccp(630,370));
	pItem2->setScale(0.8f);
	CCMenu* pMenu1 = CCMenu::create(pItem2, NULL);
	pMenu1->setPosition(ccp(0,0));
	addChild(pMenu1, 31, 196);

	CCSprite* pSprite1 = CCSprite::create("sell/di3.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 31, 199);
	CCSprite* pSprite2 = CCSprite::create("sell/6/tu.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 31, 198);

	CCMenuItem* pItem1 = CCMenuItemImage::create("sell/10/queding.png", "sell/10/queding2.png", this, menu_selector(CGameControler::Sell5Resume));
	pItem1->setPosition(ccp(400,100));
	CCMenu* pMenu = CCMenu::create(pItem1, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 32, 197);
}

void CGameControler::Sell5Resume( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(9);
	}
	else
	{
		m_iSkillCanUse = 5;
		if ( m_iStateSkill[0] < 100 )
		{
			m_iSkillCanUse = 4;
			m_iStateSkill[0] = 100;
		}
		//AppDelegate::m_Sell2 = m_iSkillCanUse;
		//AppDelegate::SaveStatus();
		SkillFullCheck();
		Sell2Back();
	}
}

void CGameControler::GuideGoToChoseHero(float _t)
{
	if ( 6 == m_iLevelType )
	{
		if ( AppDelegate::m_Sell1 )
			AppDelegate::s_GrenadeNum = 10;
		else
			AppDelegate::s_GrenadeNum = 0;
		AppDelegate::s_BulletNum[3] = 140;
		//AppDelegate::s_FirstLogin = 2;
		AppDelegate::s_FirstLogin = 6;
		AppDelegate::s_LevelOpen[0] = 1;
		AppDelegate::SaveGuide();
		AppDelegate::SaveStatus();
		//AppDelegate::ChangeScene( ccbChoseHero::CreateScene() );
		AppDelegate::ChangeScene( CFirstLogin::CreateScene());
		return;
	}
}

CGameControler* CGameControler::GetGameControler()
{
	return s_pGameControler;
}

void CGameControler::ShakeTime( float _t )
{
	m_bShakeStop = false;
	m_pHero->ResumeAction();
}

void CGameControler::SupplyCheck()
{
	if ( m_iLevelType == 6 )
		return;
	bool show1 = false;
	bool show2 = false;
	if ( m_iHealBox == 0 )
		show1 = true;
	if ( m_iBulletNum[0] < 100 )
		show2 = true;
	
	m_pSullpyHealbox->setVisible(show1);
	m_pSullpyBullet->setVisible(show2);
	m_pSullpyBtn->setVisible(show1||show2);
	m_pSullpyBoard->setVisible(show1||show2);
	m_pSullpyHealbox->setPosition(ccp(770-30*(int)show2, 281+20*(int)show2));
}

void CGameControler::GetMedal( cocos2d::CCObject* sender )
{
#ifdef NoGetMoney
	return;
#endif
	if ( m_bBuyMedal )
		return;
	m_bBuyMedal = true;
	m_bSellSure = false;
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	addChild(pCCLayerColor, 69, 1195);

	CCMenuItem* pItem = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(CGameControler::GetRemove));
	pItem->setPosition(ccp(723,351));
	CCMenu* pMenu = CCMenu::create(pItem, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 71, 1196);

	CCSprite* pSprite1 = CCSprite::create("sell/money/di.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 71, 1199);
	CCSprite* pSprite2 = CCSprite::create("sell/money/1.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 73, 1198);

	//CCMenuItem* pItem1 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(CGameControler::GetMedalBtn));
	//pItem1->setPosition(ccp(219,282));
	//pItem1->setTag(1);
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(CGameControler::GetMedalBtn));
	pItem2->setPosition(ccp(243,199));
	pItem2->setTag(2);
	CCMenuItem* pItem3 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(CGameControler::GetMedalBtn));
	pItem3->setPosition(ccp(556,199));
	pItem3->setTag(3);
	//CCMenuItem* pItem4 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMedalBtn));
	//pItem4->setPosition(ccp(283,96));
	//pItem4->setTag(4);
	//CCMenuItem* pItem5 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMedalBtn));
	//pItem5->setPosition(ccp(512,96));
	//pItem5->setTag(5);
	CCMenu* pMenu1 = CCMenu::create(/*pItem1, */pItem2, pItem3, /*pItem4, pItem5, */NULL);
	pMenu1->setPosition(ccp(0,0));
	addChild(pMenu1, 72, 1197);

	isInRecharge = true;
	rechargeIndex = 0;
	if(!getChildByTag(1001)){
		CCSprite* handelSprite = ccbShopMenu::createHandelCreator();
		addChild(handelSprite,80,1001);
	}
	getChildByTag(1001)->setPosition(ccp(243,199));
	getChildByTag(1001)->setVisible(true);
}

void CGameControler::GetMedalBtn( cocos2d::CCObject* _pNode )
{
	if ( m_bSellSure )
		return;
	m_bSellSure = true;
	isInRecharge = false;
	if ( _pNode )
	{
		int numAdd = 0;
		int numMid = 0;
		int index = 2+rechargeIndex;
		PayService::pay(11+index);
		getChildByTag(1001)->setPosition(ccp(560,74));
		//switch ( index )
		//{
		//case 1:
		//	numAdd = 108;
		//	numMid = 2;
		//	break;
		//case 2:
		//	numAdd = 648;
		//	numMid = 14;
		//	break;
		//case 3:
		//	numAdd = 3528;
		//	numMid = 49;
		//	break;
		//case 4:
		//	numAdd = 8019;
		//	numMid = 99;
		//	break;
		//case 5:
		//	numAdd = 18144;
		//	numMid = 168;
		//	break;
		//}
		//AppDelegate::s_Medal += numAdd;
		//AppDelegate::SaveMedal();
	}
}

void CGameControler::GetRemove(cocos2d::CCObject* _pNode)
{
	m_bBuyMedal = false;
	m_bSellSure = false;
	isInRecharge = false;
	for ( int i = 1195; i < 1200; i++ )
	{
		if ( getChildByTag(i) )
			removeChildByTag(i);
	}
	getChildByTag(1001)->setPosition(ccp(560,74));
}

void CGameControler::SwitchMap()
{
	if ( m_vecMonsterArray.size() )
	{
		//for ( std::vector<MonsterInfomation*>::iterator it = m_vecMonsterArray.begin();
		//	it != m_vecMonsterArray.end(); it++ )
		//{
		//	//if ( (*it)->type == 5 || (*it)->type == 8 || (*it)->type == 12 )
		//	//	continue;
		//	if ( (*it)->pSprite )
		//	{
		//		m_pHero->RemoveMonster((*it)->pSprite);
		//		(*it)->pSprite = NULL;
		//	}
		//	if ( (*it)->pSprite1 )
		//	{
		//		m_pHero->RemoveMonster((*it)->pSprite1);
		//		(*it)->pSprite1 = NULL;
		//	}
		//	if ( (*it)->pSprite2 )
		//	{
		//		m_pHero->RemoveMonster((*it)->pSprite2);
		//		(*it)->pSprite2 = NULL;
		//	}
		//	if ( (*it)->pSprite3 )
		//	{
		//		m_pHero->RemoveMonster((*it)->pSprite3);
		//		(*it)->pSprite3 = NULL;
		//	}
		//}
		VECTORCLEAN(m_vecMonsterArray, MonsterInfomation);
	}

	while ( m_vecItemObejctArray.size() )
	{
		std::vector<ItemObject*>::iterator it = m_vecItemObejctArray.begin();
		DELETEITERATORFROMVECTOR(m_vecItemObejctArray, it);
	}
	m_vecItemObejctArray.clear();

	while ( m_vecAttackingObjectArray.size() )
	{
		std::vector<AttackingObject*>::iterator it = m_vecAttackingObjectArray.begin();
		DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
	}
	m_vecAttackingObjectArray.clear();

	VECTORCLEAN(m_vecShellsArray, Shells);
	VECTORCLEAN(m_vecChopperArray, Chopper);

	m_pHero->ClearMonster(false);
	switch ( m_ewtNowWeapon )
	{
	case eWT_Knife:
		m_pHero->RunAction(m_iKnifeAction[0]);
		break;
	case eWT_Pistol:
	case eWT_M4A1:
		m_pHero->RunAction(1);
		break;
	}

	m_pHero->RemoveHero();
	
	{
		char buffer[255];
		sprintf(buffer, "Tile/%d_%djg.tmx", m_unLevel, m_iMapIndex);
		m_pCMidground->removeFromParent();
		m_pCMidground = CCTMXTiledMap::create(buffer);
		m_pCMidground->setPosition( ccp(0, 0) );
		//SetScale(m_pCMidground);
		m_pCFrontgroud->addChild(m_pCMidground, -1);
	}
	{
		char buffer[255];
		sprintf(buffer, "Tile/%d_%d.tmx", m_unLevel, m_iMapIndex);
		m_pCFrontgroud->ChangeMap(buffer);
		m_iMapNum = m_pCFrontgroud->GetMapNums();
		m_iGuideMap = m_iMapNum*d_fMapSizeX;
	}
	{
		m_pHero->CreateHero();
		m_pHero->ChangeWeapon(AppDelegate::s_WeaponUse[0]);
		m_pHero->ChangeWeapon(AppDelegate::s_WeaponUse[1]);
	}
	m_fHeroLocateX = 50;
	m_fRelativeX = 50;
	SetLocation();

	if ( m_iMapIndex+1 >= m_iMapMax )
	{
		if ( !(m_iLevelType == 1 ||
			m_iLevelType == 2 ||
			m_iLevelType == 3 ||
			m_unLevel%6 == 5) && m_unLevel < 49 && m_unLevel >= 0 )
		{
			m_pWinMark = CCSprite::create("Animation/winmark/1.png");
			m_pWinMark->setPosition(ccp(d_fMapSizeX*m_iMapNum-240, 300));
			m_pWinMark->setAnchorPoint(ccp(0.0f, 0.5f));
			CCRepeatForever* pAction1 = CCRepeatForever::create(CCAnimate::create(
				common::CreateAnimationFrame("Animation/winmark/", 2, CCRect(0,0,147,63))));
			CCRepeatForever* pAction2 = CCRepeatForever::create(CCSequence::create(
				CCMoveBy::create(0.2f,ccp(60.0,0)), CCMoveBy::create(1.0f,ccp(-60.0,0)), NULL));
			m_pWinMark->runAction(pAction1);
			m_pWinMark->runAction(pAction2);
			m_pCFrontgroud->GetTileMap()->addChild(m_pWinMark, 5);
			m_pWinMark->setVisible(false);
		}
	}

	CCMoveBy* pAction1 = CCMoveTo::create(1.0, ccp(-1200*Scale_X,0));
	m_pChangeMapColor->runAction(CCSequence::create(pAction1,NULL));

	CCMoveBy* pAction = CCMoveTo::create(1.0, ccp(-1600*Scale_X,0));
	CCCallFunc* pCallfunc = CCCallFunc::create(this, callfunc_selector(CGameControler::SwitchMapEnd));
	m_pChangeMapLayer->runAction(CCSequence::create(pAction, pCallfunc, NULL));
}

bool CGameControler::SwitchMapStart()
{
	if ( m_iMapChanged )
		return false;
	m_iMapChanged = true;
	m_iMapIndex++;
	if ( m_iMapIndex >= m_iMapMax )
		return false;
	m_bPause = true;

	m_pChangeMapColor = CCLayerColor::create(ccc4(0,0,0,255), 800, 480);
	SetScale(m_pChangeMapColor);
	m_pChangeMapColor->setAnchorPoint(ccp(0,0));
	m_pChangeMapColor->setPosition(ccp(800*Scale_X,0));
	m_pScene->addChild(m_pChangeMapColor, 50);
	CCMoveBy* pAction1 = CCMoveBy::create(1.0f, ccp(-800*Scale_X,0));
	m_pChangeMapColor->runAction(pAction1);

	m_pChangeMapLayer = CCSprite::create("Tile/xingde.png");
	SetScale(m_pChangeMapLayer);
	m_pChangeMapLayer->setAnchorPoint(ccp(0,0));
	m_pChangeMapLayer->setPosition(ccp(1600*Scale_X,0));
	m_pScene->addChild(m_pChangeMapLayer, 50);
	CCMoveBy* pAction = CCMoveBy::create(1.0f, ccp(-1200*Scale_X,0));
	CCCallFunc* pCallfunc = CCCallFunc::create(this, callfunc_selector(CGameControler::SwitchMap));
	m_pChangeMapLayer->runAction(CCSequence::create(pAction, CCDelayTime::create(0.1f), pCallfunc, NULL));
	return true;
}

void CGameControler::SwitchMapEnd()
{
	m_bOver = false;
	m_bOverMove = false;
	m_bPause = false;
	m_iMapChanged = false;
	m_pChangeMapColor->stopAllActions();
	m_pChangeMapColor->removeFromParentAndCleanup(true);
	m_pChangeMapLayer->removeFromParentAndCleanup(true);
}
