#pragma once

//���ļ�����������ʽ�������շ�������

#include "ccd/lucamapi.h"
#include "ccd/lucamerr.h"
#include "ccd/lucamsci.h"


enum CameraStatus{
	CAMERA_STATUS_ERROR,
	CAMERA_STATUS_OK
} ;

//�ع�Ĳ�����
class ExposureParameter
{
public:
	int iGreyAvg;//�ҶȾ�ֵ��
	int iGreyPeak;//�Ҷȷ�ֵ��
	FLOAT fERat;//�ع�����ֵ
	int iGreyMin;//�Ҷ���Сֵ
	int iGreyMax;//�Ҷ����ֵ
	float fExposureTime;//�Զ��ع�ʱ��
	float fGain;//���棬����������ò���
	const static int GREY_MIN = 125;//�ж��Զ��ع�ʱ��Ҷȵ���Сֵ
	const static int GREY_MAX = 130;//�ж��Զ��ع�ʱ��Ҷȵ����ֵ
	const static int COUNT_LENGTH = 256;

public:
	ExposureParameter(){

	}

	ExposureParameter(float fExposureTime,int iGreyAvg,int iGreyMax,int iGreyMin,int iGreyPeak,float fERat=0.0){
		this->fExposureTime = fExposureTime;
		this->iGreyAvg = iGreyAvg;
		this->iGreyMax = iGreyMax;
		this->iGreyMin = iGreyMin;
		this->iGreyPeak = iGreyPeak;
		this->fERat = fERat;
	}


	ExposureParameter(float fExposureTime,float fGain,int iGreyAvg,int iGreyMax,int iGreyMin,int iGreyPeak,float fERat=0.0){
		this->fExposureTime = fExposureTime;
		this->fGain = fGain;
		this->iGreyAvg = iGreyAvg;
		this->iGreyMax = iGreyMax;
		this->iGreyMin = iGreyMin;
		this->iGreyPeak = iGreyPeak;
		this->fERat = fERat;
	}

	ExposureParameter(FLOAT fExposureTime){
		iGreyAvg = 0;//�ҶȾ�ֵ��
		iGreyPeak = 0;//�Ҷȷ�ֵ��
		fERat = 0.0f;//�ع�����ֵ
		iGreyMin = 0;//�Ҷ�ͼ���Ե����������Ϊ������Ҷ�ֵ
		iGreyMax = 0;//�Ҷ�ͼ�ұ�Ե��������Ϊ�����С�Ҷ�ֵ
		fGain = 0.0f;
		this->fExposureTime = fExposureTime;//�ع�ʱ��
	}
	virtual ~ExposureParameter(void){

	}

	ExposureParameter& operator= (ExposureParameter& exposureParameter){
		this->iGreyAvg = exposureParameter.iGreyAvg;
		iGreyPeak=exposureParameter.iGreyPeak;
		fERat=exposureParameter.fERat;
		iGreyMin=exposureParameter.iGreyMin;
		iGreyMax = exposureParameter.iGreyMax;
		fGain = exposureParameter.fGain;
		fExposureTime =exposureParameter.fExposureTime;
		return *this;
	}


	//��������----�õ��ع����
	static ExposureParameter& processData(float fExposureTime,BYTE*pGreyData,int iWidth,int iHeight){
		// Determine average pixel value
		UINT ulP1Avg  = 0;
		UINT ulP2Avg = 0;
		UINT ulP3Avg = 0;
		UINT ulP4Avg = 0;


		UINT iSize = iWidth*iHeight;
		//�Ҷ��Ǵ�0-255��
		UINT iGreyCounter[ExposureParameter::COUNT_LENGTH];
		memset(iGreyCounter,0,ExposureParameter::COUNT_LENGTH*sizeof(UINT));


		for (int iColumn = 0; iColumn < iHeight; iColumn += 2)
		{
			// odd rows
			for (int iRow = 0; iRow < iWidth; iRow += 2)
			{
				ulP1Avg += pGreyData[iColumn * iWidth + iRow];
				ulP2Avg += pGreyData[iColumn * iWidth + iRow + 1];
				iGreyCounter[ pGreyData[iColumn * iWidth + iRow] ]++;
				iGreyCounter[ pGreyData[iColumn * iWidth + iRow+1] ]++;
			}
			// even rows
			for (int iRow = 0; iRow < iWidth; iRow += 2)
			{
				ulP3Avg += pGreyData[(iColumn + 1) * iWidth + iRow];
				ulP4Avg += pGreyData[(iColumn + 1) * iWidth + iRow + 1];
				//ͳ�ƻҶȸ���
				iGreyCounter[ pGreyData[(iColumn + 1) * iWidth + iRow] ]++;
				iGreyCounter[ pGreyData[(iColumn + 1) * iWidth + iRow+1] ]++;
			}
		}

		int iGreyAvg,iGreyMax,iGreyMin,iGreyPeak;
		float fERat=0.0f;
		iGreyAvg = (ulP1Avg + ulP2Avg + ulP3Avg + ulP4Avg) / iSize;

		bool bFlagMax = true;
		bool bFlagMin = true;

		//���Ǽ���������С�Ҷȵ�.
		for (int i=0;i<ExposureParameter::COUNT_LENGTH;i++)
		{
			if (bFlagMax  && (0==iGreyCounter[ ExposureParameter::COUNT_LENGTH-i-1 ]))
			{	
			}else if ( bFlagMax &&   0!=iGreyCounter[ExposureParameter::COUNT_LENGTH-i-1 ])
			{
				iGreyMax=ExposureParameter::COUNT_LENGTH-i-1;
				bFlagMax = false;
			}

			if (bFlagMin && (0==iGreyCounter[i]))
			{
			}else if ( bFlagMin && 0 != iGreyCounter[i] )
			{
				iGreyMin=i;
				bFlagMin = false;
			}

			if (!bFlagMin && !bFlagMax)
			{
				break;
			}
		}

		iGreyPeak = 0;
		//�Ҷ�ֱ��ͼ�з�ֵ�Ҷ�
		for (int i=iGreyMin;i<iGreyMax;i++){
			if (iGreyCounter[i] >iGreyCounter[iGreyPeak])
			{
				iGreyPeak = i;
			}
		}
		//�ع�����ֵ
		fERat = (FLOAT)(iGreyMin+1)/(255-iGreyMax+iGreyMin+1);

		return (*(new ExposureParameter(fExposureTime,iGreyAvg,iGreyMax,iGreyMin,iGreyPeak,fERat)));

	}

