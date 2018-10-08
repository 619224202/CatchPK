#include "LoginEDLayer.h"
#include "LoginMenu.h"
#include "Common.h"
#include "AppDelegate.h"
#include "baseRes.h"
#include "MainMenu.h"
#include "PayService.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace common;

#define GoldCost		500
#define d_iGridWH		108, 102
#define d_iLocationX(i)	160*i+5
#define d_iLocationY	42
#define d_iDist			190
#define	d_iBar			390
#define d_iTime			120
#define d_iBgLocation	273, 175
#define d_iPartLocation	240, 200
#define d_iBarX			100

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
static int _Level[] =
{
	0, 0, 1, 1, 2, 2, 3, 3, 
	0, 0, 1, 1, 2, 2, 2, 3, 3,
	2, 3, 0, 1, 2, 1, 2, 3, 6, 6, 6,
	4, 4, 4, 4, 5, 6, 6, 6,
};
static int _Probability[] =
{
	100, 100, 40, 30, 20, 10, 0, 0,
	100, 100, 40, 30, 20, 10, 5, 0, 0,
	5, 0, 100, 20, 10, 5, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 100, 0, 0, 0,
};
static int _ChoseType[] =
{
	0, 0, 0, 1, 1, 2, 3, 4, 6, 5,
};
static int _DayX[] =
{
	157, 164, 170,
};
static int _DayY[] =
{
	267, 349, 425,
};

ccbLoginEDLayer* ccbLoginEDLayer::s_pccbLoginEDLayer = NULL;

ccbLoginEDLayer::ccbLoginEDLayer()
	: m_AnimationManager(NULL)
{
	for ( int i = 0; i < 36; i++ )
	{
		m_pPrize[i].digit		= i;
		m_pPrize[i].type		= _Type[i];
		m_pPrize[i].num			= _Num[i];
		m_pPrize[i].level		= _Level[i];
		m_pPrize[i].probability = _Probability[i];
	}
	m_bOnSell = false;
	butIndex = 0;
}

