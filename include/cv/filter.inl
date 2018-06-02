
typedef struct CSlice {
  int start_index, end_index;
}
CSlice;
CC_INLINE CSlice cSlice(int start, int end)
{
  CSlice slice;
  slice.start_index = start;
  slice.end_index = end;
  return slice;
}


/****************************************************************************************\
*                    CBaseImageFilter: Base class for filtering operations              *
\****************************************************************************************/

#define CC_WHOLE   0
#define CC_START   1
#define CC_END     2
#define CC_MIDDLE  4
#define CC_ISOLATED_ROI 8

typedef void (*CvRowFilterFunc)(const uchar* src, uchar* dst, void* params);
typedef void (*CvColumnFilterFunc)(uchar** src, uchar* dst, int dst_step, int count, void* params);

class CC_EXPORTS CBaseImageFilter
{
public:
    CBaseImageFilter();
    /* calls init() */
    CBaseImageFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       bool _is_separable, CSize _ksize,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));
    virtual ~CBaseImageFilter();

    /* initializes the class for processing an image of maximal width _max_width,
       input image has data type _src_type, the output will have _dst_type.
       _is_separable != 0 if the filter is separable
       (specific behaviour is defined in a derived class), 0 otherwise.
       _ksize and _anchor specify the kernel size and the anchor point. _anchor=(-1,-1) means
       that the anchor is at the center.
       to get interpolate pixel values outside the image _border_mode=CC_BORDER_*** is used,
       _border_value specify the pixel value in case of CC_BORDER_CONSTANT border mode.
       before initialization clear() is called if necessary.
    */
    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       bool _is_separable, CSize _ksize,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));
    /* releases all the internal buffers.
       for the further use of the object, init() needs to be called. */
    virtual void clear();
    /* processes input image or a part of it.
       input is represented either as matrix (img_t* src)
       or a list of row pointers (uchar** src2).
       in the later case width, _src_y1 and _src_y2 are used to specify the size.
       _dst is the output image/matrix.
       _src_roi specifies the roi inside the input image to process,
          (0,0,-1,-1) denotes the whole image.
       _dst_origin is the upper-left corner of the filtered roi within the output image.
       _phase is either CC_START, or CC_END, or CC_MIDDLE, or CC_START|CC_END, or CC_WHOLE,
          which is the same as CC_START|CC_END.
          CC_START means that the input is the first (top) stripe of the processed image [roi],
          CC_END - the input is the last (bottom) stripe of the processed image [roi],
          CC_MIDDLE - the input is neither first nor last stripe.
          CC_WHOLE - the input is the whole processed image [roi].
    */
    virtual int process(const img_t* _src, img_t* _dst,
                         CRect _src_roi=cRect(0,0,-1,-1),
                         CPoint _dst_origin=cPoint(0,0), int _flags=0);
    /* retrieve various parameters of the filtering object */
    TypeId get_src_type() const { return src_type; }
    TypeId get_dst_type() const { return dst_type; }
    int get_src_cn() const { return src_cn; }
    int get_dst_cn() const { return dst_cn; }
    TypeId get_work_type() const { return work_type; }
    CSize get_kernel_size() const { return ksize; }
    CPoint get_anchor() const { return anchor; }
    int get_width() const { return prev_x_range.end_index - prev_x_range.start_index; }
    CvRowFilterFunc get_x_filter_func() const { return x_func; }
    CvColumnFilterFunc get_y_filter_func() const { return y_func; }

protected:
    /* initializes work_type, buf_size and max_rows */ 
    virtual void get_work_params();
    /* it is called (not always) from process when _phase=CC_START or CC_WHOLE.
       the method initializes ring buffer (buf_end, buf_head, buf_tail, buf_count, rows),
       prev_width, prev_x_range, const_row, border_tab, border_tab_sz* */
    virtual void start_process(CSlice x_range, int width);
    /* forms pointers to "virtual rows" above or below the processed roi using the specified
       border mode */
    virtual void make_y_border(int row_count, int top_rows, int bottom_rows);

    virtual int fill_cyclic_buffer(const uchar* src, int src_step,
                                    int y, int y1, int y2);

    enum { ALIGN=32 };
    
    int max_width, src_cn, dst_cn, work_cn;
    /* currently, work_type must be the same as src_type in case of non-separable filters */
    TypeId min_type, src_type, dst_type, work_type;

    /* pointers to convolution functions, initialized by init method.
       for non-separable filters only y_conv should be set */
    CvRowFilterFunc x_func;
    CvColumnFilterFunc y_func;

    uchar* buffer;
    uchar** rows;
    int top_rows, bottom_rows, max_rows;
    uchar *buf_start, *buf_end, *buf_head, *buf_tail;
    int buf_size, buf_step, buf_count, buf_max_count;

    bool is_separable;
    CSize ksize;
    CPoint anchor;
    int max_ky, border_mode;
    CScalar border_value;
    uchar* const_row;
    int* border_tab;
    int border_tab_sz1, border_tab_sz;

    CSlice prev_x_range;
    int prev_width;
};


/* Derived class, for linear separable filtering. */
class CC_EXPORTS CSepFilter : public CBaseImageFilter
{
public:
    CSepFilter();
    CSepFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                 const img_t* _kx, const img_t* _ky,
                 CPoint _anchor=cPoint(-1,-1),
                 CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                 CScalar _border_value=cScalarAll(0));
    virtual ~CSepFilter();

    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       const img_t* _kx, const img_t* _ky,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));
    virtual void init_deriv(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                             int dx, int dy, int aperture_size, int flags=0);
    virtual void init_gaussian(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                                int gaussian_size, double sigma);

    /* dummy method to avoid compiler warnings */
    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       bool _is_separable, CSize _ksize,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    virtual void clear();
    const img_t* get_x_kernel() const { return kx; }
    const img_t* get_y_kernel() const { return ky; }
    int get_x_kernel_flags() const { return kx_flags; }
    int get_y_kernel_flags() const { return ky_flags; }

    enum { GENERIC=0, ASYMMETRICAL=1, SYMMETRICAL=2, POSITIVE=4, SUM_TO_1=8, INTEGER=16 };
    enum { NORMALIZE_KERNEL=1, FLIP_KERNEL=2 };

    static void init_gaussian_kernel(img_t* kernel, double sigma=-1);
    static void init_sobel_kernel(img_t* _kx, img_t* _ky, int dx, int dy, int flags=0);
    static void init_scharr_kernel(img_t* _kx, img_t* _ky, int dx, int dy, int flags=0);

protected:
    img_t kx[1];
    img_t ky[1];
    int kx_flags, ky_flags;
};


/* Derived class, for linear non-separable filtering. */
class CC_EXPORTS CvLinearFilter : public CBaseImageFilter
{
public:
    CvLinearFilter();
    CvLinearFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                    const img_t* _kernel,
                    CPoint _anchor=cPoint(-1,-1),
                    CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                    CScalar _border_value=cScalarAll(0));
    virtual ~CvLinearFilter();

    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       const img_t* _kernel,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    /* dummy method to avoid compiler warnings */
    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       bool _is_separable, CSize _ksize,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    virtual void clear();
    const img_t* get_kernel() const { return kernel; }
    uchar* get_kernel_sparse_buf() { return k_sparse; }
    int get_kernel_sparse_count() const { return k_sparse_count; }

protected:
    img_t kernel[1];
    uchar* k_sparse;
    int k_sparse_count;
};


/****************************************************************************************\
                                    Base Image Filter
\****************************************************************************************/

static void default_x_filter_func(const uchar*, uchar*, void*)
{
}

static void default_y_filter_func(uchar**, uchar*, int, int, void*)
{
}

CBaseImageFilter::CBaseImageFilter()
{
    min_type = CC_8U;
    buffer = 0;
    rows = 0;
    max_width = 0;
    x_func = default_x_filter_func;
    y_func = default_y_filter_func;
}


CBaseImageFilter::CBaseImageFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                                      bool _is_separable, CSize _ksize, CPoint _anchor,
                                      CBorderTypes _border_mode, CScalar _border_value)
{
    min_type = CC_8U;
    buffer = 0;
    rows = 0;
    max_width = 0;
    x_func = default_x_filter_func;
    y_func = default_y_filter_func;

    init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _is_separable,
          _ksize, _anchor, _border_mode, _border_value);
}


void CBaseImageFilter::clear()
{
    if (buffer) cFree(&buffer);
    rows = 0;
}


CBaseImageFilter::~CBaseImageFilter()
{
    clear();
}


void CBaseImageFilter::get_work_params()
{
    int min_rows = max_ky*2 + 3, rows = MAX(min_rows,10), row_sz;
    int width = max_width, trow_sz = 0;

    if(is_separable)
    {
        TypeId max_type = MAX((src_type), (dst_type));
        work_cn = MAX(src_cn, dst_cn);
        max_type = MAX(max_type, min_type);
        work_type = max_type;
        trow_sz = cvAlign((max_width + ksize.width - 1)*src_cn*CC_TYPE_SIZE(src_type), ALIGN);
    }
    else
    {
        work_type = src_type;
        width += ksize.width - 1;
    }
    row_sz = cvAlign(width*src_cn*CC_TYPE_SIZE(work_type), ALIGN);
    buf_size = rows*row_sz;
    buf_size = MIN(buf_size, 1 << 16);
    buf_size = MAX(buf_size, min_rows*row_sz);
    max_rows = (buf_size/row_sz)*3 + max_ky*2 + 8;
    buf_size += trow_sz;
}


void CBaseImageFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                              bool _is_separable, CSize _ksize, CPoint _anchor,
                              CBorderTypes _border_mode, CScalar _border_value)
{
    CC_FUNCNAME("CBaseImageFilter::init");

    __BEGIN__;

    int total_buf_sz, src_pix_sz, row_tab_sz, bsz;
    uchar* ptr;

    if(!(buffer && _max_width <= max_width && _src_type == src_type &&
        _dst_type == dst_type && _is_separable == is_separable &&
        _ksize.width == ksize.width && _ksize.height == ksize.height &&
        _anchor.x == anchor.x && _anchor.y == anchor.y))
        clear();

    is_separable = _is_separable != 0;
    max_width = _max_width; //MAX(_max_width,_ksize.width);
    src_type = (_src_type);
    dst_type = (_dst_type);
    src_cn = _src_cn;
    dst_cn = _dst_cn;
    ksize = _ksize;
    anchor = _anchor;

    if(anchor.x == -1)
        anchor.x = ksize.width / 2;
    if(anchor.y == -1)
        anchor.y = ksize.height / 2;

    max_ky = MAX(anchor.y, ksize.height - anchor.y - 1); 
    border_mode = _border_mode;
    border_value = _border_value;

    if(ksize.width <= 0 || ksize.height <= 0 ||
        (unsigned)anchor.x >= (unsigned)ksize.width ||
        (unsigned)anchor.y >= (unsigned)ksize.height)
        CC_ERROR(CC_StsOutOfRange, "invalid kernel size and/or anchor position");

    if(border_mode != CC_BORDER_CONSTANT && border_mode != CC_BORDER_REPLICATE &&
        border_mode != CC_BORDER_REFLECT && border_mode != CC_BORDER_REFLECT_101)
        CC_ERROR(CC_StsBadArg, "Invalid/unsupported border mode");

    get_work_params();

    prev_width = 0;
    prev_x_range = cSlice(0,0);

    buf_size = cvAlign(buf_size, ALIGN);

    src_pix_sz = CC_TYPE_SIZE(src_type)*src_cn;
    border_tab_sz1 = anchor.x*src_pix_sz;
    border_tab_sz = (ksize.width-1)*src_pix_sz;
    bsz = cvAlign(border_tab_sz*sizeof(int), ALIGN);

    assert(max_rows > max_ky*2);
    row_tab_sz = cvAlign(max_rows*sizeof(uchar*), ALIGN);
    total_buf_sz = buf_size + row_tab_sz + bsz;
    
    CC_CALL(ptr = buffer = (uchar*)cAlloc(total_buf_sz));
    
    rows = (uchar**)ptr;
    ptr += row_tab_sz;
    border_tab = (int*)ptr;
    ptr += bsz;

    buf_start = ptr;
    const_row = 0;

    if(border_mode == CC_BORDER_CONSTANT)
        cvScalarToRawData(&border_value, border_tab, src_type, 0);

    __END__;
}


