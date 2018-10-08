#include "GetItemLayer.h"
#include "AppDelegate.h"
#include "Common.h"
#include "ShopMenu.h"
#include "baseRes.h"
#include "GameControler.h"
#include "GameoverBtnMenu.h"
#include "AchieveAdd.h"
#include "PayService.h"

using namespace cocos2d;
using namespace cocos2d::extension;

static int _Type[] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 
	2, 2, 2, 2, 2, 2, 2, 2, 2, 
	3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6,
	7, 8, 9 ,10, 11, 6, 6, 6,
};
static int _Num[] =
{
	10, 50, 100, 200, 500, 1000, 2000, 5000,
	1, 2, 3, 5, 10, 20, 30, 50, 100,
	5, 10, 1, 2, 3, 1, 2, 5, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};
static int _Probability[] =
{
	100, 100, 40, 30, 20, 10, 1, 1,
	100, 100, 40, 30, 20, 10, 5, 1, 1,
	5, 1, 100, 20, 10, 5, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 100, 0, 0, 0,
};

#define d_fGameOverNumLocate1	654, 388
#define d_fGameOverNumLocate2	619, 301
#define d_fGameOverNumLocate4	573, 205
#define d_fGameOverNumLocate31	639, 280
#define d_fGameOverNumLocate32	660, 280
#define d_fGameOverNumLocate33	681, 280

ccbGetItemLayer* ccbGetItemLayer::s_pccbGetItemLayer = NULL;

ccbGetItemLayer::ccbGetItemLayer()
	: m_AnimationManager(NULL)
{
	m_bOnSell		= false;
	m_bWin			= false;
	m_iWeapon		= 0;
	m_pSprite		= NULL;
	m_pGetPrize		= NULL;
	m_bCanPress		= false;
	m_bIsWeapon		= true;
	s_pccbGetItemLayer = this;

	isInSell = false;
	sellIndex = 0;
	isInMain = false;
	isInRecharge = false;
	settleIndex = 0;
	rechargeIndex = 0;

	mainPos[0][0] = 537;
	mainPos[0][1] = 257;
	mainPos[1][0] = 209;
	mainPos[1][1] = 72;
	mainPos[2][0] = 367;
	mainPos[2][1] = 72;
	mainPos[3][0] = 525;
	mainPos[3][1] = 72;
	mainPos[4][0] = 730;
	mainPos[4][1] = 210;
	mainPos[5][0] = 730;
	mainPos[5][1] = 140;
	mainPos[6][0] = 730;
	mainPos[6][1] = 70;
	mainPos[7][0] = 730;
	mainPos[7][1] = 0;

	butPos[0][0] = 128;
	butPos[0][1] = 130;
	butPos[1][0] = 318;
	butPos[1][1] = 130;
	butPos[2][0] = 89;
	butPos[2][1] = 25;
	butPos[3][0] = 279;
	butPos[3][1] = 25;
}

ccbGetItemLayer::~ccbGetItemLayer()
{
	s_pccbGetItemLayer = NULL;
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbGetItemLayer::onNodeLoaded(cocos2d::CCNode * _pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{	
	m_pNode = _pNode;
	bool bAchieve = false;
	CCNode* pNode = _pNode->getChildByTag(7);
	for ( int i = 0; i < 59; i++ )
	{
		if ( AchieveAdd::GetNumNow(i) == AchieveAdd::GetNumNeed(i) && !AppDelegate::s_Achieve[i] )
		{
			bAchieve = true;
			break;
		}
	}
	if ( bAchieve )
	{
		CCMenuItemImage* pBtn = CCMenuItemImage::create( 
			"tu7/jiangli/di.png", "tu7/jiangli/di.png", this, menu_selector(ccbGetItemLayer::OnGetAchieve));
		pBtn->setPosition(ccp(0,0));
		CCMenu* pMenu = CCMenu::create(pBtn, NULL);
		pMenu->setPosition(ccp(0,0));
		pNode->addChild(pMenu);

		CCSprite* pSprite = CCSprite::create("tu7/jiangli/1.png");
		pSprite->setPosition(ccp(0,0));
		CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("tu5/anniu/", 3, CCRect(0,0,115,85)));
		pSprite->runAction(CCRepeatForever::create(CCSequence::create(pAction, CCDelayTime::create(2.0f), NULL)));
		pNode->addChild(pSprite);
	}

	CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ccbGetPrizeLayer", ccbGetPrizeLayerLoader::loader());
	cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	m_pGetPrize = (ccbGetPrizeLayer*)ccbReader->readNodeGraphFromFile("23.ccbi"/*, this*/);
	m_pGetPrize->setAnimationManager(ccbReader->getAnimationManager());
	m_pGetPrize->setPosition(ccp(0,0));
	addChild(m_pGetPrize, 5);
	ccbReader->release();
	m_pGetPrize->m_bOver = true;
}

SEL_MenuHandler ccbGetItemLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbGetItemLayer::OnBuy1 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbGetItemLayer::OnBuy2 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On3", ccbGetItemLayer::OnBuy3 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On4", ccbGetItemLayer::OnBuy4 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On5", ccbGetItemLayer::DisAppear );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On6", ccbGetItemLayer::OnShop );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On7", ccbGetItemLayer::OnContinue );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On8", ccbGetItemLayer::OnReturn );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On9", ccbGetItemLayer::OnReplay );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On10", ccbGetItemLayer::OnRechoseHero );
	return NULL;    
}

