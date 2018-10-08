#include "GameBossLoading.h"
#include "MainMenu.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace cocos2d::extension;

ccbGameBossMenu::ccbGameBossMenu()
{
}

ccbGameBossMenu::~ccbGameBossMenu()
{
}

void ccbGameBossMenu::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
}

SEL_MenuHandler ccbGameBossMenu::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;    
}

SEL_CCControlHandler ccbGameBossMenu::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbGameBossMenu::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbGameBossMenu::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}