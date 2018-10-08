#ifndef _CCB_PAUSE_H_
#define _CCB_PAUSE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameControler.h"

class ccbPauseMenu
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbPauseMenu, create);

	ccbPauseMenu();
	virtual ~ccbPauseMenu();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager, CGameControler* _pGameControler);

	void Appear(int _type = 0);
	void OnBack(cocos2d::CCObject *pSender = NULL);
	void OnReturn(cocos2d::CCObject *pSender);
	void OnReplay(cocos2d::CCObject *pSender);
	void OnFull(cocos2d::CCObject *pSender);
	void ReturnCallBack(float _t);

	void MusicShow();
	void BackTime(float _t);

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

	void setHandelPos(cocos2d::CCPoint point);
	void setMusic();

private:
	bool m_bPause;
	bool m_bCanPress;
	cocos2d::CCNode* m_pSprite;
	cocos2d::CCNode* m_pMusicPauseOn;
	cocos2d::CCNode* m_pMusicPauseOff;
	CGameControler*	m_pGameControler;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;

	cocos2d::CCNode* mainNode;
	bool isInPays;
	int pauseIndex;
	int butPos[5][2];
};

class CCBReader;
class ccbPauseMenuLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbPauseMenuLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbPauseMenu);
};

#endif
