#ifndef _GAME_CONTROLER_H_
#define _GAME_CONTROLER_H_

#include "cocos2d.h"
#include "Background.h"
#include "Frontground.h"
#include "Flash.h"
#include "QuestionInfo.h"
//#include "CSizeLayer.h"

//////////////////
/****游戏数据****/
//////////////////
#define d_iMoneyS	5
#define d_iMoneyN	10
#define d_iMoneyL	50
#define d_iDropsDamage		10
#define d_iPoisonGasDamage	10
#define d_iGressDamage		10
#define d_iGressDist		50
#define d_iLandStayTime		15
#define d_iThunderDamage	3100

//////////////
/****主角****/
//////////////
#define d_fHeroSpeedX	3.8f
#define d_fHeroJump		20.0f
#define d_fHeroGravity	0.8f
#define d_fHeroCrashCX	0.0f
#define d_fHeroWidth	40.0f
#define d_fHeroHeight	88.0f
#define d_fHeroCenterY	0.0f
#define d_fScaleX		0.5f
#define d_fScaleY		0.7f
#define d_fHeroAnchorX	-0.0f
#define d_fHeroAnchorY	-18.0f

//////////////////
/****界面按钮****/
//////////////////
#define d_fStarMovieTime	0.1f
#define d_iStarW			139
#define	d_iStarH			135
#define d_fStarLocateX		300.0f
#define d_fStarLocateY		240.0f
#define d_fStarLocateX1		552.0f
#define d_fStarLocateY1		92.0f
#define d_fStarLocateX2		623.0f
#define d_fStarLocateY2		130.0f
#define d_fStarLocateX3		694.0f
#define d_fStarLocateY3		99.0f
#define d_iButtonSize		40
#define d_fBloodLocateX		107.0f
#define d_fBloodLocateY		71.0f
#define d_fBloodX			116.0f
#define d_fBloodY			71.0f
#define d_iActionNum		9
static const std::string g_sButtonPath[] =
{//按钮图片
	"tu4/zuo.png",		//左
	"tu4/you.png",		//右
	"tu4/gongji.png",	//打
	"tu4/tao.png",		//跳
	"tu4/huanqiang.png",//切
	"tu4/skillbtn1.png",	//技能
	"tu4/shoulei.png",	//雷
	"tu4/zhanting1.png",//暂停
	"tu4/an.png",//药瓶1
	//"tu4/an.png",//药瓶2
};
static const std::string g_sButtonPressPath[] =
{//按下图片
	"tu4/zuo2.png",		//左
	"tu4/you2.png",		//右
	"tu4/gongji2.png",	//打
	"tu4/tao2.png",		//跳
	"tu4/huanqiang2.png",//切
	"tu4/skillbtn2.png",	//技能
	"tu4/shoulei2.png",	//雷
	"tu4/zhanting1.png",//暂停
	"tu4/an.png",//药瓶1
	//"tu4/an.png",//药瓶2
};
static const float g_fButtonX[] =
{
	80,
	230,
	710,
	757,
	650,
	470,//520,
	570,
	757.5,
	404,
	//477,
};
static const float g_fButtonY[] =
{
	55,
	55,
	50,
	155,
	444,
	38,//33,
	38,
	437,
	446,
	//446,
};
static const float g_fButtonW[] =
{
	75.0,
	75.0,
	49.0,
	45.0,
	80.0,
	45.0,//45.0,
	45.0,
	45.0,
	40.0,
	//40.0,
};
static const float g_fButtonH[] =
{
	70.0,
	70.0,
	49.0,
	53.0,
	25.0,
	53,//53.0,
	53.0,
	52.0,
	40.0,
	//40.0,
};

//////////////////
/****************/
//////////////////
enum e_ActionType
{
	eAT_MoveLeft = 0,
	eAT_MoveRight,
	eAT_Attack,
	eAT_Jump,
	eAT_ChangeWeapon,
	eAT_SKILL,
	eAT_BOMB,
	eAT_PAUSE,
	eAT_SKILL1,
	eAT_SKILL2,
	eAT_NULL,
};

