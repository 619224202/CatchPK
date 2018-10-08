#include "ShopMenu.h"
#include "MainMenu.h"
#include "LevelMenu.h"
#include "baseRes.h"
#include "ccb.h"
#include "AppDelegate.h"
#include "Flash.h"
#include "CSArmatureDataManager.h"
#include "Common.h"
#include "LoginMenu.h"
#include "FirstLogin.h"
#include "PayService.h"

extern CFlashControl* g_pHero;

using namespace cs;
using namespace cocos2d;
using namespace cocos2d::extension;

#define d_iNumShowTime		30

#define d_fShopHeroLocateX	159.0f
#define d_fShopHeroLocateY	28.0f
#define d_fShopItemLocateX	200.0f
#define d_fShopItemLocateY	229.0f
#define d_fShopItemWeight	100.0f
#define d_fShopItemHeight	50.0f
#define d_fShopItemDistX	240.0f
#define d_fShopItemDistY	130.0f
#define d_fShopBottomY		52.0f
#define d_fShopBottomX		200.0f
#define d_iShopBottomDist	105

#define d_fNumLocateGold			27, 17
#define d_fNumLocateMedal			27, 17
#define d_fOwnLocate				183, 99
#define d_fOnSellLocate				433, 266
#define d_fShopWeaponLocateGold		67, 20
#define d_fShopWeaponLocateDamage	167, 20
#define d_fShopWeaponLocateWeapon	121, 78
#define d_fShopWeaponLocateName		33, 104
#define d_fShopWeaponLocateUse1		111, 39
#define d_fShopWeaponLocateUse2		233, 39
#define d_fShopWeaponLocateUse3		-352, -39
#define d_iShopWeaponTagDamage	799
#define d_iShopWeaponTagGold	798
#define d_iShopWeaponHaveBuy	797
#define d_iShopWeaponTagUse		725
#define d_fShopWeaponLocateOnOff	413, 270
#define d_fShopWeaponLocateBuy		264, 50
#define d_fShopWeaponLocateUpgrade	135, 49
#define d_fShopShowNameLocate		116, 222
#define d_fShopShowWeaponLocate		220, 170
#define d_fShopShowStarLocate(dist)	105+45*dist, 106
#define d_fShopShowNumLocate		120, 103
#define d_fShopShowNumLocate2		163, 90
#define d_fShopShowCostPic			405, 174
#define d_fShopShowLocateDamage		405, 104
#define d_fShopShowLocateSpeed		405, 139
#define d_fShopShowLocatePower		405, 69
#define d_fShopShowLocateBullet1	405, 209
#define d_fShopShowLocateBullet2	370, 212
#define d_fShopShowBoardNumLocate1	68, 10
#define d_fShopShowBoardNumLocate2	68, 6
//#define d_fShopShowCostNum			46, 10
#define d_iShopShowTagNum		796
#define d_iShopShowTagName		700
#define d_iShopShowTagWeapon	675
#define d_iShopShowTagStar		650
#define d_iShopOnReduction		625

#define d_LocationStarParticle	19, 17
#define Size_WeaponUpgrade		222, 159

#ifndef GameTypeC
#define WeaponNum			18
#define d_iIndexMax			6
static int weapon_index[] =
{
    0, 1, 2, 14, 15, 6, 13, 3, 4, 5, 7, 17, 8, 9, 10, 11, 16, 12,
};
#else
#define WeaponNum			16
#define d_iIndexMax			5
static int weapon_index[] =
{
    0, 1, 2, 14, 15, 6, 13, 3, 4, 5, 7, 8, 9, 10, 11, 12,
};
#endif
static int weapon_power[] =
{
    0, 300, 100, 120, 200, 240, 400, 320, 360, 400, 450, 450, 500, 620, 600, 150, 550, 350,
};
static int weapon_upgrade[] =
{
    0, 0, 30, 48, 60, 96, 160, 144, 108, 120, 135, 135, 150, 186, 240, 45, 220, 140,
};

ccbShopMenu* ccbShopMenu::s_pccbShopMenu = NULL;

ccbShopMenu::ccbShopMenu()
: m_AnimationManager(NULL)
{
    m_bGuideClose	= true;
    m_bOnSell		= false;
    m_iGuideTime	= 10;
    m_iGuideIndex	= 0;
    m_iGuideTalk	= 0;
    m_iIndex		= 0;
    m_iTime			= 0;
    m_iPress		= 0;
    m_iShowWeapon	= 0;
    m_iAttackCoolTime= 0;
    m_bIn			= false;
    m_bWeaponChange	= false;
    m_bShowAction	= false;
    m_pFireSprite	= NULL;
    m_pFireObject	= NULL;
    m_iActoinTime	= 0;
    m_bNewShowWeapon= true;

	w_butIndex = 0;
	w_goodsPage = 0;
	w_groupIndex = 0;
	w_goodIndex = 0;
	isInInfo = false;
	isInRecharge = false;

	w_butPos[0][0] = 340;
	w_butPos[0][1] = 157;
	w_butPos[1][0] = 255;
	w_butPos[1][1] = -5;
	w_butPos[2][0] = 377;
	w_butPos[2][1] = -5;
	w_butPos[3][0] = 750;
	w_butPos[3][1] = 200;
	w_butPos[4][0] = 750;
	w_butPos[4][1] = 285;

	w_goodsPos[0][0] = 220;
	w_goodsPos[0][1] = 320;
	w_goodsPos[1][0] = 453;
	w_goodsPos[1][1] = 320;
	w_goodsPos[2][0] = 220;
	w_goodsPos[2][1] = 210;
	w_goodsPos[3][0] = 453;
	w_goodsPos[3][1] = 210;

    //m_pHero		= NULL;
    CCSize size	= CCDirector::sharedDirector()->getWinSize();
    m_pWeaponBG = CCSprite::create("tu3/ShopBg.png");
    m_pWeaponBG->setPosition(ccp(800/2, 480/2));
    m_pWeaponBG->setVisible(false);
    addChild(m_pWeaponBG, -1);
    schedule(schedule_selector(ccbShopMenu::Timer), 0.02f);
    
    AppDelegate::LoadOnSale();
    if ( AppDelegate::s_ShopOnSell[0] == 0 )
    {
        AppDelegate::s_ShopOnSell[0] = rand()%8+5;
        if ( AppDelegate::s_ShopOnSell[0] > 6 )
            AppDelegate::s_ShopOnSell[0]++;
    }
    for ( int i = 0; i < 4; i++ )
    {
        m_iOnSetArray[i] = AppDelegate::s_ShopOnSell[i];
    }
    if (AppDelegate::s_FirstLogin == 3)
        m_bGuideClose = false;
}

ccbShopMenu::~ccbShopMenu()
{
    s_pccbShopMenu = NULL;
    AppDelegate::SaveOnSale();
    std::vector<AttackingObject*>::iterator it = m_vecAttackingObjectArray.begin();
    for (;it != m_vecAttackingObjectArray.end(); it++ )
        REMOVE_SELF((*it)->pSprite);
    VECTORCLEAN(m_vecAttackingObjectArray, AttackingObject);
    m_pArmature->removeFromParentAndCleanup(true);
    m_pFire->removeFromParentAndCleanup(true);
    CMainMenu::ReleaseArmatureData();
    CC_SAFE_RELEASE_NULL(m_AnimationManager);
}

void ccbShopMenu::RunAction(int _type, int _frame, int _repeat)
{
    if ( !_frame )
        _frame = d_unHeroFrame;
    if ( _repeat == 0 )
    {
        m_iAttackTime++;
        if ( m_iAttackTime == 36 )
        {
            m_unLastAction = -1;
            if ( m_iShowWeapon == 2 )
                AppDelegate::AudioPlayEffect("MS/Sound/EfCut.mp3");
            else if ( m_iShowWeapon == 6 )
                AppDelegate::AudioPlayEffect("MS4/5Cut.mp3");
            else if ( m_iShowWeapon == 13 )
                AppDelegate::AudioPlayEffect("MS4/8Cut.mp3");
            m_iAttackTime = 0;
            int type = _type-1;
#ifdef Old_Version
            if ( m_iShowWeapon == 14 && AppDelegate::s_HeroType == 1 )
            {
                int seed = rand()%3;
                if ( seed == 1 )
                    type = 35;
                else if ( seed == 2 )
                    type = 36;
            }
#endif
            m_pArmature->getAnimation()->stop();
            m_pArmature->getAnimation()->playByIndex(type, 0, _frame);
        }
    }
    else
    {
        if ( !_type || _type == m_unLastAction )
            return;
        m_pArmature->getAnimation()->stop();
        m_pArmature->getAnimation()->playByIndex(_type-1, 0, _frame, _repeat);
        m_unLastAction = _type;
    }
}

void ccbShopMenu::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    s_pccbShopMenu	= this;
    if ( AppDelegate::s_HeroType == 0 )
    {
        if ( Exclusive(AppDelegate::s_WeaponUse[0]) != 0 )
            AppDelegate::s_WeaponUse[0] = 2;
    }
    else if ( AppDelegate::s_HeroType == 1 )
    {
        if ( Exclusive(AppDelegate::s_WeaponUse[0]) != 1 )
            AppDelegate::s_WeaponUse[0] = 14;
    }
    else if ( AppDelegate::s_HeroType == 2 )
    {
        AppDelegate::s_WeaponUse[0] = 15;
    }
    
    if ( !WeaponCanUse(AppDelegate::s_WeaponUse[1]) )
    {
        for ( int i = WeaponNum; i > 2; )
        {
            i--;
            if ( WeaponType(weapon_index[i]) == 3 && AppDelegate::s_WeaponOwn[weapon_index[i]] == 1 && WeaponCanUse(weapon_index[i]) )
            {
                AppDelegate::s_WeaponUse[1] = weapon_index[i];
                break;
            }
        }
    }
    
    m_pBulletNode = pNode;
    m_unLastAction = 0;
    if ( AppDelegate::s_HeroType == 0 )
    {
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("chuanqi", "", "zhujiao.png", "zhujiao.plist", "zhujiao.json");
        ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("zhujiao_1.plist", "zhujiao_1.png");
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("qiangqiang", "", "qiangqiang.png", "qiangqiang.plist", "qiangqiang.json");
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("tulongdao1", "", "tulongdao.png", "tulongdao.plist", "tulongdao.json");
    }
    else if ( AppDelegate::s_HeroType == 1 )
    {
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("chuanqi1", "", "zhujiao2.png", "zhujiao2.plist", "zhujiao2.json");
#ifdef Old_Version
        ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("zhujiao2_1.plist", "zhujiao2_1.png");
#endif
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("qiangqiang1", "", "qiangqiang2.png", "qiangqiang2.plist", "qiangqiang2.json");
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("tulongdao1", "", "tulongdao2.png", "tulongdao2.plist", "tulongdao2.json");
    }
    else if ( AppDelegate::s_HeroType == 2 )
    {
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("chuanqi", "", "bot.png", "bot.plist", "bot.json");
        ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("bot1.plist", "bot1.png");
    }
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("qianghuohua1", "", "qianghuo.png", "qianghuo.plist", "qianghuo.json");
    
    if ( AppDelegate::s_HeroType == 0 )
        m_pArmature = Armature::create("dongzuo1");
    else
        m_pArmature = Armature::create("dongzuo");
    m_pArmature->setScaleX(-1.0f);
    //m_pArmature->setScaleX(-2.0f);
    //m_pArmature->setScaleY(2.0f);
    m_pArmature->setPosition(d_fShopHeroLocateX, d_fShopHeroLocateY);
    m_pArmature->setVisible(true);
    pNode->getChildByTag(207)->addChild(m_pArmature);
    RunAction(1);
    
    m_pFire = Armature::create("qianghuohua");
    pNode->getChildByTag(207)->addChild(m_pFire);
    
    if ( AppDelegate::s_HeroType != 2 )
    {
        std::string weapon[] = {"qiangqiang7.png", "qiangqiang8.png", "qiangqiang9.png", "qiangqiang11.png",
            "qiangqiang12.png", "qiangqiang13.png", "qiangqiang14.png", "qiangqiang15.png", "qiangqiangfire.png",
            "qiangqiang17.png", "qiangqiang18.png"};
        int num1 = AppDelegate::s_HeroType == 0 ? 9 : 11;
        for (int i = 0; i < num1; i++)
        {
            SpriteDisplayData displayData;
            displayData.setParam(weapon[i].c_str());
            m_pArmature->getBone("shou1")->addDisplay(&displayData, i);
        }
        m_pArmature->getBone("shou1")->setScale(1.25f);
        
        std::string weapon1[] = {"tulongdao2.png", "tulongdao111.png", "kaishanfu111.png", "ft.png"};
#ifdef Old_Version
        int num2 = 3;
#else
        int num2 = AppDelegate::s_HeroType == 0 ? 3 : 4;
#endif
        for (int i = 0; i < num2; i++)
        {
            SpriteDisplayData displayData;
            displayData.setParam(weapon1[i].c_str());
            m_pArmature->getBone("dao")->addDisplay(&displayData, i);
        }
        if ( AppDelegate::s_HeroType == 0 )
            m_pArmature->getBone("dao")->setScale(1.25f);
    }
    SpriteWeaponChange(AppDelegate::s_WeaponUse[0]);
    SpriteWeaponChange(AppDelegate::s_WeaponUse[1]);
    
    m_pNodeMoney = pNode->getChildByTag(205);
    m_pNodeMedal = pNode->getChildByTag(206);
    m_pNodeMoney->setZOrder(20);
    m_pNodeMedal->setZOrder(21);
    
    MoneyShow();
    //common::ShowNumber(m_pNodeMoney, AppDelegate::s_Money, 19, 24, d_fNumLocateGold, "tu3/suzi.png");
    m_pNodeMoney->/*getChildByTag(888)->*/setVisible(false);
    CCSprite* pSpriteMoney = CCSprite::create("tu3/qian.png");
    pSpriteMoney->setPosition( CCPoint(5+d_fNumLocateGold+40));
    m_pNodeMoney->addChild(pSpriteMoney);
    
    common::ShowNumber(m_pNodeMedal, AppDelegate::s_Medal, 19, 24, d_fNumLocateMedal, "tu3/suzi.png");
    m_pNodeMedal->/*getChildByTag(888)->*/setVisible(false);
    CCSprite* pSpriteMedal = CCSprite::create("tu3/xunzhang.png");
    pSpriteMedal->setPosition( CCPoint(5+d_fNumLocateMedal+43));
    m_pNodeMedal->addChild(pSpriteMedal);
    
#ifndef NoGetMoney
    m_pGetMoney = CCMenuItemImage::create("tu3/huoxu.png", "tu3/huoxu2.png", this, menu_selector(ccbShopMenu::GetMoney) );
    m_pGetMoney->setPosition( CCPoint(109, -13) );
    CCMenu* pMenu1 = CCMenu::create(m_pGetMoney, NULL);
    pMenu1->setPosition(ccp(0,0));
    m_pNodeMoney->addChild(pMenu1);
    CCAnimate* pGetMoneyAction = CCAnimate::create(common::CreateAnimationFrame("tu3/huoxu/", 7, CCRect(0,0,99,40)));
    CCSprite* pGetMoney = CCSprite::create("tu3/huoxu/1.png");
    pGetMoney->setPosition(ccp(109, -13));
    pGetMoney->runAction(CCRepeatForever::create(CCSequence::create(pGetMoneyAction, CCDelayTime::create(4.0f), NULL)));
    m_pNodeMoney->addChild(pGetMoney);
    
    m_pGetMedal = CCMenuItemImage::create("tu3/huoxu.png", "tu3/huoxu2.png", this, menu_selector(ccbShopMenu::GetMedal) );
    m_pGetMedal->setPosition( CCPoint(109, -13) );
    CCMenu* pMenu2 = CCMenu::create(m_pGetMedal, NULL);
    pMenu2->setPosition(ccp(0,0));
    m_pNodeMedal->addChild(pMenu2);
    CCAnimate* pGetMedalAction = CCAnimate::create(common::CreateAnimationFrame("tu3/huoxu/", 7, CCRect(0,0,99,40)));
    CCSprite* pGetMedal = CCSprite::create("tu3/huoxu/1.png");
    pGetMedal->setPosition(ccp(109, -13));
    pGetMedal->runAction(CCRepeatForever::create(CCSequence::create(pGetMedalAction, CCDelayTime::create(4.0f), NULL)));
    m_pNodeMedal->addChild(pGetMedal);
#endif
    
    for ( int i = 0; i < 4; i++ )
    {
        m_pIndexChose[i]	= CCSprite::create("tu3/ye1.png");
        m_pIndexUnchose[i]	= CCSprite::create("tu3/ye2.png");
        m_pIndexChose[i]->setPosition( ccp(0,0) );
        m_pIndexUnchose[i]->setPosition( ccp(0,0) );
        pNode->getChildByTag(200+i)->addChild(m_pIndexChose[i]);
        pNode->getChildByTag(200+i)->addChild(m_pIndexUnchose[i]);
    }
    {
        m_pIndexChose[4]	= CCSprite::create("tu3/ye1.png");
        m_pIndexUnchose[4]	= CCSprite::create("tu3/ye2.png");
        m_pIndexChose[4]->setPosition( ccp(0,0) );
        m_pIndexUnchose[4]->setPosition( ccp(0,0) );
        pNode->getChildByTag(199)->addChild(m_pIndexChose[4]);
        pNode->getChildByTag(199)->addChild(m_pIndexUnchose[4]);
    }