ccbLoginEDLayer::~ccbLoginEDLayer()
{
	s_pccbLoginEDLayer = NULL;
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbLoginEDLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	s_pccbLoginEDLayer = this;
	m_pPrizeBoard = pNode->getChildByTag(102);

	m_pBoard = pNode->getChildByTag(101);
	m_pBoard->setZOrder(5);
	CCSprite* pBG = CCSprite::create("tu16/zhuanpan/limian.png");
	pBG->setPosition( CCPoint(d_iBgLocation) );
	m_pBoard->addChild(pBG, -5);

	m_pLayer = CSizeLayer::createWithSize(CCSize(424, 138/*800, 480*/));
	m_pLayer->setPosition(ccp(106,160));
	m_pBoard->addChild(m_pLayer, -1);

	for (int i = 0; i < 2; i++ )
	{
		m_pBar[i] = CCSprite::create("tu16/zhuanpan/pao.png");
		m_pBar[i]->setPosition( ccp(d_iBarX-i*d_iBar, d_iLocationY) );
		m_pBar[i]->setAnchorPoint( ccp(0,0) );
		m_pLayer->addChild(m_pBar[i], -2);
	}

	CCParticleSystem* pParticle = CCParticleSystemQuad::create("tu16/zhuanpan/beihou.plist");
	pParticle->setAnchorPoint(ccp(0.5f, 0.5f));
	pParticle->setPosition( CCPoint(d_iLocationX(1), 75) );
	m_pLayer->addChild(pParticle, -2);

	CCSprite* pSprite = CCSprite::create("tu16/zhuanpan/xuanzhong1.png");	
	CCArray* animFrames = CCArray::createWithCapacity(2);
	{
		CCSpriteFrame* frame = CCSpriteFrame::create("tu16/zhuanpan/xuanzhong1.png",CCRect(0, 0, d_iGridWH));
		animFrames->addObject(frame);
	}
	{
		CCSpriteFrame* frame = CCSpriteFrame::create("tu16/zhuanpan/xuanzhong2.png",CCRect(0, 0, d_iGridWH));
		animFrames->addObject(frame);
	}
	CCAnimation* pAnimation  = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
	pSprite->runAction( CCRepeatForever::create(CCAnimate::create(pAnimation)) );
	pSprite->setAnchorPoint(ccp(0.5f, 0.5f));
	pSprite->setPosition( ccp(d_iLocationX(1), 75) );
	m_pLayer->addChild(pSprite, -2);
	//m_pStarAnimation->retain();

	CCNode* pLoginDayBoard = pNode->getChildByTag(103);
	for ( int i = 0; i < 3 - AppDelegate::s_PrizeDayNum; i++ )
	{
		CCSprite* pSprite = CCSprite::create("tu16/an.png");
		pSprite->setPosition(ccp(_DayX[i], _DayY[i]));
		pLoginDayBoard->addChild(pSprite);
	}
	if ( AppDelegate::s_WeaponOwn[7] )
	{//以后
		CCSprite* pSprite2 = CCSprite::create("tu16/qian.png");
		pSprite2->setPosition(ccp(141, 70));
		pLoginDayBoard->addChild(pSprite2);
		CCSprite* pSprite1 = CCSprite::create("tu16/jiangli500.png");
		pSprite1->setPosition(ccp(141, 180));
		pLoginDayBoard->addChild(pSprite1);
	}
	else
	{//第一次
		//CCSprite* pSprite3 = CCSprite::create("tu16/qiang1.png");
		//pSprite3->setPosition(ccp(141, 70));
		//pLoginDayBoard->addChild(pSprite3);
		CCSprite* pSprite3 = CCSprite::create("tu16/qiang1.png");
		pSprite3->setPosition(ccp(141,70));
		pLoginDayBoard->addChild(pSprite3);

		CCSprite* pSprite2 = CCSprite::create("tu16/qiang2.png");
		pSprite2->setAnchorPoint(ccp(0.8f, 0.2f));
		CCScaleTo* pAction1 = CCScaleTo::create(0.5f, 1.3f);
		CCScaleTo* pAction2 = CCScaleTo::create(0.2f, 1.0f);
		CCDelayTime* pAction3 = CCDelayTime::create(1.5f);
		pSprite2->runAction( CCRepeatForever::create(CCSequence::create(pAction1, pAction2, pAction3, NULL)) );
		pSprite2->setPosition(ccp(168, 33));
		pLoginDayBoard->addChild(pSprite2);

		CCSprite* pSprite1 = CCSprite::create("tu16/santian.png");
		pSprite1->setPosition(ccp(145, 180));
		pLoginDayBoard->addChild(pSprite1);
	}

	{
		CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccNodeLoaderLibrary->registerCCNodeLoader("ccbGetPrizeLayer", ccbGetPrizeLayerLoader::loader());
		cocos2d::extension::CCBReader* ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		m_pGetPrize = (ccbGetPrizeLayer*)ccbReader->readNodeGraphFromFile("23.ccbi"/*, this*/);
		m_pGetPrize->setAnimationManager(ccbReader->getAnimationManager());
		m_pGetPrize->setPosition(ccp(0,0));
		addChild(m_pGetPrize, 5);
		ccbReader->release();
	}
	//CCLog("createHandelCreator");
	
	createHandelCreator();
	setVisible(false);
}
//增加手指
void ccbLoginEDLayer::createHandelCreator(){
	int pPos[2][2] = {{143,33},{320,41}};
	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			butPos[i][j] = pPos[i][j];
		}
	}
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
	m_pselector = CCSprite::create();
	if(m_pselector==NULL){
		CCLog("slelector is not live");
	}
	m_pselector->setAnchorPoint(ccp(0,1));
	m_pselector->setPosition(ccp(butPos[butIndex][0],butPos[butIndex][1]));
	m_pselector->initWithSpriteFrame(frame1);
	m_pselector->runAction(CCRepeatForever::create(c_hAnimate));
	m_pselector->setVisible(false);
	addChild(m_pselector, 10, 1000);
}

SEL_MenuHandler ccbLoginEDLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbLoginEDLayer::OnStart );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbLoginEDLayer::OnBack );
	return NULL;    
}


