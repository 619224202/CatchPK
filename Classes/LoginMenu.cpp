#include "LoginMenu.h"
#include "MainMenu.h"
#include "ChoseHero.h"
#include "AppDelegate.h"
#include "baseRes.h"
#include "Common.h"
#include "LoginMenu.h"
#include "HelpLayer.h"
#include "LoginEDLayer.h"
#include "AchieveAdd.h"
#include "json_lib.h"
#include "Filterword.h"
#include "PayService.h"
#include "GetItemLayer.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define d_BoardFixX		54
#define d_Min			156
#define d_Dist			103
#define d_Num			59
#define d_Max(i)		d_Min+d_Dist*(i-4)
#define d_AchieveIcon	55, 50
#define d_AchieveText	125, 62
#define d_AchieveBar	282, 35
#define d_AchieveMedal	403, 60
#define d_AchieveNum	420, 41
#define LOCATION_UP		50, 210
#define LOCATION_DOWN	50, -210
#define SIZE_TOWARD		40, 104

#define FILTERWORD_PATH "filterword.txt"

ccbLoginMenu* ccbLoginMenu::s_pLogin = NULL;

ccbLoginMenu::ccbLoginMenu()
	: m_AnimationManager(NULL)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	setKeypadEnabled(true);
#endif
	s_pLogin = this;
	m_bHelpMark = false;
	m_bAchieve = false;
	m_bPress = false;
	m_bEnter = false;
	m_bRank  = false;
	m_pItem1 = NULL;
	m_pItem2 = NULL;
	m_pItem3 = NULL;
	m_pselector=NULL;
	m_bInput = true;
	m_bExit	= false;
	m_bGetPrize	= false;
	m_bAbout	= false;
	m_iMoreType	= 0;
	groupIndex =MainGroup;
	butIndex = 0;
	m_sNameSave = "";
	m_iShowMode = 0;
	m_iShowMax = d_Max(d_Num);
	m_pHttpHandle = CHttpHandle::GetInstance();

	int pPos[7][2] = {{150,170},{253,22},{405,22},{561,22},{720,22},{763,326},{765,445}};
	for(int i=0;i<7;i++){
		for(int j=0;j<2;j++){
			butPos[i][j] = pPos[i][j];
		}
	}
	
	setPos[0][0] = 278;
	setPos[0][1] = 48;
	setPos[1][0] = 472;
	setPos[1][1] = 48;
	setPos[2][0] = 669;
	setPos[2][1] = 48;

	achievePos[0][0] = 290;
	achievePos[0][1] = 133;
	achievePos[1][0] = 290;
	achievePos[1][1] = 28;
	achievePos[2][0] = 290;
	achievePos[2][1] = -77;
	achievePos[3][0] = 290;
	achievePos[3][1] = -182;

	for(int i=0;i<5;i++){
		menuButIndex[i] = 0;
	}
	achievePage = 0;
	maxAchievePage = d_Num%4==0?d_Num/4:d_Num/4+1;
	//delete pPos;
	//butPos = 
	//CCMenuItem* pItemExit = CCMenuItemImage::create( 
	//	"tu7/tui.png", "tu7/tui2.png", this, menu_selector(ccbLoginMenu::OnExitGame));
	//pItemExit->setPosition(ccp(741, 434));
	//pItemExit->setAnchorPoint(ccp(0.5, 0.5));
	//CCMenu* pMenuExit = CCMenu::create(pItemExit, NULL);
	//pMenuExit->setPosition(ccp(0, 0));
	//addChild(pMenuExit);
}

ccbLoginMenu::~ccbLoginMenu()
{
	s_pLogin = NULL;
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	//CC_SAFE_RELEASE_NULL(m_pItem1);
	//CC_SAFE_RELEASE_NULL(m_pItem2);
	//CC_SAFE_RELEASE_NULL(m_pItem3);
}

void ccbLoginMenu::keyBackClicked()
{
	//CCLog("按下了返回");
	if(groupIndex!=MainGroup){
		OnBack();
	}else{
		OnExitGame();
	}
}

void ccbLoginMenu::keyEnterClicked(){
	
	switch(groupIndex){
		//首页
	case MainGroup:
		switch(menuButIndex[MainGroup]){
		case 0:
			
			OnMainMenu(this);
			break;
		case 1:
			OnShop(this);
			break;
		case 2:
			OnHelp(this);
			groupIndex = RankGroup;
			menuButIndex[RankGroup] = 0;
			break;
		case 3:
			OnAchieve(this);
			groupIndex = AchieveGroup;
			achievePage = 0;
			menuButIndex[AchieveGroup] = 0;
			break;
		case 4:
			OnAbout(this);
			groupIndex = AboutGroup;
			menuButIndex[AboutGroup] = 0;
			break;
		case 5:
			OnExitGame();
			groupIndex = DiaExitGroup;
			menuButIndex[DiaExitGroup] = 0;
			break;
		case 6:
			OnGetPrize(this);
			break;
		}
		
		break;
	case AboutGroup:	//其他
		switch(menuButIndex[AboutGroup]){
		case 0:	
			OnMoreHelp(this);
			break;
		case 1:
			OnMoreSet(this);
			break;
		case 2:
			OnMoreAbout(this);
			break;
		}
		break;
	case AchieveGroup:
		{
			if(!m_iShowMode){
				int achieveIndex = achievePage*4+menuButIndex[AchieveGroup];
				if ( m_iAchive[achieveIndex] == 100 && AppDelegate::s_Achieve[achieveIndex] == 0){
					OnGetAchive(this);
				}
			}
		}
		break;
	case DiaExitGroup:
		switch(menuButIndex[DiaExitGroup]){
		case 0:
			ExitYes(this);
			break;
		case 1:
			ExitNo(this);
			break;
		}
		break;
	}
	//CCLog("按下了确定键");
	
}

void ccbLoginMenu::keyArrowClicked(int arrow){
	//CCLog("按下了方向键:%d,%d",groupIndex,arrow);
	switch(groupIndex){
	case MainGroup:
		//CCLog("111111111");
		switch(arrow){
		case kTypeLeftArrowClicked:
			if(menuButIndex[MainGroup]>=1 && menuButIndex[MainGroup]<=4){
				menuButIndex[MainGroup]--;
			}else if(menuButIndex[MainGroup]==5||menuButIndex[MainGroup]==6){
				menuButIndex[MainGroup]=0;
			}
			break;
		case kTypeRightArrowClicked:
			if(menuButIndex[MainGroup]==0){
				menuButIndex[MainGroup] = 5;
			}else if(menuButIndex[MainGroup]>=1 && menuButIndex[MainGroup]<4){
				menuButIndex[MainGroup]++;
			}
			break;
		case kTypeDownArrowClicked:
			if(menuButIndex[MainGroup]==0){
				menuButIndex[MainGroup] = 1;
			}else if(menuButIndex[MainGroup]==5){
				menuButIndex[MainGroup] = 4;
			}else if(menuButIndex[MainGroup]==6){
				menuButIndex[MainGroup] = 5;
			}
			break;
		case kTypeUpArrowClicked:
			if(menuButIndex[MainGroup]==1){
				menuButIndex[MainGroup] = 0;
			}else if(menuButIndex[MainGroup]>=2&&menuButIndex[MainGroup]<=4){
				menuButIndex[MainGroup] = 5;
			}else if(menuButIndex[MainGroup]==5){
				menuButIndex[MainGroup] = 6;
			}
			break;
		}
		m_pselector->setPosition(ccp(butPos[menuButIndex[MainGroup]][0],butPos[menuButIndex[MainGroup]][1]));
		
		break;
	case AboutGroup:
		switch(arrow){
		case kTypeLeftArrowClicked:
			if(menuButIndex[AboutGroup]>0){
				menuButIndex[AboutGroup]--;
			}
			break;
		case kTypeRightArrowClicked:
			if(menuButIndex[AboutGroup]<2){
				menuButIndex[AboutGroup]++;
			}
			break;
		case kTypeDownArrowClicked:
			break;
		case kTypeUpArrowClicked:
			break;
		}
		
		{
			CCPoint nodePoint = ccp(setPos[menuButIndex[AboutGroup]][0]-m_pBoard->getPositionX(),setPos[menuButIndex[AboutGroup]][1]-m_pBoard->getPositionY());
			//CCLog("w=%f,h=%f",nodePoint.x,nodePoint.y);
			//CCLog("bX=%f,bY=%f,bpx=%f,bpy=%f",m_pBoard->getPositionX(),m_pBoard->getPositionY(),m_pBoard->getAnchorPoint().x,m_pBoard->getAnchorPoint().y);
			m_pBoard->getChildByTag(1002)->setPosition(nodePoint);
		}
		//m_pBoard->getChildByTag(1002)->setPositionX(100);
		//{
		//	CCPoint mpPos = m_pBoard->getParent()->convertToWorldSpace(m_pBoard->getPosition());
		//	CCPoint handerPos = ccp(setPos[menuButIndex[AboutGroup]][0],setPos[menuButIndex[AboutGroup]][1]);
		//	m_pBoard->getChildByTag(1002)->setPosition(setPos[menuButIndex[AboutGroup]][0],setPos[menuButIndex[AboutGroup]][1]);
		//	CCLog("x=%d,y=%d",handerPos.x-mpPos.x,handerPos.y-mpPos.y);
		//}
		break;
	case DiaExitGroup:
		if(arrow==kTypeLeftArrowClicked||arrow==kTypeRightArrowClicked){
			if(menuButIndex[DiaExitGroup]==0){
				menuButIndex[DiaExitGroup]=1;
			}else{
				menuButIndex[DiaExitGroup]=0;
			}
			if(menuButIndex[DiaExitGroup]==0){
				getChildByTag(65)->getChildByTag(1001)->setPosition(ccp(127,34));
			}else{
				getChildByTag(65)->getChildByTag(1001)->setPosition(ccp(301,34));
			}
		}
		break;
	case AchieveGroup:
		{
			int achieveIndex = achievePage*4+menuButIndex[AchieveGroup];
			if(arrow==kTypeDownArrowClicked){
				if(achieveIndex<(d_Num-1)){
					if(menuButIndex[AchieveGroup]<3){
						menuButIndex[AchieveGroup]++;
					}else{
						achievePage++;
						menuButIndex[AchieveGroup] = 0;
						for ( int i = 0; i < d_Num; i++ )
							if ( m_pBoard->getChildByTag(100+i)){
								CCNode* node = m_pBoard->getChildByTag(100+i);
								node->setPositionY( node->getPositionY() + d_Dist*4);
							}
					}
				}
			}else{
				if(achieveIndex>0){
					if(menuButIndex[AchieveGroup]>0){
						menuButIndex[AchieveGroup]--;
					}else{
						achievePage--;
						menuButIndex[AchieveGroup] = 3;
						for ( int i = 0; i < d_Num; i++ )
							if ( m_pBoard->getChildByTag(100+i)){
								CCNode* node = m_pBoard->getChildByTag(100+i);
								node->setPositionY( node->getPositionY() - d_Dist*4);
							}
					}
				}
			}
			if(achievePage==0){
				m_pUp->setVisible(false);
			}else {
				m_pUp->setVisible(true);
			}
			if(achievePage==maxAchievePage-1){
				m_pDown->setVisible(false);
			}else {
				m_pDown->setVisible(true);
			}
						
			CCNode* handNode = m_pBoard->getChildByTag(1002);
			handNode->setPosition(ccp(achievePos[menuButIndex[AchieveGroup]][0],achievePos[menuButIndex[AchieveGroup]][1]));
		}
		break;
	}
	
	//int butPos[][2] = BUTPOS;
//	CCLog("x=%d,y=%d",butPos[butIndex][0],butPos[butIndex][1]);
	
	//m_pselector->setPosition(ccp(100,100));
}

