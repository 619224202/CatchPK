#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "cocos2d.h"

//////////////
/****Menu****/
//////////////
#define d_fParticleY		10.0f
#define d_fLevelStarX		50.0f
#define d_fLevelStarY		-21.0f
#define d_fLevelStarDist	22.0f
#define d_fLevelNumLocate	8, -2
#define d_fLevelPicW		105.5f
#define d_fLevelPicH		53.5f

class CMainMenu : public cocos2d::CCLayer
{
public:
	CMainMenu();
	~CMainMenu();
	CREATE_FUNC(CMainMenu);
	virtual bool init();
	static cocos2d::CCScene* StartMenu(int _level = -1, int _difficult = -1, bool _bShop = false);
	static cocos2d::CCScene* StartGame(int _level, int _difficult);
	static void AllAdd(cocos2d::CCScene* _pScene);

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void keyBackClicked();
	virtual void keyEnterClicked();
	virtual void keyArrowClicked(int arrow);
	virtual void keyNumClicked(int keyNum);

	static cocos2d::CCNode* CreateMenuMain();
	static cocos2d::CCNode* CreateMenuLevel();
	static cocos2d::CCNode*	CreateMenuShop();
	static cocos2d::CCNode* GetMenuMain();
	static cocos2d::CCNode* GetMenuLevel();
	static cocos2d::CCNode*	GetMenuShop();

	static void SetStatus(int _stats);
	static int	GetStatus();
	static void SetDifficult(int _difficult);
	static int	GetDifficult();
	static cocos2d::CCScene* m_pScene;

	static void ReleaseArmatureData();

	static bool m_bShop;
	static bool m_bOnSell;
private:
	static int m_iStatus;
	static int m_iDifficult;
	static int m_iArmatureDigit;

	static cocos2d::CCNode* m_pNodeMain;
	static cocos2d::CCNode* m_pNodeLevel;
	static cocos2d::CCNode* m_pNodeShop;
};

#endif