enum e_WeaponType
{
	eWT_Fire = -2,
	eWT_Grenade = -1,
	eWT_Knife /*= 2*/,
	eWT_Pistol,
	eWT_M4A1,
};

struct ItemObject
{
	cocos2d::CCSprite*	pSprite1;
	cocos2d::CCSprite*	pSprite2;
	float				x;
	float				y;
	int					time;
	int					type;
	int					num;

	ItemObject()
	{
		pSprite1	= NULL;
		pSprite2	= NULL;
		time		= 0;
		num			= 5;
	}
};

struct AttackingObject
{
	cocos2d::CCSprite*	pSprite;
	e_WeaponType		type;
	float				x;
	float				y;
	float				w;
	float				h;
	float				vx;
	float				vy;
	int					toward;
	float				speed;
	float				range;
	float				rotation;
	int					damage;
	int					time;
	int					life;
	int					index;
	int					isFirst;
	bool				wall;

	AttackingObject()
	{
		pSprite = NULL;
		life	= -1;
		isFirst	= 1;
		wall	= true;
	}
};

struct Shells
{
	cocos2d::CCSprite*	pSprite;
	float				x;
	float				y;
	float				vx;
	float				vy;
	float				rotate;
	int					life;

	Shells()
	{
		pSprite	= NULL;
		life	= 100;
	}
};

struct Chopper
{
	cocos2d::CCSprite*	pSprite;
	int					type;
	float				x;
	float				y;
	float				vx;
	float				vy;
	float				va;
	float				w;
	float				h;
	float				rotate;
	int					damage;
	int					time;
	int					clearTime;
	int					toward;
	bool				active;
	bool				bomb;
	bool				touchEarth;

	Chopper()
	{
		pSprite = NULL;
		time	= 16;
		rotate	= 0;
		active	= false;
		bomb	= false;
		touchEarth	= false;
	}
};

struct BoxDown
{
	cocos2d::CCSprite*	pSprite;
	float				x;
	float				y;
};

class CGameControler : public cocos2d::CCLayer
{
public:
	CGameControler();
	~CGameControler();
	CREATE_FUNC(CGameControler);
	virtual bool init();
	static CGameControler* NewGameControler(cocos2d::CCScene* _pScene, int _unLevel, int _difficult);
	static CGameControler* GetGameControler();
	void GameLoadingSecond();
	void GameStart();

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

	virtual void keyEnterReleased();
	virtual void keyArrowReleased(int arrow);
	virtual void keyNumReleased(int keyNum);
	void ButtonPressCheck();

