

#ifndef OPENCC_IMGPROC_HAL_REPLACEMENT_HPP
#define OPENCC_IMGPROC_HAL_REPLACEMENT_HPP

//#include "cv1/core/hal/interface.h"
//#include "hal/interface.h"

#if defined __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined _MSC_VER
#  pragma warning( push )
#  pragma warning( disable: 4100 )
#endif

//! @addtogroup imgproc_hal_interface
//! @note Define your functions to override default implementations:
//! @code
//! #undef hal_add8u
//! #define hal_add8u my_add8u
//! @endcode
//! @{

/**
@brief Dummy structure storing filtering context

Users can convert this pointer to any type they want. Initialisation and destruction should be made in Init and Free function implementations correspondingly.
Example:
@code{.cpp}
int my_hal_filterInit(cvhalFilter2D **context, ...) {
    context = static_cast<cvhalFilter2D*>(new MyFilterData());
    //... init
}

int my_hal_filterFree(cvhalFilter2D *context) {
    MyFilterData *c = static_cast<MyFilterData*>(context);
    delete c;
}
@endcode
 */
struct cvhalFilter2D {};

/**
   @brief hal_filterInit
   @param context double pointer to user-defined context
   @param kernel_data pointer to kernel data
   @param kernel_step kernel step
   @param kernel_type kernel type (CC_8U, ...)
   @param kernel_width kernel width
   @param kernel_height kernel height
   @param max_width max possible image width, can be used to allocate working buffers
   @param max_height max possible image height
   @param src_type source image type
   @param dst_type destination image type
   @param borderType border processing mode (CC_HAL_BORDER_REFLECT, ...)
   @param delta added to pixel values
   @param anchor_x relative X position of center point within the kernel
   @param anchor_y relative Y position of center point within the kernel
   @param allowSubmatrix indicates whether the submatrices will be allowed as source image
   @param allowInplace indicates whether the inplace operation will be possible
   @sa filter2D, hal_Filter2D
 */
inline int hal_ni_filterInit(cvhalFilter2D **context, uchar *kernel_data, size_t kernel_step, int kernel_type, int kernel_width, int kernel_height, int max_width, int max_height, int src_type, int dst_type, int borderType, double delta, int anchor_x, int anchor_y, bool allowSubmatrix, bool allowInplace) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief hal_filter
   @param context pointer to user-defined context
   @param src_data source image data
   @param src_step source image step
   @param dst_data destination image data
   @param dst_step destination image step
   @param width images width
   @param height images height
   @param full_width full width of source image (outside the ROI)
   @param full_height full height of source image (outside the ROI)
   @param offset_x source image ROI offset X
   @param offset_y source image ROI offset Y
   @sa filter2D, hal_Filter2D
 */
inline int hal_ni_filter(cvhalFilter2D *context, uchar *src_data, size_t src_step, uchar *dst_data, size_t dst_step, int width, int height, int full_width, int full_height, int offset_x, int offset_y) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief hal_filterFree
   @param context pointer to user-defined context
   @sa filter2D, hal_Filter2D
 */
inline int hal_ni_filterFree(cvhalFilter2D *context) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#define cv_hal_filterInit hal_ni_filterInit
#define cv_hal_filter hal_ni_filter
#define cv_hal_filterFree hal_ni_filterFree
//! @endcond

/**
   @brief hal_sepFilterInit
   @param context double pointer to user-defined context
   @param src_type source image type
   @param dst_type destination image type
   @param kernel_type kernels type
   @param kernelx_data pointer to x-kernel data
   @param kernelx_length x-kernel vector length
   @param kernely_data pointer to y-kernel data
   @param kernely_length y-kernel vector length
   @param anchor_x relative X position of center point within the kernel
   @param anchor_y relative Y position of center point within the kernel
   @param delta added to pixel values
   @param borderType border processing mode (CC_HAL_BORDER_REFLECT, ...)
   @sa sepFilter2D, hal_SepFilter2D
 */
