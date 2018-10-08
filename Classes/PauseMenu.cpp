#include "PauseMenu.h"
#include "MainMenu.h"
#include "AppDelegate.h"
#include "baseRes.h"
#include "Common.h"
#include "ShopMenu.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define NeedTime 130
#define MusicBtnX	408
#define MusicBtnY	159
#define BtnDist		60

ccbPauseMenu::ccbPauseMenu()
	: m_AnimationManager(NULL)
{
	m_bCanPress = false;
	isInPays = false;
	pauseIndex = 0;
	butPos[0][0] = 286;
	butPos[0][1] = 325;
	butPos[1][0] = 403;
	butPos[1][1] = 265;
	butPos[2][0] = 410;
	butPos[2][1] = 128;
	butPos[3][0] = 527;
	butPos[3][1] = 74;
	butPos[4][0] = 693;
	butPos[4][1] = 350;
}

ccbPauseMenu::~ccbPauseMenu()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbPauseMenu::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	m_bPause = false;
	CCNode* pMusic = pNode->getChildByTag(1);
	m_pMusicPauseOn = CCSprite::create("tu4/shengying.png");
	m_pMusicPauseOff = CCSprite::create("tu4/shengying2.png");
	m_pMusicPauseOn->setPosition(ccp(0, 0));
	m_pMusicPauseOff->setPosition(ccp(0, 0));
	pMusic->addChild(m_pMusicPauseOn);
	pMusic->addChild(m_pMusicPauseOff);
	m_pMusicPauseOn->setVisible(false);
	m_pMusicPauseOff->setVisible(false);

	CCNode* pSupply = pNode->getChildByTag(101);
	CCSprite* pSprite = common::CreateAnimation("tu4/buji/1.png", ccp(0,0), ccp(0.5f,0.5f),
		"tu4/buji/", 6, CCRect(0,0,181,159), true);
	pSupply->addChild(pSprite, 1);

	pSupply = pNode->getChildByTag(102);
	pSprite = common::CreateAnimation("tu4/buji/guang1.png", ccp(0,0), ccp(0.5f,0.5f),
		"tu4/buji/guang", 3, CCRect(0,0,142,118), true);
	pSupply->addChild(pSprite, 1);

	pSupply = pNode->getChildByTag(103);
	m_pSprite = common::CreateAnimation("tu4/buji/gu1.png", ccp(0,0), ccp(0.5f,0.5f),
		"tu4/buji/gu", 3, CCRect(0,0,70,130), true);
	pSupply->addChild(m_pSprite, 1);

	CCSprite* handelSprite = ccbShopMenu::createHandelCreator();
	handelSprite->setPosition(ccp(butPos[0][0],butPos[0][1]));
	pNode->addChild(handelSprite,10,1001);
	mainNode = pNode;
}

SEL_MenuHandler ccbPauseMenu::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On1", ccbPauseMenu::OnBack );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On2", ccbPauseMenu::OnReplay );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On4", ccbPauseMenu::OnReturn );
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On5", ccbPauseMenu::OnFull );
	return NULL;    
}

SEL_CCControlHandler ccbPauseMenu::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbPauseMenu::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbPauseMenu::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbPauseMenu::setAnimationManager(CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pGameControler = _pGameControler;
}

void ccbPauseMenu::Appear(int _type)
{
	m_bPause = true;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Untitled Timeline", 0.0f);
	//m_pSprite->setRotation(120);
	setTouchEnabled(true);
	setKeypadEnabled(true);
	m_pGameControler->setKeypadEnabled(false);
	MusicShow();
	AppDelegate::AudioPause();
	isInPays = false;
	pauseIndex = 0;
	if ( 6 == m_pGameControler->m_iLevelType && !getChildByTag(988) )
	{//
		CCSprite* pSprite = CCSprite::create("guide/t8.png");
		pSprite->setPosition(ccp(0,0));
		pSprite->setAnchorPoint(ccp(0,0));
		addChild(pSprite, 20, 988);
		pauseIndex = 4;
	}
	mainNode->getChildByTag(1001)->setVisible(true);
	setHandelPos(ccp(butPos[pauseIndex][0],butPos[pauseIndex][1]));
	
}