void CBaseImageFilter::start_process(CSlice x_range, int width)
{
    int mode = border_mode;
    int pix_sz = CC_TYPE_SIZE(src_type)*src_cn, work_pix_sz = src_cn*CC_TYPE_SIZE(work_type);
    int bsz = buf_size, bw = x_range.end_index - x_range.start_index, bw1 = bw + ksize.width - 1;
    int tr_step = cvAlign(bw1*pix_sz, ALIGN);
    int i, j, k, ofs;
    
    if(x_range.start_index == prev_x_range.start_index &&
        x_range.end_index == prev_x_range.end_index &&
        width == prev_width)
        return;

    prev_x_range = x_range;
    prev_width = width;

    if(!is_separable)
        bw = bw1;
    else
        bsz -= tr_step;

    buf_step = cvAlign(bw*work_pix_sz, ALIGN);

    if(mode == CC_BORDER_CONSTANT)
        bsz -= buf_step;
    buf_max_count = bsz/buf_step;
    buf_max_count = MIN(buf_max_count, max_rows - max_ky*2);
    buf_end = buf_start + buf_max_count*buf_step;

    if(mode == CC_BORDER_CONSTANT)
    {
        int i, tab_len = ksize.width*pix_sz;
        uchar* bt = (uchar*)border_tab;
        uchar* trow = buf_end;
        const_row = buf_end + (is_separable ? 1 : 0)*tr_step;

        for(i = pix_sz; i < tab_len; i++)
            bt[i] = bt[i - pix_sz];
        for(i = 0; i < pix_sz; i++)
            trow[i] = bt[i];
        for(i = pix_sz; i < tr_step; i++)
            trow[i] = trow[i - pix_sz];
        if(is_separable)
            x_func(trow, const_row, this);
        return;
    }

    if(x_range.end_index - x_range.start_index <= 1)
        mode = CC_BORDER_REPLICATE;

    width = (width - 1)*pix_sz;
    ofs = (anchor.x-x_range.start_index)*pix_sz;

    for(k = 0; k < 2; k++)
    {
        int idx, delta;
        int i1, i2, di;

        if(k == 0)
        {
            idx = (x_range.start_index - 1)*pix_sz;
            delta = di = -pix_sz;
            i1 = border_tab_sz1 - pix_sz;
            i2 = -pix_sz;
        }
        else
        {
            idx = x_range.end_index*pix_sz;
            delta = di = pix_sz;
            i1 = border_tab_sz1;
            i2 = border_tab_sz;
        }

        if((unsigned)idx > (unsigned)width)
        {
            int shift = mode == CC_BORDER_REFLECT_101 ? pix_sz : 0;
            idx = k == 0 ? shift : width - shift;
            delta = -delta;
        }

        for(i = i1; i != i2; i += di)
        {
            for(j = 0; j < pix_sz; j++)
                border_tab[i + j] = idx + ofs + j;
            if(mode != CC_BORDER_REPLICATE)
            {
                if(delta > 0 && idx == width ||
                    delta < 0 && idx == 0)
                {
                    if(mode == CC_BORDER_REFLECT_101)
                        idx -= delta*2;
                    delta = -delta;
                }
                else
                    idx += delta;
            }
        }
    }
}


void CBaseImageFilter::make_y_border(int row_count, int top_rows, int bottom_rows)
{
    int i;
    
    if(border_mode == CC_BORDER_CONSTANT ||
        border_mode == CC_BORDER_REPLICATE)
    {
        uchar* row1 = border_mode == CC_BORDER_CONSTANT ? const_row : rows[max_ky];
        
        for(i = 0; i < top_rows && rows[i] == 0; i++)
            rows[i] = row1;

        row1 = border_mode == CC_BORDER_CONSTANT ? const_row : rows[row_count-1];
        for(i = 0; i < bottom_rows; i++)
            rows[i + row_count] = row1;
    }
    else
    {
        int j, dj = 1, shift = border_mode == CC_BORDER_REFLECT_101;

        for(i = top_rows-1, j = top_rows+shift; i >= 0; i--)
        {
            if(rows[i] == 0)
                rows[i] = rows[j];
            j += dj;
            if(dj > 0 && j >= row_count)
            {
                if(!bottom_rows)
                    break;
                j -= 1 + shift;
                dj = -dj;
            }
        }

        for(i = 0, j = row_count-1-shift; i < bottom_rows; i++, j--)
            rows[i + row_count] = rows[j];
    }
}


int CBaseImageFilter::fill_cyclic_buffer(const uchar* src, int src_step,
                                           int y0, int y1, int y2)
{
    int i, y = y0, bsz1 = border_tab_sz1, bsz = border_tab_sz;
    int pix_size = CC_TYPE_SIZE(src_type)*src_cn;
    int width = prev_x_range.end_index - prev_x_range.start_index, width_n = width*pix_size;
    bool can_use_src_as_trow = is_separable && width >= ksize.width;

    // fill the cyclic buffer
    for(; buf_count < buf_max_count && y < y2; buf_count++, y++, src += src_step)
    {
        uchar* trow = is_separable ? buf_end : buf_tail;
        uchar* bptr = can_use_src_as_trow && y1 < y && y+1 < y2 ? (uchar*)(src - bsz1) : trow;

        if(bptr != trow)
        {
            for(i = 0; i < bsz1; i++)
                trow[i] = bptr[i];
            for(; i < bsz; i++)
                trow[i] = bptr[i + width_n];
        }
        else if(!(((size_t)(bptr + bsz1)|(size_t)src|width_n) & (sizeof(int)-1)))
            for(i = 0; i < width_n; i += sizeof(int))
                *(int*)(bptr + i + bsz1) = *(int*)(src + i);
        else
            for(i = 0; i < width_n; i++)
                bptr[i + bsz1] = src[i];

        if(border_mode != CC_BORDER_CONSTANT)
        {
            for(i = 0; i < bsz1; i++)
            {
                int j = border_tab[i];
                bptr[i] = bptr[j];
            }
            for(; i < bsz; i++)
            {
                int j = border_tab[i];
                bptr[i + width_n] = bptr[j];
            }
        }
        else
        {
            const uchar *bt = (uchar*)border_tab; 
            for(i = 0; i < bsz1; i++)
                bptr[i] = bt[i];

            for(; i < bsz; i++)
                bptr[i + width_n] = bt[i];
        }

        if(is_separable)
        {
            x_func(bptr, buf_tail, this);
            if(bptr != trow)
            {
                for(i = 0; i < bsz1; i++)
                    bptr[i] = trow[i];
                for(; i < bsz; i++)
                    bptr[i + width_n] = trow[i];
            }
        }

        buf_tail += buf_step;
        if(buf_tail >= buf_end)
            buf_tail = buf_start;
    }

    return y - y0;
}

int CBaseImageFilter::process(const img_t* src, img_t* dst,
                                CRect src_roi, CPoint dst_origin, int flags)
{
    int rows_processed = 0;

    /*
        check_parameters
        initialize_horizontal_border_reloc_tab_if_not_initialized_yet
        
        for_each_source_row: src starts from src_roi.y, buf starts with the first available row
            1) if separable,
                   1a.1) copy source row to temporary buffer, form a border using border reloc tab.
                   1a.2) apply row-wise filter (symmetric, asymmetric or generic)
               else
                   1b.1) copy source row to the buffer, form a border
            2) if the buffer is full, or it is the last source row:
                 2.1) if stage != middle, form the pointers to other "virtual" rows.
                 if separable
                    2a.2) apply column-wise filter, store the results.
                 else
                    2b.2) form a sparse (offset,weight) tab
                    2b.3) apply generic non-separable filter, store the results
            3) update row pointers etc.
    */

    CC_FUNCNAME("CBaseImageFilter::process");

    __BEGIN__;

    int i, width, _src_y1, _src_y2;
    int src_x, src_y, src_y1, src_y2, dst_y;
    int pix_size = CC_TYPE_SIZE(src_type)*src_cn;
    uchar *sptr = 0, *dptr;
    int phase = flags & (CC_START|CC_END|CC_MIDDLE);
    bool isolated_roi = (flags & CC_ISOLATED_ROI) != 0;

    if(!(src))
        CC_ERROR(CC_StsBadArg, "");

    if(CC_MAT_DEPTH(src) != src_type)
        CC_ERROR(CC_StsUnmatchedFormats, "");

    width = src->cols;

    if(src_roi.width == -1 && src_roi.x == 0)
        src_roi.width = width;

    if(src_roi.height == -1 && src_roi.y == 0)
    {
        src_roi.y = 0;
        src_roi.height = src->rows;
    }

    if(src_roi.width > max_width ||
        src_roi.x < 0 || src_roi.width < 0 ||
        src_roi.y < 0 || src_roi.height < 0 ||
        src_roi.x + src_roi.width > width ||
        src_roi.y + src_roi.height > src->rows)
        CC_ERROR(CC_StsOutOfRange, "Too large source image or its ROI");

    src_x = src_roi.x;
    _src_y1 = 0;
    _src_y2 = src->rows;

    if(isolated_roi)
    {
        src_roi.x = 0;
        width = src_roi.width;
        _src_y1 = src_roi.y;
        _src_y2 = src_roi.y + src_roi.height;
    }

    if(!(dst))
        CC_ERROR(CC_StsBadArg, "");

    if(CC_MAT_DEPTH(dst) != dst_type)
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(dst_origin.x < 0 || dst_origin.y < 0)
        CC_ERROR(CC_StsOutOfRange, "Incorrect destination ROI origin");

    if(phase == CC_WHOLE)
        phase = CC_START | CC_END;
    phase &= CC_START | CC_END | CC_MIDDLE;

    // initialize horizontal border relocation tab if it is not initialized yet
    if(phase & CC_START)
        start_process(cSlice(src_roi.x, src_roi.x + src_roi.width), width);
    else if(prev_width != width || prev_x_range.start_index != src_roi.x ||
        prev_x_range.end_index != src_roi.x + src_roi.width)
        CC_ERROR(CC_StsBadArg,
            "In a middle or at the end the horizontal placement of the stripe can not be changed");

    dst_y = dst_origin.y;
    src_y1 = src_roi.y;
    src_y2 = src_roi.y + src_roi.height;

    if(phase & CC_START)
    {
        for(i = 0; i <= max_ky*2; i++)
            rows[i] = 0;
        
        src_y1 -= max_ky;
        top_rows = bottom_rows = 0;

        if(src_y1 < _src_y1)
        {
            top_rows = _src_y1 - src_y1;
            src_y1 = _src_y1;
        }

        buf_head = buf_tail = buf_start;
        buf_count = 0;
    }

    if(phase & CC_END)
    {
        src_y2 += max_ky;

        if(src_y2 > _src_y2)
        {
            bottom_rows = src_y2 - _src_y2;
            src_y2 = _src_y2;
        }
    }
    
    dptr = dst->tt.data + dst_origin.y*dst->step + dst_origin.x*CC_TYPE_SIZE(dst_type);
    sptr = src->tt.data + src_y1*src->step + src_x*pix_size;
        
    for(src_y = src_y1; src_y < src_y2;)
    {
        uchar* bptr;
        int row_count, delta;

        delta = fill_cyclic_buffer(sptr, src->step, src_y, src_y1, src_y2);

        src_y += delta;
        sptr += src->step*delta;

        // initialize the cyclic buffer row pointers
        bptr = buf_head;
        for(i = 0; i < buf_count; i++)
        {
            rows[i+top_rows] = bptr;
            bptr += buf_step;
            if(bptr >= buf_end)
                bptr = buf_start;
        }

        row_count = top_rows + buf_count;
        
        if(!rows[0] || (phase & CC_END) && src_y == src_y2)
        {
            int br = (phase & CC_END) && src_y == src_y2 ? bottom_rows : 0;
            make_y_border(row_count, top_rows, br);
            row_count += br;
        }

        if(rows[0] && row_count > max_ky*2)
        {
            int count = row_count - max_ky*2;
            if(dst_y + count > dst->rows)
                CC_ERROR(CC_StsOutOfRange, "The destination image can not fit the result");

            assert(count >= 0);
            y_func(rows + max_ky - anchor.y, dptr, dst->step, count, this);
            row_count -= count;
            dst_y += count;
            dptr += dst->step*count;
            for(bptr = row_count > 0 ?rows[count] : 0; buf_head != bptr && buf_count > 0; buf_count--)
            {
                buf_head += buf_step;
                if(buf_head >= buf_end)
                    buf_head = buf_start;
            }
            rows_processed += count;
            top_rows = MAX(top_rows - count, 0);
        }
    }

    __END__;

    return rows_processed;
}