inline int hal_ni_sepFilterInit(cvhalFilter2D **context, int src_type, int dst_type, int kernel_type, uchar *kernelx_data, int kernelx_length, uchar *kernely_data, int kernely_length, int anchor_x, int anchor_y, double delta, int borderType) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief hal_sepFilter
   @param context pointer to user-defined context
   @param src_data source image data
   @param src_step source image step
   @param dst_data destination image data
   @param dst_step destination image step
   @param width images width
   @param height images height
   @param full_width full width of source image (outside the ROI)
   @param full_height full height of source image (outside the ROI)
   @param offset_x source image ROI offset X
   @param offset_y source image ROI offset Y
   @sa sepFilter2D, hal_SepFilter2D
 */
inline int hal_ni_sepFilter(cvhalFilter2D *context, uchar *src_data, size_t src_step, uchar* dst_data, size_t dst_step, int width, int height, int full_width, int full_height, int offset_x, int offset_y) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief hal_sepFilterFree
   @param context pointer to user-defined context
   @sa sepFilter2D, hal_SepFilter2D
 */
inline int hal_ni_sepFilterFree(cvhalFilter2D *context) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#define cv_hal_sepFilterInit hal_ni_sepFilterInit
#define cv_hal_sepFilter hal_ni_sepFilter
#define cv_hal_sepFilterFree hal_ni_sepFilterFree
//! @endcond

/**
   @brief hal_morphInit
   @param context double pointer to user-defined context
   @param operation morphology operation CC_HAL_MORPH_ERODE or CC_HAL_MORPH_DILATE
   @param src_type source image type
   @param dst_type destination image type
   @param max_width max possible image width, can be used to allocate working buffers
   @param max_height max possible image height
   @param kernel_type kernel type (CC_8U, ...)
   @param kernel_data pointer to kernel data
   @param kernel_step kernel step
   @param kernel_width kernel width
   @param kernel_height kernel height
   @param anchor_x relative X position of center point within the kernel
   @param anchor_y relative Y position of center point within the kernel
   @param borderType border processing mode (CC_HAL_BORDER_REFLECT, ...)
   @param borderValue values to use for CC_HAL_BORDER_CONSTANT mode
   @param iterations number of iterations
   @param allowSubmatrix indicates whether the submatrices will be allowed as source image
   @param allowInplace indicates whether the inplace operation will be possible
   @sa erode, dilate, morphologyEx, hal_Morph
 */
inline int hal_ni_morphInit(cvhalFilter2D **context, int operation, int src_type, int dst_type, int max_width, int max_height, int kernel_type, uchar *kernel_data, size_t kernel_step, int kernel_width, int kernel_height, int anchor_x, int anchor_y, int borderType, const double borderValue[4], int iterations, bool allowSubmatrix, bool allowInplace) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief hal_morph
   @param context pointer to user-defined context
   @param src_data source image data
   @param src_step source image step
   @param dst_data destination image data
   @param dst_step destination image step
   @param width images width
   @param height images height
   @param src_full_width full width of source image (outside the ROI)
   @param src_full_height full height of source image (outside the ROI)
   @param src_roi_x source image ROI X offset
   @param src_roi_y source image ROI Y offset
   @param dst_full_width full width of destination image
   @param dst_full_height full height of destination image
   @param dst_roi_x destination image ROI X offset
   @param dst_roi_y destination image ROI Y offset
   @sa erode, dilate, morphologyEx, hal_Morph
 */
inline int hal_ni_morph(cvhalFilter2D *context, uchar *src_data, size_t src_step, uchar *dst_data, size_t dst_step, int width, int height, int src_full_width, int src_full_height, int src_roi_x, int src_roi_y, int dst_full_width, int dst_full_height, int dst_roi_x, int dst_roi_y) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief hal_morphFree
   @param context pointer to user-defined context
   @sa erode, dilate, morphologyEx, hal_Morph
 */
