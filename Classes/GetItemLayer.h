#ifndef _CCB_GETITEMLAYER_H_
#define _CCB_GETITEMLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GetPrize.h"
//#include "GameControler.h"

class ccbGetItemLayer
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbGetItemLayer, create);

	ccbGetItemLayer();
	virtual ~ccbGetItemLayer();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager, cocos2d::CCNode* _pNode);
	//void SetGame(CGameControler* p);	
	void InitBtn(int _type);
	void OnBuy1(cocos2d::CCObject *pSender = NULL);
	void OnBuy2(cocos2d::CCObject *pSender);
	void OnBuy3(cocos2d::CCObject *pSender);
	void OnBuy4(cocos2d::CCObject *pSender);
	void OnShop(cocos2d::CCObject *pSender);
	void OnContinue(cocos2d::CCObject *pSender);
	void OnReturn(cocos2d::CCObject *pSender);
	void OnReplay(cocos2d::CCObject *pSender);
	void OnRechoseHero(cocos2d::CCObject *pSender);
	void OnGetAchieve(cocos2d::CCObject *pSender = NULL);

	void StartBtn();
	void Appear1(int _type);
	void Appear2(int _type);
	void DisAppear(cocos2d::CCObject *pSender);

	void Timer(float _t);

	void GetMoney(cocos2d::CCObject* sender = NULL);
	void GetMedal(cocos2d::CCObject* sender = NULL);
	void GetMoneyBtn(cocos2d::CCObject* _pNode);
	void GetMedalBtn(cocos2d::CCObject* _pNode);
	void GetRemove(cocos2d::CCObject* _pNode = NULL);

	void setHandelPos(cocos2d::CCPoint point);
	void appearOver();
	void setHandelPos(int handelIndex);

	void GetPrizeCheck(float _t);

	bool	m_bWin;
	static ccbGetItemLayer* s_pccbGetItemLayer;

	bool isInSell;
	int sellIndex;
private:
	bool	m_bCanPress;
	//int		m_iLevel;
	int		m_iWeapon;
	int		m_iType;
	int		m_iPressType;
	cocos2d::CCNode* m_pNode;
	cocos2d::CCSprite* m_pSprite;
	cocos2d::CCNode* m_pGameControler;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;

	bool m_bOnSell;

	bool m_bIsWeapon;
	int m_iGetPrize;
	ccbGetPrizeLayer* m_pGetPrize;

	int m_levelIndex;

	int mainButIndex;

	int mainPos[8][2];
	bool isAppearOver;
	bool isInMain;
	int settleIndex;
	int butPos[4][2];

	bool isInRecharge;
	int rechargeIndex;
	bool isGetMedal;
};

class CCBReader;
class ccbGetItemLayerLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbGetItemLayerLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbGetItemLayer);
};

#endif
