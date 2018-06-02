//M*//////////////////////////////////////////////////////////////////////////////////////
//
// IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
// By downloading, copying, installing or using the software you agree to this license.
// If you do not agree to this license, do not download, install,
// copy or use the software.
//
//
// License Agreement
// For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistribution's of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// * Redistribution's in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// * The name of the copyright holders may not be used to endorse or promote products
// derived from this software without specific prior written permission.
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

/****************************************************************************************\
* Exhaustive Linearization for Robust Camera Pose and Focal Length Estimation.
* Contributed by Edgar Riba
\****************************************************************************************/

#ifndef OPENCC_CALIB3D_UPNP_H_
#define OPENCC_CALIB3D_UPNP_H_


#include "opencv2/core/core_c.h"
#include <iostream>

class upnp
{
public:
    upnp(const img_t& cameraMatrix, const img_t& opoints, const img_t& ipoints);
    ~upnp();

    double compute_pose(img_t& R, img_t& t);
private:
    template <typename T>
      void init_camera_parameters(const img_t& cameraMatrix)
      {
        uc = cameraMatrix.at<T> (0, 2);
        vc = cameraMatrix.at<T> (1, 2);
        fu = 1;
        fv = 1;
      }
      template <typename OpointType, typename IpointType>
      void init_points(const img_t& opoints, const img_t& ipoints)
      {
          for(int i = 0; i < number_of_correspondences; i++)
          {
            pws[3 * i    ] = opoints.at<OpointType>(i).x;
            pws[3 * i + 1] = opoints.at<OpointType>(i).y;
            pws[3 * i + 2] = opoints.at<OpointType>(i).z;

            us[2 * i    ] = ipoints.at<IpointType>(i).x;
            us[2 * i + 1] = ipoints.at<IpointType>(i).y;
          }
      }

      double reprojection_error(const double R[3][3], const double t[3]);
      void choose_control_points();
      void compute_alphas();
      void fill_M(img_t * M, const int row, const double * alphas, const double u, const double v);
      void compute_ccs(const double * betas, const double * ut);
      void compute_pcs(void);

      void solve_for_sign(void);

      void find_betas_and_focal_approx_1(img_t * Ut, img_t * Rho, double * betas, double * efs);
      void find_betas_and_focal_approx_2(img_t * Ut, img_t * Rho, double * betas, double * efs);
      void qr_solve(img_t * A, img_t * b, img_t * X);

      img_t compute_constraint_distance_2param_6eq_2unk_f_unk(const img_t& M1);
      img_t compute_constraint_distance_3param_6eq_6unk_f_unk(const img_t& M1, const img_t& M2);
      void generate_all_possible_solutions_for_f_unk(const double betas[5], double solutions[18][3]);

      double sign(const double v);
      double dot(const double * v1, const double * v2);
      double dotXY(const double * v1, const double * v2);
      double dotZ(const double * v1, const double * v2);
      double dist2(const double * p1, const double * p2);

      void compute_rho(double * rho);
      void compute_L_6x12(const double * ut, double * l_6x12);

      void gauss_newton(const img_t * L_6x12, const img_t * Rho, double current_betas[4], double * efs);
      void compute_A_and_b_gauss_newton(const double * l_6x12, const double * rho,
                          const double cb[4], img_t * A, img_t * b, double const f);

      double compute_R_and_t(const double * ut, const double * betas,
                   double R[3][3], double t[3]);

      void estimate_R_and_t(double R[3][3], double t[3]);

      void copy_R_and_t(const double R_dst[3][3], const double t_dst[3],
                  double R_src[3][3], double t_src[3]);


      double uc, vc, fu, fv;

      std::vector<double> pws, us, alphas, pcs;
      int number_of_correspondences;

      double cws[4][3], ccs[4][3];
      int max_nr;
      double * A1, * A2;
};

#endif // OPENCC_CALIB3D_UPNP_H_
