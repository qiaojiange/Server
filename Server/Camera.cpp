#include "StdAfx.h"
#include "Camera.h"

CCamera* CCamera::m_pCamera = NULL;

CCamera::CCamera()
{
	TRACE("---CCamera::CCamera(void)--construct---\n");
	this->m_bConnected = FALSE;

	this->m_bInitMode = FALSE;
	
	this->m_bPreviewing = FALSE;
	this->m_hCamera = NULL;
	this->m_hwnd = NULL;


}


CCamera::~CCamera(void)
{
	TRACE("----CCamera::~CCamera(void)------\n");
	if (m_bConnected)
	{
		this->disconect();
		
	}
	m_hCamera = NULL;
	m_pCamera = NULL;
}

CameraStatus CCamera::getGain(FLOAT& fGain)
{
	LONG lFlags=0;
	if (m_hCamera!=NULL)
	{
		if (LucamGetProperty(m_hCamera,LUCAM_PROP_GAIN,&fGain,&lFlags))
		{
			csMsg.Format(_T("get Gain success!"));
			return CAMERA_STATUS_OK;
		}else{
			csMsg.Format(_T("Unable to get current Gain."));
			return CAMERA_STATUS_ERROR;
		}
	}else{
		csMsg.Format(_T("Unable to open camera"));
		return CAMERA_STATUS_ERROR;
	}
}

CameraStatus CCamera::getExposure(FLOAT& fExposure)
{
	LONG lFlags=0;
	if (m_hCamera!=NULL)
	{
		if (LucamGetProperty(m_hCamera,LUCAM_PROP_EXPOSURE,&m_fExposure,&lFlags))
		{
			csMsg.Format(_T("get Exposure success!"));
			fExposure = m_fExposure;
			return CAMERA_STATUS_OK;
		}else{
			csMsg.Format(_T("Unable to get current exposure."));
			return CAMERA_STATUS_ERROR;
		}
	}else{
		csMsg.Format(_T("Unable to open camera"));
		return CAMERA_STATUS_ERROR;
	}
}

CameraStatus CCamera::connect(ULONG ulCamera)
{
	//������̫����
	if (!LucamSelectExternInterface(LUCAM_EXTERN_INTERFACE_GIGEVISION))
	{
		//this->msg = _T("Unable to set correct camera interface.");
		this->csMsg.Format(_T("Invlid camera number."));
		return CAMERA_STATUS_ERROR;
	}

	if(ulCamera<1){
		this->csMsg.Format(_T("Invlid camera number."));
		return CAMERA_STATUS_ERROR;
	}
	if(m_hCamera!=NULL){
		TRACE("-----m_hCamera!=NULL-----\n");
		disconect();
	}
	m_hCamera = LucamCameraOpen(ulCamera);
	if (m_hCamera != NULL)
	{
		this->csMsg.Format(_T("have connect to the camera"));
		this->m_bConnected = TRUE;

		return CAMERA_STATUS_OK;
	}else{
		this->csMsg.Format(_T("Unable to connect to the camera. Error code %d."),LucamGetLastError());
		this->m_bConnected = FALSE;

		return CAMERA_STATUS_ERROR;
	}
	
}

CameraStatus CCamera::disconect()
{
	if (m_bPreviewing)
	{
		//if (IDYES == AfxMessageBox(_T("Currently previewing. Do you wish to stop?"), MB_YESNOCANCEL)) {
		//	//OnBnClickedButtonPreview();
		//}
		//else return CAMERA_STATUS_ERROR;

		stopPreview();
	}

	if (NULL== m_hCamera )
	{
		this->csMsg.Format(_T("camera is not connect,please connect camera firstly!"));
		return CAMERA_STATUS_ERROR;
	}

	if (!LucamCameraClose(m_hCamera))
	{
		this->csMsg.Format(_T("Unable to disconnect to the camera."));
		return CAMERA_STATUS_ERROR;

	}else{

		this->csMsg.Format(_T("disconnect to the camera successfully."));
		m_bConnected = FALSE;
		m_hCamera = NULL;

	}

	return CAMERA_STATUS_OK;
}

