
#define DoubleMaxValue DBL_MAX
#define DoubleMinValue DBL_MIN
enum { byteMinValue = 0, byteMaxValue = 255,};

static double A(int* HistGram, int Index)
{
  double Sum = 0;
  for (int Y = 0; Y <= Index; Y++)
    Sum += HistGram[Y];
  return Sum;
}

static double B(int* HistGram, int Index)
{
  double Sum = 0;
  for (int Y = 0; Y <= Index; Y++)
    Sum += (double)Y * HistGram[Y];
  return Sum;
}

static double C(int* HistGram, int Index)
{
  double Sum = 0;
  for (int Y = 0; Y <= Index; Y++)
    Sum += (double)Y * Y * HistGram[Y];
  return Sum;
}

static double D(int* HistGram, int Index)
{
  double Sum = 0;
  for (int Y = 0; Y <= Index; Y++)
    Sum += (double)Y * Y * Y * HistGram[Y];
  return Sum;
}

/// <summary>
/// ���ڻҶ�ƽ��ֵ����ֵ
/// </summary>
/// <param name="HistGram">�Ҷ�ͼ���ֱ��ͼ</param>
/// <returns></returns>
static int GetMeanThreshold(int* HistGram)
{
  int Sum = 0, Amount = 0;
  for (int Y = 0; Y < 256; Y++)
  {
    Amount += HistGram[Y];
    Sum += Y * HistGram[Y];
  }
  return Sum / Amount;
}
/// <summary>
/// ����ģ�����Ļ�ʽ��ֵ�㷨
/// http://www.ktl.elf.stuba.sk/study/vacso/Zadania-Cvicenia/Cvicenie_3/TimA2/Huang_E016529624.pdf
/// </summary>
/// <param name="HistGram">�Ҷ�ͼ���ֱ��ͼ</param>
/// <returns></returns>

static int GetHuangFuzzyThreshold(const int* HistGram, int HistGram_Length)
{
  int x, y;
  int First, Last;
  int Threshold = -1;
  double BestEntropy = DoubleMaxValue, Entropy;
  // �ҵ���һ�������һ����0��ɫ��ֵ
  for (First = 0; First < HistGram_Length && HistGram[First] == 0; First++) ;
  for (Last = HistGram_Length - 1; Last > First && HistGram[Last] == 0; Last--) ;
  if (First == Last) {
    return First;  // ͼ����ֻ��һ����ɫ
  }
  if (First + 1 == Last) {
    return First;  // ͼ����ֻ�ж�����ɫ
  }
  // �����ۼ�ֱ��ͼ�Լ���Ӧ�Ĵ�Ȩ�ص��ۼ�ֱ��ͼ
  int* S = NULL;
  int* W = NULL;
  MYREALLOC(S, Last + 1);
  MYREALLOC(W, Last + 1);
  // �����ش�ͼ��������ı������ݿ��ܻᳬ��int�ı�ʾ��Χ�����Կ�����long����������
  S[0] = HistGram[0];
  for (y = First > 1 ? First : 1; y <= Last; y++) {
    S[y] = S[y - 1] + HistGram[y];
    W[y] = W[y - 1] + y * HistGram[y];
  }
  // ������ʽ��4������6�����õĲ��ұ�
  double* Smu = NULL;
  int Smu_Length = Last + 1 - First;
  MYREALLOC(Smu, Smu_Length);
  for (y = 1; y < Smu_Length; y++) {
    double mu = 1 / (1 + (double)y / (Last - First)); // ��ʽ��4��
    Smu[y] = -mu * log(mu) - (1 - mu) * log(1 - mu); // ��ʽ��6��
  }
  // �������������ֵ
  for (y = First; y <= Last; y++) {
    Entropy = 0;
    int mu = (int)ROUND((double)W[y] / S[y]); // ��ʽ17
    for (x = First; x <= y; x++) {
      Entropy += Smu[abs(x - mu)] * HistGram[x];
    }
    mu = (int)ROUND((double)(W[Last] - W[y]) / (S[Last] - S[y])); // ��ʽ18
    for (x = y + 1; x <= Last; x++) {
      Entropy += Smu[abs(x - mu)] * HistGram[x];  // ��ʽ8
    }
    if (BestEntropy > Entropy) {
      BestEntropy = Entropy; // ȡ��С�ش�Ϊ�����ֵ
      Threshold = y;
    }
  }
  FREE(Smu);
  FREE(W);
  FREE(S);
  return Threshold;
}

