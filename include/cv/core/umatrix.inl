/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2014, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/


#include "opencl_kernels_core.hpp"

///////////////////////////////// UMat implementation ///////////////////////////////

namespace cv {

// forward decls, implementation is below in this file
void setSize(UMat& m, int _dims, const int* _sz, const size_t* _steps,
             bool autoSteps = false);

void updateContinuityFlag(UMat& m);
void finalizeHdr(UMat& m);

// it should be a prime number for the best hash function
enum { UMAT_NLOCKS = 31 };
static Mutex umatLocks[UMAT_NLOCKS];

UMatData::UMatData(const MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = 0;
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = NULL;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CC_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    flags = 0;
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    if (originalUMatData)
    {
        UMatData* u = originalUMatData;
        CC_XADD(&(u->urefcount), -1);
        CC_XADD(&(u->refcount), -1);
        bool showWarn = false;
        if (u->refcount == 0)
        {
            if (u->urefcount > 0)
                showWarn = true;
            // simulate img_t::deallocate
            if (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ img_t::getDefaultAllocator())->unmap(u);
            }
            else
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        if (u->refcount == 0 && u->urefcount == 0) // oops, we need to free resources
        {
            showWarn = true;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }
#ifndef NDEBUG
        if (showWarn)
        {
            static int warn_message_showed = 0;
            if (warn_message_showed++ < 100)
            {
                fflush(stdout);
                fprintf(stderr, "\n! OPENCV warning: getUMat()/getMat() call chain possible problem."
                                "\n!                 Base object is dead, while nested/derived object is still alive or processed."
                                "\n!                 Please check lifetime of UMat/img_t objects!\n");
                fflush(stderr);
            }
        }
#else
        (void)showWarn;
#endif
        originalUMatData = NULL;
    }
}

void UMatData::lock()
{
    umatLocks[(size_t)(void*)this % UMAT_NLOCKS].lock();
}

void UMatData::unlock()
{
    umatLocks[(size_t)(void*)this % UMAT_NLOCKS].unlock();
}


MatAllocator* UMat::getStdAllocator()
{
#ifdef HAVE_OPENCL
    if( ocl::haveOpenCL() && ocl::useOpenCL() )
        return ocl::getOpenCLAllocator();
#endif
    return img_t::getDefaultAllocator();
}

void swap( UMat& a, UMat& b )
{
    T_SWAP(_Tp, a.flags, b.flags);
    T_SWAP(_Tp, a.dims, b.dims);
    T_SWAP(_Tp, a->rows, b->rows);
    T_SWAP(_Tp, a->cols, b->cols);
    T_SWAP(_Tp, a.allocator, b.allocator);
    T_SWAP(_Tp, a.u, b.u);
    T_SWAP(_Tp, a.offset, b.offset);

    T_SWAP(_Tp, a.size.p, b.size.p);
    T_SWAP(_Tp, a->step.p, b->step.p);
    T_SWAP(_Tp, a->step.buf[0], b->step.buf[0]);
    T_SWAP(_Tp, a->step.buf[1], b->step.buf[1]);

    if( a->step.p == b->step.buf )
    {
        a->step.p = a->step.buf;
        a.size.p = &a->rows;
    }

    if( b->step.p == a->step.buf )
    {
        b->step.p = b->step.buf;
        b.size.p = &b->rows;
    }
}


void setSize( UMat& m, int _dims, const int* _sz,
                            const size_t* _steps, bool autoSteps )
{
    CC_Assert( 0 <= _dims && _dims <= CC_MAX_DIM );
    if( m.dims != _dims )
    {
        if( m->step.p != m->step.buf )
        {
            fastFree(m->step.p);
            m->step.p = m->step.buf;
            m.size.p = &m->rows;
        }
        if( _dims > 2 )
        {
            m->step.p = (size_t*)fastMalloc(_dims*sizeof(m->step.p[0]) + (_dims+1)*sizeof(m.size.p[0]));
            m.size.p = (int*)(m->step.p + _dims) + 1;
            m.size.p[-1] = _dims;
            m->rows = m->cols = -1;
        }
    }

    m.dims = _dims;
    if( !_sz )
        return;

    size_t esz = CC_ELEM_SIZE(m.flags), total = esz;
    int i;
    for( i = _dims-1; i >= 0; i-- )
    {
        int s = _sz[i];
        CC_Assert( s >= 0 );
        m.size.p[i] = s;

        if( _steps )
            m->step.p[i] = i < _dims-1 ? _steps[i] : esz;
        else if( autoSteps )
        {
            m->step.p[i] = total;
            int64 total1 = (int64)total*s;
            if( (uint64)total1 != (size_t)total1 )
                CC_Error( CC_StsOutOfRange, "The total matrix size does not fit to \"size_t\" type" );
            total = (size_t)total1;
        }
    }

    if( _dims == 1 )
    {
        m.dims = 2;
        m->cols = 1;
        m->step[1] = esz;
    }
}


void updateContinuityFlag(UMat& m)
{
    int i, j;
    for( i = 0; i < m.dims; i++ )
    {
        if( m.size[i] > 1 )
            break;
    }

    for( j = m.dims-1; j > i; j-- )
    {
        if( m->step[j]*m.size[j] < m->step[j-1] )
            break;
    }

    uint64 total = (uint64)m->step[0]*m.size[0];
    if( j <= i && total == (size_t)total )
        m.flags |= UMat::CONTINUOUS_FLAG;
    else
        m.flags &= ~UMat::CONTINUOUS_FLAG;
}


void finalizeHdr(UMat& m)
{
    updateContinuityFlag(m);
    int d = m.dims;
    if( d > 2 )
        m->rows = m->cols = -1;
}


UMat img_t::getUMat(int accessFlags, UMatUsageFlags usageFlags) const
{
    UMat hdr;
    if(!data)
        return hdr;
    Size wholeSize;
    Point ofs;
    locateROI(wholeSize, ofs);
    Size sz(cols, rows);
    if (ofs.x != 0 || ofs.y != 0)
    {
        img_t src = *this;
        int dtop = ofs.y;
        int dbottom = wholeSize.height - src->rows - ofs.y;
        int dleft = ofs.x;
        int dright = wholeSize.width - src->cols - ofs.x;
        src.adjustROI(dtop, dbottom, dleft, dright);
        return src.getUMat(accessFlags, usageFlags)(Rect(ofs.x, ofs.y, sz.width, sz.height));
    }
    CC_Assert(data == datastart);

    accessFlags |= ACCESS_RW;
    UMatData* new_u = NULL;
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        if(!a)
            a = a0;
        new_u = a->allocate(dims, size.p, type(), data, step.p, accessFlags, usageFlags);
    }
    bool allocated = false;
    try
    {
        allocated = UMat::getStdAllocator()->allocate(new_u, accessFlags, usageFlags);
    }
    catch (const Exception& e)
    {
        fprintf(stderr, "Exception: %s\n", e.what());
    }
    if (!allocated)
    {
        allocated = getDefaultAllocator()->allocate(new_u, accessFlags, usageFlags);
        CC_Assert(allocated);
    }
    if (u != NULL)
    {
#ifdef HAVE_OPENCL
        if (ocl::useOpenCL() && new_u->currAllocator == ocl::getOpenCLAllocator())
        {
            CC_Assert(new_u->tempUMat());
        }
#endif
        new_u->originalUMatData = u;
        CC_XADD(&(u->refcount), 1);
        CC_XADD(&(u->urefcount), 1);
    }
    hdr.flags = flags;
    setSize(hdr, dims, size.p, step.p);
    finalizeHdr(hdr);
    hdr.u = new_u;
    hdr.offset = 0; //data - datastart;
    hdr.addref();
    return hdr;
}