void ccbLoginMenu::keyNumClicked(int keyNum){
	//CCLog("按下了数字键:%d",keyNum);
	switch(keyNum){
	case kTypeNum0:
		if(groupIndex!=MainGroup){
			OnBack();
		}else{
			OnExitGame();
		}
		break;
	case kTypeNum1:
		if(groupIndex==AchieveGroup){
			ChangeAchieveShow(this);
		}
		break;
	}
}

void ccbLoginMenu::LoginScene(CCScene* _pScene, int _type)
{
	AppDelegate::AudioPlayBgm("MS/Music/mainmenu/mainmenu.mp3");
	int year, month, day, hour;
	year = month = day = hour = 0;
	
	if ( _type == 0 )
	{
		int a = AppDelegate::s_DateYear;
		int b = AppDelegate::s_DateMonth;
		int c = AppDelegate::s_DateDay;
		struct cc_timeval now; 
		CCTime::gettimeofdayCocos2d(&now, NULL); 
		struct tm *tm;
		time_t timeTemp = now.tv_sec;
		tm = localtime(&timeTemp);
		year = (now.tv_sec*1000+now.tv_usec/1000)/3600000;
		month = tm->tm_mon + 1;
		day = tm->tm_mday;
		hour = tm->tm_hour;
		year -= hour;
		if ( AppDelegate::s_DateMonth != month || 
			AppDelegate::s_DateDay != day )
		{
			if ( AppDelegate::s_DateYear - year > 30 )
			{
				AppDelegate::s_PrizeDayNum = 0;
			}
			AppDelegate::s_PrizeDayNum++;
			if ( AppDelegate::s_PrizeDayNum == 3 )
			{
				if ( AppDelegate::s_WeaponOwn[7] )
					AppDelegate::s_Money += 500;
				else
				{
					AppDelegate::s_BulletNum[7] = 150;
					AppDelegate::s_WeaponOwn[7] = 1;
				}
			}
			else if ( AppDelegate::s_PrizeDayNum == 4 )
			{
				AppDelegate::s_PrizeDayNum = 1;
			}
		}
	}

	CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("MainMenu", ccbLoginMenuLoader::loader());
	cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
	CCNode* pNode = ccbReader->readNodeGraphFromFile("11.ccbi"/*, this*/);
	((ccbLoginMenu*)pNode)->setAnimationManager(ccbReader->getAnimationManager());
	pNode->setPosition(ccp(0,0));
	SetScale(pNode);
	_pScene->addChild(pNode);
	ccbReader->release();
	((ccbLoginMenu*)pNode)->m_iEnterType = _type;

	//((ccbLoginMenu*)pNode)->m_pselector = ((ccbLoginMenu*)pNode)->createHandelCreator();
	//SetScale(((ccbLoginMenu*)pNode)->m_pselector);
	//_pScene->addChild(((ccbLoginMenu*)pNode)->m_pselector);

	
	if ( _type == 0 )
	{
		((ccbLoginMenu*)pNode)->Enter();
#ifndef GameTypeC
		if ( AppDelegate::s_DateMonth != month || 
			AppDelegate::s_DateDay != day ||
			AppDelegate::s_FirstLogin == 2 )
		{//每日奖
			((ccbLoginMenu*)pNode)->m_bEnter = true;
			((ccbLoginMenu*)pNode)->m_bGetPrize = true;
			AppDelegate::s_PrizeFree = 1;
			AppDelegate::s_PrizeSave[0] = -1;
			AppDelegate::s_DateYear	= year;
			AppDelegate::s_DateMonth= month; 
			AppDelegate::s_DateDay	= day;
			((ccbLoginMenu*)pNode)->m_bEnter = true;
			((ccbLoginMenu*)pNode)->setKeypadEnabled(false);  
			((ccbLoginMenu*)pNode)->setTouchEnabled(false);
			((ccbLoginEDLayer*)((ccbLoginMenu*)pNode)->m_pLED)->initPrize();
			((ccbLoginEDLayer*)((ccbLoginMenu*)pNode)->m_pLED)->Appear();
			((ccbLoginMenu*)pNode)->m_pselector->setVisible(false);
			//((ccbLoginMenu*)pNode)->Enter();
		}
#endif
	}
	else
	{
		
		((ccbLoginMenu*)pNode)->setKeypadEnabled(true);  
		((ccbLoginMenu*)pNode)->setTouchEnabled(true);
		((ccbLoginMenu*)pNode)->OnAchieve(NULL);
	}
	
}

void ccbLoginMenu::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	PayService::ShowBar();
	m_pNode = pNode->getChildByTag(2);
	m_pNode->setVisible(false);
	CCSprite* pSprite = NULL;

	m_pBoard = pNode->getChildByTag(3);
	pSprite = CCSprite::create("tu7/di.png");
	pSprite->setPosition(ccp(d_BoardFixX, 0));
	m_pBoard->addChild(pSprite, -1);
	
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_iLocation = d_Min;
	
	
#ifdef GameTypeA
	pSprite = CCSprite::create("tu7/waikuang.png");
	pSprite->setPosition(ccp(0, 0));
	m_pNode->addChild(pSprite, 0, 1);
	m_pUp = common::CreateAnimation("Animation/towardtips/1.png", CCPoint(LOCATION_UP), ccp(0.5f, 0.5f),
		"Animation/towardtips/", 4, CCRect(0, 0, SIZE_TOWARD), true);
	m_pUp->setVisible(false);
	pNode->getChildByTag(2)->addChild(m_pUp, 10);
	m_pDown = common::CreateAnimation("Animation/towardtips/1.png", CCPoint(LOCATION_DOWN), ccp(0.5f, 0.5f),
		"Animation/towardtips/", 4, CCRect(0, 0, SIZE_TOWARD), true);
	m_pDown->setScaleY(-1.0f);
	m_pDown->setVisible(false);
	pNode->getChildByTag(2)->addChild(m_pDown, 10);