#ifndef GameTypeC
    {
        m_pIndexChose[5]	= CCSprite::create("tu3/ye1.png");
        m_pIndexUnchose[5]	= CCSprite::create("tu3/ye2.png");
        m_pIndexChose[5]->setPosition( ccp(0,0) );
        m_pIndexUnchose[5]->setPosition( ccp(0,0) );
        pNode->getChildByTag(1)->addChild(m_pIndexChose[5]);
        pNode->getChildByTag(1)->addChild(m_pIndexUnchose[5]);
    }
#endif
    
    m_pNodeBoard = pNode->getChildByTag(204);
    m_pOnSaleBoard = CCSprite::create("tu3/onsale/di.png");
    m_pOnSaleBoard->setAnchorPoint(ccp(0, 0));
    m_pOnSaleBoard->setPosition(ccp(30, 6));
    m_pNodeBoard->addChild(m_pOnSaleBoard);
    OnSaleInit();
    for ( int i = 0; i < WeaponNum; i++ )
    {
        if ( 0 == WeaponType(i) )
            m_pWeaponNode[i] = CCSprite::create("tu3/jiage.png");
        else if ( g_weaponUseMedal[i] )
            m_pWeaponNode[i] = CCSprite::create("tu3/dikuang.png");
        else
            m_pWeaponNode[i] = CCSprite::create("tu3/dikuang2.png");
        m_pWeaponNode[i]->setPosition( ccp(140+240*((i)%2), 212-128*(((i)/2)%2)) );
        m_pWeaponNode[i]->setVisible(false);
        m_pNodeBoard->addChild(m_pWeaponNode[i]);
        
        char buffer[255];
        //if ( AppDelegate::s_HeroType == 1 && i == 2 )
        //	sprintf(buffer, "tu3/qiang/quan.png");
        //else if ( AppDelegate::s_HeroType == 2 && i == 2 )
        //	sprintf(buffer, "tu3/qiang/jiguang.png");
        //else
        sprintf(buffer, "tu3/qiang/%d.png", i);
        CCSprite* pSprite = CCSprite::create(buffer);
        pSprite->setPosition( CCPoint(d_fShopWeaponLocateWeapon) );
        pSprite->setScaleY(0.7f);
        pSprite->setScaleX(-0.7f);
        m_pWeaponNode[i]->addChild(pSprite, 2);
        
        //if ( AppDelegate::s_HeroType == 1 && i == 2 )
        //	sprintf(buffer, "tu3/name/quan.png");
        //else if ( AppDelegate::s_HeroType == 2 && i == 2 )
        //	sprintf(buffer, "tu3/name/jiguang.png");
        //else
        sprintf(buffer, "tu3/name/%d.png", i);
        CCSprite* pSprite1 = CCSprite::create(buffer);
        pSprite1->setPosition( CCPoint(d_fShopWeaponLocateName) );
        pSprite1->setScale(0.5f);
        m_pWeaponNode[i]->addChild(pSprite1, 2);
        
        CCSprite* pSprite2 = CCSprite::create("tu3/yongyou.png");
        pSprite2->setPosition( CCPoint(d_fOwnLocate) );
        m_pWeaponNode[i]->addChild(pSprite2, 8, d_iShopWeaponHaveBuy);
        
        int sell1 = 1;
        int sell2 = 1;
        if ( WeaponOnSell(i) )
        {
            sell1 = 5;
            sell2 = 4;
        }
        if ( WeaponType(i) > 1 )
        {
            sell1 *= 2;
            sell2 *= AppDelegate::s_WeaponLevel[i] + 2 + AppDelegate::s_WeaponOwn[i];
        }
        if ( WeaponOnSell(i) )
        {
            if ( 0 == AppDelegate::s_WeaponOwn[i] )
            {
                CCSprite* pSpriteBoard = common::CreateAnimation("tu3/onsale/1.png", ccp(0,0), ccp(0,0),
                                                                 "tu3/onsale/", 9, CCRect(0,0,219,128), true);
                m_pWeaponNode[i]->addChild(pSpriteBoard, 4, d_iShopOnReduction);
            }
            
            CCSprite* pSprite2 = CCSprite::create("tu3/onsale/te.png");
            pSprite2->setPosition( CCPoint(53,65) );
            m_pWeaponNode[i]->addChild(pSprite2, 1);
        }
        if ( AppDelegate::s_WeaponOwn[i] == 0 )
        {
            pSprite2->setVisible(false);
        }
        if ( i > 0 )
        {
            int num = weapon_power[i];
            if ( i > 1 )
                num += weapon_upgrade[i]*AppDelegate::s_WeaponLevel[i];
            common::ShowNumber(m_pWeaponNode[i], num, 19, 24, d_fShopWeaponLocateDamage, "tu3/suzi.png", d_iShopWeaponTagDamage, 0, 0.6f);
        }
        {
            int x = 0;
            if ( g_priceBuy[i] > 999 )
                x = 1;
            common::ShowNumber(m_pWeaponNode[i], g_priceBuy[i]*sell2/sell1, 19, 24, 57 - 11*x , 20, "tu3/suzi.png", d_iShopWeaponTagGold, 0, 0.6f);
        }
        
        int type = Exclusive(i);
        if ( AppDelegate::s_HeroType != type && -1 != type )
        {
            char buffer[255];
            sprintf(buffer, "tu3/type/%d.png", type);
            CCSprite* pSprite = CCSprite::create(buffer);
            pSprite->setPosition(ccp(125, 77));
            m_pWeaponNode[i]->addChild(pSprite, 5);
        }
    }
    
    VisibleSet(0.0f);
    pNode->getChildByTag(400)->setZOrder(-3);
    m_pNodeBoard->setZOrder(-2);
    
    m_pNodeWeaponUse = pNode->getChildByTag(211);
    for ( int i = 0; i < WeaponNum; i++ )
    {
        if ( 0 == WeaponType(i) || 1 == WeaponType(i) )
            continue;
        char buffer[255];
        
        //if ( AppDelegate::s_HeroType == 1 && i == 2 )
        //	sprintf(buffer, "tu3/qiang2/quan.png");
        //else if ( AppDelegate::s_HeroType == 2 && i == 2 )
        //	sprintf(buffer, "tu3/qiang2/jiguang.png");
        //else
        sprintf(buffer, "tu3/qiang2/%d.png", i);
        CCSprite* pSprite = CCSprite::create(buffer);
        if ( 2 == WeaponType(i) )
            pSprite->setPosition( CCPoint(d_fShopWeaponLocateUse1) );
        else if ( 3 == WeaponType(i) )
            pSprite->setPosition( CCPoint(d_fShopWeaponLocateUse2) );
        else
            pSprite->setPosition( CCPoint(d_fShopWeaponLocateUse3) );
        pSprite->setVisible(false);
        //pSprite->setScaleY(0.9f);
        //pSprite->setScaleX(0.9f);
        m_pNodeWeaponUse->addChild(pSprite, 0, d_iShopWeaponTagUse+i);
        
    }
    SetWeaponUse(AppDelegate::s_WeaponUse[0]);
    SetWeaponUse(AppDelegate::s_WeaponUse[1]);
    //SetWeaponUse(AppDelegate::s_WeaponUse[2]);
    
    m_pNodeShowBoard = pNode->getChildByTag(401);
    {
        m_pShowWeaponOff = CCSprite::create("tu3/wuqizidan2.png");
        m_pShowWeaponOff->setPosition( CCPoint(d_fShopWeaponLocateOnOff) );
        m_pShowWeaponOff->setVisible(false);
        m_pNodeShowBoard->addChild(m_pShowWeaponOff);
        m_pShowWeaponOn = CCMenuItemImage::create("tu3/wuqizidan1.png", "tu3/wuqizidan1.png", this, menu_selector(ccbShopMenu::OnWeaponOnOff) );
        m_pShowWeaponOn->setPosition( CCPoint(d_fShopWeaponLocateOnOff) );
        
        for ( int i = 0; i < WeaponNum; i++ )
        {
            char buffer[255];
            //if ( AppDelegate::s_HeroType == 1 && i == 2 )
            //	sprintf(buffer, "tu3/qiang/quan.png");
            //else if ( AppDelegate::s_HeroType == 2 && i == 2 )
            //	sprintf(buffer, "tu3/qiang/jiguang.png");
            //else
            sprintf(buffer, "tu3/qiang/%d.png", i);
            CCSprite* pSprite = CCSprite::create(buffer);
            pSprite->setPosition( CCPoint(d_fShopShowWeaponLocate) );
            //pSprite->setScale(0.7f);
            pSprite->setVisible(false);
            m_pNodeShowBoard->addChild(pSprite, 1, d_iShopShowTagWeapon+i);
            
            //if ( AppDelegate::s_HeroType == 1 && i == 2 )
            //	sprintf(buffer, "tu3/name/quan.png");
            //else if ( AppDelegate::s_HeroType == 2 && i == 2 )
            //	sprintf(buffer, "tu3/name/jiguang.png", i);
            //else
            sprintf(buffer, "tu3/name/%d.png", i);
            CCSprite* pSprite1 = CCSprite::create(buffer);
            pSprite1->setPosition( CCPoint(d_fShopShowNameLocate) );
            pSprite1->setScale(0.9f);
            pSprite1->setVisible(false);
            m_pNodeShowBoard->addChild(pSprite1, 0, d_iShopShowTagName+i);
        }
        
        CCSprite* pSprite2 = CCSprite::create("tu3/onsale/zhe.png");
        pSprite2->setPosition( CCPoint(153, 140) );
        m_pNodeShowBoard->addChild(pSprite2, 0, d_iShopOnReduction);
        
        m_pShowWeaponBuy = CCMenuItemImage::create("tu3/goumai.png", "tu3/goumai2.png", this, menu_selector(ccbShopMenu::OnBuy) );
        m_pShowWeaponBuy->setPosition( CCPoint(d_fShopWeaponLocateBuy) );
        
        m_pShowWeaponEquip = CCMenuItemImage::create("tu3/zhuangbei1.png", "tu3/zhuangbei2.png", this, menu_selector(ccbShopMenu::OnEquip) );
        m_pShowWeaponEquip->setPosition( CCPoint(d_fShopWeaponLocateBuy) );
        m_pShowWeaponEquip->setVisible(false);
        
        m_pShowWeaponUpgrade = CCMenuItemImage::create("tu3/shengji.png", "tu3/shengji2.png", this, menu_selector(ccbShopMenu::OnUpgrade) );
        m_pShowWeaponUpgrade->setPosition( CCPoint(d_fShopWeaponLocateUpgrade) );
        
        m_pShowWeaponBullet[0] = CCMenuItemImage::create("tu3/buy/1.png", "tu3/buy/1_1.png", this, menu_selector(ccbShopMenu::Buy1) );
        m_pShowWeaponBullet[0]->setPosition( CCPoint(-130+d_fShopWeaponLocateBuy) );
        m_pShowWeaponBullet[1] = CCMenuItemImage::create("tu3/buy/2.png", "tu3/buy/2_1.png", this, menu_selector(ccbShopMenu::Buy2) );
        m_pShowWeaponBullet[1]->setPosition( CCPoint(d_fShopWeaponLocateBuy) );
        m_pShowWeaponBullet[2] = CCMenuItemImage::create("tu3/buy/3.png", "tu3/buy/3_1.png", this, menu_selector(ccbShopMenu::Buy3) );
        m_pShowWeaponBullet[2]->setPosition( CCPoint(130+d_fShopWeaponLocateBuy) );
        
        CCMenu* menu = CCMenu::create(m_pShowWeaponOn, m_pShowWeaponBuy, m_pShowWeaponEquip, m_pShowWeaponUpgrade,
                                      m_pShowWeaponBullet[0], m_pShowWeaponBullet[1], m_pShowWeaponBullet[2], NULL);
        menu->setPosition( ccp(0, 0) );
        m_pNodeShowBoard->addChild(menu);
        
        m_pShopEquiped = CCSprite::create("tu3/yizhuangbei.png");
        m_pShopEquiped->setPosition( CCPoint(d_fShopWeaponLocateBuy) );
        m_pNodeShowBoard->addChild(m_pShopEquiped);
        
        m_pShopUpgradeMax = CCSprite::create("tu3/max.png");
        m_pShopUpgradeMax->setPosition( CCPoint(d_fShopWeaponLocateUpgrade) );
        m_pNodeShowBoard->addChild(m_pShopUpgradeMax);
        
        for ( int i = 0; i < 3; i++ )
        {
            m_pStar1[i] = CCSprite::create("tu3/shangdianxing.png");
            m_pStar1[i]->setPosition( CCPoint(d_fShopShowStarLocate(i)) );
            m_pNodeShowBoard->addChild(m_pStar1[i], 2);
            m_pStar2[i] = CCSprite::create("tu3/shangdianxing2.png");
            m_pStar2[i]->setPosition( CCPoint(d_fShopShowStarLocate(i)) );
            m_pNodeShowBoard->addChild(m_pStar2[i], 2);
        }
        
        for ( int i = 0; i < WeaponNum; i++ )
        {
            if ( WeaponType(i) < 3 )
                continue;
            char buffer[255];
            sprintf(buffer, "tu3/bullet/%d.png", i);
            m_pWeaponBullet[i] = CCSprite::create(buffer);
            m_pWeaponBullet[i]->setPosition( CCPoint(270, 170) );
            m_pWeaponBullet[i]->setVisible(false);
            m_pNodeShowBoard->addChild(m_pWeaponBullet[i]);
        }
        //m_pBullet = CCSprite::create("tu3/qiang/zidan.png");
        //m_pBullet->setPosition( CCPoint(d_fShopShowWeaponLocate) );
        //m_pNodeShowBoard->addChild(m_pBullet);
        
        m_pBulletNum = CCSprite::create("tu3/zidanshu.png");
        m_pBulletNum->setPosition( CCPoint(d_fShopShowNumLocate) );
        m_pNodeShowBoard->addChild(m_pBulletNum);
        
        m_pHealboxNum = CCSprite::create("tu3/xuebaoshu.png");
        m_pHealboxNum->setPosition( CCPoint(d_fShopShowNumLocate) );
        m_pNodeShowBoard->addChild(m_pHealboxNum);
        
        m_pGrenadeNum = CCSprite::create("tu3/shouleishu.png");
        m_pGrenadeNum->setPosition( CCPoint(d_fShopShowNumLocate) );
        m_pNodeShowBoard->addChild(m_pGrenadeNum);
        
        m_pDamage = CCSprite::create("tu3/shanghai.png");
        m_pDamage->setPosition( CCPoint(d_fShopShowLocateDamage) );
        m_pNodeShowBoard->addChild(m_pDamage);
        common::ShowNumber(m_pDamage, 0, 19, 24, d_fShopShowBoardNumLocate1, "tu3/zidansuzi.png", 888, 0, 0.8f);
        
        m_pSpeed = CCSprite::create("tu3/shudu.png");
        m_pSpeed->setPosition( CCPoint(d_fShopShowLocateSpeed) );
        m_pNodeShowBoard->addChild(m_pSpeed);
        common::ShowNumber(m_pSpeed, 0, 19, 24, d_fShopShowBoardNumLocate1, "tu3/zidansuzi.png", 888, 0, 0.8f);
        
        m_pPower = CCSprite::create("tu3/weili.png");
        m_pPower->setPosition( CCPoint(d_fShopShowLocatePower) );
        m_pNodeShowBoard->addChild(m_pPower);
        common::ShowNumber(m_pPower, 0, 19, 24, d_fShopShowBoardNumLocate1, "tu3/zidansuzi.png", 888, 0, 0.8f);
        
        m_pLife = CCSprite::create("tu3/shengming.png");
        m_pLife->setPosition( CCPoint(d_fShopShowLocateDamage) );
        m_pNodeShowBoard->addChild(m_pLife);
        common::ShowNumber(m_pLife, 100, 19, 24, d_fShopShowBoardNumLocate1, "tu3/zidansuzi.png", 888, 0, 0.8f);
        
        m_pBulletMany = CCSprite::create("tu3/zidan.png");
        m_pBulletMany->setPosition( CCPoint(d_fShopShowLocateBullet1) );
        m_pNodeShowBoard->addChild(m_pBulletMany);
        common::ShowNumber(m_pBulletMany, 0, 19, 24, d_fShopShowBoardNumLocate1, "tu3/zidansuzi.png", 888, 0, 0.8f);
        
        m_pBulletLitter = CCSprite::create("tu3/zidan2.png");
        m_pBulletLitter->setPosition( CCPoint(d_fShopShowLocateBullet2) );
        m_pNodeShowBoard->addChild(m_pBulletLitter);
        common::ShowNumber(m_pBulletLitter, 0, 19, 24, d_fShopShowBoardNumLocate2, "tu3/zidansuzi2.png", 888, 0, 0.8f);
        
        m_pCostPic[0] = CCSprite::create("tu3/buy1.png");
        m_pCostPic[0]->setPosition( CCPoint(d_fShopShowCostPic) );
        m_pNodeShowBoard->addChild(m_pCostPic[0]);
        common::ShowNumber(m_pCostPic[0], 0, 19, 24, 48, 10, "tu3/zidansuzi2.png", 888, 0, 0.8f);
        m_pCostPic[1] = CCSprite::create("tu3/buy2.png");
        m_pCostPic[1]->setPosition( CCPoint(d_fShopShowCostPic) );
        m_pNodeShowBoard->addChild(m_pCostPic[1]);
        common::ShowNumber(m_pCostPic[1], 0, 19, 24, 48, 10, "tu3/zidansuzi2.png", 888, 0, 0.8f);
    }
	//添加手指
	CCSprite* selectSprite = ccbShopMenu::createHandelCreator();
	selectSprite->setPosition(ccp(w_butPos[0][0],w_butPos[0][1]));
	//selectSprite->setPosition(ccp(0,0));
	addChild(selectSprite,50,1001);
	//CCPoint point = m_pNodeShowBoard->convertToNodeSpace(selectSprite->getPosition());
	//CCPoint point=CCDirector::sharedDirector()->convertToGL(selectSprite->getPosition());
	CCLog("menuShop createHandel   ------------------");
	selectSprite->setVisible(false);
	//CCLog("px=%f,py=%f",point.x,point.y);
	w_groupIndex = 0;
	w_goodsPage = 0;
	w_goodIndex = 0;
	setKeypadEnabled(false);
}

