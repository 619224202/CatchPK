#include "PayService.h"
#include "AppDelegate.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "SimpleAudioEngine.h"

#endif
#include "logo.h"
#include "ccb.h"
//#include "AccessData.h"

bool PayService::showed = false;
bool PayService::m_bIsPay = false;
CCCallFunc* PayService::successFc;
CCCallFunc* PayService::failFc;
CCCallFunc* PayService::onCancelExitFc;

PayService* PayService::instance = NULL;

PayService* PayService::getInstance() {
	if (instance == NULL) {
		instance = new PayService();
	}
	return instance;
}

PayService::PayService()
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","GetImei", "()J") )
//	{
//		AppDelegate::s_imei = minfo.env->CallStaticLongMethod(minfo.classID, minfo.methodID);
//		//jchar* chararray = minfo.env->GetCharArrayElements(str, 0);
//		//AppDelegate::s_imei = chararray;
//		//AppDelegate::s_imei = minfo.env->CallStaticCharMethod(minfo.classID, minfo.methodID, 0);
//		//AppDelegate::s_imei = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID, 0);
//	}
//#endif
}

void PayService::setCheckTrue(void)
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","setCheckTrue", "()V");
//	if ( isHave ) 
//		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
//#endif
}

void PayService::PlayLogo()
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","playVideo", "(Ljava/lang/String;)V");
//	if ( isHave ) 
//		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, minfo.env->NewStringUTF("logo.mp4"));
//#endif
}

void PayService::ShareSend(int _type, int _level)
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	bool isHave = false;
//	switch ( _type )
//	{
//	case 1:
//		AppDelegate::LoadTencent();
//		if ( AppDelegate::s_Tencent )
//			isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","OnSendTencent", "(I)V");
//		else
//			isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","OnSendTencentAuth", "(I)V");
//		break;
//	case 2:
//		isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","OnSendWX", "(I)V");
//		break;
//	case 3:
//		isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","OnSendSina", "(I)V");
//		break;
//	}
//	if ( isHave ) 
//		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, _level);
//#endif
}

void PayService::pay(int id)
{
	if ( m_bIsPay )
		return;
	m_bIsPay = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
	#ifndef Pay_Test
		#ifndef Plat_Telecom_Demo
			JniMethodInfo minfo;//定义Jni函数信息结构体
			bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","pay", "(I)V");
			if ( isHave ) 
				minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, id);
		#endif
	#else
		AppDelegate::GetPayContent(id);
	#endif
#else
	AppDelegate::GetPayContent(id);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
}

extern "C"
{
//#ifdef Movie_Logo
//	void Java_com_gamedo_taijiman_service_PayService_logoOver(JNIEnv* env, jobject firedragonpzy)
//	{
//		logo::logoOver();
//	}
//#endif
//
//	void Java_com_gamedo_taijiman_service_PayService_ShareTencentAuth(JNIEnv* env, jobject firedragonpzy)
//	{
//		AppDelegate::SaveTencent();
//	}
//	
//	void Java_com_gamedo_taijiman_service_PayService_ShareOK(JNIEnv* env, jobject firedragonpzy)
//	{
//		if ( ccbLayer::s_pccbLayer )
//			ccbLayer::s_pccbLayer->ShareOK();
//	}
//
//	void Java_com_gamedo_taijiman_service_PayService_ShareCancle(JNIEnv* env, jobject firedragonpzy)
//	{
//		if ( ccbLayer::s_pccbLayer )
//			ccbLayer::s_pccbLayer->ShareCancle();
//	}
//
//	void Java_com_gamedo_taijiman_service_PayService_sendSuccess(JNIEnv* env, jobject firedragonpzy)
//	{
//		PayService::getInstance()->callSuccessFc();
//	}
//
//	void Java_com_gamedo_taijiman_service_PayService_sendFail(JNIEnv* env, jobject firedragonpzy)
//	{
//		PayService::getInstance()->callfailFc();
//	}
//
//	void Java_com_gamedo_taijiman_service_PayService_resumeSound(JNIEnv* env, jobject firedragonpzy)
//	{
//		AppDelegate::AudioBackResume();
//	}
//
//	void Java_com_gamedo_taijiman_service_PayService_pauseSound(JNIEnv* env, jobject firedragonpzy)
//	{
//		AppDelegate::AudioBackPause();
//	}
//
//	void Java_com_gamedo_taijiman_service_PayService_setSound(JNIEnv* env, jobject firedragonpzy,jboolean mute,jboolean change)
//	{
//	}
//
//#ifdef Plat_91
//	void Java_com_gamedo_taijiman_service_PayService_sendOnCancelExit(JNIEnv* env, jobject firedragonpzy)
//	{
//		AppDelegate::m_bCanStartGame = 1;
//		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PayService::callOnCancelExitFc), PayService::getInstance(), 0 ,0, 0, false);
//	}
//
//	void Java_com_gamedo_taijiman_service_PayService_sendOnConfirmExit(JNIEnv* env, jobject firedragonpzy)
//	{
//		exit(0);
//	}
//
//	//返回手机卡服务商
//	void Java_com_gamedo_taijiman_service_PayService_setProvidesId(JNIEnv *env, jobject _obj, jint _id)
//	{ 
//	}
//	//返回手机卡服务商
//	void Java_com_gamedo_taijiman_service_PayService_getPayAward(JNIEnv *env, jobject _obj, jint _id)
//	{
//		AppDelegate::GetPayContent(_id);
//	}
//	//返回查询漏单结果
//	void Java_com_gamedo_taijiman_service_PayService_checkTempPayIdResult(JNIEnv *env, jobject _obj, jstring _id, jint _success)
//	{ 
//		char*   rtn   =   NULL;
//		jclass   clsstring   =   env->FindClass("java/lang/String");
//		jstring   strencode   =   env->NewStringUTF("GB2312");
//		jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
//		jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(_id,mid,strencode);
//		jsize   alen   =   env->GetArrayLength(barr);
//		jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);
//		if(alen   >   0)
//		{
//			rtn   =   (char*)malloc(alen+1);
//			memcpy(rtn,ba,alen);
//			rtn[alen]=0;
//		}
//		env->ReleaseByteArrayElements(barr,ba,0);
//		std::string stemp(rtn);
//		free(rtn);
//		AppDelegate::DeletePayInfo(stemp.c_str(), _success);
//	}
//
//	//添加查询漏单结果
//	void Java_com_gamedo_taijiman_service_PayService_addcheckTempPayId(JNIEnv *env, jobject _obj, jstring _id, jint _key)
//	{ 
//		char*   rtn   =   NULL;
//		jclass   clsstring   =   env->FindClass("java/lang/String");
//		jstring   strencode   =   env->NewStringUTF("GB2312");
//		jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
//		jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(_id,mid,strencode);
//		jsize   alen   =   env->GetArrayLength(barr);
//		jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);
//		if(alen   >   0)
//		{
//			rtn   =   (char*)malloc(alen+1);
//			memcpy(rtn,ba,alen);
//			rtn[alen]=0;
//		}
//		env->ReleaseByteArrayElements(barr,ba,0);
//		std::string stemp(rtn);
//		free(rtn);
//		AppDelegate::SavePayInfo(stemp.c_str(), _key);
//	}
//	//平台游戏前必选显示已近显示完毕
//	void Java_com_gamedo_taijiman_service_PayService_canStartGame(JNIEnv *env, jobject _obj)
//	{ 
//		AppDelegate::m_bCanStartGame = 1;
//	}
//#endif // _DEBUG
//
//	void Java_com_gamedo_taijiman_service_PayService_GetPay(JNIEnv *env, jobject _obj, jint _id)
//	{ 
//		AppDelegate::GetPayContent(_id);
//	}
//	void Java_com_gamedo_taijiman_service_PayService_CanclePay(JNIEnv *env, jobject _obj, jint _id)
//	{ 
//		AppDelegate::CanclePay(_id);
//	}
//
#endif
}

