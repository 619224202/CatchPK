#include "GetPrize.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace cocos2d::extension;

ccbGetPrizeLayer::ccbGetPrizeLayer()
	: m_AnimationManager(NULL)
{
	m_bOver = false;
}

ccbGetPrizeLayer::~ccbGetPrizeLayer()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbGetPrizeLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	m_pNode = pNode->getChildByTag(101);

	CCSprite* pSprite = CCSprite::create("tu17/guang.png");
	pSprite->setPosition(ccp(292,208));
	addChild(pSprite, -1, 2);
	CCRotateBy* pCCRotateTo = CCRotateBy::create(2.0f, 180);
	pSprite->runAction(CCRepeatForever::create(pCCRotateTo));
	pSprite->setVisible(false);

}

SEL_MenuHandler ccbGetPrizeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;    
}

SEL_CCControlHandler ccbGetPrizeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbGetPrizeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbGetPrizeLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbGetPrizeLayer::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
}

cocos2d::SEL_CallFuncN ccbGetPrizeLayer::onResolveCCBCCCallFuncSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "get", ccbGetPrizeLayer::get);
	return 0;
}

void ccbGetPrizeLayer::initPrize( int _index )
{
	m_iIndex = _index;
	char buffer[255];
	sprintf(buffer, "tu17/%d.png", m_iIndex);
	CCSprite* pSprite1 = CCSprite::create(buffer);
	pSprite1->setAnchorPoint(ccp(0, 0.5f));
	pSprite1->setPosition(ccp(200, 208));
	addChild(pSprite1, 1, 1);
}

void ccbGetPrizeLayer::Appear()
{
	setVisible(true);
	m_bOver = false;
	getChildByTag(2)->setVisible(true);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
}

void ccbGetPrizeLayer::get( cocos2d::CCNode *pSender )
{
	scheduleOnce(schedule_selector(ccbGetPrizeLayer::Start), 1.50f);
}

void ccbGetPrizeLayer::Start( float _t )
{
	if ( getChildByTag(1) )
		getChildByTag(1)->removeFromParentAndCleanup(true);
	getChildByTag(2)->setVisible(false);
	setVisible(false);
	m_bOver = true;
}