CCSprite* ccbShopMenu::createHandelCreator(){
	CCTexture2D* img_selector = CCTextureCache::sharedTextureCache()->addImage("pt1.png");
	CCSpriteFrame* frame1 = CCSpriteFrame::createWithTexture(img_selector,CCRect(0,0,58,77));
	CCSpriteFrame* frame2 = CCSpriteFrame::createWithTexture(img_selector,CCRect(58,0,58,77));
	CCAnimation* c_hAnimation = CCAnimation::create();
	 c_hAnimation->setDelayPerUnit(0.05f);
	c_hAnimation->addSpriteFrame(frame1);
	c_hAnimation->addSpriteFrame(frame2);
	CCAnimate* c_hAnimate = CCAnimate::create(c_hAnimation);
	if(c_hAnimate==NULL){
		CCLog("animate create failed");
	}
	//CCActionInterval* action = CCSequence::create()
	CCSprite* m_pselector = CCSprite::create();
	if(m_pselector==NULL){
		CCLog("slelector is not live");
	}
	m_pselector->setAnchorPoint(ccp(0,1));
	
	m_pselector->initWithSpriteFrame(frame1);
	m_pselector->runAction(CCRepeatForever::create(c_hAnimate));
	m_pselector->setVisible(false);
	return m_pselector;
}

SEL_MenuHandler ccbShopMenu::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On5", ccbShopMenu::OnReturn );
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "On6", ccbShopMenu::OnShopBack );
    return NULL;
}

SEL_CCControlHandler ccbShopMenu::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ccbShopMenu::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    return false;
}

bool ccbShopMenu::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    bool bRet = false;
    return bRet;
}

void ccbShopMenu::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
    CC_SAFE_RELEASE_NULL(m_AnimationManager);
    m_AnimationManager = pAnimationManager;
    CC_SAFE_RETAIN(m_AnimationManager);
}

void ccbShopMenu::ccTouchesBegan( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
    if ( m_bOnSell )
        return;
    if ( m_iTime < 5 )
    {
        m_ccPoint = CCPoint(-1, -1);
        return;
    }
    m_ccPoint = ((CCTouch*)*(pTouches->begin()))->getLocation();
}

void ccbShopMenu::ccTouchesEnded( cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent )
{
    if ( m_bOnSell )
        return;
    if ( m_ccPoint.x > 0 && m_ccPoint.y > 0 )
    {
        CCPoint point = ((CCTouch*)*(pTouches->begin()))->getLocation();
        if ( abs(m_ccPoint.x - point.x) > 80 && m_iTime >= 20 && m_iStatus == 0 )
        {
            if ( m_ccPoint.x < point.x && m_iIndex > 0 )
            {
                if ( !m_bGuideClose )
                    return;
                m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan2", 0.0f);
                IndexChange(false);
                m_iTime = 0;
            }
            else if ( m_ccPoint.x > point.x && m_iIndex < 5 )
            {
                if ( !m_bGuideClose )
                {
                    if ( m_iIndex > 2 || m_iGuideIndex != 3 )
                        return;
                    else if ( m_iIndex == 2 )
                    {
                        GuideFingerClear();
                        m_iGuideIndex = 4;
                    }
                }
                m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan", 0.0f);
                IndexChange(true);
                m_iTime = 0;
            }
        }
        else if ( abs(m_ccPoint.x - point.x) < 15
                 && abs(m_ccPoint.y - point.y) < 15
                 && m_iTime >= 20 && m_iStatus == 0 )
        {
            if ( m_iIndex == 0 )
            {
                if ( !m_bGuideClose )
                    return;
                if ( abs(m_ccPoint.x - 335*Scale_X) < 228*Scale_X
                    && abs(m_ccPoint.y - 288*Scale_Y) < 140*Scale_Y )
                {
                    m_iStatus = 1;
                    m_bWeapon = true;
                    m_bShowAction = true;
                    int num = m_iOnSetArray[0];
                    int type = 1;
                    if ( SetWeaponShow(num, type) )
                        m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang", 0.0f);
                    m_ccPoint = CCPoint(-1, -1);
                    return;
                }
            }
            else
            {
                if ( abs(m_ccPoint.x - d_fShopItemLocateX*Scale_X) < d_fShopItemWeight*Scale_X
                    && abs(m_ccPoint.y - d_fShopItemLocateY*Scale_Y) < d_fShopItemHeight*Scale_Y )
                    m_iStatus = 3;
                else if ( abs(m_ccPoint.x - d_fShopItemLocateX*Scale_X - d_fShopItemDistX*Scale_X) < d_fShopItemWeight*Scale_X
                         && abs(m_ccPoint.y - d_fShopItemLocateY*Scale_Y) < d_fShopItemHeight*Scale_Y )
                    m_iStatus = 4;
                else if ( abs(m_ccPoint.x - d_fShopItemLocateX*Scale_X) < d_fShopItemWeight*Scale_X
                         && abs(m_ccPoint.y - d_fShopItemLocateY*Scale_Y - d_fShopItemDistY*Scale_Y) < d_fShopItemHeight*Scale_Y )
                    m_iStatus = 1;
                else if ( abs(m_ccPoint.x - d_fShopItemLocateX*Scale_X - d_fShopItemDistX*Scale_X) < d_fShopItemWeight*Scale_X
                         && abs(m_ccPoint.y - d_fShopItemLocateY*Scale_Y - d_fShopItemDistY*Scale_Y) < d_fShopItemHeight*Scale_Y )
                    m_iStatus = 2;
                if ( m_iStatus > 0 )
                {
                    if ( !m_bGuideClose )
                    {
                        if ( m_iStatus != 1 || m_iGuideIndex != 5 || m_iIndex != 3 )
                        {
                            m_iStatus = 0;
                            return;
                        }
                        else
                        {
                            m_iGuideIndex = 6;
                            GuideFingerClear();
                        }
                    }
                    else if ( m_iIndex > 4 )
                    {
                        if ( m_iStatus > 2 )
                        {
                            if ( getChildByTag(101) )
                            {
                                getChildByTag(101)->stopAllActions();
                                removeChildByTag(101);
                            }
                            CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
                            pSprite->setPosition(ccp(317, 306));
                            CCFadeOut* pAction = CCFadeOut::create(2.0f);
                            pSprite->runAction(CCSequence::create(pAction,
                                                                  CCCallFunc::create(this, callfunc_selector(ccbShopMenu::disappear)), NULL));
                            addChild(pSprite, 10, 101);
                            m_iStatus = 0;
                            return;
                        }
                    }
                    m_bWeapon = true;
                    m_bShowAction = true;
                    int num = m_iStatus+m_iIndex*4-5;
                    int type = 0;
                    if ( SetWeaponShow(num, type) )
                        m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang", 0.0f);
                    m_ccPoint = CCPoint(-1, -1);
                    return;
                }
            }
        }
        //}
        if ( m_ccPoint.y < d_fShopBottomY*Scale_Y && m_ccPoint.x > d_fShopBottomX*Scale_X )
        {
            if ( !m_bGuideClose )
                return;
            int a = (m_ccPoint.x/Scale_X - d_fShopBottomX)/d_iShopBottomDist;
            if ( a > -1 && a < 2 )
            {
                m_bWeapon = true;
                m_bShowAction = true;
                if ( m_iStatus == 0 )
                {
                    m_iStatus = 1;
                    m_bNewShowWeapon = true;
                    if ( SetWeaponShow(AppDelegate::s_WeaponUse[a], 1) )
                        m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang", 0.0f);
                }
                else if ( m_iShowWeapon != AppDelegate::s_WeaponUse[a] && m_bNewShowWeapon )
                {
                    m_bNewShowWeapon = false;
                    m_iNewShowWeapon = AppDelegate::s_WeaponUse[a];
                    m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang3", 0.0f);
                }
            }
        }
    }
    m_ccPoint = CCPoint(-1, -1);
}

void ccbShopMenu::keyBackClicked(){
	if(isInRecharge){
		GetRemove();
		isInRecharge = false;
		return;
	}
	if(!isInInfo){
		getChildByTag(1001)->setVisible(false);
		OnReturn(this);
		
	}else{
		OnShopBack(this);
		w_groupIndex = w_beforeGroupIndex;
		if(w_groupIndex==0 && w_goodsPage>0){
			if(w_goodsPage==0){
				setFignerPos(ccp(w_butPos[0][0],w_butPos[0][1]));
			}else{
				setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
			}
		}else {
			setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
		}
		isInInfo = false;
		CCLog("group=%d",w_goodIndex);
	}
}

void ccbShopMenu::keyEnterClicked(){
	if(isInRecharge){
		if(w_groupIndex==4){
			if(w_rechargeIndex==0){
				payMoneyBtn(2);
			}else if(w_rechargeIndex==1){
				payMoneyBtn(3);
			}
		}else if(w_groupIndex==5){
			if(w_rechargeIndex==0){
				payMedalBtn(2);
			}else if(w_rechargeIndex==1){
				payMedalBtn(3);
			}
		}
		return;
	}
	if(isInInfo){
		int num = m_iStatus+m_iIndex*4-5;
		bool isTuijian = false;
		int pn = 0;
		if(w_groupIndex==0){
			if(w_goodsPage==0){
				num = m_iOnSetArray[0];
				pn = num;
			}else{
				pn = weapon_index[num];
				//isTuijian=WeaponCanUse(weapon_index[num]) && AppDelegate::s_WeaponOwn[weapon_index[num]];
			}
		}else{
			num = AppDelegate::s_WeaponUse[w_butIndex-1];
			pn = num;
		}
		if(WeaponType(pn)==0){
			if(AppDelegate::s_Healbox<d_iHealBoxMax){
				OnBuy(this);
			} 
		}else if(WeaponType(pn)==1){
			if(AppDelegate::s_GrenadeNum<d_iGrenadeMax){
				OnBuy(this);
			}
		}else{
			if(!WeaponCanUse(pn)){
				//
				OnBuy(this);
			}else{
				if(!AppDelegate::s_WeaponOwn[pn]){
					//购买武器
					OnBuy(this);
				}else{
					if(w_diaIndex==0){
						if(AppDelegate::s_WeaponLevel[pn]<3){
							//升级武器
							OnUpgrade(this);
						}
					}else{
						if ( pn != AppDelegate::s_WeaponUse[0]
							&& pn != AppDelegate::s_WeaponUse[1]
							&& pn != AppDelegate::s_WeaponUse[2] ){
							//装备
							OnEquip(this);
						}
					}
				}
			}
		}
		return;
	}

	

	if(w_groupIndex==0){
		if(w_goodsPage==0){
			//跳转购买
			if ( !m_bGuideClose )
                return;
			m_iStatus = 1;
			m_bWeapon = true;
			m_bShowAction = true;
			int num = m_iOnSetArray[0];
			int type = 1;
			if ( SetWeaponShow(num, type) ){
				m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang", 0.0f);
				//w_groupIndex = 2;
				isInInfo = true;
				w_beforeGroupIndex = 0;
				CCLog("numId=%d",num);
				//如果不是武器
				if(!WeaponCanUse(num)){
					setFignerPos(ccp(336,143));
				}else{
					//130
					if(!AppDelegate::s_WeaponOwn[num]){
						setFignerPos(ccp(336,143));
					}else{
						setFignerPos(ccp(220,157));
					}
				}
			}
			m_ccPoint = CCPoint(-1, -1);
		}else{
			//购买别的
			m_iStatus = w_goodIndex+1;
			m_iIndex = w_goodsPage;
			if ( m_iStatus > 0 )
                {
                    if ( !m_bGuideClose )
                    {
                        if ( m_iStatus != 1 || m_iGuideIndex != 5 || m_iIndex != 3 )
                        {
                            m_iStatus = 0;
                            return;
                        }
                        else
                        {
                            m_iGuideIndex = 6;
                            GuideFingerClear();
                        }
                    }
                    else if ( m_iIndex > 4 )
                    {
                        if ( m_iStatus > 2 )
                        {
                            if ( getChildByTag(101) )
                            {
                                getChildByTag(101)->stopAllActions();
                                removeChildByTag(101);
                            }
                            CCSprite* pSprite = CCSprite::create("tu14/qidai.png");
                            pSprite->setPosition(ccp(317, 306));
                            CCFadeOut* pAction = CCFadeOut::create(2.0f);
                            pSprite->runAction(CCSequence::create(pAction,
                            CCCallFunc::create(this, callfunc_selector(ccbShopMenu::disappear)), NULL));
                            addChild(pSprite, 10, 101);
                            m_iStatus = 0;
                            return;
                        }
                    }
                    m_bWeapon = true;
                    m_bShowAction = true;
                    int num = m_iStatus+m_iIndex*4-5;
                    int type = 0;
                    if ( SetWeaponShow(num, type) ){
                        m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang", 0.0f);
						//w_groupIndex = 2;
						isInInfo = true;
						w_beforeGroupIndex = 0;
						CCLog("w_index=%d",weapon_index[num]);
						if(!WeaponCanUse(weapon_index[num])){
							setFignerPos(ccp(336,143));
						}else{
							//130
							if(!AppDelegate::s_WeaponOwn[weapon_index[num]]){
								setFignerPos(ccp(336,143));
							}else{
								setFignerPos(ccp(220,157));
							}
						}
					}
                    m_ccPoint = CCPoint(-1, -1);
                    return;
                }
		}
		
	}else if(w_groupIndex==1){
		if(w_butIndex==1||w_butIndex==2){
			if ( !m_bGuideClose )
                return;
            m_bWeapon = true;
            m_bShowAction = true;
            if ( m_iStatus == 0 )
            {
                m_iStatus = 1;
                m_bNewShowWeapon = true;
				
                if ( SetWeaponShow(AppDelegate::s_WeaponUse[w_butIndex-1], 1) ){
                    m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang", 0.0f);
					isInInfo = true;
					setFignerPos(ccp(220,157));
					w_diaIndex = 0;
				}
            }
            else if ( m_iShowWeapon != AppDelegate::s_WeaponUse[w_butIndex-1] && m_bNewShowWeapon )
            {
				isInInfo = true;
                m_bNewShowWeapon = false;
                m_iNewShowWeapon = AppDelegate::s_WeaponUse[w_butIndex-1];
                m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang3", 0.0f);
				setFignerPos(ccp(220,157));
				w_diaIndex = 0;
            }
            
		}else if(w_butIndex==3){
			GetMedal();
		}else if(w_butIndex==4){
			GetMoney();
		}
		w_beforeGroupIndex = 1;
	}else if(w_groupIndex==2){
		
		//w_beforeGroupIndex = 2;
	}
}

