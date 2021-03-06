

#include "binaryop.h"

#include <algorithm>
#include <functional>

namespace ncnn
{

DEFINE_LAYER_CREATOR(BinaryOp)

BinaryOp::BinaryOp()
{
  one_blob_only = false;
  support_inplace = false;
}

int BinaryOp::load_param(const ParamDict& pd)
{
  op_type = pd.get(0, 0);

  return 0;
}

template<typename Op>
static int binary_op(const Blob* a, const Blob* b, Blob* c)
{
  Op op;

  int w = a.w;
  int h = a.h;
  int channels = a.c;
  int size = w * h;

  int w1 = b.w;
  int h1 = b.h;
  int channels1 = b.c;
  int size1 = w1 * h1;

  if (a.dims == 3) {
    c.create(w, h, channels);

    if (c.empty()) {
      return -100;
    }

    if (b.dims == 3) {
      #pragma omp parallel for

      for (int q = 0; q < channels; q++) {
        const float* ptr = a.channel(q);
        const float* ptr1 = b.channel(q);
        float* outptr = c.channel(q);

        for (int i = 0; i < size; i++) {
          outptr[i] = op(ptr[i], ptr1[i]);
        }
      }

      return 0;
    }

    if (b.dims == 2) {
      #pragma omp parallel for

      for (int q = 0; q < channels; q++) {
        const float* ptr = a.channel(q);
        const float* ptr1 = b.data + h * q;
        float* outptr = c.channel(q);

        for (int y = 0; y < h; y++) {
          const float b0 = ptr1[y];

          for (int x = 0; x < w; x++) {
            outptr[x] = op(ptr[x], b0);
          }

          ptr += w;
          outptr += w;
        }
      }

      return 0;
    }

    if (b.dims == 1) {
      if (b.w == 1) {
        const float b0 = b.data[0];
        #pragma omp parallel for

        for (int q = 0; q < channels; q++) {
          const float* ptr = a.channel(q);
          float* outptr = c.channel(q);

          for (int i = 0; i < size; i++) {
            outptr[i] = op(ptr[i], b0);
          }

          ptr += w;
          outptr += w;
        }

        return 0;
      }

      #pragma omp parallel for

      for (int q = 0; q < channels; q++) {
        const float* ptr = a.channel(q);
        const float b0 = b.data[q];
        float* outptr = c.channel(q);

        for (int i = 0; i < size; i++) {
          outptr[i] = op(ptr[i], b0);
        }

        ptr += w;
        outptr += w;
      }

      return 0;
    }
  }
  else if (a.dims == 2) {
    if (b.dims == 3) {
      c.create(w1, h1, channels1);

      if (c.empty()) {
        return -100;
      }

      #pragma omp parallel for

      for (int q = 0; q < channels1; q++) {
        const float* ptr = a.data + h1 * q;
        const float* ptr1 = b.channel(q);
        float* outptr = c.channel(q);

        for (int y = 0; y < h1; y++) {
          const float a0 = ptr[y];

          for (int x = 0; x < w1; x++) {
            outptr[x] = op(a0, ptr1[x]);
          }

          ptr1 += w1;
          outptr += w1;
        }
      }

      return 0;
    }

    c.create(w, h);

    if (c.empty()) {
      return -100;
    }

    if (b.dims == 2) {
      const float* ptr = a;
      const float* ptr1 = b;
      float* outptr = c;

      for (int i = 0; i < size; i++) {
        outptr[i] = op(ptr[i], ptr1[i]);
      }

      return 0;
    }

    if (b.dims == 1) {
      c.create(w, h);

      if (c.empty()) {
        return -100;
      }

      if (b.w == 1) {
        const float* ptr = a;
        const float b0 = b.data[0];
        float* outptr = c;

        for (int i = 0; i < size; i++) {
          outptr[i] = op(ptr[i], b0);
        }

        return 0;
      }

      const float* ptr = a;
      const float* ptr1 = b;
      float* outptr = c;

      for (int y = 0; y < h; y++) {
        const float b0 = ptr1[y];

        for (int x = 0; x < w; x++) {
          outptr[x] = op(ptr[x], b0);
        }

        ptr += w;
        outptr += w;
      }

      return 0;
    }
  }
  else if (a.dims == 1) {
    if (a.w == 1) {
      if (b.dims == 3) {
        c.create(w1, h1, channels1);

        if (c.empty()) {
          return -100;
        }

        const float a0 = a.data[0];
        #pragma omp parallel for

        for (int q = 0; q < channels1; q++) {
          const float* ptr1 = b.channel(q);
          float* outptr = c.channel(q);

          for (int i = 0; i < size1; i++) {
            outptr[i] = op(a0, ptr1[i]);
          }

          ptr1 += w1;
          outptr += w1;
        }

        return 0;
      }

      if (b.dims == 2) {
        c.create(w1, h1);

        if (c.empty()) {
          return -100;
        }

        const float a0 = a.data[0];
        const float* ptr1 = b;
        float* outptr = c;

        for (int i = 0; i < size1; i++) {
          outptr[i] = op(a0, ptr1[i]);
        }

        return 0;
      }

      if (b.dims == 1) {
        c.create(w1);

        if (c.empty()) {
          return -100;
        }

        const float a0 = a.data[0];
        const float* ptr1 = b;
        float* outptr = c;

        for (int i = 0; i < size1; i++) {
          outptr[i] = op(a0, ptr1[i]);
        }

        return 0;
      }
    }

    if (b.dims == 3) {
      c.create(w1, h1, channels1);

      if (c.empty()) {
        return -100;
      }

      #pragma omp parallel for

      for (int q = 0; q < channels1; q++) {
        const float a0 = a.data[q];
        const float* ptr1 = b.channel(q);
        float* outptr = c.channel(q);

        for (int i = 0; i < size1; i++) {
          outptr[i] = op(a0, ptr1[i]);
        }

        ptr1 += w1;
        outptr += w1;
      }

      return 0;
    }

    if (b.dims == 2) {
      c.create(w1, h1);

      if (c.empty()) {
        return -100;
      }

      const float* ptr = a;
      const float* ptr1 = b;
      float* outptr = c;

      for (int y = 0; y < h1; y++) {
        const float a0 = ptr[y];

        for (int x = 0; x < w1; x++) {
          outptr[x] = op(a0, ptr1[x]);
        }

        ptr1 += w1;
        outptr += w1;
      }

      return 0;
    }

    if (b.dims == 1) {
      c.create(w);

      if (c.empty()) {
        return -100;
      }

      if (b.w == 1) {
        const float* ptr = a;
        const float b0 = b.data[0];
        float* outptr = c;

        for (int i = 0; i < size; i++) {
          outptr[i] = op(ptr[i], b0);
        }

        return 0;
      }

      const float* ptr = a;
      const float* ptr1 = b;
      float* outptr = c;

      for (int i = 0; i < size; i++) {
        outptr[i] = op(ptr[i], ptr1[i]);
      }
    }
  }

  return 0;
}

template<typename T>
struct binary_op_max : std::binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const {
    return MAX(x, y);
  }
};

