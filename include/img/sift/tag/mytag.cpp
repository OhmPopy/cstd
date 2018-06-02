/*#include <vld.h>*/
#include "nrutil.h"
#include "GrayShow.h"
#include "SIFT.h"
#include <iostream>
#include <string>
#include <math.h>
#include <io.h>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdio.h> 
#include <functional>
#include  <assert.h> 




int HistogramBlock = 256;
float HistogramRange1[2]={0,255};
float *HistogramRange[1]={&HistogramRange1[0]}; 

using namespace std;
void siftfeature(const char * imgname, vector <vector<double> > & siftf);
void texturefeature(const char * imgname,double *texturef);//�Ҷȹ��־���������������
void featurevec(vector <double> & tvec,vector <vector<double> >  & svec,string img) ;//��ȡ�����е�����ֵ
void HSVcolor(string imgname,string imgname1,double & HSVval);
double Eu(vector <double> vector1,vector <double> vector2);//����ŷʽ����
typedef BOOL (WINAPI *EnumerateFunc) (LPCSTR lpFileOrPath,vector<string> &pUserData);
void doFileEnumeration(const char *  lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, vector<string> &pUserData);
BOOL WINAPI myEnumerateFunc(LPCSTR lpFileOrPath, vector<string> &pUserData);
int siftnum(vector<vector<double> > vector1,vector<vector<double> > vector2);
struct tagNode{	string name;	double tnum;	double    tagnum;};
bool namegreatermark(const tagNode& s1,const tagNode& s2)//������������
{
	return s1.name < s2.name;
}
bool tnumlessmark(const tagNode& s1,const tagNode& s2)//����ֵ��������
{
	return s1.tnum > s2.tnum;
}
int   remove_tail(char*   str);
void simfilevsdir(string img1,string path,vector<tagNode> & rst);


int main()
{
	
	unsigned long ticks1,ticks2;
	
	string path;
	cout<<"����ͼ��";
	cin>>path;	
	string imgdir;
	vector<string> pathvec;
	// 	cout<<"����ͼ��";
	// 	cin>>imgdir;
	ticks1=   GetTickCount();//������ʱ
	//	doFileEnumeration(imgdir.c_str(), TRUE, TRUE, myEnumerateFunc,  pathvec);//�����ļ��У���ȡ·��
	string img;
	cout<<"����ͼ��";
	cin>>img;
	
	vector<tagNode> rst;
	simfilevsdir(img,path,rst);
	for (int j=rst.size()-1;j>=0;j--)
	{
		cout<<rst[j].name<<"  "<<rst[j].tnum<<"  "<<rst[j].tagnum<<endl;
    }
	
	ticks2=   GetTickCount();//������ʱ
	cout<<ticks2-ticks1<<endl;
	return 0;
}


