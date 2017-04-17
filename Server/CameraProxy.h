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
					
					//Json::Value parameterList;
					Json::Value parameter1;
					parameter1["name"]="width";
					parameter1["val"] = width;
					response["parameterList"].append(parameter1);
					
					Json::Value parameter2;
					parameter2["name"]="heigh";
					parameter2["val"] = heigh;
					response["parameterList"].append(parameter2);
					
					Json::Value parameter3;
					parameter3["name"]="gain";
					parameter3["val"] = gain;
					response["parameterList"].append(parameter3);

					Json::Value parameter4;
					parameter4["name"]="exposure";
					parameter4["val"] = exposure;
					response["parameterList"].append(parameter4);

				
					Json::Value parameter5;
					parameter5["name"]="is8Bit";
					parameter5["val"] = is8Bit;
					response["parameterList"].append(parameter5);

					Json::FastWriter write;
					std::string result = write.write(response);

					CStringA strContentLength;
					strContentLength.Format("%d",result.length());
					THeader header[] = { {"Content-Type","text/plain"},{"Content-Length",strContentLength}};
					pSend->SendResponse(dwConnID,HSC_OK,"Server OK",header,sizeof(header)/sizeof(THeader),(BYTE*)result.c_str(),result.length());
				
					
				}
				break;

			case CAMERA_SET_PARAMETER:
								

				break;

			case CAMERA_START_PREVIEW:
				//开始预览---- 获取本地的图片，使用另一个方法，之后在调用的过程中包含该方法。即可获得本地的最新图片。
				

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

