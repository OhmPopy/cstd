

/* ////////////////////////////////////////////////////////////////////
//
//  Arithmetic and logical operations: +, -, *, /, &, |, ^, ~, abs ...
//
// */


/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

void convertAndUnrollScalar( const img_t* sc, int buftype, uchar* scbuf, size_t blocksize )
{
    int scn = (int)CC_MAT_TOTAL(sc), cn = CC_MAT_CN(buftype);
    size_t esz = CC_ELEM_SIZE(buftype);
    getConvertFunc(sc CC_MAT_DEPTH(), buftype)(sc->tt.data, 1, 0, 1, scbuf, 1, Size(MIN(cn, scn), 1), 0);
    // unroll the scalar
    if( scn < cn )
    {
        CC_Assert( scn == 1 );
        size_t esz1 = CC_ELEM_SIZE1(buftype);
        for( size_t i = esz1; i < esz; i++ )
            scbuf[i] = scbuf[i - esz1];
    }
    for( size_t i = esz; i < blocksize*esz; i++ )
        scbuf[i] = scbuf[i - esz];
}


enum { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

#ifdef HAVE_OPENCL

static const char* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

static bool ocl_binary_op(const img_t* _src1, const img_t* _src2, img_t* _dst,
                          const img_t* _mask, bool bitwise, int oclop, bool haveScalar )
{
    bool haveMask = !_mask.empty();
    int srctype = _src1 CC_MAT_TYPE();
    int srcdepth = CC_MAT_DEPTH(srctype);
    int cn = CC_MAT_CN(srctype);

    const ocl::Device d = ocl::Device::getDefault();
    bool doubleSupport = d.doubleFPConfig() > 0;
    if( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CC_64F && !bitwise))
        return false;

    char opts[1024];
    int kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    int scalarcn = kercn == 3 ? 4 : kercn;
    int rowsPerWI = d.isIntel() ? 4 : 1;

