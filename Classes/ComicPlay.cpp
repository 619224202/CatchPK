#include "ComicPlay.h"
#include "AppDelegate.h"
#include "FirstLogin.h"
#include "GameControler.h"
#include "baseRes.h"
#include "MainMenu.h"
#include "ChoseHero.h"
#include "PayService.h"

using namespace cocos2d;
using namespace cocos2d::extension;

bool ccbComicLayer::m_bCanPress = false;
bool ccbComicLayer::m_bAuidio	= false;

ccbComicLayer::ccbComicLayer()
	: m_AnimationManager(NULL)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	setKeypadEnabled(true);
#endif
	AppDelegate::AudioPlayEffect("SoundEffect/zmanhua1.mp3");
	PayService::HideBar();
	if ( AppDelegate::s_FirstLogin )
	{
		m_bCanPress = true;
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCSprite* pSprite = CCSprite::create("tu12/dianji1.png");
		pSprite->setPosition( ccp(700, 25) );
		pSprite->setScale(0.8f);
		addChild(pSprite, 2);
		CCActionInterval* pAction1 = CCScaleTo::create(1.0f, 1.2f);
		CCActionInterval* pAction2 = CCScaleTo::create(1.0f, 0.8f);
		pSprite->runAction( CCRepeatForever::create(CCSequence::create(pAction1, pAction2, NULL)) );
	}
	setKeypadEnabled(true);
}

ccbComicLayer::~ccbComicLayer()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbComicLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
}

SEL_MenuHandler ccbComicLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;    
}

SEL_CCControlHandler ccbComicLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}

SEL_CallFuncN ccbComicLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over", ccbComicLayer::Next);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over2", ccbComicLayer::Over);

	return NULL;
}

bool ccbComicLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbComicLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbComicLayer::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
}

void ccbComicLayer::keyBackClicked(){
	if(m_bCanPress && AppDelegate::s_FirstLogin){
		Over(this);
	}
}

void ccbComicLayer::keyEnterClicked(){
	keyBackClicked();
}

void ccbComicLayer::Next( CCNode* sender )
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
}

void ccbComicLayer::Over( CCNode* sender )
{
	m_bCanPress = false;
	if ( AppDelegate::s_FirstLogin == 0 )
	{
		AppDelegate::DataInit();
		AppDelegate::AudioInit2();
#ifdef No_Trans
		AppDelegate::s_FirstLogin = 6;
		AppDelegate::SaveGuide();
		AppDelegate::ChangeScene(CFirstLogin::CreateScene(), etFade, 1.0f);
#else
		scheduleOnce(schedule_selector(ccbComicLayer::StartGuide), 0.1f);
#endif
	}
	else if ( AppDelegate::s_FirstLogin == 2 )
	{
		//stopAllActions();
		//pauseSchedulerAndActions();
		AppDelegate::ChangeScene( ccbChoseHero::CreateScene() );
	}
	else if ( AppDelegate::s_FirstLogin == 3 )
	{
		//stopAllActions();
		//pauseSchedulerAndActions();
		AppDelegate::ChangeScene( CMainMenu::StartMenu(0, 0, true) );
	}
	else if ( AppDelegate::s_FirstLogin == 4 )
	{
		//stopAllActions();
		//pauseSchedulerAndActions();
		AppDelegate::ChangeScene( CMainMenu::StartMenu() );
	}
	else /*if ( !m_bCanPress )*/
	{
		//stopAllActions();
		//pauseSchedulerAndActions();
		AppDelegate::ChangeScene(CFirstLogin::CreateScene(), etFade, 1.0f);
		//m_bCanPress = true;
		////AppDelegate::DataInit();
		//CCSize size = CCDirector::sharedDirector()->getWinSize();
		//CCSprite* pSprite = CCSprite::create("tu12/dianji1.png");
		//pSprite->setPosition( ccp(700, 455) );
		//pSprite->setScale(0.8f);
		//addChild(pSprite, 2);
		//CCActionInterval* pAction1 = CCScaleTo::create(1.0f, 1.2f);
		//CCActionInterval* pAction2 = CCScaleTo::create(1.0f, 0.8f);
		//pSprite->runAction( CCRepeatForever::create(CCSequence::create(pAction1, pAction2, NULL)) );
	}
}



ComicLayer::ComicLayer()
{
}

cocos2d::CCScene* ComicLayer::CreateScene(bool _bFirst)
{
	ComicLayer* pLayer = new ComicLayer;
	pLayer->autorelease();
	//SetScale(pLayer);
	pLayer->m_pScene = CCScene::create();
	pLayer->m_pScene->addChild(pLayer, -1);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ccbComicLayer", ccbComicLayerLoader::loader());
	cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	pLayer->m_pNode = (ccbComicLayer*)ccbReader->readNodeGraphFromFile("17.ccbi"/*, this*/);
	pLayer->m_pNode->setAnimationManager(ccbReader->getAnimationManager());
	pLayer->m_pNode->setAnchorPoint(ccp(0.0f,0.0f));
	pLayer->m_pNode->setPosition(ccp(0,0));
	pLayer->m_pScene->addChild(pLayer->m_pNode, -1);
	SetScale(pLayer->m_pNode);
	pLayer->m_pNode->m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
	ccbReader->release();

	pLayer->scheduleOnce(schedule_selector(ccbComicLayer::Start), 0.10f);
	return pLayer->m_pScene;
}

void ComicLayer::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( !ccbComicLayer::m_bCanPress )
		return;
	ccbComicLayer::m_bCanPress = false;
	setTouchEnabled(false);
	if ( AppDelegate::s_FirstLogin == 0 )
	{
		AppDelegate::DataInit();
		AppDelegate::AudioInit2();
		scheduleOnce(schedule_selector(ccbComicLayer::StartGuide), 0.1f);
	}
	else if ( AppDelegate::s_FirstLogin == 2 )
	{
		//stopAllActions();
		//pauseSchedulerAndActions();
		AppDelegate::ChangeScene( ccbChoseHero::CreateScene() );
	}
	else if ( AppDelegate::s_FirstLogin == 3 )
	{
		//stopAllActions();
		//pauseSchedulerAndActions();
		AppDelegate::ChangeScene( CMainMenu::StartMenu(0, 0, true) );
	}
	else if ( AppDelegate::s_FirstLogin == 4 )
	{
		//stopAllActions();
		//pauseSchedulerAndActions();
		AppDelegate::ChangeScene( CMainMenu::StartMenu() );
	}
	else
	{
		//stopAllActions();
		//pauseSchedulerAndActions();
		AppDelegate::ChangeScene(CFirstLogin::CreateScene(), etFade, 1.0f);
	}

}

void ccbComicLayer::Start( float _t )
{
	setTouchEnabled(true);
}

void ccbComicLayer::Audio()
{
	if ( m_bAuidio )
		return;
	m_bAuidio = true;
	AppDelegate::AudioInit1();
}

void ccbComicLayer::StartGuide( float _t )
{
	CCScene* pScene = CCScene::create();
	CGameControler* pLayer = CGameControler::NewGameControler(pScene, -2, -1);
	pLayer->GameLoadingSecond();
	pLayer->GameStart();
	AppDelegate::ChangeScene(pScene);
}