SEL_CCControlHandler ccbLoginEDLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbLoginEDLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbLoginEDLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbLoginEDLayer::setAnimationManager(CCBAnimationManager *pAnimationManager, cocos2d::CCNode* _pNode)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pFather = _pNode;
}

void ccbLoginEDLayer::keyBackClicked()
{
	OnBack();
}

void ccbLoginEDLayer::keyEnterClicked(){
	if(butIndex==0){
		OnBack();
	}else{
		OnStart(this);
	}
}

void ccbLoginEDLayer::keyArrowClicked(int arrow){
	//CCLog("按下了方向键:%d",arrow);
	switch(arrow){
	case kTypeLeftArrowClicked:
	case kTypeRightArrowClicked:
		if(butIndex==0){
			butIndex = 1;
		}else{
			butIndex = 0;
		}
		
		break;
	case kTypeDownArrowClicked:
		
		break;
	case kTypeUpArrowClicked:
		
		break;
	}
	//int butPos[][2] = BUTPOS;
//	CCLog("x=%d,y=%d",butPos[butIndex][0],butPos[butIndex][1]);
	m_pselector->setPosition(ccp(butPos[butIndex][0],butPos[butIndex][1]));
	//m_pselector->setPosition(ccp(100,100));
}

void ccbLoginEDLayer::keyNumClicked(int keyNum){
	//CCLog("按下了数字键:%d",keyNum);
	switch(keyNum){
	case kTypeNum0:
		OnBack();
		break;
	}
}

void ccbLoginEDLayer::initPrize()
{
	if ( AppDelegate::s_PrizeSave[0] < 0 )
	{//重随
		for ( int i = 0; i < 10; i++ )
		{//每日奖励
			int num = rand()%20+1;
			while ( num )
			{
				for ( int j = 0; j < 36; j++ )
				{
					if ( m_pPrize[j].level == _ChoseType[i] && !m_pPrize[j].chosed )
					{
						num--;
						if ( num == 0 )
						{
							m_pPrize[j].chosed = true;
							m_ppChosePrize[i] = &m_pPrize[j];
							AppDelegate::s_PrizeSave[i] = j;
							break;
						}
					}
				}
			}
		}
		AppDelegate::SaveEveryDayItem();
		//for ( int i = 0; i < 1; i++ )
		//{//打折
		//	bool ok = true;
		//	while ( ok )
		//	{
		//		ok = false;
		//		AppDelegate::s_ShopOnSell[i] = rand()%10+4;
		//		if ( AppDelegate::s_ShopOnSell[i] == 7 )
		//			ok = true;
		//		else for ( int j = 0; j < i; j++ )
		//		{
		//			if ( AppDelegate::s_ShopOnSell[j] == AppDelegate::s_ShopOnSell[i] )
		//				ok = true;
		//		}
		//	}
		//}
	}
	else
	{
		for ( int i = 0; i < 10; i++ )
		{
			m_ppChosePrize[i] = &m_pPrize[AppDelegate::s_PrizeSave[i]];
		}
	}
}

