#ifndef _CCB_CCB_SHOP_H_
#define _CCB_CCB_SHOP_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Hero.h"
#include "CSArmature.h"
#include "GameControler.h"

static int g_priceBuy[] =
{
    300, 260,
    100, 200, 500,
    1500, 600, 5200,
    13500, 36800, 100, 280,
    800, 1200,
    30, 100,
    350, 50,
};

static int g_priceUpgrade[] =
{
    300, 260,
    100, 200, 500,
    1500, 600, 5200,
    13500, 36800, 100, 280,
    800, 1200,
    30, 100,
    350, 50,
};

static int g_priceBullet[] =
{
    300, 260,
    100, 30, 120,
    200, 60, 240,
    500, 300, 280, 500,
    800, 500,
    0, 0,
    400, 300,
};

static int g_weaponSpeedShow[] =
{
    0,0,
    36, 72, 240, 90, 36, 180, 72, 120, 180, 360, 450, 36,
    36, 36, 72, 90,
};

static int g_weaponUseMedal[] =
{
    1,1,
    1,1,1,1,0,1,1,1,0,0,0,0,
    0,0,
    0,0,
};

class ccbShopMenu
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbShopMenu, create);
    
    ccbShopMenu();
    virtual ~ccbShopMenu();
    
	static cocos2d::CCSprite* createHandelCreator();

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    
    void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
    
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

    void Timer(float _dt);
    
    void Appear();
    void OnReturn(cocos2d::CCObject *pSender);
    void ReturnCallBack(float _t);
    void OnShopBack(cocos2d::CCObject *pSender);
    void IndexChange(bool _bLeft);
    void WeaponChange();
    void WeaponChangeTime(float _t);
    void WeaponChangeOver(float _t);
    
    void RunAction(int _type, int _frame = 0, int _repeat = 1 );
    void SpriteWeaponChange(int _index);
    
    void VisibleSet(float _t);
    void SetWeaponUse(int _num);
    bool SetWeaponShow(int _num, int _type = 0);
    void ShowUseWeapon();
    
    void OnWeaponOnOff(cocos2d::CCObject *pSender);
    void OnBuy(cocos2d::CCObject *pSender);
    void OnEquip(cocos2d::CCObject *pSender);
    void OnUpgrade(cocos2d::CCObject *pSender);
    void UpgradeParticle(cocos2d::CCNode* _pNode);
    void WeaponUpgradeOver(cocos2d::CCNode* _pNode);
    
    static int WeaponType(int _num);
    static bool WeaponOnSell(int _num);
    
    void AttackRun();
    void AttackCheck();
    void BulletAimMonster(cocos2d::CCNode* _pNode);
    void CallBackRemoveSelf(cocos2d::CCNode* _pNode);
    void CreateAttack();
    int FireIndex(int _type);
    int ActionIndex(int _type);
    
    int Week(int _day);
    void OnSaleInit();
    void OnSaleMin(int _num);
    void OnSaleDigit(float _t);
    void OnSaleBuy(cocos2d::CCObject* sender);
    
    void ChangeShow(cocos2d::CCNode *pSender);
    void MoneyShow();
    bool WeaponCanUse(int _type);
    static int Exclusive(int _type);
    
    void GuideCheck();
    void GuideTalk(int _index, int _type = 0);
    void GuideTalkNext(float _t = 0);
    void GuideFingerCreate(int _x, int _y);
    void GuideFingerClear();
    void GuideFingerMove();
    void GuideShow();
    
    void disappear();
    void Buy(int _num);
    void Buy1(cocos2d::CCObject* sender);
    void Buy2(cocos2d::CCObject* sender);
    void Buy3(cocos2d::CCObject* sender);
    void GetMoney(cocos2d::CCObject* sender = NULL);
    void GetMedal(cocos2d::CCObject* sender = NULL);
    void GetMoneyBtn(cocos2d::CCObject* _pNode);
    void GetMedalBtn(cocos2d::CCObject* _pNode);
    void GetRemove(cocos2d::CCObject* _pNode = NULL);
    
    cocos2d::extension::CCBAnimationManager* m_AnimationManager;
    static ccbShopMenu* s_pccbShopMenu;
    int		m_iStatus;
