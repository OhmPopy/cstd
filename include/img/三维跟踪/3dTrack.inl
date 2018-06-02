#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include "3dTrack.h"


// 09.28. ���Ӻ͹켣������صı����ʹ���.
// 09.27. ������������ж�����: ����ǳ���, ���Ϸ��Ѵ���.


// ������ֵ.
const int FADE_CONFIRM = 1; // ��ǰδʹ��, ��ʧ�Ĺ켣����ɾ��.
const int NORM_CONFIRM = 15;
const int COMB_NEW_THD = 10;
const int SPLT_CONFIRM = 5;
const int EDGE_CONFIRM = 3;


// �������Ա����.

double TriDimTracker::MoveDirection( int preX, int preY, int curX, int curY ) {
  const double pi = 3.1415926;
  
  if ( curX == preX ) {
    // Ŀ�꾲ֹ.
    if ( curY == preY ) {
      return 0.0;
    }
    else {
      // Ŀ�괹ֱ�����˶�.
      if ( curY > preY ) {
        return 90.0;
      }
      
      // Ŀ�괹ֱ�����˶�.
      else {
        return 270.0;
      }
    }
  }
  else if ( curY == preY ) {
    // Ŀ��ˮƽ�����˶�.
    if ( curX > preX ) {
      return 0.0;
    }
    
    // Ŀ��ˮƽ�����˶�.
    else {
      return 180.0;
    }
  }
  
  // Ŀ���˶�������ˮƽ����ֱ������.
  else {
    double angle = atan( double( curY - preY ) / double( curX - preX ) );
    
    if ( curX < preX ) {
      angle += pi;
    }
    else if ( curY < preY && curX > preX ) {
      angle += ( pi + pi );
    }
    
    return 180.0 * angle / pi;
  }
}

void TriDimTracker::AnalyzeMem( int **norm, int **emerge, int **fade, int **combine, int **split, int num ) {
  *norm = new int[ m_CurObjNum * num ];
  *combine = new int[ m_CurObjNum * num + num ];
  *split = new int[ m_CurObjNum * num + m_CurObjNum ];
  *emerge = new int[ num ];
  *fade = new int[ m_CurObjNum ];
  
  if ( NULL == *norm || NULL == *combine || NULL == *split || NULL == *emerge || NULL == *fade ) {
    exit( -1 );
  }
  
  memset( *norm, 0, m_CurObjNum * num * sizeof( int ) );
  memset( *combine, 0, ( m_CurObjNum * num + num ) * sizeof( int ) );
  memset( *split, 0, ( m_CurObjNum * num + m_CurObjNum ) * sizeof( int ) );
  memset( *emerge, 0, num * sizeof( int ) );
  memset( *fade, 0, m_CurObjNum * sizeof( int ) );
}

void TriDimTracker::DistMatrix( const int *map, uchar **binList, int num, uchar *relation ) {
  uchar * bin = new uchar[ m_SceneH * m_SceneW ];
  
  if ( NULL == bin ) {
    exit( -1 );
  }
  
  memset( relation, 0, m_CurObjNum * num * sizeof( uchar ) );
  
  for ( int i = 0; i < m_CurObjNum; ++i ) {
    TrackTarget *&pt = *( m_TargetList + *( map + i ) );
    
    assert( NULL != pt );
    
    Real2Bin( pt->real, bin, m_SceneH, m_SceneW );
    
    for ( int j = 0; j < num; ++j ) {
      if ( BinImgInter( bin, *( binList + j ), m_SceneH, m_SceneW ) > pt->area / 20 ) {
        *( relation + i * num + j ) = 1;
      }
    }
  }
  
  delete [] bin;
  bin = NULL;
}

