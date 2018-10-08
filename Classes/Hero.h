#ifndef _Hero_H_
#define _Hero_H_

#include "cocos2d.h"
#include "CSArmature.h"

class CHero : public cocos2d::CCLayer
{
public:
    CHero();
    ~CHero();
    
    //void SetArmature(cs::Armature* _p);
    //void SetFire(cs::Armature* _p);
    void InitMonster();
    void SetMonster(cs::Armature* _p);
    void RemoveMonster(cs::Armature* _p);
    void SetMonsterBlood(cs::Armature* _p);
    void RemoveMonsterBlood();
    void RemoveHero();
    void PauseAction();
    void PauseHero();
    void PauseMonster();
    void ResumeAction();
    void ResumeHero();
    void ResumeMonster();
    void CreateHero();
    cs::Armature* CreateVIP();
    cs::Armature* GetArmature();
    cs::Armature* GetFire();
    void ChangeWeapon(int _index);
    void ClearMonster(bool _remove = true);
    
    void RunAction(int _type, int _frame = 0 );
    void HeroAim(float _rotation, float _toward);
    int GetAction();
    float GetRotation();
    
    int MonsterCount();
    cocos2d::CCLayer* m_pFather;
    int				m_unLastAction;
    float			m_fRotation;
private:
    int				m_iHashMonster[255];
    int				m_iHashObject[255];
    float			m_fBaseLength;
    cs::Armature*	m_pArmature;
    cs::Armature*	m_pFire;
    cs::Armature*	m_pVIP;
    cocos2d::CCArray* m_pArmaArray;
    cocos2d::CCArray* m_pBloodArray;
};

#endif