void ccbLoginEDLayer::Appear()
{
	setVisible(true);
	m_pselector->setVisible(true);
	setKeypadEnabled(true);
	m_bPress	= false;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 1800, 600);
	pCCLayerColor->setPosition(ccp(0,0));
	addChild(pCCLayerColor, -10, 14);

	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
	//scheduleOnce(schedule_selector(ccbLoginEDLayer::Start), 1.0f);

	bool remove = false;
	if ( m_pPrizeBoard->getChildByTag(10) )
		remove = true;
	for ( int i = 0; i < 10; i++ )
	{
		if ( remove )
			m_pPrizeBoard->removeChildByTag(10+i);
		CCSprite* pBoard = CCSprite::create("tu16/di.png");
		pBoard->setAnchorPoint(ccp(0.5f,0.5f));
		pBoard->setPosition(ccp((i%5)*94+82, 228-87*(i/5)));
		m_pPrizeBoard->addChild(pBoard, 0, 10+i);

		if ( m_ppChosePrize[i]->level == 4 || m_ppChosePrize[i]->level == 3 || m_ppChosePrize[i]->level == 6 )
		{
			CCSprite* pEffect = common::CreateAnimation("tu16/san1.png", ccp(46,44), ccp(0.5f,0.5f),
				"tu16/san", 2, CCRect(0,0,92,89), true);
			pBoard->addChild(pEffect);
		}
		if ( m_ppChosePrize[i]->level == 4 || m_ppChosePrize[i]->level == 6 )
		{
			CCSprite* pEffect = common::CreateAnimation("Animation/prize4/1.png", ccp(46,44), ccp(0.5f, 0.5f),
				"Animation/prize4/", 9, CCRect(0,0,90,88), true);
			pBoard->addChild(pEffect);
		}
		char buffer[255];
		if ( m_ppChosePrize[i]->digit < 0 || m_ppChosePrize[i]->digit > 35 )
			m_ppChosePrize[i]->digit = 0;
		sprintf(buffer, "tu16/item/%d.png", m_ppChosePrize[i]->digit);
		CCSprite* pSprite = CCSprite::create(buffer);
		pSprite->setAnchorPoint(ccp(0.5f,0.5f));
		pSprite->setPosition(ccp(46,44));
		pBoard->addChild(pSprite);
	}

	if ( m_pPrizeBoard->getChildByTag(30) )
		m_pPrizeBoard->removeChildByTag(30);
	CCSprite* pSprite1 = NULL;
	if ( AppDelegate::s_PrizeFree )
	{
		pSprite1 = CCSprite::create("tu16/mianfei.png");
		pSprite1->setAnchorPoint(ccp(0.5f,0.5f));
	}
	else
	{
#ifdef Plat_91
		pSprite1 = CCSprite::create("tu16/500_1.png");
#else
		pSprite1 = CCSprite::create("tu16/500.png");
#endif
		pSprite1->setAnchorPoint(ccp(0.5f,0.5f));
	}
	pSprite1->setPosition(ccp(435,50));
	m_pPrizeBoard->addChild(pSprite1, 0, 30);

	//if ( AppDelegate::s_FirstLogin < 5 )
	//{//手指
	//	CCSprite* pSprite = common::CreateAnimation("guide/shop/shou1.png", ccp(263,50), ccp(0.9f,0.95f),
	//		"guide/shop/shou", 2, CCRect(0,0,94,125), true);
	//	m_pPrizeBoard->addChild(pSprite, 50, 493);
	//}

	remove = false;
	if ( m_pLayer->getChildByTag(40) )
		remove = true;
	for ( int i = 0; i < 3; i++ )
	{
		if ( remove )
			m_pLayer->removeChildByTag(40+i);
		m_iArray[i] = i;
		int a = rand()%3+i*3;
		char buffer[255];
		sprintf(buffer, "tu16/item/%d.png", AppDelegate::s_PrizeSave[a]);
		m_iIndex[i] = a;
		m_pSprite[i] = CCSprite::create(buffer);
		m_pSprite[i]->setPosition(CCPoint(d_iLocationX(i), 75));
		m_pSprite[i]->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pLayer->addChild(m_pSprite[i], -2, 40+i);
	}
}

void ccbLoginEDLayer::Start( float _t )
{
	m_iSaveX	= getPositionX();
	m_iFix		= 5;
	m_iToward	= 0;
	m_iLocate	= 0;
	m_iTime		= 0;
	m_iDist		= 1;
	m_iSpeed	= 30;
	m_iAim		= 5;
	m_bStop		= false;
	m_pGetPrize->m_bOver = false;
	schedule(schedule_selector(ccbLoginEDLayer::Timer), 0.01f);
}

