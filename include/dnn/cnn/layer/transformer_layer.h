


#include "caffe/layer.hpp"

#include "caffe/custom_layers.hpp"



  
  void TransformerLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    top[0]->ReshapeLike(*bottom[0]);

    CHECK(prevSize1.c == 6) << "Theta's dimension must be 6.";

    int shape1[3] = { 1, 3, bottom[0]->shape(2) * bottom[0]->shape(3) };
    vector<int> CoordinateTarget_shape(shape1, shape1 + 3);

    CoordinateTarget.Reshape(CoordinateTarget_shape);
    Dtype* CoordinateTarget_data = CoordinateTarget;
    for (i = 0; i < bottom[0]->shape(2); i++) {
      for (j = 0; j < bottom[0]->shape(3); j++) {
        CoordinateTarget_data[i * bottom[0]->shape(2) + j] = (Dtype)i / (Dtype)bottom[0]->shape(2) * 2 - 1;
        CoordinateTarget_data[i * bottom[0]->shape(2) + j + CoordinateTarget.shape(2)] = (Dtype)j / (Dtype)bottom[0]->shape(3) * 2 - 1;
        CoordinateTarget_data[i * bottom[0]->shape(2) + j + CoordinateTarget.shape(2) * 2] = 1;
      }
    }
    int shape2[4] = { prevSize.n, 2, bottom[0]->shape(2), bottom[0]->shape(3) };
    vector<int> CoordinateSource_shape(shape2, shape2 + 4);
    CoordinateSource.Reshape(CoordinateSource_shape);
  }

  
  void TransformerLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    const Dtype* prevData = prevData;
    Dtype* nextData = nextData;
    const Dtype* theta_data = nextData;
    const Dtype* CoordinateTarget_data = CoordinateTarget.cpu_data();
    Dtype*  CoordinateSource_data = CoordinateSource;
    int num = prevSize.n;
    int spatial_dim = bottom[0]->shape(2) * bottom[0]->shape(3);

    blas_sset(top[0]->count(), 0, nextData);
    for (n = 0; n < num; n++) {
      /*theta_data += 6 * n;
      LOG(INFO) << "theta:" << theta_data[0] << " " << theta_data[1] << " " << theta_data[2] << std::endl
      << theta_data[3] << " " << theta_data[4] << " " << theta_data[5];
      theta_data -= 6 * n;*/
      /*CoordinateSource_data += n * 3 * spatial_dim;
      LOG(INFO) << CoordinateTarget_data[0] << " " << CoordinateTarget_data[1] << " " << CoordinateTarget_data[2] << std::endl
      << CoordinateTarget_data[3] << " " << CoordinateTarget_data[4] << " " << CoordinateTarget_data[5] << std::endl
      << CoordinateTarget_data[6] << " " << CoordinateTarget_data[7] << " " << CoordinateTarget_data[8]<<std::endl
      << CoordinateTarget_data[9] << " " << CoordinateTarget_data[10] << " " << CoordinateTarget_data[11] << std::endl
      << CoordinateTarget_data[12] << " " << CoordinateTarget_data[13] << " " << CoordinateTarget_data[14] << std::endl
      << CoordinateTarget_data[15] << " " << CoordinateTarget_data[16] << " " << CoordinateTarget_data[17] << std::endl
      << CoordinateTarget_data[18] << " " << CoordinateTarget_data[19] << " " << CoordinateTarget_data[20] << std::endl
      << CoordinateTarget_data[21] << " " << CoordinateTarget_data[22] << " " << CoordinateTarget_data[23] << std::endl
      << CoordinateTarget_data[24] << " " << CoordinateTarget_data[25] << " " << CoordinateTarget_data[26];
      CoordinateSource_data -= n * 3 * spatial_dim;*/
      blas_sgemm(CblasNoTrans, CblasNoTrans, 2, CoordinateTarget.shape(2), 3,
        (Dtype)(1), theta_data + n * 6, CoordinateTarget_data, (Dtype)(0), CoordinateSource_data + n * 2 * spatial_dim);
      /*CoordinateSource_data += n * 2 * spatial_dim;
      LOG(INFO) << CoordinateSource_data[0] << " " << CoordinateSource_data[1] << " " << CoordinateSource_data[2] << std::endl
      << CoordinateSource_data[3] << " " << CoordinateSource_data[4] << " " << CoordinateSource_data[5] << std::endl
      << CoordinateSource_data[6] << " " << CoordinateSource_data[7] << " " << CoordinateSource_data[8] << std::endl
      << CoordinateSource_data[9] << " " << CoordinateSource_data[10] << " " << CoordinateSource_data[11] << std::endl
      << CoordinateSource_data[12] << " " << CoordinateSource_data[13] << " " << CoordinateSource_data[14] << std::endl
      << CoordinateSource_data[15] << " " << CoordinateSource_data[16] << " " << CoordinateSource_data[17];
      CoordinateSource_data -= n * 2 * spatial_dim;*/
      for (i = 0; i < bottom[0]->shape(2); i++) {
        for (j = 0; j < bottom[0]->shape(3); j++) {
          Dtype x = CoordinateSource_data[CoordinateSource.offset(n, 0, i, j)] * bottom[0]->shape(2) / 2 + (Dtype)bottom[0]->shape(2) / 2;
          Dtype y = CoordinateSource_data[CoordinateSource.offset(n, 1, i, j)] * bottom[0]->shape(3) / 2 + (Dtype)bottom[0]->shape(3) / 2;
          //LOG(INFO) << x << " " << y;
          if (x >= 0 && x <= CoordinateSource.shape(2) - 1 && y >= 0 && y <= CoordinateSource.shape(3) - 1) {
            for (c = 0; c < prevSize.c; c++) {
              for (xx = floor(x); xx <= ceil(x); xx++) {
                for (yy = floor(y); yy <= ceil(y); yy++) {
                  nextData[top[0]->offset(n, c, i, j)] += bottom[0]->data_at(n, c, xx, yy) * (1 - abs(x - xx)) * (1 - abs(y - yy));
                  //LOG(INFO) <<"("<< n << " " << c << " " << i << " " << j << ")("<<x<<","<<y<<")("<<xx<<","<<yy<<") " << nextData[top[0]->offset(n, c, i, j)];
                }
              }
            }
          }
        }
      }
    }
  }

  
  void TransformerLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
    const Dtype* nextDiff = nextDiff;
    const Dtype* nextData = top[0]->cpu_data();
    Dtype* data_diff = bottom[0]->mutable_diff();
    Dtype* theta_diff = bottom[1]->mutable_diff();
    int num = prevSize.n;
    int spatial_dim = bottom[0]->shape(2) * bottom[0]->shape(3);
    const Dtype* CoordinateTarget_data = CoordinateTarget.cpu_data();
    const Dtype*  CoordinateSource_data = CoordinateSource.cpu_data();
    Dtype* CoordinateSource_diff = CoordinateSource.mutable_diff();

    blas_sset(Data_count(prev->size), 0, data_diff);
    blas_sset(CoordinateSource.count(), 0, CoordinateSource_diff);
    for (n = 0; n < num; n++) {
      for (i = 0; i < bottom[0]->shape(2); i++) {
        for (j = 0; j < bottom[0]->shape(3); j++) {
          Dtype x = CoordinateSource_data[CoordinateSource.offset(n, 0, i, j)] * bottom[0]->shape(2) / 2 + (Dtype)bottom[0]->shape(2) / 2;
          Dtype y = CoordinateSource_data[CoordinateSource.offset(n, 1, i, j)] * bottom[0]->shape(3) / 2 + (Dtype)bottom[0]->shape(3) / 2;
          if (x >= 0 && x <= CoordinateSource.shape(2) - 1 && y >= 0 && y <= CoordinateSource.shape(3) - 1) {
            for (c = 0; c < prevSize.c; c++) {
              for (xx = floor(x); xx <= ceil(x); xx++) {
                for (yy = floor(y); yy <= ceil(y); yy++) {
                  data_diff[bottom[0]->offset(n, c, xx, yy)] += nextDiff[top[0]->offset(n, c, i, j)] * (1 - abs(x - xx)) * (1 - abs(y - yy));
                  //LOG(INFO) << n << " " << c << " " << i << " " << j << " " << data_diff[bottom[0]->offset(n, c, xx, yy)];
                  CoordinateSource_diff[CoordinateSource.offset(n, 0, i, j)] += nextDiff[top[0]->offset(n, c, i, j)] * bottom[0]->data_at(n, c, xx, yy) * blas_ssign(xx - x) * (1 - abs(y - yy)) * (Dtype)bottom[0]->shape(2) / 2;
                  CoordinateSource_diff[CoordinateSource.offset(n, 1, i, j)] += nextDiff[top[0]->offset(n, c, i, j)] * bottom[0]->data_at(n, c, xx, yy) * (1 - abs(x - xx)) * blas_ssign(yy - y) * (Dtype)bottom[0]->shape(3) / 2;
                }
              }
            }
          }
        }
      }
      blas_sgemm(CblasNoTrans, CblasTrans, 2, 3, CoordinateTarget.shape(2),
        (Dtype)(1), CoordinateSource_diff + n * 2 * spatial_dim, CoordinateTarget_data, (Dtype)(0), theta_diff + n * 6);
    }

  }


#ifdef CPU_ONLY
  STUB_GPU(TransformerLayer);
#endif

  INSTANTIATE_CLASS(TransformerLayer);
  REGISTER_LAYER_CLASS(Transformer);


