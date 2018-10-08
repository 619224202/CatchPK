#include "GameoverMenu.h"
#include "AppDelegate.h"
#include "GameoverBtnMenu.h"
#include "Common.h"
//#include "GameControler.h"
#include "ShopMenu.h"
#include "baseRes.h"
#include "GetItemLayer.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define d_fGameOverNumLocate1	654, 388
#define d_fGameOverNumLocate2	619, 301
#define d_fGameOverNumLocate4	573, 205
#define d_fGameOverNumLocate31	639, 280
#define d_fGameOverNumLocate32	660, 280
#define d_fGameOverNumLocate33	681, 280

ccbGameoverMenu::ccbGameoverMenu()
	: m_AnimationManager(NULL)
{
	m_bWin			= false;
	m_bNowKill		= false;
	m_bNowMedal		= false;
	m_bNowGold		= false;
	m_bComboMoney	= false;
	m_bStartAddCombo= false;
	m_bOverAddCombo	= false;
	m_iWeapon		= 0;
	m_iNowKill		= 0;
	m_iNowMedal		= 0;
	m_iNowGold		= 0;
	m_iStateNowTime = 0;
	m_pSprite		= NULL;
}

ccbGameoverMenu::~ccbGameoverMenu()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbGameoverMenu::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	m_pNode = pNode;
	m_pBoard = pNode->getChildByTag(8);
}

SEL_MenuHandler ccbGameoverMenu::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbGameoverMenu::OnShop );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbGameoverMenu::OnReturn );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On3", ccbGameoverMenu::OnReplay );
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On901", ccbGameoverMenu::OnBuy1 );
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On902", ccbGameoverMenu::OnBuy2 );
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On903", ccbGameoverMenu::OnBuy3 );
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On904", ccbGameoverMenu::OnBuy4 );
	return NULL;    
}

SEL_CCControlHandler ccbGameoverMenu::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}

bool ccbGameoverMenu::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbGameoverMenu::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

cocos2d::SEL_CallFuncN ccbGameoverMenu::onResolveCCBCCCallFuncSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "repeat", ccbGameoverMenu::Next);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over", ccbGameoverMenu::Over);
	return NULL;
}

void ccbGameoverMenu::setAnimationManager(CCBAnimationManager *pAnimationManager, int _level)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_iLevel = _level;
}

void ccbGameoverMenu::SetGame( CGameControler* p )
{
	m_pGameControler = p;
}

