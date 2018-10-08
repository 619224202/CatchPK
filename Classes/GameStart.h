#ifndef _CCB_STARTGAME_H_
#define _CCB_STARTGAME_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameControler.h"

class ccbStartGame
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbStartGame, create);

	ccbStartGame();
	virtual ~ccbStartGame();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler);

	void Start(cocos2d::CCNode* sender);
	void Repeat(cocos2d::CCNode* sender);
	void FlyOut(cocos2d::CCNode* sender);

	void Appear(cocos2d::CCPoint _StartPoint, cocos2d::CCPoint _EndPont, int _type = 0);
	void RemoveThis();

	void Timer(float _t);

	void GuideBox();

private:
	bool m_bQustionTime;
	bool m_bType;
	int m_iStayTimes;
	int m_iType;
	cocos2d::CCPoint m_ccpEnd;
	cocos2d::CCNode* m_pNode;
	cocos2d::CCNode* m_pNode2;
	CGameControler*	m_pGameControler;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
	cocos2d::CCSprite* m_pGuideBox;
	cocos2d::CCSprite* m_pGuideHolder;
};

class CCBReader;
class ccbStartGameLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbStartGameLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbStartGame);
};

#endif