void simfilevsdir(string img1,string path,vector<tagNode> & rst)//����һ��ͼ�������ݼ����ƶ�����
{
	vector<double>imgcvec1,imgtvec1;
	vector<vector<double> >imgsvec1;
	IplImage * img1test=cvLoadImage(img1.c_str(),1);
	if(!img1test)
	{
		cout<<img1<<"   ͼ���ʽ����"<<endl;
		return ;
	}
	cvReleaseImage(&img1test);
	featurevec(imgtvec1,imgsvec1,img1);//����Դͼ�����sift����
	// 	if (!imgtvec1[0])
	// 	{
	// 		return ;
	// 	}
	
	vector<string> pathvec;
	doFileEnumeration(path.c_str(), TRUE, TRUE, myEnumerateFunc,  pathvec);//�����ļ��У���ȡ·��
	string img2;
	vector<double>imgtvec2;
	vector<vector<double> >imgsvec2;
	int num=0;
	double Dsim=0;
	double Deu=0;
	int i=0;
	vector<tagNode> tstu;
	vector<tagNode> sstu;
	vector<tagNode> HSVhis;
	double HSVval;
	tagNode tag;
	for( i=0;i<pathvec.size();i++)
	{
		//	system("CLS");
		cout<<'\r'<<i+1<<" ";
		img2=pathvec[i];
		IplImage * img2test=cvLoadImage(img2.c_str(),1);
		if(!img2test)
		{
			cout<<img2<<"   ͼ���ʽ����"<<endl;
			continue ;
		}
		cvReleaseImage(&img2test);
		featurevec(imgtvec2,imgsvec2,img2);
		// 		if (!imgtvec2[0])
		// 		{
		// 			continue;
		// 		}
		//�����������룬�Լ�sift������ƥ�����
		HSVcolor(img1,img2,HSVval);
		num=siftnum(imgsvec1,imgsvec2);
		Deu=Eu(imgtvec1,imgtvec2);
		tag.name=img2;
		tag.tnum=Deu;
		tstu.push_back(tag);
		tag.tnum=num;
		sstu.push_back(tag);
		tag.tnum=HSVval;
		HSVhis.push_back(tag);
		imgtvec2.erase(imgtvec2.begin(), imgtvec2.end());
		imgsvec2.erase(imgsvec2.begin(), imgsvec2.end());
	}
	sort(tstu.begin(), tstu.end(),tnumlessmark) ; //��������//�ֱ�Ӵ�С���򣬲�������������	
	sort(sstu.begin(), sstu.end(),tnumlessmark) ; //��������
	sort(HSVhis.begin(), HSVhis.end(),tnumlessmark) ; //��������
	
	double w=0;
	int tagsize=sstu.size();
	// 	if (sstu[0].tnum<5)
	// 	{
	// 		for (i=0;i<tagsize;i++)
	// 		{	
	// 			sstu[i].tagnum=tagsize;//������ƥ�����С��5���������д�Ϊ��ͬ�ִӶ���Ӱ������
	// 		}
	// 		w=tagsize;
	// 	} 
	// 	else 
	// 	{
	// 			for (i=0;i<tagsize;i++)
	//  		{	
	//  			sstu[i].tagnum=2*tagsize*sstu[i].tnum/sstu[0].tnum;
	//  		}
	// 			w=tagsize;
	// 	};
	
	for (i=0;i<tagsize;i++)
	{
		if (	tstu[i].tnum==0)
		{
			tstu[i].tagnum=10*tstu[0].tnum/tstu[tagsize-2].tnum;
		} 
		else
		{
			tstu[i].tagnum=tstu[0].tnum/tstu[i].tnum;//���������������ĸ���ͷ�
		}
		if (HSVhis[i].tnum==0)
		{
			HSVhis[i].tagnum=10*HSVhis[0].tnum/HSVhis[tagsize-2].tnum;
		} 
		else
		{
			HSVhis[i].tagnum=HSVhis[0].tnum/HSVhis[i].tnum;//��Bhattacharyya�����ĸ���ͷ�
		}
		if (sstu[i].tnum==0)
		{
			sstu[i].tagnum=0;
		} 
		else
		{
			sstu[i].tagnum=(sstu[i].tnum+1)/(sstu[tagsize-1].tnum+1);
		}
		
	}
	ofstream outfile("result.txt");
	outfile<<"����"<<endl;
	for (i=0;i<tstu.size();i++)
	{
		outfile<<tstu[i].name<<" "<<tstu[i].tnum<<"  "<<tstu[i].tagnum<<endl;
	}
	
	outfile<<"HSV"<<endl;
	for (i=0;i<tstu.size();i++)
	{
		outfile<<HSVhis[i].name<<" "<<HSVhis[i].tnum<<"  "<<HSVhis[i].tagnum<<endl;
	}
	outfile<<"sift"<<endl;
	for (i=0;i<tstu.size();i++)
	{
		outfile<<sstu[i].name<<" "<<sstu[i].tnum<<"  "<<sstu[i].tagnum<<endl;
	}
	// 		for (i=0;i<tstu.size();i++)
	// 	{
	// 		cout<<tstu[i].name<<" "<<tstu[i].tnum<<"  "<<tstu[i].tagnum<<endl;
	// 	}
	// 		for (i=0;i<tstu.size();i++)
	// 	{
	// 		cout<<sstu[i].name<<" "<<sstu[i].tnum<<"  "<<sstu[i].tagnum<<endl;
	// 	}
	// 			for (i=0;i<tstu.size();i++)
	// 	{
	// 		cout<<HSVhis[i].name<<" "<<HSVhis[i].tnum<<"  "<<HSVhis[i].tagnum<<endl;
	// 	}
	//���°������ֽ�������ͳ����������������
	sort(tstu.begin(), tstu.end(),namegreatermark) ; 
	sort(sstu.begin(), sstu.end(),namegreatermark) ; 
	sort(HSVhis.begin(), HSVhis.end(),namegreatermark) ; 
	
	for (i=0;i<tagsize;i++)
	{
		tag.name=tstu[i].name;
		tag.tnum=(tstu[i].tagnum+sstu[i].tagnum+HSVhis[i].tagnum)/(tstu[0].tagnum+sstu[tagsize-1].tagnum+HSVhis[0].tagnum);
		tag.tagnum=0;
		rst.push_back(tag);
	}
	sort(rst.begin(), rst.end(),tnumlessmark) ; //��ƽ���ֽ�������
	for (i=0;i<tagsize;i++) 
	{
		rst[i].tagnum=i+1;
	}
	// 	for (int j=0;j<tagsize;j++)
	// 	{
	// 		cout<<rst[j].name<<"  "<<rst[j].tnum<<"  "<<rst[j].tagnum<<endl;
	// 	}
	
	outfile<<"�����"<<endl;
	
	for (i=0;i<tstu.size();i++)
	{
		outfile<<rst[i].name<<" "<<rst[i].tnum<<"  "<<rst[i].tagnum<<endl;
	}
	
	// 	for (i=0;i<apvec.size();i++)
	// 	{
	// 		cout<<apvec[i]<<endl;
	// 	}
	
}