static bool IsDimodal(double* HistGram)       // ���ֱ��ͼ�Ƿ�Ϊ˫���
{
  // ��ֱ��ͼ�ķ���м�����ֻ�з���λ2��Ϊ˫�� 
  int Count = 0;
  for (int Y = 1; Y < 255; Y++)
  {
    if (HistGram[Y - 1] < HistGram[Y] && HistGram[Y + 1] < HistGram[Y])
    {
      Count++;
      if (Count > 2) return false;
    }
  }
  if (Count == 2)
    return true;
  else
    return false;
}


/// <summary>
/// ���ڹȵ���Сֵ����ֵ
/// �˷���ʵ���ھ�������˫��ֱ��ͼ��ͼ����Ѱ��˫��Ĺȵ���Ϊ��ֵ
/// References: 
/// J. M. S. Prewitt and M. L. Mendelsohn, "The analysis of cell images," in
/// nnals of the New York Academy of Sciences, vol. 128, pp. 1035-1053, 1966.
/// C. A. Glasbey, "An analysis of histogram-based thresholding algorithms,"
/// CVGIP: Graphical Models and Image Processing, vol. 55, pp. 532-537, 1993.
/// </summary>
/// <param name="HistGram">�Ҷ�ͼ���ֱ��ͼ</param>
/// <param name="HistGramS">����ƽ�����ֱ��ͼ</param>
/// <returns></returns>
static int GetMinimumThreshold(int* HistGram, int* HistGramS)
{
  int Y, Iter = 0;
  double* HistGramC = new double[256];           // ���ھ������⣬һ��Ҫ�ø���������������ò�����ȷ�Ľ��
  double* HistGramCC = new double[256];          // ���ֵ�Ĺ��̻��ƻ�ǰ������ݣ������Ҫ��������
  for (Y = 0; Y < 256; Y++)
  {
    HistGramC[Y] = HistGram[Y];
    HistGramCC[Y] = HistGram[Y];
  }
  
  // ͨ���������ֵ��ƽ��ֱ��ͼ
  while (IsDimodal(HistGramCC) == false)                                        // �ж��Ƿ��Ѿ���˫���ͼ����      
  {
    HistGramCC[0] = (HistGramC[0] + HistGramC[0] + HistGramC[1]) / 3;                 // ��һ��
    for (Y = 1; Y < 255; Y++)
      HistGramCC[Y] = (HistGramC[Y - 1] + HistGramC[Y] + HistGramC[Y + 1]) / 3;     // �м�ĵ�
    HistGramCC[255] = (HistGramC[254] + HistGramC[255] + HistGramC[255]) / 3;         // ���һ��
    memcpy(HistGramC, HistGramCC, 256 * sizeof(double));
    Iter++;
    if (Iter >= 1000) return -1;                                                   // ֱ��ͼ�޷�ƽ��Ϊ˫��ģ����ش������
  }
  for (Y = 0; Y < 256; Y++) HistGramS[Y] = (int)HistGramCC[Y];
  // ��ֵ��Ϊ����֮�����Сֵ 
  bool Peakfound = false;
  for (Y = 1; Y < 255; Y++)
  {
    if (HistGramCC[Y - 1] < HistGramCC[Y] && HistGramCC[Y + 1] < HistGramCC[Y]) Peakfound = true;
    if (Peakfound == true && HistGramCC[Y - 1] >= HistGramCC[Y] && HistGramCC[Y + 1] >= HistGramCC[Y])
      return Y - 1;
  }
  return -1;
}

