#ifndef _CCB_FIRSTLOGIN_H_
#define _CCB_FIRSTLOGIN_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class ccbFirstLogin
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbFirstLogin, create);

	ccbFirstLogin();
	virtual ~ccbFirstLogin();
	virtual void keyBackClicked();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);

	void Appear();
	void Bat();
	void Timer(float _t);
	void StartDisappear(float _t);
	void Start();

	int m_iTime;
	bool m_bStart;
	cocos2d::CCSprite* m_pStartPic;
private:
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
};

class CCBReader;
class ccbFirstLoginLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbFirstLoginLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbFirstLogin);
};

class CFirstLogin : public cocos2d::CCLayer
{
public:
	CFirstLogin();

	void InitData(float _t);
	void Over();

	static cocos2d::CCScene* CreateScene(bool _bFirst = true, int _type = 0);
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

private:
	ccbFirstLogin* m_pNode;
	cocos2d::CCScene* m_pScene;
};

#endif