	void AttackRun();
	void AttackCheck();
	void BossBox();
	void BossMovie(float _t);
	bool BeAttack(int _damage);
	void BossShakeOver();
	void BloodRemove();
	void BossBoxStart(float _x, float _y);
	void BulletAimMonster(cocos2d::CCNode* _pNode);
	bool CheckCrash(float _fHeroX, float& _fHeroY, float _w, float _h, float _cy, bool _bHero = true);
	bool CheckInWall(float _x, float _y, float _w, float _h);
	void CreateAttack();
	void CreateItem(float& _x, float& _y, int _type = -1, int _num = 5);
	void CallBackDropOver(cocos2d::CCNode* pNode, void* data);
	void CallBackBoxDestroy(cocos2d::CCNode* pNode, void* data);
	void CallBackGasOver(cocos2d::CCNode* pNode, void* data);
	void CallBackGas();
	void CallBackInPrison();
	void CallBackRemoveSelf(cocos2d::CCNode* _pNode);
	void CallBackRemoveSelf(cocos2d::CCNode* _pNode, void* data);
	void CallBackRemoveSelfAction(cocos2d::CCNode* _pNode, void* data);
	void CallBackRemoveSelfP(cocos2d::CCNode* _pNode, void* data);
	void CallBackRemoveSelf2(cocos2d::CCNode* _pNode);
	void CallBackRemoveSelfRelife();
	void CreateInPrisonPlist();
	void CreateCombo();
	void CheckToSkill();
	void DeadCheck();
	void EffectTime();
	void GrenadeCreate();
	void GrenadeRun();
	void GrenadeBomb();
	void GrenadeBomb1();
	void GrenadeOver();
	void GameOver(float _dt);
	void GetMoney(int _num);
	void GetItemEffectOver();
	void HeroAction();
	void HeroAim();
	void HealEffectOver1();
	void HealEffectOver2();
	void HealParticleOver(float _t);
	void HeroStart();
	void HeadShakeOver();
	void ItemCheck();
	void KeyTip();
	void Lose(float _dt);
	bool LimitInit();
	void LimitUpdata();
	void MonsterBeFire(cocos2d::CCNode* _pNode);
	void MonsterFresh();
	void MonsterRun();
	void MonsterAttackRun();
	void MonsterMove(MonsterInfomation* _pMonster, int _index = 0, bool _bMove = false);
	bool MonsterMoveCommon(MonsterInfomation* _pMonster, bool _bMove = false, float _speed = 0.0f);
	bool MonsterMoveCheck1(float _x, float &_y);
	bool MonsterMoveCheck2(float _x, float _y);
	bool MonsterBeAttack(MonsterInfomation* _pMonster, int _damage, int _type = 0);
	void MoveCheck(int _un, int _digit);
	void NeedKillShow();
	void NPCTalk(float _t);
	void NPCTalkStart();
	void ObjectRun();
	void OilcanBomb(cocos2d::CCNode* _pNode);
	void PauseGame(int _type = 0);
	void PlaneFly();
	bool RectInRect(float len1, float wid1, float len2, float wid2);
	void Relife();
	void RelifeMovie(float _t);
	template<class T>
	bool SetVisible(T* _p, float _x, float _y);
	void SetLocation();
	void ShakeTime(float _t);
	void ShowStar();
	void ShowStarMovie1(float _dt);
	void ShowStarMovie2(float _dt);
	void ShowStarMovie3(float _dt);
	void ShowStarBoss(float _dt);
	void ShowStarBoss1();
	void ShowOverBtn(float _dt);
	void ShowScore();
	void ShakeWindow();
	void ShowHealBox();
	void SkillInit();
	bool SkillCheck();
	void SkillAction1();
	void SkillStart();
	void SkillThunderSecond(float _t);
	void SkillThunderThird(float _t);
	void SkillThunderOver(float _t);
	void SkillPlantSecond(float _t);
	void SkillPlantThird(float _t);
	void SkillPlantCallback(cocos2d::CCNode* _pNode);
	void SkillPlantOver(float _t);
	void SkillTimer();
	void SkillPlantDisappear(float _t);
	void SkillBotSecond(float _t);
	void ShillBotThird(float _t);
	void ShillBotOver(float _t);
	void SkillFullCheck();
	void SupplyCheck();
	bool SwitchMapStart();
	void SwitchMap();
	void SwitchMapEnd();
	void Timer(float _dt);
	void TimerStart(float _dt);
	void TipOver();
	void VIPCheck();
	void WinCheck();
	void Win(float _dt);

	void heroJump();
	void heroChangeWeapon();
	void heroUseBomb();
	void heroUseSkill();
	

	void GuideCheck();
	void GuideTalk(int _index,int _type = 0);
	void GuideTalkLogic(float _t = 0);
	void GuideShield(int _index);
	void GuideResume(cocos2d::CCObject *pSender = NULL);
	void GuideVIP();
	void GuideBuy(cocos2d::CCObject *pSender = NULL);
	void GuideBack(cocos2d::CCObject *pSender = NULL);
	void GuideGoToChoseHero(float _t);

	void Sell2();
	void Sell2Resume(cocos2d::CCObject *pSender = NULL);
	void Sell2Back(cocos2d::CCObject *pSender = NULL);
	void Sell3();
	void Sell3Resume(cocos2d::CCObject *pSender = NULL);
	void Sell3Back(cocos2d::CCObject *pSender = NULL);
	void Sell3Resume1(cocos2d::CCObject *pSender = NULL);
	void Sell3Resume2(cocos2d::CCObject *pSender = NULL);
	void Sell4_1(cocos2d::CCObject *pSender);
	void Sell4(cocos2d::CCObject *pSender = NULL);
	void Sell4Resume(cocos2d::CCObject *pSender = NULL);
	void Sell5();
	void Sell5Resume(cocos2d::CCObject *pSender = NULL);