void ccbGameoverMenu::Win(int _complate)
{
	m_bWin = true;
	if ( m_iLevel + 1 < d_iLevelNum*6 )
	{
		if ( AppDelegate::s_LevelOpen[m_iLevel+1] == 0 )
			AppDelegate::s_LevelOpen[m_iLevel+1] = 1;
	}
	AppDelegate::s_LevelOpen[m_iLevel] = 2;
	scheduleOnce(schedule_selector(ccbGameoverMenu::StartDigit), 3.50f);
	//m_AnimationManager->setAnimationCompletedCallback(this,callfunc_selector(ccbGameoverMenu::AppearOver));
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
	//AppDelegate::AudioStopBgm();
	AppDelegate::AudioPlayBgm("MS/Music/EfWinBG.mp3", false);

	if ( m_iLevel%6 != 5 && m_iLevel < 49 )
	{
		//CCDictionary *strings = CCDictionary::createWithContentsOfFileThreadSafe("Tile/QustionStrings.xml");
		for ( int i = 0; i < 3; i++ )
		{
			CCSprite* pSprite;
			if ( m_pGameControler->m_structQuestion.complate[m_pGameControler->m_structQuestion.index[i]-1] )
				pSprite = CCSprite::create("tu5/kuai.png");
			else
				pSprite = CCSprite::create("tu5/kuai2.png");
			pSprite->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pNode->getChildByTag(201+i)->addChild(pSprite);

			char buffer[255];
			//sprintf(buffer, "%d", m_iLevel*3+i);
			////const char* str = ((CCString*)strings->objectForKey(buffer))->m_sString.c_str();
			//CCLabelTTF* pLaber = CCLabelTTF::create(str, "Arial", 26,
			//	CCSizeMake(260, 26), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
			sprintf(buffer, "Tile/QuestionInfo/%d.png", m_pGameControler->m_unLevel*3+i);
			CCSprite* pLaber = CCSprite::create(buffer);
			if ( pLaber )
			{
				pLaber->setAnchorPoint(ccp(0.0f, 0.0f));
				pLaber->setPosition(ccp(-124, -12));
				//pLaber->setColor( ccc3(0, 0, 0) );
				m_pNode->getChildByTag(201+i)->addChild(pLaber);
			}
		}
	}

	if ( m_iLevel%6 != 5 && m_iLevel < 49 && m_iLevel != 30 )
	{
		CCSprite* pSprite1 = CCSprite::create("tu5/xingxing/hei.png");
		CCSprite* pSprite2 = CCSprite::create("tu5/xingxing/hei.png");
		CCSprite* pSprite3 = CCSprite::create("tu5/xingxing/hei.png");
		pSprite1->setPosition(ccp(d_fStarLocateX1+2-m_pBoard->getPositionX()+308, d_fStarLocateY1-1));
		pSprite2->setPosition(ccp(d_fStarLocateX2+2-m_pBoard->getPositionX()+308, d_fStarLocateY2-1));
		pSprite3->setPosition(ccp(d_fStarLocateX3+2-m_pBoard->getPositionX()+308, d_fStarLocateY3-1));
		m_pBoard->addChild(pSprite1);
		m_pBoard->addChild(pSprite2);
		m_pBoard->addChild(pSprite3);
	}
	else
	{
#ifndef GameTypeC
		CCSprite* pSprite = CCSprite::create("tu5/boss/hei.png");
		pSprite->setPosition(ccp(d_fStarLocateX2-m_pBoard->getPositionX()+310, d_fStarLocateY2-30));
		m_pBoard->addChild(pSprite);
#endif
	}
	AppDelegate::s_Money += m_pGameControler->m_iMoneyTo + m_pGameControler->m_iComboMax*10;
	//AppDelegate::s_Medal += m_pGameControler->m_iStateNumMedal;
	AppDelegate::s_GrenadeNum	= m_pGameControler->m_iGrenadeNum;
	AppDelegate::s_Healbox		= m_pGameControler->m_iHealBox;
	AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = m_pGameControler->m_iBulletNum[0];
	//AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[2]] = m_pGameControler->m_iBulletNum[1];
	AppDelegate::s_SkillEnergy[0] = m_pGameControler->m_iStateSkill[0];
	AppDelegate::s_SkillEnergy[1] = m_pGameControler->m_iStateSkill[1];
	AppDelegate::s_KillMonster += m_pGameControler->m_iStateNumKill;
	AppDelegate::m_Sell2 = m_pGameControler->m_iSkillCanUse;
	AppDelegate::SaveStatus();
	AppDelegate::SaveLevel(m_iLevel);
	AppDelegate::SaveAchievement();
	//InitBtn();
}

void ccbGameoverMenu::Lose(int _complate)
{
	m_bWin = false;
	
	AppDelegate::s_Money += m_pGameControler->m_iStateNumGold + m_pGameControler->m_iComboMax*10;
	//AppDelegate::s_Medal += m_pGameControler->m_iStateNumMedal;
	AppDelegate::s_GrenadeNum	= m_pGameControler->m_iGrenadeNum;
	AppDelegate::s_Healbox		= m_pGameControler->m_iHealBox;
	AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = m_pGameControler->m_iBulletNum[0];
	//AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[2]] = m_pGameControler->m_iBulletNum[1];
	AppDelegate::s_SkillEnergy[0] = m_pGameControler->m_iStateSkill[0];
	AppDelegate::s_SkillEnergy[1] = m_pGameControler->m_iStateSkill[1];
	AppDelegate::s_KillMonster += m_pGameControler->m_iStateNumKill;
	AppDelegate::m_Sell2 = m_pGameControler->m_iSkillCanUse;
	AppDelegate::SaveStatus();
	AppDelegate::SaveLevel(m_iLevel);
	AppDelegate::SaveAchievement();
	//InitBtn();
	//m_AnimationManager->setAnimationCompletedCallback(this,callfunc_selector(ccbGetItemLayer::appearOver));
	//m_AnimationManager->setAnimationCompletedCallback(this,callfun_selector(ccbGameoverMenu::AppearOver));
	//((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->appearOver();
	scheduleOnce(schedule_selector(ccbGameoverMenu::ItemAppearOver), 6.5f);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
	AppDelegate::AudioPlayBgm("MS/Music/Lose.mp3", false);
}

void ccbGameoverMenu::ItemAppearOver(float _t){
	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->appearOver();
}


void ccbGameoverMenu::StartDigit( float _t )
{
	schedule(schedule_selector(ccbGameoverMenu::Timer), 0.01f);
}