SEL_CCControlHandler ccbGetItemLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}

bool ccbGetItemLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbGetItemLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbGetItemLayer::setAnimationManager(CCBAnimationManager *pAnimationManager, cocos2d::CCNode* _pNode)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pGameControler = _pNode;
}

//void ccbGetItemLayer::SetGame( CGameControler* p )
//{
//	m_pGameControler = p;
//}

void ccbGetItemLayer::StartBtn()
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2 copy", 0.0f);
}

void ccbGetItemLayer::OnBuy1( cocos2d::CCObject *pSender )
{
//	CCLog("buy1");
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	if ( !m_bCanPress )
		return;
	if ( m_iWeapon == 0 && !m_bIsWeapon )
	{
		if ( AppDelegate::s_Medal < 30 )
		{
			GetMedal();
			return;
		}
		AppDelegate::s_Medal -= 30;
		m_iGetPrize = 0;
		int num = rand()%50+1;
		while ( num )
		{
			for ( int i = 0; i < 25; i++ )
			{
				if ( _Probability[i] != 0 )
				{
					m_iGetPrize = i;
					num--;
					if ( !num )
						break;
				}
			}
		}
		m_pGetPrize->initPrize(m_iGetPrize);
		m_pGetPrize->Appear();
		schedule(schedule_selector(ccbGetItemLayer::GetPrizeCheck), 0.02f);
	}
	else if ( m_iWeapon )
	{
		bool successd = false;
		if ( m_iType == 0 )
		{
#ifdef Plat_Telecom_Demo
			AppDelegate::s_VIP = 1;
			AppDelegate::SaveVIP();
			successd = true;
#else
			if ( pSender )
			{
				PayService::pay(11);
			}
			else
			{
				successd = true;
			}
#endif
		}
		else if ( m_iType == 1 )
		{
			int sell1 = 1;
			int sell2 = 1;
			if ( ccbShopMenu::WeaponOnSell(m_iWeapon) )
			{
				sell1 = 5;
				sell2 = 4;
			}
			int cost = g_priceUpgrade[m_iWeapon]*sell2/sell1;
			if ( g_weaponUseMedal[m_iWeapon] )
			{
				if ( AppDelegate::s_Money >= cost )
				{
					AppDelegate::s_Money -= cost;
					successd = true;
				}
				else
				{
					GetMoney();
					return;
				}
			}
			else
			{
				if ( AppDelegate::s_Medal >= cost )
				{
					AppDelegate::s_Medal -= cost;
					successd = true;
				}
				else
				{
					GetMedal();
					return;
				}
			}
			if ( successd )
			{
				AppDelegate::s_WeaponUse[1] = m_iWeapon;
				CCNode* pNode = m_pNode->getChildByTag(3);
				common::ShowNumber(pNode, AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]], 19, 24, 78, 57, "tu3/zidansuzi.png", 888, 0, 0.7f);
				common::ShowNumber(pNode, g_priceBullet[AppDelegate::s_WeaponUse[1]], 19, 24, 70, 24, "tu3/zidansuzi.png", 889, 0, 0.8f);
				AppDelegate::AudioPlayEffect("MS2/buy.mp3");
				AppDelegate::s_WeaponOwn[m_iWeapon] = 1;
				CCNode* pSprite = m_pNode->getChildByTag(1)->getChildByTag(1);
				//pSprite->removeFromParentAndCleanup(true);
				//pSprite->stopAllActions();
				//pSprite->setScale(1.5f);
				m_iWeapon = 0;
			}
			else
			{//失败提示

			}
		}
		if ( successd )
		{
			m_pNode->getChildByTag(5)->setVisible(false);
		}
	}

}

