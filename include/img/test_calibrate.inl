
// OpenCV: ������궨ԭ��

int test_calibrate()
{
  int cube_length = 7;

  CvCapture* capture;

  capture = cvCreateCameraCapture(0);

  if (capture == 0) {
    printf("�޷���������ͷ�豸��\n\n");
    return 0;
  }
  else {
    printf("��������ͷ�豸�ɹ�����\n\n");
  }

  IplImage* frame;

  cvNamedWindow("�����֡��ȡ����", 1); //cvNamedWindow()������������Ļ�ϴ���һ�����ڣ�������ʾ��ͼ������ڸô����С������ĵ�һ������ָ���˸ô��ڵĴ��ڱ���,���Ҫʹ��HighGUI�����ṩ������������ô��ڽ��н���ʱ�����ǽ�ͨ���ò���ֵ����������ڡ�

  printf("����C������ȡ��ǰ֡������Ϊ�궨ͼƬ...\n����Q�����˳���ȡ֡����...\n\n");

  int number_image = 1;
  char* str1;
  str1 = ".jpg";
  char filename[20] = "";

  while (true) {
    frame = cvQueryFrame(capture); // ������ͷ�����ļ���ץȡ������һ֡

    if (!frame) {
      break;
    }

    cvShowImage("�����֡��ȡ����", frame); //ͼ����ʾ


    if (cvWaitKey(10) == 'c') {
      sprintf_s(filename, "%d.jpg", number_image); // int sprintf_s( char *buffer, size_t sizeOfBuffer, const char *format [, argument] ... );


      �����������Ҫ�����ǽ����ɸ�argument����format��ʽ�浽buffer��



      cvSaveImage(filename, frame); //����
      cout << "�ɹ���ȡ��ǰ֡�������ļ���" << filename << "����...\n\n";
      printf("����C������ȡ��ǰ֡������Ϊ�궨ͼƬ...\n����Q�����˳���ȡ֡����...\n\n");
      number_image++;
    }
    else if (cvWaitKey(10) == 'q') {
      printf("��ȡͼ��֡�������...\n\n");
      cout << "���ɹ���ȡ" << --number_image << "֡ͼ�񣡣�\n\n";
      break;
    }
  }

  cvReleaseImage(&frame); //�ͷ�ͼ��

  cvDestroyWindow("�����֡��ȡ����");

  IplImage* show;
  cvNamedWindow("RePlay", 1);

  int a = 1;
  int number_image_copy = number_image;

  CvSize board_size = cvSize(7, 7); // Cvsizes:OpenCV�Ļ�����������֮һ����ʾ������С��������Ϊ���ȡ���CvPoint�ṹ���ƣ������ݳ�Ա��integer���͵�width��height��
  //cvSize��

  int board_width = board_size.width;
  int board_height = board_size.height;
  int total_per_image = board_width * board_height;
  CvPoint2D32f* image_points_buf = new CvPoint2D32f[total_per_image];
  CvMat* image_points = cvCreateMat(number_image * total_per_image, 2, CC_32FC1); //ͼ������ϵ
  CvMat* object_points = cvCreateMat(number_image * total_per_image, 3, CC_32FC1); //��������ϵ
  CvMat* point_counts = cvCreateMat(number_image, 1, CC_32SC1); //
  CvMat* intrinsic_matrix = cvCreateMat(3, 3, CC_32FC1); //
  CvMat* distortion_coeffs = cvCreateMat(5, 1, CC_32FC1);

  int count;
  int found;
  int step;
  int successes = 0;

  while (a <= number_image_copy) {
    sprintf_s(filename, "%d.jpg", a);
    show = cvLoadImage(filename, -1);

    found = cvFindChessboardCorners(show, board_size, image_points_buf, &count,
        CC_CALIB_CB_ADAPTIVE_THRESH | CC_CALIB_CB_FILTER_QUADS);

    if (found == 0) {
      cout << "��" << a << "֡ͼƬ�޷��ҵ����̸����нǵ�!\n\n";
      cvNamedWindow("RePlay", 1);
      cvShowImage("RePlay", show);
      cvWaitKey(0);

    }
    else {
      cout << "��" << a << "֡ͼ��ɹ����" << count << "���ǵ�...\n";

      cvNamedWindow("RePlay", 1);

      IplImage* gray_image = cvCreateImage(cvGetSize(show), 8, 1); //����ͷ����������IplImage* cvCreateImage( CvSize size, int depth, int channels ); depth ͼ��Ԫ�ص�λ���

      cvCvtColor(show, gray_image, CC_BGR2GRAY); // cvCvtColor(...),��Opencv�����ɫ�ռ�ת������������ʵ��rgb��ɫ��HSV,HSI����ɫ�ռ��ת����Ҳ����ת��Ϊ�Ҷ�ͼ��

      cout << "��ȡԴͼ��Ҷ�ͼ�������...\n";
cvFindCornerSubPix(gray_image, image_points_buf, count, cvSize(11, 11), cvSize(-1, -1), ���ڷǳ��ӽ�P�����ز����˺�С������ֵ�������������ؾ��󲢲����ǿ���ġ�Ϊ�˽��������⣬һ����Լ򵥵��޳���P��ǳ��������ء��������: ero_zone������һ������(��win���ƣ���ͨ����winС)����������ڷ������Լ�����ؾ����в������ǡ��������Ҫ����һ����������zero_zoneӦ����ΪcvSize(-1�� -1)0
          cvTermCriteria(CC_TERMCRIT_EPS + CC_TERMCRIT_ITER, 30, 0.1));
      cout << "�Ҷ�ͼ�����ػ��������...\n";
      cvDrawChessboardCorners(show, board_size, image_points_buf, count, found);
      cout << "��Դͼ���ϻ��ƽǵ�������...\n\n";
      cvShowImage("RePlay", show);
      cvWaitKey(0);
    }

    if (total_per_image == count) {
      step = successes * total_per_image;

      for (int i = step, j = 0; j < total_per_image; ++i, ++j) {
        CC_MAT_ELEM(*image_points, float, i, 0) = image_points_buf[j].x; // opencv���������ʾ���ÿ��Ԫ�صĺ꣬�����ֻ�Ե�ͨ��������Ч����ͨ��CC_MAT_ELEM( matrix, elemtype, row, col )��������matrix��Ҫ���ʵľ��󡡡�elemtype������Ԫ�ص����͡���row����Ҫ����Ԫ�ص���������col����Ҫ����Ԫ�ص�����

        CC_MAT_ELEM(*image_points, float, i, 1) = image_points_buf[j].y; // ����ÿ���ǵ��������ֵ������image_point_buf��
        CC_MAT_ELEM(*object_points, float, i, 0) = (float)(j / cube_length);
        CC_MAT_ELEM(*object_points, float, i, 1) = (float)(j % cube_length);
        CC_MAT_ELEM(*object_points, float, i, 2) = 0.0f;
      }

      CC_MAT_ELEM(*point_counts, int, successes, 0) = total_per_image;
      successes++;
    }

    a++;
  }

  cvReleaseImage(&show);
  cvDestroyWindow("RePlay");
  cout << "*********************************************\n";
  cout << number_image << "֡ͼƬ�У��궨�ɹ���ͼƬΪ" << successes << "֡...\n";
  cout << number_image << "֡ͼƬ�У��궨ʧ�ܵ�ͼƬΪ" << number_image - successes << "֡...\n\n";
  cout << "*********************************************\n\n";

  cout << "���������ʼ����������ڲ���...\n\n";


  CvCapture* capture1;
  capture1 = cvCreateCameraCapture(0);
  IplImage* show_colie;
  show_colie = cvQueryFrame(capture1);


  CvMat* object_points2 = cvCreateMat(successes * total_per_image, 3, CC_32FC1); // OpenCV ����Ҫ�ľ���任������ʹ�÷���ΪcvMat* cvCreateMat ( int rows, int cols, int type ); ����type�������κ�Ԥ�������ͣ�Ԥ�������͵Ľṹ���£�CC_<bit_depth> (S|U|F)C<number_of_channels>��

  CvMat* image_points2 = cvCreateMat(successes * total_per_image, 2, CC_32FC1);
  CvMat* point_counts2 = cvCreateMat(successes, 1, CC_32SC1);

  for (int i = 0; i < successes * total_per_image; ++i) {
    CC_MAT_ELEM(*image_points2, float, i, 0) = CC_MAT_ELEM(*image_points, float, i, 0); //�����洢�ǵ���ȡ�ɹ���ͼ��Ľǵ�
    CC_MAT_ELEM(*image_points2, float, i, 1) = CC_MAT_ELEM(*image_points, float, i, 1);
    CC_MAT_ELEM(*object_points2, float, i, 0) = CC_MAT_ELEM(*object_points, float, i, 0);
    CC_MAT_ELEM(*object_points2, float, i, 1) = CC_MAT_ELEM(*object_points, float, i, 1);
    CC_MAT_ELEM(*object_points2, float, i, 2) = CC_MAT_ELEM(*object_points, float, i, 2);
  }

  for (int i = 0; i < successes; ++i) {
    CC_MAT_ELEM(*point_counts2, int, i, 0) = CC_MAT_ELEM(*point_counts, int, i, 0);
  }


  cvReleaseMat(&object_points);
  cvReleaseMat(&image_points);
  cvReleaseMat(&point_counts);


  CC_MAT_ELEM(*intrinsic_matrix, float, 0, 0) = 1.0f;
  CC_MAT_ELEM(*intrinsic_matrix, float, 1, 1) = 1.0f;


  cvCalibrateCamera2(object_points2, image_points2, point_counts2, cvGetSize(show_colie),
      intrinsic_matrix, distortion_coeffs, NULL, NULL, 0);

  cout << "������ڲ�������Ϊ��\n";
  cout << CC_MAT_ELEM(*intrinsic_matrix, float, 0, 0) << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 0, 1)
      << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 0, 2)
      << "\n\n";
  cout << CC_MAT_ELEM(*intrinsic_matrix, float, 1, 0) << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 1, 1)
      << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 1, 2)
      << "\n\n";
  cout << CC_MAT_ELEM(*intrinsic_matrix, float, 2, 0) << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 2, 1)
      << "          " << CC_MAT_ELEM(*intrinsic_matrix, float, 2, 2)
      << "\n\n";

  cout << "����ϵ������Ϊ��\n";
  cout << CC_MAT_ELEM(*distortion_coeffs, float, 0, 0) << "    " << CC_MAT_ELEM(*distortion_coeffs, float, 1, 0)
      << "    " << CC_MAT_ELEM(*distortion_coeffs, float, 2, 0)
      << "    " << CC_MAT_ELEM(*distortion_coeffs, float, 3, 0)
      << "    " << CC_MAT_ELEM(*distortion_coeffs, float, 4, 0)
      << "\n\n";

  cvSave("Intrinsics.xml", intrinsic_matrix);
  cvSave("Distortion.xml", distortion_coeffs);

  cout << "��������󡢻���ϵ�������Ѿ��ֱ�洢����ΪIntrinsics.xml��Distortion.xml�ĵ���\n\n";

  CvMat* intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
  CvMat* distortion = (CvMat*)cvLoad("Distortion.xml");

  IplImage* mapx = cvCreateImage(cvGetSize(show_colie), IMG_DEPTH_32F, 1);
  IplImage* mapy = cvCreateImage(cvGetSize(show_colie), IMG_DEPTH_32F, 1);

  cvInitUndistortMap(intrinsic, distortion, mapx, mapy);

  cvNamedWindow("ԭʼͼ��", 1);
  cvNamedWindow("�ǻ���ͼ��", 1);

  cout << "����E�����˳���ʾ...\n\n";

  while (show_colie) {
    IplImage* clone = cvCloneImage(show_colie);
    cvShowImage("ԭʼͼ��", show_colie);
    cvRemap(clone, show_colie, mapx, mapy);
    cvReleaseImage(&clone);
    cvShowImage("�ǻ���ͼ��", show_colie);

    if (cvWaitKey(10) == 'e') {
      break;
    }

    show_colie = cvQueryFrame(capture1);
  }

  return 0;
}