//	CCLog("do ")
	char buffer[50];
	for ( int i = 0; i < d_Num; i++ )
	{
		int j = i /*> 43 ? i + 8 : i*/;
		CCSprite* pSprite = CCSprite::create("tu7/achiveboard.png");
		pSprite->setPosition(ccp(d_BoardFixX, d_Min - d_Dist*i));
		m_pBoard->addChild(pSprite, 0, 100+i);

		sprintf(buffer, "tu7/tubiao/%d.png", j+1);
		CCSprite* pSprite2 = CCSprite::create(buffer);
		pSprite2->setScale(0.9f);
		pSprite2->setPosition(CCPoint(d_AchieveIcon));
		pSprite->addChild(pSprite2, 2); 

		sprintf(buffer, "tu7/zi/%d.png", j+1);
		pSprite2 = CCSprite::create(buffer);
		pSprite2->setScale(0.9f);
		pSprite2->setAnchorPoint(ccp(0,0));
		pSprite2->setPosition(CCPoint(d_AchieveText));
		pSprite->addChild(pSprite2, 3);

		pSprite2 = CCSprite::create("tu7/xunzhang.png");
		pSprite2->setPosition(CCPoint(d_AchieveMedal));
		pSprite->addChild(pSprite2, 4);
		common::ShowNumber(pSprite, AchieveAdd::GetMedal(j), 20, 29, d_AchieveNum, "tu7/suzi.png", 888, 6);

		int t = 0;
		int num = AchieveAdd::GetNumNow(j);
		if ( num < 10 )
			t = 1;
		else if ( num < 100 )
			t = 2;
		else if ( num < 1000 )
			t = 3;
		else if ( num < 10000 )
			t = 4;
		else if ( num < 100000 )
			t = 5;
		else
			t = 6;
		common::ShowNumber(pSprite, num, 12, 16, 275, 28, "tu7/suzi2.png", 100);
		CCSprite* pSprite4 = CCSprite::create("tu7/xie.png");
		pSprite4->setPosition(ccp(275+12*t, 28));
		pSprite4->setAnchorPoint(ccp(0,0));
		pSprite->addChild(pSprite4);
		t++;
		common::ShowNumber(pSprite, AchieveAdd::GetNumNeed(j), 12, 16, 275+12*t, 28, "tu7/suzi2.png", 101);

		CCSprite* pSprite3 = CCSprite::create("tu7/jindi2.png");
		pSprite3->setPosition(ccp(121,28));
		pSprite3->setAnchorPoint(ccp(0,0));
		pSprite->addChild(pSprite3);

		CCSprite* pLoadingBar1 = CCSprite::create("tu7/jindu1.png");
		CCProgressTimer* pProgressBar1 = CCProgressTimer::create(pLoadingBar1);
		pProgressBar1->setPosition( ccp(121,28) );
		pProgressBar1->setAnchorPoint(ccp(0,0));
		pProgressBar1->setType(kCCProgressTimerTypeBar);//
		pProgressBar1->setMidpoint(ccp(0, 1));
		pProgressBar1->setBarChangeRate(ccp(1, 0));
		pSprite->addChild(pProgressBar1, 1);
		m_iAchive[j] = 100 * AchieveAdd::GetNumNow(j) / AchieveAdd::GetNumNeed(j);
		//m_iAchive[i] = rand()%105;///////////////////wait for delete
		if ( m_iAchive[j] >= 100 )
			m_iAchive[j] = 100;
		else
			m_iAchive[j] = m_iAchive[j] * 96 /100;
		pProgressBar1->setPercentage(m_iAchive[j]);

		CCNode* pBtnNode;
		if ( m_iAchive[j] == 100 )
		{
			if ( AppDelegate::s_Achieve[j] == 1 )
				pBtnNode = (CCNode*)CCSprite::create("tu7/yilingqu.png");
			else
			{
				CCMenuItem* pItem = CCMenuItemImage::create( 
					"tu7/weilingqu.png", "tu7/weilingqu2.png", this, menu_selector(ccbLoginMenu::OnGetAchive));
				pItem->setPosition(ccp(0,0));
				pBtnNode = (CCNode*)CCMenu::create(pItem, NULL);
			}
		}
		else
			pBtnNode = (CCNode*)CCSprite::create("tu7/weidacheng.png");
		pBtnNode->setPosition(ccp(500, 58));
		pSprite->addChild(pBtnNode, 0, j);

		/**
		m_pItem0 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::ChangeAchieveShow), 
			CCMenuItemImage::create( "tu7/xianshikelingqu.png", "tu7/xianshikelingqu2.png" ),
			CCMenuItemImage::create( "tu7/xianshiquanbu.png", "tu7/xianshiquanbu2.png" ),
			NULL );
		m_pItem0->setPosition(ccp(0, 0));
		CCMenu* pMenu = CCMenu::create(m_pItem0, NULL);
		pMenu->setPosition(ccp(619,12));
		m_pNode->getChildByTag(1)->addChild(pMenu, 1);
		*/
	}
	m_pItem0 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::ChangeAchieveShow), 
			CCMenuItemImage::create( "tu7/xianshikelingqu.png", "tu7/xianshikelingqu2.png" ),
			CCMenuItemImage::create( "tu7/xianshiquanbu.png", "tu7/xianshiquanbu2.png" ),
			NULL );
	m_pItem0->setPosition(ccp(0, 0));
	CCMenu* pMenu = CCMenu::create(m_pItem0, NULL);
	pMenu->setPosition(ccp(619,12));
	m_pNode->getChildByTag(1)->addChild(pMenu, 1);
#endif
	{//更多
		m_iMoreType = -1;
		pSprite = CCSprite::create("tu7/about.png");
		pSprite->setPosition(ccp(0, 0));
		m_pBoard->addChild(pSprite, 1, 98);

		//设置
		m_pItem1 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::OnMoreHelp), 
			CCMenuItemImage::create( "tu7/help1.png", "tu7/help2.png" ),
			CCMenuItemImage::create( "tu7/help2.png", "tu7/help2.png" ),
			NULL );
		m_pItem2 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::OnMoreSet), 
			CCMenuItemImage::create( "tu7/shezhi.png", "tu7/shezhi2.png" ),
			CCMenuItemImage::create( "tu7/shezhi2.png", "tu7/shezhi2.png" ),
			NULL );
		m_pItem3 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::OnMoreAbout), 
			CCMenuItemImage::create( "tu7/about1.png", "tu7/about2.png" ),
			CCMenuItemImage::create( "tu7/about2.png", "tu7/about2.png" ),
			NULL );
		m_pItem1->setPosition(ccp(228, 63));
		m_pItem2->setPosition(ccp(423, 63));
		m_pItem3->setPosition(ccp(619, 63));
		CCMenu* pMenu = CCMenu::create(m_pItem1, m_pItem2, m_pItem3, NULL);
		pMenu->setPosition(ccp(0,0));
		pSprite->addChild(pMenu, 1);

		CCSprite* pd = createHandelCreator();
		//SetScale(pd);
		m_pBoard->addChild(pd,10,1002);

		{//帮助
			CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			ccNodeLoaderLibrary->registerCCNodeLoader("ccbHelpLayer", ccbHelpLayerLoader::loader());
			cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			CCNode* pNode = ccbReader->readNodeGraphFromFile("21.ccbi");
			//SetScale(pNode);
			pNode->setVisible(false);
			pNode->setPosition(-411, -251);
			m_pHelpActionManager = ccbReader->getAnimationManager();
			((ccbHelpLayer*)pNode)->setAnimationManager(m_pHelpActionManager, this);
			ccbReader->release();
			m_pBoard->addChild(pNode, 1, 1);

#ifndef GameTypeC
			CCSprite* pSprite = common::CreateAnimation("tu7/shou1.png", ccp(406, 160), ccp(0.5f,0.5f),
				"tu7/shou", 2, CCRect(0,0,171,62), true);
			pNode->addChild(pSprite, 0, 1);
			if ( AppDelegate::s_HelpMark == 1 )
				pSprite->setVisible(false);
			AppDelegate::s_HelpMark = 1;
			AppDelegate::SaveHelpMark();
#endif
		}

		{//设置
			CCSprite* pBoard = CCSprite::create("tu7/ying.png");
			pBoard->setPosition(ccp(50,-46));
			pBoard->setVisible(false);
			m_pBoard->addChild(pBoard, 1, 10);
			m_pItemVoice = CCMenuItemToggle::createWithTarget(this, 
				menu_selector(ccbLoginMenu::OnSetVoice), 
				CCMenuItemImage::create( "tu7/guan.png", "tu7/guan.png" ),
				CCMenuItemImage::create( "tu7/kai.png", "tu7/kai.png" ),
				NULL );
			m_pItemVoice->setPosition(ccp(238,10));
			m_pItemVoice->setSelectedIndex(AppDelegate::s_VoiceOpen);

			m_pNameBtn = CCMenuItemImage::create( 
				"tu7/xiugai.png", "tu7/xiugai.png", this, menu_selector(ccbLoginMenu::InputName));
			m_pNameBtn->setPosition(ccp(142,170));

			CCMenu* pMenu = CCMenu::create(m_pItemVoice, m_pNameBtn, NULL);
			pMenu->setPosition(ccp(50,50));
			pBoard->addChild(pMenu);

			m_sNameSave = CCUserDefault::sharedUserDefault()->getStringForKey("name");
			if ( m_sNameSave == "" )
				m_sNameSave = "New Player";
			m_pNameInput = (CCTextFieldTTF*)CCTextFieldTTF::textFieldWithPlaceHolder("Your Name", "Thonburi", 36);
			m_pNameInput->setPosition(ccp(253, 219));
			m_pNameInput->setDelegate(this);
			m_pNameInput->setString(m_sNameSave.c_str());
			pBoard->addChild(m_pNameInput);
		}

