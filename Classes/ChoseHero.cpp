#include "ChoseHero.h"
#include "MainMenu.h"
#include "AppDelegate.h"
#include "Flash.h"
#include "Common.h"
#include "baseRes.h"
#include "FirstLogin.h"
#include "PayService.h"
#include "ShopMenu.h"

using namespace cocos2d;
using namespace cocos2d::extension;

static int bodyIndex[]=
{//正常对图
	3, 5, 0,
	2, 1, 4,
};
static int bodyIndex2[]=
{//图对正常
	2, 4, 3,
	0, 5, 1,
};
static int bodyIndex3[]=
{//对应每日奖励图片名字
	35,34,27,
	33,26,25,
};
static int bodyX[]=
{
	363, 455, 322, 495, 406, 406,
};
static int bodyY[]=
{
	152, 152, 290, 290, 320, 260,
};
static int bodyToward[]=
{
	-1, 1, -1, 1, 1, 1,
};

extern CFlashControl* g_pHero;

ccbChoseHero* ccbChoseHero::s_pccbChoseHero = NULL;

ccbChoseHero::ccbChoseHero()
	: m_AnimationManager(NULL)
{
	m_pMenuShow	= NULL;
	m_pMenuStart= NULL;
	m_bPress = false;
	m_bIsChose	= false;
	m_bCanChose = false;
	m_iHero3Buy = 0;
	m_iHero3BuyDigit = 0;
	m_iHero3BuyTimer = 0;
	isShowRobit = false;
	isShowHero2 = false;
	isShowModules = false;
	mainIndex = 0;
	robitIndex = 0;
	hero2Index = 0;
	modulesIndex = 0;
	AppDelegate::AudioStopB();

	mainPos[0][0] = 61;
	mainPos[0][1] = 435;
	mainPos[1][0] = 396;
	mainPos[1][1] = 386;
	mainPos[2][0] = 732;
	mainPos[2][1] = 435;
	mainPos[3][0] = 405;
	mainPos[3][1] = 30;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	setKeypadEnabled(true);
#endif
}

ccbChoseHero::~ccbChoseHero()
{
	s_pccbChoseHero = NULL;
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbChoseHero::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	s_pccbChoseHero = this;
	PayService::HideBar();
	m_pNode = pNode;
	m_pHeroName1 = CCSprite::create("Animation/HeroName/a6.png");
	m_pHeroName1->setPosition(ccp(0,0));
	pNode->getChildByTag(101)->addChild(m_pHeroName1);
	Callback1(m_pHeroName1);

	m_pHeroName2 = CCSprite::create("Animation/HeroName/b6.png");
	m_pHeroName2->setPosition(ccp(0,0));
	pNode->getChildByTag(102)->addChild(m_pHeroName2);
	Callback2(m_pHeroName2);
	setTouchEnabled(true);

	{
		CCMenuItem* pItem ;
		pItem = CCMenuItemImage::create("tu14/xuanze.png", "tu14/xuanze2.png", this, menu_selector(ccbChoseHero::OnHero1));
		pItem->setPosition(ccp(0,0));
		CCMenu* pMenu = CCMenu::create(pItem, NULL);
		pMenu->setPosition(ccp(0,0));
		pNode->getChildByTag(2)->addChild(pMenu);
	}
	{
		CCMenuItem* pItem ;
		if ( AppDelegate::s_Hero2 == 0 )
			pItem = CCMenuItemImage::create("tu14/xuanze.png", "tu14/xuanze2.png", this, menu_selector(ccbChoseHero::OnHero2));
		else
			pItem = CCMenuItemImage::create("tu14/liji.png", "tu14/liji2.png", this, menu_selector(ccbChoseHero::OnBuy2));
		pItem->setPosition(ccp(0,0));
		CCMenu* pMenu = CCMenu::create(pItem, NULL);
		pMenu->setPosition(ccp(0,0));
		pNode->getChildByTag(1)->addChild(pMenu, 0, 1);
	}
	{
		CCMenuItem* pItem ;
		if ( AppDelegate::s_Hero3 == 0 )
			pItem = CCMenuItemImage::create("tu14/xuan.png", "tu14/xuan2.png", this, menu_selector(ccbChoseHero::OnHero3));
		else
			pItem = CCMenuItemImage::create("tu14/lijikaiqi.png", "tu14/lijikaiqi2.png", this, menu_selector(ccbChoseHero::OnBuy3));
		pItem->setPosition(ccp(0,0));
		CCMenu* pMenu = CCMenu::create(pItem, NULL);
		pMenu->setPosition(ccp(0,0));
		pNode->getChildByTag(3)->addChild(pMenu, 3, 1);
	}

	m_pHero3Board[0] = CCSprite::create("sell/di1.png");
	m_pHero3Board[0]->setPosition(ccp(400, 240));
	m_pHero3Board[0]->setVisible(false);
	addChild(m_pHero3Board[0], 6);
	if ( AppDelegate::s_Hero3 != 0 )
	{
		m_pHero3Board[1] = CCSprite::create("tu14/buy/tu.png");
		m_pHero3Board[1]->setPosition(ccp(400, 240));
		m_pHero3Board[1]->setVisible(false);
		addChild(m_pHero3Board[1], 6);

		CCMenuItem* pItem0 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbChoseHero::OnBuy3Disappear));
		pItem0->setPosition(ccp(725,437));
		m_pMenuDisappear = CCMenu::create(pItem0, NULL);
		m_pMenuDisappear->setPosition(ccp(0,0));
		m_pMenuDisappear->setVisible(false);
		addChild(m_pMenuDisappear, 5);

		for ( int i = 0; i < 6; i++ )
		{
			char buffer[255];
			sprintf(buffer, "tu14/buy/b%d.png", i+1);
			m_pHero3Body1[i] = CCSprite::create(buffer);
			sprintf(buffer, "tu14/buy/a%d.png", i+1);
			m_pHero3Body2[i] = CCSprite::create(buffer);
			m_pHero3Body1[i]->setVisible(false);
			m_pHero3Body2[i]->setVisible(false);
			m_pHero3Body1[i]->setPosition(ccp(245+155*(bodyIndex[i]%3), 285-125*(bodyIndex[i]/3)));
			m_pHero3Body2[i]->setPosition(ccp(245+155*(bodyIndex[i]%3), 285-125*(bodyIndex[i]/3)));
			addChild(m_pHero3Body1[i], 6);
			addChild(m_pHero3Body2[i], 6);
		}