void ccbGetItemLayer::OnBuy2( cocos2d::CCObject *pSender )
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	if ( AppDelegate::s_Healbox > 2 || !m_bCanPress )
		return;
	if ( AppDelegate::s_Money < 300 )
	{
		GetMoney();
		return;
	}
	AppDelegate::AudioPlayEffect("MS2/buy.mp3");
	AppDelegate::s_Healbox ++;
	AppDelegate::s_Money -= 300;
	common::ShowNumberUpdate(m_pNode->getChildByTag(2), AppDelegate::s_Healbox, 888);
	CCNode* pNode = m_pNode->getChildByTag(2)->getChildByTag(888);
	pNode->stopAllActions();
	pNode->setScale(1.5f);
	CCActionInterval* pAction2 = CCScaleTo::create(0.2f, 1.0f);
	pNode->runAction( pAction2 );
}

void ccbGetItemLayer::OnBuy3( cocos2d::CCObject *pSender )
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] >= 999 || !m_bCanPress )
		return;
	if ( AppDelegate::s_Money < g_priceBullet[AppDelegate::s_WeaponUse[1]] )
	{
		GetMoney();
		return;
	}
	AppDelegate::AudioPlayEffect("MS2/buy.mp3");
	AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
	if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
		AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
	AppDelegate::s_Money -= g_priceBullet[AppDelegate::s_WeaponUse[1]];
	common::ShowNumberUpdate(m_pNode->getChildByTag(3), AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]], 888);
	CCNode* pNode = m_pNode->getChildByTag(3)->getChildByTag(888);
	pNode->stopAllActions();
	pNode->setScale(1.5f);
	CCActionInterval* pAction2 = CCScaleTo::create(0.2f, 1.0f);
	pNode->runAction( pAction2 );
}

void ccbGetItemLayer::OnBuy4( cocos2d::CCObject *pSender )
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	if ( AppDelegate::s_GrenadeNum >= d_iGrenadeMax || !m_bCanPress )
		return;
	if ( AppDelegate::s_Money < 260 )
	{
		GetMoney();
		return;
	}
	AppDelegate::AudioPlayEffect("MS2/buy.mp3");
	AppDelegate::s_GrenadeNum ++;
	AppDelegate::s_Money -= 260;
	common::ShowNumberUpdate(m_pNode->getChildByTag(4), AppDelegate::s_GrenadeNum, 888);
	CCNode* pNode = m_pNode->getChildByTag(4)->getChildByTag(888);
	pNode->stopAllActions();
	pNode->setScale(1.5f);
	CCActionInterval* pAction2 = CCScaleTo::create(0.2f, 1.0f);
	pNode->runAction( pAction2 );
}

void ccbGetItemLayer::InitBtn(int _type)
{
	setVisible(true);
	m_bCanPress = true;
	if ( _type )
		m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
	else
		m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline3", 0.0f);
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	addChild(pCCLayerColor, -1, 10);

	{//左边
		CCNode* pNode = m_pNode->getChildByTag(2);
		common::ShowNumber(pNode, AppDelegate::s_Healbox, 19, 24, 94, 59, "tu3/zidansuzi.png", 888, 0, 0.8f);
		common::ShowNumber(pNode, 300, 19, 24, 70, 24, "tu3/zidansuzi.png", 889, 0, 0.8f);

		pNode = m_pNode->getChildByTag(3);
		common::ShowNumber(pNode, AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]], 19, 24, 78, 57, "tu3/zidansuzi.png", 888, 0, 0.7f);
		common::ShowNumber(pNode, g_priceBullet[AppDelegate::s_WeaponUse[1]], 19, 24, 70, 24, "tu3/zidansuzi.png", 889, 0, 0.8f);

		pNode = m_pNode->getChildByTag(4);
		common::ShowNumber(pNode, AppDelegate::s_GrenadeNum, 19, 24, 83, 57, "tu3/zidansuzi.png", 888, 0, 0.8f);
		common::ShowNumber(pNode, 260, 19, 24, 70, 24, "tu3/zidansuzi.png", 889, 0, 0.8f);
	}
	setKeypadEnabled(true);
}

