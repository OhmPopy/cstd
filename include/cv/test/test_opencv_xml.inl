
#include "parser/xml.inl"

CC_IMPL void
cvComment(vdstr_t* v, const char* comment) {

}

int test_opencv_xml() 
{
  char buf[1096] = {0};
  if (1) {
    // �����ļ��洢���� 
    CFileStorage fs[1]={0};
    //pf = fopen("test.xml", "wb");  fclose(pf);
    memstream_init( fs->file, buf, countof(buf));
    fstream_init( fs->file, stdout );
    cvFileStorageBegin(fs, 0);
    // дע��
    cvWriteComment(fs,"����дXML�ļ�",1);
    // ��ʼд�ṹ��������ͼmap��Ҳ���������ֵ�����ڵ㼯�� 
    cvWriteStructBegin(fs,"Employee");
    // ע��
    cvWriteComment(fs,"MAP���ͣ����������䣬нˮ", 1);
    // ����
    cvWriteString(fs,"name","��Խ");
    // ����
    cvWriteInt(fs,"age",18);
    // нˮ
    cvWriteReal(fs,"salary",2780.3);
    // ���۴���
    cvWriteInt(fs,"sales_count",4);
    
    {
      // ���۾������� 
      int sales_record[]={30000,4200,50090};
      // ע�� 
      cvWriteComment(fs,"SEQ���ͣ����ۼ�¼",1);
      // ��ʼд�ṹ������������sequence�������ֵ�����ڵ㼯�� 
      cvWriteStructBegin(fs,"sales_record");
      // ǰ3�����ۼ�¼ 
      cvWriteRawData(fs,sales_record,3,"i");
      // ��4�����ۼ�¼��ע�������� 
      cvWriteInt(fs,0,6100);
      // ���� 
      cvWriteStructEnd(fs);
    }
    {
      // ע�� 
      cvWriteComment(fs,"MAP���ͣ�����",1);
      // ��ʼ 
      cvWriteStructBegin(fs,"Parent");
      // ���� 
      cvWriteString(fs,"father","��˴");
      // ĸ�� 
      cvWriteString(fs,"mother","����");
      // ���� 
      cvWriteStructEnd(fs);
    }
    if (1) {
      img_t mat[1]={0};
      cvSetMat(mat, 3,3,CC_32F);//����һ������  
      cvSetIdentity(mat);  
      cvWriteMat(fs,"A",mat, NULL);//д��һ��������CvMat  
      imfree(mat);//�ͷž���ռ�  
    }
    // ���� 
    cvWriteStructEnd(fs);
    // �ͷ��ļ��洢���� 
    cvFileStorageEnd(fs);
  }
  if (1) {
    int indexFaces[] = {0, 1, 2};
    char* namePerson[] = {"aaa", "bbb", "ccc"};
    char* pathFaces[] = {"C:\\Pictures\\kobe", "C:\\Pictures\\Li", "C:\Pictures\Sun"};
    // �����ļ��洢����
    CFileStorage fs[1]={0};
    //pf = fopen("test.xml", "wb");  fclose(pf);
    memstream_init( fs->file, buf, countof(buf));
    fstream_init( fs->file, stdout );
    cvFileStorageBegin(fs, 0);
    cvWriteInt( fs, "nFaces", countof(indexFaces) );  
    cvWriteStructBegin( fs, "CVFaceRecog" );
    int i;
    for (i=0;i<countof(indexFaces);i++) {
      char person[100];  
      snprintf( person, countof(person), "person_%d", (i+1) );//�������ֿ����������ʱ��������  
      cvWriteStructBegin(fs, person);
      cvWriteInt( fs, "index", indexFaces[i] );
      cvWriteString( fs, "name", namePerson[i]);
      cvWriteString( fs, "directory", pathFaces[i]);
      cvWriteStructEnd(fs);
    }
    cvWriteStructEnd(fs);
    cvFileStorageEnd(fs);
  }

  if (1) {
    int i;
    char* namePerson[3] = {0};
    char* pathFaces[3] = {0};
    int indexFaces[3] = {0};
    vdstr_t dv[1] = {0};
#if 0
    CvFileNode* root = cvGetRootFileNode( fileStorage, 0);  
    CvFileNode* data = cvGetFileNodeByName( fileStorage, root, "CVFaceRecog" );  
    seq = data->data.seq;  
    cvStartReadSeq( seq, &reader, 0 );  
    int nFaces = cvReadIntByName( fileStorage, 0, "nFaces", 0 );  
    for(i = 0; i < nFaces; i++)  
    {
      CFileNode *pt = (CvFileNode*)reader.ptr;  
      namePerson.push_back(cvReadStringByName(fileStorage, pt, "name", 0));  
      pathFaces.push_back(cvReadStringByName(fileStorage, pt, "directory", 0));  
      indexFaces.push_back(cvReadIntByName(fileStorage,pt,"index",0));  
      CV_NEXT_SEQ_ELEM(seq->elem_size, reader);  
    }  
    cvReleaseFileStorage( &fileStorage );  
#endif
  }
  if (1) {
    // �ļ��ڵ�
    CFileNode * node, *node2;
    char * str;
    int count;
    int * d;
    
    //cve_dm.debug_break();
    // ��XML�ļ�
    buf_t fs[1] = {0};
    buf_load("test.xml", fs);
#if 0
    // ��õ�һ�����ݽڵ�
    node = cvGetFileNodeByName(fs,0,"Employee");
    str = cvReadStringByName(fs,node,"name");
    printf("\n����=%s",str);
    printf("\n����=%d",cvReadIntByName(fs,node,"age"));
    printf("\nнˮ=%g",cvReadRealByName(fs,node,"salary"));
    count = cvReadIntByName(fs,node,"sales_count");
    printf("\n����%d��",count);
    d = cvAlloc(sizeof(int)*count);
    if(d) 
    { 
      int i;
      node2 = cvGetFileNodeByName(fs,node,"sales_record");
      if(node2) 
      { 
        cvReadRawData(fs,node2,d,"i");
        printf("\n���ۼ�¼=");
        for(i=0;i<count;i++) 
          printf("%d, ",d[i]);
      } 
      cvFree(&d);
    } 
    // ��õڶ���ڵ� 
    node = cvGetFileNodeByName(fs,node,"Parent");
    printf("\n���ڵ�=%s",cvGetFileNodeName(node));
    str = cvReadStringByName(fs,node,"father");
    printf("\n����=%s",str);
    str = cvReadStringByName(fs,node,"mother");
    printf("\nĸ��=%s",str);
#endif
    bffree(fs);
  }
  return 0;
}
