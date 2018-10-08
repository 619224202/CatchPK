#include "cocos2d.h"

class logo : public cocos2d::CCScene
{
public:
	virtual bool init();
	static cocos2d::CCScene* Create();
	cocos2d::CCScene* CreateScene();
	void time(float _t);

	static void logoOver();

	void StartGame(float _t);
	void StartGame_WIN32(float _t);
	void StartGame_Telecom(float _t);

	static cocos2d::CCScene* s_pScene;
};