private:
    bool	m_bGuideClose;
    int		m_iGuideIndex;
    int		m_iGuideTalk;
    int		m_iGuideTime;
    
    int		m_iOnSetArray[4];
    int		m_iIndex;
    int		m_iTime;
    int		m_iPress;
    int		m_unLastAction;
    int		m_iShowWeapon;
    int		m_iAttackTime;
    int		m_iRandAction;
    bool	m_bIn;
    bool	m_bReturn;
    bool	m_bWeapon;
    bool	m_bWeaponChange;
    bool	m_bShowAction;
    //int		m_iStartTime;
    //CHero*	m_pHero;
    cs::Armature*		m_pArmature;
    cocos2d::CCPoint	m_ccPoint;
    cocos2d::CCNode*	m_pNodeMoney;
    cocos2d::CCNode*	m_pNodeMedal;
    cocos2d::CCNode*	m_pNodeBoard;
    cocos2d::CCNode*	m_pNodeWeaponUse;
    cocos2d::CCNode*	m_pNodeShowBoard;
    cocos2d::CCSprite*	m_pShowWeaponOff;
    cocos2d::CCMenuItemImage*	m_pShowWeaponOn;
    cocos2d::CCMenuItemImage*	m_pShowWeaponBuy;
    cocos2d::CCMenuItemImage*	m_pShowWeaponEquip;
    cocos2d::CCMenuItemImage*	m_pShowWeaponUpgrade;
    cocos2d::CCMenuItemImage*	m_pShowWeaponBullet[3];
    cocos2d::CCMenuItemImage*	m_pGetMoney;
    cocos2d::CCMenuItemImage*	m_pGetMedal;
    cocos2d::CCSprite*	m_pShopEquiped;
    cocos2d::CCSprite*	m_pShopUpgradeMax;
    cocos2d::CCSprite*	m_pWeaponBG;
    cocos2d::CCSprite*	m_pWeaponNode[20];
    cocos2d::CCSprite*	m_pWeaponBullet[20];
    cocos2d::CCSprite*	m_pIndexChose[6];
    cocos2d::CCSprite*	m_pIndexUnchose[6];
    cocos2d::CCSprite*	m_pStar1[3];
    cocos2d::CCSprite*	m_pStar2[3];
    cocos2d::CCSprite*	m_pBullet;
    cocos2d::CCSprite*	m_pBulletNum;
    cocos2d::CCSprite*	m_pHealboxNum;
    cocos2d::CCSprite*	m_pGrenadeNum;
    cocos2d::CCSprite*	m_pDamage;
    cocos2d::CCSprite*	m_pSpeed;
    cocos2d::CCSprite*	m_pPower;
    cocos2d::CCSprite*	m_pLife;
    cocos2d::CCSprite*	m_pBulletMany;
    cocos2d::CCSprite*	m_pBulletLitter;
    cocos2d::CCSprite*	m_pCostPic[2];
    
    std::vector<AttackingObject*>	m_vecAttackingObjectArray;
    cocos2d::CCSprite*		m_pFireSprite;
    AttackingObject*		m_pFireObject;
    cs::Armature*			m_pFire;
    int m_iAttackCoolTime;
    int m_iActoinTime;
    cocos2d::CCNode*		m_pBulletNode;
    
    cocos2d::CCSprite*		m_pOnSaleBoard;
    int m_iOnSaleTime;
    
    int m_iNewShowWeapon;
    bool m_bNewShowWeapon;
    int m_iGuideBuyTime;
    
    bool m_bOnSell;

	int w_butPos[5][2];
	int w_groupIndex;
	int w_butIndex;
	int w_goodsPos[4][2];
	int w_goodsPage;
	int w_goodIndex;
	int w_diaIndex;
	bool isInInfo;
	int w_beforeGroupIndex;
	int w_rechargeIndex;
	int w_beforePosX,w_beforePosY;
	bool isInRecharge;

	void setFignerPos(cocos2d::CCPoint point);
	void payMoneyBtn(int tag);
	void payMedalBtn(int tag);
};

class CCBReader;
class ccbShopMenuLoader : public cocos2d::extension::CCLayerLoader 
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbShopMenuLoader, loader);
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbShopMenu);
};

#endif