void ccbShopMenu::keyArrowClicked(int arrow){
	switch(arrow){
	case kTypeUpArrowClicked:
		if(isInInfo||isInRecharge){
			return;
		}
		if(w_groupIndex==0){
			if(w_goodsPage>0){
				if(w_goodIndex/2==0){
					w_goodIndex = w_goodIndex+2;
				}else{
					w_goodIndex = w_goodIndex-2;
				}
				setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
			}
		}else if(w_groupIndex==1){
			if(w_butIndex==1||w_butIndex==2){
				w_groupIndex = 0;
				w_butIndex = 0;
				if(w_goodsPage==0){
					setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
				}else{
					setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
				}
			}else if(w_butIndex==3){
				w_butIndex=4;
				setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
			}else if(w_butIndex==4){
				w_butIndex=3;
				setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
			}
		}else if(w_groupIndex==2){
			//左右是否转
		}
		break;
	case kTypeDownArrowClicked:
		if(isInInfo||isInRecharge){
			return;
		}
		if(w_groupIndex==0){
			if(w_goodsPage==0){
				w_groupIndex = 1;
				w_butIndex = 1;
				setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
			}else{
				if(w_goodIndex/2==0){
					w_goodIndex+=2;
					setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
				}else{
					w_groupIndex = 1;
					w_butIndex = 1;
					setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
				}
			}
		}else if(w_groupIndex==1){
			if(w_butIndex==3){
				w_butIndex=2;
			}else if(w_butIndex==4){
				w_butIndex = 3;
			}
			setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
		}
		break;
	case kTypeLeftArrowClicked:
		if(isInRecharge){
			if(w_rechargeIndex==0){
				w_rechargeIndex = 1;
				setFignerPos(ccp(556,199));
			}else{
				w_rechargeIndex = 0;
				setFignerPos(ccp(243,199));
			}
			return;
		}
		if(isInInfo){
			int num = m_iStatus+m_iIndex*4-5;
			 bool isTuijian = false;
			 if(w_groupIndex==0){
				 if(w_goodsPage==0){
					num = m_iOnSetArray[0];
					isTuijian = WeaponCanUse(num) && AppDelegate::s_WeaponOwn[num];
				 }else{
					isTuijian=WeaponCanUse(weapon_index[num]) && AppDelegate::s_WeaponOwn[weapon_index[num]];
				 }
			 }else{
				 num = AppDelegate::s_WeaponUse[w_butIndex-1];
				 isTuijian = true;
			 }
			 if(isTuijian){
				if(w_diaIndex==0){
					w_diaIndex = 1;
					setFignerPos(ccp(350,157));
				}else{
					w_diaIndex = 0;
					setFignerPos(ccp(220,157));
				}
			 }
			 return;
		}
		
		if(w_groupIndex==0){
			if(w_goodsPage>0){
				if(w_goodIndex%2==1){
					w_goodIndex --;
				}else{
					w_goodIndex = w_goodIndex+1;
					w_goodsPage--;
					m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan2", 0.0f);
					IndexChange(false);
					if(w_goodsPage==0){
						w_goodIndex = 0;
					}
				}
				if(w_goodsPage==0){
					setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
				}else{
					setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
				}
			}
		}else if(w_groupIndex==1){
			if(w_butIndex==2){
				w_butIndex = 1;
				setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
			}else if(w_butIndex==3||w_butIndex==4){
				w_butIndex = 0;
				w_groupIndex = 0;
				w_goodIndex = (w_goodIndex/2)*2+1;
				//getChildByTag(1001)->setPosition(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
				if(w_goodsPage==0){
					setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
				}else{
					setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
				}
			}
		}else if(w_groupIndex==2){
			 
		}else if(w_groupIndex==4||w_groupIndex==5){
			
		}
		break;
	case kTypeRightArrowClicked:
		if(isInRecharge){
			if(w_rechargeIndex==0){
				w_rechargeIndex = 1;
				setFignerPos(ccp(556,199));
			}else{
				w_rechargeIndex = 0;
				setFignerPos(ccp(243,199));
			}
			return;
		}
		if(isInInfo){
			 int num = m_iStatus+m_iIndex*4-5;
			 bool isTuijian = false;
			 if(w_groupIndex==0){
				 if(w_goodsPage==0){
					num = m_iOnSetArray[0];
					isTuijian = WeaponCanUse(num) && AppDelegate::s_WeaponOwn[num];
				 }else{
					isTuijian=WeaponCanUse(weapon_index[num]) && AppDelegate::s_WeaponOwn[weapon_index[num]];
				 }
			 }else{
				num = AppDelegate::s_WeaponUse[w_butIndex-1];
				isTuijian = true;
			 
			 }
			 if(isTuijian){
				if(w_diaIndex==0){
					w_diaIndex = 1;
					setFignerPos(ccp(350,157));
				}else{
					w_diaIndex = 0;
					setFignerPos(ccp(220,157));
				}
			 }
			 return;
		}
		
		if(w_groupIndex==0){
			if(w_goodsPage==0){
				w_goodIndex = 0;
				w_goodsPage = 1;
				 m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan", 0.0f);
				 IndexChange(true);
				setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
			}else if(w_goodsPage<5){
				if(w_goodIndex%2==0){
					w_goodIndex ++;
				}else {
					w_goodIndex = (w_goodIndex/2)*2;
					w_goodsPage++;
					 m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan", 0.0f);
					 IndexChange(true);
				}
				CCLog("goodPage=%d",w_goodsPage);
				setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
			}else{
				if(w_goodIndex%2==0){
					w_goodIndex ++;
					setFignerPos(ccp(w_goodsPos[w_goodIndex][0],w_goodsPos[w_goodIndex][1]));
				}else {
					w_groupIndex = 1;
					w_butIndex = 4;
					setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
				}
			}
		}else if(w_groupIndex==1){
			if(w_butIndex==1){
				w_butIndex = 2;
			}else if(w_butIndex==2){
				w_butIndex = 3;
			}
			setFignerPos(ccp(w_butPos[w_butIndex][0],w_butPos[w_butIndex][1]));
		}else if(w_groupIndex==2){
			
		}else if(w_groupIndex==4||w_groupIndex==5){
			
		}
		break;
	}
}

void ccbShopMenu::keyNumClicked(int keyNum){
	switch(keyNum){
	case kTypeNum0:
		keyBackClicked();
		break;
	}
}

void ccbShopMenu::setFignerPos(CCPoint point){
	getChildByTag(1001)->setPosition(point);
}

void ccbShopMenu::SpriteWeaponChange( int _index )
{
    if ( AppDelegate::s_HeroType == 2 )
        return;
    int index = AppDelegate::WeaponIndex(_index);
    if ( index >= 0 )
    {
        if ( index < 20 )
        {
            SpriteWeaponChange(AppDelegate::s_WeaponUse[0]);
            m_pArmature->getBone("shou1")->changeDisplayByIndex(index, true);
        }
        else
            m_pArmature->getBone("dao")->changeDisplayByIndex(index-20, true);
    }
}

void ccbShopMenu::SetWeaponUse( int _num )
{
    if ( _num == 0 )
        return;
    int* pInt = NULL;
    if ( 2 == WeaponType(_num) )
        pInt = &AppDelegate::s_WeaponUse[0];
    else if ( 3 == WeaponType(_num) )
        pInt = &AppDelegate::s_WeaponUse[1];
    else
        pInt = &AppDelegate::s_WeaponUse[2];
    m_pNodeWeaponUse->getChildByTag(d_iShopWeaponTagUse+*pInt)->setVisible(false);
    *pInt = _num;
    m_pNodeWeaponUse->getChildByTag(d_iShopWeaponTagUse+*pInt)->setVisible(true);
}

bool ccbShopMenu::SetWeaponShow( int _num, int _type )
{
    int in = _type ? _num : weapon_index[_num];
    if ( !WeaponCanUse(in) )
    {
        m_iStatus = 0;
        m_ccPoint = ccp(0, 0);
        return false;
    }
    for ( int i = 0; i < d_iIndexMax; i++ )
    {
        m_pIndexChose[i]->setVisible(false);
        m_pIndexUnchose[i]->setVisible(false);
    }
    
    for ( int i = 0; i < WeaponNum; i++ )
    {
        m_pNodeShowBoard->getChildByTag(d_iShopShowTagName+i)->setVisible(false);
        m_pNodeShowBoard->getChildByTag(d_iShopShowTagWeapon+i)->setVisible(false);
    }
    //////////////////
    /****武器界面****/
    //////////////////
    if ( m_bWeapon )
    {
        for ( int i = 0; i < 3; i++ )
            m_pShowWeaponBullet[i]->setVisible(false);
        
        if ( WeaponType(m_iShowWeapon) > 2 )
            m_pWeaponBullet[m_iShowWeapon]->setVisible(false);
        
        m_iShowWeapon = in;
        int sell1 = 1;
        int sell2 = 1;
        if ( WeaponOnSell(m_iShowWeapon) )
        {
            sell1 = 5;
            sell2 = 4;
        }
        if ( WeaponType(m_iShowWeapon) > 1 )
        {
            sell1 *= 2;
            sell2 *= AppDelegate::s_WeaponLevel[m_iShowWeapon] + 2 + AppDelegate::s_WeaponOwn[m_iShowWeapon];
        }
        CCNode* pCostPic = NULL;
        if ( !g_weaponUseMedal[m_iShowWeapon] )
        {
            m_pCostPic[0]->setVisible(false);
            m_pCostPic[1]->setVisible(true);
            pCostPic = m_pCostPic[1];
        }
        else
        {
            m_pCostPic[0]->setVisible(true);
            m_pCostPic[1]->setVisible(false);
            pCostPic = m_pCostPic[0];
        }
        
        m_iTime = 26;
        m_iAttackTime = 35;
        m_iRandAction = 2;
        if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 20 )
        {
            if ( m_bShowAction )
            {
                RunAction(12, 0, 0);
                m_bShowAction = false;
            }
        }
        else if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 21 )
        {
            if ( m_bShowAction )
            {
                RunAction(26, 0, 0);
                m_bShowAction = false;
            }
        }
        else if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 22 )
        {
            if ( m_bShowAction )
            {
                RunAction(33, 0, 0);
                m_bShowAction = false;
            }
        }
        else if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 23 )
        {
            if ( m_bShowAction )
            {
                if ( AppDelegate::s_WeaponLevel[15] == 0 )
                    RunAction(12, 0, 0);
                else
                    RunAction(16+AppDelegate::s_WeaponLevel[15], 0, 0);
                m_bShowAction = false;
            }
        }
        else
        {
            if ( 1 < WeaponType(m_iShowWeapon) )
                SpriteWeaponChange(m_iShowWeapon);
        }
        //if ( m_bShowAction )
        //{
        //	RunAction(1);
        //	m_bShowAction = false;
        //}
        //}
        
        if ( 3 == WeaponType(m_iShowWeapon) )
            m_iActoinTime = 150;
        else
            m_iActoinTime = 0;
        
        bool bOnSellShow = false;
        if ( WeaponOnSell(m_iShowWeapon) )
            bOnSellShow = true;
        m_pNodeShowBoard->getChildByTag(d_iShopOnReduction)->setVisible(bOnSellShow);
        
        m_pBulletNum->setVisible(false);
        m_pShowWeaponOff->setVisible(false);
        
        m_pNodeShowBoard->getChildByTag(d_iShopShowTagName+m_iShowWeapon)->setVisible(true);
        m_pNodeShowBoard->getChildByTag(d_iShopShowTagWeapon+m_iShowWeapon)->setVisible(true);
        
        bool bShow = false;
        if ( WeaponType(m_iShowWeapon) > 1 )
        {
            int d = AppDelegate::WeaponDamage(m_iShowWeapon);
            float s = g_weaponSpeedShow[m_iShowWeapon]*(1+0.2f*AppDelegate::s_WeaponLevel[m_iShowWeapon]);
            m_pDamage->setVisible(true);
            common::ShowNumberUpdate(m_pDamage, d);
            m_pSpeed->setVisible(true);
            common::ShowNumberUpdate(m_pSpeed, s);
            m_pPower->setVisible(true);
            int sp = weapon_power[m_iShowWeapon] + weapon_upgrade[m_iShowWeapon]*AppDelegate::s_WeaponLevel[m_iShowWeapon];
            common::ShowNumberUpdate(m_pPower, sp);
            m_pLife->setVisible(false);
            if ( 2 != WeaponType(m_iShowWeapon) )
            {
                bool bBullet = true;
                if ( AppDelegate::s_BulletNum[m_iShowWeapon] <= g_iWeaponBulletOnce[m_iShowWeapon]*3 )
                    bBullet = false;
                m_pBulletMany->setVisible(bBullet);
                m_pBulletLitter->setVisible(!bBullet);
                common::ShowNumberUpdate(m_pBulletMany, AppDelegate::s_BulletNum[m_iShowWeapon]);
                common::ShowNumberUpdate(m_pBulletLitter, AppDelegate::s_BulletNum[m_iShowWeapon]);
                if ( AppDelegate::s_WeaponOwn[m_iShowWeapon] )
                {
                    m_pShowWeaponOn->setVisible(true);
                    m_pShowWeaponOn->setOpacity(255);
                }
                else
                    m_pShowWeaponOn->setVisible(false);
            }
            else
            {
                m_pBulletMany->setVisible(false);
                m_pBulletLitter->setVisible(false);
                m_pShowWeaponOn->setVisible(false);
            }
            m_pNodeShowBoard->removeChildByTag(d_iShopShowTagNum);
            m_pHealboxNum->setVisible(false);
            m_pGrenadeNum->setVisible(false);
            bool bEquiped = false;
            bool bUpgrade = false;
            if ( AppDelegate::s_WeaponOwn[m_iShowWeapon] )
                bShow = true;
            m_pShowWeaponBuy->setVisible(!bShow);
            if ( m_iShowWeapon == AppDelegate::s_WeaponUse[0]
                || m_iShowWeapon == AppDelegate::s_WeaponUse[1]
                || m_iShowWeapon == AppDelegate::s_WeaponUse[2] )
                bEquiped = true;
            m_pShopEquiped->setVisible(bShow && bEquiped);
            m_pShowWeaponEquip->setVisible(bShow && !bEquiped);
            if ( AppDelegate::s_WeaponLevel[m_iShowWeapon] == 3 )
            {
                pCostPic->setVisible(false);
                bUpgrade = true;
            }
            else
                common::ShowNumberUpdate(pCostPic, g_priceUpgrade[m_iShowWeapon]*sell2/sell1);
            m_pShopUpgradeMax->setVisible(bShow && bUpgrade);
            m_pShowWeaponUpgrade->setVisible(bShow && !bUpgrade);
            m_pShopUpgradeMax->setPosition( CCPoint(d_fShopWeaponLocateUpgrade) );
        }
        else
        {
            m_pNodeShowBoard->getChildByTag(d_iShopShowTagName+m_iShowWeapon)->setVisible(true);
            m_pSpeed->setVisible(false);
            m_pPower->setVisible(false);
            m_pBulletMany->setVisible(false);
            m_pBulletLitter->setVisible(false);
            m_pShowWeaponOn->setVisible(false);
            if ( 0 == WeaponType(m_iShowWeapon) )
            {
                m_pDamage->setVisible(false);
                m_pHealboxNum->setVisible(true);
                m_pGrenadeNum->setVisible(false);
                m_pLife->setVisible(true);
                if ( AppDelegate::s_Healbox >= d_iHealBoxMax )
                    bShow = true;
                common::ShowNumber(m_pNodeShowBoard, AppDelegate::s_Healbox, 19, 24, d_fShopShowNumLocate2, "tu3/zidansuzi.png", d_iShopShowTagNum);
                common::ShowNumberUpdate(pCostPic, g_priceUpgrade[m_iShowWeapon]*sell2/sell1);
            }
            else if ( 1 == WeaponType(m_iShowWeapon) )
            {
                m_pDamage->setVisible(true);
                m_pLife->setVisible(false);
                m_pHealboxNum->setVisible(false);
                m_pGrenadeNum->setVisible(true);
                if ( AppDelegate::s_GrenadeNum >= d_iGrenadeMax )
                    bShow = true;
                common::ShowNumber(m_pNodeShowBoard, AppDelegate::s_GrenadeNum, 19, 24, d_fShopShowNumLocate2, "tu3/zidansuzi.png", d_iShopShowTagNum);
                common::ShowNumberUpdate(pCostPic, g_priceUpgrade[m_iShowWeapon]*sell2/sell1);
                common::ShowNumberUpdate(m_pDamage, g_iWeaponDamage[1] );
            }
            m_pShowWeaponBuy->setVisible(!bShow);
            m_pShopUpgradeMax->setVisible(bShow);
            m_pShopUpgradeMax->setPosition( CCPoint(d_fShopWeaponLocateBuy) );
            m_pShowWeaponUpgrade->setVisible(false);
            m_pShopEquiped->setVisible(false);
            m_pShowWeaponEquip->setVisible(false);
        }
        
        for ( int i = 0; i < 3; i++ )
        {
            bool bShow	= false;
            bool bNo	= false;
            if ( i < AppDelegate::s_WeaponLevel[m_iShowWeapon] )
                bShow = true;
            if ( !bShow && WeaponType(m_iShowWeapon) > 1 )
                bNo = true;
            m_pStar1[i]->setVisible(bShow);
            m_pStar2[i]->setVisible(bNo);
        }
    }
    //////////////////
    /****子弹界面****/
    //////////////////
    else
    {
        m_pNodeShowBoard->getChildByTag(d_iShopShowTagName+m_iShowWeapon)->setVisible(true);
        m_pCostPic[0]->setVisible(true);
        m_pCostPic[1]->setVisible(false);
        m_pWeaponBullet[m_iShowWeapon]->setVisible(true);
        m_pBulletNum->setVisible(true);
        m_pHealboxNum->setVisible(false);
        m_pGrenadeNum->setVisible(false);
        m_pShowWeaponUpgrade->setVisible(false);
        m_pShopEquiped->setVisible(false);
        m_pShowWeaponEquip->setVisible(false);
        m_pDamage->setVisible(false);
        m_pSpeed->setVisible(false);
        m_pPower->setVisible(false);
        m_pLife->setVisible(false);
        m_pBulletMany->setVisible(false);
        m_pBulletLitter->setVisible(false);
        for ( int i = 0; i < 3; i++ )
        {
            m_pStar1[i]->setVisible(false);
            m_pStar2[i]->setVisible(false);
        }
        bool bBuy = true;
        if ( AppDelegate::s_BulletNum[m_iShowWeapon] >= 999 )
            bBuy = false;
        //m_pShowWeaponBuy->setVisible(bBuy);
        for ( int i = 0; i < 3; i++ )
            m_pShowWeaponBullet[i]->setVisible(bBuy);
        m_pShopUpgradeMax->setVisible(!bBuy);
        m_pShopUpgradeMax->setPosition( CCPoint(-130 + d_fShopWeaponLocateBuy) );
        m_pShowWeaponBuy->setVisible(false);
        common::ShowNumber(m_pNodeShowBoard, AppDelegate::s_BulletNum[m_iShowWeapon], 19, 24, d_fShopShowNumLocate2, "tu3/zidansuzi.png", d_iShopShowTagNum);
        common::ShowNumberUpdate(m_pCostPic[0], g_priceBullet[m_iShowWeapon]);//价格 wait for update;
    }
    return true;
}

void ccbShopMenu::Appear()
{
#ifndef GameTypeC
    AppDelegate::AudioPlayBgm("MS/Music/mainmenu/ShopAchieve.mp3");
#endif
    m_bReturn = false;
    m_pArmature->setVisible(true);
    m_iIndex	= 0;
    m_iTime		= 0;
    m_iPress	= 0;
    m_iStatus	= 0;
    m_bIn		= true;
    VisibleSet(0.0f);
    setTouchEnabled(true);
    m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline", 0.0f);
}

