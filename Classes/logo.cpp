#include "logo.h"
#include "baseRes.h"
#include "AppDelegate.h"
#include "FirstLogin.h"
#include "ComicPlay.h"
#include "PayService.h"

using namespace cocos2d;

CCScene* logo::s_pScene = NULL;

cocos2d::CCScene* logo::CreateScene()
{
	CCScene* pScene = logo::create();
	s_pScene = pScene;
	//CCLayer* pLayer = CCLayer::create();
//#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#ifdef Plat_Telecom_Logo
	PayService::TelecomInit();
	CCLayer* pLayer = CCLayer::create();
	pLayer->setPosition(ccp(0,0));
	SetScale(pLayer);
	pScene->addChild(pLayer, 0, 99);
	CCSprite* pSprite = CCSprite::create("logo_telecom.png");
	if ( pSprite )
	{
		pSprite->setPosition(ccp(400,240));
		pLayer->addChild(pSprite, 0, 99);
	}
	pScene->scheduleOnce(schedule_selector(logo::StartGame_Telecom), 1.00f);
//#endif
#else
#ifdef Movie_Logo
	PayService::PlayLogo();
#else
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(255,255,255,255), 800, 480);
	SetScale(pCCLayerColor);
	pScene->addChild(pCCLayerColor);
	CCSprite* pSprite = CCSprite::create("gamedo.png");
	pSprite->setPosition(ccp(400,240));
	pCCLayerColor->addChild(pSprite);
	pScene->scheduleOnce(schedule_selector(logo::time), 0.01f);
#endif
#endif
	return pScene;
}

void logo::time( float _t )
{
	AppDelegate::ChangeScene(ComicLayer::CreateScene(), etFade, 1.0f);
}

void logo::logoOver()
{
	s_pScene->scheduleOnce(schedule_selector(logo::time), 0.1f);
}

bool logo::init()
{
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	scheduleOnce(schedule_selector(logo::StartGame_WIN32), 0.05f);
#else 
	#ifdef Plat_91
	schedule(schedule_selector(logo::StartGame), 0.05f);
	#else
	scheduleOnce(schedule_selector(logo::StartGame_WIN32), 0.05f);
	#endif
#endif
	return true;
}

cocos2d::CCScene* logo::Create()
{
	logo* pScene = new logo;
	pScene->init();
	pScene->autorelease();
	return pScene;
}

void logo::StartGame( float _t )
{
	if ( AppDelegate::m_bCanStartGame == 1 )
	{
		CCScene* pScene = CreateScene();
		AppDelegate::ChangeScene(pScene);
		unschedule(schedule_selector(logo::StartGame));
	}
}

void logo::StartGame_WIN32( float _t )
{
	CCScene* pScene = CreateScene();
	AppDelegate::ChangeScene(pScene);
}

void logo::StartGame_Telecom( float _t )
{
	if ( getChildByTag(99)->getChildByTag(99) )
	{
		CCSprite* pSprite = (CCSprite*)(getChildByTag(99)->getChildByTag(99));
		CCFadeOut* pAction = CCFadeOut::create(0.5f);
		pSprite->runAction(pAction);
	}

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(255,255,255,255), 800, 480);
	{
		SetScale(pCCLayerColor);
		addChild(pCCLayerColor);
		pCCLayerColor->setOpacity(0);
		CCDelayTime* pAction1 = CCDelayTime::create(0.5f);
		CCFadeIn* pAction2 = CCFadeIn::create(0.5f);
		CCDelayTime* pAction3 = CCDelayTime::create(1.0f);
		pCCLayerColor->runAction(CCSequence::create(pAction1, pAction2, pAction3, NULL));
	}

	CCSprite* pSprite = CCSprite::create("gamedo.png");
	{
		pSprite->setPosition(ccp(400,240));
		pCCLayerColor->addChild(pSprite);
		pSprite->setOpacity(0);
		CCDelayTime* pAction1 = CCDelayTime::create(0.5f);
		CCFadeIn* pAction2 = CCFadeIn::create(0.5f);
		CCDelayTime* pAction3 = CCDelayTime::create(1.0f);
		pSprite->runAction(CCSequence::create(pAction1, pAction2, pAction3, NULL));
	}

	scheduleOnce(schedule_selector(logo::time), 2.00f);
}