#ifdef GameTypeA
		{//关于
			CCSprite* pSprite = CCSprite::create("tu7/aboutinfo.jpg");
			pSprite->setPosition(ccp(58, 0));
			pSprite->setVisible(false);
			m_pBoard->addChild(pSprite, 0, 20);
		}
#endif
	}

	for( int i = 0; i < d_Num+2; i++ )
		if ( m_pBoard->getChildByTag(98+i) )
			m_pBoard->getChildByTag(98+i)->setVisible(false);

	CCSprite* pStartGameBtn = common::CreateAnimation("Animation/StartGameBtn/1.png", ccp(60, 27), ccp(0,0),
		"Animation/StartGameBtn/", 6, CCRect(0,0,185,201), true);
	pStartGameBtn->setVisible(false);
	addChild(pStartGameBtn, 1, 89);
	
#ifndef GameTypeC
	{
		CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("ccbLoginEDLayer", ccbLoginEDLayerLoader::loader());
		cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		m_pLED = ccbReader->readNodeGraphFromFile("22.ccbi"/*, this*/);
		((ccbLoginEDLayer*)m_pLED)->setAnimationManager(ccbReader->getAnimationManager(), this);
		m_pLED->setPosition(ccp(0,0));
		//SetScale(m_pLED);
		addChild(m_pLED, 11);
		ccbReader->release();
	}

	{
		CCNode* pGetPrizeNode = pNode->getChildByTag(4);
		getChildByTag(4349)->setZOrder(2);
		pGetPrizeNode->setZOrder(3);
		CCSprite* pSprite = CCSprite::create("tu7/jiangli/1.png");
		pSprite->setPosition(ccp(0,0));
		CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("tu7/jiangli/", 3, CCRect(0,0,117,59)));
		pSprite->runAction(CCRepeatForever::create(CCSequence::create(pAction, CCDelayTime::create(2.0f), NULL)));
		pGetPrizeNode->addChild(pSprite);
	}

	{//排行榜
		pSprite = CCSprite::create("tu7/help.png");
		pSprite->setPosition(ccp(0, 0));
		m_pBoard->addChild(pSprite, 0, 99);

		CCSprite* pDi = CCSprite::create("tu7/paihangbang/di.png");
		pDi->setAnchorPoint(ccp(0,0));
		pDi->setPosition(ccp(115,1));
		pSprite->addChild(pDi, -1);

		m_pRankList = CCSprite::create("tu7/paihangbang/zhongkuang.png");
		m_pRankList->setAnchorPoint(ccp(0,0));
		m_pRankList->setPosition(ccp(150,40));
		pSprite->addChild(m_pRankList);

		m_pNameInput1 = (CCTextFieldTTF*)CCTextFieldTTF::textFieldWithPlaceHolder("Your Name", "Thonburi", 27);
		m_pNameInput1->setDelegate(this);
		m_pNameInput1->setString(m_sNameSave.c_str());
		m_pNameInput1->setAnchorPoint(ccp(0,0));
		m_pNameInput1->setPosition(ccp(107, 4));
		m_pRankList->addChild(m_pNameInput1, 0, 1001);

		m_pRank1 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::OnRank1), 
			CCMenuItemImage::create( "tu7/paihangbang/gaofu.png", "tu7/paihangbang/gaofu2.png" ),
			CCMenuItemImage::create( "tu7/paihangbang/gaofu2.png", "tu7/paihangbang/gaofu2.png" ),
			NULL );
		m_pRank2 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::OnRank2), 
			CCMenuItemImage::create( "tu7/paihangbang/chengjiu.png", "tu7/paihangbang/chengjiu2.png" ),
			CCMenuItemImage::create( "tu7/paihangbang/chengjiu2.png", "tu7/paihangbang/chengjiu2.png" ),
			NULL );
		m_pRank3 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::OnRank3), 
			CCMenuItemImage::create( "tu7/paihangbang/jiangshi.png", "tu7/paihangbang/jiangshi2.png" ),
			CCMenuItemImage::create( "tu7/paihangbang/jiangshi2.png", "tu7/paihangbang/jiangshi2.png" ),
			NULL );
		m_pRank4 = CCMenuItemToggle::createWithTarget(this, 
			menu_selector(ccbLoginMenu::OnRank4), 
			CCMenuItemImage::create( "tu7/paihangbang/xingxing.png", "tu7/paihangbang/xingxing2.png" ),
			CCMenuItemImage::create( "tu7/paihangbang/xingxing2.png", "tu7/paihangbang/xingxing2.png" ),
			NULL );
		m_pRank1->setPosition(ccp(162, 356));
		m_pRank2->setPosition(ccp(286, 356));
		m_pRank3->setPosition(ccp(410, 356));
		m_pRank4->setPosition(ccp(534, 356));
		CCMenu* pMenu = CCMenu::create(m_pRank1, m_pRank2, m_pRank3, m_pRank4, NULL);
		pMenu->setPosition(ccp(54,43));
		pSprite->addChild(pMenu);
	}
#endif
	//创建手指动画
	m_pselector= createHandelCreator();
	m_pselector->setPosition(ccp(butPos[menuButIndex[MainGroup]][0],butPos[menuButIndex[MainGroup]][1]));
	pNode->addChild(m_pselector,10,1001);
}

CCSprite* ccbLoginMenu::createHandelCreator(){
	CCTexture2D* img_selector = CCTextureCache::sharedTextureCache()->addImage("pt1.png");
	CCSpriteFrame* frame1 = CCSpriteFrame::createWithTexture(img_selector,CCRect(0,0,58,77));
	CCSpriteFrame* frame2 = CCSpriteFrame::createWithTexture(img_selector,CCRect(58,0,58,77));
	CCAnimation* c_hAnimation = CCAnimation::create();
	 c_hAnimation->setDelayPerUnit(0.05f);
	c_hAnimation->addSpriteFrame(frame1);
	c_hAnimation->addSpriteFrame(frame2);
	CCAnimate* c_hAnimate = CCAnimate::create(c_hAnimation);
	if(c_hAnimate==NULL){
		CCLog("animate create failed");
	}
	//CCActionInterval* action = CCSequence::create()
	CCSprite* m_pselector = CCSprite::create();
	if(m_pselector==NULL){
		CCLog("slelector is not live");
	}
	m_pselector->setAnchorPoint(ccp(0,1));
	
	m_pselector->initWithSpriteFrame(frame1);
	m_pselector->runAction(CCRepeatForever::create(c_hAnimate));
	m_pselector->setVisible(false);
	return m_pselector;
}

SEL_MenuHandler ccbLoginMenu::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbLoginMenu::OnMainMenu );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbLoginMenu::OnShop );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On4", ccbLoginMenu::OnAchieve );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On3", ccbLoginMenu::OnHelp );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On5", ccbLoginMenu::OnAbout );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On6", ccbLoginMenu::OnBack );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On7", ccbLoginMenu::OnGetPrize );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On8", ccbLoginMenu::OnExitGame );
	return NULL;    
}

SEL_CCControlHandler ccbLoginMenu::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}

bool ccbLoginMenu::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbLoginMenu::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbLoginMenu::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
}

void ccbLoginMenu::OnMainMenu( cocos2d::CCObject *pSender )
{
	if ( m_bExit ) 
		return;
	if ( m_bEnter )
		return;

#ifndef GameTypeC
	m_bEnter = true;
	//AppDelegate::ChangeScene( CMainMenu::StartMenu() );
	ccbGetItemLayer::s_pccbGetItemLayer = NULL;
	ccbLoginEDLayer::s_pccbLoginEDLayer = NULL;
	AppDelegate::ChangeScene( ccbChoseHero::CreateScene() );
	AppDelegate::AudioInit2();
#else
	m_bEnter = true;
	AppDelegate::ChangeScene( CMainMenu::StartMenu() );
#endif

}

