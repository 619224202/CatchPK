#ifndef _CCB_LOGINEDLAYER_H_
#define _CCB_LOGINEDLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CSizeLayer.h"
#include "GetPrize.h"

struct Prize
{
	int digit;
	int type;
	int num;
	int level;
	int probability;
	bool chosed;

	Prize()
	{
		chosed = false;
	};
};


class ccbLoginEDLayer
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbLoginEDLayer, create);

	ccbLoginEDLayer();
	virtual ~ccbLoginEDLayer();

	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager, cocos2d::CCNode* _pNode);

	void Appear();
	void Start(float _t);
	void Timer(float _t);
	void OnStart(cocos2d::CCObject* sender);
	void OnBack(cocos2d::CCObject* sender = NULL);

	void initPrize();
	int RandomResult();
	void GetPrize();
	void Next();

	void GetMoney(cocos2d::CCObject* sender = NULL);
	void GetMedal(cocos2d::CCObject* sender = NULL);
	void GetMoneyBtn(cocos2d::CCObject* _pNode);
	void GetMedalBtn(cocos2d::CCObject* _pNode);
	void GetRemove(cocos2d::CCObject* _pNode = NULL);

	void createHandelCreator();

	static ccbLoginEDLayer* s_pccbLoginEDLayer;
private:
	int	m_iSaveX;
	int	m_iFix;
	int	m_iToward;
	int	m_iLocate;
	int	m_iTime;
	int	m_iDist;
	int	m_iSpeed;
	int	m_iAim;
	int m_iArray[3];
	int	m_iIndex[3];
	bool m_bPress;
	int	m_iAimLocateX;
	bool m_bStop;
	Prize m_pPrize[36];
	Prize* m_ppChosePrize[10];
	common::CSizeLayer* m_pLayer;
	cocos2d::CCNode* m_pPrizeBoard;
	cocos2d::CCNode* m_pFather;
	cocos2d::CCNode* m_pBoard;
	cocos2d::CCParticleSystem* m_pParticle;
	cocos2d::CCSprite* m_pSprite[4];
	cocos2d::CCSprite* m_pBar[2];
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
	cocos2d::CCSprite* m_pselector;

	ccbGetPrizeLayer* m_pGetPrize;

	bool m_bOnSell;

	int butPos[7][2];
	int butIndex;
};

class CCBReader;
class ccbLoginEDLayerLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbLoginEDLayerLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbLoginEDLayer);
};

#endif
