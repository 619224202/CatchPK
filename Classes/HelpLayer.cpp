#include "HelpLayer.h"
#include "MainMenu.h"
#include "AppDelegate.h"
#include "LoginMenu.h"

using namespace cocos2d;
using namespace cocos2d::extension;

ccbHelpLayer::ccbHelpLayer()
	: m_AnimationManager(NULL)
{
	m_iHelpType = 0;
}

ccbHelpLayer::~ccbHelpLayer()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbHelpLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
}

SEL_MenuHandler ccbHelpLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbHelpLayer::On1 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbHelpLayer::On2 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On3", ccbHelpLayer::On3 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On4", ccbHelpLayer::On4 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On5", ccbHelpLayer::On5 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On6", ccbHelpLayer::On6 );
	return NULL;    
}

SEL_CCControlHandler ccbHelpLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbHelpLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbHelpLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbHelpLayer::setAnimationManager(CCBAnimationManager *pAnimationManager, cocos2d::CCNode* _pNode)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pNode = _pNode;
}

void ccbHelpLayer::On1( cocos2d::CCObject *pSender )
{
	if ( m_iHelpType )
		return;
	((ccbLoginMenu*)m_pNode)->HelpMarkShow();
	m_iHelpType = 1;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
}

void ccbHelpLayer::On2( cocos2d::CCObject *pSender )
{
	if ( m_iHelpType )
		return;
	((ccbLoginMenu*)m_pNode)->HelpMarkShow();
	m_iHelpType = 2;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline3", 0.0f);
}

void ccbHelpLayer::On3( cocos2d::CCObject *pSender )
{
	if ( m_iHelpType )
		return;
	((ccbLoginMenu*)m_pNode)->HelpMarkShow();
	m_iHelpType = 3;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline4", 0.0f);
}

void ccbHelpLayer::On4( cocos2d::CCObject *pSender )
{
	if ( m_iHelpType )
		return;
	((ccbLoginMenu*)m_pNode)->HelpMarkShow();
	m_iHelpType = 5;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline6", 0.0f);
}

void ccbHelpLayer::On5( cocos2d::CCObject *pSender )
{
	if ( m_iHelpType )
		return;
	((ccbLoginMenu*)m_pNode)->HelpMarkShow();
	m_iHelpType = 4;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline5", 0.0f);
}

void ccbHelpLayer::On6( cocos2d::CCObject *pSender )
{
	if ( m_iHelpType )
		return;
	((ccbLoginMenu*)m_pNode)->HelpMarkShow();
	setTouchEnabled(true);
	m_iHelpType = 6;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline7", 0.0f);
}

void ccbHelpLayer::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( !m_iHelpType )
		return;
	setTouchEnabled(false);
	char buffer[255];
	sprintf(buffer, "Default Timeline%d hui", m_iHelpType+1);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration(buffer, 0.0f);
}

void ccbHelpLayer::Show( cocos2d::CCNode* sender /*= NULL*/ )
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
}

cocos2d::SEL_CallFuncN ccbHelpLayer::onResolveCCBCCCallFuncSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "ok1", ccbHelpLayer::OK);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "ok2", ccbHelpLayer::OK);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "ok3", ccbHelpLayer::OK);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "ok4", ccbHelpLayer::OK);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "ok5", ccbHelpLayer::OK);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "ok6", ccbHelpLayer::OK);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "end1", ccbHelpLayer::END);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "end2", ccbHelpLayer::END);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "end3", ccbHelpLayer::END);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "end4", ccbHelpLayer::END);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "end5", ccbHelpLayer::END);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "end6", ccbHelpLayer::END);
	return NULL;
}

void ccbHelpLayer::OK( cocos2d::CCNode* sender /*= NULL*/ )
{
	setTouchEnabled(true);
}

void ccbHelpLayer::END( cocos2d::CCNode* sender /*= NULL*/ )
{
	m_iHelpType = 0;
	//Show(NULL);
}