int   remove_tail(char*   str)
{
	int   len   =   strlen(str);
	int   i;
	
	for(i=len-1; i>= 0; i--)
	{              
		if   (str[i] ==   '\\')
		{
			break;
		}
	}
	return i;
	
	
} 


//��ȡHSVֱ��ͼ��������������Bthattacharyya���� 
void HSVcolor(string imgname,string imgname1,double & HSVval)
{
	double  Hval, Sval, Vval;
	IplImage *src=cvLoadImage(imgname.c_str(),1); 
	IplImage* src_hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	
	int h_bins = 16, s_bins = 8,v_bins=8;
	
	float h_ranges[] = { 0, 180 }; 
	float s_ranges[] = { 0, 255 };
	float v_ranges[] = { 0, 255 };
	float* hrangearr[]={h_ranges};
	float* srangearr[]={s_ranges};
	float* vrangearr[]={v_ranges};
	
	cvCvtColor( src, src_hsv, CC_BGR2HSV );
	cvCvtPixToPlane( src_hsv, h_plane, s_plane, v_plane, 0 );
	
	CvHistogram * hhist = cvCreateHist( 1, &h_bins, CC_HIST_ARRAY, hrangearr, 1 );
	CvHistogram * shist = cvCreateHist( 1, &s_bins, CC_HIST_ARRAY, srangearr, 1 );
	CvHistogram * vhist = cvCreateHist( 1, &v_bins, CC_HIST_ARRAY, vrangearr, 1 );
	//����ֱ��ͼ	
	cvCalcHist( &h_plane, hhist, 0, 0 );
	cvCalcHist( &s_plane, shist, 0, 0 );
	cvCalcHist( &v_plane, vhist, 0, 0 );
	//�ڶ���ͼ
	IplImage *src1=cvLoadImage(imgname1.c_str(),1); 
	
	IplImage* src1_hsv = cvCreateImage( cvGetSize(src1), 8, 3 );
	IplImage* h1_plane = cvCreateImage( cvGetSize(src1), 8, 1 );
	IplImage* s1_plane = cvCreateImage( cvGetSize(src1), 8, 1 );
	IplImage* v1_plane = cvCreateImage( cvGetSize(src1), 8, 1 );
	
	cvCvtColor( src1, src1_hsv, CC_BGR2HSV );
	cvCvtPixToPlane( src1_hsv, h1_plane, s1_plane, v1_plane, 0 );
	CvHistogram * hhist1 = cvCreateHist( 1, &h_bins, CC_HIST_ARRAY, hrangearr, 1 );	
	CvHistogram * shist1 = cvCreateHist( 1, &s_bins, CC_HIST_ARRAY, srangearr, 1 );
	CvHistogram * vhist1 = cvCreateHist( 1, &v_bins, CC_HIST_ARRAY, vrangearr, 1 );
	cvCalcHist( &h1_plane, hhist1, 0, 0 );
	cvCalcHist( &s1_plane, shist1, 0, 0 );	
	cvCalcHist( &v1_plane, vhist1, 0, 0 );
	
	//ֱ��ͼ��һ��
	cvNormalizeHist(hhist1,1);
	cvNormalizeHist(shist1,1);
	cvNormalizeHist(vhist1,1);
	cvNormalizeHist(hhist,1);  
	cvNormalizeHist(shist,1);
	cvNormalizeHist(vhist,1);
	//�ֱ����HSV����������Bthattacharyya���� 
	Hval=cvCompareHist(hhist, hhist1, CC_COMP_BHATTACHARYYA);
	Sval=cvCompareHist(shist, shist1, CC_COMP_BHATTACHARYYA);
	Vval=cvCompareHist(vhist, vhist1, CC_COMP_BHATTACHARYYA);
	HSVval=(Hval+Sval+Vval)/3;
	cvReleaseImage(&src_hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&src1_hsv);
	cvReleaseImage(&h1_plane);
	cvReleaseImage(&s1_plane);
	cvReleaseImage(&v1_plane);
	cvReleaseImage(&src);
	cvReleaseImage(&src1);
}