/// <summary>
/// ����˫��ƽ��ֵ����ֵ
/// �˷���ʵ���ھ�������˫��ֱ��ͼ��ͼ����Ѱ��˫��Ĺȵ���Ϊ��ֵ
/// References: 
/// J. M. S. Prewitt and M. L. Mendelsohn, "The analysis of cell images," in
/// nnals of the New York Academy of Sciences, vol. 128, pp. 1035-1053, 1966.
/// C. A. Glasbey, "An analysis of histogram-based thresholding algorithms,"
/// CVGIP: Graphical Models and Image Processing, vol. 55, pp. 532-537, 1993.
/// </summary>
/// <param name="HistGram">�Ҷ�ͼ���ֱ��ͼ</param>
/// <param name="HistGramS">����ƽ�����ֱ��ͼ</param>
/// <returns></returns>

static int GetIntermodesThreshold(int* HistGram, int* HistGramS)
{
  int Y, Iter = 0, Index;
  double* HistGramC = new double[256];           // ���ھ������⣬һ��Ҫ�ø���������������ò�����ȷ�Ľ��
  double* HistGramCC = new double[256];          // ���ֵ�Ĺ��̻��ƻ�ǰ������ݣ������Ҫ��������
  for (Y = 0; Y < 256; Y++)
  {
    HistGramC[Y] = HistGram[Y];
    HistGramCC[Y] = HistGram[Y];
  }
  // ͨ���������ֵ��ƽ��ֱ��ͼ
  while (IsDimodal(HistGramCC) == false)                                                  // �ж��Ƿ��Ѿ���˫���ͼ����      
  {
    HistGramCC[0] = (HistGramC[0] + HistGramC[0] + HistGramC[1]) / 3;                   // ��һ��
    for (Y = 1; Y < 255; Y++)
      HistGramCC[Y] = (HistGramC[Y - 1] + HistGramC[Y] + HistGramC[Y + 1]) / 3;       // �м�ĵ�
    HistGramCC[255] = (HistGramC[254] + HistGramC[255] + HistGramC[255]) / 3;           // ���һ��
    memcpy(HistGramC, HistGramCC, 256 * sizeof(double));         // �������ݣ�Ϊ��һ�ε�����׼��
    Iter++;
    if (Iter >= 10000) return -1;                                                       // �ƺ�ֱ��ͼ�޷�ƽ��Ϊ˫��ģ����ش������
  }
  for (Y = 0; Y < 256; Y++) HistGramS[Y] = (int)HistGramCC[Y];
  // ��ֵΪ����ֵ��ƽ��ֵ
  int* Peak = new int[2];
  for (Y = 1, Index = 0; Y < 255; Y++)
    if (HistGramCC[Y - 1] < HistGramCC[Y] && HistGramCC[Y + 1] < HistGramCC[Y]) Peak[Index++] = Y - 1;
    return ((Peak[0] + Peak[1]) / 2);
}
/// <summary>
/// �ٷֱ���ֵ
/// </summary>
/// <param name="HistGram">�Ҷ�ͼ���ֱ��ͼ</param>
/// <param name="Tile">������ͼ������ռ������ٷֱ�</param>
/// <returns></returns>
static int GetPTileThreshold(int* HistGram, int Tile = 50)
{
  int Y, Amount = 0, Sum = 0;
  for (Y = 0; Y < 256; Y++) Amount += HistGram[Y];        //  ��������
  for (Y = 0; Y < 256; Y++)
  {
    Sum = Sum + HistGram[Y];
    if (Sum >= Amount * Tile / 100) return Y;
  }
  return -1;
}