void ccbPauseMenu::OnBack( cocos2d::CCObject *pSender )
{
	if ( pSender != 0 && 6 == m_pGameControler->m_iLevelType )
		return;
	if ( !m_bPause || m_pGameControler->m_bSellOpen || m_bCanPress || m_pGameControler->m_bExitGame )
		return;
	m_bPause = false;
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Untitled Timeline2", 0.0f);
	//m_pSprite->setRotation(120);
	scheduleOnce(schedule_selector(ccbPauseMenu::BackTime), 0.60f);
	setTouchEnabled(false);
	setKeypadEnabled(false);
	isInPays = false;
	m_pGameControler->setKeypadEnabled(true);
	mainNode->getChildByTag(1001)->setVisible(false);
	AppDelegate::AudioResume();
}

void ccbPauseMenu::OnReplay( cocos2d::CCObject *pSender )
{
	if ( 6 == m_pGameControler->m_iLevelType || m_pGameControler->m_bSellOpen || m_pGameControler->m_bExitGame )
		return;
	AppDelegate::ChangeScene( CMainMenu::StartGame(m_pGameControler->m_unLevel, m_pGameControler->m_iDifficult) );
}

void ccbPauseMenu::OnReturn(cocos2d::CCObject *pSender)
{
	if ( 6 == m_pGameControler->m_iLevelType || m_pGameControler->m_bSellOpen || m_pGameControler->m_bExitGame )
		return;
	m_pGameControler->GameOver(0.0f);
}

void ccbPauseMenu::ReturnCallBack( float _t )
{
}

void ccbPauseMenu::MusicShow()
{
	bool bOpen = true;
	if ( AppDelegate::s_VoiceOpen == 0 )
		bOpen = false;
	m_pMusicPauseOn->setVisible(bOpen & m_bPause);
	m_pMusicPauseOff->setVisible(!bOpen & m_bPause);
	
}

void ccbPauseMenu::BackTime( float _t )
{
	m_pGameControler->m_bPause = false;
	m_pGameControler->resumeSchedulerAndActions();
	m_pGameControler->m_pHero->ResumeAction();
	m_pGameControler->removeChildByTag(1044);
	
	if ( m_pGameControler->m_iGuideAction == 13 )
		m_pGameControler->m_iGuideAction = 14;
	MusicShow();
}

void ccbPauseMenu::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
	if ( m_pGameControler->m_bSellOpen || m_pGameControler->m_bExitGame )
		return;
	CCPoint location = ((CCTouch*)*(pTouches->begin()))->getLocation();
	if ( fabs(location.x - MusicBtnX*Scale_X) < BtnDist*Scale_X 
		&& fabs(location.y - MusicBtnY*Scale_Y) < BtnDist*Scale_Y )
	{
		AppDelegate::AudioSwitch();
		if ( AppDelegate::s_VoiceOpen )
		{
			if ( m_pGameControler->m_iLevelType == 6 )
				AppDelegate::AudioPlayBgm("MS/Music/level2/1.mp3");
			else if ( m_pGameControler->m_iLevelType > 54 )
			{
				char buffer[250];
				sprintf(buffer,"MS/Music/level%d/%d.mp3", m_pGameControler->m_unLevel-54, 2);
				AppDelegate::AudioPlayBgm(buffer);
			}
			else if ( m_pGameControler->m_iLevelType > 48 )
			{
				char buffer[250];
				sprintf(buffer,"MS/Music/level%d/%d.mp3", m_pGameControler->m_unLevel-48, 1);
				AppDelegate::AudioPlayBgm(buffer);
			}
			else
			{
				char buffer[250];
				sprintf(buffer,"MS/Music/level%d/%d.mp3", m_pGameControler->m_unLevel/6+1, m_pGameControler->m_unLevel%6%2+1);
				AppDelegate::AudioPlayBgm(buffer);
			}
		}
		else
			AppDelegate::AudioStopBgm();
		MusicShow();
	}
}

