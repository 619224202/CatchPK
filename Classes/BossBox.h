#ifndef _CCB_BOSSBOX_H_
#define _CCB_BOSSBOX_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameControler.h"

class ccbBossBox
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbBossBox, create);

	ccbBossBox();
	virtual ~ccbBossBox();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);

	void Over(cocos2d::CCNode* sender);

private:
	CGameControler*	m_pGameControler;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
};

class CCBReader;
class ccbBossBoxLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbBossBoxLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbBossBox);
};

#endif