void ccbGetItemLayer::Appear1(int _type)
{
	if ( AppDelegate::s_VIP )
	{
		Appear2(1);
		return;
	}
	m_iType = 0;
	m_iWeapon = 1;
	CCNode* pNode = m_pNode->getChildByTag(1);

	CCSprite* pSpriteBoard = CCSprite::create("tu18/shang2.png");
	pNode->addChild(pSpriteBoard);

	{
		CCSprite* pMovie1 = CCSprite::create("tu18/te.png");
		pMovie1->setPosition(ccp(245, 95));
		pSpriteBoard->addChild(pMovie1);
		CCDelayTime* pDelay1 = CCDelayTime::create(0.4f);
		CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 1.5f);
		CCScaleTo* pAction2 = CCScaleTo::create(0.3f, 1.0f);
		CCDelayTime* pDelay2 = CCDelayTime::create(1.2f);
		pMovie1->runAction(CCRepeatForever::create(CCSequence::create(pDelay1, pAction1, pAction2, pDelay2, NULL)));
	}
	{
		CCSprite* pMovie1 = CCSprite::create("tu18/vip.png");
		pMovie1->setPosition(ccp(168, 98));
		pSpriteBoard->addChild(pMovie1);
		CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 1.5f);
		CCScaleTo* pAction2 = CCScaleTo::create(0.3f, 1.0f);
		CCDelayTime* pDelay1 = CCDelayTime::create(1.6f);
		pMovie1->runAction(CCRepeatForever::create(CCSequence::create(pAction1, pAction2, pDelay1, NULL)));
	}
	{
		CCSprite* pSprite1 = CCSprite::create("tu18/x2.png");
		pSprite1->setPosition(ccp(261, 51));
		pSpriteBoard->addChild(pSprite1);

		CCSprite* pSprite2 = CCSprite::create("tu18/yongyou1.png");
		pSprite2->setPosition(ccp(388, 42));
		pSpriteBoard->addChild(pSprite2);
	}
	InitBtn(_type);
}

void ccbGetItemLayer::Appear2(int _type)
{//枪
	m_iType = 1;
	m_iWeapon = 0;
	CCNode* pNode = m_pNode->getChildByTag(1);
	for ( int i = 12; i > 4; i-- )
	{
		if ( i == 6 || i == 7 )
			continue;
		if ( AppDelegate::s_HeroType != ccbShopMenu::Exclusive(i) && -1 != ccbShopMenu::Exclusive(i) )
			continue;
		if ( AppDelegate::s_WeaponOwn[i] == 0 )
			m_iWeapon = i;
		if ( AppDelegate::s_WeaponOwn[i] == 1 && m_iWeapon )
			break;
	}

	if ( m_iWeapon )
	{
		CCSprite* pSpriteBoard = CCSprite::create("tu18/shang.png");
		pNode->addChild(pSpriteBoard);

		CCSprite* pSprite2 = CCSprite::create("tu18/yongyou1.png");
		pSprite2->setPosition(ccp(388, 42));
		pSpriteBoard->addChild(pSprite2);

		char buffer[50];
		sprintf(buffer, "tu3/qiang/%d.png", m_iWeapon);
		CCSprite* pSprite = CCSprite::create(buffer);
		pSprite->setScale(0.7f);
		pSprite->setPosition(ccp(-50, 0));
		pNode->addChild(pSprite, 1, 1);
		CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 1.1f);
		CCScaleTo* pAction2 = CCScaleTo::create(0.3f, 0.7f);
		pSprite->runAction(CCRepeatForever::create(CCSequence::create(pAction1, pAction2, CCDelayTime::create(1.0f), NULL)));

		sprintf(buffer, "tu5/name/%d.png", m_iWeapon);
		pSprite = CCSprite::create(buffer);
		pSprite->setPosition(ccp(-108, 52));
		pNode->addChild(pSprite);

		int sell1 = 1;
		int sell2 = 1;
		if ( ccbShopMenu::WeaponOnSell(m_iWeapon) )
		{
			sell1 = 5;
			sell2 = 4;
		}
		if ( g_weaponUseMedal[m_iWeapon] )
			pSprite = CCSprite::create("tu3/qian.png");
		else
			pSprite = CCSprite::create("tu3/xunzhang.png");
		pSprite->setAnchorPoint(ccp(0.5f, 0.25f));
		pSprite->setPosition(ccp(98,48));
		pNode->addChild(pSprite, 2, 2);
		common::ShowNumber(pSprite, g_priceUpgrade[m_iWeapon]*sell2/sell1, 19, 24, 41, 5, "tu3/zidansuzi2.png", 888, 0, 0.8f);

		//pSprite = CCSprite::create("tu3/weili.png");
		//pSprite->setPosition(ccp(84,115));
		//pSprite->setRotation(6);
		//pNode->addChild(pSprite, 2, 3);
		int d = AppDelegate::WeaponDamage(m_iWeapon);
		int sp = AppDelegate::WeaponSpeed(m_iWeapon);
		if ( sp == 0 )
			sp = 100;
		common::ShowNumber(pSprite, d*100/sp, 19, 24, 79, -33, "tu3/zidansuzi.png", 889, 0, 0.8f);
	}
	else
	{
		m_bIsWeapon = false;
		CCSprite* pSpriteBoard = CCSprite::create("tu18/shang3.png");
		pNode->addChild(pSpriteBoard);
	}
	InitBtn(_type);
}