void TriDimTracker::AnalyzeDistMat( const int *map,
                                    const uchar *relation,
                                    int num,
                                    int *norm,
                                    int *emerge,
                                    int *fade,
                                    int *combine,
                                    int *split ) {
  int i, j, k, sum;
  
  // ���켣��Ӧ�������.
  for ( i = 0; i < m_CurObjNum; ++i ) {
    for ( j = 0; j < num; ++j ) {
      if ( *( relation + i * num + j ) > 0 ) {
        for ( k = j + 1, sum = 0; k < num; ++k ) {
          sum += *( relation + i * num + k );
        }
        
        if ( 0 == sum ) {
          for ( k = 0, sum = 0; k < m_CurObjNum; ++k ) {
            sum += *( relation + k * num + j );
          }
          
          if ( sum == 1 ) {
            *( norm + i * num + j ) = 1;
          }
        }
        
        break;
      }
    }
  }
  
  // ��ʧ, ����.
  for ( i = 0; i < m_CurObjNum; ++i ) {
    for ( j = 0, sum = 0; j < num; ++j ) {
      sum += *( relation + i * num + j );
    }
    
    if ( 0 == sum ) {
      *( fade + i ) = 1;
    }
    else if ( sum > 1 ) {
      int * p = split + i * ( num + 1 );
      
      *( p + 0 ) = sum;
      
      for ( j = 0, k = 1; j < num; ++j ) {
        if ( *( relation + i * num + j ) > 0 ) {
          *( p + k++ ) = j;
        }
      }
    }
  }
  
  // ��Ŀ��, �켣�ϲ�.
  for ( i = 0; i < num; ++i ) {
    for ( j = 0, sum = 0; j < m_CurObjNum; ++j ) {
      sum += *( relation + j * num + i );
    }
    
    if ( sum == 0 ) {
      *( emerge + i ) = 1;
    }
    else if ( sum > 1 ) {
      int * p = combine + i * ( m_CurObjNum + 1 );
      
      *( p + 0 ) = sum;
      
      for ( j = 0, k = 1; j < m_CurObjNum; ++j ) {
        if ( *( relation + j * num + i ) > 0 ) {
          *( p + k++ ) = j;
        }
      }
    }
  }
}

void TriDimTracker::ProcFade( const int *map, const int *fade, const Calibration *pCbr ) {
  assert( NULL != map && NULL != fade && NULL != pCbr );
  
  for ( int i = 0; i < m_CurObjNum; ++i ) {
    if ( 1 == *( fade + i ) ) {
      TrackTarget *&pt = *( m_TargetList + *( map + i ) );
      
      assert( NULL != pt );
      
      pt->s = ST_DELETE;
    }
  }
}

int TriDimTracker::ProcEmerge( const uchar *src,
                               const int *emerge,
                               uchar **bin,
                               const int *rect,
                               const double *feet,
                               const int *area,
                               const int *center,
                               const int *feeti,
                               const double *len,
                               const double *compact,
                               int num ) {
  int loc, inc = 0;
  
  for ( int i = 0; i < num; ++i ) {
    // �ж�Ϊ�³��ֵ��ſ�.
    if ( 1 == *( emerge + i ) ) {
      // ���ҿ��й켣ָ��.
      loc = NullPtr();
      
      // �켣��Ŀ�Ѵﵽ�������Ŀ, �޷����ӹ켣.
      if ( 0 > loc ) {
        return inc;
      }
      
      // ���ӹ켣.
      else {
        // �켣��Ϣ��ʼ��.
        InitTrace( loc,             // �켣�ڹ켣ָ���е�����.
                   center + i * 2,  // ������������.
                   rect + i * 4,    // �ſ��������.
                   feet + i * 2,    // �ſ�ײ���������.
                   *( area + i ),      // �ſ����.
                   feeti + i * 2,   // �ſ�ײ���������.
                   *( len + i ),       // �ſ�ʵ�ʸ߶�.
                   *( compact + i ) ); // �ſ��ʵ��.
                   
        // ��ȡĿ���ֵͼ��Ӧλ�õ���ɫ��Ϣ.
        ColorMsg( src, *( bin + i ), m_SceneH, m_SceneW, ( *( m_TargetList + loc ) ) ->real );
        
        // ���¹켣�䶯����.
        ++inc;
      }
    }
  }
  
  return inc;
}