/****************************************************************************************\
                                    Separable Linear Filter
\****************************************************************************************/

static void icvFilterRowSymm_8u32s(const uchar* src, int* dst, void* params);
static void icvFilterColSymm_32s8u(const int** src, uchar* dst, int dst_step,
                                    int count, void* params);
static void icvFilterColSymm_32s16s(const int** src, short* dst, int dst_step,
                                     int count, void* params);
static void icvFilterRowSymm_8u32f(const uchar* src, float* dst, void* params);
static void icvFilterRow_8u32f(const uchar* src, float* dst, void* params);
static void icvFilterRowSymm_16s32f(const short* src, float* dst, void* params);
static void icvFilterRow_16s32f(const short* src, float* dst, void* params);
static void icvFilterRowSymm_16u32f(const ushort* src, float* dst, void* params);
static void icvFilterRow_16u32f(const ushort* src, float* dst, void* params);
static void icvFilterRowSymm_32f(const float* src, float* dst, void* params);
static void icvFilterRow_32f(const float* src, float* dst, void* params);

static void icvFilterColSymm_32f8u(const float** src, uchar* dst, int dst_step,
                                    int count, void* params);
static void icvFilterCol_32f8u(const float** src, uchar* dst, int dst_step,
                                int count, void* params);
static void icvFilterColSymm_32f16s(const float** src, short* dst, int dst_step,
                                     int count, void* params);
static void icvFilterCol_32f16s(const float** src, short* dst, int dst_step,
                                 int count, void* params);
static void icvFilterColSymm_32f16u(const float** src, ushort* dst, int dst_step,
                                     int count, void* params);
static void icvFilterCol_32f16u(const float** src, ushort* dst, int dst_step,
                                 int count, void* params);
static void icvFilterColSymm_32f(const float** src, float* dst, int dst_step,
                                  int count, void* params);
static void icvFilterCol_32f(const float** src, float* dst, int dst_step,
                              int count, void* params);

CSepFilter::CSepFilter()
{
    min_type = CC_32F;
    kx_flags = ky_flags = 0;
    BZERO1(kx);
    BZERO1(ky);
}


CSepFilter::CSepFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                          const img_t* _kx, const img_t* _ky,
                          CPoint _anchor, CBorderTypes _border_mode,
                          CScalar _border_value)
{
    min_type = CC_32F;
    init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _kx, _ky, _anchor, _border_mode, _border_value);
    BZERO1(kx);
    BZERO1(ky);
}


void CSepFilter::clear()
{
    imfree(kx);
    imfree(ky);
    CBaseImageFilter::clear();
}


CSepFilter::~CSepFilter()
{
    clear();
}


#undef FILTER_BITS
#define FILTER_BITS 8

void CSepFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                        const img_t* _kx, const img_t* _ky,
                        CPoint _anchor, CBorderTypes _border_mode,
                        CScalar _border_value)
{
    CC_FUNCNAME("CSepFilter::init");

    __BEGIN__;

    CSize _ksize;
    TypeId filter_type;
    int i, xsz, ysz;
    int convert_filters = 0;
    double xsum = 0, ysum = 0;
    const float eps = FLT_EPSILON*100.f;

    if(!(_kx) || !(_ky) ||
        _kx->cols != 1 && _kx->rows != 1 ||
        _ky->cols != 1 && _ky->rows != 1 ||
        CC_MAT_CN(_kx) != 1 || CC_MAT_CN(_ky) != 1 ||
        !CC_ARE_TYPES_EQ(_kx,_ky))
        CC_ERROR(CC_StsBadArg,
        "Both kernels must be valid 1d single-channel vectors of the same types");

    if(_src_cn != _dst_cn)
        CC_ERROR(CC_StsUnmatchedFormats, "Input and output must have the same number of channels");

    filter_type = MAX(CC_32F, CC_MAT_DEPTH(_kx));

    _ksize.width = _kx->rows + _kx->cols - 1;
    _ksize.height = _ky->rows + _ky->cols - 1;

    CC_CALL(CBaseImageFilter::init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, 1, _ksize,
                                      _anchor, _border_mode, _border_value));

    if(!(kx && CC_ARE_SIZES_EQ(kx,_kx)))
    {
        imfree(kx);
        CC_CALL(cvCreateMat(kx, _kx->rows, _kx->cols, filter_type));
    }

    if(!(ky && CC_ARE_SIZES_EQ(ky,_ky)))
    {
        imfree(ky);
        CC_CALL(cvCreateMat(ky, _ky->rows, _ky->cols, filter_type));
    }

    CC_CALL(cvConvert(_kx, kx));
    CC_CALL(cvConvert(_ky, ky));

    xsz = kx->rows + kx->cols - 1;
    ysz = ky->rows + ky->cols - 1;
    kx_flags = ky_flags = ASYMMETRICAL + SYMMETRICAL + POSITIVE + SUM_TO_1 + INTEGER;
    
    if(!(xsz & 1))
        kx_flags &= ~(ASYMMETRICAL + SYMMETRICAL);
    if(!(ysz & 1))
        ky_flags &= ~(ASYMMETRICAL + SYMMETRICAL);

    for(i = 0; i < xsz; i++)
    {
        float v = kx->tt.fl[i];
        xsum += v;
        if(v < 0)
            kx_flags &= ~POSITIVE;
        if(fabs(v - cRound(v)) > eps)
            kx_flags &= ~INTEGER;
        if(fabs(v - kx->tt.fl[xsz - i - 1]) > eps)
            kx_flags &= ~SYMMETRICAL;
        if(fabs(v + kx->tt.fl[xsz - i - 1]) > eps)
            kx_flags &= ~ASYMMETRICAL;
    }

    if(fabs(xsum - 1.) > eps)
        kx_flags &= ~SUM_TO_1;
    
    for(i = 0; i < ysz; i++)
    {
        float v = ky->tt.fl[i];
        ysum += v;
        if(v < 0)
            ky_flags &= ~POSITIVE;
        if(fabs(v - cRound(v)) > eps)
            ky_flags &= ~INTEGER;
        if(fabs(v - ky->tt.fl[ysz - i - 1]) > eps)
            ky_flags &= ~SYMMETRICAL;
        if(fabs(v + ky->tt.fl[ysz - i - 1]) > eps)
            ky_flags &= ~ASYMMETRICAL;
    }

    if(fabs(ysum - 1.) > eps)
        ky_flags &= ~SUM_TO_1;

    x_func = 0;
    y_func = 0;

    if(src_type == CC_8U)
    {
        if(dst_type == CC_8U &&
            ((kx_flags&ky_flags) & (SYMMETRICAL + POSITIVE + SUM_TO_1)) == SYMMETRICAL + POSITIVE + SUM_TO_1)
        {
            x_func = (CvRowFilterFunc)icvFilterRowSymm_8u32s;
            y_func = (CvColumnFilterFunc)icvFilterColSymm_32s8u;
            kx_flags &= ~INTEGER;
            ky_flags &= ~INTEGER;
            convert_filters = 1;
        }
        else if(dst_type == CC_16S &&
            (kx_flags & (SYMMETRICAL + ASYMMETRICAL)) && (kx_flags & INTEGER) &&
            (ky_flags & (SYMMETRICAL + ASYMMETRICAL)) && (ky_flags & INTEGER))
        {
            x_func = (CvRowFilterFunc)icvFilterRowSymm_8u32s;
            y_func = (CvColumnFilterFunc)icvFilterColSymm_32s16s;
            convert_filters = 1;
        }
        else
        {
            if(dst_type > CC_32F)
                CC_ERROR(CC_StsUnsupportedFormat, "8u->64f separable filtering is not supported");

            if(kx_flags & (SYMMETRICAL + ASYMMETRICAL))
                x_func = (CvRowFilterFunc)icvFilterRowSymm_8u32f;
            else
                x_func = (CvRowFilterFunc)icvFilterRow_8u32f;
        }
    }
    else if(src_type == CC_16U)
    {
        if(dst_type > CC_32F)
            CC_ERROR(CC_StsUnsupportedFormat, "16u->64f separable filtering is not supported");

        if(kx_flags & (SYMMETRICAL + ASYMMETRICAL))
            x_func = (CvRowFilterFunc)icvFilterRowSymm_16u32f;
        else
            x_func = (CvRowFilterFunc)icvFilterRow_16u32f;
    }
    else if(src_type == CC_16S)
    {
        if(dst_type > CC_32F)
            CC_ERROR(CC_StsUnsupportedFormat, "16s->64f separable filtering is not supported");

        if(kx_flags & (SYMMETRICAL + ASYMMETRICAL))
            x_func = (CvRowFilterFunc)icvFilterRowSymm_16s32f;
        else
            x_func = (CvRowFilterFunc)icvFilterRow_16s32f;
    }
    else if(src_type == CC_32F)
    {
        if(dst_type != CC_32F)
            CC_ERROR(CC_StsUnsupportedFormat, "When the input has 32f data type, the output must also have 32f type");

        if(kx_flags & (SYMMETRICAL + ASYMMETRICAL))
            x_func = (CvRowFilterFunc)icvFilterRowSymm_32f;
        else
            x_func = (CvRowFilterFunc)icvFilterRow_32f;
    }
    else
        CC_ERROR(CC_StsUnsupportedFormat, "Unknown or unsupported input data type");

    if(!y_func)
    {
        if(dst_type == CC_8U)
        {
            if(ky_flags & (SYMMETRICAL + ASYMMETRICAL))
                y_func = (CvColumnFilterFunc)icvFilterColSymm_32f8u;
            else
                y_func = (CvColumnFilterFunc)icvFilterCol_32f8u;
        }
        else if(dst_type == CC_16U)
        {
            if(ky_flags & (SYMMETRICAL + ASYMMETRICAL))
                y_func = (CvColumnFilterFunc)icvFilterColSymm_32f16u;
            else
                y_func = (CvColumnFilterFunc)icvFilterCol_32f16u;
        }
        else if(dst_type == CC_16S)
        {
            if(ky_flags & (SYMMETRICAL + ASYMMETRICAL))
                y_func = (CvColumnFilterFunc)icvFilterColSymm_32f16s;
            else
                y_func = (CvColumnFilterFunc)icvFilterCol_32f16s;
        }
        else if(dst_type == CC_32F)
        {
            if(ky_flags & (SYMMETRICAL + ASYMMETRICAL))
                y_func = (CvColumnFilterFunc)icvFilterColSymm_32f;
            else
                y_func = (CvColumnFilterFunc)icvFilterCol_32f;
        }
        else
            CC_ERROR(CC_StsUnsupportedFormat, "Unknown or unsupported input data type");
    }

    if(convert_filters)
    {
        int scale = kx_flags & ky_flags & INTEGER ? 1 : (1 << FILTER_BITS);
        int sum;
        
        for(i = sum = 0; i < xsz; i++)
        {
            int t = cRound(kx->tt.fl[i]*scale);
            kx->tt.i[i] = t;
            sum += t;
        }
        if(scale > 1)
            kx->tt.i[xsz/2] += scale - sum;

        for(i = sum = 0; i < ysz; i++)
        {
            int t = cRound(ky->tt.fl[i]*scale);
            ky->tt.i[i] = t;
            sum += t;
        }
        if(scale > 1)
            ky->tt.i[ysz/2] += scale - sum;
        kx->tid = CC_32S;
        ky->tid = CC_32S;
    }

    __END__;
}


void CSepFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                        bool _is_separable, CSize _ksize,
                        CPoint _anchor, CBorderTypes _border_mode,
                        CScalar _border_value)
{
    CBaseImageFilter::init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _is_separable,
                             _ksize, _anchor, _border_mode, _border_value);
}


static void
icvFilterRowSymm_8u32s(const uchar* src, int* dst, void* params)
{
    const CSepFilter* state = (const CSepFilter*)params;
    const img_t* _kx = state->get_x_kernel();
    const int* kx = _kx->tt.i;
    int ksize = _kx->cols + _kx->rows - 1;
    int i = 0, j, k, width = state->get_width();
    int cn = state->get_src_cn();
    int ksize2 = ksize/2, ksize2n = ksize2*cn;
    int is_symm = state->get_x_kernel_flags() & CSepFilter::SYMMETRICAL;
    const uchar* s = src + ksize2n;

    kx += ksize2;
    width *= cn;

    if(is_symm)
    {
        if(ksize == 1 && kx[0] == 1)
        {
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = s[i], s1 = s[i+1];
                dst[i] = s0; dst[i+1] = s1;
            }
            s += i;
        }
        else if(ksize == 3)
        {
            if(kx[0] == 2 && kx[1] == 1)
                for(; i <= width - 2; i += 2, s += 2)
                {
                    int s0 = s[-cn] + s[0]*2 + s[cn], s1 = s[1-cn] + s[1]*2 + s[1+cn];
                    dst[i] = s0; dst[i+1] = s1;
                }
            else if(kx[0] == 10 && kx[1] == 3)
                for(; i <= width - 2; i += 2, s += 2)
                {
                    int s0 = s[0]*10 + (s[-cn] + s[cn])*3, s1 = s[1]*10 + (s[1-cn] + s[1+cn])*3;
                    dst[i] = s0; dst[i+1] = s1;
                }
            else if(kx[0] == 2*64 && kx[1] == 1*64)
                for(; i <= width - 2; i += 2, s += 2)
                {
                    int s0 = (s[0]*2 + s[-cn] + s[cn]) << 6;
                    int s1 = (s[1]*2 + s[1-cn] + s[1+cn]) << 6;
                    dst[i] = s0; dst[i+1] = s1;
                }
            else
            {
                int k0 = kx[0], k1 = kx[1];
                for(; i <= width - 2; i += 2, s += 2)
                {
                    int s0 = s[0]*k0 + (s[-cn] + s[cn])*k1, s1 = s[1]*k0 + (s[1-cn] + s[1+cn])*k1;
                    dst[i] = s0; dst[i+1] = s1;
                }
            }
        }
        else if(ksize == 5)
        {
            int k0 = kx[0], k1 = kx[1], k2 = kx[2];
            if(k0 == 6*16 && k1 == 4*16 && k2 == 1*16)
                for(; i <= width - 2; i += 2, s += 2)
                {
                    int s0 = (s[0]*6 + (s[-cn] + s[cn])*4 + (s[-cn*2] + s[cn*2])*1) << 4;
                    int s1 = (s[1]*6 + (s[1-cn] + s[1+cn])*4 + (s[1-cn*2] + s[1+cn*2])*1) << 4;
                    dst[i] = s0; dst[i+1] = s1;
                }
            else
                for(; i <= width - 2; i += 2, s += 2)
                {
                    int s0 = s[0]*k0 + (s[-cn] + s[cn])*k1 + (s[-cn*2] + s[cn*2])*k2;
                    int s1 = s[1]*k0 + (s[1-cn] + s[1+cn])*k1 + (s[1-cn*2] + s[1+cn*2])*k2;
                    dst[i] = s0; dst[i+1] = s1;
                }
        }
        else
            for(; i <= width - 4; i += 4, s += 4)
            {
                int f = kx[0];
                int s0 = f*s[0], s1 = f*s[1], s2 = f*s[2], s3 = f*s[3];
                for(k = 1, j = cn; k <= ksize2; k++, j += cn)
                {
                    f = kx[k];
                    s0 += f*(s[j] + s[-j]); s1 += f*(s[j+1] + s[-j+1]);
                    s2 += f*(s[j+2] + s[-j+2]); s3 += f*(s[j+3] + s[-j+3]);
                }

                dst[i] = s0; dst[i+1] = s1;
                dst[i+2] = s2; dst[i+3] = s3;
            }

        for(; i < width; i++, s++)
        {
            int s0 = kx[0]*s[0];
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)
                s0 += kx[k]*(s[j] + s[-j]);
            dst[i] = s0;
        }
    }
    else
    {
        if(ksize == 3 && kx[0] == 0 && kx[1] == 1)
            for(; i <= width - 2; i += 2, s += 2)
            {
                int s0 = s[cn] - s[-cn], s1 = s[1+cn] - s[1-cn];
                dst[i] = s0; dst[i+1] = s1;
            }
        else
            for(; i <= width - 4; i += 4, s += 4)
            {
                int s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                for(k = 1, j = cn; k <= ksize2; k++, j += cn)
                {
                    int f = kx[k];
                    s0 += f*(s[j] - s[-j]); s1 += f*(s[j+1] - s[-j+1]);
                    s2 += f*(s[j+2] - s[-j+2]); s3 += f*(s[j+3] - s[-j+3]);
                }

                dst[i] = s0; dst[i+1] = s1;
                dst[i+2] = s2; dst[i+3] = s3;
            }

        for(; i < width; i++, s++)
        {
            int s0 = kx[0]*s[0];
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)
                s0 += kx[k]*(s[j] - s[-j]);
            dst[i] = s0;
        }
    }
}


#define ICC_FILTER_ROW(flavor, srctype, dsttype, load_macro)      \
static void                                                         \
icvFilterRow_##flavor(const srctype* src, dsttype* dst, void*params)\
{                                                                   \
    const CSepFilter* state = (const CSepFilter*)params;          \
    const img_t* _kx = state->get_x_kernel();                       \
    const dsttype* kx = (const dsttype*)(_kx->tt.data);             \
    int ksize = _kx->cols + _kx->rows - 1;                          \
    int i = 0, k, width = state->get_width();                       \
    int cn = state->get_src_cn();                                   \
    const srctype* s;                                               \
                                                                    \
    width *= cn;                                                    \
                                                                    \
    for(; i <= width - 4; i += 4)                                 \
    {                                                               \
        double f = kx[0];                                           \
        double s0=f*load_macro(src[i]), s1=f*load_macro(src[i+1]),  \
                s2=f*load_macro(src[i+2]), s3=f*load_macro(src[i+3]);\
        for(k = 1, s = src + i + cn; k < ksize; k++, s += cn)     \
        {                                                           \
            f = kx[k];                                              \
            s0 += f*load_macro(s[0]);                               \
            s1 += f*load_macro(s[1]);                               \
            s2 += f*load_macro(s[2]);                               \
            s3 += f*load_macro(s[3]);                               \
        }                                                           \
        dst[i] = (dsttype)s0; dst[i+1] = (dsttype)s1;               \
        dst[i+2] = (dsttype)s2; dst[i+3] = (dsttype)s3;             \
    }                                                               \
                                                                    \
    for(; i < width; i++)                                         \
    {                                                               \
        double s0 = (double)kx[0]*load_macro(src[i]);               \
        for(k = 1, s = src + i + cn; k < ksize; k++, s += cn)     \
            s0 += (double)kx[k]*load_macro(s[0]);                   \
        dst[i] = (dsttype)s0;                                       \
    }                                                               \
}


ICC_FILTER_ROW(8u32f, uchar, float, CC_8TO32F)
ICC_FILTER_ROW(16s32f, short, float, CC_NOP)
ICC_FILTER_ROW(16u32f, ushort, float, CC_NOP)
ICC_FILTER_ROW(32f, float, float, CC_NOP)


#define ICC_FILTER_ROW_SYMM(flavor, srctype, dsttype, load_macro) \
static void                                                         \
icvFilterRowSymm_##flavor(const srctype* src,                      \
                           dsttype* dst, void* params)             \
{                                                                   \
    const CSepFilter* state = (const CSepFilter*)params;          \
    const img_t* _kx = state->get_x_kernel();                       \
    const dsttype* kx = (const dsttype*)(_kx->tt.data);             \
    int ksize = _kx->cols + _kx->rows - 1;                          \
    int i = 0, j, k, width = state->get_width();                    \
    int cn = state->get_src_cn();                                   \
    int is_symm=state->get_x_kernel_flags()&CSepFilter::SYMMETRICAL;\
    int ksize2 = ksize/2, ksize2n = ksize2*cn;                      \
    const srctype* s = src + ksize2n;                               \
                                                                    \
    kx += ksize2;                                                   \
    width *= cn;                                                    \
                                                                    \
    if(is_symm)                                                   \
    {                                                               \
        for(; i <= width - 4; i += 4, s += 4)                     \
        {                                                           \
            double f = kx[0];                                       \
            double s0=f*load_macro(s[0]), s1=f*load_macro(s[1]),    \
                   s2=f*load_macro(s[2]), s3=f*load_macro(s[3]);    \
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)         \
            {                                                       \
                f = kx[k];                                          \
                s0 += f*load_macro(s[j] + s[-j]);                   \
                s1 += f*load_macro(s[j+1] + s[-j+1]);               \
                s2 += f*load_macro(s[j+2] + s[-j+2]);               \
                s3 += f*load_macro(s[j+3] + s[-j+3]);               \
            }                                                       \
                                                                    \
            dst[i] = (dsttype)s0; dst[i+1] = (dsttype)s1;           \
            dst[i+2] = (dsttype)s2; dst[i+3] = (dsttype)s3;         \
        }                                                           \
                                                                    \
        for(; i < width; i++, s++)                                \
        {                                                           \
            double s0 = (double)kx[0]*load_macro(s[0]);             \
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)         \
                s0 += (double)kx[k]*load_macro(s[j] + s[-j]);       \
            dst[i] = (dsttype)s0;                                   \
        }                                                           \
    }                                                               \
    else                                                            \
    {                                                               \
        for(; i <= width - 4; i += 4, s += 4)                     \
        {                                                           \
            double s0 = 0, s1 = 0, s2 = 0, s3 = 0;                  \
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)         \
            {                                                       \
                double f = kx[k];                                   \
                s0 += f*load_macro(s[j] - s[-j]);                   \
                s1 += f*load_macro(s[j+1] - s[-j+1]);               \
                s2 += f*load_macro(s[j+2] - s[-j+2]);               \
                s3 += f*load_macro(s[j+3] - s[-j+3]);               \
            }                                                       \
                                                                    \
            dst[i] = (dsttype)s0; dst[i+1] = (dsttype)s1;           \
            dst[i+2] = (dsttype)s2; dst[i+3] = (dsttype)s3;         \
        }                                                           \
                                                                    \
        for(; i < width; i++, s++)                                \
        {                                                           \
            double s0 = 0;                                          \
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)         \
                s0 += (double)kx[k]*load_macro(s[j] - s[-j]);       \
            dst[i] = (dsttype)s0;                                   \
        }                                                           \
    }                                                               \
}


ICC_FILTER_ROW_SYMM(8u32f, uchar, float, CC_8TO32F)
ICC_FILTER_ROW_SYMM(16s32f, short, float, CC_NOP)
ICC_FILTER_ROW_SYMM(16u32f, ushort, float, CC_NOP)