#ifndef Plat_Telecom_Demo
		m_pLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
		m_pLayerColor->setVisible(false);
		addChild(m_pLayerColor, 4);

		CCMenuItem* pItem1 = CCMenuItemImage::create("sell/huode3-1.png", "sell/huode3-2.png", this, menu_selector(ccbChoseHero::OnBuy3Start));
		pItem1->setPosition(ccp(400,35));
		m_pMenuStart = CCMenu::create(pItem1, NULL);
		m_pMenuStart->setPosition(ccp(0,0));
		m_pMenuStart->setVisible(false);
		addChild(m_pMenuStart, 6);

		CCMenuItem* pItem2 = CCMenuItemImage::create("tu14/goumai.png", "tu14/goumai2.png", this, menu_selector(ccbChoseHero::OnBuy3Show));
		pItem2->setPosition(ccp(400,-82));
		m_pMenuShow = CCMenu::create(pItem2, NULL);
		m_pMenuShow->setPosition(ccp(0,0));
		addChild(m_pMenuShow, 4);
#endif
	}

	if ( AppDelegate::s_FirstLogin == 2 )
	{
		m_bCanChose = true;
		Guide1();
	}

	CCSprite* handelSprite = ccbShopMenu::createHandelCreator();
	handelSprite->setPosition(ccp(mainPos[0][0],mainPos[0][1]));
	addChild(handelSprite,20,1001);

	handelSprite->setVisible(true);
	setKeypadEnabled(true);
}

SEL_MenuHandler ccbChoseHero::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbChoseHero::OnHero1 );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbChoseHero::OnHero2 );
	return NULL;
}

SEL_CCControlHandler ccbChoseHero::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbChoseHero::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbChoseHero::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbChoseHero::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
}

cocos2d::SEL_CallFuncN ccbChoseHero::onResolveCCBCCCallFuncSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over", ccbChoseHero::over);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over1", ccbChoseHero::over);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "over2", ccbChoseHero::over);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "start", ccbChoseHero::Appear);

	return 0;
}

void ccbChoseHero::over( CCNode* sender )
{
	AppDelegate::ChangeScene( CMainMenu::StartMenu() );
}

cocos2d::CCScene* ccbChoseHero::CreateScene()
{
	CCScene* pScene = CCScene::create();
	CCLayer* pLayer = CCLayer::create();
	pScene->addChild(pLayer);

	CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ccbChoseHero", ccbChoseHeroLoader::loader());
	cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	CCNode* pNode = ccbReader->readNodeGraphFromFile("20.ccbi"/*, this*/);
	SetScale(pNode);
	((ccbChoseHero*)pNode)->setAnimationManager(ccbReader->getAnimationManager());
	ccbReader->getAnimationManager()->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
	pNode->setPosition(ccp(0,0));
	ccbReader->release();
	pLayer->addChild(pNode);

	return pScene;
}

void ccbChoseHero::Callback1( CCNode* _pNode )
{
	float time = ((float)(rand()%20))/10.0f + 1.0f ;
	CCDelayTime* pDelay = CCDelayTime::create(time);
	CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("Animation/HeroName/a", 6, CCRect(0,0,59,201)));
	CCCallFuncN* pCallback = CCCallFuncN::create(this, callfuncN_selector(ccbChoseHero::Callback1));

	_pNode->runAction(CCSequence::create(pDelay, pAction, pCallback, NULL));
}

void ccbChoseHero::Callback2( CCNode* _pNode )
{
	float time = ((float)(rand()%20))/10.0f + 1.0f ;
	CCDelayTime* pDelay = CCDelayTime::create(time);
	CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("Animation/HeroName/b", 6, CCRect(0,0,55,201)));
	CCCallFuncN* pCallback = CCCallFuncN::create(this, callfuncN_selector(ccbChoseHero::Callback2));

	_pNode->runAction(CCSequence::create(pDelay, pAction, pCallback, NULL));
}