inline int hal_ni_morphFree(cvhalFilter2D *context) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#define cv_hal_morphInit hal_ni_morphInit
#define cv_hal_morph hal_ni_morph
#define cv_hal_morphFree hal_ni_morphFree
//! @endcond

/**
   @brief hal_resize
   @param src_type source and destination image type
   @param src_data source image data
   @param src_step source image step
   @param src_width source image width
   @param src_height source image height
   @param dst_data destination image data
   @param dst_step destination image step
   @param dst_width destination image width
   @param dst_height destination image height
   @param inv_scale_x inversed scale X coefficient
   @param inv_scale_y inversed scale Y coefficient
   @param interpolation interpolation mode (CC_HAL_INTER_NEAREST, ...)
   @sa resize, hal_resize
 */
inline int hal_ni_resize(TypeId src_type, int cn, const uchar *src_data, size_t src_step, int src_width, int src_height, uchar *dst_data, size_t dst_step, int dst_width, int dst_height, double inv_scale_x, double inv_scale_y, int interpolation) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief hal_warpAffine
   @param src_type source and destination image type
   @param src_data source image data
   @param src_step source image step
   @param src_width source image width
   @param src_height source image height
   @param dst_data destination image data
   @param dst_step destination image step
   @param dst_width destination image width
   @param dst_height destination image height
   @param M 2x3 matrix with transform coefficients
   @param interpolation interpolation mode (CC_HAL_INTER_NEAREST, ...)
   @param borderType border processing mode (CC_HAL_BORDER_REFLECT, ...)
   @param borderValue values to use for CC_HAL_BORDER_CONSTANT mode
   @sa warpAffine, hal_warpAffine
 */
inline int hal_ni_warpAffine(TypeId src_type, int cn, const uchar *src_data, size_t src_step, int src_width, int src_height, uchar *dst_data, size_t dst_step, int dst_width, int dst_height, const double M[6], int interpolation, int borderType, const double borderValue[4]) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }
/**
   @brief hal_warpPerspectve
   @param src_type source and destination image type
   @param src_data source image data
   @param src_step source image step
   @param src_width source image width
   @param src_height source image height
   @param dst_data destination image data
   @param dst_step destination image step
   @param dst_width destination image width
   @param dst_height destination image height
   @param M 3x3 matrix with transform coefficients
   @param interpolation interpolation mode (CC_HAL_INTER_NEAREST, ...)
   @param borderType border processing mode (CC_HAL_BORDER_REFLECT, ...)
   @param borderValue values to use for CC_HAL_BORDER_CONSTANT mode
   @sa warpPerspective, hal_warpPerspective
 */
inline int hal_ni_warpPerspectve(TypeId src_type, int cn, const uchar *src_data, size_t src_step, int src_width, int src_height, uchar *dst_data, size_t dst_step, int dst_width, int dst_height, const double M[9], int interpolation, int borderType, const double borderValue[4]) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#define cv_hal_resize hal_ni_resize
#define cv_hal_warpAffine hal_ni_warpAffine
#define cv_hal_warpPerspective hal_ni_warpPerspectve
//! @endcond

/**
   @brief hal_cvtBGRtoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U, CC_16U, CC_32F)
   @param scn source image channels (3 or 4)
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R channels will be swapped (BGR->RGB or RGB->BGR)
   Convert between BGR, BGRA, RGB and RGBA image formats.
 */
inline int hal_ni_cvtBGRtoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int scn, int dcn, bool swapBlue) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGRtoBGR5x5
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param scn source image channels (3 or 4)
   @param swapBlue if set to true B and R source channels will be swapped (treat as RGB)
   @param greenBits number of bits for green channel (5 or 6)
   Convert from BGR, BGRA, RGB and RGBA to packed BGR or RGB (16 bits per pixel, 555 or 565).
   Support only CC_8U images (input 3 or 4 channels, output 2 channels).
 */