CameraStatus CCamera::startPreview(HWND hwnd)
{
	if (m_hCamera ==NULL || !m_bConnected)
	{
		this->csMsg.Format(_T("disconnect camera"));
		return CAMERA_STATUS_ERROR;

	}else{
		this->m_hwnd = hwnd;
		if (!LucamStreamVideoControl(m_hCamera,START_DISPLAY,hwnd))
		{
			this->csMsg.Format(_T("preview failure!"));
			return CAMERA_STATUS_ERROR;
		}else{
			this->csMsg.Format(_T("preview successfully"));
			this->m_bPreviewing = TRUE;
			return CAMERA_STATUS_OK;
		}
	}
}

CameraStatus CCamera::stopPreview()
{
	if (m_hCamera == NULL || !m_bConnected || !m_bPreviewing)
	{
		this->csMsg.Format(_T("cannot connect camera"));
		return CAMERA_STATUS_ERROR;

	}else{
		
		//this->m_hwnd = hwnd;
		TRACE(_T("----------stopPreview-------\n"));
		if (!LucamStreamVideoControl(m_hCamera,STOP_STREAMING,m_hwnd))
		{
			this->csMsg.Format(_T("cannot stop to play video"));
			return CAMERA_STATUS_ERROR;
		}else{
			TRACE(_T("----------LucamStreamVideoControl---true----\n"));
			this->m_bPreviewing = FALSE;
			this->csMsg.Format(_T("stop preview successfully"));
			return CAMERA_STATUS_OK;
		}
	}
}

CameraStatus CCamera::setCameraParameter(FLOAT& exposure,INT& width,INT& heigh,FLOAT& gain,INT xOffset,INT& yOffset,BOOL is8Bit)
{
	if (m_hCamera == NULL || !m_bConnected )
	{
		this->csMsg.Format(_T("cannot connect camera"));
		return CAMERA_STATUS_ERROR;
	}else{

		FLOAT frameRate = 100000.0f;;

		m_frameFormat.width = width;
		m_frameFormat.height = heigh;
		m_frameFormat.xOffset = xOffset;
		m_frameFormat.yOffset = yOffset;

		if (is8Bit)
		{
			m_frameFormat.pixelFormat = LUCAM_PF_8;
		}else{
			m_frameFormat.pixelFormat = LUCAM_PF_16;
		}	

		if (!LucamSetProperty(m_hCamera,LUCAM_PROP_GAIN,gain,0))
		{
			this->csMsg.Format(_T("set gain failure"));
			return CAMERA_STATUS_ERROR;
		}

		if (!LucamSetProperty(m_hCamera,LUCAM_PROP_EXPOSURE,exposure,0))
		{
			this->csMsg.Format(_T("set exposure failure"));
			return CAMERA_STATUS_ERROR;
		}

		//�޸�֡�ĸ�ʽ��������ֹͣԤ����֮���ڽ���Ԥ���Ϳ��Կ������º��ͼ��
		if (m_bPreviewing)
		{
			stopPreview();
			m_bPreviewing = FALSE;
			//����֡��ʽ
			if (!LucamSetFormat(m_hCamera,&m_frameFormat,frameRate))
			{
				this->csMsg.Format(_T("Unable to set camera video format."));
				return CAMERA_STATUS_ERROR;
			}
		}
		if (m_hwnd != NULL)
		{
			startPreview(m_hwnd);
		}
	

		return CAMERA_STATUS_OK;
	}
}

CameraStatus CCamera::getCameraParameter(FLOAT& exposure,INT& width,INT& heigh,FLOAT& gain,INT& xOffset,INT& yOffset,BOOL& is8Bit)
{
	if (m_hCamera == NULL)
	{
		this->csMsg.Format(_T("cannot connect camera"));
		return CAMERA_STATUS_ERROR;
	}else{

		this->getExposure(exposure);
		FLOAT frameRate;
		this->getFrameFormat(m_frameFormat,frameRate);
		width = m_frameFormat.width;
		heigh = m_frameFormat.height;
		xOffset = m_frameFormat.xOffset;
		yOffset = m_frameFormat.yOffset;
		m_fFrameRate = 100000.0;
		is8Bit = m_frameFormat.pixelFormat==LUCAM_PF_8?TRUE:FALSE;
		this->getGain(gain);
		this->csMsg.Format(_T("get parameter successfully."));
		return CAMERA_STATUS_OK;

	}
}

