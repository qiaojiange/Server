#pragma once
//#include "ccd/lucamapi.h"
#include "Camera.h"
#include "http/HPSocket.h"
#include "json/json.h"

#define UL_CAMERA 1


class CCameraProxy
{
public:
	CCameraProxy(void);
	~CCameraProxy(void);

	/************************************************************************/
	/* 处理请求的json数据                                                                     */
	/************************************************************************/
	 void handJsonData(IHttpServer* pSend, CONNID dwConnID,CStringA& jsonData);

	 void stopPreview(Json::Value &response, CStringA &cameraMsg, IHttpServer* pSend, CONNID dwConnID);

	 void startPreview(Json::Value &response, CStringA &cameraMsg, IHttpServer* pSend, CONNID dwConnID);

	 void connect(Json::Value &response, CStringA &cameraMsg, IHttpServer* pSend, CONNID dwConnID);

	 void disconnect(Json::Value &response, CStringA &cameraMsg, IHttpServer* pSend, CONNID dwConnID);

	 void getParameter(Json::Value &response, IHttpServer* pSend, CONNID dwConnID);

	 void setParameter(Json::Value &request, Json::Reader &reader, Json::Value &response, IHttpServer* pSend, CONNID dwConnID);





private: 
	CCamera* m_pCamera;

	//这个地方和java必须定义的是一样的顺序，否则调用的函数不一样，导致出错
	enum OperateID{
		//      连接
		CAMERA_CONNECT=0,

		//      断开连接
		CAMERA_DISCONNECT,

		//        保存图片
		CAMERA_SAVE_PICTURE16,

		//        获取参数
		CAMERA_GET_PARAMETER,

		//        设置参数
		CAMERA_SET_PARAMETER,

		//        开始预览
		CAMERA_START_PREVIEW,

		//        停止预览
		CAMERA_STOP_PREVIEW

	};

};