void ccbGameoverMenu::Timer( float _t )
{
	if ( m_bNowGold && m_bNowMedal && m_bNowKill && m_bComboMoney )
	{
		unschedule(schedule_selector(ccbGameoverMenu::Timer));
		m_pGameControler->ShowStar();
	}

	bool bEffect = false;

	m_iStateNowTime ++;
	if ( m_iNowKill < m_pGameControler->m_iStateNumKill )
	{
		bEffect = true;
		m_iNowKill ++;
	}
	else if ( !m_bNowKill )
		m_bNowKill = true;
	common::ShowNumber(this, m_iNowKill/*AppDelegate::s_Money*/, 20, 19, d_fGameOverNumLocate1, "tu5/suzi.png", 801);

	if ( m_iStateNowTime > 30 )
	{
		if ( m_iNowMedal < m_pGameControler->m_iComboMax )
		{
			bEffect = true;
			m_iNowMedal ++;
		}
		else if ( !m_bNowMedal )
			m_bNowMedal = true;
		common::ShowNumber(this, m_iNowMedal/*AppDelegate::s_Money*/, 20, 19, d_fGameOverNumLocate2, "tu5/suzi.png", 802, 0, 1.1f);
	}

	if ( m_iStateNowTime > 60 && !m_bNowGold )
	{
		int digit = 10;
		if ( m_pGameControler->m_iStateNumGold < 100 )
			digit = 2;
		if ( m_iNowGold < m_pGameControler->m_iStateNumGold )
		{
			bEffect = true;
			m_iNowGold += digit;
		}
		else
		{
			m_iNowGold = m_pGameControler->m_iStateNumGold;
			m_bNowGold = true;
		}
		common::ShowNumber(this, m_iNowGold/*AppDelegate::s_Money*/, 20, 19, d_fGameOverNumLocate4, "tu5/suzi.png", 804, 0, 1.2f);
	}

	if ( m_bNowGold && !m_bStartAddCombo )
	{
		m_bStartAddCombo = true;
		CCSprite* pSprite = CCSprite::create("tu5/jia.png");
		int i = 0;
		if ( m_iNowGold < 100 )
			i = 1;
		else if ( m_iNowGold < 1000 )
			i = 2;
		else if ( m_iNowGold < 10000 )
			i = 3;
		pSprite->setPosition(CCPoint(46 + 19*i + d_fGameOverNumLocate4 + 8));
		common::ShowNumber(pSprite, m_pGameControler->m_iComboMax*10, 20, 19, 15, 0, "tu5/suzi.png", 804, 0, 1.2f);
		addChild(pSprite, 0, 19);
		pSprite->getChildByTag(804)->runAction(CCSequence::create(CCDelayTime::create(1.0f), CCFadeOut::create(0.5f), NULL));
		pSprite->runAction( CCSequence::create(CCDelayTime::create(1.0f), CCFadeOut::create(0.5f), 
			CCCallFunc::create(this, callfunc_selector(ccbGameoverMenu::AddOver)), NULL) );
	}
	else if ( m_bStartAddCombo && m_bOverAddCombo && !m_bComboMoney )
	{
		int digit = 5;
		if ( m_pGameControler->m_iComboMax < 10 )
			digit = 1;
		if ( m_iNowGold < m_pGameControler->m_iStateNumGold + m_pGameControler->m_iComboMax*10 )
		{
			bEffect = true;
			m_iNowGold += digit;
		}
		else
		{
			m_iNowGold = m_pGameControler->m_iStateNumGold + m_pGameControler->m_iComboMax*10;
			m_bComboMoney = true;
		}
		common::ShowNumber(this, m_iNowGold/*AppDelegate::s_Money*/, 20, 19, d_fGameOverNumLocate4, "tu5/suzi.png", 804, 0, 1.2f);
	}

	if ( bEffect )
		AppDelegate::AudioPlayEffect("MS4/numadd.mp3");
}

void ccbGameoverMenu::AddOver()
{
	getChildByTag(19)->removeFromParentAndCleanup(true);
	m_bOverAddCombo = true;
}

void ccbGameoverMenu::Next( CCNode* sender )
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);
}

void ccbGameoverMenu::Over( CCNode* sender )
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);
	((ccbGetItemLayer*)m_pGameControler->m_pGetItemCCB)->Appear2(0);
	//float time = rand()%20 + 20;
	//scheduleOnce(schedule_selector(ccbGameoverMenu::LosePlay), time/10.0f);
}

void ccbGameoverMenu::OnShop( cocos2d::CCObject *pSender )
{
	if ( m_pGameControler->m_bEndPress )
		return;
	((ccbGameoverBtnMenu*)m_pGameControler->m_pGameoverBtnNode)->OnShop(NULL);
}

