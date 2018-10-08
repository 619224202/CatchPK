#include "BossBox.h"
#include "MainMenu.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace cocos2d::extension;

ccbBossBox::ccbBossBox()
	: m_AnimationManager(NULL)
{
}

ccbBossBox::~ccbBossBox()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbBossBox::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
}

SEL_MenuHandler ccbBossBox::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;    
}

SEL_CCControlHandler ccbBossBox::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbBossBox::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbBossBox::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbBossBox::setAnimationManager(CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pGameControler = _pGameControler;
}

cocos2d::SEL_CallFuncN ccbBossBox::onResolveCCBCCCallFuncSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over", ccbBossBox::Over);
	return 0;
}

void ccbBossBox::Over( cocos2d::CCNode* sender )
{
	m_pGameControler->m_bBossBox = true;
}