inline int hal_ni_cvtBGRtoBGR5x5(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int scn, bool swapBlue, int greenBits) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGR5x5toBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R destination channels will be swapped (write RGB)
   @param greenBits number of bits for green channel (5 or 6)
   Convert from packed BGR or RGB (16 bits per pixel, 555 or 565) to BGR, BGRA, RGB and RGBA.
   Support only CC_8U images (input 2 channels, output 3 or 4 channels).
 */
inline int hal_ni_cvtBGR5x5toBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int dcn, bool swapBlue, int greenBits) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGRtoGray
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U, CC_16U or CC_32F)
   @param scn source image channels (3 or 4)
   @param swapBlue if set to true B and R source channels will be swapped (treat as RGB)
   Convert from BGR, BGRA, RGB or RGBA to 1-channel gray.
 */
inline int hal_ni_cvtBGRtoGray(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int scn, bool swapBlue) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtGraytoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U, CC_16U or CC_32F)
   @param dcn destination image channels (3 or 4)
   Convert from 1-channel gray to BGR, RGB, RGBA or BGRA.
 */
inline int hal_ni_cvtGraytoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int dcn) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGR5x5toGray
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param greenBits number of bits for green channel (5 or 6)
   Convert from packed BGR (16 bits per pixel, 555 or 565) to 1-channel gray.
   Support only CC_8U images.
 */
inline int hal_ni_cvtBGR5x5toGray(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int greenBits) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtGraytoBGR5x5
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param greenBits number of bits for green channel (5 or 6)
   Convert from 1-channel gray to packed BGR (16 bits per pixel, 555 or 565).
   Support only CC_8U images.
 */
inline int hal_ni_cvtGraytoBGR5x5(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int greenBits) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGRtoYUV
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U, CC_16U or CC_32F)
   @param scn source image channels (3 or 4)
   @param swapBlue if set to true B and R source channels will be swapped (treat as RGB)
   @param isCbCr if set to true write output in YCbCr format
   Convert from BGR, RGB, BGRA or RGBA to YUV or YCbCr.
 */
inline int hal_ni_cvtBGRtoYUV(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int scn, bool swapBlue, bool isCbCr) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtYUVtoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U, CC_16U or CC_32F)
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R destination channels will be swapped (write RGB)
   @param isCbCr if set to true treat source as YCbCr
   Convert from YUV or YCbCr to BGR, RGB, BGRA or RGBA.
 */
inline int hal_ni_cvtYUVtoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int dcn, bool swapBlue, bool isCbCr) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGRtoXYZ
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U, CC_16U or CC_32F)
   @param scn source image channels (3 or 4)
   @param swapBlue if set to true B and R source channels will be swapped (treat as RGB)
   Convert from BGR, RGB, BGRA or RGBA to XYZ.
 */
inline int hal_ni_cvtBGRtoXYZ(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int scn, bool swapBlue) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtXYZtoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U, CC_16U or CC_32F)
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R destination channels will be swapped (write RGB)
   Convert from XYZ to BGR, RGB, BGRA or RGBA.
 */
inline int hal_ni_cvtXYZtoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int dcn, bool swapBlue) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGRtoHSV
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U or CC_32F)
   @param scn source image channels (3 or 4)
   @param swapBlue if set to true B and R source channels will be swapped (treat as RGB)
   @param isFullRange if set to true write hue in range 0-255 (0-360 for float) otherwise in range 0-180
   @param isHSV if set to true write HSV otherwise HSL
   Convert from BGR, RGB, BGRA or RGBA to HSV or HSL.
 */
inline int hal_ni_cvtBGRtoHSV(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int scn, bool swapBlue, bool isFullRange, bool isHSV) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtHSVtoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U or CC_32F)
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R destination channels will be swapped (write RGB)
   @param isFullRange if set to true read hue in range 0-255 (0-360 for float) otherwise in range 0-180
   @param isHSV if set to true treat source as HSV otherwise HSL
   Convert from HSV or HSL to BGR, RGB, BGRA or RGBA.
 */