void ccbPauseMenu::setMusic(){
	if ( m_pGameControler->m_bSellOpen || m_pGameControler->m_bExitGame )
		return;
	
	AppDelegate::AudioSwitch();
	if ( AppDelegate::s_VoiceOpen )
	{
		if ( m_pGameControler->m_iLevelType == 6 )
			AppDelegate::AudioPlayBgm("MS/Music/level2/1.mp3");
		else if ( m_pGameControler->m_iLevelType > 54 )
		{
			char buffer[250];
			sprintf(buffer,"MS/Music/level%d/%d.mp3", m_pGameControler->m_unLevel-54, 2);
			AppDelegate::AudioPlayBgm(buffer);
		}
		else if ( m_pGameControler->m_iLevelType > 48 )
		{
			char buffer[250];
			sprintf(buffer,"MS/Music/level%d/%d.mp3", m_pGameControler->m_unLevel-48, 1);
			AppDelegate::AudioPlayBgm(buffer);
		}
		else
		{
			char buffer[250];
			sprintf(buffer,"MS/Music/level%d/%d.mp3", m_pGameControler->m_unLevel/6+1, m_pGameControler->m_unLevel%6%2+1);
			AppDelegate::AudioPlayBgm(buffer);
		}
	}
	else
		AppDelegate::AudioStopBgm();
	MusicShow();
	
}

void ccbPauseMenu::keyBackClicked()
{
	if(!isInPays){
		OnBack(this);
	}else {
		m_pGameControler->Sell2Back(this);
		isInPays = false;
	}
}

void ccbPauseMenu::keyEnterClicked(){
	if(isInPays){
		m_pGameControler->Sell4Resume(this);
		return;
	}
	switch(pauseIndex){
	case 0:
		OnBack(this);
		break;
	case 1:
		OnReplay(this);
		break;
	case 2:
		setMusic();
		break;
	case 3:
		OnReturn(this);
		break;
	case 4:
		OnFull(this);
		break;
	}
}

void ccbPauseMenu::keyArrowClicked(int arrow){
	if(isInPays){
		return;
	}
	//CCLog("按下了方向键:%d",arrow);
	switch(arrow){
	case kTypeLeftArrowClicked:
		if(pauseIndex==1){
			pauseIndex = 0;
		}else if(pauseIndex==3){
			pauseIndex = 2;
		}else if(pauseIndex==4){
			pauseIndex = 1;
		}
		break;
	case kTypeRightArrowClicked:
		if(pauseIndex==1||pauseIndex==2){
			pauseIndex = 4;
		}else if(pauseIndex==0){
			pauseIndex = 1;
		}else if(pauseIndex==3){
			pauseIndex = 4;
		}
		break;
	case kTypeDownArrowClicked:
		if(pauseIndex<3){
			pauseIndex ++;
		}
		break;
	case kTypeUpArrowClicked:
		if(pauseIndex>0 && pauseIndex<4){
			pauseIndex --;
		}
		break;
	}
	setHandelPos(ccp(butPos[pauseIndex][0],butPos[pauseIndex][1]));
}

void ccbPauseMenu::keyNumClicked(int keyNum){
	//CCLog("按下了数字键:%d",keyNum);
	if(keyNum==kTypeNum0){
		keyBackClicked();
	}
}

void ccbPauseMenu::setHandelPos(cocos2d::CCPoint point){
	mainNode->getChildByTag(1001)->setPosition(point);
}

void ccbPauseMenu::OnFull( cocos2d::CCObject *pSender )
{
	if ( m_pGameControler->m_bSellOpen || !m_bPause || m_pGameControler->m_bExitGame )
		return;
	m_bCanPress = false;
	m_pGameControler->Sell4();
	setHandelPos(ccp(320,210));
	isInPays = true;
	if ( getChildByTag(988) )
		removeChildByTag(988);
}