	void OnExitGame(cocos2d::CCObject* sender = NULL);
	void ExitYes(cocos2d::CCObject* sender);
	void ExitNo(cocos2d::CCObject* sender);
	void GetMedal( cocos2d::CCObject* sender = NULL );
	void GetMedalBtn( cocos2d::CCObject* _pNode );
	void GetRemove( cocos2d::CCObject* _pNode = NULL );

	int		m_iSkillCanUse;
	bool	m_bSellOpen;
	bool	m_bExitGame;

	int		m_iStateHP;
	int		m_iStateNumKill;
	int		m_iStateNumMedal;
	int		m_iStateNumGold;
	int		m_iStateGold;
	int*	m_iStateBullet;
	int		m_iStateSkill[2];
	int		m_iBulletNum[2];
	int		m_iGrenadeNum;
	int		m_iGrenadeTime;
	int		m_iSkill;
	int		m_iHealBox;
	int		m_iVIPAttackedTime;
	int		m_iVIPLastAction;
	int		m_iBloodLessTime;
	int		m_iComboMax;
	int		m_iMoneyTo;

	int		m_unLevel;
	int		m_iLevelType;
	int		m_iDifficult;
	float	m_fHeroLocateX;
	float	m_fHeroLocateY;
	bool	m_bPause;
	bool	m_bSkill;
	bool	m_bStartMovie;
	CHero*	m_pHero;
	int		m_iFireX[2];
	int		m_iFireY[2];
	bool	m_bBossBox;
	int		m_iYDist;
	int		m_iYDist2;

	e_WeaponType	m_ewtNowWeapon;

	cocos2d::CCNode*		m_pGameoverBtnNode;
	cocos2d::CCNode*		m_pGetItemCCB;
	CFrontground*			m_pCFrontgroud;
	CBackground*			m_pCBackgroud;
	cocos2d::CCTMXTiledMap* m_pCMidground;
	QuestionInfo			m_structQuestion;
	bool					m_bEndPress;
	int		m_iHeroJumps;
	int m_iGuideAction;

	bool					m_bWin;
private:
	static CGameControler*	s_pGameControler;

	cocos2d::CCTexture2D*	m_ptBullet[2];
	int						m_iBulletW[3];
	int						m_iBulletH[3];
	bool					m_bStrike[2];
	int						m_bShells[2];
	int						m_iActionType1[2];
	int						m_iActionType2[2];
	int						m_iWeaponDamage[3];
	int						m_iWeaponSpeed[3];
	int						m_iFireIndex[2];
	int						m_iKnifeAction[7];

	int	m_iActionIndex;
	int	m_iNeedKill;