static void
icvFilterRowSymm_32f(const float* src, float* dst, void* params)
{
    const CSepFilter* state = (const CSepFilter*)params;
    const img_t* _kx = state->get_x_kernel();
    const float* kx = _kx->tt.fl;
    int ksize = _kx->cols + _kx->rows - 1;
    int i = 0, j, k, width = state->get_width();
    int cn = state->get_src_cn();
    int ksize2 = ksize/2, ksize2n = ksize2*cn;
    int is_symm = state->get_x_kernel_flags() & CSepFilter::SYMMETRICAL;
    const float* s = src + ksize2n;

    kx += ksize2;
    width *= cn;

    if(is_symm)
    {
        if(ksize == 3 && fabs(kx[0]-2.) <= FLT_EPSILON && fabs(kx[1]-1.) <= FLT_EPSILON)
            for(; i <= width - 2; i += 2, s += 2)
            {
                float s0 = s[-cn] + s[0]*2 + s[cn], s1 = s[1-cn] + s[1]*2 + s[1+cn];
                dst[i] = s0; dst[i+1] = s1;
            }
        else if(ksize == 3 && fabs(kx[0]-10.) <= FLT_EPSILON && fabs(kx[1]-3.) <= FLT_EPSILON)
            for(; i <= width - 2; i += 2, s += 2)
            {
                float s0 = s[0]*10 + (s[-cn] + s[cn])*3, s1 = s[1]*10 + (s[1-cn] + s[1+cn])*3;
                dst[i] = s0; dst[i+1] = s1;
            }
        else
            for(; i <= width - 4; i += 4, s += 4)
            {
                double f = kx[0];
                double s0 = f*s[0], s1 = f*s[1], s2 = f*s[2], s3 = f*s[3];
                for(k = 1, j = cn; k <= ksize2; k++, j += cn)
                {
                    f = kx[k];
                    s0 += f*(s[j] + s[-j]); s1 += f*(s[j+1] + s[-j+1]);
                    s2 += f*(s[j+2] + s[-j+2]); s3 += f*(s[j+3] + s[-j+3]);
                }

                dst[i] = (float)s0; dst[i+1] = (float)s1;
                dst[i+2] = (float)s2; dst[i+3] = (float)s3;
            }

        for(; i < width; i++, s++)
        {
            double s0 = (double)kx[0]*s[0];
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)
                s0 += (double)kx[k]*(s[j] + s[-j]);
            dst[i] = (float)s0;
        }
    }
    else
    {
        if(ksize == 3 && fabs(kx[0]) <= FLT_EPSILON && fabs(kx[1]-1.) <= FLT_EPSILON)
            for(; i <= width - 2; i += 2, s += 2)
            {
                float s0 = s[cn] - s[-cn], s1 = s[1+cn] - s[1-cn];
                dst[i] = s0; dst[i+1] = s1;
            }
        else
            for(; i <= width - 4; i += 4, s += 4)
            {
                double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                for(k = 1, j = cn; k <= ksize2; k++, j += cn)
                {
                    double f = kx[k];
                    s0 += f*(s[j] - s[-j]); s1 += f*(s[j+1] - s[-j+1]);
                    s2 += f*(s[j+2] - s[-j+2]); s3 += f*(s[j+3] - s[-j+3]);
                }

                dst[i] = (float)s0; dst[i+1] = (float)s1;
                dst[i+2] = (float)s2; dst[i+3] = (float)s3;
            }

        for(; i < width; i++, s++)
        {
            double s0 = (double)kx[0]*s[0];
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)
                s0 += (double)kx[k]*(s[j] - s[-j]);
            dst[i] = (float)s0;
        }
    }
}


static void
icvFilterColSymm_32s8u(const int** src, uchar* dst, int dst_step, int count, void* params)
{
    const CSepFilter* state = (const CSepFilter*)params;
    const img_t* _ky = state->get_y_kernel();
    const int* ky = _ky->tt.i;
    int ksize = _ky->cols + _ky->rows - 1, ksize2 = ksize/2;
    int i, k, width = state->get_width();
    int cn = state->get_src_cn();

    width *= cn;
    src += ksize2;
    ky += ksize2;

    for(; count--; dst += dst_step, src++)
    {
        if(ksize == 3)
        {
            const int* sptr0 = src[-1], *sptr1 = src[0], *sptr2 = src[1];
            int k0 = ky[0], k1 = ky[1];
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sptr1[i]*k0 + (sptr0[i] + sptr2[i])*k1;
                int s1 = sptr1[i+1]*k0 + (sptr0[i+1] + sptr2[i+1])*k1;
                s0 = CC_DESCALE(s0, FILTER_BITS*2);
                s1 = CC_DESCALE(s1, FILTER_BITS*2);
                dst[i] = (uchar)s0; dst[i+1] = (uchar)s1;
            }
        }
        else if(ksize == 5)
        {
            const int* sptr0 = src[-2], *sptr1 = src[-1],
                *sptr2 = src[0], *sptr3 = src[1], *sptr4 = src[2];
            int k0 = ky[0], k1 = ky[1], k2 = ky[2];
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sptr2[i]*k0 + (sptr1[i] + sptr3[i])*k1 + (sptr0[i] + sptr4[i])*k2;
                int s1 = sptr2[i+1]*k0 + (sptr1[i+1] + sptr3[i+1])*k1 + (sptr0[i+1] + sptr4[i+1])*k2;
                s0 = CC_DESCALE(s0, FILTER_BITS*2);
                s1 = CC_DESCALE(s1, FILTER_BITS*2);
                dst[i] = (uchar)s0; dst[i+1] = (uchar)s1;
            }
        }
        else
            for(i = 0; i <= width - 4; i += 4)
            {
                int f = ky[0];
                const int* sptr = src[0] + i, *sptr2;
                int s0 = f*sptr[0], s1 = f*sptr[1], s2 = f*sptr[2], s3 = f*sptr[3];
                for(k = 1; k <= ksize2; k++)
                {
                    sptr = src[k] + i;
                    sptr2 = src[-k] + i;
                    f = ky[k];
                    s0 += f*(sptr[0] + sptr2[0]);
                    s1 += f*(sptr[1] + sptr2[1]);
                    s2 += f*(sptr[2] + sptr2[2]);
                    s3 += f*(sptr[3] + sptr2[3]);
                }

                s0 = CC_DESCALE(s0, FILTER_BITS*2);
                s1 = CC_DESCALE(s1, FILTER_BITS*2);
                dst[i] = (uchar)s0; dst[i+1] = (uchar)s1;
                s2 = CC_DESCALE(s2, FILTER_BITS*2);
                s3 = CC_DESCALE(s3, FILTER_BITS*2);
                dst[i+2] = (uchar)s2; dst[i+3] = (uchar)s3;
            }

        for(; i < width; i++)
        {
            int s0 = ky[0]*src[0][i];
            for(k = 1; k <= ksize2; k++)
                s0 += ky[k]*(src[k][i] + src[-k][i]);

            s0 = CC_DESCALE(s0, FILTER_BITS*2);
            dst[i] = (uchar)s0;
        }
    }
}


static void
icvFilterColSymm_32s16s(const int** src, short* dst,
                         int dst_step, int count, void* params)
{
    const CSepFilter* state = (const CSepFilter*)params;
    const img_t* _ky = state->get_y_kernel();
    const int* ky = (const int*)_ky->tt.data;
    int ksize = _ky->cols + _ky->rows - 1, ksize2 = ksize/2;
    int i = 0, k, width = state->get_width();
    int cn = state->get_src_cn();
    int is_symm = state->get_y_kernel_flags() & CSepFilter::SYMMETRICAL;
    int is_1_2_1 = is_symm && ksize == 3 && ky[1] == 2 && ky[2] == 1;
    int is_3_10_3 = is_symm && ksize == 3 && ky[1] == 10 && ky[2] == 3;
    int is_m1_0_1 = !is_symm && ksize == 3 && ky[1] == 0 &&
        ky[2]*ky[2] == 1 ? (ky[2] > 0 ? 1 : -1) : 0;

    width *= cn;
    src += ksize2;
    ky += ksize2;
    dst_step /= sizeof(dst[0]);

    if(is_symm)
    {
        for(; count--; dst += dst_step, src++)
        {
            if(is_1_2_1)
            {
                const int *src0 = src[-1], *src1 = src[0], *src2 = src[1];

                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = src0[i] + src1[i]*2 + src2[i],
                        s1 = src0[i+1] + src1[i+1]*2 + src2[i+1];

                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }
            }
            else if(is_3_10_3)
            {
                const int *src0 = src[-1], *src1 = src[0], *src2 = src[1];

                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = src1[i]*10 + (src0[i] + src2[i])*3,
                        s1 = src1[i+1]*10 + (src0[i+1] + src2[i+1])*3;

                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }
            }
            else
                for(i = 0; i <= width - 4; i += 4)
                {
                    int f = ky[0];
                    const int* sptr = src[0] + i, *sptr2;
                    int s0 = f*sptr[0], s1 = f*sptr[1],
                        s2 = f*sptr[2], s3 = f*sptr[3];
                    for(k = 1; k <= ksize2; k++)
                    {
                        sptr = src[k] + i; sptr2 = src[-k] + i; f = ky[k];
                        s0 += f*(sptr[0] + sptr2[0]); s1 += f*(sptr[1] + sptr2[1]);
                        s2 += f*(sptr[2] + sptr2[2]); s3 += f*(sptr[3] + sptr2[3]);
                    }

                    dst[i] = CC_CAST_16S(s0); dst[i+1] = CC_CAST_16S(s1);
                    dst[i+2] = CC_CAST_16S(s2); dst[i+3] = CC_CAST_16S(s3);
                }

            for(; i < width; i++)
            {
                int s0 = ky[0]*src[0][i];
                for(k = 1; k <= ksize2; k++)
                    s0 += ky[k]*(src[k][i] + src[-k][i]);
                dst[i] = CC_CAST_16S(s0);
            }
        }
    }
    else
    {
        for(; count--; dst += dst_step, src++)
        {
            if(is_m1_0_1)
            {
                const int *src0 = src[-is_m1_0_1], *src2 = src[is_m1_0_1];

                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = src2[i] - src0[i], s1 = src2[i+1] - src0[i+1];
                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }
            }
            else
                for(i = 0; i <= width - 4; i += 4)
                {
                    int f = ky[0];
                    const int* sptr = src[0] + i, *sptr2;
                    int s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    for(k = 1; k <= ksize2; k++)
                    {
                        sptr = src[k] + i; sptr2 = src[-k] + i; f = ky[k];
                        s0 += f*(sptr[0] - sptr2[0]); s1 += f*(sptr[1] - sptr2[1]);
                        s2 += f*(sptr[2] - sptr2[2]); s3 += f*(sptr[3] - sptr2[3]);
                    }

                    dst[i] = CC_CAST_16S(s0); dst[i+1] = CC_CAST_16S(s1);
                    dst[i+2] = CC_CAST_16S(s2); dst[i+3] = CC_CAST_16S(s3);
                }

            for(; i < width; i++)
            {
                int s0 = ky[0]*src[0][i];
                for(k = 1; k <= ksize2; k++)
                    s0 += ky[k]*(src[k][i] - src[-k][i]);
                dst[i] = CC_CAST_16S(s0);
            }
        }
    }
}


#define ICC_FILTER_COL(flavor, srctype, dsttype, worktype,     \
                        cast_macro1, cast_macro2)              \