void TriDimTracker::ProcOne2One( const uchar *src,
                                 const int *map,
                                 const int *norm,
                                 uchar **bin,
                                 const int *rect,
                                 const double *feet,
                                 const int *area,
                                 const int *center,
                                 const int *feeti,
                                 const double *len,
                                 const double *compact,
                                 const Calibration *pCbr,
                                 int num ) {
  for ( int i = 0; i < m_CurObjNum; ++i ) {
    for ( int j = 0; j < num; ++j ) {
      if ( 1 == *( norm + i * num + j ) ) {
        TrackTarget *&pt = *( m_TargetList + *( map + i ) );
        
        assert( NULL != pt && ST_DELETE != pt->s );
        
        // ����״̬, ���ӵ÷�(���Ŷ�).
        if ( ST_NORMAL == pt->s ) {
          ++( pt->score );
        }
        
        // �¹켣, �ж��Ƿ�ﵽת��Ϊ����״̬������.
        else if ( ST_EMERGE == pt->s ) {
          ++( pt->score );
          
          if ( NORM_CONFIRM < pt->score ) {
            pt->s = ST_NORMAL;
            pt->score = 1;
          }
        }
        
        // ������ʧ�еĹ켣.
        else if ( ST_FADING == pt->s ) {
          pt->s = ST_EMERGE;
          pt->score = 1;
        }
        
        // �����ںϲ����������еĹ켣.
        else if ( ST_COMBINE == pt->s || ST_SPLIT == pt->s ) {
          pt->s = ST_NORMAL;
          pt->score = 1;
        }
        
        // ������ɾ���Ĺ켣.
        else {
          assert( ST_DELETE == pt->s );
          continue;
        }
        
        // ��ɫ��Ϣ����Ϊ��ǰ֡��Ӧ�Ķ�ֵ���λ����ɫ��Ϣ.
        ColorMsg( src, *( bin + j ), m_SceneH, m_SceneW, pt->real );
        
        // �����Ϣ.
        pt->area = *( area + j );
        
        // ������Ϣ(����������Ϣ����֮ǰ).
        pt->direction = MoveDirection( pt->rect.w, pt->rect.h, *( center + j * 2 + 1 ), *( center + j * 2 + 0 ) );
        
        // ������Ϣ.
        pt->rect.h = *( center + j * 2 + 0 );
        pt->rect.w = *( center + j * 2 + 1 );
        
        // ������Ϣ.
        pt->rect.l = *( rect + j * 4 + 0 );
        pt->rect.t = *( rect + j * 4 + 1 );
        pt->rect.r = *( rect + j * 4 + 2 );
        pt->rect.b = *( rect + j * 4 + 3 );
        
        // �ײ�ͼ������.
        *( pt->fti + 0 ) = *( feeti + j * 2 + 0 );
        *( pt->fti + 1 ) = *( feeti + j * 2 + 1 );
        
        // �ײ���������.
        *( pt->ftw + 0 ) = *( feet + j * 2 + 0 );
        *( pt->ftw + 1 ) = *( feet + j * 2 + 1 );
        
        // ʵ�ʸ߶�.
        pt->len = *( len + j );
        
        // ��ʵ��.
        pt->cmt = *( compact + j );
        
        // Ԥ���������.
        pt->pred.update( pCbr->GetTMat(), pt->ftw );
        
        // ����ʵ������, Ӧ�÷���Ԥ��״̬����֮��.
        pt->speed = pt->pred.GetSpeed();
        
        // Ԥ��.
        pt->pred.predict();
      }
    }
  }
}

