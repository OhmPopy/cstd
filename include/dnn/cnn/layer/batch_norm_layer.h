
//#include "batchnorm.inl"

static Dtype BatchNormLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  struct Blob* mean = WBLOB(0);
  struct Blob* var = WBLOB(1);
  struct Blob* scale = WBLOB(2);
  //float scale = FARG(scale);
  float eps = 1e-5;//FARG(eps);//����ƽ��ϵ��
  float moving_average_fraction = 0.99;//FARG(moving_average_fraction);
  int use_global_stats = false;//IARG(use_global_stats);
  int disable_mean = false;//IARG(disable_mean);
  int disable_variance = false;//IARG(disable_variance);
  
  // extra temporarary variables is used to carry out sums/broadcasting
  // using BLAS
  int channels = prev->size.c;
  int mean_count = channels;
  int variance_count = mean_count;
  int temp_count = Data_count(prev->size);
  int x_norm_count = Data_count(prev->size);

  float* batch_sum_multiplier = NULL;
  float* num_by_chans = NULL;
  float* spatial_sum_multiplier = NULL;
  int numbychans, spatial_dim;
  ASSERT(layer->w_size==3);
  //ѵ��ʱ��ֵ�ͷ����ǻ���ÿ��batch���м���ģ������Ե�ʱ���ֵ�ͷ����Ƕ��������ݼ����Եġ�
  use_global_stats = MNARG(phase) == M_Test;
  //use_global_stats = MARG(use_global_stats);
  //if (moving_average_fraction<0.01) {    moving_average_fraction = 0.99;  }