void ccbShopMenu::OnReturn(cocos2d::CCObject *pSender)
{
    if ( m_bOnSell )
        return;
    if ( m_bReturn )
        return;
    if ( AppDelegate::s_FirstLogin == 3 )
        return;
    
    std::vector<AttackingObject*>::iterator it = m_vecAttackingObjectArray.begin();
    for (;it != m_vecAttackingObjectArray.end(); it++ )
        REMOVE_SELF((*it)->pSprite);
    VECTORCLEAN(m_vecAttackingObjectArray, AttackingObject);
    m_iActoinTime = 0;
    CMainMenu::m_bShop = false;
    m_bReturn = true;
    setTouchEnabled(false);
	setKeypadEnabled(false);
    m_pWeaponBG->setVisible(false);
	getChildByTag(1001)->setVisible(false);
    m_iStatus	= -1;
    m_bIn		= false;
    m_pNodeMoney->/*getChildByTag(888)->*/setVisible(false);
    m_pNodeMedal->/*getChildByTag(888)->*/setVisible(false);
    m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("fanhui", 0.0f);
    scheduleOnce(schedule_selector(ccbShopMenu::ReturnCallBack), 1.0f);
}

void ccbShopMenu::ReturnCallBack( float _t )
{
    m_pArmature->setVisible(false);
    if ( CMainMenu::GetDifficult() == -1 )
    {
        CCScene* pScene = CFirstLogin::CreateScene(false);
        //ccbLoginMenu::LoginScene(pScene);
        AppDelegate::ChangeScene( pScene );
    }
    else if ( AppDelegate::s_FirstLogin == 4 )
    {
        CMainMenu::SetStatus(0);
        ((ccbLayer*)CMainMenu::GetMenuMain())->AllBackWithBottom();
        //((ccbLayer*)CMainMenu::GetMenuMain())->ObjectShow();
    }
    else switch ( CMainMenu::GetStatus() )
    {
        case 0:
            ((ccbLayer*)CMainMenu::GetMenuMain())->AllBackWithBottom();
            //((ccbLayer*)CMainMenu::GetMenuMain())->ObjectShow();
            break;
        case 2:
            ((ccbLevelMenu*)CMainMenu::GetMenuLevel())->Appear();
            ((ccbLayer*)CMainMenu::GetMenuMain())->AllBack();
            ((ccbLayer*)CMainMenu::GetMenuMain())->ObjectShow();
            break;
    }
    ((ccbLayer*)CMainMenu::GetMenuMain())->setTouchEnabled(true);
}

void ccbShopMenu::Timer( float _dt )
{
    if ( !m_bIn )
        return;
    if ( m_iAttackCoolTime > 0 )
        m_iAttackCoolTime--;
    AttackRun();
    AttackCheck();
    GuideCheck();
    if ( m_iTime <= d_iNumShowTime+250 )
        m_iTime++;
    if ( m_iTime == 25 && m_iStatus != -1 )
    {
        m_pWeaponBG->setVisible(true);
		setKeypadEnabled(true);
		getChildByTag(1001)->setVisible(true);
    }
    else if ( m_iTime == d_iNumShowTime-1 )
    {
        m_pNodeMoney->/*getChildByTag(888)->*/setVisible(true);
        m_pNodeMedal->/*getChildByTag(888)->*/setVisible(true);
    }
    if ( m_iActoinTime > 0 && 3 == WeaponType(m_iShowWeapon) )
    {
        m_iActoinTime--;
        CreateAttack();
        return;
    }
    if ( m_iTime == d_iNumShowTime+100 || m_iTime == 25 )
    {
        m_iTime = 26;
        m_iRandAction = rand()%3;
    }
    if ( m_iTime > 25 )
    {
        if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 20 )
        {
            switch ( m_iRandAction )
            {
                case 0:
                    m_iAttackTime = 0;
                    RunAction(8);
                    break;
                case 1:
                    m_iAttackTime = 0;
                    RunAction(9);
                    break;
                case 2:
                    RunAction(12, 0, 0);
                    break;
            }
        }
        else if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 21 )
        {
            switch ( m_iRandAction )
            {
                case 0:
                    m_iAttackTime = 0;
                    RunAction(22);
                    break;
                case 1:
                    m_iAttackTime = 0;
                    RunAction(23);
                    break;
                case 2:
                    RunAction(26, 0, 0);
                    break;
            }
        }
        else if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 22 )
        {
            switch ( m_iRandAction )
            {
                case 0:
                    m_iAttackTime = 0;
                    RunAction(29);
                    break;
                case 1:
                    m_iAttackTime = 0;
                    RunAction(30);
                    break;
                case 2:
                    RunAction(33, 0, 0);
                    break;
            }
        }
        else if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 23 )
        {
            switch ( m_iRandAction )
            {
                case 0:
                    m_iAttackTime = 0;
                    RunAction(8);
                    break;
                case 1:
                    m_iAttackTime = 0;
                    RunAction(9);
                    break;
                case 2:
                    if ( AppDelegate::s_WeaponLevel[15] == 0 )
                        RunAction(12, 0, 0);
                    else
                        RunAction(16+AppDelegate::s_WeaponLevel[15], 0, 0);
                    break;
            }
        }
        else
        {
            switch ( m_iRandAction )
            {
                case 0:
                    RunAction(1);
                    break;
                case 1:
                    RunAction(2);
                    break;
                case 2:
                    RunAction(1);
                    //RunAction(6);
                    break;
            }
            if ( 0 != WeaponType(m_iShowWeapon) && 1 != WeaponType(m_iShowWeapon) )
                SpriteWeaponChange(m_iShowWeapon);
        }
    }
}

void ccbShopMenu::OnShopBack( cocos2d::CCObject *pSender )
{
    if ( m_bOnSell )
        return;
    if ( !m_iStatus )
        return;
    if ( m_bWeaponChange )
        return;
    if ( !m_bGuideClose )
    {
        if ( m_iGuideIndex != 16 )
            return;
        else
        {
            m_bGuideClose = true;
            AppDelegate::s_FirstLogin = 4;
            AppDelegate::SaveGuide();
            AppDelegate::SaveStatus();
            GuideFingerClear();
        }
    }
    m_iStatus = 0;
    m_ccPoint = ccp(0, 0);
    for ( int i = 0; i < d_iIndexMax; i++ )
    {
        if ( i == m_iIndex )
            m_pIndexChose[i]->setVisible(true);
        else
            m_pIndexUnchose[i]->setVisible(true);
    }
    for ( int i = 0; i < 16; i++ )
    {
        if ( AppDelegate::s_WeaponOwn[i] )
            m_pWeaponNode[i]->getChildByTag(d_iShopWeaponHaveBuy)->setVisible(true);
    }
    m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan shang2", 0.0f);
}

void ccbShopMenu::VisibleSet( float _t )
{
    for ( int i = 0; i < 4; i++ )
    {
        m_pWeaponNode[weapon_index[i*4]]->setVisible(false);
        m_pWeaponNode[weapon_index[i*4+1]]->setVisible(false);
        m_pWeaponNode[weapon_index[i*4+2]]->setVisible(false);
        m_pWeaponNode[weapon_index[i*4+3]]->setVisible(false);
    }
#ifndef GameTypeC
    m_pWeaponNode[weapon_index[16]]->setVisible(false);
    m_pWeaponNode[weapon_index[17]]->setVisible(false);
#endif
    m_pOnSaleBoard->setVisible(false);
    for ( int i = 0; i < d_iIndexMax; i++ )
    {
        bool bShow = false;
        if ( i == m_iIndex )
            bShow = true;
        m_pIndexChose[i]->setVisible(bShow);
        m_pIndexUnchose[i]->setVisible(!bShow);
    }
    if ( 0 == m_iIndex )
    {
        //for ( int i = 0; i < 4; i++ )
        //{
        //	m_pWeaponNode[m_iOnSetArray[i]]->setVisible(true);
        //	m_pWeaponNode[m_iOnSetArray[i]]->setPosition( ccp(140+240*((i)%2), 212-128*(((i)/2)%2)) );
        //}
        m_pOnSaleBoard->setVisible(true);
    }
#ifndef GameTypeC
    else if ( 5 == m_iIndex )
    {
        m_pWeaponNode[weapon_index[16]]->setVisible(true);
        m_pWeaponNode[weapon_index[17]]->setVisible(true);
        m_pWeaponNode[weapon_index[17]]->setPosition( ccp(140+240, 212) );
        //m_pWeaponNode[weapon_index[16]]->setPosition( ccp(140, 212) );
    }
#endif
    //else if ( 4 == m_iIndex )
    //{
    //	for ( int i = 0; i < 2; i++ )
    //	{
    //		m_pWeaponNode[weapon_index[m_iIndex*4+i-4]]->setVisible(true);
    //		m_pWeaponNode[weapon_index[m_iIndex*4+i-4]]->setPosition( ccp(140+240*((i)%2), 212-128*(((i)/2)%2)) );
    //	}
    //}
    else
    {
        for ( int i = 0; i < 4; i++ )
        {
            m_pWeaponNode[weapon_index[m_iIndex*4+i-4]]->setVisible(true);
            m_pWeaponNode[weapon_index[m_iIndex*4+i-4]]->setPosition( ccp(140+240*((i)%2), 212-128*(((i)/2)%2)) );
        }
    }
}

void ccbShopMenu::IndexChange( bool _bLeft )
{
    if ( _bLeft )
        m_iIndex++;
    else
        m_iIndex--;
    scheduleOnce(schedule_selector(ccbShopMenu::VisibleSet), 0.4f);
}

void ccbShopMenu::OnWeaponOnOff( cocos2d::CCObject *pSender )
{
    if ( m_bOnSell )
        return;
    if ( !m_bGuideClose )
    {
        if ( m_iGuideIndex != 12 )
            return;
        else
        {
            m_iGuideIndex = 13;
        }
    }
    AppDelegate::AudioPlayEffect("MS3/WeaponTypeChange.mp3");
    m_bWeapon = !m_bWeapon;
    if( m_bWeapon )
    {
        m_pShowWeaponOn->setOpacity(255);
        m_pShowWeaponOff->setVisible(false);
    }
    else
    {
        m_pShowWeaponOn->setOpacity(0);
        m_pShowWeaponOff->setVisible(true);
    }
    SetWeaponShow(m_iShowWeapon, 1);
}

void ccbShopMenu::OnBuy( cocos2d::CCObject *pSender )
{
    if ( m_bOnSell )
        return;
    if ( m_bWeapon )
    {
        if ( !m_bGuideClose )
        {
            if ( m_iGuideIndex != 7 )
                return;
            else
            {
                m_iGuideIndex = 8;
            }
        }
        int sell1 = 1;
        int sell2 = 1;
        if ( WeaponOnSell(m_iShowWeapon) )
        {
            sell1 = 5;
            sell2 = 4;
        }
        if ( g_weaponUseMedal[m_iShowWeapon] )
        {
            if ( AppDelegate::s_Money < g_priceBuy[m_iShowWeapon]*sell2/sell1 )
            {//花钱
                GetMoney();
                return;
            }
            AppDelegate::AudioPlayEffect("MS2/buy.mp3");
            AppDelegate::s_Money -= g_priceBuy[m_iShowWeapon]*sell2/sell1;
        }
        else
        {
            if ( AppDelegate::s_Medal < g_priceBuy[m_iShowWeapon]*sell2/sell1 )
            {//花钱
                GetMedal();
                return;
            }
            AppDelegate::AudioPlayEffect("MS2/buy.mp3");
            AppDelegate::s_Medal -= g_priceBuy[m_iShowWeapon]*sell2/sell1;
        }
        if ( WeaponType(m_iShowWeapon) > 1 )
        {
            AppDelegate::s_WeaponOwn[m_iShowWeapon] = 1;
            AppDelegate::SaveWeapon(m_iShowWeapon);
            if ( WeaponOnSell(m_iShowWeapon) )
                m_pWeaponNode[m_iShowWeapon]->removeChildByTag(d_iShopOnReduction);
            int num = 3;
            if ( m_iShowWeapon > 10 )
                num =2;
            AppDelegate::s_BulletNum[m_iShowWeapon] = num*g_iWeaponBulletOnce[m_iShowWeapon];
			
			//改变手指坐标
			setFignerPos(ccp(220,157));
			w_diaIndex = 0;
        }
        else if ( WeaponType(m_iShowWeapon) == 0 )
            AppDelegate::s_Healbox ++;
        else if ( WeaponType(m_iShowWeapon) == 1 )
            AppDelegate::s_GrenadeNum ++;
        
        if ( WeaponType(m_iShowWeapon) > 1 )
        {
            sell1 *= 2;
            sell2 *= AppDelegate::s_WeaponLevel[m_iShowWeapon] + 2 + AppDelegate::s_WeaponOwn[m_iShowWeapon];
        }
        common::ShowNumberUpdate(m_pWeaponNode[m_iShowWeapon], g_priceBuy[m_iShowWeapon]*sell2/sell1, d_iShopWeaponTagGold);
    }
    else
    {
        if ( !m_bGuideClose )
        {
            if ( m_iGuideIndex != 14 )
                return;
            else
            {
                if ( m_iGuideBuyTime > 0 )
                    m_iGuideBuyTime--;
                else
                {
                    m_iGuideIndex = 15;
                    GuideFingerClear();
                }
            }
        }
        if ( AppDelegate::s_BulletNum[m_iShowWeapon] < 999 )
        {
            if ( AppDelegate::s_Money < g_priceBullet[m_iShowWeapon] )
            {//花钱
                GetMoney();
                return;
            }
            AppDelegate::AudioPlayEffect("MS2/buy.mp3");
            AppDelegate::s_Money -= g_priceBullet[m_iShowWeapon];
            CCNode* pNode = m_pNodeShowBoard->getChildByTag(d_iShopShowTagNum);
            pNode->stopAllActions();
            pNode->setScale(2.0f);
            CCActionInterval* pAction2 = CCScaleTo::create(0.3f, 1.0f);
            pNode->runAction( pAction2 );
            
            AppDelegate::s_BulletNum[m_iShowWeapon] += g_iWeaponBulletOnce[m_iShowWeapon];
            if ( AppDelegate::s_BulletNum[m_iShowWeapon] > 999 )
                AppDelegate::s_BulletNum[m_iShowWeapon] = 999;
            AppDelegate::SaveBullet(m_iShowWeapon);
        }
    }
    SetWeaponShow(m_iShowWeapon, 1);
    MoneyShow();
    common::ShowNumberUpdate(m_pNodeMedal, AppDelegate::s_Medal);
}

void ccbShopMenu::OnEquip( cocos2d::CCObject *pSender )
{
    if ( m_bOnSell )
        return;
    if ( m_bWeaponChange )
        return;
    if ( !m_bGuideClose )
    {
        if ( m_iGuideIndex != 8 )
            return;
        else
        {
            m_iGuideIndex = 9;
            GuideFingerClear();
        }
    }
    m_bWeaponChange = true;
    WeaponChange();
    if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 20 )
        RunAction(12, 0, 0);
    else if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 21 )
        RunAction(26, 0, 0);
    else if ( AppDelegate::WeaponIndex(m_iShowWeapon) == 22 )
        RunAction(33, 0, 0);
    //else
    //	RunAction(6);
    m_iTime = d_iNumShowTime;
}

void ccbShopMenu::WeaponChange()
{
    m_AnimationManager->runAnimationsForSequenceNamedTweenDuration("Default Timeline zhuan copy", 0.0f);
    scheduleOnce(schedule_selector(ccbShopMenu::WeaponChangeTime), 0.8f);
}

void ccbShopMenu::WeaponChangeTime( float _t )
{
    SetWeaponUse(m_iShowWeapon);
    SetWeaponShow(m_iShowWeapon, 1);
    scheduleOnce(schedule_selector(ccbShopMenu::WeaponChangeOver), 0.5f);
}

void ccbShopMenu::WeaponChangeOver( float _t )
{
    m_bWeaponChange = false;
}