CameraStatus CCamera::getFrameFormat(LUCAM_FRAME_FORMAT& fFormat,FLOAT& fFrameRate)
{
	if (!m_bConnected)
	{
		this->csMsg.Format(_T("please connect ccd,then get Frame Data��"));
		return CameraStatus::CAMERA_STATUS_ERROR;
	}else{
		//��ȡ֡��ʽ
		if (!LucamGetFormat(m_hCamera, &fFormat, &fFrameRate))
		{
			//AfxMessageBox(_T("Unable to get camera video format. Capture frames may not work properly."));
			this->csMsg.Format(_T("Unable to get camera video format. Capture frames may not work properly."));
			return CAMERA_STATUS_ERROR;
		}else{
			this->csMsg.Format(_T("have got camera video format."));
			return CAMERA_STATUS_OK;
		}

	}
}

CameraStatus CCamera::autoExposure(FLOAT& exposureTime)
{
	if (!m_bPreviewing)
	{
		this->csMsg.Format(_T("havn't preview,cannot use auto exposure!"));
		return CAMERA_STATUS_ERROR;
	}
	FLOAT fLowExposeTime = 20.0f;
	FLOAT fHightExposeTime = 41.0f;
	FLOAT fMidExposeTime = 0.0f;

	int iLowGreyAvg = 0;
	int iHightGreyAvg = 0;
	int iMidGreyAvg = 0;

	////���ļ�
	//CStdioFile mFile;
	//CFileException mExcept;
	//mFile.Open( _T("data3.txt"), CFile::modeWrite|CFile::modeCreate, &mExcept);

	//�Ƿ��ҵ�����ͼƬ��false:û���ҵ�����ͼƬ��true:�ҵ�����ͼƬ
	bool bFindOptimalExposeTime = false;
	int maxGreyAvg = 0;
	int minGreyAvg =255;

	//�ɼ�ͼƬ�����м��㣬����ExposureParameter	
	ExposureParameter exposureParameter ;

	while (fLowExposeTime < (fHightExposeTime-1) )
	{
		fMidExposeTime = (fHightExposeTime+fLowExposeTime)/2;

		if (fMidExposeTime>40)
		{
			break;
		}

		takePictureCalculate(fMidExposeTime,exposureParameter);
	

		if (minGreyAvg > exposureParameter.iGreyAvg)
		{
			minGreyAvg = exposureParameter.iGreyAvg;
		}

		if (maxGreyAvg < exposureParameter.iGreyAvg)
		{
			maxGreyAvg = exposureParameter.iGreyAvg;
		}


		if (exposureParameter.iGreyAvg>ExposureParameter::GREY_MIN && exposureParameter.iGreyAvg<ExposureParameter::GREY_MAX)
		{
			bFindOptimalExposeTime = true;
			break;
		}

		//2�����к���
		if (exposureParameter.iGreyAvg<128)
		{
			fLowExposeTime = fMidExposeTime;
		}else{
			fHightExposeTime = fMidExposeTime;
		}

	}
	exposureTime = fMidExposeTime;

	//1.����ҵ��ˣ����õ�����У�����ʾ�����ҳɹ�
	if (bFindOptimalExposeTime)
	{
		setExposure(fMidExposeTime);
		//����д
		/*CString strTemp;
		strTemp.Format(_T("%.2f"),fMidExposeTime);
		GetDlgItem(m_hwnd, IDC_EDIT_EXPOSURE)->SetWindowText(strTemp);
		AfxMessageBox(_T("�Զ��ع�ɹ���"));*/

		exposureTime = fMidExposeTime;
		csMsg.Format(_T("auto exposure success!"));
		return CAMERA_STATUS_OK;
	}else{
		//2.������ʾ���Ҳ��ɹ�
		if (minGreyAvg>128)
		{
			//AfxMessageBox(_T("�Զ��ع�ʧ�ܣ����С��Ȧ��"));
			csMsg.Format(_T("auto exposure failure ,please turn down aperture!"));
			return CAMERA_STATUS_ERROR;
		}

		if (maxGreyAvg <128)
		{
			csMsg.Format(_T("auto exposure failure ,please turn up aperture!"));
			return CAMERA_STATUS_ERROR;
		}
	}
	return CAMERA_STATUS_ERROR;
}

