#define QuestionTypeNum 7

struct QuestionInfo
{
	int index[3];
	int type[QuestionTypeNum];
	int	num[QuestionTypeNum];
	int id[QuestionTypeNum];
	int complate[QuestionTypeNum];

	QuestionInfo()
	{
		index[0] = 0;
		index[1] = 0;
		index[2] = 0;
		for ( int i = 0; i < QuestionTypeNum; i++ )
		{
			type[i] = 0;
			num[i]	= 0;
			id[i]	= 0;
			complate[i] = 0;
		}
	}
};

static int data_qustioninfo_type[] =
{
	1,3,4,
	1,5,4,
	6,5,4,
	1,7,4,
	1,7,4,
	0,0,0,

	1,3,4,
	2,3,4,
	1,5,4,
	1,2,4,
	1,7,4,
	0,0,0,

	1,3,4,
	2,3,4,
	1,7,4,
	6,5,4,
	1,3,4,
	0,0,0,

	1,3,4,
	6,5,4,
	1,7,4,
	1,2,4,
	1,3,4,
	0,0,0,
};

static int data_qustioninfo_num[] =
{
	60,10,1,
	70,100,1,
	50,100,1,
	80,20,1,
	110,30,1,
	0,0,0,

	90,25,1,
	2,30,1,
	100,200,1,
	100,1,1,
	110,45,1,
	0,0,0,

	80,25,1,
	2,50,1,
	90,30,1,
	50,200,1,
	100,60,1,
	0,0,0,

	70,25,1,
	50,200,1,
	80,45,1,
	60,1,1,
	90,60,1,
	0,0,0,
};

static int data_qustioninfo_id[] =
{
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,//

	0,0,0,
	0,8,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,//

	0,0,0,
	0,9,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,//

	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,//
};