int TriDimTracker::ProcSplit( const uchar *src,
                              const int *map,
                              const int *split,
                              uchar **bin,
                              const int *rect,
                              const double *feet,
                              const int *area,
                              const int *center,
                              const int *feeti,
                              const double *len,
                              const double *compact,
                              const Calibration *pCbr,
                              int num ) {
  int loc, inc = 0;
  
  for ( int i = 0; i < m_CurObjNum; ++i ) {
    if ( 1 < *( split + i * ( num + 1 ) + 0 ) ) {
      TrackTarget *&pt = *( m_TargetList + *( map + i ) );
      
      assert( NULL != pt );
      
      // ״̬����.
      if ( ST_DELETE == pt->s ) {
        continue;
      }
      
      if ( ST_EMERGE == pt->s || ST_FADING == pt->s ) {
        pt->s = ST_DELETE;
      }
      else {
        // ����״̬�÷�δ�ﵽ��ֵ, ����Ϊһ�����崦��.
        if ( ( ST_SPLIT == pt->s && SPLT_CONFIRM > ++( pt->score ) && pt->attri != VEHICLE ) || ST_NORMAL == pt->s || ST_COMBINE == pt->s ) {
          // �����ͺϲ�״̬����.
          if ( ST_SPLIT != pt->s ) {
            pt->s = ST_SPLIT;
            pt->score = 1;
          }
          
          // �����ֵͼ��Ϣ�ڴ�.
          uchar *comb = new uchar[ m_SceneH * m_SceneW ];
          
          if ( NULL == comb ) {
            exit( -1 );
          }
          
          memset( comb, 0, m_SceneH * m_SceneW * sizeof( uchar ) );
          
          // ��ȡ��ֵͼ�����Ϣ.
          for ( int j = 0; j < *( split + i * ( num + 1 ) + 0 ); ++j ) {
            const int pos = *( split + i * ( num + 1 ) + ( j + 1 ) );
            
            for ( int k = 0; k < m_SceneH * m_SceneW; ++k ) {
              *( comb + k ) |= *( *( bin + pos ) + k );
            }
          }
          
          // ��ɫ��Ϣ.
          ColorMsg( src, comb, m_SceneH, m_SceneW, pt->real );
          
          // �ſ���Ϣ.
          BinMap<uchar> tmpBlock( comb, 1, m_SceneH, m_SceneW );
          
          // �����Ϣ.
          pt->area = *( tmpBlock.GetArea() + 0 );
          
          // ������Ϣ.
          pt->direction = MoveDirection( pt->rect.w, pt->rect.h, *( tmpBlock.GetCentroid() + 1 ), *( tmpBlock.GetCentroid() + 0 ) );
          
          // ������Ϣ.
          pt->rect.h = *( tmpBlock.GetCentroid() + 0 );
          pt->rect.w = *( tmpBlock.GetCentroid() + 1 );
          
          // ������Ϣ.
          pt->rect.l = *( tmpBlock.GetRect() + 0 );
          pt->rect.t = *( tmpBlock.GetRect() + 1 );
          pt->rect.r = *( tmpBlock.GetRect() + 2 );
          pt->rect.b = *( tmpBlock.GetRect() + 3 );
          
          // ʵ�ʳ���, ��ʵ��, �ײ���������, �ײ�ͼ������.
          pCbr->TriDimInfo( comb,
                            m_SceneH,
                            m_SceneW,
                            pt->area,
                            tmpBlock.GetRect(),
                            &( pt->len ),
                            &( pt->cmt ),
                            pt->ftw,
                            pt->fti );
                            
          // Ԥ����Ϣ����.
          pt->pred.update( pCbr->GetTMat(), pt->ftw );
          
          // ����ʵ������, Ӧ�÷���Ԥ��״̬����֮��.
          pt->speed = pt->pred.GetSpeed();
          
          // Ԥ��.
          pt->pred.predict();
          
          // �ͷ��ڴ�.
          delete [] comb;
          comb = NULL;
        }
        
        // ����״̬ȷ��, �����ſ�ȫ���������ſ鴦��.
        else {
          // ɾ��ԭ�켣�������Ѻ�ĵ�һ�����Ϣ���ɹ켣�ɸ�ָ��ָ��, �ù켣����ԭ�켣ID.
          int index = *( split + i * ( num + 1 ) + 1 );
          
          // ����ԭID.
          int oldID = pt->label;
          
          delete pt;
          pt = NULL;
          
          pt = new TrackTarget( *( len + index ),             // Ŀ��ʵ�ʸ߶�.
                                *( compact + index ),         // Ŀ���ʵ��.
                                feet + index * 2,          // Ŀ��ײ���������.
                                feeti + index * 2,         // Ŀ��ײ���������.
                                *( center + index * 2 + 0 ),  // Ŀ��������������.
                                *( center + index * 2 + 1 ),
                                *( rect + index * 4 + 0 ),    // Ŀ���������.
                                *( rect + index * 4 + 1 ),
                                *( rect + index * 4 + 2 ),
                                *( rect + index * 4 + 3 ),
                                *( area + index ),            // Ŀ�����.
                                oldID,                     // ����ԭID.
                                m_SceneH,
                                m_SceneW );
                                
          // ���켣�����Ƿ�ɹ�.
          if ( NULL == pt )
            exit( -1 );
            
          pt->pred.predict();
          
          // ��ɫ��Ϣ.
          ColorMsg( src, *( bin + index ), m_SceneH, m_SceneW, pt->real );
          
          // ���������Ѻ���ſ��½��켣.
          for ( int j = 1; j < *( split + i * ( num + 1 ) + 0 ); ++j ) {
            index = *( split + i * ( num + 1 ) + ( j + 1 ) );
            
            // ��ȡ���й켣ָ��.
            loc = NullPtr();
            
            // �켣��Ŀ�Ѵﵽ�������Ŀ, �޷����ӹ켣.
            if ( 0 > loc ) {
              return inc;
            }
            
            // ���ӹ켣.
            else {
              InitTrace( loc,                 // �켣����.
                         center + index * 2,  // ��������.
                         rect + index * 4,    // Ŀ�����.
                         feet + index * 2,    // Ŀ��ײ���������.
                         *( area + index ),      // Ŀ�����.
                         feeti + index * 2,   // Ŀ��ײ���������.
                         *( len + index ),       // Ŀ��ʵ�ʸ߶�.
                         *( compact + index ) ); // Ŀ���ʵ��.
                         
              // Ŀ����ɫ��Ϣ.
              ColorMsg( src, *( bin + index ), m_SceneH, m_SceneW, ( *( m_TargetList + loc ) ) ->real );
              
              ++inc;
            }
          }
        }
      }
    }
  }
  
  return inc;
}

