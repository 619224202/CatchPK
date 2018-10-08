#include "MainMenu.h"
#include "GameLoading.h"
#include "MenuLoading.h"
#include "baseRes.h"
#include "AppDelegate.h"
#include "ccb.h"
#include "LevelMenu.h"
#include "ChoseMenu.h"
#include "ShopMenu.h"
#include "CSArmatureDataManager.h"

using namespace cocos2d;
using namespace cocos2d::extension;

int CMainMenu::m_iStatus			= 0;
int CMainMenu::m_iDifficult			= 0;
bool CMainMenu::m_bShop				= false;
bool CMainMenu::m_bOnSell			= false;
CCNode*	CMainMenu::m_pNodeMain		= NULL;
CCNode*	CMainMenu::m_pNodeLevel		= NULL;
CCNode*	CMainMenu::m_pNodeShop		= NULL;
CCScene* CMainMenu::m_pScene		= NULL;
int CMainMenu::m_iArmatureDigit = 2;

CMainMenu::CMainMenu()
{
}

CMainMenu::~CMainMenu()
{
}

bool CMainMenu::init()
{
	m_iStatus			= 0;
	m_iDifficult		= 0;
	m_bOnSell			= false;
	m_bShop				= false;
	m_pNodeMain			= NULL;
	m_pNodeLevel		= NULL;
	m_pNodeShop			= NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	this->setKeypadEnabled(true);
#endif
	return true;
}

CCScene* CMainMenu::StartMenu(int _level, int _difficult, bool _bShop)
{
	m_iStatus = -1;
	CCScene* pScene = CCScene::create();
	CMenuLoading::GetMenuLoadingLayer(pScene, _level, _difficult, _bShop);
	return pScene;
}

CCScene* CMainMenu::StartGame(int _level, int _difficult)
{
	CCScene* pScene = CCScene::create();
	CGameLoading::GetGameLoadingLayer(pScene, _level, _difficult);
	return pScene;
}

void CMainMenu::AllAdd( cocos2d::CCScene* _pScene )
{
	m_pScene = _pScene;
	CMainMenu::m_iArmatureDigit = 2;
	CMainMenu* pLayer = CMainMenu::create();
	//SetScale(pLayer);
	_pScene->addChild(pLayer);
	m_pNodeMain			= NULL;
	m_pNodeLevel		= NULL;
	m_pNodeShop			= NULL;

	CCNode* pNode1 = CreateMenuMain();
	SetScale(pNode1);
	_pScene->addChild(pNode1);
	((ccbLayer*)pNode1)->m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);

	CCNode* pNode3 = CreateMenuLevel();
	SetScale(pNode3);
	_pScene->addChild(pNode3);
	((ccbLevelMenu*)pNode3)->m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);

	CCNode* pNode4 = CreateMenuShop();
	SetScale(pNode4);
	_pScene->addChild(pNode4);
	((ccbShopMenu*)pNode4)->m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("fanhui", 0.0f);
}

void CMainMenu::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCSetIterator iter = pTouches->begin();
	int location = (int)((CCTouch*)(*iter))->getLocation().x;
	int i = (location - 60)/80;
	AppDelegate::ChangeScene( StartGame(i, CMainMenu::GetDifficult()) );
}

void CMainMenu::keyBackClicked()
{
	//AppDelegate::SaveAll();
	//CCDirector::sharedDirector()->end();
	//exit(0);
	if ( m_bShop )
	{
		if ( ((ccbShopMenu*)m_pNodeShop)->m_iStatus == 0 )
			((ccbShopMenu*)m_pNodeShop)->OnReturn(NULL);
		else
			((ccbShopMenu*)m_pNodeShop)->OnShopBack(NULL);

	}
	else
	{
		((ccbLayer*)m_pNodeMain)->OnReturn(NULL);
	}
}

void CMainMenu::keyEnterClicked(){
	//CCLog("按下了确定键");
}

void CMainMenu::keyArrowClicked(int arrow){
	//CCLog("按下了方向键:%d",arrow);
}

void CMainMenu::keyNumClicked(int keyNum){
	//CCLog("按下了数字键:%d",keyNum);
	if(keyNum==kTypeNum0){
		keyBackClicked();
	}
}

CCNode* CMainMenu::CreateMenuMain()
{
	if ( !m_pNodeMain )
	{
		CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("ccbLayer", ccbLayerLoader::loader());
		cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		m_pNodeMain = ccbReader->readNodeGraphFromFile("1.ccbi"/*, this*/);
		((ccbLayer*)m_pNodeMain)->setAnimationManager(ccbReader->getAnimationManager());
		m_pNodeMain->setPosition(ccp(0,0));
		ccbReader->release();
	}
	return m_pNodeMain;
}

cocos2d::CCNode* CMainMenu::CreateMenuLevel()
{
	if ( !m_pNodeLevel )
	{
		CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("ccbLevelMenu", ccbLevelMenuLoader::loader());
		cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		m_pNodeLevel = ccbReader->readNodeGraphFromFile("2.ccbi"/*, this*/);
		((ccbLevelMenu*)m_pNodeLevel)->setAnimationManager(ccbReader->getAnimationManager());
		m_pNodeLevel->setPosition(ccp(0,0));
		ccbReader->release();
	}
	return m_pNodeLevel;
}

cocos2d::CCNode* CMainMenu::CreateMenuShop()
{
	if ( !m_pNodeShop )
	{
		//CCBAnimationManager* actionManager = NULL;
		CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("ccbShopMenu", ccbShopMenuLoader::loader());
		cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		m_pNodeShop = ccbReader->readNodeGraphFromFile("5.ccbi"/*, this*/);
		((ccbShopMenu*)m_pNodeShop)->setAnimationManager(ccbReader->getAnimationManager());
		m_pNodeShop->setPosition(ccp(0,0));
		ccbReader->release();
	}
	return m_pNodeShop;
}

cocos2d::CCNode* CMainMenu::GetMenuMain()
{
	return m_pNodeMain;
}

cocos2d::CCNode* CMainMenu::GetMenuLevel()
{
	return m_pNodeLevel;
}

cocos2d::CCNode* CMainMenu::GetMenuShop()
{
	return m_pNodeShop;
}

void CMainMenu::SetStatus( int _stats )
{
	m_iStatus = _stats;
}

int CMainMenu::GetStatus()
{
	return m_iStatus;
}

void CMainMenu::SetDifficult( int _difficult )
{
	m_iDifficult = _difficult;
}

int CMainMenu::GetDifficult()
{
	return m_iDifficult;
}

void CMainMenu::ReleaseArmatureData()
{
	CMainMenu::m_iArmatureDigit --;
	if ( CMainMenu::m_iArmatureDigit == 0 )
	{
		cs::ArmatureDataManager::sharedArmatureDataManager()->removeAll();
		cs::ArmatureDataManager::sharedArmatureDataManager()->purgeArmatureSystem();
		CCTextureCache::sharedTextureCache()->removeAllTextures();
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
	}
}