void ccbChoseHero::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( m_bCanChose )
		return;
	if ( m_pHero3Board[0]->isVisible() )
		return;
	CCPoint point = ((CCTouch*)(*(pTouches->begin())))->getLocation();
	float x0 = point.x;
	float y = point.y;

	if ( x0 < 230*Scale_X )
		OnHero1();
	else if ( x0 > 570*Scale_X )
		OnHero2();
	else
		OnHero3();
}

void ccbChoseHero::keyEnterClicked(){
	if(isShowRobit){
		if(robitIndex==1){
			OnBuy3Cancle();
			isShowRobit = false;
			setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
		}else if(robitIndex==0){
			//
			if(AppDelegate::s_Hero3==0){
				OnHero3();
			}else{
				OnBuy3Buy();
			}
		}
		return;
	}
	if(isShowHero2){
		if(hero2Index==1){
			OnBuy2Cancle();
			isShowHero2 = false;
			setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
		}else if(hero2Index==0){
			//
			if(AppDelegate::s_Hero2==0){
				OnHero2();
			}else{
				OnBuy2Buy(this);
			}
		}
		return;
	}
	if(isShowModules){
		if(modulesIndex==1){
			OnBuy3Disappear();
		}else if(modulesIndex==0){
			OnBuy3Start();
		}
		return;
	}
	
	if(mainIndex==0){
		OnHero1();
	}else if(mainIndex==1){
		if(AppDelegate::s_Hero3==0){
			OnHero3();
		}else{
			isShowRobit = true;
			robitIndex = 0;
			OnBuy3();
			setFingerPos(ccp(735,180));
		}
	}else if(mainIndex==2){
		if(AppDelegate::s_Hero2==0){
			OnHero2();
		}else{
			isShowHero2 = true;
			hero2Index = 0;
			setFingerPos(ccp(735,180));
			OnBuy2();
			
		}
	}else if(mainIndex==3){
		isShowModules = true;
		modulesIndex = 0;
		OnBuy3Show();
		setFingerPos(ccp(405,20));
	}
}

void ccbChoseHero::keyArrowClicked(int arrow){
	if(isShowRobit){
		if(arrow == kTypeUpArrowClicked || arrow==kTypeDownArrowClicked){
			if(robitIndex==1){
				robitIndex = 0;
			}else{
				robitIndex = 1;
			}
		}
		if(robitIndex==0){
			setFingerPos(ccp(735,180));
		}else{
			setFingerPos(ccp(753,405));
		}
		return;
	}
	if(isShowHero2){
		if(arrow == kTypeUpArrowClicked || arrow==kTypeDownArrowClicked){
			if(hero2Index==1){
				hero2Index = 0;
			}else{
				hero2Index = 1;
			}
		}
		if(hero2Index==0){
			setFingerPos(ccp(735,180));
		}else{
			setFingerPos(ccp(753,405));
		}
		return;
	}

	if(isShowModules){
		switch(arrow){
		case kTypeUpArrowClicked:
		case kTypeDownArrowClicked:
			if(modulesIndex==0){
				modulesIndex = 1;
			}else{
				modulesIndex = 0;
			}
			break;
		}
		if(modulesIndex==0){
			setFingerPos(ccp(405,20));
		}else{
			setFingerPos(ccp(735,430));
		}
		
		return;
	}
	switch(arrow){
	case kTypeDownArrowClicked:
		if(mainIndex<3 && AppDelegate::s_Hero3 != 0){
			mainIndex = 3;
		}
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
		break;
	case kTypeUpArrowClicked:
		if(mainIndex==3){
			mainIndex = 1;
		}
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
		break;
	case kTypeLeftArrowClicked:
		if(mainIndex==3){
			mainIndex = 0;
		}else if(mainIndex<3 && mainIndex>0){
			mainIndex --;
		}
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
		break;
	case kTypeRightArrowClicked:
		if(mainIndex==3){
			mainIndex = 2;
		}else if(mainIndex>=0 && mainIndex<2){
			mainIndex ++;
		}
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
		break;
	}
}

void ccbChoseHero::keyNumClicked(int keyNum){
	if(!isShowRobit && !isShowModules){
		if(keyNum==kTypeNum1){

		}else if(keyNum==kTypeNum2){

		}else if(keyNum==kTypeNum0){

		}
	}else {
		if(keyNum==kTypeNum0){
			OnBuy3Disappear();
		}
	}
}

void ccbChoseHero::setFingerPos(CCPoint point){
	getChildByTag(1001)->setPosition(point);
}

void ccbChoseHero::disappear()
{
	getChildByTag(10)->stopAllActions();
	removeChildByTag(10);
}

void ccbChoseHero::OnHero1(cocos2d::CCObject *pSender)
{
	if ( m_pHero3Board[0]->isVisible() )
		return;
	if ( m_bPress || m_bCanChose )
		return;
	m_bPress = true;
	AppDelegate::s_HeroType = 0;
	getChildByTag(1001)->setVisible(false);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline1", 0.0f);
	if ( m_pMenuShow )
		m_pMenuShow->setVisible(false);
}

