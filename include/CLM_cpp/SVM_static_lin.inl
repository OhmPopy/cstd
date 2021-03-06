// Copyright (C) 2015, University of Cambridge,
// all rights reserved.
//
// THIS SOFTWARE IS PROVIDED �AS IS� FOR ACADEMIC USE ONLY AND ANY EXPRESS
// OR IMPLIED WARRANTIES WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY.
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Notwithstanding the license granted herein, Licensee acknowledges that certain components
// of the Software may be covered by so-called �open source� software licenses (�Open Source
// Components�), which means any software licenses approved as open source licenses by the
// Open Source Initiative or any substantially similar licenses, including without limitation any
// license that, as a condition of distribution of the software licensed under such license,
// requires that the distributor make the software available in source code format. Licensor shall
// provide a list of Open Source Components for a particular version of the Software upon
// Licensee�s request. Licensee will comply with the applicable terms of such licenses and to
// the extent required by the licenses covering Open Source Components, the terms of such
// licenses will apply in lieu of the terms of this Agreement. To the extent the terms of the
// licenses applicable to Open Source Components prohibit any of the restrictions in this
// License Agreement with respect to such Open Source Component, such restrictions will not
// apply to such Open Source Component. To the extent the terms of the licenses applicable to
// Open Source Components require Licensor to make an offer to provide source code or
// related information in connection with the Software, such offer is hereby made. Any request
// for source code or related information should be directed to cl-face-tracker-distribution@lists.cam.ac.uk
// Licensee acknowledges receipt of notices for the Open Source Components for the initial
// delivery of the Software.

//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite one of the following works:
//
//       Tadas Baltrusaitis, Marwa Mahmoud, and Peter Robinson.
//     Cross-dataset learning and person-specific normalisation for automatic Action Unit detection
//       Facial Expression Recognition and Analysis Challenge 2015,
//       IEEE International Conference on Automatic Face and Gesture Recognition, 2015
//
///////////////////////////////////////////////////////////////////////////////

#include "SVM_static_lin.h"

#include "CLM_core.h"


void SVM_static_lin::Read(std::ifstream& stream, const std::vector<std::string>& au_names)
{

  if (this->means.empty()) {
    ReadMatBin(stream, this->means);
  }
  else {
    Mat_<double> m_tmp;
    ReadMatBin(stream, m_tmp);

    if (norm(m_tmp - this->means > 0.00001)) {
      cout << "Something went wrong with the SVR dynamic regressors" << endl;
    }
  }

  Mat_<double> support_vectors_curr;
  ReadMatBin(stream, support_vectors_curr);

  double bias;
  stream.read((char*)&bias, 8);

  // Read in positive or negative class
  double pos_class;
  stream.read((char*)&pos_class, 8);

  double neg_class;
  stream.read((char*)&neg_class, 8);


  // Add a column vector to the matrix of support vectors (each column is a support vector)
  if (!this->support_vectors.empty()) {
    transpose(this->support_vectors, this->support_vectors);
    transpose(support_vectors_curr, support_vectors_curr);
    this->support_vectors.push_back(support_vectors_curr);
    transpose(this->support_vectors, this->support_vectors);

    transpose(this->biases, this->biases);
    this->biases.push_back(Mat_<double>(1, 1, bias));
    transpose(this->biases, this->biases);

  }
  else {
    this->support_vectors.push_back(support_vectors_curr);
    this->biases.push_back(Mat_<double>(1, 1, bias));
  }

  this->pos_classes.push_back(pos_class);
  this->neg_classes.push_back(neg_class);

  for (size_t i = 0; i < au_names.size(); ++i) {
    this->AU_names.push_back(au_names[i]);
  }
}

// Prediction using the HOG descriptor
void SVM_static_lin::Predict(std::vector<double>& predictions, std::vector<std::string>& names, const Mat_<double>& fhog_descriptor, const Mat_<double>& geom_params)
{
  if (AU_names.size() > 0) {
    Mat_<double> preds;

    if (fhog_descriptor.cols ==  this->means.cols) {
      preds = (fhog_descriptor - this->means) * this->support_vectors + this->biases;
    }
    else {
      Mat_<double> input;
      hconcat(fhog_descriptor, geom_params, input);

      preds = (input - this->means) * this->support_vectors + this->biases;
    }

    for (int i = 0; i < preds.cols; ++i) {
      if (preds.at<double>(i) > 0) {
        predictions.push_back(pos_classes[i]);
      }
      else {
        predictions.push_back(neg_classes[i]);
      }
    }

    names = this->AU_names;
  }
}