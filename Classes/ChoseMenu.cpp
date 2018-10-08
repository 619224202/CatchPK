#include "ChoseMenu.h"
#include "baseRes.h"
#include "MainMenu.h"
#include "LevelMenu.h"
#include "ccb.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define NeedTime 130

ccbChoseMenu::ccbChoseMenu()
	: m_AnimationManager(NULL)
	, m_iLevel(0)
{
	m_AnimationManager = NULL;
}

ccbChoseMenu::~ccbChoseMenu()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbChoseMenu::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
}

SEL_MenuHandler ccbChoseMenu::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On5", ccbChoseMenu::OnReturn );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On6", ccbChoseMenu::OnMode1 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On7", ccbChoseMenu::OnMode2 );
	return NULL;    
}

SEL_CCControlHandler ccbChoseMenu::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbChoseMenu::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbChoseMenu::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbChoseMenu::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
}

void ccbChoseMenu::Appear()
{
	m_iLevel = ((ccbLayer*)CMainMenu::GetMenuMain())->GetLevel();
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
}

void ccbChoseMenu::OnReturn(cocos2d::CCObject *pSender)
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("fanhui", 0.0f);
	((ccbLayer*)CMainMenu::GetMenuMain())->ButtonBack();
	scheduleOnce(schedule_selector(ccbChoseMenu::ReturnCallBack), 0.5f);
}

void ccbChoseMenu::ReturnCallBack( float _t )
{
	CMainMenu::SetStatus(2);
	((ccbLevelMenu*)CMainMenu::GetMenuLevel())->removeChildByTag(10);
	((ccbLevelMenu*)CMainMenu::GetMenuLevel())->m_iChose = 0;
}

void ccbChoseMenu::OnMode1( cocos2d::CCObject *pSender )
{
	AppDelegate::ChangeScene( CMainMenu::StartGame(1 + m_iLevel*7, CMainMenu::GetDifficult()) );
}

void ccbChoseMenu::OnMode2( cocos2d::CCObject *pSender )
{
	AppDelegate::ChangeScene( CMainMenu::StartGame(2 + m_iLevel*7, CMainMenu::GetDifficult()) );
}
