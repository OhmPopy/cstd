// =========================================================================
//
//             ����ƽ�������ķ�ʽ��������AGC��ʵ��
//
// =========================================================================
//
//��(C) 2003-2010 ���ݺ���ͨ�ż��Źɷ����޹�˾
//   ��  �ߣ�   �� �� ��
//   ��  ����    V 1.0
//   �������ڣ� 2008��3��1��
//   ������ڣ� 2008��3��1��
//--------------------------------------------------------------------------
//  ����˵����
//     1. ����func_dBToLevel��func_LevelTodB����ʵ��dB��level֮���ת��
//     2. AGC�Ŀ��Ʒ�Χ[ -54,54]dB, ��108dB, �������������¸�0.1dB�ķ�Χ��
//
//--------------------------------------------------------------------------

#include "cstd.h"
#include "math/rand.inl"
#include "img/loadmat.inl"
#include "func_dBToLevel.inl"
#include "func_LevelTodB.inl"
int AGC_MeanPwr_Test_25K_ToTw() {
  double *Mag_vec = NULL;
  sys_chdir("E:/code/cstd/AGC");
  
  //rand('state',0);
  //randn('state',0);
  rng_t rng[1];
  rng_mt19937_init(rng, 10);
  //-------------------------------------------------------------------------
  //  AGC�������
  int AGC_SamplNum = 128;                     // ÿ128�������һ��
  //   �����������
  int fs = 25000;                             // ������
  int fo_list[] = {1111}; // �ź�Ƶ��
  int HoldTime = AGC_SamplNum*100;            // ����ʱ�䣬
  // // Mag_list = [10, 3, 8, 20, 5] ;      // ��������
  // Mag_list = [20,4,10,3,3];
  //Mag_list = randint(1,10,300) + 1;
  int Mag_list[] = {286,70,183,146,268,229,137,6,247,134};
  int TotalLen = countof(Mag_list)*HoldTime;
  int i, j;
  int Mag_vec_len=HoldTime*countof(Mag_list);
  //Mag_vec = reshape(repmat(Mag_list, HoldTime,1),1,[]);    // ���źű����
  MYREALLOC(Mag_vec, Mag_vec_len);
  for (i=0; i<Mag_vec_len; ++i) {
    Mag_vec[i] = Mag_list[i/HoldTime];
  }
  // ���������Ĳ���
  double Narrow_SNR_dB = 100;                  // խ������
  double SNR_dB = 100;                        // �������
  int noise_len = 0;
  double BandWidth = 3000;                    // ����
  double r = 0.9;
  
  int ii=0;
  //*************************************************************************
  // ������ʼ�ź�
  img_t im[20] = {0};
  int fo_list_len = countof(fo_list);
  img_t* sig_src_list = im+ii++;
  img_t* sig_src = im+ii++;
  img_t* sig_chan = im+ii++;
  img_t* tmp = im+ii++;
  img_t* AGC_out_Tmp = im+ii++;
  img_t* AGC_out = im+ii++;
  img_t* AvgMag_tmp_save = im+ii++;
  img_t* Record3 = im+ii++;
  img_t* Record4 = im+ii++;
  img_t* AvgMag_save = im+ii++;
  img_t* agc_delta_save = im+ii++;
  img_t* agc_dB_save = im+ii++;
  img_t* agc_level_save = im+ii++;

  //sig_src_list = zeros(length(fo_list),TotalLen);
  imsetsize_f8(sig_src_list, fo_list_len, TotalLen, 1);
  //for ii = 1:length(fo_list )
  //    sig_src_list(ii,:) = sin(2*pi*fo_list(ii)*(0:TotalLen - 1)/fs + rand(1,1)*2*pi);
  //end
  const double pi = M_PI;
  ii = 0;
  for (ii=0; ii<fo_list_len; ++ii) {
    for (i=0; i<TotalLen; ++i) {
      double t = rng_real3(rng);
      sig_src_list->tt.f8[ii*TotalLen] = sin(2*pi*fo_list[ii]*i/fs + t*2*pi);
    }
  }
  if (1) {
    loadmat_fromfile("sig_src_list.dat", sizeof(double), sig_src_list);
  }
  
  //sig_src = sum(sig_src_list,1)/length( fo_list);
  immean_f8(sig_src_list, OptCol, sig_src);
  // sig_src = 1 + r*cos(2*pi*1000*(0:TotalLen - 1)/16000);
  // sig_src = floor(sig_src.*2000*2.^8);
  // plot( sig_src)
  //sig_src = 420 * cos(2*pi*1000*(1:5000)/fs);//Rf-60dBm
  //sig_chan = [zeros(1,noise_len ),sig_src];
  imsetsize_f8(sig_src, 1, 5000, 1);
  imsetsize_f8(sig_chan, 1, 5000+noise_len, 1);
  for (i=0; i<sig_src->w; ++i) {
    sig_chan->tt.f8[i+noise_len]=sig_src->tt.f8[i] = 420 * cos(2*pi*1000*(i+1)/fs);
  }
  for (i=0; i<noise_len; ++i) {
    sig_chan->tt.f8[i] = 0;
  }
  // ��ʼ�źŽ������� ͨ���ŵ���������,AWGN�ŵ����������ϣ��±�Ƶ�˶�������ģ��ŵ��ģ��ǣÿ�����ȫ����
  // imp = sum(abs( sig_src).^2)/length(sig_src );
  // h_narrow = fir1(1024, 2*BandWidth/fs ,'low',chebwin(1025,30));
  // narrow_noise = conv(h_narrow , wgn(1,length( sig_tx) - length(h_narrow )+1, -10*log10(h_narrow*h_narrow')- Narrow_SNR_dB,imp,'complex'));  // �����˲��������������Ϊ֮ǰ��m��������mΪ�˲�������
  // sig_chan = sig_tx + floor(narrow_noise +  wgn(1,length( sig_tx), -SNR_dB,imp ,'complex'));    // ��������� խ������
  // sig_chan = sig_tx + wgn(1,length( sig_tx), -SNR_dB,imp ,'complex');   // ���п������
  // sig_chan = [sig_chan(1:31000),111*ones(1,1000),round(0.7*sig_chan(1:30000)),77*ones(1,2000),round(0.25*sig_chan(1:16000))];
  //
  //sig_chan = [1*sig_chan,zeros(1,1000),1*sig_chan,zeros(1,1000),1*sig_chan,zeros(1,1000),1*sig_chan,zeros(1,1000)];
  {
    int old_w = sig_chan->w;
    int old_w_1000 = old_w+1000;
    imclone2(sig_chan, tmp);
    imsetsize_f8(sig_chan, 1, old_w_1000*4, 1);
    for (i=0; i<old_w; ++i) {
      double t = tmp->tt.f8[i];
      sig_chan->tt.f8[old_w_1000*0 + i] = t;
      sig_chan->tt.f8[old_w_1000*1 + i] = t;
      sig_chan->tt.f8[old_w_1000*2 + i] = t;
      sig_chan->tt.f8[old_w_1000*3 + i] = t;
    }
    for (i=old_w; i<old_w_1000; ++i) {
      sig_chan->tt.f8[old_w_1000*0 + i] = 0.;
      sig_chan->tt.f8[old_w_1000*1 + i] = 0.;
      sig_chan->tt.f8[old_w_1000*2 + i] = 0.;
      sig_chan->tt.f8[old_w_1000*3 + i] = 0.;
    }
  }
  //***************AGC ����**********************************************************
  //�����У��ǣÿ���
  // ���Ƚ���ƽ���������㣬 ÿ128�������һ�Σ����ٿ��Զ�200Hz���ڵĵ����źŲ�ȡ��һ������������
  // sig_chan = sig_tx;    // �������������
  //AGC_TotalNum = ceil( length( sig_chan)/AGC_SamplNum);
  int AGC_TotalNum = ceil( sig_chan->w/AGC_SamplNum);
  //sig_chan = [sig_chan,zeros(1,AGC_TotalNum*AGC_SamplNum - length(sig_chan ))];
  {
    int old_w = sig_chan->w;
    int len = AGC_TotalNum*AGC_SamplNum;
    imclone2(sig_chan, tmp);
    imsetsize_f8(sig_chan, 1, len, 1);
    for (i=0; i<old_w; ++i) {
      sig_chan->tt.f8[i] = tmp->tt.f8[i];
    }
  }
#define round(x)  (int)(x+0.5)
  //figure;plot(real(sig_chan));
  // AGC���Ʋ��ֵĳ�ʼ��
  double agc_dB = 0;
  double DB_Threoshold = 64*64;                  // �ȶ����������ֵ
  //sig_chan = real(round(sig_chan));              // ֻ����ʵ�ź�
  for (i=0; i<sig_chan->w; ++i) {
    sig_chan->tt.f8[i] = round(sig_chan->tt.f8[i]);
  }
  // figure;plot(sig_chan);
  double x_d1 = 0, x_d2 = 0, y_d1 = 0, y_d2 = 0;    // iir�˲���ز����ĳ�ʼ��
  //------butter(2,0.1)-------
  int iir_b[] = { 329,   658,   329 };
  int iir_a[] = {16384, -25576, 10507};  // 2.^14�Ŵ�
  
  //RecordSlotPower = zeros(1,8);
  double RecordSlotPower[8] = {0};
  int g_s08_0dBSnr = -113;                                        // �����0dBʱ����Ƶ��ǿ
  int g_s16_RxRfAgc = 0;                                          // ��Ƶ�ϱ���ǿdB
  int BaseNoisePwr = 30;                                          // ��̨����ֵ
  int RfThreshold = -100;                                         // �������ޣ���ƽ����
  int PwrBackLevdB = 5;                                           // ���ʻ���
  int VadState = 0;                                               // �������أ�0��ʾ���������1��ʾ�������

  imsetsize_f8(AGC_out_Tmp, 1, AGC_SamplNum, 1);
  imsetsize_f8(AGC_out, 1, AGC_SamplNum*AGC_TotalNum, 1);
  imsetsize_f8(AvgMag_tmp_save, 1, AGC_TotalNum, 1);
  imsetsize_f8(Record3, 1, AGC_TotalNum, 1);
  imsetsize_f8(Record4, 1, AGC_TotalNum, 1);
  imsetsize_f8(AvgMag_save, 1, AGC_TotalNum, 1);
  imsetsize_f8(agc_delta_save, 1, AGC_TotalNum, 1);
  imsetsize_f8(agc_dB_save, 1, AGC_TotalNum, 1);
  imsetsize_f8(agc_level_save, 1, AGC_TotalNum, 1);
for (ii = 0; ii<AGC_TotalNum; ++ii) {
  //****************************************
  double Agc_Level = 0;
  double agc_dB_abs = 0;
  //func_dBToLevel(abs(agc_dB));     // ���ú���func_dBToLevel����agc_dB��Ӧ�ĵķŴ���
  agc_dB_abs = fabs(agc_dB);
  func_dBToLevel(&agc_dB, 1, &Agc_Level);
  
  // ����agc_dB�ķ��Ŷ������źŽ��зŴ����˥��
  for (i=0; i<AGC_SamplNum;++i) {
    const double* xx = sig_chan->tt.f8 + ii*AGC_SamplNum;
    if (agc_dB > 0) {      //  agc_dBΪ��ֵ��ʾ���źŽ��зŴ�
      //AGC_out_Tmp->tt.f8[i] = floor( Agc_Level* sig_chan((ii -1)*AGC_SamplNum + 1 : ii*AGC_SamplNum)/2.^6);
      AGC_out_Tmp->tt.f8[i] = floor( pow(Agc_Level * xx[i]/2., 6));
    } else {               //  agc_dBΪ��ֵ��ʾ���źŽ���˥��
      //AGC_out_Tmp->tt.f8[i] = floor( 2.^6* sig_chan((ii -1)*AGC_SamplNum + 1 : ii*AGC_SamplNum)/Agc_Level);
      AGC_out_Tmp->tt.f8[i] = floor( pow(2., 6) * xx[i]/Agc_Level);
    }
  }

  //AGC_out_Tmp = sign(AGC_out_Tmp).*min(abs(AGC_out_Tmp),32767);     // ��������16bit�洢���ض�
    for (i=0; i<AGC_out_Tmp->h*AGC_out_Tmp->w;++i) {
      double t = AGC_out_Tmp->tt.f8[i];
      AGC_out_Tmp->tt.f8[i] = CV_SIGN(t) * MIN(fabs(t),32767);
    }
  
    // AGC����ź�AGC_out
    //AGC_out((ii -1)*AGC_SamplNum + 1 : ii*AGC_SamplNum) = AGC_out_Tmp;
    for (j=0; j<AGC_out_Tmp->h; ++j) {
      for (i=0; i<AGC_out_Tmp->w; ++i) {
        AGC_out->tt.f8[j*AGC_out->w + ii*AGC_SamplNum + i] = AGC_out_Tmp->tt.f8[j*AGC_out_Tmp->w + i];
      }
    }
    //*** iir �˲�����*************************
    // ���Ƚ���ƽ�����ȵļ��㣬
    //AvgMag_tmp = floor(min(sqrt(sum(abs(AGC_out_Tmp ).^2)/AGC_SamplNum) ,32767));  // ����ƽ�������������õ�ƽ������
    double s=0;
    for (j=0; j<AGC_out_Tmp->h; ++j) {
      for (i=0; i<AGC_out_Tmp->w; ++i) {
        double t = AGC_out_Tmp->tt.f8[i];
         s += t*t;
      }
    }
    double AvgMag_tmp = sqrt(s/AGC_SamplNum);
    AvgMag_tmp = floor(MIN(AvgMag_tmp, 32767));
    //AvgMag_tmp_save(ii) = AvgMag_tmp;
    AvgMag_tmp_save->tt.f8[ii] = AvgMag_tmp;
    
    ////////////////////////////%20150611 by chenliang%////////////////////////////////////////////
    //RecordSlotPower = [AvgMag_tmp,RecordSlotPower(1:end-1)];              // ��¼������ź�����FIFO
    MEMMOVE(RecordSlotPower+1, RecordSlotPower, countof(RecordSlotPower)-1);
    RecordSlotPower[0] = AvgMag_tmp;
    //[MaxFramePower,MaxPos] = max(RecordSlotPower);                          // �����������,��ֹ����
    double MaxFramePower = RecordSlotPower[0];
    int MaxPos = 0;
    for (i=1; i<countof(RecordSlotPower); ++i) {
      if (RecordSlotPower[i]>MaxFramePower) {
        MaxFramePower = RecordSlotPower[i];
        MaxPos = i;
      }
    }

    double NoisePwr=0, SigPwr=0, RfPwr=0;
    // ���㳡ǿ,����MaxFramePower��AvgFramePower
    if(AvgMag_tmp>BaseNoisePwr*2) {
      NoisePwr = log10(BaseNoisePwr);
      SigPwr = log10(MaxFramePower-BaseNoisePwr);//�������
      RfPwr = (SigPwr - NoisePwr)*10 + g_s08_0dBSnr + g_s16_RxRfAgc;
      if(RfPwr<-121) {
        RfPwr = -121;
      }
    } else {
      RfPwr = -121;
    }
    // �������״̬
    double VadOnThd = RfThreshold;
    double VadOffThd = RfThreshold-PwrBackLevdB;
    if(VadState==1) {
      if(RfPwr<VadOffThd) {
        VadState = 0;//���Ϊ0
      }
    } else {
      if(RfPwr>VadOnThd) {
        VadState = 1;//�������
      }
    }

    Record3->tt.f8[ii] = RfPwr;
    Record4->tt.f8[ii] = MaxFramePower;
    // %20150611 by chenliang%
    
    // ͨ��IIR�˲��� �˳���������
    //AvgMag = round((AvgMag_tmp*iir_b(1) + x_d1*iir_b(2) + x_d2*iir_b(3) - y_d1*iir_a(2) - y_d2*iir_a(3))/2.^14);
    double t = (AvgMag_tmp*iir_b[0] + x_d1*iir_b[1] + x_d2*iir_b[2] - y_d1*iir_a[1] - y_d2*iir_a[2])/2;
    double AvgMag = round(pow(t, 14));
    x_d2 = x_d1; x_d1 = AvgMag_tmp;    // ������ӳ�������
    y_d2 = y_d1; y_d1 = AvgMag;        // ������ӳ�������
    AvgMag_save->tt.f8[ii] = AvgMag;  // ���Ա��������ñ���
    //------------------------------
    if (AvgMag < 0) {
        AvgMag = 100;                    // ���IIR�˲������ֵΪ������0��ǿ�Ƹ�ֵΪ100; ��ֹ���̫��
    }
    AvgMag =  MIN(fabs(AvgMag) ,32767);     // ��֤���Ϊ16bit����
    //****************************
    // ��ƽ������ת��ΪdBֵ
    AvgMag = MAX(AvgMag,1);                // ��ֹ���֣������
    double AvgMagDB=0;
    func_LevelTodB(&AvgMag, 1, &AvgMagDB);    // ���ú���func_LevelTodB����AvgMag�Ķ�������
    // ����AGC_Level�ĵ���
    double agc_delta = AvgMagDB - DB_Threoshold;       // ����ƽ������dBֵ���趨���޵Ĳ�ֵdBֵ
    //    agc_delta  = sign(agc_delta)*min(10*64, abs(agc_delta)); // ÿ�����ĵ������ܳ���10dB;
    //*********** �������ֵ�����ۼӣ����µ�������****** ���޷�Χ����0.1dB**********************
    if (agc_delta  > floor(0.1*64)) {
      agc_dB = agc_dB - ceil(agc_delta/4.);     // �ݶ���糣��Ϊ 4 ,ʵ��ʱ�ʵ���������������ȡ��ceil,��֤ÿ�����ٵ���1/64dB
    } else {
      if ( agc_delta < -floor(0.1*64)) {
        agc_dB =  agc_dB - floor( agc_delta/16);   // �ݶ��ŵ�糣��Ϊ 16, ʵ��ʱ�ʵ���������������ȡ��floor,��֤ÿ�����ٵ���1/64dB
      } else {
        agc_dB = agc_dB;
      }
    }
    //  ��������dB�� 20*log10(32767/64) = 54dB;�����ܵĿ��Ʒ�ΧΪ����54dB,��108dB������ֵΪ 3456 = 54*64
    agc_dB = CV_SIGN(agc_dB)* MIN(fabs(agc_dB), 3456);
    //****************************************
    //�����ǲ��Ա�����ʵ���в��ñ���
    agc_delta_save->tt.f8[ii] = agc_delta;
    agc_dB_save->tt.f8[ii] = agc_dB;
    agc_level_save->tt.f8[ii] = Agc_Level;              //�������õı���������ʵ��
}

#if 0
//****************************************
// fvtool( iir_b,iir_a );    // ����IIR�˲�����Ӧ
// ������ֱȽ�

figure;plot(Record3,'r');hold on;plot(real(0.2*sig_chan(1:128:end)),'g');
figure;plot(Record3,'g');
figure;plot(Record4,'r');
figure;plot(AGC_out,'b');
figure;
subplot(2,1,1); plot(sig_tx );grid on;title('���͵��ź�')
subplot(2,1,2); plot(real(sig_chan) );grid on;title('�������ź�')
figure;
subplot(2,1,1); plot(real(sig_chan),'r');grid on; hold on;plot(AGC_out);legend('�ŵ��ź�','AGC����ź�')
subplot(2,1,2); plot(AGC_out);grid on;title('AGC����ź�)')
figure;
subplot(2,1,1); plot(agc_dB_save/64);grid on;title('AGC������dBֵ')
subplot(2,1,2); plot(agc_level_save/64 );grid on;title('AGC�����ĵ�ƽֵ')
figure;
subplot(2,1,1);plot(20*log10(AvgMag_tmp_save));grid on;title('��ʼ�����ƽ������ (dB)')
subplot(2,1,2);plot(20*log10(max(abs(AvgMag_save),1)));grid on;title('����IIR�˲����ƽ������ (dB)')
figure;
plot(agc_delta_save);grid on; title( 'ƽ�����������޵Ĳ���dBֵ(Agc-delta���Ŵ���64��)')
//-----------------------
Mag_list
IIR_Neg_pos = find(AvgMag_save<0)
IIR_Neg_val = AvgMag_save(AvgMag_save<0)

#endif
  imfrees(im, 10);
return 0;
}