void TriDimTracker::ProcMerge( const uchar *src,
                               const int *map,
                               const int *combine,
                               uchar **bin,
                               const int *rect,
                               const double *feet,
                               const int *area,
                               const int *center,
                               const int *feeti,
                               const double *len,
                               const double *compact,
                               const Calibration *pCbr,
                               int num ) {
  uchar * rgb = new uchar[ m_SceneH * m_SceneW * 3 ];
  uchar *tmpRGB = new uchar[ m_SceneH * m_SceneW * 3 ];
  uchar *tmpBin = new uchar[ m_SceneH * m_SceneW ];
  
  if ( NULL == rgb || NULL == tmpRGB || NULL == tmpBin ) {
    exit( -1 );
  }
  
  for ( int i = 0; i < num; ++i ) {
    // ��ǰ������ſ��ɶ����켣�ϲ�����.
    if ( 1 < *( combine + i * ( m_CurObjNum + 1 ) + 0 ) ) {
      // ���кϲ��ſ��״̬��ΪEMERGEʱɾ�����кϲ��ſ��Ӧ�켣.
      for ( int j = 0, sum_new = 0; j < *( combine + i * ( m_CurObjNum + 1 ) + 0 ); ++j ) {
        TrackTarget *&pt = *( m_TargetList + *( map + *( combine + i * ( m_CurObjNum + 1 ) + ( j + 1 ) ) ) );
        
        if ( pt->s == ST_EMERGE ) {
          ++sum_new;
        }
      }
      
      if ( sum_new == *( combine + i * ( m_CurObjNum + 1 ) + 0 ) ) {
        for ( j = 0; j < *( combine + i * ( m_CurObjNum + 1 ) + 0 ); ++j ) {
          TrackTarget *&pt = *( m_TargetList + *( map + *( combine + i * ( m_CurObjNum + 1 ) + ( j + 1 ) ) ) );
          
          pt->s = ST_DELETE;
        }
        
        continue;
      }
      
      // �ϲ��ſ��Ӧ�Ĳ�ɫ��Ϣ.
      ColorMsg( src, *( bin + i ), m_SceneH, m_SceneW, rgb );
      
      for ( j = 0; j < *( combine + i * ( m_CurObjNum + 1 ) + 0 ); ++j ) {
        // ��ȡ�����ϲ��Ĺ켣ָ��.
        TrackTarget *&pt = *( m_TargetList + *( map + *( combine + i * ( m_CurObjNum + 1 ) + ( j + 1 ) ) ) );
        
        assert( NULL != pt );
        
        // ״̬����.
        if ( ST_DELETE == pt->s ) {
          continue;
        }
        
        if ( ST_FADING == pt->s ) {
          pt->s = ST_DELETE;
        }
        
        // ���ſ���Ѱ����ƥ���λ��.
        else {
          // ���ڳ�����Ե�Ĺ켣״̬��Ϊɾ��.
          if ( TraceOnBorder( pt, EDGE_CONFIRM ) > 0 || ( pt->s == ST_EMERGE && pt->score < COMB_NEW_THD ) ) {
            pt->s = ST_DELETE;
          }
          else {
            // ״̬��Ϣ.
            if ( ST_COMBINE == pt->s ) {
              ++( pt->score );
            }
            else {
              pt->s = ST_COMBINE;
              pt->score = 1;
            }
            
            // ����������Χ.
            double minFW[ 2 ], maxFW[ 2 ];
            
            int minFI[ 2 ], maxFI[ 2 ], ratio;
            
            if ( pt->speed > 400 ) {
              ratio = 7;
            }
            else if ( 200 < pt->speed && pt->speed <= 400 ) {
              ratio = 5;
            }
            else {
              ratio = 3;
            }
            
            maxFW[ 0 ] = pt->pred.GetPredX() + ratio * sqrt( pt->pred.GetVX() );
            maxFW[ 1 ] = pt->pred.GetPredY() + ratio * sqrt( pt->pred.GetVY() );
            
            minFW[ 0 ] = pt->pred.GetPredX() - ratio * sqrt( pt->pred.GetVX() );
            minFW[ 1 ] = pt->pred.GetPredY() - ratio * sqrt( pt->pred.GetVY() );
            
            DimTrans32( pCbr->GetTMat(), maxFW, maxFI );
            DimTrans32( pCbr->GetTMat(), minFW, minFI );
            
            int minx, maxx, miny, maxy;
            
            if ( minFI[ 0 ] < maxFI[ 0 ] ) {
              minx = minFI[ 0 ];
              maxx = maxFI[ 0 ];
            }
            else {
              minx = maxFI[ 0 ];
              maxx = minFI[ 0 ];
            }
            
            if ( minFI[ 1 ] < maxFI[ 1 ] ) {
              miny = minFI[ 1 ];
              maxy = maxFI[ 1 ];
            }
            else {
              miny = maxFI[ 1 ];
              maxy = minFI[ 1 ];
            }
            
            // λ��ƥ��.
            double curDist = 0.0;
            double minDist = 9e99;
            
            int offsetX = 0;
            int offsetY = 0;
            
            int R, G, B;
            
            bool tag = false;
            
            for ( int m = miny; m <= maxy; ++m ) {
              for ( int n = minx; n <= maxx; ++n ) {
                if ( m > 0 && m < m_SceneH - 1 && n > 0 && n < m_SceneW - 1 ) {
                  curDist = 0;
                  
                  Move( pt->real, m_SceneH, m_SceneW, n - pt->fti[ 0 ], m - pt->fti[ 1 ], tmpRGB );
                  
                  // �õ��ƶ����ſ�Ķ�ֵͼ��Ϣ.
                  Real2Bin( tmpRGB, tmpBin, m_SceneH, m_SceneW );
                  
                  if ( BinImgInter( tmpBin, *( bin + i ), m_SceneH, m_SceneW ) >= pt->area / 2 ) {
                    for ( int y = 0; y < m_SceneH; ++y ) {
                      for ( int x = 0; x < m_SceneW; ++x ) {
                        if ( *( rgb + y * m_SceneW * 3 + x * 3 + 0 ) != 0 ||
                             *( rgb + y * m_SceneW * 3 + x * 3 + 1 ) != 0 ||
                             *( rgb + y * m_SceneW * 3 + x * 3 + 2 ) != 0 ) {
                          B = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 0 ) - *( rgb + y * m_SceneW * 3 + x * 3 + 0 );
                          G = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 1 ) - *( rgb + y * m_SceneW * 3 + x * 3 + 1 );
                          R = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 2 ) - *( rgb + y * m_SceneW * 3 + x * 3 + 2 );
                        }
                        else {
                          B = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 0 ) + 255;
                          G = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 1 ) + 255;
                          R = *( tmpRGB + y * m_SceneW * 3 + x * 3 + 2 ) + 255;
                        }
                        
                        curDist += ( B * B + G * G + R * R );
                      }
                    }
                    
                    if ( curDist < minDist ) {
                      tag = true;
                      minDist = curDist;
                      
                      offsetX = n - pt->fti[ 0 ];
                      offsetY = m - pt->fti[ 1 ];
                    }
                  }
                }
              }
            }
            
            if ( !tag ) {
              pt->s = ST_DELETE;
              continue;
            }
            
            // ������Ϣ.
            pt->direction = MoveDirection( pt->rect.w, pt->rect.h, pt->rect.w + offsetX, pt->rect.h + offsetY );
            
            // ���, ����.
            pt->rect.w += offsetX;
            pt->rect.h += offsetY;
            
            pt->rect.l += offsetX;
            pt->rect.t += offsetY;
            pt->rect.r += offsetX;
            pt->rect.b += offsetY;
            
            // Ŀ��ײ���������.
            pt->fti[ 0 ] += offsetX;
            pt->fti[ 1 ] += offsetY;
            
            // Ŀ��ײ���������.
            DimTrans23( pCbr->GetITMat(), pt->fti, pt->ftw );
            
            // ��ɫ��Ϣ�ƶ�.
            ColorMove( pt, offsetX, offsetY );
            
            // Ԥ���������.
            pt->pred.update( pCbr->GetTMat(), pt->ftw );
            
            // ����ʵ������, Ӧ�÷���Ԥ��״̬����֮��.
            pt->speed = pt->pred.GetSpeed();
            
            // Ԥ��.
            pt->pred.predict();
          }
        }
      }
    }
  }
  
  delete [] tmpBin;
  tmpBin = NULL;
  delete [] tmpRGB;
  tmpRGB = NULL;
  delete [] rgb;
  rgb = NULL;
}