void ccbLoginMenu::OnShop( cocos2d::CCObject *pSender )
{
	if ( m_bExit ) 
		return;
	if ( m_bEnter )
		return;
	m_bEnter = true;
	//AppDelegate::AudioPlayEffect("MS/Sound/Press.mp3");
	ccbGetItemLayer::s_pccbGetItemLayer = NULL;
	ccbLoginEDLayer::s_pccbLoginEDLayer = NULL;
	AppDelegate::ChangeScene( CMainMenu::StartMenu(0, -1, true) );
}

void ccbLoginMenu::OnAchieve( cocos2d::CCObject *pSender )
{
#ifdef GameTypeA
	if ( m_bExit ) 
		return;
	if ( m_bEnter )
		return;
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	if ( getChildByTag(3439) )
		getChildByTag(3439)->removeFromParent();
	m_bEnter = true;
	m_bAchieve = true;
	m_iType	= 1;
	AppDelegate::AudioPlayBgm("MS/Music/mainmenu/ShopAchieve.mp3");
	Achieve();
	for( int i = 0; i < 2; i++ )
		m_pBoard->getChildByTag(98+i)->setVisible(false);
	m_pItem0->setVisible(true);
	
		//SetScale(pd);
	CCNode* handNode = m_pBoard->getChildByTag(1002);
	handNode->setVisible(true);
	handNode->setPosition(ccp(achievePos[0][0],achievePos[0][1]));
	//AppDelegate::AudioPlayEffect("MS/Sound/EfAchieve.mp3");
#endif
#ifdef GameTypeB
	m_bPress = false;
	if ( getChildByTag(410) )
	{
		getChildByTag(410)->stopAllActions();
		removeChildByTag(410);
	}
	CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	pSprite->setPosition(ccp(400, 240));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::TipDisappear)), NULL));
	addChild(pSprite, 5, 410);
#endif
}

void ccbLoginMenu::OnHelp( cocos2d::CCObject *pSender )
{
	if ( m_bExit ) 
		return;
	if ( m_bEnter )
		return;
#ifndef GameTypeC
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	if ( getChildByTag(3439) )
		getChildByTag(3439)->removeFromParent();
	m_pHttpHandle->UpdateData(this);
	m_bEnter = true;
	m_iType = 2;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);
	for( int i = 0; i < d_Num+2; i++ )
		if ( m_pBoard->getChildByTag(98+i) )
			m_pBoard->getChildByTag(98+i)->setVisible(false);
	m_pBoard->getChildByTag(99)->setVisible(true);
	m_pNode->setVisible(false);
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 1800, 600);
	pCCLayerColor->setPosition(ccp(-1300,-300));
	m_pBoard->addChild(pCCLayerColor, -10, 14);
#ifdef GameTypeA
	m_pItem0->setVisible(false);
#endif
	//AppDelegate::AudioPlayEffect("MS/Sound/EfAchieve.mp3");
	m_bRank = true;
	OnRank1(NULL);
#else
	if ( getChildByTag(410) )
	{
		getChildByTag(410)->stopAllActions();
		removeChildByTag(410);
	}
	CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	pSprite->setPosition(ccp(400, 240));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::TipDisappear)), NULL));
	addChild(pSprite, 5, 410);
#endif
}

void ccbLoginMenu::OnAbout(cocos2d::CCObject *pSender)
{
	if ( m_bExit ) 
		return;
	if ( m_bEnter )
		return;
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	if ( getChildByTag(3439) )
		getChildByTag(3439)->removeFromParent();
	m_bEnter = true;
	m_iType = 3;
	m_AnimationManager->setAnimationCompletedCallback(this,callfunc_selector(ccbLoginMenu::setAboutHandel));
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);
	for( int i = 0; i < d_Num+1; i++ )
		if ( m_pBoard->getChildByTag(99+i))
			m_pBoard->getChildByTag(99+i)->setVisible(false);
	m_pBoard->getChildByTag(98)->setVisible(true);
	//CCPoint nodePoint = m_pBoard->convertToNodeSpace(ccp(setPos[menuButIndex[AboutGroup]][0]-m_pBoard->getPositionX(),setPos[menuButIndex[AboutGroup]][1]-m_pBoard->getPositionY()));
	CCPoint nodePoint = m_pBoard->convertToNodeSpace(ccp(setPos[menuButIndex[AboutGroup]][0],setPos[menuButIndex[AboutGroup]][1]));
	m_pBoard->getChildByTag(1002)->setPosition(nodePoint);
	CCPoint point = m_pBoard->convertToWorldSpace(nodePoint);
	CCLog("x=%f,y=%f",point.x,point.y);
	m_pBoard->getChildByTag(1002)->setVisible(true);
	m_pNode->setVisible(false);
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 1800, 600);
	pCCLayerColor->setPosition(ccp(-1300,-300));
	m_pBoard->addChild(pCCLayerColor, -10, 14);
	//m_pselector->setVisible(true);
	
#ifdef GameTypeA
	m_pItem0->setVisible(false);
#endif

#ifndef GameTypeC
	AppDelegate::AudioPlayBgm("MS2/moreIndex.mp3");
#endif
	OnMore1(false);
	OnMore2(false);
	OnMore3(false);
}

void ccbLoginMenu::setAboutHandel(){
	CCLog("animation over");
	//CCPoint nodePoint = m_pBoard->convertToNodeSpace(ccp(setPos[menuButIndex[AboutGroup]][0],setPos[menuButIndex[AboutGroup]][1]));
	m_pBoard->getChildByTag(1002)->setPosition(ccp(setPos[menuButIndex[AboutGroup]][0]-m_pBoard->getPositionX(),setPos[menuButIndex[AboutGroup]][1]-m_pBoard->getPositionY()));
	//CCPoint point = m_pBoard->convertToWorldSpace(nodePoint);
}

void ccbLoginMenu::Show(CCNode* sender)
{
	if ( m_iType != 3 )
		return;
	switch ( m_iMoreType )
	{
	case -1:
	case 0:
		OnMoreHelp(NULL);
		break;
	case 1:
		OnMoreSet(NULL);
		break;
	case 2:
		OnMoreAbout(NULL);
		break;
	}
}

void ccbLoginMenu::Enter()
{
	setTouchEnabled(true);
	setKeypadEnabled(true);
	m_bEnter = false;
	m_bGetPrize = false;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
}

void ccbLoginMenu::Achieve()
{
	if ( m_bExit ) 
		return;
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	if ( getChildByTag(3439) )
		getChildByTag(3439)->removeFromParent();
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);

	AppDelegate::AudioPlayEffect("MS2/rankPress.mp3");
	for( int i = 0; i < d_Num+2; i++ )
		if ( m_pBoard->getChildByTag(98+i) )
			m_pBoard->getChildByTag(98+i)->setVisible(true);
	m_pNode->setVisible(true);

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 1800, 600);
	pCCLayerColor->setPosition(ccp(-1400,-300));
	m_pBoard->addChild(pCCLayerColor, -10, 14);
	scheduleOnce(schedule_selector(ccbLoginMenu::AchieveStart), 1.5f);
}

void ccbLoginMenu::OnBack( cocos2d::CCObject *pSender )
{
	if ( m_bExit ) 
		return;
	if ( !m_bEnter || m_bGetPrize )
		return;
	m_pBoard->removeChildByTag(14);
	m_pBoard->getChildByTag(1002)->setVisible(false);
	m_iType = 0;
	m_iRankType = -1;
	m_bEnter = false;
	m_bRank = false;
	if ( m_bAchieve || m_iType == 3)
	{
		AppDelegate::AudioPlayBgm("MS/Music/mainmenu/mainmenu.mp3");
		m_bAchieve = false;
	}
	if ( !m_iMoreType )
		m_iMoreType = -1;
	TipDisappear();
	HelpMarkShow();
	OnMore1(false);
	OnMore2(false);
	OnMore3(false);
	groupIndex = MainGroup;
#ifndef GameTypeC
	m_pDown->setVisible(false);
#endif
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy copy", 0.0f);
	//AppDelegate::AudioPlayEffect("MS/Sound/EfFirstLogin.mp3");
}

void ccbLoginMenu::AchieveStart(float _t)
{
	m_bPress = false;
	m_iShowMode = 1 - m_iEnterType;
	m_iEnterType = 0;
	m_pDown->setVisible(true);
	ChangeAchieveShow(NULL);
}

void ccbLoginMenu::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( m_bExit ) 
		return;
	if ( m_bRank )
	{
		int x = ((CCTouch*)(*pTouches->begin()))->getLocation().x;
		int y = ((CCTouch*)(*pTouches->begin()))->getLocation().y;
		if ( x > 297*Scale_X && x < 463*Scale_X )
		{
			if ( y > 61*Scale_Y && y < 86*Scale_Y )
			{
				m_pNameInput->attachWithIME();
			}
		}
		return;
	}
	if ( !m_bAchieve && !m_bAbout)
		return;
	m_bPress = true;
	m_iPress = ((CCTouch*)(*pTouches->begin()))->getLocation().y;

