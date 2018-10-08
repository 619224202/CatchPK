#ifndef _C_HTTPHANDLE_H_
#define _C_HTTPHANDLE_H_

#include "cocos2d.h"

//struct PostData
//{
//	int id;
//	int money;
//	int achieve;
//	int killed;
//	int star;
//	std::string name;
//};

class CHttpHandle : public cocos2d::CCLayer
{
public:
	CHttpHandle();
	~CHttpHandle();

	static CHttpHandle* GetInstance();
	static void DestroyInstance();

	void UpdateData(cocos2d::CCNode* _pNode);
	void GetData(int _type);
	void CallbackFunction(cocos2d::CCNode *sender, void *data);

private:
	//char* Serialize();
	//PostData* Unserialize(char* _pChar);

	//PostData* m_pPostData;
	cocos2d::CCNode* m_pNode;
	bool m_bInit;
	static CHttpHandle* g_pInstance;
};
#endif