void TriDimTracker::InitTrace( int loc,
                               const int *center,
                               const int *rect,
                               const double *feet,
                               int area,
                               const int *feeti,
                               double len,
                               double compact ) {
  TrackTarget *&pt = *( m_TargetList + loc );
  
  pt = new TrackTarget( len,
                        compact,
                        feet,
                        feeti,
                        *( center + 0 ),
                        *( center + 1 ),
                        *( rect + 0 ),
                        *( rect + 1 ),
                        *( rect + 2 ),
                        *( rect + 3 ),
                        area,
                        NewID(),
                        m_SceneH,
                        m_SceneW );
                        
  if ( NULL == pt )
    exit( -1 );
    
  pt->pred.predict();
}

void TriDimTracker::ColorMove( TrackTarget *&pt, int offsetX, int offsetY ) {
  assert( NULL != pt );
  
  uchar *rgb = new uchar[ m_SceneH * m_SceneW * 3 ];
  
  if ( NULL == rgb ) {
    exit( -1 );
  }
  
  // ��ɫ��Ϣ�ƶ�.
  Move( pt->real, m_SceneH, m_SceneW, offsetX, offsetY, rgb );
  memcpy( pt->real, rgb, m_SceneH * m_SceneW * 3 * sizeof( uchar ) );
  
  delete [] rgb;
  rgb = NULL;
}

