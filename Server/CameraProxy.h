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
	 void handJsonData(IHttpServer* pSend, CONNID dwConnID,CStringA& jsonData){
		Json::Reader reader;
		Json::Value request;
		if (reader.parse(jsonData.GetBuffer(0),request))
		{
			if (SHOW_TRACE)
			{
				TRACE("---CCameraProxy---parse ----true\n");
			}

			int operateId = request["operateId"].asInt();
			OperateID operateID = static_cast<OperateID>(operateId);
			CStringA cameraMsg;

			Json::Value response;
			switch(operateId){

			case CAMERA_CONNECT:{
				TRACE("-----CAMERA_CONNECT--\n");
				if (CAMERA_STATUS_ERROR == m_pCamera->connect(UL_CAMERA))
				{
					response["status"] = CAMERA_STATUS_ERROR;
				}else{
					response["status"] = CAMERA_STATUS_OK;
				}

				cameraMsg = m_pCamera->getCCDMessageCStringA();
				response["describe"] = cameraMsg.GetBuffer(0);

				
				Json::FastWriter write;
				std::string result = write.write(response);

				CStringA strContentLength;
				strContentLength.Format("%d",result.length());
				THeader header[] = { {"Content-Type","text/plain"},{"Content-Length",strContentLength}};
				pSend->SendResponse(dwConnID,HSC_OK,"Server OK",header,sizeof(header)/sizeof(THeader),(BYTE*)result.c_str(),result.length());
				
			}
			
				break;

			case CAMERA_DISCONNECT:{
					TRACE("-----CAMERA_DISCONNECT--\n");
					if (CAMERA_STATUS_ERROR == m_pCamera->disconect())
					{
						response["status"] = CAMERA_STATUS_ERROR;
					}else{
						response["status"] = CAMERA_STATUS_OK;
					}

					cameraMsg = m_pCamera->getCCDMessageCStringA();
					response["describe"] = cameraMsg.GetBuffer(0);

					//TRACE(str);
					Json::FastWriter write;
					std::string result = write.write(response);

					CStringA strContentLength;
					strContentLength.Format("%d",result.length());
					THeader header[] = { {"Content-Type","text/plain"},{"Content-Length",strContentLength}};
					pSend->SendResponse(dwConnID,HSC_OK,"Server OK",header,sizeof(header)/sizeof(THeader),(BYTE*)result.c_str(),result.length());
				
				}
				break;

			case CAMERA_GET_PARAMETER:{
					INT width = 0;
					INT heigh = 0;
					FLOAT gain = 0.0f;
					FLOAT exposure = 0.0f;
					int xOffset = 0;
					int yOffset = 0;
					BOOL is8Bit = FALSE;
			
					if (CAMERA_STATUS_ERROR == 	m_pCamera->getCameraParameter(exposure,width,heigh,gain,xOffset,yOffset,is8Bit))
					{
						response["status"] = CAMERA_STATUS_ERROR;		
					}else{
						response["status"] = CAMERA_STATUS_OK;
					}
					response["describe"]= m_pCamera->getCCDMessageCStringA().GetBuffer(0);
					response["deviceName"] = "camera";

					Json::Value parameter1;
				
					parameter1["width"] = width;
					
					parameter1["heigh"] = heigh;
					
					parameter1["gain"] = gain;
					
					parameter1["exposure"] = exposure;

					parameter1["is8Bit"] = is8Bit;

					parameter1["xOffset"] = xOffset;
					
					parameter1["yOffset"] = yOffset;

					
					Json::FastWriter write;
					response["device"] = write.write(parameter1);

					std::string result = write.write(response);

					CStringA strContentLength;
					strContentLength.Format("%d",result.length());
					THeader header[] = { {"Content-Type","text/plain"},{"Content-Length",strContentLength}};
					pSend->SendResponse(dwConnID,HSC_OK,"Server OK",header,sizeof(header)/sizeof(THeader),(BYTE*)result.c_str(),result.length());
				
				}
				break;

			case CAMERA_SET_PARAMETER:{
				INT width = 0;
				INT heigh = 0;
				FLOAT gain = 0.0f;
				FLOAT exposure = 0.0f;
				int xOffset = 0;
				int yOffset = 0;
				BOOL is8Bit = FALSE;

				std::string strCamera = request["device"].asString();
				Json::Value root;
				if (reader.parse(strCamera.c_str(),root))
				{
					width = root["width"].asInt();
					heigh = root["heigh"].asInt();
					//TRACE("----width=%d---heigh = %d--\n",width,heigh);
					gain = root["gain"].asDouble();
					exposure = root["exposure"].asDouble();
					xOffset = root["xOffset"].asInt();
					yOffset = root["yOffset"].asInt();
					is8Bit = root["is8Bit"].asBool();

					if (CAMERA_STATUS_ERROR == m_pCamera->setCameraParameter(exposure,width,heigh,gain,xOffset,yOffset,is8Bit))
					{
						response["status"] = CAMERA_STATUS_ERROR;		
					}else{
						response["status"] = CAMERA_STATUS_OK;
					}

					response["describe"] = m_pCamera->getCCDMessageCStringA().GetBuffer(0);
					response["deviceName"] = "camera";


					Json::FastWriter write;
					std::string result = write.write(response);

					CStringA strContentLength;
					strContentLength.Format("%d",result.length());
					THeader header[] = { {"Content-Type","text/plain"},{"Content-Length",strContentLength}};
					pSend->SendResponse(dwConnID,HSC_OK,"Server OK",header,sizeof(header)/sizeof(THeader),(BYTE*)result.c_str(),result.length());
	
				}

			}
			
				break;

			case CAMERA_START_PREVIEW:
				//��ʼԤ��---- ��ȡ���ص�ͼƬ��ʹ����һ��������֮���ڵ��õĹ����а����÷��������ɻ�ñ��ص�����ͼƬ��
				

				break;

			case CAMERA_STOP_PREVIEW:
				break;

			case CAMERA_SAVE_PICTURE16:

				break;
			}
		
		}
	}





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

