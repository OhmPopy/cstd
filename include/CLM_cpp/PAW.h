


#ifndef __PAW_h_
#define __PAW_h_

//===========================================================================
/**
    A Piece-wise Affine Warp
  The ideas for this piece-wise affine triangular warping are taken from the
  Active appearance models revisited by Iain Matthews and Simon Baker in IJCV 2004
  This is used for both validation of landmark detection, and for avatar animation

  The code is based on the CLM tracker by Jason Saragih et al.
*/

class PAW
{
public:
  // Number of pixels after the warping to neutral shape
  int     number_of_pixels;

  // Minimum x coordinate in destination
  double  min_x;

  // minimum y coordinate in destination
  double  min_y;

  // Destination points (landmarks to be warped to)
  Mat destination_landmarks;

  // Destination points (landmarks to be warped from)
  Mat source_landmarks;

  // Triangulation, each triangle is warped using an affine transform
  Mat triangulation;

  // Triangle index, indicating which triangle each of destination pixels lies in
  Mat triangle_id;

  // Indicating if the destination warped pixels is valid (lies within a face)
  Mat pixel_mask;

  // A number of precomputed coefficients that are helpful for quick warping

  // affine coefficients for all triangles (see Matthews and Baker 2004)
  // 6 coefficients for each triangle (are computed from alpha and beta)
  // This is computed during each warp based on source landmarks
  Mat coefficients;

  // matrix of (c,x,y) coeffs for alpha
  Mat alpha;

  // matrix of (c,x,y) coeffs for alpha
  Mat beta;

  // x-source of warped points
  Mat map_x;

  // y-source of warped points
  Mat map_y;

  // Default constructor
  PAW() {
    ;
  }

  // Construct a warp from a destination shape and triangulation
  PAW(const Mat& destination_shape, const Mat& triangulation);

  // The final optional argument allows for optimisation if the triangle indices from previous frame are known (for tracking in video)
  PAW(const Mat& destination_shape, const Mat& triangulation, double in_min_x, double in_min_y, double in_max_x, double in_max_y);

  // Copy constructor
  PAW(const PAW& other): destination_landmarks(other.destination_landmarks.clone()), source_landmarks(other.source_landmarks.clone()), triangulation(other.triangulation.clone()),
    triangle_id(other.triangle_id.clone()), pixel_mask(other.pixel_mask.clone()), coefficients(other.coefficients.clone()), alpha(other.alpha.clone()), beta(other.beta.clone()), map_x(other.map_x.clone()), map_y(other.map_y.clone()) {
    this->number_of_pixels = other.number_of_pixels;
    this->min_x = other.min_x;
    this->min_y = other.min_y;
  }

  void Read(FILE* s);

  // The actual warping
  void Warp(const Mat& image_to_warp, Mat& destination_image, const Mat& landmarks_to_warp);

  // Compute coefficients needed for warping
  void CalcCoeff();

  // Perform the actual warping
  void WarpRegion(Mat& map_x, Mat& map_y);

  inline int NumberOfLandmarks() const {
    return destination_landmarks.rows / 2;
  } ;
  inline int NumberOfTriangles() const {
    return triangulation.rows;
  } ;

  // The width and height of the warped image
  inline int constWidth() const {
    return pixel_mask.cols;
  }
  inline int Height() const {
    return pixel_mask.rows;
  }

private:

  int findTriangle(const CPoint2d& point, const double* control_points, int num_tris, int guess = -1) const;

};
//===========================================================================

#endif
