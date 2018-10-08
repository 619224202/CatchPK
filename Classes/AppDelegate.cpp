#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "Flash.h"
#include "baseRes.h"
#include "FileCommon.h"
//#include "ComicPlay.h"
#include "PayService.h"

//#define TEST true
//#define ShowFrame true
#define ShowFrame false

#include "logo.h"
#include "ChoseHero.h"
#include "LevelMenu.h"
#include "GameControler.h"
#include "HttpHandle.h"
#include "GetItemLayer.h"
#include "ShopMenu.h"
#include "LoginEDLayer.h"
#include "GameoverBtnMenu.h"
#include "MainMenu.h"
#include "FirstLogin.h"

#define LevelOpenNum	18

CFlashControl* g_pHero = new CFlashControl;

int AppDelegate::s_LevelOpen[36]= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelStar[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelPerfect[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelUnfire[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelHero2[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelHero3[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelKey[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_BulletNum[18]= {0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_WeaponLevel[18]	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_WeaponOwn[18]	= {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_WeaponUse[3]		=  {2,3,0};
int AppDelegate::s_Achieve[63]		= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,0};
int AppDelegate::s_AchieveNum[63]	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,0};
int AppDelegate::s_Money		= 2000;
int AppDelegate::s_Medal		= 0;
int AppDelegate::s_GrenadeNum	= 0;
int AppDelegate::s_VoiceOpen	= 1;
int AppDelegate::s_Healbox		= 3;
int AppDelegate::s_FirstLogin	= 0;
int AppDelegate::s_SkillEnergy[2]= {0, 0};
int AppDelegate::s_DateYear	= 0;
int AppDelegate::s_DateMonth= 0;
int AppDelegate::s_DateDay	= 0;
int AppDelegate::s_PrizeSave[10] = {0,0,0,0,0,0,0,0,0,0};
//static std::string m_sLastBgPath= "";
int AppDelegate::s_PrizeFree = 0;
int AppDelegate::s_PrizeDayNum = 0;
int AppDelegate::s_ShopOnSell[4] = {0,0,0,0};
int AppDelegate::s_KillMonster = 0;
long AppDelegate::s_imei	= 0;

int AppDelegate::s_OnSaleNum = 0;
int AppDelegate::s_OnSaleTimes = 0;
int AppDelegate::s_OnSaleDay = 0;
int AppDelegate::s_OnSaleHour = 0;

int AppDelegate::s_VIP = 0;
int AppDelegate::s_HelpMark = 0;
int AppDelegate::s_HeroType = 0;

int AppDelegate::s_Hero2 = 3;
int AppDelegate::s_Hero3 = 6;
int AppDelegate::s_Hero3Array[6] = {0,0,0,0,0,0,};

int AppDelegate::m_voice = 50;
int AppDelegate::m_canVoiceResume = 0;
int AppDelegate::m_Sell1 = 0;
int AppDelegate::m_Sell2 = 0;

int AppDelegate::m_Share = 0;

int AppDelegate::m_bCanStartGame = 0;

int AppDelegate::m_iPayInfoIndex = 0;
int AppDelegate::m_iPayInfoMax = 0;

int AppDelegate::s_Tencent = 0;

using namespace CocosDenshion;
USING_NS_CC;

AppDelegate::AppDelegate()
{
	SimpleAudioEngine::sharedEngine()->preloadEffect("SoundEffect/zmanhua1.mp3");
	s_WeaponUse[0] = 2;
	s_WeaponUse[1] = 3;
	s_WeaponUse[2] = -1;
	s_WeaponOwn[2] = 1;
	s_WeaponOwn[3] = 1;
	s_BulletNum[2] = 100;
	s_BulletNum[3] = 100;
#ifdef All_Level_Open
	for ( int i = 0; i < LevelLimit*6; i++ )
#else
	for ( int i = 0; i < 1; i++ )
#endif
	{
		s_LevelOpen[i] = 1;
		s_LevelStar[i] = 0;
	}
#ifdef Plat_Telecom_Demo
	s_Money = 4999999;
	s_Medal = 89999;
#endif
}

AppDelegate::~AppDelegate()
{
	//SaveAll();
	CHttpHandle::DestroyInstance();
	SimpleAudioEngine::end();
	//SAFEDELETEARRAY(s_LevelOpen);
	//SAFEDELETEARRAY(s_LevelStar);
	//SAFEDELETEARRAY(s_BulletNum);
	//SAFEDELETEARRAY(s_WeaponLevel);
	//SAFEDELETEARRAY(s_WeaponOwn);
	//SAFEDELETEARRAY(s_WeaponUse);
	//SAFEDELETEARRAY(s_Achieve);
	//SAFEDELETEARRAY(s_AchieveNum);
}

void AppDelegate::SaveTencent()
{
	common::SaveData("_st", 1);
}

void AppDelegate::LoadTencent()
{
	common::LoadData("_st", s_Tencent);
}

void AppDelegate::SaveOnSale()
{
	common::SaveData("_a", s_OnSaleNum);//s_Money
	common::SaveData("_b", s_OnSaleTimes);//s_GrenadeNum
	common::SaveData("_c", s_OnSaleDay);//s_VoiceOpen
	common::SaveData("_d", s_OnSaleHour);//s_Healbox
	SaveArray("y", s_ShopOnSell, 4);//s_LevelStar
}

void AppDelegate::LoadOnSale()
{
	common::LoadData("_a", s_OnSaleNum);//s_Money
	common::LoadData("_b", s_OnSaleTimes);//s_GrenadeNum
	common::LoadData("_c", s_OnSaleDay);//s_VoiceOpen
	common::LoadData("_d", s_OnSaleHour);//s_Healbox
	LoadArray("y", s_ShopOnSell, 4);//s_LevelStar
}

void AppDelegate::SaveMoney()
{
	common::SaveData("a", s_Money);//s_Money
}

void AppDelegate::SaveMedal()
{
	common::SaveData("b", s_Medal);//s_Medal
}

void AppDelegate::SaveStatus()
{
	common::SaveData("a", s_Money);//s_Money
	common::SaveData("b", s_Medal);//s_Medal
	common::SaveData("c", s_GrenadeNum);//s_GrenadeNum
	common::SaveData("d", s_VoiceOpen);//s_VoiceOpen
	common::SaveData("e", s_Healbox);//s_Healbox
	SaveArray("o", s_SkillEnergy, 1);//s_SkillEnergy
	SaveArray("l", s_WeaponUse, 2);//s_WeaponUse
	common::SaveData("j", s_WeaponLevel, s_WeaponUse[0]);//s_WeaponLevel
	common::SaveData("k", s_WeaponOwn, s_WeaponUse[0]);//s_WeaponOwn
	common::SaveData("j", s_WeaponLevel, s_WeaponUse[1]);//s_WeaponLevel
	common::SaveData("k", s_WeaponOwn, s_WeaponUse[1]);//s_WeaponOwn
	SaveArray("i", s_BulletNum, 18);//s_BulletNum
	common::SaveData("z", s_KillMonster);//s_Healbox
	common::SaveData("ba", s_Hero2);//s_Healbox
	common::SaveData("bb", s_Hero3);//s_Healbox
	SaveArray("bc", s_Hero3Array, 6);//s_LevelStar
	common::SaveData("_s1", m_Sell1);//s_Money
	common::SaveData("_s2", m_Sell2);//s_Money
}

void AppDelegate::SaveBody()
{
	common::SaveData("bb", s_Hero3);//s_Healbox
	SaveArray("bc", s_Hero3Array, 6);//s_LevelStar
}
void AppDelegate::SaveLevel( int _level )
{
	//common::SaveData("g", s_LevelOpen, _level);//s_LevelOpen
	common::SaveData("g", s_LevelOpen, _level+1);//s_LevelOpen
	common::SaveData("h", s_LevelStar, _level);//s_LevelStar
	common::SaveData("v", s_LevelPerfect, _level);//s_LevelOpen
	common::SaveData("w", s_LevelUnfire, _level);//s_LevelStar
	common::SaveData("x", s_LevelKey, _level);//s_LevelStar
	common::SaveData("bd", s_LevelHero2, _level);//s_LevelOpen
	common::SaveData("be", s_LevelHero3, _level);//s_LevelStar
}

void AppDelegate::SaveAchievement()
{
	SaveArray("n", s_AchieveNum, 63);//s_AchieveNum
}

void AppDelegate::SaveAchievement( int _index )
{
	common::SaveData("m", s_Achieve, _index);//s_LevelOpen
	common::SaveData("n", s_AchieveNum, _index);//s_LevelStar
	common::SaveData("b", s_Medal);//s_Medal
}

void AppDelegate::SaveWeapon()
{
	SaveArray("i", s_BulletNum, 18);//s_BulletNum
	SaveArray("j", s_WeaponLevel, 18);//s_WeaponLevel
	SaveArray("k", s_WeaponOwn, 18);//s_WeaponOwn
}

void AppDelegate::SaveWeapon( int _index )
{
	common::SaveData("j", s_WeaponLevel, _index);//s_WeaponLevel
	common::SaveData("k", s_WeaponOwn, _index);//s_WeaponOwn
	common::SaveData("a", s_Money);//s_Money
}

void AppDelegate::SaveBullet( int _index )
{
	common::SaveData("i", s_BulletNum, _index);//s_LevelOpen
	common::SaveData("a", s_Money);//s_Money
}

void AppDelegate::SaveInit()
{
	SaveArray("l", s_WeaponUse, 2);//s_WeaponUse
	AppDelegate::SaveStatus();
#ifdef All_Level_Open
	for ( int i = 0; i < 24; i++ )
#else
	for ( int i = 0; i < 1; i++ )
#endif
	{
		AppDelegate::SaveLevel(i);
	}
	//common::SaveData("a", s_Money);//s_Money
	//common::SaveData("b", s_Medal);//s_Medal
	//common::SaveData("c", s_GrenadeNum);//s_GrenadeNum
	//common::SaveData("e", s_Healbox);//s_Healbox
	//SaveArray("k", s_WeaponOwn, 4);//s_WeaponOwn
	//SaveArray("g", s_LevelOpen, 2);//s_LevelOpen
	//SaveArray("i", s_BulletNum, 3);//s_BulletNum
	//common::SaveData("ba", s_Hero2);//s_Healbox
	//common::SaveData("bb", s_Hero3);//s_Healbox
}

void AppDelegate::SaveEveryDayItem()
{
	common::SaveData("p", s_DateYear);//
	common::SaveData("q", s_DateMonth);//
	common::SaveData("r", s_DateDay);//
	SaveArray("s", s_PrizeSave, 10);//
	common::SaveData("t", s_PrizeFree);//
	common::SaveData("u", s_PrizeDayNum);//
}

void AppDelegate::SaveAll()
{
	common::SaveData("a", s_Money);//s_Money
	common::SaveData("b", s_Medal);//s_Medal
	common::SaveData("c", s_GrenadeNum);//s_GrenadeNum
	common::SaveData("d", s_VoiceOpen);//s_VoiceOpen
	common::SaveData("e", s_Healbox);//s_Healbox
	SaveArray("g", s_LevelOpen, 36);//s_LevelOpen
	CCLog("saveLvlOPen:%d",s_LevelOpen[0]);
	SaveArray("h", s_LevelStar, 36);//s_LevelStar
	SaveArray("i", s_BulletNum, 18);//s_BulletNum
	SaveArray("j", s_WeaponLevel, 18);//s_WeaponLevel
	SaveArray("k", s_WeaponOwn, 18);//s_WeaponOwn
	SaveArray("l", s_WeaponUse, 3);//s_WeaponUse
	SaveArray("m", s_Achieve, 63);//s_Achieve
	SaveArray("n", s_AchieveNum, 63);//s_AchieveNum
	SaveArray("o", s_SkillEnergy, 2);//s_SkillEnergy
	common::SaveData("p", s_DateYear);//
	common::SaveData("q", s_DateMonth);//
	common::SaveData("r", s_DateDay);//
	SaveArray("s", s_PrizeSave, 10);//
	common::SaveData("t", s_PrizeFree);//
	common::SaveData("u", s_PrizeDayNum);//
	SaveArray("v", s_LevelPerfect, LevelOpenNum);//s_LevelOpen
	SaveArray("w", s_LevelUnfire, LevelOpenNum);//s_LevelStar
	SaveArray("x", s_LevelKey, LevelOpenNum);//s_LevelStar
	common::SaveData("z", s_KillMonster);//s_Healbox
	common::SaveData("aa", s_imei);//s_Healbox
	common::SaveData("_g", s_HeroType);//s_Healbox
	common::SaveData("ba", s_Hero2);//s_Healbox
	common::SaveData("bb", s_Hero3);//s_Healbox
	SaveArray("bc", s_Hero3Array, 6);//s_LevelStar
	SaveArray("bd", s_LevelHero2, LevelOpenNum);//s_LevelOpen
	SaveArray("be", s_LevelHero3, LevelOpenNum);//s_LevelStar
	common::SaveData("_s1", m_Sell1);//s_Money
	common::SaveData("_s2", m_Sell2);//s_Money
}

void AppDelegate::SaveGuide()
{
	common::SaveData("f", s_FirstLogin);//s_FirstLogin
}

void AppDelegate::SaveShare()
{
	common::SaveData("_s3", m_Share);//s_Money
}

void AppDelegate::SaveGetPrize()
{
	common::SaveData("t", s_PrizeFree);//
}

void AppDelegate::LoadAll()
{
	common::LoadData("a", s_Money);
	common::LoadData("b", s_Medal);
	common::LoadData("c", s_GrenadeNum);
	common::LoadData("d", s_VoiceOpen);
	common::LoadData("e", s_Healbox);
	LoadArray("g", s_LevelOpen, 36);
	CCLog("loadLvlOPen:%d",s_LevelOpen[0]);
	LoadArray("h", s_LevelStar, 36);
	LoadArray("i", s_BulletNum, 18);
	LoadArray("j", s_WeaponLevel, 18);
	LoadArray("k", s_WeaponOwn, 18);
	LoadArray("l", s_WeaponUse, 3);
	LoadArray("m", s_Achieve, 63);
	LoadArray("n", s_AchieveNum, 63);
	LoadArray("o", s_SkillEnergy, 1);
	common::LoadData("p", s_DateYear);//
	common::LoadData("q", s_DateMonth);//
	common::LoadData("r", s_DateDay);//
	LoadArray("s", s_PrizeSave, 10);//
	common::LoadData("t", s_PrizeFree);//
	common::LoadData("u", s_PrizeDayNum);//
	LoadArray("v", s_LevelPerfect, LevelOpenNum);//s_LevelOpen
	LoadArray("w", s_LevelUnfire, LevelOpenNum);//s_LevelStar
	LoadArray("x", s_LevelKey, LevelOpenNum);//s_LevelStar
	common::LoadData("z", s_KillMonster);//s_Healbox
	common::SaveData("aa", s_imei);//s_Healbox
	common::LoadData("_e", s_VIP);//s_Healbox
	common::LoadData("_f", s_HelpMark);//s_Healbox
	common::LoadData("_g", s_HeroType);//s_Healbox
	common::LoadData("ba", s_Hero2);//s_Healbox
	common::LoadData("bb", s_Hero3);//s_Healbox
	LoadArray("bc", s_Hero3Array, 6);//s_LevelStar
	LoadArray("bd", s_LevelHero2, LevelOpenNum);//s_LevelOpen
	LoadArray("be", s_LevelHero3, LevelOpenNum);//s_LevelStar
	common::LoadData("_s1", m_Sell1);//s_Money
	common::LoadData("_s2", m_Sell2);//s_Money
	common::LoadData("_s3", m_Share);//s_Money
	common::LoadData("_fa", m_iPayInfoIndex);//s_Money
	common::LoadData("_fb", m_iPayInfoMax);//s_Money
	
}

bool AppDelegate::applicationDidFinishLaunching()
{
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(eglView);
	pDirector->setDisplayStats(true);
    // turn on display FPS
    //pDirector->setDisplayStats(ShowFrame);
    pDirector->setAnimationInterval(1.0 / 60);

	CCSize frameSize = eglView->getFrameSize();
	
	float sw = frameSize.width;
	float sh = frameSize.height;
	CCLog("sw=%f,sh=%f",sw,sh);
	//eglView->setFrameSize(800,480);
//#ifdef TEST
	eglView->setFrameSize(1280,720);
//#endif
	eglView->setDesignResolutionSize(sw,sh, kResolutionExactFit);
    // create a scene. it's an autorelease object
	//CCScene *pScene = ccbLoginMenu::LoginScene();

#ifdef TEST
	//LoadAll();
	//CCScene *pScene = CFirstLogin::CreateScene();
	//CCScene *pScene = logo::Create();
	CCScene* pScene = CMainMenu::StartGame(0, 0);
#else
#ifdef GameTypeA
		common::LoadData("f", s_FirstLogin);
		CCScene *pScene = logo::Create();
#endif

#ifdef GameTypeB
		CCScene *pScene = CFirstLogin::CreateScene();
#endif
#endif
	pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	//CCTextureCache::sharedTextureCache()->removeAllTextures();

	/*{
	CCDirector::sharedDirector()->pause();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	m_voice = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
	PayService::getInstance()->setCheckTrue();
	}*/

	//m_canVoiceResume = 0;
	//PayService::OnPause();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	//CCTextureCache::sharedTextureCache()->reloadAllTextures();
	//CCDirector::sharedDirector()->resume();
	//if ( CGameControler::GetGameControler() )
	//	CGameControler::GetGameControler()->PauseGame();
	//m_canVoiceResume++;
	//PayService::OnResume();
	//SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	//SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	//SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_voice);
}

void AppDelegate::AudioBackResume()
{
	m_canVoiceResume++;
	if ( m_canVoiceResume >= 2 )
	{
		CCTextureCache::sharedTextureCache()->reloadAllTextures();
		m_canVoiceResume = 0;
		//CCDirector::sharedDirector()->resume();
		CCDirector::sharedDirector()->startAnimation();
		if ( CGameControler::GetGameControler() )
			CGameControler::GetGameControler()->PauseGame(1);
		else
		{
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_voice);
	}
}

void AppDelegate::AudioBackPause()
{
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	PayService::getInstance()->setCheckTrue();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	m_voice = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
	m_canVoiceResume = 0;
	//CCDirector::sharedDirector()->pause();
	CCDirector::sharedDirector()->stopAnimation();
}

void AppDelegate::ChangeScene(cocos2d::CCScene* _pScene, eTransition _transition, float _time)
{
	CCDirector::sharedDirector()->getRunningScene()->stopAllActions();
	CCDirector::sharedDirector()->getRunningScene()->pauseSchedulerAndActions();
	CCScene* pScene = NULL;
	switch ( _transition )
	{
	case etHorizontal:
		pScene = CCTransitionProgressHorizontal::create(_time, _pScene);
		break;
	case etVertical:
		pScene = CCTransitionProgressVertical::create(_time, _pScene);
		break;
	case etOutIn:
		pScene = CCTransitionProgressInOut::create(_time, _pScene);
		break;
	case etFade:
		pScene = CCTransitionFade::create(_time, _pScene);
		break;
	default:
		break;
	}
	if ( pScene )
	{
		//SetScale(_pScene);
		CCDirector::sharedDirector()->replaceScene(_pScene);
	}
}

void AppDelegate::ChangeScene(cocos2d::CCScene* _pScene)
{
	if ( _pScene )
	{
		CCDirector::sharedDirector()->getRunningScene()->stopAllActions();
		CCDirector::sharedDirector()->getRunningScene()->pauseSchedulerAndActions();
		CCDirector::sharedDirector()->replaceScene(_pScene);
	}
}

void AppDelegate::AudioInit1()
{
	//SimpleAudioEngine::end();
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/mainmenu/mainmenu.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level1/1.mp3");
#ifndef GameTypeC
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level1/2.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level2/1.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level2/2.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/mainmenu/ShopAchieve.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/mainmenu/LevelChose.mp3");	
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS2/moreIndex.mp3");
#endif

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfUpgrade.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfBuyWeapon.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/Lose.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/EfWinBG.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfDrops.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfGas.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfCut.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfMonsterBeCutted.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfObjectBeCutted.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfHeroDead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfBloodLess.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfJump.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfGetItem.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfGetBullet.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfBeAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfBeAttacked2.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfRock.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/ShellDrop.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/grenade.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/3.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/4.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/5.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/7.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/8.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/9.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/10.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/11.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/standby.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/attack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/dead.mp3");
	////SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/skillattack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/beAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/dog/standby.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/dog/attack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/dog/dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/dog/beAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/female/standby.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/female/attack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/female/dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/female/beAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/hawk/attack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/hawk/dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/hawk/beAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/standby.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/attack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/beAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/chopper.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/buy.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/firegun.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/getKey.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/grenadegun.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/heal.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/opendoor.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/rankPress.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/wallbroken.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/1_10Dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/4BeAttack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/4Dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/BOSS1dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/BOSS2dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/BoxBroken.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/WeaponTypeChange.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/star.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/NPCTalk.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/getAchieve.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/1BeAttack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/1Dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/3BeAttack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/5Cut.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/8Cut.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/noBullet.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/numadd.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/skillcontinue.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/skillstart.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/bodybuy.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/bodydown.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/bodyscan.mp3");
}

void AppDelegate::AudioInit2()
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level2/1.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfDrops.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfCut.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfMonsterBeCutted.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfObjectBeCutted.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfBloodLess.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfJump.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfGetItem.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfGetBullet.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfBeAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfBeAttacked2.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/EfRock.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Sound/ShellDrop.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/grenade.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/3.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Weapon/9.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/standby.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/attack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/dead.mp3");
	////SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/skillattack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/boss1/beAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/standby.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/attack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/beAttacked.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS/Monster/male/chopper.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS2/heal.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/BOSS1dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/BoxBroken.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/WeaponTypeChange.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/star.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS3/NPCTalk.mp3");

	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/1BeAttack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/1Dead.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/3BeAttack.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/5Cut.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/noBullet.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/numadd.mp3");
	//SimpleAudioEngine::sharedEngine()->preloadEffect("MS4/skillstart.mp3");
}

void AppDelegate::AudioInit3()
{
}

void AppDelegate::AudioPlayBgm(const char* _path, bool _Repeat)
{
	if ( s_VoiceOpen )
	{
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(100);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(_path, _Repeat);
	}
}

void AppDelegate::AudioStopBgm()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
}

void AppDelegate::AudioStopB()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void AppDelegate::AudioPause()
{
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::AudioResume()
{
	if ( AppDelegate::s_VoiceOpen )
	{
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
}

int AppDelegate::AudioPlayEffect(const char* _path)
{
	int id = -1;
	if ( s_VoiceOpen )
	{
		id = SimpleAudioEngine::sharedEngine()->playEffect(_path);
	}
	return id;
}

void AppDelegate::AudioStopEffect( int _id )
{
	//if ( _id >= 0 )
	//	SimpleAudioEngine::sharedEngine()->stopEffect(_id);
}

int AppDelegate::WeaponIndex( int _num )
{
	switch ( _num )
	{
	case 3:
		return 0;
	case 4:
		return 1;
	case 5:
		return 2;
	case 7:
		return 3;
	case 8:
		return 4;
	case 9:
		return 5;
	case 10:
		return 6;
	case 11:
		return 7;
	case 12:
		return 8;
	case 16:
		return 9;
	case 17:
		return 10;
	case 2:
		return 20;
	case 6:
		return 21;
	case 13:
		return 22;
	case 14:
#ifdef Old_Version
		return 20;
#else
		return 23;
#endif
	case 15:
		return 30;
	}
	return -1;
}

int AppDelegate::WeaponSpeed( int _num )
{
	return g_iWeaponCooltime[_num] - g_iWeaponUpgradeS[_num]*AppDelegate::s_WeaponLevel[_num];
}

int AppDelegate::WeaponDamage( int _num )
{
	return g_iWeaponDamage[_num] + g_iWeaponUpgradeD[_num]*AppDelegate::s_WeaponLevel[_num];
}

void AppDelegate::DataInit()
{
	PayService::getInstance();
	if ( AppDelegate::s_FirstLogin == 0 )
	{
		AppDelegate::SaveInit();
		char buffer[255];
		sprintf(buffer, "NP%.4d", abs(s_imei%10000));
		cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("name", buffer);
		cocos2d::CCUserDefault::sharedUserDefault()->flush();
	}
}

void AppDelegate::DataInit2()
{
	PayService::getInstance();
	if ( AppDelegate::s_FirstLogin == 0 )
	{
		AppDelegate::SaveInit();
		char buffer[255];
		sprintf(buffer, "NP%.4d", abs(s_imei%10000));
		cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("name", buffer);
		cocos2d::CCUserDefault::sharedUserDefault()->flush();
	}
	else
		AppDelegate::LoadAll();
}

void AppDelegate::SaveVIP()
{
	common::SaveData("_e", s_VIP);//s_Healbox
}

void AppDelegate::SaveHelpMark()
{
	common::SaveData("_f", s_HelpMark);//s_Healbox
}

void AppDelegate::AudioSwitch()
{
	AppDelegate::s_VoiceOpen = 1 - AppDelegate::s_VoiceOpen;
	AudioSetVoice();
}

void AppDelegate::AudioSetVoice()
{
	if ( AppDelegate::s_VoiceOpen == 0 )
	{
		m_voice = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_voice);
	}
}

void AppDelegate::SavePayInfo( const char* _tempPayId, int _tempPayType )
{
	if ( m_iPayInfoIndex > m_iPayInfoMax )
		m_iPayInfoIndex = m_iPayInfoMax;
	common::SaveData("_fc", _tempPayId, m_iPayInfoIndex);
	common::SaveData("_fd", _tempPayType, m_iPayInfoIndex);

	if ( m_iPayInfoIndex >= m_iPayInfoMax )
	{
		m_iPayInfoIndex++;
		m_iPayInfoMax++;
	}

	common::SaveData("_fa", m_iPayInfoIndex);//s_Money
	common::SaveData("_fb", m_iPayInfoMax);//s_Money
}

void AppDelegate::DeletePayInfo( const char* _tempPayId, int _tempPayIdKey )
{
	int index = -1;
	for ( int i = 0; i < m_iPayInfoMax; i++ )
	{
		std::string getPayID;
		common::LoadData("_fc", getPayID, i);
		std::string nowPayID = _tempPayId;
		if ( getPayID == "" )
		{
			m_iPayInfoIndex = i;
		}
		else if ( strcmp(getPayID.c_str(), nowPayID.c_str()) )
		{
			m_iPayInfoIndex = i;
			index = i;
			break;
		}
	}
	if ( m_iPayInfoIndex + 1 == m_iPayInfoMax )
	{
		m_iPayInfoMax = m_iPayInfoIndex;
	}
	if ( index >= 0 )
	{
		int type = 0;
		common::LoadData("_fd", type, m_iPayInfoIndex);
		GetPayContent(type);
		common::SaveData("_fc", "", index);
		common::SaveData("_fd", -1, index);
	}
}

void AppDelegate::GetPayContent( int _type )
{//计费内容
	if ( _type < 0 )
		return;
	if ( _type > 17 )
		return;
	switch (_type)
	{
	case 1:
		{
			AppDelegate::m_Sell1		= 1;
			AppDelegate::s_WeaponOwn[6] = 1;
			AppDelegate::s_WeaponOwn[9] = 1;
			if ( AppDelegate::s_HeroType == 0 )
			{
				AppDelegate::s_WeaponUse[0] = 6;
				AppDelegate::s_WeaponUse[1] = 9;
			}
			AppDelegate::s_BulletNum[9] += 100;
			if ( AppDelegate::s_BulletNum[9] > 999 )
				AppDelegate::s_BulletNum[9] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->GuideBuy();
		}
		break;
	case 2:
		{
			AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
			if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
				AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
			AppDelegate::m_Sell1 = 1;
			int iSkillCanUse = 5;
			if ( AppDelegate::s_SkillEnergy[0] < 100 )
			{
				iSkillCanUse = 4;
				AppDelegate::s_SkillEnergy[0] = 100;
			}
			AppDelegate::m_Sell2 = iSkillCanUse;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_Money += 2000;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell2Resume();
		}
		break;
	case 3:
		{
			AppDelegate::m_Sell1 = 1;
			AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
			if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
				AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell3Resume();
		}
		break;
	case 4:
		{
			AppDelegate::m_Sell1		= 1;
			AppDelegate::s_WeaponOwn[6] = 1;
			AppDelegate::s_WeaponOwn[9] = 1;
			if ( AppDelegate::s_HeroType == 0 )
			{
				AppDelegate::s_WeaponUse[0] = 6;
				AppDelegate::s_WeaponUse[1] = 9;
			}
			AppDelegate::s_BulletNum[9] += 100;
			if ( AppDelegate::s_BulletNum[9] > 999 )
				AppDelegate::s_BulletNum[9] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveStatus();
			if ( ccbLevelMenu::s_pccbLevelMenu )
				ccbLevelMenu::s_pccbLevelMenu->Sell1Resume();
			if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu )
				ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->Sell1Resume();
		}
		break;
	case 5:
		{
			if ( ccbChoseHero::s_pccbChoseHero )
				ccbChoseHero::s_pccbChoseHero->OnBuy3Start();
		}
		break;
	case 6:
		{
			AppDelegate::s_Hero3 = 0;
			AppDelegate::s_WeaponOwn[15] = 1;
			if ( AppDelegate::s_WeaponOwn[8] == 0 )
			{
				AppDelegate::s_WeaponOwn[8] = 1;
				AppDelegate::s_BulletNum[8] = 100;
			}
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveStatus();
			AppDelegate::SaveWeapon(15);
			AppDelegate::SaveWeapon(8);
			AppDelegate::SaveBullet(8);
			if ( ccbChoseHero::s_pccbChoseHero )
				ccbChoseHero::s_pccbChoseHero->OnBuy3PaySwitch();
		}
		break;
	case 7:
		{
			AppDelegate::s_Hero2 = 0;
			AppDelegate::s_WeaponOwn[14] = 1;
			AppDelegate::s_WeaponOwn[17] = 1;
			AppDelegate::s_BulletNum[17] = 135;
			AppDelegate::s_LevelOpen[30] = 1;
			AppDelegate::SaveStatus();
			AppDelegate::SaveWeapon(14);
			AppDelegate::SaveWeapon(17);
			AppDelegate::SaveLevel(30);
			if ( ccbChoseHero::s_pccbChoseHero )
				ccbChoseHero::s_pccbChoseHero->OnBuy2Buy();
		}
		break;
	case 8:
		{
			AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += 3*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
			if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
				AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum += 5;
			if ( AppDelegate::s_GrenadeNum > 10 )
				AppDelegate::s_GrenadeNum = 10;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell4Resume();
		}
		break;
	case 9:
		{
			int iSkillCanUse = 5;
			if ( AppDelegate::s_SkillEnergy[0] < 100 )
			{
				iSkillCanUse = 4;
				AppDelegate::s_SkillEnergy[0] = 100;
			}
			AppDelegate::m_Sell2 = iSkillCanUse;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell5Resume();
		}
		break;
	case 10:
		{
			AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
			if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
				AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell3Resume1();
		}
		break;
	case 11:
		{
			AppDelegate::s_VIP = 1;
			AppDelegate::SaveVIP();
			if ( ccbGetItemLayer::s_pccbGetItemLayer )
				ccbGetItemLayer::s_pccbGetItemLayer->OnBuy1();
		}
		break;
	case 12:
		{
			AppDelegate::s_Medal += 100;
			AppDelegate::SaveMedal();
		}
		break;
	case 13:
		{
			AppDelegate::s_Medal += 200;
			AppDelegate::SaveMedal();
		}
		break;
	case 14:
		{
			AppDelegate::s_Medal += 2000;
			AppDelegate::SaveMedal();
		}
		break;
	case 15:
		{
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveMoney();
		}
		break;
	case 16:
		{
			AppDelegate::s_Money += 2000;
			AppDelegate::SaveMoney();
		}
		break;
	case 17:
		{
			AppDelegate::s_Money += 20000;
			AppDelegate::SaveMoney();
		}
		break;
	}
	if ( _type > 11 )
	{
		if ( ccbGetItemLayer::s_pccbGetItemLayer )
			ccbGetItemLayer::s_pccbGetItemLayer->GetRemove();
		else if ( ccbLoginEDLayer::s_pccbLoginEDLayer )
		{
			ccbLoginEDLayer::s_pccbLoginEDLayer->GetRemove();
		}
		else if ( ccbShopMenu::s_pccbShopMenu )
		{
			ccbShopMenu::s_pccbShopMenu->MoneyShow();
			ccbShopMenu::s_pccbShopMenu->GetRemove();
		}
		else if ( CGameControler::GetGameControler() )
			CGameControler::GetGameControler()->GetRemove();
	}
	PayService::m_bIsPay = false;
}

void AppDelegate::CanclePay( int _type )
{
	PayService::m_bIsPay = false;
}
