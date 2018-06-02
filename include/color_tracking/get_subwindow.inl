
#include "get_feature_map.inl"
#include "cv/matmul.inl"

int get_subwindow(const img_t* im, const int* pos, const int* sz, int non_pca_features, int pca_features, const double* w2c, img_t* out_npca, img_t* out_pca) {
  // [out_npca, out_pca] = get_subwindow(im, pos, sz, non_pca_features, pca_features, w2c)
  // im�������һ֡ͼƬ
  //pos����һ֡��Ŀ����λ�ã�������λ�ã�
  //sz���������С�����������С����֡���仯
  //non_pca_features����gray�����Ҷ�ͼ
  // pca_features����cn����(��Ȿ�Ĺ�ʽ��7������Сֵʱ���൱�������PCA.)
  // Extracts the non-PCA and PCA features from image im at position pos and window size sz.
  //The features are given in non_pca_features and pca_features.
  //out_npca is the window of non-PCA features ����һ����[-0.5,0.5].
  // out_pca is the PCA-features reshaped to [prod(sz) num_pca_feature_dim].
  //  w2c is the Color Names matrix if used.
  
  
  img_t im_patch[10] = {0};
  int i, j;
  
  //%check for out-of-bounds coordinates, and set them to the values at the borders
  
  //%extract image
  //im_patch = im(ys, xs, :);
  imsetsize(im_patch, sz[0], sz[1], im->c, 1);
  for (i=0; i<im_patch->h; ++i) {
    const uchar* im1;
    uchar* im_patch1 = im_patch->tt.data + im_patch->s * i;
    int x, y = i + pos[0] - (sz[0]/2);//%ys�ķ�Χ������������ϱ�~���±�
    y = BOUND(y, 0, im->h-1);
    im1 = im->tt.data + im->s*y;
    for (j=0; j<im_patch->w; ++j) {
      x = j + pos[1] - (sz[1]/2);//%xs�ķ�Χ��������������~���ұ�
      x = BOUND(x, 0, im->w-1);
      memcpy(im_patch1+j*im->c, im1+x*im->c, im->c);
    }
  }
  //imshow(im_patch); imshowmat_u1_cn(im_patch, 0); cvWaitKey(-1);
  
  //% compute non-pca feature map
  if (non_pca_features) {
    //%��ȡͼ���im_patch�е�non_pca_features
    get_feature_map(im_patch, non_pca_features, w2c, out_npca);
  } else {
    //out_npca = [];
  }
  //imshowmat_f8(out_npca); cvWaitKey(-1);
  
  
  //% compute pca feature map
  if (pca_features) {
    get_feature_map(im_patch, pca_features, w2c, out_pca);//%��ȡͼ���im_patch�е�pca_features
    //out_pca = reshape(temp_pca, [prod(sz), size(temp_pca, 3)]);//%�������ݣ��޶���С�����������ͶӰ�������
    out_pca->h*=out_pca->w;
    out_pca->w = out_pca->c/sizeof(double);
    out_pca->c = sizeof(double);
    out_pca->s = out_pca->w*out_pca->c;
    //imshowmat_f8(out_pca); cvWaitKey(-1);
  } else {
    //out_pca = [];
  }
  
  imfrees(im_patch, 10);
  return 0;
}