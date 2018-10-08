#include "GameControler.h"
#include "baseRes.h"
#include "Hero.h"
#include "CSArmatureDataManager.h"
#include "AppDelegate.h"

extern CFlashControl* g_pHero;
using namespace cs;
using namespace cocos2d;

std::string s_BotBone[] =
{
    "kaiqiangshou",
    "huohua1",
    "huohua2",
};

CHero::CHero()
{
    m_unLastAction	= 0;
    m_fRotation		= 0.0f;
    m_pArmature		= NULL;
    m_pFire			= NULL;
    m_pVIP			= NULL;
    m_pFather		= NULL;
    m_pArmaArray	= new CCArray();
    m_pBloodArray	= new CCArray();
    memset(m_iHashMonster, 0, sizeof(int)*255);
    memset(m_iHashObject, 0, sizeof(int)*255);
    
    if ( AppDelegate::s_HeroType == 0 )
    {
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("chuanqi", "", "zhujiao.png", "zhujiao.plist", "zhujiao.json");
        ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("zhujiao_1.plist", "zhujiao_1.png");
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("qiangqiang", "", "qiangqiang.png", "qiangqiang.plist", "qiangqiang.json");
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("tulongdao1", "", "tulongdao.png", "tulongdao.plist", "tulongdao.json");
    }
    else if ( AppDelegate::s_HeroType == 1 )
    {
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("chuanqi", "", "zhujiao2.png", "zhujiao2.plist", "zhujiao2.json");
#ifdef Old_Version
        ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("zhujiao2_1.plist", "zhujiao2_1.png");
#endif
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("qiangqiang", "", "qiangqiang2.png", "qiangqiang2.plist", "qiangqiang2.json");
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("tulongdao1", "", "tulongdao2.png", "tulongdao2.plist", "tulongdao2.json");
    }
    else if ( AppDelegate::s_HeroType == 2 )
    {
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("chuanqi", "", "bot.png", "bot.plist", "bot.json");
        ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("bot1.plist", "bot1.png");
    }
    
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("qianghuohua1", "", "qianghuo.png", "qianghuo.plist", "qianghuo.json");
    CreateHero();
}

void CHero::CreateHero()
{
    if ( AppDelegate::s_HeroType == 0 )
        m_pArmature = Armature::create("dongzuo1");
    else
        m_pArmature = Armature::create("dongzuo");
    if ( AppDelegate::s_HeroType != 2 )
        m_pArmature->getBone("shou1")->setScale(1.25f);
    
    m_pArmature->setPosition( 0, 0 );
    CGameControler::GetGameControler()->m_pCFrontgroud->GetTileMap()->addChild(m_pArmature, 4);
    RunAction(1);
    
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
        
        std::string weapon1[] = {"tulongdao2.png", "tulongdao111.png", "kaishanfu111.png", "ft.png",};
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
    
    m_pFire = Armature::create("qianghuohua");
    CGameControler::GetGameControler()->m_pCFrontgroud->GetTileMap()->addChild(m_pFire, 4);
    m_unLastAction = 0;
}

CHero::~CHero()
{
    //m_pArmature->removeFromParentAndCleanup(true);
    //m_pFire->removeFromParentAndCleanup(true);
    if ( m_pVIP )
    {
        m_pVIP->removeFromParentAndCleanup(true);
    }
    CCObject *_object = NULL;
    CCARRAY_FOREACH(m_pArmaArray, _object)
    {
        Armature *armature = (Armature*)_object;
        armature->removeFromParentAndCleanup(true);
    }
    CCARRAY_FOREACH(m_pBloodArray, _object)
    {
        Armature *armature = (Armature*)_object;
        armature->removeFromParentAndCleanup(true);
    }
    m_pArmaArray->removeAllObjects();
    delete m_pArmaArray;
    m_pBloodArray->removeAllObjects();
    delete m_pBloodArray;
    ArmatureDataManager::sharedArmatureDataManager()->removeAll();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ArmatureDataManager::sharedArmatureDataManager()->purgeArmatureSystem();
#endif
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
}

