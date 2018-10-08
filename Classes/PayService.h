#ifndef _PAYSERVICE_H_

#define _PAYSERVICE_H_
#include "cocos2d.h"
USING_NS_CC;

class PayService : public cocos2d::CCObject{
public:
	PayService();
	static PayService* getInstance();
	static void pay(int id);
	static void PlayLogo();
	void callSuccessFc();
	void callfailFc();
	void callOnCancelExitFc();
	static void exitGame();
	static void moreGame();
	static void setCheckTrue(void);

	static void HideBar();
	static void ShowBar();
	static void OnPause();
	static void OnResume();
	static void UpdataScore(int _score);
	static void TelecomInit();
	static void ShareSend(int _type, int _level);

	static bool m_bIsPay;
private:
	static CCCallFunc* successFc;
	static CCCallFunc* failFc;
	static CCCallFunc* onCancelExitFc;
	static bool showed;
	static PayService* instance;
};

#endif