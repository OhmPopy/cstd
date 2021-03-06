
struct LSTM : public Layer
{
public:
  LSTM();
  virtual ~LSTM();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

public:
  // param
  int num_output;
  int weight_data_size;

  // model
  Blob weight_hc_data;
  Blob weight_xc_data;
  Blob bias_c_data;
};


LSTM::LSTM()
{
  one_blob_only = false;
  support_inplace = false;
}

LSTM::~LSTM()
{
}

int LSTM::load_param(const ParamDict& pd)
{
  num_output = pd.get(0, 0);
  weight_data_size = pd.get(1, 0);

  return 0;
}

#if NCNN_STDIO
int LSTM::load_model(FILE* binfp)
{
  int nread;

  int size = weight_data_size / 2 / num_output / 4;

  // raw weight data
  weight_hc_data.create(size * 4, num_output);

  if (weight_hc_data.empty()) {
    return -100;
  }

  nread = fread(weight_hc_data.data, size * 4 * num_output * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "LSTM read weight_hc_data failed %d\n", nread);
    return -1;
  }

  weight_xc_data.create(size * 4, num_output);

  if (weight_xc_data.empty()) {
    return -100;
  }

  nread = fread(weight_xc_data.data, size * 4 * num_output * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "LSTM read weight_xc_data failed %d\n", nread);
    return -1;
  }

  bias_c_data.create(4, num_output);

  if (bias_c_data.empty()) {
    return -100;
  }

  nread = fread(bias_c_data.data, 4 * num_output * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "LSTM read bias_c_data failed %d\n", nread);
    return -1;
  }

  return 0;
}
#endif // NCNN_STDIO

int LSTM::load_model(const unsigned char*& mem)
{
  int size = weight_data_size / 2 / num_output / 4;

  // raw weight data
  weight_hc_data = Blob(size * 4, num_output, (float*)mem);
  mem += size * 4 * num_output * sizeof(float);

  weight_xc_data = Blob(size * 4, num_output, (float*)mem);
  mem += size * 4 * num_output * sizeof(float);

  bias_c_data = Blob(4, num_output, (float*)mem);
  mem += 4 * num_output * sizeof(float);

  return 0;
}

int LSTM::forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const
{
  // size x 1 x T
  const Blob* input_blob = bottom_blobs[0];

  // T, 0 or 1 each
  const Blob* cont_blob = bottom_blobs[1];

  int T = input_blob.c;
  int size = input_blob.w;

  // initial hidden state
  Blob hidden(num_output);

  if (hidden.empty()) {
    return -100;
  }

  hidden.fill(0.f);

  // internal cell state
  Blob cell(num_output);

  if (cell.empty()) {
    return -100;
  }

  // 4 x num_output
  Blob gates(4, num_output);

  if (gates.empty()) {
    return -100;
  }

  Blob* next = top_blobs[0];
  next->create(num_output, 1, T);

  if (next->empty()) {
    return -100;
  }

  // unroll
  for (t = 0; t < T; t++) {
    // clip hidden by continuation indicator
    // h_cont_{t-1} = cont_t * h_{t-1}
    // h_cont_{t-1} = h_{t-1} if cont_t == 1
    //                0       otherwise
    // calculate hidden
    // gate_input_t := W_hc * h_conted_{t-1} + W_xc * x_t + b_c
    const float cont = cont_blob.data[t];
    const Blob x = input_blob.channel(t);
    float* hidden_data = hidden;

    for (q = 0; q < num_output; q++) {
      float h_cont = cont ? hidden_data[q] : 0.f;

      const float* x_data = x;
      const float* bias_c_data_ptr = bias_c_data.data + 4 * q;
      float* gates_data = gates.data + 4 * q;

      // gate I F O G
      const float* weight_hc_data_I = weight_hc_data.data + weight_hc_data.w * q;
      const float* weight_xc_data_I = weight_xc_data.data + weight_xc_data.w * q;
      const float* weight_hc_data_F = weight_hc_data.data + weight_hc_data.w * q + size;
      const float* weight_xc_data_F = weight_xc_data.data + weight_xc_data.w * q + size;
      const float* weight_hc_data_O = weight_hc_data.data + weight_hc_data.w * q + size * 2;
      const float* weight_xc_data_O = weight_xc_data.data + weight_xc_data.w * q + size * 2;
      const float* weight_hc_data_G = weight_hc_data.data + weight_hc_data.w * q + size * 3;
      const float* weight_xc_data_G = weight_xc_data.data + weight_xc_data.w * q + size * 3;

      float I = bias_c_data_ptr[0];
      float F = bias_c_data_ptr[1];
      float O = bias_c_data_ptr[2];
      float G = bias_c_data_ptr[3];

      for (i = 0; i < size; i++) {
        I += weight_hc_data_I[i] * h_cont + weight_xc_data_I[i] * x_data[i];
        F += weight_hc_data_F[i] * h_cont + weight_xc_data_F[i] * x_data[i];
        O += weight_hc_data_O[i] * h_cont + weight_xc_data_O[i] * x_data[i];
        G += weight_hc_data_G[i] * h_cont + weight_xc_data_G[i] * x_data[i];
      }

      gates_data[0] = I;
      gates_data[1] = F;
      gates_data[2] = O;
      gates_data[3] = G;
    }

    // lstm unit
    // sigmoid(I)
    // sigmoid(F)
    // sigmoid(O)
    // tanh(G)
    // c_t := f_t .* c_{t-1} + i_t .* g_t
    // h_t := o_t .* tanh[c_t]
    float* cell_data = cell;
    Blob output = Blob_channel_p(next, t);
    float* output_data = output;

    for (q = 0; q < num_output; q++) {
      float* gates_data = gates.data + 4 * q;

      float I = gates_data[0];
      float F = gates_data[1];
      float O = gates_data[2];
      float G = gates_data[3];

      I = 1.f / (1.f + exp(-I));
      F = cont ? 0.f : 1.f / (1.f + exp(-F));
      O = 1.f / (1.f + exp(-O));
      G = tanh(G);

      float cell = F * cell_data[q] + I * G;
      float H = O * tanh(cell);

      cell_data[q] = cell;
      hidden_data[q] = H;
      output_data[q] = H;
    }

    // no cell output here
  }

  return 0;
}

} // namespace ncnn