	CString toString(){

	}
	CString toStringExter(){

	}

};


class CCamera
{
private:
	CCamera(void);
	
	
	static CCamera* m_pCamera;

	////ʵ������������
	//class CGarbo{//Ψһ�Ĺ�����������������ɾ��CCarmera��ʵ��
	//public:
	//	~CGarbo(){
	//		if (CCamera::m_pCamera!=nullptr)
	//		{
	//			delete m_pCamera;
	//		}
	//	}
	//};
	//static CGarbo garbo;//����һ����̬��Ա�������������ʱ��ϵͳ���Զ�������������������

public:
	~CCamera(void);

	static CCamera* getInstance(){
		if (m_pCamera == NULL)
		{
			m_pCamera = new CCamera();
		}
		return m_pCamera;
	}



	CameraStatus getGain(FLOAT& fGain);

	
	//��ȡ�ع�ʱ��,�ع�ʱ��͸��µ���Ա������
	CameraStatus getExposure(FLOAT& fExposure);

	//�����ع�ʱ��
	CameraStatus setExposure(float fExposure){
		LONG lFlags;
		if (m_hCamera!=NULL)
		{
			if (LucamSetProperty(m_hCamera,LUCAM_PROP_EXPOSURE,fExposure,0))
			{
				this->m_fExposure = fExposure;
				csMsg.Format(_T("set Exposure success"));
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


	/************************************************************************/
	/* �������                                                              */
	/************************************************************************/
	CameraStatus connect(ULONG ulCamera);

	

	/************************************************************************/
	/* �Ͽ����������                                                                     */
	/************************************************************************/
	CameraStatus disconect();


	//��ʼԤ��
	CameraStatus startPreview(HWND hwnd);


	//ֹͣԤ��
	CameraStatus stopPreview(HWND hwnd);


	CameraStatus setCameraParameter(FLOAT& exposure,INT& width,INT& heigh,FLOAT& gain,INT xOffset,INT& yOffset,BOOL is8Bit);


	CameraStatus getCameraParameter(FLOAT& exposure,INT& width,INT& heigh,FLOAT& gain,INT& xOffset,INT& yOffset,BOOL& is8Bit);
	
	
	//��ȡ֡��ʽ����֡����
	CameraStatus getFrameFormat(LUCAM_FRAME_FORMAT& fFormat,FLOAT& fFrameRate);


	/************************************************************************/
	/* �����Զ��ع�                                                                     */
	/************************************************************************/
	CameraStatus autoExposure(FLOAT& exposureTime);

	//���û��Ԥ�����ͱ��治��ͼƬ -- ����֡����
	bool takeImage(BYTE *pbFrame);

	
	CameraStatus savePictureTo16TIFF();

	
	CameraStatus savePictureToJPEG();

	LPCTSTR getCCDMessage(){
		LPCTSTR lpctStr = (LPCTSTR)csMsg;
		return lpctStr;
	}

	//��ȡCStringA��Ϣ
	CStringA getCCDMessageCStringA(){
		CStringA str1;
		str1 = csMsg;
		return str1;
	}

	CameraStatus takePictureCalculate(FLOAT fExposeTime,ExposureParameter& exposureParameter);
//	ExposureParameter takePictureCalculate(FLOAT fExposeTime);


private:

	//���к���ִ����ɣ�������msg��д����Ϣ
	CString csMsg;

	//֡��ʽ
	LUCAM_FRAME_FORMAT m_frameFormat;

	//����ľ��
	HANDLE m_hCamera;

	//����
	BOOL m_bConnected ;
	
	//�������
	HWND m_hwnd;

	//֡����
	FLOAT m_fFrameRate;
	
	//���ID
	ULONG m_ulCameraId;

	BOOL m_bInitMode;

	//Ԥ��
	BOOL m_bPreviewing;

	//���ͼƬ���
	ULONG m_ulMaxWidth ;
	//���ͼƬ�߶�
	ULONG m_ulMaxHeight ;

	//�ع���
	FLOAT m_fExposure;
	//gamaֵ
	FLOAT m_fGAMMA;



};

