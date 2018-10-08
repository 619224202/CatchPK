#include "ccb.h"
#include "LevelMenu.h"
#include "ShopMenu.h"
#include "MainMenu.h"
#include "baseRes.h"
#include "AppDelegate.h"
#include "LoginMenu.h"
#include "FirstLogin.h"
#include "Common.h"
#include "CSArmatureDataManager.h"
#include "PayService.h"
#include "ShopMenu.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define NeedTime	100
#define TAG_LIGHT	268
#define TAG_BGARRAY	280
#define SIZE_LIGHT	329, 273

#define d_fLevelNameXFix		10.0f
#define d_fLevelNameYFix		134.0f
#define d_fLevelDifficultX		65.0f
#define d_fLevelNameDifficultX	-30.0f

#define AnchonPoint 0.69f, 0.50f
#define nodeLocation 170, 152

static int s_BossLocateX[] =
{
	420, 415, 410, 400, 317, 400,
};

static float s_BossScale[] =
{
	1.8f, 1.2f, 1.55f, 1.5f, 1.0f, 1.6f,
};

ccbLayer* ccbLayer::s_pccbLayer = NULL;

ccbLayer::ccbLayer()
	: m_AnimationManager(NULL)
{
	s_pccbLayer = this;
	m_iGuideIndex = 0;
	m_iPress	= 0;
	m_iToward	= 0;
	m_iLevel	= 0;
	m_iStartTime= 0;
	m_iTime		= 0;
	m_iShowMode = 0;
	m_iBossIndex= 0;
	m_bShare	= false;
	m_bShareWait= false;
	m_bCanChangeMode = false;
	m_ccSize	= CCDirector::sharedDirector()->getWinSize();
	m_iLocation	= 800/2.0f;
	m_pNodeFirst= NULL;
	m_pScene	= NULL;
	m_pShareMain= NULL;
	m_bButton	= false;
	m_bEnd		= false;
	m_bShop		= false;
	m_bReturn	= false;
	m_bLevel	= true;
	m_ccPoint	= CCPoint(800/2.0, 0);
	m_ccPointDown = CCPoint(800/2.0, 0);
	schedule(schedule_selector(ccbLayer::FixLocation), 0.01f);

	m_pFSprite		= NULL;
	m_pSSprite		= NULL;
	m_pName			= NULL;
	m_group		= 0;
	m_bigIndex	= 0;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	this->setKeypadEnabled(true);
//#endif
}

ccbLayer::~ccbLayer()
{
	s_pccbLayer = NULL;
	//SAFEDELETEARRAY(m_pFSprite);
	//SAFEDELETEARRAY(m_pSSprite);
	//SAFEDELETEARRAY(m_pName);
#ifdef GameTypeA
	for ( int i = 0; i < 6; i++ )
	{
		m_pBoss[i]->removeFromParentAndCleanup(true);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_pBoss2Head[i]->removeFromParentAndCleanup(true);
	}
#endif
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	CMainMenu::ReleaseArmatureData();
}

void ccbLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	m_pNode = pNode;
	pNode->getChildByTag(201)->setZOrder(5);
	pNode->getChildByTag(202)->setZOrder(5);
	m_pNodeFirst	= pNode->getChildByTag(111);
	m_pNodeLight	= pNode->getChildByTag(8);
	m_pNodeLight->setZOrder(5);

	CCParticleSystem* pParticle = CCParticleSystemQuad::create("MainMenuParticle.plist");
	pParticle->setPosition( ccp(400, d_fParticleY) );
	addChild(pParticle, 0, 100);

	CCNode*	pTile = pNode->getChildByTag(4);
	CCSprite* pSpriteName = CCSprite::create("Animation/levelchose/1.png");
#ifndef GameTypeC
	CCAnimate* pNameAction = CCAnimate::create( common::CreateAnimationFrame("Animation/levelchose/", 7, CCRect(0,0,283,59)) );
	pSpriteName->runAction( CCRepeatForever::create(CCSequence::create(pNameAction, CCDelayTime::create(2.0f), NULL)) );
#endif
	pSpriteName->setPosition( ccp(400+d_fLevelNameXFix, d_fLevelNameYFix) );
	pTile->addChild(pSpriteName, 0, 6);

	CCSprite* pSpriteDown = CCSprite::create("tu/di.png");
	pSpriteDown->setPosition(ccp(400, d_fLevelNameYFix));
	pTile->addChild(pSpriteDown, 0, 1);

	m_pFSprite	= new CCSprite[d_iLevelNum+1];
	m_pSSprite	= new CCSprite[d_iLevelNum+1];
	m_pName		= new CCSprite[d_iLevelNum+1];
	CCLog("AppDelegate::s_LevelOpen[0]=%d",AppDelegate::s_LevelOpen[0]);
	if(!AppDelegate::s_LevelOpen[0]){
		AppDelegate::s_LevelOpen[0] = 1;
	}
	for ( int i = 0; i < d_iLevelNum; i++ )
	{
		char levelname[255];
		sprintf(levelname, "%d.png", i+1);
		if ( AppDelegate::s_LevelOpen[i*6] )//i < 2额外的关卡限制
		{
			string str = "tu/a";
			str += levelname;
			m_pSSprite[i].initWithFile(str.c_str());
			m_pFSprite[i].initWithFile("tu/kuanggai.png");
			m_pFSprite[i].setAnchorPoint(CCPoint(AnchonPoint));

			CCSprite* p = CCSprite::create(str.c_str());
			p->setVisible(false);
			p->setPosition(ccp(0,0));
			m_pNodeFirst->addChild(p, 0, i);
			CCSprite* p1 = CCSprite::create("tu/kuanggai.png");
			p1->setAnchorPoint(CCPoint(AnchonPoint));
			p1->setPosition(CCPoint(nodeLocation));
			p->addChild(p1);
			
			CCSprite* pItem = CCSprite::create( "tu/tiaozhan2.png");
			pItem->setPosition(ccp(121,183));
			m_pFSprite[i].addChild(pItem, 0, 103);

			int star = 0;
			for (int j = 0; j < 5; j++)
			{
				star += AppDelegate::s_LevelStar[i*6+j];
			}
			int t = 0;
			if ( star < 10 )
				t = 1;
			else if ( star < 100 )
				t = 2;
			common::ShowNumber(&m_pFSprite[i], star, 12, 15, 105, 141, "tu/suzi.png", 100);
			CCSprite* pSprite4 = CCSprite::create("tu/heng.png");
			pSprite4->setPosition(ccp(105+12*t, 141));
			pSprite4->setAnchorPoint(ccp(0,0));
			m_pFSprite[i].addChild(pSprite4, 1, 102);
			t++;
			common::ShowNumber(&m_pFSprite[i], 15, 12, 15, 105+12*t, 141, "tu/suzi.png", 101);
		}
		else
		{
			string str = "tu/b";
			str += levelname;
			m_pSSprite[i].initWithFile(str.c_str());
			m_pFSprite[i].initWithFile("tu/suo.png");
			m_pFSprite[i].setAnchorPoint(CCPoint(AnchonPoint));

			CCSprite* p = CCSprite::create(str.c_str());
			p->setVisible(false);
			p->setPosition(ccp(0,0));
			m_pNodeFirst->addChild(p, 0, i);
			CCSprite* p1 = CCSprite::create("tu/suo.png");
			p1->setAnchorPoint(CCPoint(AnchonPoint));
			p1->setPosition(CCPoint(nodeLocation));
			p->addChild(p1);
		}
#ifndef GameTypeC
			char bj[255];
			sprintf(bj, "tu/bj%d.jpg", i+1);
			CCSprite* pBG = CCSprite::create(bj);
			pBG->setAnchorPoint(ccp(0.0f, 0.0f));
			pBG->setVisible(false);
			addChild(pBG, -1, TAG_BGARRAY+i);
#endif
		m_pFSprite[i].setPosition(ccp(-400,-400));
		m_pSSprite[i].setPosition(ccp(-400,-400));
		SetPosition(&m_pFSprite[i], m_iLocation + 400*(i-m_iLevel) );
		SetPosition(&m_pSSprite[i], m_iLocation + 400*(i-m_iLevel) );
		addChild(&m_pSSprite[i],1);
		addChild(&m_pFSprite[i],3);
	}
