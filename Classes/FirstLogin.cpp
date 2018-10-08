#include "FirstLogin.h"
#include "AppDelegate.h"
#include "LoginMenu.h"
#include "baseRes.h"
#include "PayService.h"

using namespace cocos2d;
using namespace cocos2d::extension;

CFirstLogin::CFirstLogin()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	setKeypadEnabled(true);
#endif
}

cocos2d::CCScene* CFirstLogin::CreateScene(bool _bFirst, int _type)
{
	CFirstLogin* pLayer = new CFirstLogin;
	SetScale(pLayer/*->m_pNode*/);
	pLayer->autorelease();
	pLayer->m_pScene = CCScene::create();
	pLayer->m_pScene->addChild(pLayer, -1);
	
	CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ccbFirstLogin", ccbFirstLoginLoader::loader());
	cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	pLayer->m_pNode = (ccbFirstLogin*)ccbReader->readNodeGraphFromFile("16.ccbi"/*, this*/);
	pLayer->m_pNode->setAnimationManager(ccbReader->getAnimationManager());
	pLayer->m_pNode->setAnchorPoint(ccp(0.0f,0.0f));
	pLayer->m_pNode->setPosition(ccp(0,0));
	SetScale(pLayer->m_pNode);
	pLayer->m_pScene->addChild(pLayer->m_pNode, -1);
	ccbReader->release();
	ccbReader = NULL;
	CCLog("player=%d",pLayer->m_pNode->getChildrenCount());

	//pLayer->m_pNode->m_pStartPic = CCSprite::create("tu12/jianzai1.png");
	//pLayer->m_pNode->m_pStartPic->setPosition( ccp(650, 48) );
	//pLayer->m_pNode->m_pStartPic->setScale(1.2f);
	//pLayer->m_pNode->addChild(pLayer->m_pNode->m_pStartPic, 2, 10);
	//CCActionInterval* pAction1 = CCScaleTo::create(1.0f, 1.2f);
	//CCActionInterval* pAction2 = CCScaleTo::create(1.0f, 0.8f);
	//pLayer->m_pNode->m_pStartPic->runAction( CCRepeatForever::create(CCSequence::create(pAction1, pAction2, NULL)) );
	//pLayer->m_pNode->m_pStartPic->setVisible(false);

	if ( _type )
	{
		if ( pLayer->m_pNode->m_pStartPic )
			pLayer->m_pNode->m_pStartPic->removeFromParent();
		pLayer->m_pNode->Bat();
		ccbLoginMenu::LoginScene(pLayer->m_pScene, 1);
	}
	else if ( _bFirst )
	{
		pLayer->scheduleOnce(schedule_selector(CFirstLogin::InitData), 2.5f);
	}
	else
	{
		pLayer->Over();
		pLayer->m_pNode->Bat();
	}

	return pLayer->m_pScene;
}

void CFirstLogin::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	//if ( !m_pNode->m_bStart )
	//	return;
	//m_pNode->Start();
	//setTouchEnabled(false);
	//ccbLoginMenu::LoginScene(m_pScene);
}

void CFirstLogin::InitData( float _t )
{
	AppDelegate::AudioInit1();
	if ( AppDelegate::s_VoiceOpen == 0 )
	{
		AppDelegate::AudioSetVoice();
	}
	AppDelegate::DataInit2();
	//m_pNode->Appear();
	//m_pNode->scheduleOnce(schedule_selector(ccbFirstLogin::StartDisappear), 2.5f);
	Over();
	m_pNode->Bat();
}

void CFirstLogin::Over()
{
	//CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	//ccNodeLoaderLibrary->registerCCNodeLoader("ccbFirstLogin", ccbFirstLoginLoader::loader());
	//cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	//m_pNode = (ccbFirstLogin*)ccbReader->readNodeGraphFromFile("16.ccbi"/*, this*/);
	//m_pNode->setAnimationManager(ccbReader->getAnimationManager());
	//m_pNode->setAnchorPoint(ccp(0.0f,0.0f));
	//m_pNode->setPosition(ccp(0,0));
	//SetScale(m_pNode);
	//m_pScene->addChild(m_pNode, -1);
	//ccbReader->release();

	if ( m_pNode->m_pStartPic )
		m_pNode->m_pStartPic->removeFromParent();

	//m_pNode->m_pStartPic = CCSprite::create("tu12/dianji.png");
	//m_pNode->m_pStartPic->setPosition( ccp(400, 48) );
	//m_pNode->m_pStartPic->setScale(0.8f);
	//m_pNode->addChild(m_pNode->m_pStartPic, 2);
	//CCActionInterval* pAction1 = CCScaleTo::create(1.0f, 1.2f);
	//CCActionInterval* pAction2 = CCScaleTo::create(1.0f, 0.8f);
	//m_pNode->m_pStartPic->runAction( CCRepeatForever::create(CCSequence::create(pAction1, pAction2, NULL)) );
	//m_pNode->m_pStartPic->setVisible(false);
	//setTouchEnabled(true);

	//m_pNode->Start();
	//setTouchEnabled(false);
	ccbLoginMenu::LoginScene(m_pScene);
}

ccbFirstLogin::ccbFirstLogin()
	: m_AnimationManager(NULL)
{
	m_iTime = -1;
	m_bStart = false;
	m_pStartPic = NULL;
	schedule(schedule_selector(ccbFirstLogin::Timer), 0.1f);
}

ccbFirstLogin::~ccbFirstLogin()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbFirstLogin::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
}

SEL_MenuHandler ccbFirstLogin::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}

SEL_CCControlHandler ccbFirstLogin::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbFirstLogin::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbFirstLogin::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbFirstLogin::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
}

void ccbFirstLogin::Appear()
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
}

void ccbFirstLogin::StartDisappear( float _t )
{
	if ( getChildByTag(10) )
		removeChildByTag(10);
	m_iTime = 0;
	m_bStart = true;
	//m_pStartPic->setVisible(true);
}

void ccbFirstLogin::Start()
{
	//m_pStartPic->setVisible(false);
}

void ccbFirstLogin::Bat()
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Untitled Timeline", 0.0f);
}

void ccbFirstLogin::Timer( float _t )
{
	if ( m_iTime < 0 )
		return;
	m_iTime ++;
	if ( m_iTime >50 )
	{
		int a = rand()%100;
		if ( a < 4 )
		{
			m_iTime = 0;
			Bat();
		}
	}
}

void ccbFirstLogin::keyBackClicked()
{
#ifdef Plat_91
	PayService::exitGame();
#endif
}
