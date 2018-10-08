#ifndef _ACHIEVEADD_H_
#define _ACHIEVEADD_H_

//////////////
/****成就****/
//////////////
static const int g_iAchieveNum[] =
{
	10, 100, 1000, 10000,
	50, 500, 5000, 50000,
	10, 50, 200, 500,
	5000, 20000, 50000, 200000,
	1, 5, 10, 15, 25,
	1, 5, 10, 15, 25,
	10, 20, 10, 20, //
	10, 20, 40, 75,
	1, 1, 1, 1, 1, 1,
	1, 10, 50, 200,
	1, 10, 50, 200,
	1, 10, 50, 200,
	1, 5, 10,
	3, 10, 20, 50,
};

static const int g_iAchieveMedal[] =
{
	5, 10, 15, 20,
	5, 10, 15, 20,
	5, 10, 15, 20,
	5, 10, 15, 30,
	5, 10, 15, 20, 50,
	5, 10, 15, 20, 50,
	20, 40, 30, 60,
	5, 10, 20, 30,
	5, 10, 15, 20, 25, 30,
	5, 10, 20, 30,
	10, 15, 25, 40,
	15, 25, 40, 60, 
	5, 10, 20, 
	5, 10, 15, 20,
};

class AchieveAdd
{
public:
	static void Add(int _index, int _num = 0, int _level = 0);
	static int GetNumNow(int _index);
	static int GetNumNeed(int _index);
	static int GetMedal(int _index);

private:
	inline static void add(int _index);
};


#endif