void siftfeature(const char * imgname, vector <vector<double> > & siftf) //sift��ת��������������
{
	
	vector<double> line;
	SIFT *sift = new SIFT(imgname, 4, 2);
	sift->DoSift();	
	for (int i=0;i<sift->m_numKeypoints;i++)
	{
		for (int j=0;j<128;j++)
		{
			line.push_back(sift->m_keyDescs[i].fv[j]);
		}
		siftf.push_back(line);
		line.erase(line.begin(), line.end());		 
	}
	
	delete sift;
}


void texturefeature(const char * imgname,double *texturef)//�Ҷȹ��־���������������
{
	CGrayShow m_grayShow;
	char *strFile;
	int length=strlen(imgname);
	strFile=(char*)pmalloc(length+1);
	for (int i=0;i<length;i++)
	{
		strFile[i]=imgname[i];
	}
	strFile[length]='\0';
	m_grayShow.LoadImage(strFile);
	// 	if (!m_grayShow.ImageArray)
	// 	{
	// 		return ;
	// 	}
	
	double dEnergy			  = 0.0;
	double dEntropy			  = 0.0;
	double dInertiaQuadrature = 0.0;
	double dLocalCalm		  = 0.0;
	double dCorrelation		  = 0.0;
	double dEnergy1			  = 0.0;
	double dEntropy1		  = 0.0;
	double dInertiaQuadrature1= 0.0;
	double dLocalCalm1		  = 0.0;
	double dCorrelation1	  = 0.0;
	unsigned char** arLocalImage;
	arLocalImage = cmatrix(0, m_grayShow.FilterWindowWidth-1, 0, m_grayShow.FilterWindowWidth-1);
	int rolltimeH = m_grayShow.ImageHeight/m_grayShow.FilterWindowWidth;
	int rolltimeW = m_grayShow.ImageWidth /m_grayShow.FilterWindowWidth;
	int j;
	int p,q;
	
	//��ͼ��ֳ����ɸ����ڣ������������ֵ
	for(i=0; i< rolltimeH; i++)
	{
		for(j=0; j<rolltimeW; j++)
		{
			//���ȸ�ֵ���Ӵ���
			for(p=0; p<m_grayShow.FilterWindowWidth; p++)
			{
				for(q=0; q<m_grayShow.FilterWindowWidth; q++)
				{
					arLocalImage[p][q] = m_grayShow.ImageArray[i*m_grayShow.FilterWindowWidth+p][j*m_grayShow.FilterWindowWidth+q];
				}
			}
			m_grayShow.ComputeMatrix(arLocalImage, m_grayShow.FilterWindowWidth);
			m_grayShow.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, m_grayShow.PMatrixH, m_grayShow.GrayLayerNum);
			dEnergy              += dEnergy1;
			dEntropy             += dEntropy1;
			dInertiaQuadrature   += dInertiaQuadrature1;
			dCorrelation         += dCorrelation1;
			dLocalCalm           += dLocalCalm1;
		}
	}
	dEnergy              /= (rolltimeH*rolltimeW);
	dEntropy             /= (rolltimeH*rolltimeW);
	dInertiaQuadrature   /= (rolltimeH*rolltimeW);
	dCorrelation         /= (rolltimeH*rolltimeW);
	dLocalCalm           /= (rolltimeH*rolltimeW);
	texturef[0]=dEnergy;
	texturef[1]=dEntropy;
	texturef[2]=dInertiaQuadrature;
	texturef[3]=dLocalCalm;
	
}