/// <summary>
/// �����������ֵ
/// </summary>
/// <param name="HistGram">�Ҷ�ͼ���ֱ��ͼ</param>
/// <returns></returns>
static int GetIterativeBestThreshold(int* HistGram)
{
  int X, Iter = 0;
  int MeanValueOne, MeanValueTwo, SumOne, SumTwo, SumIntegralOne, SumIntegralTwo;
  int MinValue, MaxValue;
  int Threshold, NewThreshold;
  
  for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;
  for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ;
  
  if (MaxValue == MinValue) return MaxValue;          // ͼ����ֻ��һ����ɫ             
  if (MinValue + 1 == MaxValue) return MinValue;      // ͼ����ֻ�ж�����ɫ
  
  Threshold = MinValue;
  NewThreshold = (MaxValue + MinValue) >> 1;
  while (Threshold != NewThreshold)    // ��ǰ�����ε����Ļ����ֵ��ͬʱ����������    
  {
    SumOne = 0; SumIntegralOne = 0;
    SumTwo = 0; SumIntegralTwo = 0;
    Threshold = NewThreshold;
    for (X = MinValue; X <= Threshold; X++)         //������ֵ��ͼ��ָ��Ŀ��ͱ��������֣���������ֵ�ƽ���Ҷ�ֵ      
    {
      SumIntegralOne += HistGram[X] * X;
      SumOne += HistGram[X];
    }
    MeanValueOne = SumIntegralOne / SumOne;
    for (X = Threshold + 1; X <= MaxValue; X++)
    {
      SumIntegralTwo += HistGram[X] * X;
      SumTwo += HistGram[X];
    }
    MeanValueTwo = SumIntegralTwo / SumTwo;
    NewThreshold = (MeanValueOne + MeanValueTwo) >> 1;       //����µ���ֵ
    Iter++;
    if (Iter >= 1000) return -1;
  }
  return Threshold;
}


static int GetOSTUThreshold(int* HistGram)
{
  int X, Y, Amount = 0;
  int PixelBack = 0, PixelFore = 0, PixelIntegralBack = 0, PixelIntegralFore = 0, PixelIntegral = 0;
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma;              // ��䷽��;
  int MinValue, MaxValue;
  int Threshold = 0;
  
  for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;
  for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ;
  if (MaxValue == MinValue) return MaxValue;          // ͼ����ֻ��һ����ɫ             
  if (MinValue + 1 == MaxValue) return MinValue;      // ͼ����ֻ�ж�����ɫ
  
  for (Y = MinValue; Y <= MaxValue; Y++) Amount += HistGram[Y];        //  ��������
  
  PixelIntegral = 0;
  for (Y = MinValue; Y <= MaxValue; Y++) PixelIntegral += HistGram[Y] * Y;
  SigmaB = -1;
  for (Y = MinValue; Y < MaxValue; Y++)
  {
    PixelBack = PixelBack + HistGram[Y];
    PixelFore = Amount - PixelBack;
    OmegaBack = (double)PixelBack / Amount;
    OmegaFore = (double)PixelFore / Amount;
    PixelIntegralBack += HistGram[Y] * Y;
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;
    MicroBack = (double)PixelIntegralBack / PixelBack;
    MicroFore = (double)PixelIntegralFore / PixelFore;
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);
    if (Sigma > SigmaB)
    {
      SigmaB = Sigma;
      Threshold = Y;
    }
  }
  return Threshold;
}

static int Get1DMaxEntropyThreshold(int* HistGram)
{
  int X, Y, Amount = 0;
  double* HistGramD = new double[256];
  double SumIntegral, EntropyBack, EntropyFore, MaxEntropy;
  int MinValue = 255, MaxValue = 0;
  int Threshold = 0;
  
  for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;
  for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ;
  if (MaxValue == MinValue) return MaxValue;          // ͼ����ֻ��һ����ɫ             
  if (MinValue + 1 == MaxValue) return MinValue;      // ͼ����ֻ�ж�����ɫ
  
  for (Y = MinValue; Y <= MaxValue; Y++) Amount += HistGram[Y];        //  ��������
  
  for (Y = MinValue; Y <= MaxValue; Y++) HistGramD[Y] = (double)HistGram[Y] / Amount + 1e-17;
  
  MaxEntropy = DoubleMinValue; ;
  for (Y = MinValue + 1; Y < MaxValue; Y++)
  {
    SumIntegral = 0;
    for (X = MinValue; X <= Y; X++) SumIntegral += HistGramD[X];
    EntropyBack = 0;
    for (X = MinValue; X <= Y; X++) EntropyBack += (-HistGramD[X] / SumIntegral * log(HistGramD[X] / SumIntegral));
    EntropyFore = 0;
    for (X = Y + 1; X <= MaxValue; X++) EntropyFore += (-HistGramD[X] / (1 - SumIntegral) * log(HistGramD[X] / (1 - SumIntegral)));
    if (MaxEntropy < EntropyBack + EntropyFore)
    {
      Threshold = Y;
      MaxEntropy = EntropyBack + EntropyFore;
    }
  }
  return Threshold;
}

