
if (1)
{
  int poscntha;
  double* pdata = MALLOC(double, pos_num);

  for (i = 0; i < samp_num; ++i) {
    if (wi[ i ] > 0.f) {
      int aa = i < pos_num;

      if (aa) {
        pdata[ samp_cnt[ 1 ] ] = vi[ i ];
      }

      samp_cnt[ aa ] ++;
    }
  }

  // ��ha�㷧ֵ
  poscntha = (int)floor((1 - ha) * samp_cnt[ 1 ]);
  poscntha = MIN(MAX(poscntha, 0), samp_cnt[ 1 ] - 1);
  QSORT(pdata, pdata + samp_cnt[ 1 ], LESS, double);
  stage->thd = (float)(pdata[ poscntha ] - x_esp);
  FREE(pdata);
}

// ��ͨ����Ŀ
for (i = 0; i < samp_num; ++i)
{
  if (wi[ i ] > 0.f) {
    if (vi[ i ] > stage->thd) {
      // ��ʾͨ����
      pass_cnt[ i < pos_num ] ++;
    }
  }
}

pwi->Ha = (double) pass_cnt[ 1 ] / samp_cnt[ 1 ];
pwi->Fa = (double) pass_cnt[ 0 ] / samp_cnt[ 0 ];
print_macro printweak(pwi, weak, "\n");

ca->weak_len++, ++weak;

if (pwi->Ha >= ha && pwi->Fa <= fa)
{
  stage->len = ca->weak_len - weaklen;
  break;
}
}

for (i = 0; i < samp_num; ++i)
{
  if (wi[ i ] > 0.f) {
    if (vi[ i ] < stage->thd) {
      // û��ͨ��ǿ��������������Ȩֵ��-1����ʾ��������
      wi[ i ] = -1.f;
    }
  }
}
}
FREE(vi);
FREE(wi);
FREE(fi);
FREE(flag);
}
#undef get_pos_macro
#undef get_neg_macro
#undef get_feat_macro
#undef print_macro
#undef get_feat_macro
#undef printweak