void featurevec(/*vector <double> & cvec,*/vector <double> & tvec,vector <vector<double> >  & svec,string img) //��ȡ�����е�����ֵ
{
	
	
	double tf[4];
	texturefeature(img.c_str(),tf);
	// 	if (!tf[0])
	// 	{
	// 		return ;
	// 	}
	int i=0;
	while (i<4)
	{
		tvec.push_back(tf[i]);
		i++;
	}
	
	siftfeature(img.c_str(),svec);
	
	
}

double Eu(vector <double> vector1,vector <double> vector2)//����ŷʽ����
{
	double D=0;
	for (int i=0;i<vector1.size();i++)
	{
		D+=pow(vector1[i]-vector2[i],2);
	}
    D=sqrt(D);
	return D;
}

int siftnum(vector<vector<double> > vector1,vector<vector<double> > vector2)
{
	int num=0;
	double ratio;
	vector<vector<double> > euvec;
	vector<double> line;
	
	for (int i=0;i<vector1.size();i++)
	{
		for (int j=0;j<vector2.size();j++)
		{
			line.push_back(Eu(vector1[i],vector2[j]));
		}
		sort(line.begin(),line.end());
		ratio=line[0]/line[1];
		if (ratio<siftratio)
		{
			num++;
		}
		euvec.push_back(line);
		line.erase(line.begin(), line.end());
	}
	return num;
}



void doFileEnumeration(const char *  lpPath, BOOL bRecursion, BOOL bEnumFiles, EnumerateFunc pFunc, vector<string> &pUserData)
{
	
	static BOOL s_bUserBreak = FALSE;
	try{
		//-------------------------------------------------------------------------
		if(s_bUserBreak) return;
		
		int len = strlen(lpPath);
		if(lpPath==NULL || len<=0) return;
		
		
		char path[MAX_PATH];
		strcpy(path, lpPath);
		if(lpPath[len-1] != '\\') strcat(path, "\\");
		strcat(path, "*");
		
		WIN32_FIND_DATA fd;
		HANDLE hFindFile = FindFirstFile(path, &fd);
		if(hFindFile == INVALID_HANDLE_VALUE)
		{
			::FindClose(hFindFile); return;
		}
		
		char tempPath[MAX_PATH]; BOOL bUserReture=TRUE; BOOL bIsDirectory;
		
		BOOL bFinish = FALSE;
		while(!bFinish)
		{
			strcpy(tempPath, lpPath);
			if(lpPath[len-1] != '\\') strcat(tempPath, "\\");
			strcat(tempPath, fd.cFileName);
			
			bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);//������ļ���
			
			//�����.��..����
			if( bIsDirectory
				&& (strcmp(fd.cFileName, ".")==0 || strcmp(fd.cFileName, "..")==0)) 
			{		
				bFinish = (FindNextFile(hFindFile, &fd) == FALSE);
				continue;
			}
			
			if(pFunc && bEnumFiles!=bIsDirectory)
			{
				bUserReture = pFunc(tempPath, pUserData);
				if(bUserReture==FALSE)
				{
					s_bUserBreak = TRUE; ::FindClose(hFindFile);
					return;
				}
			}
			
			
			if(bIsDirectory && bRecursion) //����Ŀ¼
			{
				doFileEnumeration(tempPath, bRecursion, bEnumFiles, pFunc, pUserData);
			}
			
			bFinish = (FindNextFile(hFindFile, &fd) == FALSE);
		}
		
		::FindClose(hFindFile);
		
		//-------------------------------------------------------------------------
	}catch(...){ assert(0); return; }
}
BOOL WINAPI myEnumerateFunc(LPCSTR lpFileOrPath, vector<string> &pUserData)
{
	char* pdot;
	if((pdot = strrchr(lpFileOrPath, '.')) && stricmp(pdot, ".jpg") == 0)
	{
		pUserData.push_back(lpFileOrPath);
	}
	return TRUE;
	
}