// http://fiji.sc/wiki/index.php/Auto_Threshold#Huang
//   W. Tsai, "Moment-preserving thresholding: a new approach," Computer
//   Vision, Graphics, and Image Processing, vol. 29, pp. 377-393, 1985.
//
//  C. A. Glasbey, "An analysis of histogram-based thresholding algorithms,"
//  CVGIP: Graphical Models and Image Processing, vol. 55, pp. 532-537, 1993.

static byte GetMomentPreservingThreshold(int* HistGram)
{
  int X, Y, Index = 0, Amount = 0;
  double* Avec = new double[256];
  double X2, X1, X0, Min;
  
  for (Y = 0; Y <= 255; Y++) Amount += HistGram[Y];        //  ��������
  for (Y = 0; Y < 256; Y++) Avec[Y] = (double)A(HistGram, Y) / Amount;       // The threshold is chosen such that A(y,t)/A(y,n) is closest to x0.
  
  // The following finds x0.
  
  X2 = (double)(B(HistGram, 255) * C(HistGram, 255) - A(HistGram, 255) * D(HistGram, 255)) / (double)(A(HistGram, 255) * C(HistGram, 255) - B(HistGram, 255) * B(HistGram, 255));
  X1 = (double)(B(HistGram, 255) * D(HistGram, 255) - C(HistGram, 255) * C(HistGram, 255)) / (double)(A(HistGram, 255) * C(HistGram, 255) - B(HistGram, 255) * B(HistGram, 255));
  X0 = 0.5 - (B(HistGram, 255) / A(HistGram, 255) + X2 / 2) / sqrt(X2 * X2 - 4 * X1);
  
  for (Y = 0, Min = DoubleMaxValue; Y < 256; Y++)
  {
    if (fabs(Avec[Y] - X0) < Min)
    {
      Min = fabs(Avec[Y] - X0);
      Index = Y;
    }
  }
  return (byte)Index;
}

static int GetKittlerMinError(int* HistGram)
{
  int X, Y;
  int MinValue, MaxValue;
  int Threshold ;
  int PixelBack, PixelFore;
  double OmegaBack, OmegaFore, MinSigma, Sigma, SigmaBack, SigmaFore;
  for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;
  for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ;
  if (MaxValue == MinValue) return MaxValue;          // ͼ����ֻ��һ����ɫ             
  if (MinValue + 1 == MaxValue) return MinValue;      // ͼ����ֻ�ж�����ɫ
  Threshold = -1;
  MinSigma = 1E+20;
  for (Y = MinValue; Y < MaxValue; Y++)
  {
    PixelBack = 0; PixelFore = 0;
    OmegaBack = 0; OmegaFore = 0;
    for (X = MinValue; X <= Y; X++)
    {
      PixelBack += HistGram[X];
      OmegaBack = OmegaBack + X * HistGram[X];
    }
    for (X = Y + 1; X <= MaxValue; X++)
    {
      PixelFore += HistGram[X];
      OmegaFore = OmegaFore + X * HistGram[X];
    }
    OmegaBack = OmegaBack / PixelBack;
    OmegaFore = OmegaFore / PixelFore;
    SigmaBack = 0; SigmaFore = 0;
    for (X = MinValue; X <= Y; X++) SigmaBack = SigmaBack + (X - OmegaBack) * (X - OmegaBack) * HistGram[X];
    for (X = Y + 1; X <= MaxValue; X++) SigmaFore = SigmaFore + (X - OmegaFore) * (X - OmegaFore) * HistGram[X];
    if (SigmaBack == 0 || SigmaFore == 0)
    {
      if (Threshold == -1)
        Threshold = Y;
    }
    else
    {
      SigmaBack = sqrt(SigmaBack / PixelBack);
      SigmaFore = sqrt(SigmaFore / PixelFore);
      Sigma = 1 + 2 * (PixelBack * log(SigmaBack / PixelBack) + PixelFore * log(SigmaFore / PixelFore));
      if (Sigma < MinSigma)
      {
        MinSigma = Sigma;
        Threshold = Y;
      }
    }
  }
  return Threshold;
}