//
//     ���궨����ʵ�ַ���
//       1 ������ƽ����ͼ��ƽ��֮���ӳ�����
//       ������ƽ����ͼ��ƽ��֮���ӳ����󣬼���ӳ�����ʱ������������ĳ���ģ�ͣ�ֻ�Ǹ���ƽ���������Ͷ�Ӧ��ͼ�����������ݣ�������С���˷�������õ�[ [ix] ]
//       .2 ����������������
//       �ɼ���õ��ı��ƽ���ͼ��ƽ���ӳ�����õ���������ڲ�������صĻ������̹�ϵ����ⷽ�̵õ�������ڲ����������Ǿ�ͷ�Ļ���ģ�ͣ��������ⷽ�̻�
//       �õ��ڲ�������Ϊ��ֵ�����з������Ż��������Ӷ���������в�����׼ȷֵ [[x] ]
//       .3 ��������������֮������λ�ù�ϵ
//       ��˫Ŀ�Ӿ�ϵͳ������������ⲿ�����ֱ�ΪRl, Tl,��Rr, Tr,����Rl, Tl��ʾ�����������������ϵ�����λ�ã�Rr, Tr��ʾ�����������������ϵ�����λ�� [[xi] ]����ˣ����ڿռ�����һ�㣬�������������ϵ�������������ϵ�������������ϵ�е�����ֱ�ΪXw,, Xl , Xr������:Xl=RlXw+Tl ;Xr=RrXw+Tr .��ˣ���̨�����֮�����Լ��ι�ϵ��������ʽ��ʾR=RrRl-1 ;T=Tr- RrRl-1Tl
//       ��ʵ�ʱ궨�����У��ɱ궨�ж���̨�����ͬʱ��������궨���Էֱ�����̨��������ڡ���������Ӷ��������Ա궨����������ڲ�������������ͬʱ�궨��˫Ŀ�Ӿ�ϵͳ�Ľṹ���� [xii] ���ɵ�������궨���̿���֪�����궨��ÿ�任һ��λ�þͿ��Եõ�һ������������:Rr,Tr,��Rl, Tl����ˣ��ɹ�ʽR=RrRl-1 ;T=Tr- RrRl-1Tl�����Եõ�һ��ṹ����R��T