bool CCamera::takeImage(BYTE *pbFrame)
{
	bool bWasPreviewing = false;

	if (pbFrame == NULL) return (false);

	if (!m_bPreviewing)
	{
		return false;
	}
	else
		bWasPreviewing = true;

	if (!LucamTakeVideo(m_hCamera, 1, pbFrame))
	{
		DWORD dwError = LucamGetLastError();
		return (false);
	}

	return (true);
}


//�����������ʹ��16λ���ز��ܱ����tifͼƬ�����򱨴�
CameraStatus CCamera::savePictureTo16TIFF()
{
	if(!this->m_bConnected){
		this->csMsg.Format(_T("please connect ccd,then get Frame Data��"));
		return CameraStatus::CAMERA_STATUS_ERROR;
	}else{
		if (!this->m_bPreviewing)
		{
			this->csMsg.Format(_T("please preview the video,then save picture��"));
			return CameraStatus::CAMERA_STATUS_ERROR;

		}else{

			ULONG pixeFormat = LUCAM_PF_16;//�����16λ��
			bool is16Bit = true;
			if (m_frameFormat.pixelFormat != pixeFormat)
			{
				//m_frameFormat.pixelFormat = pixeFormat;
				//is16Bit = false;//ԭ������16λ������������֡��ʽ���������׵��²�����������֡���ݡ�����ʹ���ȸ���Ϊ16λ��֮����ʹ�øú�������ͼƬ
				//stopPreview(m_hwnd);
				//if (!LucamSetFormat(m_hCamera,&m_frameFormat,m_fFrameRate))
				//{
				//	csMsg.Format(_T("set Frame format failure!\n"));
				//	return CCD_STATUS_ERROR;
				//}
				//startPreview(m_hwnd);
				this->csMsg.Format(_T("pixe format is 8 bit,not allow to save 16 tif image!"));
				return CAMERA_STATUS_ERROR;
			}

			ULONG iSize = m_frameFormat.height*m_frameFormat.width/(m_frameFormat.subSampleX*m_frameFormat.subSampleY);

			if (m_frameFormat.pixelFormat == LUCAM_PF_16)
			{
				iSize = iSize*2;
			}

			LUCAM_IMAGE_FORMAT lifTest;

			BYTE* pbFrame = new BYTE[iSize];
			if(pbFrame ==NULL){
				this->csMsg.Format(_T("memery is not enough!"));
				return CameraStatus::CAMERA_STATUS_ERROR;
			}
			if (!takeImage(pbFrame))
			{
				AfxMessageBox(_T("Failed to capture image."),MB_OK);
				delete[] pbFrame;
				return CAMERA_STATUS_ERROR;
			}


			LUCAM_CONVERSION lcConversion;
			// Convert to RGB
			lcConversion.CorrectionMatrix = LUCAM_CM_FLUORESCENT;
			lcConversion.DemosaicMethod = LUCAM_DM_HIGHER_QUALITY;

			USHORT* pBitmap = new USHORT[iSize*3];
			if (pBitmap == NULL){
				this->csMsg.Format(_T("Could not allocate necessary memory to save image. Image not saved."));
				return CAMERA_STATUS_ERROR;
			}

			int iWidth = m_frameFormat.width/m_frameFormat.subSampleX;
			int iHeight = m_frameFormat.height/m_frameFormat.subSampleY;

			if (!LucamConvertFrameToRgb48(m_hCamera,pBitmap,(USHORT*)pbFrame,iWidth,iHeight,m_frameFormat.pixelFormat,&lcConversion))
			{
				delete[] pBitmap;
				pBitmap = NULL;
				csMsg.Format(_T("Failed to convert raw frame. Image not saved. Error %d."),LucamGetLastError());
				return CAMERA_STATUS_ERROR;
			}



			if (!LucamSaveImageEx(m_hCamera,m_frameFormat.width,m_frameFormat.height,LUCAM_PF_48,(BYTE*)pBitmap,"F:/temp.tif"))
			{
				this->csMsg.Format(_T("save to tif picture failure��"));
				return CameraStatus::CAMERA_STATUS_ERROR;
			} 
			else
			{
				this->csMsg.Format(_T("save to 16bit tif picture success��"));
			}

			delete[] pBitmap;
			delete[] pbFrame;
			return CameraStatus::CAMERA_STATUS_OK;
		}

	}
}