void ccbChoseHero::OnHero2(cocos2d::CCObject *pSender)
{
	//CCLog("visible=%d,m_bPress=%d,m_bCanChose=%d",m_pHero3Board[0]->isVisible(),m_bPress,m_bCanChose);
	if ( m_pHero3Board[0]->isVisible() )
		return;
	if ( m_bPress || m_bCanChose )
		return;
	m_bPress = true;
#ifdef GameTypeA
	if ( AppDelegate::s_Hero2 != 0 )
	{
		m_bPress = false;
		OnBuy2(NULL);
		return;
	}
	AppDelegate::s_HeroType = 1;
	getChildByTag(1001)->setVisible(false);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
	if ( m_pMenuShow )
		m_pMenuShow->setVisible(false);
#endif

#ifdef GameTypeB
	m_bPress = false;
	if ( getChildByTag(10) )
	{
		getChildByTag(10)->stopAllActions();
		removeChildByTag(10);
	}
	CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	pSprite->setPosition(ccp(400, 240));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbChoseHero::disappear)), NULL));
	addChild(pSprite, 5, 10);
#endif
}

void ccbChoseHero::OnHero3( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_pHero3Board[0]->isVisible() )
		return;
	if ( m_bPress || m_bCanChose )
		return;
	m_bPress = true;
#ifdef GameTypeA
	if ( AppDelegate::s_Hero3 != 0 )
	{
		m_bPress = false;
		OnBuy3(NULL);
		return;
	}
	AppDelegate::s_HeroType = 2;
	getChildByTag(1001)->setVisible(false);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline3", 0.0f);
	if ( m_pMenuShow )
		m_pMenuShow->setVisible(false);
#endif
#ifdef GameTypeB
	m_bPress = false;
	if ( getChildByTag(10) )
	{
		getChildByTag(10)->stopAllActions();
		removeChildByTag(10);
	}
	CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	pSprite->setPosition(ccp(400, 240));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbChoseHero::disappear)), NULL));
	addChild(pSprite, 5, 10);
#endif
}

void ccbChoseHero::OnBuy2( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_pHero3Board[0]->isVisible() )
		return;
	if ( m_bPress || m_bCanChose )
		return;
#ifdef GameTypeA
#ifdef Plat_Telecom_Demo
	OnBuy2Buy();
	return;
#endif
	m_bCanChose = true;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	//pCCLayerColor->setPosition(ccp(0,0));
	//SetScale(pCCLayerColor);
	addChild(pCCLayerColor, 5, 196);

	CCMenuItem* pItem1 = CCMenuItemImage::create("sell/huode2-1.png", "sell/huode2-2.png", this, menu_selector(ccbChoseHero::OnBuy2Buy));
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbChoseHero::OnBuy2Cancle));
	pItem1->setPosition(ccp(725,195));
	pItem2->setPosition(ccp(728,437));
	CCMenu* pMenu = CCMenu::create(pItem1, pItem2, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 5, 197);

	CCSprite* pSprite1 = CCSprite::create("sell/di1.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 5, 198);
	CCSprite* pSprite2 = CCSprite::create("sell/9/tu.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 5, 199);
#endif

#ifdef GameTypeB
	m_bPress = false;
	if ( getChildByTag(10) )
	{
		getChildByTag(10)->stopAllActions();
		removeChildByTag(10);
	}
	CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	pSprite->setPosition(ccp(400, 240));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbChoseHero::disappear)), NULL));
	addChild(pSprite, 5, 10);
#endif
}

void ccbChoseHero::OnBuy3( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_pHero3Board[0]->isVisible() )
		return;
	if ( m_bPress || m_bCanChose )
		return;
#ifdef GameTypeA
#ifdef Plat_Telecom_Demo
	OnBuy3Buy();
	return;
#endif
	m_bCanChose = true;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	//pCCLayerColor->setPosition(ccp(0,0));
	//SetScale(pCCLayerColor);
	addChild(pCCLayerColor, 5, 196);

	CCMenuItem* pItem1 = CCMenuItemImage::create("sell/huode2-1.png", "sell/huode2-2.png", this, menu_selector(ccbChoseHero::OnBuy3Buy));
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbChoseHero::OnBuy3Cancle));
	pItem1->setPosition(ccp(725,195));
	pItem2->setPosition(ccp(728,437));
	CCMenu* pMenu = CCMenu::create(pItem1, pItem2, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 5, 197);

	CCSprite* pSprite1 = CCSprite::create("sell/di1.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 5, 198);
	CCSprite* pSprite2 = CCSprite::create("sell/chosehero/tu.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 5, 199);
#endif
#ifdef GameTypeB
	m_bPress = false;
	if ( getChildByTag(10) )
	{
		getChildByTag(10)->stopAllActions();
		removeChildByTag(10);
	}
	CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	pSprite->setPosition(ccp(400, 240));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbChoseHero::disappear)), NULL));
	addChild(pSprite, 5, 10);
#endif
}

void ccbChoseHero::ShowSoundEffect()
{
	if ( m_bPress || m_bCanChose )
		return;
	AppDelegate::AudioPlayEffect("MS4/bodyscan.mp3");
}

