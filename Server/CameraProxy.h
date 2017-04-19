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
	/* 处理请求的json数据                                                    */
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
	bool isSave;

	//jpg
	CStringA m_filePathJpg;
	//16位图片
	CStringA m_filePath16;

	//这个地方和java必须定义的是一样的顺序，否则调用的函数不一样，导致出错
	enum OperateID{
		//      连接  括号表示序号(0)，不要乱动
		CAMERA_CONNECT=0,

		//      断开连接（1）
		CAMERA_DISCONNECT,

		//       保存成16位图片(2)
		CAMERA_SAVE_PICTURE16,

		//       获取参数（3）
		CAMERA_GET_PARAMETER,

		//       设置参数（4）
		CAMERA_SET_PARAMETER,

		//       开始预览（5）
		CAMERA_START_PREVIEW,

		//       停止预览（6）
		CAMERA_STOP_PREVIEW,

		//		 传输图片（7）
		CAMERA_TRANS_PICTURE

	};


};

