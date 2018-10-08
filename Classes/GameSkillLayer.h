#ifndef _CCB_SKILL_H_
#define _CCB_SKILL_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameControler.h"

class ccbSkillLayer
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbSkillLayer, create);

	ccbSkillLayer();
	virtual ~ccbSkillLayer();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler);

	void Appear();
	void Appear2();
	void Appear3();
	void over(cocos2d::CCNode* sender);

private:
	CGameControler*	m_pGameControler;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
};

class CCBReader;
class ccbSkillLayerLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbSkillLayerLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbSkillLayer);
};

#endif