CameraStatus CCamera::savePictureToJPEG(CStringA& filePath)
{
	if(!this->m_bConnected){
		this->csMsg.Format(_T("please connect ccd,then get Frame Data��"));
		return CameraStatus::CAMERA_STATUS_ERROR;
	}else{
		if (!this->m_bPreviewing)
		{
			this->csMsg.Format(_T("please preview the video,then save picture��"));
			return CameraStatus::CAMERA_STATUS_ERROR;

		}else{
			//����ط�������8λ����16λ���ܽ��б��棬���Ǳ����jpgͼƬ,�����������ʾ�Ĺ����ж���8λ�ġ�
			//���ֻ��Ϊ������ͼ����ʾ�������������Ϳ����ˣ�������������ͼƬ���б���

			ULONG iSize = (m_frameFormat.width*m_frameFormat.height)/(m_frameFormat.subSampleX*m_frameFormat.subSampleY);
			if (m_frameFormat.pixelFormat == LUCAM_PF_16)
			{
				iSize = iSize*2;
			}


			BYTE* pbFrame = new BYTE[iSize];
			if(pbFrame ==NULL){
				this->csMsg.Format(_T("memery is not enough!"));
				return CameraStatus::CAMERA_STATUS_ERROR;
			}

			if (!takeImage(pbFrame))
			{
				this->csMsg.Format(_T("Failed to capture image."));
				delete[] pbFrame;
				return CAMERA_STATUS_ERROR;
			}

			LUCAM_CONVERSION lcConversion;

			// Convert to RGB
			lcConversion.CorrectionMatrix = LUCAM_CM_FLUORESCENT;
			lcConversion.DemosaicMethod = LUCAM_DM_HIGHER_QUALITY;

			BYTE* pBitmap = new BYTE[iSize*3];
			if (pBitmap == NULL){
				return CAMERA_STATUS_ERROR;
			}

			int iWidth = m_frameFormat.width/m_frameFormat.subSampleX;
			int iHeight = m_frameFormat.height/m_frameFormat.subSampleY;

			if (!LucamConvertFrameToRgb24(m_hCamera,pBitmap,pbFrame,iWidth,iHeight,m_frameFormat.pixelFormat,&lcConversion))
			{
				delete[] pBitmap;
				pBitmap = NULL;
				return CAMERA_STATUS_ERROR;
			}


			if (!LucamSaveImageEx(m_hCamera,m_frameFormat.width,m_frameFormat.height,LUCAM_PF_24,pBitmap,filePath.GetBuffer(0)))
			{
				this->csMsg.Format(_T("save to jpg picture failure��"));
				TRACE("---save to jpg picture failure��---\n");
				return CameraStatus::CAMERA_STATUS_ERROR;
			} 
			else
			{
				this->csMsg.Format(_T("save to jpg picture success��"));
				TRACE("---save to jpg picture success��---\n");
			}

			delete[] pBitmap;
			delete[] pbFrame;
			return CameraStatus::CAMERA_STATUS_OK;
		}

	}
}