void ccbChoseHero::Show()
{
	if ( m_bFirstShow )
		return;
	m_bFirstShow = true;

	CCSprite* pSprite;
	if ( AppDelegate::s_Hero3 == 0 )
	{
		pSprite = CCSprite::create("tu14/jixie/youse.png");
	}
	else
	{
		CCMoveBy* pAction = CCMoveBy::create(0.5f, ccp(0, 122));
		if ( m_pMenuShow )
			m_pMenuShow->runAction(pAction);
		pSprite = CCSprite::create("tu14/jixie/wuse.png");
		m_pHero3Body = pSprite;
		if ( AppDelegate::s_FirstLogin != 2 )
		{
			for ( int i = 0; i < 6; i++ )
			{
				if ( AppDelegate::s_Hero3Array[i] )
				{
					char buffer[255];
					sprintf(buffer, "tu14/jixie/ren%d.png", i+1);
					CCSprite* pSpriteItem = CCSprite::create(buffer);
					pSpriteItem->setPosition(ccp(0,0));
					pSpriteItem->setAnchorPoint(ccp(0,0));
					pSprite->addChild(pSpriteItem);
				}
			}
		}

		if ( AppDelegate::s_FirstLogin == 2 )
		{
			AppDelegate::s_Hero3Array[0] = 0;
			AppDelegate::s_Hero3 = 6;
			m_iHero3BuyAim = 0;
			OnBuy3Disappear1(NULL);
		}
	}
	pSprite->setPosition(ccp(0,-10));
	pSprite->setAnchorPoint(ccp(0.5f, 1.0f));
	m_pNode->getChildByTag(3)->addChild(pSprite, 0, 2);
}

void ccbChoseHero::Appear(CCNode* _pNode)
{
	m_bFirstShow = false;
	CCSprite* pSprite = CCSprite::create("tu14/jixie/yi1.png");
	pSprite->setPosition(ccp(0,-10));
	pSprite->setAnchorPoint(ccp(0.5f, 1.0f));
	CCAnimate* pAction1 = CCAnimate::create(common::CreateAnimationFrame("tu14/jixie/yi", 6, CCRect(0,0,342,373)));
	CCDelayTime* pAction2 = CCDelayTime::create(3.0f);
	pSprite->runAction(CCRepeatForever::create(CCSequence::create(CCCallFunc::create(this, callfunc_selector(ccbChoseHero::ShowSoundEffect)), pAction1, 
		CCCallFunc::create(this, callfunc_selector(ccbChoseHero::Show)), pAction2, NULL)));
	m_pNode->getChildByTag(3)->addChild(pSprite, 1);
}

void ccbChoseHero::OnYes( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(6);
		m_iBuy3Type = 1;
	}
	else
	{
#ifdef Plat_Telecom_Demo
		AppDelegate::s_WeaponOwn[8] = 1;
		AppDelegate::s_BulletNum[8] = 100;
		AppDelegate::s_Healbox = 3;
		AppDelegate::s_GrenadeNum = 10;
		AppDelegate::s_Money += 9999;
		AppDelegate::SaveStatus();
		AppDelegate::SaveWeapon(8);
#endif
		scheduleOnce(schedule_selector(ccbChoseHero::OnYesOver), 0.1f);
	}
}

void ccbChoseHero::OnYesOver(float _t)
{
	OnNo(NULL);
}

void ccbChoseHero::OnBuy3PaySwitch()
{
	switch( m_iBuy3Type )
	{
	case 1:
		OnYes();
		break;
	case 2:
		OnBuy3Buy();
		break;
	}
}

void ccbChoseHero::OnNo( cocos2d::CCObject *pSender /*= NULL*/ )
{
	AppDelegate::ChangeScene(CFirstLogin::CreateScene(), etFade, 1.0f);
}

void ccbChoseHero::OnBuy2Buy( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(7);
	}
	else
	{
#ifdef Plat_Telecom_Demo
		AppDelegate::s_Hero2 = 0;
		AppDelegate::s_WeaponOwn[14] = 1;
		AppDelegate::SaveStatus();
		AppDelegate::SaveWeapon(14);
#endif

		m_pNode->getChildByTag(1)->removeChildByTag(1);

		CCMenuItem* pItem = CCMenuItemImage::create("tu14/xuanze.png", "tu14/xuanze2.png", this, menu_selector(ccbChoseHero::OnHero2));
		pItem->setPosition(ccp(0,0));
		CCMenu* pMenu = CCMenu::create(pItem, NULL);
		pMenu->setPosition(ccp(0,0));
		m_pNode->getChildByTag(1)->addChild(pMenu, 0, 1);

		OnBuy2Cancle();
	}
}

void ccbChoseHero::OnBuy2Cancle( cocos2d::CCObject *pSender /*= NULL*/ )
{
	m_bCanChose = false;
	removeChildByTag(196);
	removeChildByTag(197);
	removeChildByTag(198);
	removeChildByTag(199);
	isShowModules = false;
	isShowRobit = false;
	isShowHero2 = false;
	if(mainIndex==3 && AppDelegate::s_Hero3==0){
		mainIndex = 1;
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
	}else{
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
	}
}