void ccbGetItemLayer::DisAppear(cocos2d::CCObject *pSender)
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	if ( !m_bCanPress )
		return;
	((CGameControler*)m_pGameControler)->m_bEndPress = false;
	m_bCanPress = false;
	removeChildByTag(10);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
	settleIndex = 0;
	isInMain = true;
	setHandelPos(ccp(butPos[settleIndex][0],butPos[settleIndex][1]));
}

void ccbGetItemLayer::OnShop( cocos2d::CCObject *pSender )
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->OnShop(NULL);
}

void ccbGetItemLayer::OnContinue( cocos2d::CCObject *pSender )
{
	
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->OnContinue(NULL);
}

void ccbGetItemLayer::OnReturn( cocos2d::CCObject *pSender )
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	unschedule(schedule_selector(ccbGetItemLayer::Timer));
	((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->OnReturn(NULL);
}

void ccbGetItemLayer::OnReplay( cocos2d::CCObject *pSender )
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->OnReplay(NULL);
}

void ccbGetItemLayer::Timer( float _t )
{

}

void ccbGetItemLayer::OnRechoseHero( cocos2d::CCObject *pSender )
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	m_bOnSell = true;
	((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->OnRechoseHero(NULL);
}

void ccbGetItemLayer::OnGetAchieve( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	m_bOnSell = true;
	((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->OnGetAchieve(NULL);
}

void ccbGetItemLayer::GetMoney( cocos2d::CCObject* sender )
{
#ifdef NoGetMoney
	return;
#endif
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	m_bOnSell = true;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	addChild(pCCLayerColor, 29, 1195);

	CCMenuItem* pItem = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbGetItemLayer::GetRemove));
	pItem->setPosition(ccp(723,351));
	CCMenu* pMenu = CCMenu::create(pItem, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 31, 1196);

	CCSprite* pSprite1 = CCSprite::create("sell/money/di.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 31, 1199);
	CCSprite* pSprite2 = CCSprite::create("sell/money/2.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 33, 1198);

	//CCMenuItem* pItem1 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMoneyBtn));
	//pItem1->setPosition(ccp(219,282));
	//pItem1->setTag(1);
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMoneyBtn));
	pItem2->setPosition(ccp(243,199));
	pItem2->setTag(2);
	CCMenuItem* pItem3 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMoneyBtn));
	pItem3->setPosition(ccp(556,199));
	pItem3->setTag(3);
	//CCMenuItem* pItem4 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMoneyBtn));
	//pItem4->setPosition(ccp(286,96));
	//pItem4->setTag(4);
	//CCMenuItem* pItem5 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMoneyBtn));
	//pItem5->setPosition(ccp(512,96));
	//pItem5->setTag(5);
	CCMenu* pMenu1 = CCMenu::create(/*pItem1, */pItem2, pItem3, /*pItem4, pItem5, */NULL);
	pMenu1->setPosition(ccp(0,0));
	addChild(pMenu1, 32, 1197);

	isInRecharge = true;
	rechargeIndex = 0;
	setHandelPos(ccp(267,151));
	isGetMedal = false;
}