void ccbShopMenu::OnUpgrade( cocos2d::CCObject *pSender )
{
    if ( m_bOnSell )
        return;
    if ( !m_bGuideClose )
    {
        if ( m_iGuideIndex != 10 )
            return;
        else
        {
            m_iGuideIndex = 11;
            GuideFingerClear();
        }
    }
    int sell1 = 1;
    int sell2 = 1;
    if ( WeaponOnSell(m_iShowWeapon) )
    {
        sell1 = 5;
        sell2 = 4;
    }
    if ( WeaponType(m_iShowWeapon) > 1 )
    {
        sell1 *= 2;
        sell2 *= AppDelegate::s_WeaponLevel[m_iShowWeapon] + 2 + AppDelegate::s_WeaponOwn[m_iShowWeapon];
    }
    if ( g_weaponUseMedal[m_iShowWeapon] )
    {
        if ( AppDelegate::s_Money < g_priceUpgrade[m_iShowWeapon]*sell2/sell1 )
        {//花钱
            GetMoney();
            return;
        }
        AppDelegate::s_Money -= g_priceUpgrade[m_iShowWeapon]*sell2/sell1;
    }
    else
    {
        if ( AppDelegate::s_Medal < g_priceBuy[m_iShowWeapon]*sell2/sell1 )
        {//花钱
            GetMedal();
            return;
        }
        AppDelegate::s_Medal -= g_priceUpgrade[m_iShowWeapon]*sell2/sell1;
    }
    AppDelegate::AudioPlayEffect("MS/Sound/EfUpgrade.mp3");
    
#ifndef GameTypeC
    CCSprite* pSprite = common::CreateAnimation( "Animation/weaponup/1.png", CCPoint(d_fShopShowWeaponLocate), ccp(0.5, 0.5),
                                                "Animation/weaponup/", 4, CCRect(0, 0, Size_WeaponUpgrade),
                                                false, false, 0.1f, CCCallFuncN::create(this, callfuncN_selector(ccbShopMenu::WeaponUpgradeOver)) );
    m_pNodeShowBoard->addChild(pSprite, 2);
#endif
    
    m_pStar1[AppDelegate::s_WeaponLevel[m_iShowWeapon]]->setScale(2.0f);
    //CCActionInterval* pAction1 = CCScaleTo::create(0.2f, 1.5f);
    CCActionInterval* pAction2 = CCScaleTo::create(0.2f, 1.0f);
    m_pStar1[AppDelegate::s_WeaponLevel[m_iShowWeapon]]->runAction(
                                                                   CCSequence::create(/*pAction1, */pAction2, CCCallFuncN::create(this, callfuncN_selector(ccbShopMenu::UpgradeParticle)), NULL) );
    
    AppDelegate::s_WeaponLevel[m_iShowWeapon]++;
    AppDelegate::SaveWeapon(m_iShowWeapon);
    SetWeaponShow(m_iShowWeapon, 1);
    MoneyShow();
    common::ShowNumberUpdate(m_pNodeMedal, AppDelegate::s_Medal);
    float s = g_weaponSpeedShow[m_iShowWeapon]*(1+0.2f*AppDelegate::s_WeaponLevel[m_iShowWeapon]);
    int num = weapon_power[m_iShowWeapon] + weapon_upgrade[m_iShowWeapon]*AppDelegate::s_WeaponLevel[m_iShowWeapon];
    common::ShowNumberUpdate(m_pWeaponNode[m_iShowWeapon], num, d_iShopWeaponTagDamage);
    if ( AppDelegate::s_WeaponLevel[m_iShowWeapon] == 3 )
        common::ShowNumberUpdate(m_pWeaponNode[m_iShowWeapon], 0, d_iShopWeaponTagGold);
    else
    {
        int sell1 = 1;
        int sell2 = 1;
        if ( WeaponOnSell(m_iShowWeapon) )
        {
            sell1 = 5;
            sell2 = 4;
        }
        if ( WeaponType(m_iShowWeapon) > 1 )
        {
            sell1 *= 2;
            sell2 *= AppDelegate::s_WeaponLevel[m_iShowWeapon] + 2 + AppDelegate::s_WeaponOwn[m_iShowWeapon];
        }
        common::ShowNumberUpdate(m_pWeaponNode[m_iShowWeapon], g_priceBuy[m_iShowWeapon]*sell2/sell1, d_iShopWeaponTagGold);
    }
}

void ccbShopMenu::UpgradeParticle( cocos2d::CCNode* _pNode )
{
    CCParticleSystem* pParticle = CCParticleSystemQuad::create("tu3/xingxing.plist");
    pParticle->setPosition( CCPoint(d_LocationStarParticle) ); //setPosition(point);
    _pNode->addChild(pParticle, 1);
}

void ccbShopMenu::WeaponUpgradeOver( cocos2d::CCNode* _pNode )
{
    _pNode->removeFromParentAndCleanup(true);
}

int ccbShopMenu::WeaponType( int _num )
{
    if ( 0 == _num )
        return 0;//药包
    else if ( 1 == _num )
        return 1;//炸弹
    else if ( 2 == _num || 6 == _num || 13 == _num || 14 == _num || 15 == _num )
        return 2;//近战
    else if ( 3 == _num || 5 == _num || 9 == _num )
        return 3;//短枪
    else
        return 3;//长枪
}

bool ccbShopMenu::WeaponOnSell( int _num )
{
    for ( int i = 0; i < 1; i++ )
    {
        if ( _num == AppDelegate::s_ShopOnSell[i] )
            return true;
    }
    return false;
}

void ccbShopMenu::AttackRun()
{
    ////////////////////////
    /****攻击与怪墙检测****/
    ////////////////////////
    if ( m_vecAttackingObjectArray.size() )
    {
        std::vector<AttackingObject*>::iterator it = m_vecAttackingObjectArray.begin();
        for (;it != m_vecAttackingObjectArray.end(); it++ )
        {
            switch ( (*it)->type )
            {
                case eWT_Knife:
                    (*it)->time --;
                    break;
                case eWT_Pistol:
                case eWT_M4A1:
                    (*it)->x += (*it)->vx;
                    (*it)->y += (*it)->vy;
                    if ( (*it)->pSprite )
                        (*it)->pSprite->setPosition( ccp((*it)->x, (*it)->y) );
                    if ( (*it)->damage == 8 )
                        (*it)->life--;
                    else if ( (*it)->damage == 10 )
                    {
                        float r = (*it)->pSprite->getRotation();
                        r += 10;
                        (*it)->pSprite->setRotation(r);
                    }
                    break;
                case eWT_Fire:
                    (*it)->life--;
                    break;
            }
        }
    }
}

void ccbShopMenu::AttackCheck()
{
    if ( m_vecAttackingObjectArray.size() )
    {
        while ( m_vecAttackingObjectArray.size() )
        {
            bool bStop		= true;
            bool bStopFor	= false;
            std::vector<AttackingObject*>::iterator it = m_vecAttackingObjectArray.begin();
            for (;it != m_vecAttackingObjectArray.end(); it++ )
            {
                if ( 0 == (*it)->life )
                {
                    if ( (*it)->pSprite )
                        (*it)->pSprite->stopAllActions();
                    if ( m_pFireObject == (*it) )
                        m_pFireSprite = NULL;
                    REMOVE_SELF((*it)->pSprite);
                    DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
                    bStop		= false;
                    bStopFor	= true;
                }
                else switch ( (*it)->type )
                {
                    case eWT_Knife:
                    case eWT_Fire:
                        if ( (*it)->time == 0 )
                        {
                            REMOVE_SELF((*it)->pSprite);
                            DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
                            bStop		= false;
                            bStopFor	= true;
                        }
                        break;
                    case eWT_Pistol:
                    case eWT_M4A1:
                        if ( (*it)->wall )
                        {
                            if ( (*it)->x < -20.0 )
                            {
                                bStop		= false;
                                bStopFor	= true;
                                if ( (*it)->isFirst && (*it)->damage == 9 )
                                {
                                    char str[100] = {0};
                                    CCArray* animFrames1 = CCArray::createWithCapacity(7);
                                    for(int i = 1; i < 7; i++)
                                    {
                                        sprintf(str,"Animation/grenade/d%d.png", i);
                                        CCSpriteFrame* frame = CCSpriteFrame::create(str,CCRect(0, 0, 81, 99));
                                        animFrames1->addObject(frame);
                                    }
                                    CCAnimation* pAnimation = CCAnimation::createWithSpriteFrames(animFrames1, d_fStarMovieTime);
                                    CCSequence* pSequence = CCSequence::create(
                                                                               CCAnimate::create(pAnimation),
                                                                               CCCallFuncN::create(this, callfuncN_selector(ccbShopMenu::CallBackRemoveSelf)),
                                                                               NULL);
                                    CCSprite* pBomb = CCSprite::create("Animation/grenade/d1.png");
                                    pBomb->setPosition(ccp((*it)->x, (*it)->y));
                                    pBomb->runAction(pSequence);
                                    m_pBulletNode->getChildByTag(207)->addChild(pBomb, 5);
                                    
                                    AppDelegate::AudioPlayEffect("MS/Weapon/grenade.mp3");
                                    
                                    CCSprite* pSprite = CCSprite::create("bullet/9/1.png");
                                    pSprite->setAnchorPoint(ccp(0, 0.5f));
                                    pSprite->setPosition(ccp((*it)->x, (*it)->y));
                                    pSprite->setRotation(180);
                                    pSprite->setScale((*it)->toward);
                                    m_pBulletNode->getChildByTag(207)->addChild(pSprite, 9);
                                    pSprite->runAction(CCSequence::create(CCMoveBy::create(0.6f, ccp(-50*(*it)->toward, 0)),
                                                                          CCCallFuncN::create(this, callfuncN_selector(ccbShopMenu::BulletAimMonster)), NULL));
                                    
                                    CCSprite* pSprite1 = CCSprite::create("bullet/9/1.png");
                                    pSprite1->setAnchorPoint(ccp(0, 0.5f));
                                    pSprite1->setPosition(ccp((*it)->x, (*it)->y));
                                    pSprite1->setRotation(240);
                                    pSprite1->setScale((*it)->toward);
                                    m_pBulletNode->getChildByTag(207)->addChild(pSprite1, 9);
                                    pSprite1->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-30*(*it)->toward, 40*(*it)->toward)),
                                                                           CCCallFuncN::create(this, callfuncN_selector(ccbShopMenu::BulletAimMonster)), NULL));
                                    
                                    CCSprite* pSprite2 = CCSprite::create("bullet/9/1.png");
                                    pSprite2->setAnchorPoint(ccp(0, 0.5f));
                                    pSprite2->setPosition(ccp((*it)->x, (*it)->y));
                                    pSprite2->setRotation(120);
                                    pSprite2->setScale((*it)->toward);
                                    m_pBulletNode->getChildByTag(207)->addChild(pSprite2, 9);
                                    pSprite2->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(-30*(*it)->toward, -40*(*it)->toward)),
                                                                           CCCallFuncN::create(this, callfuncN_selector(ccbShopMenu::BulletAimMonster)), NULL));
                                }
                                REMOVE_SELF((*it)->pSprite);
                                DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
                                break;
                            }
                        }
                        else if ( (*it)->x < -20.0 )
                        {
                            REMOVE_SELF((*it)->pSprite);
                            DELETEITERATORFROMVECTOR(m_vecAttackingObjectArray, it);
                            bStop		= false;
                            bStopFor	= true;
                            break;
                        }
                        break;
                }
                if ( bStopFor )
                    break;
            }
            if ( bStop )
                break;
        }
    }
}

void ccbShopMenu::BulletAimMonster(CCNode* _pNode)
{
    AttackingObject* pAttackingObject	= new AttackingObject;
    m_vecAttackingObjectArray.push_back(pAttackingObject);
    pAttackingObject->isFirst	= 0;
    pAttackingObject->type	= eWT_Pistol;
    pAttackingObject->time		= 10;
    pAttackingObject->life		= 20;
    pAttackingObject->speed		= 10;
    pAttackingObject->x		= _pNode->getPositionX();
    pAttackingObject->y		= _pNode->getPositionY();
    pAttackingObject->pSprite = CCSprite::create("bullet/9/1.png");
    pAttackingObject->pSprite->setAnchorPoint(ccp(0, 0.5f));
    pAttackingObject->pSprite->setPosition( ccp(pAttackingObject->x, pAttackingObject->y) );
    m_pBulletNode->getChildByTag(207)->addChild(pAttackingObject->pSprite, 10);
    
    {
        float toward = 1.0f;
        pAttackingObject->vx = pAttackingObject->speed*toward;
        pAttackingObject->vy = 0;
        pAttackingObject->pSprite->setScaleX(toward);
    }
    _pNode->removeFromParentAndCleanup(true);
}

void ccbShopMenu::CallBackRemoveSelf( CCNode* _pNode )
{
    _pNode->removeFromParentAndCleanup(true);
}

void ccbShopMenu::CreateAttack()
{
    if ( m_iAttackCoolTime > 0 )
        return;
    
    m_pArmature->getAnimation()->playByIndex(ActionIndex(m_iShowWeapon)-1, 0, 11);
    m_unLastAction = 0;
    if ( m_iShowWeapon == 12 && m_pFireSprite )
    {
        m_pFireObject->life = 10;
        m_iAttackCoolTime	= g_iWeaponCooltime[m_iShowWeapon];
        AppDelegate::AudioPlayEffect("MS2/firegun.mp3");
    }
    else
    {
        AttackingObject* pAttackingObject	= new AttackingObject;
        m_vecAttackingObjectArray.push_back(pAttackingObject);
        pAttackingObject->toward = -1;
        pAttackingObject->damage = m_iShowWeapon;
        if ( m_iShowWeapon == 12 )
        {
            AppDelegate::AudioPlayEffect("MS2/firegun.mp3");
            pAttackingObject->type		= eWT_Fire;
            m_iAttackCoolTime			= g_iWeaponCooltime[m_iShowWeapon];
            m_pFireObject				= pAttackingObject;
            m_pFireObject->time			= 10;
            m_pFireObject->damage		= 12;
            pAttackingObject->life		= 10;
            pAttackingObject->speed		= 0;
            pAttackingObject->pSprite	= common::CreateAnimation("Animation/fire/1.png", CCPoint(0, 0), CCPoint(0.02f, 0.14f),
                                                                  "Animation/fire/", 7, CCRect(0, 0, 313, 133), true);
            m_pBulletNode->getChildByTag(207)->addChild(pAttackingObject->pSprite, 3);
            pAttackingObject->pSprite->setScaleX(pAttackingObject->toward);
            pAttackingObject->w			= 270;
            pAttackingObject->h			= 60;
            //pAttackingObject->rotation	= m_pHero->GetRotation();
            //pAttackingObject->pSprite->setRotation( -pAttackingObject->rotation * pAttackingObject->toward );
            //pAttackingObject->rotation  = (float)(pAttackingObject->rotation * PI_FIX);
            
            pAttackingObject->y		= d_fShopHeroLocateY+g_fFireY[m_iShowWeapon];
            if ( AppDelegate::s_HeroType == 0 )
                pAttackingObject->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX[m_iShowWeapon];
            else if ( AppDelegate::s_HeroType == 1 )
                pAttackingObject->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX2[m_iShowWeapon];
            else if ( AppDelegate::s_HeroType == 2 )
            {
                pAttackingObject->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX3S;
                pAttackingObject->y			= d_fShopHeroLocateY + g_fFireY3S - 28;
            }
            pAttackingObject->pSprite->setPosition( ccp(pAttackingObject->x, pAttackingObject->y) );
            m_pFireSprite = pAttackingObject->pSprite;
        }
        else
        {
            int x = 0;
            int y = 0;
            if ( AppDelegate::s_HeroType == 0 )
            {
                x = - g_fFireX[m_iShowWeapon];
                y = g_fFireY[m_iShowWeapon];
            }
            else if ( AppDelegate::s_HeroType == 1 )
            {
                x = - g_fFireX2[m_iShowWeapon];
                y = g_fFireY2[m_iShowWeapon];
            }
            else if ( AppDelegate::s_HeroType == 2 )
            {
                x = - g_fFireX3S;
                y = g_fFireY3S;
            }
            m_pFire->setPosition(x+d_fShopHeroLocateX, y+d_fShopHeroLocateY);
            m_pFire->setScaleX(-1);
            m_pFire->getAnimation()->playByIndex(FireIndex(m_iShowWeapon),0,10);
            
            pAttackingObject->type	= eWT_Pistol;
            m_iAttackCoolTime		= g_iWeaponCooltime[m_iShowWeapon];
            char name[255];
            sprintf(name, "MS/Weapon/%d.mp3", m_iShowWeapon);
            AppDelegate::AudioPlayEffect(name);
            pAttackingObject->time		= 10;
            pAttackingObject->life		= 20;
            pAttackingObject->speed		= 10;
            pAttackingObject->rotation	= 0;
            if ( AppDelegate::s_HeroType == 0 )
            {
                pAttackingObject->x		= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX[m_iShowWeapon];
                pAttackingObject->y		= d_fShopHeroLocateY+g_fFireY[m_iShowWeapon];
            }
            else if ( AppDelegate::s_HeroType == 1 )
            {
                pAttackingObject->x		= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX2[m_iShowWeapon];
                pAttackingObject->y		= d_fShopHeroLocateY+g_fFireY2[m_iShowWeapon];
            }
            else if ( AppDelegate::s_HeroType == 2 )
            {
                pAttackingObject->x		= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX3S;
                pAttackingObject->y		= d_fShopHeroLocateY + g_fFireY3S;
            }
            pAttackingObject->vx = pAttackingObject->speed*cosf(pAttackingObject->rotation)*pAttackingObject->toward;
            pAttackingObject->vy = pAttackingObject->speed*sinf(pAttackingObject->rotation);
            char bulletName[255];
            if ( m_iShowWeapon != 8 )
            {
                sprintf(bulletName, "bullet/%d/3.png", m_iShowWeapon/*, AppDelegate::s_WeaponLevel[AppDelegate::s_WeaponUse[i+1]]+1*/);
                pAttackingObject->pSprite = CCSprite::create(bulletName);
                pAttackingObject->pSprite->setScaleX(pAttackingObject->toward);
                pAttackingObject->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
                pAttackingObject->pSprite->setPosition( ccp(pAttackingObject->x, pAttackingObject->y) );
                m_pBulletNode->getChildByTag(207)->addChild(pAttackingObject->pSprite, 3);
                pAttackingObject->pSprite->setRotation( -pAttackingObject->rotation * pAttackingObject->toward /(PI_FIX));
            }
            if ( m_iShowWeapon < 12 )
            {
                //if ( !m_pFireGas )
                //{
                //	m_pFireGas = CCSprite::create("yan1.png");
                //	m_pFireGas->setScale(1.3f);
                //	m_pFireGas->setPosition(ccp(pAttackingObject->x-15*(-1.0f),pAttackingObject->y));
                //	m_pFireGas->setAnchorPoint(ccp(0.5f,0.1f));
                //	m_pHero->addChild(m_pFireGas);
                //	CCFadeOut* pAction = CCFadeOut::create(0.5f);
                //	CCMoveBy* pAction1 = CCMoveBy::create(0.5f, ccp(0, 30));
                //	m_pFireGas->runAction(CCSequence::create(pAction, NULL));
                //	m_pFireGas->runAction(CCSequence::create(pAction1,
                //		CCCallFunc::create(this, callfunc_selector(CGameControler::CallBackGas)), NULL));
                //}
                //if ( AppDelegate::s_WeaponUse[1] == 10 )
                //	pAttackingObject->vy += (rand()%10/10.0f - 0.5f);
                /*else */if ( m_iShowWeapon == 5 )
                {
                    AttackingObject* pAttackingObject1 = new AttackingObject;
                    m_vecAttackingObjectArray.push_back(pAttackingObject1);
                    pAttackingObject1->type		= eWT_Pistol;
                    pAttackingObject1->toward	= -1;
                    pAttackingObject1->time		= 10;
                    pAttackingObject1->life		= 20;
                    pAttackingObject1->speed	= 10;
                    pAttackingObject1->rotation	= 20;
                    pAttackingObject1->rotation = (float)(pAttackingObject1->rotation * PI_FIX);
                    pAttackingObject1->y		= d_fShopHeroLocateY+g_fFireY[m_iShowWeapon];
                    if ( AppDelegate::s_HeroType == 0 )
                        pAttackingObject1->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX[m_iShowWeapon];
                    else if ( AppDelegate::s_HeroType == 1 )
                        pAttackingObject1->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX2[m_iShowWeapon];
                    else if ( AppDelegate::s_HeroType == 2 )
                    {
                        pAttackingObject1->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX3S;
                        pAttackingObject1->y			= d_fShopHeroLocateY + g_fFireY3S;
                    }
                    pAttackingObject1->vx = pAttackingObject1->speed*cosf(pAttackingObject1->rotation)*pAttackingObject1->toward;
                    pAttackingObject1->vy = pAttackingObject1->speed*sinf(pAttackingObject1->rotation);
                    pAttackingObject1->pSprite = CCSprite::create(bulletName);
                    pAttackingObject1->pSprite->setScaleX(pAttackingObject1->toward);
                    pAttackingObject1->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
                    pAttackingObject1->pSprite->setPosition( ccp(pAttackingObject1->x, pAttackingObject1->y) );
                    m_pBulletNode->getChildByTag(207)->addChild(pAttackingObject1->pSprite, 3);
                    pAttackingObject1->pSprite->setRotation( -pAttackingObject1->rotation * pAttackingObject1->toward /(PI_FIX));
                    
                    AttackingObject* pAttackingObject2 = new AttackingObject;
                    m_vecAttackingObjectArray.push_back(pAttackingObject2);
                    pAttackingObject2->toward	= (-1.0f);
                    pAttackingObject2->type	= eWT_Pistol;
                    pAttackingObject2->time		= 10;
                    pAttackingObject2->life		= 20;
                    pAttackingObject2->speed	= 10;
                    pAttackingObject2->rotation	= -20;
                    pAttackingObject2->rotation = (float)(pAttackingObject2->rotation * PI_FIX);
                    pAttackingObject2->y		= d_fShopHeroLocateY+g_fFireY[m_iShowWeapon];
                    if ( AppDelegate::s_HeroType == 0 )
                        pAttackingObject2->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX[m_iShowWeapon];
                    else if ( AppDelegate::s_HeroType == 1 )
                        pAttackingObject2->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX2[m_iShowWeapon];
                    else if ( AppDelegate::s_HeroType == 2 )
                    {
                        pAttackingObject2->x			= d_fShopHeroLocateX - d_fWeaponCenterX - g_fFireX3S;
                        pAttackingObject2->y			= d_fShopHeroLocateY + g_fFireY3S;
                    }
                    pAttackingObject2->vx = pAttackingObject2->speed*cosf(pAttackingObject2->rotation)*pAttackingObject->toward;
                    pAttackingObject2->vy = pAttackingObject2->speed*sinf(pAttackingObject2->rotation);
                    pAttackingObject2->pSprite = CCSprite::create(bulletName);
                    pAttackingObject2->pSprite->setScaleX(pAttackingObject2->toward);
                    pAttackingObject2->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
                    pAttackingObject2->pSprite->setPosition( ccp(pAttackingObject2->x, pAttackingObject2->y) );
                    m_pBulletNode->getChildByTag(207)->addChild(pAttackingObject2->pSprite, 3);
                    pAttackingObject2->pSprite->setRotation( -pAttackingObject2->rotation * pAttackingObject2->toward /(PI_FIX));
                }
            }
            else if ( m_iShowWeapon == 16 )
            {
                AttackingObject* pAttackingObject1 = new AttackingObject;
                m_vecAttackingObjectArray.push_back(pAttackingObject1);
                pAttackingObject1->type		= eWT_Pistol;
                pAttackingObject1->toward	= pAttackingObject->toward;
                pAttackingObject1->time		= 10;
                pAttackingObject1->life		= 20;
                pAttackingObject1->speed	= 10;
                pAttackingObject1->rotation	= pAttackingObject->rotation;
                pAttackingObject1->x		= pAttackingObject->x;
                pAttackingObject1->y		= pAttackingObject->y - 10.0f;
                pAttackingObject1->vx		= pAttackingObject->vx;
                pAttackingObject1->vy		= pAttackingObject->vy;
                pAttackingObject1->pSprite = CCSprite::create(bulletName);
                pAttackingObject1->pSprite->setScaleX(pAttackingObject1->toward);
                pAttackingObject1->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
                pAttackingObject1->pSprite->setPosition( ccp(pAttackingObject1->x, pAttackingObject1->y) );
                m_pBulletNode->getChildByTag(207)->addChild(pAttackingObject1->pSprite, 3);
                pAttackingObject1->pSprite->setRotation( -pAttackingObject1->rotation * pAttackingObject1->toward /(PI_FIX));
                
                AttackingObject* pAttackingObject2 = new AttackingObject;
                m_vecAttackingObjectArray.push_back(pAttackingObject2);
                pAttackingObject2->toward	= pAttackingObject->toward;
                pAttackingObject2->type		= eWT_Pistol;
                pAttackingObject2->time		= 10;
                pAttackingObject2->life		= 20;
                pAttackingObject2->speed	= 10;
                pAttackingObject2->rotation	= pAttackingObject->rotation;
                pAttackingObject2->x		= pAttackingObject->x;
                pAttackingObject2->y		= pAttackingObject->y + 10.0f;
                pAttackingObject2->vx		= pAttackingObject->vx;
                pAttackingObject2->vy		= pAttackingObject->vy;
                pAttackingObject2->pSprite = CCSprite::create(bulletName);
                pAttackingObject2->pSprite->setScaleX(pAttackingObject2->toward);
                pAttackingObject2->pSprite->setAnchorPoint(ccp(0.05f, 0.5f));
                pAttackingObject2->pSprite->setPosition( ccp(pAttackingObject2->x, pAttackingObject2->y) );
                m_pBulletNode->getChildByTag(207)->addChild(pAttackingObject2->pSprite, 3);
                pAttackingObject2->pSprite->setRotation( -pAttackingObject2->rotation * pAttackingObject2->toward /(PI_FIX));
            }
        }
    }
}