void PayService::exitGame(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//#ifdef Plat_91
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","exitGame", "()V");
//	if (!isHave) {
//	}else{
//		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
//	}
//#endif
//#ifdef Plat_Telecom
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","exitGame", "(I)V");
//	if (!isHave) {
//	}else{
//		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, AppDelegate::s_Money);
//	}
//#endif
#endif
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	exit(0);
#endif
}

void PayService::HideBar()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//#ifdef Plat_91
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","hideBar", "()V") )
//	{
//		minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
//	}
//#endif
#ifdef Plat_Telecom
	//JniMethodInfo minfo;//定义Jni函数信息结构体
	//if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","HideBar", "()V") )
	//{
	//	minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
	//}
#endif
#endif
}

void PayService::ShowBar()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//#ifdef Plat_91
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","openBar", "()V") )
//	{
//		minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
//	}
//#endif
#ifdef Plat_Telecom
	//JniMethodInfo minfo;//定义Jni函数信息结构体
	//if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","ShowBar", "()V") )
	//{
	//	minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
	//}
#endif
#endif
}

void PayService::OnPause()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//#ifdef Plat_Telecom
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","onPause", "()V") )
//	{
//		minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
//	}
//#endif
#endif
}

void PayService::OnResume()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//#ifdef Plat_Telecom
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","onResume", "()V") )
//	{
//		minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
//	}
//#endif
#endif
}

void PayService::moreGame(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","moreGame", "()V");
//	if (!isHave) {
//	}else{
//		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
//	}
#endif
}

void PayService::callSuccessFc()
{
	PayService::successFc->execute();
	PayService::successFc->release();
	PayService::successFc = NULL;
	if(PayService::failFc!=NULL){
		PayService::failFc->release();
		PayService::failFc = NULL;
	}
	showed = false;
}

void PayService::callfailFc()
{
	if(PayService::successFc!=NULL){
		PayService::successFc->release();
		PayService::successFc = NULL;
	}
	if(PayService::failFc!=NULL){
		PayService::failFc->execute();
		PayService::failFc->release();
		PayService::failFc = NULL;
	}

	showed = false;
}

void PayService::callOnCancelExitFc()
{
	//PayService::onCancelExitFc->execute();
	//PayService::onCancelExitFc->release();
	//PayService::onCancelExitFc = NULL;
}

void PayService::UpdataScore(int _score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//#ifdef Plat_Telecom
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","onResume", "(I)V") )
//	{
//		minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID, _score);
//	}
//#endif
#endif
}

void PayService::TelecomInit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
//#ifdef Plat_Telecom
//	JniMethodInfo minfo;//定义Jni函数信息结构体
//	if ( JniHelper::getStaticMethodInfo(minfo,"com/gamedo/taijiman/service/PayService","onInit", "()V") )
//	{
//		minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
//	}
//#endif
#endif
}

