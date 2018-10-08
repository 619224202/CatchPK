#include "GameoverBtnMenu.h"
#include "MainMenu.h"
#include "AppDelegate.h"
#include "Common.h"
#include "AchieveAdd.h"
#include "ChoseHero.h"
#include "FirstLogin.h"
#include "PayService.h"
#include "ShopMenu.h"
#include "GetItemLayer.h"

using namespace cocos2d;
using namespace cocos2d::extension;

ccbGameoverBtnMenu* ccbGameoverBtnMenu::s_pccbGameoverBtnMenu = NULL;

ccbGameoverBtnMenu::ccbGameoverBtnMenu()
	: m_AnimationManager(NULL)
{
	s_pccbGameoverBtnMenu = this;
	m_bOnSell = false;
}

ccbGameoverBtnMenu::~ccbGameoverBtnMenu()
{
	s_pccbGameoverBtnMenu = NULL;
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbGameoverBtnMenu::onNodeLoaded(cocos2d::CCNode * _pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	bool bAchieve = false;
	//CCNode* pNode = _pNode->getChildByTag(1);
	//for ( int i = 0; i < 59; i++ )
	//{
	//	if ( AchieveAdd::GetNumNow(i) == AchieveAdd::GetNumNeed(i) && !AppDelegate::s_Achieve[i] )
	//	{
	//		bAchieve = true;
	//		break;
	//	}
	//}
	//if ( bAchieve )
	//{
	//	CCMenuItemImage* pBtn = CCMenuItemImage::create( 
	//		"tu7/jiangli/di.png", "tu7/jiangli/di.png", this, menu_selector(ccbGameoverBtnMenu::OnGetAchieve));
	//	pBtn->setPosition(ccp(0,0));
	//	CCMenu* pMenu = CCMenu::create(pBtn, NULL);
	//	pMenu->setPosition(ccp(0,0));
	//	pNode->addChild(pMenu);

	//	CCSprite* pSprite = CCSprite::create("tu7/jiangli/1.png");
	//	pSprite->setPosition(ccp(0,0));
	//	CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("tu5/anniu/", 3, CCRect(0,0,115,85)));
	//	pSprite->runAction(CCRepeatForever::create(CCSequence::create(pAction, CCDelayTime::create(2.0f), NULL)));
	//	pNode->addChild(pSprite);
	//}

	//CCLog("-------------------create Init-----------");
	
}

SEL_MenuHandler ccbGameoverBtnMenu::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbGameoverBtnMenu::OnContinue );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbGameoverBtnMenu::OnShop );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On3", ccbGameoverBtnMenu::OnReplay );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On4", ccbGameoverBtnMenu::OnReturn );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On5", ccbGameoverBtnMenu::OnRechoseHero );
	return NULL;    
}

SEL_CCControlHandler ccbGameoverBtnMenu::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbGameoverBtnMenu::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbGameoverBtnMenu::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbGameoverBtnMenu::setAnimationManager(CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pGameControler = _pGameControler;
}

void ccbGameoverBtnMenu::Win()
{
	//CCLog("----------win-------------");
	//m_AnimationManager->setAnimationCompletedCallback(this,callfunc_selector(ccbGameoverBtnMenu::appearOver));
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->appearOver();
	
}



void ccbGameoverBtnMenu::Lose()
{
	//CCLog("----------lose-------------");
	//m_AnimationManager->setAnimationCompletedCallback();
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->appearOver();
}

void ccbGameoverBtnMenu::OnContinue( cocos2d::CCObject *pSender )
{
	//CCLog("11111111continue Game>>>>>>>>>>>>>>>>>>");
	if ( m_pGameControler->m_bEndPress && pSender )
		return;
	if ( m_bOnSell )
		return;
	//int level = 0;
	//if ( m_pGameControler->m_unLevel < 49 )
	//	level = m_pGameControler->m_unLevel/6;
	//else
	//	level = m_pGameControler->m_unLevel-49;
#ifdef GameTypeA
#ifdef Plat_MM
	if ( AppDelegate::m_Sell1 == 0 && m_pGameControler->m_unLevel > 4 )
#else
	if ( AppDelegate::m_Sell1 == 0 )
#endif
	{
		Sell1();
	}
	else if ( m_pGameControler->m_unLevel < LevelLimit*6-1 )//测试,正式需修改
	{
		m_bOnSell = true;
#else
	if ( m_pGameControler->m_unLevel < 11 )//测试,正式需修改
	{
		m_bOnSell = true;
#endif
		AppDelegate::ChangeScene( CMainMenu::StartGame(m_pGameControler->m_unLevel+1, m_pGameControler->m_iDifficult) );
	}
	else
	{
		m_bOnSell = true;
		AppDelegate::ChangeScene( CMainMenu::StartMenu() );
	}
}

void ccbGameoverBtnMenu::OnShop( cocos2d::CCObject *pSender )
{
	if ( m_pGameControler->m_bEndPress && pSender )
		return;
	if ( m_bOnSell )
		return;
	m_bOnSell = true;
	int level = m_pGameControler->m_unLevel/6;
	AppDelegate::ChangeScene( CMainMenu::StartMenu(level, m_pGameControler->m_iDifficult, true) );
}

void ccbGameoverBtnMenu::OnReplay( cocos2d::CCObject *pSender )
{
	if ( m_pGameControler->m_bEndPress && pSender )
		return;
	if ( m_bOnSell )
		return;
	m_bOnSell = true;
	AppDelegate::ChangeScene( CMainMenu::StartGame(m_pGameControler->m_unLevel, m_pGameControler->m_iDifficult) );
}

void ccbGameoverBtnMenu::OnReturn(cocos2d::CCObject *pSender)
{
	if ( m_pGameControler->m_bEndPress && pSender )
		return;
	if ( m_bOnSell )
		return;
	m_bOnSell = true;
	m_pGameControler->GameOver(0.0f);
	m_pGameControler->getChildByTag(1001)->setVisible(false);
	setKeypadEnabled(false);
}

void ccbGameoverBtnMenu::OnRechoseHero( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_pGameControler->m_bEndPress && pSender )
		return;
	if ( m_bOnSell )
		return;
	m_bOnSell = true;
	AppDelegate::ChangeScene( ccbChoseHero::CreateScene() );
}