// Also called intermeans
// Iterative procedure based on the isodata algorithm [T.W. Ridler, S. Calvard, Picture 
// thresholding using an iterative selection method, IEEE Trans. System, Man and 
// Cybernetics, SMC-8 (1978) 630-632.] 
// The procedure divides the image into objects and background by taking an initial threshold,
// then the averages of the pixels at or below the threshold and pixels above are computed. 
// The averages of those two values are computed, the threshold is incremented and the 
// process is repeated until the threshold is larger than the composite average. That is,
//  threshold = (average background + average objects)/2
// The code in ImageJ that implements this function is the getAutoThreshold() method in the ImageProcessor class. 
//
// From: Tim Morris (dtm@ap.co.umist.ac.uk)
// Subject: Re: Thresholding method?
// posted to sci.image.processing on 1996/06/24
// The algorithm implemented in NIH Image sets the threshold as that grey
// value, G, for which the average of the averages of the grey values
// below and above G is equal to G. It does this by initialising G to the
// lowest sensible value and iterating:

// L = the average grey value of pixels with intensities < G
// H = the average grey value of pixels with intensities > G
// is G = (L + H)/2?
// yes => exit
// no => increment G and repeat
//
// There is a discrepancy with IJ because they are slightly different methods

static int GetIsoDataThreshold(int* HistGram, int HistGram_Length)
{
  int i, l, toth, totl, h, g = 0;
  for (i = 1; i < HistGram_Length; i++)
  {
    if (HistGram[i] > 0)
    {
      g = i + 1;
      break;
    }
  }
  while (true)
  {
    l = 0;
    totl = 0;
    for (i = 0; i < g; i++)
    {
      totl = totl + HistGram[i];
      l = l + (HistGram[i] * i);
    }
    h = 0;
    toth = 0;
    for (i = g + 1; i < HistGram_Length; i++)
    {
      toth += HistGram[i];
      h += (HistGram[i] * i);
    }
    if (totl > 0 && toth > 0)
    {
      l /= totl;
      h /= toth;
      if (g == (int)ROUND((l + h) / 2.0))
        break;
    }
    g++;
    if (g > HistGram_Length - 2)
    {
      return 0;
    }
  }
  return g;
}