int ccbShopMenu::FireIndex( int _type )
{
    int index = 0;
    switch ( _type )
    {
        case 3:
            index = 6;
            break;
        case 4:
            index = 5;
            break;
        case 5:
            index = 4;
            break;
        case 7:
            index = 0;
            break;
        case 8:
            index = 2;
            break;
        case 9:
            index = 1;
            break;
        case 10:
            index = 3;
            break;
        case 11:
            index = 7;
            break;
        case 12:
            index = -1;
        case 16:
        case 17:
            index = 8;
            break;
    }
    return index;
}

int ccbShopMenu::ActionIndex( int _type )
{
    int index = 0;
    if ( AppDelegate::s_HeroType == 2 )
    {
        index = 6;
    }
    else switch ( _type )
    {
        case 3:
        case 8:
        case 16:
        case 17:
            //index = 17;
            index = 6;
            break;
        case 4:
        case 5:
        case 7:
        case 10:
            //index = 18;
            index = 20;
            break;
        case 9:
        case 11:
        case 12://火枪
            //index = 19;
            index = 21;
            break;
    }
    return index;
}

int ccbShopMenu::Week( int _day )
{
    int week = _day/7;
    if ( week > 3 )
        week = 3;
    return week;
}

void ccbShopMenu::OnSaleMin( int _num )
{
    AppDelegate::s_OnSaleNum -= _num;
    while ( AppDelegate::s_OnSaleNum <= 0 /*&& AppDelegate::s_OnSaleTimes == 1*/ )
    {
        AppDelegate::s_OnSaleNum += 100;
        AppDelegate::s_OnSaleTimes = 0;
    }
    if ( AppDelegate::s_OnSaleTimes == 0 )
    {
        m_pOnSaleBoard->getChildByTag(811)->setVisible(false);
        m_pOnSaleBoard->getChildByTag(812)->setVisible(true);
    }
    else
    {
        m_pOnSaleBoard->getChildByTag(811)->setVisible(true);
        m_pOnSaleBoard->getChildByTag(812)->setVisible(false);
    }
    if ( AppDelegate::s_WeaponOwn[AppDelegate::s_ShopOnSell[0]] )
    {
        m_pOnSaleBoard->getChildByTag(801)->setVisible(false);
        m_pOnSaleBoard->getChildByTag(802)->setVisible(false);
        m_pOnSaleBoard->getChildByTag(803)->setVisible(true);
    }
    
    common::ShowNumberUpdate(m_pOnSaleBoard, AppDelegate::s_OnSaleNum, 887);
    int x = 0;
    if ( AppDelegate::s_OnSaleNum < 10 )
        x = 2;
    else if ( AppDelegate::s_OnSaleNum < 100 )
        x = 1;
    m_pOnSaleBoard->getChildByTag(887)->setPositionX(58+26.6f*x);
}

void ccbShopMenu::OnSaleDigit( float _t )
{
    m_iOnSaleTime --;
    if ( m_iOnSaleTime < 0 )
    {
        OnSaleMin(rand()%2);
        m_iOnSaleTime = 60 + rand()%300;
    }
}

void ccbShopMenu::OnSaleInit()
{
    CCSprite* pSprite1 = common::CreateAnimation("tu3/onsale/waikuang1.png", ccp(0,0), ccp(0,0),
                                                 "tu3/onsale/waikuang", 6, CCRect(0,0,464,264), true);
    m_pOnSaleBoard->addChild(pSprite1);
    
    common::ShowNumber(m_pOnSaleBoard, AppDelegate::s_OnSaleNum, 38, 65, 58, 26, "tu4/limitN1.png", 887, 0, 0.7f);
    
    CCMenuItem* pItem = CCMenuItemImage::create("tu3/onsale/anniu1.png", "tu3/onsale/anniu1.png", this, menu_selector(ccbShopMenu::OnSaleBuy));
    pItem->setAnchorPoint(ccp(0,0));
    CCMenu* pMenu = CCMenu::create(pItem, NULL);
    pMenu->setPosition(ccp(175, 27));
    m_pOnSaleBoard->addChild(pMenu, 0, 801);
    CCSprite* pSprite3 = common::CreateAnimation("tu3/onsale/a1.png", ccp(0,0), ccp(0,0),
                                                 "tu3/onsale/a", 5, CCRect(0,0,134,40), true);
    pSprite3->setPosition(ccp(175, 27));
    m_pOnSaleBoard->addChild(pSprite3, 0, 802);
    CCSprite* pSprite2 = CCSprite::create("tu3/onsale/yihuode.png");
    pSprite2->setAnchorPoint(ccp(0,0));
    pSprite2->setPosition(ccp(175, 27));
    pSprite2->setVisible(false);
    m_pOnSaleBoard->addChild(pSprite2, 0, 803);
    
    CCSprite* pSprite4 = CCSprite::create("tu3/onsale/xianshi.png");
    pSprite4->setAnchorPoint(ccp(0,0));
    pSprite4->setPosition(ccp(24,77));
    m_pOnSaleBoard->addChild(pSprite4, 0, 811);
    CCSprite* pSprite5 = CCSprite::create("tu3/onsale/jiatui.png");
    pSprite5->setAnchorPoint(ccp(0,0));
    pSprite5->setPosition(ccp(24,77));
    m_pOnSaleBoard->addChild(pSprite5, 0, 812);
    
    {
        struct cc_timeval now; 
        CCTime::gettimeofdayCocos2d(&now, NULL); 
        struct tm* tm;
		time_t timeTemp = now.tv_sec;
        tm = localtime(&timeTemp);
        int day = tm->tm_mday;
        int	hour = tm->tm_hour;
        
        if ( Week(day) != Week(AppDelegate::s_OnSaleDay) )
        {
            AppDelegate::s_ShopOnSell[0] = rand()%8+5;
            if ( AppDelegate::s_ShopOnSell[0] > 6 )
                AppDelegate::s_ShopOnSell[0]++;
            AppDelegate::s_OnSaleDay = Week(day)*7;
            AppDelegate::s_OnSaleHour = 0;
            AppDelegate::s_OnSaleTimes = 1;
            AppDelegate::s_OnSaleNum = 200;
        }
        
        int a = day - AppDelegate::s_OnSaleDay;
        int b = hour - AppDelegate::s_OnSaleHour;
        AppDelegate::s_OnSaleDay = day;
        AppDelegate::s_OnSaleHour = hour;
        int c = 0;
        for ( int j = 0; j < a*24+b; j++ )
        {
            c += rand()%4;
        }
        OnSaleMin(c);
    }
    
    int i = AppDelegate::s_ShopOnSell[0];
    float s = g_weaponSpeedShow[i]*(1+0.2f*AppDelegate::s_WeaponLevel[i]);
    int num = weapon_power[i] + weapon_upgrade[i]*AppDelegate::s_WeaponLevel[i];
    float scale = 0.8f;
    if ( num > 999 )
        scale = 0.6f;
    common::ShowNumber(m_pOnSaleBoard, num, 19, 24, 379, 37, "tu3/suzi.png", d_iShopWeaponTagDamage, 0, scale);
    
    char weaponName[255];
    sprintf(weaponName, "tu3/qiang/%d.png", i);
    CCSprite* pSprite6 = CCSprite::create(weaponName);
    pSprite6->setAnchorPoint(ccp(0,0));
    pSprite6->setPosition(ccp(110, 116));
    m_pOnSaleBoard->addChild(pSprite6);
    
    sprintf(weaponName, "tu3/name/%d.png", i);
    CCSprite* pSprite7 = CCSprite::create(weaponName);
    pSprite7->setAnchorPoint(ccp(0,0));
    pSprite7->setPosition(ccp(25, 205));
    m_pOnSaleBoard->addChild(pSprite7);
    
    CCSprite* pSprite8 = CCSprite::create("tu3/onsale/onsale.png");
    pSprite8->setAnchorPoint(ccp(0,0));
    pSprite8->setPosition(ccp(221,107));
    m_pOnSaleBoard->addChild(pSprite8);
    
    common::ShowNumber(m_pOnSaleBoard, g_priceUpgrade[i], 19, 24, 355, 137, "tu3/zidansuzi2.png", 891, 0, 0.8f);
    common::ShowNumber(m_pOnSaleBoard, g_priceUpgrade[i]*4/5, 19, 24, 348, 162, "tu3/zidansuzi2.png", 892);
    
    CCSprite* pSprite9 = CCSprite::create("tu3/onsale/hua.png");
    pSprite9->setAnchorPoint(ccp(0,0));
    pSprite9->setPosition(ccp(359,124));
    m_pOnSaleBoard->addChild(pSprite9);
    
    CCSprite* pSprite10 = NULL;
    if ( g_weaponUseMedal[i] )
        pSprite10 = CCSprite::create("tu3/qian.png");
    else
        pSprite10 = CCSprite::create("tu3/xunzhang.png");
    pSprite10->setAnchorPoint(ccp(0.5f,0.5f));
    pSprite10->setPosition(ccp(298,187));
    m_pOnSaleBoard->addChild(pSprite10);
    
    int type = Exclusive(i);
    if ( AppDelegate::s_HeroType != type && -1 != type )
    {
        char buffer[255];
        sprintf(buffer, "tu3/type/%d.png", type);
        CCSprite* pSprite = CCSprite::create(buffer);
        pSprite->setPosition(ccp(340, 80));
        m_pOnSaleBoard->addChild(pSprite, 5);
    }
}

void ccbShopMenu::OnSaleBuy(cocos2d::CCObject* sender)
{
    if ( m_bOnSell )
        return;
    int in = AppDelegate::s_ShopOnSell[0];
    if ( WeaponCanUse(in) )
    {
        if ( AppDelegate::s_WeaponOwn[in] == 0 )
        {
            m_iShowWeapon = AppDelegate::s_ShopOnSell[0];
            m_bWeapon = true;
            OnBuy(NULL);
            if ( AppDelegate::s_WeaponOwn[in] == 1 )
                OnSaleMin(1);
        }
    }
}

cocos2d::SEL_CallFuncN ccbShopMenu::onResolveCCBCCCallFuncSelector( cocos2d::CCObject * pTarget, const char* pSelectorName )
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "changeshow", ccbShopMenu::ChangeShow);
    return NULL;
}

void ccbShopMenu::ChangeShow( cocos2d::CCNode *pSender )
{
    m_bNewShowWeapon = true;
    SetWeaponShow(m_iNewShowWeapon, 1);
}

void ccbShopMenu::MoneyShow()
{
    float scale = 1.0f;
    int yfix = 0;
    if ( AppDelegate::s_Money > 99999 )
    {
        scale = 0.8f;
        yfix = 5;
    }
    common::ShowNumber(m_pNodeMoney, AppDelegate::s_Money, 19, 24, d_fNumLocateGold+yfix, "tu3/suzi.png", 888, 0, scale);
    
    scale = 1.0f;
    yfix = 0;
    if ( AppDelegate::s_Medal > 99999 )
    {
        scale = 0.8f;
        yfix = 5;
    }
    common::ShowNumber(m_pNodeMedal, AppDelegate::s_Medal, 19, 24, d_fNumLocateMedal+yfix, "tu3/suzi.png", 888, 0, scale);
}