void ccbGetItemLayer::GetMedal( cocos2d::CCObject* sender )
{
#ifdef NoGetMoney
	return;
#endif
	if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->m_bOnSell )
		return;
	if ( m_bOnSell || !m_pGetPrize->m_bOver )
		return;
	m_bOnSell = true;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	addChild(pCCLayerColor, 29, 1195);

	CCMenuItem* pItem = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbGetItemLayer::GetRemove));
	pItem->setPosition(ccp(723,351));
	CCMenu* pMenu = CCMenu::create(pItem, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 31, 1196);

	CCSprite* pSprite1 = CCSprite::create("sell/money/di.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 31, 1199);
	CCSprite* pSprite2 = CCSprite::create("sell/money/1.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 33, 1198);

	//CCMenuItem* pItem1 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMedalBtn));
	//pItem1->setPosition(ccp(219,282));
	//pItem1->setTag(1);
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMedalBtn));
	pItem2->setPosition(ccp(243,199));
	pItem2->setTag(2);
	CCMenuItem* pItem3 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMedalBtn));
	pItem3->setPosition(ccp(556,199));
	pItem3->setTag(3);
	//CCMenuItem* pItem4 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMedalBtn));
	//pItem4->setPosition(ccp(283,96));
	//pItem4->setTag(4);
	//CCMenuItem* pItem5 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbGetItemLayer::GetMedalBtn));
	//pItem5->setPosition(ccp(512,96));
	//pItem5->setTag(5);
	CCMenu* pMenu1 = CCMenu::create(/*pItem1, */pItem2, pItem3, /*pItem4, pItem5, */NULL);
	pMenu1->setPosition(ccp(0,0));
	addChild(pMenu1, 32, 1197);

	isInRecharge = true;
	rechargeIndex = 0;
	setHandelPos(ccp(267,151));
	isGetMedal = true;
}

void ccbGetItemLayer::GetMoneyBtn( cocos2d::CCObject* _pNode )
{
	if ( _pNode )
	{
		int numAdd = 0;
		int numMid = 0;
		int index = 0;
		if(rechargeIndex==0){
			index = 2;
		}else{
			index = 3;
		}
		PayService::pay(14+index);

		//switch ( index )
		//{
		//case 1:
		//	numAdd = 1080;
		//	numMid = 2;
		//	break;
		//case 2:
		//	numAdd = 6480;
		//	numMid = 14;
		//	break;
		//case 3:
		//	numAdd = 35280;
		//	numMid = 49;
		//	break;
		//case 4:
		//	numAdd = 80190;
		//	numMid = 99;
		//	break;
		//case 5:
		//	numAdd = 181440;
		//	numMid = 168;
		//	break;
		//}
		//AppDelegate::s_Money += numAdd;
		//AppDelegate::SaveMoney();
	}
	else
		GetRemove();
}

void ccbGetItemLayer::GetMedalBtn( cocos2d::CCObject* _pNode )
{
	if ( _pNode )
	{
		int numAdd = 0;
		int numMid = 0;
		int index = 0;
		if(rechargeIndex==0){
			index = 2;
		}else{
			index = 3;
		}
		PayService::pay(11+index);

		//switch ( index )
		//{
		//case 1:
		//	numAdd = 108;
		//	numMid = 2;
		//	break;
		//case 2:
		//	numAdd = 648;
		//	numMid = 14;
		//	break;
		//case 3:
		//	numAdd = 3528;
		//	numMid = 49;
		//	break;
		//case 4:
		//	numAdd = 8019;
		//	numMid = 99;
		//	break;
		//case 5:
		//	numAdd = 18144;
		//	numMid = 168;
		//	break;
		//}
		//AppDelegate::s_Medal += numAdd;
		//AppDelegate::SaveMedal();
	}
	else
		GetRemove();
}