void UMat::create(int d, const int* _sizes, int _type, UMatUsageFlags _usageFlags)
{
    this->usageFlags = _usageFlags;

    int i;
    CC_Assert(0 <= d && d <= CC_MAX_DIM && _sizes);
    _type = CC_MAT_DEPTH(_type);

    if( u && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
    {
        if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
            return;
        for( i = 0; i < d; i++ )
            if( size[i] != _sizes[i] )
                break;
        if( i == d && (d > 1 || size[1] == 1))
            return;
    }

    int _sizes_backup[CC_MAX_DIM]; // #5991
    if (_sizes == (this->size.p))
    {
        for(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    if( d == 0 )
        return;
    flags = (_type & CC_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);
    offset = 0;

    if( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
        if (!a)
        {
            a = a0;
            a0 = img_t::getDefaultAllocator();
        }
        try
        {
            u = a->allocate(dims, size, _type, 0, step.p, 0, usageFlags);
            CC_Assert(u != 0);
        }
        catch(...)
        {
            if(a != a0)
                u = a0->allocate(dims, size, _type, 0, step.p, 0, usageFlags);
            CC_Assert(u != 0);
        }
        CC_Assert( step[dims-1] == (size_t)CC_ELEM_SIZE(flags) );
    }

    finalizeHdr(*this);
    addref();
}

void UMat::create(const std::vector<int>& _sizes, int _type, UMatUsageFlags _usageFlags)
{
    create((int)_sizes.size(), _sizes.data(), _type, _usageFlags);
}

void UMat::copySize(const UMat& m)
{
    setSize(*this, m.dims, 0, 0);
    for( int i = 0; i < dims; i++ )
    {
        size[i] = m.size[i];
        step[i] = m->step[i];
    }
}


UMat::~UMat()
{
    release();
    if( step.p != step.buf )
        fastFree(step.p);
}

void UMat::deallocate()
{
    u->currAllocator->deallocate(u);
    u = NULL;
}


UMat::UMat(const UMat& m, const CRange& _rowRange, const CRange& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0), size(&rows)
{
    CC_Assert( m.dims >= 2 );
    if( m.dims > 2 )
    {
        CAutoBuffer<CRange> rs(m.dims);
        rs[0] = _rowRange;
        rs[1] = _colRange;
        for( int i = 2; i < m.dims; i++ )
            rs[i] = CRange::all();
        *this = m(rs);
        return;
    }

    *this = m;
    if( _rowRange != CRange::all() && _rowRange != CRange(0,rows) )
    {
        CC_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end && _rowRange.end <= m->rows );
        rows = _rowRange.size();
        offset += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    if( _colRange != CRange::all() && _colRange != CRange(0,cols) )
    {
        CC_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end && _colRange.end <= m->cols );
        cols = _colRange.size();
        offset += _colRange.start*elemSize();
        flags &= cols < m->cols ? ~CONTINUOUS_FLAG : -1;
        flags |= SUBMATRIX_FLAG;
    }

    if( rows == 1 )
        flags |= CONTINUOUS_FLAG;

    if( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(const UMat& m, const Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    allocator(m.allocator), usageFlags(m.usageFlags), u(m.u), offset(m.offset + roi.y*m->step[0]), size(&rows)
{
    CC_Assert( m.dims <= 2 );
    flags &= roi.width < m->cols ? ~CONTINUOUS_FLAG : -1;
    flags |= roi.height == 1 ? CONTINUOUS_FLAG : 0;

    size_t esz = CC_ELEM_SIZE(flags);
    offset += roi.x*esz;
    CC_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m->cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m->rows );
    if( u )
        CC_XADD(&(u->urefcount), 1);
    if( roi.width < m->cols || roi.height < m->rows )
        flags |= SUBMATRIX_FLAG;

    step[0] = m->step[0]; step[1] = esz;

    if( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


UMat::UMat(const UMat& m, const CRange* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0), size(&rows)
{
    int i, d = m.dims;

    CC_Assert(ranges);
    for( i = 0; i < d; i++ )
    {
        CRange r = ranges[i];
        CC_Assert( r == CRange::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    for( i = 0; i < d; i++ )
    {
        CRange r = ranges[i];
        if( r != CRange::all() && r != CRange(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }
    updateContinuityFlag(*this);
}

UMat::UMat(const UMat& m, const std::vector<CRange>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0), size(&rows)
{
    int i, d = m.dims;

    CC_Assert((int)ranges.size() == d);
    for (i = 0; i < d; i++)
    {
        CRange r = ranges[i];
        CC_Assert(r == CRange::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *this = m;
    for (i = 0; i < d; i++)
    {
        CRange r = ranges[i];
        if (r != CRange::all() && r != CRange(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            offset += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }
    updateContinuityFlag(*this);
}

UMat UMat::diag(int d) const
{
    CC_Assert( dims <= 2 );
    UMat m = *this;
    size_t esz = elemSize();
    int len;

    if( d >= 0 )
    {
        len = MIN(cols - d, rows);
        m.offset += esz*d;
    }
    else
    {
        len = MIN(rows + d, cols);
        m.offset -= step[0]*d;
    }
    CC_DbgAssert( len > 0 );

    m.size[0] = m->rows = len;
    m.size[1] = m->cols = 1;
    m->step[0] += (len > 1 ? esz : 0);

    if( m->rows > 1 )
        m.flags &= ~CONTINUOUS_FLAG;
    else
        m.flags |= CONTINUOUS_FLAG;

    if( size() != Size(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    return m;
}

void UMat::locateROI( Size& wholeSize, Point& ofs ) const
{
    CC_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = (ptrdiff_t)offset, delta2 = (ptrdiff_t)u->size;

    if( delta1 == 0 )
        ofs.x = ofs.y = 0;
    else
    {
        ofs.y = (int)(delta1/step[0]);
        ofs.x = (int)((delta1 - step[0]*ofs.y)/esz);
        CC_DbgAssert( offset == (size_t)(ofs.y*step[0] + ofs.x*esz) );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (int)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = MAX(wholeSize.height, ofs.y + rows);
    wholeSize.width = (int)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = MAX(wholeSize.width, ofs.x + cols);
}


UMat& UMat::adjustROI( int dtop, int dbottom, int dleft, int dright )
{
    CC_Assert( dims <= 2 && step[0] > 0 );
    Size wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    int row1 = MAX(ofs.y - dtop, 0), row2 = MIN(ofs.y + rows + dbottom, wholeSize.height);
    int col1 = MAX(ofs.x - dleft, 0), col2 = MIN(ofs.x + cols + dright, wholeSize.width);
    offset += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    if( esz*cols == step[0] || rows == 1 )
        flags |= CONTINUOUS_FLAG;
    else
        flags &= ~CONTINUOUS_FLAG;
    return *this;
}


UMat UMat::reshape(int new_cn, int new_rows) const
{
    int cn = CC_MAT_CN();
    UMat hdr = *this;

    if( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CC_MAT_CN_MASK) | ((new_cn-1) << CC_CN_SHIFT);
        hdr->step[dims-1] = CC_ELEM_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        return hdr;
    }

    CC_Assert( dims <= 2 );

    if( new_cn == 0 )
        new_cn = cn;

    int total_width = cols * cn;

    if( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    if( new_rows != 0 && new_rows != rows )
    {
        int total_size = total_width * rows;
        if( !isContinuous() )
            CC_Error( CC_BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        if( (unsigned)new_rows > (unsigned)total_size )
            CC_Error( CC_StsOutOfRange, "Bad new number of rows" );

        total_width = total_size / new_rows;

        if( total_width * new_rows != total_size )
            CC_Error( CC_StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr->rows = new_rows;
        hdr->step[0] = total_width * elemSize1();
    }

    int new_width = total_width / new_cn;

    if( new_width * new_cn != total_width )
        CC_Error( CC_BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr->cols = new_width;
    hdr.flags = (hdr.flags & ~CC_MAT_CN_MASK) | ((new_cn-1) << CC_CN_SHIFT);
    hdr->step[1] = CC_ELEM_SIZE(hdr.flags);
    return hdr;
}

UMat UMat::diag(const UMat& d)
{
    CC_Assert( d->cols == 1 || d->rows == 1 );
    int len = d->rows + d->cols - 1;
    UMat m(len, len, d CC_MAT_TYPE(), Scalar(0));
    UMat md = m.diag();
    if( d->cols == 1 )
        d.copyTo(md);
    else
        transpose(d, md);
    return m;
}

int UMat::cvCheckVector(int _elemChannels, int _depth, bool _requireContinuous) const
{
    return (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims == 2 && (((rows == 1 || cols == 1) && CC_MAT_CN() == _elemChannels) ||
                        (cols == _elemChannels && CC_MAT_CN() == 1))) ||
        (dims == 3 && CC_MAT_CN() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (int)(total()*CC_MAT_CN()/_elemChannels) : -1;
}

UMat UMat::reshape(int _cn, int _newndims, const int* _newsz) const
{
    if(_newndims == dims)
    {
        if(_newsz == 0)
            return reshape(_cn);
        if(_newndims == 2)
            return reshape(_cn, _newsz[0]);
    }

    if (isContinuous())
    {
        CC_Assert(_cn >= 0 && _newndims > 0 && _newndims <= CC_MAX_DIM && _newsz);

        if (_cn == 0)
            _cn = this->CC_MAT_CN();
        else
            CC_Assert(_cn <= CC_CN_MAX);

        size_t total_elem1_ref = this->total() * this->CC_MAT_CN();
        size_t total_elem1 = _cn;

        CAutoBuffer<int, 4> newsz_buf( (size_t)_newndims );

        for (int i = 0; i < _newndims; i++)
        {
            CC_Assert(_newsz[i] >= 0);

            if (_newsz[i] > 0)
                newsz_buf[i] = _newsz[i];
            else if (i < dims)
                newsz_buf[i] = this->size[i];
            else
                CC_Error(CC_StsOutOfRange, "Copy dimension (which has zero size) is not present in source matrix");

            total_elem1 *= (size_t)newsz_buf[i];
        }

        if (total_elem1 != total_elem1_ref)
            CC_Error(CC_StsUnmatchedSizes, "Requested and source matrices have different count of elements");

        UMat hdr = *this;
        hdr.flags = (hdr.flags & ~CC_MAT_CN_MASK) | ((_cn-1) << CC_CN_SHIFT);
        setSize(hdr, _newndims, (int*)newsz_buf, NULL, true);

        return hdr;
    }

    CC_Error(CC_StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
    return UMat();
}

img_t UMat::getMat(int accessFlags) const
{
    if(!u)
        return img_t();
    // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
    accessFlags |= ACCESS_RW;
    UMatDataAutoLock autolock(u);
    if(CC_XADD(&u->refcount, 1) == 0)
        u->currAllocator->map(u, accessFlags);
    if (u->data != 0)
    {
        img_t hdr(dims, size.p, type(), u->data + offset, step.p);
        hdr.flags = flags;
        hdr.u = u;
        hdr.datastart = u->data;
        hdr.data = u->data + offset;
        hdr.datalimit = hdr.dataend = u->data + u->size;
        return hdr;
    }
    else
    {
        CC_XADD(&u->refcount, -1);
        CC_Assert(u->data != 0 && "Error mapping of UMat to host memory.");
        return img_t();
    }
}

void* UMat::handle(int accessFlags) const
{
    if( !u )
        return 0;

    CC_Assert(u->refcount == 0);
    CC_Assert(!u->deviceCopyObsolete() || u->copyOnMap());
    if (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    if ((accessFlags & ACCESS_WRITE) != 0)
        u->markHostCopyObsolete(true);

    return u->handle;
}

void UMat::ndoffset(size_t* ofs) const
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    for( int i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

void UMat::copyTo(img_t* _dst) const
{
    CC_INSTRUMENT_REGION()

    int dtype = _dst CC_MAT_TYPE();
    if( _dst.fixedType() && dtype != type() )
    {
        CC_Assert( CC_MAT_CN() == CC_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        return;
    }

    if( empty() )
    {
        _dst.release();
        return;
    }

    size_t i, sz[CC_MAX_DIM], srcofs[CC_MAX_DIM], dstofs[CC_MAX_DIM], esz = elemSize();
    for( i = 0; i < (size_t)dims; i++ )
        sz[i] = size.p[i];
    sz[dims-1] *= esz;
    ndoffset(srcofs);
    srcofs[dims-1] *= esz;

    _dst cvSetMat( dims, size.p, type() );
    if( _dst.isUMat() )
    {
        UMat dst = _dst.getUMat();
        if( u == dst.u && dst.offset == offset )
            return;

        if (u->currAllocator == dst.u->currAllocator)
        {
            dst.ndoffset(dstofs);
            dstofs[dims-1] *= esz;
            u->currAllocator->copy(u, dst.u, dims, sz, srcofs, step.p, dstofs, dst->step.p, false);
            return;
        }
    }

    img_t *dst = _dst;
    u->currAllocator->download(u, dst->tt.data, dims, sz, srcofs, step.p, dst->step.p);
}

void UMat::copyTo(img_t* _dst, const img_t* _mask) const
{
    CC_INSTRUMENT_REGION()

    if( _mask.empty() )
    {
        copyTo(_dst);
        return;
    }
#ifdef HAVE_OPENCL
    int cn = CC_MAT_CN(), mtype = _mask CC_MAT_TYPE(), mdepth = CC_MAT_DEPTH(mtype), mcn = CC_MAT_CN(mtype);
    CC_Assert( mdepth == CC_8U && (mcn == 1 || mcn == cn) );

    if (ocl::useOpenCL() && _dst.isUMat() && dims <= 2)
    {
        UMatData * prevu = _dst.getUMat().u;
        _dst cvSetMat( dims, size, type() );

        UMat dst = _dst.getUMat();

        bool haveDstUninit = false;
        if( prevu != dst.u ) // do not leave dst uninitialized
            haveDstUninit = true;

        String opts = format("-D COPY_TO_MASK -D T1=%s -D scn=%d -D mcn=%d%s",
                             ocl::memopTypeToStr(depth()), cn, mcn,
                             haveDstUninit ? " -D HAVE_DST_UNINIT" : "");

        ocl::Kernel k("copyToMask", ocl::core::copyset_oclsrc, opts);
        if (!k.empty())
        {
            k.args(ocl::KernelArg::ReadOnlyNoSize(*this),
                   ocl::KernelArg::ReadOnlyNoSize(_mask.getUMat()),
                   haveDstUninit ? ocl::KernelArg::WriteOnly(dst) :
                                   ocl::KernelArg::ReadWrite(dst));

            size_t globalsize[2] = { (size_t)cols, (size_t)rows };
            if (k.run(2, globalsize, NULL, false))
            {
                CC_IMPL_ADD(CC_IMPL_OCL);
                return;
            }
        }
    }
#endif
    img_t src = getMat(ACCESS_READ);
    src.copyTo(_dst, _mask);
}

void UMat::convertTo(img_t* _dst, int _type, double alpha, double beta) const
{
    CC_INSTRUMENT_REGION()

    bool noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    int stype = type(), cn = CC_MAT_CN(stype);

    if( _type < 0 )
        _type = _dst.fixedType() ? _dst CC_MAT_TYPE() : stype;
    else
        _type = CC_MAKETYPE(CC_MAT_DEPTH(_type), cn);

    int sdepth = CC_MAT_DEPTH(stype), ddepth = CC_MAT_DEPTH(_type);
    if( sdepth == ddepth && noScale )
    {
        copyTo(_dst);
        return;
    }
#ifdef HAVE_OPENCL
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;
    bool needDouble = sdepth == CC_64F || ddepth == CC_64F;
    if( dims <= 2 && cn && _dst.isUMat() && ocl::useOpenCL() &&
            ((needDouble && doubleSupport) || !needDouble) )
    {
        int wdepth = MAX(CC_32F, sdepth), rowsPerWI = 4;

        char cvt[2][40];
        ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                      format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s%s%s",
                             ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                             ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0]),
                             ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1]),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "", noScale ? " -D NO_SCALE" : ""));
        if (!k.empty())
        {
            UMat src = *this;
            _dst cvSetMat( size(), _type );
            UMat dst = _dst.getUMat();

            float alphaf = (float)alpha, betaf = (float)beta;
            ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                    dstarg = ocl::KernelArg::WriteOnly(dst, cn);

            if (noScale)
                k.args(srcarg, dstarg, rowsPerWI);
            else if (wdepth == CC_32F)
                k.args(srcarg, dstarg, alphaf, betaf, rowsPerWI);
            else
                k.args(srcarg, dstarg, alpha, beta, rowsPerWI);

            size_t globalsize[2] = { (size_t)dst->cols * cn, ((size_t)dst->rows + rowsPerWI - 1) / rowsPerWI };
            if (k.run(2, globalsize, NULL, false))
            {
                CC_IMPL_ADD(CC_IMPL_OCL);
                return;
            }
        }
    }
#endif
    img_t m = getMat(ACCESS_READ);
    m.convertTo(_dst, _type, alpha, beta);
}

UMat& UMat::setTo(const img_t* _value, const img_t* _mask)
{
    CC_INSTRUMENT_REGION()

    bool haveMask = !_mask.empty();
#ifdef HAVE_OPENCL
    int tp = type(), cn = CC_MAT_CN(tp), d = CC_MAT_DEPTH(tp);

    if( dims <= 2 && cn <= 4 && CC_MAT_DEPTH(tp) < CC_64F && ocl::useOpenCL() )
    {
        img_t value = _value;
        CC_Assert( checkScalar(value, type(), _value.kind(), _InputArray::UMAT) );
        int kercn = haveMask || cn == 3 ? cn : MAX(cn, ocl::predictOptimalVectorWidth(*this)),
                kertp = CC_MAKE_TYPE(d, kercn);

        double buf[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0 };
        convertAndUnrollScalar(value, tp, (uchar *)buf, kercn / cn);

        int scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
        String opts = format("-D dstT=%s -D rowsPerWI=%d -D dstST=%s -D dstT1=%s -D cn=%d",
                             ocl::memopTypeToStr(kertp), rowsPerWI,
                             ocl::memopTypeToStr(CC_MAKETYPE(d, scalarcn)),
                             ocl::memopTypeToStr(d), kercn);

        ocl::Kernel setK(haveMask ? "setMask" : "set", ocl::core::copyset_oclsrc, opts);
        if( !setK.empty() )
        {
            ocl::KernelArg scalararg(0, 0, 0, 0, buf, CC_ELEM_SIZE(d) * scalarcn);
            UMat mask;

            if( haveMask )
            {
                mask = _mask.getUMat();
                CC_Assert( mask.size() == size() && mask CC_MAT_TYPE() == CC_8UC1 );
                ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
                        dstarg = ocl::KernelArg::ReadWrite(*this);
                setK.args(maskarg, dstarg, scalararg);
            }
            else
            {
                ocl::KernelArg dstarg = ocl::KernelArg::WriteOnly(*this, cn, kercn);
                setK.args(dstarg, scalararg);
            }

            size_t globalsize[] = { (size_t)cols * cn / kercn, ((size_t)rows + rowsPerWI - 1) / rowsPerWI };
            if( setK.run(2, globalsize, NULL, false) )
            {
                CC_IMPL_ADD(CC_IMPL_OCL);
                return *this;
            }
        }
    }
#endif
    img_t m = getMat(haveMask ? ACCESS_RW : ACCESS_WRITE);
    m.setTo(_value, _mask);
    return *this;
}

UMat& UMat::operator = (const Scalar& s)
{
    setTo(s);
    return *this;
}

UMat UMat::t() const
{
    UMat m;
    transpose(*this, m);
    return m;
}

UMat UMat::inv(int method) const
{
    UMat m;
    invert(*this, m, method);
    return m;
}

UMat UMat::mul(const img_t* m, double scale) const
{
    UMat dst;
    multiply(*this, m, dst, scale);
    return dst;
}

#ifdef HAVE_OPENCL

static bool ocl_dot( const img_t* _src1, const img_t* _src2, double & res )
{
    UMat src1 = _src1.getUMat() cvReshape(,1), src2 = _src2.getUMat() cvReshape(,1);

    int type = src1 CC_MAT_TYPE(), depth = CC_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    if ( !doubleSupport && depth == CC_64F )
        return false;

    int dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    int ddepth = MAX(CC_32F, depth);

    int wgs2_aligned = 1;
    while (wgs2_aligned < (int)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    char cvt[40];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CC_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CC_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt),
                         (int)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1 CC_IS_CONT_MAT() ? " -D HAVE_SRC_CONT" : "",
                         _src2 CC_IS_CONT_MAT() ? " -D HAVE_SRC2_CONT" : "", kercn));
    if (k.empty())
        return false;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1->cols, (int)src1 CC_MAT_TOTAL(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    if (k.run(1, &globalsize, &wgs, false))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        return true;
    }
    return false;
}

#endif

double UMat::dot(const img_t* m) const
{
    CC_INSTRUMENT_REGION()

    CC_Assert(m.sameSize(*this) && m CC_MAT_TYPE() == type());

#ifdef HAVE_OPENCL
    double r = 0;
    CC_OCL_RUN_(dims <= 2, ocl_dot(*this, m, r), r)
#endif

    return getMat(ACCESS_READ).dot(m);
}

UMat UMat::zeros(int rows, int cols, int type)
{
    return UMat(rows, cols, type, Scalar::all(0));
}

UMat UMat::zeros(Size size, int type)
{
    return UMat(size, type, Scalar::all(0));
}

UMat UMat::zeros(int ndims, const int* sz, int type)
{
    return UMat(ndims, sz, type, Scalar::all(0));
}

UMat UMat::ones(int rows, int cols, int type)
{
    return UMat::ones(Size(cols, rows), type);
}

UMat UMat::ones(Size size, int type)
{
    return UMat(size, type, Scalar(1));
}

UMat UMat::ones(int ndims, const int* sz, int type)
{
    return UMat(ndims, sz, type, Scalar(1));
}

UMat UMat::eye(int rows, int cols, int type)
{
    return UMat::eye(Size(cols, rows), type);
}

UMat UMat::eye(Size size, int type)
{
    UMat m(size, type);
    setIdentity(m);
    return m;
}

}