bool ccbShopMenu::WeaponCanUse( int _type )
{
    bool can = false;
    if ( _type == 0 ||
        _type == 1 )
        can = true;
    else switch ( AppDelegate::s_HeroType )
    {
        case 0:
            if ( _type == 2||
                _type == 3 ||
                _type == 4 ||
                _type == 6 ||
                _type == 7 ||
                _type == 9 ||
                _type == 11 )
                can = true;
            break;
        case 1:
            if ( _type == 14 ||
                _type == 13 ||
                _type == 16 ||
                _type == 17 )
                can = true;
            break;
        case 2:
            if ( 
                _type == 5 ||
                _type == 8 ||
                _type == 10 ||
                _type == 12	||
                _type == 15 )
                can = true;
            break;
    }
    
    return can;
}

int ccbShopMenu::Exclusive( int _type )
{
    int i = -1;
    switch ( _type )
    {
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 9:
        case 11:
            i = 0;
            break;
        case 13:
        case 14:
        case 16:
        case 17:
            i = 1;
            break;
        case 5:
        case 8:
        case 10:
        case 12:
        case 15:
            i = 2;
            break;
    }
    return i;
}

void ccbShopMenu::GuideTalk( int _index , int _type )
{
    int yfix = 0;
    if ( _type == 1 )
        yfix = -100;
    if ( m_pBulletNode->getChildByTag(490) )
        m_pBulletNode->getChildByTag(490)->removeFromParentAndCleanup(true);
    m_iGuideTalk = _index;
    CCSprite* pTipBoard = CCSprite::create("guide/dikuang2.png");
    pTipBoard->setAnchorPoint(ccp(0,0));
    pTipBoard->setPosition(ccp(400, 290+yfix));
    m_pBulletNode->addChild(pTipBoard, 30, 490);
    
    if ( m_pBulletNode->getChildByTag(491) )
        m_pBulletNode->getChildByTag(491)->removeFromParentAndCleanup(true);
    CCSprite* pNPC = common::CreateAnimation("guide/zhihui1.png", ccp(660, 300+yfix), ccp(0.0f, 0.0f),
                                             "guide/zhihui", 4, CCRect(0,0,138,158), false);
    m_pBulletNode->addChild(pNPC, 30, 491);
    
    if ( m_pBulletNode->getChildByTag(492) )
        m_pBulletNode->getChildByTag(492)->removeFromParentAndCleanup(true);
    char buffer[255];
    sprintf(buffer, "guide/shop/%d.png", _index);
    CCSprite* pTip = CCSprite::create(buffer);
    pTip->setAnchorPoint(ccp(0,0));
    pTip->setPosition(ccp(420, 310+yfix));
    m_pBulletNode->addChild(pTip, 30, 492);
    
    schedule(schedule_selector(ccbShopMenu::GuideTalkNext), 2.0f);
}

void ccbShopMenu::GuideFingerCreate( int _x, int _y )
{
    if ( m_pBulletNode->getChildByTag(493) )
        m_pBulletNode->getChildByTag(493)->removeFromParentAndCleanup(true);
    CCSprite* pSprite = common::CreateAnimation("guide/shop/shou1.png", ccp(_x,_y), ccp(0.9f,0.95f),
                                                "guide/shop/shou", 2, CCRect(0,0,94,125), true);
    m_pBulletNode->addChild(pSprite, 50, 493);
}

void ccbShopMenu::GuideFingerClear()
{
    if ( m_pBulletNode->getChildByTag(490) )
        m_pBulletNode->getChildByTag(490)->removeFromParentAndCleanup(true);
    if ( m_pBulletNode->getChildByTag(491) )
        m_pBulletNode->getChildByTag(491)->removeFromParentAndCleanup(true);
    if ( m_pBulletNode->getChildByTag(492) )
        m_pBulletNode->getChildByTag(492)->removeFromParentAndCleanup(true);
    if ( m_pBulletNode->getChildByTag(493) )
        m_pBulletNode->getChildByTag(493)->removeFromParentAndCleanup(true);
    if ( m_pBulletNode->getChildByTag(494) )
        m_pBulletNode->getChildByTag(494)->removeFromParentAndCleanup(true);
}

void ccbShopMenu::GuideFingerMove()
{
    m_pBulletNode->getChildByTag(493)->setPosition(ccp(372, 260));
    CCMoveBy* pAction = CCMoveBy::create(1.0f, ccp(-200, 0));
    CCCallFunc* pCallFunc = CCCallFunc::create(this, callfunc_selector(ccbShopMenu::GuideFingerMove));
    m_pBulletNode->getChildByTag(493)->runAction(CCSequence::create(pAction, pCallFunc, NULL));
}

void ccbShopMenu::GuideCheck()
{
    if ( m_bGuideClose )
        return;
    if ( m_iGuideTime > 0 )
    {
        m_iGuideTime--;
        return;
    }
    
    switch ( m_iGuideIndex )
    {
        case 0:
            GuideTalk(1);
            m_iGuideIndex = 1;
            break;
        case 2:
        {
            CCSprite* pSprite = CCSprite::create("guide/shop/shou2.png");
            pSprite->setPosition(ccp(372, 260));
            pSprite->setAnchorPoint(ccp(0.9f,0.95f));
            m_pBulletNode->addChild(pSprite, 50, 493);
            CCSprite* pSprite1 = CCSprite::create("guide/shop/fangxiang.png");
            pSprite1->setPosition(ccp(345, 270));
            pSprite1->setAnchorPoint(ccp(1.0f,0.5f));
            m_pBulletNode->addChild(pSprite1, 49, 494);
            m_iGuideTalk = 1;
            GuideFingerMove();
            m_iGuideIndex = 3;
            GuideTalk(2);
        }
            break;
        case 4:
            GuideTalk(3);
            GuideFingerCreate(207, 366);
            m_iGuideIndex = 5;
            break;
        case 6:
            GuideTalk(4);
            GuideFingerCreate(334, 189);
            m_iGuideIndex = 7;
            break;
        case 9:
            GuideTalk(5);
            GuideFingerCreate(207, 184);
            m_iGuideIndex = 10;
            break;
        case 11:
            GuideTalk(6, 1);
            GuideFingerCreate(467, 412);
            m_iGuideIndex = 12;
            break;
        case 13:
            GuideFingerCreate(204, 189);
            m_iGuideBuyTime = 3;
            m_iGuideIndex = 14;
            break;
        case 15:
            GuideTalk(7, 1);
            GuideFingerCreate(88, 404);
            m_iGuideIndex = 16;
            break;
    }
}

void ccbShopMenu::GuideTalkNext( float _t /*= 0*/ )
{
    unschedule(schedule_selector(ccbShopMenu::GuideTalkNext));
    switch( m_iGuideTalk )
    {
        case 0:
            GuideFingerClear();
            m_iGuideIndex = 2;
            break;
        case 1:
        {
            GuideFingerClear();
            CCSprite* pSprite = common::CreateAnimation("guide/shop/xianshi1.png", ccp(0,0), ccp(0,0),
                                                        "guide/shop/xianshi", 2, CCRect(0,0,800,480), true);
            m_pBulletNode->addChild(pSprite, 50, 493);
            m_iGuideTalk = 0;
            schedule(schedule_selector(ccbShopMenu::GuideTalkNext), 4.0f);
        }
            break;
            //case 2:
            //GuideFingerClear();
            //m_iGuideIndex = 2;
            //break;
    }
}

void ccbShopMenu::GuideShow()
{
    
}

void ccbShopMenu::disappear()
{
    getChildByTag(101)->stopAllActions();
    removeChildByTag(101);
}

void ccbShopMenu::Buy1( cocos2d::CCObject* sender )
{
    if ( !m_bGuideClose )
    {
        if ( m_iGuideIndex != 14 )
            return;
        else
        {
            if ( m_iGuideBuyTime > 0 )
                m_iGuideBuyTime--;
            else
            {
                m_iGuideIndex = 15;
                GuideFingerClear();
            }
        }
    }
    Buy(1);
}

void ccbShopMenu::Buy2( cocos2d::CCObject* sender )
{
    if ( !m_bGuideClose )
        return;
    Buy(10);
}

void ccbShopMenu::Buy3( cocos2d::CCObject* sender )
{
    if ( !m_bGuideClose )
        return;
    if ( AppDelegate::s_BulletNum[m_iShowWeapon] >= 999 )
        return;
    int num = (999 - AppDelegate::s_BulletNum[m_iShowWeapon]) / g_iWeaponBulletOnce[m_iShowWeapon] + 1;
    Buy(num);
}

void ccbShopMenu::Buy( int _num )
{
    if ( m_bOnSell )
        return;
    if ( AppDelegate::s_BulletNum[m_iShowWeapon] >= 999 )
        return;
    if ( AppDelegate::s_Money < g_priceBullet[m_iShowWeapon]*_num )
    {//花钱
        GetMoney();
        return;
    }
    AppDelegate::AudioPlayEffect("MS2/buy.mp3");
    AppDelegate::s_Money -= g_priceBullet[m_iShowWeapon]*_num;
    CCNode* pNode = m_pNodeShowBoard->getChildByTag(d_iShopShowTagNum);
    pNode->stopAllActions();
    pNode->setScale(2.0f);
    CCActionInterval* pAction2 = CCScaleTo::create(0.3f, 1.0f);
    pNode->runAction( pAction2 );
    
    AppDelegate::s_BulletNum[m_iShowWeapon] += g_iWeaponBulletOnce[m_iShowWeapon]*_num;
    if ( AppDelegate::s_BulletNum[m_iShowWeapon] > 999 )
        AppDelegate::s_BulletNum[m_iShowWeapon] = 999;
    AppDelegate::SaveBullet(m_iShowWeapon);
    SetWeaponShow(m_iShowWeapon, 1);
    MoneyShow();
}

void ccbShopMenu::GetMoney( cocos2d::CCObject* sender )
{
#ifdef NoGetMoney
    return;
#endif
    char buffer[255];
    sprintf(buffer, "%d", AppDelegate::s_FirstLogin);
    if ( AppDelegate::s_FirstLogin == 3 )
        return;
    char buffer1[255];
    sprintf(buffer1, "%d", m_bOnSell);
    if ( m_bOnSell )
        return;
    m_bOnSell = true;
    
    CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
    addChild(pCCLayerColor, 29, 1195);
    
    CCMenuItem* pItem = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbShopMenu::GetRemove));
    pItem->setPosition(ccp(723,351));
    CCMenu* pMenu = CCMenu::create(pItem, NULL);
    pMenu->setPosition(ccp(0,0));
    addChild(pMenu, 31, 1196);
    
    CCSprite* pSprite1 = CCSprite::create("sell/money/di.png");
    pSprite1->setPosition(ccp(400, 240));
    addChild(pSprite1, 31, 1199);
    CCSprite* pSprite2 = CCSprite::create("sell/money/2.png");
    pSprite2->setPosition(ccp(400, 240));
    addChild(pSprite2, 33, 1198);
    
    //CCMenuItem* pItem1 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMoneyBtn));
    //pItem1->setPosition(ccp(219,282));
    //pItem1->setTag(1);
    CCMenuItem* pItem2 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMoneyBtn));
    pItem2->setPosition(ccp(243,199));
    pItem2->setTag(2);
    CCMenuItem* pItem3 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMoneyBtn));
    pItem3->setPosition(ccp(556,199));
    pItem3->setTag(3);
    //CCMenuItem* pItem4 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMoneyBtn));
    //pItem4->setPosition(ccp(286,96));
    //pItem4->setTag(4);
    //CCMenuItem* pItem5 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMoneyBtn));
    //pItem5->setPosition(ccp(512,96));
    //pItem5->setTag(5);
    CCMenu* pMenu1 = CCMenu::create(/*pItem1, */pItem2, pItem3, /*pItem4, pItem5,*/ NULL);
    pMenu1->setPosition(ccp(0,0));
    addChild(pMenu1, 32, 1197);
    

	w_beforeGroupIndex = w_groupIndex;
	w_groupIndex = 4;
	isInRecharge = true;
	w_rechargeIndex = 0;
	w_beforePosX = getChildByTag(1001)->getPositionX();
	w_beforePosY = getChildByTag(1001)->getPositionY();
	setFignerPos(ccp(243,199));
}

void ccbShopMenu::GetMedal( cocos2d::CCObject* sender )
{
#ifdef NoGetMoney
    return;
#endif
    if ( AppDelegate::s_FirstLogin == 3 )
        return;
    if ( m_bOnSell )
        return;
    m_bOnSell = true;
    
    CCLayerColor* pCCLayerColor = CCLayerColor::create(ccc4(20,20,20,230), 800, 480);
    addChild(pCCLayerColor, 29, 1195);
    
    CCMenuItem* pItem = CCMenuItemImage::create("sell/guan.png", "sell/guan.png", this, menu_selector(ccbShopMenu::GetRemove));
    pItem->setPosition(ccp(723,351));
    CCMenu* pMenu = CCMenu::create(pItem, NULL);
    pMenu->setPosition(ccp(0,0));
    addChild(pMenu, 31, 1196);
    
    CCSprite* pSprite1 = CCSprite::create("sell/money/di.png");
    pSprite1->setPosition(ccp(400, 240));
    addChild(pSprite1, 31, 1199);
    CCSprite* pSprite2 = CCSprite::create("sell/money/1.png");
    pSprite2->setPosition(ccp(400, 240));
    addChild(pSprite2, 33, 1198);
    
    //CCMenuItem* pItem1 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMedalBtn));
    //pItem1->setPosition(ccp(219,282));
    //pItem1->setTag(1);
    CCMenuItem* pItem2 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMedalBtn));
    pItem2->setPosition(ccp(243,199));
    pItem2->setTag(2);
    CCMenuItem* pItem3 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMedalBtn));
    pItem3->setPosition(ccp(556,199));
    pItem3->setTag(3);
    //CCMenuItem* pItem4 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMedalBtn));
    //pItem4->setPosition(ccp(283,96));
    //pItem4->setTag(4);
    //CCMenuItem* pItem5 = CCMenuItemImage::create("sell/money/goumia.png", "sell/money/goumia2.png", this, menu_selector(ccbShopMenu::GetMedalBtn));
    //pItem5->setPosition(ccp(512,96));
    //pItem5->setTag(5);
    CCMenu* pMenu1 = CCMenu::create(/*pItem1, */pItem2, pItem3, /*pItem4, pItem5, */NULL);
    pMenu1->setPosition(ccp(0,0));
    addChild(pMenu1, 32, 1197);

	w_beforeGroupIndex = w_groupIndex;
	w_groupIndex = 5;
	isInRecharge = true;
	w_rechargeIndex = 0;
	w_beforePosX = getChildByTag(1001)->getPositionX();
	w_beforePosY = getChildByTag(1001)->getPositionY();
	setFignerPos(ccp(243,199));
}

void ccbShopMenu::GetMoneyBtn( cocos2d::CCObject* _pNode )
{
    if ( _pNode )
    {
        int numAdd = 0;
        int numMid = 0;
        int index = ((CCNode*)_pNode)->getTag();
        PayService::pay(14+index);
        
        //switch ( index )
        //{
        //case 1:
        //	numAdd = 1080;
        //	numMid = 2;
        //	break;
        //case 2:
        //	numAdd = 6480;
        //	numMid = 14;
        //	break;
        //case 3:
        //	numAdd = 35280;
        //	numMid = 49;
        //	break;
        //case 4:
        //	numAdd = 80190;
        //	numMid = 99;
        //	break;
        //case 5:
        //	numAdd = 181440;
        //	numMid = 168;
        //	break;
        //}
        //AppDelegate::s_Money += numAdd;
        //AppDelegate::SaveMoney();
    }
    else
    {
        MoneyShow();
        GetRemove();
    }
}

void ccbShopMenu::GetMedalBtn( cocos2d::CCObject* _pNode )
{
    if ( _pNode )
    {
        int numAdd = 0;
        int numMid = 0;
        int index = ((CCNode*)_pNode)->getTag();
        PayService::pay(11+index);
        
        //switch ( index )
        //{
        //case 1:
        //	numAdd = 108;
        //	numMid = 2;
        //	break;
        //case 2:
        //	numAdd = 648;
        //	numMid = 14;
        //	break;
        //case 3:
        //	numAdd = 3528;
        //	numMid = 49;
        //	break;
        //case 4:
        //	numAdd = 8019;
        //	numMid = 99;
        //	break;
        //case 5:
        //	numAdd = 18144;
        //	numMid = 168;
        //	break;
        //}
        //AppDelegate::s_Medal += numAdd;
        //AppDelegate::SaveMedal();
    }
    else
    {
        MoneyShow();
        GetRemove();
    }
}

void ccbShopMenu::payMoneyBtn(int tag){
	if(tag>=0){
		 int numAdd = 0;
        int numMid = 0;
        PayService::pay(14+tag);
	}else{
		MoneyShow();
        GetRemove();
	}

}

void ccbShopMenu::payMedalBtn(int tag){
	if(tag>=0){
		int numAdd = 0;
        int numMid = 0;
        PayService::pay(11+tag);
	}else{
		MoneyShow();
        GetRemove();
	}
}

void ccbShopMenu::GetRemove(cocos2d::CCObject* _pNode)
{
    if ( !m_bOnSell )
        return;
    m_bOnSell = false;
    for ( int i = 1195; i < 1200; i++ )
    {
        if ( getChildByTag(i) )
            removeChildByTag(i);
    }
	w_groupIndex = w_beforeGroupIndex;
	isInRecharge = false;
	w_rechargeIndex = 0;
	
	setFignerPos(ccp(w_beforePosX,w_beforePosY));
}
