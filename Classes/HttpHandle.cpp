#include "HttpHandle.h"
#include "AppDelegate.h"
#include "cocos-ext.h"
#include "LoginMenu.h"

using namespace cocos2d;
using namespace cocos2d::extension;

#define GameID	109

CHttpHandle* CHttpHandle::g_pInstance = NULL;

static std::string PostType[] =
{
	"money",
	"achievement",
	"corpse",
	"star",
};

void CHttpHandle::UpdateData(cocos2d::CCNode* _pNode)
{
//#ifdef GameTypeA
	m_bInit = true;
	m_pNode = _pNode;
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://221.233.134.15/GameDoSDK/updateRecordTaijiMan");
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, callfuncND_selector(CHttpHandle::CallbackFunction));

	int acheve = 0;
	for ( int i = 0; i < 63; i++ )
		acheve += AppDelegate::s_Achieve[i];
	int star = 0;
	for ( int i = 0; i < 6*5; i++ )
		star += AppDelegate::s_LevelStar[i];

	std::string name = CCUserDefault::sharedUserDefault()->getStringForKey("name");
	char buffer[255];
	sprintf(buffer, "gameId=%d&imei=%Ld&money=%d&achievement=%d&corpse=%d&star=%d&playerName=%s", GameID, AppDelegate::s_imei, AppDelegate::s_Money, acheve, AppDelegate::s_KillMonster, star, name.c_str());
	request->setRequestData(buffer, strlen(buffer)); 

	request->setTag("100");
	CCHttpClient::getInstance()->send(request);
	request->release();
//#endif
}

void CHttpHandle::GetData( int _type )
{
//#ifdef GameTypeA
	CCHttpRequest* request = new CCHttpRequest();
	int type = _type > 4 ? _type-4 : _type;
	if ( type < 4 )
		request->setUrl("http://221.233.134.15/GameDoSDK/getRecordTaijiManTop5");
	else
		request->setUrl("http://221.233.134.15/GameDoSDK/getRecordTaijiManFront5");
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, callfuncND_selector(CHttpHandle::CallbackFunction));

	char buffer[255];
	sprintf(buffer, "gameId=%d&recordName=%s&imei=%Ld", GameID, PostType[type].c_str(), AppDelegate::s_imei);
	request->setRequestData(buffer, strlen(buffer)); 

	char tag[10];
	sprintf(tag, "%d", _type);
	request->setTag(tag);
	CCHttpClient::getInstance()->send(request);
	request->release();
//#endif
}

void CHttpHandle::CallbackFunction( cocos2d::CCNode *sender, void *data )
{
	CCHttpResponse *response = (CCHttpResponse*)data;

	if (!response)
		return;
	if ( !ccbLoginMenu::s_pLogin )
	{
		return;
	}
	if (!response->isSucceed()) 
	{
		//CCLog("response failed");
		((ccbLoginMenu*)m_pNode)->NetError();
		return;
	}

	std::vector<char> *buffer = response->getResponseData();
	int type = atoi(response->getHttpRequest()->getTag());

	std::string str = "";
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		str += (*buffer)[i];
	}
	//CCLog(str.c_str());
	((ccbLoginMenu*)m_pNode)->ShowRank(type, str.c_str());
}

CHttpHandle::CHttpHandle()
{
	m_bInit = false;
}

CHttpHandle::~CHttpHandle()
{
}

CHttpHandle* CHttpHandle::GetInstance()
{
	if ( !g_pInstance )
	{
		g_pInstance = new CHttpHandle;
	}
	return g_pInstance;
}

void CHttpHandle::DestroyInstance()
{
	if ( g_pInstance )
	{
		delete g_pInstance;
		g_pInstance = NULL;
	}
}

//char* CHttpHandle::Serialize()
//{
//	char* pChar = NULL;
//	return pChar;
//}
//
//PostData* CHttpHandle::Unserialize( char* _pChar )
//{
//	PostData* pPostData = new PostData;
//
//	return pPostData;
//}