void CHero::InitMonster()
{
    CCArray* pArray = ((CGameControler*)m_pFather)->m_pCFrontgroud->m_pMonsterArray;
    for ( unsigned int i = 0; i < pArray->count(); i++ )
    {
        CCDictionary* pObject = (CCDictionary*)pArray->objectAtIndex(i);
        int kind = pObject->valueForKey("id")->uintValue()%100 - 1;
#ifdef GameTypeC
        if ( kind == 4 || kind == 6 )
            continue;
        if ( kind != 5 && kind != 8 )
            kind = kind%2;
#endif
        if ( m_iHashMonster[kind] )
            continue;
#ifdef GameTypeC
        if ( kind != 0 && kind != 1 && kind != 5 && kind != 8 )
            continue;
        if ( kind == 5 || kind == 8 )
#else
            if ( kind == 5 )
#endif
            {
                ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss1dz1.plist", "Monster/jsboss1dz1.png");
#ifdef GameTypeC
                int in = 0;
#else
                int in = 4;
#endif
                if ( !m_iHashMonster[in] )
                {
                    char Buffer[255];
                    m_iHashMonster[in] = 1;
                    sprintf(Buffer,"monster%d",in);
                    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(Buffer, "", g_cMonsterPath[in].c_str(), g_cMonsterPlist[in].c_str(),g_cMonsterXML[in].c_str());
                }
            }
            else if ( kind == 8 )
            {
                ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Boss2_1", "", "Monster/jsboss2_1.png", "Monster/jsboss2_1.plist", "Monster/jsboss2_1.json");
                ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Boss2_2", "", "Monster/jsboss2_2.png", "Monster/jsboss2_2.plist", "Monster/jsboss2_2.json");
                ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Boss2_3", "", "Monster/jsboss2_3.png", "Monster/jsboss2_3.plist", "Monster/jsboss2_3.json");
            }
            else if ( kind == 12 )
            {
                ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss3_2.plist", "Monster/jsboss3_2.png");
                ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss3_3.plist", "Monster/jsboss3_3.png");
            }
            else if ( kind == 13 )
            {
                ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss4_2.plist", "Monster/jsboss4_2.png");
                ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss4_3.plist", "Monster/jsboss4_3.png");
            }
            else if ( kind == 14 )
            {
                ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss5_2.plist", "Monster/jsboss5_2.png");
                ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("monster16", "", g_cMonsterPath[16].c_str(), g_cMonsterPlist[16].c_str(),g_cMonsterXML[16].c_str());
                int in = 2;
                if ( !m_iHashMonster[in] )
                {
                    char Buffer[255];
                    m_iHashMonster[in] = 1;
                    sprintf(Buffer,"monster%d",in);
                    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(Buffer, "", g_cMonsterPath[in].c_str(), g_cMonsterPlist[in].c_str(),g_cMonsterXML[in].c_str());
                }
                in = 3;
                if ( !m_iHashMonster[in] )
                {
                    char Buffer[255];
                    m_iHashMonster[in] = 1;
                    sprintf(Buffer,"monster%d",in);
                    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(Buffer, "", g_cMonsterPath[in].c_str(), g_cMonsterPlist[in].c_str(),g_cMonsterXML[in].c_str());
                }
                in = 7;
                if ( !m_iHashMonster[in] )
                {
                    char Buffer[255];
                    m_iHashMonster[in] = 1;
                    sprintf(Buffer,"monster%d",in);
                    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(Buffer, "", g_cMonsterPath[in].c_str(), g_cMonsterPlist[in].c_str(),g_cMonsterXML[in].c_str());
                }
                in = 15;
                if ( !m_iHashMonster[in] )
                {
                    char Buffer[255];
                    m_iHashMonster[in] = 1;
                    sprintf(Buffer,"monster%d",in);
                    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(Buffer, "", g_cMonsterPath[in].c_str(), g_cMonsterPlist[in].c_str(),g_cMonsterXML[in].c_str());
                }
            }
            else if ( kind == 17 )
            {
                ArmatureDataManager::sharedArmatureDataManager()->addSpriteFrameFromFile("Monster/jsboss6_2.plist", "Monster/jsboss6_2.png");
            }
        m_iHashMonster[kind] = 1;
        char Buffer[255];
        sprintf(Buffer,"monster%d",kind);
        ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(Buffer, "", g_cMonsterPath[kind].c_str(), g_cMonsterPlist[kind].c_str(),g_cMonsterXML[kind].c_str());
    }
    
    MapObject* pMapObjectArray = ((CGameControler*)m_pFather)->m_pCFrontgroud->m_pObjectArray;
    for ( int i = 0; i < ((CGameControler*)m_pFather)->m_pCFrontgroud->m_unObjectArrayNum; i++ )
    {
        MapObject* pMapObject = &pMapObjectArray[i];
        int kind = pMapObject->unKind;
        if ( m_iHashObject[kind] )
            continue;
        switch ( kind )
        {
                //case 0:
                //	ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("zhedang", "", "Tile/zhedang.png", "Tile/zhedang.plist", "Tile/zhedang.json");
                //	break;
            case 12:
                ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("shirenhua", "", "Tile/shirenhua.png", "Tile/shirenhua.plist", "Tile/shirenhua.json");
                break;
            case 18:
                ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("js12dz", "", "Tile/fish.png", "Tile/fish.plist", "Tile/fish.json");
                break;
        }
        m_iHashObject[kind] = 1;
    }
}

