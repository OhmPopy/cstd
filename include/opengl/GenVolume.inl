//��������������
//********************************************************************//
//x,y,z:���������½�����
//side:������߳�
//density:�������Ӧ�ı���ֵ
//Data:�������
//Dim:�����ݴ�С
//********************************************************************//
void GenCube(int x, int y, int z, int side, int density, byte* Data, int* Dim)
{
  int max_x = x + side, max_y = y + side, max_z = z + side;
  int Dimxy = Dim[0] * Dim[1];
  for (int k = z; k < max_z; k++) {
    for (int j = y; j < max_y; j++) {
      for (int i = x; i < max_x; i++) {
        Data[k * Dimxy + j * Dim[0] + i] = density;
      }
    }
  }
}
//������������
//********************************************************************//
//x,y,z:��������
//radius:��뾶
//density:�����Ӧ�ı���ֵ
//Data:�������
//Dim:�����ݴ�С
//********************************************************************//
void GenSphere(int x, int y, int z, int radius, int density, byte* Data, int* Dim)
{
  int radius2 = radius * radius;
  int Dimxy = Dim[0] * Dim[1];
  for (int k = 0; k < Dim[2]; k++) {
    for (int j = 0; j < Dim[1]; j++) {
      for (int i = 0; i < Dim[0]; i++) {
        if ((i - x) * (i - x) + (j - y) * (j - y) + (k - z) * (k - z) <= radius2) {
          Data[k * Dimxy + j * Dim[0] + i] = density;
        }
      }
    }
  }
}
//����������
//********************************************************************//
//����һ���������壬�ڲ�����һ�����壬�����м��ְ���һ��С������
//Data:�������
//Dim:�����ݴ�С
//********************************************************************//
void GenVolume(byte* Data, int* Dim)
{
  GenCube(0, 0, 0, 200, 100, Data, Dim); //��������
  GenSphere(100, 100, 100, 80, 200, Data, Dim); //����
  GenCube(70, 70, 70, 60, 300, Data, Dim); //С������
}