	cocos2d::CCLayerColor*	m_pChangeMapColor;
	cocos2d::CCSprite*		m_pChangeMapLayer;
	cocos2d::CCTexture2D*	m_ptBox;
	cocos2d::CCTexture2D*	m_ptShell;
	cocos2d::CCTexture2D*	m_ptDropsA[3];
	cocos2d::CCTexture2D*	m_ptDropsB[7];
	cocos2d::CCTexture2D*	m_ptDropsT[5];
	cocos2d::CCTexture2D*	m_ptHead[11];
	cocos2d::CCTexture2D*	m_ptDropsD;
	cocos2d::CCTexture2D*	m_ptHeadShake[4];
	cocos2d::CCTexture2D*	m_ptPoisonGas[9];
	cocos2d::CCScene*		m_pScene;
	cocos2d::CCSize			m_cSize;
	cocos2d::CCNode*		m_pMenuNode;
	cocos2d::CCNode*		m_pPauseNode;
	cocos2d::CCNode*		m_pGameoverNode;
	cocos2d::CCNode*		m_pSkillNode;
	cocos2d::CCNode*		m_pGameStart;
	cocos2d::CCLayer*		m_pBGLayer;
	cocos2d::CCSprite*		m_pHealEffect1;
	cocos2d::CCSprite*		m_pHealEffect2;
	cocos2d::CCParticleSystem* m_pHealParticle;
	cocos2d::CCSprite*		m_pGetItemEffect;
	cocos2d::CCSprite*		m_pHeadShake;
	cocos2d::CCProgressTimer* m_pSkillBar[2];
	cocos2d::CCSprite*		m_pRope[2];
	cocos2d::CCParticleSystemQuad* m_pSkill1Particle;
	cocos2d::CCSprite*		m_pSkillDown[2];
	cocos2d::CCSprite*		m_pWinMark;
	cocos2d::CCSprite*		m_pBeAttackedEffect;
	cocos2d::CCSprite*		m_pBG;
	bool					m_bOver;
	bool					m_bOverMove;
	bool					m_bDead;
	bool					m_bBoss;
	bool					m_bBossPause;
	
	bool					m_bBossWin;
	bool					m_bBombedMovie;
	bool					m_bSkillPause;
	bool					m_bWeaponNeedChange;
	bool					m_bMonsterCry;
	int						m_iGameMinute;
	int						m_iGameSecond;
	int						m_iWave;
	int						m_iWaveThis;
	int						m_iWaveDistTime;
	int						m_iBoxIndex;
	int						m_iWinTime;
	int						m_iSkillFullTime[2];
	int						m_iBulletIndex;
	int						m_iCombo;
	int						m_iComboTime;
	bool					m_bComboShow;
	
	bool	m_bBoxBrokenEffect;
	bool	m_bBeAttackedEffect;
	int		m_timeBeAttackedEffect;

	float	m_fRelativeX;
	float	m_fHeroToward;
	float	m_fHeroSpeedX;
	float	m_fHeroSpeedY;
	float	m_fHeroObjectSpeed;
	float	m_fAimLocateX;
	float	m_fAimLocateY;
	float	m_fHeroLastX;
	float	m_fHeroLastY;
	int		m_iUnderAttacked;
	int		m_iAttackCoolTime;
	int		m_iBulletTime;
	int		m_iRamp;
	bool	m_bDrop;
	bool	m_bAttacked;
	bool	m_bBossView;
	bool	m_bDamaged;
	int		m_iJumpActionType;
	int		m_iFireTime;
	int		m_iShakeTime;
	float	m_iShakeLocate;

	bool	m_bAimMonster;
	float	m_fAimRotation;

	int*	m_pMoveActionArray;
	int		m_iMoveDigit;

	bool			m_bAttack;
	int				m_iAttackDigit;

	MonsterInfomation*	m_pCache;

	std::vector<AttackingObject*>	m_vecAttackingObjectArray;
	std::vector<Shells*>			m_vecShellsArray;
	std::vector<MonsterInfomation*>	m_vecMonsterArray;
	std::vector<ItemObject*>		m_vecItemObejctArray;
	std::vector<Chopper*>			m_vecChopperArray;
	std::vector<BoxDown*>			m_vecBoxDownArray;

	cocos2d::CCSprite*	m_pGrenade;
	float				m_fGrenadeLocateX;
	float				m_fGrenadeLocateY;
	float				m_fGrenadeSpeedX;
	float				m_fGrenadeSpeedY;
	int					m_iGrenadeRotate;
	int					m_iGrenadeToward;

	cocos2d::CCSprite*		m_pThunder1;
	cocos2d::CCSprite*		m_pSkillMan;
	cocos2d::CCSprite*		m_pThunder2;
	cocos2d::CCSprite*		m_pThunder3;
	cocos2d::CCArray*		m_pArrayThunder;
	int						m_iPlantTime;

	cocos2d::CCArray*		m_pArrayBlood;

	int	m_iStar;