void ccbGetItemLayer::GetRemove(cocos2d::CCObject* _pNode)
{
	if ( !m_bOnSell )
		return;
	m_bOnSell = false;
	isInRecharge = false;
	
	for ( int i = 1195; i < 1200; i++ )
	{
		if ( getChildByTag(i) )
			removeChildByTag(i);
	}
	if(isInSell){
		setHandelPos(ccp(butPos[settleIndex][0],butPos[settleIndex][1]));
		return;
	}
	if(!isInMain){
		setHandelPos(ccp(mainPos[mainButIndex][0],mainPos[mainButIndex][1]));
		return;
	}else{
		if(sellIndex==0){
			setHandelPos(ccp(725,175));
		}else{
			setHandelPos(ccp(752,407));
		}
	}
	
}

void ccbGetItemLayer::GetPrizeCheck( float _t )
{
	if ( m_pGetPrize->m_bOver )
	{

		switch ( _Type[m_iGetPrize] )
		{
		case 1:
			AppDelegate::s_Money += _Num[m_iGetPrize];
			//if ( AppDelegate::s_Money > 99999 )
			//	AppDelegate::s_Money = 99999;
			break;
		case 2:
			AppDelegate::s_Medal += _Num[m_iGetPrize];
			if ( AppDelegate::s_Medal > 99999 )
				AppDelegate::s_Medal = 99999;
			break;
		case 3:
			AppDelegate::s_GrenadeNum += _Num[m_iGetPrize];
			if ( AppDelegate::s_GrenadeNum > 10 )
				AppDelegate::s_GrenadeNum = 10;
			common::ShowNumberUpdate(m_pNode->getChildByTag(4), AppDelegate::s_GrenadeNum, 888);
			break;
		case 4:
			AppDelegate::s_Healbox += _Num[m_iGetPrize];
			if ( AppDelegate::s_Healbox > 3 )
				AppDelegate::s_Healbox = 3;
			common::ShowNumberUpdate(m_pNode->getChildByTag(2), AppDelegate::s_Healbox, 888);
			break;
		case 5:
			{
				int num = _Num[m_iGetPrize];
				if ( AppDelegate::s_SkillEnergy[0] < 100 )
				{
					AppDelegate::s_SkillEnergy[0] = 100;
					num--;
				}
				AppDelegate::m_Sell2 += num;
			}
			break;
		case 11:
			break;
		}
		AppDelegate::SaveStatus();
		unschedule(schedule_selector(ccbGetItemLayer::GetPrizeCheck));
	}
}

