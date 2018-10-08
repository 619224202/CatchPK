#ifndef _CCB_CCB_CHOSE_H_
#define _CCB_CCB_CHOSE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class ccbChoseMenu
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbChoseMenu, create);

	ccbChoseMenu();
	virtual ~ccbChoseMenu();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

	

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);

	void Appear();
	void OnReturn(cocos2d::CCObject *pSender);
	void ReturnCallBack(float _t);

	void OnMode1(cocos2d::CCObject *pSender);
	void OnMode2(cocos2d::CCObject *pSender);

private:
	int	m_iLevel;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
};

class CCBReader;
class ccbChoseMenuLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbChoseMenuLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbChoseMenu);
};

#endif