cs::Armature* CHero::CreateVIP()
{
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("Boss2_1", "", "Role/vip.png", "Role/vip.plist", "Role/vip.json");
    m_pVIP = Armature::create("nvrenzhidongzuo");
    return m_pVIP;
}

void CHero::SetMonster(Armature* _p)
{
    CGameControler::GetGameControler()->m_pCFrontgroud->GetTileMap()->addChild(_p, 3);
    //_p->getAnimation()->setAnimationScale(0.4);
    m_pArmaArray->addObject(_p);
}

void CHero::RemoveMonster(Armature* _p)
{
    if ( !_p )
        return;
    //removeChild(_p);
    _p->getAnimation()->stop();
    _p->removeFromParentAndCleanup(true);
    m_pArmaArray->removeObject(_p, true);
}

void CHero::SetMonsterBlood(Armature* _p)
{
    //addChild(_p);
    //m_pBloodArray->addObject(_p);
}

void CHero::RemoveMonsterBlood()
{
    //CCObject *_object = NULL;
    //CCARRAY_FOREACH(m_pBloodArray, _object)
    //{
    //	((Armature*)_object)->getAnimation()->stop();
    //	removeChild((Armature*)_object);
    //}
    //removeChild(_p);
    //_p->getAnimation()->stop();
    //m_pBloodArray->removeObject(_p, true);
}

Armature* CHero::GetArmature()
{
    return m_pArmature;
}

Armature* CHero::GetFire()
{
    return m_pFire;
}

void CHero::ChangeWeapon( int _index )
{
    if ( AppDelegate::s_HeroType < 2 )
    {
        int index = AppDelegate::WeaponIndex(_index);
        if ( index >= 0 )
        {
            if ( index < 20 )
                m_pArmature->getBone("shou1")->changeDisplayByIndex(index, true);
            else
                m_pArmature->getBone("dao")->changeDisplayByIndex(index-20, true);
        }
    }
}

void CHero::RunAction(int _type,int _frame)
{
    if ( !_type || _type == m_unLastAction )
        return;
    if ( m_unLastAction )
        m_pArmature->getAnimation()->stop();
    if ( !_frame )
        _frame = d_unHeroFrame;
    m_pArmature->getAnimation()->playByIndex(_type-1, 0, _frame);
    m_unLastAction = _type;
}

void CHero::PauseAction()
{
    PauseHero();
    PauseMonster();
}

void CHero::PauseHero()
{
    m_pArmature->getAnimation()->pause();
    m_pFire->getAnimation()->pause();
}