//�������д�Ĳ���ʵʱ���������״̬��Ϣ---�ȶ�֮�󣬵ø�
//ExposureParameter CCamera::takePictureCalculate(FLOAT fExposeTime)
//{
//	ULONG iSize = m_frameFormat.width*m_frameFormat.height;
//	BYTE* pFrame;
//	BYTE* pGreyData;//����Ҷ����ݣ�
//
//	if (m_frameFormat.pixelFormat == LUCAM_PF_16)
//		iSize *= 2;
//	pFrame = new BYTE[iSize];
//	if (pFrame==NULL)
//	{
//		AfxMessageBox(_T("�ڴ����ʧ�ܣ�"));
//		return NULL;
//	}
//
//	pGreyData = new BYTE[iSize];
//	if (pGreyData==NULL)
//	{
//		AfxMessageBox(_T("�ڴ����ʧ�ܣ�"));
//		return NULL;
//	}
//
//	setExposure(fExposeTime);
//	Sleep(80);
//	//��ȡͼƬ
//	if (!takeImage(pFrame))
//	{
//		AfxMessageBox(_T("��ȡͼ������ʧ�ܣ�"));
//		return NULL;
//	}
//
//
//	LUCAM_CONVERSION lcConversion;
//	lcConversion.DemosaicMethod = LUCAM_DM_HIGHER_QUALITY;
//	lcConversion.CorrectionMatrix= LUCAM_CM_FLUORESCENT;
//	//��ȡ�Ҷ�����
//	LucamConvertFrameToGreyscale8(m_hCamera,pGreyData,pFrame,m_frameFormat.width/m_frameFormat.subSampleY,m_frameFormat.height/m_frameFormat.subSampleY,m_frameFormat.pixelFormat,&lcConversion);
//
//	ExposureParameter exposureparameter =ExposureParameter::processData(fExposeTime,pGreyData,m_frameFormat.width/m_frameFormat.subSampleX,m_frameFormat.height/m_frameFormat.subSampleY);
//
//	delete[] pFrame;
//	delete[] pGreyData;
//
//	return exposureparameter;
//}


CameraStatus CCamera::takePictureCalculate(FLOAT fExposeTime,ExposureParameter& exposureParameter)
{
	ULONG iSize = m_frameFormat.width*m_frameFormat.height;
	BYTE* pFrame;
	BYTE* pGreyData;//����Ҷ����ݣ�

	if (m_frameFormat.pixelFormat == LUCAM_PF_16)
		iSize *= 2;
	pFrame = new BYTE[iSize];
	if (pFrame==NULL)
	{
		//AfxMessageBox(_T("�ڴ����ʧ�ܣ�"));
		this->csMsg.Format(_T("alloc memery failure!"));
		return CAMERA_STATUS_ERROR;
	}

	pGreyData = new BYTE[iSize];
	if (pGreyData==NULL)
	{
		this->csMsg.Format(_T("allocate memery failure!"));
		return CAMERA_STATUS_ERROR;
	}

	setExposure(fExposeTime);
	Sleep(80);
	//��ȡͼƬ
	if (!takeImage(pFrame))
	{
		this->csMsg.Format(_T("get frame data failure!"));
		return CAMERA_STATUS_ERROR;
	}


	LUCAM_CONVERSION lcConversion;
	lcConversion.DemosaicMethod = LUCAM_DM_HIGHER_QUALITY;
	lcConversion.CorrectionMatrix= LUCAM_CM_FLUORESCENT;
	//��ȡ�Ҷ�����
	LucamConvertFrameToGreyscale8(m_hCamera,pGreyData,pFrame,m_frameFormat.width/m_frameFormat.subSampleY,m_frameFormat.height/m_frameFormat.subSampleY,m_frameFormat.pixelFormat,&lcConversion);

	 exposureParameter =ExposureParameter::processData(fExposeTime,pGreyData,m_frameFormat.width/m_frameFormat.subSampleX,m_frameFormat.height/m_frameFormat.subSampleY);

	delete[] pFrame;
	delete[] pGreyData;

	return CAMERA_STATUS_OK;
}

