
#include "inifile.h"
#include "ml/feathaar.inl"
//weaktype
#define updateweight LUT_update_weight
#define geterror LUT_get_error
#define findsamp HAAR_find_samp
#define getfeat HAAR_feat
#define uninit_feat HAAR_uninit


#include "ml/adaboost.inl"

int boosttrain_init(BOOST* hh);

int boosttrain_main()
{
#include "ml/adaboost_train_cascade_impl.inl"
  return 0;
}

int boosttrain_init(BOOST* hh)
{
  int maxstagelen, maxweaklen;
  ininame(INIFILENAME);
  iniappname(ADABOOSTSECT);
  hh->ha = inigetfloat("ha");
  hh->fa = inigetfloat("fa");
  hh->min_neg_rate = inigetfloat("��С����������");
  maxstagelen = inigetint("���ǿ��������");
  maxweaklen = inigetint("�������������");
  //inigetfloat( "������ѡ����", hh->select_rate );
  hh->flag_unreselect = inigetint("����ѡ����");
  //hh->curr_pos_num = inigetint( "������������" );
  //hh->pos_num_step = inigetint( "������������" );

  // ����������������
  hh->pos_num = inigetint("��������Ŀ");
  hh->samp_num = inigetint("��������Ŀ") + hh->pos_num;
  cascade_new(hh->ca, "cat.dat", maxstagelen, maxweaklen, sizeof(weaktype));
  hh->ca->is_nesting = inigetint("ʹ��nesting�ṹ");
  HAAR_init(hh);

  logprintf("<< boost ����: lut, ��������: haar, ��������: %6d >>\n", hh->feat_num);

  return 0;
}

int test_adaboost()
{
  boosttrain_main();
  return 0;
}
