#include "GameSkillLayer.h"
#include "baseRes.h"

using namespace cocos2d;
using namespace cocos2d::extension;

ccbSkillLayer::ccbSkillLayer()
	: m_AnimationManager(NULL)
{
}

ccbSkillLayer::~ccbSkillLayer()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbSkillLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
}

SEL_MenuHandler ccbSkillLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;    
}

SEL_CCControlHandler ccbSkillLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbSkillLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbSkillLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbSkillLayer::setAnimationManager(CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pGameControler = _pGameControler;
}

void ccbSkillLayer::over( CCNode* sender )
{
	removeChildByTag(10);
	setVisible(false);
	m_pGameControler->m_bPause = false;
}

cocos2d::SEL_CallFuncN ccbSkillLayer::onResolveCCBCCCallFuncSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over", ccbSkillLayer::over);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over2", ccbSkillLayer::over);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over3", ccbSkillLayer::over);
	return NULL;
}

void ccbSkillLayer::Appear()
{
#ifdef GameTypeA
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,200), 800, 480);
	pCCLayerColor->setPosition(ccp(0,0));
	addChild(pCCLayerColor, -1, 10);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
#endif
#ifdef GameTypeB
	m_pGameControler->m_bPause = false;
#endif
}

void ccbSkillLayer::Appear2()
{
#ifdef GameTypeA
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,200), 800, 480);
	pCCLayerColor->setPosition(ccp(0,0));
	addChild(pCCLayerColor, -1, 10);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Untitled Timeline", 0.0f);
#endif
}

void ccbSkillLayer::Appear3()
{
#ifdef GameTypeA
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,200), 800, 480);
	pCCLayerColor->setPosition(ccp(0,0));
	addChild(pCCLayerColor, -1, 10);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Untitled Timeline2", 0.0f);
#endif
}