// Shanhbag A.G. (1994) "Utilization of Information Measure as a Means of
//  Image Thresholding" Graphical Models and Image Processing, 56(5): 414-419
// Ported to ImageJ plugin by G.Landini from E Celebi's fourier_0.8 routines
static int GetShanbhagThreshold(int* HistGram, int HistGram_Length)
{
  int threshold;
  int ih, it;
  int first_bin;
  int last_bin;
  double term;
  double tot_ent;  /* total entropy */
  double min_ent;  /* max entropy */
  double ent_back; /* entropy of the background pixels at a given threshold */
  double ent_obj;  /* entropy of the object pixels at a given threshold */
  double* norm_histo = NULL; /* normalized histogram */
  double* P1 = NULL; /* cumulative normalized histogram */
  double* P2 = NULL;
  MYREALLOC(norm_histo, HistGram_Length*3);
  P1 = norm_histo + HistGram_Length;
  P2 = norm_histo + HistGram_Length*2;

  int total = 0;
  for (ih = 0; ih < HistGram_Length; ih++)
    total += HistGram[ih];
  
  for (ih = 0; ih < HistGram_Length; ih++)
    norm_histo[ih] = (double)HistGram[ih] / total;
  
  P1[0] = norm_histo[0];
  P2[0] = 1.0 - P1[0];
  for (ih = 1; ih < HistGram_Length; ih++)
  {
    P1[ih] = P1[ih - 1] + norm_histo[ih];
    P2[ih] = 1.0 - P1[ih];
  }
  
  /* Determine the first non-zero bin */
  first_bin = 0;
  for (ih = 0; ih < HistGram_Length; ih++)
  {
    if (!(fabs(P1[ih]) < 2.220446049250313E-16))
    {
      first_bin = ih;
      break;
    }
  }
  
  /* Determine the last non-zero bin */
  last_bin = HistGram_Length - 1;
  for (ih = HistGram_Length - 1; ih >= first_bin; ih--)
  {
    if (!(fabs(P2[ih]) < 2.220446049250313E-16))
    {
      last_bin = ih;
      break;
    }
  }
  
  // Calculate the total entropy each gray-level
  // and find the threshold that maximizes it 
  threshold = -1;
  min_ent = DoubleMaxValue;
  
  for (it = first_bin; it <= last_bin; it++)
  {
    /* Entropy of the background pixels */
    ent_back = 0.0;
    term = 0.5 / P1[it];
    for (ih = 1; ih <= it; ih++)
    { //0+1?
      ent_back -= norm_histo[ih] * log(1.0 - term * P1[ih - 1]);
    }
    ent_back *= term;
    
    /* Entropy of the object pixels */
    ent_obj = 0.0;
    term = 0.5 / P2[it];
    for (ih = it + 1; ih < HistGram_Length; ih++)
    {
      ent_obj -= norm_histo[ih] * log(1.0 - term * P2[ih]);
    }
    ent_obj *= term;
    
    /* Total entropy */
    tot_ent = fabs(ent_back - ent_obj);
    
    if (tot_ent < min_ent)
    {
      min_ent = tot_ent;
      threshold = it;
    }
  }
  FREE(norm_histo);
  return threshold;
}

// M. Emre Celebi
// 06.15.2007
// Ported to ImageJ plugin by G.Landini from E Celebi's fourier_0.8 routines
static int GetYenThreshold(int* HistGram, int HistGram_Length)
{
  int threshold;
  int ih, it;
  double crit;
  double max_crit;
  double* norm_histo = NULL; /* normalized histogram */
  double* P1 = NULL; /* cumulative normalized histogram */
  double* P1_sq = NULL;
  double* P2_sq = NULL;

  MYREALLOC(norm_histo, HistGram_Length*4);
  P1 = norm_histo + HistGram_Length;
  P1_sq = norm_histo + HistGram_Length*2;
  P2_sq = norm_histo + HistGram_Length*3;
  
  int total = 0;
  for (ih = 0; ih < HistGram_Length; ih++)
    total += HistGram[ih];
  
  for (ih = 0; ih < HistGram_Length; ih++)
    norm_histo[ih] = (double)HistGram[ih] / total;
  
  P1[0] = norm_histo[0];
  for (ih = 1; ih < HistGram_Length; ih++)
    P1[ih] = P1[ih - 1] + norm_histo[ih];
  
  P1_sq[0] = norm_histo[0] * norm_histo[0];
  for (ih = 1; ih < HistGram_Length; ih++)
    P1_sq[ih] = P1_sq[ih - 1] + norm_histo[ih] * norm_histo[ih];
  
  P2_sq[HistGram_Length - 1] = 0.0;
  for (ih = HistGram_Length - 2; ih >= 0; ih--)
    P2_sq[ih] = P2_sq[ih + 1] + norm_histo[ih + 1] * norm_histo[ih + 1];
  
  /* Find the threshold that maximizes the criterion */
  threshold = -1;
  max_crit = DoubleMinValue;
  for (it = 0; it < HistGram_Length; it++)
  {
    crit = -1.0 * ((P1_sq[it] * P2_sq[it]) > 0.0 ? log(P1_sq[it] * P2_sq[it]) : 0.0) + 2 * ((P1[it] * (1.0 - P1[it])) > 0.0 ? log(P1[it] * (1.0 - P1[it])) : 0.0);
    if (crit > max_crit)
    {
      max_crit = crit;
      threshold = it;
    }
  }
  FREE(norm_histo);
  return threshold;
}