#ifdef GameTypeC
	for ( int i = 0; i < 2; i++ )
#else
	for ( int i = 0; i < 6; i++ )
#endif
	{
		char name[100];
		sprintf(name,"LevelName/level%d.png",i+1);
		m_pName[i].initWithFile(name);
		m_pName[i].setPosition(ccp(400+d_fLevelNameXFix, d_fLevelNameYFix));
		m_pName[i].setVisible(false);
		pTile->addChild(&m_pName[i],2);
	}
	m_pNodeFirst->getChildByTag(0)->setVisible(true);
#ifndef GameTypeC
	getChildByTag(TAG_BGARRAY)->setVisible(true);
#endif

	CCSprite* p = NULL;
	CCSprite* p1 = NULL;
	if ( AppDelegate::s_LevelOpen[30] && AppDelegate::s_HeroType == 1 )
	{
		m_pSSprite[d_iLevelNum].initWithFile("tu/a6.png");//问号图片
		m_pFSprite[d_iLevelNum].initWithFile("tu/taijilevel.png");
		p = CCSprite::create("tu/a6.png");
		p1 = CCSprite::create("tu/kuanggai.png");
	}
	else
	{
		m_pSSprite[d_iLevelNum].initWithFile("tu/b6.png");//问号图片
		m_pFSprite[d_iLevelNum].initWithFile("tu/zhuanshu.png");
		p = CCSprite::create("tu/b6.png");
		p1 = CCSprite::create("tu/zhuanshu.png");
	}
	m_pSSprite[d_iLevelNum].setPosition(ccp(-400,-400));
	m_pFSprite[d_iLevelNum].setPosition(ccp(-400,-400));
	m_pFSprite[d_iLevelNum].setAnchorPoint(CCPoint(AnchonPoint));
	SetPosition(&m_pFSprite[d_iLevelNum], m_iLocation + 400*(d_iLevelNum-m_iLevel) );
	SetPosition(&m_pSSprite[d_iLevelNum], m_iLocation + 400*(d_iLevelNum-m_iLevel) );
	addChild(&m_pSSprite[d_iLevelNum],1);
	addChild(&m_pFSprite[d_iLevelNum],3);			
	p->setVisible(false);
	p->setPosition(ccp(0,0));
	m_pNodeFirst->addChild(p, 0, 5);
	p1->setAnchorPoint(CCPoint(AnchonPoint));
	p1->setPosition(CCPoint(nodeLocation));
	p->addChild(p1);

#ifndef GameTypeC
	m_pNameChange = CCSprite::create("LevelName/mo1.png");
	common::RunNewAnimation(m_pNameChange, "LevelName/mo", 2, CCRect(0,0,159,32), true);
	m_pNameChange->setPosition(ccp(400+d_fLevelNameXFix, d_fLevelNameYFix));
	pTile->addChild(m_pNameChange,2);
	m_pNameChange->setVisible(false);

	m_pNameEffect = common::CreateAnimation("Animation/levelchose/dian/1.png", ccp(800/2+d_fLevelNameXFix, d_fLevelNameYFix),
		ccp(0.5f, 0.5f), "Animation/levelchose/dian/", 8, CCRect(0,0,239,114), true);
	pTile->addChild(m_pNameEffect,2);
	m_pNameEffect->setVisible(false);
#endif

	m_pName[0].setVisible(true);

	{
		m_pHideLevel = CCSprite::create("tu/bi.png");
		m_pHideLevel->setPosition(ccp(158,242));
		addChild(m_pHideLevel, 4);
		m_pHideLevel->setVisible(false);

		CCMenuItem* pItem = CCMenuItemImage::create( "tu/tiaozhan.png", "tu/tiaozhan2.png", this, menu_selector(ccbLayer::HideLevel));
		pItem->setPosition(ccp(0,0));
		CCMenu* pMenu = CCMenu::create(pItem, NULL);
		pMenu->setPosition(ccp(120,157));
		m_pHideLevel->addChild(pMenu);

		CCSprite* pSprite5 = CCSprite::create("Animation/hidelevel/1.png");
		pSprite5->setAnchorPoint(ccp(0,0));
		pSprite5->setPosition(ccp(-1,20));
		CCAnimate* pAction = CCAnimate::create(common::CreateAnimationFrame("Animation/hidelevel/", 7, CCRect(0,0,216,230)));
		pSprite5->runAction( CCRepeatForever::create(CCSequence::create(pAction, CCDelayTime::create(2.0f), NULL)));
		m_pHideLevel->addChild(pSprite5,5);

		HideShow();
	}

