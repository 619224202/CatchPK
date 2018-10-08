#include "MenuLoading.h"
#include "GameLoadingMenu.h"
#include "ccb.h"
#include "ShopMenu.h"
#include "LevelMenu.h"
#include "AppDelegate.h"
#include "baseRes.h"

using namespace cocos2d;
using namespace cocos2d::extension;

CMenuLoading* CMenuLoading::GetMenuLoadingLayer(cocos2d::CCScene* _pScene, int _iLevel, int _difficult, bool _bShop)
{
	AppDelegate::AudioStopBgm();
	CMenuLoading* pLayer = new CMenuLoading;
	pLayer->autorelease();
	pLayer->m_pScene		= _pScene;
	pLayer->m_iLevel		= _iLevel;
	pLayer->m_iDifficult	= _difficult;
	pLayer->m_bShop			= _bShop;

	CMainMenu::SetStatus(-1);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ccbLoadingMenu", ccbGameLoadingMenuLoader::loader());
	cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	pLayer->m_pNode = ccbReader->readNodeGraphFromFile("10.ccbi"/*, this*/);
	pLayer->m_pNode->setPosition(ccp(0,0));
	((ccbGameLoadingMenu*)pLayer->m_pNode)->setAnimationManager(ccbReader->getAnimationManager());
	((ccbGameLoadingMenu*)pLayer->m_pNode)->Appear();
	ccbReader->release();
	SetScale(pLayer->m_pNode);
	_pScene->addChild(pLayer->m_pNode, 21, 4);

	//SetScale(pLayer);
	_pScene->addChild(pLayer, 20, 3);

	CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(20,20,20,255), 800, 480);
	SetScale(pLayerColor);
	_pScene->addChild(pLayerColor, 19, 2);
	//AppDelegate::AudioPlayEffect("MS/Sound/EfLoadingOpen.mp3");
	return pLayer;
}

CMenuLoading::CMenuLoading()
{
	scheduleOnce(schedule_selector(CMenuLoading::Loading), 0.90f);
}

CMenuLoading::~CMenuLoading()
{

}

void CMenuLoading::Loading( float _t )
{
	((ccbGameLoadingMenu*)m_pNode)->Loading();
	CMainMenu::AllAdd(m_pScene);
	scheduleOnce(schedule_selector(CMenuLoading::Loading1), 0.50f);
}

void CMenuLoading::Loading1( float _t )
{
	//AppDelegate::AudioPlayEffect("MS/Sound/BGMLoadingLoad.mp3");
	if ( m_iLevel > -1 )
	{
		((ccbLayer*)CMainMenu::GetMenuMain())->m_iLevel = m_iLevel;
		((ccbLayer*)CMainMenu::GetMenuMain())->SetSpriteDisable(false);
	}

	((ccbLayer*)CMainMenu::GetMenuMain())->AllMoveWithBottom();
	scheduleOnce(schedule_selector(CMenuLoading::LoadingOver), 2.50f);
}
//
//void CMenuLoading::Loading2( float _t )
//{
//	((ccbGameLoadingMenu*)m_pNode)->Loading();
//	scheduleOnce(schedule_selector(CMenuLoading::LoadingOver), 1.50f);
//}

void CMenuLoading::LoadingOver( float _t )
{
	((ccbGameLoadingMenu*)m_pNode)->Disappear();
	//AppDelegate::AudioPlayEffect("MS/Sound/EfLoadingClose.mp3");
	scheduleOnce(schedule_selector(CMenuLoading::Start), 0.80f);
	m_pScene->removeChildByTag(2);
}

void CMenuLoading::Start( float _t )
{
	m_pScene->removeChildByTag(4);
	m_pScene->removeChildByTag(3);
	CMainMenu::SetDifficult(m_iDifficult);
	if ( m_iLevel > -1 )
	{
		if ( m_bShop )
		{
			if ( m_iDifficult > -1 && m_iLevel < 6 )
				CMainMenu::SetStatus(2);
			else
			{
				((ccbLayer*)CMainMenu::GetMenuMain())->m_iLevel = 0;
				CMainMenu::SetStatus(0);
			}
			//AppDelegate::AudioPlayBgm("MS/Music/shop.mp3");
			((ccbShopMenu*)CMainMenu::GetMenuShop())->Appear();
		}
		else
		{
			CMainMenu::SetStatus(2);
			CMainMenu::SetDifficult(m_iDifficult);
#ifndef GameTypeC
			AppDelegate::AudioPlayBgm("MS/Music/mainmenu/LevelChose.mp3");
#else
			AppDelegate::AudioPlayBgm("MS/Music/mainmenu/mainmenu.mp3");
#endif
			((ccbLayer*)CMainMenu::GetMenuMain())->BottomMove();
			((ccbLayer*)CMainMenu::GetMenuMain())->Back();
			((ccbLevelMenu*)CMainMenu::GetMenuLevel())->Appear();
		}
	}
	else
	{
		CMainMenu::SetStatus(0);
		//AppDelegate::AudioPlayBgm("MS/Music/ChoseLevel.mp3");
		((ccbLayer*)CMainMenu::GetMenuMain())->AllBackWithBottom();
	}
}
