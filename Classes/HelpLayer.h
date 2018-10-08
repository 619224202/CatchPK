#ifndef _CCB_HELPLAYER_H_
#define _CCB_HELPLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class ccbHelpLayer
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbHelpLayer, create);

	ccbHelpLayer();
	virtual ~ccbHelpLayer();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager, cocos2d::CCNode* _pNode);

	void On1(cocos2d::CCObject *pSender);
	void On2(cocos2d::CCObject *pSender);
	void On3(cocos2d::CCObject *pSender);
	void On4(cocos2d::CCObject *pSender);
	void On5(cocos2d::CCObject *pSender);
	void On6(cocos2d::CCObject *pSender);
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void Show(cocos2d::CCNode* sender = NULL);
	void OK(cocos2d::CCNode* sender = NULL);
	void END(cocos2d::CCNode* sender = NULL);

	int	m_iHelpType;
private:
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
	cocos2d::CCNode* m_pNode;
};

class CCBReader;
class ccbHelpLayerLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbHelpLayerLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbHelpLayer);
};

#endif
