#include "GameStart.h"
#include "MainMenu.h"
#include "AppDelegate.h"
#include "baseRes.h"
#include "Common.h"

using namespace cocos2d;
using namespace cocos2d::extension;

ccbStartGame::ccbStartGame()
	: m_AnimationManager(NULL)
{
}

ccbStartGame::~ccbStartGame()
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbStartGame::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) 
{
	m_pNode2 = pNode->getChildByTag(102);
	m_pNode = pNode->getChildByTag(101);
	setAnchorPoint(ccp(0.5f, 0.5f));
}

SEL_MenuHandler ccbStartGame::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;    
}

SEL_CCControlHandler ccbStartGame::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) 
{
	return NULL;
}


bool ccbStartGame::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) 
{
	return false;
}

bool ccbStartGame::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
	bool bRet = false;
	return bRet;
}

void ccbStartGame::setAnimationManager(CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler)
{
	CC_SAFE_RELEASE_NULL(m_AnimationManager);
	m_AnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(m_AnimationManager);
	m_pGameControler = _pGameControler;
	if ( m_pGameControler->m_unLevel%6 != 5 && m_pGameControler->m_unLevel < 30 && m_pGameControler->m_unLevel >= 0 )
		m_bType = 0;
	else
		m_bType = 1;
	if ( m_bType )
	{
		m_pNode->setVisible(false);
		m_pNode2->setVisible(true);
		//CCParticleSystemQuad* pParticle = CCParticleSystemQuad::create("tu8/wind.plist");
		//pParticle->setAnchorPoint(ccp(0.5f, 0.0f));
		//pParticle->setPosition(ccp(357, 172));
		//m_pNode2->addChild(pParticle, -1);
		CCSprite* pSprite = CCSprite::create("tu8/feijiti.png");
		pSprite->setAnchorPoint(ccp(0.0f,0.0f));
		//pSprite->setPosition(ccp(100,-100));
		m_pNode2->addChild(pSprite);
		CCParticleSystemQuad* pParticle = CCParticleSystemQuad::create("tu8/1.plist");
		//pParticle->setAnchorPoint(ccp(0.5f, 0.0f));
		pParticle->setPosition(ccp(440, 210));
		m_pNode2->addChild(pParticle);
	}
	else
	{
		m_pNode2->setVisible(false);
		m_pNode->setVisible(true);
		//CCParticleSystemQuad* pParticle = CCParticleSystemQuad::create("tu8/wind.plist");
		//pParticle->setAnchorPoint(ccp(0.5f, 0.0f));
		//pParticle->setPosition(ccp(357, 172));
		//m_pNode->addChild(pParticle, -1);
		CCSprite* pSprite = CCSprite::create("tu8/feijiti.png");
		pSprite->setAnchorPoint(ccp(0.0f,0.0f));
		//pSprite->setPosition(ccp(100,-100));
		m_pNode->addChild(pSprite);
		CCParticleSystemQuad* pParticle = CCParticleSystemQuad::create("tu8/1.plist");
		//pParticle->setAnchorPoint(ccp(0.5f, 0.0f));
		pParticle->setPosition(ccp(440, 210));
		m_pNode->addChild(pParticle);
	}
}

cocos2d::SEL_CallFuncN ccbStartGame::onResolveCCBCCCallFuncSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{

	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "start", ccbStartGame::Start);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "repeat", ccbStartGame::Repeat);
	CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "flyout", ccbStartGame::FlyOut);
	return 0;
}

void ccbStartGame::Start( cocos2d::CCNode* sender )
{
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
	if ( m_bType )
	{
		m_pNode->setVisible(false);
		m_pNode2->setVisible(true);
	}
	else
	{
		m_pNode2->setVisible(false);
		m_pNode->setVisible(true);
	}
}

void ccbStartGame::Repeat( cocos2d::CCNode* sender )
{
	m_iStayTimes++;
	if ( m_iType == 0 )
	{
		if ( m_iStayTimes == 1 )
		{
			m_bQustionTime = false;
			m_pGameControler->HeroStart();
			unschedule(schedule_selector(ccbStartGame::Timer));
		}
		else if ( m_iStayTimes == 4 )
		{
			//CCMoveTo* pMoveTo = CCMoveTo::create(1.0f, ccp(m_ccpEnd.x+800, m_ccpEnd.y+300));
			//stopAllActions();
			//runAction(CCSequence::create(pMoveTo, CCCallFunc::create(this, callfunc_selector(ccbStartGame::RemoveThis)), NULL));
			return m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline3", 0.0f);
		}
		m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
		if ( m_bType )
		{
			m_pNode->setVisible(false);
			m_pNode2->setVisible(true);
		}
		else
		{
			m_pNode2->setVisible(false);
			m_pNode->setVisible(true);
		}
	}
	else
	{
		if ( m_iStayTimes == 1 )
		{
			unschedule(schedule_selector(ccbStartGame::Timer));
			CCMoveBy* pAction1 = CCMoveBy::create(1.0f, ccp(0, -155));
			CCAnimate* pAction2 = CCAnimate::create(common::CreateAnimationFrame("guide/box/", 2, CCRect(0,0,106,108)));
			CCDelayTime* pDelay = CCDelayTime::create(0.5f);
			CCCallFunc* pCallfunc = CCCallFunc::create(this, callfunc_selector(ccbStartGame::GuideBox));
			m_pGuideBox->runAction(CCSequence::create(pAction1, pAction2, pDelay, pCallfunc, NULL));
			m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline2", 0.0f);
		}
		else if ( m_iStayTimes == 2 )
		{
			m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline3", 0.0f);
		}
	}
}