void ccbChoseHero::OnBuy3Buy( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( pSender )
	{
		PayService::pay(6);
		m_iBuy3Type = 2;
	}
	else
	{
#ifdef Plat_Telecom_Demo
		AppDelegate::s_Hero3 = 0;
		AppDelegate::s_WeaponOwn[15] = 1;
		if ( AppDelegate::s_WeaponOwn[8] == 0 )
		{
			AppDelegate::s_WeaponOwn[8] = 1;
			AppDelegate::s_BulletNum[8] = 100;
		}
		AppDelegate::s_Healbox = 3;
		AppDelegate::s_GrenadeNum = 10;
		AppDelegate::s_Money += 9999;
		AppDelegate::SaveStatus();
		AppDelegate::SaveWeapon(15);
		AppDelegate::SaveWeapon(8);
		AppDelegate::SaveBullet(8);
#endif
		scheduleOnce(schedule_selector(ccbChoseHero::OnBuy3BuyOver), 0.1f);
	}
}

void ccbChoseHero::OnBuy3BuyOver(float _t)
{
	m_pNode->getChildByTag(3)->removeChildByTag(1);
	m_pNode->getChildByTag(3)->removeChildByTag(2);
	CCMenuItem* pItem = CCMenuItemImage::create("tu14/xuan.png", "tu14/xuan2.png", this, menu_selector(ccbChoseHero::OnHero3));
	pItem->setPosition(ccp(0,0));
	CCMenu* pMenu = CCMenu::create(pItem, NULL);
	pMenu->setPosition(ccp(0,0));
	m_pNode->getChildByTag(3)->addChild(pMenu, 0, 1);

	if ( m_pMenuShow )
		m_pMenuShow->setVisible(false);
	CCSprite* pSprite = CCSprite::create("tu14/jixie/youse.png");
	pSprite->setPosition(ccp(0,-10));
	pSprite->setAnchorPoint(ccp(0.5f, 1.0f));
	m_pNode->getChildByTag(3)->addChild(pSprite, 0, 2);
	OnBuy2Cancle();
}

void ccbChoseHero::OnBuy3Cancle( cocos2d::CCObject *pSender /*= NULL*/ )
{
	m_bCanChose = false;
	removeChildByTag(196);
	removeChildByTag(197);
	removeChildByTag(198);
	removeChildByTag(199);
}

void ccbChoseHero::Guide1()
{
	CCSprite* pTipBoard = CCSprite::create("guide/dikuang2.png");
	pTipBoard->setAnchorPoint(ccp(0,0));
	pTipBoard->setPosition(ccp(400, 290));
	addChild(pTipBoard, 30, 490);

	CCSprite* pNPC = common::CreateAnimation("guide/zhihui1.png", ccp(660, 300), ccp(0.0f, 0.0f),
		"guide/zhihui", 4, CCRect(0,0,138,158), false);
	addChild(pNPC, 30, 491);

	char buffer[255];
	sprintf(buffer, "guide/14.png");
	CCSprite* pTip = CCSprite::create(buffer);
	pTip->setAnchorPoint(ccp(0,0));
	pTip->setPosition(ccp(420, 310));
	addChild(pTip, 30, 492);

	scheduleOnce(schedule_selector(ccbChoseHero::Guide2), 3.0f);
}

void ccbChoseHero::Guide2( float _t )
{
	getChildByTag(490)->removeFromParentAndCleanup(true);
	getChildByTag(491)->removeFromParentAndCleanup(true);
	getChildByTag(492)->removeFromParentAndCleanup(true);

	CCSprite* pTipBoard = CCSprite::create("guide/dikuang2.png");
	pTipBoard->setAnchorPoint(ccp(0,0));
	pTipBoard->setPosition(ccp(400, 290));
	addChild(pTipBoard, 30, 490);

	CCSprite* pNPC = common::CreateAnimation("guide/zhihui1.png", ccp(660, 300), ccp(0.0f, 0.0f),
		"guide/zhihui", 4, CCRect(0,0,138,158), false);
	addChild(pNPC, 30, 491);

	char buffer[255];
	sprintf(buffer, "guide/15.png");
	CCSprite* pTip = CCSprite::create(buffer);
	pTip->setAnchorPoint(ccp(0,0));
	pTip->setPosition(ccp(420, 310));
	addChild(pTip, 30, 492);

	scheduleOnce(schedule_selector(ccbChoseHero::Guide3), 3.0f);
}

void ccbChoseHero::Guide3( float _t )
{
#ifdef Plat_Telecom_Demo
	OnNo();
	return;
#endif
	getChildByTag(490)->removeFromParentAndCleanup(true);
	getChildByTag(491)->removeFromParentAndCleanup(true);
	getChildByTag(492)->removeFromParentAndCleanup(true);
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	//pCCLayerColor->setPosition(ccp(0,0));
	//SetScale(pCCLayerColor);
	addChild(pCCLayerColor, 5);
	CCMenuItem* pItem1 = CCMenuItemImage::create("sell/huode2-1.png", "sell/huode2-2.png", this, menu_selector(ccbChoseHero::OnYes));
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbChoseHero::OnNo));
	pItem1->setPosition(ccp(725,195));
	pItem2->setPosition(ccp(728,437));
	CCMenu* pMenu = CCMenu::create(pItem1, pItem2, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 5);

	CCSprite* pSprite1 = CCSprite::create("sell/di1.png");
	pSprite1->setPosition(ccp(400, 240));
	addChild(pSprite1, 5);
	CCSprite* pSprite2 = CCSprite::create("sell/chosehero/tu.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 5);
}

