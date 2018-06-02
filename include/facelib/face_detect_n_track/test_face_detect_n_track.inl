
#include "cstd.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "VideoFaceDetector.h"

const cv::String    WINDOW_NAME("Camera video");
const cv::String    CASCADE_FILE("haarcascade_frontalface_default.xml");

#ifdef _WIN64
#pragma comment(lib,"opencv_world320.lib")
#pragma comment(lib,"opencv_ts320.lib")
#pragma comment(lib,"strmiids.lib")
#else
#ifdef _DEBUG
#pragma comment(lib,"opencv_world300d.lib")
#else
#pragma comment(lib,"opencv_world300.lib")
#endif
#endif

#include "VideoFaceDetector.inl"
#include "cap.h"

using namespace cv;

int sub_test_opencv_xml_write(void)
{
	// �����ļ��洢����  
	CvFileStorage *fs = cvOpenFileStorage("test.json", 0, CV_STORAGE_WRITE);
	// дע��  
	cvWriteComment(fs, "����дXML�ļ�", 1);

	// ��ʼд�ṹ��������ͼmap��Ҳ���������ֵ�����ڵ㼯��  
	cvStartWriteStruct(fs, "Employee", CV_NODE_MAP);
	// ע��  
	cvWriteComment(fs, "MAP���ͣ����������䣬нˮ", 1);
	// ����  
	cvWriteString(fs, "name", "��Խ");
	// ����  
	cvWriteInt(fs, "age", 18);
	// нˮ  
	cvWriteReal(fs, "salary", 2780.3);
	// ���۴���  
	cvWriteInt(fs, "sales_count", 4);
	{
		// ���۾�������  
		int sales_record[] = { 30000,4200,50090 };
		// ע��  
		cvWriteComment(fs, "SEQ���ͣ����ۼ�¼", 1);
		// ��ʼд�ṹ������������sequence�������ֵ�����ڵ㼯��  
		cvStartWriteStruct(fs, "sales_record", CV_NODE_SEQ);
		// ǰ3�����ۼ�¼  
		cvWriteRawData(fs, sales_record, 3, "i");
		// ��4�����ۼ�¼��ע��������  
		cvWriteInt(fs, 0, 6100);
		// ����  
		cvEndWriteStruct(fs);
	}
	{
		// ע��  
		cvWriteComment(fs, "MAP���ͣ�����", 1);
		// ��ʼ  
		cvStartWriteStruct(fs, "Parent", CV_NODE_MAP);
		// ����  
		cvWriteString(fs, "father", "��˴");
		// ĸ��  
		cvWriteString(fs, "mother", "����");
		// ����  
		cvEndWriteStruct(fs);
	}
	// ����  
	cvEndWriteStruct(fs);
	// �ͷ��ļ��洢����  
	cvReleaseFileStorage(&fs);
	return 0;
}

int test_face_detect_n_track() {
	capdev cap[1] = { 0 };
	int h = 720, w = 1280;
#ifdef _WIN32
	sys_chdir("E:/code/cstd/include/facelib/face_detect_n_track");
#endif
	sub_test_opencv_xml_write();
	if (capdshow_open(cap, 0, w, h) > 0) {
		int fram = 0;
		char ch = 0;
		img_t im[1] = { 0 };
		cap_getframe(cap, im, CAP_RGB);
		Mat image;
		h = 480, w = 640;
		h = im->h, w = im->w;
		VideoFaceDetector detector(CASCADE_FILE);
		for (; 'q' != ch && 'Q' != ch; ++fram) {
			cap_getframe(cap, im, CAP_RGB);
			image = Mat(h, w, CV_8UC3, im->tt.data, im->s);
			{utime_start(_start_time);
			detector.findFace(image);
			printf("%f\n", utime_elapsed(_start_time)); }

			if (detector.isFaceFound())
			{
				cv::rectangle(image, detector.face(), cv::Scalar(255, 0, 0), 2);
				cv::circle(image, detector.facePosition(), 30, cv::Scalar(0, 255, 0), 2);
			}
			imshow_(im);
			ch = WaitKey(10);
		}
		cap_close(cap);
		imfree(im);
	}
	return 0;
}




int test_face_detect_n_track1(int argc, char** argv)
{
#ifdef _WIN32
	sys_chdir("E:/code/cstd/include/facelib/face_detect_n_track");
#endif
	// Try opening camera
	cv::VideoCapture camera(0);
	//cv::VideoCapture camera("D:\\video.mp4");
	if (!camera.isOpened()) {
		fprintf(stderr, "Error getting camera...\n");
		exit(1);
	}

	cv::namedWindow(WINDOW_NAME, cv::WINDOW_KEEPRATIO | cv::WINDOW_AUTOSIZE);

	VideoFaceDetector detector(CASCADE_FILE);
	cv::Mat image;
	double fps = 0, time_per_frame;
	while (true)
	{
		camera >> image;
		auto start = cv::getCPUTickCount();
		detector.findFace(image);
		auto end = cv::getCPUTickCount();

		time_per_frame = (end - start) / cv::getTickFrequency();
		fps = (15 * fps + (1 / time_per_frame)) / 16;

		printf("Time per image: %3.3f\tFPS: %3.3f\n", time_per_frame, fps);

		if (detector.isFaceFound())
		{
			cv::rectangle(image, detector.face(), cv::Scalar(255, 0, 0));
			cv::circle(image, detector.facePosition(), 30, cv::Scalar(0, 255, 0));
		}
		
		cv::imshow(WINDOW_NAME, image);
		if (cv::waitKey(25) == 27) break;
	}

	return 0;
}