#ifdef GameTypeA
	{
		for ( int i = 0; i < 6; i++ )
		{
			char name[100];
			sprintf(name,"LevelName/boss%d.png",i+1);
			m_pNameBoss[i] = CCSprite::create(name);
			m_pNameBoss[i]->setPosition(ccp(400+d_fLevelNameXFix, d_fLevelNameYFix));
			m_pNameBoss[i]->setVisible(false);
			pTile->addChild(m_pNameBoss[i],2);

		}
		cs::ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss1dz1.plist", "Monster/jsboss1dz1.png");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("5", "", g_cMonsterPath[5].c_str(), g_cMonsterPlist[5].c_str(),g_cMonsterXML[5].c_str());

		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Boss2_1", "", "Monster/jsboss2_1.png", "Monster/jsboss2_1.plist", "Monster/jsboss2_1.json");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Boss2_2", "", "Monster/jsboss2_2.png", "Monster/jsboss2_2.plist", "Monster/jsboss2_2.json");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Boss2_3", "", "Monster/jsboss2_3.png", "Monster/jsboss2_3.plist", "Monster/jsboss2_3.json");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Boss2", "", g_cMonsterPath[8].c_str(), g_cMonsterPlist[8].c_str(),g_cMonsterXML[8].c_str());

		cs::ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss3_2.plist", "Monster/jsboss3_2.png");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss3_3.plist", "Monster/jsboss3_3.png");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("12", "", g_cMonsterPath[12].c_str(), g_cMonsterPlist[12].c_str(),g_cMonsterXML[12].c_str());

		cs::ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss4_2.plist", "Monster/jsboss4_2.png");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss4_3.plist", "Monster/jsboss4_3.png");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("13", "", g_cMonsterPath[13].c_str(), g_cMonsterPlist[13].c_str(),g_cMonsterXML[13].c_str());

		cs::ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss5_2.plist", "Monster/jsboss5_2.png");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("14", "", g_cMonsterPath[14].c_str(), g_cMonsterPlist[14].c_str(),g_cMonsterXML[14].c_str());

		cs::ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss6_2.plist", "Monster/jsboss6_2.png");
		cs::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("17", "", g_cMonsterPath[17].c_str(), g_cMonsterPlist[17].c_str(),g_cMonsterXML[17].c_str());

		int bossindex[6] = {5, 8, 12, 13, 14, 17};
		for ( int i = 0; i < 6; i++ )
		{
			m_pBoss[i] = cs::Armature::create(g_sMonsterName[bossindex[i]].c_str());
			m_pBoss[i]->setVisible(false);
			m_pBoss[i]->setPosition(ccp(s_BossLocateX[i], 46));
			m_pBoss[i]->setScale(s_BossScale[i]);
			addChild(m_pBoss[i], 2);

			char buffer[255];
			sprintf(buffer, "LevelName/close%d.png", i+1);
			m_pBossClose[i] = CCSprite::create(buffer);
			m_pBossClose[i]->setPosition(ccp(400, 50));
			m_pBossClose[i]->setVisible(false);
			addChild(m_pBossClose[i], 6);

			sprintf(buffer, "LevelName/info%d.png", i+1);
			m_pBossInfo[i] = CCSprite::create(buffer);
			m_pBossInfo[i]->setPosition(ccp(400, 360));
			m_pBossInfo[i]->setVisible(false);
			addChild(m_pBossInfo[i], 6);

			if ( i != 1 )
				m_pBoss[i]->getAnimation()->playByIndex(0, 0, 30);
			else
			{
				m_pBoss2Head[2] = cs::Armature::create("jsboss2_hou");
				m_pBoss2Head[2]->setPosition(445, 128);
				m_pBoss2Head[2]->getAnimation()->playByIndex(0, 0, 60);
				m_pBoss2Head[2]->setScale(s_BossScale[i]);
				m_pBoss2Head[2]->setVisible(false);
				addChild(m_pBoss2Head[2], 2);

				m_pBoss2Head[1] = cs::Armature::create("jsboss2_zhu");
				m_pBoss2Head[1]->setPosition(417, 124);
				m_pBoss2Head[1]->getAnimation()->playByIndex(0, 0, 60);
				m_pBoss2Head[1]->setScale(s_BossScale[i]);
				m_pBoss2Head[1]->setVisible(false);
				addChild(m_pBoss2Head[1], 2);

				m_pBoss2Head[0] = cs::Armature::create("jsboss2_du");
				m_pBoss2Head[0]->setPosition(369, 120);
				m_pBoss2Head[0]->getAnimation()->playByIndex(0, 0, 60);
				m_pBoss2Head[0]->setScale(s_BossScale[i]);
				m_pBoss2Head[0]->setVisible(false);
				addChild(m_pBoss2Head[0], 2);
			}

		}

		CCMenuItem* pItemLeft = CCMenuItemImage::create("tu/fangxiang1.png", "tu/fangxiang12.png", this, menu_selector(ccbLayer::Left));
		CCMenuItem* pItemRight = CCMenuItemImage::create("tu/fangxiang2.png", "tu/fangxiang22.png", this, menu_selector(ccbLayer::Right));
		pItemLeft->setPosition(ccp(150, 240));
		pItemRight->setPosition(ccp(650, 240));
		m_pMenuChoseBoss = CCMenu::create(pItemLeft, pItemRight, NULL);
		m_pMenuChoseBoss->setPosition(ccp(0,0));
		addChild(m_pMenuChoseBoss, 20);
		m_pMenuChoseBoss->setVisible(false);

		CCMenuItemImage* pItemEnter = CCMenuItemImage::create("tu/tiaozhan21.png", "tu/tiaozhan22.png", this, menu_selector(ccbLayer::BossEnter));
		pItemEnter->setPosition(ccp(0,0));
		CCMenu*	pMenuEnter = CCMenu::create(pItemEnter, NULL);
		pMenuEnter->setPosition(ccp(0,0));
		pNode->getChildByTag(1)->addChild(pMenuEnter, 0, 1);

		CCSprite* pSpriteEnter = CCSprite::create("tu/weikaiqi.png");
		pSpriteEnter->setPosition(ccp(0,0));
		pNode->getChildByTag(1)->addChild(pSpriteEnter, 0, 2);
	}
#endif

#ifndef GameTypeC
#ifdef Share_Open
	m_iShare = 0;
	for ( int i = 0; i < 6; i++ )
	{
		if ( AppDelegate::s_LevelOpen[i*6+5] == 2 )
			m_iShare ++;
		else
			break;
	}
	if ( AppDelegate::m_Share < 6 )
	{
		CCMenuItemImage* pItem = CCMenuItemImage::create("share/fenxiang.png", "share/fenxiang.png", this, menu_selector(ccbLayer::ShareShow));
		pItem->setPosition(ccp(742, 122));
		m_pShareMain = CCMenu::create(pItem, NULL);
		m_pShareMain->setPosition(ccp(0,0));
		pTile->addChild(m_pShareMain, 10);
	}
#endif
#endif
	/*CCSprite* handelSprite = ccbShopMenu::createHandelCreator();
	handelSprite->setVisible(false);
	pNode->addChild(handelSprite,50,1001);
	handelSprite->setPosition(ccp(413,245));*/
}

SEL_MenuHandler ccbLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On8", ccbLayer::OnReturn );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On9", ccbLayer::OnShop );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbLayer::ChangeShowModeStart );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbLayer::ChangeShowModeStart );
	return NULL;    
}

SEL_CCControlHandler ccbLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}

bool ccbLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbLayer::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
}

void ccbLayer::OnReturn(cocos2d::CCObject *pSender)
{
	if ( m_bShare )
		return;
	if ( m_bReturn || AppDelegate::s_FirstLogin == 4 || CMainMenu::m_bOnSell )
		return;
	m_bCanChangeMode = false;
	switch ( CMainMenu::GetStatus() )
	{
	case 0:
		{
			m_bReturn = true;
			CCScene* pScene = CFirstLogin::CreateScene(false);
			//ccbLoginMenu::LoginScene(pScene);
			AppDelegate::ChangeScene( pScene );
		}
		break;
	case 2:
		m_bReturn = true;
		m_bLevel = false;
		SetTouch();
		//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
		((ccbLevelMenu*)CMainMenu::GetMenuLevel())->Return();
		break;
	}
}

void ccbLayer::setHandelPos(CCPoint point){
	getChildByTag(1001)->setPosition(point);
}

void ccbLayer::OnShop( cocos2d::CCObject *pSender )
{	
	if ( m_bShare || m_iShowMode )
		return;
	if ( m_iStartTime < NeedTime || m_bShop || AppDelegate::s_FirstLogin == 4 || CMainMenu::m_bOnSell )
		return;
	m_bShop = true;
	
	CMainMenu::m_bShop = true;
	switch ( CMainMenu::GetStatus() )
	{
	case 0:
		SetDistouch();
		//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		if ( getChildByTag(TAG_LIGHT) )
		{
			getChildByTag(TAG_LIGHT)->stopAllActions();
			removeChildByTag(TAG_LIGHT);
		}
		AllMoveWithBottom();
		for ( int i = 0; i < d_iLevelNum + 1; i++ )
		{
			m_pFSprite[i].setVisible(false);
			m_pSSprite[i].setVisible(false);
		}
		scheduleOnce(schedule_selector(ccbLayer::GoToShop), 0.5f);
		break;
	case 2:
		SetDistouch();
		//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		((ccbLevelMenu*)CMainMenu::GetMenuLevel())->ShopReturn();
		AllMove();
		scheduleOnce(schedule_selector(ccbLayer::GoToShop), 0.6f);
		break;
	}	
}

