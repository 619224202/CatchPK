#include "GameStartMenu.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define TAG_WAVENUM	20

//CCNode* ccbGameStart::m_pNode = NULL;
//ccbGameStart* ccbGameStart::m_this = NULL;

ccbGameStart::ccbGameStart()
	: m_AnimationManager(NULL)
{
	m_pNode = NULL;
}

ccbGameStart::~ccbGameStart()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbGameStart::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	//m_this = this;
	m_pNode = pNode->getChildByTag(102);
	for ( int i = 1; i < 10; i++ )
	{
		char buffer[255];
		sprintf(buffer, "tu10/%d.png", i);
		CCSprite* pSprite = CCSprite::create(buffer);
		pSprite->setVisible(false);
		m_pNode->addChild(pSprite, 0, TAG_WAVENUM+i);
	}
}

SEL_MenuHandler ccbGameStart::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;    
}

SEL_CCControlHandler ccbGameStart::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbGameStart::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbGameStart::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbGameStart::setAnimationManager(CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pGameControler = _pGameControler;
}

void ccbGameStart::Appear()
{
	setVisible(true);
	m_AnimationManager->runAnimationsForSequenceNamed("Default Timeline");
	scheduleOnce(schedule_selector(ccbGameStart::Start), 1.50f);
}

void ccbGameStart::Start(float _t)
{
	setVisible(false);
	//m_pGameControler->removeChild(this);
	m_pGameControler->m_bStartMovie = true;
}

void ccbGameStart::Wave(int _num)
{
	setVisible(true);
	for (int i = 1; i < 10; i++ )
	{
		m_pNode->getChildByTag(TAG_WAVENUM+i)->setVisible(false);
	}
	m_AnimationManager->runAnimationsForSequenceNamed("Default Timeline1");
	m_pNode->getChildByTag(TAG_WAVENUM+_num)->setVisible(true);
	scheduleOnce(schedule_selector(ccbGameStart::Start), 1.50f);
}