template<typename T>
struct binary_op_min : std::binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const {
    return MIN(x, y);
  }
};

template<typename T>
struct binary_op_pow : std::binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const {
    return pow(x, y);
  }
};

int BinaryOp::forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const
{
  const Blob* bottom_blob = bottom_blobs[0];
  const Blob* bottom_blob1 = bottom_blobs[1];

  Blob* top_blob = top_blobs[0];

  if (op_type == Operation_ADD) {
    return binary_op< std::plus<float> >(bottom_blob, bottom_blob1, top_blob);
  }

  if (op_type == Operation_SUB) {
    return binary_op< MINus<float> >(bottom_blob, bottom_blob1, top_blob);
  }

  if (op_type == Operation_MUL) {
    return binary_op< std::multiplies<float> >(bottom_blob, bottom_blob1, top_blob);
  }

  if (op_type == Operation_DIV) {
    return binary_op< std::divides<float> >(bottom_blob, bottom_blob1, top_blob);
  }

  if (op_type == Operation_MAX) {
    return binary_op< binary_op_max<float> >(bottom_blob, bottom_blob1, top_blob);
  }

  if (op_type == Operation_MIN) {
    return binary_op< binary_op_min<float> >(bottom_blob, bottom_blob1, top_blob);
  }

  if (op_type == Operation_POW) {
    return binary_op< binary_op_pow<float> >(bottom_blob, bottom_blob1, top_blob);
  }

  return 0;
}

} // namespace ncnn