void ccbLayer::GoToShop( float _dt )
{
	if ( !CMainMenu::GetMenuShop() )
	{
		CCNode* pNode = CMainMenu::CreateMenuShop();
		CCDirector::sharedDirector()->getRunningScene()->addChild(pNode, 1);
	}
	((ccbShopMenu*)CMainMenu::GetMenuShop())->Appear();
}

void ccbLayer::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( m_bShare )
		return;
	if ( m_iShowMode == 0 )
	{
		if ( m_bEnd || m_bLevel || CMainMenu::m_pScene->getChildByTag(4) || m_iLocation != 400 || m_iShowMode )
			return;
		if ( m_iPress || m_iStartTime < NeedTime || -1 == CMainMenu::GetStatus() )
			return;

		//m_pHideLevel->setVisible(false);
		//m_pSSprite[m_iLevel].setZOrder(1);
		//m_pFSprite[m_iLevel].setZOrder(3);
		m_ccPoint = ((CCTouch*)*(pTouches->begin()))->getLocation();
		m_iPress = (*pTouches->begin())->m_uID;
		//m_ccPoint = touch->getLocation();
		m_ccPointDown = m_ccPoint;
		return;
	}
	else
	{
		CCPoint point = ((CCTouch*)*(pTouches->begin()))->getLocation();
		if ( fabs(point.x - 400.0f*Scale_X) < 150.0f*Scale_X && fabs(point.y - 240.0f*Scale_Y) < 150.0f*Scale_Y )
			BossEnter();
	}
}

void ccbLayer::ccTouchesMoved( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( m_bShare )
		return;
	if ( m_bLevel || m_iStartTime < NeedTime || m_bEnd )
		return;
	if ( AppDelegate::s_FirstLogin == 4 )
	{
		CCPoint point1 = ((CCTouch*)*(pTouches->begin()))->getLocation();
		m_ccPoint = point1;
		return;
	}
	int i = (*pTouches->begin())->m_uID;
	if ( m_iPress != i )
		return;

	if ( getChildByTag(TAG_LIGHT) )
	{
		getChildByTag(TAG_LIGHT)->stopAllActions();
		removeChildByTag(TAG_LIGHT);
	}

	m_pHideLevel->setVisible(false);
	//m_pSSprite[m_iLevel].setZOrder(1);
	//m_pFSprite[m_iLevel].setZOrder(3);
	CCPoint point1 = ((CCTouch*)*(pTouches->begin()))->getLocation();
	//CCPoint point1 = touch->getLocation();
	CCPoint point2 = m_pFSprite[m_iLevel].getPosition();

	m_iLocation = point2.x+point1.x-m_ccPoint.x;
	m_ccPoint = point1;
	
	float scale = fabs( (800/2.0f-m_iLocation)/(800/2.0f+100) );
	int color = 255*(1.0 - scale*2);
	if ( color < 10 )
		color = 0;
	((CCSprite*)m_pNodeLight)->setOpacity(color);

	for ( int i = 0; i < d_iLevelNum+1; i++ )
	{
		SetPosition(&m_pFSprite[i], m_iLocation + 800/2.0*(i-m_iLevel) );
		SetPosition(&m_pSSprite[i], m_iLocation + 800/2.0*(i-m_iLevel) );
	}
#ifdef GameTypeC
	for ( int i = 0; i < 2; i++ )
#else
	for ( int i = 0; i < 6; i++ )
#endif
		m_pName[i].setVisible(false);
#ifdef GameTypeA
	for ( int i = 0; i < 6; i++ )
		m_pNameBoss[i]->setVisible(false);
#endif
	ObjectShow();
}

void ccbLayer::ccTouchesEnded( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( m_bShare )
		return;
	if ( m_bEnd || m_bLevel )
		return;

	CCSetIterator iter = pTouches->begin();
	int i = -1;
	for (; iter != pTouches->end(); iter++/*, digit++*/)
	{
		i = (*iter)->m_uID;
		if ( i == m_iPress )
			break;
	}
	if ( m_iPress != i )
		return;
	m_iPress = 0;
	
	if ( abs(m_ccPoint.x - m_ccPointDown.x) < 10 
		&& abs(m_ccPoint.y - m_ccPointDown.y) < 10 
		&& m_iStartTime >= NeedTime
		&& abs(m_ccPointDown.x - m_ccSize.width/2) < 160 
		&& abs(m_ccPointDown.y - m_ccSize.height/2) < 140 
		&& CMainMenu::GetStatus() == 0 )
	{
		
		if ( !AppDelegate::s_LevelOpen[m_iLevel*6] )
			return;
//#ifdef GameTypeA
//		if ( m_iLevel > LevelLimit-1 )
//			return;
//#else
//		if ( m_iLevel > 1 )
//			return;
//#endif
		
		if ( m_iLevel == 5 )
		{
			if ( AppDelegate::s_HeroType == 1 )
				AppDelegate::ChangeScene( CMainMenu::StartGame(30, 0) );
			return;
		}
		
		if ( AppDelegate::s_FirstLogin == 4 )
		{
			GuideFingerClear();
			GuideFingerCreate(138, 295);
		}

		m_bLevel = true;
		CMainMenu::SetStatus(2);
		//BottomMove();
		((ccbLevelMenu*)CMainMenu::GetMenuLevel())->Appear();
		BottomMove();
		//scheduleOnce(schedule_selector(ccbDifficultMenu::ChoseLevelCallBack), 0.5f);

		//if ( !CMainMenu::GetMenuDifficult() )
		//{
		//	CCNode* pNode = CMainMenu::CreateMenuDifficult();
		//	CCDirector::sharedDirector()->getRunningScene()->addChild(pNode, 0);
		//}
		SetDistouch();
		////CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		//((ccbDifficultMenu*)CMainMenu::GetMenuDifficult())->Appear();
		//CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230));
		//pCCLayerColor->setPosition(ccp(0,0));
		//addChild(pCCLayerColor, 5, 10);
		//CMainMenu::SetStatus(1);
	}
	ObjectShow();
}

void ccbLayer::keyBackClicked()
{
	OnReturn(this);
}

void ccbLayer::keyEnterClicked(){
	//CCLog("按下了确定键");
	if(m_iShowMode==0){
		doEnter();
	}else{
		BossEnter();
	}
}

void ccbLayer::doEnter(){
	if ( m_bShare )
		return;
	if ( m_bEnd || m_bLevel )
		return;
	if(CMainMenu::GetStatus() == 0){
		if ( m_bEnd || m_bLevel || CMainMenu::m_pScene->getChildByTag(4) || m_iLocation != 400 || m_iShowMode )
			return;
		if ( m_iPress || m_iStartTime < NeedTime || -1 == CMainMenu::GetStatus() )
			return;
		if ( !AppDelegate::s_LevelOpen[m_iLevel*6] )
			return;
		if ( m_iLevel == 5 )
		{
			if ( AppDelegate::s_HeroType == 1 )
				AppDelegate::ChangeScene( CMainMenu::StartGame(30, 0) );
			return;
		}
		
		if ( AppDelegate::s_FirstLogin == 4 )
		{
			GuideFingerClear();
			GuideFingerCreate(138, 295);
		}
		m_bLevel = true;
		CMainMenu::SetStatus(2);
		//BottomMove();
		((ccbLevelMenu*)CMainMenu::GetMenuLevel())->Appear();
		BottomMove();
		
		SetDistouch();
	}
	ObjectShow();
}