//#ifndef Plat_Telecom
//#ifndef Plat_Unicom
//	if ( m_bAbout )
//		//m_pBoard->getChildByTag(20)->pauseSchedulerAndActions();
//		m_pBoard->getChildByTag(20)->stopAllActions();
//#endif
//#endif
}

void ccbLoginMenu::ccTouchesMoved( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( !m_bPress )
		return;
	int y = ((CCTouch*)(*pTouches->begin()))->getLocation().y;

//	if ( m_bAbout )
//	{
//#ifdef GameTypeA
//#ifndef Plat_Telecom
//		int position = m_pBoard->getChildByTag(20)->getPositionY()+y-m_iPress;
//		if ( position < -850 )
//			position = -850;
//		else if ( position > 850 )
//			position = 850;
//		m_pBoard->getChildByTag(20)->setPositionY(position);
//		m_iPress = y;
//#endif
//#endif
	/*	}
	else */if ( m_bAchieve )
	{
		m_iLocation += 2*(y - m_iPress);
		if ( m_iLocation <= d_Min )
		{
			m_iLocation = d_Min;
			m_pUp->setVisible(false);
		}
		else 
		{
			int max = m_iShowMax > d_Max(4) ? m_iShowMax : d_Max(4);
			if ( m_iLocation >= max )
			{
				m_iLocation = max;
				m_pDown->setVisible(false);
			}
			else
				m_pDown->setVisible(true);
		}
		if ( m_iLocation <= d_Min )
			m_pUp->setVisible(false);
		else
			m_pUp->setVisible(true);

		m_iPress = y;
		if ( m_iShowMode )
		{
			int j = 0;
			for ( int i = 0; i < d_Num; i++ )
				if ( m_pBoard->getChildByTag(100+i) )
					if ( m_pBoard->getChildByTag(100+i)->isVisible() )
					{
						m_pBoard->getChildByTag(100+i)->setPositionY( m_iLocation - d_Dist*j );
						j++;
					}
		}
		else
			for ( int i = 0; i < d_Num; i++ )
				if ( m_pBoard->getChildByTag(100+i) )
					m_pBoard->getChildByTag(100+i)->setPositionY( m_iLocation - d_Dist*i);
	}
}

void ccbLoginMenu::ccTouchesEnded( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( !m_bPress )
		return;
	m_bPress = false;
//	if ( m_bAbout )
//		//m_pBoard->getChildByTag(20)->resumeSchedulerAndActions();
//	{
//#ifdef GameTypeA
//#ifndef Plat_Telecom
//#ifndef Plat_Unicom
//		float y = 850 - m_pBoard->getChildByTag(20)->getPositionY();
//		float time = y / 500.0f * 3.0f;
//		if ( time > 0 )
//		{
//			CCMoveBy* pAction = CCMoveBy::create(time, ccp(0, y));
//			m_pBoard->getChildByTag(20)->runAction(CCSequence::create(pAction, /*CCDelayTime::create(1.0f),*/ 
//				CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::SeeAgain)), NULL));
//		}
//		else
//		{
//			m_pBoard->getChildByTag(20)->runAction(CCSequence::create(CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::SeeAgain)), NULL));
//		}
//#endif
//#endif
//#endif
	//}
}

void ccbLoginMenu::Appear( CCNode* sender /*= NULL*/ )
{
	getChildByTag(89)->setVisible(true);
	if(!m_bEnter)
		m_pselector->setVisible(true);
	if ( !getChildByTag(3438) )
	{
		ShakeName();
	}
#ifdef ViolentVersion
	if ( !getChildByTag(3439) )
	{
		CCSprite* pSprite = CCSprite::create("tu7/baoli.png");
		pSprite->setPosition(ccp(475, 302));
		addChild(pSprite, 1, 3439);
	}
#endif
}

void ccbLoginMenu::Disappear( CCNode* sender /*= NULL*/ )
{
	getChildByTag(89)->setVisible(false);
	m_pselector->setVisible(false);
}

cocos2d::SEL_CallFuncN ccbLoginMenu::onResolveCCBCCCallFuncSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "appear", ccbLoginMenu::Appear);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "appear1", ccbLoginMenu::Appear);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "disappear", ccbLoginMenu::Disappear);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "show", ccbLoginMenu::Show);
	return 0;
}

void ccbLoginMenu::OnMoreHelp( CCObject* sender )
{
	if ( m_bExit ) 
		return;
	if ( m_iMoreType )
		m_pHelpActionManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	if ( getChildByTag(3439) )
		getChildByTag(3439)->removeFromParent();

	((ccbHelpLayer*)(m_pBoard->getChildByTag(1)))->m_iHelpType = 0;
	m_iMoreType = 0;
	m_bHelpMark = true;
	OnMore1(true);
	OnMore2(false);
	OnMore3(false);
	m_pItem1->setSelectedIndex(1);
	m_pItem2->setSelectedIndex(0);
	m_pItem3->setSelectedIndex(0);
}

void ccbLoginMenu::OnMoreSet( CCObject* sender )
{
	if ( m_bExit ) 
		return;
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	if ( getChildByTag(3439) )
		getChildByTag(3439)->removeFromParent();
	m_iMoreType = 1;
	OnMore1(false);
	OnMore2(true);
	OnMore3(false);
	m_pItem1->setSelectedIndex(0);
	m_pItem2->setSelectedIndex(1);
	m_pItem3->setSelectedIndex(0);
	m_pBoard->getChildByTag(1)->getChildByTag(1)->setVisible(false);
}

void ccbLoginMenu::OnMoreAbout( CCObject* sender )
{
	if ( m_bExit ) 
		return;
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	if ( getChildByTag(3439) )
		getChildByTag(3439)->removeFromParent();
	m_iMoreType = 2;
	OnMore1(false);
	OnMore2(false);
	OnMore3(true);
	m_pItem1->setSelectedIndex(0);
	m_pItem2->setSelectedIndex(0);
	m_pItem3->setSelectedIndex(1);
	m_pBoard->getChildByTag(1)->getChildByTag(1)->setVisible(false);
}

void ccbLoginMenu::OnSetVoice( CCObject* sender )
{
	if ( m_bExit ) 
		return;
	AppDelegate::AudioSwitch();
	m_pItemVoice->setSelectedIndex(AppDelegate::s_VoiceOpen);
	if ( AppDelegate::s_VoiceOpen )
		AppDelegate::AudioPlayBgm("MS/Music/mainmenu/mainmenu.mp3");
	else
		AppDelegate::AudioStopBgm();

}

void ccbLoginMenu::OnMore1( bool _bShow )
{
	AppDelegate::AudioPlayEffect("MS2/rankPress.mp3");
	for ( int i = 1; i < 2; i++ )
	{
		m_pBoard->getChildByTag(i)->setVisible(_bShow);
	}
}

void ccbLoginMenu::OnMore2( bool _bShow )
{
	AppDelegate::AudioPlayEffect("MS2/rankPress.mp3");
	for ( int i = 10; i < 11; i++ )
	{
		m_pBoard->getChildByTag(i)->setVisible(_bShow);
	}
}

void ccbLoginMenu::OnMore3( bool _bShow )
{
	AppDelegate::AudioPlayEffect("MS2/rankPress.mp3");
	m_bAbout = _bShow;

#ifdef GameTypeA
	CCNode* pSprite = m_pBoard->getChildByTag(20);
#ifdef No_About
	_bShow = false;
#endif
	pSprite->setVisible(_bShow);
	if ( _bShow )
	{
#ifdef Plat_Telecom
		pSprite->setPositionY(45);
#else
#ifdef Plat_Unicom
		pSprite->setPositionY(45);
#else
		pSprite->setPositionY(-600);
		pSprite->stopAllActions();
		CCMoveBy* pAction = CCMoveBy::create(7.8f, ccp(0, 1300));
		pSprite->runAction(CCSequence::create(pAction, /*CCDelayTime::create(1.0f), */
			CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::SeeAgain)), NULL));
#endif
#endif
	}
#endif
}

void ccbLoginMenu::SeeAgain()
{
#ifdef GameTypeA
	CCNode* pSprite = m_pBoard->getChildByTag(20);
	pSprite->setPositionY(-750);
	pSprite->stopAllActions();
	CCMoveBy* pAction = CCMoveBy::create(9.0f, ccp(0, 1500));
	pSprite->runAction(CCSequence::create(pAction, /*CCDelayTime::create(1.0f),*/ 
		CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::SeeAgain)), NULL));
#endif
}