void ccbLoginEDLayer::Timer( float _t )
{
	//if ( m_iToward == 1 )
	//{
	//	m_iFix = -5;
	//	m_iToward = 0;
	//}
	//else if ( m_iToward == 0 )
	//{
	//	m_iFix = 5;
	//	m_iToward =1;
	//}
	//else
	//	m_iFix = 0;
	if ( m_iTime < d_iTime )
		m_iTime++;
	if ( !m_bStop || m_iAimLocateX == d_iLocationX(0) || m_iLocate != 0 )
	{
		if ( m_bStop )
			m_iAimLocateX += m_iSpeed;
		m_iLocate += m_iSpeed;
		if ( m_iLocate > d_iDist )
		{
			m_iLocate = 0;
			for ( int i = 2; i > 0; i-- )
			{
				std::swap(m_iArray[i], m_iArray[i-1]);
				std::swap(m_iIndex[i], m_iIndex[i-1]);
			}
			int tag = m_pSprite[m_iArray[0]]->getTag();
			m_pSprite[m_iArray[0]]->removeFromParent();
			char buffer[255];
			int index = -1;
			bool repeat = true;
			while (repeat)
			{
				repeat = false;
				if ( m_iTime > d_iTime - 8 && !m_bStop)
				{
					index = RandomResult();
					m_iAim = index;
					m_iAimLocateX = d_iLocationX(0);
					m_bStop = true;
				}
				else
					index = rand()%10;

				for (int i = 1; i < 3; i++ )
				{
					if ( index == m_iIndex[i] )
					{
						repeat = true;
						if ( m_bStop )
							m_bStop = false;
					}
				}
			}
			m_iIndex[0] = index;
			if ( AppDelegate::s_PrizeSave[index] < 0 || AppDelegate::s_PrizeSave[index] > 35 )
				AppDelegate::s_PrizeSave[index] = 0;
			sprintf(buffer, "tu16/item/%d.png", AppDelegate::s_PrizeSave[index]);
			m_pSprite[m_iArray[0]] = CCSprite::create(buffer);
			m_pSprite[m_iArray[0]]->setAnchorPoint(ccp(0.5f,0.5f));
			m_pSprite[m_iArray[0]]->setPosition(CCPoint(d_iLocationX(m_iArray[0]), 75));
			m_pLayer->addChild(m_pSprite[m_iArray[0]], 0, tag);
		}
		for ( int i = 0; i < 3; i++ )
			m_pSprite[m_iArray[i]]->setPositionX(d_iLocationX(i)+m_iLocate);
		for ( int i = 0; i < 2; i++ )
			m_pBar[i]->setPositionX(d_iBarX-i*d_iBar+m_iLocate);
	}
	else
	{
		if ( m_iTime == d_iTime )
		{
			GetPrize();
			m_iToward = 2;
			m_pParticle = CCParticleSystemQuad::create("tu16/zhuanpan/xuanzhong.plist");
			m_pParticle->setPosition( ccp(d_iLocationX(1), 75) );
			m_pParticle->setAnchorPoint(ccp(0.5f, 0.5f));
			m_pLayer->addChild(m_pParticle, 1);
		}
		m_iTime++;
		if ( m_iTime == d_iTime + 50 )
		{
			m_pParticle->setVisible(false);
			m_pParticle->removeFromParentAndCleanup(true);
			Next();
		}

		if ( m_pGetPrize->m_bOver )
		{
			if ( m_ppChosePrize[m_iAim]->type != 11 )
			{
				if ( AppDelegate::s_FirstLogin == 2 )
				{
					AppDelegate::s_FirstLogin = 3;
					AppDelegate::SaveGuide();
					AppDelegate::ChangeScene( CMainMenu::StartMenu(0, 0, true) );
					return;
				}
				((ccbLoginMenu*)m_pFather)->m_bEnter = false;
				((ccbLoginMenu*)m_pFather)->m_bGetPrize = false;
				((ccbLoginMenu*)m_pFather)->setKeypadEnabled(true);
				((ccbLoginMenu*)m_pFather)->m_pselector->setVisible(true);
				getChildByTag(14)->removeFromParent();
				m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline3", 0.0f);
				setVisible(false);
				m_pselector->setVisible(false);
				setKeypadEnabled(false);
			}
			else
				scheduleOnce(schedule_selector(ccbLoginEDLayer::Start), 0.0f);
			unschedule(schedule_selector(ccbLoginEDLayer::Timer));
		}
	}
	//setPositionX(m_iSaveX+m_iFix);
}