void ccbLayer::keyArrowClicked(int arrow){
	switch(arrow){
	case kTypeLeftArrowClicked:
		if(m_iShowMode==0){
			if(m_iLevel<5){
				doMove(true);
			}
		}else{
			Left(this);
		}
		break;
	case kTypeRightArrowClicked:
		if(m_iShowMode==0){
			if(m_iLevel>0){
				doMove(false);
			}
		}else{
			Right(this);
		}
		break;
	}
}

void ccbLayer::keyNumClicked(int keyNum){
	//CCLog("按下了数字键:%d",keyNum);
	if(keyNum==kTypeNum0){
		keyBackClicked();
	}else if(keyNum==kTypeNum1){
		OnShop(this);
	}else if(keyNum==kTypeNum2){
		ChangeShowModeStart(this);
	}
}

void ccbLayer::doMove(bool moveLeft){
	if ( m_bShare )
		return;
	if ( m_bLevel || m_iStartTime < NeedTime || m_bEnd )
		return;
	if ( AppDelegate::s_FirstLogin == 4 )
	{
		return;
	}
	
	if ( getChildByTag(TAG_LIGHT) )
	{
		getChildByTag(TAG_LIGHT)->stopAllActions();
		removeChildByTag(TAG_LIGHT);
	}

	m_pHideLevel->setVisible(false);
	//m_pSSprite[m_iLevel].setZOrder(1);
	//m_pFSprite[m_iLevel].setZOrder(3);
	
	/*CCLog("MoveBigIcon>>>>>>>>>>>>>111,location=%f",m_iLocation);
	float scale = fabs( (800/2.0f-m_iLocation)/(800/2.0f+100) );
	int color = 255*(1.0 - scale*2);
	if ( color < 10 )
		color = 0;
	((CCSprite*)m_pNodeLight)->setOpacity(color);*/
	CCPoint point2 = m_pFSprite[m_iLevel].getPosition();
	
	m_ccPoint.y = point2.y;
	m_ccPointDown.x = point2.x;
	m_ccPointDown.y = point2.y;

	if(moveLeft){
		m_iLocation = point2.x-51;
		m_ccPoint.x = point2.x-51;
	}else{
		m_iLocation = point2.x+51;
		m_ccPoint.x = point2.x+51;
	}

	for ( int i = 0; i < d_iLevelNum+1; i++ )
	{
		SetPosition(&m_pFSprite[i], m_iLocation + 800/2.0*(i-m_iLevel) );
		SetPosition(&m_pSSprite[i], m_iLocation + 800/2.0*(i-m_iLevel) );
	}
#ifdef GameTypeC
	for ( int i = 0; i < 2; i++ )
#else
	for ( int i = 0; i < 6; i++ )
#endif
		m_pName[i].setVisible(false);
#ifdef GameTypeA
	for ( int i = 0; i < 6; i++ )
		m_pNameBoss[i]->setVisible(false);
#endif
	ObjectShow();
}

void ccbLayer::SetSpriteDisable(bool m_bVisible)
{
	for ( int i = 0; i < d_iLevelNum+1; i++ )
	{
		//if ( abs( i - m_iLevel ) < 2 )
		//{
			m_pFSprite[i].setVisible(m_bVisible);
			m_pSSprite[i].setVisible(m_bVisible);
		//}
	}
	m_bEnd = !m_bVisible;
	m_iPress = m_bVisible - 1;
	//setTouchEnabled(m_bVisible);
}

void ccbLayer::SetPosition( cocos2d::CCNode* _pNode, float _x, bool _bCheck)
{
	if (_x < -300 || _x > 1100 ) 
	{
		_pNode->setVisible(false);
		return;
	}

	_pNode->setVisible(true);
	float scale = fabs( (800/2.0f-_x)/(800/2.0f+100) );
	float yscale = 0.0f;
	if ( 1 > scale*scale )
		yscale = sqrt(1-scale*scale);

	float y = yscale * 480*3/20.0f + 480*7/20.0f;
	_pNode->setPosition( ccp(_x, y) );
	_pNode->setScale(1.0 - scale/2.0);
	int color = 255*(1.0 - scale);
	if ( color < 0 )
		color = 0;
	((CCSprite*)_pNode)->setColor(ccc3(color, color, color));
	if ( _pNode->getChildByTag(100) )
	{
		((CCSprite*)_pNode->getChildByTag(100))->setColor(ccc3(color, color, color));
		((CCSprite*)_pNode->getChildByTag(101))->setColor(ccc3(color, color, color));
		((CCSprite*)_pNode->getChildByTag(102))->setColor(ccc3(color, color, color));
		((CCSprite*)_pNode->getChildByTag(103))->setColor(ccc3(color, color, color));
	}
}

void ccbLayer::FixLocation( float _dt )
{
	if ( m_iShowMode )
		return;
	if ( m_iLocation == 800/2.0f && 0 == CMainMenu::GetStatus() && m_iStartTime >= NeedTime && !m_bShop )
	{
		if ( AppDelegate::s_LevelOpen[m_iLevel*6] )
		{
			m_iTime++;
			if ( m_iTime == 32 && getChildByTag(TAG_LIGHT) )
			{
				getChildByTag(TAG_LIGHT)->stopAllActions();
				removeChildByTag(TAG_LIGHT);
			}
			else if ( m_iTime > rand()%200 + 200 )
			{
				m_iTime = 0;
#ifndef GameTypeC
				CCSprite* pSprite = common::CreateAnimation("tu/xiaoguo/1.png", ccp(m_iLocation-4, 480/2.0f), ccp(0.5f, 0.5f),
					"tu/xiaoguo/", 9, CCRect(0, 0, SIZE_LIGHT), false, false, 0.05f);
				addChild(pSprite, 2, TAG_LIGHT);
#endif
			}
		}
	}

	if ( m_iStartTime < NeedTime )
	{
		m_iStartTime++;
		//CCLog("m_iStartTime=%d",m_iStartTime);
		bool setvisible = false;
		if ( m_iStartTime == NeedTime )
		{
			//m_pFSprite[m_iLevel].setZOrder(6);
			//m_pSSprite[m_iLevel].setZOrder(5);
			//int limit = LevelLimit;
#ifdef GameTypeC
			limit = 2;
#endif
			if ( AppDelegate::s_LevelOpen[m_iLevel*6] && m_iLevel < 5 )
			{
				m_pHideLevel->setVisible(true);
				GuideInit();
			}
			setvisible = true;
		}
		SetSpriteDisable(setvisible);
		if ( m_pNodeFirst )
		{
			m_pNodeFirst->setVisible(!setvisible);
		}
	}

	if ( m_iPress /*|| m_iLocation == 800/2.0 */)
		return;

	if ( m_ccPointDown.x < m_ccPoint.x - 50.0f )
		m_iToward = 1;
	else if ( m_ccPointDown.x > m_ccPoint.x  + 50.0f )
		m_iToward = -1;
	else
		m_iToward = 0;
	m_ccPointDown.x = m_ccPoint.x;

	if ( m_iLevel > 0 && m_iToward > 0 && m_iLocation > 800/2.0 )
	{
		m_iLocation -= 800/2.0;
		m_iLevel--;
	}
	else if ( m_iLevel < d_iLevelNum && m_iToward < 0 && m_iLocation < 800/2.0 )
	{
		m_iLocation += 800/2.0;
		m_iLevel++;
	}

	float speed = 0.0f;
	if ( m_iLocation < 800/2.0 || m_iToward > 0 )
		speed = 8.0f;
	else if ( m_iLocation > 800/2.0 || m_iToward < 0 )
		speed = -8.0f;
	m_iLocation += speed;
	
	float scale = fabs( (800/2.0f-m_iLocation)/(800/2.0f+100) );
	int color = 255*(1.0 - scale*2);
	if ( color < 10 )
		color = 0;
	((CCSprite*)m_pNodeLight)->setOpacity(color);

	for ( int i = 0; i < d_iLevelNum+1; i++ )
	{
		bool bCheck = false;
		if ( i == m_iLevel )
			bCheck = true;
		SetPosition(&m_pFSprite[i], m_iLocation + 800/2.0*(i-m_iLevel), bCheck);
		SetPosition(&m_pSSprite[i], m_iLocation + 800/2.0*(i-m_iLevel), bCheck);
	}
	ObjectShow();
}

