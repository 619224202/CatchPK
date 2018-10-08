#ifndef _CCB_CHOSEHERO_H_
#define _CCB_CHOSEHERO_H_

#include "cocos2d.h"
#include "cocos-ext.h"


class ccbChoseHero
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbChoseHero, create);

	ccbChoseHero();
	virtual ~ccbChoseHero();
	static cocos2d::CCScene* CreateScene();

	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);

	void OnHero1(cocos2d::CCObject *pSender = NULL);
	void OnHero2(cocos2d::CCObject *pSender = NULL);
	void OnHero3(cocos2d::CCObject *pSender = NULL);
	void OnBuy2(cocos2d::CCObject *pSender = NULL);
	void OnBuy3(cocos2d::CCObject *pSender = NULL);
	void OnYes(cocos2d::CCObject *pSender = NULL);
	void OnYesOver(float _t);
	void OnNo(cocos2d::CCObject *pSender = NULL);

	void Appear(CCNode* _pNode);
	void Show();
	void ShowSoundEffect();

	void over(CCNode* sender);

	void Callback1(CCNode* _pNode);
	void Callback2(CCNode* _pNode);

	void disappear();

	void Guide1();
	void Guide2(float _t);
	void Guide3(float _t);

	void OnBuy2Buy(cocos2d::CCObject *pSender = NULL);
	void OnBuy2Cancle(cocos2d::CCObject *pSender = NULL);
	void OnBuy3Buy(cocos2d::CCObject *pSender = NULL);
	void OnBuy3BuyOver(float _t);
	void OnBuy3Cancle(cocos2d::CCObject *pSender = NULL);

	void OnBuy3Show(cocos2d::CCObject *pSender = NULL);
	void OnBuy3Disappear(cocos2d::CCObject *pSender = NULL);
	void OnBuy3Start(cocos2d::CCObject *pSender = NULL);
	void OnBuy3Timer(float _t);
	void OnBuy3Over();
	void OnBuy3Disappear1(cocos2d::CCNode* _pNode);
	void OnBuy3Disappear2(cocos2d::CCNode* _pNode);
	void OnBuy3Disappear3(float _t);

	void CallBack(cocos2d::CCNode* _pNode);
	void OnBuy3PaySwitch();
	static ccbChoseHero* s_pccbChoseHero;
private:
	bool m_bPress;
	bool m_bFirstShow;
	bool m_bCanChose;
	bool m_bIsChose;
	cocos2d::CCNode* m_pNode;
	cocos2d::CCSprite* m_pHeroName1;
	cocos2d::CCSprite* m_pHeroName2;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;

	cocos2d::CCSprite* m_pHero3Board[2];
	cocos2d::CCSprite* m_pHero3Body1[6];
	cocos2d::CCSprite* m_pHero3Body2[6];
	cocos2d::CCSprite* m_pHero3Body;
	cocos2d::CCLayerColor* m_pLayerColor;
	cocos2d::CCMenu* m_pMenuStart;
	cocos2d::CCMenu* m_pMenuShow;
	cocos2d::CCMenu* m_pMenuDisappear;
	int m_iHero3Buy;
	int m_iHero3BuyDigit;
	int m_iHero3BuyTimer;
	int m_iHero3BuyAim;
	int m_iBuy3Type;

	bool isShowRobit;
	bool isShowModules;
	bool isShowHero2;
	int mainIndex;
	int robitIndex;
	int modulesIndex;
	int hero2Index;
	int mainPos[4][2];

	void setFingerPos(cocos2d::CCPoint point);
};

class CCBReader;
class ccbChoseHeroLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbChoseHeroLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbChoseHero);
};

#endif
