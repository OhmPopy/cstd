/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
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
//   * The name of Intel Corporation may not be used to endorse or promote products
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



typedef struct CvDI
{
    double d;
    int    i;
} CvDI;

static int C_CDECL
icvCmpDI(const void* a, const void* b, void*)
{
    const CvDI* e1 = (const CvDI*) a;
    const CvDI* e2 = (const CvDI*) b;

    return (e1->d < e2->d) ? -1 : (e1->d > e2->d);
}

CC_IMPL void
cvCreateTestSet(int type, img_t** samples,
                 int num_samples,
                 int num_features,
                 img_t** responses,
                 int num_classes, ...)
{
    img_t* mean = NULL;
    img_t* cov = NULL;
    CvMemStorage* storage = NULL;

    CC_FUNCNAME("cvCreateTestSet");

    __BEGIN__;

    if(samples)
        *samples = NULL;
    if(responses)
        *responses = NULL;

    if(type != CC_TS_CONCENTRIC_SPHERES)
        CC_ERROR(CC_StsBadArg, "Invalid type parameter");

    if(!samples)
        CC_ERROR(CC_StsNullPtr, "samples parameter must be not NULL");

    if(!responses)
        CC_ERROR(CC_StsNullPtr, "responses parameter must be not NULL");

    if(num_samples < 1)
        CC_ERROR(CC_StsBadArg, "num_samples parameter must be positive");

    if(num_features < 1)
        CC_ERROR(CC_StsBadArg, "num_features parameter must be positive");

    if(num_classes < 1)
        CC_ERROR(CC_StsBadArg, "num_classes parameter must be positive");

    if(type == CC_TS_CONCENTRIC_SPHERES)
    {
        CvSeqWriter writer;
        CvSeqReader reader;
        img_t sample;
        CvDI elem;
        CvSeq* seq = NULL;
        int i, cur_class;

        CC_CALL(*samples = cvCreateMat(num_samples, num_features, CC_32FC1));
        CC_CALL(*responses = cvCreateMat(1, num_samples, CC_32SC1));
        CC_CALL(mean = cvCreateMat(1, num_features, CC_32FC1));
        CC_CALL(cvSetZero(mean));
        CC_CALL(cov = cvCreateMat(num_features, num_features, CC_32FC1));
        CC_CALL(cvSetIdentity(cov));

        /* fill the feature values matrix with random numbers drawn from standard
           normal distribution */
        CC_CALL(cvRandMVNormal(mean, cov, *samples));

        /* calculate distances from the origin to the samples and put them
           into the sequence along with indices */
        CC_CALL(storage = cvCreateMemStorage());
        CC_CALL(cvStartWriteSeq(0, sizeof(CvSeq), sizeof(CvDI), storage, &writer));
        for(i = 0; i < (*samples)->rows; ++i)
        {
            CC_CALL(cvGetRow(*samples, &sample, i));
            elem.i = i;
            CC_CALL(elem.d = cvNorm(&sample, NULL, CC_L2));
            CC_WRITE_SEQ_ELEM(elem, writer);
        }
        CC_CALL(seq = cvEndWriteSeq(&writer));

        /* sort the sequence in a distance ascending order */
        CC_CALL(cvSeqSort(seq, icvCmpDI, NULL));

        /* assign class labels */
        num_classes = MIN(num_samples, num_classes);
        CC_CALL(cvStartReadSeq(seq, &reader));
        CC_READ_SEQ_ELEM(elem, reader);
        for(i = 0, cur_class = 0; i < num_samples; ++cur_class)
        {
            int last_idx;
            double max_dst;

            last_idx = num_samples * (cur_class + 1) / num_classes - 1;
            CC_CALL(max_dst = (*((CvDI*) cvGetSeqElem(seq, last_idx))).d);
            max_dst = MAX(max_dst, elem.d);

            for(; elem.d <= max_dst && i < num_samples; ++i)
            {
                CC_MAT_ELEM(**responses, int, 0, elem.i) = cur_class;
                if(i < num_samples - 1)
                {
                    CC_READ_SEQ_ELEM(elem, reader);
                }
            }
        }
    }

    __END__;

    if(cvGetErrStatus() < 0)
    {
        if(samples)
            cvReleaseMat(samples);
        if(responses)
            cvReleaseMat(responses);
    }
    cvReleaseMat(&mean);
    cvReleaseMat(&cov);
    cvReleaseMemStorage(&storage);
}


