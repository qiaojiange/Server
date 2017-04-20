
#include "StdAfx.h"
#include "CameraProxy.h"
#include "Server.h"


CCameraProxy::CCameraProxy(void)
{
	TRACE("---------CCameraProxy::CCameraProxy(void)----------\n");
	m_pCamera = CCamera::getInstance();
	isSave = false;

	m_filePathJpg.Format("D:/temp.jpg");
}



CCameraProxy::~CCameraProxy(void)
{
	TRACE("------CCameraProxy::~CCameraProxy(void)-------\n");
	if (NULL != m_pCamera)
	{
		/*m_pCamera->stopPreview(theApp.m_hwndPic);
		m_pCamera->disconect();*/

		delete m_pCamera;
	}

}

//处理json数据
void CCameraProxy::handJsonData(IHttpServer* pSend, CONNID dwConnID,CStringA& jsonData)
{
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

		case CAMERA_CONNECT:
			connect(response, cameraMsg, pSend, dwConnID);
			break;

		case CAMERA_DISCONNECT:
			disconnect(response, cameraMsg, pSend, dwConnID);
			break;

		case CAMERA_GET_PARAMETER:
			TRACE("---CAMERA_GET_PARAMETER---\n");
			getParameter(response, pSend, dwConnID);
			break;

		case CAMERA_SET_PARAMETER:
			TRACE("---CAMERA_SET_PARAMETER---\n");
			setParameter(request, reader, response, pSend, dwConnID);
			break;

		case CAMERA_START_PREVIEW:
			TRACE("---CAMERA_START_PREVIEW---\n");
			//开始预览---- 获取本地的图片，使用另一个方法，之后在调用的过程中包含该方法。即可获得本地的最新图片。
			startPreview(response, cameraMsg, pSend, dwConnID);
			break;

		case CAMERA_STOP_PREVIEW:
			TRACE("---CAMERA_STOP_PREVIEW---\n");
			stopPreview(response, cameraMsg, pSend, dwConnID);
			break;

		case CAMERA_SAVE_PICTURE16:
			//m_pCamera->savePictureTo16TIFF();
			break;
		case  CAMERA_TRANS_PICTURE:
			{
				TRACE("-----CAMERA_TRANS_PICTURE------\n");
			//	isSave = true;//先传过来一张图片
				if (!isSave)
				{
					
					if (CAMERA_STATUS_ERROR == m_pCamera->savePictureToJPEG(m_filePathJpg))
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

				/*	if (!pSend->IsKeepAlive(dwConnID))
					{
						pSend->Release(dwConnID);
					}*/

					isSave = true;

				}else{
					TRACE("----send file=---\n");
					CString filePath;
					filePath = m_filePathJpg;
					CFile file(filePath,CFile::modeRead);
					ULONGLONG length = file.GetLength();
					BYTE* pData = new BYTE[length];
					file.Read(pData,length);
					TRACE("----length=---\n");
					CStringA strContentLength;
					strContentLength.Format("%u",length);
					THeader header[]  = {{"Content-Type","picture"},{"Content-Length",strContentLength}};
					int iHeaderCount = sizeof(header)/sizeof(THeader);
					pSend->SendResponse(dwConnID,HSC_OK,"HP Http Server OK",header,iHeaderCount,pData,length);

					//pSend->SendLocalFile(dwConnID,m_filePathJpg,200,nullptr,header,iHeaderCount);
					//pSend->SendSmallFile(dwConnID,filePath);

					TRACE("------SendResponse----ok--------\n");
					/*	if (!pSend->IsKeepAlive(dwConnID))
					{
					TRACE("--!pSend->IsKeepAlive(dwConnID)-----\n");
					pSend->Release(dwConnID);
					}*/
					file.Close();
					isSave = false;

				}
			



			}
			break;
		}

	}
}

void CCameraProxy::setParameter(Json::Value &request, Json::Reader &reader, Json::Value &response, IHttpServer* pSend, CONNID dwConnID)
{
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

void CCameraProxy::getParameter(Json::Value &response, IHttpServer* pSend, CONNID dwConnID)
{
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

void CCameraProxy::disconnect(Json::Value &response, CStringA &cameraMsg, IHttpServer* pSend, CONNID dwConnID)
{
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

void CCameraProxy::connect(Json::Value &response, CStringA &cameraMsg, IHttpServer* pSend, CONNID dwConnID)
{
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

void CCameraProxy::startPreview(Json::Value &response, CStringA &cameraMsg, IHttpServer* pSend, CONNID dwConnID)
{
	TRACE("-----startPreview--\n");
	if (CAMERA_STATUS_ERROR == m_pCamera->startPreview(theApp.m_hwndPic))
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

void CCameraProxy::stopPreview(Json::Value &response, CStringA &cameraMsg, IHttpServer* pSend, CONNID dwConnID)
{

	TRACE("-----stopPreview--\n");
	if (CAMERA_STATUS_ERROR == m_pCamera->stopPreview())
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
