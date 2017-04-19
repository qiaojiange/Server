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
	/* ���������json����                                                    */
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
	//16λͼƬ
	CStringA m_filePath16;

	//����ط���java���붨�����һ����˳�򣬷�����õĺ�����һ�������³���
	enum OperateID{
		//      ����  ���ű�ʾ���(0)����Ҫ�Ҷ�
		CAMERA_CONNECT=0,

		//      �Ͽ����ӣ�1��
		CAMERA_DISCONNECT,

		//       �����16λͼƬ(2)
		CAMERA_SAVE_PICTURE16,

		//       ��ȡ������3��
		CAMERA_GET_PARAMETER,

		//       ���ò�����4��
		CAMERA_SET_PARAMETER,

		//       ��ʼԤ����5��
		CAMERA_START_PREVIEW,

		//       ֹͣԤ����6��
		CAMERA_STOP_PREVIEW,

		//		 ����ͼƬ��7��
		CAMERA_TRANS_PICTURE

	};


};

