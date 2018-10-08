#include "AchieveAdd.h"
#include "AppDelegate.h"

void AchieveAdd::Add( int _index, int _num /*= 0*/, int _level /*= 0*/ )
{
	switch ( _index )
	{
	case 0:
	case 1:
	case 2:
	case 3://近战杀
		for ( int i = 0; i < 4; i++ )
			add(i);
		break;
	case 4:
	case 5:
	case 6:
	case 7://远程杀
		for ( int i = 4; i < 8; i++ )
			add(i);
		break;
	case 8:
	case 9:
	case 10:
	case 11://手雷
		for ( int i = 8; i < 12; i++ )
			add(i);
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20://无伤
		if ( AppDelegate::s_LevelPerfect[_level] == 0 )
		{
			AppDelegate::s_LevelPerfect[_level] = 1;
			for ( int i = 16; i < 21; i++ )
				add(i);
		}
		break;
	case 21:
	case 22:
	case 23:
	case 24:
	case 25://不开枪
		if ( AppDelegate::s_LevelUnfire[_level] == 0 )
		{
			AppDelegate::s_LevelUnfire[_level] = 1;
			for ( int i = 21; i < 26; i++ )
				add(i);
		}
		break;
	case 26:
	case 27://太极之子过关
		if ( AppDelegate::s_LevelHero2[_level] == 0 )
		{
			AppDelegate::s_LevelHero2[_level] = 1;
			for ( int i = 26; i < 28; i++ )
				add(i);
		}
		break;
	case 28:
	case 29:
		if ( AppDelegate::s_LevelHero3[_level] == 0 )
		{
			AppDelegate::s_LevelHero3[_level] = 1;
			for ( int i = 28; i < 30; i++ )
				add(i);
		}
		break;
	case 40:
	case 41:
	case 42:
	case 43://技能1
		for ( int i = 40; i < 44; i++ )
			add(i);
		break;
	case 44:
	case 45:
	case 46:
	case 47://技能2
		for ( int i = 44; i < 48; i++ )
			add(i);
		break;
	case 48:
	case 49:
	case 50:
	case 51://技能3
		for ( int i = 48; i < 52; i++ )
			add(i);
		break;
	case 52:
	case 53:
	case 54:
		for ( int i = 52; i < 55; i++ )
			add(i);
		break;
	case 55:
	case 56:
	case 57:
	case 58:
		if ( AppDelegate::s_AchieveNum[58] < _num )
			for ( int i = 55; i < 59; i++ )
				AppDelegate::s_AchieveNum[i] = _num < g_iAchieveNum[i] ? _num : g_iAchieveNum[i];
		break;
	default:
		break;
	}
}

int AchieveAdd::GetNumNow( int _index )
{
	if ( AppDelegate::s_Achieve[_index] )
		return g_iAchieveNum[_index];

	int num = 0;
	switch ( _index )
	{
	case 12:
	case 13:
	case 14:
	case 15:
		num = AppDelegate::s_Money;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		for ( int i = 0; i < 36; i++ )
			if ( AppDelegate::s_LevelPerfect[i] )
				num++;
		break;
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
		for ( int i = 0; i < 36; i++ )
			if ( AppDelegate::s_LevelUnfire[i] )
				num++;
		break;
	case 26:
	case 27:
		for ( int i = 0; i < 36; i++ )
			if ( AppDelegate::s_LevelHero2[i] )
				num++;
		break;
	case 28:
	case 29:
		for ( int i = 0; i < 36; i++ )
			if ( AppDelegate::s_LevelHero3[i] )
				num++;
		break;
	case 30:
	case 31:
	case 32:
	case 33:
		for ( int i = 0; i < 36; i++ )
			num += AppDelegate::s_LevelStar[i];
		break;
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
		if ( AppDelegate::s_LevelOpen[(_index-30)*6+5] == 2 )
			num = 1;
		break;
	case 39:
		if ( AppDelegate::s_LevelOpen[35] == 2 )
			num = 1;
		break;
	default:
		num = AppDelegate::s_AchieveNum[_index];
		break;
	}
	if ( num > g_iAchieveNum[_index] )
		num = g_iAchieveNum[_index];
	return num;
}

int AchieveAdd::GetNumNeed( int _index )
{
	return g_iAchieveNum[_index];
}

int AchieveAdd::GetMedal( int _index )
{
	return g_iAchieveMedal[_index];
}

void AchieveAdd::add( int _index )
{
	if ( AppDelegate::s_AchieveNum[_index] < g_iAchieveNum[_index] )
		AppDelegate::s_AchieveNum[_index]++;
}