void CHero::PauseMonster()
{
    CCObject *_object = NULL;
    CCARRAY_FOREACH(m_pArmaArray, _object)
    {
        Armature *armature = (Armature*)_object;
        armature->getAnimation()->pause();
    }
    CCARRAY_FOREACH(m_pBloodArray, _object)
    {
        Armature *armature = (Armature*)_object;
        armature->getAnimation()->pause();
    }
}

void CHero::ResumeAction()
{
    ResumeHero();
    ResumeMonster();
}

void CHero::ResumeHero()
{
    m_pArmature->getAnimation()->resume();
    m_pFire->getAnimation()->resume();
}

void CHero::ResumeMonster()
{
    CCObject *_object = NULL;
    CCARRAY_FOREACH(m_pArmaArray, _object)
    {
        Armature *armature = (Armature*)_object;
        armature->getAnimation()->resume();
    }
    CCARRAY_FOREACH(m_pBloodArray, _object)
    {
        Armature *armature = (Armature*)_object;
        armature->getAnimation()->resume();
    }
}

void CHero::HeroAim(float _rotation, float _toward)
{
    m_fRotation = _rotation;
    //m_fRotation++;
    if ( AppDelegate::s_HeroType != 2 )
    {
        for ( int i = 0; i < d_iRotateObjectNum; i++ )
        {
            m_pArmature->getBone(d_sRotateObject[i].c_str())->setRotation(-m_fRotation);
        }
        for ( int i = 0; i < d_iRotateFireNum; i++ )
        {
            cs::Bone* pBone = m_pFire->getBone(d_sRotateFire[i].c_str());
            pBone->setRotation(-m_fRotation);
        }
    }
    else
    {
        m_pArmature->getBone("kaiqiangshou")->setRotation(-m_fRotation);
    }
    if ( m_pFire )
    {
        float x = 0.0f;
        float y = 0.0f;
        switch ( m_unLastAction )
        {
            case 1:
            case 6:
            case 17:
                break;
            case 2:
            case 14:
                if ( AppDelegate::s_HeroType == 0 )
                    y = d_fRunHigher;
                else if ( AppDelegate::s_HeroType == 1 )
                    y = 2;
                break;
            case 4:
            case 7:
            case 18:
                if ( AppDelegate::s_HeroType != 2 )
                    y = d_fJumpHigher;
                break;
            default:
                break;
        }
        float rotation = m_fRotation*PI_FIX;
        float cx = ((CGameControler*)m_pFather)->m_iFireX[g_pHero->m_unNowWeapon-1];
        float cy = ((CGameControler*)m_pFather)->m_iFireY[g_pHero->m_unNowWeapon-1];
        if ( AppDelegate::s_HeroType == 1 )
        {
            if ( AppDelegate::s_WeaponUse[1] == 3 && ((CGameControler*)m_pFather)->m_ewtNowWeapon == 1 )
            {
                if ( ((CGameControler*)m_pFather)->m_iHeroJumps != 0 )
                {
                    cx += 2;
                    cy += 6;
                }
            }
        }
        x = _toward * cx * cosf(rotation);
        y += cy+cx*sinf(rotation);
        m_pFire->setPosition(x+((CGameControler*)m_pFather)->m_fHeroLocateX, y+((CGameControler*)m_pFather)->m_fHeroLocateY);
    }
}

int CHero::GetAction()
{
    return m_unLastAction;
}

float CHero::GetRotation()
{
    return m_fRotation;
}

void CHero::ClearMonster(bool _remove)
{
    CCObject *_object = NULL;
    if ( _remove )
    {
        CCARRAY_FOREACH(m_pArmaArray, _object)
        {
            Armature *armature = (Armature*)_object;
            armature->removeFromParentAndCleanup(true);
        }
    }
    m_pArmaArray->removeAllObjects();
}

int CHero::MonsterCount()
{
    return m_pArmaArray->count();
}

void CHero::RemoveHero()
{
    m_pArmature->getAnimation()->stop();
    m_pArmature->removeFromParentAndCleanup(true);
    m_pFire->getAnimation()->stop();
    m_pFire->removeFromParentAndCleanup(true);
}