static void                                                     \
icvFilterCol_##flavor(const srctype** src, dsttype* dst,       \
                       int dst_step, int count, void* params)  \
{                                                               \
    const CSepFilter* state = (const CSepFilter*)params;      \
    const img_t* _ky = state->get_y_kernel();                   \
    const srctype* ky = (const srctype*)_ky->tt.data;          \
    int ksize = _ky->cols + _ky->rows - 1;                      \
    int i, k, width = state->get_width();                       \
    int cn = state->get_src_cn();                  \
                                                                \
    width *= cn;                                                \
    dst_step /= sizeof(dst[0]);                                 \
                                                                \
    for(; count--; dst += dst_step, src++)                    \
    {                                                           \
        for(i = 0; i <= width - 4; i += 4)                    \
        {                                                       \
            double f = ky[0];                                   \
            const srctype* sptr = src[0] + i;                   \
            double s0 = f*sptr[0], s1 = f*sptr[1],              \
                   s2 = f*sptr[2], s3 = f*sptr[3];              \
            worktype t0, t1;                                    \
            for(k = 1; k < ksize; k++)                        \
            {                                                   \
                sptr = src[k] + i; f = ky[k];                   \
                s0 += f*sptr[0]; s1 += f*sptr[1];               \
                s2 += f*sptr[2]; s3 += f*sptr[3];               \
            }                                                   \
                                                                \
            t0 = cast_macro1(s0); t1 = cast_macro1(s1);         \
            dst[i]=cast_macro2(t0); dst[i+1]=cast_macro2(t1);   \
            t0 = cast_macro1(s2); t1 = cast_macro1(s3);         \
            dst[i+2]=cast_macro2(t0); dst[i+3]=cast_macro2(t1); \
        }                                                       \
                                                                \
        for(; i < width; i++)                                 \
        {                                                       \
            double s0 = (double)ky[0]*src[0][i];                \
            worktype t0;                                        \
            for(k = 1; k < ksize; k++)                        \
                s0 += (double)ky[k]*src[k][i];                  \
            t0 = cast_macro1(s0);                               \
            dst[i] = cast_macro2(t0);                           \
        }                                                       \
    }                                                           \
}


ICC_FILTER_COL(32f8u, float, uchar, int, cRound, CC_CAST_8U)
ICC_FILTER_COL(32f16s, float, short, int, cRound, CC_CAST_16S)
ICC_FILTER_COL(32f16u, float, ushort, int, cRound, CC_CAST_16U)

#define ICC_FILTER_COL_SYMM(flavor, srctype, dsttype, worktype,    \
                             cast_macro1, cast_macro2)             \
static void                                                         \
icvFilterColSymm_##flavor(const srctype** src, dsttype* dst,       \
                           int dst_step, int count, void* params)  \
{                                                                   \
    const CSepFilter* state = (const CSepFilter*)params;          \
    const img_t* _ky = state->get_y_kernel();                       \
    const srctype* ky = (const srctype*)_ky->tt.data;              \
    int ksize = _ky->cols + _ky->rows - 1, ksize2 = ksize/2;        \
    int i, k, width = state->get_width();                           \
    int cn = state->get_src_cn();                      \
    int is_symm = state->get_y_kernel_flags() & CSepFilter::SYMMETRICAL;\
                                                                    \
    width *= cn;                                                    \
    src += ksize2;                                                  \
    ky += ksize2;                                                   \
    dst_step /= sizeof(dst[0]);                                     \
                                                                    \
    if(is_symm)                                                   \
    {                                                               \
        for(; count--; dst += dst_step, src++)                    \
        {                                                           \
            for(i = 0; i <= width - 4; i += 4)                    \
            {                                                       \
                double f = ky[0];                                   \
                const srctype* sptr = src[0] + i, *sptr2;           \
                double s0 = f*sptr[0], s1 = f*sptr[1],              \
                       s2 = f*sptr[2], s3 = f*sptr[3];              \
                worktype t0, t1;                                    \
                for(k = 1; k <= ksize2; k++)                      \
                {                                                   \
                    sptr = src[k] + i;                              \
                    sptr2 = src[-k] + i;                            \
                    f = ky[k];                                      \
                    s0 += f*(sptr[0] + sptr2[0]);                   \
                    s1 += f*(sptr[1] + sptr2[1]);                   \
                    s2 += f*(sptr[2] + sptr2[2]);                   \
                    s3 += f*(sptr[3] + sptr2[3]);                   \
                }                                                   \
                                                                    \
                t0 = cast_macro1(s0); t1 = cast_macro1(s1);         \
                dst[i]=cast_macro2(t0); dst[i+1]=cast_macro2(t1);   \
                t0 = cast_macro1(s2); t1 = cast_macro1(s3);         \
                dst[i+2]=cast_macro2(t0); dst[i+3]=cast_macro2(t1); \
            }                                                       \
                                                                    \
            for(; i < width; i++)                                 \
            {                                                       \
                double s0 = (double)ky[0]*src[0][i];                \
                worktype t0;                                        \
                for(k = 1; k <= ksize2; k++)                      \
                    s0 += (double)ky[k]*(src[k][i] + src[-k][i]);   \
                t0 = cast_macro1(s0);                               \
                dst[i] = cast_macro2(t0);                           \
            }                                                       \
        }                                                           \
    }                                                               \
    else                                                            \
    {                                                               \
        for(; count--; dst += dst_step, src++)                    \
        {                                                           \
            for(i = 0; i <= width - 4; i += 4)                    \
            {                                                       \
                double f = ky[0];                                   \
                const srctype* sptr = src[0] + i, *sptr2;           \
                double s0 = 0, s1 = 0, s2 = 0, s3 = 0;              \
                worktype t0, t1;                                    \
                for(k = 1; k <= ksize2; k++)                      \
                {                                                   \
                    sptr = src[k] + i;                              \
                    sptr2 = src[-k] + i;                            \
                    f = ky[k];                                      \
                    s0 += f*(sptr[0] - sptr2[0]);                   \
                    s1 += f*(sptr[1] - sptr2[1]);                   \
                    s2 += f*(sptr[2] - sptr2[2]);                   \
                    s3 += f*(sptr[3] - sptr2[3]);                   \
                }                                                   \
                                                                    \
                t0 = cast_macro1(s0); t1 = cast_macro1(s1);         \
                dst[i]=cast_macro2(t0); dst[i+1]=cast_macro2(t1);   \
                t0 = cast_macro1(s2); t1 = cast_macro1(s3);         \
                dst[i+2]=cast_macro2(t0); dst[i+3]=cast_macro2(t1); \
            }                                                       \
                                                                    \
            for(; i < width; i++)                                 \
            {                                                       \
                double s0 = (double)ky[0]*src[0][i];                \
                worktype t0;                                        \
                for(k = 1; k <= ksize2; k++)                      \
                    s0 += (double)ky[k]*(src[k][i] - src[-k][i]);   \
                t0 = cast_macro1(s0);                               \
                dst[i] = cast_macro2(t0);                           \
            }                                                       \
        }                                                           \
    }                                                               \
}


ICC_FILTER_COL_SYMM(32f8u, float, uchar, int, cRound, CC_CAST_8U)
ICC_FILTER_COL_SYMM(32f16s, float, short, int, cRound, CC_CAST_16S)
ICC_FILTER_COL_SYMM(32f16u, float, ushort, int, cRound, CC_CAST_16U)


static void
icvFilterCol_32f(const float** src, float* dst,
                  int dst_step, int count, void* params)
{
    const CSepFilter* state = (const CSepFilter*)params;
    const img_t* _ky = state->get_y_kernel();
    const float* ky = (const float*)_ky->tt.data;
    int ksize = _ky->cols + _ky->rows - 1;
    int i, k, width = state->get_width();
    int cn = state->get_src_cn();

    width *= cn;
    dst_step /= sizeof(dst[0]);

    for(; count--; dst += dst_step, src++)
    {
        for(i = 0; i <= width - 4; i += 4)
        {
            double f = ky[0];
            const float* sptr = src[0] + i;
            double s0 = f*sptr[0], s1 = f*sptr[1],
                   s2 = f*sptr[2], s3 = f*sptr[3];
            for(k = 1; k < ksize; k++)
            {
                sptr = src[k] + i; f = ky[k];
                s0 += f*sptr[0]; s1 += f*sptr[1];
                s2 += f*sptr[2]; s3 += f*sptr[3];
            }

            dst[i] = (float)s0; dst[i+1] = (float)s1;
            dst[i+2] = (float)s2; dst[i+3] = (float)s3;
        }

        for(; i < width; i++)
        {
            double s0 = (double)ky[0]*src[0][i];
            for(k = 1; k < ksize; k++)
                s0 += (double)ky[k]*src[k][i];
            dst[i] = (float)s0;
        }
    }
}


static void
icvFilterColSymm_32f(const float** src, float* dst,
                      int dst_step, int count, void* params)
{
    const CSepFilter* state = (const CSepFilter*)params;
    const img_t* _ky = state->get_y_kernel();
    const float* ky = (const float*)_ky->tt.data;
    int ksize = _ky->cols + _ky->rows - 1, ksize2 = ksize/2;
    int i = 0, k, width = state->get_width();
    int cn = state->get_src_cn();
    int is_symm = state->get_y_kernel_flags() & CSepFilter::SYMMETRICAL;
    int is_1_2_1 = is_symm && ksize == 3 &&
        fabs(ky[1] - 2.) <= FLT_EPSILON && fabs(ky[2] - 1.) <= FLT_EPSILON;
    int is_3_10_3 = is_symm && ksize == 3 &&
            fabs(ky[1] - 10.) <= FLT_EPSILON && fabs(ky[2] - 3.) <= FLT_EPSILON;
    int is_m1_0_1 = !is_symm && ksize == 3 &&
            fabs(ky[1]) <= FLT_EPSILON && fabs(ky[2]*ky[2] - 1.) <= FLT_EPSILON ?
            (ky[2] > 0 ? 1 : -1) : 0;

    width *= cn;
    src += ksize2;
    ky += ksize2;
    dst_step /= sizeof(dst[0]);

    if(is_symm)
    {
        for(; count--; dst += dst_step, src++)
        {
            if(is_1_2_1)
            {
                const float *src0 = src[-1], *src1 = src[0], *src2 = src[1];

                for(i = 0; i <= width - 4; i += 4)
                {
                    float s0 = src0[i] + src1[i]*2 + src2[i],
                          s1 = src0[i+1] + src1[i+1]*2 + src2[i+1],
                          s2 = src0[i+2] + src1[i+2]*2 + src2[i+2],
                          s3 = src0[i+3] + src1[i+3]*2 + src2[i+3];

                    dst[i] = s0; dst[i+1] = s1;
                    dst[i+2] = s2; dst[i+3] = s3;
                }
            }
            else if(is_3_10_3)
            {
                const float *src0 = src[-1], *src1 = src[0], *src2 = src[1];

                for(i = 0; i <= width - 4; i += 4)
                {
                    float s0 = src1[i]*10 + (src0[i] + src2[i])*3,
                          s1 = src1[i+1]*10 + (src0[i+1] + src2[i+1])*3,
                          s2 = src1[i+2]*10 + (src0[i+2] + src2[i+2])*3,
                          s3 = src1[i+3]*10 + (src0[i+3] + src2[i+3])*3;

                    dst[i] = s0; dst[i+1] = s1;
                    dst[i+2] = s2; dst[i+3] = s3;
                }
            }
            else
                for(i = 0; i <= width - 4; i += 4)
                {
                    double f = ky[0];
                    const float* sptr = src[0] + i, *sptr2;
                    double s0 = f*sptr[0], s1 = f*sptr[1],
                           s2 = f*sptr[2], s3 = f*sptr[3];
                    for(k = 1; k <= ksize2; k++)
                    {
                        sptr = src[k] + i; sptr2 = src[-k] + i; f = ky[k];
                        s0 += f*(sptr[0] + sptr2[0]); s1 += f*(sptr[1] + sptr2[1]);
                        s2 += f*(sptr[2] + sptr2[2]); s3 += f*(sptr[3] + sptr2[3]);
                    }

                    dst[i] = (float)s0; dst[i+1] = (float)s1;
                    dst[i+2] = (float)s2; dst[i+3] = (float)s3;
                }

            for(; i < width; i++)
            {
                double s0 = (double)ky[0]*src[0][i];
                for(k = 1; k <= ksize2; k++)
                    s0 += (double)ky[k]*(src[k][i] + src[-k][i]);
                dst[i] = (float)s0;
            }
        }
    }
    else
    {
        for(; count--; dst += dst_step, src++)
        {
            if(is_m1_0_1)
            {
                const float *src0 = src[-is_m1_0_1], *src2 = src[is_m1_0_1];

                for(i = 0; i <= width - 4; i += 4)
                {
                    float s0 = src2[i] - src0[i], s1 = src2[i+1] - src0[i+1],
                          s2 = src2[i+2] - src0[i+2], s3 = src2[i+3] - src0[i+3];
                    dst[i] = s0; dst[i+1] = s1;
                    dst[i+2] = s2; dst[i+3] = s3;
                }
            }
            else
                for(i = 0; i <= width - 4; i += 4)
                {
                    double f = ky[0];
                    const float* sptr = src[0] + i, *sptr2;
                    double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                    for(k = 1; k <= ksize2; k++)
                    {
                        sptr = src[k] + i; sptr2 = src[-k] + i; f = ky[k];
                        s0 += f*(sptr[0] - sptr2[0]); s1 += f*(sptr[1] - sptr2[1]);
                        s2 += f*(sptr[2] - sptr2[2]); s3 += f*(sptr[3] - sptr2[3]);
                    }

                    dst[i] = (float)s0; dst[i+1] = (float)s1;
                    dst[i+2] = (float)s2; dst[i+3] = (float)s3;
                }

            for(; i < width; i++)
            {
                double s0 = (double)ky[0]*src[0][i];
                for(k = 1; k <= ksize2; k++)
                    s0 += (double)ky[k]*(src[k][i] - src[-k][i]);
                dst[i] = (float)s0;
            }
        }
    }
}