void ccbGetItemLayer::keyBackClicked()
{
	//AppDelegate::SaveAll();
	//CCDirector::sharedDirector()->end();
	//exit(0);
	if(isInRecharge){
		GetRemove();
		return;
	}
	if(isInMain){
		OnReturn(this);
		return;
	}

	if(!isInSell){
		//if()
		//DisAppear(this);
		OnReturn(this);
	}else{
		((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->Sell1Back();
	}
}

void ccbGetItemLayer::keyEnterClicked(){
	if(isInRecharge){
		if(rechargeIndex==0){
			if(isGetMedal){
				GetMedalBtn(this);
			}else{
				GetMoneyBtn(this);
			}
		}else{
			if(isGetMedal){
				GetMedalBtn(this);
			}else{
				GetMoneyBtn(this);
			}
		}
		return;
	}
	CCLog("按下了确定键");
	if(isInMain){
		switch(settleIndex){
		case 0:
			if(((CGameControler*)m_pGameControler)->m_bWin){
				OnContinue(this);
			}else{
				OnReplay(this);
			}
			break;
		case 1:
			OnShop(this);
			break;
		case 2:
			OnReturn(this);
			break;
		case 3:
			OnRechoseHero(this);
			break;
		}
		return;
	}
	if(!isInSell){
		switch(mainButIndex){
		case 0:
			OnBuy1();
			break;
		case 1:
			OnBuy2(this);
			break;
		case 2:
			OnBuy3(this);
			break;
		case 3:
			OnBuy4(this);
			break;
		case 4:
			if(((CGameControler*)m_pGameControler)->m_bWin){
				OnContinue(this);
			}else{
				OnReplay(this);
			}
			break;
		case 5:
			OnShop(this);
			break;
		case 6:
			OnRechoseHero(this);
			break;
		case 7:
			OnReturn(this);
			break;
		}
	}else{
		if(sellIndex==0){
			((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->Sell1Resume(this);
		}else{
			((ccbGameoverBtnMenu*)((CGameControler*)m_pGameControler)->m_pGameoverBtnNode)->Sell1Back(this);
		}
	}
}

void ccbGetItemLayer::keyArrowClicked(int arrow){
	//CCLog("111按下了方向键:%d",arrow);
	if(isInRecharge){
		switch(arrow){
		case kTypeLeftArrowClicked:
		case kTypeRightArrowClicked:
			if(rechargeIndex==0){
				rechargeIndex = 1;
			}else{
				rechargeIndex = 0;
			}
			if(rechargeIndex==0){
				setHandelPos(ccp(267,151));
			}else{
				setHandelPos(ccp(582,151));
			}
			break;
		}
		return;
	}
	if(isInMain){
		switch(arrow){
		case kTypeDownArrowClicked:
		case kTypeUpArrowClicked:
			if(settleIndex==0 || settleIndex==1){
				settleIndex +=2;
			}else{
				settleIndex-=2;
			}
			break;
		case kTypeLeftArrowClicked:
		case kTypeRightArrowClicked:
			if(settleIndex==0||settleIndex==2){
				settleIndex++;
			}else if(settleIndex==1 || settleIndex==3){
				settleIndex--;
			}
			break;
		}
		setHandelPos(ccp(butPos[settleIndex][0],butPos[settleIndex][1]));
		return;
	}
	if(!isInSell){
		switch(arrow){
		case kTypeLeftArrowClicked:
			if(mainButIndex>4){
				mainButIndex = 3;
			}else if(mainButIndex>1){
				mainButIndex --;
			}
			break;
		case kTypeRightArrowClicked:
			if(mainButIndex==0 || mainButIndex==3){
				mainButIndex = 4;
			}else if(mainButIndex<3){
				mainButIndex++;
			}
			break;
		case kTypeUpArrowClicked:
			if(mainButIndex>0 && mainButIndex<4){
				mainButIndex = 0;
			}else if(mainButIndex>4){
				mainButIndex --;
			}
			break;
		case kTypeDownArrowClicked:
			if(mainButIndex==0){
				mainButIndex = 1;
			}else if(mainButIndex>=4 && mainButIndex<7){
				mainButIndex ++;
			}
			break;
		}
		setHandelPos(ccp(mainPos[mainButIndex][0],mainPos[mainButIndex][1]));
	}else{
		switch(arrow){
		case kTypeUpArrowClicked:
		case kTypeDownArrowClicked:
			if(isInSell){
				if(sellIndex==0){
					sellIndex = 1;
				}else{
					sellIndex = 0;
				}
				if(sellIndex==0){
					setHandelPos(ccp(725,175));
				}else{
					setHandelPos(ccp(752,407));
				}
			}
		break;
		}
	}
}

void ccbGetItemLayer::keyNumClicked(int keyNum){
	//CCLog("按下了数字键:%d",keyNum);
	if(keyNum==kTypeNum0){
		keyBackClicked();
	}
}

void ccbGetItemLayer::setHandelPos(CCPoint point){
	m_pGameControler->getChildByTag(1001)->setPosition(point);
}

void ccbGetItemLayer::setHandelPos(int handelIndex){
	if(!isInMain){
		if(handelIndex==-1){
			handelIndex = mainButIndex;
		}
		setHandelPos(ccp(mainPos[handelIndex][0],mainPos[handelIndex][1]));
	}else{
		if(handelIndex==-1){
			handelIndex = settleIndex;
		}
		setHandelPos(ccp(butPos[handelIndex][0],butPos[handelIndex][1]));
	}
}

void ccbGetItemLayer::appearOver(){
	if(!m_pGameControler->getChildByTag(1001)){
		CCSprite* handelSprite = ccbShopMenu::createHandelCreator();
		m_pGameControler->addChild(handelSprite,100,1001);
		handelSprite->setPosition(ccp(0,0));
	}
	m_pGameControler->getChildByTag(1001)->setVisible(true);
	setKeypadEnabled(true);
	mainButIndex = 0;
	setHandelPos(ccp(mainPos[mainButIndex][0],mainPos[mainButIndex][1]));
	isAppearOver = true;
	//CCLog("appear over");
}

//void ccbGetItemLayer::LosePlay( float _t )
//{
//	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);
//}