//batch_sum_multiplier_ batch_size��С�Ŀռ䣬Ҳ�Ǹ����ڼ���meanBuffʱ������Ҫͼ�����Ӧ��ͨ��ֵ��ӡ�
  resetArr(&batch_sum_multiplier, prev->size.n);
  blas_sset(prev->size.n, 1.f, batch_sum_multiplier);

  //spatial_sum_multiplier_��һ��ͼ���С�Ŀռ�(height*width)������ʼ��ֵΪ 1 �� 
  //�������ڼ���meanBuffʱ����ͨ���˵ķ�ʽ��һ��ͼ���ֵ��ӣ������һ����ֵ 
  spatial_dim = Data_2DSize(prev->size);
  resetArr(&spatial_sum_multiplier, spatial_dim);
  blas_sset(spatial_dim, 1.f, spatial_sum_multiplier); //��ʼ��ֵΪ 1
  
  numbychans = channels*prev->size.n;
  resetArr(&num_by_chans, numbychans);
  blas_sset(numbychans, 0, num_by_chans);

  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      int i = 0;
      float* meanBuff = resetArrZero(&next->bufs[i++], mean_count);  //ͨ����,��channelֵ��С���洢���Ǿ�ֵ
      float* varianceBuff = resetArrZero(&next->bufs[i++], variance_count);//ͨ��������channelֵ��С���洢���Ƿ���ֵ
      float* tempBuff = resetZero(&next->bufs[i++], prev->size);
      float* x_normBuff = resetZero(&next->bufs[i++], prev->size);
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      DataSize size = prev->size;
      int num = size.n;
      int spatial_dim = Data_2DSize(size); //spatial_dimֵ�� ͼ��height*width  
      
      if (prevData != nextData) {
        blas_scopy(Data_count(size), prevData, nextData);
      }
      
      if (use_global_stats) {
        // ֱ���ñ���ķ���;�ֵ������ƽ��ϵ��
        // use the stored mean/variance estimates.
        const float scale_factor = scale->data[0] == 0 ? 0 : 1 / scale->data[0];
        blas_sscale(mean_count, scale_factor, mean->data, meanBuff);
        blas_sscale(variance_count, scale_factor, var->data, varianceBuff);
      }
      else {
        if (!disable_mean) {
          // compute mean �����ֵ
          //��ÿһ��ͼ��ֵ���Ϊһ��ֵ������channels_ * num��ֵ��Ȼ���ٳ��� 1/num*spatial_dim������洢��blob num_by_chans_��
          //channel*num �У�spatial_dim �У���С��height*width
          blas_sgemv(CblasNoTrans, channels * num, spatial_dim, 1. / (num * spatial_dim), prevData, spatial_sum_multiplier, 0., num_by_chans);
          //�������õ���ֵ��С��num*channel�� ��ͼ���ÿ��ͨ����ֵ��ӣ������channel����ֵ������洢��meanBuff��
          blas_sgemv(CblasTrans, num, channels, 1., num_by_chans, batch_sum_multiplier, 0., meanBuff);
        }
      }
      
      if (!disable_mean) {
        // subtract mean
        //��ȥ��ֵ ��channels_��ֵ�ľ�ֵmean_������չ��num_*channels_*height*width������top_data���ݼ�ȥ��ֵ
        blas_sgemm(CblasNoTrans, CblasNoTrans, num, channels, 1, 1, batch_sum_multiplier, meanBuff, 0., num_by_chans);
        blas_sgemm(CblasNoTrans, CblasNoTrans, channels * num, spatial_dim, 1, -1, num_by_chans, spatial_sum_multiplier, 1., nextData);
      }
      
      if (!use_global_stats) {
        if (!disable_variance) {
          // compute variance using var(X) = E((X-EX)^2)
          // ���㷽��  (X-EX)^2 //��������ÿһ��ֵ�󷽲����洢��blob temp_��  
          blas_ssqr(Data_count(size), nextData, tempBuff);  // (X-EX)^2
          //ͬ�ϼ��� mean_�ķ�ʽ������ ���� ��
          blas_sgemv(CblasNoTrans, channels * num, spatial_dim, 1. / (num * spatial_dim), tempBuff, spatial_sum_multiplier, 0., num_by_chans);
          // E((X_EX)^2)//ͬ�ϼ��� mean_�ķ�ʽ������ ���� �� (�˴�num_by_chans_ת��)  
          blas_sgemv(CblasTrans, num, channels, 1., num_by_chans, batch_sum_multiplier, 0., varianceBuff);
        }
        
        // ���㻬��ƽ�� compute and save moving average
        scale->data[0] *= moving_average_fraction;
        scale->data[0] += 1;
        if (!disable_mean) {
          blas_saxpby(mean_count, 1.f, meanBuff, moving_average_fraction, mean->data);
        }
        if (!disable_variance) {
          int m = Data_count(size) / channels;
          float bias_correction_factor = m > 1 ? (float)(m) / (m - 1) : 1;
          blas_saxpby(variance_count, bias_correction_factor, varianceBuff, moving_average_fraction, var->data);
        }
      }
      
      if (!disable_variance) {
        // normalize variance //�� variance ÿ��ֵ��һ����С��ֵ eps_,��ֹ�� 0�������  
        blas_sadd_scalar(variance_count, eps, varianceBuff);
        blas_ssqrt(variance_count, varianceBuff, varianceBuff);
        
        // replicate variance to input size
        // ��������������ͬ�����mean_һ������channels_��ֵ�ķ���variance_������չ��num_*channels_*height*width
        blas_sgemm(CblasNoTrans, CblasNoTrans, num, channels, 1, 1, batch_sum_multiplier, varianceBuff, 0., num_by_chans);
        blas_sgemm(CblasNoTrans, CblasNoTrans, channels * num, spatial_dim, 1, 1., num_by_chans, spatial_sum_multiplier, 0., tempBuff);
        blas_sdiv(temp_count, nextData, tempBuff, nextData);
      }
      // TODO(cdoersch): The caching is only needed because later in-place layers
      //                 might clobber the data.  Can we skip this if they won't?
      blas_scopy(x_norm_count, nextData, x_normBuff); //�� ���Ľ��top_data ���ݸ��� �� x_norm_�С�
      if (0) {
        double aaa = Blob_dataDot(next);
        for (i=0; i<Data_count(next->size); ++i) {
          if (fabs(nextData[i])>10) {
            printf("%f\n", nextData[i]);
          }
        }
        printf("%f\n", aaa);
      }
    }
    break;
  case Op_Backward:
    {
      int i = 0;
      float* meanBuff = next->bufs[i++];
      const float* varianceBuff = next->bufs[i++];
      const float* tempBuff = next->bufs[i++];
      float* x_normBuff = next->bufs[i++];
      float* prevDiff = setDiffZero(prev);
      const float* nextDiff;
      const float* nextData = x_normBuff;
      int num, spatial_dim;
      if (prev != next) {
        nextDiff = getDiff(next);
      }
      else {
        blas_scopy(x_norm_count, getDiff(next), x_normBuff);
        nextDiff = x_normBuff;
      }
      if (use_global_stats) {
        if (disable_variance) {
          if (prev != next) {
            blas_scopy(Data_count(next->size), getDiff(next), prevDiff);
          }
        }
        else {
          blas_sdiv(temp_count, nextDiff, tempBuff, prevDiff);
        }
        return 0;
      }
      num = prev->size.n;
      spatial_dim = Data_count(prev->size) / (prev->size.n*channels);
      // if Y = (X-mean(X))/(sqrt(var(X)+eps)), then
      //
      // dE(Y)/dX =
      //   (dE/dY - mean(dE/dY) - mean(dE/dY \cdot Y) \cdot Y)
      //     ./ sqrt(var(X) + eps)
      //
      // where \cdot and ./ are hadamard product and elementwise division,
      // respectively, dE/dY is the next diff, and mean/var/sum are all computed
      // along all dimensions except the channels dimension.  In the above
      // equation, the operations allow for expansion (i.e. broadcast) along all
      // dimensions except the channels dimension where required.
      // --------------------------------------------------------
      // If disable_vairance is set, the derivative change to
      // dE(Y)/dX = dE/dY - mean(dE/dY)
      // If disable_mean is set, derivative becomes
      // dE(Y)/dX =
      //   (dE/dY - mean(dE/dY \cdot Y) \cdot Y)
      //     ./ sqrt(var(X) + eps)
      
      if (!disable_variance) {
        // sum(dE/dY \cdot Y)
        blas_smul(temp_count, nextData, nextDiff, prevDiff);
        blas_sgemv(CblasNoTrans, channels * num, spatial_dim, 1., prevDiff, spatial_sum_multiplier, 0., num_by_chans);
        blas_sgemv(CblasTrans, num, channels, 1., num_by_chans, batch_sum_multiplier, 0., meanBuff);
        // reshape (broadcast) the above
        blas_sgemm(CblasNoTrans, CblasNoTrans, num, channels, 1, 1, batch_sum_multiplier, meanBuff, 0., num_by_chans);
        blas_sgemm(CblasNoTrans, CblasNoTrans, channels * num, spatial_dim, 1, 1., num_by_chans, spatial_sum_multiplier, 0., prevDiff);
        // sum(dE/dY \cdot Y) \cdot Y
        blas_smul(temp_count, nextData, prevDiff, prevDiff);
      }
      else {
        blas_sset(temp_count, (0.f), prevDiff);
      }
      
      if (!disable_mean) {
        // sum(dE/dY)
        blas_sgemv(CblasNoTrans, channels * num, spatial_dim, 1., nextDiff, spatial_sum_multiplier, 0., num_by_chans);
        blas_sgemv(CblasTrans, num, channels, 1., num_by_chans, batch_sum_multiplier, 0., meanBuff);
        // reshape (broadcast) the above to make
        // sum(dE/dY)+sum(dE/dY \cdot Y) \cdot Y
        blas_sgemm(CblasNoTrans, CblasNoTrans, num, channels, 1, 1, batch_sum_multiplier, meanBuff, 0., num_by_chans);
        blas_sgemm(CblasNoTrans, CblasNoTrans, num * channels, spatial_dim, 1, 1., num_by_chans, spatial_sum_multiplier, 1., prevDiff);
      }
      
      // dE/dY - mean(dE/dY)-mean(dE/dY \cdot Y) \cdot Y
      blas_saxpby(temp_count, 1.f, nextDiff, (float)(-1. / (num * spatial_dim)), prevDiff);
      
      if (!disable_variance) {
        // note: temp still contains sqrt(var(X)+eps), computed during the forward pass.
        blas_sdiv(temp_count, prevDiff, tempBuff, prevDiff);
      }
    }
    break;
  default:
    break;
  }
  FREE(batch_sum_multiplier);
  FREE(num_by_chans);
  FREE(spatial_sum_multiplier);
  return 0;
}
static int net_bn(struct Net* net, int input)
{
  const char* type = "bn";
  struct Layer* layer = net_addLayer(net, 1, 1, 3, BatchNormLayer, type);
  DataSize prevSize = IBLOB(0)->size;
  int m, v, s;
  LIBLOB(0) = input;
  m = LWBLOB(0) = net_addBlob(net, dataSize(1, prevSize.c, 1, 1), type, "m");
  v = LWBLOB(1) = net_addBlob(net, dataSize(1, prevSize.c, 1, 1), type, "v");
  s = LWBLOB(2) = net_addBlob(net, dataSize(1, 1, 1, 1), type, "s");
  Blob_init_const(BLOB(m), 0.f);
  Blob_init_const(BLOB(v), 0.f);
  Blob_init_const(BLOB(s), 0.f);
  return LOBLOB(0) = net_addBlob(net, IBLOB(0)->size, type, "o");
}