void ccbGameoverBtnMenu::OnGetAchieve( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_pGameControler->m_bEndPress && pSender )
		return;
	if ( m_bOnSell )
		return;
	m_bOnSell = true;
	AppDelegate::ChangeScene(CFirstLogin::CreateScene(true, 1));
}

void ccbGameoverBtnMenu::ReturnCallBack( float _t )
{
}

void ccbGameoverBtnMenu::Sell1()
{
#ifdef Plat_Telecom_Demo
	Sell1Resume();
	return;
#endif
	if ( m_bOnSell )
		return;
	m_bOnSell = true;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	//pCCLayerColor->setPosition(ccp(0,0));
	//SetScale(pCCLayerColor);
	m_pGameControler->addChild(pCCLayerColor, 49, 694);
	CCMenuItem* pItem1 = CCMenuItemImage::create("sell/huode2-1.png", "sell/huode2-2.png", this, menu_selector(ccbGameoverBtnMenu::Sell1Resume));
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbGameoverBtnMenu::Sell1Back));
	pItem1->setPosition(ccp(725,195));
	pItem2->setPosition(ccp(728,437));
	CCMenu* pMenu = CCMenu::create(pItem1, pItem2, NULL);
	pMenu->setPosition(ccp(0,0));
	m_pGameControler->addChild(pMenu, 50, 697);

	CCSprite* pSprite1 = CCSprite::create("sell/di1.png");
	pSprite1->setPosition(ccp(400, 240));
	m_pGameControler->addChild(pSprite1, 51, 699);
	CCSprite* pSprite4 = CCSprite::create("sell/vip/tu.png");
	pSprite4->setPosition(ccp(400, 240));
	m_pGameControler->addChild(pSprite4, 51, 698);
	CCSprite* pSprite2 = common::CreateAnimation("sell/vip/meizi1.png", ccp(265, 190), ccp(0.5f, 0.5f),
		"sell/vip/meizi", 2, CCRect(0,0,282,284), true);
	m_pGameControler->addChild(pSprite2, 51, 696);
	CCSprite* pSprite3 = CCSprite::create("sell/vip/dang.png");
	pSprite3->setPosition(ccp(265, 50));
	m_pGameControler->addChild(pSprite3, 51, 695);

	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->isInSell = true;
	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->sellIndex = 0;
	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->setHandelPos(ccp(725,175));
}

void ccbGameoverBtnMenu::Sell1Resume( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(4);
	}
	else
	{
#ifndef Plat_Telecom_Demo
		AppDelegate::m_Sell1		= 1;
		AppDelegate::s_WeaponOwn[6] = 1;
		AppDelegate::s_WeaponOwn[9] = 1;
		if ( AppDelegate::s_HeroType == 0 )
		{
			AppDelegate::s_WeaponUse[0] = 6;
			AppDelegate::s_WeaponUse[1] = 9;
		}
		AppDelegate::s_BulletNum[9] += 100;
		if ( AppDelegate::s_BulletNum[9] > 999 )
			AppDelegate::s_BulletNum[9] = 999;
		AppDelegate::s_Healbox = 3;
		AppDelegate::s_GrenadeNum = 10;
		AppDelegate::s_Money += 9999;
		AppDelegate::SaveStatus();
#endif
		scheduleOnce(schedule_selector(ccbGameoverBtnMenu::Sell1Enter), 0.1f);
	}
}

void ccbGameoverBtnMenu::Sell1Enter(float _t)
{
	AppDelegate::ChangeScene( CMainMenu::StartGame(m_pGameControler->m_unLevel+1, m_pGameControler->m_iDifficult) );
}

void ccbGameoverBtnMenu::Sell1Back( cocos2d::CCObject *pSender /*= NULL*/ )
{
	m_bOnSell = false;
	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->isInSell = false;
	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->setHandelPos(-1);

	if ( m_pGameControler->getChildByTag(694) )
		m_pGameControler->getChildByTag(694)->removeFromParentAndCleanup(true);
	if ( m_pGameControler->getChildByTag(696) )
		m_pGameControler->getChildByTag(696)->removeFromParentAndCleanup(true);
	if ( m_pGameControler->getChildByTag(695) )
		m_pGameControler->getChildByTag(695)->removeFromParentAndCleanup(true);
	if ( m_pGameControler->getChildByTag(697) )
		m_pGameControler->getChildByTag(697)->removeFromParentAndCleanup(true);
	if ( m_pGameControler->getChildByTag(698) )
		m_pGameControler->getChildByTag(698)->removeFromParentAndCleanup(true);
	if ( m_pGameControler->getChildByTag(699) )
		m_pGameControler->getChildByTag(699)->removeFromParentAndCleanup(true);
	
}