void ccbLoginEDLayer::OnStart( cocos2d::CCObject* sender )
{
	if ( m_bOnSell )
		return;
	if ( m_bPress )
		return;
	m_pselector->setVisible(false);
	if ( AppDelegate::s_PrizeFree )
		AppDelegate::s_PrizeFree = 0;
#ifdef Plat_91
	else if ( AppDelegate::s_Medal < 5 )
	{
		GetMedal();
		return;
	}
	else
		AppDelegate::s_Medal -= 5;
#else
	else if ( AppDelegate::s_Money < GoldCost )
	{
#ifndef NoGetMoney
		GetMoney();
#endif
		return;
	}
	else
		AppDelegate::s_Money -= GoldCost;
#endif

	if ( m_pPrizeBoard->getChildByTag(493) )
		m_pPrizeBoard->getChildByTag(493)->removeFromParentAndCleanup(true);

	m_bPress = true;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
	scheduleOnce(schedule_selector(ccbLoginEDLayer::Start), 1.5f);
}

void ccbLoginEDLayer::OnBack( cocos2d::CCObject* sender )
{
	if ( m_bOnSell )
		return;
	if ( m_bPress )
		return;
	if ( AppDelegate::s_FirstLogin == 2 )
		return;
	m_bPress = true;
	m_pselector->setVisible(false);
	setKeypadEnabled(false);
	((ccbLoginMenu*)m_pFather)->m_bEnter = false;
	((ccbLoginMenu*)m_pFather)->m_bGetPrize = false;
	((ccbLoginMenu*)m_pFather)->setKeypadEnabled(true);
	((ccbLoginMenu*)m_pFather)->m_pselector->setVisible(true);
	//((ccbLoginMenu*)m_pFather)->Enter();
	getChildByTag(14)->removeFromParent();
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline4", 0.0f);
}

int ccbLoginEDLayer::RandomResult()
{
	int a = -1;
	int index = 2;
	while ( a == -1 )
	{
		int num = rand()%8+1;
		while ( num )
		{
			for ( int j = 0; j < 10; j++ )
			{
				if ( m_ppChosePrize[j]->level%5 == index )
				{
					num--;
					if ( num == 0 )
					{
						int r = rand()%100;
						if ( m_ppChosePrize[j]->probability > r )
							a = j;
						break;
					}
				}
			}
		}
		index--;
	}
	return a;
}

void ccbLoginEDLayer::Next()
{
	m_pGetPrize->initPrize(m_ppChosePrize[m_iAim]->digit);
	m_pGetPrize->Appear();
}

void ccbLoginEDLayer::GetPrize()
{
	switch ( m_ppChosePrize[m_iAim]->type )
	{
	case 1:
		AppDelegate::s_Money += m_ppChosePrize[m_iAim]->num;
		//if ( AppDelegate::s_Money > 99999 )
		//	AppDelegate::s_Money = 99999;
		break;
	case 2:
		AppDelegate::s_Medal += m_ppChosePrize[m_iAim]->num;
		if ( AppDelegate::s_Medal > 99999 )
			AppDelegate::s_Medal = 99999;
		break;
	case 3:
		AppDelegate::s_GrenadeNum += m_ppChosePrize[m_iAim]->num;
		if ( AppDelegate::s_GrenadeNum > 10 )
			AppDelegate::s_GrenadeNum = 10;
		break;
	case 4:
		AppDelegate::s_Healbox += m_ppChosePrize[m_iAim]->num;
		if ( AppDelegate::s_Healbox > 3 )
			AppDelegate::s_Healbox = 3;
		break;
	case 5:
		{
			int num = m_ppChosePrize[m_iAim]->num;
			if ( AppDelegate::s_SkillEnergy[0] < 100 )
			{
				AppDelegate::s_SkillEnergy[0] = 100;
				num--;
			}
			AppDelegate::m_Sell2 += num;
		}
		break;
	case 6:
		//AppDelegate::s_SkillEnergy[1] += m_ppChosePrize[m_iAim]->num;
		//if ( AppDelegate::s_SkillEnergy[1] > 100 )
		//	AppDelegate::s_SkillEnergy[1] = 100;
		break;
	case 11:
		break;
	}
	AppDelegate::SaveGetPrize();
	AppDelegate::SaveStatus();
}