    sprintf(opts, "-D %s%s -D %s -D dstT=%s%s -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            bitwise ? ocl::memopTypeToStr(CC_MAKETYPE(srcdepth, kercn)) :
                ocl::typeToStr(CC_MAKETYPE(srcdepth, kercn)), doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(CC_MAKETYPE(srcdepth, 1)) :
                ocl::typeToStr(CC_MAKETYPE(srcdepth, 1)),
            bitwise ? ocl::memopTypeToStr(CC_MAKETYPE(srcdepth, scalarcn)) :
                ocl::typeToStr(CC_MAKETYPE(srcdepth, scalarcn)),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    if (k.empty())
        return false;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    if( haveScalar )
    {
        size_t esz = CC_ELEM_SIZE1(srctype)*scalarcn;
        double buf[4] = {0,0,0,0};

        if( oclop != OCL_OP_NOT )
        {
            img_t src2sc = _src2;
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(0, 0, 0, 0, buf, esz);

        if( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        else
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    else
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        if( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        else
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1->cols * cn / kercn, ((size_t)src1->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, 0, false);
}

#endif

static void binary_op( const img_t* _src1, const img_t* _src2, img_t* _dst,
                       const img_t* _mask, const BinaryFuncC* tab,
                       bool bitwise, int oclop )
{
    const _InputArray *psrc1 = &_src1, *psrc2 = &_src2;
    int kind1 = psrc1->kind(), kind2 = psrc2->kind();
    int type1 = psrc1->tid(), depth1 = CC_MAT_DEPTH(type1), cn = CC_MAT_CN(type1);
    int type2 = psrc2->tid(), depth2 = CC_MAT_DEPTH(type2), cn2 = CC_MAT_CN(type2);
    int dims1 = psrc1->dims(), dims2 = psrc2->dims();
    Size sz1 = dims1 <= 2 ? psrc1->size() : Size();
    Size sz2 = dims2 <= 2 ? psrc2->size() : Size();
#ifdef HAVE_OPENCL
    bool use_opencl = (kind1 == _InputArray::UMAT || kind2 == _InputArray::UMAT) &&
            dims1 <= 2 && dims2 <= 2;
#endif
    bool haveMask = !_mask.empty(), haveScalar = false;
    BinaryFuncC func;

    if( dims1 <= 2 && dims2 <= 2 && kind1 == kind2 && sz1 == sz2 && type1 == type2 && !haveMask )
    {
        _dst cvSetMat(sz1, type1);
        CC_OCL_RUN(use_opencl,
                   ocl_binary_op(*psrc1, *psrc2, _dst, _mask, bitwise, oclop, false))

        if( bitwise )
        {
            func = *tab;
            cn = (int)CC_ELEM_SIZE(type1);
        }
        else
            func = tab[depth1];

        img_t src1 = psrc1->getMat(), src2 = psrc2->getMat(), dst = _dst;
        Size sz = getContinuousSize(src1, src2, dst);
        size_t len = sz.width*(size_t)cn;
        if( len == (size_t)(int)len )
        {
            sz.width = (int)len;
            func(src1->tt.data, src1->step, src2->tt.data, src2->step, dst->tt.data, dst->step, sz.width, sz.height, 0);
            return;
        }
    }

    if( oclop == OCL_OP_NOT )
        haveScalar = true;
    else if( (kind1 == _InputArray::MATX) + (kind2 == _InputArray::MATX) == 1 ||
        !psrc1->sameSize(*psrc2) || type1 != type2 )
    {
        if( checkScalar(*psrc1, type2, kind1, kind2) )
        {
            // src1 is a scalar; swap it with src2
            swap(psrc1, psrc2);
            swap(type1, type2);
            swap(depth1, depth2);
            swap(cn, cn2);
            swap(sz1, sz2);
        }
        else if( !checkScalar(*psrc2, type1, kind2, kind1) )
            CC_Error( CC_StsUnmatchedSizes,
                      "The operation is neither 'array op array' (where arrays have the same size and type), "
                      "nor 'array op scalar', nor 'scalar op array'" );
        haveScalar = true;
    }
    else
    {
        CC_Assert( psrc1->sameSize(*psrc2) && type1 == type2 );
    }

    size_t esz = CC_ELEM_SIZE(type1);
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    BinaryFunc copymask = 0;
    bool reallocate = false;

    if( haveMask )
    {
        int mtype = _mask CC_MAT_TYPE();
        CC_Assert( (mtype == CC_8U || mtype == CC_8S) && _mask.sameSize(*psrc1));
        copymask = getCopyMaskFunc(esz);
        reallocate = !_dst.sameSize(*psrc1) || _dst CC_MAT_TYPE() != type1;
    }

    CAutoBuffer<uchar> _buf;
    uchar *scbuf = 0, *maskbuf = 0;

    _dst.createSameSize(*psrc1, type1);
    // if this is mask operation and dst has been reallocated,
    // we have to clear the destination
    if( haveMask && reallocate )
        _dst.setTo(0.);

    CC_OCL_RUN(use_opencl,
               ocl_binary_op(*psrc1, *psrc2, _dst, _mask, bitwise, oclop, haveScalar))


    img_t src1 = psrc1->getMat(), src2 = psrc2->getMat();
    img_t *dst = _dst, mask = _mask;

    if( bitwise )
    {
        func = *tab;
        cn = (int)esz;
    }
    else
        func = tab[depth1];

    if( !haveScalar )
    {
        const img_t* arrays[] = { &src1, &src2, &dst, &mask, 0 };
        uchar* ptrs[4];

        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size, blocksize = total;

        if( blocksize*cn > INT_MAX )
            blocksize = INT_MAX/cn;

        if( haveMask )
        {
            blocksize = MIN(blocksize, blocksize0);
            _buf.allocate(blocksize*esz);
            maskbuf = _buf;
        }

        for( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            for( size_t j = 0; j < total; j += blocksize )
            {
                int bsz = (int)MIN(total - j, blocksize);

                func( ptrs[0], 0, ptrs[1], 0, haveMask ? maskbuf : ptrs[2], 0, bsz*cn, 1, 0 );
                if( haveMask )
                {
                    copymask( maskbuf, 0, ptrs[3], 0, ptrs[2], 0, Size(bsz, 1), &esz );
                    ptrs[3] += bsz;
                }

                bsz *= (int)esz;
                ptrs[0] += bsz; ptrs[1] += bsz; ptrs[2] += bsz;
            }
        }
    }
    else
    {
        const img_t* arrays[] = { &src1, &dst, &mask, 0 };
        uchar* ptrs[3];

        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size, blocksize = MIN(total, blocksize0);

        _buf.allocate(blocksize*(haveMask ? 2 : 1)*esz + 32);
        scbuf = _buf;
        maskbuf = cAlignPtr(scbuf + blocksize*esz, 16);

        convertAndUnrollScalar( src2, src1 CC_MAT_TYPE(), scbuf, blocksize);

        for( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            for( size_t j = 0; j < total; j += blocksize )
            {
                int bsz = (int)MIN(total - j, blocksize);

                func( ptrs[0], 0, scbuf, 0, haveMask ? maskbuf : ptrs[1], 0, bsz*cn, 1, 0 );
                if( haveMask )
                {
                    copymask( maskbuf, 0, ptrs[2], 0, ptrs[1], 0, Size(bsz, 1), &esz );
                    ptrs[2] += bsz;
                }

                bsz *= (int)esz;
                ptrs[0] += bsz; ptrs[1] += bsz;
            }
        }
    }
}

static BinaryFuncC* getMaxTab()
{
    static BinaryFuncC maxTab[] =
    {
        (BinaryFuncC)GET_OPTIMIZED(hal_max8u), (BinaryFuncC)GET_OPTIMIZED(hal_max8s),
        (BinaryFuncC)GET_OPTIMIZED(hal_max16u), (BinaryFuncC)GET_OPTIMIZED(hal_max16s),
        (BinaryFuncC)GET_OPTIMIZED(hal_max32s),
        (BinaryFuncC)GET_OPTIMIZED(hal_max32f), (BinaryFuncC)hal_max64f,
        0
    };

    return maxTab;
}

static BinaryFuncC* getMinTab()
{
    static BinaryFuncC minTab[] =
    {
        (BinaryFuncC)GET_OPTIMIZED(hal_min8u), (BinaryFuncC)GET_OPTIMIZED(hal_min8s),
        (BinaryFuncC)GET_OPTIMIZED(hal_min16u), (BinaryFuncC)GET_OPTIMIZED(hal_min16s),
        (BinaryFuncC)GET_OPTIMIZED(hal_min32s),
        (BinaryFuncC)GET_OPTIMIZED(hal_min32f), (BinaryFuncC)hal_min64f,
        0
    };

    return minTab;
}



void bitwise_and(const img_t* a, const img_t* b, img_t* c, const img_t* mask)
{
    CC_INSTRUMENT_REGION()

    BinaryFuncC f = (BinaryFuncC)GET_OPTIMIZED(hal_and8u);
    binary_op(a, b, c, mask, &f, true, OCL_OP_AND);
}

void bitwise_or(const img_t* a, const img_t* b, img_t* c, const img_t* mask)
{
    CC_INSTRUMENT_REGION()

    BinaryFuncC f = (BinaryFuncC)GET_OPTIMIZED(hal_or8u);
    binary_op(a, b, c, mask, &f, true, OCL_OP_OR);
}

void bitwise_xor(const img_t* a, const img_t* b, img_t* c, const img_t* mask)
{
    CC_INSTRUMENT_REGION()

    BinaryFuncC f = (BinaryFuncC)GET_OPTIMIZED(hal_xor8u);
    binary_op(a, b, c, mask, &f, true, OCL_OP_XOR);
}

void bitwise_not(const img_t* a, img_t* c, const img_t* mask)
{
    CC_INSTRUMENT_REGION()

    BinaryFuncC f = (BinaryFuncC)GET_OPTIMIZED(hal_not8u);
    binary_op(a, a, c, mask, &f, true, OCL_OP_NOT);
}

void max( const img_t* src1, const img_t* src2, img_t* dst )
{
    CC_INSTRUMENT_REGION()

    binary_op(src1, src2, dst, NULL, getMaxTab(), false, OCL_OP_MAX );
}

void min( const img_t* src1, const img_t* src2, img_t* dst )
{
    CC_INSTRUMENT_REGION()

    binary_op(src1, src2, dst, NULL, getMinTab(), false, OCL_OP_MIN );
}

void max(const img_t* src1, const img_t* src2, img_t* dst)
{
    CC_INSTRUMENT_REGION()

    img_t* _dst(dst);
    binary_op(src1, src2, _dst, NULL, getMaxTab(), false, OCL_OP_MAX );
}

void min(const img_t* src1, const img_t* src2, img_t* dst)
{
    CC_INSTRUMENT_REGION()

    img_t* _dst(dst);
    binary_op(src1, src2, _dst, NULL, getMinTab(), false, OCL_OP_MIN );
}

void max(const UMat& src1, const UMat& src2, UMat& dst)
{
    CC_INSTRUMENT_REGION()

    img_t* _dst(dst);
    binary_op(src1, src2, _dst, NULL, getMaxTab(), false, OCL_OP_MAX );
}

void min(const UMat& src1, const UMat& src2, UMat& dst)
{
    CC_INSTRUMENT_REGION()

    img_t* _dst(dst);
    binary_op(src1, src2, _dst, NULL, getMinTab(), false, OCL_OP_MIN );
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

static int actualScalarDepth(const double* data, int len)
{
    int i = 0, minval = INT_MAX, maxval = INT_MIN;
    for(; i < len; ++i)
    {
        int ival = cRound(data[i]);
        if( ival != data[i] )
            break;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    return i < len ? CC_64F :
        minval >= 0 && maxval <= (int)UCHAR_MAX ? CC_8U :
        minval >= (int)SCHAR_MIN && maxval <= (int)SCHAR_MAX ? CC_8S :
        minval >= 0 && maxval <= (int)USHRT_MAX ? CC_16U :
        minval >= (int)SHRT_MIN && maxval <= (int)SHRT_MAX ? CC_16S :
        CC_32S;
}

#ifdef HAVE_OPENCL

static bool ocl_arithm_op(const img_t* _src1, const img_t* _src2, img_t* _dst,
                          const img_t* _mask, int wtype,
                          void* usrdata, int oclop,
                          bool haveScalar )
{
    const ocl::Device d = ocl::Device::getDefault();
    bool doubleSupport = d.doubleFPConfig() > 0;
    int type1 = _src1 CC_MAT_TYPE(), depth1 = CC_MAT_DEPTH(type1), cn = CC_MAT_CN(type1);
    bool haveMask = !_mask.empty();

    if ( (haveMask || haveScalar) && cn > 4 )
        return false;

    int dtype = _dst CC_MAT_TYPE(), ddepth = CC_MAT_DEPTH(dtype), wdepth = MAX(CC_32S, CC_MAT_DEPTH(wtype));
    if (!doubleSupport)
        wdepth = MIN(wdepth, CC_32F);

    wtype = CC_MAKETYPE(wdepth, cn);
    int type2 = haveScalar ? wtype : _src2 CC_MAT_TYPE(), depth2 = CC_MAT_DEPTH(type2);
    if (!doubleSupport && (depth2 == CC_64F || depth1 == CC_64F))
        return false;

    int kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    int scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    char cvtstr[4][32], opts[1024];
    sprintf(opts, "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CC_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CC_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CC_MAKETYPE(ddepth, kercn)),
            ocl::typeToStr(ddepth), ocl::typeToStr(CC_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CC_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0]),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1]),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2]),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CC_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CC_8U, ddepth, kercn, cvtstr[3]) : "noconvert");

    size_t usrdata_esz = CC_ELEM_SIZE(wdepth);
    const uchar* usrdata_p = (const uchar*)usrdata;
    const double* usrdata_d = (const double*)usrdata;
    float usrdata_f[3];
    int i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    if( n > 0 && wdepth == CC_32F )
    {
        for( i = 0; i < n; i++ )
            usrdata_f[i] = (float)usrdata_d[i];
        usrdata_p = (const uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    if (k.empty())
        return false;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    if( haveScalar )
    {
        size_t esz = CC_ELEM_SIZE1(wtype)*scalarcn;
        double buf[4]={0,0,0,0};
        img_t src2sc = _src2;

        if( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(0, 0, 0, 0, buf, esz);

        if( !haveMask )
        {
            if(n == 0)
                k.args(src1arg, dstarg, scalararg);
            else if(n == 1)
                k.args(src1arg, dstarg, scalararg,
                       ocl::KernelArg(0, 0, 0, 0, usrdata_p, usrdata_esz));
            else
                CC_Error(CC_StsNotImplemented, "unsupported number of extra parameters");
        }
        else
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    else
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        if( !haveMask )
        {
            if (n == 0)
                k.args(src1arg, src2arg, dstarg);
            else if (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(0, 0, 0, 0, usrdata_p, usrdata_esz));
            else if (n == 3)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(0, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(0, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(0, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            else
                CC_Error(CC_StsNotImplemented, "unsupported number of extra parameters");
        }
        else
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1->cols * cn / kercn, ((size_t)src1->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, NULL, false);
}

#endif

static void arithm_op(const img_t* _src1, const img_t* _src2, img_t* _dst,
                      const img_t* _mask, int dtype, BinaryFuncC* tab, bool muldiv=false,
                      void* usrdata=0, int oclop=-1 )
{
    const _InputArray *psrc1 = &_src1, *psrc2 = &_src2;
    int kind1 = psrc1->kind(), kind2 = psrc2->kind();
    bool haveMask = !_mask.empty();
    bool reallocate = false;
    int type1 = psrc1->tid(), depth1 = CC_MAT_DEPTH(type1), cn = CC_MAT_CN(type1);
    int type2 = psrc2->tid(), depth2 = CC_MAT_DEPTH(type2), cn2 = CC_MAT_CN(type2);
    int wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    Size sz1 = dims1 <= 2 ? psrc1->size() : Size();
    Size sz2 = dims2 <= 2 ? psrc2->size() : Size();
#ifdef HAVE_OPENCL
    bool use_opencl = OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2;
#endif
    bool src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    bool src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);

    if( (kind1 == kind2 || cn == 1) && sz1 == sz2 && dims1 <= 2 && dims2 <= 2 && type1 == type2 &&
        !haveMask && ((!_dst.fixedType() && (dtype < 0 || CC_MAT_DEPTH(dtype) == depth1)) ||
                       (_dst.fixedType() && _dst CC_MAT_TYPE() == type1)) &&
        ((src1Scalar && src2Scalar) || (!src1Scalar && !src2Scalar)) )
    {
        _dst.createSameSize(*psrc1, type1);
        CC_OCL_RUN(use_opencl,
            ocl_arithm_op(*psrc1, *psrc2, _dst, _mask,
                          (!usrdata ? type1 : MAX(depth1, CC_32F)),
                          usrdata, oclop, false))

        img_t src1 = psrc1->getMat(), src2 = psrc2->getMat(), dst = _dst;
        Size sz = getContinuousSize(src1, src2, dst, src1 CC_MAT_CN());
        tab[depth1](src1->tt.data, src1->step, src2->tt.data, src2->step, dst->tt.data, dst->step, sz.width, sz.height, usrdata);
        return;
    }

    bool haveScalar = false, swapped12 = false;

    if( dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == _InputArray::MATX && (sz1 == Size(1,4) || sz1 == Size(1,1))) ||
        (kind2 == _InputArray::MATX && (sz2 == Size(1,4) || sz2 == Size(1,1))) )
    {
        if( checkScalar(*psrc1, type2, kind1, kind2) )
        {
            // src1 is a scalar; swap it with src2
            swap(psrc1, psrc2);
            swap(sz1, sz2);
            swap(type1, type2);
            swap(depth1, depth2);
            swap(cn, cn2);
            swap(dims1, dims2);
            swapped12 = true;
            if( oclop == OCL_OP_SUB )
                oclop = OCL_OP_RSUB;
            if ( oclop == OCL_OP_DIV_SCALE )
                oclop = OCL_OP_RDIV_SCALE;
        }
        else if( !checkScalar(*psrc2, type1, kind2, kind1) )
            CC_Error( CC_StsUnmatchedSizes,
                     "The operation is neither 'array op array' "
                     "(where arrays have the same size and the same number of channels), "
                     "nor 'array op scalar', nor 'scalar op array'" );
        haveScalar = true;
        CC_Assert(type2 == CC_64F && (sz2.height == 1 || sz2.height == 4));

        if (!muldiv)
        {
            img_t sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc img_ptr(double, ), sz2 == Size(1, 1) ? cn2 : cn);
            if( depth2 == CC_64F && (depth1 < CC_32S || depth1 == CC_32F) )
                depth2 = CC_32F;
        }
        else
            depth2 = CC_64F;
    }

    if( dtype < 0 )
    {
        if( _dst.fixedType() )
            dtype = _dst CC_MAT_TYPE();
        else
        {
            if( !haveScalar && type1 != type2 )
                CC_Error(CC_StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CC_MAT_DEPTH(dtype);

    if( depth1 == depth2 && dtype == depth1 )
        wtype = dtype;
    else if( !muldiv )
    {
        wtype = depth1 <= CC_8S && depth2 <= CC_8S ? CC_16S :
                depth1 <= CC_32S && depth2 <= CC_32S ? CC_32S : MAX(depth1, depth2);
        wtype = MAX(wtype, dtype);

        // when the result of addition should be converted to an integer type,
        // and just one of the input arrays is floating-point, it makes sense to convert that input to integer type before the operation,
        // instead of converting the other input to floating-point and then converting the operation result back to integers.
        if( dtype < CC_32F && (depth1 < CC_32F || depth2 < CC_32F) )
            wtype = CC_32S;
    }
    else
    {
        wtype = MAX(depth1, MAX(depth2, CC_32F));
        wtype = MAX(wtype, dtype);
    }

    dtype = CC_MAKETYPE(dtype, cn);
    wtype = CC_MAKETYPE(wtype, cn);

    if( haveMask )
    {
        int mtype = _mask CC_MAT_TYPE();
        CC_Assert( (mtype == CC_8UC1 || mtype == CC_8SC1) && _mask.sameSize(*psrc1) );
        reallocate = !_dst.sameSize(*psrc1) || _dst CC_MAT_TYPE() != dtype;
    }

    _dst.createSameSize(*psrc1, dtype);
    if( reallocate )
        _dst.setTo(0.);

    CC_OCL_RUN(use_opencl,
               ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype,
               usrdata, oclop, haveScalar))

    BinaryFunc cvtsrc1 = type1 == wtype ? 0 : getConvertFunc(type1, wtype);
    BinaryFunc cvtsrc2 = type2 == type1 ? cvtsrc1 : type2 == wtype ? 0 : getConvertFunc(type2, wtype);
    BinaryFunc cvtdst = dtype == wtype ? 0 : getConvertFunc(wtype, dtype);

    size_t esz1 = CC_ELEM_SIZE(type1), esz2 = CC_ELEM_SIZE(type2);
    size_t dsz = CC_ELEM_SIZE(dtype), wsz = CC_ELEM_SIZE(wtype);
    size_t blocksize0 = (size_t)(BLOCK_SIZE + wsz-1)/wsz;
    BinaryFunc copymask = getCopyMaskFunc(dsz);
    img_t src1 = psrc1->getMat(), src2 = psrc2->getMat(), dst = _dst, mask = _mask;

    CAutoBuffer<uchar> _buf;
    uchar *buf, *maskbuf = 0, *buf1 = 0, *buf2 = 0, *wbuf = 0;
    size_t bufesz = (cvtsrc1 ? wsz : 0) +
                    (cvtsrc2 || haveScalar ? wsz : 0) +
                    (cvtdst ? wsz : 0) +
                    (haveMask ? dsz : 0);
    BinaryFuncC func = tab[CC_MAT_DEPTH(wtype)];

    if( !haveScalar )
    {
        const img_t* arrays[] = { &src1, &src2, &dst, &mask, 0 };
        uchar* ptrs[4];

        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size, blocksize = total;

        if( haveMask || cvtsrc1 || cvtsrc2 || cvtdst )
            blocksize = MIN(blocksize, blocksize0);

        _buf.allocate(bufesz*blocksize + 64);
        buf = _buf;
        if( cvtsrc1 )
            buf1 = buf, buf = cAlignPtr(buf + blocksize*wsz, 16);
        if( cvtsrc2 )
            buf2 = buf, buf = cAlignPtr(buf + blocksize*wsz, 16);
        wbuf = maskbuf = buf;
        if( cvtdst )
            buf = cAlignPtr(buf + blocksize*wsz, 16);
        if( haveMask )
            maskbuf = buf;

        for( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            for( size_t j = 0; j < total; j += blocksize )
            {
                int bsz = (int)MIN(total - j, blocksize);
                Size bszn(bsz*cn, 1);
                const uchar *sptr1 = ptrs[0], *sptr2 = ptrs[1];
                uchar* dptr = ptrs[2];
                if( cvtsrc1 )
                {
                    cvtsrc1( sptr1, 1, 0, 1, buf1, 1, bszn, 0 );
                    sptr1 = buf1;
                }
                if( ptrs[0] == ptrs[1] )
                    sptr2 = sptr1;
                else if( cvtsrc2 )
                {
                    cvtsrc2( sptr2, 1, 0, 1, buf2, 1, bszn, 0 );
                    sptr2 = buf2;
                }

                if( !haveMask && !cvtdst )
                    func( sptr1, 1, sptr2, 1, dptr, 1, bszn.width, bszn.height, usrdata );
                else
                {
                    func( sptr1, 1, sptr2, 1, wbuf, 0, bszn.width, bszn.height, usrdata );
                    if( !haveMask )
                        cvtdst( wbuf, 1, 0, 1, dptr, 1, bszn, 0 );
                    else if( !cvtdst )
                    {
                        copymask( wbuf, 1, ptrs[3], 1, dptr, 1, Size(bsz, 1), &dsz );
                        ptrs[3] += bsz;
                    }
                    else
                    {
                        cvtdst( wbuf, 1, 0, 1, maskbuf, 1, bszn, 0 );
                        copymask( maskbuf, 1, ptrs[3], 1, dptr, 1, Size(bsz, 1), &dsz );
                        ptrs[3] += bsz;
                    }
                }
                ptrs[0] += bsz*esz1; ptrs[1] += bsz*esz2; ptrs[2] += bsz*dsz;
            }
        }
    }
    else
    {
        const img_t* arrays[] = { &src1, &dst, &mask, 0 };
        uchar* ptrs[3];

        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size, blocksize = MIN(total, blocksize0);

        _buf.allocate(bufesz*blocksize + 64);
        buf = _buf;
        if( cvtsrc1 )
            buf1 = buf, buf = cAlignPtr(buf + blocksize*wsz, 16);
        buf2 = buf; buf = cAlignPtr(buf + blocksize*wsz, 16);
        wbuf = maskbuf = buf;
        if( cvtdst )
            buf = cAlignPtr(buf + blocksize*wsz, 16);
        if( haveMask )
            maskbuf = buf;

        convertAndUnrollScalar( src2, wtype, buf2, blocksize);

        for( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            for( size_t j = 0; j < total; j += blocksize )
            {
                int bsz = (int)MIN(total - j, blocksize);
                Size bszn(bsz*cn, 1);
                const uchar *sptr1 = ptrs[0];
                const uchar* sptr2 = buf2;
                uchar* dptr = ptrs[1];

                if( cvtsrc1 )
                {
                    cvtsrc1( sptr1, 1, 0, 1, buf1, 1, bszn, 0 );
                    sptr1 = buf1;
                }

                if( swapped12 )
                    T_SWAP(_Tp, sptr1, sptr2);

                if( !haveMask && !cvtdst )
                    func( sptr1, 1, sptr2, 1, dptr, 1, bszn.width, bszn.height, usrdata );
                else
                {
                    func( sptr1, 1, sptr2, 1, wbuf, 1, bszn.width, bszn.height, usrdata );
                    if( !haveMask )
                        cvtdst( wbuf, 1, 0, 1, dptr, 1, bszn, 0 );
                    else if( !cvtdst )
                    {
                        copymask( wbuf, 1, ptrs[2], 1, dptr, 1, Size(bsz, 1), &dsz );
                        ptrs[2] += bsz;
                    }
                    else
                    {
                        cvtdst( wbuf, 1, 0, 1, maskbuf, 1, bszn, 0 );
                        copymask( maskbuf, 1, ptrs[2], 1, dptr, 1, Size(bsz, 1), &dsz );
                        ptrs[2] += bsz;
                    }
                }
                ptrs[0] += bsz*esz1; ptrs[1] += bsz*dsz;
            }
        }
    }
}

static BinaryFuncC* getAddTab()
{
    static BinaryFuncC addTab[] =
    {
        (BinaryFuncC)GET_OPTIMIZED(hal_add8u), (BinaryFuncC)GET_OPTIMIZED(hal_add8s),
        (BinaryFuncC)GET_OPTIMIZED(hal_add16u), (BinaryFuncC)GET_OPTIMIZED(hal_add16s),
        (BinaryFuncC)GET_OPTIMIZED(hal_add32s),
        (BinaryFuncC)GET_OPTIMIZED(hal_add32f), (BinaryFuncC)hal_add64f,
        0
    };

    return addTab;
}

static BinaryFuncC* getSubTab()
{
    static BinaryFuncC subTab[] =
    {
        (BinaryFuncC)GET_OPTIMIZED(hal_sub8u), (BinaryFuncC)GET_OPTIMIZED(hal_sub8s),
        (BinaryFuncC)GET_OPTIMIZED(hal_sub16u), (BinaryFuncC)GET_OPTIMIZED(hal_sub16s),
        (BinaryFuncC)GET_OPTIMIZED(hal_sub32s),
        (BinaryFuncC)GET_OPTIMIZED(hal_sub32f), (BinaryFuncC)hal_sub64f,
        0
    };

    return subTab;
}

static BinaryFuncC* getAbsDiffTab()
{
    static BinaryFuncC absDiffTab[] =
    {
        (BinaryFuncC)GET_OPTIMIZED(hal_absdiff8u), (BinaryFuncC)GET_OPTIMIZED(hal_absdiff8s),
        (BinaryFuncC)GET_OPTIMIZED(hal_absdiff16u), (BinaryFuncC)GET_OPTIMIZED(hal_absdiff16s),
        (BinaryFuncC)GET_OPTIMIZED(hal_absdiff32s),
        (BinaryFuncC)GET_OPTIMIZED(hal_absdiff32f), (BinaryFuncC)hal_absdiff64f,
        0
    };

    return absDiffTab;
}

void add( const img_t* src1, const img_t* src2, img_t* dst,
          const img_t* mask, int dtype )
{
    CC_INSTRUMENT_REGION()

    arithm_op(src1, src2, dst, mask, dtype, getAddTab(), false, 0, OCL_OP_ADD );
}

void subtract( const img_t* _src1, const img_t* _src2, img_t* _dst,
               const img_t* mask, int dtype )
{
    CC_INSTRUMENT_REGION()

#ifdef HAVE_TEGRA_OPTIMIZATION
    if (tegra::useTegra())
    {
        int kind1 = _src1.kind(), kind2 = _src2.kind();
        img_t src1 = _src1, src2 = _src2;
        bool src1Scalar = checkScalar(src1, _src2 CC_MAT_TYPE(), kind1, kind2);
        bool src2Scalar = checkScalar(src2, _src1 CC_MAT_TYPE(), kind2, kind1);

        if (!src1Scalar && !src2Scalar &&
            src1 CC_MAT_DEPTH() == CC_8U && src2 CC_MAT_TYPE() == src1 CC_MAT_TYPE() &&
            src1.dims == 2 && src2.size() == src1.size() &&
            mask.empty())
        {
            if (dtype < 0)
            {
                if (_dst.fixedType())
                {
                    dtype = _dst CC_MAT_DEPTH();
                }
                else
                {
                    dtype = src1 CC_MAT_DEPTH();
                }
            }

            dtype = CC_MAT_DEPTH(dtype);

            if (!_dst.fixedType() || dtype == _dst CC_MAT_DEPTH())
            {
                _dst cvSetMat(src1.size(), CC_MAKE_TYPE(dtype, src1 CC_MAT_CN()));

                if (dtype == CC_16S)
                {
                    img_t *dst = _dst;
                    if(tegra::subtract_8u8u16s(src1, src2, dst))
                        return;
                }
                else if (dtype == CC_32F)
                {
                    img_t *dst = _dst;
                    if(tegra::subtract_8u8u32f(src1, src2, dst))
                        return;
                }
                else if (dtype == CC_8S)
                {
                    img_t *dst = _dst;
                    if(tegra::subtract_8u8u8s(src1, src2, dst))
                        return;
                }
            }
        }
    }
#endif
    arithm_op(_src1, _src2, _dst, mask, dtype, getSubTab(), false, 0, OCL_OP_SUB );
}

void absdiff( const img_t* src1, const img_t* src2, img_t* dst )
{
    CC_INSTRUMENT_REGION()

    arithm_op(src1, src2, dst, NULL, -1, getAbsDiffTab(), false, 0, OCL_OP_ABSDIFF);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

static BinaryFuncC* getMulTab()
{
    static BinaryFuncC mulTab[] =
    {
        (BinaryFuncC)hal_mul8u, (BinaryFuncC)hal_mul8s, (BinaryFuncC)hal_mul16u,
        (BinaryFuncC)hal_mul16s, (BinaryFuncC)hal_mul32s, (BinaryFuncC)hal_mul32f,
        (BinaryFuncC)hal_mul64f, 0
    };

    return mulTab;
}

static BinaryFuncC* getDivTab()
{
    static BinaryFuncC divTab[] =
    {
        (BinaryFuncC)hal_div8u, (BinaryFuncC)hal_div8s, (BinaryFuncC)hal_div16u,
        (BinaryFuncC)hal_div16s, (BinaryFuncC)hal_div32s, (BinaryFuncC)hal_div32f,
        (BinaryFuncC)hal_div64f, 0
    };

    return divTab;
}

static BinaryFuncC* getRecipTab()
{
    static BinaryFuncC recipTab[] =
    {
        (BinaryFuncC)hal_recip8u, (BinaryFuncC)hal_recip8s, (BinaryFuncC)hal_recip16u,
        (BinaryFuncC)hal_recip16s, (BinaryFuncC)hal_recip32s, (BinaryFuncC)hal_recip32f,
        (BinaryFuncC)hal_recip64f, 0
    };

    return recipTab;
}

void multiply(const img_t* src1, const img_t* src2,
                  img_t* dst, double scale, int dtype)
{
    CC_INSTRUMENT_REGION()

    arithm_op(src1, src2, dst, NULL, dtype, getMulTab(),
              true, &scale, std::abs(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE);
}

void divide(const img_t* src1, const img_t* src2,
                img_t* dst, double scale, int dtype)
{
    CC_INSTRUMENT_REGION()

    arithm_op(src1, src2, dst, NULL, dtype, getDivTab(), true, &scale, OCL_OP_DIV_SCALE);
}

void divide(double scale, const img_t* src2,
                img_t* dst, int dtype)
{
    CC_INSTRUMENT_REGION()

    arithm_op(src2, src2, dst, NULL, dtype, getRecipTab(), true, &scale, OCL_OP_RECIP_SCALE);
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

static BinaryFuncC* getAddWeightedTab()
{
    static BinaryFuncC addWeightedTab[] =
    {
        (BinaryFuncC)GET_OPTIMIZED(hal_addWeighted8u), (BinaryFuncC)GET_OPTIMIZED(hal_addWeighted8s), (BinaryFuncC)GET_OPTIMIZED(hal_addWeighted16u),
        (BinaryFuncC)GET_OPTIMIZED(hal_addWeighted16s), (BinaryFuncC)GET_OPTIMIZED(hal_addWeighted32s), (BinaryFuncC)hal_addWeighted32f,
        (BinaryFuncC)hal_addWeighted64f, 0
    };

    return addWeightedTab;
}

void addWeighted( const img_t* src1, double alpha, const img_t* src2,
                      double beta, double gamma, img_t* dst, int dtype )
{
    CC_INSTRUMENT_REGION()

    double scalars[] = {alpha, beta, gamma};
    arithm_op(src1, src2, dst, NULL, dtype, getAddWeightedTab(), true, scalars, OCL_OP_ADDW);
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

static BinaryFuncC getCmpFunc(int depth)
{
    static BinaryFuncC cmpTab[] =
    {
        (BinaryFuncC)GET_OPTIMIZED(hal_cmp8u), (BinaryFuncC)GET_OPTIMIZED(hal_cmp8s),
        (BinaryFuncC)GET_OPTIMIZED(hal_cmp16u), (BinaryFuncC)GET_OPTIMIZED(hal_cmp16s),
        (BinaryFuncC)GET_OPTIMIZED(hal_cmp32s),
        (BinaryFuncC)GET_OPTIMIZED(hal_cmp32f), (BinaryFuncC)hal_cmp64f,
        0
    };

    return cmpTab[depth];
}

static double getMinVal(int depth)
{
    static const double tab[] = {0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX, 0};
    return tab[depth];
}

static double getMaxVal(int depth)
{
    static const double tab[] = {255, 127, 65535, 32767, INT_MAX, FLT_MAX, DBL_MAX, 0};
    return tab[depth];
}

#ifdef HAVE_OPENCL

static bool ocl_compare(const img_t* _src1, const img_t* _src2, img_t* _dst, int op, bool haveScalar)
{
    const ocl::Device& dev = ocl::Device::getDefault();
    bool doubleSupport = dev.doubleFPConfig() > 0;
    int type1 = _src1 CC_MAT_TYPE(), depth1 = CC_MAT_DEPTH(type1), cn = CC_MAT_CN(type1),
            type2 = _src2 CC_MAT_TYPE(), depth2 = CC_MAT_DEPTH(type2);

    if (!doubleSupport && depth1 == CC_64F)
        return false;

    if (!haveScalar && (!_src1.sameSize(_src2) || type1 != type2))
            return false;

    int kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    if (depth1 >= CC_16U)
        kercn = 1;

    int scalarcn = kercn == 3 ? 4 : kercn;
    const char * const operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    char cvt[40];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CC_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CC_8UC(kercn)), kercn,
                         ocl::convertTypeStr(depth1, CC_8U, kercn, cvt),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CC_8U),
                         ocl::typeToStr(CC_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    if (k.empty())
        return false;

    UMat src1 = _src1.getUMat();
    Size size = src1.size();
    _dst cvSetMat(size, CC_8UC(cn));
    UMat dst = _dst.getUMat();

    if (haveScalar)
    {
        size_t esz = CC_ELEM_SIZE1(type1) * scalarcn;
        double buf[4] = { 0, 0, 0, 0 };
        img_t src2 = _src2;

        if( depth1 > CC_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        else
        {
            double fval = 0;
            getConvertFunc(depth2, CC_64F)(src2->tt.data, 1, 0, 1, (uchar *)&fval, 1, Size(1, 1), 0);
            if( fval < getMinVal(depth1) )
                return dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), true;

            if( fval > getMaxVal(depth1) )
                return dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), true;

            int ival = cRound(fval);
            if( fval != ival )
            {
                if( op == CMP_LT || op == CMP_GE )
                    ival = cCeil(fval);
                else if( op == CMP_LE || op == CMP_GT )
                    ival = cFloor(fval);
                else
                    return dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), true;
            }
            convertAndUnrollScalar(img_t(1, 1, CC_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(0, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    else
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1),
               ocl::KernelArg::ReadOnlyNoSize(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst->cols * cn / kercn, ((size_t)dst->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, NULL, false);
}

#endif

void compare(const img_t* _src1, const img_t* _src2, img_t* _dst, int op)
{
    CC_INSTRUMENT_REGION()

    CC_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    bool haveScalar = false;

    if ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.sameSize(_src2)
            || _src1 CC_MAT_TYPE() != _src2 CC_MAT_TYPE())
    {
        if (checkScalar(_src1, _src2 CC_MAT_TYPE(), _src1.kind(), _src2.kind()))
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            return;
        }
        else if( !checkScalar(_src2, _src1 CC_MAT_TYPE(), _src2.kind(), _src1.kind()) )
            CC_Error( CC_StsUnmatchedSizes,
                     "The operation is neither 'array op array' (where arrays have the same size and the same type), "
                     "nor 'array op scalar', nor 'scalar op array'" );
        haveScalar = true;
    }

    CC_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_compare(_src1, _src2, _dst, op, haveScalar))

    int kind1 = _src1.kind(), kind2 = _src2.kind();
    img_t src1 = _src1, src2 = _src2;

    if( kind1 == kind2 && src1.dims <= 2 && src2.dims <= 2 && src1.size() == src2.size() && src1 CC_MAT_TYPE() == src2 CC_MAT_TYPE() )
    {
        int cn = src1 CC_MAT_CN();
        _dst cvSetMat(src1.size(), CC_8UC(cn));
        img_t *dst = _dst;
        Size sz = getContinuousSize(src1, src2, dst, src1 CC_MAT_CN());
        getCmpFunc(src1 CC_MAT_DEPTH())(src1->tt.data, src1->step, src2->tt.data, src2->step, dst->tt.data, dst->step, sz.width, sz.height, &op);
        return;
    }

    int cn = src1 CC_MAT_CN(), depth1 = src1 CC_MAT_DEPTH(), depth2 = src2 CC_MAT_DEPTH();

    _dst cvSetMat(src1.dims, src1.size, CC_8UC(cn));
    src1 = src1 cvReshape(,1); src2 = src2 cvReshape(,1);
    img_t *dst = _dst cvReshape(,1);

    size_t esz = src1 CC_ELEM_SIZE();
    size_t blocksize0 = (size_t)(BLOCK_SIZE + esz-1)/esz;
    BinaryFuncC func = getCmpFunc(depth1);

    if( !haveScalar )
    {
        const img_t* arrays[] = { &src1, &src2, &dst, 0 };
        uchar* ptrs[3];

        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size;

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, ptrs[1], 0, ptrs[2], 0, (int)total, 1, &op );
    }
    else
    {
        const img_t* arrays[] = { &src1, &dst, 0 };
        uchar* ptrs[2];

        NAryMatIterator it(arrays, ptrs);
        size_t total = it.size, blocksize = MIN(total, blocksize0);

        CAutoBuffer<uchar> _buf(blocksize*esz);
        uchar *buf = _buf;

        if( depth1 > CC_32S )
            convertAndUnrollScalar( src2, depth1, buf, blocksize );
        else
        {
            double fval=0;
            getConvertFunc(depth2, CC_64F)(src2->tt.data, 1, 0, 1, (uchar*)&fval, 1, Size(1,1), 0);
            if( fval < getMinVal(depth1) )
            {
                dst = Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0);
                return;
            }

            if( fval > getMaxVal(depth1) )
            {
                dst = Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0);
                return;
            }

            int ival = cRound(fval);
            if( fval != ival )
            {
                if( op == CMP_LT || op == CMP_GE )
                    ival = cCeil(fval);
                else if( op == CMP_LE || op == CMP_GT )
                    ival = cFloor(fval);
                else
                {
                    dst = Scalar::all(op == CMP_NE ? 255 : 0);
                    return;
                }
            }
            convertAndUnrollScalar(img_t(1, 1, CC_32S, &ival), depth1, buf, blocksize);
        }

        for( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            for( size_t j = 0; j < total; j += blocksize )
            {
                int bsz = (int)MIN(total - j, blocksize);
                func( ptrs[0], 0, buf, 0, ptrs[1], 0, bsz, 1, &op);
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz;
            }
        }
    }
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

template <typename T>
struct InRange_SIMD
{
    int operator () (const T *, const T *, const T *, uchar *, int) const
    {
        return 0;
    }
};

#if CC_SSE2

template <>
struct InRange_SIMD<uchar>
{
    int operator () (const uchar * src1, const uchar * src2, const uchar * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        if (USE_SSE2)
        {
            __m128i v_full = _mm_set1_epi8(-1), v_128 = _mm_set1_epi8(-128);

            for ( ; x <= len - 16; x += 16 )
            {
                __m128i v_src = _mm_add_epi8(_mm_loadu_si128((const __m128i *)(src1 + x)), v_128);
                __m128i v_mask1 = _mm_cmpgt_epi8(_mm_add_epi8(_mm_loadu_si128((const __m128i *)(src2 + x)), v_128), v_src);
                __m128i v_mask2 = _mm_cmpgt_epi8(v_src, _mm_add_epi8(_mm_loadu_si128((const __m128i *)(src3 + x)), v_128));
                _mm_storeu_si128((__m128i *)(dst + x), _mm_andnot_si128(_mm_or_si128(v_mask1, v_mask2), v_full));
            }
        }

        return x;
    }
};

template <>
struct InRange_SIMD<schar>
{
    int operator () (const schar * src1, const schar * src2, const schar * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        if (USE_SSE2)
        {
            __m128i v_full = _mm_set1_epi8(-1);

            for ( ; x <= len - 16; x += 16 )
            {
                __m128i v_src = _mm_loadu_si128((const __m128i *)(src1 + x));
                __m128i v_mask1 = _mm_cmpgt_epi8(_mm_loadu_si128((const __m128i *)(src2 + x)), v_src);
                __m128i v_mask2 = _mm_cmpgt_epi8(v_src, _mm_loadu_si128((const __m128i *)(src3 + x)));
                _mm_storeu_si128((__m128i *)(dst + x), _mm_andnot_si128(_mm_or_si128(v_mask1, v_mask2), v_full));
            }
        }

        return x;
    }
};

template <>
struct InRange_SIMD<ushort>
{
    int operator () (const ushort * src1, const ushort * src2, const ushort * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        if (USE_SSE2)
        {
            __m128i v_zero = _mm_setzero_si128(), v_full = _mm_set1_epi16(-1), v_32768 = _mm_set1_epi16(-32768);

            for ( ; x <= len - 8; x += 8 )
            {
                __m128i v_src = _mm_add_epi16(_mm_loadu_si128((const __m128i *)(src1 + x)), v_32768);
                __m128i v_mask1 = _mm_cmpgt_epi16(_mm_add_epi16(_mm_loadu_si128((const __m128i *)(src2 + x)), v_32768), v_src);
                __m128i v_mask2 = _mm_cmpgt_epi16(v_src, _mm_add_epi16(_mm_loadu_si128((const __m128i *)(src3 + x)), v_32768));
                __m128i v_res = _mm_andnot_si128(_mm_or_si128(v_mask1, v_mask2), v_full);
                _mm_storel_epi64((__m128i *)(dst + x), _mm_packus_epi16(_mm_srli_epi16(v_res, 8), v_zero));
            }
        }

        return x;
    }
};

template <>
struct InRange_SIMD<short>
{
    int operator () (const short * src1, const short * src2, const short * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        if (USE_SSE2)
        {
            __m128i v_zero = _mm_setzero_si128(), v_full = _mm_set1_epi16(-1);

            for ( ; x <= len - 8; x += 8 )
            {
                __m128i v_src = _mm_loadu_si128((const __m128i *)(src1 + x));
                __m128i v_mask1 = _mm_cmpgt_epi16(_mm_loadu_si128((const __m128i *)(src2 + x)), v_src);
                __m128i v_mask2 = _mm_cmpgt_epi16(v_src, _mm_loadu_si128((const __m128i *)(src3 + x)));
                __m128i v_res = _mm_andnot_si128(_mm_or_si128(v_mask1, v_mask2), v_full);
                _mm_storel_epi64((__m128i *)(dst + x), _mm_packus_epi16(_mm_srli_epi16(v_res, 8), v_zero));
            }
        }

        return x;
    }
};

template <>
struct InRange_SIMD<int>
{
    int operator () (const int * src1, const int * src2, const int * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        if (USE_SSE2)
        {
            __m128i v_zero = _mm_setzero_si128(), v_full = _mm_set1_epi32(-1);

            for ( ; x <= len - 8; x += 8 )
            {
                __m128i v_src = _mm_loadu_si128((const __m128i *)(src1 + x));
                __m128i v_res1 = _mm_or_si128(_mm_cmpgt_epi32(_mm_loadu_si128((const __m128i *)(src2 + x)), v_src),
                    _mm_cmpgt_epi32(v_src, _mm_loadu_si128((const __m128i *)(src3 + x))));

                v_src = _mm_loadu_si128((const __m128i *)(src1 + x + 4));
                __m128i v_res2 = _mm_or_si128(_mm_cmpgt_epi32(_mm_loadu_si128((const __m128i *)(src2 + x + 4)), v_src),
                    _mm_cmpgt_epi32(v_src, _mm_loadu_si128((const __m128i *)(src3 + x + 4))));

                __m128i v_res = _mm_packs_epi32(_mm_srli_epi32(_mm_andnot_si128(v_res1, v_full), 16),
                                                _mm_srli_epi32(_mm_andnot_si128(v_res2, v_full), 16));
                _mm_storel_epi64((__m128i *)(dst + x), _mm_packus_epi16(v_res, v_zero));
            }
        }

        return x;
    }
};

template <>
struct InRange_SIMD<float>
{
    int operator () (const float * src1, const float * src2, const float * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        if (USE_SSE2)
        {
            __m128i v_zero = _mm_setzero_si128();

            for ( ; x <= len - 8; x += 8 )
            {
                __m128 v_src = _mm_loadu_ps(src1 + x);
                __m128 v_res1 = _mm_and_ps(_mm_cmple_ps(_mm_loadu_ps(src2 + x), v_src),
                    _mm_cmple_ps(v_src, _mm_loadu_ps(src3 + x)));

                v_src = _mm_loadu_ps(src1 + x + 4);
                __m128 v_res2 = _mm_and_ps(_mm_cmple_ps(_mm_loadu_ps(src2 + x + 4), v_src),
                    _mm_cmple_ps(v_src, _mm_loadu_ps(src3 + x + 4)));

                __m128i v_res1i = _mm_cvtps_epi32(v_res1), v_res2i = _mm_cvtps_epi32(v_res2);
                __m128i v_res = _mm_packs_epi32(_mm_srli_epi32(v_res1i, 16), _mm_srli_epi32(v_res2i, 16));
                _mm_storel_epi64((__m128i *)(dst + x), _mm_packus_epi16(v_res, v_zero));
            }
        }

        return x;
    }
};

#elif CC_NEON

template <>
struct InRange_SIMD<uchar>
{
    int operator () (const uchar * src1, const uchar * src2, const uchar * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        for ( ; x <= len - 16; x += 16 )
        {
            uint8x16_t values = vld1q_u8(src1 + x);
            uint8x16_t low = vld1q_u8(src2 + x);
            uint8x16_t high = vld1q_u8(src3 + x);

            vst1q_u8(dst + x, vandq_u8(vcgeq_u8(values, low), vcgeq_u8(high, values)));
        }
        return x;
    }
};

template <>
struct InRange_SIMD<schar>
{
    int operator () (const schar * src1, const schar * src2, const schar * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        for ( ; x <= len - 16; x += 16 )
        {
            int8x16_t values = vld1q_s8(src1 + x);
            int8x16_t low = vld1q_s8(src2 + x);
            int8x16_t high = vld1q_s8(src3 + x);

            vst1q_u8(dst + x, vandq_u8(vcgeq_s8(values, low), vcgeq_s8(high, values)));
        }
        return x;
    }
};

template <>
struct InRange_SIMD<ushort>
{
    int operator () (const ushort * src1, const ushort * src2, const ushort * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        for ( ; x <= len - 16; x += 16 )
        {
            uint16x8_t values = vld1q_u16((const uint16_t*)(src1 + x));
            uint16x8_t low = vld1q_u16((const uint16_t*)(src2 + x));
            uint16x8_t high = vld1q_u16((const uint16_t*)(src3 + x));
            uint8x8_t  r1 = vmovn_u16(vandq_u16(vcgeq_u16(values, low), vcgeq_u16(high, values)));

            values = vld1q_u16((const uint16_t*)(src1 + x + 8));
            low = vld1q_u16((const uint16_t*)(src2 + x + 8));
            high = vld1q_u16((const uint16_t*)(src3 + x + 8));
            uint8x8_t  r2 = vmovn_u16(vandq_u16(vcgeq_u16(values, low), vcgeq_u16(high, values)));

            vst1q_u8(dst + x, vcombine_u8(r1, r2));
        }
        return x;
    }
};

template <>
struct InRange_SIMD<short>
{
    int operator () (const short * src1, const short * src2, const short * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        for ( ; x <= len - 16; x += 16 )
        {
            int16x8_t values = vld1q_s16((const int16_t*)(src1 + x));
            int16x8_t low = vld1q_s16((const int16_t*)(src2 + x));
            int16x8_t high = vld1q_s16((const int16_t*)(src3 + x));
            uint8x8_t r1 = vmovn_u16(vandq_u16(vcgeq_s16(values, low), vcgeq_s16(high, values)));

            values = vld1q_s16((const int16_t*)(src1 + x + 8));
            low = vld1q_s16((const int16_t*)(src2 + x + 8));
            high = vld1q_s16((const int16_t*)(src3 + x + 8));
            uint8x8_t r2 = vmovn_u16(vandq_u16(vcgeq_s16(values, low), vcgeq_s16(high, values)));

            vst1q_u8(dst + x, vcombine_u8(r1, r2));
        }
        return x;
    }
};

template <>
struct InRange_SIMD<int>
{
    int operator () (const int * src1, const int * src2, const int * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        for ( ; x <= len - 8; x += 8 )
        {
            int32x4_t values = vld1q_s32((const int32_t*)(src1 + x));
            int32x4_t low = vld1q_s32((const int32_t*)(src2 + x));
            int32x4_t high = vld1q_s32((const int32_t*)(src3 + x));

            uint16x4_t r1 = vmovn_u32(vandq_u32(vcgeq_s32(values, low), vcgeq_s32(high, values)));

            values = vld1q_s32((const int32_t*)(src1 + x + 4));
            low = vld1q_s32((const int32_t*)(src2 + x + 4));
            high = vld1q_s32((const int32_t*)(src3 + x + 4));

            uint16x4_t r2 = vmovn_u32(vandq_u32(vcgeq_s32(values, low), vcgeq_s32(high, values)));

            uint16x8_t res_16 = vcombine_u16(r1, r2);

            vst1_u8(dst + x, vmovn_u16(res_16));
        }
        return x;
    }
};

template <>
struct InRange_SIMD<float>
{
    int operator () (const float * src1, const float * src2, const float * src3,
                     uchar * dst, int len) const
    {
        int x = 0;

        for ( ; x <= len - 8; x += 8 )
        {
            float32x4_t values = vld1q_f32((const float32_t*)(src1 + x));
            float32x4_t low = vld1q_f32((const float32_t*)(src2 + x));
            float32x4_t high = vld1q_f32((const float32_t*)(src3 + x));

            uint16x4_t r1 = vmovn_u32(vandq_u32(vcgeq_f32(values, low), vcgeq_f32(high, values)));

            values = vld1q_f32((const float32_t*)(src1 + x + 4));
            low = vld1q_f32((const float32_t*)(src2 + x + 4));
            high = vld1q_f32((const float32_t*)(src3 + x + 4));

            uint16x4_t r2 = vmovn_u32(vandq_u32(vcgeq_f32(values, low), vcgeq_f32(high, values)));

            uint16x8_t res_16 = vcombine_u16(r1, r2);

            vst1_u8(dst + x, vmovn_u16(res_16));
        }
        return x;
    }
};

#endif

template <typename T>
static void inRange_(const T* src1, size_t step1, const T* src2, size_t step2,
         const T* src3, size_t step3, uchar* dst, size_t step,
         Size size)
{
    step1 /= sizeof(src1[0]);
    step2 /= sizeof(src2[0]);
    step3 /= sizeof(src3[0]);

    InRange_SIMD<T> vop;

    for( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        int x = vop(src1, src2, src3, dst, size.width);
        #if CC_ENABLE_UNROLLED
        for( ; x <= size.width - 4; x += 4 )
        {
            int t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #endif
        for( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}


static void inRange8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                      const uchar* src3, size_t step3, uchar* dst, size_t step, Size size)
{
    inRange_(src1, step1, src2, step2, src3, step3, dst, step, size);
}

static void inRange8s(const schar* src1, size_t step1, const schar* src2, size_t step2,
                      const schar* src3, size_t step3, uchar* dst, size_t step, Size size)
{
    inRange_(src1, step1, src2, step2, src3, step3, dst, step, size);
}

static void inRange16u(const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                       const ushort* src3, size_t step3, uchar* dst, size_t step, Size size)
{
    inRange_(src1, step1, src2, step2, src3, step3, dst, step, size);
}

static void inRange16s(const short* src1, size_t step1, const short* src2, size_t step2,
                       const short* src3, size_t step3, uchar* dst, size_t step, Size size)
{
    inRange_(src1, step1, src2, step2, src3, step3, dst, step, size);
}

static void inRange32s(const int* src1, size_t step1, const int* src2, size_t step2,
                       const int* src3, size_t step3, uchar* dst, size_t step, Size size)
{
    inRange_(src1, step1, src2, step2, src3, step3, dst, step, size);
}

static void inRange32f(const float* src1, size_t step1, const float* src2, size_t step2,
                       const float* src3, size_t step3, uchar* dst, size_t step, Size size)
{
    inRange_(src1, step1, src2, step2, src3, step3, dst, step, size);
}

static void inRange64f(const double* src1, size_t step1, const double* src2, size_t step2,
                       const double* src3, size_t step3, uchar* dst, size_t step, Size size)
{
    inRange_(src1, step1, src2, step2, src3, step3, dst, step, size);
}

static void inRangeReduce(const uchar* src, uchar* dst, size_t len, int cn)
{
    int k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    if( k == 1 )
        for( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    else if( k == 2 )
        for( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    else if( k == 3 )
        for( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    else
        for( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    for( ; k < cn; k += 4 )
    {
        for( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

typedef void (*InRangeFunc)( const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                             const uchar* src3, size_t step3, uchar* dst, size_t step, Size sz );

static InRangeFunc getInRangeFunc(int depth)
{
    static InRangeFunc inRangeTab[] =
    {
        (InRangeFunc)GET_OPTIMIZED(inRange8u), (InRangeFunc)GET_OPTIMIZED(inRange8s), (InRangeFunc)GET_OPTIMIZED(inRange16u),
        (InRangeFunc)GET_OPTIMIZED(inRange16s), (InRangeFunc)GET_OPTIMIZED(inRange32s), (InRangeFunc)GET_OPTIMIZED(inRange32f),
        (InRangeFunc)inRange64f, 0
    };

    return inRangeTab[depth];
}

#ifdef HAVE_OPENCL

static bool ocl_inRange( const img_t* _src, const img_t* _lowerb,
                         const img_t* _upperb, img_t* _dst )
{
    const ocl::Device & d = ocl::Device::getDefault();
    int skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Size ssize = _cvGetSize(src), lsize = _lowerb.size(), usize = _upperb.size();
    int stype = _src CC_MAT_TYPE(), ltype = _lowerb CC_MAT_TYPE(), utype = _upperb CC_MAT_TYPE();
    int sdepth = CC_MAT_DEPTH(stype), ldepth = CC_MAT_DEPTH(ltype), udepth = CC_MAT_DEPTH(utype);
    int cn = CC_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    bool lbScalar = false, ubScalar = false;

    if( (lkind == _InputArray::MATX && skind != _InputArray::MATX) ||
        ssize != lsize || stype != ltype )
    {
        if( !checkScalar(_lowerb, stype, lkind, skind) )
            CC_Error( CC_StsUnmatchedSizes,
                     "The lower bounary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = true;
    }

    if( (ukind == _InputArray::MATX && skind != _InputArray::MATX) ||
        ssize != usize || stype != utype )
    {
        if( !checkScalar(_upperb, stype, ukind, skind) )
            CC_Error( CC_StsUnmatchedSizes,
                     "The upper bounary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = true;
    }

    if (lbScalar != ubScalar)
        return false;

    bool doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    if ( (!doubleSupport && sdepth == CC_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        return false;

    int kercn = haveScalar ? cn : MAX(MIN(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    if (kercn % cn != 0)
        kercn = cn;
    int colsPerWI = kercn / cn;
    String opts = format("%s-D cn=%d -D srcT=%s -D srcT1=%s -D dstT=%s -D kercn=%d -D depth=%d%s -D colsPerWI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CC_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CC_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    if (ker.empty())
        return false;

    _dst cvSetMat(ssize, CC_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    img_t lscalar, uscalar;

    if (lbScalar && ubScalar)
    {
        lscalar = _lowerb;
        uscalar = _upperb;

        size_t esz = src CC_ELEM_SIZE();
        size_t blocksize = 36;

        CAutoBuffer<uchar> _buf(blocksize*(((int)lbScalar + (int)ubScalar)*esz + cn) + 2*cn*sizeof(int) + 128);
        uchar *buf = cAlignPtr(_buf + blocksize*cn, 16);

        if( ldepth != sdepth && sdepth < CC_32S )
        {
            int* ilbuf = (int*)cAlignPtr(buf + blocksize*esz, 16);
            int* iubuf = ilbuf + cn;

            BinaryFunc sccvtfunc = getConvertFunc(ldepth, CC_32S);
            sccvtfunc(lscalar->tt.data, 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(uscalar->tt.data, 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            int minval = cRound(getMinVal(sdepth)), maxval = cRound(getMaxVal(sdepth));

            for( int k = 0; k < cn; k++ )
            {
                if( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = img_t(cn, 1, CC_32S, ilbuf);
            uscalar = img_t(cn, 1, CC_32S, iubuf);
        }

        lscalar.convertTo(lscalar, stype);
        uscalar.convertTo(uscalar, stype);
    }
    else
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    if (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    else
        ker.args(srcarg, dstarg, ocl::KernelArg::ReadOnlyNoSize(lscalaru),
               ocl::KernelArg::ReadOnlyNoSize(uscalaru), rowsPerWI);

    size_t globalsize[2] = { (size_t)ssize.width / colsPerWI, ((size_t)ssize.height + rowsPerWI - 1) / rowsPerWI };
    return ker.run(2, globalsize, NULL, false);
}

#endif

void inRange(const img_t* _src, const img_t* _lowerb,
                 const img_t* _upperb, img_t* _dst)
{
    CC_INSTRUMENT_REGION()

    CC_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    int skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    img_t *src = _src, lb = _lowerb, ub = _upperb;

    bool lbScalar = false, ubScalar = false;

    if( (lkind == _InputArray::MATX && skind != _InputArray::MATX) ||
        src.size != lb.size || src CC_MAT_TYPE() != lb CC_MAT_TYPE() )
    {
        if( !checkScalar(lb, src CC_MAT_TYPE(), lkind, skind) )
            CC_Error( CC_StsUnmatchedSizes,
                     "The lower bounary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = true;
    }

    if( (ukind == _InputArray::MATX && skind != _InputArray::MATX) ||
        src.size != ub.size || src CC_MAT_TYPE() != ub CC_MAT_TYPE() )
    {
        if( !checkScalar(ub, src CC_MAT_TYPE(), ukind, skind) )
            CC_Error( CC_StsUnmatchedSizes,
                     "The upper bounary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = true;
    }

    CC_Assert(lbScalar == ubScalar);

    int cn = src CC_MAT_CN(), depth = src CC_MAT_DEPTH();

    size_t esz = src CC_ELEM_SIZE();
    size_t blocksize0 = (size_t)(BLOCK_SIZE + esz-1)/esz;

    _dst cvSetMat(src.dims, src.size, CC_8UC1);
    img_t *dst = _dst;
    InRangeFunc func = getInRangeFunc(depth);

    const img_t* arrays_sc[] = { &src, &dst, 0 };
    const img_t* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4];

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t total = it.size, blocksize = MIN(total, blocksize0);

    CAutoBuffer<uchar> _buf(blocksize*(((int)lbScalar + (int)ubScalar)*esz + cn) + 2*cn*sizeof(int) + 128);
    uchar *buf = _buf, *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = cAlignPtr(buf + blocksize*cn, 16);

    if( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = cAlignPtr(buf + blocksize*esz, 16);

        CC_Assert( lb CC_MAT_TYPE() == ub CC_MAT_TYPE() );
        int scdepth = lb CC_MAT_DEPTH();

        if( scdepth != depth && depth < CC_32S )
        {
            int* ilbuf = (int*)cAlignPtr(buf + blocksize*esz, 16);
            int* iubuf = ilbuf + cn;

            BinaryFunc sccvtfunc = getConvertFunc(scdepth, CC_32S);
            sccvtfunc(lb->tt.data, 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(ub->tt.data, 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            int minval = cRound(getMinVal(depth)), maxval = cRound(getMaxVal(depth));

            for( int k = 0; k < cn; k++ )
            {
                if( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = img_t(cn, 1, CC_32S, ilbuf);
            ub = img_t(cn, 1, CC_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src CC_MAT_TYPE(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src CC_MAT_TYPE(), ubuf, blocksize );
    }

    for( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        for( size_t j = 0; j < total; j += blocksize )
        {
            int bsz = (int)MIN(total - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            if( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            if( !ubScalar )
            {
                int idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, Size(bsz*cn, 1));
            if( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}

/****************************************************************************************\
*                                Earlier API: cvAdd etc.                                 *
\****************************************************************************************/

CC_IMPL void
cvNot( const img_t* srcarr, img_t* dstarr )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);
    CC_Assert( src.size == dst.size && src CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    bitwise_not( src, dst );
}


CC_IMPL void
cvAnd( const img_t* srcarr1, const img_t* srcarr2, img_t* dstarr, const img_t* maskarr )
{
    img_t src1 = cvarrToMat(srcarr1), src2 = cvarrToMat(srcarr2),
        dst = cvarrToMat(dstarr), mask;
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    bitwise_and( src1, src2, dst, mask );
}


CC_IMPL void
cvOr( const img_t* srcarr1, const img_t* srcarr2, img_t* dstarr, const img_t* maskarr )
{
    img_t src1 = cvarrToMat(srcarr1), src2 = cvarrToMat(srcarr2),
        dst = cvarrToMat(dstarr), mask;
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    bitwise_or( src1, src2, dst, mask );
}


CC_IMPL void
cvXor( const img_t* srcarr1, const img_t* srcarr2, img_t* dstarr, const img_t* maskarr )
{
    img_t src1 = cvarrToMat(srcarr1), src2 = cvarrToMat(srcarr2),
        dst = cvarrToMat(dstarr), mask;
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    bitwise_xor( src1, src2, dst, mask );
}


CC_IMPL void
cvAndS( const img_t* srcarr, CScalar s, img_t* dstarr, const img_t* maskarr )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr), mask;
    CC_Assert( src.size == dst.size && src CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    bitwise_and( src, (const Scalar&)s, dst, mask );
}


CC_IMPL void
cvOrS( const img_t* srcarr, CScalar s, img_t* dstarr, const img_t* maskarr )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr), mask;
    CC_Assert( src.size == dst.size && src CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    bitwise_or( src, (const Scalar&)s, dst, mask );
}


CC_IMPL void
cvXorS( const img_t* srcarr, CScalar s, img_t* dstarr, const img_t* maskarr )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr), mask;
    CC_Assert( src.size == dst.size && src CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    bitwise_xor( src, (const Scalar&)s, dst, mask );
}


CC_IMPL void cvAdd( const img_t* srcarr1, const img_t* srcarr2, img_t* dstarr, const img_t* maskarr )
{
    img_t src1 = cvarrToMat(srcarr1), src2 = cvarrToMat(srcarr2),
        dst = cvarrToMat(dstarr), mask;
    CC_Assert( src1.size == dst.size && src1 CC_MAT_CN() == dst CC_MAT_CN() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    add( src1, src2, dst, mask, dst CC_MAT_TYPE() );
}


CC_IMPL void cvSub( const img_t* srcarr1, const img_t* srcarr2, img_t* dstarr, const img_t* maskarr )
{
    img_t src1 = cvarrToMat(srcarr1), src2 = cvarrToMat(srcarr2),
        dst = cvarrToMat(dstarr), mask;
    CC_Assert( src1.size == dst.size && src1 CC_MAT_CN() == dst CC_MAT_CN() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    subtract( src1, src2, dst, mask, dst CC_MAT_TYPE() );
}


CC_IMPL void cvAddS( const img_t* srcarr1, CScalar value, img_t* dstarr, const img_t* maskarr )
{
    img_t src1 = cvarrToMat(srcarr1),
        dst = cvarrToMat(dstarr), mask;
    CC_Assert( src1.size == dst.size && src1 CC_MAT_CN() == dst CC_MAT_CN() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    add( src1, (const Scalar&)value, dst, mask, dst CC_MAT_TYPE() );
}


CC_IMPL void cvSubRS( const img_t* srcarr1, CScalar value, img_t* dstarr, const img_t* maskarr )
{
    img_t src1 = cvarrToMat(srcarr1),
        dst = cvarrToMat(dstarr), mask;
    CC_Assert( src1.size == dst.size && src1 CC_MAT_CN() == dst CC_MAT_CN() );
    if( maskarr )
        mask = cvarrToMat(maskarr);
    subtract( (const Scalar&)value, src1, dst, mask, dst CC_MAT_TYPE() );
}


CC_IMPL void cvMul( const img_t* srcarr1, const img_t* srcarr2,
                    img_t* dstarr, double scale )
{
    img_t src1 = cvarrToMat(srcarr1), src2 = cvarrToMat(srcarr2),
        dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && src1 CC_MAT_CN() == dst CC_MAT_CN() );
    multiply( src1, src2, dst, scale, dst CC_MAT_TYPE() );
}


CC_IMPL void cvDiv( const img_t* srcarr1, const img_t* srcarr2,
                    img_t* dstarr, double scale )
{
    img_t src2 = cvarrToMat(srcarr2),
        dst = cvarrToMat(dstarr), mask;
    CC_Assert( src2.size == dst.size && src2 CC_MAT_CN() == dst CC_MAT_CN() );

    if( srcarr1 )
        divide( cvarrToMat(srcarr1), src2, dst, scale, dst CC_MAT_TYPE() );
    else
        divide( scale, src2, dst, dst CC_MAT_TYPE() );
}


CC_IMPL void
cvAddWeighted( const img_t* srcarr1, double alpha,
               const img_t* srcarr2, double beta,
               double gamma, img_t* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), src2 = cvarrToMat(srcarr2),
        dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && src1 CC_MAT_CN() == dst CC_MAT_CN() );
    addWeighted( src1, alpha, src2, beta, gamma, dst, dst CC_MAT_TYPE() );
}


CC_IMPL  void
cvAbsDiff( const img_t* srcarr1, const img_t* srcarr2, img_t* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );

    absdiff( src1, cvarrToMat(srcarr2), dst );
}


CC_IMPL void
cvAbsDiffS( const img_t* srcarr1, img_t* dstarr, CScalar scalar )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );

    absdiff( src1, (const Scalar&)scalar, dst );
}


CC_IMPL void
cvInRange( const void* srcarr1, const void* srcarr2,
           const void* srcarr3, void* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && dst CC_MAT_TYPE() == CC_8U );

    inRange( src1, cvarrToMat(srcarr2), cvarrToMat(srcarr3), dst );
}


CC_IMPL void
cvInRangeS( const void* srcarr1, CScalar lowerb, CScalar upperb, void* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && dst CC_MAT_TYPE() == CC_8U );

    inRange( src1, (const Scalar&)lowerb, (const Scalar&)upperb, dst );
}


CC_IMPL void
cvCmp( const void* srcarr1, const void* srcarr2, void* dstarr, int cmp_op )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && dst CC_MAT_TYPE() == CC_8U );

    compare( src1, cvarrToMat(srcarr2), dst, cmp_op );
}


CC_IMPL void
cvCmpS( const void* srcarr1, double value, void* dstarr, int cmp_op )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && dst CC_MAT_TYPE() == CC_8U );

    compare( src1, value, dst, cmp_op );
}


CC_IMPL void
cvMin( const void* srcarr1, const void* srcarr2, void* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );

    min( src1, cvarrToMat(srcarr2), dst );
}


CC_IMPL void
cvMax( const void* srcarr1, const void* srcarr2, void* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );

    max( src1, cvarrToMat(srcarr2), dst );
}


CC_IMPL void
cvMinS( const void* srcarr1, double value, void* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );

    min( src1, value, dst );
}


CC_IMPL void
cvMaxS( const void* srcarr1, double value, void* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);
    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );

    max( src1, value, dst );
}

//=======================================

#if (ARITHM_USE_IPP == 1)
static inline void fixSteps(int width, int height, size_t elemSize, size_t& step1, size_t& step2, size_t& step)
{
    if( height == 1 )
        step1 = step2 = step = width*elemSize;
}
#define CALL_IPP_BIN_E_12(fun) \
    CC_IPP_CHECK() \
    { \
        fixSteps(width, height, sizeof(dst[0]), step1, step2, step); \
        if (0 <= CC_INSTRUMENT_FUN_IPP(fun, src1, (int)step1, src2, (int)step2, dst, (int)step, ippiSize(width, height), 0)) \
        { \
            CC_IMPL_ADD(CC_IMPL_IPP); \
            return; \
        } \
        setIppErrorStatus(); \
    }

#define CALL_IPP_BIN_E_21(fun) \
    CC_IPP_CHECK() \
    { \
        fixSteps(width, height, sizeof(dst[0]), step1, step2, step); \
        if (0 <= CC_INSTRUMENT_FUN_IPP(fun, src2, (int)step2, src1, (int)step1, dst, (int)step, ippiSize(width, height), 0)) \
        { \
            CC_IMPL_ADD(CC_IMPL_IPP); \
            return; \
        } \
        setIppErrorStatus(); \
    }

#define CALL_IPP_BIN_12(fun) \
    CC_IPP_CHECK() \
    { \
        fixSteps(width, height, sizeof(dst[0]), step1, step2, step); \
        if (0 <= CC_INSTRUMENT_FUN_IPP(fun, src1, (int)step1, src2, (int)step2, dst, (int)step, ippiSize(width, height))) \
        { \
            CC_IMPL_ADD(CC_IMPL_IPP); \
            return; \
        } \
        setIppErrorStatus(); \
    }

#define CALL_IPP_BIN_21(fun) \
    CC_IPP_CHECK() \
    { \
        fixSteps(width, height, sizeof(dst[0]), step1, step2, step); \
        if (0 <= CC_INSTRUMENT_FUN_IPP(fun, src2, (int)step2, src1, (int)step1, dst, (int)step, ippiSize(width, height))) \
        { \
            CC_IMPL_ADD(CC_IMPL_IPP); \
            return; \
        } \
        setIppErrorStatus(); \
    }

#else
#define CALL_IPP_BIN_E_12(fun)
#define CALL_IPP_BIN_E_21(fun)
#define CALL_IPP_BIN_12(fun)
#define CALL_IPP_BIN_21(fun)
#endif


//=======================================
// Add
//=======================================

void add8u( const uchar* src1, size_t step1,
                   const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(add8u, cv_hal_add8u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_E_12(ippiAdd_8u_C1RSfs)
    (vBinOp<uchar, OpAdd<uchar>, IF_SIMD(VAdd<uchar>)>(src1, step1, src2, step2, dst, step, width, height));
}

void add8s( const schar* src1, size_t step1,
                   const schar* src2, size_t step2,
                   schar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(add8s, cv_hal_add8s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp<schar, OpAdd<schar>, IF_SIMD(VAdd<schar>)>(src1, step1, src2, step2, dst, step, width, height);
}

void add16u( const ushort* src1, size_t step1,
                    const ushort* src2, size_t step2,
                    ushort* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(add16u, cv_hal_add16u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_E_12(ippiAdd_16u_C1RSfs)
    (vBinOp<ushort, OpAdd<ushort>, IF_SIMD(VAdd<ushort>)>(src1, step1, src2, step2, dst, step, width, height));
}

void add16s( const short* src1, size_t step1,
                    const short* src2, size_t step2,
                    short* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(add16s, cv_hal_add16s, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_E_12(ippiAdd_16s_C1RSfs)
    (vBinOp<short, OpAdd<short>, IF_SIMD(VAdd<short>)>(src1, step1, src2, step2, dst, step, width, height));
}

void add32s( const int* src1, size_t step1,
                    const int* src2, size_t step2,
                    int* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(add32s, cv_hal_add32s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp32<int, OpAdd<int>, IF_SIMD(VAdd<int>)>(src1, step1, src2, step2, dst, step, width, height);
}

void add32f( const float* src1, size_t step1,
                    const float* src2, size_t step2,
                    float* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(add32f, cv_hal_add32f, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_12(ippiAdd_32f_C1R)
    (vBinOp32<float, OpAdd<float>, IF_SIMD(VAdd<float>)>(src1, step1, src2, step2, dst, step, width, height));
}

void add64f( const double* src1, size_t step1,
                    const double* src2, size_t step2,
                    double* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(add64f, cv_hal_add64f, src1, step1, src2, step2, dst, step, width, height)
    vBinOp64<double, OpAdd<double>, IF_SIMD(VAdd<double>)>(src1, step1, src2, step2, dst, step, width, height);
}

//=======================================
// Subtract
//=======================================

void sub8u( const uchar* src1, size_t step1,
                   const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(sub8u, cv_hal_sub8u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_E_21(ippiSub_8u_C1RSfs)
    (vBinOp<uchar, OpSub<uchar>, IF_SIMD(VSub<uchar>)>(src1, step1, src2, step2, dst, step, width, height));
}

void sub8s( const schar* src1, size_t step1,
                   const schar* src2, size_t step2,
                   schar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(sub8s, cv_hal_sub8s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp<schar, OpSub<schar>, IF_SIMD(VSub<schar>)>(src1, step1, src2, step2, dst, step, width, height);
}

void sub16u( const ushort* src1, size_t step1,
                    const ushort* src2, size_t step2,
                    ushort* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(sub16u, cv_hal_sub16u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_E_21(ippiSub_16u_C1RSfs)
    (vBinOp<ushort, OpSub<ushort>, IF_SIMD(VSub<ushort>)>(src1, step1, src2, step2, dst, step, width, height));
}

void sub16s( const short* src1, size_t step1,
                    const short* src2, size_t step2,
                    short* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(sub16s, cv_hal_sub16s, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_E_21(ippiSub_16s_C1RSfs)
    (vBinOp<short, OpSub<short>, IF_SIMD(VSub<short>)>(src1, step1, src2, step2, dst, step, width, height));
}

void sub32s( const int* src1, size_t step1,
                    const int* src2, size_t step2,
                    int* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(sub32s, cv_hal_sub32s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp32<int, OpSub<int>, IF_SIMD(VSub<int>)>(src1, step1, src2, step2, dst, step, width, height);
}

void sub32f( const float* src1, size_t step1,
                   const float* src2, size_t step2,
                   float* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(sub32f, cv_hal_sub32f, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_21(ippiSub_32f_C1R)
    (vBinOp32<float, OpSub<float>, IF_SIMD(VSub<float>)>(src1, step1, src2, step2, dst, step, width, height));
}

void sub64f( const double* src1, size_t step1,
                    const double* src2, size_t step2,
                    double* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(sub64f, cv_hal_sub64f, src1, step1, src2, step2, dst, step, width, height)
    vBinOp64<double, OpSub<double>, IF_SIMD(VSub<double>)>(src1, step1, src2, step2, dst, step, width, height);
}

//=======================================

#if (ARITHM_USE_IPP == 1)
#define CALL_IPP_MIN_MAX(fun, type) \
    CC_IPP_CHECK() \
    { \
        type* s1 = (type*)src1; \
        type* s2 = (type*)src2; \
        type* d  = dst; \
        fixSteps(width, height, sizeof(dst[0]), step1, step2, step); \
        int i = 0; \
        for(; i < height; i++) \
        { \
            if (0 > CC_INSTRUMENT_FUN_IPP(fun, s1, s2, d, width)) \
                break; \
            s1 = (type*)((uchar*)s1 + step1); \
            s2 = (type*)((uchar*)s2 + step2); \
            d  = (type*)((uchar*)d + step); \
        } \
        if (i == height) \
        { \
            CC_IMPL_ADD(CC_IMPL_IPP); \
            return; \
        } \
        setIppErrorStatus(); \
    }
#else
#define CALL_IPP_MIN_MAX(fun, type)
#endif

//=======================================
// Max
//=======================================

void max8u( const uchar* src1, size_t step1,
                   const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(max8u, cv_hal_max8u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_MIN_MAX(ippsMaxEvery_8u, uchar)
    vBinOp<uchar, OpMax<uchar>, IF_SIMD(VMax<uchar>)>(src1, step1, src2, step2, dst, step, width, height);
}

void max8s( const schar* src1, size_t step1,
                   const schar* src2, size_t step2,
                   schar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(max8s, cv_hal_max8s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp<schar, OpMax<schar>, IF_SIMD(VMax<schar>)>(src1, step1, src2, step2, dst, step, width, height);
}

void max16u( const ushort* src1, size_t step1,
                    const ushort* src2, size_t step2,
                    ushort* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(max16u, cv_hal_max16u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_MIN_MAX(ippsMaxEvery_16u, ushort)
    vBinOp<ushort, OpMax<ushort>, IF_SIMD(VMax<ushort>)>(src1, step1, src2, step2, dst, step, width, height);
}

void max16s( const short* src1, size_t step1,
                    const short* src2, size_t step2,
                    short* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(max16s, cv_hal_max16s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp<short, OpMax<short>, IF_SIMD(VMax<short>)>(src1, step1, src2, step2, dst, step, width, height);
}

void max32s( const int* src1, size_t step1,
                    const int* src2, size_t step2,
                    int* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(max32s, cv_hal_max32s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp32<int, OpMax<int>, IF_SIMD(VMax<int>)>(src1, step1, src2, step2, dst, step, width, height);
}

void max32f( const float* src1, size_t step1,
                    const float* src2, size_t step2,
                    float* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(max32f, cv_hal_max32f, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_MIN_MAX(ippsMaxEvery_32f, float)
    vBinOp32<float, OpMax<float>, IF_SIMD(VMax<float>)>(src1, step1, src2, step2, dst, step, width, height);
}

void max64f( const double* src1, size_t step1,
                    const double* src2, size_t step2,
                    double* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(max64f, cv_hal_max64f, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_MIN_MAX(ippsMaxEvery_64f, double)
    vBinOp64<double, OpMax<double>, IF_SIMD(VMax<double>)>(src1, step1, src2, step2, dst, step, width, height);
}

//=======================================
// Min
//=======================================

void min8u( const uchar* src1, size_t step1,
                   const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(min8u, cv_hal_min8u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_MIN_MAX(ippsMinEvery_8u, uchar)
    vBinOp<uchar, OpMin<uchar>, IF_SIMD(VMin<uchar>)>(src1, step1, src2, step2, dst, step, width, height);
}

void min8s( const schar* src1, size_t step1,
                   const schar* src2, size_t step2,
                   schar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(min8s, cv_hal_min8s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp<schar, OpMin<schar>, IF_SIMD(VMin<schar>)>(src1, step1, src2, step2, dst, step, width, height);
}

void min16u( const ushort* src1, size_t step1,
                    const ushort* src2, size_t step2,
                    ushort* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(min16u, cv_hal_min16u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_MIN_MAX(ippsMinEvery_16u, ushort)
    vBinOp<ushort, OpMin<ushort>, IF_SIMD(VMin<ushort>)>(src1, step1, src2, step2, dst, step, width, height);
}

void min16s( const short* src1, size_t step1,
                    const short* src2, size_t step2,
                    short* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(min16s, cv_hal_min16s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp<short, OpMin<short>, IF_SIMD(VMin<short>)>(src1, step1, src2, step2, dst, step, width, height);
}

void min32s( const int* src1, size_t step1,
                    const int* src2, size_t step2,
                    int* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(min32s, cv_hal_min32s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp32<int, OpMin<int>, IF_SIMD(VMin<int>)>(src1, step1, src2, step2, dst, step, width, height);
}

void min32f( const float* src1, size_t step1,
                    const float* src2, size_t step2,
                    float* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(min32f, cv_hal_min32f, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_MIN_MAX(ippsMinEvery_32f, float)
    vBinOp32<float, OpMin<float>, IF_SIMD(VMin<float>)>(src1, step1, src2, step2, dst, step, width, height);
}

void min64f( const double* src1, size_t step1,
                    const double* src2, size_t step2,
                    double* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(min64f, cv_hal_min64f, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_MIN_MAX(ippsMinEvery_64f, double)
    vBinOp64<double, OpMin<double>, IF_SIMD(VMin<double>)>(src1, step1, src2, step2, dst, step, width, height);
}

//=======================================
// AbsDiff
//=======================================

void absdiff8u( const uchar* src1, size_t step1,
                       const uchar* src2, size_t step2,
                       uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(absdiff8u, cv_hal_absdiff8u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_12(ippiAbsDiff_8u_C1R)
    (vBinOp<uchar, OpAbsDiff<uchar>, IF_SIMD(VAbsDiff<uchar>)>(src1, step1, src2, step2, dst, step, width, height));
}

void absdiff8s( const schar* src1, size_t step1,
                       const schar* src2, size_t step2,
                       schar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(absdiff8s, cv_hal_absdiff8s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp<schar, OpAbsDiff<schar>, IF_SIMD(VAbsDiff<schar>)>(src1, step1, src2, step2, dst, step, width, height);
}

void absdiff16u( const ushort* src1, size_t step1,
                        const ushort* src2, size_t step2,
                        ushort* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(absdiff16u, cv_hal_absdiff16u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_12(ippiAbsDiff_16u_C1R)
    (vBinOp<ushort, OpAbsDiff<ushort>, IF_SIMD(VAbsDiff<ushort>)>(src1, step1, src2, step2, dst, step, width, height));
}

void absdiff16s( const short* src1, size_t step1,
                        const short* src2, size_t step2,
                        short* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(absdiff16s, cv_hal_absdiff16s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp<short, OpAbsDiff<short>, IF_SIMD(VAbsDiff<short>)>(src1, step1, src2, step2, dst, step, width, height);
}

void absdiff32s( const int* src1, size_t step1,
                        const int* src2, size_t step2,
                        int* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(absdiff32s, cv_hal_absdiff32s, src1, step1, src2, step2, dst, step, width, height)
    vBinOp32<int, OpAbsDiff<int>, IF_SIMD(VAbsDiff<int>)>(src1, step1, src2, step2, dst, step, width, height);
}

void absdiff32f( const float* src1, size_t step1,
                        const float* src2, size_t step2,
                        float* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(absdiff32f, cv_hal_absdiff32f, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_12(ippiAbsDiff_32f_C1R)
    (vBinOp32<float, OpAbsDiff<float>, IF_SIMD(VAbsDiff<float>)>(src1, step1, src2, step2, dst, step, width, height));
}

void absdiff64f( const double* src1, size_t step1,
                        const double* src2, size_t step2,
                        double* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(absdiff64f, cv_hal_absdiff64f, src1, step1, src2, step2, dst, step, width, height)
    vBinOp64<double, OpAbsDiff<double>, IF_SIMD(VAbsDiff<double>)>(src1, step1, src2, step2, dst, step, width, height);
}

//=======================================
// Logical
//=======================================

#if (ARITHM_USE_IPP == 1)
#define CALL_IPP_UN(fun) \
    CC_IPP_CHECK() \
    { \
        fixSteps(width, height, sizeof(dst[0]), step1, step2, step); (void)src2; \
        if (0 <= CC_INSTRUMENT_FUN_IPP(fun, src1, (int)step1, dst, (int)step, ippiSize(width, height))) \
        { \
            CC_IMPL_ADD(CC_IMPL_IPP); \
            return; \
        } \
        setIppErrorStatus(); \
    }
#else
#define CALL_IPP_UN(fun)
#endif

void and8u( const uchar* src1, size_t step1,
                   const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(and8u, cv_hal_and8u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_12(ippiAnd_8u_C1R)
    (vBinOp<uchar, OpAnd<uchar>, IF_SIMD(VAnd<uchar>)>(src1, step1, src2, step2, dst, step, width, height));
}

void or8u( const uchar* src1, size_t step1,
                  const uchar* src2, size_t step2,
                  uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(or8u, cv_hal_or8u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_12(ippiOr_8u_C1R)
    (vBinOp<uchar, OpOr<uchar>, IF_SIMD(VOr<uchar>)>(src1, step1, src2, step2, dst, step, width, height));
}

void xor8u( const uchar* src1, size_t step1,
                   const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(xor8u, cv_hal_xor8u, src1, step1, src2, step2, dst, step, width, height)
    CALL_IPP_BIN_12(ippiXor_8u_C1R)
    (vBinOp<uchar, OpXor<uchar>, IF_SIMD(VXor<uchar>)>(src1, step1, src2, step2, dst, step, width, height));
}

void not8u( const uchar* src1, size_t step1,
                   const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* )
{
    CALL_HAL(not8u, cv_hal_not8u, src1, step1, dst, step, width, height)
    CALL_IPP_UN(ippiNot_8u_C1R)
    (vBinOp<uchar, OpNot<uchar>, IF_SIMD(VNot<uchar>)>(src1, step1, src2, step2, dst, step, width, height));
}

//=======================================

#if ARITHM_USE_IPP
inline static IppCmpOp convert_cmp(int _cmpop)
{
    return _cmpop == CMP_EQ ? ippCmpEq :
        _cmpop == CMP_GT ? ippCmpGreater :
        _cmpop == CMP_GE ? ippCmpGreaterEq :
        _cmpop == CMP_LT ? ippCmpLess :
        _cmpop == CMP_LE ? ippCmpLessEq :
        (IppCmpOp)-1;
}
#define CALL_IPP_CMP(fun) \
    CC_IPP_CHECK() \
    { \
        IppCmpOp op = convert_cmp(*(int *)_cmpop); \
        if( op  >= 0 ) \
        { \
            fixSteps(width, height, sizeof(dst[0]), step1, step2, step); \
            if (0 <= CC_INSTRUMENT_FUN_IPP(fun, src1, (int)step1, src2, (int)step2, dst, (int)step, ippiSize(width, height), op)) \
            { \
                CC_IMPL_ADD(CC_IMPL_IPP); \
                return; \
            } \
            setIppErrorStatus(); \
        } \
    }
#else
#define CALL_IPP_CMP(fun)
#endif

//=======================================
// Compare
//=======================================

void cmp8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                  uchar* dst, size_t step, int width, int height, void* _cmpop)
{
    CALL_HAL(cmp8u, cv_hal_cmp8u, src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop)
    CALL_IPP_CMP(ippiCompare_8u_C1R)
  //vz optimized  cmp_(src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop);
    int code = *(int*)_cmpop;
    step1 /= sizeof(src1[0]);
    step2 /= sizeof(src2[0]);
    if( code == CMP_GE || code == CMP_LT )
    {
        T_SWAP(_Tp, src1, src2);
        T_SWAP(_Tp, step1, step2);
        code = code == CMP_GE ? CMP_LE : CMP_GT;
    }

    if( code == CMP_GT || code == CMP_LE )
    {
        int m = code == CMP_GT ? 0 : 255;
        for( ; height--; src1 += step1, src2 += step2, dst += step )
        {
            int x =0;
            #if CC_SSE2
            if( USE_SSE2 )
            {
                __m128i m128 = code == CMP_GT ? _mm_setzero_si128() : _mm_set1_epi8 (-1);
                __m128i c128 = _mm_set1_epi8 (-128);
                for( ; x <= width - 16; x += 16 )
                {
                    __m128i r00 = _mm_loadu_si128((const __m128i*)(src1 + x));
                    __m128i r10 = _mm_loadu_si128((const __m128i*)(src2 + x));
                    // no simd for 8u comparison, that's why we need the trick
                    r00 = _mm_sub_epi8(r00,c128);
                    r10 = _mm_sub_epi8(r10,c128);

                    r00 =_mm_xor_si128(_mm_cmpgt_epi8(r00, r10), m128);
                    _mm_storeu_si128((__m128i*)(dst + x),r00);

                }
            }
            #elif CC_NEON
            uint8x16_t mask = code == CMP_GT ? vdupq_n_u8(0) : vdupq_n_u8(255);

            for( ; x <= width - 16; x += 16 )
            {
                vst1q_u8(dst+x, veorq_u8(vcgtq_u8(vld1q_u8(src1+x), vld1q_u8(src2+x)), mask));
            }

           #endif

            for( ; x < width; x++ ){
                dst[x] = (uchar)(-(src1[x] > src2[x]) ^ m);
            }
        }
    }
    else if( code == CMP_EQ || code == CMP_NE )
    {
        int m = code == CMP_EQ ? 0 : 255;
        for( ; height--; src1 += step1, src2 += step2, dst += step )
        {
            int x = 0;
            #if CC_SSE2
            if( USE_SSE2 )
            {
                __m128i m128 =  code == CMP_EQ ? _mm_setzero_si128() : _mm_set1_epi8 (-1);
                for( ; x <= width - 16; x += 16 )
                {
                    __m128i r00 = _mm_loadu_si128((const __m128i*)(src1 + x));
                    __m128i r10 = _mm_loadu_si128((const __m128i*)(src2 + x));
                    r00 = _mm_xor_si128 ( _mm_cmpeq_epi8 (r00, r10), m128);
                    _mm_storeu_si128((__m128i*)(dst + x), r00);
                }
            }
            #elif CC_NEON
            uint8x16_t mask = code == CMP_EQ ? vdupq_n_u8(0) : vdupq_n_u8(255);

            for( ; x <= width - 16; x += 16 )
            {
                vst1q_u8(dst+x, veorq_u8(vceqq_u8(vld1q_u8(src1+x), vld1q_u8(src2+x)), mask));
            }
           #endif
           for( ; x < width; x++ )
                dst[x] = (uchar)(-(src1[x] == src2[x]) ^ m);
        }
    }
}

void cmp8s(const schar* src1, size_t step1, const schar* src2, size_t step2,
                  uchar* dst, size_t step, int width, int height, void* _cmpop)
{
    CALL_HAL(cmp8s, cv_hal_cmp8s, src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop)
    cmp_(src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop);
}

void cmp16u(const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                  uchar* dst, size_t step, int width, int height, void* _cmpop)
{
    CALL_HAL(cmp16u, cv_hal_cmp16u, src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop)
    CALL_IPP_CMP(ippiCompare_16u_C1R)
    cmp_(src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop);
}

void cmp16s(const short* src1, size_t step1, const short* src2, size_t step2,
                  uchar* dst, size_t step, int width, int height, void* _cmpop)
{
    CALL_HAL(cmp16s, cv_hal_cmp16s, src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop)
    CALL_IPP_CMP(ippiCompare_16s_C1R)
   //vz optimized cmp_(src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop);

    int code = *(int*)_cmpop;
    step1 /= sizeof(src1[0]);
    step2 /= sizeof(src2[0]);
    if( code == CMP_GE || code == CMP_LT )
    {
        T_SWAP(_Tp, src1, src2);
        T_SWAP(_Tp, step1, step2);
        code = code == CMP_GE ? CMP_LE : CMP_GT;
    }

    if( code == CMP_GT || code == CMP_LE )
    {
        int m = code == CMP_GT ? 0 : 255;
        for( ; height--; src1 += step1, src2 += step2, dst += step )
        {
            int x =0;
            #if CC_SSE2
            if( USE_SSE2)
            {
                __m128i m128 =  code == CMP_GT ? _mm_setzero_si128() : _mm_set1_epi16 (-1);
                for( ; x <= width - 16; x += 16 )
                {
                    __m128i r00 = _mm_loadu_si128((const __m128i*)(src1 + x));
                    __m128i r10 = _mm_loadu_si128((const __m128i*)(src2 + x));
                    r00 = _mm_xor_si128 ( _mm_cmpgt_epi16 (r00, r10), m128);
                    __m128i r01 = _mm_loadu_si128((const __m128i*)(src1 + x + 8));
                    __m128i r11 = _mm_loadu_si128((const __m128i*)(src2 + x + 8));
                    r01 = _mm_xor_si128 ( _mm_cmpgt_epi16 (r01, r11), m128);
                    r11 = _mm_packs_epi16(r00, r01);
                    _mm_storeu_si128((__m128i*)(dst + x), r11);
                }
                if( x <= width-8)
                {
                    __m128i r00 = _mm_loadu_si128((const __m128i*)(src1 + x));
                    __m128i r10 = _mm_loadu_si128((const __m128i*)(src2 + x));
                    r00 = _mm_xor_si128 ( _mm_cmpgt_epi16 (r00, r10), m128);
                    r10 = _mm_packs_epi16(r00, r00);
                    _mm_storel_epi64((__m128i*)(dst + x), r10);

                    x += 8;
                }
            }
            #elif CC_NEON
            uint8x16_t mask = code == CMP_GT ? vdupq_n_u8(0) : vdupq_n_u8(255);

            for( ; x <= width - 16; x += 16 )
            {
                int16x8_t in1 = vld1q_s16(src1 + x);
                int16x8_t in2 = vld1q_s16(src2 + x);
                uint8x8_t t1 = vmovn_u16(vcgtq_s16(in1, in2));

                in1 = vld1q_s16(src1 + x + 8);
                in2 = vld1q_s16(src2 + x + 8);
                uint8x8_t t2 = vmovn_u16(vcgtq_s16(in1, in2));

                vst1q_u8(dst+x, veorq_u8(vcombine_u8(t1, t2), mask));
            }
            #endif

            for( ; x < width; x++ ){
                 dst[x] = (uchar)(-(src1[x] > src2[x]) ^ m);
            }
        }
    }
    else if( code == CMP_EQ || code == CMP_NE )
    {
        int m = code == CMP_EQ ? 0 : 255;
        for( ; height--; src1 += step1, src2 += step2, dst += step )
        {
            int x = 0;
            #if CC_SSE2
            if( USE_SSE2 )
            {
                __m128i m128 =  code == CMP_EQ ? _mm_setzero_si128() : _mm_set1_epi16 (-1);
                for( ; x <= width - 16; x += 16 )
                {
                    __m128i r00 = _mm_loadu_si128((const __m128i*)(src1 + x));
                    __m128i r10 = _mm_loadu_si128((const __m128i*)(src2 + x));
                    r00 = _mm_xor_si128 ( _mm_cmpeq_epi16 (r00, r10), m128);
                    __m128i r01 = _mm_loadu_si128((const __m128i*)(src1 + x + 8));
                    __m128i r11 = _mm_loadu_si128((const __m128i*)(src2 + x + 8));
                    r01 = _mm_xor_si128 ( _mm_cmpeq_epi16 (r01, r11), m128);
                    r11 = _mm_packs_epi16(r00, r01);
                    _mm_storeu_si128((__m128i*)(dst + x), r11);
                }
                if( x <= width - 8)
                {
                    __m128i r00 = _mm_loadu_si128((const __m128i*)(src1 + x));
                    __m128i r10 = _mm_loadu_si128((const __m128i*)(src2 + x));
                    r00 = _mm_xor_si128 ( _mm_cmpeq_epi16 (r00, r10), m128);
                    r10 = _mm_packs_epi16(r00, r00);
                    _mm_storel_epi64((__m128i*)(dst + x), r10);

                    x += 8;
                }
            }
            #elif CC_NEON
            uint8x16_t mask = code == CMP_EQ ? vdupq_n_u8(0) : vdupq_n_u8(255);

            for( ; x <= width - 16; x += 16 )
            {
                int16x8_t in1 = vld1q_s16(src1 + x);
                int16x8_t in2 = vld1q_s16(src2 + x);
                uint8x8_t t1 = vmovn_u16(vceqq_s16(in1, in2));

                in1 = vld1q_s16(src1 + x + 8);
                in2 = vld1q_s16(src2 + x + 8);
                uint8x8_t t2 = vmovn_u16(vceqq_s16(in1, in2));

                vst1q_u8(dst+x, veorq_u8(vcombine_u8(t1, t2), mask));
            }
            #endif
            for( ; x < width; x++ )
                dst[x] = (uchar)(-(src1[x] == src2[x]) ^ m);
        }
    }
}

void cmp32s(const int* src1, size_t step1, const int* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* _cmpop)
{
    CALL_HAL(cmp32s, cv_hal_cmp32s, src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop)
    cmp_(src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop);
}

void cmp32f(const float* src1, size_t step1, const float* src2, size_t step2,
                  uchar* dst, size_t step, int width, int height, void* _cmpop)
{
    CALL_HAL(cmp32f, cv_hal_cmp32f, src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop)
    CALL_IPP_CMP(ippiCompare_32f_C1R)
    cmp_(src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop);
}

void cmp64f(const double* src1, size_t step1, const double* src2, size_t step2,
                  uchar* dst, size_t step, int width, int height, void* _cmpop)
{
    CALL_HAL(cmp64f, cv_hal_cmp64f, src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop)
    cmp_(src1, step1, src2, step2, dst, step, width, height, *(int*)_cmpop);
}

//=======================================

#if defined HAVE_IPP
#define CALL_IPP_MUL(fun) \
    CC_IPP_CHECK() \
    { \
        if (std::fabs(fscale - 1) <= FLT_EPSILON) \
        { \
            if (CC_INSTRUMENT_FUN_IPP(fun, src1, (int)step1, src2, (int)step2, dst, (int)step, ippiSize(width, height), 0) >= 0) \
            { \
                CC_IMPL_ADD(CC_IMPL_IPP); \
                return; \
            } \
            setIppErrorStatus(); \
        } \
    }

#define CALL_IPP_MUL_2(fun) \
    CC_IPP_CHECK() \
    { \
        if (std::fabs(fscale - 1) <= FLT_EPSILON) \
        { \
            if (CC_INSTRUMENT_FUN_IPP(fun, src1, (int)step1, src2, (int)step2, dst, (int)step, ippiSize(width, height)) >= 0) \
            { \
                CC_IMPL_ADD(CC_IMPL_IPP); \
                return; \
            } \
            setIppErrorStatus(); \
        } \
    }

#else
#define CALL_IPP_MUL(fun)
#define CALL_IPP_MUL_2(fun)
#endif

//=======================================
// Multilpy
//=======================================

void mul8u( const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(mul8u, cv_hal_mul8u, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    float fscale = (float)*(const double*)scale;
    CALL_IPP_MUL(ippiMul_8u_C1RSfs)
    mul_(src1, step1, src2, step2, dst, step, width, height, fscale);
}

void mul8s( const schar* src1, size_t step1, const schar* src2, size_t step2,
                   schar* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(mul8s, cv_hal_mul8s, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    mul_(src1, step1, src2, step2, dst, step, width, height, (float)*(const double*)scale);
}

void mul16u( const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                    ushort* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(mul16u, cv_hal_mul16u, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    float fscale = (float)*(const double*)scale;
    CALL_IPP_MUL(ippiMul_16u_C1RSfs)
    mul_(src1, step1, src2, step2, dst, step, width, height, fscale);
}

void mul16s( const short* src1, size_t step1, const short* src2, size_t step2,
                    short* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(mul16s, cv_hal_mul16s, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    float fscale = (float)*(const double*)scale;
    CALL_IPP_MUL(ippiMul_16s_C1RSfs)
    mul_(src1, step1, src2, step2, dst, step, width, height, fscale);
}

void mul32s( const int* src1, size_t step1, const int* src2, size_t step2,
                    int* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(mul32s, cv_hal_mul32s, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    mul_(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
}

void mul32f( const float* src1, size_t step1, const float* src2, size_t step2,
                    float* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(mul32f, cv_hal_mul32f, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    float fscale = (float)*(const double*)scale;
    CALL_IPP_MUL_2(ippiMul_32f_C1R)
    mul_(src1, step1, src2, step2, dst, step, width, height, fscale);
}

void mul64f( const double* src1, size_t step1, const double* src2, size_t step2,
                    double* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(mul64f, cv_hal_mul64f, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    mul_(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
}

//=======================================
// Divide
//=======================================

void div8u( const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                   uchar* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(div8u, cv_hal_div8u, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    if( src1 )
        div_i(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
    else
        recip_i(src2, step2, dst, step, width, height, *(const double*)scale);
}

void div8s( const schar* src1, size_t step1, const schar* src2, size_t step2,
                  schar* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(div8s, cv_hal_div8s, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    div_i(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
}

void div16u( const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                    ushort* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(div16u, cv_hal_div16u, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    div_i(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
}

void div16s( const short* src1, size_t step1, const short* src2, size_t step2,
                    short* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(div16s, cv_hal_div16s, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    div_i(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
}

void div32s( const int* src1, size_t step1, const int* src2, size_t step2,
                    int* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(div32s, cv_hal_div32s, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    div_i(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
}

void div32f( const float* src1, size_t step1, const float* src2, size_t step2,
                    float* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(div32f, cv_hal_div32f, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    div_f(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
}

void div64f( const double* src1, size_t step1, const double* src2, size_t step2,
                    double* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(div64f, cv_hal_div64f, src1, step1, src2, step2, dst, step, width, height, *(const double*)scale)
    div_f(src1, step1, src2, step2, dst, step, width, height, *(const double*)scale);
}

//=======================================
// Reciprocial
//=======================================

void recip8u( const uchar*, size_t, const uchar* src2, size_t step2,
                  uchar* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(recip8u, cv_hal_recip8u, src2, step2, dst, step, width, height, *(const double*)scale)
    recip_i(src2, step2, dst, step, width, height, *(const double*)scale);
}

void recip8s( const schar*, size_t, const schar* src2, size_t step2,
                  schar* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(recip8s, cv_hal_recip8s, src2, step2, dst, step, width, height, *(const double*)scale)
    recip_i(src2, step2, dst, step, width, height, *(const double*)scale);
}

void recip16u( const ushort*, size_t, const ushort* src2, size_t step2,
                   ushort* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(recip16u, cv_hal_recip16u, src2, step2, dst, step, width, height, *(const double*)scale)
    recip_i(src2, step2, dst, step, width, height, *(const double*)scale);
}

void recip16s( const short*, size_t, const short* src2, size_t step2,
                   short* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(recip16s, cv_hal_recip16s, src2, step2, dst, step, width, height, *(const double*)scale)
    recip_i(src2, step2, dst, step, width, height, *(const double*)scale);
}

void recip32s( const int*, size_t, const int* src2, size_t step2,
                   int* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(recip32s, cv_hal_recip32s, src2, step2, dst, step, width, height, *(const double*)scale)
    recip_i(src2, step2, dst, step, width, height, *(const double*)scale);
}

void recip32f( const float*, size_t, const float* src2, size_t step2,
                   float* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(recip32f, cv_hal_recip32f, src2, step2, dst, step, width, height, *(const double*)scale)
    recip_f(src2, step2, dst, step, width, height, *(const double*)scale);
}

void recip64f( const double*, size_t, const double* src2, size_t step2,
                   double* dst, size_t step, int width, int height, void* scale)
{
    CALL_HAL(recip64f, cv_hal_recip64f, src2, step2, dst, step, width, height, *(const double*)scale)
    recip_f(src2, step2, dst, step, width, height, *(const double*)scale);
}

//=======================================
// Add weighted
//=======================================

void
addWeighted8u( const uchar* src1, size_t step1,
               const uchar* src2, size_t step2,
               uchar* dst, size_t step, int width, int height,
               void* scalars )
{
    CALL_HAL(addWeighted8u, cv_hal_addWeighted8u, src1, step1, src2, step2, dst, step, width, height, (const double*)scalars)
    const double* scalars_ = (const double*)scalars;
    float alpha = (float)scalars_[0], beta = (float)scalars_[1], gamma = (float)scalars_[2];

    for( ; height--; src1 += step1, src2 += step2, dst += step )
    {
        int x = 0;

#if CC_SSE2
        if( USE_SSE2 )
        {
            __m128 a4 = _mm_set1_ps(alpha), b4 = _mm_set1_ps(beta), g4 = _mm_set1_ps(gamma);
            __m128i z = _mm_setzero_si128();

            for( ; x <= width - 8; x += 8 )
            {
                __m128i u = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i*)(src1 + x)), z);
                __m128i v = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i*)(src2 + x)), z);

                __m128 u0 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(u, z));
                __m128 u1 = _mm_cvtepi32_ps(_mm_unpackhi_epi16(u, z));
                __m128 v0 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(v, z));
                __m128 v1 = _mm_cvtepi32_ps(_mm_unpackhi_epi16(v, z));

                u0 = _mm_add_ps(_mm_mul_ps(u0, a4), _mm_mul_ps(v0, b4));
                u1 = _mm_add_ps(_mm_mul_ps(u1, a4), _mm_mul_ps(v1, b4));
                u0 = _mm_add_ps(u0, g4); u1 = _mm_add_ps(u1, g4);

                u = _mm_packs_epi32(_mm_cvtps_epi32(u0), _mm_cvtps_epi32(u1));
                u = _mm_packus_epi16(u, u);

                _mm_storel_epi64((__m128i*)(dst + x), u);
            }
        }
#elif CC_NEON
        float32x4_t g = vdupq_n_f32 (gamma);

        for( ; x <= width - 8; x += 8 )
        {
            uint8x8_t in1 = vld1_u8(src1+x);
            uint16x8_t in1_16 = vmovl_u8(in1);
            float32x4_t in1_f_l = vcvtq_f32_u32(vmovl_u16(vget_low_u16(in1_16)));
            float32x4_t in1_f_h = vcvtq_f32_u32(vmovl_u16(vget_high_u16(in1_16)));

            uint8x8_t in2 = vld1_u8(src2+x);
            uint16x8_t in2_16 = vmovl_u8(in2);
            float32x4_t in2_f_l = vcvtq_f32_u32(vmovl_u16(vget_low_u16(in2_16)));
            float32x4_t in2_f_h = vcvtq_f32_u32(vmovl_u16(vget_high_u16(in2_16)));

            float32x4_t out_f_l = vaddq_f32(vmulq_n_f32(in1_f_l, alpha), vmulq_n_f32(in2_f_l, beta));
            float32x4_t out_f_h = vaddq_f32(vmulq_n_f32(in1_f_h, alpha), vmulq_n_f32(in2_f_h, beta));
            out_f_l = vaddq_f32(out_f_l, g);
            out_f_h = vaddq_f32(out_f_h, g);

            uint16x4_t out_16_l = vqmovun_s32(cv_vrndq_s32_f32(out_f_l));
            uint16x4_t out_16_h = vqmovun_s32(cv_vrndq_s32_f32(out_f_h));

            uint16x8_t out_16 = vcombine_u16(out_16_l, out_16_h);
            uint8x8_t out = vqmovn_u16(out_16);

            vst1_u8(dst+x, out);
        }
#endif
        #if CC_ENABLE_UNROLLED
        for( ; x <= width - 4; x += 4 )
        {
            float t0, t1;
            t0 = CC_8TO32F(src1[x])*alpha + CC_8TO32F(src2[x])*beta + gamma;
            t1 = CC_8TO32F(src1[x+1])*alpha + CC_8TO32F(src2[x+1])*beta + gamma;

            dst[x] = saturate_cast<uchar>(t0);
            dst[x+1] = saturate_cast<uchar>(t1);

            t0 = CC_8TO32F(src1[x+2])*alpha + CC_8TO32F(src2[x+2])*beta + gamma;
            t1 = CC_8TO32F(src1[x+3])*alpha + CC_8TO32F(src2[x+3])*beta + gamma;

            dst[x+2] = saturate_cast<uchar>(t0);
            dst[x+3] = saturate_cast<uchar>(t1);
        }
        #endif

        for( ; x < width; x++ )
        {
            float t0 = CC_8TO32F(src1[x])*alpha + CC_8TO32F(src2[x])*beta + gamma;
            dst[x] = saturate_cast<uchar>(t0);
        }
    }
}

void addWeighted8s( const schar* src1, size_t step1, const schar* src2, size_t step2,
                           schar* dst, size_t step, int width, int height, void* scalars )
{
    CALL_HAL(addWeighted8s, cv_hal_addWeighted8s, src1, step1, src2, step2, dst, step, width, height, (const double*)scalars)
    addWeighted_<schar, float>(src1, step1, src2, step2, dst, step, width, height, scalars);
}

void addWeighted16u( const ushort* src1, size_t step1, const ushort* src2, size_t step2,
                            ushort* dst, size_t step, int width, int height, void* scalars )
{
    CALL_HAL(addWeighted16u, cv_hal_addWeighted16u, src1, step1, src2, step2, dst, step, width, height, (const double*)scalars)
    addWeighted_<ushort, float>(src1, step1, src2, step2, dst, step, width, height, scalars);
}

void addWeighted16s( const short* src1, size_t step1, const short* src2, size_t step2,
                            short* dst, size_t step, int width, int height, void* scalars )
{
    CALL_HAL(addWeighted16s, cv_hal_addWeighted16s, src1, step1, src2, step2, dst, step, width, height, (const double*)scalars)
    addWeighted_<short, float>(src1, step1, src2, step2, dst, step, width, height, scalars);
}

void addWeighted32s( const int* src1, size_t step1, const int* src2, size_t step2,
                            int* dst, size_t step, int width, int height, void* scalars )
{
    CALL_HAL(addWeighted32s, cv_hal_addWeighted32s, src1, step1, src2, step2, dst, step, width, height, (const double*)scalars)
    addWeighted_<int, double>(src1, step1, src2, step2, dst, step, width, height, scalars);
}

void addWeighted32f( const float* src1, size_t step1, const float* src2, size_t step2,
                            float* dst, size_t step, int width, int height, void* scalars )
{
    CALL_HAL(addWeighted32f, cv_hal_addWeighted32f, src1, step1, src2, step2, dst, step, width, height, (const double*)scalars)
    addWeighted_<float, double>(src1, step1, src2, step2, dst, step, width, height, scalars);
}

void addWeighted64f( const double* src1, size_t step1, const double* src2, size_t step2,
                            double* dst, size_t step, int width, int height, void* scalars )
{
    CALL_HAL(addWeighted64f, cv_hal_addWeighted64f, src1, step1, src2, step2, dst, step, width, height, (const double*)scalars)
    addWeighted_<double, double>(src1, step1, src2, step2, dst, step, width, height, scalars);
}