#define SMALL_GAUSSIAN_SIZE  7

void CSepFilter::init_gaussian_kernel(img_t* kernel, double sigma)
{
    static const float small_gaussian_tab[][SMALL_GAUSSIAN_SIZE/2+1] =
    {
        {1.f},
        {0.5f, 0.25f},
        {0.375f, 0.25f, 0.0625f},
        {0.28125f, 0.21875f, 0.109375f, 0.03125f}
    };

    CC_FUNCNAME("CSepFilter::init_gaussian_kernel");

    __BEGIN__;

    TypeId type;
    int i, n, step, cn = CC_MAT_CN(kernel);
    const float* fixed_kernel = 0;
    double sigmaX, scale2X, sum;
    float* cf;
    double* cd;

    if(!(kernel))
        CC_ERROR(CC_StsBadArg, "kernel is not a valid matrix");

    type = CC_MAT_DEPTH(kernel);
    
    if(kernel->cols != 1 && kernel->rows != 1 ||
        (kernel->cols + kernel->rows - 1) % 2 == 0 ||
        type != CC_32F && type != CC_64F)
        CC_ERROR(CC_StsBadSize, "kernel should be 1D floating-point vector of odd (2*k+1) size");

    n = kernel->cols + kernel->rows - 1;

    if(n <= SMALL_GAUSSIAN_SIZE && sigma <= 0)
        fixed_kernel = small_gaussian_tab[n>>1];

    sigmaX = sigma > 0 ? sigma : (n/2 - 1)*0.3 + 0.8;
    scale2X = -0.5/(sigmaX*sigmaX);
    step = kernel->rows == 1 ? 1 : kernel->step/CC_ELEMTYPE_SIZE(kernel);
    cf = kernel->tt.fl;
    cd = kernel->tt.db;

    sum = fixed_kernel ? -fixed_kernel[0] : -1.;

    for(i = 0; i <= n/2; i++)
    {
        double t = fixed_kernel ? (double)fixed_kernel[i] : exp(scale2X*i*i);
        if(type == CC_32F && cn==1)
        {
            cf[(n/2+i)*step] = (float)t;
            sum += cf[(n/2+i)*step]*2;
        }
        else
        {
            cd[(n/2+i)*step] = t;
            sum += cd[(n/2+i)*step]*2;
        }
    }

    sum = 1./sum;
    for(i = 0; i <= n/2; i++)
    {
        if(type == CC_32F && cn==1)
            cf[(n/2+i)*step] = cf[(n/2-i)*step] = (float)(cf[(n/2+i)*step]*sum);
        else
            cd[(n/2+i)*step] = cd[(n/2-i)*step] = cd[(n/2+i)*step]*sum;
    }

    __END__;
}


void CSepFilter::init_sobel_kernel(img_t* _kx, img_t* _ky, int dx, int dy, int flags)
{
    CC_FUNCNAME("CSepFilter::init_sobel_kernel");

    __BEGIN__;

    int i, j, k, msz;
    int* kerI;
    bool normalize = (flags & NORMALIZE_KERNEL) != 0;
    bool flip = (flags & FLIP_KERNEL) != 0;

    if(!(_kx) || !(_ky))
        CC_ERROR(CC_StsBadArg, "One of the kernel matrices is not valid");

    msz = MAX(_kx->cols + _kx->rows, _ky->cols + _ky->rows);
    if(msz > 32)
        CC_ERROR(CC_StsOutOfRange, "Too large kernel size");

    kerI = (int*)cvStackAlloc(msz*sizeof(kerI[0]));

    if(dx < 0 || dy < 0 || dx+dy <= 0)
        CC_ERROR(CC_StsOutOfRange,
            "Both derivative orders (dx and dy) must be non-negative "
            "and at least one of them must be positive.");

    for(k = 0; k < 2; k++)
    {
        img_t* kernel = k == 0 ? _kx : _ky;
        int order = k == 0 ? dx : dy;
        int n = kernel->cols + kernel->rows - 1, step;
        TypeId type = CC_MAT_DEPTH(kernel);
        double scale = !normalize ? 1. : 1./(1 << (n-order-1));
        int iscale = 1;

        if(kernel->cols != 1 && kernel->rows != 1 ||
            (kernel->cols + kernel->rows - 1) % 2 == 0 ||
            type != CC_32F && type != CC_64F && type != CC_32S)
            CC_ERROR(CC_StsOutOfRange,
            "Both kernels must be 1D floating-point or integer vectors of odd (2*k+1) size.");

        if(normalize && n > 1 && type == CC_32S)
            CC_ERROR(CC_StsBadArg, "Integer kernel can not be normalized");

        if(n <= order)
            CC_ERROR(CC_StsOutOfRange,
            "Derivative order must be smaller than the corresponding kernel size");

        if(n == 1)
            kerI[0] = 1;
        else if(n == 3)
        {
            if(order == 0)
                kerI[0] = 1, kerI[1] = 2, kerI[2] = 1;
            else if(order == 1)
                kerI[0] = -1, kerI[1] = 0, kerI[2] = 1;
            else
                kerI[0] = 1, kerI[1] = -2, kerI[2] = 1;
        }
        else
        {
            int oldval, newval;
            kerI[0] = 1;
            for(i = 0; i < n; i++)
                kerI[i+1] = 0;

            for(i = 0; i < n - order - 1; i++)
            {
                oldval = kerI[0];
                for(j = 1; j <= n; j++)
                {
                    newval = kerI[j]+kerI[j-1];
                    kerI[j-1] = oldval;
                    oldval = newval;
                }
            }

            for(i = 0; i < order; i++)
            {
                oldval = -kerI[0];
                for(j = 1; j <= n; j++)
                {
                    newval = kerI[j-1] - kerI[j];
                    kerI[j-1] = oldval;
                    oldval = newval;
                }
            }
        }

        step = kernel->rows == 1 ? 1 : kernel->step/CC_ELEMTYPE_SIZE(kernel);
        if(flip && (order & 1) && k)
            iscale = -iscale, scale = -scale;

        for(i = 0; i < n; i++)
        {
            if(type == CC_32S)
                kernel->tt.i[i*step] = kerI[i]*iscale;
            else if(type == CC_32F)
                kernel->tt.fl[i*step] = (float)(kerI[i]*scale);
            else
                kernel->tt.db[i*step] = kerI[i]*scale;
        }
    }

    __END__;
}


void CSepFilter::init_scharr_kernel(img_t* _kx, img_t* _ky, int dx, int dy, int flags)
{
    CC_FUNCNAME("CSepFilter::init_scharr_kernel");

    __BEGIN__;

    int i, k;
    int kerI[3];
    bool normalize = (flags & NORMALIZE_KERNEL) != 0;
    bool flip = (flags & FLIP_KERNEL) != 0;

    if(!(_kx) || !(_ky))
        CC_ERROR(CC_StsBadArg, "One of the kernel matrices is not valid");

    if(((dx|dy)&~1) || dx+dy != 1)
        CC_ERROR(CC_StsOutOfRange,
            "Scharr kernel can only be used for 1st order derivatives");

    for(k = 0; k < 2; k++)
    {
        img_t* kernel = k == 0 ? _kx : _ky;
        int order = k == 0 ? dx : dy;
        int n = kernel->cols + kernel->rows - 1, step;
        TypeId type = CC_MAT_DEPTH(kernel);
        double scale = !normalize ? 1. : order == 0 ? 1./16 : 1./2;
        int iscale = 1;

        if(kernel->cols != 1 && kernel->rows != 1 ||
            kernel->cols + kernel->rows - 1 != 3 ||
            type != CC_32F && type != CC_64F && type != CC_32S)
            CC_ERROR(CC_StsOutOfRange,
            "Both kernels must be 1D floating-point or integer vectors containing 3 elements each.");

        if(normalize && type == CC_32S)
            CC_ERROR(CC_StsBadArg, "Integer kernel can not be normalized");

        if(order == 0)
            kerI[0] = 3, kerI[1] = 10, kerI[2] = 3;
        else
            kerI[0] = -1, kerI[1] = 0, kerI[2] = 1;

        step = kernel->rows == 1 ? 1 : kernel->step/CC_ELEMTYPE_SIZE(kernel);
        if(flip && (order & 1) && k)
            iscale = -iscale, scale = -scale;

        for(i = 0; i < n; i++)
        {
            if(type == CC_32S)
                kernel->tt.i[i*step] = kerI[i]*iscale;
            else if(type == CC_32F)
                kernel->tt.fl[i*step] = (float)(kerI[i]*scale);
            else
                kernel->tt.db[i*step] = kerI[i]*scale;
        }
    }

    __END__;
}


void CSepFilter::init_deriv(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                              int dx, int dy, int aperture_size, int flags)
{
    CC_FUNCNAME("CSepFilter::init_deriv");

    __BEGIN__;

    int kx_size = aperture_size == CC_SCHARR ? 3 : aperture_size, ky_size = kx_size;
    float kx_data[CC_MAX_SOBEL_KSIZE], ky_data[CC_MAX_SOBEL_KSIZE];
    img_t _kx, _ky;

    if(kx_size <= 0 || ky_size > CC_MAX_SOBEL_KSIZE)
        CC_ERROR(CC_StsOutOfRange, "Incorrect aperture_size");

    if(kx_size == 1 && dx)
        kx_size = 3;
    if(ky_size == 1 && dy)
        ky_size = 3;

    cvMat(&_kx, 1, kx_size, CC_32F, 1, kx_data);
    cvMat(&_ky, 1, ky_size, CC_32F, 1, ky_data);

    if(aperture_size == CC_SCHARR)
    {
        CC_CALL(init_scharr_kernel(&_kx, &_ky, dx, dy, flags));
    }
    else
    {
        CC_CALL(init_sobel_kernel(&_kx, &_ky, dx, dy, flags));
    }

    CC_CALL(init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, &_kx, &_ky));

    __END__;
}


void CSepFilter::init_gaussian(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                                 int gaussian_size, double sigma)
{
    float* kdata = 0;
    
    CC_FUNCNAME("CSepFilter::init_gaussian");

    __BEGIN__;

    img_t _kernel;

    if(gaussian_size <= 0 || gaussian_size > 1024)
        CC_ERROR(CC_StsBadSize, "Incorrect size of gaussian kernel");

    kdata = (float*)cvStackAlloc(gaussian_size*sizeof(kdata[0]));
    cvMat(&_kernel, 1, gaussian_size, CC_32F, 1, kdata);

    CC_CALL(init_gaussian_kernel(&_kernel, sigma));
    CC_CALL(init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, &_kernel, &_kernel)); 

    __END__;
}


