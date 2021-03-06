

#include "convolutiondepthwise_arm.h"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace ncnn
{

#include "convolution_1x1.h"
#include "convolution_2x2.h"
#include "convolution_3x3.h"
#include "convolution_4x4.h"
#include "convolution_5x5.h"
#include "convolution_7x7.h"

DEFINE_LAYER_CREATOR(ConvolutionDepthWise_arm)

int ConvolutionDepthWise_arm::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  // convolv with NxN kernel
  // value = value + bias

  if (kernel_size > 7 || stride > 4 || dilation != 1) {
    return ConvolutionDepthWise::forward(bottom_blob, top_blob);
  }

  typedef void (*conv_func)(const img_t*, img_t*, const img_t*, const img_t*);

  // kernel_size x stride
  conv_func conv_func_table[7][4] = {
    {
      conv1x1s1_neon,
      conv1x1s2_neon,
      0,
      0
    }, // kernel_size = 1
    {
      conv2x2s1_neon,
      0,
      0,
      0
    }, // kernel_size = 2
    {
      conv3x3s1_neon,
      conv3x3s2_neon,
      0,
      0
    }, // kernel_size = 3
    {
      0,
      0,
      0,
      conv4x4s4_neon
    }, // kernel_size = 4
    {
      conv5x5s1_neon,
      conv5x5s2_neon,
      0,
      0
    }, // kernel_size = 5
    {
      0,
      0,
      0,
      0
    }, // kernel_size = 6
    {
      conv7x7s1_neon,
      conv7x7s2_neon,
      0,
      0
    }  // kernel_size = 7
  };

  conv_func conv = conv_func_table[kernel_size - 1][stride - 1];

  if (!conv) {
    return ConvolutionDepthWise::forward(bottom_blob, top_blob);
  }

  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);

  img_t bottom_blob_bordered = bottom_blob;

  if (pad > 0) {
    copy_make_border(bottom_blob, bottom_blob_bordered, pad, pad, pad, pad, BORDER_CONSTANT, 0.f);

    if (bottom_blob_bordered.empty()) {
      return -100;
    }

    w = bottom_blob_bordered.w;
    h = bottom_blob_bordered.h;
  }
  else if (pad == -233) {
    int wpad = kernel_size + (w - 1) / stride * stride - w;
    int hpad = kernel_size + (h - 1) / stride * stride - h;

    if (wpad > 0 || hpad > 0) {
      copy_make_border(bottom_blob, bottom_blob_bordered, hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, BORDER_CONSTANT, 0.f);

      if (bottom_blob_bordered.empty()) {
        return -100;
      }
    }

    w = bottom_blob_bordered.w;
    h = bottom_blob_bordered.h;
  }

  int outw = (w - kernel_size) / stride + 1;
  int outh = (h - kernel_size) / stride + 1;

  top_blob->create(outw, outh, num_output);

  if (top_blob->empty()) {
    return -100;
  }

  const int maxk = kernel_size * kernel_size;

  // depth-wise
  if (channels == group && group == num_output) {
#ifdef _OPENMP
    int nested_current = omp_get_nested();
    omp_set_nested(0);
#endif

    #pragma omp parallel for

    for (int g = 0; g < group; g++) {
      img_t bottom_blob_bordered_g = bottom_blob_bordered.channel(g);
      img_t top_blob_g = pBox_channel_p(top_blob, g);
      img_t weight_data_g(maxk, (float*)(weight_data + maxk * g));
      img_t bias_data_g;

      if (bias_term) {
        bias_data_g = img_t(1, (float*)(bias_data + g));
      }

      conv(bottom_blob_bordered_g, top_blob_g, weight_data_g, bias_data_g);
    }

#ifdef _OPENMP
    omp_set_nested(nested_current);
#endif
    return 0;
  }

  const int channels_g = channels / group;
  const int num_output_g = num_output / group;

  for (int g = 0; g < group; g++) {
    img_t bottom_blob_bordered_g(w, h, channels_g, bottom_blob_bordered.channel(channels_g * g));
    img_t top_blob_g(outw, outh, num_output_g, pBox_channel_p(top_blob, num_output_g * g));
    img_t weight_data_g(maxk * channels_g * num_output_g, (float*)(weight_data + maxk * channels_g * num_output_g * g));
    img_t bias_data_g;

    if (bias_term) {
      bias_data_g = img_t(num_output_g, (float*)(bias_data + num_output_g * g));
    }

    conv(bottom_blob_bordered_g, top_blob_g, weight_data_g, bias_data_g);
  }

  return 0;
}

} // namespace ncnn