void ccbLoginMenu::OnGetPrize( cocos2d::CCObject *pSender )
{
	if ( m_bExit ) 
		return;
	if ( m_bEnter )
		return;
#ifndef GameTypeC
	m_bEnter = true;
	m_bGetPrize = true;
	for( int i = 0; i < d_Num+2; i++ )
		if ( m_pBoard->getChildByTag(98+i) )
			m_pBoard->getChildByTag(98+i)->setVisible(false);
	m_pNode->setVisible(false);
	//m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline copy", 0.0f);
	((ccbLoginEDLayer*)m_pLED)->initPrize();
	((ccbLoginEDLayer*)m_pLED)->Appear();
	setKeypadEnabled(false);
	m_pselector->setVisible(false);
#else
	if ( getChildByTag(410) )
	{
		getChildByTag(410)->stopAllActions();
		removeChildByTag(410);
	}
	CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	pSprite->setPosition(ccp(400, 240));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::TipDisappear)), NULL));
	addChild(pSprite, 5, 410);
#endif
}

void ccbLoginMenu::OnGetAchive( cocos2d::CCObject* sender )
{
	if ( m_bExit ) 
		return;
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	if ( getChildByTag(3439) )
		getChildByTag(3439)->removeFromParent();
	CCMenu *menu = (CCMenu*)((CCMenuItem*)sender)->getParent();
	int i = menu->getTag();
	if ( AppDelegate::s_Achieve[i] == 1 )
		return;
	AppDelegate::AudioPlayEffect("MS3/getAchieve.mp3");
	AppDelegate::s_Achieve[i] = 1;
	AppDelegate::s_Medal += AchieveAdd::GetMedal(i);
	CCNode* pParent = menu->getParent();
	menu->removeFromParentAndCleanup(true);
	CCSprite* pSprite = CCSprite::create("tu7/yilingqu.png");
	pSprite->setPosition(ccp(500, 58));
	pParent->addChild(pSprite, 0, i);
	AppDelegate::SaveAchievement(i);
}

void ccbLoginMenu::ChangeAchieveShow(cocos2d::CCObject* sender)
{
	AppDelegate::AudioPlayEffect("MS2/rankPress.mp3");
	if ( !m_iShowMode )
	{
		m_iLocation = 0;
		m_iShowMode = 1;
		m_iShowMax = 0;
		for ( int i = 0; i < d_Num; i++ )
		{
			if ( AppDelegate::s_Achieve[i] == 0 && m_iAchive[i] == 100 )
			{
				m_pBoard->getChildByTag(100+i)->setPosition(ccp(d_BoardFixX, d_Min - d_Dist*m_iShowMax));
				m_pBoard->getChildByTag(100+i)->setVisible(true);
				m_iShowMax++;
			}
			else
				m_pBoard->getChildByTag(100+i)->setVisible(false);
		}
		if(m_iShowMax>0){
			maxAchievePage = m_iShowMax%4==0?m_iShowMax/4:m_iShowMax/4+1;
			achievePage = 0;
			menuButIndex[AchieveGroup] = 0;
			m_pBoard->getChildByTag(1002)->setPosition(ccp(achievePos[0][0],achievePos[0][1]));
		}else{
			m_pBoard->getChildByTag(1002)->setVisible(false);
		}

		if ( m_iShowMax < 5 )
		{
			if ( m_iShowMax == 0 )
			{
				if ( m_pBoard->getChildByTag(410) )
				{
					m_pBoard->getChildByTag(410)->stopAllActions();
					m_pBoard->removeChildByTag(410);
				}
				CCSprite* pSprite = CCSprite::create("tu7/wu.png");
				pSprite->setPosition(ccp(50, 0));
				m_pBoard->addChild(pSprite, 5, 410);
			}
			m_pDown->setVisible(false);
		}
		else
			m_pDown->setVisible(true);
		m_iShowMax = d_Max(m_iShowMax);
		 
	}
	else
	{
		TipDisappear();
		m_iLocation = 0;
		m_iShowMode = 0;
		m_iShowMax = d_Max(d_Num);
		for ( int i = 0; i < d_Num; i++ )
		{
			m_pBoard->getChildByTag(100+i)->setPosition(ccp(d_BoardFixX, d_Min - d_Dist*i));
			m_pBoard->getChildByTag(100+i)->setVisible(true);
		}
		m_pDown->setVisible(true);
		maxAchievePage = d_Num%4==0?d_Num/4:d_Num/4+1;
		achievePage = 0;
		menuButIndex[AchieveGroup] = 0;
		m_pBoard->getChildByTag(1002)->setPosition(ccp(achievePos[0][0],achievePos[0][1]));
	}
	m_pUp->setVisible(false);
	m_pItem0->setSelectedIndex(m_iShowMode);

}

void ccbLoginMenu::InputName( cocos2d::CCObject* sender )
{
	if ( m_bExit ) 
		return;
	m_pNameInput->attachWithIME();
}

bool ccbLoginMenu::onTextFieldInsertText( CCTextFieldTTF * pSender, const char * text, int nLen )
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_pNameInput->setString(text);
	int length = m_pNameInput->getCharCount();
	if ( length > 6 || m_pNameInput->getString() == "" || !common::CanInput(text) )
	{
		if ( getChildByTag(146) )
		{
			getChildByTag(146)->stopAllActions();
			removeChildByTag(146);
		}
		CCSprite* pSprite = CCSprite::create("tu7/kuai.png");
		pSprite->setAnchorPoint(ccp(0.5f, 0.5f));
		pSprite->setPosition(ccp(470, 295));
		CCFadeOut* pAction = CCFadeOut::create(4.0f);
		pSprite->runAction(CCSequence::create(pAction,
			CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::disappear)), NULL));
		addChild(pSprite, 50, 146);
		m_pNameInput->setString(m_sNameSave.c_str());
		m_pNameInput1->setString(m_sNameSave.c_str());
	}
	else
	{
		m_sNameSave = text;
		//((CCLabelTTF*)m_pRankList->getChildByTag(1001))->setString(m_sNameSave.c_str());
		m_pNameInput->setString(m_sNameSave.c_str());
		m_pNameInput1->setString(m_sNameSave.c_str());
		CCUserDefault::sharedUserDefault()->setStringForKey("name", m_pNameInput->getString());
		CCUserDefault::sharedUserDefault()->flush();
		m_pHttpHandle->UpdateData(this);
		ShowRank(m_iRankType);
	}
#else
	std::string oldName = m_pNameInput->getString();
	m_pNameInput->setString((oldName+text).c_str());
	int length = m_pNameInput->getCharCount();
	if ( length > 6 || !common::CanInput((oldName+text).c_str()) )
	{
		if ( getChildByTag(146) )
		{
			getChildByTag(146)->stopAllActions();
			removeChildByTag(146);
		}
		CCSprite* pSprite = CCSprite::create("tu7/kuai.png");
		pSprite->setAnchorPoint(ccp(0.5f, 0.5f));
		pSprite->setPosition(ccp(470, 295));
		CCFadeOut* pAction = CCFadeOut::create(4.0f);
		pSprite->runAction(CCSequence::create(pAction,
			CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::disappear)), NULL));
		addChild(pSprite, 50, 146);

		m_pNameInput->setString(m_sNameSave.c_str());
		m_pNameInput1->setString(m_sNameSave.c_str());
	}
	else
	{
		m_sNameSave = oldName + text;
		m_pNameInput->setString(m_sNameSave.c_str());
		m_pNameInput1->setString(m_sNameSave.c_str());
		CCUserDefault::sharedUserDefault()->setStringForKey("name", m_pNameInput->getString());
		CCUserDefault::sharedUserDefault()->flush();
		m_pHttpHandle->UpdateData(this);
		ShowRank(m_iRankType);
	}
#endif
	return true;
}

void ccbLoginMenu::disappear()
{
	getChildByTag(146)->stopAllActions();
	removeChildByTag(146);
}

void ccbLoginMenu::OnRank1( cocos2d::CCObject* sender )
{
	m_pRank1->setSelectedIndex(1);
	m_pRank2->setSelectedIndex(0);
	m_pRank3->setSelectedIndex(0);
	m_pRank4->setSelectedIndex(0);
	ShowRank(0);
}

void ccbLoginMenu::OnRank2( cocos2d::CCObject* sender )
{
	m_pRank1->setSelectedIndex(0);
	m_pRank2->setSelectedIndex(1);
	m_pRank3->setSelectedIndex(0);
	m_pRank4->setSelectedIndex(0);
	ShowRank(1);
}

void ccbLoginMenu::OnRank3( cocos2d::CCObject* sender )
{
	m_pRank1->setSelectedIndex(0);
	m_pRank2->setSelectedIndex(0);
	m_pRank3->setSelectedIndex(1);
	m_pRank4->setSelectedIndex(0);
	ShowRank(2);
}

void ccbLoginMenu::OnRank4( cocos2d::CCObject* sender )
{
	m_pRank1->setSelectedIndex(0);
	m_pRank2->setSelectedIndex(0);
	m_pRank3->setSelectedIndex(0);
	m_pRank4->setSelectedIndex(1);
	ShowRank(3);
}