inline int hal_ni_cvtHSVtoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int dcn, bool swapBlue, bool isFullRange, bool isHSV) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGRtoLab
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U or CC_32F)
   @param scn source image channels (3 or 4)
   @param swapBlue if set to true B and R source channels will be swapped (treat as RGB)
   @param isLab if set to true write Lab otherwise Luv
   @param srgb if set to true use sRGB gamma correction
   Convert from BGR, RGB, BGRA or RGBA to Lab or Luv.
 */
inline int hal_ni_cvtBGRtoLab(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int scn, bool swapBlue, bool isLab, bool srgb) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtLabtoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param depth image depth (one of CC_8U or CC_32F)
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R destination channels will be swapped (write RGB)
   @param isLab if set to true treat input as Lab otherwise Luv
   @param srgb if set to true use sRGB gamma correction
   Convert from Lab or Luv to BGR, RGB, BGRA or RGBA.
 */
inline int hal_ni_cvtLabtoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int depth, int dcn, bool swapBlue, bool isLab, bool srgb) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtTwoPlaneYUVtoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param dst_width,dst_height destination image size
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R destination channels will be swapped (write RGB)
   @param uIdx U-channel index in the interleaved U/V plane (0 or 1)
   Convert from YUV (YUV420sp (or NV12/NV21) - Y plane followed by interleaved U/V plane) to BGR, RGB, BGRA or RGBA.
   Only for CC_8U.
 */
inline int hal_ni_cvtTwoPlaneYUVtoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int dst_width, int dst_height, int dcn, bool swapBlue, int uIdx) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtThreePlaneYUVtoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param dst_width,dst_height destination image size
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R destination channels will be swapped (write RGB)
   @param uIdx U-channel plane index (0 or 1)
   Convert from YUV (YUV420p (or YV12/YV21) - Y plane followed by U and V planes) to BGR, RGB, BGRA or RGBA.
   Only for CC_8U.
 */
inline int hal_ni_cvtThreePlaneYUVtoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int dst_width, int dst_height, int dcn, bool swapBlue, int uIdx) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtBGRtoThreePlaneYUV
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param scn source image channels (3 or 4)
   @param swapBlue if set to true B and R source channels will be swapped (treat as RGB)
   @param uIdx U-channel plane index (0 or 1)
   Convert from BGR, RGB, BGRA or RGBA to YUV (YUV420p (or YV12/YV21) - Y plane followed by U and V planes).
   Only for CC_8U.
 */
inline int hal_ni_cvtBGRtoThreePlaneYUV(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int scn, bool swapBlue, int uIdx) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtOnePlaneYUVtoBGR
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   @param dcn destination image channels (3 or 4)
   @param swapBlue if set to true B and R destination channels will be swapped (write RGB)
   @param uIdx U-channel index (0 or 1)
   @param ycn Y-channel index (0 or 1)
   Convert from UYVY, YUY2 or YVYU to BGR, RGB, BGRA or RGBA.
   Only for CC_8U.
 */
inline int hal_ni_cvtOnePlaneYUVtoBGR(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height, int dcn, bool swapBlue, int uIdx, int ycn) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }


/**
   @brief hal_cvtRGBAtoMultipliedRGBA
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   Convert from BGRA or RGBA to format with multiplied alpha channel.
   Only for CC_8U.
 */
