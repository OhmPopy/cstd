
#include "cstd.h"

typedef struct dicom_t {
  int unused;
} dicom_t;

enum IMAGEFORMAT {
  IF_UNKNOWN,
  IF_BMP = 1,
  IF_GIF = 2,
  IF_JPG = 3,
  IF_PNG = 4,
  IF_ICO,
  IF_TIF,
  IF_TGA,
  IF_PCX,
  IF_WBMP,
  IF_WMF,
  IF_JP2,
  IF_JPC,
  IF_PGX,
  IF_PNM,
  IF_RAS,
  IF_JBG,
  IF_MNG,
  IF_SKA,
  IF_RAW,
  IF_DICOM
};
struct POSITIONINGLINEDATA {
  IPOINT pt1;
  IPOINT pt2;
  str_t strSerialUID; //����UID
  str_t strSerialInstanceNum; //���к�
  str_t strInstanceNum; //Ӱ���
  str_t strSOPInstanceUID;
};
struct Equation { //��άֱ��һ�㷽��:ax+by+c=0;
  double a;
  double b;
  double c;
};

struct PLANE { // ƽ��Ľ������� a*x+b*y+c*z+d=0 Ϊͳһ��ʾ��Լ�� a >= 0
  double a;
  double b;
  double c;
  double d;
};

struct Point3D { //�ռ��
  double x;
  double y;
  double z;
};

#define DICOMINFO_STR_DEF_DEF(DICOMINFO_STR_DEF) \
  DICOMINFO_STR_DEF(Modality, ͼ������) \
  DICOMINFO_STR_DEF(PatientsName, ��������) \
  DICOMINFO_STR_DEF(PatientsAge, ��������) \
  DICOMINFO_STR_DEF(PatientID, ����ID) \
  DICOMINFO_STR_DEF(PatientsBirthDate, ���˳�������) \
  DICOMINFO_STR_DEF(Manufacturer, ������) \
  DICOMINFO_STR_DEF(InstitutionName, ��������) \
  DICOMINFO_STR_DEF(ManufacturersModelName, �ͺ�) \
  DICOMINFO_STR_DEF(GantryDetectorTilt, �豸��б��) \
  DICOMINFO_STR_DEF(ContentTime, ͼ����ʱ��) \
  DICOMINFO_STR_DEF(BodyPartExamined, ��鲿λ) \
  DICOMINFO_STR_DEF(StudyTime, ���ʱ��) \
  DICOMINFO_STR_DEF(StudyDate, �������) \
  DICOMINFO_STR_DEF(KVP, ��ѹ) \
  DICOMINFO_STR_DEF(XRayTubeCurrent, ����) \
  DICOMINFO_STR_DEF(SliceThickness, ��� ) \
  DICOMINFO_STR_DEF(SliceLocation, ��Ƭλ��) \
  DICOMINFO_STR_DEF(StudyInstanceUID, ���UID) \
  DICOMINFO_STR_DEF(StudyID, ���ID) \
  DICOMINFO_STR_DEF(SeriesInstanceUID, ����UDI) \
  DICOMINFO_STR_DEF(SeriesNumber, ���б��) \
  DICOMINFO_STR_DEF(InstanceNumber, ͼ����) \
  DICOMINFO_STR_DEF(ExposureTime, �ع�ʱ��) \
  DICOMINFO_STR_DEF(PerformingPhysiciansName, ���ҽ��) \
  DICOMINFO_STR_DEF(InstitutionalDepartmentName, �ͼ����) \
  DICOMINFO_STR_DEF(SOPInstanceUID, SOPInstanceUID) \
  DICOMINFO_STR_DEF(FrameOfReferenceUID, FrameOfReferenceUID) \
  DICOMINFO_STR_DEF(AccessionNumber, �����ˮ��) \
  DICOMINFO_STR_DEF(ImageType, ͼƬ����) \
   

struct DICOMINFO {
  //#define DICOMINFO_STR_DEF(name, nameinfo)  str_t str ## name[1];
#define DICOMINFO_STR_DEF(name, nameinfo)  char str ## name[32];
#define DICOMINFO_FLOAT_DEF(name, nameinfo)  double d ## name[32];
  DICOMINFO_STR_DEF_DEF(DICOMINFO_STR_DEF)
#undef DICOMINFO_STR_DEF

  //
  Point3D ImagePosition; //ԭ��
  Point3D ImageOrientationX; //X��������
  Point3D ImageOrientationY; //Y��������
  double dPixelSpacingX; //X
  double dPixelSpacingY; //Y �����ؼ�ľ���
  BOOL bIsPositioningImage;//�Ƿ��Ƕ�λͼ
  int iAllFrame; //ͼ��֡��
  char strFilePath[MAX_PATH]; //ͼ��·��
  int WinWidth; //
  int WinCenter;
};
#define WND_BREAKE 2
#define ST_FIT 0 //�ʺϴ���
#define ST_NORMAL 1 //ʵ�ʴ�С
#define ST_ZOOM_OUT 2 //�Ŵ�
#define ST_ZOOM_IN 3 //��С