void ccbLoginMenu::ShowRank( int _type )
{
	if ( _type < 0 )
		return;
	m_iRankType = _type;
	AppDelegate::AudioPlayEffect("MS2/rankPress.mp3");
	for ( int i = 0; i < 6; i++ )
	{
		rank_rank[i] = 0;
		rank_name[i] = "";
		rank_score[i] = 0;
	}
	ShowRank();
	m_pHttpHandle->GetData(_type);
}

void ccbLoginMenu::ShowRank( int _type, const char* _char )
{
	if ( m_iRankType != _type )
		return;

	std::string str = _char;
	Json::Reader reader;
	Json::Value value;
	value.clear();
	reader.parse(str, value);
	for ( int i = 0; i < 6; i++ )
	{
		if ( value["gameChartsSingleVos"][i]["order"].isNull() )
			continue;
		rank_rank[i] = value["gameChartsSingleVos"][i]["order"].asInt();
		rank_name[i] = value["gameChartsSingleVos"][i]["playerNameString"].asString();
		rank_score[i] = value["gameChartsSingleVos"][i]["score"].asInt();
	}
	ShowRank();
}

void ccbLoginMenu::ShowRank()
{	
	bool remove = false;
	if ( m_pRankList->getChildByTag(1) )
		remove = true;
	for ( int i = 0; i < 5; i++ )
	{
		if (remove)
			m_pRankList->removeChildByTag(i+1);
		CCSprite* pSprite = CCSprite::create("tu7/paihangbang/xiaokuang.png");
		pSprite->setAnchorPoint(ccp(0,0));
		pSprite->setPosition(ccp(12, 272-56*i));
		m_pRankList->addChild(pSprite, 0, i+1);

		if ( rank_rank[i] == 0 )
			continue;

		if ( rank_rank[i] < 4 )
		{
			char buffer[255];
			sprintf(buffer, "tu7/paihangbang/%d.png", rank_rank[i]);
			CCSprite* pMedal = CCSprite::create(buffer);
			pMedal->setPosition(ccp(12, 27));
			pSprite->addChild(pMedal);
		}
		float sclae = 1.0f;
		int xdist = 0;
		int ydist = 0;
		if ( rank_rank[i] < 10 )
		{
			xdist = 0;
		}
		else if ( rank_rank[i] < 100 )
		{
			xdist = -14;
		}
		else if ( rank_rank[i] < 1000 )
		{
			sclae = 0.8f;
			xdist = -18;
			ydist = 1;
		}
		else if ( rank_rank[i] < 10000 )
		{
			sclae = 0.7f;
			xdist = -20;
			ydist = 4;
		}
		else if ( rank_rank[i] < 100000 )
		{
			sclae = 0.5f;
			xdist = -16;
			ydist = 6;
		}
		common::ShowNumber(pSprite, rank_rank[i], 26, 30, 32+xdist, 15+ydist, "tu7/paihangbang/paiming.png", 888, 0, sclae);
		if ( rank_name[i] != "" )
		{
			CCLabelTTF* pLabel = CCLabelTTF::create(rank_name[i].c_str(), "Arial", 27,
				CCSizeMake(170, 32), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
			pLabel->setAnchorPoint(ccp(0,0));
			pLabel->setPosition(ccp(97, 15));
			pSprite->addChild(pLabel);
		}
		common::ShowNumber(pSprite, rank_score[i], 26, 30, 290, 16, "tu7/paihangbang/paiming.png", 889);
	}

	//if ( rank_rank[5] != 0 )
	//{
		float sclae = 1.0f;
		int xdist = 0;
		int ydist = 0;
		if ( rank_rank[5] < 10 )
		{
			xdist = 0;
		}
		else if ( rank_rank[5] < 100 )
		{
			xdist = -14;
		}
		else if ( rank_rank[5] < 1000 )
		{
			sclae = 0.8f;
			xdist = -18;
			ydist = 0;
		}
		else if ( rank_rank[5] < 10000 )
		{
			sclae = 0.7f;
			xdist = -20;
			ydist = 3;
		}
		else if ( rank_rank[5] < 100000 )
		{
			sclae = 0.5f;
			xdist = -16;
			ydist = 5;
		}
		common::ShowNumber(m_pRankList, rank_rank[5], 26, 30, 40+xdist, 5+ydist, "tu7/paihangbang/paiming.png", 888, 0, sclae);
		common::ShowNumber(m_pRankList, rank_score[5], 26, 30, 300, 5, "tu7/paihangbang/paiming.png", 889);
	//}
}

void ccbLoginMenu::HelpMarkShow()
{
	if ( m_bHelpMark )
	{
		m_pBoard->getChildByTag(1)->getChildByTag(1)->setVisible(false);
		m_bHelpMark = false;
	}
}

void ccbLoginMenu::OnExitGame( cocos2d::CCObject* sender )
{
	if ( m_bEnter || m_bGetPrize )
		return;
	if ( m_bExit )
		return;
	m_bExit = true;
#ifdef Plat_91
	scheduleOnce(schedule_selector(ccbLoginMenu::ExitProtecter), 2.0f);
	PayService::exitGame();
#else
	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	pCCLayerColor->setAnchorPoint(ccp(0,0));
	pCCLayerColor->setPosition(ccp(0,0));
	addChild(pCCLayerColor, 19, 1488);

	CCSprite* pBoard = CCSprite::create("tu7/exit/tuichu.png");
	pBoard->setPosition(ccp(400, 240));
	addChild(pBoard, 20, 65);

	CCMenuItemImage* pYes = CCMenuItemImage::create( 
		"tu7/exit/shi.png", "tu7/exit/shi2.png", this, menu_selector(ccbLoginMenu::ExitYes));
	CCMenuItemImage* pNo = CCMenuItemImage::create( 
		"tu7/exit/fou.png", "tu7/exit/fou2.png", this, menu_selector(ccbLoginMenu::ExitNo));
	pYes->setPosition(ccp(127, 54));
	pNo->setPosition(ccp(301, 54));
	CCMenu* pMenu = CCMenu::create(pYes, pNo, NULL);
	pMenu->setPosition(ccp(0,0));
	pBoard->addChild(pMenu);

	groupIndex=DiaExitGroup;
	menuButIndex[groupIndex] = 0;
	//加载手
	CCSprite* handelSprite = createHandelCreator();
	handelSprite->setPosition(ccp(127,34));
	pBoard->addChild(handelSprite,10,1001);
	handelSprite->setVisible(true);

#endif
}

void ccbLoginMenu::ExitYes( cocos2d::CCObject* sender )
{
	CCLog("exit Game");
#ifdef Plat_Telecom
	PayService::exitGame();
#endif
	AppDelegate::SaveAll();
	exit(0);
}

void ccbLoginMenu::ExitNo( cocos2d::CCObject* sender )
{
	m_bExit = false;
	groupIndex = MainGroup;
	removeChildByTag(65);
	removeChildByTag(1488);
}

void ccbLoginMenu::ExitReady()
{
	if ( m_bGetPrize )
	{
		((ccbLoginEDLayer*)m_pLED)->OnBack();
	}
	else if ( m_bEnter )
	{
		OnBack();
	}
	else
	{
		OnExitGame();
	}
}

void ccbLoginMenu::NetError()
{
	if ( m_pBoard->getChildByTag(410) )
	{
		m_pBoard->getChildByTag(410)->stopAllActions();
		m_pBoard->removeChildByTag(410);
	}
	CCSprite* pSprite = CCSprite::create("tu7/wangluo.png");
	pSprite->setPosition(ccp(50, 0));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::TipDisappear)), NULL));
	m_pBoard->addChild(pSprite, 5, 410);
}

void ccbLoginMenu::TipDisappear()
{
	if ( m_pBoard->getChildByTag(410) )
	{
		m_pBoard->getChildByTag(410)->stopAllActions();
		m_pBoard->removeChildByTag(410);
	}
}

void ccbLoginMenu::ExitProtecter( float _t )
{
	m_bExit = false;
}

void ccbLoginMenu::ShakeName()
{
	if ( getChildByTag(3438) )
		getChildByTag(3438)->removeFromParentAndCleanup(true);
	CCSprite* pSprite = CCSprite::create("tu12/zi6.png");
#ifdef Plat_Unicom
	pSprite->setPosition(ccp(250,370));
#else
	pSprite->setPosition(ccp(261,368));
#endif
	addChild(pSprite, 1, 3438);

	CCAnimate* pAction1 = CCAnimate::create(common::CreateAnimationFrame("tu12/zi", 6, CCRect(0,0,432,143)));
	float time = ((float)(rand()%30 + 20))/10.0f;
	CCDelayTime* pAction2 = CCDelayTime::create(time);
	CCCallFunc* pAction3 = CCCallFunc::create(this, callfunc_selector(ccbLoginMenu::ShakeName));
	pSprite->runAction(CCSequence::create(pAction1, pAction2, pAction3, NULL));
}