/****************************************************************************************\
                              Non-separable Linear Filter
\****************************************************************************************/

static void icvLinearFilter_8u(const uchar** src, uchar* dst, int dst_step,
                                int count, void* params);
static void icvLinearFilter_16s(const short** src, short* dst, int dst_step,
                                 int count, void* params);
static void icvLinearFilter_16u(const ushort** src, ushort* dst, int dst_step,
                                 int count, void* params);
static void icvLinearFilter_32f(const float** src, float* dst, int dst_step,
                                 int count, void* params);

CvLinearFilter::CvLinearFilter()
{
    BZERO1(kernel);
    k_sparse = 0;
}

CvLinearFilter::CvLinearFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                                const img_t* _kernel, CPoint _anchor,
                                CBorderTypes _border_mode, CScalar _border_value)
{
    BZERO1(kernel);
    k_sparse = 0;
    init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _kernel,
          _anchor, _border_mode, _border_value);
}


void CvLinearFilter::clear()
{
    imfree(kernel);
    cFree(&k_sparse);
    CBaseImageFilter::clear();
}


CvLinearFilter::~CvLinearFilter()
{
    clear();
}


void CvLinearFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                           const img_t* _kernel, CPoint _anchor,
                           CBorderTypes _border_mode, CScalar _border_value)
{
    CC_FUNCNAME("CvLinearFilter::init");

    __BEGIN__;

    TypeId type = _src_type;
    int cn = _src_cn;
    CPoint* nz_loc;
    float* coeffs;
    int i, j, k = 0;

    if(!(_kernel))
        CC_ERROR(CC_StsBadArg, "kernel is not valid matrix");

    src_type = _src_type;
    dst_type = _dst_type;

    if(_src_type != _dst_type)
        CC_ERROR(CC_StsUnmatchedFormats,
        "The source and destination image types must be the same");

    CC_CALL(CBaseImageFilter::init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn,
        false, cvGetMatSize(_kernel), _anchor, _border_mode, _border_value));

    if(!(kernel->tt.data && k_sparse && ksize.width == kernel->cols && ksize.height == kernel->rows))
    {
        imfree(kernel);
        cFree(&k_sparse);
        CC_CALL(cvCreateMat(kernel, ksize.height, ksize.width, CC_32F, 1));
        CC_CALL(k_sparse = (uchar*)cAlloc(
            ksize.width*ksize.height*(2*sizeof(int) + sizeof(uchar*) + sizeof(float))));
    }
    
    CC_CALL(cvConvert(_kernel, kernel));
    
    nz_loc = (CPoint*)k_sparse;
    for(i = 0; i < ksize.height; i++)
    {
        for(j = 0; j < ksize.width; j++)
            if(fabs(((float*)(kernel->tt.data + i*kernel->step))[j])>FLT_EPSILON)
                nz_loc[k++] = cPoint(j,i);
    }
    if(k == 0)
        nz_loc[k++] = anchor;
    k_sparse_count = k;
    coeffs = (float*)((uchar**)(nz_loc + k_sparse_count) + k_sparse_count);

    for(k = 0; k < k_sparse_count; k++)
    {
        coeffs[k] = CC_MAT_ELEM(kernel, float, nz_loc[k].y, nz_loc[k].x);
        nz_loc[k].x *= cn;
    }

    x_func = 0;
    if(type == CC_8U)
        y_func = (CvColumnFilterFunc)icvLinearFilter_8u;
    else if(type == CC_16S)
        y_func = (CvColumnFilterFunc)icvLinearFilter_16s;
    else if(type == CC_16U)
        y_func = (CvColumnFilterFunc)icvLinearFilter_16u;
    else if(type == CC_32F)
        y_func = (CvColumnFilterFunc)icvLinearFilter_32f;
    else
        CC_ERROR(CC_StsUnsupportedFormat, "Unsupported image type");

    __END__;
}


void CvLinearFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                           bool _is_separable, CSize _ksize,
                           CPoint _anchor, CBorderTypes _border_mode,
                           CScalar _border_value)
{
    CBaseImageFilter::init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _is_separable,
                             _ksize, _anchor, _border_mode, _border_value);
}


#define ICC_FILTER(flavor, arrtype, worktype, load_macro,          \
                    cast_macro1, cast_macro2)                      \
static void                                                         \
icvLinearFilter_##flavor(const arrtype** src, arrtype* dst,        \
                    int dst_step, int count, void* params)         \
{                                                                   \
    CvLinearFilter* state = (CvLinearFilter*)params;                \
    int width = state->get_width();                                 \
    int cn = state->get_src_cn();                      \
    int i, k;                                                       \
    CPoint* k_sparse = (CPoint*)state->get_kernel_sparse_buf();   \
    int k_count = state->get_kernel_sparse_count();                 \
    const arrtype** k_ptr = (const arrtype**)(k_sparse + k_count);  \
    const arrtype** k_end = k_ptr + k_count;                        \
    const float* k_coeffs = (const float*)(k_ptr + k_count);        \
                                                                    \
    width *= cn;                                                    \
    dst_step /= sizeof(dst[0]);                                     \
                                                                    \
    for(; count > 0; count--, dst += dst_step, src++)             \
    {                                                               \
        for(k = 0; k < k_count; k++)                              \
            k_ptr[k] = src[k_sparse[k].y] + k_sparse[k].x;          \
                                                                    \
        for(i = 0; i <= width - 4; i += 4)                        \
        {                                                           \
            const arrtype** kp = k_ptr;                             \
            const float* kc = k_coeffs;                             \
            double s0 = 0, s1 = 0, s2 = 0, s3 = 0;                  \
            worktype t0, t1;                                        \
                                                                    \
            while(kp != k_end)                                    \
            {                                                       \
                const arrtype* sptr = (*kp++) + i;                  \
                float f = *kc++;                                    \
                s0 += f*load_macro(sptr[0]);                        \
                s1 += f*load_macro(sptr[1]);                        \
                s2 += f*load_macro(sptr[2]);                        \
                s3 += f*load_macro(sptr[3]);                        \
            }                                                       \
                                                                    \
            t0 = cast_macro1(s0); t1 = cast_macro1(s1);             \
            dst[i] = cast_macro2(t0);                               \
            dst[i+1] = cast_macro2(t1);                             \
            t0 = cast_macro1(s2); t1 = cast_macro1(s3);             \
            dst[i+2] = cast_macro2(t0);                             \
            dst[i+3] = cast_macro2(t1);                             \
        }                                                           \
                                                                    \
        for(; i < width; i++)                                     \
        {                                                           \
            const arrtype** kp = k_ptr;                             \
            const float* kc = k_coeffs;                             \
            double s0 = 0;                                          \
            worktype t0;                                            \
                                                                    \
            while(kp != k_end)                                    \
            {                                                       \
                const arrtype* sptr = *kp++;                        \
                float f = *kc++;                                    \
                s0 += f*load_macro(sptr[i]);                        \
            }                                                       \
                                                                    \
            t0 = cast_macro1(s0);                                   \
            dst[i] = cast_macro2(t0);                               \
        }                                                           \
    }                                                               \
}


ICC_FILTER(8u, uchar, int, CC_8TO32F, cRound, CC_CAST_8U)
ICC_FILTER(16u, ushort, int, CC_NOP, cRound, CC_CAST_16U)
ICC_FILTER(16s, short, int, CC_NOP, cRound, CC_CAST_16S)
ICC_FILTER(32f, float, float, CC_NOP, CC_CAST_32F, CC_NOP)


/////////////////////// common functions for working with IPP filters ////////////////////

static img_t* icvIPPFilterInit(img_t* arr, const img_t* src, int stripe_size, CSize ksize)
{
    CSize temp_size;
    int pix_size = CC_ELEMTYPE_SIZE(src);
    temp_size.width = cvAlign(src->cols + ksize.width - 1,8/CC_ELEMTYPE_SIZE(src));
    //temp_size.width = src->cols + ksize.width - 1;
    temp_size.height = (stripe_size*2 + temp_size.width*pix_size) / (temp_size.width*pix_size*2);
    temp_size.height = MAX(temp_size.height, ksize.height);
    temp_size.height = MIN(temp_size.height, src->rows + ksize.height - 1);
    
    return cvCreateMat(arr, temp_size.height, temp_size.width, src->tid);
}


static int icvIPPFilterNextStripe(const img_t* src, img_t* temp, int y,
                            CSize ksize, CPoint anchor)
{
    int pix_size = CC_ELEMTYPE_SIZE(src);
    int src_step = src->step ? src->step : CC_STUB_STEP;
    int temp_step = temp->step ? temp->step : CC_STUB_STEP;
    int i, dy, src_y1 = 0, src_y2;
    int temp_rows;
    uchar* temp_ptr = temp->tt.data;
    CSize stripe_size, temp_size;

    dy = MIN(temp->rows - ksize.height + 1, src->rows - y);
    if(y > 0)
    {
        int temp_ready = ksize.height - 1;
        
        for(i = 0; i < temp_ready; i++)
            memcpy(temp_ptr + temp_step*i, temp_ptr +
                    temp_step*(temp->rows - temp_ready + i), temp_step);

        temp_ptr += temp_ready*temp_step;
        temp_rows = dy;
        src_y1 = y + temp_ready - anchor.y;
        src_y2 = src_y1 + dy;
        if(src_y1 >= src->rows)
        {
            src_y1 = src->rows - 1;
            src_y2 = src->rows;
        }
    }
    else
    {
        temp_rows = dy + ksize.height - 1;
        src_y2 = temp_rows - anchor.y;
    }

    src_y2 = MIN(src_y2, src->rows);

    stripe_size = cSize(src->cols, src_y2 - src_y1);
    temp_size = cSize(temp->cols, temp_rows);
    icvCopyReplicateBorder_8u(src->tt.data + src_y1*src_step, src_step,
                      stripe_size, temp_ptr, temp_step, temp_size,
                      (y == 0 ? anchor.y : 0), anchor.x, pix_size);
    return dy;
}

//////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL void
cvFilter2D(const img_t* src, img_t* dst, const img_t* kernel, CPoint anchor CC_DEFAULT(cPoint(-1,-1)))
{
    const int dft_filter_size = 100;

    CvLinearFilter filter;
    img_t ipp_kernel[1] = {0};
    
    // below that approximate size OpenCV is faster
    const int ipp_lower_limit = 20;
    img_t temp[1] = {0};

    CC_FUNCNAME("cvFilter2D");

    __BEGIN__;

    int coi1 = 0, coi2 = 0;
    TypeId type;
    int cn;

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    type = CC_MAT_DEPTH(src);
    cn = CC_MAT_CN(src);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(kernel->cols*kernel->rows >= dft_filter_size &&
        kernel->cols <= src->cols && kernel->rows <= src->rows)
    {
        if(src->tt.data == dst->tt.data)
        {
            cvCloneMat(temp, src);
            src = temp;
        }
        icvCrossCorr(src, kernel, dst, anchor);
        EXIT;
    }

    CC_CALL(filter.init(src->cols, type, cn, type, cn, kernel, anchor));
    CC_CALL(filter.process(src, dst));

    __END__;

    imfree(temp);
    imfree(ipp_kernel);
}

CC_IMPL void
cvSepFilter2D( const img_t* src, img_t* dst, TypeId ddepth,
             const img_t* kernelX, const img_t* kernelY,
             CPoint anchor = cPoint(-1,-1),
             double delta = 0, CBorderTypes _border_mode = CC_BORDER_DEFAULT )
{
    CSepFilter sep_filter;

    CC_FUNCNAME("cvSepFilter2D");

    __BEGIN__;

    int cn = CC_MAT_CN(src);
    TypeId src_type = CC_MAT_DEPTH(src);
    TypeId dst_type = ddepth;
    const img_t* KX = kernelX;
    const img_t* KY = kernelY;

    CC_CALL(sep_filter.init(src->cols, src_type, cn, dst_type, cn, KX, KY, anchor, _border_mode));
    CC_CALL(sep_filter.process(src, dst));

    __END__;
}

