#pragma once

//本文件所有命名方式均采用驼峰命名法

#include "ccd/lucamapi.h"
#include "ccd/lucamerr.h"
#include "ccd/lucamsci.h"


enum CameraStatus{
	CAMERA_STATUS_ERROR,
	CAMERA_STATUS_OK
} ;

//曝光的参数类
class ExposureParameter
{
public:
	int iGreyAvg;//灰度均值；
	int iGreyPeak;//灰度峰值；
	FLOAT fERat;//曝光量比值
	int iGreyMin;//灰度最小值
	int iGreyMax;//灰度最大值
	float fExposureTime;//自动曝光时间
	float fGain;//增益，多数情况下用不上
	const static int GREY_MIN = 125;//判断自动曝光时候灰度的最小值
	const static int GREY_MAX = 130;//判断自动曝光时候灰度的最大值
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
		iGreyAvg = 0;//灰度均值；
		iGreyPeak = 0;//灰度峰值；
		fERat = 0.0f;//曝光量比值
		iGreyMin = 0;//灰度图左边缘处像素数量为零的最大灰度值
		iGreyMax = 0;//灰度图右边缘像素数量为零的最小灰度值
		fGain = 0.0f;
		this->fExposureTime = fExposureTime;//曝光时间
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


	//处理数据----得到曝光参数
	static ExposureParameter& processData(float fExposureTime,BYTE*pGreyData,int iWidth,int iHeight){
		// Determine average pixel value
		UINT ulP1Avg  = 0;
		UINT ulP2Avg = 0;
		UINT ulP3Avg = 0;
		UINT ulP4Avg = 0;


		UINT iSize = iWidth*iHeight;
		//灰度是从0-255的
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
				//统计灰度个数
				iGreyCounter[ pGreyData[(iColumn + 1) * iWidth + iRow] ]++;
				iGreyCounter[ pGreyData[(iColumn + 1) * iWidth + iRow+1] ]++;
			}
		}

		int iGreyAvg,iGreyMax,iGreyMin,iGreyPeak;
		float fERat=0.0f;
		iGreyAvg = (ulP1Avg + ulP2Avg + ulP3Avg + ulP4Avg) / iSize;

		bool bFlagMax = true;
		bool bFlagMin = true;

		//这是计算最大和最小灰度的.
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
		//灰度直方图中峰值灰度
		for (int i=iGreyMin;i<iGreyMax;i++){
			if (iGreyCounter[i] >iGreyCounter[iGreyPeak])
			{
				iGreyPeak = i;
			}
		}
		//曝光量比值
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

	////实现垃圾回收器
	//class CGarbo{//唯一的工作是在析构函数中删除CCarmera的实例
	//public:
	//	~CGarbo(){
	//		if (CCamera::m_pCamera!=nullptr)
	//		{
	//			delete m_pCamera;
	//		}
	//	}
	//};
	//static CGarbo garbo;//定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数。

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

	
	//读取曝光时间,曝光时间就更新到成员变量中
	CameraStatus getExposure(FLOAT& fExposure);

	//设置曝光时间
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
	/* 连接相机                                                              */
	/************************************************************************/
	CameraStatus connect(ULONG ulCamera);

	

	/************************************************************************/
	/* 断开相机的连接                                                                     */
	/************************************************************************/
	CameraStatus disconect();


	//开始预览
	CameraStatus startPreview(HWND hwnd);


	//停止预览
	CameraStatus stopPreview(HWND hwnd);


	CameraStatus setCameraParameter(FLOAT& exposure,INT& width,INT& heigh,FLOAT& gain,INT xOffset,INT& yOffset,BOOL is8Bit);


	CameraStatus getCameraParameter(FLOAT& exposure,INT& width,INT& heigh,FLOAT& gain,INT& xOffset,INT& yOffset,BOOL& is8Bit);
	
	
	//获取帧格式，和帧速率
	CameraStatus getFrameFormat(LUCAM_FRAME_FORMAT& fFormat,FLOAT& fFrameRate);


	/************************************************************************/
	/* 设置自动曝光                                                                     */
	/************************************************************************/
	CameraStatus autoExposure(FLOAT& exposureTime);

	//如果没有预览，就保存不了图片 -- 捕获帧数据
	bool takeImage(BYTE *pbFrame);

	
	CameraStatus savePictureTo16TIFF();

	
	CameraStatus savePictureToJPEG();

	LPCTSTR getCCDMessage(){
		LPCTSTR lpctStr = (LPCTSTR)csMsg;
		return lpctStr;
	}

	//获取CStringA信息
	CStringA getCCDMessageCStringA(){
		CStringA str1;
		str1 = csMsg;
		return str1;
	}

	CameraStatus takePictureCalculate(FLOAT fExposeTime,ExposureParameter& exposureParameter);
//	ExposureParameter takePictureCalculate(FLOAT fExposeTime);


private:

	//所有函数执行完成，都会向msg中写入消息
	CString csMsg;

	//帧格式
	LUCAM_FRAME_FORMAT m_frameFormat;

	//相机的句柄
	HANDLE m_hCamera;

	//连接
	BOOL m_bConnected ;
	
	//保留句柄
	HWND m_hwnd;

	//帧速率
	FLOAT m_fFrameRate;
	
	//相机ID
	ULONG m_ulCameraId;

	BOOL m_bInitMode;

	//预览
	BOOL m_bPreviewing;

	//最大图片宽度
	ULONG m_ulMaxWidth ;
	//最大图片高度
	ULONG m_ulMaxHeight ;

	//曝光量
	FLOAT m_fExposure;
	//gama值
	FLOAT m_fGAMMA;



};