void ccbLayer::ObjectShow()
{
	if ( m_iShowMode )
		return;
	//for ( int i = 0; i < 6; i++ )
	//{
	//	m_pSSprite[i].setZOrder(2);
	//	m_pFSprite[i].setZOrder(3);
	//}
	//m_pSSprite[m_iLevel].setZOrder(5);
	//m_pFSprite[m_iLevel].setZOrder(6);
	if ( abs(m_iLocation - 800/2.0) < 10.0 )
	{
		m_iLocation = 800/2.0;
		//int limit = LevelLimit;
#ifdef GameTypeC
		limit = 2;
#endif
		if ( m_iPress == 0 && AppDelegate::s_LevelOpen[m_iLevel*6] && m_iStartTime >= NeedTime )
		{
			m_pFSprite[m_iLevel].setPositionX(m_iLocation);
			m_pSSprite[m_iLevel].setPositionX(m_iLocation);
			if ( m_iLevel < 5 )
				m_pHideLevel->setVisible(true);
			HideShow();
		}
#ifndef GameTypeC
		for ( int i = 0; i < d_iLevelNum; i++ )
		{
			getChildByTag(TAG_BGARRAY+i)->setVisible(false);
		}
		if ( m_iLevel < d_iLevelNum )
			getChildByTag(TAG_BGARRAY+m_iLevel)->setVisible(true);
#endif
	}
	if ( abs ( m_iLocation - 800/2 ) < 20.0f )
	{
#ifndef GameTypeC
		m_pNameChange->setVisible(false);
		m_pNameEffect->setVisible(false);
#endif
		if ( m_iLevel < d_iLevelNum+1 )
			m_pName[m_iLevel].setVisible(true);
	}
	else
	{
		//m_pNodeLight->setVisible(false);
#ifndef GameTypeC
		m_pNameChange->setVisible(true);
		m_pNameEffect->setVisible(true);
#endif
	}
}

void ccbLayer::ShowDifficultName()
{
	//for ( int i = 0; i < d_iLevelNum; i++ )
	//{
	//	m_pName[i].setPositionX(800/2+d_fLevelNameDifficultX);
	//}
	//m_pDifficult[CMainMenu::GetDifficult()].setVisible(true);
}

void ccbLayer::ShowName()
{
	//for ( int i = 0; i < d_iLevelNum; i++ )
	//{
	//	m_pName[i].setPositionX(800/2+d_fLevelNameXFix);
	//}
	//for ( int i = 0; i < 3; i++ )
	//{
	//	m_pDifficult[i].setVisible(false);
	//}
}

void ccbLayer::Back()
{
	removeChildByTag(10);
	if ( 0 == CMainMenu::GetStatus() )
	{
		for ( int i = 0; i < d_iLevelNum+1; i++ )
		{
			bool bCheck = false;
			if ( i == m_iLevel )
				bCheck = true;
			SetPosition(&m_pFSprite[i], m_iLocation + 800/2.0*(i-m_iLevel), bCheck);
			SetPosition(&m_pSSprite[i], m_iLocation + 800/2.0*(i-m_iLevel), bCheck);
		}
		SetTouch();
		//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	}
	ObjectShow();
}

int ccbLayer::GetLevel()
{
	return m_iLevel;
}

void ccbLayer::BottomMove()
{
	m_pHideLevel->setVisible(false);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("dizuoshou", 0.0f);
}

void ccbLayer::BottomBack()
{
	m_pHideLevel->setVisible(false);
	m_bButton	= false;
	m_bShop		= false;
	m_iPress	= 0;
	m_iToward	= 0;
	m_iStartTime= 0;
	//ShowName();
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("dizuoshou hui", 0.0f);
}

void ccbLayer::ButtonMove()
{
	m_pHideLevel->setVisible(false);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("daditushou", 0.0f);
}

void ccbLayer::ButtonBack()
{
	m_pHideLevel->setVisible(false);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("daditushou hui", 0.0f);
}

void ccbLayer::AllMove()
{
	m_pHideLevel->setVisible(false);
	m_iStartTime = NeedTime + 5;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("zhuanchang", 0.0f);
}

void ccbLayer::AllBack()
{
	m_pHideLevel->setVisible(false);
	m_bCanChangeMode = false;
	m_bShop		= false;
	m_bReturn	= false;
	SetSpriteDisable(false);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("zhuanchang hui", 0.0f);
	if ( 0 == CMainMenu::GetStatus() )
		m_iStartTime = 0;
}

void ccbLayer::AllMoveWithBottom()
{
	m_pHideLevel->setVisible(false);
	SetSpriteDisable(false);
	m_iStartTime = NeedTime + 5;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("zhuanchang dizuo", 0.0f);
}

void ccbLayer::AllBackWithBottom()
{
	m_pHideLevel->setVisible(false);
#ifndef GameTypeC
	AppDelegate::AudioPlayBgm("MS/Music/mainmenu/LevelChose.mp3");
#endif
	m_bCanChangeMode = false;
	m_bButton	= false;
	m_bShop		= false;
	m_bReturn	= false;
	m_bLevel	= false;
	m_iLocation = 400;
	m_iPress	= 0;
	m_iToward	= 0;
	m_iStartTime= 0;
#ifndef GameTypeC
	if ( getChildByTag(TAG_BGARRAY+m_iLevel) )
		getChildByTag(TAG_BGARRAY+m_iLevel)->setVisible(true);
#endif
	m_pNodeFirst->getChildByTag(m_iLevel)->setVisible(true);
	CMainMenu::SetDifficult(0);
	ObjectShow();
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
}

void ccbLayer::SetTouch(CCNode* sender)
{
	setTouchEnabled(true);
	setKeypadEnabled(true);
}

void ccbLayer::SetDistouch(CCNode* sender)
{
	removeChildByTag(TAG_LIGHT);
	setTouchEnabled(false);
	setKeypadEnabled(false);
}

cocos2d::SEL_CallFuncN ccbLayer::onResolveCCBCCCallFuncSelector( CCObject * pTarget, const char* pSelectorName )
{
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "Start", ccbLayer::SetTouch);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "Over", ccbLayer::SetDistouch);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "Effect", ccbLayer::Effect);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "On5", ccbLayer::ChangeShowMode);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "On6", ccbLayer::ChangeShowMode);

	return NULL;
}

