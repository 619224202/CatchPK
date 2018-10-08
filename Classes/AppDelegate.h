#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "baseRes.h"

#ifdef GameTypeC
	#define d_iLevelNum		2
#else
	#define d_iLevelNum		5
#endif

enum eTransition
{
	etHorizontal,
	etVertical,
	etInOut,
	etOutIn,
	etFade,
};

class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching();

    virtual void applicationDidEnterBackground();
	virtual void applicationWillEnterForeground();
	static void AudioBackResume();
	static void AudioBackPause();

	static void ChangeScene(cocos2d::CCScene* _pScene, eTransition _transition, float _time);
	static void ChangeScene(cocos2d::CCScene* _pScene);
	static void SaveAll();
	static void LoadAll();
	static void SaveStatus();
	static void SaveLevel(int _level);
	static void SaveAchievement();
	static void SaveAchievement(int _index);
	static void SaveOnSale();
	static void LoadOnSale();
	static void SaveVIP();
	static void SaveHelpMark();
	static void SaveGuide();
	static void SaveInit();
	static void SaveBody();
	static void SaveWeapon();
	static void SaveWeapon(int _index);
	static void SaveBullet(int _index);
	static void SaveShare();
	static void SaveEveryDayItem();
	static void SaveMoney();
	static void SaveMedal();
	static void SaveGetPrize();

	static void AudioInit1();
	static void AudioInit2();
	static void AudioInit3();
	static void AudioPlayBgm(const char* _path, bool _Repeat = true);
	static void AudioStopBgm();
	static void AudioPause();
	static void AudioResume();
	static int AudioPlayEffect(const char* _path);
	static void AudioStopEffect(int _id);
	static void AudioSetVoice();

	static int WeaponIndex(int _num);
	static int WeaponSpeed(int _num);
	static int WeaponDamage(int _num);

	static void DataInit();
	static void DataInit2();

	static void AudioSwitch();
	static void AudioStopB();

	static void SavePayInfo(const char* _tempPayId, int _tempPayIdKey);
	static void DeletePayInfo(const char* _tempPayId, int _tempPayIdKey);
	static void GetPayContent(int _type);
	static void CanclePay( int _type );
	static void SaveTencent();
	static void LoadTencent();


	//static std::string m_sLastBgPath;
	static int s_Tencent;
	static int s_LevelOpen[36];
	static int s_LevelStar[36];
	static int s_LevelPerfect[36];
	static int s_LevelUnfire[36];
	static int s_LevelHero2[36];
	static int s_LevelHero3[36];
	static int s_LevelKey[36];
	static int s_BulletNum[18];
	static int s_WeaponLevel[18];
	static int s_WeaponOwn[18];
	static int s_WeaponUse[3];
	static int s_Achieve[63];
	static int s_AchieveNum[63];
	static int s_Money;
	static int s_Medal;
	static int s_GrenadeNum;
	static int s_VoiceOpen;
	static int s_Healbox;
	static int s_FirstLogin;
	static int s_SkillEnergy[2];
	static int s_DateYear;
	static int s_DateMonth;
	static int s_DateDay;
	static int s_PrizeSave[10];
	static int s_PrizeFree;
	static int s_PrizeDayNum;
	static int s_ShopOnSell[4];
	static int s_KillMonster;
	static long s_imei;

	static int s_OnSaleNum;
	static int s_OnSaleTimes;
	static int s_OnSaleDay;
	static int s_OnSaleHour;

	static int s_VIP;
	static int s_HelpMark;
	static int s_HeroType;

	static int s_Hero2;
	static int s_Hero3;
	static int s_Hero3Array[6];

	static int m_voice;
	static int m_canVoiceResume;
	static int m_Sell1;
	static int m_Sell2;

	static int m_Share;

	static int m_bCanStartGame;

	static int m_iPayInfoMax;
	static int m_iPayInfoIndex;
};

#endif  // __APP_DELEGATE_H__

