#ifndef _CCB_COMICPLAY_H_
#define _CCB_COMICPLAY_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class ccbComicLayer
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbComicLayer, create);

	ccbComicLayer();
	virtual ~ccbComicLayer();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);

	virtual void keyBackClicked();
	virtual void keyEnterClicked();

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);

	void Next(CCNode* sender);
	void Over(CCNode* sender);

	void Start(float _t);
	static void Audio();
	void StartGuide(float _t);

	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
	static bool m_bCanPress;
private:
	static bool m_bAuidio;
};

class CCBReader;
class ccbComicLayerLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbComicLayerLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbComicLayer);
};

class ComicLayer : public cocos2d::CCLayer
{
public:
	ComicLayer();

	static cocos2d::CCScene* CreateScene(bool _bFirst = true);
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

private:
	ccbComicLayer* m_pNode;
	cocos2d::CCScene* m_pScene;
};

#endif