void ccbLayer::Effect( CCNode* sender /*= NULL*/ )
{
	//AppDelegate::AudioPlayEffect("MS/Sound/EfCCBNew.mp3");
}

void ccbLayer::HideLevel( cocos2d::CCObject* sender )
{
	if ( m_bShare )
		return;
	if ( m_iAllStar < 15 )
	{
		if ( getChildByTag(101) )
		{
			getChildByTag(101)->stopAllActions();
			removeChildByTag(101);
		}
		CCSprite* pSprite = CCSprite::create("tu/xing.png");
		pSprite->setPosition(ccp(400, 240));
		CCFadeOut* pAction = CCFadeOut::create(2.0f);
		pSprite->runAction(CCSequence::create(pAction,
			CCCallFunc::create(this, callfunc_selector(ccbLayer::disappear)), NULL));
		addChild(pSprite, 10, 101);
	}
	else
	{
#ifdef ChallengeLevel
		AppDelegate::ChangeScene( CMainMenu::StartGame(49 + m_iLevel, CMainMenu::GetDifficult()) );
#else
		if ( getChildByTag(101) )
		{
			getChildByTag(101)->stopAllActions();
			removeChildByTag(101);
		}
		CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
		pSprite->setPosition(ccp(400, 240));
		CCFadeOut* pAction = CCFadeOut::create(2.0f);
		pSprite->runAction(CCSequence::create(pAction,
			CCCallFunc::create(this, callfunc_selector(ccbLayer::disappear)), NULL));
		addChild(pSprite, 10, 101);
#endif
	}
}

void ccbLayer::HideShow()
{
	m_iAllStar = 0;
	for (int i = 0; i < 5; i++)
	{
		m_iAllStar += AppDelegate::s_LevelStar[m_iLevel*6+i];
	}
	//int t = 0;
	//if ( m_iAllStar < 10 )
	//	t = 1;
	//else if ( m_iAllStar < 100 )
	//	t = 2;
	//common::ShowNumber(m_pHideLevel, m_iAllStar, 12, 15, 43, 19, "tu/suzi.png", 100);
	//m_pHideLevel->getChildByTag(5)->setPositionX(43+12*t);
	//t++;
	//common::ShowNumber(m_pHideLevel, 18, 12, 15, 43+12*t, 19, "tu/suzi.png", 101);
}

void ccbLayer::disappear()
{
	getChildByTag(101)->stopAllActions();
	removeChildByTag(101);
}

void ccbLayer::ChangeShowMode(CCNode* _pNode)
{
	m_bCanChangeMode = false;
}

void ccbLayer::ShowBoss( int _index, bool _show )
{
	if ( m_bShare )
		return;
	m_pNameBoss[_index]->setVisible(_show);
	m_pBoss[_index]->setVisible(_show);
	if ( _index == 1 )
	{
		for ( int i = 0; i < 3; i++ )
		{
			m_pBoss2Head[i]->setVisible(_show);
		}
	}
	if ( _show )
	{
		m_pNameChange->setVisible(false);
		m_pNameEffect->setVisible(false);
#ifdef Boss_Level_Open
		if ( 1 == 1 )
#else
		if ( AppDelegate::s_LevelOpen[_index*6+5] == 2 )
#endif
		{
			m_pNode->getChildByTag(1)->getChildByTag(1)->setVisible(true);
			m_pNode->getChildByTag(1)->getChildByTag(2)->setVisible(false);
		}
		else
		{
			if ( _index == 1 )
			{
				for ( int i = 0; i < 3; i++ )
				{
					m_pBoss2Head[i]->setColor(ccc3(25,25,25));
				}
			}
			m_pBoss[_index]->setColor(ccc3(25,25,25));
			m_pNode->getChildByTag(1)->getChildByTag(1)->setVisible(false);
			m_pNode->getChildByTag(1)->getChildByTag(2)->setVisible(true);
			m_pBossClose[_index]->setVisible(true);
		}
		m_pBossInfo[_index]->setVisible(true);
		for ( int i = 0; i < d_iLevelNum; i++ )
		{
			getChildByTag(TAG_BGARRAY+i)->setVisible(false);
		}
		if ( _index < d_iLevelNum )
			getChildByTag(TAG_BGARRAY+_index)->setVisible(true);
	}
	else
	{
		m_pBossClose[_index]->setVisible(false);
		m_pBossInfo[_index]->setVisible(false);
	}
}

void ccbLayer::ChangeShowModeStart( CCObject *pSender )
{
	if ( m_bShare || m_bShop )
		return;
#ifdef GameTypeA
	if ( m_bCanChangeMode || AppDelegate::s_FirstLogin == 4 || CMainMenu::GetStatus() != 0 )
		return;
	m_iPress = 0;
	m_bCanChangeMode = true;
	SetTouch();
	if ( m_iShowMode )
		m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("dizuoshou copy copy", 0.0f);
	else
		m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("dizuoshou copy", 0.0f);
	m_iShowMode = 1 - m_iShowMode;
	HideAll();
#endif
#ifdef GameTypeB
	if ( getChildByTag(410) )
	{
		getChildByTag(410)->stopAllActions();
		removeChildByTag(410);
	}
	CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	pSprite->setPosition(ccp(400, 240));
	CCFadeOut* pAction = CCFadeOut::create(1.0f);
	pSprite->runAction(CCSequence::create(pAction,
		CCCallFunc::create(this, callfunc_selector(ccbLayer::tipdisappear)), NULL));
	addChild(pSprite, 5, 410);
#endif
}

void ccbLayer::HideAll()
{
	if ( m_iShowMode == 0 )
	{
		for ( int i = 0; i < 6; i++ )
		{
			m_pNameBoss[i]->setVisible(false);
			m_pBoss[i]->setVisible(false);
			m_pBossClose[i]->setVisible(false);
			m_pBossInfo[i]->setVisible(false);
		}
		for ( int i = 0; i < 3; i++ )
		{
			m_pBoss2Head[i]->setVisible(false);
		}
		m_pMenuChoseBoss->setVisible(false);
		setTouchEnabled(true);
		setKeypadEnabled(true);
#ifndef GameTypeC
		for ( int i = 0; i < d_iLevelNum; i++ )
		{
			getChildByTag(TAG_BGARRAY+i)->setVisible(false);
		}
		if ( m_iLevel < d_iLevelNum )
			getChildByTag(TAG_BGARRAY+m_iLevel)->setVisible(true);
#endif
	}
	else
	{
#ifdef GameTypeA
		ShowBoss(m_iBossIndex , true);
		m_pMenuChoseBoss->setVisible(true);

		m_pNodeFirst->setVisible(false);
		for ( int i = 0; i < d_iLevelNum+1; i++ )
		{
			m_pSSprite[i].setVisible(false);
			m_pFSprite[i].setVisible(false);
		}
		for ( int i = 0; i < d_iLevelNum+1; i++ )
		{
			m_pName[i].setVisible(false);
		}
		m_pHideLevel->setVisible(false);
		if ( getChildByTag(TAG_LIGHT) )
		{
			getChildByTag(TAG_LIGHT)->stopAllActions();
			removeChildByTag(TAG_LIGHT);
		}
#endif
	}
}