void ccbChoseHero::OnBuy3Show( cocos2d::CCObject *pSender /*= NULL*/ )
{
#ifdef Plat_Telecom_Demo
	return;
#endif
	if ( m_pHero3Board[0]->isVisible() || m_bCanChose )
		return;
	m_bCanChose = true;
	m_pHero3Board[0]->setVisible(true);
	m_pHero3Board[1]->setVisible(true);
	for ( int i = 0; i < 6; i++ )
	{
		m_pHero3Body1[i]->setVisible(true);
	}
	m_pHero3Body2[bodyIndex2[m_iHero3Buy]]->setVisible(true);
	m_pLayerColor->setVisible(true);
	m_pMenuStart->setVisible(true);
	m_pMenuDisappear->setVisible(true);
	m_iHero3BuyDigit = -1;
}

void ccbChoseHero::OnBuy3Disappear( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_bIsChose )
		return;
	m_bCanChose = false;
	m_pHero3Board[0]->setVisible(false);
	m_pHero3Board[1]->setVisible(false);
	for ( int i = 0; i < 6; i++ )
	{
		m_pHero3Body1[i]->setVisible(false);
	}
	m_pHero3Body2[bodyIndex2[m_iHero3Buy]]->setVisible(false);
	m_pMenuStart->setVisible(false);
	m_pMenuDisappear->setVisible(false);
	m_pLayerColor->setVisible(false);
	isShowModules = false;
	isShowRobit = false;
	isShowHero2 = false;
	if(mainIndex==3 && AppDelegate::s_Hero3==0){
		mainIndex = 1;
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
	}else{
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
	}
}

void ccbChoseHero::OnBuy3Start( cocos2d::CCObject *pSender /*= NULL*/ )
{
	//CCLog("digit=%d",m_iHero3BuyDigit);
	if ( m_iHero3BuyDigit >= 0 )
		return;
	if ( pSender )
	{
		PayService::pay(5);
	}
	else
	{
		m_pMenuShow->setVisible(false);
		m_bIsChose = true;
		m_iHero3BuyDigit = 40;
		schedule(schedule_selector(ccbChoseHero::OnBuy3Timer), 0.04f);
		if ( AppDelegate::s_Hero3 == 2 )
		{
			int times = 3;
			m_iHero3BuyAim = rand()%6; 
			int aim = m_iHero3BuyAim;
			while ( AppDelegate::s_Hero3Array[m_iHero3BuyAim] == 0 && times > 0 )
			{
				times --;
				m_iHero3BuyAim = rand()%6; 
				if ( aim != m_iHero3BuyAim && AppDelegate::s_Hero3Array[m_iHero3BuyAim] == 1 )
					break;
				aim = m_iHero3BuyAim;
			} 
		}
		else if ( AppDelegate::s_Hero3 == 1 )
		{
			do 
			{
				m_iHero3BuyAim = rand()%6; 
			} while ( AppDelegate::s_Hero3Array[m_iHero3BuyAim] == 0 );
		}
		else if ( AppDelegate::s_Hero3 > 1 )
			m_iHero3BuyAim = rand()%6; 
	}
	//CCLog("hero3=%d,buyAim=%d",AppDelegate::s_Hero3,m_iHero3BuyAim);
}

void ccbChoseHero::OnBuy3Timer( float _t )
{
	m_iHero3BuyTimer++;
	int need = 1;
	if ( m_iHero3BuyDigit < 10 )
		need = 5;
	else if ( m_iHero3BuyDigit < 3 )
		need = 10;
	if ( m_iHero3BuyTimer <= need )
		return;
	if ( m_iHero3BuyDigit == 0 )
		return;

	m_iHero3BuyTimer = 0;
	m_pHero3Body2[bodyIndex2[m_iHero3Buy]]->setVisible(false);
	m_iHero3Buy = (m_iHero3Buy+1)%6;
	m_pHero3Body2[bodyIndex2[m_iHero3Buy]]->setVisible(true);
	if ( m_iHero3BuyDigit > 1 )
	{
		AppDelegate::AudioPlayEffect("MS4/bodybuy.mp3");
		m_iHero3BuyDigit --;
	}
	else if ( bodyIndex2[m_iHero3Buy] == m_iHero3BuyAim )
	{
		m_iHero3BuyDigit = 0;
		OnBuy3Over();
		unschedule(schedule_selector(ccbChoseHero::OnBuy3Timer));
	}
	else
		AppDelegate::AudioPlayEffect("MS4/bodybuy.mp3");
}

void ccbChoseHero::OnBuy3Over()
{
	char buffer[255];
	sprintf(buffer, "tu16/item/%d.png", bodyIndex3[m_iHero3BuyAim]);
	CCSprite* pSprite = CCSprite::create(buffer);
	pSprite->setPosition(ccp(245+155*(bodyIndex[m_iHero3BuyAim]%3), 285-125*(bodyIndex[m_iHero3BuyAim]/3)));
	addChild(pSprite, 6);

	CCMoveTo* pAction1 = CCMoveTo::create(1.0f, ccp(400, 240));
	CCScaleTo* pAction2 = CCScaleTo::create(1.0f, 10.0f);
	pSprite->runAction( pAction1 );
	pSprite->runAction( CCSequence::create(pAction2, 
		CCCallFuncN::create(this, callfuncN_selector(ccbChoseHero::OnBuy3Disappear1)), NULL));

	getChildByTag(1001)->setVisible(false);
	setKeypadEnabled(false);
}

