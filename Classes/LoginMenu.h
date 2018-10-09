#ifndef _CCB_LOGINMENU_H_
#define _CCB_LOGINMENU_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpHandle.h"

//const int butPos[7][2]={{150,170},{253,52},{405,52},{561,52},{720,52},{763,326},{765,445}};

//using namespace cocos2d;
enum menuGroup{
	MainGroup,
	RankGroup,
	AchieveGroup,
	AboutGroup,
	DiaExitGroup,
};

class ccbLoginMenu
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
	, public cocos2d::CCTextFieldDelegate
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbLoginMenu, create);
	
	ccbLoginMenu();
	virtual ~ccbLoginMenu();
	static void LoginScene(cocos2d::CCScene* _pScene, int _type = 0);
	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);

	void OnMainMenu(cocos2d::CCObject *pSender);
	void OnShop(cocos2d::CCObject *pSender);
	void OnAchieve(cocos2d::CCObject *pSender);
	void OnHelp(cocos2d::CCObject *pSender);
	void OnAbout(cocos2d::CCObject *pSender);
	void OnBack(cocos2d::CCObject *pSender = NULL);
	void OnGetPrize(cocos2d::CCObject *pSender);

	bool CanInput();

	void Enter();
	void Achieve();
	void AchieveStart(float _t);
	void Appear(cocos2d::CCNode* sender = NULL);
	void Disappear(cocos2d::CCNode* sender = NULL);
	void Show(cocos2d::CCNode* sender = NULL);

	cocos2d::CCSprite* createHandelCreator();
	//点击事件
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	//virtual void onKeyPressed(CCEventKeyboard::KeyCode keyCode, CCEvent* event);

	void OnMoreHelp(cocos2d::CCObject* sender);
	void OnMoreSet(cocos2d::CCObject* sender);
	void OnMoreAbout(cocos2d::CCObject* sender);
	void OnSetVoice(cocos2d::CCObject* sender);
	void OnGetAchive(cocos2d::CCObject* sender);

	void OnRank1(cocos2d::CCObject* sender);
	void OnRank2(cocos2d::CCObject* sender);
	void OnRank3(cocos2d::CCObject* sender);
	void OnRank4(cocos2d::CCObject* sender);
	void ShowRank();
	void ShowRank(int _type);
	void ShowRank(int _type, const char* _char);

	void OnMore1(bool _bShow);
	void OnMore2(bool _bShow);
	void OnMore3(bool _bShow);
	void SeeAgain();

	void ChangeAchieveShow(cocos2d::CCObject* sender);
	void InputName(cocos2d::CCObject* sender);
	virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * pSender, const char * text, int nLen);
	void disappear();
	void HelpMarkShow();
	void ExitReady();
	void OnExitGame(cocos2d::CCObject* sender = NULL);
	void ExitYes(cocos2d::CCObject* sender);
	void ExitNo(cocos2d::CCObject* sender);
	void NetError();
	void TipDisappear();

	void ExitProtecter(float _t);

	void ShakeName();

	void setAboutHandel();

	int m_iEnterType;
	bool m_bAchieve;
	bool m_bEnter;
	bool m_bGetPrize;
	bool m_bRank;
	cocos2d::CCNode*	m_pLED;
	static ccbLoginMenu* s_pLogin;
	cocos2d::CCSprite*	 m_pselector;
private:
	int m_iShowMode;
	int	m_iShowMax;
	int	m_iLocation;
	int	m_iPress;
	int	m_iMoreType;
	int	m_iType;
	bool m_bPress;
	bool m_bAbout;
	int	m_iAchive[59];
	cocos2d::CCSprite*	m_pUp;
	cocos2d::CCSprite*	m_pDown;
	cocos2d::CCNode*	m_pBoard;
	cocos2d::CCNode*	m_pNode;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
	cocos2d::extension::CCBAnimationManager* m_pHelpActionManager;
	cocos2d::CCMenuItemToggle* m_pItem0;
	cocos2d::CCMenuItemToggle* m_pItem1;
	cocos2d::CCMenuItemToggle* m_pItem2;
	cocos2d::CCMenuItemToggle* m_pItem3;
	cocos2d::CCMenuItemToggle* m_pRank1;
	cocos2d::CCMenuItemToggle* m_pRank2;
	cocos2d::CCMenuItemToggle* m_pRank3;
	cocos2d::CCMenuItemToggle* m_pRank4;
	cocos2d::CCMenuItemToggle* m_pItemVoice;
	cocos2d::CCMenuItem*		m_pNameBtn;
	cocos2d::CCTextFieldTTF*	m_pNameInput;
	cocos2d::CCTextFieldTTF*	m_pNameInput1;
	cocos2d::CCNode*			m_pRankList;
	cocos2d::CCMenuItem*		pItem;
	
	bool	m_bInput;
	std::string m_sNameSave;

	CHttpHandle* m_pHttpHandle;
	int m_iRankType;

	int rank_rank[6];
	std::string rank_name[6];
	int rank_score[6];
	bool m_bHelpMark;
	bool m_bExit;
	cocos2d::CCNode* m_pGetPrizeNode;
	int butPos[7][2];
	int setPos[3][2];
	int achievePos[4][2];
	menuGroup groupIndex;
	int menuButIndex[5];
	int butIndex;
	int achievePage;
	int maxAchievePage;
};

class CCBReader;
class ccbLoginMenuLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbLoginMenuLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbLoginMenu);
};

#endif
