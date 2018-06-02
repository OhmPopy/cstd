/********************************************************************

  wr_tiff.h


  ���ļ���;��  TIFFͼ��дģ�鸨�������ļ�

          ���湦�ܣ��ɽ�1��4��8��24λ�ĵ�ҳ���ҳͼ�󱣴�
                Ϊ��Ӧ��TIFFͼ��

  ���ļ���д�ˣ�  YZ        yzfree##yeah.net

  ���ļ��汾��  20905
  ����޸��ڣ�  2002-9-5

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
      ��ַ�ռ������
    ----------------------------------------------------------------

    ���ô����д�˼���Ȩ��Ϣ��
  ----------------------------------------------------------------
  �������  TIFF�������(LibTiff)   Ŀ¼��libtiff

  Copyright (c) 1988-1997 Sam Leffler
  Copyright (c) 1991-1997 Silicon Graphics, Inc.

  Permission to use, copy, modify, distribute, and sell this software and
  its documentation for any purpose is hereby granted without fee, provided
  that (i) the above copyright notices and this permission notice appear in
  all copies of the software and related documentation, and (ii) the names of
  Sam Leffler and Silicon Graphics may not be used in any advertising or
  publicity relating to the software without the specific, prior written
  permission of Sam Leffler and Silicon Graphics.

  THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
  EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

  IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
  ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
  LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
  OF THIS SOFTWARE.
  ----------------------------------------------------------------

  ������ʷ��

    2002-9    ����ӿ�������2.2�������ڴ�������
    2002-4    ��һ��������


********************************************************************/


#ifndef __WR_TIFF_INC__
#define __WR_TIFF_INC__


#ifndef __ISEE_IRW_INC__
#include "iseeirw.h"          /* ���������ݽӿڶ����ļ� */
#endif


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

  /* �ӿں��� */
  int tiff_write_master_img(TIFF* tif, INFOSTR* pinfo_str);
  int tiff_write_servant_img(TIFF* tif, SUBIMGBLOCK* psub);


#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif

#endif /* __WR_TIFF_INC__ */