void ccbChoseHero::OnBuy3Disappear1( cocos2d::CCNode* _pNode )
{
	if ( _pNode )
		_pNode->removeFromParentAndCleanup(false);
	m_pHero3Board[0]->setVisible(false);
	m_pHero3Board[1]->setVisible(false);
	for ( int i = 0; i < 6; i++ )
	{
		m_pHero3Body1[i]->setVisible(false);
		m_pHero3Body2[i]->setVisible(false);
	}
	if ( m_pMenuStart )
		m_pMenuStart->setVisible(false);
	m_pMenuDisappear->setVisible(false);
	m_pLayerColor->setVisible(false);

	char buffer[255];
	sprintf(buffer, "tu14/jixie/ren%d.png", m_iHero3BuyAim+1);
	CCSprite* pSpriteItem = CCSprite::create(buffer);
	pSpriteItem->setScale(4.0f);
	pSpriteItem->setPosition(ccp(0,0));
	pSpriteItem->setAnchorPoint(ccp(0,0));
	m_pHero3Body->addChild(pSpriteItem);
	CCScaleTo* pAction2 = CCScaleTo::create(1.0f, 1.0f);
	pSpriteItem->runAction( CCSequence::create(pAction2, 
		CCCallFuncN::create(this, callfuncN_selector(ccbChoseHero::OnBuy3Disappear2)), NULL));

	
}

void ccbChoseHero::OnBuy3Disappear2( cocos2d::CCNode* _pNode )
{
	if ( _pNode )
		_pNode->removeFromParentAndCleanup(false);
	AppDelegate::AudioPlayEffect("MS4/bodydown.mp3");
	if ( AppDelegate::s_Hero3Array[m_iHero3BuyAim] == 0 )
	{
		AppDelegate::s_Hero3Array[m_iHero3BuyAim] = 1;
		AppDelegate::s_Hero3 --;
		char buffer[255];
		sprintf(buffer, "tu14/jixie/ren%d.png", m_iHero3BuyAim+1);
		CCSprite* pSpriteItem = CCSprite::create(buffer);
		pSpriteItem->setPosition(ccp(0,0));
		pSpriteItem->setAnchorPoint(ccp(0,0));
		m_pHero3Body->addChild(pSpriteItem);
	}
	if ( AppDelegate::s_Hero3 == 0 )
	{
		if ( m_pMenuShow )
			m_pMenuShow->setVisible(false);
	}
	else
	{
		if ( m_pMenuShow )
			m_pMenuShow->setVisible(true);
	}
	scheduleOnce(schedule_selector(ccbChoseHero::OnBuy3Disappear3), 0.1f);
}

void ccbChoseHero::OnBuy3Disappear3( float _t )
{
	CCSprite* pSprite = NULL;
	CCAnimate* pAction = NULL;
	if ( m_iHero3BuyAim < 5 )
	{
		pSprite = CCSprite::create("Animation/buybody/2/1.png");
		pAction = CCAnimate::create(common::CreateAnimationFrame("Animation/buybody/2/", 4, CCRect(0,0,143,208)));
	}
	else
	{
		pSprite = CCSprite::create("Animation/buybody/1/1.png");
		pAction = CCAnimate::create(common::CreateAnimationFrame("Animation/buybody/1/", 4, CCRect(0,0,144,95)));
	}
	pSprite->setPosition(ccp(bodyX[m_iHero3BuyAim], bodyY[m_iHero3BuyAim]));
	pSprite->setScaleX(bodyToward[m_iHero3BuyAim]);
	pSprite->runAction(CCSequence::create(pAction, pAction, pAction, CCCallFuncN::create(this, callfuncN_selector(ccbChoseHero::CallBack)), NULL));
	addChild(pSprite, 3);
}

void ccbChoseHero::CallBack( cocos2d::CCNode* _pNode )
{
	AppDelegate::SaveBody();
	if ( AppDelegate::s_FirstLogin != 2 )
	{ 
		m_bCanChose = false;
		m_bIsChose	= false;
	}
	_pNode->removeFromParentAndCleanup(true);
	isShowModules = false;
	isShowRobit = false;
	isShowHero2 = false;
	if(mainIndex==3 && AppDelegate::s_Hero3==0){
		mainIndex = 1;
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
	}else{
		setFingerPos(ccp(mainPos[mainIndex][0],mainPos[mainIndex][1]));
	}
	getChildByTag(1001)->setVisible(true);
	setKeypadEnabled(true);
}

void ccbChoseHero::keyBackClicked()
{
	if ( m_bPress || m_bCanChose )
		return;
	m_bPress = true;
	CCScene* pScene = CFirstLogin::CreateScene(false);
	AppDelegate::ChangeScene( pScene );
}