void TriDimTracker::MultiTrack( const uchar *src, const unsigned int *block, int num, const Calibration *pCbr, const TriDimModel *pTDM ) {
  // ����켣������Ϣ.
  int preObjNum = m_CurObjNum;
  
  // ��ά��Ϣ�ڴ����.
  double *pHeight = new double[ num ];
  double *compact = new double[ num ];
  double *pFeetWP = new double[ num * 2 ];
  
  int *pFeetIP = new int[ num * 2 ];
  int *map = new int[ m_CurObjNum ];
  
  uchar *relation = new uchar[ m_CurObjNum * num ];
  
  if ( NULL == pHeight || NULL == compact || NULL == pFeetWP || NULL == pFeetIP || NULL == map || NULL == relation ) {
    exit( -1 );
  }
  
  int *normal, *emerge, *fade, *combine, *split;
  
  AnalyzeMem( &normal, &emerge, &fade, &combine, &split, num );
  
  // ��ȡ�ſ���Ϣ.
  BinMap<unsigned int> geoMsg( block, num, m_SceneH, m_SceneW );
  
  // �ֿ��ȡ�ſ���ά�����Ϣ.
  for ( int i = 0; i < num; ++i ) {
    pCbr->TriDimInfo( *( geoMsg.GetBinPtr() + i ),
                      m_SceneH,
                      m_SceneW,
                      *( geoMsg.GetArea() + i ),
                      geoMsg.GetRect() + i * 4,
                      pHeight + i,
                      compact + i,
                      pFeetWP + i * 2,
                      pFeetIP + i * 2 );
  }
  
  // �켣λ����˳������ӳ���ϵ.
  TargetMap( map );
  
  // ���ɾ������.
  DistMatrix( map, geoMsg.GetBinPtr(), num, relation );
  
  // �����������.
  AnalyzeDistMat( map, relation, num, normal, emerge, fade, combine, split );
  
  // �����켣��Ӧ������ſ��������.
  ProcOne2One( src,
               map,
               normal,
               geoMsg.GetBinPtr(),
               geoMsg.GetRect(),
               pFeetWP,
               geoMsg.GetArea(),
               geoMsg.GetCentroid(),
               pFeetIP,
               pHeight,
               compact,
               pCbr,
               num );
               
  // ����켣�ſ���ַ��ѵ����.
  preObjNum += ProcSplit( src,
                          map,
                          split,
                          geoMsg.GetBinPtr(),
                          geoMsg.GetRect(),
                          pFeetWP,
                          geoMsg.GetArea(),
                          geoMsg.GetCentroid(),
                          pFeetIP,
                          pHeight,
                          compact,
                          pCbr,
                          num );
                          
  // ����켣��ʧ�����.
  ProcFade( map, fade, pCbr );
  
  // ��������¹켣�����.
  preObjNum += ProcEmerge( src,
                           emerge,
                           geoMsg.GetBinPtr(),
                           geoMsg.GetRect(),
                           pFeetWP,
                           geoMsg.GetArea(),
                           geoMsg.GetCentroid(),
                           pFeetIP,
                           pHeight,
                           compact,
                           num );
                           
  // �����켣�ϲ������.
  ProcMerge( src,
             map,
             combine,
             geoMsg.GetBinPtr(),
             geoMsg.GetRect(),
             pFeetWP,
             geoMsg.GetArea(),
             geoMsg.GetCentroid(),
             pFeetIP,
             pHeight,
             compact,
             pCbr,
             num );
             
  // �ͷŷ����������ʹ�õ��ڴ�.
  FreeAnalyzeMem( &normal, &emerge, &fade, &combine, &split );
  
  // �ڴ��ͷ�.
  delete [] relation;
  relation = NULL;
  delete [] map;
  map = NULL;
  delete [] pHeight;
  pHeight = NULL;
  delete [] compact;
  compact = NULL;
  delete [] pFeetWP;
  pFeetWP = NULL;
  delete [] pFeetIP;
  pFeetIP = NULL;
  
  // ������Ե�Ĺ켣ֱ��ɾ��.
  for ( i = 0; i < m_MaxObjNum; ++i ) {
    TrackTarget *&pt = *( m_TargetList + i );
    
    if ( NULL != pt && TraceOnBorder( pt, EDGE_CONFIRM ) > 0 ) {
      pt->s = ST_DELETE;
    }
  }
  
  // �ͷ���Ҫɾ���Ĺ켣���ڴ�ռ�.
  preObjNum += DelInvalid();
  
  // �켣����.
  TraceClassify( pTDM );
  
  // ���µ�ǰ�켣����.
  m_CurObjNum = preObjNum;
}

void TriDimTracker::TraceClassify( const TriDimModel *pTDM ) {
  for ( int i = 0; i < m_MaxObjNum; ++i ) {
    TrackTarget *&pt = *( m_TargetList + i );
    
    if ( NULL != pt && ST_DELETE != pt->s ) {
      const int r = int( pt->fti[ 1 ] * pTDM->GetHSubNum() / m_SceneH );
      const int c = int( pt->fti[ 0 ] * pTDM->GetWSubNum() / m_SceneW );
      
      assert( -1 < r && r < pTDM->GetHSubNum() && -1 < c && c < pTDM->GetWSubNum() );
      
      const int size = pTDM->GetModelSize( r, c );
      
      if ( pt->area < 1.5 * size && pt->area > 0.5 * size && pt->len >= 100 && pt->len <= 195 ) {
        pt->attri = PERSON;
      }
      else if ( pt->speed < 200 && pt->area >= 1.5 * size && pt->area <= 5 * size ) {
        pt->attri = CROWD;
      }
      else if ( ( pt->speed > 200 && pt->area > 1.5 * size ) || ( pt->area > 5 * size ) ) {
        pt->attri = VEHICLE;
      }
      else {
        pt->attri = UNKNOW;
      }
    }
  }
}