void ccbStartGame::FlyOut( cocos2d::CCNode* sender )
{
	if ( m_iType == 0 )
		m_pGameControler->GameStart();
	removeFromParentAndCleanup(true);
}

void ccbStartGame::Appear( cocos2d::CCPoint _StartPoint, cocos2d::CCPoint _EndPont, int _type )
{
	m_iStayTimes = 0;
	m_ccpEnd = _EndPont;
	m_bQustionTime = true;
	m_iType = _type;
	setPosition(_StartPoint);
	//CCMoveTo* pMoveTo = CCMoveTo::create(1.0f, _EndPont);
	//runAction(pMoveTo);
	//CCMoveTo* pMoveTo1 = CCMoveTo::create(1.0f, _EndPont);
	//m_pGameControler->m_pHero->GetArmature()->runAction(pMoveTo1);
	m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);

	if ( m_iType == 0 )
	{
		m_pGameControler->m_pHero->GetArmature()->setPosition(_StartPoint);
		if ( m_pGameControler->m_unLevel%6 != 5 && m_pGameControler->m_unLevel < 30 && m_pGameControler->m_unLevel >= 0 )
		{
			//CCDictionary *strings = CCDictionary::createWithContentsOfFileThreadSafe("Tile/QustionStrings.xml");
			for ( int i = 0; i < 3; i++ )
			{
				char buffer[255];
				//sprintf(buffer, "%d", m_iLevel*3+i);
				//const char* str = ((CCString*)strings->objectForKey(buffer))->m_sString.c_str();
				//CCLabelTTF* pLaber = CCLabelTTF::create(str, "Arial", 22,
				//CCSizeMake(260, 26), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
				sprintf(buffer, "Tile/QuestionInfo/%d.png", m_pGameControler->m_unLevel*3+i);
				CCSprite* pLaber = CCSprite::create(buffer);
				if (pLaber)
				{
					pLaber->setAnchorPoint(ccp(0.0f, 0.0f));
					pLaber->setPosition(ccp(480, 93-40*i));
					//pLaber->setColor( ccc3(0, 0, 0) );
					m_pNode->addChild(pLaber);
				}
			}
		}
	}
	else
	{
		m_pGuideHolder = CCSprite::create("guide/box/ganzi.png");
		m_pGuideHolder->setPosition(ccp(_StartPoint.x, _StartPoint.y-20));
		m_pGuideHolder->setAnchorPoint(ccp(0.5f, 1.0f));
		getParent()->addChild(m_pGuideHolder, getZOrder()-1);

		m_pGuideBox = CCSprite::create("guide/box/1.png");
		m_pGuideBox->setPosition(ccp(_StartPoint.x, _StartPoint.y-115));
		m_pGuideBox->setAnchorPoint(ccp(0.5f, 0.2f));
		m_pGameControler->m_pHero->GetArmature()->getParent()->addChild(m_pGuideBox, m_pGameControler->m_pHero->GetArmature()->getZOrder()+1);
	}

	schedule(schedule_selector(ccbStartGame::Timer), 1.0f/90.0f);
}

void ccbStartGame::RemoveThis()
{
	removeFromParentAndCleanup(true);
}

void ccbStartGame::Timer( float _t )
{
	if ( m_iType == 0 )
	{
		float x = m_pNode->getPositionX();
		float y = m_pNode->getPositionY()+194+m_pGameControler->m_iYDist+m_pGameControler->m_iYDist2;
		m_pGameControler->m_pHero->GetArmature()->setPosition(x, y);
		if ( y > 240 )
		{
			m_pGameControler->m_pCBackgroud->setPositionY(Scale_Y*(240 - y)/4.0f);
			//m_pGameControler->m_pCMidground->setPositionY(Scale_Y*(240 - y));
			m_pGameControler->m_pCFrontgroud->setPositionY(Scale_Y*(240 - y));
		}
	}
	else
	{
		float x = m_pNode->getPositionX()+getPositionX();
		float y = m_pNode->getPositionY()+getPositionY();
		m_pGuideHolder->setPosition(ccp(x+50, y));
		m_pGuideBox->setPosition(ccp(x+40, y-105));
	}
}

void ccbStartGame::GuideBox()
{
	m_pGuideHolder->removeFromParentAndCleanup(true);
	m_pGuideBox->removeFromParentAndCleanup(true);
	m_pGameControler->GuideVIP();
}