void ccbGameoverMenu::OnReplay( cocos2d::CCObject *pSender )
{
	if ( m_pGameControler->m_bEndPress )
		return;
	((ccbGameoverBtnMenu*)m_pGameControler->m_pGameoverBtnNode)->OnReplay(NULL);
}

void ccbGameoverMenu::OnReturn( cocos2d::CCObject *pSender )
{
	if ( m_pGameControler->m_bEndPress )
		return;
	((ccbGameoverBtnMenu*)m_pGameControler->m_pGameoverBtnNode)->OnReturn(NULL);
}

void ccbGameoverMenu::OnRechoseHero( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_pGameControler->m_bEndPress )
		return;
	((ccbGameoverBtnMenu*)m_pGameControler->m_pGameoverBtnNode)->OnRechoseHero(NULL);
}

void ccbGameoverMenu::StartBtn()
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2 copy", 0.0f);
}

//void ccbGameoverMenu::OnBuy1( cocos2d::CCObject *pSender )
//{
//	if ( !m_iWeapon )
//		return;
//	int sell1 = 1;
//	int sell2 = 1;
//	if ( ccbShopMenu::WeaponOnSell(m_iWeapon) )
//	{
//		sell1 = 5;
//		sell2 = 4;
//	}
//	int cost = g_priceUpgrade[m_iWeapon]*sell2/sell1;
//	bool successd = false;
//	if ( g_weaponUseMedal[m_iWeapon] )
//	{
//		if ( AppDelegate::s_Money >= cost )
//		{
//			AppDelegate::s_Money -= cost;
//			successd = true;
//		}
//	}
//	else
//	{
//		if ( AppDelegate::s_Medal >= cost )
//		{
//			AppDelegate::s_Medal -= cost;
//			successd = true;
//		}
//	}
//	if ( successd )
//	{
//		AppDelegate::AudioPlayEffect("MS2/buy.mp3");
//		AppDelegate::s_WeaponOwn[m_iWeapon] = 1;
//		CCNode* pSprite = m_pNode->getChildByTag(905)->getChildByTag(1);
//		pSprite->removeFromParentAndCleanup(true);
//		//pSprite->setRotation(0);
//		//pSprite->setPosition(ccp(-258,160));
//		//pSprite->stopAllActions();
//		//pSprite->setScale(1.0f);
//		m_iWeapon = 0;
//	}
//	else
//	{//失败提示
//
//	}
//}
//
//void ccbGameoverMenu::OnBuy2( cocos2d::CCObject *pSender )
//{
//	if ( AppDelegate::s_Healbox > 2 || AppDelegate::s_Money < 300 )
//		return;
//	AppDelegate::AudioPlayEffect("MS2/buy.mp3");
//	AppDelegate::s_Healbox ++;
//	AppDelegate::s_Money -= 300;
//	common::ShowNumberUpdate(m_pNode->getChildByTag(906), AppDelegate::s_Healbox, 888);
//	CCNode* pNode = m_pNode->getChildByTag(906)->getChildByTag(888);
//	pNode->stopAllActions();
//	pNode->setScale(1.5f);
//	CCActionInterval* pAction2 = CCScaleTo::create(0.2f, 1.0f);
//	pNode->runAction( pAction2 );
//}
//
//void ccbGameoverMenu::OnBuy3( cocos2d::CCObject *pSender )
//{
//	if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] >= 999 || AppDelegate::s_Money < g_priceBullet[AppDelegate::s_WeaponUse[1]] )
//		return;
//	AppDelegate::AudioPlayEffect("MS2/buy.mp3");
//	AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
//	if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
//		AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
//	AppDelegate::s_Money -= g_priceBullet[AppDelegate::s_WeaponUse[1]];
//	common::ShowNumberUpdate(m_pNode->getChildByTag(907), AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]], 888);
//	CCNode* pNode = m_pNode->getChildByTag(907)->getChildByTag(888);
//	pNode->stopAllActions();
//	pNode->setScale(1.5f);
//	CCActionInterval* pAction2 = CCScaleTo::create(0.2f, 1.0f);
//	pNode->runAction( pAction2 );
//}

//void ccbGameoverMenu::OnBuy4( cocos2d::CCObject *pSender )
//{
//	if ( AppDelegate::s_GrenadeNum >= d_iGrenadeMax || AppDelegate::s_Money < 260 )
//		return;
//	AppDelegate::AudioPlayEffect("MS2/buy.mp3");
//	AppDelegate::s_GrenadeNum ++;
//	AppDelegate::s_Money -= 260;
//	common::ShowNumberUpdate(m_pNode->getChildByTag(908), AppDelegate::s_GrenadeNum, 888);
//	CCNode* pNode = m_pNode->getChildByTag(908)->getChildByTag(888);
//	pNode->stopAllActions();
//	pNode->setScale(1.5f);
//	CCActionInterval* pAction2 = CCScaleTo::create(0.2f, 1.0f);
//	pNode->runAction( pAction2 );
//}

