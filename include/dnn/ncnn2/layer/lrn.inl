

#include "lrn.h"
#include <math.h>

namespace ncnn
{

DEFINE_LAYER_CREATOR(LRN)

LRN::LRN()
{
  one_blob_only = true;
  support_inplace = true;
}

int LRN::load_param(const ParamDict& pd)
{
  region_type = pd.get(0, 0);
  local_size = pd.get(1, 5);
  alpha = pd.get(2, 1.f);
  beta = pd.get(3, 0.75f);

  return 0;
}

int LRN::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  // squared values with local_size padding
  img_t square_blob;
  square_blob.create(w, h, channels);

  if (square_blob.empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = square_blob.channel(q);

    for (int i = 0; i < size; i++) {
      outptr[i] = ptr[i] * ptr[i];
    }
  }

  if (region_type == NormRegion_ACROSS_CHANNELS) {
    top_blob->fill(0.f);

    const float alpha_div_size = alpha / local_size;

    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      // square sum
      float* outptr = pBox_channel_p(top_blob, q);

      for (int p = q - local_size / 2; p <= q + local_size / 2; p++) {
        if (p < 0 || p >= channels) {
          continue;
        }

        const float* sptr = square_blob.channel(p);

        for (int i = 0; i < size; i++) {
          outptr[i] += sptr[i];
        }
      }

      const float* ptr = pBox_channel_p(bottom_blob, q);

      for (int i = 0; i < size; i++) {
        outptr[i] = ptr[i] * pow(1.f + alpha_div_size * outptr[i], -beta);
      }
    }
  }
  else if (region_type == NormRegion_WITHIN_CHANNEL) {
    int outw = w;
    int outh = h;

    img_t square_blob_bordered = square_blob;
    int pad = local_size / 2;

    if (pad > 0) {
      copy_make_border(square_blob, square_blob_bordered, pad, local_size - pad - 1, pad, local_size - pad - 1, BORDER_CONSTANT, 0.f);

      if (square_blob_bordered.empty()) {
        return -100;
      }

      w = square_blob_bordered.w;
      h = square_blob_bordered.h;
    }

    const int maxk = local_size * local_size;

    const float alpha_div_size = alpha / maxk;

    // norm window offsets
    std::vector<int> _space_ofs(maxk);
    int* space_ofs = &_space_ofs[0];
    {
      int p1 = 0;
      int p2 = 0;
      int gap = w - local_size;

      for (int i = 0; i < local_size; i++) {
        for (int j = 0; j < local_size; j++) {
          space_ofs[p1] = p2;
          p1++;
          p2++;
        }

        p2 += gap;
      }
    }

    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      const img_t m = square_blob_bordered.channel(q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (int i = 0; i < outh; i++) {
        for (int j = 0; j < outw; j++) {
          const float* sptr = m.row(i) + j;

          float ss = 0.f;

          for (int k = 0; k < maxk; k++) {
            float val = sptr[ space_ofs[k] ];
            ss += val;
          }

          outptr[j] = ptr[j] * pow(1.f + alpha_div_size * ss, -beta);
        }

        ptr += outw;
        outptr += outw;
      }
    }
  }

  return 0;
}

int LRN::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  // squared values with local_size padding
  img_t square_blob;
  square_blob.create(w, h, channels);

  if (square_blob.empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_top_blob, q);
    float* outptr = square_blob.channel(q);

    for (int i = 0; i < size; i++) {
      outptr[i] = ptr[i] * ptr[i];
    }
  }

  if (region_type == NormRegion_ACROSS_CHANNELS) {
    img_t square_sum;
    square_sum.create(w, h, channels);

    if (square_sum.empty()) {
      return -100;
    }

    square_sum.fill(0.f);

    const float alpha_div_size = alpha / local_size;

    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      // square sum
      float* ssptr = square_sum.channel(q);

      for (int p = q - local_size / 2; p <= q + local_size / 2; p++) {
        if (p < 0 || p >= channels) {
          continue;
        }

        const float* sptr = square_blob.channel(p);

        for (int i = 0; i < size; i++) {
          ssptr[i] += sptr[i];
        }
      }

      float* ptr = pBox_channel_p(bottom_top_blob, q);

      for (int i = 0; i < size; i++) {
        ptr[i] = ptr[i] * pow(1.f + alpha_div_size * ssptr[i], -beta);
      }
    }
  }
  else if (region_type == NormRegion_WITHIN_CHANNEL) {
    int outw = w;
    int outh = h;

    img_t square_blob_bordered = square_blob;
    int pad = local_size / 2;

    if (pad > 0) {
      copy_make_border(square_blob, square_blob_bordered, pad, local_size - pad - 1, pad, local_size - pad - 1, BORDER_CONSTANT, 0.f);

      if (square_blob_bordered.empty()) {
        return -100;
      }

      w = square_blob_bordered.w;
      h = square_blob_bordered.h;
    }

    const int maxk = local_size * local_size;

    const float alpha_div_size = alpha / maxk;

    // norm window offsets
    std::vector<int> _space_ofs(maxk);
    int* space_ofs = &_space_ofs[0];
    {
      int p1 = 0;
      int p2 = 0;
      int gap = w - local_size;

      for (int i = 0; i < local_size; i++) {
        for (int j = 0; j < local_size; j++) {
          space_ofs[p1] = p2;
          p1++;
          p2++;
        }

        p2 += gap;
      }
    }

    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(bottom_top_blob, q);
      const img_t m = square_blob_bordered.channel(q);

      for (int i = 0; i < outh; i++) {
        for (int j = 0; j < outw; j++) {
          const float* sptr = m.row(i) + j;

          float ss = 0.f;

          for (int k = 0; k < maxk; k++) {
            float val = sptr[ space_ofs[k] ];
            ss += val;
          }

          ptr[j] = ptr[j] * pow(1.f + alpha_div_size * ss, -beta);
        }

        ptr += outw;
      }
    }
  }

  return 0;
}

} // namespace ncnn