void ccbLayer::Left( CCObject* pNode )
{
	if ( m_bShare || m_iShowMode == 0 )
		return;
	ShowBoss(m_iBossIndex, false);
	m_iBossIndex --;
	if ( m_iBossIndex < 0 )
		m_iBossIndex = 0;
	ShowBoss(m_iBossIndex, true);
}

void ccbLayer::Right( CCObject* pNode )
{
	if ( m_bShare || m_iShowMode == 0 )
		return;
	ShowBoss(m_iBossIndex, false);
	m_iBossIndex ++;
	if ( m_iBossIndex > 5 )
		m_iBossIndex = 5;
	ShowBoss(m_iBossIndex, true);
}

void ccbLayer::BossEnter( cocos2d::CCObject *pSender /*= NULL*/ )
{
	if ( m_bShare )
		return;
#ifdef Boss_Level_Open
	if ( 1 )
#else
	if ( (AppDelegate::s_LevelOpen[m_iBossIndex*6+5] == 2 && m_iBossIndex != 5) 
		|| (AppDelegate::s_LevelOpen[m_iBossIndex*6] == 2 && m_iBossIndex == 5) )
#endif
	{
		AppDelegate::ChangeScene( CMainMenu::StartGame(55 + m_iBossIndex, CMainMenu::GetDifficult()) );
	}

	//if ( getChildByTag(410) )
	//{
	//	getChildByTag(410)->stopAllActions();
	//	removeChildByTag(410);
	//}
	//CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
	//pSprite->setPosition(ccp(400, 240));
	//CCFadeOut* pAction = CCFadeOut::create(1.0f);
	//pSprite->runAction(CCSequence::create(pAction,
	//	CCCallFunc::create(this, callfunc_selector(ccbLayer::tipdisappear)), NULL));
	//addChild(pSprite, 25, 410);
}

void ccbLayer::tipdisappear()
{
	getChildByTag(410)->stopAllActions();
	removeChildByTag(410);
}

void ccbLayer::GuideInit()
{
	if ( AppDelegate::s_FirstLogin == 4 && m_iGuideIndex == 0 )
	{
		m_iGuideIndex = 1;
		GuideFingerCreate(400, 240);
	}
}

void ccbLayer::GuideFingerCreate( int _x, int _y )
{
	if ( CMainMenu::m_pScene->getChildByTag(493) )
		CMainMenu::m_pScene->getChildByTag(493)->removeFromParentAndCleanup(true);
	CCSprite* pSprite = common::CreateAnimation("guide/shop/shou1.png", ccp(_x*Scale_X,_y*Scale_Y), ccp(0.9f,0.95f),
		"guide/shop/shou", 2, CCRect(0,0,94,125), true);
	pSprite->setScale(Scale_X);
	CMainMenu::m_pScene->addChild(pSprite, 50, 493);
}

void ccbLayer::GuideFingerClear()
{
	if ( CMainMenu::m_pScene->getChildByTag(493) )
		CMainMenu::m_pScene->getChildByTag(493)->removeFromParentAndCleanup(true);
}

void ccbLayer::ShareShow( cocos2d::CCObject *pSender )
{
	if ( m_bShare || AppDelegate::s_FirstLogin == 4 )
		return;
	m_bShare = true;

	if ( CMainMenu::GetStatus() == 0 )
		m_iPress = 0;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	pCCLayerColor->setPosition(ccp(0,0));
	SetScale(pCCLayerColor);
	CMainMenu::m_pScene->addChild(pCCLayerColor, 19, 687);


	CCLayer* pLayer = CCLayer::create();
	CCSprite* pSprite = CCSprite::create("share/di.png");
	pSprite->setPosition(ccp(400, 240));
	pLayer->addChild(pSprite, 20, 688);
	CMainMenu::m_pScene->addChild(pLayer, 20, 688);
	SetScale(pLayer);

	if ( m_iShare > AppDelegate::m_Share+1 )
		AppDelegate::m_Share = m_iShare-1;
	m_iShareLevel = AppDelegate::m_Share+1;
	char buffer[255];
	sprintf(buffer, "share/%d.png", m_iShareLevel);
	CCSprite* pSprite1 = CCSprite::create(buffer);
	pSprite1->setPosition(ccp(215, 262));
	pSprite->addChild(pSprite1);

	CCMenuItemImage* pItem1 = CCMenuItemImage::create("share/teng1.png", "share/teng2.png", this, menu_selector(ccbLayer::Share1));
	CCMenuItemImage* pItem2 = CCMenuItemImage::create("share/wei1.png", "share/wei2.png", this, menu_selector(ccbLayer::Share2));
	CCMenuItemImage* pItem3 = CCMenuItemImage::create("share/xing1.png", "share/xing2.png", this, menu_selector(ccbLayer::Share3));
	pItem1->setPosition(ccp(93, 50));
	pItem2->setPosition(ccp(245, 50));
	pItem3->setPosition(ccp(400, 50));
	CCMenu* pMenu1 = CCMenu::create(pItem1, pItem2, pItem3, NULL);
	pMenu1->setPosition(ccp(0,0));
	pSprite->addChild(pMenu1);

	CCMenuItemImage* pItem4 = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbLayer::ShareDisappear));
	pItem4->setPosition(ccp(510, 333));
	CCMenu* pMenu2 = CCMenu::create(pItem4, NULL);
	pMenu2->setPosition(ccp(0,0));
	pSprite->addChild(pMenu2, -1);
}

void ccbLayer::ShareDisappear( cocos2d::CCObject *pSender )
{
	m_bShare = false;
	CMainMenu::m_pScene->removeChildByTag(687);
	CMainMenu::m_pScene->removeChildByTag(688);
	if ( AppDelegate::m_Share > 5 )
	{
		m_pShareMain->setVisible(false);
	}
}

void ccbLayer::ShareOK( cocos2d::CCObject *pSender )
{
	AppDelegate::m_Share = m_iShare;
	ShareCancle();
	ShareDisappear();
}

void ccbLayer::ShareCancle()
{
	m_bShareWait = false;
}

void ccbLayer::Share1( cocos2d::CCObject *pSender )
{
#ifndef Share_Test
	if ( AppDelegate::m_Share == m_iShare || m_bShareWait )
		return;
#endif
	m_bShareWait = true;
#ifdef Share_Tencent
	PayService::ShareSend(1, m_iShareLevel);
#endif
}

void ccbLayer::Share2( cocos2d::CCObject *pSender )
{
#ifndef Share_Test
	if ( AppDelegate::m_Share == m_iShare || m_bShareWait )
		return;
#endif
	m_bShareWait = true;
#ifdef Share_WX
	PayService::ShareSend(2, m_iShareLevel);
#endif
}

void ccbLayer::Share3( cocos2d::CCObject *pSender )
{
#ifndef Share_Test
	if ( AppDelegate::m_Share == m_iShare || m_bShareWait )
		return;
#endif
	m_bShareWait = true;
#ifdef Share_Sina
	PayService::ShareSend(3, m_iShareLevel);
#endif
}

//void ccbLayer::keyBackClicked()
//{
//	OnReturn(NULL);
//}