void ccbLoginEDLayer::GetMoney( cocos2d::CCObject* sender )
{
#ifdef NoGetMoney
	return;
#endif
	if ( m_bOnSell )
		return;
	m_bOnSell = true;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	addChild(pCCLayerColor, 29, 1195);

	CCMenuItem* pItem = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbLoginEDLayer::GetRemove));
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

	//CCMenuItem* pItem1 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMoneyBtn));
	//pItem1->setPosition(ccp(219,282));
	//pItem1->setTag(1);
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMoneyBtn));
	pItem2->setPosition(ccp(243,199));
	pItem2->setTag(2);
	CCMenuItem* pItem3 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMoneyBtn));
	pItem3->setPosition(ccp(556,199));
	pItem3->setTag(3);
	//CCMenuItem* pItem4 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMoneyBtn));
	//pItem4->setPosition(ccp(286,96));
	//pItem4->setTag(4);
	//CCMenuItem* pItem5 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMoneyBtn));
	//pItem5->setPosition(ccp(512,96));
	//pItem5->setTag(5);
	CCMenu* pMenu1 = CCMenu::create(/*pItem1, */pItem2, pItem3, /*pItem4, pItem5, */NULL);
	pMenu1->setPosition(ccp(0,0));
	addChild(pMenu1, 32, 1197);
}

void ccbLoginEDLayer::GetMedal( cocos2d::CCObject* sender )
{
#ifdef NoGetMoney
	return;
#endif
	if ( m_bOnSell )
		return;
	m_bOnSell = true;

	CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
	addChild(pCCLayerColor, 29, 1195);

	CCMenuItem* pItem = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbLoginEDLayer::GetRemove));
	pItem->setPosition(ccp(723,351));
	CCMenu* pMenu = CCMenu::create(pItem, NULL);
	pMenu->setPosition(ccp(0,0));
	addChild(pMenu, 31, 1196);

	CCSprite* pSprite1 = CCSprite::create("sell/money/di.png");
	pSprite1->setPosition(ccp(400,240));
	addChild(pSprite1, 31, 1199);
	CCSprite* pSprite2 = CCSprite::create("sell/money/1.png");
	pSprite2->setPosition(ccp(400, 240));
	addChild(pSprite2, 33, 1198);

	//CCMenuItem* pItem1 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMedalBtn));
	//pItem1->setPosition(ccp(219,282));
	//pItem1->setTag(1);
	CCMenuItem* pItem2 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMedalBtn));
	pItem2->setPosition(ccp(243,199));
	pItem2->setTag(2);
	CCMenuItem* pItem3 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMedalBtn));
	pItem3->setPosition(ccp(556,199));
	pItem3->setTag(3);
	//CCMenuItem* pItem4 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMedalBtn));
	//pItem4->setPosition(ccp(283,96));
	//pItem4->setTag(4);
	//CCMenuItem* pItem5 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbLoginEDLayer::GetMedalBtn));
	//pItem5->setPosition(ccp(512,96));
	//pItem5->setTag(5);
	CCMenu* pMenu1 = CCMenu::create(/*pItem1, */pItem2, pItem3, /*pItem4, pItem5, */NULL);
	pMenu1->setPosition(ccp(0,0));
	addChild(pMenu1, 32, 1197);
}

void ccbLoginEDLayer::GetMoneyBtn( cocos2d::CCObject* _pNode )
{
	if ( _pNode )
	{
		int numAdd = 0;
		int numMid = 0;
		int index = ((CCNode*)_pNode)->getTag();
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
		//GetRemove();
	}
	else
		GetRemove();
}

void ccbLoginEDLayer::GetMedalBtn( cocos2d::CCObject* _pNode )
{
	if ( _pNode )
	{
		int numAdd = 0;
		int numMid = 0;
		int index = ((CCNode*)_pNode)->getTag();
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

void ccbLoginEDLayer::GetRemove(cocos2d::CCObject* _pNode)
{
	if ( !m_bOnSell )
		return;
	m_bOnSell = false;
	for ( int i = 1195; i < 1200; i++ )
	{
		if ( getChildByTag(i) )
			removeChildByTag(i);
	}
}
