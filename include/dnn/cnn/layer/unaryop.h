

struct UnaryOp : public Layer
{
public:
  UnaryOp();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

  enum {
    Operation_ABS   = 0,
    Operation_NEG   = 1,
    Operation_FLOOR = 2,
    Operation_CEIL  = 3,
    Operation_SQUARE = 4,
    Operation_SQRT  = 5,
    Operation_RSQRT = 6,
    Operation_EXP   = 7,
    Operation_LOG   = 8,
    Operation_SIN   = 9,
    Operation_COS   = 10,
    Operation_TAN   = 11,
    Operation_ASIN  = 12,
    Operation_ACOS  = 13,
    Operation_ATAN  = 14,
    Operation_RECIPROCAL = 15
  };

public:
  // param
  int op_type;
};


UnaryOp::UnaryOp()
{
  one_blob_only = true;
  support_inplace = true;
}

int UnaryOp::load_param(const ParamDict& pd)
{
  op_type = pd.get(0, 0);

  return 0;
}

template<typename Op>
static int unary_op(const Blob* a, Blob* b)
{
  Op op;

  int w = a.w;
  int h = a.h;
  int channels = a.c;
  int size = w * h * channels;

  if (a.dims == 3) {
    b.create(w, h, channels);

    if (b.empty()) {
      return -100;
    }
  }
  else if (a.dims == 2) {
    b.create(w, h);

    if (b.empty()) {
      return -100;
    }
  }
  else if (a.dims == 1) {
    b.create(w);

    if (b.empty()) {
      return -100;
    }
  }

  const float* ptr = a;
  float* outptr = b;

  #pragma omp parallel for

  for (i = 0; i < size; i++) {
    outptr[i] = op(ptr[i]);
  }

  return 0;
}

template<typename Op>
static int unary_op_inplace(Blob* a)
{
  Op op;

  int size = a.total();

  float* ptr = a;

  #pragma omp parallel for

  for (i = 0; i < size; i++) {
    ptr[i] = op(ptr[i]);
  }

  return 0;
}

template<typename T>
struct unary_op_abs : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return fabs(x);
  }
};

template<typename T>
struct unary_op_neg : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return -x;
  }
};

template<typename T>
struct unary_op_floor : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return floor(x);
  }
};

template<typename T>
struct unary_op_ceil : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return ceil(x);
  }
};

template<typename T>
struct unary_op_square : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return x * x;
  }
};

template<typename T>
struct unary_op_sqrt : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return sqrt(x);
  }
};

template<typename T>
struct unary_op_rsqrt : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return 1.f / sqrt(x);
  }
};

template<typename T>
struct unary_op_exp : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return exp(x);
  }
};

template<typename T>
struct unary_op_log : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return log(x);
  }
};

template<typename T>
struct unary_op_sin : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return sin(x);
  }
};

template<typename T>
struct unary_op_cos : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return cos(x);
  }
};

template<typename T>
struct unary_op_tan : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return tan(x);
  }
};

template<typename T>
struct unary_op_asin : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return asin(x);
  }
};

template<typename T>
struct unary_op_acos : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return acos(x);
  }
};

template<typename T>
struct unary_op_atan : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return atan(x);
  }
};

template<typename T>
struct unary_op_reciprocal : std::unary_function<T, T> {
  T operator()(const T& x) const {
    return 1.f / x;
  }
};

int UnaryOp::forward(const Blob* prev, Blob* next) const
{
  if (op_type == Operation_ABS) {
    return unary_op< unary_op_abs<float> >(prev, next);
  }

  if (op_type == Operation_NEG) {
    return unary_op< unary_op_neg<float> >(prev, next);
  }

  if (op_type == Operation_FLOOR) {
    return unary_op< unary_op_floor<float> >(prev, next);
  }

  if (op_type == Operation_CEIL) {
    return unary_op< unary_op_ceil<float> >(prev, next);
  }

  if (op_type == Operation_SQUARE) {
    return unary_op< unary_op_square<float> >(prev, next);
  }

  if (op_type == Operation_SQRT) {
    return unary_op< unary_op_sqrt<float> >(prev, next);
  }

  if (op_type == Operation_RSQRT) {
    return unary_op< unary_op_rsqrt<float> >(prev, next);
  }

  if (op_type == Operation_EXP) {
    return unary_op< unary_op_exp<float> >(prev, next);
  }

  if (op_type == Operation_LOG) {
    return unary_op< unary_op_log<float> >(prev, next);
  }

  if (op_type == Operation_SIN) {
    return unary_op< unary_op_sin<float> >(prev, next);
  }

  if (op_type == Operation_COS) {
    return unary_op< unary_op_cos<float> >(prev, next);
  }

  if (op_type == Operation_TAN) {
    return unary_op< unary_op_tan<float> >(prev, next);
  }

  if (op_type == Operation_ASIN) {
    return unary_op< unary_op_asin<float> >(prev, next);
  }

  if (op_type == Operation_ACOS) {
    return unary_op< unary_op_acos<float> >(prev, next);
  }

  if (op_type == Operation_ATAN) {
    return unary_op< unary_op_atan<float> >(prev, next);
  }

  return 0;
}

int UnaryOp::forward_inplace(Blob* bottom_top_blob) const
{
  if (op_type == Operation_ABS) {
    return unary_op_inplace< unary_op_abs<float> >(bottom_top_blob);
  }

  if (op_type == Operation_NEG) {
    return unary_op_inplace< unary_op_neg<float> >(bottom_top_blob);
  }

  if (op_type == Operation_FLOOR) {
    return unary_op_inplace< unary_op_floor<float> >(bottom_top_blob);
  }

  if (op_type == Operation_CEIL) {
    return unary_op_inplace< unary_op_ceil<float> >(bottom_top_blob);
  }

  if (op_type == Operation_SQUARE) {
    return unary_op_inplace< unary_op_square<float> >(bottom_top_blob);
  }

  if (op_type == Operation_SQRT) {
    return unary_op_inplace< unary_op_sqrt<float> >(bottom_top_blob);
  }

  if (op_type == Operation_RSQRT) {
    return unary_op_inplace< unary_op_rsqrt<float> >(bottom_top_blob);
  }

  if (op_type == Operation_EXP) {
    return unary_op_inplace< unary_op_exp<float> >(bottom_top_blob);
  }

  if (op_type == Operation_LOG) {
    return unary_op_inplace< unary_op_log<float> >(bottom_top_blob);
  }

  if (op_type == Operation_SIN) {
    return unary_op_inplace< unary_op_sin<float> >(bottom_top_blob);
  }

  if (op_type == Operation_COS) {
    return unary_op_inplace< unary_op_cos<float> >(bottom_top_blob);
  }

  if (op_type == Operation_TAN) {
    return unary_op_inplace< unary_op_tan<float> >(bottom_top_blob);
  }

  if (op_type == Operation_ASIN) {
    return unary_op_inplace< unary_op_asin<float> >(bottom_top_blob);
  }

  if (op_type == Operation_ACOS) {
    return unary_op_inplace< unary_op_acos<float> >(bottom_top_blob);
  }

  if (op_type == Operation_ATAN) {
    return unary_op_inplace< unary_op_atan<float> >(bottom_top_blob);
  }

  if (op_type == Operation_RECIPROCAL) {
    return unary_op_inplace< unary_op_reciprocal<float> >(bottom_top_blob);
  }

  return 0;
}

} // namespace ncnn