inline int hal_ni_cvtRGBAtoMultipliedRGBA(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

/**
   @brief hal_cvtMultipliedRGBAtoRGBA
   @param src_data,src_step source image data and step
   @param dst_data,dst_step destination image data and step
   @param width,height image size
   Convert from format with multiplied alpha channel to BGRA or RGBA.
   Only for CC_8U.
 */
inline int hal_ni_cvtMultipliedRGBAtoRGBA(const uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#define cv_hal_cvtBGRtoBGR hal_ni_cvtBGRtoBGR
#define cv_hal_cvtBGRtoBGR5x5 hal_ni_cvtBGRtoBGR5x5
#define cv_hal_cvtBGR5x5toBGR hal_ni_cvtBGR5x5toBGR
#define cv_hal_cvtBGRtoGray hal_ni_cvtBGRtoGray
#define cv_hal_cvtGraytoBGR hal_ni_cvtGraytoBGR
#define cv_hal_cvtBGR5x5toGray hal_ni_cvtBGR5x5toGray
#define cv_hal_cvtGraytoBGR5x5 hal_ni_cvtGraytoBGR5x5
#define cv_hal_cvtBGRtoYUV hal_ni_cvtBGRtoYUV
#define cv_hal_cvtYUVtoBGR hal_ni_cvtYUVtoBGR
#define cv_hal_cvtBGRtoXYZ hal_ni_cvtBGRtoXYZ
#define cv_hal_cvtXYZtoBGR hal_ni_cvtXYZtoBGR
#define cv_hal_cvtBGRtoHSV hal_ni_cvtBGRtoHSV
#define cv_hal_cvtHSVtoBGR hal_ni_cvtHSVtoBGR
#define cv_hal_cvtBGRtoLab hal_ni_cvtBGRtoLab
#define cv_hal_cvtLabtoBGR hal_ni_cvtLabtoBGR
#define cv_hal_cvtTwoPlaneYUVtoBGR hal_ni_cvtTwoPlaneYUVtoBGR
#define cv_hal_cvtThreePlaneYUVtoBGR hal_ni_cvtThreePlaneYUVtoBGR
#define cv_hal_cvtBGRtoThreePlaneYUV hal_ni_cvtBGRtoThreePlaneYUV
#define cv_hal_cvtOnePlaneYUVtoBGR hal_ni_cvtOnePlaneYUVtoBGR
#define cv_hal_cvtRGBAtoMultipliedRGBA hal_ni_cvtRGBAtoMultipliedRGBA
#define cv_hal_cvtMultipliedRGBAtoRGBA hal_ni_cvtMultipliedRGBAtoRGBA
//! @endcond

/**
   @brief Calculate integral image
   @param depth,sdepth,sqdepth Depths of source image, sum image and square sum image
   @param src_data,src_step Source image
   @param sum_data,sum_step Sum image
   @param sqsum_data,sqsum_step Square sum image
   @param tilted_data,tilted_step Tilted sum image
   @param width,height Source image dimensions
   @param cn Number of channels
   @note Following combinations of image depths are used:
   Source | Sum | Square sum
   -------|-----|-----------
   CC_8U | CC_32S | CC_64F
   CC_8U | CC_32S | CC_32F
   CC_8U | CC_32S | CC_32S
   CC_8U | CC_32F | CC_64F
   CC_8U | CC_32F | CC_32F
   CC_8U | CC_64F | CC_64F
   CC_16U | CC_64F | CC_64F
   CC_16S | CC_64F | CC_64F
   CC_32F | CC_32F | CC_64F
   CC_32F | CC_32F | CC_32F
   CC_32F | CC_64F | CC_64F
   CC_64F | CC_64F | CC_64F
   @sa integral
*/
inline int hal_ni_integral(int depth, int sdepth, int sqdepth, const uchar * src_data, size_t src_step, uchar * sum_data, size_t sum_step, uchar * sqsum_data, size_t sqsum_step, uchar * tilted_data, size_t tilted_step, int width, int height, int cn) { return CC_HAL_ERROR_NOT_IMPLEMENTED; }

//! @cond IGNORED
#define cv_hal_integral hal_ni_integral
//! @endcond

//! @}

#if defined __GNUC__
#  pragma GCC diagnostic pop
#elif defined _MSC_VER
#  pragma warning( pop )
#endif



#endif