//void ccbGameoverMenu::InitBtn()
//{
//	{//枪
//		CCNode* pNode = m_pNode->getChildByTag(905);
//		for ( int i = 12; i > 3; i-- )
//		{
//			if ( i == 6 )
//				continue;
//			if ( AppDelegate::s_WeaponOwn[i] == 0 )
//				m_iWeapon = i;
//			if ( AppDelegate::s_WeaponOwn[i] == 1 && m_iWeapon )
//				break;
//		}
//
//		if ( m_iWeapon )
//		{
//			char buffer[50];
//			sprintf(buffer, "tu3/qiang/%d.png", m_iWeapon);
//			CCSprite* pSprite = CCSprite::create(buffer);
//			pSprite->setScale(0.7f);
//			pSprite->setPosition(ccp(95, 220));
//			pSprite->setRotation(-60);
//			pNode->addChild(pSprite, 1, 1);
//			CCScaleTo* pAction1 = CCScaleTo::create(0.1f, 1.1f);
//			CCScaleTo* pAction2 = CCScaleTo::create(0.3f, 0.7f);
//			pSprite->runAction(CCRepeatForever::create(CCSequence::create(pAction1, pAction2, CCDelayTime::create(1.0f), NULL)));
//
//			int sell1 = 1;
//			int sell2 = 1;
//			if ( ccbShopMenu::WeaponOnSell(m_iWeapon) )
//			{
//				sell1 = 5;
//				sell2 = 4;
//			}
//			if ( g_weaponUseMedal[m_iWeapon] )
//				pSprite = CCSprite::create("tu5/kuasu/buy1.png");
//			else
//				pSprite = CCSprite::create("tu5/kuasu/buy2.png");
//			pSprite->setAnchorPoint(ccp(0.5f, 0.25f));
//			pSprite->setPosition(ccp(84,85));
//			//pSprite->setRotation(6);
//			pNode->addChild(pSprite, 2, 2);
//			common::ShowNumber(pSprite, g_priceUpgrade[m_iWeapon]*sell2/sell1, 19, 24, 48, 40, "tu3/zidansuzi2.png", 888, 0, 0.8f);
//			pSprite->getChildByTag(888)->setRotation(6);
//
//			//pSprite = CCSprite::create("tu3/weili.png");
//			//pSprite->setPosition(ccp(84,115));
//			//pSprite->setRotation(6);
//			//pNode->addChild(pSprite, 2, 3);
//			int d = AppDelegate::WeaponDamage(m_iWeapon);
//			int sp = AppDelegate::WeaponSpeed(m_iWeapon);
//			if ( sp == 0 )
//				sp = 100;
//			common::ShowNumber(pSprite, d*100/sp, 19, 24, 48, 10, "tu3/zidansuzi.png", 889, 0, 0.8f);
//			pSprite->getChildByTag(889)->setRotation(6);
//		}
//	}
//
//	{//左边
//		CCNode* pNode = m_pNode->getChildByTag(906);
//		common::ShowNumber(pNode, AppDelegate::s_Healbox, 19, 24, 97, 57, "tu3/zidansuzi.png", 888, 0, 0.8f);
//		common::ShowNumber(pNode, 300, 19, 24, 70, 22, "tu3/zidansuzi.png", 889, 0, 0.8f);
//
//		pNode = m_pNode->getChildByTag(907);
//		common::ShowNumber(pNode, AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]], 19, 24, 78, 57, "tu3/zidansuzi.png", 888, 0, 0.8f);
//		common::ShowNumber(pNode, g_priceBullet[AppDelegate::s_WeaponUse[1]], 19, 24, 70, 22, "tu3/zidansuzi.png", 889, 0, 0.8f);
//
//		pNode = m_pNode->getChildByTag(908);
//		common::ShowNumber(pNode, AppDelegate::s_GrenadeNum, 19, 24, 83, 57, "tu3/zidansuzi.png", 888, 0, 0.8f);
//		common::ShowNumber(pNode, 260, 19, 24, 70, 22, "tu3/zidansuzi.png", 889, 0, 0.8f);
//	}
//}

//void ccbGameoverMenu::LosePlay( float _t )
//{
//	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);
//}

