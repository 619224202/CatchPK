#ifndef _CCB_GETPRIZE_H_
#define _CCB_GETPRIZE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class ccbGetPrizeLayer
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ccbGetPrizeLayer, create);

	ccbGetPrizeLayer();
	virtual ~ccbGetPrizeLayer();

	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);

	void Appear();
	void initPrize(int _index);
	void get(cocos2d::CCNode *pSender);
	void Start(float _t);

	bool m_bOver;
private:
	cocos2d::extension::CCBAnimationManager* m_AnimationManager;
	cocos2d::CCNode* m_pNode;
	int m_iIndex;
};

class CCBReader;
class ccbGetPrizeLayerLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ccbGetPrizeLayerLoader, loader);
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ccbGetPrizeLayer);
};

#endif
