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
	/* ���������json����                                                                     */
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

	//����ط���java���붨�����һ����˳�򣬷�����õĺ�����һ�������³���
	enum OperateID{
		//      ����
		CAMERA_CONNECT=0,

		//      �Ͽ�����
		CAMERA_DISCONNECT,

		//        ����ͼƬ
		CAMERA_SAVE_PICTURE16,

		//        ��ȡ����
		CAMERA_GET_PARAMETER,

		//        ���ò���
		CAMERA_SET_PARAMETER,

		//        ��ʼԤ��
		CAMERA_START_PREVIEW,

		//        ֹͣԤ��
		CAMERA_STOP_PREVIEW

	};

};

