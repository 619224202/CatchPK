#ifndef _CCB_CCB_H_
#define _CCB_CCB_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CSArmature.h"

class ccbLayer
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbLayer, create);

	ccbLayer();
	virtual ~ccbLayer();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
	void OnShop(cocos2d::CCObject *pSender);
	void OnReturn(cocos2d::CCObject *pSender);

	//virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	//virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	//virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

	void doMove(bool moveLeft);
	void doEnter();
	//virtual void keyBackClicked();
	void SetSpriteDisable(bool m_bVisible);

	void SetTouch(cocos2d::CCNode* sender = NULL);
	void SetDistouch(cocos2d::CCNode* sender = NULL);
	void Effect(cocos2d::CCNode* sender = NULL);
	void SetPosition(cocos2d::CCNode* _pNode, float _x, bool _bCheck = false);
	void FixLocation(float _dt);
	void Back();
	void BottomMove();
	void BottomBack();
	void ButtonMove();
	void ButtonBack();
	void AllMove();
	void AllBack();
	void AllMoveWithBottom();
	void AllBackWithBottom();
	void GoToShop(float _dt);
	void ObjectShow();

	int GetLevel();
	void ShowDifficultName();
	void ShowName();

	void HideShow();
	void HideLevel(cocos2d::CCObject* sender);
	void disappear();

	void ChangeShowModeStart(cocos2d::CCObject *pSender);
	void ChangeShowMode(cocos2d::CCNode* _pNode);
	void ShowBoss(int _index, bool _show);
	void HideAll();
	void Left(cocos2d::CCObject *pSender);
	void Right(cocos2d::CCObject *pSender);
	void BossEnter(cocos2d::CCObject *pSender = NULL);
	void tipdisappear();

	void GuideInit();
	void GuideFingerCreate(int _x, int _y);
	void GuideFingerClear();

	void ShareShow(cocos2d::CCObject *pSender);
	void ShareDisappear(cocos2d::CCObject *pSender = NULL);
	void ShareOK(cocos2d::CCObject *pSender = NULL);
	void ShareCancle();
	void Share1(cocos2d::CCObject *pSender);
	void Share2(cocos2d::CCObject *pSender);
	void Share3(cocos2d::CCObject *pSender);

	void setHandelPos(cocos2d::CCPoint point);

	static ccbLayer* s_pccbLayer;
	bool	m_bShare;
	bool	m_bShareWait;
	bool	m_bReturn;
	int		m_iLevel;
	int		m_iStartTime;
	bool	m_bLevel;
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
private:
	int		m_iAllStar;
	bool	m_bButton;
	int		m_iPress;
	float	m_iLocation;
	int		m_iToward;
	int		m_iTime;
	bool	m_bEnd;
	bool	m_bShop;
	cocos2d::CCSize		m_ccSize;
	cocos2d::CCPoint	m_ccPoint;
	cocos2d::CCPoint	m_ccPointDown;
	cocos2d::CCSprite*	m_pFSprite;
	cocos2d::CCSprite*	m_pSSprite;
	cocos2d::CCSprite*	m_pName;
	cocos2d::CCSprite*	m_pNameChange;
	cocos2d::CCSprite*	m_pNameEffect;
	cocos2d::CCScene*	m_pScene;
	cocos2d::CCSprite*	m_pHideLevel;

	cocos2d::CCNode*	m_pNodeFirst;
	cocos2d::CCNode*	m_pNodeLight;
	
	int m_iShowMode;
	int m_iBossIndex;
	bool m_bCanChangeMode;
	cocos2d::CCNode*	m_pNode;
	cs::Armature* m_pBoss[6];
	cs::Armature* m_pBoss2Head[3];
	cocos2d::CCSprite*	m_pNameBoss[6];
	cocos2d::CCSprite*	m_pBossClose[6];
	cocos2d::CCSprite*	m_pBossInfo[6];
	cocos2d::CCMenu*	m_pMenuChoseBoss;

	int m_iGuideIndex;

	int m_iShare;
	int m_iShareLevel;
	cocos2d::CCMenu*	m_pShareMain;

	int m_bigIndex;
	int m_group;
};

class CCBReader;
class ccbLayerLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbLayerLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbLayer);
};

#endif