	cs::Armature*			m_pVIP;
	cocos2d::CCSprite*		m_pEnemyTipL;
	cocos2d::CCSprite*		m_pEnemyTipR;
	cocos2d::CCSprite*		m_pVipTip1;
	cocos2d::CCSprite*		m_pVipTip2;
	cocos2d::CCSprite*		m_pVipHPBoard;
	cocos2d::CCProgressTimer*	m_pHpBar;
	int						m_iVipTipTime;
	int						m_iVIPHp;

	bool	m_bIsMoveObject;
	float	m_fAliveLocateX;
	float	m_fAliveLocateY;
	float	m_fOnLandLocateX;
	float	m_fOnLandLocateY;

	cocos2d::CCSprite*		m_pFireSprite;
	AttackingObject*		m_pFireObject;

	cocos2d::CCSprite*			m_pBossBloodBoard;
	cocos2d::CCProgressTimer*	m_pBossBloodProgress;
	float m_iBossLimitX;
	cocos2d::CCSprite*			m_pSpriteBuffer1;
	cocos2d::CCSprite*			m_pSpriteBuffer2;
	cocos2d::CCSprite*			m_pSpriteBuffer3;
	int m_iFrozeTime;
	int m_iPrisonTime;

	int					m_iLimitTimeNeed;
	cocos2d::CCSprite*	m_pLimitBoard;
	bool				m_bLimitStart;
	int					m_iLimitDigit;

	bool				m_bPrisonWater;
	cocos2d::CCSprite*	m_pFireGas;
	cocos2d::CCParticleSystemQuad* m_pParitcleInPrison;

	bool m_bPerfect;
	bool m_bUnFire;

	int	m_iAllStar;

	cocos2d::CCSprite* m_pWeaponMark1;
	cocos2d::CCSprite* m_pWeaponMark2;
	unsigned int m_unMoveEventID;
	cocos2d::CCPoint m_pointMoveEvent;

	int m_iKeyNum;
	int m_iKeyIndex[3];
	cocos2d::CCSprite* m_pKeyTip;

	int m_iYFix;
	int m_iNPCTalkTime;
	bool m_bCanPause;

	std::string m_sCutSound;

	bool m_bEffect1;
	bool m_bEffect2;
	bool m_bEffect3;
	bool m_bEffect4;
	bool m_bEffect5;
	bool m_bEffect6;

	int m_iTaijimanAttack;

	int	m_bCreateMonster;
	int m_iBossCreateX;
	int m_iBossCreateY;
	int m_iPressCount;

	cocos2d::CCSprite* m_pBoss6Skill;

	int m_iMapNum;
	int m_iGuideMap;
	int m_iGuideBtn;
	int m_iGuideNow;
	int m_iGuideTime;
	int m_iGuideTalk;
	bool m_bGuidePause;
	bool m_bGuideWeaponGet;
	bool m_bGuideBuyVip;
	int m_bGuideBuyVipIndex;
	int m_iWeaponUse[2];
	bool m_bInSand;

	cocos2d::CCSprite* m_pRelife;
	bool m_bFirstDown;

	float m_fHeroSpeedXRes;
	
	cocos2d::CCSprite* m_pSkillDigit;

	int m_iHoldTime;
	int m_iHoldToward;

	int m_iJumpCanTimes;
	float m_fJumpSpeedInit;

	int m_iBossBoxNeed;

	cocos2d::CCMenu* m_pSullpyBtn;
	cocos2d::CCSprite* m_pSullpyHealbox;
	cocos2d::CCSprite* m_pSullpyBullet;
	cocos2d::CCSprite* m_pSullpyBoard;

	bool m_bSellSure;
	bool m_bShakeStop;
	
	int m_iMapIndex;
	int m_iMapMax;
	bool m_iMapChanged;
	bool m_bBuyMedal;

	bool keyPressed;
	int keyCode;
	bool bInChangeWeapon;

	bool isInRelife;
	int relifeIndex;

	bool isInExit;
	int exitIndex;

	bool isInRecharge;
	int rechargeIndex;

	bool isBuji;

	std::string m_sBulletPath;
};

#endif
