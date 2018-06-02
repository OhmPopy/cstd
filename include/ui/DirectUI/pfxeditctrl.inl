#include <imm.h>
#pragma comment(lib, "imm32.lib") 
// C4711: Ϊ����չ��ѡ���˺�����function������Ȼ��������δ���Ϊ������������
// ���ڸú�����ִ��������
//#pragma warning(disable : 4711)
// ����ʹ����CSplitterWnd�����ɸ�CSplitterWnd�ṩˮƽ������ʱ������ñ���
// #define _SYNC_SCROLLING
// ���ƻ��з�����С���
const LONG PFX_EC_MIN_LINE_BREAK_WIDTH = 4;
// �ı���������ı������Ե�������߼���Ԫ��
const LONG PFX_EC_MIN_TEXT_INDENT_LEFT = 5;
// �ı���������ı����ϱ�Ե�������߼���Ԫ��
const LONG PFX_EC_MIN_TEXT_INDENT_TOP = 3;
// ��С���к������
const LONG PFX_EC_LINE_NUMBER_MIN_WIDTH = 20;
// �к���������ͼ��߿��ļ��
const LONG PFX_EC_LINE_NUMBER_INDENT_LEFT = 5;
// �к�����������к����ұߵ�����
const LONG PFX_EC_LINE_NUMBER_INDENT_RIGHT = 0;
// ��С�ı�߸߶�
const LONG PFX_EC_RULER_MIN_HEIGHT = 16;
// ��߿̶��з��Ǹ���ֵ���������������̶�
const LONG PFX_EC_RULER_RESOLUTION = 10;
// ��߿̶��߿��
const LONG PFX_EC_RULER_SCALE_WIDTH = 1;
// ��ӡʱ���ĺͱ���ľ���
const LONG PFX_EC_DEF_SPACING_HEADER_AND_TEXT = 20;
// ��ӡʱ���ĺͽ�ע�ľ���
const LONG PFX_EC_DEF_SPACING_FOOTER_AND_TEXT = 10;
// �����ע���ĺ�ˮƽ�ָ��ߵľ���
const LONG PFX_EC_DEF_SPACING_HEADER_FOOTER_TEXT_AND_SEPARATOR = 1;
inline BOOL CPfxEditView::IsLeadByte( UINT c ) {
  return PFX_EC_IS_LEAD_BYTE( c );
}
inline BOOL CPfxEditView::IsWhitespace( WCHAR wc ) const {
  return ( wc == PFX_EC_CHR_SPACE ) || ( wc == PFX_EC_CHR_TAB );
}
inline BOOL CPfxEditView::IsDelayRecalcWrapping() const {
  return m_bDelayRecalcWrapping;
}
inline BOOL CPfxEditView::IsFirstSiblingView() const {
  ASSERT( m_pdb != NULL );
  return ( this == m_pdb->m_arrViews[ 0 ] );
}
inline BOOL CPfxEditView::IsWrapping() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsWrapping();
}
inline BOOL CPfxEditView::IsWrapAtWindowEdge() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsWrapAtWindowEdge();
}
inline BOOL CPfxEditView::IsWrapAfterColumns() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsWrapAfterColumns();
}
inline BOOL CPfxEditView::IsDrawRuler() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsDrawRuler();
}
inline BOOL CPfxEditView::IsDrawLineNumber() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsDrawLineNumber();
}
inline BOOL CPfxEditView::IsDrawWhitespace() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsDrawWhitespace();
}
inline BOOL CPfxEditView::IsDrawLineBreak() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsDrawLineBreak();
}
inline BOOL CPfxEditView::IsDrawHilighted() const {
  ASSERT( m_pti != NULL );
  return m_pti->IsDrawHilighted();
}
inline BOOL CPfxEditView::IsCanColumnSelect() const {
  return !IsWrapping();
}
inline BOOL CPfxEditView::IsCanColumnPaste() const {
  return ( IsCanPaste() && !IsWrapping() );
}
inline BOOL CPfxEditView::IsCanDelete() const {
  ASSERT( m_pdb != NULL );
  return !IsReadOnly() && ( m_pdb->GetDocSize() > 0 );
}
inline BOOL CPfxEditView::IsCanCopy() const {
  return IsTextSelected();
}
inline BOOL CPfxEditView::IsCanCut() const {
  return !IsReadOnly() && IsTextSelected();
}
inline BOOL CPfxEditView::IsCanPaste() const {
  return !IsReadOnly() && ::IsClipboardFormatAvailable( CF_TEXT );
}
inline BOOL CPfxEditView::IsCanUndo() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsCanUndo();
}
inline BOOL CPfxEditView::IsCanRedo() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsCanRedo();
}
inline BOOL CPfxEditView::IsColumnSelect() const {
  ASSERT( m_pdb != NULL );
  return ( m_nSelMode == PFX_EC_SM_COLUMN ) && ( m_pdb->GetDocSize() > 0 );
}
inline BOOL CPfxEditView::IsOverride() const {
  return m_bOverride;
}
inline BOOL CPfxEditView::IsReadOnly() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->IsReadOnly();
}
inline BOOL CPfxEditView::IsTracking() const {
  return m_bTrackingMouse;
}
inline BOOL CPfxEditView::IsColumnSelectTracking() const {
  return m_bTrackingColSelect;
}
inline BOOL CPfxEditView::IsDragging() const {
  return m_bDragging;
}
inline BOOL CPfxEditView::IsDragDropHandled() const {
  return m_bDragDropHandled;
}
inline BOOL CPfxEditView::IsTextSelected() const {
  return m_nSelLineFrom != PFX_EC_INVALID_LINE;
}
inline BOOL CPfxEditView::IsDragDropEnabled() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bEnableDragDrop;
}
inline BOOL CPfxEditView::IsExpandTabToSpaceChars() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bExpandTabToSpaceChars;
}
inline BOOL CPfxEditView::IsHomeKeyGotoFirstNonWhitespaceChar() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bHomeKeyGotoFirstNonWhitespaceChar;
}
inline BOOL CPfxEditView::IsSyncActiveLineBkColorWithTextBkColor() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bSyncActiveLineBkColorWithTextBkColor;
}
inline BOOL CPfxEditView::IsSyncTextBkColorWithSystem() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_bSyncTextBkColorWithSystem;
}
inline LONG CPfxEditView::GetCaretReferPoint() const {
  return m_ptCaretRefer.x;
}
inline LONG CPfxEditView::GetFirstVisibleLine() const {
  return m_ptFirstVisible.y;
}
inline LONG CPfxEditView::GetLastVisibleLine() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->TextVerifyLine( m_ptFirstVisible.y + m_sizeVisible.cy - 1 );
}
inline LONG CPfxEditView::GetUndoName() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetUndoName();
}
inline LONG CPfxEditView::GetRedoName() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetRedoName();
}
inline LONG CPfxEditView::GetDocSize() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetDocSize();
}
inline LONG CPfxEditView::GetTabSize() const {
  ASSERT( m_pti != NULL );
  return m_pti->m_nTabSize;
}
inline LONG CPfxEditView::GetLineCount() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineCount();
}
inline LONG CPfxEditView::GetLineUpperBound() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineUpperBound();
}
inline LONG CPfxEditView::GetLineLength( LONG nLine ) const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineLength( nLine );
}
inline LONG CPfxEditView::GetLineData( LONG nLine, LPTSTR pBuffer, LONG nBufMax ) const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineData( nLine, pBuffer, nBufMax );
}
inline LONG CPfxEditView::GetWrapStyle() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetWrapStyle();
}
inline LONG CPfxEditView::GetWrapWidth() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetWrapWidth();
}
inline LONG CPfxEditView::GetLineBreakSize() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineBreakSize();
}
inline LONG CPfxEditView::GetLineBreakType() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetLineBreakType();
}
inline HFONT CPfxEditView::GetFont() {
  return PFX_EC_DATA.m_hFontText;
}
inline LONG CPfxEditView::GetLineHeight() {
  return PFX_EC_DATA.m_nLineHeight;
}
inline LONG CPfxEditView::GetSpacingLines() {
  return PFX_EC_DATA.m_nSpacingLines;
}
inline LONG CPfxEditView::GetSpacingLineNumberAndText() {
  return PFX_EC_DATA.m_nSpacingLineNumberAndText;
}
inline LONG CPfxEditView::GetLineTextVertOffset() const {
  return GetSpacingLines() / 2;
}
inline void CPfxEditView::SetColumnSelect() {
  SetColumnSelectTracking( true );
}
inline void CPfxEditView::SetColumnSelectTracking( BOOL bOn ) {
  m_bTrackingColSelect = bOn;
}
inline void CPfxEditView::SetDragging( BOOL bOn ) {
  m_bDragging = bOn;
}
inline void CPfxEditView::SetDragDropHandled( BOOL bOn ) {
  m_bDragDropHandled = bOn;
}
inline void CPfxEditView::SetDelayRecalcWrapping( BOOL bOn ) {
  m_bDelayRecalcWrapping = bOn;
}
inline void CPfxEditView::SetOverride( BOOL bOn ) {
  m_bOverride = bOn;
}
inline void CPfxEditView::SetReadOnly( BOOL bOn ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetReadOnly( bOn );
}
inline void CPfxEditView::SetModifiedFlag( BOOL bChanged ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetModifiedFlag( bChanged );
}
inline void CPfxEditView::SetLineBreakType( LONG nType ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetLineBreakType( nType );
}
inline void CPfxEditView::SetLockBufferCompact( BOOL bOn ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetLockBufferCompact( bOn );
}
inline void CPfxEditView::SetLockWordWrapEvent( BOOL bOn ) {
  ASSERT( m_pdb != NULL );
  m_pdb->SetLockWordWrapEvent( bOn );
}
inline void CPfxEditView::SetUndoGroup( BOOL bBegin, WORD fUndoName ) {
  ASSERT( m_pdb != NULL );
  m_pdb->UndoSetGroup( bBegin, fUndoName );
}
inline LONG CPfxEditView::BookmarkGetCount() const {
  ASSERT( m_pdb != NULL );
  return m_pdb->GetBookmarkCount();
}
inline BOOL CPfxEditView::ParserIsEscape( WCHAR wc ) const {
  ASSERT( m_pti != NULL );
  return m_pti->IsEscape( wc );
}
inline BOOL CPfxEditView::ParserIsKeyword( LPCTSTR pData, LONG cch ) const {
  ASSERT( m_pti != NULL );
  return ( m_pti->m_mapKeywords.Lookup( pData, cch ) != -1 );
}
// CPfxEditData...
CPfxEditData PFX_EC_DATA;
CPfxEditData::CPfxEditData() {
  // ��ʼ������
  m_nCharHeight = 0;
  m_nCharWidthMax = 0;
  m_nCharWidthAve = 0;
  m_nCharWidthMaxNum = 0;
  m_nCharWidthTab = 0;
  m_nCharWidthSpace = 0;
  m_nCharWidthLineBreak = 0;
  m_nRulerCharHeight = 0;
  m_nRulerCharWidthAve = 0;
  m_nRulerHeight = 0;
  m_nSpacingLines = 0;
  m_nSpacingLineNumberAndText = PFX_EC_MIN_TEXT_INDENT_LEFT;
  m_sizeCaret.cx = 0;
  m_sizeCaret.cy = 0;
  m_hFontText = NULL;
  m_hCurStandard = ::LoadCursor( NULL, IDC_ARROW );
  m_hCurText = ::LoadCursor( NULL, IDC_IBEAM );
  m_hCurColSelect = ::LoadCursor( NULL, IDC_CROSS );
  m_hDataDragDropCopied = NULL;
  m_hDataClipboardCopied = NULL;
  m_nDataClipboardCopied = 0;
  ASSERT( PFX_EC_SIZEOF( m_arrLeadByte ) >= UCHAR_MAX );
#ifdef _UNICODE
  for ( UCHAR c = 0; c < UCHAR_MAX; c++ ) {
    m_arrLeadByte[ c ] = false;
  }
#else
  // BLOCK����¼ϵͳ���ô���ҳ�еġ�LeadByte���ַ�
  // ��Ҫ��CRT��_isleadbyte()��Ϊ��ʱCRT��û�г��Ի�
  for ( UCHAR c = 0; c < UCHAR_MAX; c++ ) {
    m_arrLeadByte[ c ] = ::IsDBCSLeadByte( c );
  }
#endif
}
CPfxEditData::~CPfxEditData() {
  // �ͷŹ����Դ
  HCURSOR arrHCursor[ 3 ];
  arrHCursor[ 0 ] = m_hCurStandard;
  arrHCursor[ 1 ] = m_hCurText;
  arrHCursor[ 2 ] = m_hCurColSelect;
  for ( LONG i = 0; i < PFX_EC_SIZEOF( arrHCursor ); i++ ) {
    if ( arrHCursor[ i ] != NULL ) {
      ::DestroyCursor( arrHCursor[ i ] );
    }
  }
}
// CPfxEditAlloc...
CPfxEditAlloc::CAllocator::CAllocator( size_t nUnitSize, size_t nMaxUnits ) {
  ASSERT( nUnitSize >= sizeof( CNode ) );
  ASSERT( nMaxUnits > 1 );
  m_nUnitSize = nUnitSize;
  m_nMaxUnits = nMaxUnits;
  m_nBlockSize = m_nUnitSize * m_nMaxUnits + sizeof( CPlex );
  m_nBlocks = 0;
  m_pBlocks = NULL;
}
void* CPfxEditAlloc::CAllocator::Alloc() {
  CNode * pNode;
  CPlex* pBlock = m_pBlocks;
  while ( pBlock != NULL ) {
    if ( pBlock->pNodeFree != NULL ) {
      pNode = pBlock->pNodeFree;
      pBlock->pNodeFree = pBlock->pNodeFree->pNext;
      pBlock->nUsed++;
      ASSERT( ( pBlock->nUsed >= 0 ) && ( pBlock->nUsed <= m_nMaxUnits ) );
      return ( void* ) pNode;
    }
    pBlock = pBlock->pNext;
  }
  pNode = NULL;
  pBlock = ( CPlex* ) new BYTE[ m_nBlockSize ];
  if ( pBlock != NULL ) {
    pBlock->pNext = m_pBlocks;
    pBlock->pPrev = NULL;
    pBlock->nUsed = 1;
    if ( m_pBlocks != NULL ) {
      m_pBlocks->pPrev = pBlock;
    }
    m_pBlocks = pBlock;
    m_nBlocks++;
    pNode = ( CNode* ) pBlock->data();
    ( BYTE*& ) pNode += ( m_nUnitSize * m_nMaxUnits ) - m_nUnitSize;
    CNode* pNodeFree = NULL;
    for ( int i = m_nMaxUnits - 1; i >= 0; i--, ( BYTE*& ) pNode -= m_nUnitSize ) {
      pNode->pNext = pNodeFree;
      pNodeFree = pNode;
    }
    pNode = pNodeFree;
    pBlock->pNodeFree = pNodeFree->pNext;
  }
  return ( void* ) pNode;
}
void CPfxEditAlloc::CAllocator::Free( void* lp ) {
#ifdef _DEBUG
  BOOL bHit = false;
#endif
  CPlex* pBlock = m_pBlocks;
  size_t cbBlock = m_nBlockSize;
  while ( pBlock != NULL ) {
    // ����ı�������̫��ʱ�䣬��Ӧ�ÿ���m_nBlocks����Ŀ��ʹ��
    // ��Ҫ����һ�㼸���ʮ����OK��
    if ( ( ( BYTE* ) lp <= ( BYTE* ) pBlock )
         || ( ( BYTE* ) lp >= ( BYTE* ) ( ( BYTE* ) pBlock + cbBlock ) ) ) {
      pBlock = pBlock->pNext;
      continue;
    }
#ifdef _DEBUG
    bHit = true;
#endif
    pBlock->nUsed--;
    ASSERT( ( pBlock->nUsed >= 0 ) && ( pBlock->nUsed <= m_nMaxUnits ) );
    // ��һ���Ѿ�û����ʹ�þͿ����ͷŵ�
    if ( pBlock->nUsed > 0 ) {
      ASSERT( m_nBlocks > 0 );
      ASSERT( ( ( BYTE* ) lp >= ( ( BYTE* ) pBlock + sizeof( CPlex ) ) ) );
      CNode* pNode = ( CNode* ) lp;
      pNode->pNext = pBlock->pNodeFree;
      pBlock->pNodeFree = pNode;
    } else {
      CPlex* pNext = pBlock->pNext;
      CPlex* pPrev = pBlock->pPrev;
      if ( pBlock == m_pBlocks ) {
        m_pBlocks = pNext;
        if ( pNext != NULL ) {
          pNext->pPrev = NULL;
        }
      } else {
        pPrev->pNext = pNext;
        if ( pNext != NULL ) {
          pNext->pPrev = pPrev;
        }
      }
      m_nBlocks--;
      delete[] ( BYTE* ) pBlock;
    }
    break;
  }
#ifdef _DEBUG
  ASSERT( bHit );
#endif
}
void CPfxEditAlloc::CAllocator::FreeAll() {
  CPlex * pBlock = m_pBlocks;
  while ( pBlock != NULL ) {
    BYTE * bytes = ( BYTE* ) pBlock;
    pBlock = pBlock->pNext;
    delete[] bytes;
  }
  m_nBlocks = 0;
  m_pBlocks = NULL;
}
const int PFX_EC_ALLOC_SIZE_1 = ( 8 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_2 = ( 16 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_3 = ( 24 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_4 = ( 32 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_5 = ( 64 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_6 = ( 128 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_7 = ( 256 * sizeof( TCHAR ) );
const int PFX_EC_ALLOC_SIZE_8 = ( 512 * sizeof( TCHAR ) );
const int PFX_EC_BLOCK_SIZE_1 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_1 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_2 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_2 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_3 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_3 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_4 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_4 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_5 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_5 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_6 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_6 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_7 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_7 + sizeof( int ) ) );
const int PFX_EC_BLOCK_SIZE_8 = ( PFX_EC_ROUND4( PFX_EC_ALLOC_SIZE_8 + sizeof( int ) ) );
CPfxEditAlloc::CPfxEditAlloc() : m_b1( PFX_EC_BLOCK_SIZE_1, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_1 ) ), m_b2( PFX_EC_BLOCK_SIZE_2, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_2 ) ), m_b3( PFX_EC_BLOCK_SIZE_3, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_3 ) ), m_b4( PFX_EC_BLOCK_SIZE_4, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_4 ) ), m_b5( PFX_EC_BLOCK_SIZE_5, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_5 ) ), m_b6( PFX_EC_BLOCK_SIZE_6, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_6 ) ), m_b7( PFX_EC_BLOCK_SIZE_7, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_7 ) ), m_b8( PFX_EC_BLOCK_SIZE_8, ( ( 4 * 1024 ) / PFX_EC_BLOCK_SIZE_8 ) ) {}
CPfxEditAlloc::~CPfxEditAlloc() {}
void* CPfxEditAlloc::Alloc( size_t cb ) {
  int * pData;
  if ( cb <= PFX_EC_ALLOC_SIZE_1 ) {
    pData = ( int* ) m_b1.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_1;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_2 ) {
    pData = ( int* ) m_b2.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_2;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_3 ) {
    pData = ( int* ) m_b3.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_3;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_4 ) {
    pData = ( int* ) m_b4.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_4;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_5 ) {
    pData = ( int* ) m_b5.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_5;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_6 ) {
    pData = ( int* ) m_b6.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_6;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_7 ) {
    pData = ( int* ) m_b7.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_7;
    pData++;
    return ( void* ) pData;
  }
  if ( cb <= PFX_EC_ALLOC_SIZE_6 ) {
    pData = ( int* ) m_b6.Alloc();
    *pData = PFX_EC_ALLOC_SIZE_6;
    pData++;
    return ( void* ) pData;
  }
  return new BYTE[ cb ];
}
void CPfxEditAlloc::Free( void* lp ) {
  if ( lp == NULL ) {
    return ;
  }
  int* pData = ( ( int* ) lp - 1 );
  switch ( *pData ) {
  default:
    delete lp;
    break;
  case PFX_EC_ALLOC_SIZE_1:
    m_b1.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_2:
    m_b2.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_3:
    m_b3.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_4:
    m_b4.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_5:
    m_b5.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_6:
    m_b6.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_7:
    m_b7.Free( pData );
    break;
  case PFX_EC_ALLOC_SIZE_8:
    m_b8.Free( pData );
    break;
  }
}
void CPfxEditAlloc::FreeAll() {
  m_b1.FreeAll();
  m_b2.FreeAll();
  m_b3.FreeAll();
  m_b4.FreeAll();
  m_b5.FreeAll();
  m_b6.FreeAll();
}
// CPfxEditHeap...
void* CPfxEditHeap::CAllocator::Alloc( size_t cb ) {
  CNode * pNode = NULL;
  for ( CNode * pNodeT = m_pNodes; pNodeT != NULL; pNodeT = pNodeT->pNext ) {
    if ( ( pNodeT->nRef == 0 ) && ( pNodeT->nSize >= cb ) ) {
      pNode = pNodeT;
      break;
    }
  }
  if ( pNode == NULL ) {
    size_t nAllocSize = PFX_EC_ROUND( cb, PFX_EC_TASKMEM_ALLOC_SIZE );
    size_t nBlockSize = sizeof( CNode ) + nAllocSize;
    pNode = reinterpret_cast<CNode*>( new BYTE[ nBlockSize ] );
    pNode->nRef = 0;
    pNode->nSize = nAllocSize;
    pNode->pPrev = NULL;
    pNode->pNext = m_pNodes;
    if ( m_pNodes != NULL ) {
      m_pNodes->pPrev = pNode;
    }
    m_nNodes++;
    m_pNodes = pNode;
  }
  pNode->nRef = 1;
  return pNode->data();
}
void CPfxEditHeap::CAllocator::Free( void* lp ) {
  if ( lp == NULL ) {
    return ;
  }
  TRACE( "CPfxEditHeap::CAllocator::Free(): Current blocks are: %d\n", m_nNodes );
  CNode* pNode = reinterpret_cast<CNode*>( reinterpret_cast<CNode*>( lp ) - 1 );
  if ( ( m_nNodes <= 3 ) && ( pNode->nSize <= PFX_EC_TASKMEM_CONST_SIZE ) ) {
    pNode->nRef = 0;
    return ;
  }
  CNode* pPrev = pNode->pPrev;
  CNode* pNext = pNode->pNext;
  if ( pNode == m_pNodes ) {
    m_pNodes = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = NULL;
    }
  } else {
    pPrev->pNext = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = pPrev;
    }
  }
  m_nNodes--;
  delete pNode;
}
void CPfxEditHeap::CAllocator::FreeAll() {
  for ( CNode * pNodeH = m_pNodes; pNodeH != NULL; ) {
    CNode* pNodeT = pNodeH;
    pNodeH = pNodeH->pNext;
    delete pNodeT;
  }
  m_nNodes = 0;
  m_pNodes = NULL;
}
void CPfxEditHeap::Free() {
  if ( m_pBuffer != NULL ) {
    PFX_EC_DATA.m_heap.Free( m_pBuffer );
    m_pBuffer = NULL;
  }
}
void* CPfxEditHeap::Alloc( size_t cb ) {
  Free();
  m_pBuffer = PFX_EC_DATA.m_heap.Alloc( cb );
  m_nBufferSize = m_nDataSize = ( ( m_pBuffer != NULL ) ? cb : 0 );
  return m_pBuffer;
}
void* CPfxEditHeap::ReAlloc( size_t cb ) {
  if ( m_pBuffer == NULL ) {
    return Alloc( cb );
  }
  if ( cb <= m_nBufferSize ) {
    m_nDataSize = cb;
    return m_pBuffer;
  }
  void* pBuffer = PFX_EC_DATA.m_heap.Alloc( cb );
  if ( pBuffer != NULL ) {
    memcpy( pBuffer, m_pBuffer, m_nDataSize );
    PFX_EC_DATA.m_heap.Free( m_pBuffer );
    m_pBuffer = pBuffer;
    m_nBufferSize = m_nDataSize = cb;
  }
  return pBuffer;
}
// CPfxEditHash...
CPfxEditHash::CPfxEditHash() {
  memset( m_arrHashTable, 0, sizeof( m_arrHashTable ) );
  m_nCount = 0;
  m_bMatchCase = true;
}
CPfxEditHash::~CPfxEditHash() {
  RemoveAll();
}
void CPfxEditHash::RemoveAll() {
  if ( GetCount() <= 0 ) {
    return ;
  }
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrHashTable ); i++ ) {
    CAssoc* pAssoc = m_arrHashTable[ i ];
    while ( pAssoc != NULL ) {
      CAssoc * pNext = pAssoc->pNext;
      PFX_EC_DATA.MemFree( pAssoc );
      pAssoc = pNext;
    }
  }
  m_nCount = 0;
  memset( m_arrHashTable, 0, sizeof( m_arrHashTable ) );
}
void CPfxEditHash::RemoveAt( LPCTSTR pszKey ) {
  CAssocInfo i;
  if ( GetAssocAt( pszKey, -1, i ) ) {
    m_arrHashTable[ i.nHash ] = i.pAssoc->pNext;
    PFX_EC_DATA.MemFree( i.pAssoc );
  }
}
void CPfxEditHash::SetAt( LONG nKeyClass, LPCTSTR pszKey ) {
  if ( ( pszKey == NULL ) || ( *pszKey == '\0' ) ) {
    return ;
  }
  CAssocInfo i;
  if ( GetAssocAt( pszKey, -1, i ) )           //      ��������ƥ����
  {
    i.pAssoc->nKeyClass = nKeyClass;
  } else // û��ƥ�������������
  {
    // sizeof(CAssoc)���Ѿ�����������NULL�ַ��Ŀռ�
    CAssoc* pAssoc = static_cast<CAssoc*>( PFX_EC_DATA.MemAlloc( sizeof( CAssoc ) + ( i.cchKey * sizeof( TCHAR ) ) ) );
    ASSERT( pAssoc != NULL );
    ASSERT( AfxIsValidAddress( pAssoc, sizeof( CAssoc ), true ) );
    memcpy( pAssoc->szKey, i.pszKey, ( i.cchKey * sizeof( TCHAR ) ) );
    pAssoc->cchKey = i.cchKey;
    pAssoc->szKey[ i.cchKey ] = '\0';
    pAssoc->nKeyClass = nKeyClass;
    pAssoc->pNext = m_arrHashTable[ i.nHash ];
    m_arrHashTable[ i.nHash ] = pAssoc;
    m_nCount++;
  }
}
// LPCTSTR pszKey     - �ؼ��֣����ַ���һ��ҪNULL��β����ʱ��cchָ������
// LONG cch           - �������-1��ٶ��ַ���ΪNULL��β�������Զ�����
// CAssocInfo& retVal - ����m_arrHashTable��ɢ��ֵ��CAssoc*ָ��
// ��������ֵ��ƥ�䷵��TRUE���򷵻�FALSE
BOOL CPfxEditHash::GetAssocAt( LPCTSTR pszKey, LONG cch, CAssocInfo& retVal ) {
  if ( pszKey == NULL ) {
    return false;
  }
  // ���ܴ�������Ĺؼ���
  if ( cch == -1 ) {
    cch = _tcslen( pszKey );
  }
  LONG cchKey = min( cch, PFX_EC_KEYWORD_MAP_MAX_KEYWORD_LEN );
  // ���Դ�Сд�Ƚ���Ҫ��ת����Сд������һ����������NULL��β
  if ( !m_bMatchCase ) {
    memcpy( m_szKeyLower, pszKey, cchKey * sizeof( TCHAR ) );
    CharLowerBuff( m_szKeyLower, cchKey );
    m_szKeyLower[ cchKey ] = '\0';
    pszKey = m_szKeyLower;
  }
  retVal.pszKey = pszKey;
  retVal.cchKey = cchKey;
  retVal.nHash = HashKey( pszKey, cchKey );
  for ( CAssoc * pAssoc = m_arrHashTable[ retVal.nHash ]; pAssoc != NULL; pAssoc = pAssoc->pNext ) {
    // ���Ȳ���ȾͲ���Ҫ�����Ƚ�
    if ( cch != pAssoc->cchKey ) {
      continue;
    }
    // �ȽϹؼ���
    LPCTSTR s1 = pAssoc->szKey;
    LPCTSTR s2 = pszKey;
    while ( *s1 && ( *s1 == *s2 ) ) {
      s1++;
      s2++;
    }
    if ( *s1 == '\0' ) {
      retVal.pAssoc = pAssoc;
      return true;
    }
  }
  return false;
}
// CKeyClassArray
CKeyClassArray::CKeyClassArray() {
  // ����Ԥ������Ϊ��Ч�����ⱻ�����Զ�����ռ�õط�
  ASSERT( kcPredefinedKeyClassNumber < PFX_EC_SIZEOF( m_arrClasses ) );
  for ( int i = 0; i < kcPredefinedKeyClassNumber; i++ ) {
    m_arrClasses[ i ].m_bEmpty = false;
  }
}
CKeyClassArray::~CKeyClassArray() {
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrClasses ); i++ ) {
    RemoveClass( i );
  }
}
CKeyClassArray&
CKeyClassArray::operator=( const CKeyClassArray& src ) {
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrClasses ); i++ ) {
    m_arrClasses[ i ] = src.m_arrClasses[ i ];
  }
  return *this;
}
// ��ȡ�ؼ��ַ�����Ϣ
POSITION CKeyClassArray::GetClassFirst() const {
  return ( !m_arrClasses[ 0 ].IsEmpty() ? ( POSITION ) - 1 : NULL );
}
void CKeyClassArray::GetClassNext( POSITION& rPosition, CPfxEditKeyClass& theClass ) const {
  int i = ( int ) rPosition;
  if ( i == -1 )           //      Ҫ���һ����Ԫ
  {
    i = 0;
  }
  if ( !IsValidClass( i ) ) {
    theClass.SetEmpty();
  } else {
    theClass = m_arrClasses[ i ];
  }
  i++;
  if ( !IsValidClass( i ) ) {
    i = NULL;
  }
  rPosition = ( POSITION ) i;
}
LPCTSTR CKeyClassArray::GetClassName( LONG nIndex ) const {
  return ( IsValidClass( nIndex ) ? ( LPCTSTR ) ( m_arrClasses[ nIndex ].m_strClassName ) : NULL );
}
void CKeyClassArray::SetClassName( LONG nIndex, LPCTSTR pszClassName ) {
  if ( IsValidClass( nIndex ) ) {
    m_arrClasses[ nIndex ].m_strClassName = ( pszClassName != NULL ) ? pszClassName : _T( "" );
  }
}
// ��ӹؼ���-��ɫ����ԣ������ѱ����������
LONG CKeyClassArray::SetClass( const CPfxEditKeyClass& theClass ) {
  for ( LONG i = 0; i < PFX_EC_SIZEOF( m_arrClasses ); i++ ) {
    CPfxEditKeyClass& item = m_arrClasses[ i ];
    // ��������յĵ�Ԫ������ǰ��û����theClass��ͬ����
    if ( item.IsEmpty() ) {
      item = theClass;
      item.m_bEmpty = false;
      break;
    }
    // ���������ͬ�������������ִ�Сд��
    if ( item.IsClass( theClass.m_strClassName ) ) {
      item.m_color = theClass.m_color;
      item.m_bEmpty = false;
      break;
    }
  }
  return ( ( i < PFX_EC_SIZEOF( m_arrClasses ) ) ? i : -1 );
}
void CKeyClassArray::RemoveClass( LONG nIndex ) {
  if ( IsValidClass( nIndex ) ) {
    m_arrClasses[ nIndex ].SetEmpty();
  }
}
void CKeyClassArray::RemoveClass( LPCTSTR pszClassName ) {
  if ( pszClassName == NULL ) {
    return ;
  }
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrClasses ); i++ ) {
    const CPfxEditKeyClass& item = m_arrClasses[ i ];
    if ( item.IsClass( pszClassName ) ) {
      RemoveClass( i );
      break;
    }
  }
}
// CMarkerPairArr
CMarkerPairArr::CMarkerPairArr() {
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrMarkers ); i++ ) {
    m_arrMarkers[ i ] = NULL;
  }
}
CMarkerPairArr::~CMarkerPairArr() {
  RemoveAll();
}
void CMarkerPairArr::RemoveAll() {
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrMarkers ); i++ ) {
    FreeMarkerChain( m_arrMarkers[ i ] );
    m_arrMarkers[ i ] = NULL;
  }
}
void CMarkerPairArr::FreeMarkerChain( CPfxEditMarkerPair* pMarker ) {
  while ( pMarker != NULL ) {
    CPfxEditMarkerPair * pNext = pMarker->pNext;
    PFX_EC_DATA.MemFree( pMarker );
    pMarker = pNext;
  }
}
CPfxEditMarkerPair* CMarkerPairArr::CopyMarkerChain( CPfxEditMarkerPair* pMarker ) {
  CPfxEditMarkerPair * pReturn = NULL;
  if ( pMarker != NULL ) {
    pReturn = CreateMarker( pMarker->pszOn, pMarker->pszOff );
    pMarker = pMarker->pNext;
    CPfxEditMarkerPair* pPrev = pReturn;
    while ( pMarker != NULL ) {
      CPfxEditMarkerPair * pNext = CreateMarker( pMarker->pszOn, pMarker->pszOff );
      if ( pNext == NULL ) {
        break;
      }
      pPrev->pNext = pNext;
      pNext->pPrev = pPrev;
      pPrev = pNext;
      pMarker = pMarker->pNext;
    }
  }
  return pReturn;
}
CPfxEditMarkerPair* CMarkerPairArr::CreateMarker( LPCTSTR pszOn, LPCTSTR pszOff ) {
  LONG cchOn = ( ( pszOn != NULL ) ? _tcslen( pszOn ) : 0 );
  LONG cchOff = ( ( pszOff != NULL ) ? _tcslen( pszOff ) : 0 );
  if ( ( cchOn == 0 ) && ( cchOff == 0 ) ) {
    return NULL;
  }
  CPfxEditMarkerPair* pMarker = static_cast<CPfxEditMarkerPair*>( PFX_EC_DATA.MemAlloc( sizeof( CPfxEditMarkerPair ) + ( ( cchOn + cchOff ) * sizeof( TCHAR ) ) ) );
  if ( pMarker == NULL ) {
    return NULL;
  }
  pMarker->pNext = NULL;
  pMarker->pPrev = NULL;
  if ( cchOn <= 0 ) {
    pMarker->pszOn = NULL;
    pMarker->cchOn = 0;
    pMarker->wLowerOn = 0;
    pMarker->wUpperOn = 0;
  } else {
    LPTSTR lp = &pMarker->szKey[ 0 ];
    _tcscpy( lp, pszOn );
    WCHAR wc = ( !CPfxEditView::IsLeadByte( *lp ) ? ( ( WCHAR ) * lp ) : ( *( ( LPWSTR ) lp ) ) );
    pMarker->pszOn = lp;
    pMarker->cchOn = cchOn;
    pMarker->wLowerOn = wc;
    pMarker->wUpperOn = wc;
    CharLowerBuff( ( LPTSTR ) & pMarker->wLowerOn, sizeof( pMarker->wLowerOn ) );
    CharUpperBuff( ( LPTSTR ) & pMarker->wUpperOn, sizeof( pMarker->wUpperOn ) );
  }
  if ( cchOff <= 0 ) {
    pMarker->pszOff = NULL;
    pMarker->cchOff = 0;
    pMarker->wLowerOff = 0;
    pMarker->wUpperOff = 0;
  } else {
    LPTSTR lp = &pMarker->szKey[ cchOn + 1 ];
    _tcscpy( lp, pszOff );
    WCHAR wc = ( !CPfxEditView::IsLeadByte( *lp ) ? ( ( WCHAR ) * lp ) : ( *( ( LPWSTR ) lp ) ) );
    pMarker->pszOff = lp;
    pMarker->cchOff = cchOff;
    pMarker->wLowerOff = wc;
    pMarker->wUpperOff = wc;
    CharLowerBuff( ( LPTSTR ) & pMarker->wLowerOff, sizeof( pMarker->wLowerOff ) );
    CharUpperBuff( ( LPTSTR ) & pMarker->wUpperOff, sizeof( pMarker->wUpperOff ) );
  }
  return pMarker;
}
CMarkerPairArr&
CMarkerPairArr::operator=( const CMarkerPairArr& src ) {
  RemoveAll();
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrMarkers ); i++ ) {
    m_arrMarkers[ i ] = CopyMarkerChain( src.m_arrMarkers[ i ] );
  }
  return *this;
}
// CPfxEditTypeInfo...
CPfxEditTypeInfo::CPfxEditTypeInfo() {
  m_bDrawHilighted = false;
  m_bDrawRuler = false;
  m_bDrawLineNumber = true;
  m_bDrawLineBreak = true;
  m_bDrawWhitespace = true;
  m_bEnableDragDrop = true;
  m_bExpandTabToSpaceChars = false;
  m_bSyncTextBkColorWithSystem = true;
  m_bHomeKeyGotoFirstNonWhitespaceChar = true;
  m_bSyncActiveLineBkColorWithTextBkColor = false;
  m_nTabSize = 8;
  m_nIndentSize = 8;
  m_nWrapStyle = PFX_EC_WRAP_NONE;
  m_nWrapWidth = 0;
  m_wEscape = 0; // ת���ַ�
  SetMatchCase( true );
  // ����ȱʡ�ļ����ͣ�*.txt; *.*����ȱʡ����
  SetDelimiters( NULL );
  // Ԥ������ɫ
  COLORREF clrDefault = PFX_EC_CLR_TEXT;
  SetKeyClassColor( kcText, clrDefault );
  SetKeyClassColor( kcTextSelected, PFX_EC_CLR_TEXT_SELECTED );
  SetKeyClassColor( kcRuler, PFX_EC_CLR_RULER );
  SetKeyClassColor( kcLineNumber, PFX_EC_CLR_LINE_NUMBER );
  SetKeyClassColor( kcWhitespace, PFX_EC_CLR_WHITESPACE );
  SetKeyClassColor( kcBkText, PFX_EC_CLR_BK_TEXT );
  SetKeyClassColor( kcBkTextSelected, PFX_EC_CLR_BK_TEXT_SELECTED );
  SetKeyClassColor( kcBkRuler, PFX_EC_CLR_BK_RULER );
  SetKeyClassColor( kcBkLineNumber, PFX_EC_CLR_BK_LINE_NUMBER );
  SetKeyClassColor( kcBkActiveLine, PFX_EC_CLR_BK_ACTIVE_LINE );
  SetKeyClassColor( kcBkBookmark, PFX_EC_CLR_BK_BOOKMARK );
  SetKeyClassColor( kcHeaderAndFooter, clrDefault );
  SetKeyClassColor( kcQuotedString1, clrDefault );
  SetKeyClassColor( kcQuotedString2, clrDefault );
  SetKeyClassColor( kcLineComment1, clrDefault );
  SetKeyClassColor( kcLineComment2, clrDefault );
  SetKeyClassColor( kcBlockComment1, clrDefault );
  SetKeyClassColor( kcBlockComment2, clrDefault );
}
CPfxEditTypeInfo::~CPfxEditTypeInfo() {}
CString CPfxEditTypeInfo::GetDelimiters() const {
#ifdef _UNICODE
  return m_arrDelimiters;
#else
  // �ڷ�_UNICODEģʽ��ANSI�ַ�����չ��16λ��ţ�����Ҫ��16λ��ANSI��
  // ����ԭ��8λ
  // ����ÿ�����ʶ����ת����MBS�ַ�ʱ����ҪPFX_EC_MAX_MB_LEN���ֽ�
  char sz[ PFX_EC_SIZEOF( m_arrDelimiters ) * PFX_EC_MAX_MB_LEN ];
  memset( sz, 0, sizeof( sz ) );
  // ͳ�ƹ��е��ʶ������
  LPSTR p = sz;
  for ( int i = 0; i < PFX_EC_SIZEOF( m_arrDelimiters ); i++ ) {
    WCHAR wc = m_arrDelimiters[ i ];
    if ( wc == '\0' ) {
      break;
    }
    // �������ANSI�ַ���wc�ĸ߰�λ����
    if ( ( wc & 0xFF00 ) == 0 ) {
      *p = ( char ) wc;
      p++;
    }
    // ��������MBCS�ַ�
    else {
      *( LPWSTR ) p = wc;
      p += sizeof( wc );
    }
  }
  return sz;
#endif
}
void CPfxEditTypeInfo::SetDelimiters( LPCTSTR pszDelimiters ) {
  memset( m_arrDelimiters, 0, sizeof( m_arrDelimiters ) );
  if ( pszDelimiters == NULL ) {
    pszDelimiters = GetDelimitersDefault();
  }
#ifdef _UNICODE
  for ( LONG i = 0; i < ( PFX_EC_SIZEOF( m_arrDelimiters ) - 1 ); i++ ) {
    WCHAR wc = *pszDelimiters++;
    if ( wc == '\0' ) {
      break;
    }
    m_arrDelimiters[ i ] = wc;
  }
#else
  for ( LONG i = 0; i < ( PFX_EC_SIZEOF( m_arrDelimiters ) - 1 ); i++ ) {
    TCHAR tc = *pszDelimiters++;
    if ( tc == '\0' ) {
      break;
    }
    WCHAR wc = tc;
    if ( CPfxEditView::IsLeadByte( tc ) && ( *pszDelimiters != '\0' ) ) {
      WCHAR ch = *pszDelimiters++;
      wc |= ( ch << 0x08 );
    }
    m_arrDelimiters[ i ] = wc;
  }
#endif
}
void CPfxEditTypeInfo::SetEscape( LPCTSTR pszEscape ) {
  WCHAR wc = 0;
  if ( pszEscape != NULL ) {
    if ( !CPfxEditView::IsLeadByte( *pszEscape ) ) {
      wc = ( ( WCHAR ) * pszEscape );
    } else {
      wc = ( *( LPCWSTR ) pszEscape );
    }
  }
  m_wEscape = wc;
}
void CPfxEditTypeInfo::SetMarkerPair( EPfxEditMarkerPair id, LPCTSTR pszOn, LPCTSTR pszOff ) {
  if ( !m_arrMarkerPair.IsValidMarker( id ) ) {
    return ;
  }
  m_arrMarkerPair.FreeMarkerChain( m_arrMarkerPair.m_arrMarkers[ id ] );
  m_arrMarkerPair.m_arrMarkers[ id ] = m_arrMarkerPair.CreateMarker( pszOn, pszOff );
}
void CPfxEditTypeInfo::AppendMarkerPair( EPfxEditMarkerPair id, LPCTSTR pszOn, LPCTSTR pszOff ) {
  if ( !m_arrMarkerPair.IsValidMarker( id ) ) {
    return ;
  }
  CPfxEditMarkerPair* pMarkerNew = m_arrMarkerPair.CreateMarker( pszOn, pszOff );
  CPfxEditMarkerPair* pMarkerOld = m_arrMarkerPair.m_arrMarkers[ id ];
  if ( pMarkerOld != NULL ) {
    pMarkerOld->pPrev = pMarkerNew;
  }
  pMarkerNew->pNext = pMarkerOld;
  m_arrMarkerPair.m_arrMarkers[ id ] = pMarkerNew;
}
// CPfxEditPrintInfo...
// C4244: conversion from 'float' to 'long', possible loss of data
#pragma warning (disable: 4244)
CPfxEditPrintInfo::CPfxEditPrintInfo() {
  m_fRatioX = 0;
  m_fRatioY = 0;
}
CPfxEditPrintInfo::~CPfxEditPrintInfo() {}
void CPfxEditPrintInfo::PrinterLogicUnitFromDisplay( CRect& rect ) {
  ASSERT( m_fRatioX != 0 );
  ASSERT( m_fRatioY != 0 );
  rect.left *= m_fRatioX;
  rect.top *= m_fRatioY;
  rect.right *= m_fRatioX;
  rect.bottom *= m_fRatioY;
}
void CPfxEditPrintInfo::PrinterLogicUnitFromDisplay( CPoint& rpt ) {
  ASSERT( m_fRatioX != 0 );
  ASSERT( m_fRatioY != 0 );
  rpt.x *= m_fRatioX;
  rpt.y *= m_fRatioY;
}
BOOL CPfxEditPrintInfo::SetMapDisplayFont() {
  ASSERT( m_pFontDisplay != NULL );
  LOGFONT lf;
  GetLogFont( m_pFontDisplay, &lf );
  lf.lfHeight = PrinterLogicUnitFromDisplayY( lf.lfHeight );
  lf.lfWidth = 0;
  if ( m_fontText != NULL ) {
    m_fontText.DeleteObject();
  }
  return m_fontText.CreateFontIndirect( &lf ) != NULL;
}
BOOL CPfxEditPrintInfo::SetPrinterFont( HFONT pFont ) {
  UNUSED( pFont );
  return false;
}
void CPfxEditPrintInfo::SetDC( HDC hdcDisplay, HDC hdcPrinter ) {
  CDCH pdcDisplay = hdcDisplay;
  CDCH pdcPrinter = hdcPrinter;
  LONG nOldMapMode = pdcPrinter.SetMapMode( MM_TEXT );
  FLOAT fLogPixelsXDisplay = ( FLOAT ) pdcDisplay.GetDeviceCaps( LOGPIXELSX );
  FLOAT fLogPixelsYDisplay = ( FLOAT ) pdcDisplay.GetDeviceCaps( LOGPIXELSY );
  FLOAT fLogPixelsXPrinter = ( FLOAT ) pdcPrinter.GetDeviceCaps( LOGPIXELSX );
  FLOAT fLogPixelsYPrinter = ( FLOAT ) pdcPrinter.GetDeviceCaps( LOGPIXELSY );
  // ��ԭDCӳ��ģʽ
  pdcPrinter.SetMapMode( nOldMapMode );
  m_pdcPrinter = pdcPrinter;
  m_pdcDisplay = pdcDisplay;
  // ��ʾDC�е��߼���Ԫ�ʹ�ӡDC�е��߼���Ԫ����ϵ��
  m_fRatioX = fLogPixelsXPrinter / fLogPixelsXDisplay;
  m_fRatioY = fLogPixelsYPrinter / fLogPixelsYDisplay;
  // �����ӡDCÿˮƽ�ʹ�ֱӢ�����������߼���Ԫ��
  m_fLogUnitsPerInchX = fLogPixelsXPrinter;
  m_fLogUnitsPerInchY = fLogPixelsYPrinter;
  // �����ӡDCÿˮƽ�ʹ�ֱ�������������߼���Ԫ��,һӢ�����2.54����
  m_fLogUnitsPerCmX = m_fLogUnitsPerInchX / 2.54;
  m_fLogUnitsPerCmY = m_fLogUnitsPerInchX / 2.54;
}
// conversion from 'float' to 'long', possible loss of data
#pragma warning (default: 4244) 
// CPfxEditDoc
// ���ĳ�е�ѡ�񳤶ȵ��ڸ��г���+�ó�������������е�ѡ��������з�
const LONG PFX_EC_VIRTUAL_LINE_BREAK_SIZE = 1;
const LONG PFX_EC_LINE_LINEINFO_ARRAY_GROWBY = 2048;
const LONG PFX_EC_LINE_MEMBLOCK_ALLOC_LENGTH = 65520;
const LONG PFX_EC_LINE_MEMBLOCK_UNUSED_PERCENT = 10;
const LONG PFX_EC_UNDO_UNDOINFO_ARRAY_MAXLEN = ( ( 512 * 1024 ) / sizeof( CPfxEditUndoInfo ) );
const LONG PFX_EC_UNDO_UNDOINFO_ARRAY_GROWBY = ( PFX_EC_UNDO_UNDOINFO_ARRAY_MAXLEN / 8 );
const LONG PFX_EC_UNDO_MEMBLOCK_ALLOC_LENGTH = 32768;
const LONG PFX_EC_UNDO_MEMBLOCK_UNUSED_PERCENT = 10;
// UNICODE�ı��ļ���ʶ��һ���UNICODE�ı��ļ��Ŀ�ͷ����������ʶ
const WORD PFX_EC_UNICODE_FILE_SIGNATURE = 0xFEFF;
const WORD PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE = 0xFFFE;
#define PFX_EC_ULE_TO_UBE(c)  (MAKEWORD(HIBYTE(c), LOBYTE(c)))
#define PFX_EC_UBE_TO_ULE(c)  (MAKEWORD(HIBYTE(c), LOBYTE(c))) 
// CPfxEditDocFile
CPfxEditDocFile::CPfxEditDocFile() {
  m_hMapping = NULL;
  m_nMappingSize = 0;
  m_pMappingAddress = NULL;
}
CPfxEditDocFile::~CPfxEditDocFile() {
  Close();
}
BOOL CPfxEditDocFile::MapFile( HANDLE hFile, LONG nSize ) {
  ASSERT( m_hMapping == NULL );
  ASSERT( m_nMappingSize == 0 );
  ASSERT( m_pMappingAddress == NULL );
  ASSERT( nSize > 0 );
  if ( nSize <= 0 ) {
    return false;
  }
  DWORD dwProtect;
  DWORD dwMappingSize;
  DWORD dwDesiredAccess;
  if ( hFile != ( HANDLE ) 0xFFFFFFFF ) {
    dwProtect = PAGE_READONLY | SEC_COMMIT;
    dwMappingSize = nSize;
    dwDesiredAccess = FILE_MAP_READ;
  } else {
    dwProtect = PAGE_READWRITE | SEC_COMMIT;
    dwMappingSize = PFX_EC_ROUND( nSize, 4096 );
    dwDesiredAccess = FILE_MAP_WRITE;
  }
  m_hMapping = ::CreateFileMapping( hFile, NULL, dwProtect, 0, dwMappingSize, NULL );
  if ( m_hMapping == NULL ) {
    AfxThrowMemoryException();
    return false;
  }
  m_pMappingAddress = ::MapViewOfFile( m_hMapping, dwDesiredAccess, 0, 0, dwMappingSize );
  if ( m_pMappingAddress == NULL ) {
    AfxThrowMemoryException();
    return false;
  }
  m_nMappingSize = nSize;
  return true;
}
void CPfxEditDocFile::Close() {
  if ( m_pMappingAddress != NULL ) {
    ::UnmapViewOfFile( m_pMappingAddress );
    m_nMappingSize = 0;
    m_pMappingAddress = NULL;
  }
  if ( m_hMapping != NULL ) {
    ::CloseHandle( m_hMapping );
    m_hMapping = NULL;
  }
}
// �������Ļ������Ƿ�����ȷ��UTF8�������ݣ��ú���ֻȷ����UCS2ת����������
// ����ASCII�ַ�Ҳ�ǺϷ���UTF8�ַ������Ե�������ȫ�����ݶ���ASCII�ַ�ʱ����
// �ᷴ��TRUE�����ʹ������Ϊ�����ַ�����ASCII�ַ�������²�Ӧ����UTF8����ô
// ���Լ�������ȥ����Ƿ����е��ַ�����ASCII�������
BOOL CPfxEditDocUtf8Converter::IsUtf8FromUnicode( LPCSTR pUtf8, LONG cch ) const {
  if ( ( pUtf8 == NULL ) || ( cch <= 0 ) ) {
    return false;
  }
  LPCSTR pCur = pUtf8;
  LPCSTR pEnd = pUtf8 + cch;
  while ( pCur < pEnd ) {
    LONG nSize = Utf8CharSize( static_cast<BYTE>( *pCur ) );
    if ( ( nSize == 0 ) || ( ( pCur + nSize ) > pEnd ) ) {
      return false;
    }
    // ȷ����UTF8���ַ������Ժ�Ҫ��������ֽ��Ƿ�Ϸ�
    // �����ֽڵĺϷ���Χ
    // 10xx-xxxx
    // 1000-0000->1011-1111 = 0x80->0xBF
    LPCSTR pNextChar = pCur + nSize;
    while ( ++pCur < pNextChar ) {
      if ( ( ( ( BYTE ) * pCur ) < 0x80 ) || ( ( ( BYTE ) * pCur ) > 0xBF ) ) {
        return false;
      }
    }
  }
  return true;
}
// ��UTF8�ַ�ת��ΪUNICODE�ַ����߼�����Ҫ�������������
LONG CPfxEditDocUtf8Converter::Utf8ToUnicode( LPCSTR pUtf8, LONG cchUtf8, LPWSTR pUnicode, LONG cchUnicode ) const {
  if ( ( pUtf8 == NULL )
       || ( cchUtf8 <= 0 )
       || ( ( cchUnicode != 0 ) && ( pUnicode == NULL ) ) ) {
    return 0;
  }
  LPWSTR pUnicodeCur = pUnicode;
  LPWSTR pUnicodeEnd = pUnicode + cchUnicode;
  const BYTE* pUtf8Cur = reinterpret_cast<const BYTE*>( pUtf8 );
  const BYTE* pUtf8End = reinterpret_cast<const BYTE*>( pUtf8 ) + cchUtf8;
  LONG nSizeInUnicode = 0;
  // ���������Ҫ�������������
  if ( cchUnicode == 0 ) {
    while ( pUtf8Cur < pUtf8End ) {
      LONG nSize = Utf8CharSize( *pUtf8Cur );
      // ���������Ч�ַ���Utf8CharSize()�����㣬��ʱ��ǿ
      // �ȵ���һ���ַ�����
      pUtf8Cur += max( 1, nSize );
      // �������һ������UTF8�ַ����ͽ����ɢ
      if ( pUtf8Cur <= pUtf8End ) {
        nSizeInUnicode++;
      } else {
        nSizeInUnicode += ( max( 1, nSize ) );
        nSizeInUnicode -= ( pUtf8Cur - pUtf8End );
      }
    }
    return nSizeInUnicode;
  }
  // ��ʼִ��UTF8ת����UNICODE�Ĳ���
  while ( ( pUtf8Cur < pUtf8End ) && ( pUnicodeCur < pUnicodeEnd ) ) {
    BYTE b3;
    BYTE b2;
    BYTE b1 = *pUtf8Cur++;
    nSizeInUnicode++;
    switch ( Utf8CharSize( b1 ) ) {
    default:
    case 0:           //      ��Ч����һ����ASCII����
    case 1:           //      U+00000000-U+0000007F: 0xxxxxxx
      // x1111111 = 0x7F
      *pUnicodeCur++ = ( WORD ) b1;
      break;
    case 2:           //      U+00000080-U+000007FF: 110xxxxx 10xxxxxx
      // xxx11111 = 0x1F, xx111111 = 0x3F
      {
        b1 &= 0x1F;
        b2 = 0;
      }
      if ( pUtf8Cur < pUtf8End ) {
        b2 = *pUtf8Cur++;
        b2 &= 0x3F;
      }
      *pUnicodeCur++ = ( WORD ) ( MAKEWORD( ( b2 << 2 ), b1 ) >> 2 );
      break;
    case 3:           //      U+00000800-U+0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
      // xxxx1111 = 0x0F, xx111111 = 0x3F, xx111111 = 0x3F
      {
        b1 &= 0x0F;
        b2 = 0;
        b3 = 0;
      }
      if ( pUtf8Cur < pUtf8End ) {
        b2 = *pUtf8Cur++;
        b2 &= 0x3F;
      }
      if ( pUtf8Cur < pUtf8End ) {
        b3 = *pUtf8Cur++;
        b3 &= 0x3F;
      }
      *pUnicodeCur++ = ( WORD ) ( ( ( ( WORD ) b1 ) << 12 ) | ( ( ( WORD ) b2 ) << 6 ) | ( ( WORD ) b3 ) );
      break;
    }
  }
  return nSizeInUnicode;
}
// ��UNICODE�ַ�ת��ΪUTF8�ַ����߼�����Ҫ�������������
LONG CPfxEditDocUtf8Converter::UnicodeToUtf8( LPCWSTR pUnicode, LONG cchUnicode, LPSTR pUtf8, LONG cchUtf8 ) const {
  if ( ( pUnicode == NULL )
       || ( cchUnicode <= 0 )
       || ( ( cchUtf8 != 0 ) && ( pUtf8 == NULL ) ) ) {
    return 0;
  }
  LPCWSTR pUnicodeCur = pUnicode;
  LPCWSTR pUnicodeEnd = pUnicode + cchUnicode;
  LPBYTE pUtf8Cur = reinterpret_cast<LPBYTE>( pUtf8 );
  LPBYTE pUtf8End = reinterpret_cast<LPBYTE>( pUtf8 ) + cchUtf8;
  LONG nSizeInUtf8 = 0;
  // �����������������Ĵ�С
  if ( cchUtf8 == 0 ) {
    while ( pUnicodeCur < pUnicodeEnd ) {
      nSizeInUtf8 += UnicodeToUtf8CharSize( *pUnicodeCur );
      pUnicodeCur++;
    }
    return nSizeInUtf8;
  }
  // ��ʼִ��UNICODEת����UTF8�Ĳ���
  while ( ( pUnicodeCur < pUnicodeEnd ) && ( pUtf8Cur < pUtf8End ) ) {
    WORD w = *pUnicodeCur++;
    switch ( UnicodeToUtf8CharSize( w ) ) {
    default:
    case 1:           //      U+00000000-U+0000007F: 0xxxxxxx
      // 0xxxxxxx = 0x7F
      {
        *pUtf8Cur++ = ( BYTE ) ( LOBYTE( w ) );
        nSizeInUtf8++;
      }
      break;
    case 2:           //      U+00000080-U+000007FF: 110xxxxx 10xxxxxx
      // 0x07FF��11bit��b1ȡ5bit��b2ȡ6bit
      // 110xxxxx = 0xC0, 10xxxxxx = 0x80, xx111111 = 0x3F
      // b1 = 0xC0 | (HIBYTE((w << 2)))
      // b2 = 0x80 | (LOBYTE(w) & 0x03F)
      {
        *pUtf8Cur++ = ( BYTE ) ( 0xC0 | ( HIBYTE( w << 2 ) ) );
        nSizeInUtf8++;
      }
      if ( pUtf8Cur < pUtf8End ) {
        *pUtf8Cur++ = ( BYTE ) ( 0x80 | ( LOBYTE( w ) & 0x03F ) );
        nSizeInUtf8++;
      }
      break;
    case 3:           //      U+00000800-U+0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
      // 0xFFFF��16bit��b1ȡ4bit, b2ȡ6bit, b3ȡ6bit
      // 1110xxxx = 0xE0, 10xxxxxx = 0x80, 10xxxxxx = 0x80
      // b1 = 0xE0 | ((HIBYTE(w) >> 4)
      // b2 = 0x80 | ((w & 0x0FFF) << 2)
      // b3 = 0x80 | (LOBYTE(w) & 0x3F)
      {
        *pUtf8Cur++ = ( BYTE ) ( 0xE0 | ( HIBYTE( w ) >> 4 ) );
        nSizeInUtf8++;
      }
      if ( pUtf8Cur < pUtf8End ) {
        //  0000xxxx-xx000000->00000000-00xxxxxx=shr 6 bit
        *pUtf8Cur++ = ( BYTE ) ( 0x80 | ( ( w & 0x0FFF ) >> 6 ) );
        nSizeInUtf8++;
      }
      if ( pUtf8Cur < pUtf8End ) {
        *pUtf8Cur++ = ( BYTE ) ( 0x80 | ( LOBYTE( w ) & 0x3F ) );
        nSizeInUtf8++;
      }
      break;
    }
  }
  return nSizeInUtf8;
}
CPfxEditDoc::CPfxEditDoc() {
  m_bDrawRuler = false;
  m_bDrawLineNumber = true;
  m_bDrawLineBreak = true;
  m_bDrawWhitespace = true;
  m_bReadOnly = false;
  m_bLockedBufferCompact = false;
  m_bLockedWordWrapEvent = false;
  m_nWrapStyle = PFX_EC_WRAP_NONE;
  m_nWrapWidth = 0;
  m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_DOS;
  m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
  m_nOpenConverter = PFX_EC_FILE_CONVERTER_AUTO_DETECT;
  m_nSaveConverter = PFX_EC_FILE_CONVERTER_ANSI;
  m_nBookmarks = 0;
  m_nDocSize = 0;
  m_nDocLineCount = 1;
  m_rectIndent.left = m_rectIndent.top = 0;
  m_rectIndent.right = m_rectIndent.bottom = 0;
  m_clsLine.nli = 0;
  m_clsLine.nliRemain = 0;
  m_clsLine.nliGrowBy = PFX_EC_LINE_LINEINFO_ARRAY_GROWBY;
  m_clsLine.pli = NULL;
  m_clsLine.pmb = NULL;
  m_clsUndo.nui = 0;
  m_clsUndo.nuiRemain = 0;
  m_clsUndo.nuiGrowBy = PFX_EC_UNDO_UNDOINFO_ARRAY_GROWBY;
  m_clsUndo.nuiMax = PFX_EC_UNDO_UNDOINFO_ARRAY_MAXLEN;
  m_clsUndo.nPosCur = 0;
  m_clsUndo.nPosUnmodified = 0;
  m_clsUndo.pui = NULL;
  m_clsUndo.pmb = NULL;
  m_clsUndo.nGroupName = PFX_EC_UNDO_UNKNOWN;
  m_clsUndo.bGroupBegin = false;
  m_clsUndo.nGroupLockCount = 0;
  m_arrViews[ 0 ] = m_arrViews[ 1 ] = NULL;
  OnNewDocument();
}
CPfxEditDoc::~CPfxEditDoc() {
  DeleteContents();
}
// CPfxEditDoc serialization
void CPfxEditDoc::Serialize( CArchive& ar ) {
  if ( ar.IsStoring() ) {
    // д���ļ�ʱ��Ӧ��ʹ��ӳ���ļ����ƣ���Ϊʹ��ӳ���ļ�����д
    // ��Զ���ļ�ʱ����ȫ��ϵͳ�޷���֤�ļ�����������ͬ��
    switch ( GetFileConverter() ) {
    default:
    case PFX_EC_FILE_CONVERTER_ANSI:
      FileSaveAnsi( ar );
      break;
    case PFX_EC_FILE_CONVERTER_UTF8:
      FileSaveUtf8( ar );
      break;
    case PFX_EC_FILE_CONVERTER_UNICODE:
      FileSaveUnicode( ar );
      break;
    case PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN:
      FileSaveUnicodeBigEndian( ar );
      break;
    }
  } else {
    CPfxEditDocFile file;
    if ( !file.MapFile( reinterpret_cast<HANDLE>( ar.GetFile() ->m_hFile ), static_cast<LONG>( ar.GetFile() ->GetLength() ) ) ) {
      return ;
    }
    LONG nFileConverter = GetOpenConverter();
    if ( nFileConverter == PFX_EC_FILE_CONVERTER_AUTO_DETECT ) {
      nFileConverter = FileGetConverterType( static_cast<LPCTSTR>( file.GetBuffer() ), file.GetSize() );
    }
    // �����ļ����õ�ת��������ļ����õ�ת����һ��
    SetFileConverter( nFileConverter );
    ASSERT( nFileConverter != PFX_EC_FILE_CONVERTER_AUTO_DETECT );
    switch ( nFileConverter ) {
    default:
    case PFX_EC_FILE_CONVERTER_ANSI:
      FileLoadAnsi( file );
      break;
    case PFX_EC_FILE_CONVERTER_UTF8:
      FileLoadUtf8( file );
      break;
    case PFX_EC_FILE_CONVERTER_UNICODE:
      FileLoadUnicode( file );
      break;
    case PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN:
      FileLoadUnicodeBigEndian( file );
      break;
    }
  }
}
// ָʾ�ĵ��Ƿ��Ѿ����޸�
// BOOL bModified - TRUE�������޸ģ�FALSE��ʾ�ĵ�δ���޸�
// ��CPfxEditDoc��������һ��Ҫ���û����SetModifiedFlag()
void CPfxEditDoc::SetModifiedFlag( BOOL bModified ) {
  if ( !bModified ) {
    UndoSetUnmodified();
  }
  CDocument::SetModifiedFlag( bModified );
}
void CPfxEditDoc::DeleteContents() {
  BufferLineCleanup();
  BufferUndoCleanup();
  m_nBookmarks = 0;
  m_nDocSize = 0;
  m_nDocLineCount = 1;
  m_nLineLastParsed = m_nLineLastParsedButCanceled = -1;
  m_bLockedBufferCompact = false;
  m_bLockedWordWrapEvent = false;
}
void CPfxEditDoc::OnChangedViewList() {
  ASSERT( PFX_EC_SIZEOF( m_arrViews ) == 2 );
  m_arrViews[ 0 ] = m_arrViews[ 1 ] = NULL;
  POSITION pos = GetFirstViewPosition();
  for ( LONG i = 0; ( pos != NULL ) && ( i < PFX_EC_SIZEOF( m_arrViews ) ); ) {
    // �����϶�һ����CPfxEditView���п�����CPreviewView
    CView* pView = GetNextView( pos );
    // if ( pView->IsKindOf( RUNTIME_CLASS( CPfxEditView ) ) )
    {
      m_arrViews[ i ] = static_cast<CPfxEditView*>( pView );
      i++;
    }
  }
  // һ����Ҫ��������ǰ�棬��Ϊ�����ܻᡰdelete this���Ӷ������Ĵ���Ƿ�����
  CDocument::OnChangedViewList();
}
BOOL CPfxEditDoc::OnNewDocument() {
  if ( CDocument::OnNewDocument() ) {
    return OnInitDocument( NULL );
  }
  return false;
}
BOOL CPfxEditDoc::OnOpenDocument( LPCTSTR pszPathName ) {
  if ( CDocument::OnOpenDocument( pszPathName ) ) {
    return OnInitDocument( pszPathName );
  }
  return false;
}
BOOL CPfxEditDoc::OnSaveDocument( LPCTSTR pszPathName ) {
  BOOL bSaveAs = IsReadOnly() || ( GetPathName() != pszPathName );
  BOOL bResult = CDocument::OnSaveDocument( pszPathName );
  if ( bResult && bSaveAs ) {
    bResult = OnInitDocument( pszPathName );
  }
  return bResult;
}
// ���ĵ���װ���������һ���ĵ�ʱ���ڳ�ʼ���ĵ��ĸ��ֲ�������������ĵ�
// ����չ��������ʹ�������﷨���÷�����
// LPCTSTR pszPathName - �ĵ��ļ�������·���������������NULL���������
//                    һ���½��Ŀ��ĵ�������ʹ�á��½������
// �ɹ�����TRUE��ʧ�ܷ���FALSE
// ������һ��Ҫ���û���ı�����
BOOL CPfxEditDoc::OnInitDocument( LPCTSTR pszPathName ) {
  UNUSED( pszPathName );
  // �����ĵ�ʱ�����ʼ���ı��������������Ƿ�����
  if ( TextGetLineCount() == 0 ) {
    TextInsertLine( 0, NULL, 0, PFX_EC_LF_NONE );
  }
  m_bDrawRuler = true;
  m_bDrawLineNumber = true;
  m_bDrawLineBreak = true;
  m_bDrawWhitespace = true;
  m_nWrapStyle = PFX_EC_WRAP_NONE;
  m_nWrapWidth = 0;
  return true;
}
// �����Զ����¼�
// UINT nCode    - �û��Զ�����Ϣ
// LPARAM lParam - �����Ϣ��صĲ���
// �Զ������Ϣ������
LONG CPfxEditDoc::OnEvent( UINT nCode, LPARAM lParam ) {
  UNUSED( nCode );
  UNUSED( lParam );
  return 0;
}
// �����¼������¼�����OnEvent()�¼�����������
// UINT nCode    - ���Զ������Ϣ
// LPARAM lParam - �����Ϣ��صĲ���
// OnEvent()�Ը���Ϣ�Ĵ�����
LONG CPfxEditDoc::EventSendMsg( UINT nCode, LPARAM lParam ) {
  if ( nCode == PFX_EN_WORD_WRAPPING ) {
    // ֻ�е��ĵ��������64KTCHARʱ�ŷ��ͽ�����Ϣ
    if ( IsLockedWordWrapEvent()
         || ( GetDocSize() < ( 64 * 1024 ) ) ) {
      return 0;
    }
  }
  return OnEvent( nCode, lParam );
}
// ���´�Դ�ļ���ȡ����
// �ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL CPfxEditDoc::Reload() {
  if ( !IsCanReload() ) {
    return false;
  }
  BOOL bOldReadOnly = IsReadOnly();
  LONG nOldWrapStyle = GetWrapStyle();
  LONG nOldWrapWidth = GetWrapWidth();
  // ���������ͼ��ǰ��״̬�Ա���ִ�в�����ָ���Щ����
  struct CState {
    CPoint ptCurrent;
    CPfxEditView* pView;
  };
  CState state[ PFX_EC_SIZEOF( m_arrViews ) ];
  for ( LONG i = 0; i < PFX_EC_SIZEOF( state ); i++ ) {
    CState& rState = state[ i ];
    rState.pView = m_arrViews[ i ];
    if ( rState.pView == NULL ) {
      continue;
    }
    rState.ptCurrent = rState.pView->GetCurrentPosition();
    rState.pView->OnInitialize();
  }
  // �������������ĵ�����
  CString strPathName = GetPathName();
  if ( !OnOpenDocument( strPathName ) ) {
    return false;
  }
  // ���ȱ������³�ʼ��������ͼ
  m_nWrapWidth = 0;
  m_nWrapStyle = PFX_EC_WRAP_NONE;
  SendInitialUpdate();
  // ���ԭ����ֻ���������غ�ҲӦ����ֻ��
  SetReadOnly( IsReadOnly() || bOldReadOnly );
  // ��ͼ�ָ�����ǰ��λ�ã���������������ͽ���ǰλ�����õ��ĵ���ͷ
  for ( i = 0; i < PFX_EC_SIZEOF( m_arrViews ); i++ ) {
    CState& rState = state[ i ];
    if ( rState.pView == NULL ) {
      continue;
    }
    CPoint cpNew( 0, 0 );
    if ( GetLineCount() > rState.ptCurrent.y ) {
      cpNew.y = rState.ptCurrent.y;
      cpNew.x = min( rState.ptCurrent.x, GetLineLength( cpNew.y ) );
    }
    rState.pView->SetCurrentPosition( cpNew.y, cpNew.x );
  }
  CPfxEditView* pFirstView = m_arrViews[ 0 ];
  if ( pFirstView != NULL ) {
    pFirstView->SetViewWrapping( nOldWrapStyle, nOldWrapWidth );
  }
  return true;
}
// �����뱾�ĵ�����������CPfxEditView
// BOOL bRecalcWrap - �Ƿ�����ִ���Զ�����
void CPfxEditDoc::UpdateAllViews( BOOL bRecalcWrap ) {
  CPfxEditView * pView = m_arrViews[ 0 ];
  if ( pView == NULL ) {
    return ;
  }
  // ǿ��ˢ�������ǰ�Զ����з�ʽ��PFX_EC_WRAP_WINDOWEDGE
  if ( !bRecalcWrap || !IsWrapAtWindowEdge() ) {
    pView->UpdateAllViews( PFX_EC_UT_ALL, 0, 0 );
  } else {
    pView->UpdateLayout();
    // CPfxEditView::SetViewWrapping()���ж��ı����Ŀ���Ƿ��Ѿ�
    // �ı䣬���û�иı�Ͳ�ִ�л��У�����Ҫ���Զ����з������
    // ��PFX_EC_WRAP_NONEȻ��������ִ�л��в���
    m_nWrapStyle = PFX_EC_WRAP_NONE;
    m_nWrapWidth = 0;
    pView->SetViewWrapping( PFX_EC_WRAP_WINDOWEDGE, 0 );
  }
}
BOOL CPfxEditDoc::IsValidLine( LONG nLine ) const {
  return ( nLine >= 0 ) && ( nLine < m_nDocLineCount );
}
BOOL CPfxEditDoc::IsValidChar( LONG nLine, LONG nChar ) const {
  CPoint pt( nChar, nLine );
  if ( ( pt.y >= 0 ) && ( pt.y < GetLineCount() ) && ( pt.x >= 0 ) ) {
    TextAbsLineToMainLine( pt );
    if ( pt.x <= TextGetLineAt( pt.y ) ->nActLen ) {
      return true;
    }
  }
  return false;
}
LONG CPfxEditDoc::GetValidLine( LONG nLine ) const {
  if ( nLine >= GetLineCount() ) {
    nLine = GetLineUpperBound();
  }
  return max( 0, nLine );
}
LONG CPfxEditDoc::GetValidChar( LONG nLine, LONG nChar ) const {
  ASSERT( ( nLine >= 0 ) && ( nLine < GetLineCount() ) );
  CPoint pt( 0, GetValidLine( nLine ) );
  TextAbsLineToMainLine( pt );
  LONG cch = TextGetLineAt( pt.y ) ->nActLen;
  if ( nChar > cch ) {
    nChar = cch;
  }
  return max( 0, nChar );
}
// ��ȡ��ǰ����ʹ�õ�CPfxEditTypeInfo�����ָ�롣��������û�����ù���
// ����ʹ��SetTypeInfoPtr()�����򷵻�һ�ڲ�Ĭ�ϵĶ����ָ��
// �����ǰû����ͼ�ͷ���NULL�����򷵻�һ����ָ��
const CPfxEditTypeInfo* CPfxEditDoc::GetTypeInfoPtr() const {
  return ( ( m_arrViews[ 0 ] != NULL ) ? m_arrViews[ 0 ] ->m_pti : NULL );
}
// BOOL SetTypeInfoPtr(const CPfxEditTypeInfo* pTypeInfo)
// �����ĵ��������������������﷨������
#if 0
void setSyntaxHilight() {
  static CPfxEditTypeInfo ti;
  static const LPCTSTR arrKeywords[] = {
                                         _T( "int" ), _T( "float" ), _T( "char" ), _T( "unsigned" ), _T( "do" ), _T( "while" ), _T( "for" ), _T( "inline" ), _T( "const" ),
                                       }
                                       LONG nClass = ti.SetKeyClass( _T( "������" ), RGB( 0, 0, 255 ) );
  for ( int i = 0; i < PFX_EC_SIZEOF( arrKeywords ); i++ ) {
    ti.SetKeyword( arrKeywords[ i ], nClass );
  }
  static const LPCTSTR arrOperators[] = {
                                          _T( "+" ), _T( "-" ), _T( "*" ), _T( "/" ), _T( ">>" ), _T( "<<" ), _T( "->" ), _T( "||" ), _T( "&&" ), _T( "!" ),
                                        }
                                        nClass = ti.SetKeyClass( _T( "������" ), RGB( 0, 255, 0 ) );
  for ( int i = 0; i < PFX_EC_SIZEOF( arrOperators ); i++ ) {
    ti.SetKeyword( arrOperators[ i ], nClass );
  }
  SetTypeInfoPtr( &ti );
}
#endif 
// const CPfxEditTypeInfo* pTypeInfo - CPfxEditTypeInfoָ�롣��ָ�����
//      ָ��һ�㶨�Ķ��󣬼���˵һ�����ñ���������ָ����ָ��Ķ����
//      ����һֱ���ڣ�ֱ������ʹ�ø�ָ��� CPfxEditDoc�����١���˲�
//      ���ڶ�ջ���� CPfxEditTypeInfo���󣬶�Ӧ��ʹ��new�����������
//      ʹ�þ�̬���������ʹ�� new����������ö�����ô���ס�ڳ�
//      �����ʱ��deleteɾ���ö���CPfxEditDoc�����Զ�Ϊ��ɾ����
//      ��������£������ʹ��һCList�������Щ���󼯣���Ϊ����﷨
//      �༭������ͬʱ֧�ֺܶ������Եı༭����������������壺
//      typedef CList<CPfxEditTypeInfo, CPfxEditTypeInfo&>CListTypeInfo;
BOOL CPfxEditDoc::SetTypeInfoPtr( CPfxEditTypeInfo* pTypeInfo ) {
  if ( pTypeInfo == NULL ) {
    pTypeInfo = &PFX_EC_DATA.m_defTypeInfo;
  }
  for ( LONG i = 0; i < PFX_EC_SIZEOF( m_arrViews ); i++ ) {
    if ( m_arrViews[ i ] != NULL ) {
      m_arrViews[ i ] ->m_pti = pTypeInfo;
    }
  }
  m_bDrawRuler = pTypeInfo->m_bDrawRuler;
  m_bDrawLineNumber = pTypeInfo->m_bDrawLineNumber;
  m_bDrawLineBreak = pTypeInfo->m_bDrawLineBreak;
  m_bDrawWhitespace = pTypeInfo->m_bDrawWhitespace;
  CPfxEditView* pView = m_arrViews[ 0 ];
  if ( pView != NULL ) {
    pView->UpdateAllViews( PFX_EC_UT_ALL, 0, 0 );
    // �µ��Զ���������ֻ��Ӱ�쵱ǰ�Ѿ������Զ����е��ĵ������
    // ĳ�ĵ���ǰ�����Զ�����״̬������������µ��Զ�����ָʾ
    if ( IsWrapping() ) {
      pView->SetViewWrapping(
        pTypeInfo->m_nWrapStyle, pTypeInfo->m_nWrapWidth );
    }
  }
  return true;
}
// ��ȡUNDO����
LONG CPfxEditDoc::GetUndoName() const {
  const CPfxEditUndoInfo * pui = UndoGetInfo();
  return ( ( pui != NULL ) ? pui->fUndoName : PFX_EC_UNDO_UNKNOWN );
}
// ��ȡREDO����
LONG CPfxEditDoc::GetRedoName() const {
  const CPfxEditUndoInfo * pui = UndoGetRedoInfo();
  return ( ( pui != NULL ) ? pui->fUndoName : PFX_EC_UNDO_UNKNOWN );
}
void CPfxEditDoc::SetLineBreakType( LONG nType ) {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS ) || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX ) || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  switch ( nType ) {
  case PFX_EC_LINE_BREAK_TYPE_DOS:
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    break;
  default:
    nType = PFX_EC_LINE_BREAK_TYPE_DOS;
    break;
  }
  if ( GetLineBreakType() == nType )           //      û�иı䣬���ظ���
  {
    return ;
  }
  // �����ĵ���С��Ϣ
  LONG nDocSize = 0;
  LONG nLineBreakSize = GetLineBreakSizeFromType( nType );
  for ( LONG i = 0, nCount = TextGetLineCount(); i < nCount; i++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( i );
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      nDocSize += ( pli->nLen + nLineBreakSize );
    } else {
      nDocSize += pli->nLen;
    }
  }
  // ����ѡ���ı���С
  for ( i = 0; i < PFX_EC_SIZEOF( m_arrViews ); i++ ) {
    CPfxEditView* pView = m_arrViews[ i ];
    if ( ( pView != NULL ) && pView->IsTextSelected() ) {
      pView->m_nSelSize = pView->GetSelSize( nType, false );
    }
  }
  m_nDocSize = nDocSize;
  m_nLineBreakType = nType;
  m_nLineBreakSize = GetLineBreakSizeFromType( nType );
}
// ��ȡָ���еĳ���
// LONG nLine - ����Ϊ��ֵ���к�
// �����еĳ��ȣ�����к���Ч����-1�����򷵻�ʵ�ʳ��ȣ���λ��TCHAR
LONG CPfxEditDoc::GetLineLength( LONG nLine ) const {
  LONG nLength = -1;
  if ( IsValidLine( nLine ) ) {
    CPoint pt( 0, nLine );
    TextAbsLineToMainLine( pt );
    nLength = TextGetLineAt( pt.y ) ->nActLen;
  }
  return nLength;
}
// ��ȡĳ������
// LONG nLine     - �кţ�����Ϊ��ֵ
// LPTSTR pBuffer - �������ݵĻ�����
// LONG nBufMax   - ����������NULL�ַ��ĳ���
// ����ʵ�ʸ��Ƶ��ַ�����������NULL�ַ�����λ��TCHAR
LONG CPfxEditDoc::GetLineData( LONG nLine, LPTSTR pBuffer, LONG nBufMax ) const {
  ASSERT( AfxIsValidAddress( pBuffer, ( nBufMax * sizeof( TCHAR ) ), true ) );
  if ( ( pBuffer == NULL ) || ( nBufMax <= 0 ) ) {
    return 0;
  }
  LONG cch = 0;
  if ( IsValidLine( nLine ) ) {
    CPfxEditView * pView = m_arrViews[ 0 ];
    ASSERT( pView != NULL );
    CPoint pt( 0, nLine );
    TextAbsLineToMainLine( pt );
    const CPfxEditLineInfo* pli = TextGetLineAt( pt.y );
    cch = pli->nActLen;
    // ΪNULL�ַ�����
    nBufMax--;
    if ( nBufMax < cch ) {
      cch = pView->CalcCharsToCopy( pli->pData, nBufMax );
    }
    if ( cch > 0 ) {
      memcpy( pBuffer, pli->pData, ( cch * sizeof( TCHAR ) ) );
      pBuffer += cch;
    }
  }
  *pBuffer = '\0';
  return cch;
}
LPCSTR CPfxEditDoc::GetLineBreakDataFromTypeA( LONG nType ) const {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  // ���з�����
  static const CHAR szLineBreakDos[] = "\r\n"; // Dos, Windows
  static const CHAR szLineBreakMac[] = "\r"; // Macintosh
  static const CHAR szLineBreakUnix[] = "\n"; // Unix
  LPCSTR pszLineBreakData;
  switch ( nType ) {
  default:           // ��ͨ
  case PFX_EC_LINE_BREAK_TYPE_DOS:
    pszLineBreakData = szLineBreakDos;
    break;
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
    pszLineBreakData = szLineBreakUnix;
    break;
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    pszLineBreakData = szLineBreakMac;
    break;
  }
  return pszLineBreakData;
}
LONG CPfxEditDoc::GetLineBreakSizeFromTypeA( LONG nType ) const {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  LONG nLineBreakSize;
  switch ( nType ) {
  default:           // ��ͨ
  case PFX_EC_LINE_BREAK_TYPE_DOS:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
    break;
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_UNIX;
    break;
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_MAC;
    break;
  }
  return nLineBreakSize;
}
LPCWSTR CPfxEditDoc::GetLineBreakDataFromTypeW( LONG nType ) const {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  // ���з�����
  static const WCHAR szLineBreakDos[] = L"\r\n"; // Dos, Windows
  static const WCHAR szLineBreakMac[] = L"\r"; // Macintosh
  static const WCHAR szLineBreakUnix[] = L"\n"; // Unix
  LPCWSTR pszLineBreakData;
  switch ( nType ) {
  default:           // ��ͨ
  case PFX_EC_LINE_BREAK_TYPE_DOS:
    pszLineBreakData = szLineBreakDos;
    break;
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
    pszLineBreakData = szLineBreakUnix;
    break;
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    pszLineBreakData = szLineBreakMac;
    break;
  }
  return pszLineBreakData;
}
LONG CPfxEditDoc::GetLineBreakSizeFromTypeW( LONG nType ) const {
  ASSERT( ( nType == PFX_EC_LINE_BREAK_TYPE_DOS )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_UNIX )
          || ( nType == PFX_EC_LINE_BREAK_TYPE_MAC ) );
  LONG nLineBreakSize;
  switch ( nType ) {
  default:           // ��ͨ
  case PFX_EC_LINE_BREAK_TYPE_DOS:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
    break;
  case PFX_EC_LINE_BREAK_TYPE_UNIX:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_UNIX;
    break;
  case PFX_EC_LINE_BREAK_TYPE_MAC:
    nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_MAC;
    break;
  }
  return nLineBreakSize;
}
// ��ȡ��Ҫָ�����ȵĻ�����
void CPfxEditDoc::BufferLineGetSpace( LONG nLen, TCHAR** ppBuffer, CPfxEditDocMemBlock** ppMemBlock ) {
  ASSERT( nLen > 0 );
  // �������е��ڴ�飬������û�п��õĿռ䣬����о��ڿ��õĿռ��ڷ�
  // �䣬����Ҫ�����µ��ڴ��
  for ( CPfxEditDocMemBlock * pMemBlock = m_clsLine.pmb; pMemBlock != NULL; pMemBlock = pMemBlock->pNext ) {
    if ( pMemBlock->nRemain >= nLen ) {
      break;
    }
  }
  // û�п��õĿռ�����Ҫ��Ҫ����һ�µ��ڴ��
  if ( pMemBlock == NULL ) {
    pMemBlock = BufferLineAllocBlock( nLen );
  }
  *ppBuffer = pMemBlock->pRemain;
  *ppMemBlock = pMemBlock;
  pMemBlock->nRemain -= nLen;
  pMemBlock->pRemain += nLen;
  ASSERT( pMemBlock->nRemain >= 0 );
}
// ��ĳ���ڴ��ִ����������
void CPfxEditDoc::BufferLineCompact( CPfxEditDocMemBlock* pCurMemBlock, LONG nNewUnused ) {
  ASSERT( pCurMemBlock != NULL );
  ASSERT( nNewUnused >= 0 );
  // ������ڴ���Ѿ������޷�ʹ�õĿռ��ܺʹﵽĳ����������Ҫ���µ���
  // ���ڴ�顣������������������һ�飬Ȼ��Ѿɿ�����ݰᵽ�¿�ȥ����
  // ���µ���������ɿ���ص��Ѿ������ȥ�Ļ���������Ϣ
  pCurMemBlock->nUnused += nNewUnused;
  LONG nActualUsed = pCurMemBlock->nLength - ( pCurMemBlock->nUnused + pCurMemBlock->nRemain );
  if ( ( pCurMemBlock->nUnused < pCurMemBlock->nMaxUnused ) && ( nActualUsed > 0 ) ) {
    return ;
  }
  ASSERT( ( nActualUsed >= 0 ) && ( nActualUsed <= pCurMemBlock->nLength ) );
  TRACE( "CPfxEditDoc::BufferLineCompact() Begin dump of mem block 0x%X:\n", pCurMemBlock );
  TRACE( "\tOld block size %d TCHAR(s)\n", pCurMemBlock->nLength );
  TRACE( "\tOld block unused size %d TCHAR(s)\n", pCurMemBlock->nUnused );
  TRACE( "\tOld block remain size %d TCHAR(s)\n", pCurMemBlock->nRemain );
  // ����Ѿ����õĿռ����ʣ��Ŀռ���ڸ��ڴ�����������ܴ�С�����
  // ���Ѿ�û����ʹ������ڴ�飬�����ͷŵ������Ҳ���Ҫ��������ռ�
  if ( nActualUsed > 0 ) {
    if ( m_bLockedBufferCompact ) {
      return ;
    }
    CPfxEditDocMemBlock* pNewMemBlock = BufferLineAllocBlock( nActualUsed );
    // ɨ����������Ϣ�����飬������ɿ�������ж������µ����仺
    // ����ָ�룬ʹָ֮���¿�
    LONG nRemain = pNewMemBlock->nRemain;
    LPTSTR pRemain = pNewMemBlock->pRemain;
    CPfxEditLineInfo* pli = m_clsLine.pli;
    CPfxEditLineInfo* pliEnd = m_clsLine.pli + m_clsLine.nli;
    for ( ; pli < pliEnd; pli++ ) {
      if ( pCurMemBlock != pli->pMemBlock ) {
        continue;
      }
      ASSERT( pli->nLen > 0 );
      ASSERT( pli->pData != NULL );
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      LONG cch = pli->nActLen;
      LPTSTR pData = pRemain;
      memcpy( pData, pli->pData, cch * sizeof( TCHAR ) );
      pli->pData = pData;
      pli->pMemBlock = pNewMemBlock;
      // �������У����һ��Ҳ��û��PFX_EC_LF_HARDRETURN
      // ��־�ģ����Ա�����(pli < pliEnd)������
      if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
        pData += pli->nLen;
        while ( ( ++pli ) < pliEnd ) {
          ASSERT( PFX_EC_IS_SUBLINE( pli ) );
          ASSERT( pli->nLen > 0 );
          ASSERT( pli->pData != NULL );
          ASSERT( pli->pMemBlock == NULL );
          pli->pData = pData;
          if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
            break;
          }
          pData += pli->nLen;
        }
      }
      nRemain -= cch;
      pRemain += cch;
    }
    pNewMemBlock->nRemain = nRemain;
    pNewMemBlock->pRemain = pRemain;
    ASSERT( nRemain >= 0 );
    TRACE( "\tNew block handle 0x%X\n", pNewMemBlock );
    TRACE( "\tNew block size %d\n", pNewMemBlock->nLength );
    TRACE( "\tNew block remain size %d\n", pNewMemBlock->nRemain );
  }
  // ɾ���ɵ��ڴ��
  BufferLineRemoveBlock( pCurMemBlock );
  TRACE( "Deleting block 0x%X\n", pCurMemBlock );
  TRACE( "Line memory block chain: %X\n", m_clsLine.pmb );
  TRACE( "End dump CPfxEditDoc::BufferLineCompact()\n" );
}
CPfxEditDocMemBlock* CPfxEditDoc::BufferLineAllocBlock( LONG nBlockSize ) {
  ASSERT( nBlockSize > 0 );
  // ����һ�µ��ڴ�飬�µ��ڴ��ṹ���£�
  // |=========================|
  // | CPfxEditDocMemBlock�ṹ | ���������ٸ��ڴ���ʹ�������
  // |-------------------------|
  // |       ������            |
  // |=========================|
  // ���ӵĽṹ��С
  LONG nExtraSize = sizeof( CPfxEditDocMemBlock );
  // �����ڴ��Ĵ�С�ǣ����ӵĽṹ��С + ��������С��
  LONG nAllocSize = PFX_EC_ROUND( ( nBlockSize + nExtraSize ), PFX_EC_LINE_MEMBLOCK_ALLOC_LENGTH );
  // �����ڴ��Ĵ�С������PFX_EC_LINE_MEMBLOCK_ALLOC_LENGTH�ı������������
  // ���Ĵ�С�ǣ������ڴ��Ĵ�С - ���ӵĽṹ��С��
  CPfxEditDocMemBlock* pMemBlock = ( CPfxEditDocMemBlock* ) new TCHAR[ nAllocSize ];
  memset( pMemBlock, 0, sizeof( TCHAR ) * nAllocSize );
  // �����µ��ڴ��ָ�룬���Ƿ�����ͷ����Ϊ�µĻ����ͨ���кܶ���ÿ�
  // �䣬�������Ż�����
  pMemBlock->pNext = m_clsLine.pmb;
  pMemBlock->pPrev = NULL;
  if ( m_clsLine.pmb != NULL ) {
    m_clsLine.pmb->pPrev = pMemBlock;
  }
  m_clsLine.pmb = pMemBlock;
  // ���ø����ڴ�����Ϣ
  pMemBlock->nLength = nAllocSize - nExtraSize;
  pMemBlock->nRemain = pMemBlock->nLength;
  pMemBlock->pRemain = ( LPTSTR ) ( ( LPTSTR ) pMemBlock + nExtraSize );
  pMemBlock->nUnused = 0;
  pMemBlock->nMaxUnused = pMemBlock->nLength / PFX_EC_LINE_MEMBLOCK_UNUSED_PERCENT;
  return pMemBlock;
}
void CPfxEditDoc::BufferLineRemoveBlock( CPfxEditDocMemBlock* pMemBlock ) {
  ASSERT( pMemBlock != NULL );
  ASSERT( AfxIsValidAddress( pMemBlock, sizeof( CPfxEditDocMemBlock ) ) );
  if ( pMemBlock == NULL ) {
    return ;
  }
  CPfxEditDocMemBlock* pNext = pMemBlock->pNext;
  CPfxEditDocMemBlock* pPrev = pMemBlock->pPrev;
  if ( pMemBlock == m_clsLine.pmb ) {
    m_clsLine.pmb = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = NULL;
    }
  } else {
    pPrev->pNext = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = pPrev;
    }
  }
  delete pMemBlock;
}
// �ͷ���������Ϣ��Դ
void CPfxEditDoc::BufferLineCleanup() {
  // ����л�����
  if ( m_clsLine.pli != NULL ) {
    delete[] m_clsLine.pli;
  }
  CPfxEditDocMemBlock* pNodeHead = m_clsLine.pmb;
  while ( pNodeHead != NULL ) {
    CPfxEditDocMemBlock * pNodeTemp = pNodeHead;
    pNodeHead = pNodeHead->pNext;
    delete pNodeTemp;
  }
  m_clsLine.pli = NULL;
  m_clsLine.nli = m_clsLine.nliRemain = 0;
  m_clsLine.pmb = NULL;
}
void CPfxEditDoc::BufferUndoGetSpace( LONG nLen, TCHAR** ppBuffer, CPfxEditDocMemBlock** ppMemBlock ) {
  ASSERT( nLen > 0 );
  // �������е��ڴ�飬������û�п��õĿռ䣬����о��ڿ��õĿռ��ڷ�
  // �䣬����Ҫ�����µ��ڴ��
  CPfxEditDocMemBlock* pMemBlock = m_clsUndo.pmb;
  for ( ; pMemBlock != NULL; pMemBlock = pMemBlock->pNext ) {
    if ( pMemBlock->nRemain >= nLen ) {
      break;
    }
  }
  // û�п��õĿռ�����Ҫ��Ҫ����һ�µ��ڴ��
  if ( pMemBlock == NULL ) {
    pMemBlock = BufferUndoAllocBlock( nLen );
  }
  *ppBuffer = pMemBlock->pRemain;
  *ppMemBlock = pMemBlock;
  pMemBlock->nRemain -= nLen;
  pMemBlock->pRemain += nLen;
  ASSERT( pMemBlock->nRemain >= 0 );
}
// ѹ��ָ����UNDO�ڴ�飬���µ�����Դ�Ա����˷��ڴ�ռ�
void CPfxEditDoc::BufferUndoCompact( CPfxEditDocMemBlock* pCurMemBlock, LONG nNewUnused ) {
  ASSERT( nNewUnused >= 0 );
  ASSERT( pCurMemBlock != NULL );
  // ������ڴ���Ѿ������޷�ʹ�õĿռ��ܺʹﵽĳ����������Ҫ���µ���
  // ���ڴ�顣������������������һ�飬Ȼ��Ѿɿ�����ݰᵽ�¿�ȥ����
  // ���µ���������ɿ���ص��Ѿ������ȥ�Ļ���������Ϣ
  pCurMemBlock->nUnused += nNewUnused;
  LONG nActualUsed = pCurMemBlock->nLength - ( pCurMemBlock->nUnused + pCurMemBlock->nRemain );
  if ( ( pCurMemBlock->nUnused < pCurMemBlock->nMaxUnused ) && ( nActualUsed > 0 ) ) {
    return ;
  }
  ASSERT( ( nActualUsed >= 0 ) && ( nActualUsed <= pCurMemBlock->nLength ) );
  TRACE( "\n\n\n" );
  TRACE( "CPfxEditDoc::BufferUndoCompact() Begin dump of mem block 0x%X:\n", pCurMemBlock );
  TRACE( "\tOld block size %d TCHAR(s)\n", pCurMemBlock->nLength );
  TRACE( "\tOld block unused size %d TCHAR(s)\n", pCurMemBlock->nUnused );
  TRACE( "\tOld block remain size %d TCHAR(s)\n", pCurMemBlock->nRemain );
  // ����Ѿ����õĿռ����ʣ��Ŀռ���ڸ��ڴ�����������ܴ�С�����
  // ���Ѿ�û����ʹ������ڴ�飬�����ͷŵ������Ҳ���Ҫ��������ռ�
  if ( nActualUsed > 0 ) {
    if ( m_bLockedBufferCompact ) {
      return ;
    }
    CPfxEditDocMemBlock* pNewMemBlock = BufferUndoAllocBlock( nActualUsed );
    // ɨ������UNDO��Ϣ���飬������ɿ�����Ļ�����������������
    // ����ȥ
    LONG nRemain = pNewMemBlock->nRemain;
    LPTSTR pRemain = pNewMemBlock->pRemain;
    CPfxEditUndoInfo* pui = m_clsUndo.pui;
    CPfxEditUndoInfo* puiEnd = m_clsUndo.pui + m_clsUndo.nui;
    for ( ; pui < puiEnd; pui++ ) {
      if ( pCurMemBlock == pui->pMemBlock ) {
        ASSERT( pui->nLen > 0 );
        ASSERT( pui->pData != NULL );
        LONG cch = pui->nLen;
        LPTSTR pData = pRemain;
        memcpy( pData, pui->pData, ( cch * sizeof( TCHAR ) ) );
        pui->pData = pData;
        pui->pMemBlock = pNewMemBlock;
        nRemain -= cch;
        pRemain += cch;
      }
    }
    ASSERT( nRemain >= 0 );
    pNewMemBlock->nRemain = nRemain;
    pNewMemBlock->pRemain = pRemain;
    TRACE( "\tNew block handle 0x%X\n", pNewMemBlock );
    TRACE( "\tNew block size %d\n", pNewMemBlock->nLength );
    TRACE( "\tNew block remain size %d\n", pNewMemBlock->nRemain );
  }
  // ɾ���ɵ��ڴ��
  BufferUndoRemoveBlock( pCurMemBlock );
  TRACE( "Deleting block 0x%X\n", pCurMemBlock );
  TRACE( "Undo memory block chain: %X\n", m_clsUndo.pmb );
  TRACE( "End dump of CPfxEditDoc::BufferUndoCompact()\n" );
}
// ����һUNDO�ڴ��
CPfxEditDocMemBlock* CPfxEditDoc::BufferUndoAllocBlock( LONG nBlockSize ) {
  ASSERT( nBlockSize > 0 );
  // ����һ�µ��ڴ�飬�µ��ڴ��ṹ���£�
  // |=========================|
  // | CPfxEditDocMemBlock�ṹ | ������ṹ�������ٸ��ڴ���ʹ�������
  // |-------------------------|
  // |       ������            |
  // |=========================|
  // ���ӵĽṹ��С
  LONG nExtraSize = sizeof( CPfxEditDocMemBlock );
  // �����ڴ��Ĵ�С�ǣ����ӵĽṹ��С + ��������С��
  LONG nAllocSize = PFX_EC_ROUND( ( nBlockSize + nExtraSize ), PFX_EC_UNDO_MEMBLOCK_ALLOC_LENGTH );
  // �����ڴ��Ĵ�С������PFX_EC_UNDO_MEMBLOCK_ALLOC_LENGTH�ı��������
  // �������Ĵ�С�ǣ������ڴ��Ĵ�С - ���ӵĽṹ��С��
  CPfxEditDocMemBlock* pMemBlock = ( CPfxEditDocMemBlock* ) new TCHAR[ nAllocSize ];
  // �����µ��ڴ��ָ�룬���Ƿ�����ͷ����Ϊ�µĻ����ͨ���кܶ���ÿ�
  // �䣬�������Ż�����
  pMemBlock->pNext = m_clsUndo.pmb;
  pMemBlock->pPrev = NULL;
  if ( m_clsUndo.pmb != NULL ) {
    m_clsUndo.pmb->pPrev = pMemBlock;
  }
  m_clsUndo.pmb = pMemBlock;
  // ���ø����ڴ�����Ϣ
  pMemBlock->nLength = nAllocSize - nExtraSize;
  pMemBlock->nRemain = pMemBlock->nLength;
  pMemBlock->pRemain = ( LPTSTR ) ( ( LPTSTR ) pMemBlock + nExtraSize );
  pMemBlock->nUnused = 0;
  pMemBlock->nMaxUnused = pMemBlock->nLength / PFX_EC_UNDO_MEMBLOCK_UNUSED_PERCENT;
  return pMemBlock;
}
// ɾ��һUNDO�ڴ��
void CPfxEditDoc::BufferUndoRemoveBlock( CPfxEditDocMemBlock* pMemBlock ) {
  ASSERT( pMemBlock != NULL );
  ASSERT( AfxIsValidAddress( pMemBlock, sizeof( CPfxEditDocMemBlock ) ) );
  if ( pMemBlock == NULL ) {
    return ;
  }
  CPfxEditDocMemBlock* pNext = pMemBlock->pNext;
  CPfxEditDocMemBlock* pPrev = pMemBlock->pPrev;
  if ( pMemBlock == m_clsUndo.pmb ) {
    m_clsUndo.pmb = pNext;
    if ( m_clsUndo.pmb != NULL ) {
      m_clsUndo.pmb->pPrev = NULL;
    }
  } else {
    pPrev->pNext = pNext;
    if ( pNext != NULL ) {
      pNext->pPrev = pPrev;
    }
  }
  delete pMemBlock;
}
// �ͷ�����UNDO��REDO��Դ
void CPfxEditDoc::BufferUndoCleanup() {
  // ���UNDO������
  if ( m_clsUndo.pui != NULL ) {
    delete[] m_clsUndo.pui;
  }
  CPfxEditDocMemBlock* pNodeHead = m_clsUndo.pmb;
  while ( pNodeHead != NULL ) {
    CPfxEditDocMemBlock * pNodeTemp = pNodeHead;
    pNodeHead = pNodeHead->pNext;
    delete pNodeTemp;
  }
  m_clsUndo.pui = NULL;
  m_clsUndo.nui = m_clsUndo.nuiRemain = 0;
  m_clsUndo.pmb = NULL;
  m_clsUndo.nPosCur = m_clsUndo.nPosUnmodified = 0;
  m_clsUndo.nGroupName = PFX_EC_UNDO_UNKNOWN;
  m_clsUndo.bGroupBegin = false;
  m_clsUndo.nGroupLockCount = 0;
}
// ���һ����������Ƿ�Ϸ���������Ϸ��͵����ɺϷ�
LONG CPfxEditDoc::TextVerifyLine( LONG nLine ) const {
  if ( nLine >= TextGetLineCount() ) {
    nLine = TextGetLineUpperBound();
  }
  return max( 0, nLine );
}
// ���ĳ������ָ������λ���Ƿ�Ϸ���������Ϸ�������ɺϷ�
LONG CPfxEditDoc::TextVerifyChar( LONG nLine, LONG nChar ) const {
  ASSERT( TextIsValidLine( nLine ) );
  if ( nChar > TextGetLineAt( nLine ) ->nLen ) {
    nChar = TextGetLineAt( nLine ) ->nLen;
  }
  return max( 0, nChar );
}
// ��ָ������ǰ����ָ����Ŀ�����У��������б���ʼ���ɿ���
// ��һ�е�ָ��
// ��������Ӱ������Ϣ����������֮ǰʹ�� TextGetLineAt()���ĳЩָ��
// ������Щָ�뻹��ʹ�õĻ���Ӧ���ڵ��� TextInsertLine()���ٴ�ʹ��
// LineGetAt()ˢ����Щָ�룬������ܻᷢ�����벻���Ľ��
CPfxEditLineInfo* CPfxEditDoc::TextInsertLine( LONG nIndex, LONG nCount ) {
  ASSERT( nIndex >= 0 && nIndex <= TextGetLineCount() );
  ASSERT( nCount > 0 );
  // ���������Ŀյ�Ԫ�����Ҫ��������ĳߴ磬������ܳ���Ϊ���õ�Ԫ
  // ��ʣ�൥Ԫ�����������ǡ���ʵ�ġ�һά�������飬�ɵ�һ����Ч��Ԫ��
  // ���Ԫ�м�û�пյ���Ч��Ԫ
  if ( m_clsLine.nliRemain < nCount ) {
    LONG nNewSize = ( m_clsLine.nli + nCount + PFX_EC_LINE_LINEINFO_ARRAY_GROWBY );
    CPfxEditLineInfo* pli = new CPfxEditLineInfo[ nNewSize ];
    if ( m_clsLine.pli != NULL ) {
      memcpy( pli, m_clsLine.pli, m_clsLine.nli * sizeof( CPfxEditLineInfo ) );
      delete[] m_clsLine.pli;
    }
    m_clsLine.pli = pli;
    m_clsLine.nliRemain = nNewSize - m_clsLine.nli;
  }
  CPfxEditLineInfo* pli = m_clsLine.pli + nIndex;
  LONG nli = m_clsLine.nli - nIndex;
  if ( nli > 0 ) {
    memmove( pli + nCount, pli, nli * sizeof( CPfxEditLineInfo ) );
  }
  m_clsLine.nli += nCount;
  m_clsLine.nliRemain -= nCount;
  return pli;
}
// ɾ����ָ��λ�ÿ�ʼָ����Ŀ����
// ���ñ������󽫻�Ӱ������Ϣ������˷���֮ǰʹ�� TextGetLineAt()���
// ���ҵ�ǰ����ʹ�õ�ָ�붼Ӧ���ٴε���LineGetAt()ˢ��
void CPfxEditDoc::TextRemoveLine( LONG nIndex, LONG nCount ) {
  ASSERT( nIndex >= 0 && ( nIndex + nCount ) <= TextGetLineCount() );
  ASSERT( nCount > 0 );
  LONG nli = m_clsLine.nli - ( nIndex + nCount );
  CPfxEditLineInfo* pli = m_clsLine.pli + nIndex;
  if ( nli > 0 ) {
    memmove( pli, pli + nCount, nli * sizeof( CPfxEditLineInfo ) );
  }
  // ���������һ����Ч��Ԫ�ǿ�ָ����ָʾ����˸�ָ�����Ŀյ�Ԫ�Ƿ�
  // Ϊ�㲢����Ҫ
  m_clsLine.nli -= nCount;
  m_clsLine.nliRemain += nCount;
  // ����������ж���PFX_EC_LINE_LINEINFO_ARRAY_GROWBY 125%���յ�Ԫʱ��Ҫ
  // ������ĳߴ��������������Լ����ڴ�ռ��
  if ( m_clsLine.nliRemain > ( PFX_EC_LINE_LINEINFO_ARRAY_GROWBY * 5 / 4 ) ) {
    LONG nNewSize = ( m_clsLine.nli + PFX_EC_LINE_LINEINFO_ARRAY_GROWBY );
    pli = new CPfxEditLineInfo[ nNewSize ];
    memmove( pli, m_clsLine.pli, m_clsLine.nli * sizeof( CPfxEditLineInfo ) );
    delete[] m_clsLine.pli;
    m_clsLine.pli = pli;
    m_clsLine.nliRemain = PFX_EC_LINE_LINEINFO_ARRAY_GROWBY;
  }
}
void CPfxEditDoc::TextInsertLine( LONG nIndex, LPCTSTR pLine, LONG nLen, DWORD dwFlags ) {
  ASSERT( nLen >= 0 );
  CPfxEditLineInfo* pli = TextInsertLine( nIndex, 1 );
  if ( nLen > 0 ) {
    ASSERT( pLine != NULL );
    LPTSTR pBuffer = NULL;
    CPfxEditDocMemBlock* pMemBlock = NULL;
    BufferLineGetSpace( nLen, &pBuffer, &pMemBlock );
    memcpy( pBuffer, pLine, nLen * sizeof( TCHAR ) );
    pli->dwFlags = dwFlags;
    pli->nActLen = pli->nLen = nLen;
    pli->pData = pBuffer;
    pli->pMemBlock = pMemBlock;
  } else {
    pli->dwFlags = dwFlags;
    pli->nActLen = pli->nLen = 0;
    pli->pData = NULL;
    pli->pMemBlock = NULL;
  }
  pli->nFirstChar = 0;
  pli->nIndex = PFX_EC_INVALID_LINE;
  pli->nSelFrom = pli->nSelTo = PFX_EC_INVALID_SELECT;
}
void CPfxEditDoc::TextRemoveLine( LONG nIndex ) {
  ASSERT( false );
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( nIndex ) ) );
  CPfxEditLineInfo* pli = TextGetLineAt( nIndex );
  CPfxEditDocMemBlock* pMemBlock;
  pMemBlock = static_cast<CPfxEditDocMemBlock*>( pli->pMemBlock );
  LONG nLen = pli->nActLen;
  TextRemoveLine( nIndex, 1 );
  BufferLineCompact( pMemBlock, nLen );
}
// ������λ�õ�������Ϣת��Ϊ���λ�õ�������Ϣ
void CPfxEditDoc::TextAbsLineToSubLine( CPoint& rpt ) const {
  if ( !IsValidChar( rpt.y, rpt.x ) ) {
    rpt.y = GetValidLine( rpt.y );
    rpt.x = GetValidChar( rpt.y, rpt.x );
  }
  TextAbsLineToMainLine( rpt );
  TextMainLineToSubLine( rpt );
}
void CPfxEditDoc::TextAbsLineToMainLine( CPoint& rpt ) const {
  if ( ( rpt.y < 0 ) || ( rpt.y >= GetLineCount() ) ) {
    rpt = CPoint( 0, 0 );
    return ;
  }
  if ( !IsWrapping() ) {
    return ;
  }
  // ����ʹ���۰���ҷ�������ָ����������
  for ( LONG nMin = rpt.y, nMax = TextGetLineCount(), nTarget = 0; /* empty */; /* empty */ ) {
    LONG nCur = nMin + ( nMax - nMin ) / 2;
    LONG nLine = TextGetThisMainLine( nCur );
    LONG nIndex = TextGetLineAt( nLine ) ->nIndex;
    // |---------------|-V--------------|
    if ( nIndex > rpt.y ) {
      nMax = nCur - 1;
    }
    // |-------------V-|----------------|
    else if ( nIndex < rpt.y ) {
      nMin = nCur + 1;
    } else {
      nTarget = nLine;
      break;
    }
  }
  rpt.y = nTarget;
}
LONG CPfxEditDoc::TextAbsLineToOffset( const CPoint& rpt ) const {
  CPoint pt = rpt;
  TextAbsLineToSubLine( pt );
  return TextSubLineToOffset( pt );
}
// ��������е������ת�����������е�����š������ǰ���Ǵ����Զ�����ģ
// ʽ����ʲôҲ����
// CPoint& pt - [����]ָ�������к�
//           [���]���������к�
void CPfxEditDoc::TextMainLineToAbsLine( CPoint& rpt ) const {
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( rpt.y ) ) );
  if ( IsWrapping() ) {
    rpt.y = TextGetLineAt( rpt.y ) ->nIndex;
  }
}
// ��������������׵�ƫ����ת��Ϊ��������е�ƫ���������磺��2����3����
// �У������г��ȷֱ�Ϊ10, 20, 15������������������е�ƫ������12����
// ��ֵ�ǣ���3��ƫ����2
// CPoint& rpt - X������������п�ʼ����ƫ������Y�������������
void CPfxEditDoc::TextMainLineToSubLine( CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( rpt.y ) ) );
  ASSERT( ( rpt.x >= 0 ) && ( rpt.x <= TextGetLineAt( rpt.y ) ->nActLen ) );
  if ( IsWrapping() ) {
    for ( LONG nLineUpperBound = TextGetLineUpperBound(); rpt.y < nLineUpperBound; rpt.y++ ) {
      const CPfxEditLineInfo* pli = TextGetLineAt( rpt.y );
      rpt.x -= pli->nLen;
      if ( rpt.x <= 0 ) {
        rpt.x += pli->nLen;
        break;
      }
    }
  }
}
// �����λ�õ�������Ϣת��Ϊ����λ�õ�������Ϣ
void CPfxEditDoc::TextSubLineToAbsLine( CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  ASSERT( TextIsValidChar( rpt.y, rpt.x ) );
  TextSubLineToMainLine( rpt );
  TextMainLineToAbsLine( rpt );
}
// �������λ��ת�����������λ��
void CPfxEditDoc::TextSubLineToMainLine( CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  ASSERT( TextIsValidChar( rpt.y, rpt.x ) );
  const CPfxEditLineInfo* pli = TextGetLineAt( rpt.y );
  if ( PFX_EC_IS_SUBLINE( pli ) ) {
    rpt.y -= pli->nActLen;
    rpt.x += pli->pData - TextGetLineAt( rpt.y ) ->pData;
  }
}
// ����ĳ�㵽�ĵ���ͷ��λ��
// CPoint& rpt - �õ㶼�ĵ���ͷ��λ�ã���Ե�����λ�ã�
// ���ظõ㵽�ĵ���ͷ��ƫ����
LONG CPfxEditDoc::TextSubLineToOffset( const CPoint& rpt ) const {
  if ( !TextIsValidChar( rpt.y, rpt.x ) ) {
    return 0;
  }
  for ( LONG nOffset = rpt.x, i = 0; i < rpt.y; i++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( i );
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      nOffset += ( pli->nLen + PFX_EC_VIRTUAL_LINE_BREAK_SIZE );
    } else {
      nOffset += pli->nLen;
    }
  }
  return nOffset;
}
// LONG nOffset - ���ĵ���ʼ��ƫ����
// CPoint& rpt  - ���ؼ�����
// ������ֵ��rpt��
void CPfxEditDoc::TextOffsetToSubLine( LONG nOffset, CPoint& rpt ) const {
  LONG nLineCount = TextGetLineCount();
  for ( rpt = CPoint( 0, 0 ); rpt.y < nLineCount; rpt.y++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( rpt.y );
    nOffset -= pli->nLen;
    if ( nOffset <= 0 ) {
      rpt.x = pli->nLen + nOffset;
      break;
    }
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      nOffset -= PFX_EC_VIRTUAL_LINE_BREAK_SIZE;
    }
    ASSERT( nOffset >= 0 );
  }
  if ( nOffset > 0 ) {
    rpt.y = TextGetLineUpperBound();
    rpt.x = TextGetLineLength( rpt.y );
  }
}
// ��ȡ��ǰ�����ڵ������к�
// LONG nLine - ��ǰ���кţ�������������Ҳ����������
// ���ص�ǰ������Ϊ��ֵ�������к�
LONG CPfxEditDoc::TextGetThisMainLine( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  ASSERT( ( nLine >= 0 ) && ( nLine < TextGetLineCount() ) );
  // �����������nActLen�Ǹ��е����е���ƫ���������������еĳ���
  const CPfxEditLineInfo* pli = TextGetLineAt( nLine );
  return ( !PFX_EC_IS_SUBLINE( pli ) ? nLine : ( nLine - pli->nActLen ) );
}
// �����ǰ���Ѿ��ǵ�һ��һ�����ڵ��з���PFX_EC_INVALID_LINE�����򷵻���һ���������
LONG CPfxEditDoc::TextGetPrevMainLine( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  const CPfxEditLineInfo* pli = TextGetLineAt( nLine );
  if ( PFX_EC_IS_SUBLINE( pli ) ) {
    // ����������������
    nLine -= pli->nActLen;
  }
  // ����nLineӦ��ָ��һ����
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( nLine ) ) );
  if ( nLine <= 0 ) {
    return PFX_EC_INVALID_LINE;
  }
  nLine--;
  pli = TextGetLineAt( nLine );
  return ( !PFX_EC_IS_SUBLINE( pli ) ? nLine : ( nLine - pli->nActLen ) );
}
// �����ǰ���Ѿ������һ�����ڵ��з���PFX_EC_INVALID_LINE�����򷵻���һ���������
LONG CPfxEditDoc::TextGetNextMainLine( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  LONG nLineCount = TextGetLineCount();
  for ( nLine++; nLine < nLineCount; nLine++ ) {
    if ( !PFX_EC_IS_SUBLINE( TextGetLineAt( nLine ) ) ) {
      return nLine;
    }
  }
  return PFX_EC_INVALID_LINE;
}
// ��ȡһ���а������к��������ڵ�������
// LONG nLine - ����ſ����������кţ�Ҳ�����������к�
// ���ص���ֵ�ǰ�����������
LONG CPfxEditDoc::TextGetLineSubLines( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  // ��������о�ת�������ڵ������к�
  if ( PFX_EC_IS_SUBLINE( TextGetLineAt( nLine ) ) ) {
    nLine = TextGetThisMainLine( nLine );
  }
  LONG nSubLines = 1; // ������һ����
  LONG nLineCount = TextGetLineCount();
  while ( ( ++nLine < nLineCount )
          && ( PFX_EC_IS_SUBLINE( TextGetLineAt( nLine ) ) ) ) {
    nSubLines++;
  }
  return nSubLines;
}
// ��ȡ��ĳ��ĳ�п�ʼ��ĳ��ĳ�н����Ŀ�Ĵ�С��������TextGetBlockData()
// ǰ����ʹ�ñ�����������Ҫ�Ļ�������С
// CPoint& rptFrom     - ��㣬��λ���������λ��
// CPoint& rptTo       - �յ㣬��λ���������λ��
// LONG nLineBreakType - ���з����������ͼ��㡣��ѡ�����У�
//                    PFX_EC_LINE_BREAK_TYPE_DOS
//                    PFX_EC_LINE_BREAK_TYPE_UNIX
//                    PFX_EC_LINE_BREAK_TYPE_MAC
LONG CPfxEditDoc::TextGetBlockSize( const CPoint& rptFrom, const CPoint& rptTo, LONG nLineBreakType ) {
  ASSERT( rptFrom.y <= rptTo.y );
  ASSERT( TextIsValidChar( rptFrom.y, rptFrom.x ) );
  ASSERT( TextIsValidChar( rptTo.y, rptTo.x ) );
  if ( rptFrom.y == rptTo.y ) {
    return rptTo.x - rptFrom.x;
  }
  LONG nLineBreakSize = GetLineBreakSizeFromType( nLineBreakType );
  // rptFrom.y��ȡrptFrom.x�Ժ󲿷��ַ�
  const CPfxEditLineInfo* pli = TextGetLineAt( rptFrom.y );
  LONG nSize = pli->nLen - rptFrom.x;
  if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
    nSize += nLineBreakSize;
  }
  // �м䲿��ȡ����
  for ( LONG i = rptFrom.y + 1; i < rptTo.y; i++ ) {
    pli = TextGetLineAt( i );
    nSize += ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ? ( pli->nLen + nLineBreakSize ) : pli->nLen );
  }
  // rptTo.y��ȡrptTo.x��ǰ�����ַ�
  nSize += rptTo.x;
  ASSERT( nSize > 0 );
  return nSize;
}
// ��ȡ��ĳ��ĳ�п�ʼ��ĳ��ĳ�н����Ŀ������
LONG CPfxEditDoc::TextGetBlockData( const CPoint& rptFrom, const CPoint& rptTo, LPTSTR pBuffer, LONG nBufMax, LONG nLineBreakType ) {
  ASSERT( rptFrom.y <= rptTo.y );
  ASSERT( TextIsValidChar( rptFrom.y, rptFrom.x ) );
  ASSERT( TextIsValidChar( rptTo.y, rptTo.x ) );
  LPTSTR pCur = pBuffer;
  CPoint ptFrom = rptFrom;
  CPoint ptTo = rptTo;
  // ���������Χ����ͬһ����
  if ( ptFrom.y == ptTo.y ) {
    ASSERT( ptFrom.x <= ptTo.x );
    LONG nLen = min( nBufMax, ( ptTo.x - ptFrom.x ) );
    if ( nLen > 0 ) {
      const CPfxEditLineInfo * pli = TextGetLineAt( ptFrom.y );
      memcpy( pCur, pli->pData + ptFrom.x, nLen * sizeof( TCHAR ) );
      pCur += nLen;
    }
  } else {
    LONG nLineBreakSize = GetLineBreakSizeFromType( nLineBreakType );
    LPCTSTR pLineBreakData = GetLineBreakDataFromType( nLineBreakType );
    // ���Ȼ�ȡ��һ��ptFrom.x������ַ�
    const CPfxEditLineInfo* pli = TextGetLineAt( ptFrom.y );
    LONG nLen = pli->nLen - ptFrom.x;
    if ( nLen > 0 ) {
      nLen = min( nBufMax, nLen );
      memcpy( pCur, pli->pData + ptFrom.x, nLen * sizeof( TCHAR ) );
      pCur += nLen;
      nBufMax -= nLen;
    }
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      nBufMax -= nLineBreakSize;
      if ( nBufMax >= 0 ) {
        memcpy( pCur, pLineBreakData, nLineBreakSize * sizeof( TCHAR ) );
        pCur += nLineBreakSize;
      }
    }
    // ��ȡ�м��е�����
    for ( ptFrom.y++; ( ptFrom.y < ptTo.y ) && ( nBufMax > 0 ); ptFrom.y++ ) {
      pli = TextGetLineAt( ptFrom.y );
      nLen = min( nBufMax, pli->nLen );
      if ( nLen > 0 ) {
        memcpy( pCur, pli->pData, ( nLen * sizeof( TCHAR ) ) );
        pCur += nLen;
        nBufMax -= nLen;
      }
      if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
        nBufMax -= nLineBreakSize;
        if ( nBufMax >= 0 ) {
          memcpy( pCur, pLineBreakData, nLineBreakSize * sizeof( TCHAR ) );
          pCur += nLineBreakSize;
        }
      }
    }
    // ��ȡ����е�����
    if ( ( nBufMax > 0 ) && ( ptTo.x > 0 ) ) {
      nLen = min( nBufMax, ptTo.x );
      memcpy( pCur, TextGetLineAt( ptTo.y ) ->pData, nLen * sizeof( TCHAR ) );
      pCur += nLen;
    }
  }
  return ( pCur - pBuffer );
}
void CPfxEditDoc::TextUpdateDocSize( LONG cchChanged ) {
  m_nDocSize += ( cchChanged * sizeof( TCHAR ) );
  ASSERT( m_nDocSize >= 0 );
  SetModifiedFlag( true );
}
void CPfxEditDoc::TextUpdateLineIndex( LONG nFromLine, LONG nBaseIndex ) {
  ASSERT( ( nFromLine >= 0 ) && ( nFromLine < TextGetLineCount() ) );
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( nFromLine ) ) );
  ASSERT( nBaseIndex <= nFromLine );
  if ( !IsWrapping() ) {
    ASSERT( nBaseIndex == -1 );
    // ������������
    m_nDocLineCount = TextGetLineCount();
  } else {
    ASSERT( ( nBaseIndex >= 0 ) && ( nBaseIndex < TextGetLineCount() ) );
    CPfxEditLineInfo* pli = TextGetLineAt( nFromLine );
    CPfxEditLineInfo* pliEnd = TextGetLineAt( TextGetLineUpperBound() );
    for ( ; pli <= pliEnd; pli++ ) {
      if ( !PFX_EC_IS_SUBLINE( pli ) ) {
        pli->nIndex = nBaseIndex++;
      }
    }
    // ������������
    m_nDocLineCount = nBaseIndex;
  }
  ASSERT( m_nDocLineCount >= 1 );
}
// һ���б��޸����Ժ������¼�������������Ϣ������������һָ����������
// ������ڸ����п�ͷ��ĳһƫ������ֵ�����µĵ�ǰλ�ã��������Ӧ���ַ�
// λ�ã������Ұ�һ������������Ϣд�ص�����Ϣ����
// CPfxEditLineInfo* pliHead - �洢���Ѿ�����õ�����Ϣ�Ļ������������׵�
//                          ַָ�뱻������nFirstModifyLine����Ϣ��ָ��
// LONG nFirstModifyLine     - �����޸ĵĵ�һ�е�����ţ��޸Ĵ���һ�п�ʼ
// LONG nModifiedLines       - ������������б��޸��ˣ��޸ĺ���nAdjustedLines��
// LONG nAdjustedLines       - �μ�LONG ModifiedLines�Ľ���
// CPoint& rptCurPos         - ���룺�µ����к�����ƫ����λ��
//                          ���أ����ؼ���õ��µ�ǰλ�ã�����λ�ã�
// CPfxEditLineInfo* pliHead���������Ӧ�ñ�CPfxEditView::WrapLineSingle()
// ��WrapLineMultiple()���������
void CPfxEditDoc::TextUpdateLineInfo( CPfxEditLineInfo* pliHead, LONG nFirstModifiedLine, LONG nModifiedLines, LONG nAdjustedLines, CPoint& rptCurPos ) {
  ASSERT( rptCurPos.y >= nFirstModifiedLine );
  ASSERT( rptCurPos.x >= 0 );
  ASSERT( ( nFirstModifiedLine + nAdjustedLines ) > rptCurPos.y );
  CPfxEditLineInfo* pli = ( pliHead + ( rptCurPos.y - nFirstModifiedLine ) );
  LPCTSTR lp = pli->pData + rptCurPos.x;
  rptCurPos.x = 0;
  for ( LONG nLastModifyLine = nFirstModifiedLine + nAdjustedLines; rptCurPos.y < nLastModifyLine; ++rptCurPos.y, ++pli ) {
    if ( lp <= ( pli->pData + pli->nLen ) ) {
      rptCurPos.x = lp - pli->pData;
      break;
    }
  }
  LONG nLines = nAdjustedLines - nModifiedLines;
  if ( nLines > 0 )           //      �޸ĺ����������޸�ǰ����������һЩ��
  {
    // ����������п��ܲ���Ҫ�ƶ���������
    TextInsertLine( ( nFirstModifiedLine + nModifiedLines ), nLines );
  } else if ( nLines < 0 )           //      ����ɾ��һЩ��
  {
    TextRemoveLine( nLastModifyLine, -nLines );
  }
  memcpy( TextGetLineAt( nFirstModifiedLine ), pliHead, nAdjustedLines * sizeof( CPfxEditLineInfo ) );
  // �������޸����ĵ������һ�У���һ��Ҫ������һ�е�PFX_EC_LF_HARDRETURN
  // ��־����Ϊ���һ�п϶�û�иñ�־
  if ( TextGetLineCount() == nLastModifyLine ) {
    TextGetLineAt( TextGetLineUpperBound() ) ->dwFlags &= ~PFX_EC_LF_HARDRETURN;
  }
}
void CPfxEditDoc::TextConstructEmptyLineInfo( CPfxEditLineInfo* pli, LONG nCount ) const {
  ASSERT( nCount > 0 );
  for ( LONG i = 0; i < nCount; i++, pli++ ) {
    pli->dwFlags = PFX_EC_LF_HARDRETURN;
    pli->nIndex = PFX_EC_INVALID_LINE;
    pli->nFirstChar = 0;
    pli->nActLen = pli->nLen = 0;
    pli->nSelFrom = pli->nSelTo = PFX_EC_INVALID_SELECT;
    pli->pData = NULL;
    pli->pMemBlock = NULL;
  }
}
// ��ȡһ�����������һ�е�����
// CPfxEditLineInfo* pli - ָ��һ����Ϣ�ṹ���ýṹӦ����TextGetLineAt()
//                      �����ص�
// LONG nNewLen          - �µĻ��������ȣ�����ֵ��ȡ[0->��Ҫ�ĳ���]
// 1���������л�����ѹ�����ܣ��������գ�����μ�BufferLineCompact()
// 2�����ڵ��ñ���������ܵ���ִ���������ղ�������һ��ִ�����������ղ���
// �����е������ݻ����������ܻ�ı䣬����һ�����ñ�����������ؼ�ס
// ���µ���TextGetLineAt()������ȡĳ�е��µ�����Ϣ�����磺
// void AppendTextToLine(LONG nLineIndex, LPCTSTR pTextToAppend)
// {
//       CPfxEditLineInfo* pli = TextGetLineAt(TextGetThisMainLine(nLineIndex));
//       CPfxEditLineInfo* pli2 = TextGetLineAt(0); // ʾ���ã�
//       LONG nLenOfTextToAppend = lstrlen(pTextToAppend);
//       LONG nOldLen = TextGetLineLength(nLineIndex);
//       LONG nNewLen = nOldLen + nLenOfTextToAppend;
//       ���ñ�������ȡ��Ҫ�Ļ�����
//       TextGetBuffer(pli, nNewLen);
////   ��סһ��Ҫˢ�������ѻ�ȡ��CPfxEditLineInfoָ��
//       pli = TextGetLineAt(nLineIndex);
//       pli2 = TextGetLineAt(0);
//       LPTSTR pData = pli->pData;
//       LPTSTR pInsertPoint = pData + pli->nActLen;
//       memcpy(pInsertPoint, pTextToAppend, nLenOfTextToAppend * sizeof(TCHAR));
// }
// ��������ֻ��ʾ����ʵ����Ҫ�����ı�Ӧ�õ���TextInsertChars()��Ҫɾ��
// �ı�Ӧ��ʹ��TextRemoveChars()
void CPfxEditDoc::TextGetBuffer( CPfxEditLineInfo* pli, LONG nNewLen ) {
  ASSERT( ( pli >= TextGetLineAt( 0 ) ) && ( pli <= TextGetLineAt( TextGetLineUpperBound() ) ) );
  ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
  ASSERT( nNewLen >= 0 );
  LPTSTR pBuffer;
  CPfxEditDocMemBlock* pNewMemBlock;
  CPfxEditDocMemBlock* pCurMemBlock;
  LONG nOldLen = pli->nActLen;
  if ( nOldLen == nNewLen ) {
    return ;
  }
  // ԭ���ǿ��У����������¿ռ�
  if ( 0 == nOldLen ) {
    pBuffer = NULL;
    pNewMemBlock = NULL;
    if ( nNewLen > 0 ) {
      BufferLineGetSpace( nNewLen, &pBuffer, &pNewMemBlock );
    }
    pli->nActLen = pli->nLen = nNewLen;
    pli->pData = pBuffer;
    pli->pMemBlock = pNewMemBlock;
    return ;
  }
  pCurMemBlock = static_cast<CPfxEditDocMemBlock*>( pli->pMemBlock );
  ASSERT( pCurMemBlock != NULL );
  if ( 0 == nNewLen ) {
    pli->nActLen = pli->nLen = 0;
    pli->pData = NULL;
    pli->pMemBlock = NULL;
    BufferLineCompact( pCurMemBlock, nOldLen );
    return ;
  }
  // ԭ�������Ǹÿ��ڴ������һ�У�������Ҫ�����ߴ磬����������Զ�̬��
  // �����ڴ��Ŀ�������ָ�����ﵽĿ�ģ���ʹ��������Ч��
  if ( ( ( pli->pData + nOldLen ) == pCurMemBlock->pRemain )
       && ( ( pCurMemBlock->nRemain + nOldLen ) >= nNewLen ) ) {
    ASSERT( nOldLen > 0 );
    ASSERT( pli->pData != NULL );
    pli->nActLen = pli->nLen = nNewLen;
    nOldLen -= nNewLen;
    pCurMemBlock->nRemain += nOldLen;
    pCurMemBlock->pRemain -= nOldLen;
    return ;
  } else {
    // �����Ҫ����С��������ֻ��Ҫ���������ĳߴ粢����ԭ���Ļ�
    // ��Ϳ���
    if ( nNewLen < nOldLen ) {
      nOldLen -= nNewLen;
      pli->nActLen = pli->nLen = nNewLen;
    } else {
      BufferLineGetSpace( nNewLen, &pBuffer, &pNewMemBlock );
      memcpy( pBuffer, pli->pData, ( nOldLen * sizeof( TCHAR ) ) );
      pli->nActLen = pli->nLen = nNewLen;
      pli->pData = pBuffer;
      pli->pMemBlock = pNewMemBlock;
    }
    BufferLineCompact( pCurMemBlock, nOldLen );
    return ;
  }
}
// ��ָ��λ�ò����ı������ı����ݿ��԰�����������з�����ֻ����DOS�͵�
// ���س�/���С���\r\n��
// CPoint& rpt         - �ڸ�λ�ò����ı��������������λ��
// LPCTSTR pData       - Ҫ������ı�ָ��
// LONG cch            - Ҫ������ı��ĳ���
// CPfxEditView* pView - ���ĸ���ͼ��ִ�б�����������ò���ΪNULL����
//                    ����ˢ�²���������Ҫ���Ժ�ִ��һ��ȫ��ˢ�¡���
//                    ��Ҫ���һ��������ʱָ��ΪNULL���Ի�ø��ߵ�ִ
//                    ���ٶ�
void CPfxEditDoc::TextInsertChars( CPoint& rpt, LPCTSTR pData, LONG cch, CPfxEditView* pView ) {
  ASSERT( cch > 0 );
  ASSERT( TextIsValidChar( rpt.y, rpt.x ) );
  CPfxEditHeap Heap;
  LONG cchChanged;
  LONG nAdjustedLines;
  LONG nModifiedLines;
  CPoint ptNewPos;
  // �����ж�Ҫ������ַ��а����ж�����
  LPCTSTR pCur = pData;
  LPCTSTR pEnd = pCur + cch;
  LONG nLinesToInsert = 1;
  while ( pCur < pEnd ) {
    // Ҫ������ַ��л��з�ֻ����DOS�͵�CRLF����'\r\n'
    if ( ( PFX_EC_CHR_CR == *pCur++ )
         && ( pCur < pEnd )
         && ( PFX_EC_CHR_LF == *pCur ) ) {
      nLinesToInsert++;
      pCur++;
    }
  }
  CPoint ptCurPos = rpt;
  TextSubLineToMainLine( ptCurPos );
  nModifiedLines = TextGetLineSubLines( ptCurPos.y );
  // ���Ҫ������ַ���û�л��з�������ͼ򵥵ö�
  if ( nLinesToInsert == 1 ) {
    CPfxEditLineInfo * pli = TextGetLineAt( ptCurPos.y );
    LONG cchOldLine = pli->nActLen;
    LONG cchNewLine = cchOldLine + cch;
    TextGetBuffer( pli, cchNewLine );
    // �ƶ�����������ַ����ڳ��ռ��Ų�����ַ�
    LPCTSTR pStart = pli->pData;
    pCur = pStart + ptCurPos.x;
    if ( cchOldLine > ptCurPos.x ) {
      memmove( const_cast<LPTSTR>( ( pCur + cch ) ), pCur, ( cchOldLine - ptCurPos.x ) * sizeof( TCHAR ) );
    }
    // ���������ַ�
    memcpy( const_cast<LPTSTR>( pCur ), pData, ( cch * sizeof( TCHAR ) ) );
    pCur += cch;
    // ���µ�������Ϣ
    nAdjustedLines = m_arrViews[ 0 ] ->WrapLineSingle( Heap, pli );
    LONG nFirstModifiedLine = ptCurPos.y;
    ptNewPos.x = pCur - pStart;
    ptNewPos.y = ptCurPos.y;
    TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), nFirstModifiedLine, nModifiedLines, nAdjustedLines, ptNewPos );
    cchChanged = cch;
  }
  // ���Ҫ������ַ������ж��У���Ҫ���������У���һ���ҳ�Ҫ�������
  // ����Ȼ����������Ϣ���з����㹻������Ϣ�ռ䣬��Ϊÿһ�з��仺������
  // �ڶ�����Ϊ�Զ����ж������²������
  else {
    LONG nModifiedLinesBaseIndex = TextGetLineAt( ptCurPos.y ) ->nIndex;
    nModifiedLines += nLinesToInsert;
    CPfxEditLineInfo* pli = TextInsertLine( ptCurPos.y, nLinesToInsert );
    TextConstructEmptyLineInfo( pli, nLinesToInsert );
    LONG nOldMainLine = ptCurPos.y + nLinesToInsert;
    CPfxEditLineInfo* pliOld = TextGetLineAt( nOldMainLine );
    LONG cchOldLine = pliOld->nActLen;
    // |<-�޸��в����ǰ��������->|<-Ҫ������ַ�-|<-�޸��в����󲿷�����->|
    // |                          |               |                          |
    // |abc123abc123abc123abc123ab|abc123\r\nabc12|abc123abc123abc123abc123ab|
    // |                          |    |     |    |                          |
    // |<-�µĵ�һ��--------------|    |<-|->|    |<-�µ������------------->|
    //                                   |
    //                               �µ��м���
    // ���Ƚ�ԭ���в�����ǰ�벿��������ַ���һ�����з�ǰ���
    // �ַ��ϲ�������ɵ�һ��
    LPCTSTR pStart = pCur = pData;
    pEnd = pStart + cch;
    while ( pCur < pEnd ) {
      if ( ( PFX_EC_CHR_CR == *pCur++ )
           && ( pCur < pEnd )
           && ( PFX_EC_CHR_LF == *pCur ) ) {
        pCur--;
        break;
      }
    }
    pCur = min( pCur, pEnd );
    // ��һ�еĳ���
    LONG nLen1 = ptCurPos.x;
    LONG nLen2 = pCur - pStart;
    LONG cchNewLine = nLen1 + nLen2;
    LONG nLineIndex = ptCurPos.y;
    pli = TextGetLineAt( nLineIndex );
    if ( cchNewLine > 0 ) {
      TextGetBuffer( pli, cchNewLine );
    }
    // ����ǰ�벿���ַ�
    if ( nLen1 > 0 ) {
      memcpy( pli->pData, pliOld->pData, ( nLen1 * sizeof( TCHAR ) ) );
    }
    if ( nLen2 > 0 ) {
      memcpy( ( pli->pData + nLen1 ), pStart, ( nLen2 * sizeof( TCHAR ) ) );
    }
    // ���Ƶ�һ�к����һ���е��м��е����ݣ���Щ�����ڲ������
    // ���ж���һ�����з����ָ�
    pCur += PFX_EC_LINE_BREAK_SIZE_DOS;
    pStart = pCur;
    while ( pCur < pEnd ) {
      if ( ( PFX_EC_CHR_CR == *pCur++ )
           && ( pCur < pEnd )
           && ( PFX_EC_CHR_LF == *pCur ) ) {
        nLineIndex++;
        cchNewLine = ( pCur - pStart - 1 );
        if ( cchNewLine > 0 ) {
          pli = TextGetLineAt( nLineIndex );
          TextGetBuffer( pli, cchNewLine );
          memcpy( pli->pData, pStart, cchNewLine * sizeof( TCHAR ) );
        }
        pStart = ++pCur;
      }
    }
    pCur = min( pCur, pEnd );
    pStart = min( pStart, pEnd );
    // ���һ�е�����
    nLen1 = pCur - pStart;
    nLen2 = cchOldLine - ptCurPos.x;
    cchNewLine = nLen1 + nLen2;
    nLineIndex++;
    pli = TextGetLineAt( nLineIndex );
    if ( cchNewLine > 0 ) {
      TextGetBuffer( pli, cchNewLine );
    }
    if ( nLen1 > 0 ) {
      memcpy( pli->pData, pStart, ( nLen1 * sizeof( TCHAR ) ) );
    }
    if ( nLen2 > 0 ) {
      memcpy( pli->pData + nLen1, pliOld->pData + ptCurPos.x, nLen2 * sizeof( TCHAR ) );
    }
    // �ͷű��޸���ԭ���Ļ�����
    TextGetBuffer( pliOld, 0 );
    // ���µ������޸��˵�����Ϣ
    nLineIndex = nLinesToInsert - 1;
    pli = TextGetLineAt( ptCurPos.y );
    // �ָ���һ�����޸��е�ԭ���ı�־����ǩ...�ȣ�
    pli->dwFlags = pliOld->dwFlags | PFX_EC_LF_HARDRETURN;
    nAdjustedLines = m_arrViews[ 0 ] ->WrapLineMultiple( Heap, pli, nLinesToInsert, nLineIndex );
    LONG nFirstModifiedLine = ptCurPos.y;
    ptNewPos.x = nLen1;
    ptNewPos.y = ptCurPos.y + nLineIndex;
    TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), nFirstModifiedLine, nModifiedLines, nAdjustedLines, ptNewPos );
    TextUpdateLineIndex( ptCurPos.y, nModifiedLinesBaseIndex );
    cchChanged = cch - ( nLinesToInsert * ( PFX_EC_LINE_BREAK_SIZE_DOS - GetLineBreakSize() ) );
  }
  if ( pView != NULL ) {
    DWORD dwFlags;
    LONG nFirstUpdateLine;
    LONG nLastUpdateLine;
    if ( nModifiedLines != nAdjustedLines ) {
      dwFlags = PFX_EC_UT_VERT;
      nFirstUpdateLine = ptCurPos.y;
      nLastUpdateLine = -1; // ����ֱ�����ڵױ�
    } else {
      dwFlags = PFX_EC_UT_HORZ;
      nFirstUpdateLine = ptCurPos.y;
      nLastUpdateLine = ptCurPos.y + nAdjustedLines - 1;
    }
    // ����޸ĺ�ǰ����е�����û�иı䣬ֻ�ǵ�ǰ�༭�иı��ˣ�
    // ��ô����ͼ����ı��˵��ַ��Ŀ�ȣ������Ǵӱ༭�е����׿�ʼ����
    LONG xTextPos;
    if ( ( rpt.y == ptNewPos.y )
         && ( rpt.x < ptNewPos.x )
         && ( rpt.x == pView->m_ptEditPos.x )
         && ( rpt.y == pView->m_ptEditPos.y ) ) {
      pCur = TextGetLineAt( ptNewPos.y ) ->pData + rpt.x;
      xTextPos = pView->m_ptTextPos.x + pView->CalcStringWidth( pCur, ptNewPos.x - rpt.x, pView->m_ptLogiPos.x, NULL );
    } else {
      xTextPos = pView->TransCharPosToTextPos( ptNewPos );
    }
    pView->m_ptEditPos = ptNewPos;
    pView->UpdateAllViews( dwFlags, nFirstUpdateLine, nLastUpdateLine );
    pView->EnsureVisible( pView->m_ptEditPos.y, xTextPos );
    pView->UpdateCurrentPosition( xTextPos, true, true );
    // UpdateAllViews()���ܻ�����OnSize()�����ã�������Ϊ���ĵ�
    // �������仯�п��ܵ��´�ֱ����������ʾ�����ػ������ر���ʾ��
    // �⽫�ᵼ����ͼ�Ŀͻ����Ŀ����仯���������OnSize()����
    // �á������������������ҵ�ǰ���ڸ��ݴ��ڿ�ȶ��Զ����е�
    // ģʽʱ���Զ����к�ptNewPos���ܻ��Ϊ��Чλ�ã���
    // pView->m_ptEditPos���ɴ����ն������ٵģ�Ҳ��ʹ˵����ֵһ
    // ֱ������ȷ�ģ�����Ҫ������ֵ����������ptNewPos��ֵ
    ptNewPos = pView->m_ptEditPos;
  }
  TextUpdateDocSize( + cchChanged );
  rpt = ptNewPos; // ������ֵ
  ASSERT( TextIsValidLine( ptNewPos.y ) );
  ASSERT( TextIsValidChar( ptNewPos.y, ptNewPos.x ) );
}
// ɾ����ָ����rptFrom��rptTo֮����ı�
// CPoint& rptFrom     - ����㿪ʼɾ���������������λ��
// CPoint& rptTo       - ɾ������һ�㣬�����������λ��
// CPfxEditView* pView - ���ĸ���ͼ(view)��ִ��ɾ������ΪҪˢ�¸���ͼ
void CPfxEditDoc::TextRemoveChars( CPoint& rptFrom, CPoint& rptTo, CPfxEditView* pView ) {
  ASSERT( rptFrom.y <= rptTo.y );
  ASSERT( TextIsValidChar( rptFrom.y, rptFrom.x ) );
  ASSERT( TextIsValidChar( rptTo.y, rptTo.x ) );
  CPfxEditHeap Heap;
  LONG cchChanged;
  LONG nModifiedLines;
  LONG nAdjustedLines;
  CPoint ptNewPos;
  // ת��Ϊ���к�����ƫ����
  CPoint ptMainFirst = rptFrom;
  TextSubLineToMainLine( ptMainFirst );
  CPoint ptMainLast = rptTo;
  TextSubLineToMainLine( ptMainLast );
  if ( ptMainLast.y != ptMainFirst.y ) {
    LONG nLineBreakSize = GetLineBreakSize();
    // �����е�һ�е����һ���й��ж����лᱻ�޸�
    nModifiedLines = ( ptMainLast.y + TextGetLineSubLines( ptMainLast.y ) ) - ptMainFirst.y;
    ASSERT( nModifiedLines > 0 );
    // ���������ͷһ�к����һ�е�δɾ�����ֺϲ��������м��н�
    // ��ɾ�������е��б�־(dwFlags)����ȡԭͷһ�еı�־
    CPfxEditLineInfo* pliMainFirst = TextGetLineAt( ptMainFirst.y );
    CPfxEditLineInfo* pliMainLast = TextGetLineAt( ptMainLast.y );
    LONG nModifiedLinesBaseIndex = pliMainFirst->nIndex;
    LONG cchMainFirst = pliMainFirst->nActLen;
    LONG cchMainLast = pliMainLast->nActLen;
    LONG cchNewLine = ptMainFirst.x + ( cchMainLast - ptMainLast.x );
    // ���·����һ�еĻ�����
    TextGetBuffer( pliMainFirst, cchNewLine );
    // ������е�δ��ɾ�����ֽӵ���һ��δ��ɾ�����ֵ�β��
    if ( !PFX_EC_IS_EMPTY_LINE( pliMainFirst )
         && !PFX_EC_IS_EMPTY_LINE( pliMainLast ) ) {
      memcpy( pliMainFirst->pData + ptMainFirst.x, pliMainLast->pData + ptMainLast.x, ( cchMainLast - ptMainLast.x ) * sizeof( TCHAR ) );
    }
    // ������ݺϲ������濪ʼɾ����һ�����°������һ�����ڵ���
    // ���У������ͷ�������ʹ�õĻ�����
    cchChanged = cchMainFirst;
    for ( LONG i = TextGetNextMainLine( ptMainFirst.y ); ( i != PFX_EC_INVALID_LINE ) && ( i <= ptMainLast.y ); i = TextGetNextMainLine( i ) ) {
      CPfxEditLineInfo * pli = TextGetLineAt( i );
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      cchChanged += ( pli->nActLen + nLineBreakSize );
      if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        TextGetBuffer( pli, 0 );
      }
    }
    // ���µ����ϲ�����У�ɾ�����������Ϣ
    nAdjustedLines = m_arrViews[ 0 ] ->WrapLineSingle( Heap, pliMainFirst );
    LONG nFirstModifiedLine = ptMainFirst.y;
    ptNewPos = ptMainFirst;
    TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), ptMainFirst.y, nModifiedLines, nAdjustedLines, ptNewPos );
    TextUpdateLineIndex( ptMainFirst.y, nModifiedLinesBaseIndex );
    // ��һ�β�����ɾ���˵��ַ���
    cchChanged -= cchNewLine;
  } else // ɾ����������һ���н��У��Լ򵥷�������
  {
    nModifiedLines = TextGetLineSubLines( ptMainFirst.y );
    CPfxEditLineInfo* pliMainFirst = TextGetLineAt( ptMainFirst.y );
    LONG cchMainFirst = pliMainFirst->nActLen;
    cchChanged = ptMainLast.x - ptMainFirst.x;
    ASSERT( cchChanged >= 0 );
    LONG cchNewLine = cchMainFirst - cchChanged;
    LONG cchLastPart = cchMainFirst - ptMainLast.x;
    if ( cchLastPart > 0 ) {
      memmove( pliMainFirst->pData + ptMainFirst.x, pliMainFirst->pData + ptMainLast.x, cchLastPart * sizeof( TCHAR ) );
    }
    // ע�������κε����л������Ĳ����󣬶�����ˢ��ָ��û�����
    // ��ָ�룬��Ϊ��������ַ�����Ѿ��ı�
    TextGetBuffer( pliMainFirst, cchNewLine );
    // ���µ����ϲ�����У�ɾ�����������Ϣ
    nAdjustedLines = m_arrViews[ 0 ] ->WrapLineSingle( Heap, pliMainFirst );
    LONG nFirstModifiedLine = ptMainFirst.y;
    ptNewPos = ptMainFirst;
    TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), ptMainFirst.y, nModifiedLines, nAdjustedLines, ptNewPos );
  }
  if ( pView != NULL ) {
    DWORD dwFlags;
    LONG nFirstUpdateLine;
    LONG nLastUpdateLine;
    if ( ( nModifiedLines != nAdjustedLines )
         || ( ptMainFirst.y != ptMainLast.y ) ) {
      dwFlags = PFX_EC_UT_VERT;
      nFirstUpdateLine = ptMainFirst.y;
      nLastUpdateLine = -1; // ����ֱ�����ڵױ�
    } else {
      dwFlags = PFX_EC_UT_HORZ;
      nFirstUpdateLine = ptMainFirst.y;
      nLastUpdateLine = ptMainFirst.y + nAdjustedLines - 1;
    }
    if ( ptNewPos != pView->m_ptEditPos ) {
      pView->m_ptEditPos = ptNewPos;
      pView->UpdateAllViews( dwFlags, nFirstUpdateLine, nLastUpdateLine );
      pView->m_ptTextPos.x = pView->TransCharPosToTextPos( pView->m_ptEditPos );
      pView->EnsureVisible( pView->m_ptEditPos.y, pView->m_ptTextPos.x );
      pView->UpdateCurrentPosition( pView->m_ptTextPos.x, true, true );
    } else {
      pView->m_ptEditPos = ptNewPos;
      pView->UpdateAllViews( dwFlags, nFirstUpdateLine, nLastUpdateLine );
      pView->EnsureVisible( pView->m_ptEditPos.y, pView->m_ptTextPos.x );
      pView->UpdateCurrentPosition( pView->m_ptTextPos.x, false, true );
    }
    // UpdateAllViews()���ܻ�����OnSize()�����ã�������Ϊ���ĵ�
    // �������仯�п��ܵ��´�ֱ����������ʾ�����ػ������ر���ʾ��
    // �⽫�ᵼ����ͼ�Ŀͻ����Ŀ����仯���������OnSize()����
    // �á������������������ҵ�ǰ���ڸ��ݴ��ڿ�ȶ��Զ����е�
    // ģʽʱ���Զ����к�ptNewPos���ܻ��Ϊ��Чλ�ã���
    // pView->m_ptEditPos���ɴ����ն������ٵģ�Ҳ��ʹ˵����ֵһ
    // ֱ������ȷ�ģ�����Ҫ������ֵ����������ptNewPos��ֵ
    ptNewPos = pView->m_ptEditPos;
  }
  // ���ݼ����ˣ������Ǹ���
  TextUpdateDocSize( -cchChanged );
  rptFrom = ptNewPos; // ������ֵ
  ASSERT( TextIsValidLine( ptNewPos.y ) );
  ASSERT( TextIsValidChar( ptNewPos.y, ptNewPos.x ) );
}
// ΪUNDO��¼�ṩ������
// CPfxEditUndoInfo* pui - ָ��һ����Ϣ�ṹ���ýṹӦ����UndoGetInfo()
//                      �����ص�
// LONG nLen             - �µĻ��������ȣ�����ֵ��ȡ[0->��Ҫ�ĳ���]
// ��Ҫ��ʾ����ο�TextGetBuffer()
void CPfxEditDoc::UndoGetBuffer( CPfxEditUndoInfo* pui, LONG nLen ) {
  ASSERT( pui != NULL );
  LONG i;
  LPTSTR pBuffer;
  CPfxEditDocMemBlock* pNewMemBlock;
  CPfxEditDocMemBlock* pCurMemBlock;
  // ���ԭ��û�����뻺����
  if ( PFX_EC_IS_EMPTY_UNDO( pui ) ) {
    pBuffer = NULL;
    pNewMemBlock = NULL;
    if ( nLen > 0 ) {
      BufferUndoGetSpace( nLen, &pBuffer, &pNewMemBlock );
    }
    pui->nLen = nLen;
    pui->pData = pBuffer;
    pui->pMemBlock = pNewMemBlock;
    return ;
  }
  i = pui->nLen;
  pCurMemBlock = static_cast<CPfxEditDocMemBlock*>( pui->pMemBlock );
  ASSERT( pCurMemBlock != NULL );
  // ԭ�������뻺��������������NULL������
  if ( nLen == 0 ) {
    pui->nLen = 0;
    pui->pData = NULL;
    pui->pMemBlock = NULL;
    BufferUndoCompact( pCurMemBlock, i );
    return ;
  }
  // ԭ���Ļ������Ǹÿ��ڴ������һ������ȥ�Ŀ飬������Ҫ�����ߴ磬
  // ����������Զ�̬�������ڴ��Ŀ�������ָ�����ﵽĿ�ģ���ʹ������
  // ��Ч��
  if ( ( ( pui->pData + i ) == pCurMemBlock->pRemain )
       && ( ( pCurMemBlock->nRemain + i ) >= nLen ) ) {
    ASSERT( i > 0 );
    ASSERT( pui->pData != NULL );
    pui->nLen = nLen;
    i -= nLen;
    pCurMemBlock->nRemain += i;
    pCurMemBlock->pRemain -= i;
    return ;
  } else {
    BufferUndoGetSpace( nLen, &pBuffer, &pNewMemBlock );
    memcpy( pBuffer, pui->pData, ( min( nLen, i ) * sizeof( TCHAR ) ) );
    pui->nLen = nLen;
    pui->pData = pBuffer;
    pui->pMemBlock = pNewMemBlock;
    BufferUndoCompact( pCurMemBlock, i );
    return ;
  }
}
void CPfxEditDoc::UndoInsert( CPfxEditUndoInfo* pui, CPoint& rptRetPos, BOOL bRedo ) {
  ASSERT( pui != NULL );
  ASSERT( PFX_EC_IS_EMPTY_UNDO( pui ) );
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  ASSERT( pui->ptTo.y != PFX_EC_INVALID_LINE );
  // ԭ���ǲ��������UNDO��ɾ��������REDO�ǲ������
  CPoint ptFrom = pui->ptFrom;
  TextAbsLineToMainLine( ptFrom );
  TextMainLineToSubLine( ptFrom );
  CPoint ptTo = pui->ptTo;
  TextAbsLineToMainLine( ptTo );
  TextMainLineToSubLine( ptTo );
  // ɾ��֮ǰ����Ҫɾ�������ݣ��Ա�REDOʱʹ��
  UndoGetBuffer( pui, TextGetBlockSize( ptFrom, ptTo, PFX_EC_LINE_BREAK_TYPE_DOS ) );
  ASSERT( !PFX_EC_IS_EMPTY_UNDO( pui ) );
  TextGetBlockData( ptFrom, ptTo, pui->pData, pui->nLen, PFX_EC_LINE_BREAK_TYPE_DOS );
  TextRemoveChars( ptFrom, ptTo, NULL );
  rptRetPos = ptFrom;
  m_clsUndo.nPosCur += ( !bRedo ? -1 : + 1 );
  ASSERT( m_clsUndo.nPosCur >= 0 );
  if ( m_clsUndo.nPosCur == m_clsUndo.nPosUnmodified ) {
    SetModifiedFlag( false );
  }
}
void CPfxEditDoc::UndoRemove( CPfxEditUndoInfo* pui, CPoint& rptRetPos, BOOL bRedo ) {
  ASSERT( pui != NULL );
  ASSERT( !PFX_EC_IS_EMPTY_UNDO( pui ) );
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  // ԭ����ɾ��������UNDO�ǲ��������REDO��ɾ������
  CPoint pt = pui->ptFrom;
  TextAbsLineToMainLine( pt );
  TextMainLineToSubLine( pt );
  TextInsertChars( pt, pui->pData, pui->nLen, NULL );
  rptRetPos = pt;
  TextSubLineToMainLine( pt );
  pui->ptTo = pt;
  // �ͷ�UNDO������
  UndoGetBuffer( pui, 0 );
  ASSERT( PFX_EC_IS_EMPTY_UNDO( pui ) );
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  ASSERT( pui->ptTo.y != PFX_EC_INVALID_LINE );
  m_clsUndo.nPosCur += ( !bRedo ? -1 : + 1 );
  ASSERT( m_clsUndo.nPosCur >= 0 );
  if ( m_clsUndo.nPosCur == m_clsUndo.nPosUnmodified ) {
    SetModifiedFlag( false );
  }
}
// ���ַ�������������ñ�����
// CPoint ptFrom - ��ʼλ�ã�X���ڲ�������ĵ�һ���ַ�λ�ã�����ֵ�����
//              �������ף�Y���ڲ�������ĵ�һ���кţ�����һ��Ҫ������
// CPoint ptTo   - X���ڱ������һ���ַ���λ�ã���ֵ�������ptTo.y���ԣ�
//              Y�������һ���кţ����п��������У�Ҳ����������
void CPfxEditDoc::UndoAddInsert( CPoint ptFrom, CPoint ptTo ) {
  ASSERT( ( ptFrom.y >= 0 ) && ( ptFrom.y < TextGetLineCount() ) );
  ASSERT( !PFX_EC_IS_SUBLINE( TextGetLineAt( ptFrom.y ) ) );
  ASSERT( ( ptFrom.x >= 0 ) && ( ptFrom.x <= TextGetLineAt( ptFrom.y ) ->nActLen ) );
  ASSERT( TextIsValidChar( ptTo.y, ptTo.x ) );
  TextMainLineToAbsLine( ptFrom );
  TextSubLineToMainLine( ptTo );
  TextMainLineToAbsLine( ptTo );
  // �����һ���������ļ����ַ����ͽ���ϲ�Ϊһ��UNDO��¼
  if ( ( m_clsUndo.nGroupName == PFX_EC_UNDO_TYPING ) && ( ptFrom.y == ptTo.y ) ) {
    CPfxEditUndoInfo * pui = UndoGetInfo();
    if ( ( pui != NULL )
         && ( pui->fUndoName == m_clsUndo.nGroupName )
         && ( pui->ptFrom.y == ptFrom.y )
         && ( pui->ptFrom.y == pui->ptTo.y )
         && ( pui->ptTo.x == ptFrom.x ) ) {
      pui->ptTo.x = ptTo.x;
      return ;
    }
  }
  CPfxEditUndoInfo* pui = UndoAddRecord();
  pui->fAction = UndoGetFlags( PFX_EC_UF_INSERT );
  pui->fUndoName = m_clsUndo.nGroupName;
  pui->ptFrom = ptFrom;
  pui->ptTo = ptTo;
  pui->nLen = 0;
  pui->pData = NULL;
  pui->pMemBlock = NULL;
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  ASSERT( pui->ptTo.y != PFX_EC_INVALID_LINE );
}
// ��ɾ���ַ�ǰ�����ñ�����������UNDO��Ϣ
// CPoint ptFrom - ��������λ��
// CPoint ptTo   - �յ������λ��
void CPfxEditDoc::UndoAddRemove( CPoint ptFrom, CPoint ptTo ) {
  ASSERT( ptFrom.y <= ptTo.y );
  ASSERT( TextIsValidChar( ptFrom.y, ptFrom.x ) );
  ASSERT( TextIsValidChar( ptTo.y, ptTo.x ) );
  CPfxEditUndoInfo* pui = UndoAddRecord();
  ASSERT( pui != NULL );
  UndoGetBuffer( pui, TextGetBlockSize( ptFrom, ptTo, PFX_EC_LINE_BREAK_TYPE_DOS ) );
  ASSERT( !PFX_EC_IS_EMPTY_UNDO( pui ) );
  TextGetBlockData( ptFrom, ptTo, pui->pData, pui->nLen, PFX_EC_LINE_BREAK_TYPE_DOS );
  TextSubLineToMainLine( ptFrom );
  TextMainLineToAbsLine( ptFrom );
  TextSubLineToMainLine( ptTo );
  TextMainLineToAbsLine( ptTo );
  pui->fAction = UndoGetFlags( PFX_EC_UF_REMOVE );
  pui->fUndoName = m_clsUndo.nGroupName;
  pui->ptFrom = ptFrom;
  pui->ptTo = ptTo;
  ASSERT( pui->ptFrom.y != PFX_EC_INVALID_LINE );
  ASSERT( pui->ptTo.y != PFX_EC_INVALID_LINE );
}
// ÿ��Ҫ���һ���µ�UNDO��¼ʱ���ñ�����������һ��¼�ռ�
CPfxEditUndoInfo* CPfxEditDoc::UndoAddRecord() {
  // �Ƿ���REDO��Ϣ������о�ɾ������REDO��Ϣ�����ͷ����еĻ�����
  if ( m_clsUndo.nPosCur < m_clsUndo.nui ) {
    // m_clsUndo.nPosCur��ָ��ľ������һ��REDO��Ϣ
    LONG nCurPos = m_clsUndo.nPosCur;
    CPfxEditUndoInfo* pui = m_clsUndo.pui + nCurPos;
    for ( ; nCurPos < m_clsUndo.nui; nCurPos++, pui++ ) {
      if ( !PFX_EC_IS_EMPTY_UNDO( pui ) ) {
        BufferUndoCompact( static_cast<CPfxEditDocMemBlock*>( pui->pMemBlock ), pui->nLen );
      }
    }
    m_clsUndo.nuiRemain += ( m_clsUndo.nui - m_clsUndo.nPosCur );
    m_clsUndo.nui = m_clsUndo.nPosCur;
  }
  // ����ռ䲻���Ҫ���µ�������ߴ�
  if ( m_clsUndo.nuiRemain == 0 ) {
    // �����ǰ��û�дﵽ���UNDO����������UNDO����������
    if ( m_clsUndo.nui < m_clsUndo.nuiMax ) {
      LONG nGrowBy = min( m_clsUndo.nuiGrowBy, ( m_clsUndo.nuiMax - m_clsUndo.nui ) );
      LONG nNewLen = m_clsUndo.nui + nGrowBy;
      CPfxEditUndoInfo* pui = new CPfxEditUndoInfo[ nNewLen ];
      if ( m_clsUndo.pui != NULL ) {
        memcpy( pui, m_clsUndo.pui, m_clsUndo.nui * sizeof( CPfxEditUndoInfo ) );
        delete[] m_clsUndo.pui;
      }
      m_clsUndo.pui = pui;
      m_clsUndo.nuiRemain = nNewLen - m_clsUndo.nui;
    }
    // ����UNDO�����Ѿ��޷�����������ķ�����ɾ�������UNDO��¼
    // ����������һ������������ڳ��ռ������µļ�¼
    else {
      LONG nDelCount = m_clsUndo.nui / 2;
      CPfxEditUndoInfo* pui = m_clsUndo.pui;
      CPfxEditUndoInfo* puiEnd = m_clsUndo.pui + nDelCount;
      for ( ; pui < puiEnd; pui++ ) {
        if ( !PFX_EC_IS_EMPTY_UNDO( pui ) ) {
          BufferUndoCompact( static_cast<CPfxEditDocMemBlock*>( pui->pMemBlock ), pui->nLen );
        }
      }
      pui->fAction |= PFX_EC_UF_BEGIN_GROUP;
      m_clsUndo.nui -= nDelCount;
      m_clsUndo.nuiRemain += nDelCount;
      m_clsUndo.nPosCur = m_clsUndo.nui;
      m_clsUndo.nPosUnmodified -= nDelCount;
      memmove( m_clsUndo.pui, pui, m_clsUndo.nui * sizeof( CPfxEditUndoInfo ) );
    }
  }
  CPfxEditUndoInfo* pui = m_clsUndo.pui + m_clsUndo.nPosCur;
  m_clsUndo.nui++;
  m_clsUndo.nuiRemain--;
  m_clsUndo.nPosCur++;
  pui->fAction = PFX_EC_UF_UNKNOWN;
  pui->ptFrom.x = pui->ptTo.x = PFX_EC_INVALID_CHAR;
  pui->ptFrom.y = pui->ptTo.y = PFX_EC_INVALID_LINE;
  pui->nLen = 0;
  pui->pData = NULL;
  pui->pMemBlock = NULL;
  return pui;
}
// �����UNDO������Ҫ���Ϊ��ʱ���ñ���������ɲ�����Ҳ������ñ�����
// BOOL bBegin - TRUE���ڱ�ʶһ��Ŀ�ʼ��FALSE��ʾ����һ�����
void CPfxEditDoc::UndoSetGroup( BOOL bBegin, WORD fUndoName ) {
  if ( bBegin ) {
    // ��ֹ��һ��GROUP����֮ǰ�ֿ�ʼBEGIN_GROUP
    if ( m_clsUndo.nGroupLockCount == 0 ) {
      m_clsUndo.bGroupBegin = true;
      m_clsUndo.nGroupName = fUndoName;
    }
    m_clsUndo.nGroupLockCount++;
  } else {
    ASSERT( m_clsUndo.nGroupLockCount > 0 );
    if ( m_clsUndo.nGroupLockCount == 1 ) {
      m_clsUndo.bGroupBegin = false;
      m_clsUndo.nGroupName = PFX_EC_UNDO_UNKNOWN;
    }
    m_clsUndo.nGroupLockCount--;
  }
}
WORD CPfxEditDoc::UndoGetFlags( WORD wDefaultFlags ) {
  if ( m_clsUndo.nGroupLockCount > 0 ) {
    wDefaultFlags |= PFX_EC_UF_GROUP;
  }
  if ( m_clsUndo.bGroupBegin ) {
    wDefaultFlags |= PFX_EC_UF_BEGIN_GROUP;
    m_clsUndo.bGroupBegin = false;
  }
  return wDefaultFlags;
}
CPfxEditUndoInfo* CPfxEditDoc::UndoGetInfo() const {
  if ( UndoGetCount() <= 0 ) {
    return NULL;
  }
  return ( m_clsUndo.pui + ( m_clsUndo.nPosCur - 1 ) );
}
CPfxEditUndoInfo* CPfxEditDoc::UndoGetRedoInfo() const {
  if ( UndoGetRedoCount() <= 0 ) {
    return NULL;
  }
  return ( m_clsUndo.pui + m_clsUndo.nPosCur );
}
BOOL CPfxEditDoc::FileIsAnsi( LPCSTR pData, LONG cch ) const {
  UNUSED( pData );
  UNUSED( cch );
  return true; // ���Ƿ���TRUE
}
BOOL CPfxEditDoc::FileIsUtf8( LPCSTR pData, LONG cch ) const {
  if ( ( pData == NULL ) || ( cch <= 0 ) ) {
    return false;
  }
  // ���ǰ�����ķ�����⣺
  // 1. �ҵ�һ���з�ASCII�ַ����н��м��
  //  a) ��������зǷ��ַ�������0���Ϳ϶�����UTF8����
  //  b) �����еĵ�һ���ַ���ʼ���룬�ж�ÿ���ַ��ĳ��Ȳ��ƶ���дָ
  // �룬������ն�дָ���뻻�з���λ���غϾͱ���������ȷ��UTF8
  // ����
  // 2. �������ļ����ݶ���ASCII�ַ�ʱ��������ANSI����Ҳ��UTF8���룬��
  //  ���������Ļ������ǾͰ�������ANSI����
  LPCSTR pCur = pData;
  const LPCSTR pEnd = pData + cch;
  LPCSTR pTargetLine = NULL;
  while ( ( pCur < pEnd ) && ( pTargetLine == NULL ) ) {
    // ɨ�軻�з����ڵ�ַ
    // ע����UTF8��"\r", "\n"����ĳ�����ֽ��ַ���һ����
    LPCSTR pLineStart = pCur;
    while ( pCur < pEnd ) {
      CHAR c = *pCur++;
      if ( !__isascii( c ) || ( c == 0 ) )           //      Ŀ�����ҵ���
      {
        pTargetLine = pLineStart;
        break;
      }
      if ( ( PFX_EC_CHR_CR == c ) || ( PFX_EC_CHR_LF == c ) ) {
        if ( ( PFX_EC_CHR_CR == c )           //      MSDOS�͵Ļ��з�
             && ( pCur < pEnd )
             && ( PFX_EC_CHR_LF == *pCur ) ) {
          pCur++; // Խ��PFX_EC_CHR_LF
        }
        break;
      }
    }
  }
  if ( pTargetLine == NULL )           //      ��������ַ�����ASCII�ַ��͵�ANSI����
  {
    return false;
  }
  // �����Ѿ��ҵ�һ�����з�ASCII�ַ����У�����ȷ�����е���ĩλ��
  const LPCSTR pLineStart = pTargetLine;
  LPCSTR pLineEnd = pEnd;
  for ( pCur = pLineStart; pCur < pEnd; pCur++ ) {
    if ( ( PFX_EC_CHR_CR == *pCur ) || ( PFX_EC_CHR_LF == *pCur ) ) {
      pLineEnd = pCur;
      break;
    }
  }
  CPfxEditDocUtf8Converter u8;
  return u8.IsUtf8FromUnicode( pLineStart, ( pLineEnd - pLineStart ) );
}
BOOL CPfxEditDoc::FileIsUnicode( LPCSTR pData, LONG cch ) const {
  // 1. ����ļ�Ϊ�գ����������κ����͵��ĵ�
  // 2. ����ļ������ݲ��ܱ�sizeof(WCHAR)��������Ҳ����UNICODE�ļ�
  if ( ( pData == NULL )
       || ( cch < sizeof( WCHAR ) )
       || ( cch % sizeof( WCHAR ) != 0 ) ) {
    return false;
  }
  LONG cchUnicode = cch / sizeof( WCHAR );
  LPCWSTR pUnicode = reinterpret_cast<LPCWSTR>( pData );
  // ����ļ���ͷ��UNICODE�ļ��ı�־����ȥ�ñ�־
  if ( *pUnicode != PFX_EC_UNICODE_FILE_SIGNATURE ) {
    return false;
  } else {
    pUnicode++;
    cchUnicode--;
  }
  // ���������ַ��к�����ЧUNICODE�ַ���������Ч
  LPCWSTR pStart = pUnicode;
  LPCWSTR pEnd = pStart + cchUnicode;
  while ( ( pStart < pEnd )
          && ( *pStart != PFX_EC_UNICODE_FILE_SIGNATURE )
          && ( *pStart != 0 ) ) {
    pStart++;
  }
  return ( pStart >= pEnd );
}
BOOL CPfxEditDoc::FileIsUnicodeBigEndian( LPCSTR pData, LONG cch ) const {
  // 1. ����ļ�Ϊ�գ����������κ����͵��ĵ�
  // 2. ����ļ������ݲ��ܱ�sizeof(WCHAR)��������Ҳ����UNICODE�ļ�
  if ( ( pData == NULL )
       || ( cch < sizeof( WCHAR ) )
       || ( cch % sizeof( WCHAR ) != 0 ) ) {
    return false;
  }
  LONG cchUnicode = cch / sizeof( WCHAR );
  LPCWSTR pUnicode = reinterpret_cast<LPCWSTR>( pData );
  // ����ļ���ͷ��UNICODE�ļ��ı�־����ȥ�ñ�־
  if ( *pUnicode != PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE ) {
    return false;
  } else {
    pUnicode++;
    cchUnicode--;
  }
  // ���������ַ��к�����ЧUNICODE�ַ���������Ч
  LPCWSTR pStart = pUnicode;
  LPCWSTR pEnd = pStart + cchUnicode;
  while ( ( pStart < pEnd )
          && ( *pStart != PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE )
          && ( *pStart != 0 ) ) {
    pStart++;
  }
  return ( pStart >= pEnd );
}
LONG CPfxEditDoc::FileGetConverterType( LPCSTR pData, LONG cch ) const {
  if ( FileIsUnicode( pData, cch ) ) {
    return PFX_EC_FILE_CONVERTER_UNICODE;
  }
  if ( FileIsUtf8( pData, cch ) ) {
    return PFX_EC_FILE_CONVERTER_UTF8;
  }
  if ( FileIsUnicodeBigEndian( pData, cch ) ) {
    return PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN;
  } else {
    return PFX_EC_FILE_CONVERTER_ANSI;
  }
}
void CPfxEditDoc::FileLoadAnsi( CPfxEditDocFile& file ) {
  LONG cch = file.GetSize();
  LPCTSTR pData = static_cast<LPCTSTR>( file.GetBuffer() );
  if ( ( pData == NULL ) || ( cch < 0 ) ) {
    return ;
  }
  LPCTSTR pStart = pData;
  LPCTSTR pCur = pData;
  const LPCTSTR pEnd = pData + cch;
  LONG nDocSize = 0;
  LONG nLineIndex = 0;
  LONG nLineBreakTypeDos = 0;
  LONG nLineBreakTypeMac = 0;
  LONG nLineBreakTypeUnix = 0;
  m_nDocSize = 0;
  m_nDocLineCount = 1;
  m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
  m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_DOS;
  while ( pCur < pEnd ) {
    TCHAR c = *pCur++;
    if ( ( PFX_EC_CHR_CR != c ) && ( PFX_EC_CHR_LF != c ) ) {
      continue;
    }
    // PFX_EC_LINE_BREAK_TYPE_DOS
    if ( ( PFX_EC_CHR_CR == c ) && ( pCur < pEnd ) && ( PFX_EC_CHR_LF == *pCur ) ) {
      pCur++; // Խ��PFX_EC_CHR_LF
      cch = ( pCur - pStart );
      nDocSize += cch;
      cch -= PFX_EC_LINE_BREAK_SIZE_DOS;
      nLineBreakTypeDos++;
    }
    // PFX_EC_LINE_BREAK_TYPE_UNIX
    else if ( PFX_EC_CHR_LF == c ) {
      cch = ( pCur - pStart );
      nDocSize += cch;
      cch -= PFX_EC_LINE_BREAK_SIZE_UNIX;
      nLineBreakTypeUnix++;
    }
    // PFX_EC_LINE_BREAK_TYPE_MAC
    else {
      cch = ( pCur - pStart );
      nDocSize += cch;
      cch -= PFX_EC_LINE_BREAK_SIZE_MAC;
      nLineBreakTypeMac++;
    }
    TextInsertLine( nLineIndex, pStart, cch, PFX_EC_LF_NONE | PFX_EC_LF_HARDRETURN );
    nLineIndex++;
    pStart = pCur;
  }
  cch = pEnd - pStart;
  TextInsertLine( nLineIndex, pStart, cch, PFX_EC_LF_NONE );
  m_nDocSize = nDocSize + cch;
  m_nDocLineCount = nLineIndex + 1;
  // �ɸ������з��Ķ����������ĵ��������������ͣ���ͬ�������
  // ���£���DOS�ͻ��з�����
  if ( ( nLineBreakTypeUnix > nLineBreakTypeDos )
       && ( nLineBreakTypeUnix >= nLineBreakTypeMac ) ) {
    m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_UNIX;
    m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_UNIX;
  } else if ( ( nLineBreakTypeMac > nLineBreakTypeDos ) && ( nLineBreakTypeMac >= nLineBreakTypeUnix ) ) {
    m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_MAC;
    m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_MAC;
  } else {
    m_nLineBreakSize = PFX_EC_LINE_BREAK_SIZE_DOS;
    m_nLineBreakType = PFX_EC_LINE_BREAK_TYPE_DOS;
  }
}
void CPfxEditDoc::FileLoadUtf8( CPfxEditDocFile& file ) {
  LONG cchUtf8 = file.GetSize();
  LPCSTR pUtf8 = static_cast<LPCSTR>( file.GetBuffer() );
  // ���Ƚ�UTF8����ת����UNICODE��������Ҫ�������Ž�����������Ҫ��
  // ����������
  CPfxEditDocUtf8Converter u8;
  LONG cchUnicode = u8.Utf8ToUnicode( pUtf8, cchUtf8, NULL, 0 );
  if ( cchUnicode == 0 ) {
    EventSendMsg( PFX_EE_CONVERT_ERR_ON_LOADING, 0 );
    return ;
  }
  CPfxEditDocFile file2;
  if ( !file2.MapMemory( cchUnicode * sizeof( WCHAR ) ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  // ִ��ת������
  cchUnicode = static_cast<LONG>( u8.Utf8ToUnicode( pUtf8, cchUtf8, static_cast<LPWSTR>( file2.GetBuffer() ), cchUnicode ) );
  if ( cchUnicode == 0 ) {
    EventSendMsg( PFX_EE_CONVERT_ERR_ON_LOADING, 0 );
  } else {
    file.Close();
    file2.SetSize( cchUnicode * sizeof( WCHAR ) );
    // �����Ҫ��Ҫ��BIG ENDIAN��ʽת����LITTLE ENDIAN��ʽ
    if ( PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE
         == ( *static_cast<LPCWSTR>( file2.GetBuffer() ) ) ) {
      LPWSTR pCur = static_cast<LPWSTR>( file2.GetBuffer() );
      const LPWSTR pEnd = pCur + cchUnicode;
      while ( pCur < pEnd ) {
        WCHAR ch = *pCur;
        *pCur++ = PFX_EC_UBE_TO_ULE( ch );
      }
    }
    FileLoadUnicode( file2 );
  }
}
void CPfxEditDoc::FileLoadUnicode( CPfxEditDocFile& file ) {
  // �ض̵�WCHAR��λ
  LONG cchUnicode = ( file.GetSize() / sizeof( WCHAR ) );
  LPCWSTR pUnicode = static_cast<LPCWSTR>( file.GetBuffer() );
  // ����ļ���ͷ��UNICODE�ļ��ı�־����ȥ�ñ�־
  if ( ( cchUnicode > 0 )
       && ( *pUnicode == PFX_EC_UNICODE_FILE_SIGNATURE ) ) {
    pUnicode++;
    cchUnicode--;
  }
  if ( cchUnicode <= 0 )           //      �����㲢����ʾ��������ʾ���Ǹ����ļ�
  {
    return ;
  }
  // ���㽫�����ļ�ת����MBCS�ַ�����Ҫ�Ļ������ռ�
  LONG cchMultiByte = static_cast<LONG>( ::WideCharToMultiByte( CP_ACP, 0, pUnicode, static_cast<int>( cchUnicode ), NULL, 0, NULL, NULL ) );
  if ( cchMultiByte == 0 )           //      ����
  {
    EventSendMsg( PFX_EE_CONVERT_ERR_ON_LOADING, 0 );
    return ;
  }
  CPfxEditDocFile file2; // �������ڴ������뻺����
  if ( !file2.MapMemory( cchMultiByte ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  cchMultiByte = static_cast<LONG>( ::WideCharToMultiByte( CP_ACP, 0, pUnicode, static_cast<int>( cchUnicode ), static_cast<LPSTR>( file2.GetBuffer() ), cchMultiByte, NULL, NULL ) );
  if ( cchMultiByte == 0 ) {
    EventSendMsg( PFX_EE_CONVERT_ERR_ON_LOADING, 0 );
  } else {
    file.Close(); // ���ȹر�ԭ����ӳ���Խ�ʡ�ڴ�ռ�
    file2.SetSize( cchMultiByte );
    FileLoadAnsi( file2 );
  }
}
void CPfxEditDoc::FileLoadUnicodeBigEndian( CPfxEditDocFile& file ) {
  // ����ԭ����ӳ����ֻ���ģ����Ա��봴��һ��д�Ļ����������ļ�����ת
  // ����LITTLE ENDIAN��ʽ
  CPfxEditDocFile file2;
  if ( !file2.MapMemory( file.GetSize() ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  LPCWSTR pSrc = static_cast<LPCWSTR>( file.GetBuffer() );
  LPWSTR pDst = static_cast<LPWSTR>( file2.GetBuffer() );
  // �ض̵�WCHAR��λ
  const LPWSTR pEnd = pDst + ( file2.GetSize() / sizeof( WCHAR ) );
  // ��BIG ENDIAN��ʽת����LITTLE ENDIAN��ʽ
  while ( pDst < pEnd ) {
    WCHAR ch = *pSrc++;
    *pDst++ = PFX_EC_UBE_TO_ULE( ch );
  }
  file.Close(); // ���ȹر�ԭ����ӳ���Խ�ʡ�ڴ�ռ�
  FileLoadUnicode( file2 );
}
void CPfxEditDoc::FileSaveAnsi( CArchive& ar ) {
  // д���ļ�ʱ��Ӧ��ʹ��ӳ���ļ����ƣ���Ϊʹ��ӳ���ļ�����д��Զ����
  // ��ʱ����ȫ��ϵͳ�޷���֤�ļ�����������ͬ��
  LONG nLineUpperBound = TextGetLineUpperBound();
  LONG nLineBreakSize = GetLineBreakSizeFromTypeA( m_nLineBreakType );
  LPCSTR pLineBreakData = GetLineBreakDataFromTypeA( m_nLineBreakType );
  for ( LONG nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      ar.Write( pli->pData, pli->nActLen );
    }
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
    if ( nLineIndex < nLineUpperBound ) {
      ar.Write( pLineBreakData, nLineBreakSize );
    }
  }
}
void CPfxEditDoc::FileSaveUtf8( CArchive& ar ) {
  // �ҳ�ת���ɿ��ַ���ʽʱ��Ҫ����󻺳������ȣ�����Ƶ���ı仺����
  LONG nMaxBufferSizeUnicode = 0;
  LONG nLineUpperBound = TextGetLineUpperBound();
  for ( LONG nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, NULL, 0 ) );
      if ( cch == 0 ) {
        EventSendMsg( PFX_EE_CONVERT_ERR_ON_SAVING, 0 );
        return ;
      }
      nMaxBufferSizeUnicode = max( nMaxBufferSizeUnicode, cch );
    }
    // ��������
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
  }
  // д��UNICODE�ļ���־
  CHAR szUtf8[ 16 ];
  WORD wSignature = PFX_EC_UNICODE_FILE_SIGNATURE;
  CPfxEditDocUtf8Converter u8;
  ar.Write( szUtf8, u8.UnicodeToUtf8( ( LPCWSTR ) & wSignature, 1, szUtf8, sizeof( szUtf8 ) ) );
  if ( nMaxBufferSizeUnicode == 0 )           //      �ĵ�Ϊ�գ��ⲻ�Ǵ���
  {
    return ;
  }
  // ����ÿ��UNICODE�ַ�ת����UTF8ʱ��Ҫ���Ļ���������3���ֽڣ�����
  // ���������Ҫ����󻺳�������Ϊ��
  LONG nMaxBufferSizeUtf8 = nMaxBufferSizeUnicode * 3;
  // ���뻺��������ת������
  CPfxEditDocFile file;
  if ( !file.MapMemory( ( nMaxBufferSizeUnicode * sizeof( WCHAR ) ) + nMaxBufferSizeUtf8 + 16 ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  CHAR szLineBreakData[ 32 ];
  LPWSTR pUnicode = static_cast<LPWSTR>( file.GetBuffer() );
  LPSTR pUtf8 = reinterpret_cast<LPSTR>( pUnicode + nMaxBufferSizeUnicode );
  LPCSTR pLineBreakData = szLineBreakData;
  UINT nLineBreakSize = u8.UnicodeToUtf8( GetLineBreakDataFromTypeW( m_nLineBreakType ), GetLineBreakSizeFromTypeW( m_nLineBreakType ), szLineBreakData, sizeof( szLineBreakData ) );
  // ��ʼת��
  for ( nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      UINT cchUnicode = static_cast<UINT>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, pUnicode, nMaxBufferSizeUnicode ) );
      if ( cchUnicode != 0 ) {
        UINT cchUtf8 = u8.UnicodeToUtf8( pUnicode, cchUnicode, pUtf8, nMaxBufferSizeUtf8 );
        ar.Write( pUtf8, cchUtf8 );
      }
    }
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
    if ( nLineIndex < nLineUpperBound ) {
      ar.Write( pLineBreakData, nLineBreakSize );
    }
  }
}
void CPfxEditDoc::FileSaveUnicode( CArchive& ar ) {
  // �ҳ�ת���ɿ��ַ���ʽʱ��Ҫ����󻺳������ȣ�����Ƶ���ı仺����
  LONG nMaxBufferSize = 0;
  LONG nLineUpperBound = TextGetLineUpperBound();
  for ( LONG nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, NULL, 0 ) );
      if ( cch == 0 ) {
        EventSendMsg( PFX_EE_CONVERT_ERR_ON_SAVING, 0 );
        return ;
      }
      nMaxBufferSize = max( nMaxBufferSize, cch );
    }
    // ��������
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
  }
  // д��UNICODE�ļ���־
  WCHAR wSignature = PFX_EC_UNICODE_FILE_SIGNATURE;
  ar.Write( &wSignature, sizeof( wSignature ) );
  if ( nMaxBufferSize == 0 )           //      �ĵ�Ϊ�գ��ⲻ�Ǵ���
  {
    return ;
  }
  // ���뻺��������ת������
  CPfxEditDocFile file;
  if ( !file.MapMemory( nMaxBufferSize * sizeof( WCHAR ) ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  LPWSTR pBuffer = static_cast<LPWSTR>( file.GetBuffer() );
  LONG nLineBreakSize = GetLineBreakSizeFromTypeW( m_nLineBreakType );
  LPCWSTR pLineBreakData = GetLineBreakDataFromTypeW( m_nLineBreakType );
  // ��ʼת��
  for ( nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, pBuffer, nMaxBufferSize ) );
      if ( cch != 0 ) {
        ar.Write( pBuffer, cch * sizeof( WCHAR ) );
      }
    }
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
    if ( nLineIndex < nLineUpperBound ) {
      ar.Write( pLineBreakData, nLineBreakSize * sizeof( WCHAR ) );
    }
  }
}
void CPfxEditDoc::FileSaveUnicodeBigEndian( CArchive& ar ) {
  // �ҳ�ת���ɿ��ַ���ʽʱ��Ҫ����󻺳������ȣ�����Ƶ���ı仺����
  LONG nMaxBufferSize = 0;
  LONG nLineUpperBound = TextGetLineUpperBound();
  for ( LONG nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, NULL, 0 ) );
      if ( cch == 0 ) {
        EventSendMsg( PFX_EE_CONVERT_ERR_ON_SAVING, 0 );
        return ;
      }
      nMaxBufferSize = max( nMaxBufferSize, cch );
    }
    // ��������
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
  }
  // д��UNICODE BIG ENDIAN�ļ���־
  WCHAR wSignature = PFX_EC_UNICODE_FILE_BIG_ENDIAN_SIGNATURE;
  ar.Write( &wSignature, sizeof( wSignature ) );
  if ( nMaxBufferSize == 0 )           //      �ĵ�Ϊ�գ��ⲻ�Ǵ���
  {
    return ;
  }
  // ���뻺��������ת������
  CPfxEditDocFile file;
  if ( !file.MapMemory( nMaxBufferSize * sizeof( WCHAR ) ) ) {
    EventSendMsg( PFX_EE_OUT_OF_MEMORY, 0 );
    return ;
  }
  WCHAR szLineBreakData[ 16 ];
  LPWSTR pBuffer = static_cast<LPWSTR>( file.GetBuffer() );
  LONG nLineBreakSize = GetLineBreakSizeFromTypeW( m_nLineBreakType );
  LPCWSTR pLineBreakData = szLineBreakData;
  memcpy( szLineBreakData, GetLineBreakDataFromTypeW( m_nLineBreakType ), nLineBreakSize * sizeof( WCHAR ) );
  // ��ת�ֽ�˳��
  for ( LONG i = 0; i < nLineBreakSize; i++ ) {
    WCHAR w = szLineBreakData[ i ];
    szLineBreakData[ i ] = PFX_EC_ULE_TO_UBE( w );
  }
  // ��ʼת��
  for ( nLineIndex = 0; nLineIndex <= nLineUpperBound; nLineIndex++ ) {
    const CPfxEditLineInfo* pli = TextGetLineAt( nLineIndex );
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
      LONG cch = static_cast<LONG>( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pli->pData, pli->nActLen, pBuffer, nMaxBufferSize ) );
      if ( cch != 0 ) {
        //  ��ת�ֽ�˳��
        for ( LPWSTR s1 = pBuffer, s2 = pBuffer + cch; s1 < s2; s1++ ) {
          WCHAR w = *s1;
          *s1 = PFX_EC_ULE_TO_UBE( w );
        }
        ar.Write( pBuffer, cch * sizeof( WCHAR ) );
      }
    }
    if ( !PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      while ( ( ++nLineIndex < nLineUpperBound ) && !PFX_EC_IS_DRAW_HARDRETURN( TextGetLineAt( nLineIndex ) ) ) {
        //  do nothing
      }
    }
    if ( nLineIndex < nLineUpperBound ) {
      ar.Write( pLineBreakData, nLineBreakSize * sizeof( WCHAR ) );
    }
  }
}
CPfxEditView::CPfxEditViewClipboard::CPfxEditViewClipboard( CControlUI* pWndOwner, UINT uFormat, BOOL bGet ) {
  m_hCBData = NULL;
  m_bOpened = false;
  m_bForGet = bGet;
  m_uFormat = uFormat;
  m_hWndOwner = pWndOwner;
  ASSERT( m_hWndOwner );
}
CPfxEditView::CPfxEditViewClipboard::~CPfxEditViewClipboard() {
  Close();
}
BOOL CPfxEditView::CPfxEditViewClipboard::Attach( HGLOBAL hGlobal ) {
  if ( ( hGlobal != NULL )
       && ( ::GlobalFlags( hGlobal ) != GMEM_INVALID_HANDLE ) ) {
    Close();
    m_hCBData = hGlobal;
  }
  return ( hGlobal == m_hCBData );
}
HGLOBAL CPfxEditView::CPfxEditViewClipboard::Detach() {
  HGLOBAL hGlobal = m_hCBData;
  m_hCBData = NULL;
  if ( hGlobal != NULL ) {
    UnLock( hGlobal );
  }
  return hGlobal;
}
BOOL CPfxEditView::CPfxEditViewClipboard::Open() {
  if ( !m_bOpened ) {
    m_bOpened = ::OpenClipboard( m_hWndOwner->M_HWND );
  }
  return m_bOpened;
}
void CPfxEditView::CPfxEditViewClipboard::Close() {
  // ��������ڻ�ȡ����������
  if ( m_bForGet ) {
    if ( m_hCBData != NULL ) {
      UnLock( m_hCBData );
      m_hCBData = NULL;
    }
  }
  // ����������������ü���������
  else {
    if ( m_hCBData != NULL ) {
      VERIFY( ::GlobalFree( m_hCBData ) == NULL );
      m_hCBData = NULL;
    }
  }
  if ( m_bOpened ) {
    m_bOpened = !::CloseClipboard();
  }
  ASSERT( !m_bOpened );
  ASSERT( m_hCBData == NULL );
}
BOOL CPfxEditView::CPfxEditViewClipboard::UnLock( HGLOBAL hGlobal ) {
  UINT uFlags = ::GlobalFlags( hGlobal );
  if ( uFlags == GMEM_INVALID_HANDLE ) {
    return false;
  }
  // ����ݱ��Ѿ���������������Ҫ����
  for ( UINT uLockCount = ( uFlags & GMEM_LOCKCOUNT ); uLockCount > 0; uLockCount-- ) {
    ::GlobalUnlock( hGlobal );
  }
  return true;
}
LPCTSTR CPfxEditView::CPfxEditViewClipboard::GetClipboardData() {
  ASSERT( m_bForGet );
  ASSERT( m_hCBData == NULL );
  LPCTSTR pData = NULL;
  if ( Open() ) {
    m_hCBData = ::GetClipboardData( m_uFormat );
    if ( m_hCBData != NULL ) {
      pData = Lock( m_hCBData );
    }
  }
  return pData;
}
LPTSTR CPfxEditView::CPfxEditViewClipboard::GetBuffer( LONG nBufLen, BOOL bAppend ) {
  ASSERT( !m_bForGet );
  ASSERT( m_hCBData == NULL );
  LPTSTR pBuffer = NULL;
  if ( !bAppend )           //      ׷�Ӹ���
  {
    // ����һ������������������еĺ��µļ���������
    m_hCBData = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, ( ( nBufLen + 1 ) * sizeof( TCHAR ) ) );
    // �������еļ���������
    if ( m_hCBData != NULL ) {
      pBuffer = Lock( m_hCBData );
    }
  }
  else {
    // �򿪼����壬��ѯ���ݳ��Ȳ���ȡ����
    HANDLE hData = NULL;
    LONG nDataLen = 0;
    LPCTSTR pData = NULL;
    if ( Open() ) {
      hData = ::GetClipboardData( m_uFormat );
      if ( hData != NULL ) {
        pData = Lock( hData );
      }
      if ( pData != NULL ) {
        nDataLen = _tcslen( pData );
      }
    }
    // ����һ������������������еĺ��µļ���������
    m_hCBData = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, ( ( nBufLen + nDataLen + 1 ) * sizeof( TCHAR ) ) );
    // �������еļ���������
    if ( m_hCBData != NULL ) {
      pBuffer = Lock( m_hCBData );
      if ( ( pBuffer != NULL ) && ( pData != NULL ) ) {
        memcpy( pBuffer, pData, nDataLen * sizeof( TCHAR ) );
        pBuffer += nDataLen;
      }
    }
    // �򿪵ļ�������ڶ���������ʱ��ر�
    UnLock( hData );
  }
  return pBuffer;
}
BOOL CPfxEditView::CPfxEditViewClipboard::SetClipboardData() {
  ASSERT( !m_bForGet );
  ASSERT( m_hCBData != NULL );
  if ( !UnLock( m_hCBData ) || !Open() ) {
    return false;
  }
  // ��ռ�����ԭ��������
  ::EmptyClipboard();
  if ( ::SetClipboardData( m_uFormat, m_hCBData ) == m_hCBData ) {
    m_hCBData = NULL;
  }
  return ( m_hCBData == NULL );
}
// CPfxEditViewDataSource...
CPfxEditView::CPfxEditViewDataSource::~CPfxEditViewDataSource() {
  m_pOwner->SetDragging( false );
}
BOOL CPfxEditView::CPfxEditViewDataSource::OnRenderGlobalData( LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal ) {
  UNUSED( lpFormatEtc );
  if ( *phGlobal != NULL ) {
    return false;
  }
  *phGlobal = m_pOwner->DragDropRender( true, false );
  return ( *phGlobal != NULL );
}
// CPfxEditView...
#if 0
IMPLEMENT_DYNCREATE( CPfxEditView, CView )
BEGIN_MESSAGE_MAP( CPfxEditView, CView )
//{{AFX_MSG_MAP(CPfxEditView)
ON_WM_MOUSEMOVE()
ON_WM_MOUSEWHEEL()
ON_WM_HSCROLL()
ON_WM_VSCROLL()
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_CHAR()
ON_WM_KEYDOWN()
ON_WM_SETFOCUS()
ON_WM_KILLFOCUS()
ON_WM_CAPTURECHANGED()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_LBUTTONDBLCLK()
ON_WM_CREATE()
ON_WM_RBUTTONDOWN()
ON_WM_SETCURSOR()
ON_WM_DESTROY()
//}}AFX_MSG_MAP
// MBCS support!
ON_MESSAGE( WM_IME_COMPOSITION, OnIMEComposition )
// Standard printing commands
ON_COMMAND( ID_FILE_PRINT, CView::OnFilePrint )
ON_COMMAND( ID_FILE_PRINT_DIRECT, CView::OnFilePrint )
ON_COMMAND( ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview )
END_MESSAGE_MAP()
#endif
CPfxEditView::CPfxEditView() {
  m_pdb = NULL;
  m_ppc = NULL;
  m_pti = &PFX_EC_DATA.m_defTypeInfo;
  OnInitialize();
  m_pdb = new CPfxEditDoc;
  m_pdb->m_arrViews[ 0 ] = this;
  OnCreate();
}
CPfxEditView::~CPfxEditView() {
  delete m_pdb;
}
// OLE�Ϸ�֧��
DROPEFFECT CPfxEditView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint pt ) {
  if ( IsReadOnly() ) {
    return DROPEFFECT_NONE;
  }
  DestroyCaret();
  CreateSolidCaret( ( GetSystemMetrics( SM_CXBORDER ) * 2 ), PFX_EC_DATA.m_sizeCaret.cy );
  ShowCaret();
  return OnDragOver( pDataObject, dwKeyState, pt );
}
DROPEFFECT CPfxEditView::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint pt ) {
  if ( IsReadOnly() || !pDataObject->IsDataAvailable( CF_TEXT ) ) {
    return DROPEFFECT_NONE;
  }
  CPoint ptTextPos;
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  CPoint ptEditPos;
  ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
  ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
  // ���λ�ö��뵽�еĶ���
  ptTextPos.y = TransLinePosToTextPos( ptEditPos.y );
  CaretSetPos( ptTextPos );
  return ( ( dwKeyState & MK_CONTROL ) ? DROPEFFECT_COPY : DROPEFFECT_MOVE );
}
void CPfxEditView::OnDragLeave() {
  if ( !IsReadOnly() ) {
    DestroyCaret();
  }
}
BOOL CPfxEditView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint pt ) {
  DestroyCaret(); // ɾ��������������������������
  PFX_EC_DATA.m_hDataDragDropCopied = NULL;
  if ( IsReadOnly() || !pDataObject->IsDataAvailable( CF_TEXT ) ) {
    return false;
  }
  CPfxEditView* pSiblingView = GetSiblingView();
  if ( ( pSiblingView != NULL ) && pSiblingView->IsDragging() ) {
    // ������ͼ������ת�����ı����꣬Ȼ�����ֵ���ͼ�������ı���
    // ��ת������ͼ����
    pt.y = TransViewPosToTextPosY( pt.y );
    pt.x = TransViewPosToTextPosX( pt.x );
    pt.y = pSiblingView->TransTextPosToViewPosY( pt.y );
    pt.x = pSiblingView->TransTextPosToViewPosX( pt.x );
    return pSiblingView->OnDrop( pDataObject, dropEffect, pt );
  }
  // ������š���ѡ��Χ�ڣ���������Դ�ṩ�����Լ������Լ���ѡ��Χ
  // ��������Ϸţ��򲻱ؼ�������
  if ( IsInsideSelect( pt ) && IsDragging() ) {
    return false;
  }
  HANDLE hGlobal = pDataObject->GetGlobalData( CF_TEXT );
  if ( hGlobal == NULL ) {
    return false;
  }
  CPoint ptTextPos;
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  CPoint ptEditPos;
  ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
  ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
  CPfxEditViewUndoManager um( this, ( ( dropEffect & DROPEFFECT_MOVE ) ? PFX_EC_UNDO_DRAGDROP_MOVE : PFX_EC_UNDO_DRAGDROP_COPY ) );
  if ( ( ( dropEffect & DROPEFFECT_MOVE ) == DROPEFFECT_MOVE ) && IsDragging() ) {
    DragDropRemove( ptEditPos );
  }
  // ���ߴ�������Դ�ĺ��������ݱ��Լ����ա�
  SetDragDropHandled( IsDragging() );
  BOOL bResult = DragDropPaste( hGlobal, ptEditPos );
  // ��������Դ�������Լ�Ҳ�������������ڣ������������ģ�������õ�
  // ˢ�·�������������һ��ˢ��
  UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
  return bResult;
}
void CPfxEditView::OnInitialize() {
  m_bOverride = false;
  m_bTrackingColSelect = false;
  m_bTrackingMouse = false;
  m_bDragging = false;
  m_bDelayRecalcWrapping = false;
  m_ptEditPos.x = m_ptEditPos.y = 0;
  m_ptLogiPos.x = m_ptLogiPos.y = 0;
  m_ptTextPos.x = m_ptTextPos.y = 0;
  m_ptCaretRefer.x = m_ptCaretRefer.y = 0;
  m_ptFirstVisible.x = m_ptFirstVisible.y = 0;
  m_sizeVisible.cx = m_sizeVisible.cy = 0;
  m_nSelSize = 0;
  m_nSelLineFrom = m_nSelLineTo = PFX_EC_INVALID_SELECT;
  m_nSelMode = PFX_EC_SM_NORMAL;
}
void CPfxEditView::OnInitialUpdate() {
  OnInitialize();
  UpdateLayout();
  UpdateCurrentPosition( m_ptTextPos.x, true, true );
  if ( IsFirstSiblingView() ) {
    SetViewWrapping( GetWrapStyle(), GetWrapWidth() );
  }
#ifdef _SYNC_SCROLLING
  // ��ʹ��ͬ����������ʱ������Ѿ����ֵ���ͼ����ǰ��ˮƽλ�ñ�������
  // ����ͼһ�£����ԾͲ���Ҫ����ˮƽ������
  CPfxEditView* pSiblingView = GetSiblingView();
  if ( pSiblingView == NULL ) {
    ScrollBarUpdate( SB_HORZ );
    ScrollBarSetPos( SB_HORZ, 0 );
  } else {
    m_ptFirstVisible.x = pSiblingView->m_ptFirstVisible.x;
  }
#else
  ScrollBarUpdate( SB_HORZ );
  ScrollBarSetPos( SB_HORZ, 0 );
#endif
  ScrollBarUpdate( SB_VERT );
  ScrollBarSetPos( SB_VERT, 0 );
  // CView::OnInitialUpdate();
}
void CPfxEditView::SetSpacingLines( LONG nSpacing ) {
  ASSERT( nSpacing >= 0 );
  PFX_EC_DATA.m_nSpacingLines = nSpacing;
  PFX_EC_DATA.m_nLineHeight = PFX_EC_DATA.m_nCharHeight + nSpacing;
}
void CPfxEditView::SetSpacingLineNumberAndText( LONG nSpacing ) {
  ASSERT( nSpacing >= 0 );
  PFX_EC_DATA.m_nSpacingLineNumberAndText = nSpacing;
}
// BOOL SetFont(HFONT hFont)
// ���ÿؼ�����ʾ����
// HFONT hFont - ������ݱ�
// hFont�ǹ�����Դ���ؼ���������ɾ����������Ӧ����ɾ��
// �����߱������ǰ�Ӵ��������������뽹���Ը��²����λ��
BOOL CPfxEditView::SetFont( HFONT hFont ) {
  // �������Ĵ����޸�����ͼ��˽�����ݣ������µ�����ݱ��Ƿ���ɵ���ͬ��Ҫ����
  if ( hFont == NULL ) {
    hFont = ( HFONT ) ::GetStockObject( SYSTEM_FONT );
  }
  if ( hFont == PFX_EC_DATA.m_hFontText ) {
    return true;
  }
  // BLOCK����ȡ������������
  LONG i;
  CSize size;
  // CControlUI* pWndDesktop = GetDesktopWindow();
  CWindowDC dc( 0 );
  HGDIOBJ hFontOld = dc.SelectObject( ( HGDIOBJ ) hFont );
  TEXTMETRIC tm;
  dc.GetTextMetrics( &tm );
  for ( PFX_EC_DATA.m_nCharWidthMaxNum = 0, i = '0'; i <= '9'; i++ ) {
    size = dc.GetTextExtent( ( LPCTSTR ) & i, 1 );
    PFX_EC_DATA.m_nCharWidthMaxNum = max( PFX_EC_DATA.m_nCharWidthMaxNum, size.cx );
  }
  i = PFX_EC_CHR_SPACE;
  size = dc.GetTextExtent( ( LPCTSTR ) & i, 1 );
  PFX_EC_DATA.m_nCharWidthSpace = size.cx;
  i = PFX_EC_CHR_TAB;
  size = dc.GetTextExtent( ( LPCTSTR ) & i, 1 );
  PFX_EC_DATA.m_nCharWidthTab = size.cx;
  dc.SelectObject( hFontOld );
  PFX_EC_DATA.m_hFontText = hFont;
  PFX_EC_DATA.m_sizeCaret.cy = tm.tmHeight;
  PFX_EC_DATA.m_sizeCaret.cx = 2 * GetSystemMetrics( SM_CXBORDER );
  PFX_EC_DATA.m_nCharHeight = tm.tmHeight;
  PFX_EC_DATA.m_nCharWidthAve = tm.tmAveCharWidth;
  PFX_EC_DATA.m_nCharWidthMax = max( tm.tmMaxCharWidth, ( tm.tmAveCharWidth * 2 ) );
  PFX_EC_DATA.m_nCharWidthLineBreak = max( PFX_EC_MIN_LINE_BREAK_WIDTH, ( PFX_EC_DATA.m_nCharHeight * 2 / 3 ) );
  PFX_EC_DATA.m_nLineHeight = PFX_EC_DATA.m_nCharHeight + PFX_EC_DATA.m_nSpacingLines;
  // ���ñ�߳ߴ�
  PFX_EC_DATA.m_nRulerCharHeight = ( tm.tmHeight - tm.tmInternalLeading );
  PFX_EC_DATA.m_nRulerCharWidthAve = tm.tmAveCharWidth;
  PFX_EC_DATA.m_nRulerHeight = max( PFX_EC_DATA.m_nRulerCharHeight, PFX_EC_RULER_MIN_HEIGHT );
  return true;
}
// CPfxEditView ��ӡ����
void CPfxEditView::OnPrepareDC( HDC hDC, CPrintInfo* pInfo ) {
#if 0
  ASSERT( pDC.IsPrinting() );
  // �������Ԥ��������ֻ��ӡ����ҳ��ż��ҳ����Ҫӳ����Ӧҳ
  if ( ( m_ppc != NULL ) && !pInfo->m_bPreview ) {
    switch ( m_ppc->m_nPrintRange ) {
    case PFX_EC_PRINT_PAGES_ODD:
      pInfo->m_nCurPage |= 1;
      break;
    case PFX_EC_PRINT_PAGES_EVEN:
      pInfo->m_nCurPage += ( pInfo->m_nCurPage % 2 );
      break;
    case PFX_EC_PRINT_PAGES_ALL:
    default:           // ��ͨ
      break;
    }
    // ��Ҫֹͣ��ӡ��
    if ( pInfo->m_nCurPage > ( UINT ) m_ppc->m_nToPage ) {
      pInfo->m_bContinuePrinting = false;
    }
  }
#endif
}
void CPfxEditView::OnPrint( HDC hDC, CPrintInfo* pInfo ) {
#if 0
  if ( m_ppc == NULL ) {
    return ;
  }
  CPfxEditViewPrintContext& pc = *m_ppc;
  // ����Ӧ�ô��㿪ʼ
  LONG nCurPage = max( 0, ( pInfo->m_nCurPage - 1 ) );
  nCurPage = min( nCurPage, ( pc.m_nToPage - 1 ) );
  // ����ÿҳ�ĵ�һ�к����һ��
  LONG nCurPageFirstLine = nCurPage * pc.m_nLinesPerPage;
  LONG nCurPageLastLine = nCurPageFirstLine + pc.m_nLinesPerPage;
  pc.m_nCurPage = nCurPage;
  pc.m_nCurPageFirstLine = min( nCurPageFirstLine, pc.m_nMaxLines );
  pc.m_nCurPageLastLine = min( nCurPageLastLine, pc.m_nMaxLines );
  // ����DC����
  CFontH pFontOld = pDC.GetCurrentFont();
  CPen* pPenOld = pDC.GetCurrentPen();
  LONG nMapMode = pDC.SetMapMode( MM_TEXT );
  LONG nBkMode = pDC.SetBkMode( TRANSPARENT );
  UINT nTextAlign = pDC.GetTextAlign();
  COLORREF clrText = pDC.GetTextColor();
  COLORREF clrTextBk = pDC.GetBkColor();
  TRY
  {
    if ( pc.IsPrintHeader() || pc.IsPrintFooter() ) {
    pDC.SelectObject( ( pc.FontHeaderAndFooter() == NULL ) ? pc.GetFontText() : pc.GetFontHeaderAndFooter() );
      // ���Ʊ���
      if ( pc.IsPrintHeader() ) {
        PrintHeader( pDC, pc );
      }
      // ���ƽ�ע
      if ( pc.IsPrintFooter() ) {
        PrintFooter( pDC, pc );
      }
    }
    // �����ı�
    pDC.SelectObject( pc.GetFontText() );
    if ( pc.IsPrintHilighted() && IsDrawHilighted() ) {
    PrintHilighted( pDC, pc );
    } else {
      PrintPlainText( pDC, pc );
    }
    // ���Ʊ߿���
    PrintMargin( pInfo->m_rectDraw, pDC, pc );
  }
  CATCH_ALL( e ) {
    LONG nCode = m_pdb->EventCodeFromException( e );
    e->Delete();
    m_pdb->EventSendMsg( nCode, 0 );
  }
  END_CATCH_ALL
  // �ָ�DC����
  pDC.SelectObject( pFontOld );
  pDC.SelectObject( pPenOld );
  pDC.SetMapMode( nMapMode );
  pDC.SetBkMode( nBkMode );
  pDC.SetTextAlign( nTextAlign );
  pDC.SetTextColor( clrText );
  pDC.SetBkColor( clrTextBk );
#endif
}
void CPfxEditView::OnBeginPrinting( HDC hDC, CPrintInfo* pInfo ) {
#if 0
  // ��̬����һ������--CPfxEditViewPrintContext
  PrintReleaseResource();
  m_ppc = new CPfxEditViewPrintContext;
  if ( m_ppc == NULL ) {
    return ;
  }
  CPfxEditViewPrintContext& pc = *m_ppc;
  CClientDC1 dc( 0 );
  HDC hdcDisplay = dc;
  HDC hdcPrinter = pDC;
  // ��ʼ��Ĭ����ֵ
  pc.SetDC( pdcDisplay, pdcPrinter );
  pc.m_pFontDisplay = ( GetFont() );
  pc.m_bPreview = pInfo->m_bPreview;
  pc.m_bPrintHilighted = IsDrawHilighted();
  pc.m_bPrintLineNumber = IsDrawLineNumber();
  pc.m_bPrintHeaderSeparator = true;
  pc.m_bPrintFooterSeparator = true;
  // Ĭ�ϵ�ȫ����ӡ��ȫ����ָ�û�ָ���ķ�Χ�ڵ�����ҳ����
  pc.m_nPrintRange = PFX_EC_PRINT_PAGES_ALL;
  pc.m_nFromPage = ( LONG ) ( pInfo->GetFromPage() );
  pc.m_nToPage = ( LONG ) ( pInfo->GetToPage() );
  // �Ƚ���ɫ���ƹ���
  for ( int i = 0; i < PFX_EC_SIZEOF( pc.m_arrKeyClasses ); i++ ) {
    pc.m_arrKeyClasses[ i ].m_color = m_pti->m_arrKeyClasses.m_arrClasses[ i ].m_color;
    pc.m_arrKeyClasses[ i ].m_bEmpty = m_pti->m_arrKeyClasses.m_arrClasses[ i ].m_bEmpty;
  }
  // ������ɫ
  PrintCheckColors( pc );
  // �Ʊ��ȣ�����Ļ��ʾ��ͬ
  pc.m_nTabSize = GetTabSize();
  // Ĭ�ϵ��Զ��������ұ߿�������ۻ���ʾ����һ��
  pc.m_nWrapStyle = GetWrapStyle();
  // ������ҳü���룬����ӡҳüʱ�Զ�Ϊ��
  pc.m_nSpacingHeaderAndText = pc.PrinterLogicUnitFromDisplayY( PFX_EC_DEF_SPACING_HEADER_AND_TEXT );
  // ������ҳ�ž��룬����ӡҳ��ʱ�Զ�Ϊ��
  pc.m_nSpacingFooterAndText = pc.PrinterLogicUnitFromDisplayY( PFX_EC_DEF_SPACING_FOOTER_AND_TEXT );
  // ��ȡ�ĵ�������·���������û��·����������ĵ���û����������ʱʹ
  // �ñ�����
  pc.m_strPathName = GetDocument() ->GetPathName();
  if ( pc.m_strPathName.IsEmpty() ) {
    pc.m_strPathName = GetDocument() ->GetTitle();
  }
  // ʱ��
  ::GetLocalTime( &pc.m_time );
  // ����ɴ�ӡ��Χ
  pc.m_rectDraw.SetRect( 0, 0, pdcPrinter.GetDeviceCaps( HORZRES ), pdcPrinter.GetDeviceCaps( VERTRES ) );
  pdcPrinter.DPtoLP( &pc.m_rectDraw );
  // ��ʼ���߿�
  pc.m_rectMargin.SetRectEmpty();
  // ��ʼ��ʧ�ܣ���ӡ���ټ���
  if ( !OnSetupPrintInfo( m_ppc ) ) {
    pInfo->m_bContinuePrinting = false;
    PrintReleaseResource();
    return ;
  }
  // ���������û��ָ����ӡ���壬��ӳ����ʾ�����������ӡ����
  if ( pc.m_fontText == NULL ) {
    pc.SetMapDisplayFont();
  }
  // ��������ܻ�ı���ɫ�����ã��ٴε�����ɫ
  PrintCheckColors( pc );
  // ���úô�ӡDC������
  CFontH pFontOldPrinter = pdcPrinter.SelectObject( pc.GetFontText() );
  CFontH pFontOldDisplay = pdcDisplay.GetCurrentFont();
  TRY
  {
    // ����ҳ���ʽ������
    PrintCalcLayout( pdcPrinter, pc );
    // ��ʽ���ı����ݣ����ָ�ʽ��ֻ�ǻ�ȡ������������Ϣ��һ����
    // ���������޸�������������
    PrintFormatText( pdcPrinter, pc );
    // ����ҳ��Χ
    pc.m_nLineLastParsed = -1;
    // ҳ����
    LONG nLinesPerPage = max( 1, pc.m_nLinesPerPage );
    pc.m_nTotalPages = ( pc.m_nMaxLines + ( nLinesPerPage - 1 ) ) / nLinesPerPage;
    // ���������Сҳ��Χ
    UINT nFromPage = ( UINT ) pc.m_nFromPage;
    UINT nToPage = ( UINT ) pc.m_nToPage;
    UINT nMaxPages = ( UINT ) pc.m_nTotalPages;
    nFromPage = max( nFromPage, 0 );
    nFromPage = min( nFromPage, nMaxPages );
    nToPage = min( nToPage, nMaxPages );
    nToPage = max( nToPage, 0 );
    if ( nFromPage > nToPage ) {
    LONG i = nFromPage;
    nFromPage = nToPage;
    nToPage = i;
  }
  pc.m_nFromPage = nFromPage;
  pc.m_nToPage = nToPage;
  pInfo->SetMinPage( nFromPage );
  pInfo->SetMaxPage( nToPage );
  pInfo->m_pPD->m_pd.nFromPage = ( USHORT ) nFromPage;
  pInfo->m_pPD->m_pd.nToPage = ( USHORT ) nToPage;
}
CATCH_ALL( e ) {
    LONG nCode = m_pdb->EventCodeFromException( e );
    e->Delete();
    m_pdb->EventSendMsg( nCode, 0 );
  }
  END_CATCH_ALL
  // �ָ�DC����
  pdcPrinter.SelectObject( pFontOldPrinter );
  pdcDisplay.SelectObject( pFontOldDisplay );
  // ����һЩ����Ҫ�Ĳ������������ʹ��
  pc.m_pdcDisplay = NULL;
  pc.m_pdcPrinter = NULL;
#endif
}
void CPfxEditView::OnEndPrinting( HDC /*pDC*/, CPrintInfo* pInfo ) {
  ASSERT( pInfo != NULL );
  UNUSED( pInfo );
  PrintReleaseResource();
}
// CPfxEditView ������Ϣ����
INT CPfxEditView::OnCreate() {
  m_pdb = static_cast<CPfxEditDoc*>( GetDocument() );
  ASSERT_KINDOF( CPfxEditDoc, m_pdb );
  // �������ͼ���ĵ��ĵ�һ����ͼ����ôGetTypeInfoPtr()Ӧ�÷���NULL��
  // ��ʱ����Ҫ��������Ĭ�ϵ����ã����������ͼ�����ĵ��ĵ�һ����ͼ��
  // ��GetTypeInfoPtr()��Ӧ�÷���NULL����ʱ���ĵ���������ͼ��ʹ��ͬһ
  // �������Ա���һ�µı���
  m_pti = const_cast<CPfxEditTypeInfo*>( m_pdb->GetTypeInfoPtr() );
  if ( m_pti == NULL ) {
    m_pti = &PFX_EC_DATA.m_defTypeInfo;
  }
  // �û���û��ʹ��SetFont()�������壬��ϵͳĬ����������ʾ����
  if ( GetFont() == NULL ) {
    SetFont( ( HFONT ) NULL );
  }
  // ע��OLE�Ϸ�
  // m_odt.Register( this );
  return 0;
}
void CPfxEditView::OnDestroy() {
  // MFC7�д�ӡԤ��ʱ��һ�������OnEndPrinting()����MDI�����У�MFC7��ӡ
  // Ԥ��ʱHOOKס�Ĳ�������ܵĴ��ڹ��̣�����MDI�Ӵ��ڵĴ��ڹ��̣�����
  // ��ֱ�ӵ�������ڵĹرհ�ťʱִ�е��ǹرճ��򣬶����ǹرմ�ӡԤ����
  // �ڣ�������Ҫ������ͷŴ�ӡ��Դ
  PrintReleaseResource();
}
void CPfxEditView::DoPaint( HDC hDC, const RECT& rcPaint ) {
  // if ( !GetUpdateRect( NULL ) ) {    return ;  }
  // ���������һ�������һ�ɼ��еײ�������ʱ���ܵ�����һ�в��ᱻ��ȷˢ��
  if ( IsDrawHilighted() && ParserPreParse( ( GetLastVisibleLine() + 1 ), m_pdb->TextGetLineUpperBound(), m_pdb->m_nLineLastParsedButCanceled, m_pdb->m_nLineLastParsed, m_pdb->TextGetLineAt( 0 ) ) ) {
    Invalidate( false );
  }
  CDCH dcSrc = hDC; // device context for painting
  CDC dcMem;
  CBitmap bitmap;
  CPfxEditViewPaintContext pc;
  CPoint pt( 0, m_rectText.top );
  CRect rectUpdate;
  dcSrc.GetClipBox( &rectUpdate );
  GetClientRect( rectUpdate );
  pc.m_nFontHeight = PFX_EC_DATA.m_nCharHeight;
  pc.m_nLineHeight = PFX_EC_DATA.m_nLineHeight;
  pc.m_nSpacingLines = GetLineTextVertOffset();
  // Ŀǰֻ������LEFT��Ա
  pc.m_rectIndent.left = m_pdb->m_rectIndent.left;
  LONG nLinesOfsTop = ( ( rectUpdate.top < pt.y ) ? 0 : ( ( rectUpdate.top - pt.y ) / pc.m_nLineHeight ) );
  LONG nLinesOfsBottom = ( ( rectUpdate.bottom < pt.y ) ? -1 : ( ( rectUpdate.bottom - pt.y ) / pc.m_nLineHeight ) );
  pc.m_nFirstUpdateLine = m_ptFirstVisible.y + nLinesOfsTop;
  pc.m_nLastUpdateLine = min( ( m_ptFirstVisible.y + nLinesOfsBottom ), m_pdb->TextGetLineUpperBound() );
  pc.m_sizeBitmap.cx = m_rectClient.Width();
  pc.m_sizeBitmap.cy = pc.m_nLineHeight;
  pc.m_bDrawLineNumber = IsDrawLineNumber();
  pc.m_bDrawRuler = IsDrawRuler();
  pc.m_bDrawLineBreak = IsDrawLineBreak();
  pc.m_bDrawWhitespace = IsDrawWhitespace();
  pc.m_nTextHorzOffset = m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve;
  pc.m_nCharWidthSpace = PFX_EC_DATA.m_nCharWidthSpace;
  pc.m_nCharWidthLineBreak = PFX_EC_DATA.m_nCharWidthLineBreak;
  pc.m_nCharWidthTab = PFX_EC_DATA.m_nCharWidthTab;
  pc.m_nCharWidthMax = PFX_EC_DATA.m_nCharWidthMax;
  pc.m_nTabSize = GetTabSize();
  // ������ɫ
  for ( int i = 0; i < PFX_EC_SIZEOF( pc.m_arrColors ); i++ ) {
    pc.m_arrColors[ i ] = m_pti->m_arrKeyClasses.m_arrClasses[ i ].m_color;
  }
  pc.m_arrColors[ kcBkText ] = ( !IsSyncTextBkColorWithSystem() ? pc.COLOR( kcBkText ) : ::GetSysColor( COLOR_WINDOW ) );
  pc.m_arrColors[ kcBkActiveLine ] = ( !IsSyncActiveLineBkColorWithTextBkColor() ? pc.COLOR( kcBkActiveLine ) : pc.COLOR( kcBkText ) );
  // �����޶����򣬳���������Ĳ���ϵͳ���Զ�����
  pc.m_rectUpdate.left = pt.x;
  pc.m_rectUpdate.top = pt.y + nLinesOfsTop * pc.m_nLineHeight;
  pc.m_rectUpdate.right = rectUpdate.right;
  pc.m_rectUpdate.bottom = rectUpdate.bottom;
  // �������
  pc.m_rectRuler.top = 0;
  pc.m_rectRuler.right = m_rectRuler.right;
  pc.m_rectRuler.bottom = m_rectRuler.bottom;
  pc.m_rectRuler.left = pc.m_rectLineNumber.left = 0;
  // �кź���������
  pc.m_rectLineNumber.top = pc.m_rectText.top = 0;
  pc.m_rectLineNumber.right = pc.m_rectText.left = m_rectText.left;
  pc.m_rectLineNumber.bottom = pc.m_rectText.bottom = pc.m_nLineHeight;
  pc.m_rectText.right = m_rectText.right;
  pc.m_rectColSelect.top = PFX_EC_INVALID_LINE;
  pc.m_rectColSelect.bottom = PFX_EC_INVALID_LINE;
  pc.m_pPenWhitespace = NULL;
  // Ϊ�п�ѡ�����֧��
  if ( IsColumnSelect() || ( IsTracking() && IsColumnSelectTracking() ) ) {
    // �þ����ڵ��������ı����꣬���ڽ����ұ�ת���ɴ�������
    pc.m_rectColSelect.left = TransTextPosToViewPosX( m_rectColSelect.left );
    pc.m_rectColSelect.right = TransTextPosToViewPosX( m_rectColSelect.right );
    if ( !IsColumnSelect() ) {
      pc.m_rectColSelect.top = TransTextPosToLinePos( m_rectColSelect.top );
      pc.m_rectColSelect.bottom = TransTextPosToLinePos( m_rectColSelect.bottom );
    }
    pc.m_rectColSelect.NormalizeRect();
    // ���ó����ı���ˮƽ�������������
    pc.m_rectColSelect.left = max( pc.m_rectText.left, pc.m_rectColSelect.left );
    pc.m_rectColSelect.right = min( pc.m_rectText.right, pc.m_rectColSelect.right );
  }
  if ( pc.m_bDrawWhitespace || pc.m_bDrawLineBreak ) {
    pc.m_penNormal.CreatePen( PS_SOLID, 1, pc.COLOR( kcWhitespace ) );
    if ( IsTextSelected() ) {
      pc.m_penSelect.CreatePen( PS_SOLID, 1, pc.COLOR( kcTextSelected ) );
    }
  }
  dcMem.CreateCompatibleDC( dcSrc );
  dcMem.SetBkMode( TRANSPARENT );
  bitmap.CreateCompatibleBitmap( dcSrc, m_rcItem.Width(), m_rcItem.Height() );
  dcMem.SelectObject( bitmap );
  dcMem.SelectObject( PFX_EC_DATA.m_hFontText );
  DrawBk( dcMem, pc );
  // �������������߷�Χ�ھ�Ҫˢ�±��
  if ( rectUpdate.top <= pc.m_rectRuler.bottom ) {
    DrawRuler( dcMem, pc );
  }
  if ( IsDrawHilighted() ) {
    DrawHilighted( dcMem, pc );
  } else {
    DrawPlainText( dcMem, pc );
  }
  BitBlt( dcSrc, m_rcItem.left, m_rcItem.top, m_rcItem.Width(), m_rcItem.Height(), dcMem, 0, 0, SRCCOPY );
}
void CPfxEditView::DrawBk( HDC hDC, const CPfxEditViewPaintContext& pc ) {
  // ���δ���Ƶ�ʣ����Ļ����
  if ( pc.m_rectUpdate.top < pc.m_rectUpdate.bottom ) {
    CRect rect;
    // ����к�������
    int i = ( pc.m_rectLineNumber.right - pc.m_rectIndent.left );
    if ( i > 0 )           //      �������ʾ�кžͲ���Ҫ���
    {
      ::SetBkColor( hDC, pc.COLOR( kcBkLineNumber ) );
      ::SetRect( &rect, 0, pc.m_rectUpdate.top, i, pc.m_rectUpdate.bottom );
      ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    }
    // ����ı�������
    ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
    ::SetRect( &rect, i, pc.m_rectUpdate.top, pc.m_rectText.right, pc.m_rectUpdate.bottom );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
void CPfxEditView::OnSize( UINT nType, INT cx, INT cy ) {
  UNUSED( nType );
  m_rectClient.SetRect( 0, 0, cx, cy );
  UpdateLayout();
  BOOL bInitialized = ( m_pdb->TextGetLineCount() > 0 );
  // �����Ҫ���¼����Զ����У���������ɵ�һ��ͼ�����
  if ( bInitialized && IsWrapAtWindowEdge() && IsFirstSiblingView() ) {
    SetDelayRecalcWrapping( true );
    // if ( static_cast<CFrameWnd*>( AfxGetMainWnd() ) ->GetActiveFrame() == GetParentFrame() ) {
    // SetViewWrapping( PFX_EC_WRAP_WINDOWEDGE, 0 );
    // }
  }
  // �����ǰ���ڴ�С�Ѿ��ܹ��������е��У���ʹ��һ���ڴ��ڵĶ�����ʾ
  if ( bInitialized && ( m_pdb->TextGetLineCount() < m_sizeVisible.cy ) ) {
    EnsureVisible( 0, m_ptTextPos.x );
  }
  // ��ʹδ����ɳ�ʼ��Ҳ���¹�����
  {
    ScrollBarUpdate( SB_HORZ );
    ScrollBarUpdate( SB_VERT );
  }
  // ���¶�λ�����λ�ã�WinXPϵͳ��Win2000ϵͳ�ı�����Щ��ͬ��������
  // ���ȫ������
  if ( bInitialized ) {
    UpdateCurrentPosition( m_ptTextPos.x, false, false );
  }
}
void CPfxEditView::OnMouseMove( UINT nFlags, CPoint pt ) {
  UNUSED( nFlags );
  UpdateCursor( pt );
  if ( !IsTracking() ) {
    return ;
  }
  CPoint ptTextPos;
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  if ( IsColumnSelectTracking() ) {
    if ( EnsureVisible( TransTextPosToLinePos( ptTextPos.y ), ptTextPos.x ) ) {
      UpdateWindow();
    }
    CPoint ptOrg;
    ptOrg.x = TransTextPosToViewPosX( m_rectColSelect.left );
    ptOrg.y = TransTextPosToViewPosY( TransLinePosToTextPos( TransTextPosToLinePos( m_rectColSelect.top ) ) );
    CPoint ptOld;
    ptOld.x = TransTextPosToViewPosX( m_rectColSelect.right );
    ptOld.y = TransTextPosToViewPosY( TransLinePosToTextPos( TransTextPosToLinePos( m_rectColSelect.bottom ) ) );
    CPoint ptNew;
    ptNew.x = TransTextPosToViewPosX( ptTextPos.x );
    ptNew.y = TransTextPosToViewPosY( TransLinePosToTextPos( TransTextPosToLinePos( ptTextPos.y ) ) );
    CRect rectOrg;
    rectOrg.SetRect( ptOrg.x, ptOrg.y, ptOld.x, ( ptOrg.y + PFX_EC_DATA.m_nLineHeight ) );
    rectOrg.NormalizeRect();
    CRect rectOld;
    rectOld.SetRect( ptOrg.x, ptOld.y, ptOld.x, ( ptOld.y + PFX_EC_DATA.m_nLineHeight ) );
    rectOld.NormalizeRect();
    rectOld.UnionRect( &rectOld, rectOrg );
    rectOld.IntersectRect( &rectOld, &m_rectText );
    rectOrg.SetRect( ptOrg.x, ptOrg.y, ptNew.x, ( ptOrg.y + PFX_EC_DATA.m_nLineHeight ) );
    rectOrg.NormalizeRect();
    CRect rectNew;
    rectNew.SetRect( ptOrg.x, ptNew.y, ptNew.x, ptNew.y + PFX_EC_DATA.m_nLineHeight );
    rectNew.NormalizeRect();
    rectNew.UnionRect( &rectOrg, rectNew );
    rectNew.IntersectRect( &rectNew, &m_rectText );
    CRect rectIntersect;
    rectIntersect.IntersectRect( &rectOld, &rectNew );
    rectIntersect.NormalizeRect();
    CClientDC1 dc( this );
    if ( !rectOld.EqualRect( &rectIntersect ) ) {
      CRgn rgnOld;
      rgnOld.CreateRectRgn( rectOld.left, rectOld.top, rectOld.right, rectOld.bottom );
      dc.SelectClipRgn( rgnOld );
      dc.ExcludeClipRect( &rectIntersect );
      dc.InvertRect( &rectOld );
    }
    if ( !rectNew.EqualRect( &rectIntersect ) ) {
      CRgn rgnNew;
      rgnNew.CreateRectRgn( rectNew.left, rectNew.top, rectNew.right, rectNew.bottom );
      dc.SelectClipRgn( rgnNew );
      dc.ExcludeClipRect( &rectIntersect );
      dc.InvertRect( &rectNew );
    }
    m_rectColSelect.right = ptTextPos.x;
    m_rectColSelect.bottom = ptTextPos.y;
  } else if ( PFX_EC_SM_NORMAL == m_nSelMode ) {
    CPoint ptEditPos;
    ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
    ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
    if ( ptEditPos == m_ptEditPos ) {
      return ;
    }
    CPoint ptSel = SelGetParam();
    m_ptEditPos = ptEditPos;
    SelRange( ptSel );
    EnsureVisible( m_ptEditPos.y, ptTextPos.x );
    UpdateCurrentPosition( ptTextPos.x, true, true );
  } else if ( PFX_EC_SM_WORD == m_nSelMode ) {
    CPoint ptEditPos;
    ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
    ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
    if ( ptEditPos == m_ptEditPos ) {
      return ;
    }
    CPoint ptTestA = ptEditPos;
    m_pdb->TextSubLineToAbsLine( ptTestA );
    CPoint ptTestM = ptEditPos;
    m_pdb->TextSubLineToMainLine( ptTestM );
    CPoint ptBase1 = m_rectSelWordModeBaseRange.TopLeft();
    CPoint ptBase2 = m_rectSelWordModeBaseRange.BottomRight();
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( ptTestM.y );
    const LPCTSTR pStart = pli->pData;
    const LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pCur = pStart + ptTestM.x;
    CPoint ptSelStartPos;
    if ( ( ptTestA.y < ptBase1.y )
         || ( ( ptTestA.y == ptBase1.y ) && ( ptTestA.x <= ptBase1.x ) ) ) {
      // ȡǰ�沿��
      ptSelStartPos = ptBase2;
      if ( ( pCur < pEnd )
           && !IsWhitespace( *pCur )
           && !IsDelimiter( pCur, pEnd ) ) {
        pCur = GetPrevDelimiterPos( pCur, pStart );
      }
    } else {
      // ȡ���沿��
      ptSelStartPos = ptBase1;
      if ( ( pCur < pEnd )
           && !IsWhitespace( *pCur )
           && !IsDelimiter( pCur, pEnd ) ) {
        pCur = GetNextDelimiterPos( pCur, pEnd );
      }
    }
    m_ptEditPos.x = pCur - pStart;
    m_ptEditPos.y = ptTestM.y;
    m_pdb->TextAbsLineToSubLine( ptSelStartPos );
    m_pdb->TextMainLineToSubLine( m_ptEditPos );
    SelRange( ptSelStartPos );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  } else {
    ASSERT( false );
  }
}
BOOL CPfxEditView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt ) {
  UNUSED( nFlags );
  UNUSED( pt );
  // Return Nonzero if mouse wheel scrolling is enabled; otherwise 0.
  UINT nSBCode;
  UINT nScrollLines;
#define SPI_GETWHEELSCROLLLINES 0x0068
  if ( !SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0 ) ) {
    nScrollLines = 3; // default;
  }
  if ( zDelta > 0 ) {
    nSBCode = MAKEWPARAM( SB_LINEUP, 0 );
  } else {
    nSBCode = MAKEWPARAM( SB_LINEDOWN, 0 );
  }
  for ( UINT i = 0; i < nScrollLines; i++ ) {
    m_ptFirstVisible.y = ScrollVert( nSBCode );
  }
  UpdateCurrentPosition( m_ptTextPos.x, false, false );
  return true;
}
void CPfxEditView::OnSetFocus() {
  if ( IsDelayRecalcWrapping() && IsWrapAtWindowEdge() ) {
    SetViewWrapping( PFX_EC_WRAP_WINDOWEDGE, 0 );
  }
  // �ڻ�����뽹��ʱҪ����Ƿ����ֵ���ͼ��������ֵ���ͼ���ֵ���ͼ��
  // ǰ��ѡ���ı���Ҫȡ���ֵ���ͼ��ǰ��ѡ���ı����������ì�ܣ�����˵
  // ����һʱ�������ֵ���ͼ��ֻ����һ���Ǽ������ֻ�м������ͼ������
  // ѡ���ı�
  CPfxEditView* pSiblingView = GetSiblingView();
  if ( pSiblingView != NULL ) {
    if ( pSiblingView->IsTextSelected() ) {
      pSiblingView->SelNone();
    }
  }
  // �����ֵ���ͼ�����Ѿ��޸Ĺ���ǰ�е����ݣ�ʹ������Щ������Ч������
  // Ҫ����У��
  VerifyCurrentPosition();
  CaretRebuild();
  UpdateCurrentPosition( TransCharPosToTextPos( m_ptEditPos ), true, false );
}
void CPfxEditView::OnKillFocus() {
  // CView::OnKillFocus( pNewWnd );
  CaretDestroy();
}
void CPfxEditView::OnLButtonDown( UINT nFlags, CPoint pt ) {
  UNUSED( nFlags );
  BOOL bColSelectTracking = IsColumnSelectTracking();
  SetColumnSelectTracking( false );
  // �����ĵ�ǰλ��ת��Ϊ�ı�λ��
  CPoint ptTextPos;
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  CPoint ptEditPos;
  ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
  ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
  // ���浱ǰ���Ա����ڸ��²���
  CPoint cpOld = m_ptEditPos;
  // ���SHIFT�������£���ִ�и���ѡ��
  if ( PFX_EC_IS_KEY_DOWN( VK_SHIFT ) ) {
    CPoint ptSel = SelGetParam();
    m_ptEditPos = ptEditPos;
    SelRange( ptSel );
    // ��־�������Ѿ����£����������ѡ��״̬
    SetTracking( true );
  }
  // �����������к��ڣ�������λ�ö�Ӧ����ȫ�б�ѡ�񣬶��²����λ�ý�
  // ���ƶ����ñ�ѡ���е���һ�е�����
  else if ( GetHitTestCode( pt ) == PFX_EC_HT_LINE_NUMBER ) {
    SelLine( ptEditPos.y );
    ptTextPos.x = TransCharPosToTextPos( m_ptEditPos );
    // ��־�������Ѿ����£����������ѡ��״̬
    SetTracking( true );
  }
  // �����ѡ���ı���Ϊ�Ϸű༭��׼��
  else if ( IsDragDropEnabled() && IsInsideSelect( pt ) ) {
    m_ptEditPos = ptEditPos;
    UpdateLines( cpOld.y, m_ptEditPos.y );
    UpdateCurrentPosition( ptTextPos.x, true, true );
    DROPEFFECT dropEffect = DROPEFFECT_COPY | DROPEFFECT_MOVE;
    // ������ſ�����dataSrc���󾡿��ܿ������
    {
      CPfxEditViewDataSource dataSrc( this );
      dataSrc.DelayRenderData( CF_TEXT );
      dropEffect = dataSrc.DoDragDrop( dropEffect );
    }
    if ( dropEffect == DROPEFFECT_NONE ) {
      SelNone();
    } else if ( ( dropEffect & DROPEFFECT_COPY ) && !IsDragDropHandled() ) {
      SelNone();
    }
    // �������DROP���������ڣ��ҷ�����ֵָʾɾ����������ִ��ɾ
    // ����ѡ�񲿷�
    else if ( ( dropEffect & DROPEFFECT_MOVE ) && !IsDragDropHandled() ) {
      if ( IsReadOnly() ) {
        SelNone();
      } else {
        CPfxEditViewUndoManager um( this, PFX_EC_UNDO_DRAGDROP_MOVE );
        DragDropRemove( m_ptEditPos );
      }
    }
    ptTextPos.x = m_ptTextPos.x;
    CaretRebuild();
  }
  // �����������ť��ʱ���� ALT�����ڰ���״̬��ִ���п�ѡ�����
  else if ( ( bColSelectTracking || PFX_EC_IS_KEY_DOWN( VK_MENU ) )
            && IsCanColumnSelect() ) {
    m_ptEditPos = ptEditPos;
    if ( IsTextSelected() ) {
      SelNone();
    }
    m_rectColSelect.SetRect( ptTextPos.x, ptTextPos.y, ptTextPos.x, ptTextPos.y );
    SetColumnSelectTracking( true );
    // ��־�������Ѿ����£����������ѡ��״̬
    SetTracking( true );
  } else {
    m_ptEditPos = ptEditPos;
    if ( IsTextSelected() ) {
      SelNone();
    }
    // ��־�������Ѿ����£����������ѡ��״̬
    SetTracking( true );
  }
  EnsureVisibleAndUpdateLines( m_ptEditPos.y, ptTextPos.x, cpOld.y, m_ptEditPos.y );
  UpdateCurrentPosition( ptTextPos.x, true, true );
}
void CPfxEditView::OnRButtonDown( UINT nFlags, CPoint pt ) {
  // ȡ���п�ѡ��Χѡ������
  SetColumnSelectTracking( false );
  // �����ǰ��ѡ�����ݲ������û�л���ѡ��Χ����ǰλ���������ò�
  // ȡ����ǰѡ�񣬷���ʲôҲ����
  if ( !IsInsideSelect( pt ) ) {
    // �����ĵ�ǰλ��ת��Ϊ�ı�λ��
    CPoint ptTextPos;
    ptTextPos.x = TransViewPosToTextPosX( pt.x );
    ptTextPos.y = TransViewPosToTextPosY( pt.y );
    CPoint ptEditPos;
    ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
    ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
    // ���浱ǰ���Ա����ڸ��²���
    CPoint cpOld = m_ptEditPos;
    SelNone();
    m_ptEditPos = ptEditPos;
    EnsureVisibleAndUpdateLines( m_ptEditPos.y, ptTextPos.x, cpOld.y, m_ptEditPos.y );
    UpdateCurrentPosition( ptTextPos.x, true, true );
  }
  // ��WM_CONTEXTMENU����
  // CView::OnRButtonDown( nFlags, pt );
}
void CPfxEditView::OnLButtonUp( UINT nFlags, CPoint pt ) {
  UNUSED( nFlags );
  UNUSED( pt );
  SetTracking( false );
}
void CPfxEditView::OnLButtonDblClk( UINT nFlags, CPoint pt ) {
  UNUSED( nFlags );
  switch ( GetHitTestCode( pt ) ) {
  case PFX_EC_HT_RULER:
  case PFX_EC_HT_LINE_NUMBER:
    return ;
  default:
    break;
  }
  CPoint ptTextPos;
  ptTextPos.y = TransViewPosToTextPosY( pt.y );
  ptTextPos.x = TransViewPosToTextPosX( pt.x );
  CPoint ptEditPos;
  ptEditPos.y = TransTextPosToLinePos( ptTextPos.y );
  ptEditPos.x = TransTextPosToCharPos( ptEditPos.y, ptTextPos.x );
  // ����ԭ���ĵ�ǰ�У���Ϊ����б����ͷǻ�б���ɫ��ͬ��
  // ʱ��ͱ�����£�����ԭ���Ļ����Ȼ��ʾΪ��ǰ���
  if ( ptEditPos.y != m_ptEditPos.y ) {
    LONG i = m_ptEditPos.y;
    m_ptEditPos.y = ptEditPos.y;
    UpdateLines( i, i );
  }
  SelWord( ptEditPos );
  EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  CPoint ptLeftTop = SelGetParam();
  m_pdb->TextSubLineToAbsLine( ptLeftTop );
  CPoint ptRightBottom = m_ptEditPos;
  m_pdb->TextSubLineToAbsLine( ptRightBottom );
  m_nSelMode = PFX_EC_SM_WORD;
  m_rectSelWordModeBaseRange.SetRect( ptLeftTop, ptRightBottom );
  SetTracking( true );
}
void CPfxEditView::OnCaptureChanged( CControlUI *pWnd ) {
  UNUSED( pWnd );
  if ( IsTracking() && IsColumnSelectTracking() ) {
    SelRange( m_rectColSelect );
  } else if ( m_nSelMode == PFX_EC_SM_WORD ) {
    m_nSelMode = PFX_EC_SM_NORMAL;
  }
  m_bTrackingMouse = false;
  SetColumnSelectTracking( false );
}
void CPfxEditView::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) {
  BOOL bCtrlDown = PFX_EC_IS_KEY_DOWN( VK_CONTROL );
  BOOL bShiftDown = PFX_EC_IS_KEY_DOWN( VK_SHIFT );
  BOOL bCancelTracking = true;
  switch ( nChar ) {
  case VK_BACK:
    // bCtrlDown = bWord
    OnKeyBackSpace( bCtrlDown );
    break;
  case VK_DELETE:
    // bCtrlDown = bWord
    OnKeyDelete( bCtrlDown );
    break;
  case VK_LEFT:
    // bShiftDown = bSelect
    // bCtrlDown = bWord
    OnKeyMoveLeft( bShiftDown, bCtrlDown );
    break;
  case VK_RIGHT:
    // bShiftDown = bSelect
    // bCtrlDown = bWord
    OnKeyMoveRight( bShiftDown, bCtrlDown );
    break;
  case VK_UP:
    // bShiftDown = bSelect
    // bCtrlDown = bScroll
    OnKeyMoveUp( bShiftDown, bCtrlDown );
    break;
  case VK_DOWN:
    // bShiftDown = bSelect
    // bCtrlDown = bScroll
    OnKeyMoveDown( bShiftDown, bCtrlDown );
    break;
  case VK_HOME:
    // bShiftDown = bSelect
    // bCtrlDown = bGotoSOF
    OnKeyHome( bShiftDown, bCtrlDown );
    break;
  case VK_END:
    // bShiftDown = bSelect
    // bCtrlDown = bGotoEOF
    OnKeyEnd( bShiftDown, bCtrlDown );
    break;
  case VK_PRIOR:
    // bShiftDown = bSelect
    OnKeyPageUp( bShiftDown );
    break;
  case VK_NEXT:
    // bShiftDown = bSelect
    OnKeyPageDown( bShiftDown );
    break;
  case VK_RETURN:
    OnKeyReturn();
    break;
  case VK_TAB:
    if ( !IsTracking() ) {
      OnKeyTab();
    }
    bCancelTracking = false;
    break;
  case VK_INSERT:
    OnKeyInsert();
    break;
  case VK_ESCAPE:
    SelNone();
    break;
  default:
    // CView::OnKeyDown( nChar, nRepCnt, nFlags );
    bCancelTracking = false;
    break;
  }
  // �����ǰ��ѡ���ı���ʹ��ĳЩ��ɾ����ѡ�񲿷־Ͳ�Ӧ������ѡ����֣�
  // ������ֹWM_MOUSEMOVE����������ѡ��Χ
  if ( bCancelTracking && IsTracking() ) {
    SetTracking( false );
  }
}
void CPfxEditView::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags ) {
  UNUSED( nFlags );
  UNUSED( nRepCnt );
#ifdef _UNICODE
  if ( !IsTracking() && _istprint( nChar ) ) {
    OnKeyChar( ( LPCTSTR ) & nChar, 1 );
  }
#else
  if ( IsTracking() ) {
    return ;
  }
  // ��Щ���뷨������ȫƴ���뷨��������ȫ�ǵı�����ʱ���ᷢ��ƽʱ��
  // WM_IME_XXX��Ϣ�����Ƿ�������WM_CHAR����һ����LEAD BYTE���ڶ�����
  // TRAIL BYTE������Ҫ���⴦��
  static UINT l_s_nLeadByte = 0;
  static HWND l_s_hWndOwner = NULL;
  if ( l_s_nLeadByte == 0 ) {
    if ( !_istprint( nChar ) ) {
      if ( _ismbblead( nChar ) ) {
        l_s_nLeadByte = nChar;
        l_s_hWndOwner = M_HWND;
      }
      return ;
    }
    OnKeyChar( ( LPCTSTR ) & nChar, 1 );
  } else {
    nChar = ( ( nChar << CHAR_BIT ) | l_s_nLeadByte );
    l_s_nLeadByte = 0;
    if ( ( l_s_hWndOwner != M_HWND ) || !_istlegal( nChar ) ) {
      return ;
    }
    OnKeyChar( ( LPCTSTR ) & nChar, PFX_EC_MAX_MB_LEN );
  }
#endif // #ifdef _UNICODE
}
LRESULT CPfxEditView::OnIMEComposition( WPARAM wParam, LPARAM lParam ) {
  if ( ( lParam & GCS_RESULTSTR ) == 0 ) {
    return 0; //DefWindowProc( WM_IME_COMPOSITION, wParam, lParam );
  }
  HIMC hIMC = ::ImmGetContext( M_HWND );
  if ( hIMC != NULL ) {
    CHAR sz[ MAX_PATH ];
    LONG cch = ::ImmGetCompositionString( hIMC, GCS_RESULTSTR, sz, PFX_EC_SIZEOF( sz ) );
    ::ImmReleaseContext( M_HWND, hIMC );
    // �������ԣ���Щ���뷨���ܻ᷵����
    if ( ( cch > 0 ) && ( cch != IMM_ERROR_NODATA ) && ( cch != IMM_ERROR_GENERAL ) ) {
      OnKeyChar( static_cast<LPCTSTR>( sz ), cch );
    }
  }
  return 0;
}
void CPfxEditView::OnKeyChar( LPCTSTR pData, LONG cch ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return ;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_TYPING );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
  }
  if ( !IsOverride() ) {
    CharsInsert( m_ptEditPos, pData, cch, true );
    ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
    ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  } else {
    // ����Ҫ������ַ������ж��ٸ��ַ�
    LPCTSTR pCur = pData;
    LPCTSTR pEnd = pData + cch;
    for ( LONG nCharsToInsert = 0; pCur < pEnd; nCharsToInsert++ ) {
      pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    }
    // ��ȡ�ӵ�ǰλ����nCharsToInsert����ַ�λ��
    CPoint cpOld = m_ptEditPos;
    m_pdb->TextSubLineToMainLine( cpOld );
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( cpOld.y );
    const LPCTSTR pStart = pli->pData;
    for ( pCur = pStart + cpOld.x, pEnd = pStart + pli->nActLen; ( pCur < pEnd ) && ( nCharsToInsert > 0 ); nCharsToInsert-- ) {
      pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    }
    cpOld.x = min( pCur, pEnd ) - pStart;
    m_pdb->TextMainLineToSubLine( cpOld );
    CharsReplace( m_ptEditPos, cpOld, pData, cch, true );
    ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
    ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  }
}
// void OnKeyTab()
void CPfxEditView::OnKeyTab() {
  LONG cch;
  TCHAR sz[ PFX_EC_MAX_TAB_SIZE ];
  if ( !IsExpandTabToSpaceChars() ) {
    cch = 1;
    sz[ 0 ] = PFX_EC_CHR_TAB;
  } else {
    ASSERT( GetTabSize() <= PFX_EC_MAX_TAB_SIZE );
    cch = GetTabSize() - m_ptLogiPos.x % GetTabSize();
    for ( LONG i = 0; i < cch; i++ ) {
      sz[ i ] = PFX_EC_CHR_SPACE;
    }
  }
  OnKeyChar( sz, cch );
}
// void OnKeyInsert()
void CPfxEditView::OnKeyInsert() {
  ASSERT_VALID( this );
  SetOverride( !IsOverride() );
}
// void OnKeyReturn()
void CPfxEditView::OnKeyReturn() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return ;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_TYPING );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
  }
  // �����ǰ����OVERRIDEģʽ�����»س���ֻ�Ὣ�༭���Ƶ���һ�е�����
  if ( IsOverride() ) {
    if ( !m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      m_ptEditPos.x = 0;
      m_ptEditPos.y++;
      UpdateAllViews( PFX_EC_UT_HORZ, m_ptEditPos.y - 1, m_ptEditPos.y );
      EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
    }
  } else {
    // ����������׼ʱ����Ҫ�Ŀհ��ַ���
    LONG nLeadingSpaces = CalcLeadingWhitespaces( m_ptEditPos );
    LONG nLeadingSpacesBufferSize =
      FillBufferWithWhitespacesCalcBufferSize( 0, nLeadingSpaces );
    LONG nBufferSize = nLeadingSpacesBufferSize + PFX_EC_LINE_BREAK_SIZE_DOS;
    CPfxEditHeap heap;
    LPTSTR pBuffer = reinterpret_cast<LPTSTR>( heap.Alloc( nBufferSize * sizeof( TCHAR ) ) );
    memcpy( pBuffer, m_pdb->GetLineBreakDataFromType( PFX_EC_LINE_BREAK_TYPE_DOS ), PFX_EC_LINE_BREAK_SIZE_DOS * sizeof( TCHAR ) );
    FillBufferWithWhitespacesDoFill( 0, nLeadingSpaces, pBuffer + PFX_EC_LINE_BREAK_SIZE_DOS, nLeadingSpacesBufferSize );
    CharsInsert( m_ptEditPos, pBuffer, nBufferSize, true );
  }
}
// void OnKeyBackSpace(BOOL bWord)
void CPfxEditView::OnKeyBackSpace( BOOL bWord ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return ;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_REMOVE );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
    return ;
  }
  if ( m_pdb->TextIsSOF( m_ptEditPos ) ) {
    return ;
  }
  CPoint cpNew;
  CPoint cpOld = m_ptEditPos;
  MoveLeft( cpNew, bWord, false );
  CharsRemove( cpNew, cpOld, true );
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
}
// void OnKeyDelete(BOOL bWord)
void CPfxEditView::OnKeyDelete( BOOL bWord ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return ;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_REMOVE );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
    return ;
  }
  if ( m_pdb->TextIsEOF( m_ptEditPos ) ) {
    return ;
  }
  CPoint cpNew;
  CPoint cpOld = m_ptEditPos;
  MoveRight( cpNew, bWord, false );
  CharsRemove( cpOld, cpNew, true );
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
}
// void OnKeyMoveLeft(BOOL bSelect, BOOL bWord)
void CPfxEditView::OnKeyMoveLeft( BOOL bSelect, BOOL bWord ) {
  LONG xTextPos = 0;
  if ( bSelect ) {
    CPoint ptSel = SelGetParam();
    xTextPos = MoveLeft( m_ptEditPos, bWord );
    SelRange( ptSel );
    EnsureVisible( m_ptEditPos.y, xTextPos );
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_LEFT );
      xTextPos = m_ptTextPos.x; // ����ֵ�Ѿ�������
      EnsureVisible( m_ptEditPos.y, xTextPos );
    } else {
      CPoint cpOld = m_ptEditPos;
      xTextPos = MoveLeft( m_ptEditPos, bWord );
      if ( !EnsureVisible( m_ptEditPos.y, xTextPos )
           && ( cpOld.y != m_ptEditPos.y ) ) {
        UpdateLines( cpOld.y, m_ptEditPos.y );
      }
    }
  }
  UpdateCurrentPosition( xTextPos, true, true );
}
// void OnKeyMoveRight(BOOL bSelect, BOOL bWord)
void CPfxEditView::OnKeyMoveRight( BOOL bSelect, BOOL bWord ) {
  LONG xTextPos = 0;
  if ( bSelect ) {
    CPoint ptSel = SelGetParam();
    xTextPos = MoveRight( m_ptEditPos, bWord );
    SelRange( ptSel );
    EnsureVisible( m_ptEditPos.y, xTextPos );
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_RIGHT );
      xTextPos = m_ptTextPos.x; // ����ֵ�Ѿ�������
      EnsureVisible( m_ptEditPos.y, xTextPos );
    } else {
      CPoint cpOld = m_ptEditPos;
      xTextPos = MoveRight( m_ptEditPos, bWord );
      if ( !EnsureVisible( m_ptEditPos.y, xTextPos )
           && ( cpOld.y != m_ptEditPos.y ) ) {
        UpdateLines( cpOld.y, m_ptEditPos.y );
      }
    }
  }
  UpdateCurrentPosition( xTextPos, true, true );
}
// void OnKeyMoveUp(BOOL bSelect, BOOL bScroll)
// 1)  ���SHIFT�����£�ִ��ѡ�����
// 2)  ���CTRL�����£�ִ�й���������
//  �ڹ��������е�ǰ��ʼ�տɼ����������ǰ��Ҫ���벻�ɼ���Χ���һ��
//  ����Ҫ��Ϊ�µĵ�ǰ��
// 3)  ���SHIFT��CTRL��ͬʱ������SHIFT����
void CPfxEditView::OnKeyMoveUp( BOOL bSelect, BOOL bScroll ) {
  if ( bSelect )           //      ִ��ѡ�����
  {
    if ( m_pdb->TextIsFirstLine( m_ptEditPos.y ) ) {
      // �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ��ɼ���Χ��
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // �趨ѡ��Χ
      CPoint ptSel = SelGetParam();
      // ����µĵ�ǰ�н����ɼ�����Ҫִ�������ı���������
      if ( m_ptEditPos.y != m_ptFirstVisible.y ) {
        m_ptEditPos.y--;
      } else {
        m_ptEditPos.y = m_ptFirstVisible.y = ScrollVert( SB_LINEUP );
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      SelRange( ptSel );
      EnsureVisible( m_ptEditPos.y, xTextPos );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
  else if ( bScroll )           //      VK_UPִ�����Ϲ�������
  {
    if ( m_pdb->TextIsFirstLine( m_ptFirstVisible.y ) ) {
      // �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ��ɼ���Χ��
      if ( !IsTextSelected() ) {
        EnsureVisibleAndUpdateCurrentPosition();
      }
    } else {
      m_ptFirstVisible.y = ScrollVert( SB_LINEUP );
      LONG xTextPos = m_ptTextPos.x;
      // �����ǰ��ѡ���ı�����ʹ����ǰ�뿪ѡ���ı�����
      // ��ʱ��ǰ��ֻ�ܹ��������ɼ���Χ
      if ( ( m_ptEditPos.y > GetLastVisibleLine() ) && !IsTextSelected() ) {
        xTextPos = GetCaretReferPoint();
        m_ptEditPos.y = GetLastVisibleLine();
        m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
        EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, m_ptEditPos.y + 1 );
      }
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
  else // VK_UPִ�и��ĵ�ǰ�в���
  {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_UP );
    }
    if ( m_pdb->TextIsFirstLine( m_ptEditPos.y ) ) {
      // �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ��ɼ���Χ��
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // ����µĵ�ǰ���Ѿ����ڿɼ���Χ����Ҫִ�������ı�
      // ��������
      if ( m_ptEditPos.y != m_ptFirstVisible.y ) {
        m_ptEditPos.y--;
      } else {
        m_ptEditPos.y = m_ptFirstVisible.y = ScrollVert( SB_LINEUP );
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      // ˢ�¸ı�ĵط�
      EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, m_ptEditPos.y + 1 );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
}
// void OnKeyMoveDown(BOOL bSelect, BOOL bScroll)
void CPfxEditView::OnKeyMoveDown( BOOL bSelect, BOOL bScroll ) {
  if ( bSelect )           //      ִ��ѡ�����
  {
    if ( m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      // �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ��ɼ���Χ��
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // �趨ѡ��Χ
      CPoint ptSel = SelGetParam();
      // ����µĵ�ǰ���Ѿ����ڿɼ���Χ����Ҫִ�������ı�
      // ��������
      if ( m_ptEditPos.y < GetLastVisibleLine() ) {
        m_ptEditPos.y++;
      } else {
        m_ptFirstVisible.y = ScrollVert( SB_LINEDOWN );
        m_ptEditPos.y = GetLastVisibleLine();
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      SelRange( ptSel );
      // ˢ�¸ı�ĵط�
      EnsureVisible( m_ptEditPos.y, xTextPos );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
  // ִ�й���������
  // ��ǰ�����ı�����ͬ���������������ʹ������϶����������飬����ǰ
  // ��ʼ���ڿɼ���Χ
  else if ( bScroll ) {
    if ( !m_pdb->TextIsValidLine( m_ptFirstVisible.y + m_sizeVisible.cy ) ) {
      // �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ��ɼ���Χ��
      if ( !IsTextSelected() ) {
        EnsureVisibleAndUpdateCurrentPosition();
      }
    } else {
      LONG xTextPos = m_ptTextPos.x;
      m_ptFirstVisible.y = ScrollVert( SB_LINEDOWN );
      // �����ǰ��ѡ���ı�����ʹ����ǰ�뿪ѡ���ı�����
      // ��ʱ��ǰ��ֻ�ܹ��������ɼ���Χ
      if ( ( m_ptEditPos.y < m_ptFirstVisible.y ) && !IsTextSelected() ) {
        xTextPos = GetCaretReferPoint();
        m_ptEditPos.y = m_ptFirstVisible.y;
        m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
        //  ˢ�¸ı�ĵط�
        EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, m_ptEditPos.y );
      }
      UpdateCurrentPosition( xTextPos, true, false );
    }
  } else // ִ�и��ĵ�ǰ�в�����
  {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_DOWN );
    }
    if ( m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      // �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ��ɼ���Χ��
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // ����µĵ�ǰ���Ѿ����ڿɼ���Χ����Ҫִ�������ı���������
      if ( m_ptEditPos.y < GetLastVisibleLine() ) {
        m_ptEditPos.y++;
      } else {
        m_ptFirstVisible.y = ScrollVert( SB_LINEDOWN );
        m_ptEditPos.y = GetLastVisibleLine();
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      // ȷ����ǰλ�ÿɼ�
      EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, m_ptEditPos.y - 1 );
      // �ض�λ�����
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
}
// void OnKeyHome(BOOL bSelect, BOOL bGotoSOF)
// 1)  ���CTRL���������ƶ����ĵ��ĵ�һ���ʵ���λ��
// 2)  ���CTRL��û�а��£�
//  a)  ��������óɡ�HOME���ƶ����׸��ǿհ��ַ�������ǰλ���ƶ��׸�
//      �ǿհ��ַ�������ǰλ���ƶ�������
//  b)  ���һ����û�зǿհ��ַ���ǰλ���ƶ�������
// 3)  ���SHIFT��ͬʱ���������ƶ���ǰλ�õ�ͬʱִ��ѡ�����
void CPfxEditView::OnKeyHome( BOOL bSelect, BOOL bGotoSOF ) {
  if ( bSelect ) {
    if ( bGotoSOF ) {
      if ( m_pdb->TextIsSOF( m_ptEditPos ) ) {
        //  �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ��ɼ���Χ��
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        //  ȷ��ѡ��Χ
        CPoint ptSel = SelGetParam();
        m_ptEditPos = CPoint( 0, 0 );
        SelRange( ptSel );
        EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
      }
    } else {
      LONG nNewCharPos = 0; // �������ʾ�ƶ�������
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( m_ptEditPos.y );
      if ( IsHomeKeyGotoFirstNonWhitespaceChar() && !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        const LPCTSTR pCur = pli->pData;
        const LPCTSTR pEnd = pCur + pli->nLen;
        LONG nFirstNonWhitespaceCharPos = GetNextNonWhitespacePos( pCur, pEnd ) - pCur;
        if ( m_ptEditPos.x != nFirstNonWhitespaceCharPos ) {
          nNewCharPos = nFirstNonWhitespaceCharPos;
        }
      }
      if ( nNewCharPos == m_ptEditPos.x ) {
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint ptSel = SelGetParam();
        m_ptEditPos.x = nNewCharPos;
        SelRange( ptSel );
        EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
      }
    }
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_HOME );
    }
    if ( bGotoSOF ) {
      if ( m_pdb->TextIsSOF( m_ptEditPos ) ) {
        //  �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ��ɼ���Χ��
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint cp( 0, 0 );
        EnsureVisibleAndSetAsCurrentPosition( cp );
      }
    } else {
      LONG nNewCharPos = 0; // �������ʾ�ƶ�������
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( m_ptEditPos.y );
      if ( IsHomeKeyGotoFirstNonWhitespaceChar() && !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        const LPCTSTR pCur = pli->pData;
        const LPCTSTR pEnd = pCur + pli->nLen;
        LONG nFirstNonWhitespaceCharPos = GetNextNonWhitespacePos( pCur, pEnd ) - pCur;
        if ( m_ptEditPos.x != nFirstNonWhitespaceCharPos ) {
          nNewCharPos = nFirstNonWhitespaceCharPos;
        }
      }
      if ( nNewCharPos == m_ptEditPos.x ) {
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint cp( nNewCharPos, m_ptEditPos.y );
        EnsureVisibleAndSetAsCurrentPosition( cp );
      }
    }
  }
}
// void OnKeyEnd(BOOL bSelect, BOOL bGotoEOF)
// 1)  ���ͬʱ����CTRL���ƶ����ĵ������һ����ĩ
// 2)  ���CTRL��û�а�����ǰλ���ƶ�����ĩ
// 3)  ���SHIFT��ͬʱ���������ƶ���ǰλ�õ�ͬʱִ��ѡ�����
void CPfxEditView::OnKeyEnd( BOOL bSelect, BOOL bGotoEOF ) {
  if ( bSelect ) {
    if ( bGotoEOF ) {
      if ( m_pdb->TextIsEOF( m_ptEditPos ) ) {
        //  �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ���
        //  ����Χ��
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint ptSel = SelGetParam();
        m_ptEditPos.y = m_pdb->TextGetLineUpperBound();
        m_ptEditPos.x = m_pdb->TextGetLineLength( m_ptEditPos.y );
        SelRange( ptSel );
        EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
      }
    } else {
      if ( m_pdb->TextIsEOL( m_ptEditPos ) ) {
        //  �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ���
        //  ����Χ��
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint ptSel = SelGetParam();
        m_ptEditPos.y = m_ptEditPos.y;
        m_ptEditPos.x = m_pdb->TextGetLineLength( m_ptEditPos.y );
        SelRange( ptSel );
        EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
      }
    }
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_END );
    }
    if ( bGotoEOF ) {
      if ( m_pdb->TextIsEOF( m_ptEditPos ) ) {
        //  �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ���
        //  ����Χ��
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint cp;
        cp.y = m_pdb->TextGetLineUpperBound();
        cp.x = m_pdb->TextGetLineLength( cp.y );
        EnsureVisibleAndSetAsCurrentPosition( cp );
      }
    } else {
      if ( m_pdb->TextIsEOL( m_ptEditPos ) ) {
        //  �����ǰλ�ò��ڿɼ���Χ�ڣ�Ҫ����ŵ���
        //  ����Χ��
        EnsureVisibleAndUpdateCurrentPosition();
      } else {
        CPoint cp;
        cp.y = m_ptEditPos.y;
        cp.x = m_pdb->TextGetLineLength( cp.y );
        EnsureVisibleAndSetAsCurrentPosition( cp );
      }
    }
  }
}
// void OnKeyPageUp(BOOL bSelect)
// 1)  CTRL�������ԣ�ִ�����Ϲ�һ���Ĳ���
//  a)  ����Ѿ���ѡ���ı���ȡ����ѡ�����ݣ�������ƶ�����ѡ�����ݵ�
//      ����һ��
//  b)  ��ǰ��Ҳ��ͬ������Ӧ������
// 2)  ���SHIFT�����£�ִ�����Ϲ�һ���Ĳ�������ѡ����һ������������
// 3)  ���й������������ض�λ�����
void CPfxEditView::OnKeyPageUp( BOOL bSelect ) {
  if ( bSelect ) {
    if ( m_pdb->TextIsFirstLine( m_ptEditPos.y ) ) {
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // �趨ѡ��Χ
      CPoint ptSel = SelGetParam();
      // ȷ���µĵ�ǰ�С���һ�ɼ���
      if ( m_pdb->TextIsFirstLine( m_ptFirstVisible.y ) ) {
        m_ptEditPos.y = 0;
      } else {
        LONG nFirstVisibleLine = ScrollVert( SB_PAGEUP );
        m_ptEditPos.y = m_pdb->TextVerifyLine( ( m_ptEditPos.y - ( m_ptFirstVisible.y - nFirstVisibleLine ) ) );
        m_ptFirstVisible.y = nFirstVisibleLine;
      }
      // ���¼��㵱ǰ��
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      SelRange( ptSel );
      EnsureVisible( m_ptEditPos.y, xTextPos );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  } else {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_PRIOR );
    }
    if ( m_pdb->TextIsFirstLine( m_ptEditPos.y ) ) {
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      CPoint cpOld = m_ptEditPos;
      if ( m_pdb->TextIsFirstLine( m_ptFirstVisible.y ) ) {
        m_ptEditPos.y = 0;
      } else {
        LONG nFirstVisibleLine = ScrollVert( SB_PAGEUP );
        m_ptEditPos.y = m_pdb->TextVerifyLine( ( m_ptEditPos.y - ( m_ptFirstVisible.y - nFirstVisibleLine ) ) );
        m_ptFirstVisible.y = nFirstVisibleLine;
      }
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, m_ptEditPos.y, cpOld.y );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
}
// void OnKeyPageDown(BOOL bSelect)
void CPfxEditView::OnKeyPageDown( BOOL bSelect ) {
  if ( bSelect ) {
    if ( m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      // �趨ѡ��Χ
      CPoint ptSel = SelGetParam();
      // ȷ���µĵ�ǰ�С���һ�ɼ���
      if ( m_pdb->TextIsLastLine( GetLastVisibleLine() ) ) {
        m_ptEditPos.y = m_pdb->TextGetLineUpperBound();
      } else {
        LONG nFirstVisibleLine = ScrollVert( SB_PAGEDOWN );
        m_ptEditPos.y = m_pdb->TextVerifyLine( ( m_ptEditPos.y + ( nFirstVisibleLine - m_ptFirstVisible.y ) ) );
        m_ptFirstVisible.y = nFirstVisibleLine;
      }
      // ���¼��㵱ǰ��
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      SelRange( ptSel );
      EnsureVisible( m_ptEditPos.y, xTextPos );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  } else // VK_NEXT(PAGEDOWN)
  {
    if ( IsTextSelected() ) {
      SelNoneAndUpdateCurrentPosition( VK_NEXT );
    }
    if ( m_pdb->TextIsLastLine( m_ptEditPos.y ) ) {
      EnsureVisibleAndUpdateCurrentPosition();
    } else {
      CPoint cpOld = m_ptEditPos;
      // ȷ���µĵ�ǰ�С���һ�ɼ���
      if ( m_pdb->TextIsLastLine( GetLastVisibleLine() ) ) {
        m_ptEditPos.y = m_pdb->TextGetLineUpperBound();
      } else {
        LONG nFirstVisibleLine = ScrollVert( SB_PAGEDOWN );
        m_ptEditPos.y = m_pdb->TextVerifyLine( ( m_ptEditPos.y + ( nFirstVisibleLine - m_ptFirstVisible.y ) ) );
        m_ptFirstVisible.y = nFirstVisibleLine;
      }
      // ���¼��㵱ǰ��
      LONG xTextPos = GetCaretReferPoint();
      m_ptEditPos.x = TransTextPosToCharPos( m_ptEditPos.y, xTextPos );
      EnsureVisibleAndUpdateLines( m_ptEditPos.y, xTextPos, cpOld.y, m_ptEditPos.y );
      UpdateCurrentPosition( xTextPos, true, false );
    }
  }
}
// CPfxEditView ����
void CPfxEditView::DrawVisualEOL( HDC hDC, LONG x, LONG y, LONG cx, LONG cy ) {
#ifndef LINE_BREAK_LOOKING_USE_CASE_2
  // �س�������ۣ�����1��
  //           |p3(x2, y1)-------|--------
  //           |                 | |
  //           |                 | |
  //           |                 | 2dx
  //           |                 | |
  // p0(x1, y2) |\   |   /| p2(x3, y2)-|--------
  //      | \  |  / |            | |
  //      |  \ | /  |            | dx
  //      |   \|/   |            | |
  //      |    V p1(x2, y3)------|--------
  //      |         |
  //   ---|---2dx---|---
  LONG dx = ( min( cx, cy ) + 3 ) / 4;
  LONG xc = x + 2 * dx;
  LONG x1 = xc - dx - 1;
  LONG x2 = xc - 1;
  LONG x3 = xc + dx;
  LONG y1 = y + 1 * dx;
  LONG y2 = y + 3 * dx;
  LONG y3 = y + 4 * dx;
  // HDC hDC = pDC;
  ::MoveToEx( hDC, x1, y2, NULL );
  ::LineTo( hDC, x2, y3 );
  ::LineTo( hDC, x3, y2 - 1 );
  ::MoveToEx( hDC, x2, y1, NULL );
  ::LineTo( hDC, x2, y3 );
#else
  // �س�������ۣ�����2��
  //              |
  //             |=|----------------|---
  //   (x2, y1)p2|||p3((x4, y1)     |
  //             |||                |
  //             |||                |2dx
  //             |||                |
  // ---------(x2, y2)p1|+|p4(x4, y2)------+--------------
  // (x1, y2)p0|\==|||==/|p5(x5, y2)--|---
  // (x1, y2)p7| \  |  / |            |
  //         |  \ | /  |            |dx
  //         |    v----+------------|---
  //    (x3, y3)p6|    |
  //         |    |    |
  //       --|---2dx---|--
  POINT pt[ 8 ];
  LONG dx = ( min( cx, cy ) + 3 ) / 4;
  LONG xc = x + 2 * dx;
  LONG x1 = xc - dx;
  LONG x2 = xc - dx / 2;
  LONG x3 = xc;
  LONG x4 = xc + dx / 2;
  LONG x5 = xc + dx;
  LONG y1 = y + 1 * dx;
  LONG y2 = y + 3 * dx;
  LONG y3 = y + 4 * dx;
  pt[ 0 ].x = pt[ 7 ].x = x1;
  pt[ 0 ].y = pt[ 7 ].y = pt[ 1 ].y = pt[ 4 ].y = pt[ 5 ].y = y2;
  pt[ 1 ].x = pt[ 2 ].x = x2;
  pt[ 2 ].y = pt[ 3 ].y = y1;
  pt[ 3 ].x = pt[ 4 ].x = x4;
  pt[ 5 ].x = x5;
  pt[ 6 ].x = x3;
  pt[ 6 ].y = y3;
  ::Polyline( hDC, pt, PFX_EC_SIZEOF( pt ) );
#endif
}
// void DrawVisualEOF(HDC hDC, LONG x, LONG y, LONG cx, LONG cy)
// HDC hDC - �����DC������
// LONG x - ���Ͻ�x����
// LONG y - ���Ͻ�y����
// LONG cx - ��ȣ�ע���ÿ�ȼ������Ժ��ԣ�
// LONG cy - �߶ȣ�һ���������߶ȶ������и߶�
// �����ĵ�������־�����ĵ����һ�е���󣬶��Ҿ��Բ������з�������ѡ
// �����Լ������Ժ���cx��ֻʹ��cy
void CPfxEditView::DrawVisualEOF( HDC hDC, LONG x, LONG y, LONG cx, LONG cy ) {
#ifndef LINE_BREAK_LOOKING_USE_CASE_2
  // �ĵ�������־����ۣ�����1��
  //            p0(x2, y1)
  //          /-----------------------|---
  //        /              p3(x3, y2) |
  // p1(x1, y2) <=================|=        |
  //      | \               |         | 2dx
  //      |   \ p2(x2, y3)  |         |
  //      |----|------------|---------|---
  //      | dx |     2dx    |
  // ����ļ���ʽ����Ϊ�˺�DrawEndOfLine()�е�һ�£�ʹ�����Ƴ�
  // ���ļ�ͷ��DrawEndOfLine()�е���һ�µ����
  LONG dx = ( min( cx, cy ) + 3 ) / 4;
  LONG yc = y + cy / 2;
  LONG x1 = x + dx;
  LONG x2 = x + 2 * dx;
  LONG x3 = x + 4 * dx;
  LONG y1 = yc - dx;
  LONG y2 = yc;
  LONG y3 = yc + dx;
  // HDC hDC = hDC;
  ::MoveToEx( hDC, x2, y1, NULL );
  ::LineTo( hDC, x1, y2 );
  ::LineTo( hDC, x2 + 1, y3 + 1 );
  ::MoveToEx( hDC, x1, y2, NULL );
  ::LineTo( hDC, x3, y2 );
#else
  // �ĵ�������־����ۣ�����2��
  //                     |Y
  //                     |
  // (x2, y1) p1|--------|----------------------|---
  //           /|(x2, y2)|            (x3, y2)  |
  // p0(x1, y3)    /  |p2======|===========|p3        |1dy
  // -----------|<-------------+-----------|----------|------X
  // p7(x1, y3) |  \  |p5======|=|=========|p4        |1dy
  //      |    \|(x2, y4)| |         |(x3, y4)  |
  // (x2, y5) p6|--------+-+---------+----------|---
  //      |     |        | |         |
  // ------------|-1dy-|---1dy--|-|---1dy---|----
  //                     |
  //                     |
  POINT pt[ 8 ];
  // ����ļ���ʽ����Ϊ�˺�DrawEndOfLine()�е�һ�£�ʹ�����Ƴ�
  // ���ļ�ͷ��DrawEndOfLine()�е���һ�µ����
  LONG dx = ( min( cx, cy ) + 3 ) / 4;
  LONG yc = y + cy / 2;
  LONG x1 = x + dx;
  LONG x2 = x + 2 * dx;
  LONG x3 = x + 4 * dx;
  LONG y1 = yc - dx;
  LONG y2 = yc - dx / 2;
  LONG y3 = yc;
  LONG y4 = yc + dx / 2;
  LONG y5 = yc + dx;
  pt[ 0 ].x = pt[ 7 ].x = x1;
  pt[ 0 ].y = pt[ 7 ].y = y3;
  pt[ 1 ].x = pt[ 2 ].x = pt[ 5 ].x = pt[ 6 ].x = x2;
  pt[ 1 ].y = y1;
  pt[ 2 ].y = pt[ 3 ].y = y2;
  pt[ 3 ].x = pt[ 4 ].x = x3;
  pt[ 5 ].y = pt[ 4 ].y = y4;
  pt[ 6 ].y = y5;
  ::Polyline( hDC, pt, PFX_EC_SIZEOF( pt ) );
#endif
}
void CPfxEditView::DrawVisualTab( HDC hDC, LONG x, LONG y, LONG cx, LONG cy ) {
  // HDC hDC = hDC;
  LONG x2 = x + cx - 2;
  y += ( cy / 2 );
  ::MoveToEx( hDC, x, y, NULL );
  ::LineTo( hDC, x2, y );
  ::MoveToEx( hDC, x2, y - 2, NULL );
  ::LineTo( hDC, x2, y + 3 );
}
// ������������ʱDC�����Ա����ó�TA_UPDATECP | TA_TOP | TA_LEFT���ڷ���ʱӦ
// ȷ����Щ����δ���ı䣬���������Ը��ĵ�ǰλ��
void CPfxEditView::DrawVisualSpace( HDC hDC, LONG x, LONG y, LONG cx, LONG cy ) {
  // HDC hDC = hDC;
  y += ( cy / 2 );
  ::MoveToEx( hDC, x + ( cx / 4 ), y, NULL );
  ::LineTo( hDC, x + cx - ( cx / 4 ), y );
}
void CPfxEditView::DrawRuler( HDC hdcSrc, const CPfxEditViewPaintContext& pc ) {
  CRect rect;
  CSize size( pc.m_rectRuler.Width(), pc.m_rectRuler.Height() );
  // ��������Ʊ�ߣ��������ڵ�����ʵ������һ�հ���������������ʹ
  // �����ϱ���������һ������Ա�����ֽ���
  if ( !pc.m_bDrawRuler ) {
    HDC hDC = hdcSrc;
    ::SetRect( &rect, 0, 0, pc.m_rectLineNumber.right - pc.m_rectIndent.left, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcBkLineNumber ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    ::SetRect( &rect, rect.right, 0, size.cx, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  } else {
    // ���»��Ʊ�ߣ�ʹ���ڴ�DC�������
    CDC dcMem;
    dcMem.CreateCompatibleDC( hdcSrc );
    dcMem.SetBkMode( TRANSPARENT );
    HDC hDC = dcMem;
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap( hdcSrc, size.cx, size.cy );
    HGDIOBJ hBitmapOld = ::SelectObject( hDC, bitmap );
    ::SetTextAlign( hDC, TA_TOP | TA_LEFT | TA_NOUPDATECP );
    // �����̶�֮��ľ��������������ƽ�����
    LONG nRulerScale = PFX_EC_DATA.m_nCharWidthAve;
    // ��һ��Ҫ���µĿ̶�ֵ
    LONG nCount = pc.m_nTextHorzOffset / nRulerScale;
    nCount /= PFX_EC_RULER_RESOLUTION;
    nCount *= PFX_EC_RULER_RESOLUTION;
    LONG x1 = pc.m_rectText.left;
    LONG x2 = x1 + nCount * nRulerScale - pc.m_nTextHorzOffset;
    LONG yMinor = size.cy - size.cy / 3;
    LONG yMajor = yMinor - 2;
    // ����߱���
    ::SetRect( &rect, 0, 0, size.cx, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcBkRuler ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    // ���Ʊ�߿̶�
    ::SetTextColor( hDC, pc.COLOR( kcRuler ) );
    ::SetBkColor( hDC, pc.COLOR( kcRuler ) );
    for ( ; x2 <= pc.m_rectRuler.right; x2 += nRulerScale, nCount++ ) {
      // �������̶�
      if ( ( nCount % PFX_EC_RULER_RESOLUTION ) == 0 ) {
        TCHAR sz[ 16 ];
        int szlen = _stprintf( sz, _T( "%d" ), ( nCount / PFX_EC_RULER_RESOLUTION + 1 ) );
        ::SetRect( &rect, x2, yMajor, x2 + PFX_EC_RULER_SCALE_WIDTH, size.cy );
        ::ExtTextOut( hDC, x2, 0, ETO_OPAQUE, &rect, sz, szlen, NULL );
      }
      // ���Ƹ��̶�
      else {
        ::SetRect( &rect, x2, yMinor, x2 + PFX_EC_RULER_SCALE_WIDTH, size.cy );
        ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
      }
    }
    // ���ó�����Χ������
    ::SetRect( &rect, 0, 0, x1, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcBkRuler ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    // ���Ʊ�ߵײ���ˮƽ����
    ::SetRect( &rect, 0, ( size.cy - PFX_EC_RULER_SCALE_WIDTH ), size.cx, size.cy );
    ::SetBkColor( hDC, pc.COLOR( kcRuler ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    // ������
    ::BitBlt( hdcSrc, pc.m_rectRuler.left, pc.m_rectRuler.top, size.cx, size.cy, hDC, 0, 0, SRCCOPY );
    ::SelectObject( hDC, hBitmapOld );
  }
}
// void DrawLineNumber(LONG nLine,HDC hDC,const CPfxEditViewPaintContext& pc)
void CPfxEditView::DrawLineNumber( LONG nLine, HDC hDC, const CPfxEditViewPaintContext& pc ) {
  // HDC hDC = hDC;
  LONG x = pc.m_rectLineNumber.right - pc.m_rectIndent.left;
  LONG y = pc.m_rectLineNumber.top;
  CRect rect;
  // ����к�������֮��ļ��
  ::SetRect( &rect, x, y, pc.m_rectLineNumber.right, pc.m_rectLineNumber.bottom );
  ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
  ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, NULL, NULL );
  if ( pc.m_bDrawLineNumber ) {
    TCHAR sz[ 32 ];
    LONG cch;
    if ( nLine != PFX_EC_INVALID_LINE ) {
      cch = _stprintf( sz, _T( "%d" ), ( nLine + 1 ) );
    } else {
      cch = 0;
    }
    ::SetRect( &rect, pc.m_rectLineNumber.left, y, x, pc.m_rectLineNumber.bottom );
    ::SetTextAlign( hDC, TA_RIGHT | TA_TOP | TA_NOUPDATECP );
    ::SetTextColor( hDC, pc.COLOR( kcLineNumber ) );
    ::SetBkColor( hDC, pc.COLOR( kcBkLineNumber ) );
    ::ExtTextOut( hDC, ( x - PFX_EC_LINE_NUMBER_INDENT_RIGHT ), ( y + pc.m_nSpacingLines ), ETO_CLIPPED | ETO_OPAQUE, &rect, sz, cch, NULL );
  }
}
void CPfxEditView::DrawLineBreak( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc ) {
  if ( pc.m_ptCurPos.x >= pc.m_rectText.right ) {
    return ;
  }
  if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
    CPenH pPen;
    if ( IsTextSelected()
         && ( nLine <= m_nSelLineTo )
         && ( nLine >= m_nSelLineFrom )
         && ( pli->nSelTo > pli->nLen ) ) {
      pPen = pc.m_penSelect;
    } else {
      pPen = pc.m_penNormal;
    }
    if ( pc.m_pPenWhitespace != pPen ) {
      ::SelectObject( hDC, pPen );
      pc.m_pPenWhitespace = pPen;
    }
    DrawVisualEOL( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthLineBreak, pc.m_nFontHeight );
  } else if ( nLine == m_pdb->TextGetLineUpperBound() ) {
    if ( pc.m_pPenWhitespace != pc.m_penNormal ) {
      ::SelectObject( hDC, pc.m_penNormal );
      pc.m_pPenWhitespace = pc.m_penNormal;
    }
    DrawVisualEOF( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthLineBreak, pc.m_nFontHeight );
  }
}
void CPfxEditView::DrawLineBkgnd( LONG nLine, const CPfxEditLineInfo* pli, LONG x, HDC hDC, const CPfxEditViewPaintContext& pc ) {
  CRect rect;
  // HDC hDC = hDC;
  ::SetRect( &rect, x, pc.m_rectText.top, pc.m_rectText.right, pc.m_rectText.bottom );
  if ( nLine == m_ptEditPos.y ) {
    ::SetBkColor( hDC, pc.COLOR( kcBkActiveLine ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    if ( PFX_EC_IS_DRAW_BOOKMARK( pli ) ) {
      ::SetBkColor( hDC, pc.COLOR( kcBkBookmark ) );
      ::SetRect( &rect, x, pc.m_rectText.top, pc.m_rectText.right + 1, pc.m_rectText.top + 1 );
      ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
      ::SetRect( &rect, x, pc.m_rectText.bottom, pc.m_rectText.right + 1, pc.m_rectText.bottom - 1 );
      ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    }
  } else if ( PFX_EC_IS_DRAW_BOOKMARK( pli ) ) {
    ::SetBkColor( hDC, pc.COLOR( kcBkBookmark ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  } else {
    ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
// void DrawPlainTextNormal(LPCTSTR pData,LONG nLen,LONG& rnLogChar,HDC hDC,CPfxEditViewPaintContext& pc)
void CPfxEditView::DrawPlainTextNormal( LPCTSTR pData, LONG nLen, LONG& rnLogChar, HDC hDC, CPfxEditViewPaintContext& pc ) {
  LONG x2 = pc.m_rectText.right;
  if ( pc.m_ptCurPos.x >= x2 ) {
    return ;
  }
  // ������ǰ���ڻ��Ʋ��ɼ��ַ��Ļ���
  if ( pc.m_bDrawWhitespace && ( pc.m_pPenWhitespace != pc.m_penNormal ) ) {
    ::SelectObject( hDC, pc.m_penNormal );
    pc.m_pPenWhitespace = pc.m_penNormal;
  }
  ::SetTextColor( hDC, pc.COLOR( kcText ) );
  LPCTSTR pTmp = pData;
  LPCTSTR pCur = pData;
  const LPCTSTR pEnd = pData + nLen;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    TCHAR tc = *pCur;
    if ( PFX_EC_IS_LEAD_BYTE( tc ) ) {
      rnLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( tc != PFX_EC_CHR_TAB ) {
      // ���Ҫ���ƿհ��ַ�
      if ( ( tc != PFX_EC_CHR_SPACE ) || !pc.m_bDrawWhitespace ) {
        rnLogChar++;
        pCur++;
      } else {
        if ( pCur > pTmp ) {
          ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
          ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
        }
        DrawVisualSpace( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        rnLogChar++;
        pTmp = ++pCur;
        pc.m_ptCurPos.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
      }
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
        ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      }
      LONG nTabSize = pc.m_nTabSize - ( rnLogChar % pc.m_nTabSize );
      LONG nTabWidth = nTabSize * pc.m_nCharWidthSpace;
      if ( pc.m_bDrawWhitespace ) {
        DrawVisualTab( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, nTabWidth, pc.m_nFontHeight );
      }
      rnLogChar += nTabSize;
      pTmp = ++pCur;
      pc.m_ptCurPos.x += nTabWidth;
      ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
    }
  }
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    int nTmpLen = min( pEnd, pCur ) - pTmp;
    ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, nTmpLen, NULL );
    ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
  }
}
void CPfxEditView::DrawPlainTextSelection( LONG nLine, const CPfxEditLineInfo* pli, LONG& rnLogChar, HDC hDC, CPfxEditViewPaintContext& pc ) {
  LONG x2 = pc.m_rectText.right;
  // �����ʼ���Ƶ��Ѿ��ڸ���������ұ߾Ͳ���Ҫ���л���
  if ( pc.m_ptCurPos.x >= x2 ) {
    return ;
  }
  // ��䱳��
  ::SetBkColor( hDC, pc.COLOR( kcBkTextSelected ) );
  ::SetTextColor( hDC, pc.COLOR( kcTextSelected ) );
  CRect rect;
  ::SetRect( &rect, pc.m_ptCurPos.x, pc.m_rectText.top, pc.m_rectText.right, pc.m_rectText.bottom );
  ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  // ������ǰ���ڻ��Ʋ��ɼ��ַ��Ļ���
  if ( pc.m_bDrawWhitespace && ( pc.m_pPenWhitespace != pc.m_penSelect ) ) {
    ::SelectObject( hDC, pc.m_penSelect );
    pc.m_pPenWhitespace = pc.m_penSelect;
  }
  LPCTSTR pCur = pli->pData + pli->nSelFrom;
  LPCTSTR pTmp = pCur;
  const LPCTSTR pEnd = pli->pData + min( pli->nSelTo, pli->nLen );
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    TCHAR tc = *pCur;
    if ( PFX_EC_IS_LEAD_BYTE( tc ) ) {
      rnLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( tc != PFX_EC_CHR_TAB ) {
      // ���Ҫ���ƿհ��ַ�
      if ( ( tc != PFX_EC_CHR_SPACE ) || !pc.m_bDrawWhitespace ) {
        rnLogChar++;
        pCur++;
      } else {
        if ( pCur > pTmp ) {
          ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
          ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
        }
        DrawVisualSpace( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        rnLogChar++;
        pTmp = ++pCur;
        pc.m_ptCurPos.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
      }
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
        ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      }
      LONG nTabSize = pc.m_nTabSize - ( rnLogChar % pc.m_nTabSize );
      rnLogChar += nTabSize;
      nTabSize *= pc.m_nCharWidthSpace;
      if ( pc.m_bDrawWhitespace ) {
        DrawVisualTab( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, nTabSize, pc.m_nFontHeight );
      }
      pTmp = ++pCur;
      pc.m_ptCurPos.x += nTabSize;
      ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
    }
  }
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( min( pEnd, pCur ) - pTmp ), NULL );
    ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
  }
  // ��ǰλ�ò��ܸı䣬��Ϊ����Ҫ���ƻ��з�
  LONG xFill = pc.m_ptCurPos.x;
  // ��pli->nSelTo����pli->nLenʱ������һ�е�ѡ��������з�
  if ( pli->nSelTo > pli->nLen ) {
    xFill += pc.m_nCharWidthLineBreak;
  }
  // ������ı���
  if ( x2 > xFill ) {
    DrawLineBkgnd( nLine, pli, xFill, hDC, pc );
  }
  // ������п�ѡ����ѡ�������Ѿ��Ǳ��������ַ����Լ�ѡ�������ұ�
  // ���п鷶Χ���ұ�С�������ѡ���������һ���ַ����п鷶Χ�ұߵļ�
  // ϶����ʹ��������һ����
  if ( IsColumnSelect() && ( x2 > xFill ) && ( xFill < pc.m_rectColSelect.right ) && ( pli->nSelTo >= pli->nLen ) ) {
    ::SetRect( &rect, max( pc.m_rectColSelect.left, xFill ), pc.m_rectText.top, pc.m_rectColSelect.right, pc.m_rectText.bottom );
    ::SetBkColor( hDC, pc.COLOR( kcBkTextSelected ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
void CPfxEditView::DrawPlainText( HDC hdcSrc, CPfxEditViewPaintContext& pc ) {
  CRect rect;
  BOOL bWrapping = IsWrapping();
  CPoint pt;
  CDC hdcMem;
  hdcMem.CreateCompatibleDC( hdcSrc );
  hdcMem.SetBkMode( TRANSPARENT );
  CBitmap bitmap;
  bitmap.CreateCompatibleBitmap( hdcSrc, pc.m_rectUpdate.Width(), pc.m_rectUpdate.Height() );
  hdcMem.SelectBitmap( bitmap );
  HDC hDC = hdcMem;
  pt.x = ( pc.m_rectText.left - pc.m_nTextHorzOffset );
  pt.y = ( pc.m_rectText.top + pc.m_nSpacingLines );
  for ( LONG i = pc.m_nFirstUpdateLine; i <= pc.m_nLastUpdateLine; i++ ) {
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
    LPCTSTR pData = pli->pData;
    LONG nLogChar = pli->nFirstChar;
    // ���һ���ı�
    pc.m_ptCurPos.x = pt.x;
    pc.m_ptCurPos.y = pt.y;
    ::SetTextAlign( hDC, TA_LEFT | TA_TOP | TA_UPDATECP );
    ::MoveToEx( hDC, pt.x, pt.y, NULL );
    // ���������ѡ��Χ�ڣ���Ҫ�ر���
    if ( ( i <= m_nSelLineTo ) && ( i >= m_nSelLineFrom ) ) {
      if ( pli->nSelFrom > 0 ) {
        DrawLineBkgnd( i, pli, pc.m_rectText.left, hdcMem, pc );
        DrawPlainTextNormal( pData, pli->nSelFrom, nLogChar, hdcMem, pc );
      }
      DrawPlainTextSelection( i, pli, nLogChar, hdcMem, pc );
      if ( pli->nSelTo < pli->nLen ) {
        DrawPlainTextNormal( ( pData + pli->nSelTo ), ( pli->nLen - pli->nSelTo ), nLogChar, hdcMem, pc );
      }
    } else {
      DrawLineBkgnd( i, pli, pc.m_rectText.left, hdcMem, pc );
      // �����һ��û��ѡ���ı���һ�ι������Լӿ��ٶ�
      if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        DrawPlainTextNormal( pData, pli->nLen, nLogChar, hdcMem, pc );
      }
    }
    // ���ƻس���
    if ( pc.m_bDrawLineBreak ) {
      DrawLineBreak( i, pli, hdcMem, pc );
    }
    // �����к�
    DrawLineNumber( ( ( !PFX_EC_IS_SUBLINE( pli ) && !bWrapping ) ? i : pli->nIndex ), hdcMem, pc );
    // Ϊ�п�ѡ�����֧��
    if ( ( i <= pc.m_rectColSelect.bottom ) && ( i >= pc.m_rectColSelect.top ) ) {
      ::SetRect( &rect, pc.m_rectColSelect.left, pc.m_rectText.top, pc.m_rectColSelect.right, pc.m_rectText.bottom );
      ::InvertRect( hDC, &rect );
    }
    // �������Ļ
    ::BitBlt( hdcSrc, pc.m_rectUpdate.left, pc.m_rectUpdate.top, pc.m_sizeBitmap.cx, pc.m_sizeBitmap.cy, hDC, 0, 0, SRCCOPY );
    // ��һ��
    pc.m_rectUpdate.top += pc.m_sizeBitmap.cy;
  }
}
// ����һ�������ַ�����������ǹؼ��־�����Ӧ����ɫ�������������ͨ���ı���ɫ���
// LPCTSTR pData - �ַ�����ַ
// LONG nLen     - �ַ�������
// HDC hDC       - ���������CDCָ��
// CPfxEditViewPaintContext& pc - ����ʾ��صĲ���
// ���ַ����ڲ��ú��пհ��ַ�
void CPfxEditView::DrawHilightedString1( LPCTSTR pData, LONG nLen, HDC hDC, CPfxEditViewPaintContext& pc ) {
  if ( ( pData == NULL ) || ( nLen <= 0 ) ) {
    return ;
  }
  LONG i = m_pti->m_mapKeywords.Lookup( pData, nLen );
  COLORREF clr = pc.COLOR( ( i != -1 ) ? i : kcText );
  if ( clr != pc.m_clrCurText ) {
    pc.m_clrCurText = clr;
    ::SetTextColor( hDC, clr );
  }
  ::ExtTextOut( hDC, 0, 0, 0, NULL, pData, nLen, NULL );
  ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
}
// ���һָ�����ַ��������ַ���������ͬһ����ɫ���
// LPCTSTR pData    - �ַ����ĵ�ַ
// LONG nLen        - �ַ����ĳ���
// LONG& rnLogChar  - ���ַ�����һ���ַ���Ӧ���߼��ַ���
// COLORREF color   - ������ı���ɫ
// HDC hDC          - ���������CDCָ��
// CPfxEditViewPaintContext& pc - ����ʾ��صĲ���
void CPfxEditView::DrawHilightedString2( LPCTSTR pData, LONG nLen, LONG& rnLogChar, COLORREF color, HDC hDC, CPfxEditViewPaintContext& pc ) {
  if ( ( pData == NULL ) || ( nLen == 0 ) ) {
    return ;
  }
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectText.right;
  pc.m_clrCurText = color;
  ::SetTextColor( hDC, pc.m_clrCurText );
  LPCTSTR pCur = pData;
  LPCTSTR pTmp = pCur;
  const LPCTSTR pEnd = pData + nLen;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    TCHAR tc = *pCur;
#ifdef _MBCS
    if ( PFX_EC_IS_LEAD_BYTE( tc ) ) {
      rnLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
#endif
    if ( tc != PFX_EC_CHR_TAB ) {
      // ���Ҫ���ƿհ��ַ�
      if ( ( tc != PFX_EC_CHR_SPACE ) || !pc.m_bDrawWhitespace ) {
        rnLogChar++;
        pCur++;
      } else {
        if ( pCur > pTmp ) {
          ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
          ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
        }
        DrawVisualSpace( pDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        rnLogChar++;
        pTmp = ++pCur;
        pc.m_ptCurPos.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
      }
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
        ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      }
      LONG nTabSize = pc.m_nTabSize - ( rnLogChar % pc.m_nTabSize );
      LONG nTabWidth = nTabSize * pc.m_nCharWidthSpace;
      if ( pc.m_bDrawWhitespace ) {
        DrawVisualTab( pDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, nTabWidth, pc.m_nFontHeight );
      }
      rnLogChar += nTabSize;
      pTmp = ++pCur;
      pc.m_ptCurPos.x += nTabWidth;
      ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
    }
  }
  // ���ʣ�ಿ��
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( min( pEnd, pCur ) - pTmp ), NULL );
    ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
  }
}
// ���ݸ�������һ�е�COOKIE����������ʾ���COOKIE��ص��ַ�����
// LPCTSTR pData   - һ�����ݵĿ�ʼ
// LONG nLen       - �������ݵĳ���
// LONG& rnLogChar - ��ʼ���߼��ַ���
// DWORD dwCookie  - COOKIE
// HDC hDC        - ���������CDCָ��
// CPfxEditViewPaintContext& pc - ����ʾ��صĲ���
// ����NULL��ʾ����Ҫ������������ʾ�����ط�NULL��ʾӦ�ôӸõ�ַ������������������
LPCTSTR CPfxEditView::DrawHilightedStringBaseOnCookie( LPCTSTR pData, LONG nLen, LONG& rnLogChar, DWORD dwCookie, HDC hDC, CPfxEditViewPaintContext& pc ) {
  CDCH pDC = hDC;
  const LPCTSTR pStart = pData;
  const LPCTSTR pEnd = pStart + nLen;
  // ����һ�п�ʼʹ������ע�ͣ�
  // abcdefg
  // hijklmn
  // �������ֻ��Ҫ�����ǰ�е�ȫ�����ݱ��
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT1 ) {
    DrawHilightedString2( pStart, ( pEnd - pStart ), rnLogChar, pc.COLOR( kcLineComment1 ), pDC, pc );
    return NULL;
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT2 ) {
    DrawHilightedString2( pStart, ( pEnd - pStart ), rnLogChar, pc.COLOR( kcLineComment2 ), pDC, pc );
    return NULL;
  }
  // ��һ�еĿ�ͷ���ֻ�ȫ���ǿ�ע�͵�һ���֣�
  // abcdefg /* this is block comment
  // samples */ hijklmn
  if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT1 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpBlockComment1 ) );
    if ( pFound == NULL )           //      û���ҵ���һֱ�������ĩ
    {
      pFound = pEnd;
    }
    DrawHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcBlockComment1 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT2 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpBlockComment2 ) );
    if ( pFound == NULL )           //      û���ҵ���һֱ�������ĩ
    {
      pFound = pEnd;
    }
    DrawHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcBlockComment2 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING1 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpQuotedString1 ) );
    if ( pFound == NULL )           //      û���ҵ���һֱ�������ĩ
    {
      pFound = pEnd;
    }
    DrawHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcQuotedString1 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING2 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpQuotedString2 ) );
    if ( pFound == NULL )           //      û���ҵ���һֱ�������ĩ
    {
      pFound = pEnd;
    }
    DrawHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcQuotedString2 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  return pStart;
}
// �����﷨����ʱһ���еķ�ѡ�񲿷�
void CPfxEditView::DrawHilightedNormal( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc ) {
  UNUSED( nLine );
  ASSERT( !PFX_EC_IS_EMPTY_LINE( pli ) );
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectText.right;
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nLen;
  LPCTSTR pCur = pStart;
  LONG nLogChar = pli->nFirstChar;
  DWORD dwCookie = pli->dwFlags & PFX_EC_LF_COOKIE_ALL;
  // ������ǰ���ڻ��Ʋ��ɼ��ַ��Ļ���
  if ( pc.m_bDrawWhitespace && ( pc.m_pPenWhitespace != pc.m_penNormal ) ) {
    ::SelectObject( hDC, pc.m_penNormal );
    pc.m_pPenWhitespace = pc.m_penNormal;
  }
  pc.m_clrCurText = pc.COLOR( kcText );
  ::SetTextColor( hDC, pc.m_clrCurText );
  pCur = DrawHilightedStringBaseOnCookie( pStart, pli->nLen, nLogChar, dwCookie, pDC, pc );
  // �����һ������һ��COOKIE���������Ͳ���Ҫ����
  if ( pCur == NULL ) {
    return ;
  }
  // ÿ�η���һ���ַ���ʱ�򽫸��ַ�������ı�ʶ���ĵ�һ���ַ��Ƚϣ���
  // �����Ⱦ�����Ҫ�����Ƚϣ��������Լӿ�����ٶ�
  CPfxEditViewMarkerChar mc;
  ParserGetFirstCharOfMarkerStart( mc );
  LPCTSTR pTmp = pCur;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    // ��ע��1
    if ( ( ( wc == mc.m_cLowerLineComment1 ) || ( wc == mc.m_cUpperLineComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment1 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      DrawHilightedString2( pCur, ( pEnd - pCur ), nLogChar, pc.COLOR( kcLineComment1 ), pDC, pc );
      pCur = pTmp = pEnd;
      break;
    }
    // ��ע��2
    if ( ( ( wc == mc.m_cLowerLineComment2 ) || ( wc == mc.m_cUpperLineComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment2 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      DrawHilightedString2( pCur, ( pEnd - pCur ), nLogChar, pc.COLOR( kcLineComment2 ), pDC, pc );
      pCur = pTmp = pEnd;
      break;
    }
    // ��ע��1
    if ( ( ( wc == mc.m_cLowerBlockComment1 ) || ( wc == mc.m_cUpperBlockComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment1 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment1 ) );
      if ( pTmp == NULL )           //      û���ҵ���һֱ�������ĩ
      {
        pTmp = pEnd;
      }
      DrawHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcBlockComment1 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    if ( ( ( wc == mc.m_cLowerBlockComment2 ) || ( wc == mc.m_cUpperBlockComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment2 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment2 ) );
      if ( pTmp == NULL )           //      û���ҵ���һֱ�������ĩ
      {
        pTmp = pEnd;
      }
      DrawHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcBlockComment2 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // �����ַ���1
    if ( ( ( wc == mc.m_cLowerQuotedString1 ) || ( wc == mc.m_cUpperQuotedString1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString1 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString1 ) );
      if ( pTmp == NULL )           //      û���ҵ���һֱ�������ĩ
      {
        pTmp = pEnd;
      }
      DrawHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcQuotedString1 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // �����ַ���2
    if ( ( ( wc == mc.m_cLowerQuotedString2 ) || ( wc == mc.m_cUpperQuotedString2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString2 ) ) ) {
      DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString2 ) );
      if ( pTmp == NULL )           //      û���ҵ���һֱ�������ĩ
      {
        pTmp = pEnd;
      }
      DrawHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcQuotedString2 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // �������ַ����ǿ�ı�ʶ�������潫��������Ƿ��ǹؼ��ֵ�
    // һ���֣������ǿհ��ַ��������ǵ��ʶ������
    if ( !IsWhitespace( wc ) && !IsDelimiter( wc ) ) {
      if ( bLeadByte ) {
        pCur += PFX_EC_MAX_MB_LEN;
        nLogChar += PFX_EC_MAX_MB_LEN;
      } else {
        pCur++;
        nLogChar++;
      }
    } else {
      if ( pCur > pTmp )           //      ����֮ǰ�Ĳ���
      {
        DrawHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
        pTmp = pCur;
      }
      if ( wc == PFX_EC_CHR_SPACE ) {
        if ( pc.m_bDrawWhitespace ) {
          DrawVisualSpace( pDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        }
        pCur++;
        nLogChar++;
        pc.m_ptCurPos.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
        pTmp = pCur;
      } else if ( wc == PFX_EC_CHR_TAB ) {
        LONG nTabSize = pc.m_nTabSize - ( nLogChar % pc.m_nTabSize );
        LONG nTabWidth = nTabSize * pc.m_nCharWidthSpace;
        if ( pc.m_bDrawWhitespace ) {
          DrawVisualTab( pDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, nTabWidth, pc.m_nFontHeight );
        }
        pCur++;
        nLogChar += nTabSize;
        pc.m_ptCurPos.x += nTabWidth;
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
        pTmp = pCur;
      } else // Delimiter!
      {
        if ( bLeadByte ) {
          pCur += PFX_EC_MAX_MB_LEN;
          nLogChar += PFX_EC_MAX_MB_LEN;
        } else {
          pCur++;
          nLogChar++;
        }
        //  ��Щ������ԵĹؼ����п��ܺ��е��ʶ�������������Ծ����С�.XLIST��.XALL���� ��
        //  αָ�C++�����С�==��!=��->��>>�� �����������˵��������ʶ����ʱӦ���ٷ���һ
        //  ��������ĵ��������Ƿ���һ���ؼ���
        LPCTSTR pTmpData = pCur;
        LONG nTmpLogChar = nLogChar;
        while ( ( pTmpData < pEnd ) && !IsWhitespace( *pTmpData ) ) {
          LONG nStep = ( PFX_EC_IS_LEAD_BYTE( *pTmpData ) ? PFX_EC_MAX_MB_LEN : 1 );
          //   ��������ĵ�һ���ַ����ǵ��ʶ���
          //   �����Ͱ����ö���������򲻰�����
          if ( IsDelimiter( pTmpData, pEnd ) ) {
            if ( pTmpData == pCur ) {
              pTmpData += nStep;
              nTmpLogChar += nStep;
            }
            break;
          }
          pTmpData += nStep;
          nTmpLogChar += nStep;
        }
        pTmpData = min( pTmpData, pEnd );
        if ( ( pTmpData != pCur )
             && ParserIsKeyword( pTmp, ( pTmpData - pTmp ) ) ) {
          pCur = pTmpData;
          nLogChar = nTmpLogChar;
        }
        DrawHilightedString1( pTmp, ( min( pEnd, pCur ) - pTmp ), pDC, pc );
        pTmp = pCur;
      }
    }
  }
  // ����ʣ��Ĳ��֣�����û�п��ǵ��Զ�����ʱ���е����⣬���Ե�һ����
  // ���ֱ�ǿ���۶ϵ�ʱ���������ᱻ��ȷ�ط�������ȷ����ʾ
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    DrawHilightedString1( pTmp, ( min( pEnd, pCur ) - pTmp ), pDC, pc );
  }
}
// void DrawHilightedSelection(LONG nLine,const CPfxEditLineInfo* pli,HDC hDC,CPfxEditViewPaintContext& pc)
// �����﷨����ʱһ���е�ѡ�񲿷�
void CPfxEditView::DrawHilightedSelection( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc ) {
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectText.right;
  LONG nLogChar = pli->nFirstChar;
  const LPCTSTR pLineStart = pli->pData;
  const LPCTSTR pLineEnd = pLineStart + pli->nLen;
  const LPCTSTR pSelStart = pLineStart + pli->nSelFrom;
  const LPCTSTR pSelEnd = pLineStart + min( pli->nSelTo, pli->nLen );
  // �����������
  CPoint pt;
  ::GetCurrentPositionEx( hDC, &pt );
  // �����ʼ���Ƶ��Ѿ��ڸ���������ұ߾Ͳ���Ҫ���л���
  if ( pt.x >= x2 ) {
    return ;
  }
  // ���ѡ�񲿷ֵĵ�һ���ַ����Ǳ��еĵ�һ���ַ�����Ҫ����ѡ�񲿷ֵ�һ���ַ����ڵ�ˮƽ���꣬
  // ����������ڻ��ƣ����⻹Ҫ��������ַ���Ӧ���߼�����
  if ( pSelStart > pLineStart ) {
    pt.x += CalcStringWidth( pLineStart, ( pSelStart - pLineStart ), nLogChar, pDC );
  }
  // �����ʼ���Ƶ��Ѿ��ڸ���������ұ߾Ͳ���Ҫ���л���
  if ( x2 <= pt.x ) {
    return ;
  }
  CPoint ptEnd;
  ptEnd.x = pc.m_rectText.right;
  // ���ѡ�񲿷ֵ�����ַ����Ǳ��е�����ַ�����Ҫ����ѡ�񲿷ֵĿ��
  if ( pSelEnd < pLineEnd ) {
    LONG nLogCharTemp = nLogChar;
    ptEnd.x = ( pt.x + CalcStringWidth( pSelStart, ( pSelEnd - pSelStart ), nLogCharTemp, pDC ) );
  }
  ::MoveToEx( hDC, pt.x, pt.y, NULL );
  // ��䱳��
  ::SetBkColor( hDC, pc.COLOR( kcBkTextSelected ) );
  ::SetTextColor( hDC, pc.COLOR( kcTextSelected ) );
  CRect rect;
  ::SetRect( &rect, pt.x, pc.m_rectText.top, ptEnd.x, pc.m_rectText.bottom );
  ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  // ������ǰ���ڻ��Ʋ��ɼ��ַ��Ļ���
  if ( pc.m_bDrawWhitespace && ( pc.m_pPenWhitespace != pc.m_penSelect ) ) {
    ::SelectObject( hDC, pc.m_penSelect );
    pc.m_pPenWhitespace = pc.m_penSelect;
  }
  // ��ʼ����
  LPCTSTR pTmp = pSelStart;
  LPCTSTR pCur = pSelStart;
  LPCTSTR pEnd = pSelEnd;
  while ( ( pCur < pEnd ) && ( x2 > pt.x ) ) {
    TCHAR tc = *pCur;
    if ( PFX_EC_IS_LEAD_BYTE( tc ) ) {
      pCur += PFX_EC_MAX_MB_LEN;
      nLogChar += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( tc != PFX_EC_CHR_TAB ) {
      // ���Ҫ���ƿհ��ַ�
      if ( ( tc != PFX_EC_CHR_SPACE ) || !pc.m_bDrawWhitespace ) {
        pCur++;
        nLogChar++;
      } else {
        if ( pCur > pTmp ) {
          ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
          ::GetCurrentPositionEx( hDC, &pt );
        }
        DrawVisualSpace( pDC, pt.x, pt.y, pc.m_nCharWidthSpace, pc.m_nFontHeight );
        nLogChar++;
        pTmp = ++pCur;
        pt.x += pc.m_nCharWidthSpace;
        ::MoveToEx( hDC, pt.x, pt.y, NULL );
      }
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( pCur - pTmp ), NULL );
        ::GetCurrentPositionEx( hDC, &pt );
      }
      LONG nTabSize = pc.m_nTabSize - ( nLogChar % pc.m_nTabSize );
      nLogChar += nTabSize;
      nTabSize *= pc.m_nCharWidthSpace;
      if ( pc.m_bDrawWhitespace ) {
        DrawVisualTab( pDC, pt.x, pt.y, nTabSize, pc.m_nFontHeight );
      }
      pTmp = ++pCur;
      pt.x += nTabSize;
      ::MoveToEx( hDC, pt.x, pt.y, NULL );
    }
  }
  // ���δ���Ƶ�ʣ�ಿ��
  if ( ( pCur > pTmp ) && ( x2 > pt.x ) ) {
    ::ExtTextOut( hDC, 0, 0, 0, NULL, pTmp, ( min( pEnd, pCur ) - pTmp ), NULL );
    ::GetCurrentPositionEx( hDC, &pt );
  }
  // ��pli->nSelTo����pli->nLenʱ������һ�е�ѡ��������з�
  if ( pli->nSelTo > pli->nLen ) {
    pt.x += pc.m_nCharWidthLineBreak;
  }
  // ������ı���
  if ( ( x2 > pt.x ) && ( pSelEnd >= pLineEnd ) ) {
    DrawLineBkgnd( nLine, pli, pt.x, pDC, pc );
  }
  // ������п�ѡ����ѡ�������Ѿ��Ǳ��������ַ����Լ�ѡ�������ұ߱��п鷶Χ���ұ�С��
  // �����ѡ���������һ���ַ����п鷶Χ�ұߵļ�϶����ʹ��������һ����
  if ( IsColumnSelect()
       && ( x2 > pt.x )
       && ( pt.x < pc.m_rectColSelect.right )
       && ( pli->nSelTo >= pli->nLen ) ) {
    ::SetRect( &rect, max( pc.m_rectColSelect.left, pt.x ), pc.m_rectText.top, pc.m_rectColSelect.right, pc.m_rectText.bottom );
    ::SetBkColor( hDC, pc.COLOR( kcBkTextSelected ) );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
void CPfxEditView::DrawHilighted( HDC hdcSrc, CPfxEditViewPaintContext& pc ) {
  CRect rect;
  CDC hdcMem;
  hdcMem.CreateCompatibleDC( hdcSrc );
  hdcMem.SetBkMode( TRANSPARENT );
  CBitmap bitmap;
  bitmap.CreateCompatibleBitmap( hdcSrc, pc.m_rectUpdate.Width(), pc.m_rectUpdate.Height() );
  hdcMem.SelectBitmap( bitmap );
  HDC hDC = hdcMem;
  BOOL bWrapping = IsWrapping();
  CPoint pt;
  pt.x = ( pc.m_rectText.left - pc.m_nTextHorzOffset );
  pt.y = ( pc.m_rectText.top + pc.m_nSpacingLines );
  for ( LONG i = pc.m_nFirstUpdateLine; i <= pc.m_nLastUpdateLine; i++ ) {
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
    // ���һ���ı�
    pc.m_ptCurPos.x = pt.x;
    pc.m_ptCurPos.y = pt.y;
    ::SetTextAlign( hDC, TA_LEFT | TA_TOP | TA_UPDATECP );
    ::MoveToEx( hDC, pt.x, pt.y, NULL );
    if ( ( i <= m_nSelLineTo ) && ( i >= m_nSelLineFrom ) ) {
      BOOL bNeedUpdateCurrentPosition = false;
      if ( ( pli->nSelFrom > 0 ) || ( pli->nSelTo < pli->nLen ) ) {
        DrawLineBkgnd( i, pli, pc.m_rectText.left, hdcMem, pc );
        DrawHilightedNormal( i, pli, hdcMem, pc );
        //  �ָ�ԭ�������꣬��Ϊ����ѡ�񲿷ֵĴ����
        //  ���ͷ��ʼ����
        ::MoveToEx( hDC, pt.x, pt.y, NULL );
        bNeedUpdateCurrentPosition = true;
      }
      DrawHilightedSelection( i, pli, hdcMem, pc );
      // ��Ϊ��ѡ�񲿷����������һ���У�Ȼ���������渲��
      // ѡ�����ݣ�������������Ҫ���ƻ��з����û��з���
      // ������ĩ������Ҫ������ĩλ��
      if ( !bNeedUpdateCurrentPosition ) {
        ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      } else {
        ::MoveToEx( hDC, pc.m_ptCurPos.x, pc.m_ptCurPos.y, NULL );
      }
    } else {
      DrawLineBkgnd( i, pli, pc.m_rectText.left, hdcMem, pc );
      // �����һ��û��ѡ���ı���һ�ι������Լӿ��ٶ�
      if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
        DrawHilightedNormal( i, pli, hdcMem, pc );
      }
    }
    // ���ƻس���
    if ( pc.m_bDrawLineBreak ) {
      DrawLineBreak( i, pli, hdcMem, pc );
    }
    // �����к�
    DrawLineNumber( ( ( !PFX_EC_IS_SUBLINE( pli ) && !bWrapping ) ? i : pli->nIndex ), hdcMem, pc );
    // Ϊ�п�ѡ�����֧��
    if ( ( i <= pc.m_rectColSelect.bottom ) && ( i >= pc.m_rectColSelect.top ) ) {
      ::SetRect( &rect, pc.m_rectColSelect.left, pc.m_rectText.top, pc.m_rectColSelect.right, pc.m_rectText.bottom );
      ::InvertRect( hDC, &rect );
    }
    // �������Ļ
    ::BitBlt( hdcSrc, pc.m_rectUpdate.left, pc.m_rectUpdate.top, pc.m_sizeBitmap.cx, pc.m_sizeBitmap.cy, hDC, 0, 0, SRCCOPY );
    // ��һ��
    pc.m_rectUpdate.top += pc.m_sizeBitmap.cy;
  }
  // ���δ���Ƶ�ʣ����Ļ����
  if ( pc.m_rectUpdate.top < pc.m_rectUpdate.bottom ) {
    hDC = hdcSrc;
    // ����к�������
    i = ( pc.m_rectLineNumber.right - pc.m_rectIndent.left );
    if ( i > 0 )           //      �������ʾ�кžͲ���Ҫ���
    {
      ::SetBkColor( hDC, pc.COLOR( kcBkLineNumber ) );
      ::SetRect( &rect, 0, pc.m_rectUpdate.top, i, pc.m_rectUpdate.bottom );
      ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
    }
    // ����ı�������
    ::SetBkColor( hDC, pc.COLOR( kcBkText ) );
    ::SetRect( &rect, i, pc.m_rectUpdate.top, pc.m_rectText.right, pc.m_rectUpdate.bottom );
    ::ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
  }
}
// CPfxEditView ��������
LONG CPfxEditView::TransViewPosToTextPosX( LONG xViewPos ) const {
  LONG x = ( ( m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve ) + ( xViewPos - m_rectText.left ) );
  return max( 0, x );
}
LONG CPfxEditView::TransViewPosToTextPosY( LONG yViewPos ) const {
  LONG y = ( ( m_ptFirstVisible.y * PFX_EC_DATA.m_nLineHeight ) + ( yViewPos - m_rectText.top ) );
  return max( 0, y );
}
LONG CPfxEditView::TransTextPosToViewPosX( LONG xTextPos ) const {
  ASSERT( xTextPos >= 0 );
  return m_rectText.left + ( xTextPos - ( m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve ) );
}
LONG CPfxEditView::TransTextPosToViewPosY( LONG yTextPos ) const {
  ASSERT( yTextPos >= 0 );
  return m_rectText.top + ( yTextPos - ( m_ptFirstVisible.y * PFX_EC_DATA.m_nLineHeight ) );
}
// ������ָ������Ӧ���ı����꣬��������ı�Ӱ��Ĵ�ֱ������㿪ʼ����
LONG CPfxEditView::TransLinePosToTextPos( LONG nLine ) const {
  ASSERT( m_pdb->TextIsValidLine( nLine ) );
  return ( nLine * PFX_EC_DATA.m_nLineHeight );
}
// ������ָ������ĳ�����Ӧ���ı����꣬��������ı�Ӱ���ˮƽ������㿪ʼ����
// ���rpt.x��������ȷ��λ���ϣ�������ͼ�����ò���
LONG CPfxEditView::TransCharPosToTextPos( CPoint& rpt ) const {
  ASSERT( m_pdb->TextIsValidLine( rpt.y ) );
  ASSERT( m_pdb->TextIsValidChar( rpt.y, rpt.x ) );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( rpt.y );
  if ( ( rpt.x == 0 ) || PFX_EC_IS_EMPTY_LINE( pli ) ) {
    return 0;
  }
  // �����ﲻ��ʹ��GetTabbedTextExtent()����Ϊ��������������������
  // ������������KERNINGPAIR�Ե�ԭ��ʹ�����ص���ֵ����ͼ�Ļ�ͼ���
  // ģ������������ַ������Ȳ�һ�£������������ֻ�ܷ���
  // USHRT_MAX(65535)���߼���Ԫ���������ֵ���޲����������ǵ�Ҫ����
  // ��ؼ�����һ�е���󳤶���8192��Win9x�ı����������������ޣ���
  // ��ÿ���ַ��Ŀ����10���߼���Ԫ����GetTabbedTextExtent()�Ͳ���ʤ
  // ���ˣ��������������Ǳ���ʹ���Լ������ͼ���ģ��һ�µļ��㷽����
  // ʹ����ͼ�����������λ�Ǻ�
  CClientDC1 dc( const_cast<CPfxEditView*>( this ) );
  HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
  LPCTSTR pLineStart = pli->pData;
  LPCTSTR pLineEnd = pLineStart + pli->nLen;
  LPCTSTR pCur = pLineStart;
  LPCTSTR pEnd = pLineStart + min( rpt.x, pli->nLen );
  LONG nLogChar = pli->nFirstChar;
  LONG nTestPos = 0;
  while ( pCur < pEnd ) {
    if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      pCur += PFX_EC_MAX_MB_LEN;
      nLogChar += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( *pCur != PFX_EC_CHR_TAB ) {
      pCur++;
      nLogChar++;
    } else {
      LONG nTabSize = GetTabSize() - ( nLogChar % GetTabSize() );
      nTestPos += ( ( nTabSize * PFX_EC_DATA.m_nCharWidthSpace ) - PFX_EC_DATA.m_nCharWidthTab );
      pCur++;
      nLogChar += nTabSize;
    }
  }
  // ��������������ĩ�����գ��������Ա���ԭ����rpt.x����һ��MBCS�ַ���
  // �м�����µĴ���
  rpt.x = min( pCur, pLineEnd ) - pLineStart;
  CSize size = dc.GetTextExtent( pLineStart, rpt.x );
  nTestPos += size.cx;
  dc.SelectObject( hFont );
  return nTestPos;
}
// ��������Ĵ�ֱY����������һ���ϣ������ظ��е������
// LONG yTextPos - ��������ı�Ӱ��ֱ������ʼ������귵����Ӧ�������
LONG CPfxEditView::TransTextPosToLinePos( LONG yTextPos ) const {
  ASSERT( yTextPos >= 0 );
  return min( ( yTextPos / PFX_EC_DATA.m_nLineHeight ), m_pdb->TextGetLineUpperBound() );
}
// ������һ���и�����ˮƽ����X�����ĸ��ַ��ϣ������ظ��ַ���X�ı������
// ���ַ���ƫ����
// LONG nLine     - �����
// LONG rxTextPos - ��������ı�Ӱ��ˮƽ������ʼ������꣬�������غ�õط�����뷵��ֵ������ţ�
//               ���Ӧ��������ı�Ӱ��ˮƽ������ʼ������꣬������ͨ�����ڶ�λ�����
// LONG& rxTextPos ���ո��ַ���ˮƽ����
// ������xTextPos���Ӧ���ַ�������ţ���TCHARΪ��λ��
// �����������¹�����л��㣬���x����ĳ���ַ���ǰ�벿�֣��򷵻ظ��ַ�
// �����кţ�����Ϊ��ֵ�������򷵻���һ���ַ�����ĩ���к�
LONG CPfxEditView::TransTextPosToCharPos( LONG nLine, LONG& rxTextPos ) const {
  ASSERT( rxTextPos >= 0 );
  ASSERT( m_pdb->TextIsValidLine( nLine ) );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
  LPCTSTR pStart = pli->pData;
  LPCTSTR pCur = pStart;
  LONG nTestPos = 0;
  if ( ( rxTextPos > 0 ) && !PFX_EC_IS_EMPTY_LINE( pli ) ) {
    nTestPos = rxTextPos;
    LONG nTestLen = nTestPos / PFX_EC_DATA.m_nCharWidthMax;
    nTestLen = min( nTestLen, ( PFX_EC_MAX_LINE_LENGTH / 50 ) );
    nTestLen = max( nTestLen, 1 );
    LPCTSTR pEnd = pStart + pli->nLen;
    CClientDC1 dc( const_cast<CPfxEditView*>( this ) );
    HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
    CalcCharsToFit( pCur, nTestPos, dc, pStart, pEnd, 0, nTestLen, pli->nFirstChar );
    dc.SelectObject( hFont );
    pCur = min( pCur, pEnd );
  }
  rxTextPos = nTestPos;
  return ( pCur - pStart );
}
CPoint CPfxEditView::TransViewPosToEditPos( CPoint pt ) const {
  CPoint ptText;
  ptText.x = TransViewPosToTextPosX( pt.x );
  ptText.y = TransViewPosToTextPosY( pt.y );
  CPoint ptEdit;
  ptEdit.y = TransTextPosToLinePos( ptText.y );
  ptEdit.x = TransTextPosToCharPos( ptText.y, ptText.x );
  m_pdb->TextSubLineToAbsLine( ptEdit );
  return ptEdit;
}
CPoint CPfxEditView::TransEditPosToViewPos( CPoint pt ) const {
  CPoint ptEdit;
  ptEdit.y = m_pdb->GetValidLine( pt.y );
  ptEdit.x = m_pdb->GetValidChar( ptEdit.y, pt.x );
  m_pdb->TextAbsLineToSubLine( ptEdit );
  CPoint ptText;
  ptText.y = TransLinePosToTextPos( ptEdit.y );
  ptText.x = TransCharPosToTextPos( ptEdit );
  CPoint ptView;
  ptView.x = TransTextPosToViewPosX( ptText.x );
  ptView.y = TransTextPosToViewPosY( ptText.y );
  return ptView;
}
CPoint CPfxEditView::TransCharOfsToEditPos( LONG nOffset ) const {
  nOffset = max( nOffset, 0 );
  nOffset = min( nOffset, GetDocSize() );
  CPoint ptEdit( 0, 0 );
  m_pdb->TextOffsetToSubLine( nOffset, ptEdit );
  m_pdb->TextSubLineToAbsLine( ptEdit );
  return ptEdit;
}
LONG CPfxEditView::TransEditPosToCharOfs( CPoint pt ) const {
  CPoint ptEdit;
  ptEdit.y = m_pdb->GetValidLine( pt.y );
  ptEdit.x = m_pdb->GetValidChar( ptEdit.y, pt.x );
  m_pdb->TextAbsLineToSubLine( ptEdit );
  return m_pdb->TextSubLineToOffset( ptEdit );
}
// ��һ�еı༭�����Ҫ���ĳЩ�հ��ַ���������������Ҫ�����ַ�����������
// Ϊ��Ҫ����TAB��չ��ԭ���������Ҫ���10���հ��ַ�����һ����Ҫ10��TCHAR
// �ռ䣬��Ϊ������а���TAB�ַ��Ļ���������TCHAR�ռ���㹻��
LONG CPfxEditView::FillBufferWithWhitespacesCalcBufferSize( LONG nStartLogChar, LONG nSpacesToFill ) {
  if ( ( nStartLogChar < 0 ) || ( nSpacesToFill <= 0 ) ) {
    return 0;
  }
  if ( IsExpandTabToSpaceChars() ) {
    return nSpacesToFill;
  }
  for ( LONG nBufSize = 0, nTabSize = GetTabSize(); nSpacesToFill > 0; ) {
    LONG nSpaces = nTabSize - ( nStartLogChar % nTabSize );
    nBufSize += ( ( nSpacesToFill >= nSpaces ) ? 1 : nSpacesToFill );
    nStartLogChar += nSpaces;
    nSpacesToFill -= nSpaces;
  }
  return nBufSize;
}
// ʹ�ÿո��TAB���ָ���Ļ�����
// LPTSTR pBuffer - �û����������㹻�󣬲�С��FillBufferWithWhitespacesCalcBufferSize()���ص���ֵ��С
// LONG nBufLen   - ���������ȣ������������ֻ�ǴӰ�ȫ�Ƕȳ���
void CPfxEditView::FillBufferWithWhitespacesDoFill( LONG nStartLogChar, LONG nSpacesToFill, LPTSTR pBuffer, LONG nBufLen ) {
  if ( ( pBuffer == NULL ) || ( nBufLen <= 0 ) || ( nSpacesToFill <= 0 ) || ( nStartLogChar < 0 ) ) {
    return ;
  }
  if ( IsExpandTabToSpaceChars() ) {
    nSpacesToFill = min( nSpacesToFill, nBufLen );
    for ( LONG i = 0; i < nSpacesToFill; i++ ) {
      pBuffer[ i ] = PFX_EC_CHR_SPACE;
    }
    return ;
  }
  // ��nStartLogChar����һ��TABλ���а������ٸ��ո������nTabSize��
  // ����һ��TAB���棬�����������Ŀ�Ŀո�
  LPTSTR pEnd = pBuffer + nBufLen;
  for ( LONG nTabSize = GetTabSize(); ( nSpacesToFill > 0 ) && ( pBuffer < pEnd ); /*empty*/ ) {
    LONG nSpaces = nTabSize - ( nStartLogChar % nTabSize );
    if ( nSpacesToFill >= nSpaces ) {
      *pBuffer++ = PFX_EC_CHR_TAB;
    } else {
      LPTSTR s = min( ( pBuffer + nSpacesToFill ), pEnd );
      while ( pBuffer < s ) {
        *pBuffer++ = PFX_EC_CHR_SPACE;
      }
    }
    nStartLogChar += nSpaces;
    nSpacesToFill -= nSpaces;
  }
}
// ������ĳ���и�����ˮƽ��Χ���ж��ٸ��ַ�
void CPfxEditView::CalcCharsToFit( LPCTSTR& rpCharRet, LONG& rxTextPos, HDC dc, LPCTSTR pStart, LPCTSTR pEnd, LONG nTestPos, LONG nTestLen, LONG nLogChar ) const {
  LONG nLogCharSaved;
  LONG nTestPosSaved;
  CSize size;
  LONG nTabSize = size.cx = 0;
  LONG nTestLenSaved = nTestLen;
  LONG xTextPos = rxTextPos;
  LPCTSTR pCur = pStart;
  while ( pCur < pEnd ) {
    nLogCharSaved = nLogChar;
    nTestPosSaved = nTestPos;
    nTestLen = nTestLenSaved;
    do {
      if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
        nTestLen--;
        pCur += PFX_EC_MAX_MB_LEN;
        nLogChar += PFX_EC_MAX_MB_LEN;
        continue;
      }
      if ( *pCur != PFX_EC_CHR_TAB ) {
        nTestLen--;
        pCur++;
        nLogChar++;
      } else {
        nTabSize = GetTabSize() - ( nLogChar % GetTabSize() );
        nTestPos += ( nTabSize * PFX_EC_DATA.m_nCharWidthSpace - PFX_EC_DATA.m_nCharWidthTab );
        nTestLen -= nTabSize;
        pCur++;
        nLogChar += nTabSize;
      }
    } while ( ( pCur < pEnd ) && ( nTestLen > 0 ) );
    pCur = min( pCur, pEnd );
    ::GetTextExtentPoint32( dc, pStart, ( pCur - pStart ), &size );
    nTestPos += size.cx;
    if ( nTestPos >= xTextPos ) {
      break;
    }
    pStart = pCur;
  }
  // ����Ѳ�õľ�����ڸ����ľ��벢������֮���а�������һ���ַ�����
  // �����Ҫ������һ������õ�һ���ַ������Ը�С�Ĳ���ֵ���¼�⣬��
  // �������NUM_MAX_LINE_LENGTHΪǧ����ֵʱ��෢������
  if ( ( nTestPos > xTextPos ) && ( GetNextCharPos( pStart, pEnd ) < pCur ) ) {
    nTestLen = max( 1, ( nTestLenSaved / 10 ) );
    CalcCharsToFit( rpCharRet, rxTextPos, dc, pStart, pEnd, nTestPosSaved, nTestLen, nLogCharSaved );
    return ;
  }
  if ( nTestPos > xTextPos ) {
    // ����Ӧ����ÿ��ǰ��һ���ַ���ʱ���������ַ����Ʊ����
    // ��Ҫ��ȡ����ʵ�ʿ��
    if ( *pStart == PFX_EC_CHR_TAB ) {
      size.cx = ( nTabSize * PFX_EC_DATA.m_nCharWidthSpace );
    }
    // ��������ַ��ĺ�벿��ȡ��һ���ַ�������ȡ��ǰ�ַ�
    if ( ( nTestPos - xTextPos ) > ( size.cx / 2 ) ) {
      pCur = pStart;
      nTestPos = nTestPosSaved;
    }
  }
  rpCharRet = min( pCur, pEnd );
  rxTextPos = nTestPos;
}
// LONG CalcCharsToCopy(LPCTSTR pData, LONG cch) const
// ��ĳЩ����GetSelData()�Ȳ����У��û������˻������ĳ��ȣ����û�������
// ��С�ڽ�Ҫ���Ƶ��ַ�ʱ��Ҫ�ض��ַ����ض�λ�ÿ�����һ��MBCS�ַ����м�
// λ�ã����������Ӧ�÷�����������ִ���жϿ��ض�λ���Ƿ���MBCS�ַ�����
// �䣬���������㷵����ȷ�Ŀɸ��Ƴ��ȣ���֤���Ḵ��MBCS�ַ���һ��
// LPCTSTR pData - Ҫ�����ַ������׵�ַ��һ����һ�еĿ�ʼ��ַ
// LONG cch - Ҫ���Ƶĳ��ȣ���������ַ����ض��򷵻�ֵһ����(cch - 1)
// ������ȷ�ĸ��Ƴ��ȣ�����������ȵ��ַ���֤����Ͽ�MBCS�ַ�
LONG CPfxEditView::CalcCharsToCopy( LPCTSTR pData, LONG cch ) const {
  if ( ( pData == NULL ) || ( cch <= 0 ) ) {
    return 0;
  }
#ifdef _UNICODE
  return cch;
#else
  for ( LPCTSTR pCur = pData, pEnd = pData + cch; pCur < pEnd; ) {
    if ( !PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      pCur++;
    } else {
      pCur += PFX_EC_MAX_MB_LEN;
      if ( pCur > pEnd ) {
        pCur -= PFX_EC_MAX_MB_LEN;
        break;
      }
    }
  }
  return ( pCur - pData );
#endif
}
// �����ָ���е����׵�����ĳ�ַ�֮��Ŀո���
// CPoint pt - �����㵽���λ�ã���������λ��֮ǰ�зǿհ��ַ��򵽵�
//          һ���ǿհ��ַ���ֹͣ�������������λ��
// �����׵�ָ��λ��֮��Ŀո���
LONG CPfxEditView::CalcLeadingWhitespaces( CPoint pt ) const {
  if ( !m_pdb->TextIsValidLine( pt.y ) ) {
    return 0;
  }
  m_pdb->TextSubLineToMainLine( pt );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  LPCTSTR pCur = pli->pData;
  const LPCTSTR pEnd = pCur + min( pt.x, pli->nActLen );
  LONG nSpaces = 0;
  LONG nTabSize = GetTabSize();
  while ( pCur < pEnd ) {
    TCHAR tc = *pCur;
    if ( PFX_EC_CHR_SPACE == tc ) {
      pCur++;
      nSpaces++;
    } else if ( PFX_EC_CHR_TAB == tc ) {
      pCur++;
      nSpaces += ( nTabSize - ( nSpaces % nTabSize ) );
    } else {
      break; // �����ǿհ��ַ���ֹͣ��
    }
  }
  return nSpaces;
}
LONG CPfxEditView::CalcLogicalChar( LPCTSTR pStart, LPCTSTR pEnd, LONG nTabSize, LONG nLogChar ) const {
  while ( pStart < pEnd ) {
    if ( PFX_EC_IS_LEAD_BYTE( *pStart ) ) {
      pStart += PFX_EC_MAX_MB_LEN;
      nLogChar += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( *pStart != PFX_EC_CHR_TAB ) {
      pStart++;
      nLogChar++;
    } else {
      pStart++;
      nLogChar += ( nTabSize - ( nLogChar % nTabSize ) );
    }
  }
  return nLogChar;
}
// ����һ����ָ����Ŀ�������ַ��Ŀ��֮��
// LPCTSTR pData   - �ַ�����ָ��
// LONG cch        - Ҫ������ַ���������λ��TCHAR
// LONG& rnLogChar - ���һ���ַ���Ӧ���߼������������ֵ��Ҫ�����������Ʊ���Ŀ��
// HDC hDC         - HDC ָ��
// �ɹ����ظ��ַ����Ŀ�ȣ����򷵻����ַ����п��԰������Ʊ��
LONG CPfxEditView::CalcStringWidth( LPCTSTR pData, LONG cch, LONG& rnLogChar, HDC hDC ) const {
  CDCH pDC = hDC;
  ASSERT_VALID( this );
  if ( ( pData == NULL ) || ( cch == 0 ) ) {
    return 0;
  }
  LPCTSTR pCur = pData;
  LPCTSTR pEnd = pData + cch;
  LONG nWidth = 0;
  while ( pCur < pEnd ) {
    if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      pCur += PFX_EC_MAX_MB_LEN;
      rnLogChar += PFX_EC_MAX_MB_LEN;
      continue;
    }
    if ( *pCur != PFX_EC_CHR_TAB ) {
      pCur++;
      rnLogChar++;
    } else {
      LONG nTabSize = GetTabSize();
      nTabSize = nTabSize - ( rnLogChar % nTabSize );
      nWidth += ( nTabSize * PFX_EC_DATA.m_nCharWidthSpace - PFX_EC_DATA.m_nCharWidthTab );
      pCur++;
      rnLogChar += nTabSize;
    }
  }
  if ( pCur > pEnd ) {
    rnLogChar--;
  }
  CSize size( 0, 0 );
  if ( pDC != NULL ) {
    size = pDC.GetTextExtent( pData, cch );
  } else {
    CClientDC1 dc( const_cast<CPfxEditView*>( this ) );
    HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
    size = dc.GetTextExtent( pData, cch );
    dc.SelectObject( hFont );
  }
  return ( nWidth + size.cx );
}
// �����������еĵ��߼��ַ�λ�ã�TAB��չ��ת���ɾ����ַ�λ�ã�ָ���дλ�ã�
// LONG nLine          - ����������
// LONG nLogCharToTest - Ҫת�����߼��ַ�λ��
LONG CPfxEditView::CalcAbsCharPosFromLogCharPos( LONG nLine, LONG nLogCharToTest ) const {
  if ( !m_pdb->IsValidLine( nLine ) ) {
    return 0;
  }
  // ��������ת�����������λ��
  CPoint pt( 0, nLine );
  m_pdb->TextAbsLineToMainLine( pt );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  LPCTSTR pStart = pli->pData;
  LPCTSTR pCur = pStart;
  LPCTSTR pEnd = pStart + pli->nActLen;
  LONG nTabSize = GetTabSize();
  LONG nLogChar = 0;
  while ( pCur < pEnd ) {
    if ( PFX_EC_CHR_TAB != *pCur ) {
      nLogChar++;
    } else {
      nLogChar += ( nTabSize - ( nLogChar % nTabSize ) );
    }
    if ( nLogChar > nLogCharToTest ) {
      break;
    }
    pCur++;
  }
  return ( pCur - pStart );
}
// ����һ�кź�����������ָ�������Ƿ���һ��MBCS�ַ��ĺ�벿�֣�������򷵻���һ���ַ���λ��
void CPfxEditView::VerifyPointerPosition( CPoint& rpt ) const {
  ASSERT( m_pdb->TextIsValidLine( rpt.y ) );
  ASSERT( m_pdb->TextIsValidChar( rpt.y, rpt.x ) );
  // ������Ƿ�Ϸ���������Ϸ��;���
  rpt.y = m_pdb->TextVerifyLine( rpt.y );
  rpt.x = max( 0, rpt.x );
  // ������Ƿ�Ϸ����Ƿ�����һMBCS�ַ����м䣬�������;���
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( rpt.y );
  LPCTSTR pLineStart = pli->pData;
  LPCTSTR pLineEnd = pLineStart + pli->nLen;
  LPCTSTR pCur = pLineStart;
  LPCTSTR pEnd = pLineStart + min( pli->nLen, rpt.x );
  while ( pCur < pEnd ) {
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  rpt.x = ( min( pCur, pLineEnd ) - pLineStart );
}
// ��鵱ǰ�༭λ���Ƿ�Ϸ�
void CPfxEditView::VerifyCurrentPosition() {
  // ��֤��
  LONG nLineUpperBound = m_pdb->TextGetLineUpperBound();
  if ( m_ptEditPos.y < 0 ) {
    m_ptEditPos.y = 0;
  } else if ( m_ptEditPos.y > nLineUpperBound ) {
    m_ptEditPos.y = nLineUpperBound;
  }
  // ��֤��
  LONG nCharUpperBound = m_pdb->TextGetLineLength( m_ptEditPos.y );
  if ( m_ptEditPos.x < 0 ) {
    m_ptEditPos.x = 0;
  } else if ( m_ptEditPos.x > nCharUpperBound ) {
    m_ptEditPos.x = nCharUpperBound;
  }
  // �׸��ɼ���
  if ( m_ptFirstVisible.y < 0 ) {
    m_ptFirstVisible.y = 0;
  } else if ( m_ptFirstVisible.y > nLineUpperBound ) {
    m_ptFirstVisible.y = nLineUpperBound;
  }
}
// ˢ����nFirstUpdateLine��nLastUpdateLine֮��������ڵ�����
// LONG nFirstUpdateLine - Ҫˢ�µĵ�һ�У�����Ϊ��ֵ��
// LONG nLastUpdateLine  - Ҫˢ�µ������
void CPfxEditView::UpdateLines( LONG nFirstUpdateLine, LONG nLastUpdateLine ) {
  if ( nFirstUpdateLine > nLastUpdateLine ) {
    LONG n = nLastUpdateLine;
    nLastUpdateLine = nFirstUpdateLine;
    nFirstUpdateLine = n;
  }
  nFirstUpdateLine = m_pdb->TextVerifyLine( nFirstUpdateLine );
  nLastUpdateLine = m_pdb->TextVerifyLine( nLastUpdateLine );
  LONG nFirstVisibleLine = m_ptFirstVisible.y;
  LONG nLastVisibleLine = m_ptFirstVisible.y + m_sizeVisible.cy;
  // ���Ҫ���µ��ж����ڿɼ���Χ�ھͲ���Ҫ����
  if ( ( nFirstUpdateLine > nLastVisibleLine )
       || ( nLastUpdateLine < nFirstVisibleLine ) ) {
    return ;
  }
  if ( nFirstUpdateLine < nFirstVisibleLine ) {
    nFirstUpdateLine = nFirstVisibleLine;
  }
  if ( nLastUpdateLine > nLastVisibleLine ) {
    nLastUpdateLine = nLastVisibleLine;
  }
  CRect rect;
  rect.left = m_rectClient.left;
  rect.right = m_rectClient.right;
  rect.top = m_rectText.top + ( ( nFirstUpdateLine - nFirstVisibleLine ) * PFX_EC_DATA.m_nLineHeight );
  rect.bottom = rect.top + ( ( nLastUpdateLine - nFirstUpdateLine + 1 ) * PFX_EC_DATA.m_nLineHeight );
  InvalidateRect( &rect, false );
}
// ִ��ѡ������Ժ�����SetSelRange()������ñ������������ñ�����ˢ�¸�
// ��ĵط������ֲ���Ҫȫ��ˢ��
// LONG nOldSelLineFrom - ���µ�ѡ�����֮ǰ��ѡ��Χ�ĵ�һ��
// LONG nOldSelLineTo   - ���µ�ѡ�����֮ǰ��ѡ��Χ�������
// ���nOldSelLineFrom����PFX_EC_INVALID_SELECT��ʾ����֮ǰû��ѡ���ı�
// ���nNewSelLineFrom����PFX_EC_INVALID_SELECT��ʾ����֮��û��ѡ���ı�
void CPfxEditView::UpdateLinesSelected( LONG nOldSelLineFrom, LONG nOldSelLineTo ) {
  LONG nFirstUpdateLine;
  LONG nLastUpdateLine;
  LONG nNewSelLineFrom = m_nSelLineFrom;
  LONG nNewSelLineTo = m_nSelLineTo;
  if ( ( nOldSelLineFrom == PFX_EC_INVALID_SELECT )
       || ( nNewSelLineFrom == PFX_EC_INVALID_SELECT ) ) {
    // �������֮ǰû��ѡ�񣬲���֮��Ҳû��ѡ��
    if ( ( nOldSelLineFrom == PFX_EC_INVALID_SELECT )
         && ( nNewSelLineFrom == PFX_EC_INVALID_SELECT ) ) {
      return ;
    }
    // ����֮ǰû��ѡ���ı���ֻ����²���֮��ķ�Χ
    else if ( nOldSelLineFrom == PFX_EC_INVALID_SELECT ) {
      nFirstUpdateLine = nNewSelLineFrom;
      nLastUpdateLine = nNewSelLineTo;
    }
    // ����֮��û��ѡ���ı���ֻ����²���֮ǰ�ķ�Χ
    else // (nNewSelLineFrom == PFX_EC_INVALID_SELECT)
    {
      nFirstUpdateLine = nOldSelLineFrom;
      nLastUpdateLine = nOldSelLineTo;
    }
  } else {
    // �������������ӻ����ѡ��Χ��ֻ����¸ı��˵���
    if ( ( nOldSelLineFrom == nNewSelLineFrom )
         && ( nNewSelLineTo == m_ptEditPos.y ) ) {
      nFirstUpdateLine = min( nOldSelLineTo, nNewSelLineTo );
      nLastUpdateLine = max( nOldSelLineTo, nNewSelLineTo );
    }
    // �������������ӻ����ѡ��Χ��ֻ����¸ı��˵���
    else if ( ( nOldSelLineTo == nNewSelLineTo )
              && ( nNewSelLineFrom == m_ptEditPos.y ) ) {
      nFirstUpdateLine = min( nOldSelLineFrom, nNewSelLineFrom );
      nLastUpdateLine = max( nOldSelLineFrom, nNewSelLineFrom );
    } else {
      nFirstUpdateLine = min( nOldSelLineFrom, nNewSelLineFrom );
      nLastUpdateLine = max( nOldSelLineTo, nNewSelLineTo );
    }
    nFirstUpdateLine -= ( ( ( nFirstUpdateLine - 1 ) == m_nSelLineFrom ) ? 1 : 0 );
    nLastUpdateLine += ( ( ( nLastUpdateLine + 1 ) == m_nSelLineTo ) ? 1 : 0 );
  }
  UpdateLines( nFirstUpdateLine, nLastUpdateLine );
}
// �����ߡ��кš����ĵ����ڵ�����
void CPfxEditView::UpdateLayout() {
  // Layout
  // |=========================================== |
  // |   ruler(or indent if not display ruler)    |
  // | -------|---|-------------------------------|
  // |        | i |                               |
  // |        | n |                               |
  // | line   | d |                               |
  // | number | e |            text               |
  // |        | n |                               |
  // |        | t |                               |
  // |========|===|===============================|
  LONG nTextIndentLeft;
  LONG nTextIndentTop;
  LONG nLineNumberWidth;
  CRect rect;
  rect.CopyRect( &m_rectClient );
  // ������ʾ���ʱ��rectRuler��ռ�ݵ�������ʵ��һ�հ�����Ҳ���ǵ�һ
  // �����ֵ���ͼ�ϱ�֮��Ŀհ���������Ϊ�˲�����ʹ��һ�й��ֽ�����ͼ
  // �ϱ߶��ճ���������
  if ( m_pdb->m_bDrawRuler ) {
    nTextIndentTop = PFX_EC_DATA.m_nRulerHeight;
    nTextIndentLeft = max( PFX_EC_MIN_TEXT_INDENT_LEFT, ( PFX_EC_DATA.m_nRulerCharWidthAve / 2 ) );
  } else {
    nTextIndentTop = PFX_EC_MIN_TEXT_INDENT_TOP;
    nTextIndentLeft = PFX_EC_MIN_TEXT_INDENT_LEFT;
  }
  m_rectRuler.SetRect( rect.left, rect.top, rect.right, nTextIndentTop );
  // ������ʾ�к�ʱ��rectLineNumber��ռ�ݵ�������ʵ��һ�հ���������Ϊ
  // �˲�����ʹ��ͼ����߹��ֽ����ı����ĵ�һ���ַ�������
  // �����к������ж���λ���кſ�ȵ�������λ��������������ַ����
  if ( m_pdb->m_bDrawLineNumber ) {
    nTextIndentLeft = max( nTextIndentLeft, PFX_EC_DATA.m_nSpacingLineNumberAndText );
    for ( LONG i = GetLineCount(), nLineNumberChars = 1; i >= 10; nLineNumberChars++ ) {
      i /= 10;
    }
    // ��5��Ϊ��ʹ��һ���к������봰����߿���һ�����
    i = ( nLineNumberChars * PFX_EC_DATA.m_nCharWidthMaxNum + ( PFX_EC_LINE_NUMBER_INDENT_LEFT + PFX_EC_LINE_NUMBER_INDENT_RIGHT ) );
    nLineNumberWidth = max( i, PFX_EC_LINE_NUMBER_MIN_WIDTH );
  } else {
    nLineNumberWidth = 0;
  }
  m_rectLineNumber.SetRect( rect.left, m_rectRuler.bottom, nLineNumberWidth + nTextIndentLeft, rect.bottom );
  // �ı���
  m_rectText.SetRect( m_rectLineNumber.right, m_rectLineNumber.top, rect.right, rect.bottom );
  // ����������ʾ���������һ�����������ȫ�ɼ�����в�����
  m_sizeVisible.cy = m_rectText.Height() / PFX_EC_DATA.m_nLineHeight;
  m_pdb->m_rectIndent.top = nTextIndentTop;
  m_pdb->m_rectIndent.left = nTextIndentLeft;
}
void CPfxEditView::UpdateCursor( CPoint pt ) {
  HCURSOR hCursor;
  if ( IsTracking() ) {
    if ( !IsColumnSelectTracking() || !IsCanColumnSelect() ) {
      hCursor = PFX_EC_DATA.m_hCurText;
    } else {
      hCursor = PFX_EC_DATA.m_hCurColSelect;
    }
  } else {
    switch ( GetHitTestCode( pt ) ) {
    case PFX_EC_HT_RULER:
    case PFX_EC_HT_LINE_NUMBER:
      hCursor = PFX_EC_DATA.m_hCurStandard;
      break;
    case PFX_EC_HT_TEXT:
    default:
      // ���ALT �������ҵ�ǰ�����Զ�����ģʽ���򽫹����
      // �ó��п�ѡ�����ģʽ
      if ( ( IsColumnSelectTracking() || PFX_EC_IS_KEY_DOWN( VK_MENU ) )
           && IsCanColumnSelect() ) {
        hCursor = PFX_EC_DATA.m_hCurColSelect;
      } else {
        hCursor = PFX_EC_DATA.m_hCurText;
      }
      break;
    }
  }
  SetCursor( hCursor );
}
// ���²����λ�ã����¼��㵱ǰ�߼��ַ�λ��
// x ���߼����꣬���Ǵ��ı�����߿�ʼ���㣬�����ǴӴ�����߿�ʼ����
void CPfxEditView::UpdateCurrentPosition( LONG xTextPos, BOOL bUpdateCurCharPosition, BOOL bUpdateCaretReferPoint ) {
  ASSERT( xTextPos >= 0 );
  m_ptTextPos.x = xTextPos;
  m_ptTextPos.y = TransLinePosToTextPos( m_ptEditPos.y );
  if ( bUpdateCaretReferPoint ) {
    m_ptCaretRefer.y = 0; // unused
    m_ptCaretRefer.x = xTextPos;
  }
  if ( bUpdateCurCharPosition ) {
    const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( m_ptEditPos.y );
    m_ptLogiPos.y = 0; // unused
    m_ptLogiPos.x = CalcLogicalChar( pli->pData, pli->pData + m_ptEditPos.x, GetTabSize(), pli->nFirstChar );
  }
  // ���ʹ��OLE �ϷŲ��������������϶��ı��������ڣ���ʱ�򱾴���Ӧ��
  // ���ǻ���ڣ����Բ�Ӧ����ʾ�����
  if ( IsFocused() ) {
    CaretSetPos( m_ptTextPos );
  }
}
void CPfxEditView::ScrollBarUpdate( LONG nBar ) {
  ASSERT( ( nBar == SB_HORZ ) || ( nBar == SB_VERT ) );
  // note:
  // nPage must from 0 to nMax - nMin + 1
  // nPos must between min and nMax - max(nPage �C 1, 0)
  SCROLLINFO si;
  si.cbSize = sizeof( si );
  si.fMask = SIF_PAGE | SIF_RANGE;
  si.nMin = 0;
  if ( nBar == SB_VERT ) {
    si.nMax = m_pdb->TextGetLineCount();
    si.nPage = m_sizeVisible.cy - 1;
  } else {
    si.nMax = PFX_EC_MAX_LINE_LENGTH;
    si.nPage = m_rectText.Width() / PFX_EC_DATA.m_nCharWidthAve;
#ifdef _SYNC_SCROLLING
    EnableScrollBar( SB_HORZ, ( !IsWrapAtWindowEdge() ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH ) );
#else
    ShowScrollBar( SB_HORZ, !IsWrapAtWindowEdge() );
#endif
  }
  if ( si.nPage < 0 ) {
    si.nPage = 0;
  } else if ( ( LONG ) si.nPage > ( si.nMax - si.nMin + 1 ) ) {
    si.nPage = ( UINT ) si.nMax - si.nMin + 1;
  }
  SetScrollInfo( nBar, &si, true );
}
void CPfxEditView::ScrollBarSetPos( LONG nBar, LONG nPos ) {
  SCROLLINFO si;
  si.cbSize = sizeof( si );
  si.fMask = SIF_POS;
  si.nPos = nPos;
  SetScrollInfo( nBar, &si, true );
}
void CPfxEditView::UpdateAllViews( DWORD dwFlags, LONG nFirstUpdateLine, LONG nLastUpdateLine ) {
  CPfxEditView * pSiblingView = GetSiblingView();
  switch ( dwFlags ) {
  case PFX_EC_UT_HORZ:
  case PFX_EC_UT_VERT:
    // �����Ѿ����������еķ�Χ
    m_pdb->m_nLineLastParsedButCanceled = m_pdb->m_nLineLastParsed;
    if ( m_pdb->m_nLineLastParsed >= nFirstUpdateLine ) {
      m_pdb->m_nLineLastParsed = nFirstUpdateLine - 1;
    }
    // ������ͼ����
    UpdateThisView( this, dwFlags, nFirstUpdateLine, nLastUpdateLine );
    // �����ֵ���ͼ
    if ( pSiblingView != NULL ) {
      UpdateThisView( pSiblingView, dwFlags, nFirstUpdateLine, nLastUpdateLine );
    }
    break;
  case PFX_EC_UT_LINES:           //      û�иı����ݣ���������Ҫˢ��ָ������
    if ( nLastUpdateLine == -1 ) {
      nLastUpdateLine = m_pdb->TextGetLineUpperBound();
    }
    nFirstUpdateLine = min( nFirstUpdateLine, nLastUpdateLine );
    nLastUpdateLine = max( nFirstUpdateLine, nLastUpdateLine );
    // ������ͼ����
    UpdateLines( nFirstUpdateLine, nLastUpdateLine );
    // �����ֵ���ͼ
    if ( pSiblingView != NULL ) {
      pSiblingView->UpdateLines( nFirstUpdateLine, nLastUpdateLine );
    }
    break;
  case PFX_EC_UT_LAYOUT:           //      û�иı��ı����ݣ�����Ҫ���·���COOKIE
    // ������ͼ����
    {
      UpdateLayout();
      UpdateCurrentPosition( m_ptTextPos.x, false, false );
      ScrollBarUpdate( SB_VERT );
      ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
      ScrollBarUpdate( SB_HORZ );
      ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
      Invalidate( false );
    }
    // �����ֵ���ͼ
    if ( pSiblingView != NULL ) {
      pSiblingView->VerifyCurrentPosition();
      pSiblingView->UpdateLayout();
      pSiblingView->UpdateCurrentPosition( pSiblingView->m_ptTextPos.x, false, false );
      pSiblingView->ScrollBarUpdate( SB_VERT );
      pSiblingView->ScrollBarSetPos( SB_VERT, pSiblingView->m_ptFirstVisible.y );
      pSiblingView->ScrollBarUpdate( SB_HORZ );
      pSiblingView->ScrollBarSetPos( SB_HORZ, pSiblingView->m_ptFirstVisible.x );
      pSiblingView->Invalidate( false );
    }
    break;
  case PFX_EC_UT_ALL:           //      �������ı����ݣ���Ҫȫ��ˢ�²����·���COOKIE
  default:
    m_pdb->m_nLineLastParsed = -1;
    m_pdb->m_nLineLastParsedButCanceled = -1;
    // �����Լ�
    {
      UpdateLayout();
      UpdateCurrentPosition( m_ptTextPos.x, false, false );
      ScrollBarUpdate( SB_VERT );
      ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
      ScrollBarUpdate( SB_HORZ );
      ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
      Invalidate( false );
    }
    if ( pSiblingView != NULL ) {
      pSiblingView->UpdateLayout();
      pSiblingView->VerifyCurrentPosition();
      pSiblingView->UpdateCurrentPosition( pSiblingView->m_ptTextPos.x, false, false );
      pSiblingView->ScrollBarUpdate( SB_VERT );
      pSiblingView->ScrollBarSetPos( SB_VERT, pSiblingView->m_ptFirstVisible.y );
      pSiblingView->ScrollBarUpdate( SB_HORZ );
      pSiblingView->ScrollBarSetPos( SB_HORZ, pSiblingView->m_ptFirstVisible.x );
      pSiblingView->Invalidate( false );
    }
    break;
  }
}
void CPfxEditView::UpdateThisView( CPfxEditView* pView, DWORD dwFlags, LONG nFirstUpdateLine, LONG nLastUpdateLine ) {
  LONG nLineNumberWidth = pView->m_rectLineNumber.right;
  if ( this == pView ) {
    m_ptFirstVisible.y = min( m_ptFirstVisible.y, m_pdb->TextGetLineUpperBound() );
  } else {
    pView->VerifyCurrentPosition();
  }
  if ( dwFlags == PFX_EC_UT_VERT ) {
    pView->UpdateLayout();
    pView->ScrollBarUpdate( SB_VERT );
    pView->ScrollBarSetPos( SB_VERT, pView->m_ptFirstVisible.y );
  } else {
    // Ŀǰ��֧�ָ���ˮƽ������
  }
  // ���nLastUpdateLine == PFX_EC_INVALID_LINE�ͱ�ʾ��nFirstUpdateLine��
  // �µ��ĵ������һ��
  if ( nLastUpdateLine == -1 ) {
    // �к����Ŀ�ȸı���һ��Ҫȫ��ˢ�£���Ϊ�����һ��Ҫˢ��
    if ( nLineNumberWidth != pView->m_rectLineNumber.right ) {
      pView->Invalidate( false );
    } else {
      CRect rectUpdate( &pView->m_rectClient );
      rectUpdate.top = pView->m_rectText.top + ( ( nFirstUpdateLine - pView->m_ptFirstVisible.y ) * PFX_EC_DATA.m_nLineHeight );
      pView->InvalidateRect( &rectUpdate, false );
    }
  } else {
    // �к����Ŀ�ȸı���һ��Ҫȫ��ˢ�£���Ϊ�����һ��Ҫˢ��
    if ( nLineNumberWidth != pView->m_rectLineNumber.right ) {
      pView->Invalidate( false );
    } else {
      pView->UpdateLines( nFirstUpdateLine, nLastUpdateLine );
    }
  }
}
void CPfxEditView::CaretRebuild() {
  CreateSolidCaret( PFX_EC_DATA.m_sizeCaret.cx, PFX_EC_DATA.m_sizeCaret.cy );
  ShowCaret();
}
void CPfxEditView::CaretDestroy() {
  HideCaret();
  DestroyCaret();
}
// CPoint pt - ���ı�����Ϊ���յĹ��λ��
void CPfxEditView::CaretSetPos( CPoint pt ) {
  pt.x = TransTextPosToViewPosX( pt.x );
  pt.y = TransTextPosToViewPosY( pt.y ) + GetLineTextVertOffset();
  if ( !m_rectText.PtInRect( pt ) ) {
    pt.x = -PFX_EC_DATA.m_sizeCaret.cx;
    pt.y = -PFX_EC_DATA.m_sizeCaret.cy;
  }
  SetCaretPos( pt );
}
LONG CPfxEditView::GetHitTestCode( CPoint pt ) const {
  if ( ::PtInRect( &m_rectText, pt ) ) {
    return PFX_EC_HT_TEXT;
  } else if ( ::PtInRect( &m_rectLineNumber, pt ) ) {
    return PFX_EC_HT_LINE_NUMBER;
  } else if ( ::PtInRect( &m_rectRuler, pt ) ) {
    return PFX_EC_HT_RULER;
  } else {
    return PFX_EC_HT_ERROR;
  }
}
CPfxEditView* CPfxEditView::GetSiblingView() const {
  CPfxEditView * pView = m_pdb->m_arrViews[ 0 ];
  if ( this != pView ) {
    return pView;
  } else {
    return m_pdb->m_arrViews[ 1 ];
  }
}
LONG CPfxEditView::ScrollVert( UINT nSBCode ) {
#if 0
  LONG nFirstVisibleLine;
  SCROLLINFO si;
  switch ( nSBCode ) {
  case SB_TOP:
    nFirstVisibleLine = 0;
    break;
  case SB_BOTTOM:
    nFirstVisibleLine = m_pdb->TextGetLineUpperBound();
    break;
  case SB_LINEUP:
    nFirstVisibleLine = m_ptFirstVisible.y - 1;
    break;
  case SB_LINEDOWN:
    nFirstVisibleLine = m_ptFirstVisible.y + 1;
    break;
  case SB_PAGEUP:
    GetScrollInfo( SB_VERT, &si, SIF_PAGE );
    nFirstVisibleLine = m_ptFirstVisible.y - si.nPage;
    break;
  case SB_PAGEDOWN:
    GetScrollInfo( SB_VERT, &si, SIF_PAGE );
    nFirstVisibleLine = m_ptFirstVisible.y + si.nPage;
    break;
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS );
    nFirstVisibleLine = si.nTrackPos;
    break;
  default:
    nFirstVisibleLine = m_ptFirstVisible.y;
    break;
  }
  nFirstVisibleLine = m_pdb->TextVerifyLine( nFirstVisibleLine );
  if ( m_ptFirstVisible.y != nFirstVisibleLine ) {
    CRect rectUpdate;
    ::SetRect( &rectUpdate, m_rectClient.left, m_rectRuler.bottom, m_rectClient.right, m_rectText.bottom );
    ::ScrollWindowEx( m_hWnd, 0, ( ( m_ptFirstVisible.y - nFirstVisibleLine ) * PFX_EC_DATA.m_nLineHeight ), &rectUpdate, &rectUpdate, NULL, NULL, SW_INVALIDATE );
    ScrollBarSetPos( SB_VERT, nFirstVisibleLine );
  }
  return nFirstVisibleLine;
#endif
  return 0;
}
LONG CPfxEditView::ScrollHorz( UINT nSBCode ) {
#if 0
  LONG nFirstVisibleHCol;
  SCROLLINFO si;
  switch ( nSBCode ) {
  case SB_LEFT:
    nFirstVisibleHCol = 0;
    break;
  case SB_LINELEFT:
    nFirstVisibleHCol = m_ptFirstVisible.x - 1;
    break;
  case SB_LINERIGHT:
    nFirstVisibleHCol = m_ptFirstVisible.x + 1;
    break;
  case SB_PAGELEFT:
    GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
    nFirstVisibleHCol = m_ptFirstVisible.x - si.nPage;
    break;
  case SB_PAGERIGHT:
    GetScrollInfo( SB_HORZ, &si, SIF_PAGE );
    nFirstVisibleHCol = m_ptFirstVisible.x + si.nPage;
    break;
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
    nFirstVisibleHCol = si.nTrackPos;
    break;
  case SB_RIGHT:
  default:
    nFirstVisibleHCol = m_ptFirstVisible.x;
    break;
  }
  nFirstVisibleHCol = max( 0, nFirstVisibleHCol );
  if ( m_ptFirstVisible.x != nFirstVisibleHCol ) {
    CRect rectUpdate;
    ::SetRect( &rectUpdate, m_rectText.left, m_rectRuler.top, m_rectText.right, m_rectText.bottom );
    ::ScrollWindowEx( m_hWnd, ( ( m_ptFirstVisible.x - nFirstVisibleHCol ) * PFX_EC_DATA.m_nCharWidthAve ), 0, &rectUpdate, &rectUpdate, NULL, NULL, SW_INVALIDATE );
    ScrollBarSetPos( SB_HORZ, nFirstVisibleHCol );
  }
  return nFirstVisibleHCol;
#endif
  return 0;
}
// ����ճ��
// CPoint pt - Ҫճ����λ�ã����λ�ã�
void CPfxEditView::PasteNormal( CPoint pt, LPCTSTR pData, LONG cch ) {
  ASSERT( m_pdb->TextIsValidLine( pt.y ) );
  ASSERT( m_pdb->TextIsValidChar( pt.y, pt.x ) );
  pt.y = m_pdb->TextVerifyLine( pt.y );
  pt.x = m_pdb->TextVerifyChar( pt.y, pt.x );
  CharsInsert( pt, pData, cch, true );
}
// ����ʽ�п�ճ��
// CPoint pt - Ҫճ����λ�ã����λ�ã�
void CPfxEditView::PasteColumnAlign( CPoint pt, LPCTSTR pData, LONG cch ) {
  ASSERT( !IsWrapping() );
  ASSERT( m_pdb->TextIsValidLine( pt.y ) );
  ASSERT( m_pdb->TextIsValidChar( pt.y, pt.x ) );
  if ( ( pData == NULL ) || ( cch <= 0 ) ) {
    return ;
  }
  pt.y = m_pdb->TextVerifyLine( pt.y );
  pt.x = m_pdb->TextVerifyChar( pt.y, pt.x );
  LPCTSTR pCur = pData;
  LPCTSTR pEnd = pData + cch;
  LPCTSTR pLineBreakData = m_pdb->GetLineBreakDataFromType( PFX_EC_LINE_BREAK_TYPE_DOS );
  // ����ģʽ��ճ����ճ��ʱ��ÿ�еĲ�������ı����������������������
  // ������������������һЩ�ǵȾ�������Ҳ���������������С�������Ҫ��
  // ���ʼ������ˮƽ�ı����꣬���Դ�����������������еĲ����
  const LONG nAlignPos = TransCharPosToTextPos( pt );
  CPfxEditHeap Heap;
  for ( LONG nCurLine = pt.y; pCur < pEnd; nCurLine++ ) {
    LPCTSTR pLineData = pCur;
    LPCTSTR pLineBreak = pEnd;
    while ( pCur < pEnd ) {
      if ( ( PFX_EC_CHR_CR == *pCur++ )
           && ( pCur < pEnd )
           && ( PFX_EC_CHR_LF == *pCur ) ) {
        pLineBreak = pCur - 1;
        //  ʹָ��ָ����һ�е��ַ�
        pCur++;
        break;
      }
    }
    // Ҫ�������ݵĳ���
    LONG cchLine = pLineBreak - pLineData;
    // �����ǰҪ�������е���ĩ�ı�����С��Ҫ���Ե��ı��������
    // ������ԭ���ĵ������һ�У���Ҫ��Ϊ�ز���һ�������Ŀհ���
    // ���Դﵽ�ж����Ч��
    LONG xTextPos;
    BOOL bAppendLine;
    if ( nCurLine >= m_pdb->TextGetLineCount() ) {
      bAppendLine = true;
      xTextPos = 0;
      pt.y = m_pdb->TextGetLineUpperBound();
      pt.x = m_pdb->TextGetLineLength( pt.y );
    } else {
      bAppendLine = false;
      xTextPos = nAlignPos;
      pt.y = nCurLine;
      pt.x = TransTextPosToCharPos( pt.y, xTextPos );
    }
    // �����Ҫ�;����ܵز���һЩ�հ��ַ�
    LONG nSpacesToFill = ( max( 0, ( nAlignPos - xTextPos ) ) / PFX_EC_DATA.m_nCharWidthSpace );
    LONG nBufSizeExtra = nSpacesToFill;
    LONG nStartLogChar = 0;
    if ( !IsExpandTabToSpaceChars() ) {
      if ( !bAppendLine ) {
        const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( pt.y );
        nStartLogChar = CalcLogicalChar( pli->pData, ( pli->pData + pt.x ), GetTabSize(), pli->nFirstChar );
      }
      nBufSizeExtra = FillBufferWithWhitespacesCalcBufferSize( nStartLogChar, nSpacesToFill );
    }
    // �����Ҫ�������л��Ͽհ��ַ��͹����µĻ�����������Ҫ��
    // ����ַ����ú�Ȼ��һ�ι����룬���������UNDO����Ч�ͻ�
    // ������ʹ��Ч�ʸ��ã���Ϊ����������������ҪƵ������������
    if ( ( nBufSizeExtra > 0 ) || bAppendLine ) {
      LONG nBufLen;
      LPTSTR pBuffer;
      LPTSTR pInsert;
      // ���㻺�����ĳ���
      if ( !bAppendLine ) {
        nBufLen = ( cchLine + nBufSizeExtra );
        pBuffer = static_cast<LPTSTR>( Heap.ReAlloc( nBufLen * sizeof( TCHAR ) ) );
        pInsert = pBuffer;
      } else {
        nBufLen = ( cchLine + nBufSizeExtra + PFX_EC_LINE_BREAK_SIZE_DOS );
        pBuffer = static_cast<LPTSTR>( Heap.ReAlloc( nBufLen * sizeof( TCHAR ) ) );
        pInsert = pBuffer + PFX_EC_LINE_BREAK_SIZE_DOS;
        memcpy( pBuffer, pLineBreakData, PFX_EC_LINE_BREAK_SIZE_DOS * sizeof( TCHAR ) );
      }
      if ( nBufSizeExtra > 0 )           //      ������ڶ���Ŀհ��ַ�
      {
        FillBufferWithWhitespacesDoFill( nStartLogChar, nSpacesToFill, pInsert, nBufSizeExtra );
        pInsert += nBufSizeExtra;
      }
      // ����������������
      memcpy( pInsert, pLineData, ( cchLine * sizeof( TCHAR ) ) );
      cchLine = nBufLen;
      pLineData = pBuffer;
    }
    CharsInsert( pt, pLineData, cchLine, false );
    nCurLine = pt.y;
    // �����OLE�ϷŲ�������ô���š�����ȻҪ��ͼ�ָ�֮ǰ��ѡ��
    // ������п�����TAB�ַ�����ܻ�ʹ�п�Ŀ�ȷ����仯������
    // ����Ҫ���š���һ�б����¼�����벿�ֵ��ұߵ����꣬�����
    // �������ԭ�����п�����ұߵ���ֵ�󣬾�ȡ�ϴ����ֵ
    if ( IsDragging() ) {
      LONG nColSelWidth = TransCharPosToTextPos( pt ) - nAlignPos;
      if ( nColSelWidth > m_rectColSelect.Width() ) {
        m_rectColSelect.right = m_rectColSelect.left + nColSelWidth;
      }
    }
  }
  if ( pt != m_ptEditPos ) {
    m_ptEditPos = pt;
    UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
}
// void PasteColumnAppend(CPoint pt, LPCTSTR pData, LONG cch)
// ����ĩ����ʽ�п�ճ��
void CPfxEditView::PasteColumnAppend( CPoint pt, LPCTSTR pData, LONG cch ) {
  ASSERT( IsCanColumnPaste() );
  ASSERT( m_pdb->TextIsValidLine( pt.y ) );
  ASSERT( m_pdb->TextIsValidChar( pt.y, pt.x ) );
  // ���п�ģʽ��ճ�����ݣ���ÿ�е���󸽼�����
  if ( ( pData == NULL ) || ( cch <= 0 ) ) {
    return ;
  }
  pt.y = m_pdb->TextVerifyLine( pt.y );
  pt.x = m_pdb->TextVerifyChar( pt.y, pt.x );
  LPCTSTR pCur = pData;
  LPCTSTR pEnd = pData + cch;
  LPCTSTR pLineBreakData = m_pdb->GetLineBreakDataFromType( PFX_EC_LINE_BREAK_TYPE_DOS );
  CPfxEditHeap Heap;
  for ( LONG nCurLine = pt.y; pCur < pEnd; nCurLine++ ) {
    LPCTSTR pLineData = pCur;
    LPCTSTR pLineBreak = pEnd;
    while ( pCur < pEnd ) {
      if ( ( PFX_EC_CHR_CR == *pCur++ )
           && ( pCur < pEnd )
           && ( PFX_EC_CHR_LF == *pCur ) ) {
        pLineBreak = pCur - 1;
        //  ʹָ��ָ����һ�е��ַ�
        pCur++;
        break;
      }
    }
    // Ҫ�������ݵĳ���
    LONG cchLine = pLineBreak - pLineData;
    LONG nBufLen;
    LPTSTR pBuffer;
    LPTSTR pInsert;
    // ��Ҫ��������
    if ( nCurLine >= m_pdb->TextGetLineCount() ) {
      nBufLen = cchLine + PFX_EC_LINE_BREAK_SIZE_DOS;
      pBuffer = static_cast<LPTSTR>( Heap.Alloc( nBufLen * sizeof( TCHAR ) ) );
      pInsert = pBuffer + PFX_EC_LINE_BREAK_SIZE_DOS;
      memcpy( pBuffer, pLineBreakData, PFX_EC_LINE_BREAK_SIZE_DOS * sizeof( TCHAR ) );
      pt.y = m_pdb->TextGetLineUpperBound();
      pt.x = 0;
    } else {
      // ���Ҫ���뱾�е����ݳ��Ȳ�Ϊ�����һ�ո�ԭ����
      // �ݺ��¸������ݸ����������������ո�
      if ( cchLine > 0 ) {
        nBufLen = cchLine + 1;
        pBuffer = static_cast<LPTSTR>( Heap.ReAlloc( nBufLen * sizeof( TCHAR ) ) );
        pInsert = pBuffer;
        *pInsert++ = PFX_EC_CHR_SPACE;
      } else {
        nBufLen = cchLine;
        pBuffer = static_cast<LPTSTR>( Heap.ReAlloc( nBufLen * sizeof( TCHAR ) ) );
        pInsert = pBuffer;
      }
      pt.y = nCurLine;
      pt.x = m_pdb->TextGetLineLength( pt.y );
    }
    memcpy( pInsert, pLineData, cchLine * sizeof( TCHAR ) );
    CharsInsert( pt, pBuffer, nBufLen, false );
    nCurLine = pt.y;
  }
  if ( pt != m_ptEditPos ) {
    m_ptEditPos = pt;
    UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
}
HGLOBAL CPfxEditView::DragDropRender( BOOL bDragDrop, BOOL bAppend ) {
  if ( !IsTextSelected() ) {
    return NULL;
  }
  // + 1 = ��β��NULL�ַ�
  LONG nSelSize = GetSelSize( PFX_EC_LINE_BREAK_TYPE_DOS, true ) + 1;
  if ( nSelSize <= 1 ) {
    return NULL;
  }
  CPfxEditViewClipboard clipboard( this, CF_TEXT, false );
  LPTSTR pBuffer = clipboard.GetBuffer( nSelSize, bAppend );
  if ( pBuffer == NULL ) {
    return NULL;
  }
  GetSelData( PFX_EC_LINE_BREAK_TYPE_DOS, pBuffer, nSelSize );
  HGLOBAL hGlobal = clipboard.Detach();
  // �����θ��������п�ѡ��ģʽ�½��еľͱ�ʶ���β���������
  // ������ճ����ʱ��ʵ���п鹦��
  if ( bDragDrop ) {
    if ( IsColumnSelect() ) {
      PFX_EC_DATA.m_hDataDragDropCopied = hGlobal;
    } else {
      PFX_EC_DATA.m_hDataDragDropCopied = NULL;
    }
  } else {
    if ( IsColumnSelect() ) {
      ::GlobalFlags( hGlobal );
      PFX_EC_DATA.m_nDataClipboardCopied = ::GlobalSize( hGlobal );
      PFX_EC_DATA.m_hDataClipboardCopied = hGlobal;
    } else {
      PFX_EC_DATA.m_nDataClipboardCopied = 0;
      PFX_EC_DATA.m_hDataClipboardCopied = NULL;
    }
  }
  return hGlobal;
}
// BOOL DragDropPaste(HGLOBAL hGlobal, CPoint pt)
// CPoint pt - �������������λ��
BOOL CPfxEditView::DragDropPaste( HGLOBAL hGlobal, CPoint pt ) {
  ASSERT( !IsReadOnly() ); // Ӧ���������
  if ( hGlobal == NULL ) {
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  LPCTSTR pData = static_cast<LPCTSTR>( ::GlobalLock( hGlobal ) );
  if ( pData == NULL ) {
    return false;
  }
  CPoint ptOld;
  ptOld.y = pt.y = m_pdb->TextVerifyLine( pt.y );
  ptOld.x = pt.x = m_pdb->TextVerifyChar( pt.y, pt.x );
  if ( IsWrapping() || !IsDataColumnModeRendered( true, hGlobal ) ) {
    // �������Դ�ṩ�����Լ�����DROP��Ҫ���²������
    // ��ѡ�����
    if ( IsDragging() ) {
      m_pdb->TextSubLineToAbsLine( ptOld );
    }
    PasteNormal( pt, pData, _tcslen( pData ) );
    if ( IsDragging() ) {
      m_pdb->TextAbsLineToSubLine( ptOld );
      SelRange( ptOld );
    }
  } else {
    // �������Դ�ṩ�����Լ�����DROP��Ҫ���²������
    // ��ѡ�����
    if ( IsDragging() ) {
      m_pdb->TextSubLineToAbsLine( ptOld );
    }
    PasteColumnAlign( pt, pData, _tcslen( pData ) );
    if ( IsDragging() ) {
      m_pdb->TextAbsLineToSubLine( ptOld );
      CRect rect;
      rect.left = TransCharPosToTextPos( ptOld );
      rect.right = rect.left + m_rectColSelect.Width();
      rect.top = TransLinePosToTextPos( ptOld.y );
      rect.bottom = TransLinePosToTextPos( m_ptEditPos.y );
      SelRange( rect );
    }
  }
  ::GlobalUnlock( hGlobal );
  return true;
}
// void DragDropRemove(CPoint& rpt)
// ��OLE�ϷŲ������û���ĳ���ط�ѡ����һ�����ı������Ϸţ�����ѡ�� ����
// �ű༭���ڵ�ĳ��λ�ã�����Ҫ��ɾ���Ϸ�����ѡ�����ݣ���������ȷ�Ĳ�
// ���
// CPoint& rpt - DROP������Ҫ����㣨�������λ�ã���������
// ����ɾ��ѡ�񲿷ֺ��DROP��λ��
void CPfxEditView::DragDropRemove( CPoint& rpt ) {
  ASSERT( IsTextSelected() );
  ASSERT( m_pdb->TextIsValidChar( rpt.y, rpt.x ) );
  // ���ݡ�drop�����˱�Ĵ���
  if ( !IsDragging() ) {
    SelNoneAndRemoveSelected();
    rpt = m_ptEditPos;
    return ;
  }
  // �ϷŲ������Լ���Χ�ڽ���
  // ����������ѡ��Χ֮ǰ��ֻҪ��ס����λ�þ�����
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( rpt.y );
  if ( ( ( rpt.y == m_nSelLineFrom )
         && ( rpt.x <= pli->nSelFrom ) )
       || ( rpt.y < m_nSelLineFrom ) ) {
    m_pdb->TextSubLineToAbsLine( rpt );
    SelNoneAndRemoveSelected();
    m_pdb->TextAbsLineToSubLine( rpt );
    return ;
  }
  // �п�ɾ����������ɾ����ֻ��ɾ���У�����Ҳ�������Զ�����ģʽ�½��У�
  // �����������ǲ���ı��
  if ( IsColumnSelect() ) {
    ASSERT( !IsWrapping() );
    if ( ( rpt.y >= m_nSelLineFrom )
         && ( rpt.y <= m_nSelLineTo )
         && ( rpt.x > pli->nSelFrom ) ) {
      rpt.x = max( rpt.x, pli->nSelTo );
      rpt.x -= pli->nSelTo;
      rpt.x += pli->nSelFrom;
    }
    SelNoneAndRemoveSelected();
  }
  // ���п�ѡ�񲻿�����ѡ��Χ���룬���Բ����ض���ѡ��Χ֮��
  else {
    ASSERT( ( rpt.y > m_nSelLineTo )
            || ( ( rpt.y == m_nSelLineTo )
                 && ( rpt.x >= pli->nSelTo ) ) );
    CPoint ptSelFrom;
    ptSelFrom.y = m_nSelLineFrom;
    ptSelFrom.x = m_pdb->TextGetLineAt( ptSelFrom.y ) ->nSelFrom;
    CPoint ptSelTo;
    ptSelTo.y = m_nSelLineTo;
    ptSelTo.x = m_pdb->TextGetLineAt( ptSelTo.y ) ->nSelTo;
    // ����Ŀ�������ѡ����������֮���ƫ�������µĲ�����
    // ��ԭѡ��Χ�����������ƫ����
    LONG nOffset = ( m_pdb->TextSubLineToOffset( rpt )
                     - m_pdb->TextSubLineToOffset( ptSelTo )
                     + m_pdb->TextSubLineToOffset( ptSelFrom ) );
    SelNoneAndRemoveSelected();
    m_pdb->TextOffsetToSubLine( nOffset, rpt );
  }
}
// void WrapAtUnits(LONG nWidth)
// ��ָ���߼���Ԫ��ʵʩ����
// LONG nWidth - ���߼���Ԫ��ָ�����п�ȣ��������Ե��Դ����й��ܣ���
//            ���ڴ��ڱ߻��л򰴴�ӡҳ���е�
void CPfxEditView::WrapAtUnits( LONG nWidth ) {
  ASSERT( nWidth > 0 );
  CClientDC1 dc( this );
  HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
  CPfxEditHeap Heap;
  Heap.Alloc( Heap.GetBufferConstSize() );
  CPfxEditViewWrapLineHint wl;
  wl.m_hDC = dc;
  wl.m_nWidth = nWidth;
  wl.m_nliCur = 0;
  wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
  wl.m_nGrowBy = 10000;
  wl.m_nTabSize = GetTabSize();
  wl.m_nCharWidthMax = PFX_EC_DATA.m_nCharWidthMax;
  wl.m_nCharWidthSpace = PFX_EC_DATA.m_nCharWidthSpace;
  wl.m_nCharWidthTab = PFX_EC_DATA.m_nCharWidthTab;
  LONG nMainLines = 0;
  LONG nLineCount = m_pdb->TextGetLineCount();
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( 0 );
  CPfxEditLineInfo* pliEnd = m_pdb->TextGetLineAt( ( nLineCount - 1 ) );
  // ���㻻�н������ݣ���Щ���ݽ���ת������ɰٷֱ�֪ͨ�͸�m_pdb->EventSendMsg()��
  // һ��������ʾ��ǰ�Ľ��ȸ��û���
  LONG nPercentCompleted = 0;
  LONG nLinesOfOnePercent = max( 1, ( ( GetLineCount() + 99 ) / 100 ) );
  while ( pli <= pliEnd ) {
    if ( ( nMainLines % nLinesOfOnePercent ) == 0 ) {
      nPercentCompleted++;
      m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
    }
    // ������ʵ���к�
    pli->nIndex = nMainLines++;
    WrapLineAtUnits( Heap, pli, wl );
    // ��������
    while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
      // do nothing
    }
  }
  dc.SelectObject( hFont );
  if ( ( wl.m_nliCur > 0 ) && ( Heap.GetBuffer() != NULL ) ) {
    CPoint ptFake( 0, 0 );
    m_pdb->TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), 0, nLineCount, wl.m_nliCur, ptFake );
    // һ��Ҫ��������¸ñ�ʶ������������ܳ���
    m_pdb->m_nWrapWidth = nWidth;
    m_pdb->m_nWrapStyle = PFX_EC_WRAP_WINDOWEDGE;
    ASSERT( !PFX_EC_IS_DRAW_HARDRETURN( m_pdb->TextGetLineAt( m_pdb->TextGetLineUpperBound() ) ) );
  }
  // ��ʽ���������%100
  if ( nPercentCompleted < 100 ) {
    m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, 100 );
  }
}
// ��ָ���ַ������㻻�У���ʵʩ���в���
// LONG nWidth - ���߼��ַ���ָ�����п�ȣ��߼��ַ�����ָ���Ʊ����չ����ַ���
void CPfxEditView::WrapColumns( LONG nWidth ) {
  ASSERT( nWidth > 0 );
  CPfxEditHeap Heap;
  Heap.Alloc( Heap.GetBufferConstSize() );
  CPfxEditViewWrapLineHint wl;
  wl.m_nWidth = nWidth;
  wl.m_nliCur = 0;
  wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
  wl.m_nGrowBy = 10000;
  wl.m_nTabSize = GetTabSize();
  LONG nMainLines = 0;
  LONG nLineCount = m_pdb->TextGetLineCount();
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( 0 );
  CPfxEditLineInfo* pliEnd = m_pdb->TextGetLineAt( ( nLineCount - 1 ) );
  // ���㻻�н������ݣ���Щ���ݽ���ת������ɰٷֱ�֪ͨ�͸�m_pdb->EventSendMsg()��
  // һ��������ʾ��ǰ�Ľ��ȸ��û���
  LONG nPercentCompleted = 0;
  LONG nLinesOfOnePercent = max( 1, ( ( GetLineCount() + 99 ) / 100 ) );
  while ( pli <= pliEnd ) {
    if ( ( nMainLines % nLinesOfOnePercent ) == 0 ) {
      nPercentCompleted++;
      m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
    }
    // ������ʵ���к�
    pli->nIndex = nMainLines++;
    WrapLineColumns( Heap, pli, wl );
    // ��������
    while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
      // do nothing
    }
  }
  if ( ( wl.m_nliCur > 0 ) && ( Heap.GetBuffer() != NULL ) ) {
    CPoint ptFake( 0, 0 );
    m_pdb->TextUpdateLineInfo( reinterpret_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ), 0, nLineCount, wl.m_nliCur, ptFake );
    // һ��Ҫ��������¸ñ�ʶ������������ܳ���
    m_pdb->m_nWrapWidth = nWidth;
    m_pdb->m_nWrapStyle = PFX_EC_WRAP_COLUMNS;
    ASSERT( !PFX_EC_IS_DRAW_HARDRETURN( m_pdb->TextGetLineAt( m_pdb->TextGetLineUpperBound() ) ) );
  }
  // ��ʽ���������%100
  if ( nPercentCompleted < 100 ) {
    m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, 100 );
  }
}
// void WrapRestore()
// �ָ��л������ڵ�����ϢΪû�л���ʱ��״̬
// �����ǰ����һ�����У���ԭ��ǰ��Ҫ�����������ϣ����Ҳ����Ҫ����
// �����������ǰ�в������У���ԭ��Ҫȷ����ǰ�в��䣨���ݲ�������кţ�
void CPfxEditView::WrapRestore() {
  LONG nMainLines = 0;
  LONG nLineCount = m_pdb->TextGetLineCount();
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( 0 );
  CPfxEditLineInfo* pliEnd = m_pdb->TextGetLineAt( ( nLineCount - 1 ) );
  // ���㻻�н������ݣ���Щ���ݽ���ת������ɰٷֱ�֪ͨ�͸�m_pdb->EventSendMsg()��
  // һ��������ʾ��ǰ�Ľ��ȸ��û���
  LONG nPercentCompleted = 0;
  LONG nLinesOfOnePercent = max( 1, ( ( GetLineCount() + 99 ) / 100 ) );
  for ( CPfxEditLineInfo * pliMain = pli; pli <= pliEnd; pliMain++, nMainLines++ ) {
    ASSERT( !PFX_EC_IS_SUBLINE( pli ) );
    // ֻ�е��ĵ��������64KTCHARʱ�ŷ��ͽ�����Ϣ
    if ( ( nMainLines % nLinesOfOnePercent ) == 0 ) {
      nPercentCompleted++;
      m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
    }
    // ��������в��������о�Ҫɾ�����ĸ�������
    pliMain->dwFlags = ( pli->dwFlags | PFX_EC_LF_HARDRETURN );
    pliMain->nActLen = pliMain->nLen = pli->nActLen;
    pliMain->pData = pli->pData;
    pliMain->pMemBlock = pli->pMemBlock;
    pliMain->nIndex = PFX_EC_INVALID_LINE;
    pliMain->nFirstChar = 0;
    // ��������
    while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
      // do nothing...
    }
  }
  if ( nMainLines < nLineCount ) {
    m_pdb->TextRemoveLine( nMainLines, ( nLineCount - nMainLines ) );
  }
  // һ��Ҫ��������¸ñ�ʶ���������ܳ���
  m_pdb->m_nWrapWidth = 0;
  m_pdb->m_nWrapStyle = PFX_EC_WRAP_NONE;
  // ���һ�п϶�û�лس����У�����Ҫ��ȥ�ñ�־
  pli = m_pdb->TextGetLineAt( m_pdb->TextGetLineUpperBound() );
  pli->dwFlags &= ~PFX_EC_LF_HARDRETURN;
  // ��ʽ���������%100
  if ( nPercentCompleted < 100 ) {
    m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, 100 );
  }
}
// void WrapLineAtUnits(CPfxEditHeap& Heap,const CPfxEditLineInfo* pli,CPfxEditViewWrapLineHint& wl)
// ��ָ���߼���Ԫ��ȼ���һ�е���Ϣ
// wl.m_nWidth - w.m_hDC���߼���Ԫ��
void CPfxEditView::WrapLineAtUnits( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, CPfxEditViewWrapLineHint& wl ) {
  ASSERT( pli != NULL );
  ASSERT( wl.m_hDC != NULL );
  ASSERT( wl.m_nWidth > 0 );
  ASSERT( wl.m_nGrowBy > 0 );
  if ( wl.m_nliCur >= wl.m_nliMax ) {
    wl.m_nliMax += wl.m_nGrowBy;
    Heap.ReAlloc( wl.m_nliMax * sizeof( CPfxEditLineInfo ) );
  }
  LONG nLogChar;
  CPfxEditLineInfo* pliCached = ( ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + wl.m_nliCur );
  pliCached->dwFlags = ( pli->dwFlags & ( ~( PFX_EC_LF_SUBLINE | PFX_EC_LF_HARDRETURN ) ) );
  pliCached->nActLen = pliCached->nLen = pli->nActLen;
  pliCached->pData = pli->pData;
  pliCached->pMemBlock = pli->pMemBlock;
  pliCached->nIndex = pli->nIndex;
  pliCached->nFirstChar = nLogChar = 0;
  // �ٶ�ÿ���ַ������и�����������ַ��Ŀ�ȣ����ڸ����Ŀ������С��
  // ���ɵ��ַ���Ϊ(width / max_char_width)��������ַ�����Ϊÿ���ƽ�
  // �ĳ��ȣ�ÿ���ƽ�����GetTextExtentPoint32������ö��ַ���ʵ�ʳ��ȣ�
  // ���С�ڸ�����������������պõ��ھ�ֹͣ��������ھͶ����Ѽ���
  // ������ַ��ĳ��ȣ�������ֵ����һ�����������¼���
  LPCTSTR pStart = pli->pData;
  LPCTSTR pEnd = pStart + pli->nActLen;
  // ����nWrapWidth��ȥ1��Ϊ���ұ���һ���Ŀ�϶
  for ( LONG nSubLine = 1, nWrapWidth = ( wl.m_nWidth - 1 ); pStart < pEnd; nSubLine++ ) {
    LPCTSTR pCur = pStart;
    LPCTSTR pTmp = pStart;
    LONG nTestWidth = 0;
    LONG nTestChars = ( nWrapWidth / wl.m_nCharWidthMax );
    for ( ;; )           //      ��һ���Ʋ�
    {
      CSize size;
      LONG nLogCharSaved = nLogChar;
      LONG nTestWidthSaved = nTestWidth;
      nTestChars = max( 1, nTestChars );
      for ( LONG i = nTestChars; ( i > 0 ) && ( pTmp < pEnd ); i-- ) {
        if ( PFX_EC_IS_LEAD_BYTE( *pTmp ) ) {
          pTmp += PFX_EC_MAX_MB_LEN;
          nLogChar += PFX_EC_MAX_MB_LEN;
          continue;
        }
        if ( *pTmp != PFX_EC_CHR_TAB ) {
          pTmp++;
          nLogChar++;
        } else {
          LONG nTabSize = wl.m_nTabSize - ( nLogChar % wl.m_nTabSize );
          //   ����for��i�Ѿ�����һ
          i -= ( nTabSize - 1 );
          nTestWidth += ( nTabSize * wl.m_nCharWidthSpace - wl.m_nCharWidthTab );
          pTmp++;
          nLogChar += nTabSize;
        }
      }
      // ���������Ʋ���Ѿ�������ĩ
      if ( pTmp >= pEnd ) {
        ::GetTextExtentPoint32( wl.m_hDC, pCur, pEnd - pCur, &size );
        if ( ( nTestWidth + size.cx ) <= nWrapWidth ) {
          goto WrapLineCompleted;
        }
        //  ���һ���ַ����Ѵ��ڸ����Ŀ�Ⱦ�ȡһ����
        //  ��һ���ˣ������Ƶ���������ֵ����һ����
        //  ������ͷ����
        if ( nTestChars == 1 ) {
          if ( pCur > pStart ) {
            nLogChar = nLogCharSaved;
            break;
          }
          goto WrapLineCompleted;
        } else {
          pTmp = pCur;
          nLogChar = nLogCharSaved;
          nTestWidth = nTestWidthSaved;
          nTestChars /= 10;
          continue; // for (;;) // ��һ���Ʋ�
        }
      } else {
        ::GetTextExtentPoint32( wl.m_hDC, pCur, pTmp - pCur, &size );
        nTestWidth += size.cx;
        if ( nTestWidth < nWrapWidth ) {
          pCur = pTmp;
          nTestChars = ( nWrapWidth - nTestWidth ) / wl.m_nCharWidthMax;
          continue; // for (;;) // ��һ���Ʋ�
        }
        if ( nTestWidth == nWrapWidth ) {
          pCur = pTmp;
          break;
        }
        if ( nTestChars <= 1 ) {
          //   ���һ���ַ��Ŀ�ȱ��Ѿ���������
          //   �Ŀ�ȣ�������һ��ֻ��һ���ַ���
          //   ��ôҲֻ����������
          if ( pCur == pStart ) {
            pCur = pTmp;
          }
          //   �����һ�в�ֹһ���ַ��Ͳ�������
          //   ���������
          else {
            nLogChar = nLogCharSaved;
          }
          break;
        }
        //  �Ƶ���������ֵ����һ����������ͷ����
        else {
          pTmp = pCur;
          nLogChar = nLogCharSaved;
          nTestWidth = nTestWidthSaved;
          nTestChars /= 10;
          continue; // for (;;) // ��һ���Ʋ�
        }
      }
    }
    ASSERT( pCur > pStart );
    // ���ڶ�дָ��������λ�ÿ�����һ�����ʵ��м䣬���Ҫ������
    // �ϣ��������һ�����ʵ��м��Ҫ������һ�����ʶ����������
    // �ٶȽ���ǳ�������������ֻʶ��ո���Ʊ�����������ʱ��
    // �����ո���Ʊ������ֻ�ܽ��á����ʡ�ǿ�жϿ��������ԣ���
    // �������ᵼ�¡�����������
    if ( !IsWhitespace( *pCur ) && !PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      for ( pTmp = ( pCur - 1 ); pTmp > pStart; pTmp-- ) {
        //  ���pTmp��ָ����ַ����ǿհ��ַ�����Ҫ��
        //  �����ַ���������һ�еĿ�ͷ���ǿհ��ַ�
        if ( IsWhitespace( *pTmp ) ) {
          pTmp++;
          nLogChar -= ( pCur - pTmp );
          pCur = pTmp;
          break;
        }
      }
    }
    // ���Բ�������ֿյ�����
    ASSERT( pCur < pEnd );
    ASSERT( pCur > pStart );
    pliCached->nLen = pCur - pStart;
    pliCached++;
    wl.m_nliCur++;
    if ( wl.m_nliCur >= wl.m_nliMax ) {
      wl.m_nliMax += wl.m_nGrowBy;
      Heap.ReAlloc( wl.m_nliMax * sizeof( CPfxEditLineInfo ) );
      pliCached = ( ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + wl.m_nliCur );
    }
    pliCached->dwFlags = PFX_EC_LF_SUBLINE;
    pliCached->nLen = pEnd - pCur;
    pliCached->nActLen = nSubLine;
    pliCached->pData = const_cast<LPTSTR>( pCur );
    pliCached->pMemBlock = NULL;
    pliCached->nIndex = PFX_EC_INVALID_LINE;
    pliCached->nFirstChar = nLogChar;
    pStart = pCur;
  }
WrapLineCompleted:
  wl.m_nliCur++;
  pliCached->dwFlags |= PFX_EC_LF_HARDRETURN;
  return ;
}
// void WrapLineColumns(CPfxEditHeap& Heap,const CPfxEditLineInfo* pli,CPfxEditViewWrapLineHint& wl)
// wl.m_nWidth - �����������ַ��������㣬һ�������ַ���������
void CPfxEditView::WrapLineColumns( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, CPfxEditViewWrapLineHint& wl ) {
  ASSERT( pli != NULL );
  ASSERT( wl.m_nWidth > 0 );
  ASSERT( wl.m_nGrowBy > 0 );
  if ( wl.m_nliCur >= wl.m_nliMax ) {
    wl.m_nliMax += wl.m_nGrowBy;
    Heap.ReAlloc( wl.m_nliMax * sizeof( CPfxEditLineInfo ) );
  }
  LONG nLogChar;
  CPfxEditLineInfo* pliCached = ( ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + wl.m_nliCur );
  pliCached->dwFlags = ( pli->dwFlags & ( ~( PFX_EC_LF_SUBLINE | PFX_EC_LF_HARDRETURN ) ) );
  pliCached->nActLen = pliCached->nLen = pli->nActLen;
  pliCached->pData = pli->pData;
  pliCached->pMemBlock = pli->pMemBlock;
  pliCached->nIndex = pli->nIndex;
  pliCached->nFirstChar = nLogChar = 0;
  // �������е��ַ�����TAB��������ﲻ�����е�Ҫ��Ͳ���Ҫ��������
  if ( ( pli->nActLen * wl.m_nTabSize ) > wl.m_nWidth ) {
    LPCTSTR pStart = pli->pData;
    LPCTSTR pCur = pStart;
    LPCTSTR pEnd = pStart + pli->nActLen;
    for ( LONG nSubLine = 1; pStart < pEnd; nSubLine++ ) {
      LONG nTestChars = 0;
      LPCTSTR pTmp = pCur;
      while ( ( pCur < pEnd ) && ( nTestChars < wl.m_nWidth ) ) {
        pTmp = pCur;
        if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
          nTestChars += PFX_EC_MAX_MB_LEN;
          pCur += PFX_EC_MAX_MB_LEN;
          nLogChar += PFX_EC_MAX_MB_LEN;
          continue;
        }
        if ( *pCur != PFX_EC_CHR_TAB ) {
          nTestChars++;
          pCur++;
          nLogChar++;
        } else {
          LONG nTabSize = wl.m_nTabSize
                          - ( nLogChar % wl.m_nTabSize );
          nTestChars += nTabSize;
          pCur++;
          nLogChar += nTabSize;
        }
      }
      // ���г���δ�ﵽ����Ҫ�󣬲���Ҫ����
      if ( pCur >= pEnd ) {
        break;
      }
      ASSERT( pCur > pStart );
      if ( nTestChars > wl.m_nWidth ) {
        pCur = pTmp;
      }
      // ���ڶ�дָ��������λ�ÿ�����һ�����ʵ��м䣬���
      // Ҫ�����жϣ��������һ�����ʵ��м��Ҫ������һ��
      // ���ʶ�����������ٶȽ���ǳ�������������ֻʶ���
      // ����Ʊ�����������ʱ�Ҳ����ո���Ʊ������ֻ��
      // ���á����ʡ�ǿ�жϿ��������ԣ����������ᵼ�¡���
      // ��������
      if ( !IsWhitespace( *pCur ) ) {
        for ( pTmp = ( pCur - 1 ); pTmp > pStart; pTmp-- ) {
          //   ���pTmp��ָ����ַ����ǿհ��ַ���
          //   ��Ҫ������ַ���������һ�еĿ�ͷ
          //   ���ǿհ��ַ�
          if ( IsWhitespace( *pTmp ) ) {
            pTmp++;
            nLogChar -= ( pCur - pTmp );
            pCur = pTmp;
            break;
          }
        }
      }
      // ���Բ�������ֿյ�����
      ASSERT( pCur < pEnd );
      pliCached->nLen = pCur - pStart;
      pliCached++;
      wl.m_nliCur++;
      if ( wl.m_nliCur >= wl.m_nliMax ) {
        wl.m_nliMax += wl.m_nGrowBy;
        Heap.ReAlloc( wl.m_nliMax * sizeof( CPfxEditLineInfo ) );
        pliCached = ( ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + wl.m_nliCur );
      }
      pliCached->dwFlags = PFX_EC_LF_SUBLINE;
      pliCached->nLen = pEnd - pCur;
      pliCached->nActLen = nSubLine;
      pliCached->pData = const_cast<LPTSTR>( pCur );
      pliCached->pMemBlock = NULL;
      pliCached->nIndex = PFX_EC_INVALID_LINE;
      pliCached->nFirstChar = nLogChar;
      pStart = pCur;
    }
  }
  // ���һ���еļ��㣬���һ��Ҫ����Ӳ�س���־����Ϊ����ʶһ�еĽ���
  wl.m_nliCur++;
  pliCached->dwFlags |= PFX_EC_LF_HARDRETURN;
}
// LONG WrapLineSingle(CPfxEditHeap& Heap, const CPfxEditLineInfo* pli)
// ��ͬ���к��������ڵ�������
LONG CPfxEditView::WrapLineSingle( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli ) {
  LONG nLines;
  CPfxEditLineInfo* pliTmp;
  CPfxEditViewWrapLineHint wl;
  switch ( GetWrapStyle() ) {
  case PFX_EC_WRAP_WINDOWEDGE:
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      pliTmp = static_cast<CPfxEditLineInfo*>( Heap.Alloc( sizeof( CPfxEditLineInfo ) ) );
      m_pdb->TextConstructEmptyLineInfo( pliTmp, 1 );
      pliTmp->dwFlags = pli->dwFlags;
      nLines = 1; // һ��
    } else {
      CClientDC1 dc( this );
      HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
      wl.m_hDC = dc;
      wl.m_nWidth = m_rectText.Width();
      wl.m_nliCur = 0;
      wl.m_nliMax = 0;
      wl.m_nGrowBy = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
      wl.m_nTabSize = GetTabSize();
      wl.m_nCharWidthMax = PFX_EC_DATA.m_nCharWidthMax;
      wl.m_nCharWidthSpace = PFX_EC_DATA.m_nCharWidthSpace;
      wl.m_nCharWidthTab = PFX_EC_DATA.m_nCharWidthTab;
      WrapLineAtUnits( Heap, pli, wl );
      nLines = wl.m_nliCur;
      dc.SelectObject( hFont );
    }
    break;
  case PFX_EC_WRAP_COLUMNS:
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      pliTmp = static_cast<CPfxEditLineInfo*>( Heap.Alloc( sizeof( CPfxEditLineInfo ) ) );
      m_pdb->TextConstructEmptyLineInfo( pliTmp, 1 );
      pliTmp->dwFlags = pli->dwFlags;
      nLines = 1; // һ��
    } else {
      wl.m_nWidth = GetWrapWidth();
      wl.m_nliCur = 0;
      wl.m_nliMax = 0;
      wl.m_nGrowBy = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
      wl.m_nTabSize = GetTabSize();
      WrapLineColumns( Heap, pli, wl );
      nLines = wl.m_nliCur;
    }
    break;
  case PFX_EC_WRAP_NONE:
  default:           // ��ͨ
    {
      pliTmp = static_cast<CPfxEditLineInfo*>( Heap.Alloc( sizeof( CPfxEditLineInfo ) ) );
      memcpy( pliTmp, pli, sizeof( CPfxEditLineInfo ) );
      pliTmp->nLen = pliTmp->nActLen;
      nLines = 1; // һ��
    }
    break;
  }
  pliTmp = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) );
  pliTmp->nIndex = pli->nIndex;
  pliTmp = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + ( nLines - 1 );
  pliTmp->dwFlags |= PFX_EC_LF_HARDRETURN;
  return nLines;
}
// LONG WrapLineMultiple(CPfxEditHeap& Heap,const CPfxEditLineInfo* pli,LONG nCount,LONG& rnLineToRemember)
// CPfxEditHeap& Heap      - ���ڴ�ŵ������е���Ϣ�Ļ�����
// const CPfxEditLineInfo* pli - ���������ĵ�һ�е�ָ��
// LONG nCount             - �ܹ��ж����н�������
// LONG& rnLineToRemember  - ����nCount�������У����ٸ�����ŵĸı䣬
//                        ����ʱ������ȷ��Χ��[0, (nCount-1)]����
//                        �غ����ķ�Χ������[0, (������ֵ-1)]
// ��ͬ���к��������ڵ�������
// rnLineToRemember - ����ʼ��ָ�򱻵���ǰ�Ǹ�Ҫ������У���������ſ�
// ���Ѿ��ı��ˣ���pli = (static_cast<CPfxEditLineInfo*>(Heap.GetBuffer())
// + rnLineToRemember)�ں������غ����ָ����
LONG CPfxEditView::WrapLineMultiple( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, LONG nCount, LONG& rnLineToRemember ) {
  ASSERT( ( rnLineToRemember >= 0 ) && ( rnLineToRemember < nCount ) );
  LONG i;
  LONG nLines;
  LONG nLineToRemember;
  CPfxEditViewWrapLineHint wl;
  CPfxEditLineInfo* pliTmp;
  switch ( GetWrapStyle() ) {
  case PFX_EC_WRAP_WINDOWEDGE: {
      CClientDC1 dc( this );
      HGDIOBJ hFont = dc.SelectObject( PFX_EC_DATA.m_hFontText );
      wl.m_hDC = dc;
      wl.m_nWidth = m_rectText.Width();
      wl.m_nliCur = 0;
      wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
      wl.m_nGrowBy = 10000;
      wl.m_nTabSize = GetTabSize();
      wl.m_nCharWidthMax = PFX_EC_DATA.m_nCharWidthMax;
      wl.m_nCharWidthSpace = PFX_EC_DATA.m_nCharWidthSpace;
      wl.m_nCharWidthTab = PFX_EC_DATA.m_nCharWidthTab;
      Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
      for ( i = 0; i < nCount; i++, pli++ ) {
        if ( i == rnLineToRemember ) {
          nLineToRemember = wl.m_nliCur;
        }
        WrapLineAtUnits( Heap, pli, wl );
      }
      dc.SelectObject( hFont );
      nLines = wl.m_nliCur;
    }
    break;
  case PFX_EC_WRAP_COLUMNS: {
      wl.m_nWidth = GetWrapWidth();
      wl.m_nliCur = 0;
      wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
      wl.m_nGrowBy = 10000;
      wl.m_nTabSize = GetTabSize();
      Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
      for ( i = 0; i < nCount; i++, pli++ ) {
        if ( i == rnLineToRemember ) {
          nLineToRemember = wl.m_nliCur;
        }
        WrapLineColumns( Heap, pli, wl );
      }
      nLines = wl.m_nliCur;
    }
    break;
  case PFX_EC_WRAP_NONE:
  default:           // ��ͨ
    {
      pliTmp = static_cast<CPfxEditLineInfo*>( Heap.Alloc( ( sizeof( CPfxEditLineInfo ) * nCount ) ) );
      memcpy( pliTmp, pli, ( sizeof( CPfxEditLineInfo ) * nCount ) );
      for ( i = 0; i < nCount; i++, pliTmp++ ) {
        pliTmp->nLen = pliTmp->nActLen;
      }
      nLines = nCount; // һ��
      nLineToRemember = rnLineToRemember;
    }
    break;
  }
  pliTmp = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) ) + ( nLines - 1 );
  pliTmp->dwFlags |= PFX_EC_LF_HARDRETURN;
  rnLineToRemember = nLineToRemember;
  return nLines;
}
// ���������ر��������ô�ӡ�����������Դ
BOOL CPfxEditView::OnSetupPrintInfo( CPfxEditPrintInfo* pPrintInfo ) {
  UNUSED( pPrintInfo );
  return true;
}
// void SelGetParam()
// �����ѡ��������ʹ�ü��̻����ı��˵�ǰλ�ã��������������µ�ѡ��λ
// ���Ծ����µ�ѡ��Χ
CPoint CPfxEditView::SelGetParam() const {
  if ( !IsTextSelected() ) {
    return m_ptEditPos;
  }
  const CPfxEditLineInfo* pliFrom = m_pdb->TextGetLineAt( m_nSelLineFrom );
  CPoint ptFrom( min( pliFrom->nLen, pliFrom->nSelFrom ), m_nSelLineFrom );
  const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( m_nSelLineTo );
  CPoint ptTo( min( pliTo->nLen, pliTo->nSelTo ), m_nSelLineTo );
  ASSERT( ( ptFrom == m_ptEditPos ) || ( ptTo == m_ptEditPos ) );
  return ( ( ptFrom != m_ptEditPos ) ? ptFrom : ptTo );
}
// void SelRange(CPoint pt)
// ����ѡ��Χ����ָ��λ��ѡ�񵽵�ǰλ��
void CPfxEditView::SelRange( CPoint pt ) {
  LONG i;
  LONG nSelSize;
  CPfxEditLineInfo* pli;
  // �ȱ��浱ǰ�����ã���������·�Χ��ʱ��Ҫ�õ�
  LONG nOldSelLineFrom = m_nSelLineFrom;
  LONG nOldSelLineTo = m_nSelLineTo;
  CPoint ptFrom = pt;
  CPoint ptTo = m_ptEditPos;
  // ȷ��������ȷ
  ptFrom.y = m_pdb->TextVerifyLine( ptFrom.y );
  ptFrom.x = m_pdb->TextVerifyChar( ptFrom.y, ptFrom.x );
  ptTo.y = m_pdb->TextVerifyLine( ptTo.y );
  ptTo.x = m_pdb->TextVerifyChar( ptTo.y, ptTo.x );
  // ����Ҫȷ��ptFrom.y��ptTo.y֮ǰ
  if ( ptFrom.y > ptTo.y ) {
    i = ptFrom.y;
    ptFrom.y = ptTo.y;
    ptTo.y = i;
    i = ptFrom.x;
    ptFrom.x = ptTo.x;
    ptTo.x = i;
  }
  // ���ѡ��Χ����һ����
  if ( ptFrom.y == ptTo.y ) {
    pli = m_pdb->TextGetLineAt( ptFrom.y );
    // ����Ҫȷ��ptFrom.x��ptTo.x֮ǰ
    if ( ptFrom.x > ptTo.x ) {
      i = ptFrom.x;
      ptFrom.x = ptTo.x;
      ptTo.x = i;
    }
    pli->nSelFrom = ptFrom.x;
    pli->nSelTo = ptTo.x;
    // ���ptFrom.y����ptTo.y��ptFrom.x����ptTo.x�͵���û��ѡ��
    // ����ͻ������ѡ��nSelSize�ֵ���������
    nSelSize = ptTo.x - ptFrom.x;
    if ( nSelSize == 0 ) {
      ptFrom.y = ptTo.y = PFX_EC_INVALID_SELECT;
    }
  }
  // ���ѡ��Χ�ڶ�����
  else {
    LONG nLineBreakSize = GetLineBreakSize();
    // ��һ��ѡ��󲿷�
    pli = m_pdb->TextGetLineAt( ptFrom.y );
    pli->nSelFrom = ptFrom.x;
    if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
      pli->nSelTo = pli->nLen + PFX_EC_VIRTUAL_LINE_BREAK_SIZE;
      nSelSize = pli->nLen - ptFrom.x + nLineBreakSize;
    } else {
      pli->nSelTo = pli->nLen;
      nSelSize = pli->nLen - ptFrom.x;
    }
    // �м���ȫ��ѡ��
    for ( i = ptFrom.y + 1; i < ptTo.y; i++ ) {
      pli = m_pdb->TextGetLineAt( i );
      pli->nSelFrom = 0;
      // ��Ӳ���з����вſ��Լ��ϻ��з��ĳ���
      if ( PFX_EC_IS_DRAW_HARDRETURN( pli ) ) {
        pli->nSelTo = pli->nLen + PFX_EC_VIRTUAL_LINE_BREAK_SIZE;
        nSelSize += ( pli->nLen + nLineBreakSize );
      } else {
        pli->nSelTo = pli->nLen;
        nSelSize += pli->nLen;
      }
    }
    // �����ѡ��ǰ����
    pli = m_pdb->TextGetLineAt( ptTo.y );
    pli->nSelFrom = 0;
    pli->nSelTo = ptTo.x;
    nSelSize += ptTo.x;
  }
  m_nSelLineFrom = ptFrom.y;
  m_nSelLineTo = ptTo.y;
  m_nSelSize = nSelSize;
  // ��������Ѿ����п�ѡ����һ��Ҫȡ���п�ѡ��
  if ( PFX_EC_SM_COLUMN == m_nSelMode ) {
    m_nSelMode = PFX_EC_SM_NORMAL;
    Invalidate( false );
  } else {
    // ���¸ı�ķ�Χ
    UpdateLinesSelected( nOldSelLineFrom, nOldSelLineTo );
  }
}
// void SelRange(CRect& rect)
// ִ���п�ѡ��
// CRect& rect - ���ı�����ָ��Ҫѡ��ķ�Χ������þ���Ϊ����ȡ����ǰ��
//            ����ʽ��ѡ��
// �ı������ǲ���ָϵͳ�߼�����Ҳ����ָ��ʾ���꣬�������Ƕ��ĵ�����Ӱ��
// ���ԣ�����ԭ���ǵ�һ�е�һ���ַ������Ͻǣ���λ��ϵͳ�߼���λ
void CPfxEditView::SelRange( CRect& rect ) {
  if ( IsTextSelected() ) {
    SelNone();
  }
  rect.NormalizeRect();
  LONG nLineFrom = TransTextPosToLinePos( rect.top );
  LONG nLineTo = TransTextPosToLinePos( rect.bottom );
  LONG nSelSize = 0;
  LONG xTextPosMax = 0;
  for ( LONG i = nLineFrom; i <= nLineTo; i++ ) {
    LONG xTextPos1 = rect.left;
    LONG xEditPos1 = TransTextPosToCharPos( i, xTextPos1 );
    LONG xTextPos2 = rect.right;
    LONG xEditPos2 = TransTextPosToCharPos( i, xTextPos2 );
    xTextPosMax = max( xTextPosMax, xTextPos2 );
    CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
    pli->nSelFrom = xEditPos1;
    pli->nSelTo = xEditPos2;
    nSelSize += ( xEditPos2 - xEditPos1 );
  }
  m_nSelLineFrom = nLineFrom;
  m_nSelLineTo = nLineTo;
  m_nSelSize = nSelSize;
  m_nSelMode = PFX_EC_SM_COLUMN;
  m_rectColSelect.SetRect( rect.left, rect.top, xTextPosMax, rect.bottom );
  // ���¸ı�ķ�Χ
  UpdateLinesSelected( PFX_EC_INVALID_SELECT, PFX_EC_INVALID_SELECT );
}
// void SelWord(CPoint pt)
// ѡ��һ��
// CPoint pt - ���λ��
// ���ѡ��ɹ����µĵ�ǰλ�ý���������ѡ��Χ�������
void CPfxEditView::SelWord( CPoint pt ) {
  ASSERT( m_pdb->TextIsValidLine( pt.y ) );
  ASSERT( m_pdb->TextIsValidChar( pt.y, pt.x ) );
  // �ڶ�дһ���е�����֮ǰ��Ҫע�⵱ǰλ���Ƿ�����ĩ������ĩ��д�Ƿ�
  // ���ġ������ںܶ��й���һ��黺�������ܶ�ʱ��ʹ�Ƿ���д��ĩ����
  // Ҳ���ἴʱ�����⣬����������λ�ڻ���������󲿷֣�����Ҫ����С��
  m_pdb->TextSubLineToMainLine( pt );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
    m_ptEditPos.y = pt.y;
    m_ptEditPos.x = pt.x = 0;
  } else {
    LPCTSTR pStart = pli->pData;
    LPCTSTR pCur = pStart + pt.x;
    LPCTSTR pEnd = pStart + pli->nActLen;
    // ע��ָ���������
    LPCTSTR pWordLeft = pCur;
    if ( !IsDelimiter( pCur, pEnd ) ) {
      pWordLeft = GetPrevDelimiterPos( pCur, pStart );
    }
    LPCTSTR pWordRight = GetNextCharPos( pCur, pEnd );
    if ( !IsDelimiter( pCur, pEnd ) ) {
      pWordRight = GetNextDelimiterPos( pCur, pEnd );
    }
    pt.x = pWordLeft - pStart;
    m_ptEditPos.x = pWordRight - pStart;
    m_ptEditPos.y = pt.y;
    m_pdb->TextMainLineToSubLine( pt );
    m_pdb->TextMainLineToSubLine( m_ptEditPos );
  }
  // ��ptѡ����m_ptEditPos
  SelRange( pt );
}
// void SelLine(LONG nLine)
// ѡ��һ����
void CPfxEditView::SelLine( LONG nLine ) {
  CPoint ptFrom;
  // ȷ��λ����ȷ
  m_ptEditPos.y = m_pdb->TextVerifyLine( nLine );
  m_ptEditPos.x = 0;
  // ����һ�еĿ�ʼѡ�񵽱��еĿ�ʼ��������лس��ͻ����س�һ��ѡ��
  if ( m_ptEditPos.y < m_pdb->TextGetLineUpperBound() ) {
    ptFrom.y = m_ptEditPos.y + 1;
    ptFrom.x = 0;
  } else {
    ptFrom.y = m_ptEditPos.y;
    ptFrom.x = m_pdb->TextGetLineAt( ptFrom.y ) ->nLen;
  }
  // ��ptFromѡ����m_ptEditPos
  SelRange( ptFrom );
}
// void SelNone()
// ȡ��ѡ��
void CPfxEditView::SelNone() {
  if ( !IsTextSelected() ) {
    return ;
  }
  LONG nOldSelLineFrom = m_nSelLineFrom;
  LONG nOldSelLineTo = m_nSelLineTo;
  m_nSelLineFrom = m_nSelLineTo = PFX_EC_INVALID_SELECT;
  m_nSelSize = 0;
  m_nSelMode = PFX_EC_SM_NORMAL;
  // ���¸ı�ķ�Χ
  UpdateLinesSelected( nOldSelLineFrom, nOldSelLineTo );
}
void CPfxEditView::SelNoneAndRemoveSelected() {
  if ( !IsTextSelected() ) {
    return ;
  }
  // ��ȡѡ��Χ
  CPoint ptFrom;
  CPoint ptTo;
  ptFrom.y = m_nSelLineFrom;
  ptTo.y = m_nSelLineTo;
  // ׼�����������ֵ���ͼ
  DWORD dwFlags = ( ( ptFrom.y != ptTo.y ) ? PFX_EC_UT_VERT : PFX_EC_UT_HORZ );
  LONG nFirstUpdateLine = ptFrom.y;
  LONG nLastUpdateLine = ptTo.y;
  BOOL bColSelect = IsColumnSelect();
  // ȡ��ѡ��
  SelNone();
  // ����ѡ��
  if ( !bColSelect ) {
    ptFrom.x = m_pdb->TextGetLineAt( ptFrom.y ) ->nSelFrom;
    ptTo.x = m_pdb->TextGetLineAt( ptTo.y ) ->nSelTo;
    CharsRemove( ptFrom, ptTo, true );
    ASSERT( m_pdb->TextIsValidLine( ptFrom.y ) );
    ASSERT( m_pdb->TextIsValidChar( ptFrom.y, ptFrom.x ) );
  }
  // �п�ѡ��
  else {
    ASSERT( !IsWrapping() );
    // ִ��ɾ������
    for ( LONG i = ptTo.y, j = ptFrom.y; i >= j; i-- ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      ptFrom.y = ptTo.y = i;
      ptFrom.x = pli->nSelFrom;
      ptTo.x = pli->nSelTo;
      CharsRemove( ptFrom, ptTo, ( i == j ) );
      ASSERT( m_pdb->TextIsValidLine( ptFrom.y ) );
      ASSERT( m_pdb->TextIsValidChar( ptFrom.y, ptFrom.x ) );
    }
  }
  if ( GetSiblingView() != NULL ) {
    UpdateAllViews( dwFlags, nFirstUpdateLine, nLastUpdateLine );
  }
}
void CPfxEditView::SelNoneAndUpdateCurrentPosition( LONG nAction ) {
  ASSERT( IsTextSelected() );
  CPoint ptFrom;
  CPoint ptTo;
  ptFrom.y = m_nSelLineFrom;
  ptTo.y = m_nSelLineTo;
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( ptFrom.y );
  ptFrom.x = min( pli->nLen, pli->nSelFrom );
  pli = m_pdb->TextGetLineAt( ptTo.y );
  ptTo.x = min( pli->nLen, pli->nSelTo );
  switch ( nAction ) {
  case VK_LEFT:           //      VK_UP, VK_PRIOR, VK_HOME��һ���Ĵ���
  case VK_UP:
  case VK_PRIOR:
  case VK_HOME:
    m_ptEditPos = ptFrom;
    break;
  case VK_RIGHT:           //      VK_DOWN, VK_NEXT, VK_END��һ���Ĵ���
  case VK_DOWN:
  case VK_NEXT:
  case VK_END:
    m_ptEditPos = ptTo;
    break;
  default:
    TRACE( "CPfxEditView::SelSelNoneAndUpdateCurrentPosition(): Unknown nAction %d\n", nAction );
    break;
  }
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  LONG xTextPos = TransCharPosToTextPos( m_ptEditPos );
  UpdateCurrentPosition( xTextPos, true, true );
  SelNone();
}
// void SetTracking(BOOL bOn)
// �Ƿ�ʹ��SetCapture()�������
void CPfxEditView::SetTracking( BOOL bOn ) {
  if ( bOn ) {
    SetCapture();
  } else {
    if ( ::GetCapture() == M_HWND ) {
      ::ReleaseCapture();
    }
  }
  m_bTrackingMouse = bOn;
}
// BOOL EditUndo()
// ִ��UNDO����
BOOL CPfxEditView::EditUndo() {
  if ( !IsCanUndo() ) {
    return false;
  }
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewWorkingState state( this, ( ( GetUndoName() == PFX_EC_UNDO_REPLACE_ALL ) ) );
  LONG nLineCount = m_pdb->TextGetLineCount();
  // ��ʼUNDOѭ����һ��UNDO�����κ�һ����������PFX_EC_UF_GROUP��־��һ���е�
  // ���һ��������PFX_EC_UF_BEGIN_GROUP��־
  CPoint pt( 0, 0 );
  WORD fAction;
  do {
    const CPfxEditUndoInfo* pui = m_pdb->UndoGetInfo();
    if ( pui == NULL ) {
      break;
    }
    fAction = pui->fAction;
    if ( fAction & PFX_EC_UF_INSERT ) {
      m_pdb->UndoInsert( const_cast<CPfxEditUndoInfo*>( pui ), pt, false );
    } else {
      m_pdb->UndoRemove( const_cast<CPfxEditUndoInfo*>( pui ), pt, false );
    }
  } while ( !( fAction & PFX_EC_UF_BEGIN_GROUP ) && ( fAction & PFX_EC_UF_GROUP ) );
  m_ptEditPos = pt;
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  // ��������ı��˻����������ݣ��ܶ����δ��UpdateAllViews()��
  // EnsureVisible()����֮ǰ������Ч������ҪС�ĵ��ú���
  // �������������nFirstUpdateLine=GetFirstVisibleLine()�㲻��ȷ
  DWORD dwFlags = ( ( m_pdb->TextGetLineCount() != nLineCount ) ? PFX_EC_UT_VERT : PFX_EC_UT_HORZ );
  UpdateAllViews( dwFlags, 0, -1 );
  EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  return true;
}
// ִ��REDO����
BOOL CPfxEditView::EditRedo() {
  if ( !IsCanRedo() ) {
    return false;
  }
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewWorkingState state( this, ( ( GetUndoName() == PFX_EC_UNDO_REPLACE_ALL ) ) );
  LONG nLineCount = m_pdb->TextGetLineCount();
  // ��ʼREDOѭ����һ��REDO�����κ�һ����������PFX_EC_UF_GROUP��־��һ���е�
  // ��һ��������PFX_EC_UF_BEGIN_GROUP��־
  CPoint pt( 0, 0 );
  const CPfxEditUndoInfo* pui = m_pdb->UndoGetRedoInfo();
  WORD fAction = pui->fAction;
  do {
    if ( fAction & PFX_EC_UF_INSERT ) {
      m_pdb->UndoRemove( const_cast<CPfxEditUndoInfo*>( pui ), pt, true );
    } else {
      m_pdb->UndoInsert( const_cast<CPfxEditUndoInfo*>( pui ), pt, true );
    }
    pui = m_pdb->UndoGetRedoInfo();
    if ( pui == NULL ) {
      break;
    }
    fAction = pui->fAction;
  } while ( !( fAction & PFX_EC_UF_BEGIN_GROUP ) && ( fAction & PFX_EC_UF_GROUP ) );
  m_ptEditPos = pt;
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  // ��������ı��˻����������ݣ��ܶ����δ��UpdateAllViews()��
  // EnsureVisible()����֮ǰ������Ч������ҪС�ĵ��ú���
  // �������������nFirstUpdateLine=GetFirstVisibleLine()�㲻��ȷ
  DWORD dwFlags = ( ( m_pdb->TextGetLineCount() != nLineCount ) ? PFX_EC_UT_VERT : PFX_EC_UT_HORZ );
  UpdateAllViews( dwFlags, 0, -1 );
  EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  return true;
}
// ����ǰѡ�����ݸ��Ƶ�������
// BOOL bAppend - �Ƿ�ѡ�����ݸ��ӵ����������е����ݺ���
BOOL CPfxEditView::EditCopy( BOOL bAppend ) {
  HGLOBAL hGlobal = DragDropRender( false, bAppend );
  if ( hGlobal == NULL ) {
    return false;
  }
  CPfxEditViewClipboard clipboard( this, CF_TEXT, false );
  clipboard.Attach( hGlobal );
  return clipboard.SetClipboardData();
}
// ���е�ǰ��ѡ������
// BOOL bAppend - �Ƿ񽫵�ǰѡ�����ݸ��ӵ����������е����ݺ���
BOOL CPfxEditView::EditCut( BOOL bAppend ) {
  if ( !IsTextSelected() ) {
    return false;
  }
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( !EditCopy( bAppend ) ) {
    return false;
  }
  return EditRemove( -1, 0, -1, 0 );
}
// ��ָ��λ��ճ�����Լ������ڵĿ�������
// LONG nLine, LONG nChar - ճ��λ��
// LONG nType             - ճ�����ͣ���ѡ�У�
//                       PFX_EC_PASTE_NORMAL        - ��������ʽճ��
//                       PFX_EC_PASTE_COLUMN_ALIGN  - ���п���뷽ʽճ��
//                       PFX_EC_PASTE_COLUMN_APPEND - ���п鸽�ӷ�ʽճ��
// 1) ���nLine��nChar������-1���ӵ�ǰλ��ճ��
// 2) �����ǰ�����Զ�����ģʽ���п�ճ����������Ч
// 3) ���nType����PFX_EC_PASTE_NORMAL����֮ǰ���й��п鸴�ƣ�����ǰ��
// ���������п�ճ��������Ȼ���п�ճ��
// 4) ���nType����PFX_EC_PASTE_COLUMN_ALIGN��PFX_EC_PASTE_COLUMN_APPEND����ǰ
// ���ò������п�ճ�������ʧ��
// 5) ��nType����PFX_EC_PASTE_COLUMN_APPENDʱ��������ĩ�������ݣ�nChar����
// ��nLineһ������ָʾ�Ƿ��ڵ�ǰ��ճ��
// �ɹ�����TRUE��ʧ�ܷ���FALSE
BOOL CPfxEditView::EditPaste( LONG nLine, LONG nChar, LONG nType ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  BOOL bCanPaste;
  switch ( nType ) {
  default:
  case PFX_EC_PASTE_NORMAL:
    bCanPaste = IsCanPaste();
    break;
  case PFX_EC_PASTE_COLUMN_ALIGN:
  case PFX_EC_PASTE_COLUMN_APPEND:
    bCanPaste = IsCanColumnPaste();
    break;
  }
  if ( !bCanPaste ) {
    return false;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_PASTE );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
  }
  CPoint pt( nChar, nLine );
  // �ӵ�ǰλ�ÿ�ʼճ����
  if ( ( pt.y == PFX_EC_INVALID_LINE ) && ( pt.x == PFX_EC_INVALID_CHAR ) ) {
    pt = m_ptEditPos;
  } else {
    m_pdb->TextAbsLineToSubLine( pt );
    // ������Ƿ�Ϸ�����ֹ��MBCS�ַ����м�
    VerifyPointerPosition( pt );
  }
  // ��ȡ����������
  CPfxEditViewClipboard clipboard( this, CF_TEXT, true );
  LPCTSTR pData = clipboard.GetClipboardData();
  if ( pData == NULL ) {
    return false;
  }
  LONG cch = _tcslen( pData );
  switch ( nType ) {
  case PFX_EC_PASTE_COLUMN_ALIGN:
    PasteColumnAlign( pt, pData, cch );
    break;
  case PFX_EC_PASTE_COLUMN_APPEND:
    PasteColumnAppend( pt, pData, cch );
    break;
  case PFX_EC_PASTE_NORMAL:
  default:
    // ��Ȼ������ճ���������֮ǰʹ�����п鸴�ƣ�������ȻҪ����
    // ��ճ��
    if ( !IsCanColumnPaste()
         || !IsDataColumnModeRendered( false, clipboard.GetHandle() ) ) {
      PasteNormal( pt, pData, cch );
    } else {
      PasteColumnAlign( pt, pData, cch );
    }
    break;
  }
  return true;
}
// ɾ��ָ����Χ�ڵ�����
// 1�����nFromLine��nToLine������-1��ɾ����ǰ�ַ�����ѡ������
// 2�����nFromLine����0��nToLine����-1������ĵ�
// 3�������ɾ��һ�У�Ӧ�ô�ĳ�еĵ�һ��ɾ������һ�еĵ�һ��
// *** ���в������Ƕ������к��ж��Եģ����в�����һ�� ***
BOOL CPfxEditView::EditRemove( LONG nFromLine, LONG nFromChar, LONG nToLine, LONG nToChar ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  // ɾ����ǰ�ַ�����ѡ�����ݣ�
  if ( ( nFromLine == PFX_EC_INVALID_LINE ) && ( nToLine == PFX_EC_INVALID_LINE ) ) {
    OnKeyDelete( false );
    return true;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_REMOVE );
  if ( IsTextSelected() ) {
    SelNone();
  }
  ASSERT( !IsTextSelected() );
  CPoint ptFrom( nFromChar, nFromLine );
  CPoint ptTo( nToChar, nToLine );
  // ����ĵ���
  if ( ( ptFrom.y == 0 ) && ( ptTo.y == PFX_EC_INVALID_LINE ) ) {
    ptFrom.x = 0;
    ptFrom.y = 0;
    ptTo.y = m_pdb->TextGetLineUpperBound();
    ptTo.x = m_pdb->TextGetLineLength( ptTo.y );
  } else // ����ɾ���ĵ������ַ�
  {
    m_pdb->TextAbsLineToSubLine( ptFrom );
    VerifyPointerPosition( ptFrom ); // ��ָֹ����MBCS�ַ����м�
    m_pdb->TextAbsLineToSubLine( ptTo );
    VerifyPointerPosition( ptTo ); // ��ָֹ����MBCS�ַ����м�
  }
  if ( ( ptFrom.y == ptTo.y ) && ( ptFrom.x > ptTo.x ) ) {
    LONG i = ptFrom.x;
    ptFrom.x = ptTo.x;
    ptTo.x = i;
  }
  CharsRemove( ptFrom, ptTo, true );
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  return true;
}
// ��ָ��λ�ò����ַ�������
// LONG nLine, LONG nChar - Ҫ�����ַ���λ��
// LPCTSTR pData          - Ҫ������ַ��Ļ�����ָ��
// LONG cch               - �ַ����ȵ�λ��TCHAR
// ���nLine��nChar������-1���ӵ�ǰλ�ò���
// ���в������Ƕ������к��ж��Եģ����в�����һ��
BOOL CPfxEditView::EditInsert( LONG nLine, LONG nChar, LPCTSTR pData, LONG cch ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_INSERT );
  if ( IsTextSelected() ) {
    SelNoneAndRemoveSelected();
  }
  CPoint pt( nChar, nLine );
  // �ӵ�ǰλ�ò��룿
  if ( ( pt.x == PFX_EC_INVALID_CHAR ) && ( pt.y == PFX_EC_INVALID_LINE ) ) {
    pt = m_ptEditPos;
  } else {
    m_pdb->TextAbsLineToSubLine( pt );
    VerifyPointerPosition( pt ); // ��ָֹ����MBCS�ַ����м�
  }
  CharsInsert( pt, pData, cch, true );
  ASSERT( m_pdb->TextIsValidLine( m_ptEditPos.y ) );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  return true;
}
// ѡ��ָ����Χ�ڵ�����
// 1�����nFromLine��nToLine������-1��ȡ���κ�����ѡ��
// 2�����nFromLine����0��nToLine����-1��ȫѡ
// 3�������ѡ��һ���а������з���Ӧ��ĳ�еĵ�һ��ѡ����һ�еĵ�һ��
// ���в������Ƕ������к��ж��Եģ����в�����һ��
BOOL CPfxEditView::EditSelect( LONG nFromLine, LONG nFromChar, LONG nToLine, LONG nToChar ) {
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  CPoint ptFrom( nFromChar, nFromLine );
  CPoint ptTo( nToChar, nToLine );
  // ȡ��ѡ��
  if ( ( ptFrom.y == PFX_EC_INVALID_LINE ) && ( ptTo.y == PFX_EC_INVALID_LINE ) ) {
    SelNone();
    return true;
  }
  // ȫѡ��
  if ( ( ptFrom.y == 0 ) && ( ptTo.y == PFX_EC_INVALID_LINE ) ) {
    ptFrom.x = 0;
    ptTo.y = m_pdb->TextGetLineUpperBound();
    ptTo.x = m_pdb->TextGetLineLength( ptTo.y );
  } else {
    // ����λ��ת�������λ�ò�У���������ȷ��
    m_pdb->TextAbsLineToSubLine( ptFrom );
    VerifyPointerPosition( ptFrom ); // ��ָֹ����MBCS�ַ����м�
    m_pdb->TextAbsLineToSubLine( ptTo );
    VerifyPointerPosition( ptTo ); // ��ָֹ����MBCS�ַ����м�
  }
  CPoint cpOld = m_ptEditPos;
  m_ptEditPos = ptTo;
  SelRange( ptFrom );
  // �ɵĵ�ǰλ�ñ��ı�֮ǰû�и��£�����Ҫ������һ��
  UpdateLines( cpOld.y, cpOld.y );
  EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  return true;
}
// ɾ����ĩ�հ��ַ�
BOOL CPfxEditView::EditTrimTrailingSpaces() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_TRIM_TRAILING_SPACES );
  // �޸�ǰ�ȱ��浱ǰ�ı༭λ���Ա��޸ĺ��ܹ��ָ�
  CPoint cpOld = m_ptEditPos;
  m_pdb->TextSubLineToAbsLine( cpOld );
  BOOL bNeedUpdate = false;
  for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
    CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      continue;
    }
    const LPCTSTR pStart = pli->pData;
    const LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pCur = pEnd - 1; // ָ�����һ���ַ�
    // Ŀǰ���е�MBCS�ַ�����û���κ�һ��MBCS�ַ���β�ֽ�����
    // _istspace()����ʾ�Ŀհ��ַ���Χ��
    while ( ( pCur >= pStart ) && IsWhitespace( *pCur ) ) {
      pCur--;
    }
    if ( ++pCur >= pEnd )           //      ��ĩû���пհ��ַ���
    {
      continue;
    }
    CPoint ptFrom( ( pCur - pStart ), nLine );
    CPoint ptTo( ( pEnd - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptFrom );
    m_pdb->TextMainLineToSubLine( ptTo );
    // VIEWָ��ΪNULL��ʾ�޸����ݺ���Ҫ���£��Ա�ӿ��ٶ�
    CharsRemove( ptFrom, ptTo, false );
    bNeedUpdate = true;
  }
  // ��������ı��˻����������ݣ��ܶ����δ��UpdateAllViews()��
  // EnsureVisible()����֮ǰ������Ч������ҪС�ĵ��ú���
  if ( bNeedUpdate ) {
    m_ptEditPos = cpOld;
    m_pdb->TextAbsLineToSubLine( m_ptEditPos );
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return true;
}
// �ո��ַ�ת�����Ʊ��
// BOOL bLeading - �Ƿ�ֻ�����׵Ŀո�ת��������������
// �ɹ�����TRUE��ʧ�ܷ���FALSE
// �ҵ�һ���ո񣬼��������Ʊ����������չ�ɿո�ʱӦ�������ɸ��ո���
// ���ô�����ո�ʼ�������������㹻��Ŀ�Ŀո����������㣬����ת����
// �Ʊ�������򽫺�������ո�
BOOL CPfxEditView::EditSpacesToTabs( BOOL bLeading ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewUndoManager um( this, ( bLeading ? PFX_EC_UNDO_SPACES_TO_TABS_LEADING : PFX_EC_UNDO_SPACES_TO_TABS ) );
  // ��ȡ��ǰ�༭λ�õľ������꣬�Ա����޸ĺ��ܹ��ָ���ǰ�༭λ��
  CPoint cpOld = m_ptEditPos;
  m_pdb->TextSubLineToAbsLine( cpOld );
  BOOL bNeedUpdate = false;
  for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
    const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      continue;
    }
    // ��ʼɨ��һ��
    LPCTSTR pStart = pli->pData;
    LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pCur = pStart;
    LONG nTabSize = GetTabSize();
    LONG nLogChar = 0;
    // Ŀǰ����MBCS�ַ�����û���κ�һ��MBCS�ַ���ǰ���ֽڻ�β��
    // �ڰ����ո����Կ���ʹ��дָ��򵥵���
    while ( pCur < pEnd ) {
      // ������ǿո�������������Ʊ��������Ҫ���ٸ���
      // ����ܴ�������Ʊ����
      if ( PFX_EC_CHR_SPACE != *pCur ) {
        if ( PFX_EC_CHR_TAB != *pCur ) {
          //   ������һ�ǿհ��ַ�����һ�е�ɨ��
          //   ��ֹͣ
          if ( bLeading ) {
            break;
          }
          nLogChar++;
        } else {
          nLogChar += nTabSize - ( nLogChar % nTabSize );
        }
        pCur++;
        continue;
      }
      // ���������������Ʊ����������չ�ɶ��ٸ��ո�
      LONG nCurTabSize = nTabSize - ( nLogChar % nTabSize );
      // ���������ַ��а������㹻��Ŀո���
      LONG nSpaceCount = nCurTabSize - 1;
      // ��һ��Ϊ�������Ѿ����ֵĿո�
      LPCTSTR pTmp = pCur + 1;
      while ( ( pTmp < pEnd )
              && ( nSpaceCount > 0 )
              && ( PFX_EC_CHR_SPACE == *pTmp ) ) {
        pTmp++;
        nSpaceCount--;
      }
      // YES���������㣬���Խ���nCurTabSize���ո�ת����һ
      // ���Ʊ��
      if ( nSpaceCount != 0 ) {
        nLogChar += ( pTmp - pCur );
        pCur = pTmp;
        continue;
      }
      // �滻���µĿ�ʼɨ��λ��
      LONG nNewScanOffs = pCur - pStart + 1;
      // ����nCurTabSize���ո���һ���Ʊ���滻����
      CPoint ptFrom( ( pCur - pStart ), nLine );
      m_pdb->TextMainLineToSubLine( ptFrom );
      CPoint ptTo( ( pTmp - pStart ), nLine );
      m_pdb->TextMainLineToSubLine( ptTo );
      TCHAR szTAB[ 1 ];
      szTAB[ 0 ] = PFX_EC_CHR_TAB;
      CharsReplace( ptFrom, ptTo, szTAB, 1, false );
      // ��������޸��Ժ���Ҫ����ָ��
      pli = m_pdb->TextGetLineAt( nLine );
      pStart = pli->pData;
      pEnd = pStart + pli->nActLen;
      pCur = pStart + nNewScanOffs;
      nLogChar += nCurTabSize;
      bNeedUpdate = true;
    }
  }
  if ( bNeedUpdate ) {
    // �����ǰ�༭λ��λ�ڱ��滻��nCurTabSize�������Ŀո��У���
    // Ҫͬ�����µ�ǰ�༭λ��
    m_ptEditPos.y = cpOld.y;
    m_ptEditPos.x = CalcAbsCharPosFromLogCharPos( cpOld.y, m_ptLogiPos.x );
    m_pdb->TextAbsLineToSubLine( m_ptEditPos );
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return true;
}
// �Ʊ��ת���ɿո��ַ�
BOOL CPfxEditView::EditTabsToSpaces() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  if ( IsTextSelected() ) {
    SelNone();
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_TABS_TO_SPACES );
  // ��ȡ��ǰ�༭λ�õľ������꣬�Ա����޸ĺ��ܹ��ָ���ǰ�༭λ��
  CPoint cpOld = m_ptEditPos;
  m_pdb->TextSubLineToAbsLine( cpOld );
  BOOL bNeedUpdate = false;
  for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
    const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
    if ( PFX_EC_IS_EMPTY_LINE( pli ) ) {
      continue;
    }
    // ��ʼɨ��һ��
    LPCTSTR pStart = pli->pData;
    LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pCur = pStart;
    LONG nTabSize = GetTabSize();
    LONG nLogChar = 0;
    // Ŀǰ����MBCS�ַ�����û���κ�һ��MBCS�ַ���ǰ���ֽڻ�β��
    // �ڰ����ո����Կ���ʹ��дָ��򵥵���
    while ( pCur < pEnd ) {
      if ( PFX_EC_CHR_TAB != *pCur ) {
        pCur++;
        nLogChar++;
        continue;
      }
      // ������㽫����չ�ɶ��ٸ��ո�
      LONG nCurTabSize = nTabSize - ( nLogChar % nTabSize );
      // �滻���µĿ�ʼɨ��λ��
      LONG nNewScanOffs = pCur - pStart + nCurTabSize;
      // ����nCurTabSize���ո���һ���Ʊ���滻����
      CPoint ptFrom( ( pCur - pStart ), nLine );
      m_pdb->TextMainLineToSubLine( ptFrom );
      CPoint ptTo( ( ptFrom.x + 1 ), ptFrom.y );
      TCHAR sz[ PFX_EC_MAX_TAB_SIZE ];
      for ( LONG i = 0; i < nCurTabSize; i++ ) {
        sz[ i ] = PFX_EC_CHR_SPACE;
      }
      CharsReplace( ptFrom, ptTo, sz, nCurTabSize, false );
      // ��������޸��Ժ���Ҫ����ָ��
      pli = m_pdb->TextGetLineAt( nLine );
      pStart = pli->pData;
      pEnd = pStart + pli->nActLen;
      pCur = pStart + nNewScanOffs;
      nLogChar += nCurTabSize;
      bNeedUpdate = true;
    }
  }
  if ( bNeedUpdate ) {
    // �����ǰ�༭λ��λ�ڱ��滻��nCurTabSize�������Ŀո��У���
    // Ҫͬ�����µ�ǰ�༭λ��
    m_ptEditPos.y = cpOld.y;
    m_ptEditPos.x = CalcAbsCharPosFromLogCharPos( cpOld.y, m_ptLogiPos.x );
    m_pdb->TextAbsLineToSubLine( m_ptEditPos );
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return true;
}
// ��ָ����������Χ�ڵ��ַ�ת����Сд���д
// LONG nLine  - ����Ե�����
// LONG nStart - �����еĿ�ʼת��λ��
// LONG nEnd   - ��ֹת�����ַ�λ��
// BOOL bLower - Сд���Ǵ�д
// ���ִ����ת������TRUE�����򷵻�FALSE
BOOL CPfxEditView::CaseChange( LONG nLine, LONG nStart, LONG nEnd, BOOL bLower ) {
  if ( !m_pdb->TextIsValidLine( nLine ) ) {
    return false;
  }
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
  nStart = min( nStart, nEnd );
  nEnd = max( nEnd, nStart );
  nStart = min( nStart, pli->nActLen );
  nEnd = min( nEnd, pli->nActLen );
  // ���л��㳤�ȣ�
  if ( PFX_EC_IS_EMPTY_LINE( pli ) || ( nStart == nEnd ) ) {
    return false;
  }
  // ��ס����λ��
  CPoint ptAbs( 0, nLine );
  m_pdb->TextMainLineToAbsLine( ptAbs );
  WORD wCaseType = ( WORD ) ( ( bLower ? C1_LOWER : C1_UPPER ) );
  BOOL bModified = false;
  LPCTSTR pStart = pli->pData;
  LPCTSTR pEnd = pStart + nEnd;
  LPCTSTR pCur = pStart + nStart;
  // ��ʼɨ�裬����ķ����ܹ�ʶ��MBCS�ַ��Ĵ�д��Сд������������ANSI
  // �Ĵ�Сд
  while ( pCur < pEnd ) {
    LPCTSTR pTmp = pCur;
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    WORD wCharType[ PFX_EC_MAX_MB_LEN ];
    ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pTmp, min( pCur, pEnd ) - pTmp, wCharType );
    if ( ( ( wCharType[ 0 ] & C1_ALPHA ) == 0 )
         || ( ( wCharType[ 0 ] & ( C1_UPPER | C1_LOWER ) ) == 0 )
         || ( ( wCharType[ 0 ] & wCaseType ) != 0 ) ) {
      continue;
    }
    // ��ʱ��������ÿ�������PFX_EC_SIZEOF(sz)���ַ�
    TCHAR sz[ 256 ];
    LPCTSTR pMax = min( ( pTmp + PFX_EC_SIZEOF( sz ) ), pEnd );
    while ( pCur < pMax ) {
      LPCTSTR pT = pCur + ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
      ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pCur, min( pT, pEnd ) - pCur, wCharType );
      if ( ( ( wCharType[ 0 ] & C1_ALPHA ) == 0 )
           || ( ( wCharType[ 0 ] & ( C1_UPPER | C1_LOWER ) ) == 0 )
           || ( ( wCharType[ 0 ] & wCaseType ) != 0 ) ) {
        break;
      }
      pCur = pT;
    }
    // ִ��ת����������һ���飩��д�ַ�ת����Сд
    pCur = min( pCur, pEnd );
    LONG cch = pCur - pTmp;
    memcpy( sz, pTmp, ( cch * sizeof( TCHAR ) ) );
    if ( bLower ) {
      ::CharLowerBuff( sz, cch );
    } else {
      ::CharUpperBuff( sz, cch );
    }
    // �滻���µĿ�ʼɨ��λ��
    LONG nResumePosCur = pCur - pStart;
    // ��ת���õ��ַ��滻��ԭ�����ַ�
    CPoint ptFrom( ( pTmp - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptFrom );
    CPoint ptTo( ( pCur - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptTo );
    CharsReplace( ptFrom, ptTo, sz, cch, false );
    // �޸��Ժ����λ�ÿ��ܻ�ı䣬���Ҫʹ�þ���λ����ӳ�����
    // λ��
    CPoint pt( 0, ptAbs.y );
    m_pdb->TextAbsLineToMainLine( pt );
    nLine = pt.y;
    // ��������޸��Ժ���Ҫ����ָ��
    pli = m_pdb->TextGetLineAt( nLine );
    pStart = pli->pData;
    pEnd = pStart + nEnd;
    pCur = pStart + nResumePosCur;
    bModified = true;
  }
  return bModified;
}
BOOL CPfxEditView::CaseInvert( LONG nLine, LONG nStart, LONG nEnd ) {
  if ( !m_pdb->TextIsValidLine( nLine ) ) {
    return false;
  }
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
  nStart = min( nStart, nEnd );
  nEnd = max( nEnd, nStart );
  nStart = min( nStart, pli->nActLen );
  nEnd = min( nEnd, pli->nActLen );
  // ���л��㳤�ȣ�
  if ( PFX_EC_IS_EMPTY_LINE( pli ) || ( nStart == nEnd ) ) {
    return false;
  }
  // ��ס����λ��
  CPoint ptAbs( 0, nLine );
  m_pdb->TextMainLineToAbsLine( ptAbs );
  BOOL bModified = false;
  LPCTSTR pStart = pli->pData;
  LPCTSTR pEnd = pStart + nEnd;
  LPCTSTR pCur = pStart + nStart;
  // ��ʼɨ��
  while ( pCur < pEnd ) {
    LPCTSTR pTmp = pCur;
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    WORD wCharType[ PFX_EC_MAX_MB_LEN ];
    ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pTmp, min( pCur, pEnd ) - pTmp, wCharType );
    if ( ( wCharType[ 0 ] & C1_ALPHA ) == 0 ) {
      continue;
    }
    WORD wCaseType = ( WORD ) ( ( wCharType[ 0 ] & ( C1_LOWER | C1_UPPER ) ) );
    if ( wCaseType == 0 ) {
      continue;
    }
    // ��ʱ��������ÿ�������PFX_EC_SIZEOF(sz)���ַ�
    TCHAR sz[ 256 ];
    LPCTSTR pMax = min( ( pTmp + PFX_EC_SIZEOF( sz ) ), pEnd );
    while ( pCur < pMax ) {
      LPCTSTR pT = pCur + ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
      ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pCur, min( pT, pEnd ) - pCur, wCharType );
      if ( ( wCharType[ 0 ] & C1_ALPHA ) == 0 ) {
        break;
      }
      if ( ( wCharType[ 0 ] & ( C1_LOWER | C1_UPPER ) ) != wCaseType ) {
        break;
      }
      pCur = pT;
    }
    // ִ��ת����������һ���飩��д�ַ�ת����Сд
    pCur = min( pCur, pEnd );
    LONG cch = pCur - pTmp;
    memcpy( sz, pTmp, ( cch * sizeof( TCHAR ) ) );
    if ( wCaseType & C1_UPPER )           //      ��ת��Сд
    {
      ::CharLowerBuff( sz, cch );
    } else {
      ::CharUpperBuff( sz, cch );
    }
    // �滻���µĿ�ʼɨ��λ��
    LONG nResumePosCur = pCur - pStart;
    // ��ת���õ��ַ��滻��ԭ�����ַ�
    CPoint ptFrom( ( pTmp - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptFrom );
    CPoint ptTo( ( pCur - pStart ), nLine );
    m_pdb->TextMainLineToSubLine( ptTo );
    CharsReplace( ptFrom, ptTo, sz, cch, false );
    // �޸��Ժ����λ�ÿ��ܻ�ı䣬���Ҫʹ�þ���λ����ӳ�����
    // λ��
    CPoint pt( 0, ptAbs.y );
    m_pdb->TextAbsLineToMainLine( ptAbs );
    nLine = pt.y;
    // ��������޸��Ժ���Ҫ����ָ��
    pli = m_pdb->TextGetLineAt( nLine );
    pStart = pli->pData;
    pEnd = pStart + nEnd;
    pCur = pStart + nResumePosCur;
    bModified = true;
  }
  return bModified;
}
BOOL CPfxEditView::CaseCapitalize( LONG nLine, LONG nStart, LONG nEnd ) {
  if ( !m_pdb->TextIsValidLine( nLine ) ) {
    return false;
  }
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
  nStart = min( nStart, nEnd );
  nEnd = max( nEnd, nStart );
  nStart = min( nStart, pli->nActLen );
  nEnd = min( nEnd, pli->nActLen );
  // ���л��㳤�ȣ�
  if ( PFX_EC_IS_EMPTY_LINE( pli ) || ( nStart == nEnd ) ) {
    return false;
  }
  // ��ס����λ��
  CPoint ptAbs( 0, nLine );
  m_pdb->TextMainLineToAbsLine( ptAbs );
  BOOL bModified = false;
  LPCTSTR pStart = pli->pData;
  LPCTSTR pEnd = pStart + nEnd;
  LPCTSTR pCur = pStart + nStart;
  // ��ʼɨ��
  while ( pCur < pEnd ) {
    LPCTSTR pTmp = pCur;
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
    // ���ҵ��ʵĵ�һ���ַ�
    if ( IsWhitespace( *pTmp ) || IsDelimiter( pTmp, pEnd ) ) {
      continue;
    }
    WORD wCharType[ PFX_EC_MAX_MB_LEN ];
    // ��ʹ���ǿհ��ַ�����֪�ĵ��ʶ����������Ҳ�п�����ĳЩ��
    // ����ţ�����MBCS�ַ��е�ȫ�Ǳ��ȣ���һ���ַ�ֻ������ĸ
    // ������ȫ�ǻ��ǣ����������Ҫ��
    pCur = min( pCur, pEnd );
    LONG cch = pCur - pTmp;
    ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pTmp, cch, wCharType );
    WORD wCharInfo = wCharType[ 0 ];
    if ( ( ( wCharInfo & C1_ALPHA ) == 0 )
         || ( wCharInfo & ( C1_UPPER | C1_LOWER ) ) == 0 ) {
      continue;
    }
    // ��ʱ��������ÿ�������PFX_EC_SIZEOF(sz)���ַ�
    CPoint ptFrom;
    CPoint ptTo;
    TCHAR sz[ 256 ];
    // �����һ���ַ�
    if ( ( wCharInfo & C1_LOWER ) != 0 ) {
      memcpy( sz, pTmp, ( cch * sizeof( TCHAR ) ) );
      ::CharUpperBuff( sz, cch );
      ptFrom.y = nLine;
      ptFrom.x = pTmp - pStart;
      m_pdb->TextMainLineToSubLine( ptFrom );
      ptTo.y = nLine;
      ptTo.x = pCur - pStart;
      m_pdb->TextMainLineToSubLine( ptTo );
      CharsReplace( ptFrom, ptTo, sz, cch, false );
      LONG nResumePosCur = pCur - pStart;
      LONG nResumePosTmp = pTmp - pStart;
      // ���ܸı��˻�����������һ��Ҫ����ָ��
      pli = m_pdb->TextGetLineAt( nLine );
      pStart = pli->pData;
      pEnd = pStart + nEnd;
      pCur = pStart + nResumePosCur;
      pTmp = pStart + nResumePosTmp;
      bModified = true;
    }
    // ���������ַ�
    while ( ( pCur < pEnd )
            && !IsWhitespace( *pCur )
            && !IsDelimiter( pCur, pEnd ) ) {
      pTmp = pCur;
      pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
      ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pTmp, ( min( pCur, pEnd ) - pTmp ), wCharType );
      wCharInfo = wCharType[ 0 ];
      // ����������ĸ���ַ���ͣ����
      if ( ( ( wCharInfo & C1_ALPHA ) == 0 )
           || ( wCharInfo & ( C1_UPPER | C1_LOWER ) ) == 0 ) {
        break;
      }
      // ����Сд�ַ�
      if ( ( wCharInfo & C1_LOWER ) != 0 ) {
        continue;
      }
      bModified = true;
      // ��ͼ���̶ȵش���һƬ��д�ַ����Լ���UNDO��¼��
ScanNextBlock:
      LPCTSTR pMax = min( ( pTmp + PFX_EC_SIZEOF( sz ) ), pEnd );
      while ( ( pCur < pMax )
              && !IsWhitespace( *pCur )
              && !IsDelimiter( pCur, pMax ) ) {
        cch = ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
        ::GetStringTypeEx( LOCALE_USER_DEFAULT, CT_CTYPE1, pCur, min( ( pMax - pCur ), cch ), wCharType );
        wCharInfo = wCharType[ 0 ];
        //  ����������ĸ���ַ���ͣ����
        if ( ( ( wCharInfo & C1_ALPHA ) == 0 )
             || ( wCharInfo & ( C1_UPPER | C1_LOWER ) ) == 0 ) {
          break;
        }
        //  �������Сд��ֹͣ
        if ( ( wCharInfo & C1_LOWER ) != 0 ) {
          break;
        }
        pCur += cch;
      }
      pCur = min( pCur, pEnd );
      if ( pCur == pTmp ) {
        continue;
      }
      cch = pCur - pTmp;
      memcpy( sz, pTmp, ( cch * sizeof( TCHAR ) ) );
      ::CharLowerBuff( sz, cch );
      ptFrom.y = nLine;
      ptFrom.x = pTmp - pStart;
      m_pdb->TextMainLineToSubLine( ptFrom );
      ptTo.y = nLine;
      ptTo.x = pCur - pStart;
      m_pdb->TextMainLineToSubLine( ptTo );
      CharsReplace( ptFrom, ptTo, sz, cch, false );
      LONG nResumePosCur = pCur - pStart;
      LONG nResumePosTmp = pTmp - pStart;
      // �޸��Ժ����λ�ÿ��ܻ�ı䣬���Ҫʹ�þ���λ����
      // ӳ�����λ��
      CPoint pt( 0, ptAbs.y );
      m_pdb->TextAbsLineToMainLine( pt );
      nLine = pt.y;
      // ���ܸı��˻�����������һ��Ҫ����ָ��
      pli = m_pdb->TextGetLineAt( nLine );
      pStart = pli->pData;
      pEnd = pStart + nEnd;
      pCur = pStart + nResumePosCur;
      pMax = pStart + nResumePosTmp + PFX_EC_SIZEOF( sz );
      if ( ( pCur >= pMax ) && ( pCur < pEnd ) ) {
        pTmp = pCur;
        goto ScanNextBlock;
      }
    }
  }
  return bModified;
}
// BOOL EditCaseChange(BOOL bLower)
// ��ָ����Χ�ڵ��ַ�ת����Сд���д
// BOOL bLower - ת����Сд���д
// ���ִ����ת������TRUE�����򷵻�FALSE
// �����ǰ��ѡ����ת������ѡ��Χ�ڣ�����ִ��ȫ��ת��
BOOL CPfxEditView::EditCaseChange( BOOL bLower ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewUndoManager um( this, ( bLower ? PFX_EC_UNDO_CASE_LOWER : PFX_EC_UNDO_CASE_UPPER ) );
  // ���ڸı��Сд������ı�����������˲���Ҫ���浱ǰ�༭λ��
  BOOL bModified = false;
  BOOL bSelected = IsTextSelected();
  BOOL bColSelected = IsColumnSelect();
  const LONG nSelLineFrom = m_nSelLineFrom;
  const LONG nSelLineTo = m_nSelLineTo;
  // ����ȡ��ѡ��
  if ( bSelected ) {
    SelNone();
  }
  if ( !bSelected ) {
    for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
      const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseChange( nLine, 0, pli->nActLen, bLower ) ) {
        bModified = true;
      }
    }
  } else if ( bColSelected )           //      �п�ѡ��ģʽ�²��ᴦ���Զ�����״̬
  {
    for ( LONG nLine = nSelLineFrom; nLine <= nSelLineTo; nLine++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseChange( nLine, pli->nSelFrom, pli->nSelTo, bLower ) ) {
        bModified = true;
      }
    }
  }
  else // ����ѡ��ģʽ
  {
    // ��ȡѡ��������ʼ����ֹ����
    const CPfxEditLineInfo* pliFrom = m_pdb->TextGetLineAt( nSelLineFrom );
    CPoint ptFrom( min( pliFrom->nSelFrom, pliFrom->nLen ), nSelLineFrom );
    const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( nSelLineTo );
    CPoint ptTo( min( pliTo->nSelTo, pliTo->nLen ), nSelLineTo );
    // ת��������λ��
    m_pdb->TextSubLineToAbsLine( ptFrom );
    m_pdb->TextSubLineToAbsLine( ptTo );
    for ( LONG nPosStart = ptFrom.x; ptFrom.y <= ptTo.y; ptFrom.y++ ) {
      // ������λ��ת�������λ��
      CPoint pt( 0, ptFrom.y );
      m_pdb->TextAbsLineToMainLine( pt );
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
      if ( CaseChange( pt.y, nPosStart, ( ( ptFrom.y < ptTo.y ) ? pli->nActLen : ptTo.x ), bLower ) ) {
        bModified = true;
      }
      nPosStart = 0;
    }
  }
  if ( bModified ) {
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return bModified;
}
// ��ѡ��Χ�ڵ��ַ���ת��Сд
BOOL CPfxEditView::EditCaseInvert() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_CASE_INVERT );
  // ���ڸı��Сд������ı�����������˲���Ҫ���浱ǰ�༭λ��
  BOOL bModified = false;
  BOOL bSelected = IsTextSelected();
  BOOL bColSelected = IsColumnSelect();
  const LONG nSelLineFrom = m_nSelLineFrom;
  const LONG nSelLineTo = m_nSelLineTo;
  // ����ȡ��ѡ��
  if ( bSelected ) {
    SelNone();
  }
  if ( !bSelected ) {
    for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
      const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseInvert( nLine, 0, pli->nActLen ) ) {
        bModified = true;
      }
    }
  } else if ( bColSelected )           //      �п�ѡ��ģʽ�²��ᴦ���Զ�����״̬
  {
    for ( LONG nLine = nSelLineFrom; nLine <= nSelLineTo; nLine++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseInvert( nLine, pli->nSelFrom, pli->nSelTo ) ) {
        bModified = true;
      }
    }
  }
  else // ����ѡ��ģʽ
  {
    // ��ȡѡ��������ʼ����ֹ����
    const CPfxEditLineInfo* pliFrom = m_pdb->TextGetLineAt( nSelLineFrom );
    CPoint ptFrom( ( min( pliFrom->nSelFrom, pliFrom->nLen ) ), nSelLineFrom );
    const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( nSelLineTo );
    CPoint ptTo( min( pliTo->nSelTo, pliTo->nLen ), nSelLineTo );
    // ת��������λ��
    m_pdb->TextSubLineToAbsLine( ptFrom );
    m_pdb->TextSubLineToAbsLine( ptTo );
    for ( LONG nPosStart = ptFrom.x; ptFrom.y <= ptTo.y; ptFrom.y++ ) {
      // ������λ��ת���������λ��
      CPoint pt( 0, ptFrom.y );
      m_pdb->TextAbsLineToMainLine( pt );
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
      if ( CaseInvert( pt.y, nPosStart, ( ( ptFrom.y < ptTo.y ) ? pli->nActLen : ptTo.x ) ) ) {
        bModified = true;
      }
      nPosStart = 0;
    }
  }
  if ( bModified ) {
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return bModified;
}
// ��ѡ��Χ�ڵľ�������ĸ��д
BOOL CPfxEditView::EditCaseCapitalize() {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewUndoManager um( this, PFX_EC_UNDO_CASE_CAPITALIZE );
  // ���ڸı��Сд������ı�����������˲���Ҫ���浱ǰ�༭λ��
  BOOL bModified = false;
  BOOL bSelected = IsTextSelected();
  BOOL bColSelected = IsColumnSelect();
  const LONG nSelLineFrom = m_nSelLineFrom;
  const LONG nSelLineTo = m_nSelLineTo;
  // ����ȡ��ѡ��
  if ( bSelected ) {
    SelNone();
  }
  if ( !bSelected ) {
    for ( LONG nLine = 0; nLine != PFX_EC_INVALID_LINE; nLine = m_pdb->TextGetNextMainLine( nLine ) ) {
      const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseCapitalize( nLine, 0, pli->nActLen ) ) {
        bModified = true;
      }
    }
  } else if ( bColSelected )           //      �п�ѡ��ģʽ�²��ᴦ���Զ�����״̬
  {
    for ( LONG nLine = nSelLineFrom; nLine <= nSelLineTo; nLine++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( nLine );
      if ( CaseCapitalize( nLine, pli->nSelFrom, pli->nSelTo ) ) {
        bModified = true;
      }
    }
  }
  else // ����ѡ��ģʽ
  {
    // ��ȡѡ��������ʼ����ֹ����
    const CPfxEditLineInfo* pliFrom = m_pdb->TextGetLineAt( nSelLineFrom );
    CPoint ptFrom( min( pliFrom->nSelFrom, pliFrom->nLen ), nSelLineFrom );
    const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( nSelLineTo );
    CPoint ptTo( min( pliTo->nSelTo, pliTo->nLen ), nSelLineTo );
    // ת��������λ��
    m_pdb->TextSubLineToAbsLine( ptFrom );
    m_pdb->TextSubLineToAbsLine( ptTo );
    for ( LONG nPosStart = ptFrom.x; ptFrom.y <= ptTo.y; ptFrom.y++ ) {
      // ������λ��ת���������λ��
      CPoint pt( 0, ptFrom.y );
      m_pdb->TextAbsLineToMainLine( pt );
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
      if ( CaseCapitalize( pt.y, nPosStart, ( ( ptFrom.y < ptTo.y ) ? pli->nActLen : ptTo.x ) ) ) {
        bModified = true;
      }
      nPosStart = 0;
    }
  }
  if ( bModified ) {
    VerifyCurrentPosition();
    UpdateAllViews( PFX_EC_UT_VERT, 0, -1 );
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return bModified;
}
// BOOL EditFind(CPoint& rptFrom,CPoint& rptTo,LPCTSTR pszFindWhat,LONG cchFindWhat,BOOL bMatchCase,BOOL bMatchWholeWord,BOOL bRegExp,BOOL bDirectDown,BOOL bMarkAll,BOOL bWrapAtEndOfFile)
// ִ�в��Ҳ���
// CPoint& rptFrom       - [IN/OUT]��Χ���
// CPoint& rptTo         - [OUT]��Χ�յ�
// LPCTSTR pszFindWhat   - ���Ҷ��󣬳�����cchFindWhat����
// LONG cchFindWhat      - ���Ҷ���ĳ��ȣ��������-1��ʾ�ɺ����Զ����㳤�ȣ���ʱӦ��NULL��β
// BOOL bMatchCase       - �Ƿ�ƥ���Сд
// BOOL bMatchWholeWord  - �Ƿ񵥴�ƥ��
// BOOL bRegExp          - �Ƿ�ʹ��������ʽ����ʱ���Ե���ƥ�䣩
// BOOL bDirectDown      - �Ƿ����²���
// BOOL bMarkAll         - �Ƿ�����ǩ����ҵ��Ķ���������
// BOOL bWrapAtEndOfFile - �Ƿ���ݲ��ҷ�������������յ㲢���ļ���ͷ�ۻ�
// �ҵ�����TRUE�����򷵻�FALSE
BOOL CPfxEditView::EditFind( CPoint& rptFrom, CPoint& rptTo, LPCTSTR pszFindWhat, LONG cchFindWhat, BOOL bMatchCase, BOOL bMatchWholeWord, BOOL bRegExp, BOOL bDirectDown, BOOL bMarkAll, BOOL bWrapAtEndOfFile ) {
  CPfxEditViewFindReplace FindReplace;
  FindReplace.bMatchCase = bMatchCase;
  FindReplace.bMatchWholeWord = bMatchWholeWord;
  FindReplace.bRegExp = bRegExp;
  FindReplace.bDirectDown = bDirectDown;
  FindReplace.bWrapAtEndOfFile = bWrapAtEndOfFile;
  FindReplace.bMarkAll = bMarkAll;
  FindReplace.pszFindWhat = pszFindWhat;
  FindReplace.cchFindWhat = cchFindWhat;
  FindReplace.ptFrom = rptFrom;
  FindReplace.ptTo = rptTo;
  if ( !SearchPreFind( FindReplace ) ) {
    return false;
  }
  if ( FindReplace.bMarkAll ) {
    return SearchFindMarkAll( FindReplace );
  }
  CPoint ptTo;
  CPoint ptFrom;
  if ( FindReplace.bWrapAtEndOfFile ) {
    ptTo = FindReplace.ptFrom;
    if ( FindReplace.bDirectDown ) {
      ptFrom.y = 0;
      ptFrom.x = 0;
    } else {
      ptFrom.y = m_pdb->TextGetLineUpperBound();
      ptFrom.x = m_pdb->TextGetLineLength( ptFrom.y );
    }
    m_pdb->TextSubLineToMainLine( ptFrom );
  }
  BOOL bFound = SearchDoFind( FindReplace );
  if ( !bFound && FindReplace.bWrapAtEndOfFile ) {
    m_pdb->EventSendMsg( PFX_EN_SEARCH_PASSED_EOF, ( FindReplace.bDirectDown ? EOF : ~EOF ) );
    FindReplace.ptFrom = ptFrom;
    FindReplace.ptTo = ptTo;
    bFound = SearchDoFind( FindReplace );
  }
  if ( bFound ) {
    rptFrom = FindReplace.ptFrom;
    m_pdb->TextMainLineToAbsLine( rptFrom );
    rptTo = FindReplace.ptTo;
    m_pdb->TextMainLineToAbsLine( rptTo );
  }
  return bFound;
}
// ִ���滻����
// CPoint& rptFrom,        - [IN/OUT]��Χ��㣬ֻ�е�bIgnoreFindWhatΪ
//                        TRUEʱ����Ч����������ȫ���滻����ѡ��Χ
//                        ���滻
// CPoint& rptTo,          - [IN/OUT]��Χ�յ㣬ֻ�е�bIgnoreFindWhatΪ
//                        TRUEʱ����Ч����������ȫ���滻����ѡ��Χ
//                        ���滻
// LPCTSTR pszFindWhat,    - ���Ҷ��󣬳�����cchFindWhat����
// LONG cchFindWhat,       - ���Ҷ���ĳ��ȣ��������-1��ʾ�ɺ����Զ���
//                        �㳤�ȣ���ʱӦ��NULL��β
// LPCTSTR pszReplaceWith, - �滻���󣬳�����cchReplaceWith����
// LONG cchReplaceWith,    - �滻����ĳ��ȣ��������-1��ʾ�ɺ����Զ���
//                        �㳤�ȣ���ʱӦ��NULL��β
// BOOL bMatchCase,        - �Ƿ�ƥ���Сд
// BOOL bMatchWholeWord,   - �Ƿ񵥴�ƥ��
// BOOL bRegExp,           - �Ƿ�ʹ��������ʽ����ʱ���Ե���ƥ�䣩
// BOOL bIgnoreFindWhat,   - �Ƿ���Բ��Ҷ������ָ����Χ�ڵ��ı�ֱ����
//                        �滻����ȡ������ʱ�����յ������Ч������
//                        ���������յ�
// BOOL bInSelection       - �Ƿ�ֻ��ѡ��Χ�ڽ����滻�����������յ㣬
//                        ���û��ѡ��������ʧ��
// �ɹ��滻����TRUE�����򷵻�FALSE
BOOL CPfxEditView::EditReplace( CPoint& rptFrom, CPoint& rptTo, LPCTSTR pszFindWhat, LONG cchFindWhat, LPCTSTR pszReplaceWith, LONG cchReplaceWith, BOOL bMatchCase, BOOL bMatchWholeWord, BOOL bRegExp, BOOL bIgnoreFindWhat, BOOL bInSelection ) {
  if ( IsReadOnly() ) {
    m_pdb->EventSendMsg( PFX_EN_READ_ONLY_DOCUMENT, 0 );
    return false;
  }
  CPfxEditViewFindReplace FindReplace;
  FindReplace.bMatchCase = bMatchCase;
  FindReplace.bMatchWholeWord = bMatchWholeWord;
  FindReplace.bRegExp = bRegExp;
  FindReplace.bInSelection = bInSelection;
  FindReplace.bIgnoreFindWhat = bIgnoreFindWhat;
  FindReplace.pszFindWhat = pszFindWhat;
  FindReplace.cchFindWhat = cchFindWhat;
  FindReplace.pszReplaceWith = pszReplaceWith;
  FindReplace.cchReplaceWith = cchReplaceWith;
  FindReplace.ptFrom = rptFrom;
  FindReplace.ptTo = rptTo;
  if ( !SearchPreReplace( FindReplace ) ) {
    return false;
  }
  CPfxEditViewUndoManager um( this, ( FindReplace.bIgnoreFindWhat ? PFX_EC_UNDO_REPLACE : PFX_EC_UNDO_REPLACE_ALL ) );
  if ( IsTextSelected() ) {
    SelNone();
  }
  BOOL bFound;
  if ( FindReplace.bIgnoreFindWhat )           //      �����Ѿ���ת�����������λ��
  {
    rptFrom = FindReplace.ptFrom;
    CharsReplace( FindReplace.ptFrom, FindReplace.ptTo, FindReplace.pszReplaceWith, FindReplace.cchReplaceWith, true );
    rptTo = FindReplace.ptFrom;
    m_pdb->TextSubLineToAbsLine( rptFrom );
    m_pdb->TextSubLineToAbsLine( rptTo );
    bFound = true;
  } else {
    CWaitCursor waitcursor;
    bFound = SearchDoReplace( FindReplace );
    if ( bFound ) {
      UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
    }
    EnsureVisibleAndSetAsCurrentPosition( m_ptEditPos );
  }
  return bFound;
}
BOOL CPfxEditView::EditMatchBrace() {
  BOOL bMatch = false;
  CPfxEditViewBraceHint hint;
  if ( MatchBraceGetHint( hint ) ) {
    CPoint pt;
    if ( !hint.m_bDirectDown ) {
      bMatch = MatchBraceSearchUp( hint, pt );
    } else {
      bMatch = MatchBraceSearchDown( hint, pt );
    }
    if ( bMatch ) {
      m_pdb->TextMainLineToAbsLine( pt );
      SetCurrentPosition( pt.y, pt.x );
    }
  }
  return bMatch;
}
// ��ȡ��ǰλ�������ַ�����ֵ
// ���λ�õ�ǰָ���з����ĵ�Ϊ�գ�����-1
LONG CPfxEditView::GetCurrentChar() const {
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  ASSERT( m_pdb->TextIsValidChar( m_ptEditPos.y, m_ptEditPos.x ) );
  CPoint ptMain = m_ptEditPos;
  m_pdb->TextSubLineToMainLine( ptMain ); // �������λ��ת��Ϊ�������λ��
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( ptMain.y );
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nActLen;
  LPCTSTR pCur = pStart + ptMain.x;
  // ���л�ָ����ĩ
  if ( PFX_EC_IS_EMPTY_LINE( pli ) || ( pCur >= pEnd ) ) {
    return -1;
  }
#ifdef _UNICODE
  return static_cast<LONG>( *pCur );
#else
  BYTE cl;
  BYTE ch;
  if ( !PFX_EC_IS_LEAD_BYTE( *pCur ) || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) {
    cl = *pCur;
    ch = 0;
  } else {
    cl = *pCur++;
    ch = *pCur;
  }
  return static_cast<LONG>( MAKEWORD( cl, ch ) );
#endif
}
// BOOL bLogicalPos - TRUE�����뵱ǰ�༭λ�ö�Ӧ���߼�λ�ã���ʱ��Ҫ����
//                 ���û���ӳ��ǰ�ı༭����
//                 ����FALSEʱ���ص�ǰ�Ķ�дλ�ã����Ǿ���λ��
CPoint CPfxEditView::GetCurrentPosition( BOOL bLogicalPos ) const {
  ASSERT_VALID( this );
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  CPoint pt( 0, 0 );
  if ( !bLogicalPos ) {
    pt = m_ptEditPos;
    m_pdb->TextSubLineToAbsLine( pt );
  } else {
    if ( !IsWrapping() ) {
      pt.y = m_ptEditPos.y;
    } else {
      pt.y = m_pdb->TextGetThisMainLine( m_ptEditPos.y );
      m_pdb->TextMainLineToAbsLine( pt );
    }
    pt.x = m_ptLogiPos.x;
  }
  return pt;
}
void CPfxEditView::BookmarkToggle( BOOL bOn, LONG nLine, BOOL bUpdate ) {
  ASSERT_VALID( this );
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  CPoint pt( 0, nLine );
  if ( pt.y == -1 ) {
    pt.y = m_pdb->TextGetThisMainLine( m_ptEditPos.y );
  } else {
    if ( !m_pdb->IsValidLine( pt.y ) ) {
      return ;
    }
    m_pdb->TextAbsLineToMainLine( pt );
  }
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  DWORD dwFlags = pli->dwFlags;
  if ( bOn ) {
    dwFlags |= PFX_EC_LF_BOOKMARK;
  } else {
    dwFlags &= ~PFX_EC_LF_BOOKMARK;
  }
  if ( dwFlags != pli->dwFlags ) {
    m_pdb->m_nBookmarks += ( bOn ? + 1 : -1 );
    ASSERT( m_pdb->m_nBookmarks >= 0 );
    pli->dwFlags = dwFlags;
    if ( bUpdate ) {
      UpdateAllViews( PFX_EC_UT_LINES, nLine, nLine );
    }
  }
}
// ������һ��ǩ������
// LONG nLine - �����е���һ�п�ʼ����
// Ҫ������һ�п��Դ���nLine����-1
LONG CPfxEditView::BookmarkGetNext( LONG nLine, BOOL bWrap ) {
  ASSERT_VALID( this );
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  if ( ( BookmarkGetCount() == 0 )
       || ( ( nLine != -1 ) && !m_pdb->IsValidLine( nLine ) ) ) {
    return PFX_EC_INVALID_LINE;
  }
  CPoint pt( 0, nLine );
  if ( pt.y != -1 ) {
    m_pdb->TextAbsLineToMainLine( pt );
  }
  LONG nLineFound = PFX_EC_INVALID_LINE;
  {
    LONG nLineCount = m_pdb->TextGetLineCount();
    for ( LONG i = pt.y + 1; i < nLineCount; i++ ) {
      if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
        nLineFound = i;
        break;
      }
    }
  }
  if ( ( nLineFound == PFX_EC_INVALID_LINE ) && bWrap ) {
    m_pdb->EventSendMsg( PFX_EN_SEARCH_PASSED_EOF, EOF );
    for ( LONG i = 0; i <= pt.y; i++ ) {
      if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
        nLineFound = i;
        break;
      }
    }
  }
  if ( nLineFound != PFX_EC_INVALID_LINE ) {
    CPoint ptRet( 0, nLineFound );
    m_pdb->TextMainLineToAbsLine( ptRet );
    nLineFound = ptRet.y;
  }
  return nLineFound;
}
// LONG nLine - �����е���һ�п�ʼ����
// Ҫ��������п��Դ���nLine����GetLineCount()
LONG CPfxEditView::BookmarkGetPrev( LONG nLine, BOOL bWrap ) {
  ASSERT_VALID( this );
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  if ( ( BookmarkGetCount() == 0 )
       || ( ( nLine != GetLineCount() ) && !m_pdb->IsValidLine( nLine ) ) ) {
    return PFX_EC_INVALID_LINE;
  }
  CPoint pt( 0, nLine );
  if ( pt.y != GetLineCount() ) {
    m_pdb->TextAbsLineToMainLine( pt );
  }
  LONG nLineFound = PFX_EC_INVALID_LINE;
  {
    for ( LONG i = pt.y - 1; i >= 0; i-- ) {
      if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
        nLineFound = i;
        break;
      }
    }
  }
  if ( ( nLineFound == PFX_EC_INVALID_LINE ) && bWrap ) {
    m_pdb->EventSendMsg( PFX_EN_SEARCH_PASSED_EOF, ~EOF );
    for ( LONG i = m_pdb->TextGetLineUpperBound(); i >= pt.y; i-- ) {
      if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
        nLineFound = i;
        break;
      }
    }
  }
  if ( nLineFound != PFX_EC_INVALID_LINE ) {
    CPoint ptRet( 0, nLineFound );
    m_pdb->TextMainLineToAbsLine( ptRet );
    nLineFound = ptRet.y;
  }
  return nLineFound;
}
void CPfxEditView::BookmarkClearAll() {
  ASSERT_VALID( this );
  if ( BookmarkGetCount() == 0 ) {
    return ;
  }
  for ( LONG i = 0, nLineCount = m_pdb->TextGetLineCount(); i < nLineCount; i++ ) {
    if ( PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( i ) ) ) {
      m_pdb->TextGetLineAt( i ) ->dwFlags &= ~PFX_EC_LF_BOOKMARK;
    }
  }
  m_pdb->m_nBookmarks = 0;
  UpdateAllViews( PFX_EC_UT_LINES, 0, -1 );
}
BOOL CPfxEditView::IsDrawBookmark( LONG nLine ) const {
  ASSERT( m_pdb->TextGetLineCount() > 0 );
  if ( ( BookmarkGetCount() == 0 )
       || ( ( nLine != -1 ) && !m_pdb->IsValidLine( nLine ) ) ) {
    return false;
  }
  if ( nLine != -1 ) {
    CPoint pt( 0, nLine );
    m_pdb->TextAbsLineToMainLine( pt );
    nLine = pt.y;
  } else {
    nLine = m_pdb->TextGetThisMainLine( m_ptEditPos.y );
  }
  return PFX_EC_IS_DRAW_BOOKMARK( m_pdb->TextGetLineAt( nLine ) );
}
// BOOL SetViewRuler(BOOL bOn)
// ������ʾ�����ر��
// BOOL bOn - �Ƿ���ʾ���
void CPfxEditView::SetViewRuler( BOOL bOn ) {
  if ( ( bOn && !m_pdb->m_bDrawRuler ) || ( !bOn && m_pdb->m_bDrawRuler ) ) {
    m_pdb->m_bDrawRuler = bOn;
    UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
  }
}
void CPfxEditView::SetViewLineNumber( BOOL bOn ) {
  if ( ( bOn && !m_pdb->m_bDrawLineNumber )
       || ( !bOn && m_pdb->m_bDrawLineNumber ) ) {
    m_pdb->m_bDrawLineNumber = bOn;
    UpdateAllViews( PFX_EC_UT_LAYOUT, 0, 0 );
    if ( !SetViewWrapping( GetWrapStyle(), GetWrapWidth() ) ) {
      UpdateCurrentPosition( m_ptTextPos.x, false, false );
    }
  }
}
void CPfxEditView::SetViewLineBreak( BOOL bOn ) {
  if ( ( bOn && !m_pdb->m_bDrawLineBreak )
       || ( !bOn && m_pdb->m_bDrawLineBreak ) ) {
    m_pdb->m_bDrawLineBreak = bOn;
    UpdateAllViews( PFX_EC_UT_LINES, 0, -1 );
  }
}
void CPfxEditView::SetViewWhitespace( BOOL bOn ) {
  if ( ( bOn && !m_pdb->m_bDrawWhitespace )
       || ( !bOn && m_pdb->m_bDrawWhitespace ) ) {
    m_pdb->m_bDrawWhitespace = bOn;
    UpdateAllViews( PFX_EC_UT_LINES, 0, -1 );
  }
}
// ���ִ���˻��в�����ˢ���������ֵ���ͼ������TRUE�����򷵻�FALSE
BOOL CPfxEditView::SetViewWrapping( UINT nStyle, LONG nWidth ) {
  SetDelayRecalcWrapping( false );
  // ����Ƿ���Ҫִ������������������Ҫ��ֱ�ӷ���FALSE
  switch ( nStyle ) {
  default:
  case PFX_EC_WRAP_NONE:
    if ( !IsWrapping() ) {
      return false;
    }
    nWidth = 0;
    break;
  case PFX_EC_WRAP_COLUMNS:
    if ( ( nWidth <= 0 )
         || ( IsWrapAfterColumns()
              && ( GetWrapWidth() == nWidth ) ) ) {
      return false;
    }
    break;
  case PFX_EC_WRAP_WINDOWEDGE:
    if ( IsWrapAtWindowEdge()
         && ( GetWrapWidth() == m_rectText.Width() ) ) {
      return false;
    }
    nWidth = m_rectText.Width();
    break;
  }
  // ���������ͼ��ǰ��״̬�Ա���ִ�в�����ָ���Щ����
  struct CState {
    BOOL bSelected;
    CPoint ptSel;
    CPoint ptCurrent;
    CPfxEditView* pView;
  };
  CState state[ PFX_EC_SIZEOF( m_pdb->m_arrViews ) ];
  for ( LONG i = 0; i < PFX_EC_SIZEOF( state ); i++ ) {
    CState& rState = state[ i ];
    CPfxEditView* pView = m_pdb->m_arrViews[ i ];
    rState.pView = pView;
    if ( pView == NULL ) {
      continue;
    }
    rState.ptSel = pView->SelGetParam();
    rState.bSelected = pView->IsTextSelected();
    rState.ptCurrent = pView->m_ptEditPos;
    m_pdb->TextSubLineToAbsLine( rState.ptSel );
    m_pdb->TextSubLineToAbsLine( rState.ptCurrent );
  }
  // ִ����Ҫ��Ĳ���
  switch ( nStyle ) {
  case PFX_EC_WRAP_WINDOWEDGE:
    WrapAtUnits( nWidth );
    break;
  case PFX_EC_WRAP_COLUMNS:
    WrapColumns( nWidth );
    break;
  case PFX_EC_WRAP_NONE:
  default:
    WrapRestore();
    break;
  }
  // ���ڻָ���������
  for ( i = 0; i < PFX_EC_SIZEOF( state ); i++ ) {
    CState& rState = state[ i ];
    CPfxEditView* pView = rState.pView;
    if ( pView == NULL ) {
      continue;
    }
    pView->SetCurrentPosition( rState.ptCurrent.y, rState.ptCurrent.x );
    if ( rState.bSelected ) {
      m_pdb->TextAbsLineToSubLine( rState.ptSel );
      pView->SelRange( rState.ptSel );
    }
  }
  UpdateAllViews( PFX_EC_UT_ALL, 0, -1 );
  return true;
}
// ���õ�ǰλ��
// LONG (nLine, nChar) - ����Ϊ��ֵ���У���λ��
void CPfxEditView::SetCurrentPosition( LONG nLine, LONG nChar ) {
  // ����
  // 1) ���±༭��ǰλ��
  // 2) ���²����λ��
  // 3) ���¹���������λ��
  // 4) �жϵ�ǰ������Ƿ�ɼ�
  // ����������Ա��κ���ʹ�ã���˴���Ĳ�����ɿ�������Ҫ�ϸ���
  // ��������Ч��
  // EnsureVisible()���ڲ�ʹ�õĺ�������ֻ��ȷ��ָ��λ���ڿɼ���Χ�ڣ�
  // ��SetCurrentPosition������ָ��λ�÷�����ͼ���м�
  CPoint cpOld = m_ptEditPos;
  CPoint cpNew( nChar, nLine );
  m_pdb->TextAbsLineToSubLine( cpNew );
  LONG xTextPos = TransCharPosToTextPos( cpNew );
  m_ptEditPos = cpNew;
  if ( IsTextSelected() && ( cpNew != cpOld ) ) {
    SelNone();
  }
  BOOL bNeedUpdate = false;
  // ָ�����Ƿ�ɼ���������ɼ���Ҫ����ŵ��ɼ���Χ
  // m_rectText.top+(m_sizeVisible.cy*PFX_EC_DATA.m_nLineHeight)
  // ����һ������ m_rectText.bottom
  // ��Ϊ m_sizeVisible.cy==m_rectText.Height()/PFX_EC_DATA.m_nLineHeight
  // ������/ (m_rectText.Height()+PFX_EC_DATA.m_nLineHeight-1)/PFX_EC_DATA.m_nLineHeight
  LONG i;
  if ( ( cpNew.y < m_ptFirstVisible.y )
       || ( cpNew.y >= ( m_ptFirstVisible.y + m_sizeVisible.cy ) ) ) {
    i = ( cpNew.y - m_sizeVisible.cy / 2 );
    m_ptFirstVisible.y = max( 0, i );
    ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
    bNeedUpdate = true;
  }
  // ָ�����Ƿ�ɼ���������ɼ���Ҫ����������ɼ���Χ
  LONG nHorzFirstVisibleUnit = m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve;
  LONG nHorzVisibleUnits = m_rectText.Width();
  if ( ( xTextPos < nHorzFirstVisibleUnit )
       || ( xTextPos >= ( nHorzFirstVisibleUnit + nHorzVisibleUnits ) ) ) {
    i = ( ( xTextPos - nHorzVisibleUnits / 2 ) / PFX_EC_DATA.m_nCharWidthAve );
    m_ptFirstVisible.x = max( 0, i );
    ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
    bNeedUpdate = true;
  }
  // ˢ�½��ᱻ�ӳ�ֱ����һ��WM_PAINT��Ϣ����
  if ( bNeedUpdate ) {
    Invalidate( false );
  } else {
    UpdateLines( cpOld.y, cpNew.y );
  }
  UpdateCurrentPosition( xTextPos, true, true );
}
// LONG nLine    - ����Ϊ��ֵ�������
// LONG xTextPos - ����Ϊ��ֵ���ı�Ӱ��ˮƽ����
// true  - ��ʾָ��λ�ò��ڿɼ���Χ�ڣ������Ѿ�������������ͼ�������߲�
//      ��Ҫ�ٸ��£���������Ӧ�ø��²������λ��
// false - ��ʾָ��λ���ڿɼ���Χ�ڲ��Һ���û��ִ�и�����ͼ������������
//      ����Ҫ������ͼĳ���֣�Ӧ���Լ�����
BOOL CPfxEditView::EnsureVisible( LONG nLine, LONG xTextPos ) {
  ASSERT( m_pdb->TextIsValidLine( nLine ) );
  ASSERT( xTextPos >= 0 );
  // �����û�ʹ�ù����������������ط�ȥ����ĵ����������֣�ʹ��
  // ����ǰ����㡱���ڿ��ӷ�Χ�ڣ����û������������������ĸ��������
  // �޸��ĵ�ʱ��Ҫ������ǰ����㡱�ŵ����ӷ�Χ��
  // ����
  // ���ָ����λ�ÿɼ������κ��£�����ָ��λ�ù������ɼ���Χ�ڣ�
  // ���Ҹ��¹�����λ��
  BOOL bNeedUpdate = false;
  LONG i, nLastVisibleLine = m_ptFirstVisible.y + m_sizeVisible.cy;
  // ���������������������һ��ֻ�в�����ʾ����ǰ�����������еĻ���
  // ��Ҫ���������Ϲ�һ��
  if ( ( m_ptEditPos.y == nLastVisibleLine )
       && ( ( m_rectText.top + ( m_sizeVisible.cy * PFX_EC_DATA.m_nLineHeight ) ) <= m_rectText.bottom ) ) {
    m_ptFirstVisible.y = m_pdb->TextVerifyLine( m_ptFirstVisible.y + 1 );
    ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
    bNeedUpdate = true;
  }
  // ָ�����Ƿ�ɼ���������ɼ���Ҫ����ŵ��ɼ���Χ�����ﲻ��ʹ�á�<=��
  if ( nLine < m_ptFirstVisible.y ) {
    // ���ָ�������׸��ɼ������棬��ָ���н���Ϊ�׸��ɼ���
    m_ptFirstVisible.y = nLine;
    ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
    bNeedUpdate = true;
  }
  // ����һ��Ҫ�á�>=��
  else if ( nLine >= nLastVisibleLine ) {
    // ���ָ���������ɼ������棬��ָ���н���Ϊ���ɼ���
    m_ptFirstVisible.y = m_pdb->TextVerifyLine( nLine - m_sizeVisible.cy + 1 );
    ScrollBarSetPos( SB_VERT, m_ptFirstVisible.y );
    bNeedUpdate = true;
  }
  // ָ���߼���Ԫ�Ƿ�ɼ���������ɼ���Ҫ����������ɼ���Χ
  LONG nHorzFirstVisibleUnit = m_ptFirstVisible.x * PFX_EC_DATA.m_nCharWidthAve;
  LONG nHorzVisibleUnits = m_rectText.Width();
  // ���ﲻ��ʹ�á�<=��
  if ( xTextPos < nHorzFirstVisibleUnit ) {
    // ���ָ��ˮƽλ�����׸��ɼ��߼���֮ǰ���׸��ɼ��߼��е���
    // ��ָ��ˮƽλ�� - ��ǰ�ı������ / 4��/ �߼��п�� �����
    // ������ǻ�����������ˮƽ�ƶ����ı������ / 4���߼���Ԫ
    i = ( xTextPos - nHorzVisibleUnits / 4 ) / PFX_EC_DATA.m_nCharWidthAve;
    m_ptFirstVisible.x = max( 0, i );
#ifdef _SYNC_SCROLLING
    CPfxEditView* pSiblingView = GetSiblingView();
    if ( pSiblingView != NULL ) {
      pSiblingView->m_ptFirstVisible.x = m_ptFirstVisible.x;
      pSiblingView->Invalidate( false );
    }
#endif
    ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
    bNeedUpdate = true;
  }
  // ����һ��Ҫ�á�>=��
  else if ( xTextPos >= ( nHorzFirstVisibleUnit + nHorzVisibleUnits ) ) {
    // ���ָ��ˮƽλ�������ɼ��߼���֮�����׸��ɼ��߼��е���
    // ��ָ��ˮƽλ�� - ��2 * ��ǰ�ı������ / 4����/ �߼��п�
    // �ȣ����������ǻ�����������ˮƽ�ƶ����ı������ / 4����
    // ����Ԫ
    i = ( xTextPos + nHorzVisibleUnits / 4 - nHorzVisibleUnits ) / PFX_EC_DATA.m_nCharWidthAve;
    m_ptFirstVisible.x = max( 0, i );
#ifdef _SYNC_SCROLLING
    CPfxEditView* pSiblingView = GetSiblingView();
    if ( pSiblingView != NULL ) {
      pSiblingView->m_ptFirstVisible.x = m_ptFirstVisible.x;
      pSiblingView->Invalidate( false );
    }
#endif
    ScrollBarSetPos( SB_HORZ, m_ptFirstVisible.x );
    bNeedUpdate = true;
  }
  if ( bNeedUpdate )           //      ˢ�½��ᱻ�ӳ�ֱ����һ��WM_PAINT��Ϣ����
  {
    this->Invalidate( false );
  }
  return bNeedUpdate;
}
void CPfxEditView::EnsureVisibleAndUpdateCurrentPosition() {
  EnsureVisible( m_ptEditPos.y, m_ptTextPos.x );
  UpdateCurrentPosition( m_ptTextPos.x, false, false );
}
void CPfxEditView::EnsureVisibleAndUpdateLines( LONG nLine, LONG xTextPos, LONG nFirstUpdateLine, LONG nLastUpdateLine ) {
  if ( !EnsureVisible( nLine, xTextPos ) ) {
    UpdateLines( nFirstUpdateLine, nLastUpdateLine );
  }
}
void CPfxEditView::EnsureVisibleAndSetAsCurrentPosition( CPoint pt ) {
  if ( m_ptEditPos.y != pt.y )           //      ����ˢ��ԭ���ĵ�ǰ��
  {
    UpdateLines( m_ptEditPos.y, m_ptEditPos.y );
  }
  m_ptEditPos = pt;
  LONG xTextPos = TransCharPosToTextPos( m_ptEditPos );
  EnsureVisible( m_ptEditPos.y, xTextPos );
  UpdateLines( m_ptEditPos.y, m_ptEditPos.y );
  UpdateCurrentPosition( xTextPos, true, true );
}
// LPCTSTR GetPrevCharPos(LPCTSTR pCur, LPCTSTR pStart) const
// ����дָ���Ƶ���һ���ַ����൱��CharPrev()
LPCTSTR CPfxEditView::GetPrevCharPos( LPCTSTR pCur, LPCTSTR pStart ) const {
  if ( pCur > pStart ) {
    LPCTSTR lp = pCur - 1;
    if ( PFX_EC_IS_LEAD_BYTE( *lp ) ) {
      --lp;
      return const_cast<LPTSTR>( max( lp, pStart ) );
    } else {
      while ( ( pStart <= ( --lp ) ) && ( PFX_EC_IS_LEAD_BYTE( *lp ) ) ) {
        ;
      }
      return const_cast<LPTSTR> ( ( pCur - 1 - ( ( pCur - lp ) & 0x01 ) ) );
    }
  }
  return const_cast<LPTSTR>( pStart );
}
// LPCTSTR GetNextCharPos(LPCTSTR pCur, LPCTSTR pEnd) const
// ����дָ���Ƶ���һ���ַ����൱��CharNext()
LPCTSTR CPfxEditView::GetNextCharPos( LPCTSTR pCur, LPCTSTR pEnd ) const {
  if ( pCur < pEnd ) {
    pCur += ( PFX_EC_IS_LEAD_BYTE( *pCur ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return const_cast<LPTSTR>( min( pCur, pEnd ) );
}
LPCTSTR CPfxEditView::GetPrevWordPos( LPCTSTR pCur, LPCTSTR pStart ) const {
  if ( ( pCur == NULL ) || ( pStart == NULL ) ) {
    return NULL;
  }
  // ������ȥ������ǰ��Ŀհ��ַ���PFX_EC_CHR_SPACE��PFX_EC_CHR_TAB��
  LPCTSTR pEnd = pCur;
  pCur = GetPrevNonWhitespacePos( pCur, pStart );
  if ( pCur > pStart ) {
    pCur = GetPrevCharPos( pCur, pStart );
    // �����һ���ǿհ��ַ��Ƕ�����ͼ�����ǰ�ң�ֱ���ҵ��հ���
    // ����Ƕ����������Ϊֹ
    if ( IsDelimiter( pCur, pEnd ) ) {
      while ( pCur > pStart ) {
        pCur = GetPrevCharPos( pCur, pStart );
        if ( IsWhitespace( *pCur )
             || !IsDelimiter( pCur, pEnd ) ) {
          pCur = GetNextCharPos( pCur, pEnd );
          break;
        }
      }
    }
    // �����һ���ǿհ��ַ����Ƕ�����ͼ�����ǰ�ң�ֱ���ҵ��հ�
    // �ַ��򶨽��������Ϊֹ
    else {
      while ( pCur > pStart ) {
        pCur = GetPrevCharPos( pCur, pStart );
        if ( IsWhitespace( *pCur )
             || IsDelimiter( pCur, pEnd ) ) {
          pCur = GetNextCharPos( pCur, pEnd );
          break;
        }
      }
    }
  }
  return pCur;
}
LPCTSTR CPfxEditView::GetNextWordPos( LPCTSTR pCur, LPCTSTR pEnd ) const {
  if ( ( pCur == NULL ) || ( pEnd == NULL ) ) {
    return NULL;
  }
  LPCTSTR pTmp = pCur;
  pCur = GetNextCharPos( pTmp, pEnd );
  // �����ǰλ���ǿհ��ַ���PFX_EC_CHR_SPACE��PFX_EC_CHR_TAB����һֱ���²���ֱ����
  // һ���ǿհ��ַ����µĵ�ǰλ�ý�ָ����ַ��������ǰλ�ò��ǿհ���
  // ���Ͳ��ҵ�һ�����ʶ�������һ���հ��ַ�
  // �����һ���ַ��ǿհ��ַ����ҵ��ǿհ��ַ�Ϊֹ
  if ( IsWhitespace( *pTmp ) ) {
    pCur = GetNextNonWhitespacePos( pCur, pEnd );
  }
  // �����һ���ַ��Ƕ�������ҵ���һ���Ƕ����Ϊֹ
  else if ( IsDelimiter( pTmp, pEnd ) ) {
    while ( pCur < pEnd ) {
      if ( IsWhitespace( *pCur ) ) {
        pCur = GetNextNonWhitespacePos( pCur, pEnd );
        break;
      }
      if ( !IsDelimiter( pCur, pEnd ) ) {
        break;
      }
      pCur = GetNextCharPos( pCur, pEnd );
    }
  }
  // ���������һ���ַ��ǷǶ�������ҵ�һ�������Ϊֹ
  else {
    while ( pCur < pEnd ) {
      if ( IsWhitespace( *pCur ) ) {
        pCur = GetNextNonWhitespacePos( pCur, pEnd );
        break;
      }
      if ( IsDelimiter( pCur, pEnd ) ) {
        break;
      }
      pCur = GetNextCharPos( pCur, pEnd );
    }
  }
  return pCur;
}
LPCTSTR CPfxEditView::GetPrevNonWhitespacePos( LPCTSTR pCur, LPCTSTR pStart ) const {
  ASSERT( pCur >= pStart );
  // ��(pCur == pStart == NULL)ʱ��(--pCur)����(0xFFFFFFFF)
  if ( pCur > pStart ) {
    while ( ( --pCur ) >= pStart ) {
      if ( !IsWhitespace( *pCur ) ) {
        pCur++;
        break;
      }
    }
  }
  return const_cast<LPTSTR>( max( pCur, pStart ) );
}
LPCTSTR CPfxEditView::GetNextNonWhitespacePos( LPCTSTR pCur, LPCTSTR pEnd ) const {
  ASSERT( pCur <= pEnd );
  while ( ( pCur < pEnd ) && IsWhitespace( *pCur ) ) {
    pCur++;
  }
  return const_cast<LPTSTR>( min( pCur, pEnd ) );
}
// ���ص�ָ�벻��ָ�򵥴ʶ����������������һ���ַ�
LPCTSTR CPfxEditView::GetPrevDelimiterPos( LPCTSTR pCur, LPCTSTR pStart ) const {
  ASSERT( pCur >= pStart );
  while ( pCur > pStart ) {
    LPCTSTR pNext = pCur;
    pCur = GetPrevCharPos( pCur, pStart );
    if ( IsWhitespace( *pCur ) || IsDelimiter( pCur, pNext ) ) {
      pCur = pNext;
      break;
    }
  }
  return const_cast<LPTSTR>( max( pCur, pStart ) );
}
// �����ǰ�ַ��ǵ��ʶ�����ͷ��ص�ǰ��ַ
LPCTSTR CPfxEditView::GetNextDelimiterPos( LPCTSTR pCur, LPCTSTR pEnd ) const {
  ASSERT( pCur <= pEnd );
  while ( pCur < pEnd ) {
    if ( IsWhitespace( *pCur ) || IsDelimiter( pCur, pEnd ) ) {
      break;
    }
    pCur = GetNextCharPos( pCur, pEnd );
  }
  return const_cast<LPTSTR>( min( pCur, pEnd ) );
}
LONG CPfxEditView::MoveLeft( CPoint& rpt, BOOL bWord, BOOL bCalcTextPos ) const {
  // ���rpt==m_ptEditPos�Ͳ�̫��ȫ������Ҫʹ�õ�ǰλ�õĸ���
  CPoint cp = m_ptEditPos;
  m_pdb->TextSubLineToMainLine( cp );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( cp.y );
  const LPCTSTR pStart = pli->pData;
  if ( bWord ) {
    cp.x = GetPrevWordPos( ( pStart + cp.x ), pStart ) - pStart;
  } else {
    cp.x = GetPrevCharPos( ( pStart + cp.x ), pStart ) - pStart;
  }
  m_pdb->TextMainLineToSubLine( cp );
  // �����ǰλ���������е����ײ��ҵ�ǰ�в����ĵ��ĵ�һ�У��ͽ���λ��
  // �ƶ�����һ�е���ĩ
  if ( ( cp == m_ptEditPos ) && !m_pdb->TextIsFirstLine( cp.y ) ) {
    cp.y--;
    cp.x = m_pdb->TextGetLineLength( cp.y );
  }
  rpt = cp;
  return ( !bCalcTextPos ? 0 : TransCharPosToTextPos( rpt ) );
}
LONG CPfxEditView::MoveRight( CPoint& rpt, BOOL bWord, BOOL bCalcTextPos ) const {
  // ���rpt==m_ptEditPos�Ͳ�̫��ȫ������Ҫʹ�õ�ǰλ�õĸ���
  CPoint cp = m_ptEditPos;
  m_pdb->TextSubLineToMainLine( cp );
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( cp.y );
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nActLen;
  if ( bWord ) {
    cp.x = GetNextWordPos( ( pStart + cp.x ), pEnd ) - pStart;
  } else {
    cp.x = GetNextCharPos( ( pStart + cp.x ), pEnd ) - pStart;
  }
  m_pdb->TextMainLineToSubLine( cp );
  // �����ǰλ���������е���ĩ���ҵ�ǰ�в����ĵ�������У��ͽ���λ��
  // �ƶ�����һ�е�����
  if ( ( cp == m_ptEditPos ) && !m_pdb->TextIsLastLine( cp.y ) ) {
    cp.y++;
    cp.x = 0;
  }
  rpt = cp;
  return ( !bCalcTextPos ? 0 : TransCharPosToTextPos( rpt ) );
}
// ��ȡ��ǰ��ѡ��Χ
// �����п�ѡ��ģʽ�·��ص�rnFromChar��rnToCharֻ�Ե�һ���������Ч
BOOL CPfxEditView::GetSelRange( CPoint& rptFrom, CPoint& rptTo ) const {
  if ( IsTextSelected() ) {
    const CPfxEditLineInfo * pliFrom = m_pdb->TextGetLineAt( m_nSelLineFrom );
    rptFrom.y = m_nSelLineFrom;
    rptFrom.x = min( pliFrom->nLen, pliFrom->nSelFrom );
    const CPfxEditLineInfo* pliTo = m_pdb->TextGetLineAt( m_nSelLineTo );
    rptTo.y = m_nSelLineTo;
    rptTo.x = min( pliTo->nLen, pliTo->nSelTo );
    m_pdb->TextSubLineToAbsLine( rptFrom );
    m_pdb->TextSubLineToAbsLine( rptTo );
  } else {
    rptFrom.y = rptTo.y = PFX_EC_INVALID_SELECT;
    rptFrom.x = rptTo.x = PFX_EC_INVALID_SELECT;
  }
  return IsTextSelected();
}
// ��ȡ��ǰѡ�����ݵĴ�С�����û��ѡ�������򷵻���
// LONG nLineBreakType - ָ�����з������ͣ�����
//                    DOS��PFX_EC_LINE_BREAK_TYPE_DOS��
//                    UNIX��PFX_EC_LINE_BREAK_TYPE_UNIX��
//                    MAC��PFX_EC_LINE_BREAK_TYPE_MAC
// BOOL bCopyBufSize   - ָ�����п�ѡ��ģʽ���Ƿ񷵻ظ��ƻ������Ĵ�С��
//                    ��������Ӱ������ģʽѡ��ģʽ�µķ���ֵ�������
//                    ����ΪTRUE�򷵻ذ������з��Ļ�������С������
//                    ��ʵ�ʵ�ѡ�����ݵĴ�С
// ������п�ѡ����Ϊ�����û���ӳ��ǰ��ѡ���С��Ӧ�ý�bCopyBufSize����
// ΪFALSE��������������ϻ��з��Ĵ�С����Ϊ�п�ѡ���µ�ѡ�����ݶ�������
// ���з�������ִ�и��Ʋ���ʱ��ǿ����ÿ�еĽ�β���ϻ��з���Ҳ����˵���
// ��ΪGetSelData()ȷ����������С��Ӧ�ý��ò�������ΪTRUE
LONG CPfxEditView::GetSelSize( LONG nLineBreakType, BOOL bCopyBufSize ) const {
  if ( !IsTextSelected() ) {
    return 0;
  }
  // bCopyBufSize����ֻ��Ӱ���п�ѡ��ģʽ
  if ( IsColumnSelect() ) {
    if ( !bCopyBufSize ) {
      return m_nSelSize;
    }
  } else {
    if ( GetLineBreakType() == nLineBreakType ) {
      return m_nSelSize;
    }
  }
  LONG nSelSize = 0;
  LONG nLineBreakSize = m_pdb->GetLineBreakSizeFromType( nLineBreakType );
  if ( IsColumnSelect() )           //      �п�ѡ���µ�ѡ���С
  {
    for ( LONG i = m_nSelLineFrom; i <= m_nSelLineTo; i++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      // �п�ѡ��û�а������з�����ʹ���ÿһ�ж�Ҫ���ϻ�
      // �з�
      nSelSize += ( min( pli->nSelTo, pli->nLen ) - pli->nSelFrom );
      nSelSize += nLineBreakSize;
    }
  }
  else // ����ѡ���µ�ѡ���С
  {
    for ( LONG i = m_nSelLineFrom; i <= m_nSelLineTo; i++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      // ��pli->nSelTo����pli->nLenʱ������һ�е�ѡ�����
      // ���з�
      nSelSize += ( ( pli->nSelTo > pli->nLen ) ? ( pli->nLen - pli->nSelFrom + nLineBreakSize ) : ( pli->nSelTo - pli->nSelFrom ) );
    }
  }
  return nSelSize;
}
// ��ȡ��ǰ��ѡ������
// LONG nLineBreakType - ���з����ͣ�����
//                    PFX_EC_LINE_BREAK_TYPE_DOS,PFX_EC_LINE_BREAK_TYPE_UNIX,PFX_EC_LINE_BREAK_TYPE_MAC��
//                    �������з�����������ָ�����͵Ļ��з���������
// LPTSTR pBuffer      - �������ݵĻ�����ָ��
// LONG nMaxBuf        - �û������ĳ���
// ����ʵ�ʸ��Ƶ��ַ�������������β��NULL�ַ���λ��TCHAR
// ����NULL�ַ������������Կո��ַ�����
LONG CPfxEditView::GetSelData( LONG nLineBreakType, LPTSTR pBuffer, LONG nMaxBuf ) const {
  if ( !IsTextSelected() || ( pBuffer == NULL ) || ( nMaxBuf <= 0 ) ) {
    return 0;
  }
  // �������ɽ�β��NULL�ַ�
  nMaxBuf--;
  LONG nLineBreakSize = m_pdb->GetLineBreakSizeFromType( nLineBreakType );
  LPCTSTR pLineBreakData = m_pdb->GetLineBreakDataFromType( nLineBreakType );
  LONG nSelLineTo = m_nSelLineTo;
  LPTSTR pCur = pBuffer;
  // ��ȡ�п�ѡ��ʱ������
  if ( IsColumnSelect() ) {
    for ( LONG i = m_nSelLineFrom; ( i <= nSelLineTo ) && ( nMaxBuf > 0 ); i++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      LPCTSTR pData = pli->pData + pli->nSelFrom;
      LONG nSelLen = min( pli->nLen, pli->nSelTo ) - pli->nSelFrom;
      if ( nMaxBuf < nSelLen ) {
        nSelLen = CalcCharsToCopy( pData, nMaxBuf );
      }
      // ����������Ѿ��������������б����ַ�������Ҫ����
      // �����ݽضϣ��ضϿ��ܻᷢ����һ��MBCS�ַ����м䣬
      // ����������ǲ�����ģ�����Ҫ��λ���ַ���λ��
      if ( nSelLen > 0 ) {
        memcpy( pCur, pData, ( nSelLen * sizeof( TCHAR ) ) );
        pCur += nSelLen;
        nMaxBuf -= nSelLen;
      }
      // �п�ѡ����������ζ�Ҫ���ƻ��з�
      if ( nMaxBuf >= nLineBreakSize ) {
        memcpy( pCur, pLineBreakData, nLineBreakSize * sizeof( TCHAR ) );
        pCur += nLineBreakSize;
        nMaxBuf -= nLineBreakSize;
      } else {
        break;
      }
    }
  }
  // ��ȡ����ѡ��ʱ������
  else {
    for ( LONG i = m_nSelLineFrom; ( i <= nSelLineTo ) && ( nMaxBuf > 0 ); i++ ) {
      const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( i );
      LPCTSTR pData = pli->pData + pli->nSelFrom;
      LONG nSelLen = min( pli->nLen, pli->nSelTo ) - pli->nSelFrom;
      // ����������Ѿ��������������б����ַ�������Ҫ����
      // �����ݽضϣ��ضϿ��ܻᷢ����һ��MBCS�ַ����м䣬
      // ����������ǲ�����ģ�����Ҫ��λ���ַ���λ��
      if ( nMaxBuf < nSelLen ) {
        nSelLen = CalcCharsToCopy( pData, nMaxBuf );
      }
      if ( nSelLen > 0 ) {
        memcpy( pCur, pData, ( nSelLen * sizeof( TCHAR ) ) );
        pCur += nSelLen;
        nMaxBuf -= nSelLen;
      }
      // ��pli->nSelTo����pli->nLenʱ������һ�е�ѡ�����
      // ���з�����Ҫ���ƻ��з�CRLF
      if ( pli->nSelTo > pli->nLen ) {
        if ( nMaxBuf >= nLineBreakSize ) {
          memcpy( pCur, pLineBreakData, nLineBreakSize * sizeof( TCHAR ) );
          pCur += nLineBreakSize;
          nMaxBuf -= nLineBreakSize;
        } else {
          break;
        }
      }
    }
  }
  // ��Ϊ�ؼ����ļ���ʱ����ԭ�ⲻ���Ľ����ݶ��룬��ʹ��NULL
  // �ַ�Ҳ���Ķ�������ѡ����ı��п�����NULL�ַ�������Ҫ����
  // ��飬��ѡ���ı��е�NULL�ַ��滻Ϊ�ո�
  LONG nCharsCopied = pCur - pBuffer;
  pCur = pBuffer;
  LPTSTR pEnd = pBuffer + nCharsCopied;
  while ( pCur < pEnd ) {
    if ( '\0' == *pCur ) {
      *pCur = PFX_EC_CHR_SPACE;
    }
    pCur++;
  }
  // �����β�ַ�NULL
  *pCur = '\0';
  return nCharsCopied;
}
void CPfxEditView::CharsInsert( CPoint& rptFrom, LPCTSTR pData, LONG cch, BOOL bUpdate ) {
  if ( ( pData == NULL ) || ( cch == 0 ) ) {
    return ;
  }
  CPoint ptOld = rptFrom;
  m_pdb->TextSubLineToMainLine( ptOld );
  m_pdb->TextInsertChars( rptFrom, pData, cch, ( bUpdate ? this : NULL ) );
  m_pdb->UndoAddInsert( ptOld, rptFrom );
  ASSERT( m_pdb->TextIsValidLine( rptFrom.y ) );
  ASSERT( m_pdb->TextIsValidChar( rptFrom.y, rptFrom.x ) );
}
void CPfxEditView::CharsRemove( CPoint& rptFrom, CPoint& rptTo, BOOL bUpdate ) {
  if ( rptFrom == rptTo ) {
    return ;
  }
  if ( rptFrom.y > rptTo.y ) {
    LONG i = rptFrom.y;
    rptFrom.y = rptTo.y;
    rptTo.y = i;
    i = rptFrom.x;
    rptFrom.x = rptTo.x;
    rptTo.x = i;
  }
  m_pdb->UndoAddRemove( rptFrom, rptTo );
  m_pdb->TextRemoveChars( rptFrom, rptTo, ( bUpdate ? this : NULL ) );
  ASSERT( m_pdb->TextIsValidLine( rptFrom.y ) );
  ASSERT( m_pdb->TextIsValidChar( rptFrom.y, rptFrom.x ) );
}
void CPfxEditView::CharsReplace( CPoint& rptFrom, CPoint& rptTo, LPCTSTR pData, LONG cch, BOOL bUpdate ) {
  // �൱��CharsRemove()
  if ( rptFrom != rptTo ) {
    if ( rptFrom.y > rptTo.y ) {
      CPoint pt = rptFrom;
      rptFrom = rptTo;
      rptTo = pt;
    }
    m_pdb->UndoAddRemove( rptFrom, rptTo );
    m_pdb->TextRemoveChars( rptFrom, rptTo, ( bUpdate ? this : NULL ) );
    ASSERT( m_pdb->TextIsValidLine( rptFrom.y ) );
    ASSERT( m_pdb->TextIsValidChar( rptFrom.y, rptFrom.x ) );
  }
  // �൱��CharsInsert()
  if ( ( pData != NULL ) && ( cch > 0 ) ) {
    CPoint ptOld = rptFrom;
    m_pdb->TextSubLineToMainLine( ptOld );
    m_pdb->TextInsertChars( rptFrom, pData, cch, ( bUpdate ? this : NULL ) );
    m_pdb->UndoAddInsert( ptOld, rptFrom );
    ASSERT( m_pdb->TextIsValidLine( rptFrom.y ) );
    ASSERT( m_pdb->TextIsValidChar( rptFrom.y, rptFrom.x ) );
  }
}
BOOL CPfxEditView::IsDelimiter( WCHAR wc ) const {
  ASSERT( m_pti != NULL );
  LPCWSTR lp = m_pti->m_arrDelimiters;
  while ( ( *lp != '\0' ) && ( *lp != wc ) ) {
    ++lp;
  }
  return ( *lp != '\0' );
}
BOOL CPfxEditView::IsDelimiter( LPCTSTR pCur, LPCTSTR pEnd ) const {
  if ( pCur >= pEnd ) {
    return false;
  }
  WCHAR wc = ( ( !PFX_EC_IS_LEAD_BYTE( *pCur )
                 || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? *pCur : *( ( LPCWSTR ) pCur ) );
  ASSERT( m_pti != NULL );
  LPCWSTR lp = m_pti->m_arrDelimiters;
  while ( ( *lp != '\0' ) && ( *lp != wc ) ) {
    ++lp;
  }
  return ( *lp != '\0' );
}
// ���Ը���λ���Ƿ���ѡ��Χ��
// CPoint ptView - ���ԵĴ��ڿͻ�������
BOOL CPfxEditView::IsInsideSelect( CPoint ptView ) const {
  if ( !IsTextSelected() ) {
    return false;
  }
  CPoint ptText;
  ptText.x = TransViewPosToTextPosX( ptView.x );
  ptText.y = TransViewPosToTextPosY( ptView.y );
  // ���п�ѡ��ֻ̽��ѡ�񲿷ֵ�һ�к�����У��м�Ĳ�������ˮƽ����
  // �������ﶼ������
  if ( !IsColumnSelect() ) {
    CPoint pt;
    pt.y = TransTextPosToLinePos( ptText.y );
    pt.x = TransTextPosToCharPos( pt.y, ptText.x );
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
    // ���ֻ��һ�б�ѡ�������Ҫ��ѡ��Χ�ڲ�������
    if ( m_nSelLineFrom == m_nSelLineTo ) {
      return ( ( pt.y == m_nSelLineFrom )
               && ( pt.x >= pli->nSelFrom )
               && ( pt.x <= pli->nSelTo ) );
    }
    // �����м�������������ﶼ������
    if ( ( pt.y > m_nSelLineFrom ) && ( pt.y < m_nSelLineTo ) ) {
      return true;
    }
    // ��һ��Ҫ��ѡ��ĵ�һ��֮ǰ���㲻����
    if ( ( pt.y == m_nSelLineFrom ) && ( pt.x >= pli->nSelFrom ) ) {
      return true;
    }
    // �����Ҫ��ѡ��������֮����㲻����
    if ( ( pt.y == m_nSelLineTo ) && ( pt.x < pli->nSelTo ) ) {
      return true;
    }
  } else // �п�ѡ��ֻ����������ѡ�񲿷ֲ�������
  {
    CPoint pt;
    pt.y = TransTextPosToLinePos( ptText.y );
    pt.x = 0;
    if ( ( pt.y < m_nSelLineFrom ) || ( pt.y > m_nSelLineTo ) ) {
      return false;
    }
    LONG x1 = min( m_rectColSelect.left, m_rectColSelect.right );
    LONG x2 = max( m_rectColSelect.left, m_rectColSelect.right );
    const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
    pt.x = pli->nSelFrom;
    LONG p1 = TransCharPosToTextPos( pt );
    pt.x = pli->nSelTo;
    LONG p2 = TransCharPosToTextPos( pt );
    // ������п�ѡ����ѡ�������Ѿ��Ǳ��������ַ����Լ�ѡ��
    // �����ұ߱��п鷶Χ���ұ�С�������ѡ���������һ���ַ���
    // �п鷶Χ�ұߵļ�϶����ʹ��������һ����
    if ( ( pli->nSelFrom >= pli->nLen ) && ( p1 < x1 ) ) {
      p1 = x1;
    }
    if ( ( pli->nSelTo >= pli->nLen ) && ( p2 < x2 ) ) {
      p2 = x2;
    }
    if ( ( ptText.x >= p1 ) && ( ptText.x <= p2 ) ) {
      return true;
    }
  }
  return false;
}
// �жϸ�����HGLOBAL�ڵ������Ƿ������п�ģʽ���ύ��
// BOOL bDragDrop  - �Ƿ���OLE�ϷŲ���
// HGLOBAL hGlobal - ��GlobalAlloc()������ڴ�ݱ�
BOOL CPfxEditView::IsDataColumnModeRendered( BOOL bDragDrop, HGLOBAL hGlobal ) const {
  if ( hGlobal == NULL ) {
    return false;
  }
  if ( bDragDrop ) {
    return ( hGlobal == PFX_EC_DATA.m_hDataDragDropCopied );
  }
  // ���ְ취����100%�ɿ�
  if ( ( hGlobal == PFX_EC_DATA.m_hDataClipboardCopied )
       && ( PFX_EC_DATA.m_nDataClipboardCopied > 0 ) ) {
    ::GlobalFlags( hGlobal );
    return ( PFX_EC_DATA.m_nDataClipboardCopied == ::GlobalSize( hGlobal ) );
  }
  return false;
}
// ��ӡ...
// void PrintReleaseResource()
// �ͷŴ�ӡʱ���������Դ
void CPfxEditView::PrintReleaseResource() {
  if ( m_ppc != NULL ) {
    delete m_ppc;
    m_ppc = NULL;
  }
}
// ���pc.m_arrKeyClasses[i]����ɫ�Ƿ���ָ����ӡ����pc.m_pdcPrinter)��
// ֧��
// CPfxEditViewPrintContext& pc - �Ѿ���ʼ���õĶ���m_arrKeyClasses
//       �Ѿ���m_pti->m_arrKeyClasses.m_arrClass������
// ĳЩ��Ļ��ɫ���ܲ�������ӡ����֧�֣�����Ҫ���ݴ�ӡ����֧����������
// ���������ĳ����ɫ��ӡ����֧�־���Ĭ�ϵ��ı���ɫ
void CPfxEditView::PrintCheckColors( CPfxEditViewPrintContext& pc ) {
  // �Ȼ�ȡĬ�ϵ��ı���ɫ�������Ļ���õ��ı���ɫ����֧�־�����ص�
  // ��ɫ������
  CDCH pDC = pc.m_pdcPrinter;
  COLORREF clrPaper = pDC.GetNearestColor( RGB( 255, 255, 255 ) );
  COLORREF clrText = pDC.GetNearestColor( m_pti->GetKeyClassColor( kcText ) );
  if ( ( clrText == CLR_INVALID ) || ( clrText == clrPaper ) ) {
    clrText = pDC.GetNearestColor( RGB( 0, 0, 0 ) );
  }
  for ( int i = 0; i < PFX_EC_SIZEOF( pc.m_arrKeyClasses ); i++ ) {
    COLORREF color = clrText;
    if ( !pc.m_arrKeyClasses[ i ].m_bEmpty ) {
      color = pDC.GetNearestColor(
                pc.m_arrKeyClasses[ i ].m_color );
      if ( ( color == CLR_INVALID ) || ( color == clrPaper ) ) {
        color = clrText;
      }
    }
    pc.m_arrKeyClasses[ i ].m_color = color;
  }
}
// ����ҳ�沼��
// HDC hdcPrinter - ��ӡDC
// CPfxEditViewPrintContext& pc - ��ӡ��Ϣ
void CPfxEditView::PrintCalcLayout( HDC hdcPrinter, CPfxEditViewPrintContext& pc ) {
  CDCH pdcPrinter = hdcPrinter;
  CSize size;
  for ( LONG nCharWidthMaxNum = 0, i = '0'; i <= '9'; i++ ) {
    size = pdcPrinter.GetTextExtent( ( LPCTSTR ) & i, 1 );
    nCharWidthMaxNum = max( nCharWidthMaxNum, size.cx );
  }
  i = PFX_EC_CHR_SPACE;
  size = pdcPrinter.GetTextExtent( ( LPCTSTR ) & i, 1 );
  LONG nCharWidthSpace = size.cx;
  i = PFX_EC_CHR_TAB;
  size = pdcPrinter.GetTextExtent( ( LPCTSTR ) & i, 1 );
  LONG nCharWidthTab = size.cx;
  // �к�����λ��
  LONG nLineNumberChars = 0;
  // �к������ȣ�����ӡ�к�ʱΪ��
  LONG nWidthLineNumber = 0;
  // �߿��߿��
  LONG nWidthMarginLine = 0;
  // ˮƽ�ָ��߿��
  LONG nWidthSeparateLine = 0;
  // �������߶�
  LONG nHeightHeader = 0;
  // ��ע���߶�
  LONG nHeightFooter = 0;
  // �����ע����߶�
  LONG nHeightHeaderAndFooterLine = 0;
  // ���ĺͱ���֮��Ĵ�ֱ���룬����ӡ����ʱΪ��
  LONG nSpacingHeaderAndText = 0;
  // ���ĺͽ�ע֮��Ĵ�ֱ���룬����ӡ��עʱΪ��
  LONG nSpacingFooterAndText = 0;
  // ���ĺ��к�֮���ˮƽ���룬����ӡ�к�ʱΪ��
  LONG nSpacingLineNumberAndText = 0;
  // �����к������
  if ( pc.IsPrintLineNumber() ) {
    for ( i = GetLineCount(), nLineNumberChars = 1; i >= 10; nLineNumberChars++ ) {
      i /= 10;
    }
    nWidthLineNumber = nLineNumberChars * nCharWidthMaxNum;
    nSpacingLineNumberAndText = pc.PrinterLogicUnitFromDisplayX( m_pdb->m_rectIndent.left );
  }
  TEXTMETRIC tm;
  pdcPrinter.GetTextMetrics( &tm );
  pc.m_nCharWidthTab = nCharWidthTab;
  pc.m_nCharWidthSpace = nCharWidthSpace;
  pc.m_nCharWidthMax = max( tm.tmMaxCharWidth, ( tm.tmAveCharWidth * 2 ) );
  pc.m_nFontHeight = tm.tmHeight;
  pc.m_nLineHeight = tm.tmHeight + pc.PrinterLogicUnitFromDisplayY( GetSpacingLines() );
  pc.m_nSpacingLines = pc.PrinterLogicUnitFromDisplayY( GetLineTextVertOffset() );
  // �߿��߿��
  LOGPEN logpen;
  if ( pc.IsPrintMargin() ) {
    pc.m_penMargin.GetLogPen( &logpen );
    nWidthMarginLine = logpen.lopnWidth.x;
  }
  pc.m_nWidthMarginLine = nWidthMarginLine;
  // �����ȥҳ�߾��Ŀɴ�ӡ��Χ��������ҳ�߾�
  CSize sizePaper; // ֽ�ųߴ�
  sizePaper.cx = pdcPrinter.GetDeviceCaps( PHYSICALWIDTH );
  sizePaper.cy = pdcPrinter.GetDeviceCaps( PHYSICALHEIGHT );
  pdcPrinter.DPtoLP( &sizePaper );
  CSize sizePrint; // �ɴ�ӡ��Χ
  sizePrint.cx = pdcPrinter.GetDeviceCaps( HORZRES );
  sizePrint.cy = pdcPrinter.GetDeviceCaps( VERTRES );
  pdcPrinter.DPtoLP( &sizePrint );
  CSize sizeOfsLT; // ��ߺ��ϱ���Сҳ�߾�
  sizeOfsLT.cx = pdcPrinter.GetDeviceCaps( PHYSICALOFFSETX );
  sizeOfsLT.cy = pdcPrinter.GetDeviceCaps( PHYSICALOFFSETY );
  pdcPrinter.DPtoLP( &sizeOfsLT );
  CSize sizeOfsRB; // �ұߺ��±���Сҳ�߾�
  sizeOfsRB.cx = sizePaper.cx - sizePrint.cx - sizeOfsLT.cx;
  sizeOfsRB.cy = sizePaper.cy - sizePrint.cy - sizeOfsLT.cy;
  CRect rectMargin( &pc.m_rectMargin );
  rectMargin.left = max( 0, ( rectMargin.left - sizeOfsLT.cx ) );
  rectMargin.top = max( 0, ( rectMargin.top - sizeOfsLT.cy ) );
  rectMargin.right = max( 0, ( rectMargin.right - sizeOfsRB.cx ) );
  rectMargin.bottom = max( 0, ( rectMargin.bottom - sizeOfsRB.cy ) );
  CRect rectDraw( &pc.m_rectDraw );
  rectDraw.DeflateRect( &rectMargin );
  /*
  // �ɴ�ӡ��Χ
  CSize sizeOffset;
  sizeOffset.cx         = pdcPrinter.GetDeviceCaps(PHYSICALOFFSETX);
  sizeOffset.cy         = pdcPrinter.GetDeviceCaps(PHYSICALOFFSETY);
  pdcPrinter.DPtoLP(&sizeOffset);
  CRect rectMargin(&pc.m_rectMargin);
  rectMargin.left       = max(sizeOffset.cx, rectMargin.left);
  rectMargin.top        = max(sizeOffset.cy, rectMargin.top);
  rectMargin.right      = max(sizeOffset.cx, rectMargin.right);
  rectMargin.bottom     = max(sizeOffset.cy, rectMargin.bottom);
  CRect rectDraw(&pc.m_rectDraw);
  rectDraw.DeflateRect(&rectMargin);
  rectDraw.InflateRect(sizeOffset.cx, sizeOffset.cy);
  rectDraw.IntersectRect(&rectDraw, &pc.m_rectDraw);
  */ 
  // ������ɴ�ӡ��Χ
  pc.m_rectDraw.CopyRect( &rectDraw );
  // ����ͽ�ע��ˮƽ�ָ��߸߶�
  if ( pc.IsPrintHeader() || pc.IsPrintFooter() ) {
    if ( pc.PenSeparateLine() != NULL ) {
      pc.m_penSeparateLine.GetLogPen( &logpen );
      nWidthSeparateLine = logpen.lopnWidth.x + pc.PrinterLogicUnitFromDisplayY( PFX_EC_DEF_SPACING_HEADER_FOOTER_TEXT_AND_SEPARATOR );
    }
    if ( pc.FontHeaderAndFooter() == NULL )           //      ʹ����������
    {
      nHeightHeaderAndFooterLine = pc.m_nFontHeight;
    } else {
      CFontH pFontOld = pdcPrinter.SelectObject( pc.GetFontHeaderAndFooter() );
      pdcPrinter.GetTextMetrics( &tm );
      pdcPrinter.SelectObject( pFontOld );
      nHeightHeaderAndFooterLine = tm.tmHeight;
    }
  }
  // ��������������߶�
  if ( pc.IsPrintHeader() ) {
    // ������������֮��Ĵ�ֱ�߶�
    nSpacingHeaderAndText = pc.m_nSpacingHeaderAndText;
    // �������߶ȣ��߶ȵ�����������ÿ�и߶ȼ�ˮƽ�ָ��߿��
    const CString& strHeader = pc.m_strHeader;
    for ( LONG i = 0, nLines = 1; ; nLines++ ) {
      i = strHeader.Find( PFX_EC_PRINT_MACRO_CRLF, i );
      if ( i == -1 ) {
        break;
      }
      i += _tcslen( PFX_EC_PRINT_MACRO_CRLF );
    }
    nHeightHeader = nLines * nHeightHeaderAndFooterLine + nWidthSeparateLine;
  }
  // �����ע�������߶�
  if ( pc.IsPrintFooter() ) {
    // ��ע��������֮��Ĵ�ֱ�߶�
    nSpacingFooterAndText = pc.m_nSpacingFooterAndText;
    // �������߶ȣ��߶ȵ�����������ÿ�и߶ȼ�ˮƽ�ָ��߿��
    const CString& strFooter = pc.m_strFooter;
    for ( LONG i = 0, nLines = 1; ; nLines++ ) {
      i = strFooter.Find( PFX_EC_PRINT_MACRO_CRLF, i );
      if ( i == -1 ) {
        break;
      }
      i += _tcslen( PFX_EC_PRINT_MACRO_CRLF );
    }
    nHeightFooter = nLines * nHeightHeaderAndFooterLine + nWidthSeparateLine;
  }
  pc.m_nWidthSeparateLine = nWidthSeparateLine;
  pc.m_nHeaderAndFooterLineHeight = nHeightHeaderAndFooterLine;
  // ��������
  CRect rectHeader( &rectDraw );
  rectHeader.bottom = rectHeader.top + nHeightHeader;
  rectHeader.NormalizeRect();
  pc.m_rectHeader.CopyRect( &rectHeader );
  // ��ע����
  CRect rectFooter( &rectDraw );
  rectFooter.top = rectFooter.bottom - nHeightFooter;
  rectFooter.NormalizeRect();
  pc.m_rectFooter.CopyRect( &rectFooter );
  // �к�����
  CRect rectLineNumber;
  rectLineNumber.SetRect( rectDraw.left, ( rectHeader.bottom + nSpacingHeaderAndText ), ( rectDraw.left + nWidthLineNumber ), ( rectFooter.top - nSpacingFooterAndText ) );
  rectLineNumber.NormalizeRect();
  pc.m_rectLineNumber.CopyRect( &rectLineNumber );
  // �ı�����
  CRect rectText;
  rectText.SetRect( ( rectLineNumber.right + nSpacingLineNumberAndText ), rectLineNumber.top, rectDraw.right, rectLineNumber.bottom );
  rectText.NormalizeRect();
  pc.m_rectText.CopyRect( &rectText );
  // ÿҳ�ܹ���ʾ������
  pc.m_nLinesPerPage = rectText.Height() / pc.m_nLineHeight;
}
void CPfxEditView::PrintFormatText( HDC hdcPrinter, CPfxEditViewPrintContext& pc ) {
  CPfxEditViewWrapLineHint wl;
  wl.m_hDC = hdcPrinter;
  wl.m_nliCur = 0;
  wl.m_nGrowBy = 10000;
  wl.m_nTabSize = pc.m_nTabSize;
  wl.m_nCharWidthMax = pc.m_nCharWidthMax;
  wl.m_nCharWidthTab = pc.m_nCharWidthTab;
  wl.m_nCharWidthSpace = pc.m_nCharWidthSpace;
  CPfxEditHeap& Heap = pc.m_cHeap;
  CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( 0 );
  CPfxEditLineInfo* pliEnd = m_pdb->TextGetLineAt( m_pdb->TextGetLineUpperBound() );
  CPfxEditLineInfo* pliLine = NULL;
  // ���㻻�н������ݣ���Щ���ݽ���ת������ɰٷֱ�֪ͨ�͸�
  // m_pdb->EventSendMsg()��һ��������ʾ��ǰ�Ľ��ȸ��û���
  // ÿ�ٷ�һ��ռ����������Щ������ָ������
  LONG nLine = 0;
  LONG nPercentCompleted = 0;
  LONG nLinesOfOnePercent = max( 1, ( ( GetLineCount() + 99 ) / 100 ) );
  switch ( pc.m_nWrapStyle ) {
  case PFX_EC_WRAP_WINDOWEDGE:
    wl.m_nWidth = pc.m_rectText.Width();
    wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
    Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
    for ( nLine = 0; pli <= pliEnd; nLine++ ) {
      // ���µ�ǰ��ָ��ƫ��������Ϊ�����ǰ�������Զ�����
      // ģʽ��pli->nIndex ���ǵ���PFX_EC_INVALID_LINE������
      // ӡģ����Ҫ����ֵΪ��ʵ�к�
      LONG nli = wl.m_nliCur;
      WrapLineAtUnits( Heap, pli, wl );
      pliLine = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) + nli );
      pliLine->nIndex = nLine;
      // ��������
      while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
        //  do nothing
      }
      if ( ( nLine % nLinesOfOnePercent ) == 0 ) {
        nPercentCompleted++;
        m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
      }
    }
    break;
  case PFX_EC_WRAP_COLUMNS:
    wl.m_nWidth = GetWrapWidth();
    wl.m_nliMax = Heap.GetBufferConstSize() / sizeof( CPfxEditLineInfo );
    Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
    for ( nLine = 0; pli <= pliEnd; nLine++ ) {
      // ���µ�ǰ��ָ��ƫ��������Ϊ�����ǰ�������Զ�����
      // ģʽ��pli->nIndex ���ǵ���PFX_EC_INVALID_LINE������
      // ӡģ����Ҫ����ֵΪ��ʵ�к�
      LONG nli = wl.m_nliCur;
      WrapLineColumns( Heap, pli, wl );
      pliLine = ( static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() ) + nli );
      pliLine->nIndex = nLine;
      // ��������
      while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
        //  do nothing
      }
      if ( ( nLine % nLinesOfOnePercent ) == 0 ) {
        nPercentCompleted++;
        m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
      }
    }
    break;
  case PFX_EC_WRAP_NONE:
  default:           // ��ͨ
    wl.m_nWidth = 0;
    wl.m_nliMax = GetLineCount() + 1;
    Heap.Alloc( ( wl.m_nliMax * sizeof( CPfxEditLineInfo ) ) );
    pliLine = static_cast<CPfxEditLineInfo*>( Heap.GetBuffer() );
    for ( nLine = 0; pli <= pliEnd; pliLine++, nLine++, wl.m_nliCur++ ) {
      pliLine->dwFlags = ( pli->dwFlags | PFX_EC_LF_HARDRETURN );
      pliLine->nActLen = pliLine->nLen = pli->nActLen;
      pliLine->pData = pli->pData;
      pliLine->pMemBlock = pli->pMemBlock;
      pliLine->nIndex = nLine;
      pliLine->nFirstChar = 0;
      // ��������
      while ( ( ++pli <= pliEnd ) && PFX_EC_IS_SUBLINE( pli ) ) {
        //  do nothing
      }
      if ( ( nLine % nLinesOfOnePercent ) == 0 ) {
        nPercentCompleted++;
        m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, ( LPARAM ) nPercentCompleted );
      }
    }
    break;
  }
  // �ܹ�����������б��洢�������������
  pc.m_nMaxLines = wl.m_nliCur;
  // ��ʽ���������%100
  if ( nPercentCompleted < 100 ) {
    m_pdb->EventSendMsg( PFX_EN_WORD_WRAPPING, 100 );
  }
}
void CPfxEditView::PrintHeader( HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  CRect rectHeader( &pc.m_rectHeader );
  if ( pc.IsPrintHeaderSeparator() && ( pc.PenSeparateLine() != NULL ) ) {
    pDC.SelectObject( pc.GetPenSeparateLine() );
    pDC.MoveTo( rectHeader.left, rectHeader.bottom );
    pDC.LineTo( rectHeader.right, rectHeader.bottom );
    rectHeader.bottom -= pc.m_nWidthSeparateLine;
  }
  CString& strText = pc.m_strHeader;
  for ( LONG nEnd, nStart = 0, nLength = strText.GetLength(); ; ) {
    nEnd = strText.Find( PFX_EC_PRINT_MACRO_CRLF, nStart );
    if ( nEnd == -1 ) {
      nEnd = nLength;
    }
    PrintHeaderFooterText( strText.Mid( nStart, ( nEnd - nStart ) ), rectHeader, pDC, pc );
    if ( nEnd >= nLength ) {
      break;
    }
    nStart = nEnd + _tcslen( PFX_EC_PRINT_MACRO_CRLF );
    rectHeader.OffsetRect( 0, pc.m_nHeaderAndFooterLineHeight );
  }
}
void CPfxEditView::PrintFooter( HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  CRect rectFooter( &pc.m_rectFooter );
  if ( pc.IsPrintFooterSeparator() && ( pc.PenSeparateLine() != NULL ) ) {
    pDC.SelectObject( pc.GetPenSeparateLine() );
    pDC.MoveTo( rectFooter.left, rectFooter.top );
    pDC.LineTo( rectFooter.right, rectFooter.top );
    rectFooter.top += pc.m_nWidthSeparateLine;
  }
  CString& strText = pc.m_strFooter;
  for ( LONG nEnd, nStart = 0, nLength = strText.GetLength(); ; ) {
    nEnd = strText.Find( PFX_EC_PRINT_MACRO_CRLF, nStart );
    if ( nEnd == -1 ) {
      nEnd = nLength;
    }
    PrintHeaderFooterText( strText.Mid( nStart, ( nEnd - nStart ) ), rectFooter, pDC, pc );
    if ( nEnd >= nLength ) {
      break;
    }
    nStart = nEnd + _tcslen( PFX_EC_PRINT_MACRO_CRLF );
    rectFooter.OffsetRect( 0, pc.m_nHeaderAndFooterLineHeight );
  }
}
void CPfxEditView::PrintLineNumber( LONG nLine, HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  if ( ( !pc.IsPrintLineNumber() ) || ( nLine == PFX_EC_INVALID_LINE ) ) {
    return ;
  }
  TCHAR sz[ 16 ];
  LONG cchText = _stprintf( sz, _T( "%ld" ), ( nLine + 1 ) );
  CRect rect( pc.m_rectLineNumber.left, pc.m_rectLine.top, pc.m_rectLineNumber.right, pc.m_rectLine.bottom );
  CPoint pt( rect.right, rect.top + ( rect.Height() - pc.m_nLineHeight ) / 2 );
  pDC.SetTextAlign( TA_RIGHT | TA_TOP | TA_NOUPDATECP );
  pDC.SetTextColor( pc.COLOR( kcLineNumber ) );
  ::ExtTextOut( hDC, pt.x, pt.y, ETO_CLIPPED, &rect, sz, cchText, NULL );
}
void CPfxEditView::PrintMargin( const CRect& rectDraw, HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  // �����Ԥ��������ҳ�߾�
  if ( !pc.IsPreview() || !pc.IsPrintMargin() ) {
    return ;
  }
  pDC.SelectObject( pc.GetPenMargin() );
  // ------------->
  pDC.MoveTo( rectDraw.left, pc.m_rectDraw.top );
  pDC.LineTo( rectDraw.right, pc.m_rectDraw.top );
  // -------------|
  //        |
  //        V
  pDC.MoveTo( pc.m_rectDraw.right, rectDraw.top );
  pDC.LineTo( pc.m_rectDraw.right, rectDraw.bottom );
  // -------------|
  //        |
  //        |
  // <-------------
  pDC.MoveTo( rectDraw.right, pc.m_rectDraw.bottom );
  pDC.LineTo( rectDraw.left, pc.m_rectDraw.bottom );
  // -------------|
  // ^            |
  // |            |
  // |------------|
  pDC.MoveTo( pc.m_rectDraw.left, rectDraw.bottom );
  pDC.LineTo( pc.m_rectDraw.left, rectDraw.top );
}
void CPfxEditView::PrintHilighted( HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  const CPfxEditLineInfo * pli = reinterpret_cast<const CPfxEditLineInfo*>( pc.m_cHeap.GetBuffer() );
  const CPfxEditLineInfo* pliEnd = pli;
  // ������ҳ��Ҫ��ӡ����
  ParserPreParse( pc.m_nCurPageLastLine, ( pc.m_nMaxLines - 1 ), 0, pc.m_nLineLastParsed, const_cast<CPfxEditLineInfo*>( pli ) );
  pli += pc.m_nCurPageFirstLine;
  pliEnd += pc.m_nCurPageLastLine;
  LONG nTabSize = pc.m_nTabSize;
  LONG nLineHeight = pc.m_nLineHeight;
  pc.m_rectLine.CopyRect( &pc.m_rectText );
  pc.m_rectLine.bottom = pc.m_rectText.top + nLineHeight;
  for ( ; pli < pliEnd; pli++ ) {
    // �����к�
    PrintLineNumber( pli->nIndex, pDC, pc );
    // �����ı�
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      PrintHilightedLine( pli, pDC, pc );
    }
    pc.m_rectLine.OffsetRect( 0, nLineHeight );
  }
}
void CPfxEditView::PrintHilightedString1( LPCTSTR pData, LONG nLen, HDC hDC, CPfxEditViewPrintContext& pc ) {
  CDCH pDC = hDC;
  if ( ( pData == NULL ) || ( nLen <= 0 ) ) {
    return ;
  }
  LONG i = m_pti->m_mapKeywords.Lookup( pData, nLen );
  COLORREF clr = pc.COLOR( ( i != -1 ) ? i : kcText );
  if ( clr != pc.m_clrCurText ) {
    pc.m_clrCurText = clr;
    pDC.SetTextColor( clr );
  }
  ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pData, nLen, NULL );
  ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
  pc.m_ptCurPos.y = pc.m_rectLine.top;
  pDC.MoveTo( pc.m_ptCurPos );
}
void CPfxEditView::PrintHilightedString2( LPCTSTR pData, LONG nLen, LONG& rnLogChar, COLORREF color, HDC hDC, CPfxEditViewPrintContext& pc ) {
  if ( ( pData == NULL ) || ( nLen == 0 ) ) {
    return ;
  }
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectText.right;
  pc.m_clrCurText = color;
  pDC.SetTextColor( pc.m_clrCurText );
  LPCTSTR pCur = pData;
  LPCTSTR pTmp = pCur;
  const LPCTSTR pEnd = pData + nLen;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
#ifdef _MBCS
    if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      rnLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
#endif
    if ( *pCur != PFX_EC_CHR_TAB ) {
      rnLogChar++;
      pCur++;
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pTmp, ( pCur - pTmp ), NULL );
      }
      LONG nTabSize = pc.m_nTabSize - rnLogChar % pc.m_nTabSize;
      pTmp = ++pCur;
      rnLogChar += nTabSize;
      // ���ʹ�ð��������ŵķ��������Ʊ���Ŀ�ȣ��ڴ�ӡ
      // Ԥ����ʱ����ܻ���ֲ�һ�£�����Ҫ��GDI ��������
      // ����
      TCHAR sz[ 1 ];
      sz[ 0 ] = PFX_EC_CHR_SPACE;
      CSize size;
      ::GetTextExtentPoint32( hDC, sz, 1, &size );
      ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      pc.m_ptCurPos.x += nTabSize * size.cx;
      pc.m_ptCurPos.y = pc.m_rectLine.top;
      pDC.MoveTo( pc.m_ptCurPos );
    }
  }
  if ( pCur > pTmp ) {
    ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pTmp, ( min( pEnd, pCur ) - pTmp ), NULL );
    ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
    pc.m_ptCurPos.y = pc.m_rectLine.top;
    pDC.MoveTo( pc.m_ptCurPos );
  }
}
LPCTSTR CPfxEditView::PrintHilightedStringBaseOnCookie( LPCTSTR pData, LONG nLen, LONG& rnLogChar, DWORD dwCookie, HDC hDC, CPfxEditViewPrintContext& pc ) {
  const LPCTSTR pStart = pData;
  const LPCTSTR pEnd = pStart + nLen;
  CDCH pDC = hDC;
  // ����һ�п�ʼʹ������ע�ͣ�
  // abcdefg
  // hijklmn
  // �������ֻ��Ҫ�����ǰ�е�ȫ�����ݱ��
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT1 ) {
    PrintHilightedString2( pStart, ( pEnd - pStart ), rnLogChar, pc.COLOR( kcLineComment1 ), pDC, pc );
    return NULL;
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT2 ) {
    PrintHilightedString2( pStart, ( pEnd - pStart ), rnLogChar, pc.COLOR( kcLineComment2 ), pDC, pc );
    return NULL;
  }
  // ��һ�еĿ�ͷ���ֻ�ȫ���ǿ�ע�͵�һ���֣�
  // abcdefg /* this is block comment
  // samples */ hijklmn
  if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT1 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpBlockComment1 ) );
    if ( pFound == NULL )           //      û���ҵ���һֱ�������ĩ
    {
      pFound = pEnd;
    }
    PrintHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcBlockComment1 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT2 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpBlockComment2 ) );
    if ( pFound == NULL )           //      û���ҵ���һֱ�������ĩ
    {
      pFound = pEnd;
    }
    PrintHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcBlockComment2 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING1 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpQuotedString1 ) );
    if ( pFound == NULL )           //      û���ҵ���һֱ�������ĩ
    {
      pFound = pEnd;
    }
    PrintHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcQuotedString1 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING2 ) {
    LPCTSTR pFound = ParserGetMarkerCloseEndPosition( pStart, pStart, pEnd, false, m_pti->GetMarkerPair( mpQuotedString2 ) );
    if ( pFound == NULL )           //      û���ҵ���һֱ�������ĩ
    {
      pFound = pEnd;
    }
    PrintHilightedString2( pStart, ( pFound - pStart ), rnLogChar, pc.COLOR( kcQuotedString2 ), pDC, pc );
    return ( ( pFound == pEnd ) ? NULL : pFound );
  }
  return pStart;
}
void CPfxEditView::PrintHilightedLine( const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPrintContext& pc ) {
  ASSERT( !PFX_EC_IS_EMPTY_LINE( pli ) );
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectLine.right;
  LONG nTabSize = 0;
  LONG nLogChar = pli->nFirstChar;
  DWORD dwCookie = pli->dwFlags & PFX_EC_LF_COOKIE_ALL;
  pc.m_ptCurPos.x = pc.m_rectLine.left;
  pc.m_ptCurPos.y = pc.m_rectLine.top;
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nLen;
  LPCTSTR pCur = pStart;
  pc.m_clrCurText = pc.COLOR( kcText );
  pDC.SetTextColor( pc.m_clrCurText );
  pDC.SetTextAlign( TA_LEFT | TA_TOP | TA_UPDATECP );
  pDC.MoveTo( pc.m_ptCurPos );
  pCur = PrintHilightedStringBaseOnCookie( pStart, pli->nLen, nLogChar, dwCookie, pDC, pc );
  // �����һ������һ��COOKIE���������Ͳ���Ҫ����
  if ( pCur == NULL ) {
    return ;
  }
  // ÿ�η���һ���ַ���ʱ�򽫸��ַ�������ı�ʶ���ĵ�һ���ַ��Ƚϣ���
  // �����Ⱦ�����Ҫ�����Ƚϣ��������Լӿ�����ٶ�
  CPfxEditViewMarkerChar mc;
  ParserGetFirstCharOfMarkerStart( mc );
  LPCTSTR pTmp = pCur;
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    // ��ע��1
    if ( ( ( wc == mc.m_cLowerLineComment1 ) || ( wc == mc.m_cUpperLineComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment1 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      PrintHilightedString2( pCur, ( pEnd - pCur ), nLogChar, pc.COLOR( kcLineComment1 ), pDC, pc );
      pCur = pTmp = pEnd;
      break;
    }
    // ��ע��2
    if ( ( ( wc == mc.m_cLowerLineComment2 ) || ( wc == mc.m_cUpperLineComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment2 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      PrintHilightedString2( pCur, ( pEnd - pCur ), nLogChar, pc.COLOR( kcLineComment2 ), pDC, pc );
      pCur = pTmp = pEnd;
      break;
    }
    // ��ע��1
    if ( ( ( wc == mc.m_cLowerBlockComment1 ) || ( wc == mc.m_cUpperBlockComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment1 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment1 ) );
      if ( pTmp == NULL )           //      û���ҵ���һֱ�������ĩ
      {
        pTmp = pEnd;
      }
      PrintHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcBlockComment1 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // ��ע��2
    if ( ( ( wc == mc.m_cLowerBlockComment2 ) || ( wc == mc.m_cUpperBlockComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment2 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment2 ) );
      if ( pTmp == NULL )           //      û���ҵ���һֱ�������ĩ
      {
        pTmp = pEnd;
      }
      PrintHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcBlockComment2 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // �����ַ���1
    if ( ( ( wc == mc.m_cLowerQuotedString1 ) || ( wc == mc.m_cUpperQuotedString1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString1 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString1 ) );
      if ( pTmp == NULL )           //      û���ҵ���һֱ�������ĩ
      {
        pTmp = pEnd;
      }
      PrintHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcQuotedString1 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // �����ַ���2
    if ( ( ( wc == mc.m_cLowerQuotedString2 ) || ( wc == mc.m_cUpperQuotedString2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString2 ) ) ) {
      PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
      pTmp = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString2 ) );
      if ( pTmp == NULL )           //      û���ҵ���һֱ�������ĩ
      {
        pTmp = pEnd;
      }
      PrintHilightedString2( pCur, ( pTmp - pCur ), nLogChar, pc.COLOR( kcQuotedString2 ), pDC, pc );
      pCur = pTmp;
      continue;
    }
    // �������ַ����ǿ�ı�ʶ�������潫��������Ƿ��ǹؼ��ֵ�
    // һ���֣������ǿհ��ַ��������ǵ��ʶ������
    if ( !IsWhitespace( wc ) && !IsDelimiter( wc ) ) {
      if ( bLeadByte ) {
        pCur += PFX_EC_MAX_MB_LEN;
        nLogChar += PFX_EC_MAX_MB_LEN;
      } else {
        pCur++;
        nLogChar++;
      }
    } else {
      if ( pCur > pTmp )           //      ����֮ǰ�Ĳ���
      {
        PrintHilightedString1( pTmp, ( pCur - pTmp ), pDC, pc );
        pTmp = pCur;
      }
      // ���ʹ�ð��������ŵķ��������Ʊ���Ŀ�ȣ��ڴ�ӡ
      // Ԥ����ʱ����ܻ���ֲ�һ�£�����Ҫ��GDI ��������
      // ����
      if ( ( wc == PFX_EC_CHR_SPACE ) || ( wc == PFX_EC_CHR_TAB ) ) {
        pCur++;
        PrintHilightedString2( pTmp, 1, nLogChar, pc.m_clrCurTextBkgnd, pDC, pc );
      } else // Delimiter!
      {
        if ( bLeadByte ) {
          pCur += PFX_EC_MAX_MB_LEN;
          nLogChar += PFX_EC_MAX_MB_LEN;
        } else {
          pCur++;
          nLogChar++;
        }
        //  ��Щ������ԵĹؼ����п��ܺ��е��ʶ����
        //  ���������Ծ����С�.XLIST��.XALL���� ��
        //  αָ�C++�����С�==��!=��->��>>�� ����
        //  �������˵��������ʶ����ʱӦ���ٷ���һ
        //  ��������ĵ��������Ƿ���һ���ؼ���
        LPCTSTR pTmpData = pCur;
        LONG nTmpLogChar = nLogChar;
        while ( ( pTmpData < pEnd )
                && !IsWhitespace( *pTmpData ) ) {
          LONG nStep = ( PFX_EC_IS_LEAD_BYTE( *pTmpData ) ? PFX_EC_MAX_MB_LEN : 1 );
          //   ��������ĵ�һ���ַ����ǵ��ʶ���
          //   �����Ͱ����ö���������򲻰�����
          if ( IsDelimiter( pTmpData, pEnd ) ) {
            if ( pTmpData == pCur ) {
              pTmpData += nStep;
              nTmpLogChar += nStep;
            }
            break;
          }
          pTmpData += nStep;
          nTmpLogChar += nStep;
        }
        pTmpData = min( pTmpData, pEnd );
        if ( ( pTmpData != pCur )
             && ParserIsKeyword( pTmp, ( pTmpData - pTmp ) ) ) {
          pCur = pTmpData;
          nLogChar = nTmpLogChar;
        }
        PrintHilightedString1( pTmp, ( min( pEnd, pCur ) - pTmp ), pDC, pc );
      }
      pTmp = pCur;
    }
  }
  // ����ʣ��Ĳ��֣�����û�п��ǵ��Զ�����ʱ���е����⣬���Ե�һ����
  // ���ֱ�ǿ���۶ϵ�ʱ���������ᱻ��ȷ�ط�������ȷ����ʾ
  if ( ( pCur > pTmp ) && ( x2 > pc.m_ptCurPos.x ) ) {
    PrintHilightedString1( pTmp, ( min( pEnd, pCur ) - pTmp ), pDC, pc );
  }
}
void CPfxEditView::PrintPlainText( HDC hDC, CPfxEditViewPrintContext& pc ) {
  const CPfxEditLineInfo * pli =
    reinterpret_cast<const CPfxEditLineInfo*>( pc.m_cHeap.GetBuffer() );
  const CPfxEditLineInfo* pliEnd = pli;
  CDCH pDC = hDC;
  pli += pc.m_nCurPageFirstLine;
  pliEnd += pc.m_nCurPageLastLine;
  LONG nTabSize = pc.m_nTabSize;
  LONG nLineHeight = pc.m_nLineHeight;
  pc.m_rectLine.CopyRect( &pc.m_rectText );
  pc.m_rectLine.bottom = pc.m_rectText.top + nLineHeight;
  for ( ; pli < pliEnd; pli++ ) {
    // �����к�
    PrintLineNumber( pli->nIndex, pDC, pc );
    // �����ı�
    if ( !PFX_EC_IS_EMPTY_LINE( pli ) ) {
      PrintPlainTextLine( pli, pDC, pc );
    }
    pc.m_rectLine.OffsetRect( 0, nLineHeight );
  }
}
void CPfxEditView::PrintPlainTextLine( const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPrintContext& pc ) {
  ASSERT( !PFX_EC_IS_EMPTY_LINE( pli ) );
  CDCH pDC = hDC;
  LONG x2 = pc.m_rectLine.right;
  LONG nTabSize = 0;
  LONG nLogChar = pli->nFirstChar;
  pc.m_ptCurPos.x = pc.m_rectLine.left;
  pc.m_ptCurPos.y = pc.m_rectLine.top;
  LPCTSTR pCur = pli->pData;
  LPCTSTR pTmp = pCur;
  const LPCTSTR pEnd = pCur + pli->nLen;
  pDC.SetTextAlign( TA_LEFT | TA_TOP | TA_UPDATECP );
  pDC.SetTextColor( pc.COLOR( kcText ) );
  pDC.MoveTo( pc.m_ptCurPos );
  while ( ( pCur < pEnd ) && ( x2 > pc.m_ptCurPos.x ) ) {
#ifdef _MBCS
    if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
      nLogChar += PFX_EC_MAX_MB_LEN;
      pCur += PFX_EC_MAX_MB_LEN;
      continue;
    }
#endif
    if ( *pCur != PFX_EC_CHR_TAB ) {
      nLogChar++;
      pCur++;
    } else {
      if ( pCur > pTmp ) {
        ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pTmp, pCur - pTmp, NULL );
      }
      LONG nTabSize = pc.m_nTabSize - nLogChar % pc.m_nTabSize;
      pTmp = ++pCur;
      nLogChar += nTabSize;
      // ���ʹ�ð��������ŵķ��������Ʊ���Ŀ�ȣ��ڴ�ӡ
      // Ԥ����ʱ����ܻ���ֲ�һ�£�����Ҫ��GDI ��������
      // ����
      TCHAR sz[ 1 ];
      sz[ 0 ] = PFX_EC_CHR_SPACE;
      CSize size;
      ::GetTextExtentPoint32( hDC, sz, 1, &size );
      ::GetCurrentPositionEx( hDC, &pc.m_ptCurPos );
      pc.m_ptCurPos.x += nTabSize * size.cx;
      pc.m_ptCurPos.y = pc.m_rectLine.top;
      pDC.MoveTo( pc.m_ptCurPos );
    }
  }
  if ( pCur > pTmp ) {
    ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &pc.m_rectLine, pTmp, min( pEnd, pCur ) - pTmp, NULL );
  }
}
void CPfxEditView::PrintHeaderFooterText( const CString& strText, const CRect& rectDraw, HDC hDC, CPfxEditViewPrintContext& pc ) {
  struct CAlignParam {
    UINT nAlign;
    LPCTSTR pszAlignParam;
  };
  static const CAlignParam s_align[] = { {
                                           TA_LEFT, PFX_EC_PRINT_MACRO_ALIGN_LEFT
                                         }
                                         , {TA_CENTER, PFX_EC_PRINT_MACRO_ALIGN_CENTER}, {TA_RIGHT, PFX_EC_PRINT_MACRO_ALIGN_RIGHT},
                                       };
  CString strLeft;
  CString strCenter;
  CString strRight;
  // ���ȷ���͹鲢�����������ҡ����ж���Ĳ���
  LONG i;
  LONG nStart = 0;
  UINT nAlign = TA_LEFT;
  for ( ;; ) {
    LONG nEnd = INT_MAX;
    const CAlignParam* pAlign = NULL;
    for ( i = 0; i < PFX_EC_SIZEOF( s_align ); i++ ) {
      LONG nPos = strText.Find( s_align[ i ].pszAlignParam, nStart );
      if ( ( nPos != -1 ) && ( nPos < nEnd ) ) {
        nEnd = nPos;
        pAlign = &s_align[ i ];
      }
    }
    nEnd = min( strText.GetLength(), nEnd );
    switch ( nAlign ) {
    case TA_CENTER:
      strCenter += strText.Mid( nStart, ( nEnd - nStart ) );
      break;
    case TA_RIGHT:
      strRight += strText.Mid( nStart, ( nEnd - nStart ) );
      break;
    case TA_LEFT:
    default:
      strLeft += strText.Mid( nStart, ( nEnd - nStart ) );
      break;
    }
    if ( pAlign == NULL ) {
      break;
    }
    nStart = nEnd + _tcslen( pAlign->pszAlignParam );
    nAlign = pAlign->nAlign;
  }
  // ��ʽ�����ֲ���
  CString* arrStr[] = {&strLeft, &strCenter, &strRight};
  for ( i = 0; i < PFX_EC_SIZEOF( arrStr ); i++ ) {
    CString& str = *arrStr[ i ];
    if ( str.IsEmpty() ) {
      continue;
    }
    TCHAR sz[ MAX_PATH ];
    // ʵʱ��ʽ����ǰҳ��
    if ( str.Find( PFX_EC_PRINT_MACRO_CURRENT_PAGE ) != -1 ) {
      _stprintf( sz, _T( "%d" ), ( pc.m_nCurPage + 1 ) );
      str.Replace( PFX_EC_PRINT_MACRO_CURRENT_PAGE, sz );
    }
    // ��ҳ��
    if ( str.Find( PFX_EC_PRINT_MACRO_TOTAL_PAGES ) != -1 ) {
      _stprintf( sz, _T( "%d" ), pc.m_nTotalPages );
      str.Replace( PFX_EC_PRINT_MACRO_TOTAL_PAGES, sz );
    }
    // ʱ��
    if ( str.Find( PFX_EC_PRINT_MACRO_TIME ) != -1 ) {
      ::GetTimeFormat( LOCALE_USER_DEFAULT, TIME_NOSECONDS, &pc.m_time, NULL, sz, PFX_EC_SIZEOF( sz ) );
      str.Replace( PFX_EC_PRINT_MACRO_TIME, sz );
    }
    // ����
    if ( str.Find( PFX_EC_PRINT_MACRO_DATE ) != -1 ) {
      ::GetDateFormat( LOCALE_USER_DEFAULT, DATE_LONGDATE, &pc.m_time, NULL, sz, PFX_EC_SIZEOF( sz ) );
      str.Replace( PFX_EC_PRINT_MACRO_DATE, sz );
    }
    // �ļ���
    if ( str.Find( PFX_EC_PRINT_MACRO_FILE_NAME ) != -1 ) {
      TCHAR szFileExt[ MAX_PATH ];
      _tsplitpath( pc.m_strPathName, NULL, NULL, sz, szFileExt );
      _tcscat( sz, szFileExt );
      str.Replace( PFX_EC_PRINT_MACRO_FILE_NAME, sz );
    }
    // ·����
    if ( str.Find( PFX_EC_PRINT_MACRO_PATH_NAME ) != -1 ) {
      str.Replace( PFX_EC_PRINT_MACRO_PATH_NAME, pc.m_strPathName );
    }
    // �Ʊ��
    if ( str.Find( PFX_EC_PRINT_MACRO_TAB ) != -1 ) {
      str.Replace( PFX_EC_PRINT_MACRO_TAB, _T( "\t" ) );
    }
  }
  CRect rect( &rectDraw );
  CSize size;
  CPoint pt;
  CDCH pDC = hDC;
  LONG nLogChar = 0;
  LONG nTabSize = 0;
  pDC.SetBkMode( TRANSPARENT );
  pDC.SetTextColor( pc.COLOR( kcHeaderAndFooter ) );
  // ������߲�����Ϣ��
  if ( !strLeft.IsEmpty() ) {
    pt.x = rect.left;
    pt.y = rect.top;
    LPCTSTR pStart = strLeft;
    LPCTSTR pCur = pStart;
    LPCTSTR pEnd = pStart + strLeft.GetLength();
    pDC.SetTextAlign( TA_TOP | TA_LEFT | TA_UPDATECP );
    pDC.MoveTo( pt );
    while ( pCur < pEnd ) {
      if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
        nLogChar += PFX_EC_MAX_MB_LEN;
        pCur += PFX_EC_MAX_MB_LEN;
        continue;
      }
      if ( *pCur != PFX_EC_CHR_TAB ) {
        nLogChar++;
        pCur++;
      } else {
        ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &rect, pStart, pCur - pStart, NULL );
        ::GetCurrentPositionEx( hDC, &pt );
        nTabSize = pc.m_nTabSize - nLogChar % pc.m_nTabSize;
        pt.x += ( nTabSize * pc.m_nCharWidthSpace );
        nLogChar += nTabSize;
        pStart = ++pCur;
        pDC.MoveTo( pt.x, rect.top );
      }
    }
    if ( pStart < pEnd ) {
      ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &rect, pStart, pEnd - pStart, NULL );
    }
  }
  // �����м䲿����Ϣ��
  if ( !strCenter.IsEmpty() ) {
    nTabSize = pc.m_nTabSize * pc.m_nCharWidthSpace;
    DWORD dwTabbedExt = ::GetTabbedTextExtent( hDC, strCenter, strCenter.GetLength(), 1, reinterpret_cast<LPINT>( &nTabSize ) );
    size.cx = LOWORD( dwTabbedExt );
    size.cy = HIWORD( dwTabbedExt );
    pt.x = rect.left + ( rect.Width() - size.cx ) / 2;
    pt.y = rect.top;
    LPCTSTR pStart = strCenter;
    LPCTSTR pCur = pStart;
    LPCTSTR pEnd = pStart + strCenter.GetLength();
    pDC.SetTextAlign( TA_TOP | TA_LEFT | TA_UPDATECP );
    pDC.MoveTo( pt );
    while ( pCur < pEnd ) {
#ifdef _MBCS
      if ( PFX_EC_IS_LEAD_BYTE( *pCur ) ) {
        nLogChar += PFX_EC_MAX_MB_LEN;
        pCur += PFX_EC_MAX_MB_LEN;
        continue;
      }
#endif
      if ( *pCur != PFX_EC_CHR_TAB ) {
        nLogChar++;
        pCur++;
      } else {
        ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &rect, pStart, pCur - pStart, NULL );
        ::GetCurrentPositionEx( hDC, &pt );
        nTabSize = pc.m_nTabSize - nLogChar % pc.m_nTabSize;
        pt.x += ( nTabSize * pc.m_nCharWidthSpace );
        nLogChar += nTabSize;
        pStart = ++pCur;
        pDC.MoveTo( pt.x, rect.top );
      }
    }
    if ( pStart < pEnd ) {
      ::ExtTextOut( hDC, 0, 0, ETO_CLIPPED, &rect, pStart, pEnd - pStart, NULL );
    }
  }
  // �����ұ߲�����Ϣ��
  if ( !strRight.IsEmpty() ) {
    pt.x = rect.right;
    pt.y = rect.top;
    LPCTSTR pStart = strRight;
    LPCTSTR pEnd = pStart + strRight.GetLength();
    LPCTSTR pCur = pEnd - 1;
    LPCTSTR pTmp = pCur;
    pDC.SetTextAlign( TA_TOP | TA_LEFT | TA_NOUPDATECP );
    for ( ;; ) {
      if ( *pTmp == PFX_EC_CHR_TAB ) {
        ::GetTextExtentPoint32( hDC, pCur, ( pEnd - pCur ), &size );
        pt.x -= size.cx;
        ::ExtTextOut( hDC, pt.x, pt.y, ETO_CLIPPED, &rect, pCur, ( pEnd - pCur ), NULL );
        nTabSize = pc.m_nTabSize - nLogChar % pc.m_nTabSize;
        nLogChar += nTabSize;
        pt.x -= ( nTabSize * pc.m_nCharWidthSpace );
        pEnd = pTmp;
      } else {
        if ( pTmp == pStart ) {
          ::GetTextExtentPoint32( hDC, pTmp, ( pEnd - pTmp ), &size );
          pt.x -= size.cx;
          ::ExtTextOut( hDC, pt.x, pt.y, ETO_CLIPPED, &rect, pTmp, ( pEnd - pTmp ), NULL );
          break;
        }
        nLogChar += ( pCur - pTmp );
      }
      pCur = pTmp;
      pTmp = GetPrevCharPos( pTmp, pStart );
    }
  }
}
BOOL CPfxEditView::SearchPreFind( CPfxEditViewFindReplace& rFindReplace ) {
  if ( rFindReplace.pszFindWhat == NULL ) {
    return false;
  }
  if ( rFindReplace.cchFindWhat == -1 ) {
    rFindReplace.cchFindWhat = _tcslen( rFindReplace.pszFindWhat );
  }
  if ( rFindReplace.cchFindWhat == 0 ) {
    return false;
  }
  // �������ƥ���Сд����Ҫ���ҵ��ַ���ת����Сд
  if ( !rFindReplace.bMatchCase ) {
    LPTSTR pFindWhat =
      rFindReplace.strCaseBuffer.GetBufferSetLength( rFindReplace.cchFindWhat );
    _tcsncpy( pFindWhat, rFindReplace.pszFindWhat, rFindReplace.cchFindWhat );
    _tcslwr( pFindWhat );
    rFindReplace.pszFindWhat = pFindWhat;
  }
  // ��ȫ����ǡ������������������
  if ( rFindReplace.bMarkAll ) {
    return true;
  }
  if ( rFindReplace.bDirectDown ) {
    rFindReplace.ptTo.y = m_pdb->TextGetLineUpperBound();
    rFindReplace.ptTo.x = m_pdb->TextGetLineLength( rFindReplace.ptTo.y );
  } else {
    rFindReplace.ptTo.y = 0;
    rFindReplace.ptTo.x = 0;
  }
  m_pdb->TextSubLineToMainLine( rFindReplace.ptTo );
  // У���С����Ƿ�Ϸ�
  rFindReplace.ptFrom.y = m_pdb->GetValidLine( rFindReplace.ptFrom.y );
  rFindReplace.ptFrom.x = m_pdb->GetValidChar( rFindReplace.ptFrom.y, rFindReplace.ptFrom.x );
  // ������λ��ת��Ϊ���λ��
  m_pdb->TextAbsLineToSubLine( rFindReplace.ptFrom );
  // ����Ƿ���MBCS�ַ����м�
  VerifyPointerPosition( rFindReplace.ptFrom );
  // ����ƫ����ת��������ƫ����
  m_pdb->TextSubLineToMainLine( rFindReplace.ptFrom );
  return true;
}
// �ҵ�����TRUE�����򷵻�FALSE
// �ο�CPfxEditViewFindReplace�ṹ������
BOOL CPfxEditView::SearchPreReplace( CPfxEditViewFindReplace& rFindReplace ) {
  // pszReplaceWith����ΪNULLҲ����Ϊ�㳤�ȣ������ɾ��
  if ( ( rFindReplace.pszReplaceWith != NULL )
       && ( rFindReplace.cchReplaceWith == -1 ) ) {
    rFindReplace.cchReplaceWith = _tcslen( rFindReplace.pszReplaceWith );
  }
  if ( rFindReplace.bIgnoreFindWhat ) {
    ASSERT( !rFindReplace.bInSelection );
    // У�������Ƿ�Ϸ�
    rFindReplace.ptFrom.y = m_pdb->GetValidLine( rFindReplace.ptFrom.y );
    rFindReplace.ptFrom.x = m_pdb->GetValidChar( rFindReplace.ptFrom.y, rFindReplace.ptFrom.x );
    rFindReplace.ptTo.y = m_pdb->GetValidLine( rFindReplace.ptTo.y );
    rFindReplace.ptTo.x = m_pdb->GetValidChar( rFindReplace.ptTo.y, rFindReplace.ptTo.x );
    // ������λ��ת��Ϊ���λ��
    m_pdb->TextAbsLineToSubLine( rFindReplace.ptFrom );
    m_pdb->TextAbsLineToSubLine( rFindReplace.ptTo );
    // ���ָ��λ���Ƿ���MBCS�ַ����м�
    VerifyPointerPosition( rFindReplace.ptFrom );
    VerifyPointerPosition( rFindReplace.ptTo );
  } else {
    if ( rFindReplace.pszFindWhat == NULL ) {
      return false;
    }
    if ( rFindReplace.cchFindWhat == -1 ) {
      rFindReplace.cchFindWhat = _tcslen( rFindReplace.pszFindWhat );
    }
    if ( rFindReplace.cchFindWhat == 0 ) {
      return false;
    }
    // �������ƥ���Сд����Ҫ���ҵ��ַ���ת����Сд
    if ( !rFindReplace.bMatchCase ) {
      LPTSTR pFindWhat =
        rFindReplace.strCaseBuffer.GetBufferSetLength( rFindReplace.cchFindWhat );
      _tcsncpy( pFindWhat, rFindReplace.pszFindWhat, rFindReplace.cchFindWhat );
      _tcslwr( pFindWhat );
      rFindReplace.pszFindWhat = pFindWhat;
    }
    if ( rFindReplace.bInSelection && IsTextSelected() && !IsColumnSelect() ) {
      GetSelRange( rFindReplace.ptFrom, rFindReplace.ptTo );
    } else {
      rFindReplace.ptFrom.y = 0;
      rFindReplace.ptFrom.x = 0;
      rFindReplace.ptTo.y = m_pdb->TextGetLineUpperBound();
      rFindReplace.ptTo.x = m_pdb->TextGetLineLength( rFindReplace.ptTo.y );
      // ����ƫ����ת���ɾ���λ��
      m_pdb->TextSubLineToAbsLine( rFindReplace.ptFrom );
      m_pdb->TextSubLineToAbsLine( rFindReplace.ptTo );
    }
  }
  return true;
}
BOOL CPfxEditView::SearchReplaceNormal( CPfxEditViewFindReplace& rFindReplace ) {
  ASSERT( !IsWrapping() );
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  // ����ʼ�ͽ�������ͬһ��ʱ����������ַ����ĳ������滻�ַ����ĳ���
  // ��һ��ʱ������X������ͻ�ı䣬����Ҫ��ʱ���½�����X����
  LONG cchDiffer = rFindReplace.cchReplaceWith - rFindReplace.cchFindWhat;
  BOOL bFound = false;
  while ( SearchFindNormalDown( rFindReplace ) ) {
    bFound = true;
    CharsReplace( rFindReplace.ptFrom, rFindReplace.ptTo, rFindReplace.pszReplaceWith, rFindReplace.cchReplaceWith, false );
    if ( ptTo.y == rFindReplace.ptFrom.y ) {
      ptTo.x += cchDiffer;
    }
    rFindReplace.ptTo = ptTo;
    m_ptEditPos = rFindReplace.ptFrom;
  }
  rFindReplace.ptFrom = ptFrom;
  rFindReplace.ptTo = ptTo;
  return bFound;
}
BOOL CPfxEditView::SearchReplaceWholeWord( CPfxEditViewFindReplace& rFindReplace ) {
  ASSERT( !IsWrapping() );
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  // ����ʼ�ͽ�������ͬһ��ʱ����������ַ����ĳ������滻�ַ����ĳ���
  // ��һ��ʱ������X������ͻ�ı䣬����Ҫ��ʱ���½�����X����
  LONG cchDiffer = rFindReplace.cchReplaceWith - rFindReplace.cchFindWhat;
  BOOL bFound = false;
  while ( SearchFindWholeWordDown( rFindReplace ) ) {
    bFound = true;
    CharsReplace( rFindReplace.ptFrom, rFindReplace.ptTo, rFindReplace.pszReplaceWith, rFindReplace.cchReplaceWith, false );
    if ( ptTo.y == rFindReplace.ptFrom.y ) {
      ptTo.x += cchDiffer;
    }
    rFindReplace.ptTo = ptTo;
    m_ptEditPos = rFindReplace.ptFrom;
  }
  rFindReplace.ptFrom = ptFrom;
  rFindReplace.ptTo = ptTo;
  return bFound;
}
BOOL CPfxEditView::SearchReplaceRegExp( CPfxEditViewFindReplace& rFindReplace ) {
  ASSERT( !IsWrapping() );
  UNUSED( rFindReplace );
  ASSERT( false );
  return true;
}
BOOL CPfxEditView::SearchDoReplace( CPfxEditViewFindReplace& rFindReplace ) {
  CPfxEditViewWorkingState state( this, true );
  ASSERT( !IsWrapping() );
  BOOL bFound;
  if ( rFindReplace.bRegExp ) {
    bFound = SearchReplaceRegExp( rFindReplace );
  } else if ( rFindReplace.bMatchWholeWord ) {
    bFound = SearchReplaceWholeWord( rFindReplace );
  } else // ����ƥ��
  {
    bFound = SearchReplaceNormal( rFindReplace );
  }
  if ( bFound && rFindReplace.bInSelection ) {
    m_ptEditPos = rFindReplace.ptTo;
    SelRange( rFindReplace.ptFrom );
  }
  return bFound;
}
BOOL CPfxEditView::SearchDoFind( CPfxEditViewFindReplace& rFindReplace ) {
  BOOL bFound;
  if ( rFindReplace.bRegExp ) {
    if ( rFindReplace.bDirectDown ) {
      // ????
      bFound = false;
    } else {
      // ????
      bFound = false;
    }
  } else if ( rFindReplace.bMatchWholeWord ) {
    if ( rFindReplace.bDirectDown ) {
      bFound = SearchFindWholeWordDown( rFindReplace );
    } else {
      bFound = SearchFindWholeWordUp( rFindReplace );
    }
  } else // ����ƥ��
  {
    if ( rFindReplace.bDirectDown ) {
      bFound = SearchFindNormalDown( rFindReplace );
    } else {
      bFound = SearchFindNormalUp( rFindReplace );
    }
  }
  return bFound;
}
BOOL CPfxEditView::SearchFindMarkAll( CPfxEditViewFindReplace& rFindReplace ) {
  // �����������������ǩ
  BookmarkClearAll();
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LONG nLineFrom = 0;
  LONG nLineTo = m_pdb->TextGetLineUpperBound();
  BOOL bFound = false;
  while ( ( nLineFrom <= nLineTo ) && ( nLineFrom != PFX_EC_INVALID_LINE ) ) {
    const CPfxEditLineInfo * pli = m_pdb->TextGetLineAt( nLineFrom );
    LPCTSTR pStart = pli->pData;
    LPCTSTR pEnd = pStart + pli->nActLen;
    LPCTSTR pFind = NULL;
    if ( rFindReplace.bMatchCase ) {
      if ( rFindReplace.bRegExp ) {
        //  ???????????????????
      } else if ( rFindReplace.bMatchWholeWord ) {
        pFind = SearchFindWholeWordStringCase( pStart, pEnd, pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      } else {
        pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      }
    } else {
      if ( rFindReplace.bRegExp ) {
        //  ??????????????
      } else if ( rFindReplace.bMatchWholeWord ) {
        pFind = SearchFindWholeWordStringNoCase( pStart, pEnd, pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      } else {
        pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      }
    }
    if ( pFind != NULL ) {
      CPoint pt( 0, nLineFrom );
      m_pdb->TextMainLineToAbsLine( pt );
      BookmarkToggle( true, pt.y, false );
      bFound = true;
    }
    nLineFrom = m_pdb->TextGetNextMainLine( nLineFrom );
  }
  rFindReplace.ptFrom.x = rFindReplace.ptTo.x = 0;
  rFindReplace.ptFrom.y = rFindReplace.ptTo.y = 0;
  Invalidate( false );
  return bFound;
}
// ƥ���Сд����ƥ�����ֲ����ַ���
// �ҵ�ƥ����󷵻ص�һ���ַ��ĵ�ַ��û���ҵ�ƥ����ַ�������NULL
LPCTSTR CPfxEditView::SearchFindNormalStringCase( LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat ) {
  ASSERT( cch >= 0 );
  ASSERT( cchFindWhat > 0 );
  ASSERT( pszFindWhat != NULL );
  if ( ( cch < cchFindWhat ) || ( pData == NULL ) ) {
    return NULL;
  }
  LPCTSTR pEndString = pData + cch;
  LPCTSTR pEndSearch = pData + ( cch - cchFindWhat );
  LPCTSTR pEndFindWhat = pszFindWhat + cchFindWhat;
  while ( pData <= pEndSearch ) {
    LPCTSTR s1 = pData;
    LPCTSTR s2 = pszFindWhat;
    // MBCS: ok to ++ since doing equality comparison.
    // [This depends on MBCS strings being "legal".]
    while ( ( s1 < pEndString )
            && ( s2 < pEndFindWhat )
            && ( *s1 == *s2 ) ) {
      s1++;
      s2++;
    }
    // OK��ƥ�䣡
    if ( s2 == pEndFindWhat ) {
      return pData;
    }
    pData += ( PFX_EC_IS_LEAD_BYTE( *pData ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// ���Դ�Сд�ͷ�����ƥ������ַ���
// �ҵ�ƥ����󷵻ص�һ���ַ��ĵ�ַ��û���ҵ�ƥ����ַ�������NULL
// ���ǵ����Ӷȣ������������֡�ȫ��/��ǡ������ַ�����ĸ�ַ�
// �йء�ȫ��/��ǡ������ַ���д��ĸ�ַ��Ķ�����ο�c runtime "mbctype.c"
LPCTSTR CPfxEditView::SearchFindNormalStringNoCase( LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat ) {
  ASSERT( cch >= 0 );
  ASSERT( cchFindWhat > 0 );
  ASSERT( pszFindWhat != NULL );
  if ( ( cch < cchFindWhat ) || ( pData == NULL ) ) {
    return NULL;
  }
  LPCTSTR pEndString = pData + cch;
  LPCTSTR pEndSearch = pData + ( cch - cchFindWhat );
  LPCTSTR pEndFindWhat = pszFindWhat + cchFindWhat;
  while ( pData <= pEndSearch ) {
    LPCTSTR s1 = pData;
    LPCTSTR s2 = pszFindWhat;
    while ( ( s1 < pEndString ) && ( s2 < pEndFindWhat ) ) {
      WCHAR c1;
      if ( PFX_EC_IS_LEAD_BYTE( *s1 )
           && ( ( s1 + PFX_EC_MAX_MB_LEN ) <= pEndString ) ) {
        c1 = ( WCHAR ) * ( ( LPWSTR ) s1 );
        s1 += PFX_EC_MAX_MB_LEN;
      } else {
        c1 = ( WCHAR ) * s1;
        s1++;
        //  ������ת��������Сд�ַ�������ȫ��������
        //  ����ȫ����ĸ�ַ�
        if ( ( c1 >= 'A' ) && ( c1 <= 'Z' ) ) {
          c1 += ( 'a' - 'A' );
        }
      }
      WCHAR c2;
      if ( PFX_EC_IS_LEAD_BYTE( *s2 )
           && ( ( s2 + PFX_EC_MAX_MB_LEN ) <= pEndFindWhat ) ) {
        c2 = ( WCHAR ) * ( ( LPWSTR ) s2 );
        s2 += PFX_EC_MAX_MB_LEN;
      } else {
        c2 = ( WCHAR ) * s2;
        s2++;
      }
      if ( c1 != c2 ) {
        goto ContinueCompareNextChar;
      }
    }
    // OK��ƥ�䣡
    if ( s2 >= pEndFindWhat ) {
      return pData;
    }
ContinueCompareNextChar:
    pData += ( PFX_EC_IS_LEAD_BYTE( *pData ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// �����ǡ�������ʽ��Ҳ���ǡ�����ƥ�䡱�ķ�ʽ�����ַ���
// �ҵ�����TRUE��rFindReplace������������Ϣ��ʧ�ܷ���FALSE
BOOL CPfxEditView::SearchFindNormalDown( CPfxEditViewFindReplace& rFindReplace ) {
  LPCTSTR pStart;
  LPCTSTR pEnd;
  LPCTSTR pFind;
  const CPfxEditLineInfo* pli;
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  if ( rFindReplace.bMatchCase ) {
    // ������ҷ�Χ��ͬһ����
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptFrom.x;
      pEnd = pli->pData + ptTo.x;
      pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
    }
    // ���ҷ�Χ����ͬһ����
    else {
      // ������һ��
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptFrom.x;
      pEnd = pli->pData + pli->nActLen;
      pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      // �����м���
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetNextMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y >= ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pFind = SearchFindNormalStringCase( pli->pData, pli->nActLen, pszFindWhat, cchFindWhat );
      }
      // ���������
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pFind = SearchFindNormalStringCase( pli->pData, ptTo.x, pszFindWhat, cchFindWhat );
      }
    }
  }
  // ƥ���Сд���²���
  else {
    // ������ҷ�Χ��ͬһ����
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptFrom.x;
      pEnd = pli->pData + ptTo.x;
      pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
    }
    // ���ҷ�Χ����ͬһ����
    else {
      // ������һ��
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptFrom.x;
      pEnd = pli->pData + pli->nActLen;
      pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
      // �����м���
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetNextMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y >= ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pFind = SearchFindNormalStringNoCase( pli->pData, pli->nActLen, pszFindWhat, cchFindWhat );
      }
      // ���������
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pFind = SearchFindNormalStringNoCase( pli->pData, ptTo.x, pszFindWhat, cchFindWhat );
      }
    }
  }
  if ( pFind != NULL ) {
    rFindReplace.ptFrom.y = ptFrom.y;
    rFindReplace.ptFrom.x = pFind - m_pdb->TextGetLineData( ptFrom.y );
    rFindReplace.ptTo.y = ptFrom.y;
    rFindReplace.ptTo.x = rFindReplace.ptFrom.x + cchFindWhat;
  }
  return ( pFind != NULL );
}
// �����ǡ�������ʽ��Ҳ���ǡ�����ƥ�䡱�ķ�ʽ�����ַ���
// �ҵ�����TRUE��rFindReplace������������Ϣ��ʧ�ܷ���FALSE
BOOL CPfxEditView::SearchFindNormalUp( CPfxEditViewFindReplace& rFindReplace ) {
  LPCTSTR pStart;
  LPCTSTR pEnd;
  LPCTSTR pFind;
  const CPfxEditLineInfo* pli;
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  // ������������ʱ�Ӻ������ָ��ǳ��������Բ�ȡ��һ�еĿ�ͷ��ʼ���ң�
  // Ȼ��Ƚ�λ�ã��������λ���Ѿ���������λ������ҽ���
  LPCTSTR pLastMatch = NULL;
  if ( rFindReplace.bMatchCase ) {
    // ������ҷ�Χ��ͬһ����
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptTo.x;
      pEnd = pli->pData + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pStart = pFind + cchFindWhat;
      }
    }
    // ���ҷ�Χ�������Ķ�����
    else {
      // ������һ��
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData;
      pEnd = pli->pData + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pStart = pFind + cchFindWhat;
      }
      // �����м���
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetPrevMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y < ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pStart = pli->pData;
        pEnd = pli->pData + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pStart = pFind + cchFindWhat;
        }
      }
      // ���������
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pStart = pli->pData + ptTo.x;
        pEnd = pli->pData + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindNormalStringCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pStart = pFind + cchFindWhat;
        }
      }
    }
  }
  // ���Դ�Сд���ϲ���
  else {
    // ������ҷ�Χ��ͬһ����
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData + ptTo.x;
      pEnd = pli->pData + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pStart = pFind + cchFindWhat;
      }
    }
    // ���ҷ�Χ�������Ķ�����
    else {
      // ������һ��
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pStart = pli->pData;
      pEnd = pli->pData + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pStart = pFind + cchFindWhat;
      }
      // �����м���
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetPrevMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y < ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pStart = pli->pData;
        pEnd = pli->pData + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindNormalStringNoCase( pStart, pEnd - pStart, pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pStart = pFind + cchFindWhat;
        }
      }
      // ���������
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pStart = pli->pData + ptTo.x;
        pEnd = pli->pData + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindNormalStringNoCase( pStart, ( pEnd - pStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pStart = pFind + cchFindWhat;
        }
      }
    }
  }
  if ( pFind != NULL ) {
    rFindReplace.ptFrom.y = ptFrom.y;
    rFindReplace.ptFrom.x = pFind - m_pdb->TextGetLineData( ptFrom.y );
    rFindReplace.ptTo.y = ptFrom.y;
    rFindReplace.ptTo.x = rFindReplace.ptFrom.x + cchFindWhat;
  }
  return ( pFind != NULL );
}
// LPCTSTR SearchFindWholeWordStringCase(LPCTSTR pStart,LPCTSTR pEnd,LPCTSTR pData,LONG cch,LPCTSTR pszFindWhat,LONG cchFindWhat)
// LPCTSTR pStart      - ��������ͷ�ĵ�ַ����������pDataһ����Ҳ���ܱ�
//                    pDataС���������Ա����
//                    GetPrevCharPos(LPCTSTR pCur, LPCTSTR pStart)��
//                    ��pStartһ��
// LPCTSTR pEnd        - ��������β��ַ���������Ա����
//                    GetNextCharPos(LPCTSTR pCur, LPCTSTR pEnd)�е�
//                    һ��
// LPCTSTR pData       - �����Ŀ�ʼ��ַ
// LONG cch            - �����ĳ���
// LPCTSTR pszFindWhat - Ҫ���ҵ�Ŀ���ַ���
// LONG cchFindWhat    - Ҫ���ҵ�Ŀ���ַ������ȣ�����һ��ҪNULL��β
LPCTSTR CPfxEditView::SearchFindWholeWordStringCase( LPCTSTR pStart, LPCTSTR pEnd, LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat ) {
  ASSERT( cch >= 0 );
  ASSERT( cchFindWhat > 0 );
  ASSERT( pszFindWhat != NULL );
  if ( ( cch < cchFindWhat ) || ( pData == NULL ) ) {
    return NULL;
  }
  LPCTSTR pEndString = pData + cch;
  LPCTSTR pEndSearch = pData + ( cch - cchFindWhat );
  LPCTSTR pEndFindWhat = pszFindWhat + cchFindWhat;
  LPCTSTR pPrev = ( ( pStart == pData ) ? NULL : GetPrevCharPos( pData, pStart ) );
  // ����������ĵ�һ���ַ���ʼ����Ҫ���ҵĶ�����Ӧ�ñ���Ϊ�Ƿ���Ҫ
  // ��ģ�����Ҫ��Ϊ�����õ�һ���ַ�֮ǰ�ġ��ַ����ǿհ��ַ�
  while ( pData <= pEndSearch ) {
    LPCTSTR s1 = pData;
    LPCTSTR s2 = pszFindWhat;
    WCHAR c1;
    WCHAR c2;
    while ( ( s1 < pEndString ) && ( s2 < pEndFindWhat ) ) {
      if ( PFX_EC_IS_LEAD_BYTE( *s1 )
           && ( ( s1 + PFX_EC_MAX_MB_LEN ) <= pEndString ) ) {
        c1 = ( WCHAR ) * ( ( LPWSTR ) s1 );
        s1 += PFX_EC_MAX_MB_LEN;
      } else {
        c1 = ( WCHAR ) * s1;
        s1++;
      }
      if ( PFX_EC_IS_LEAD_BYTE( *s2 )
           && ( ( s2 + PFX_EC_MAX_MB_LEN ) <= pEndFindWhat ) ) {
        c2 = ( WCHAR ) * ( ( LPWSTR ) s2 );
        s2 += PFX_EC_MAX_MB_LEN;
      } else {
        c2 = ( WCHAR ) * s2;
        s2++;
      }
      if ( c1 != c2 ) {
        goto ContinueCompareNextChar;
      }
    }
    if ( ( s2 == pEndFindWhat )           //      OK��ƥ�䣿
         //  ���Ҫ���ҵ��ַ�����һ���ַ��ǵ��ʶ������ǰ��
         //  ���Ķ����ǰһ���ַ��ǵ��ʶ��������������ǰ����
         && ( ( pPrev == NULL )           //      �����׿�ʼ������
              || IsWhitespace( *pPrev )
              || IsDelimiter( pPrev, pEnd )
              || IsWhitespace( *pszFindWhat )
              || IsDelimiter( pszFindWhat, pEndFindWhat ) )
         //  ���Ҫ���ҵ��ַ��������ַ��ǵ��ʶ������ǰ��
         //  ���Ķ���ĺ�һ���ַ��ǵ��ʶ�������������к󲿷�
         && ( ( s1 >= pEnd )           //      ������ĩ��
              || IsWhitespace( *s1 )
              || IsDelimiter( s1, pEnd )
              || IsWhitespace( c2 )
              || IsDelimiter( c2 ) ) ) {
      return pData;
    }
ContinueCompareNextChar:
    // ���µ�ǰ�ַ�λ�ã����ҵ�ƥ���ַ���ʱ��Ҫ�жϵ�ǰ�ַ��Ƿ�
    // �ǵ��ʶ����
    pPrev = pData;
    pData += ( PFX_EC_IS_LEAD_BYTE( *pData ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// LPCTSTR SearchFindWholeWordStringNoCase(LPCTSTR pStart,LPCTSTR pEnd,LPCTSTR pData,LONG cch,LPCTSTR pszFindWhat,LONG cchFindWhat)
// LPCTSTR pStart      - ��������ͷ�ĵ�ַ����������pDataһ����Ҳ���ܱ�
//                    pDataС���������Ա����
//                    GetPrevCharPos(LPCTSTR pCur, LPCTSTR pStart)��
//                    ��pStartһ��
// LPCTSTR pEnd        - ��������β��ַ���������Ա����
//                    GetNextCharPos(LPCTSTR pCur, LPCTSTR pEnd)�е�
//                    һ��
// LPCTSTR pData       - �����Ŀ�ʼ��ַ
// LONG cch            - �����ĳ���
// LPCTSTR pszFindWhat - Ҫ���ҵ�Ŀ���ַ���
// LONG cchFindWhat    - Ҫ���ҵ�Ŀ���ַ������ȣ�����һ��ҪNULL��β
LPCTSTR CPfxEditView::SearchFindWholeWordStringNoCase( LPCTSTR pStart, LPCTSTR pEnd, LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat ) {
  ASSERT( cch >= 0 );
  ASSERT( cchFindWhat > 0 );
  ASSERT( pszFindWhat != NULL );
  if ( ( cch < cchFindWhat ) || ( pData == NULL ) ) {
    return NULL;
  }
  LPCTSTR pEndString = pData + cch;
  LPCTSTR pEndSearch = pData + ( cch - cchFindWhat );
  LPCTSTR pEndFindWhat = pszFindWhat + cchFindWhat;
  LPCTSTR pPrev = ( ( pStart == pData ) ? NULL : GetPrevCharPos( pData, pStart ) );
  while ( pData <= pEndSearch ) {
    LPCTSTR s1 = pData;
    LPCTSTR s2 = pszFindWhat;
    WCHAR c1;
    WCHAR c2;
    while ( ( s1 < pEndString ) && ( s2 < pEndFindWhat ) ) {
      if ( PFX_EC_IS_LEAD_BYTE( *s1 )
           && ( ( s1 + PFX_EC_MAX_MB_LEN ) <= pEndString ) ) {
        c1 = ( WCHAR ) * ( ( LPWSTR ) s1 );
        s1 += PFX_EC_MAX_MB_LEN;
      } else {
        c1 = ( WCHAR ) * s1;
        s1++;
        //  ������ת��������Сд�ַ�������ȫ��������
        //  ����ȫ����ĸ�ַ�
        if ( ( c1 <= 'Z' ) && ( c1 >= 'A' ) ) {
          c1 += ( 'a' - 'A' );
        }
      }
      if ( PFX_EC_IS_LEAD_BYTE( *s2 )
           && ( ( s2 + PFX_EC_MAX_MB_LEN ) <= pEndFindWhat ) ) {
        c2 = ( WCHAR ) * ( ( LPWSTR ) s2 );
        s2 += PFX_EC_MAX_MB_LEN;
      } else {
        c2 = ( WCHAR ) * s2;
        s2++;
      }
      if ( c1 != c2 ) {
        goto ContinueCompareNextChar;
      }
    }
    if ( ( s2 == pEndFindWhat )           //      OK��ƥ�䣿
         //  ���Ҫ���ҵ��ַ�����һ���ַ��ǵ��ʶ������ǰ��
         //  ���Ķ����ǰһ���ַ��ǵ��ʶ��������������ǰ����
         && ( ( pPrev == NULL )           //      �����׿�ʼ������
              || IsWhitespace( *pPrev )
              || IsDelimiter( pPrev, pEnd )
              || IsWhitespace( *pszFindWhat )
              || IsDelimiter( pszFindWhat, pEndFindWhat ) )
         //  ���Ҫ���ҵ��ַ��������ַ��ǵ��ʶ������ǰ��
         //  ���Ķ���ĺ�һ���ַ��ǵ��ʶ�������������к󲿷�
         && ( ( s1 >= pEnd )           //      ������ĩ��
              || IsWhitespace( *s1 )
              || IsDelimiter( s1, pEnd )
              || IsWhitespace( c2 )
              || IsDelimiter( c2 ) ) ) {
      return pData;
    }
ContinueCompareNextChar:
    // ���µ�ǰ�ַ�λ�ã����ҵ�ƥ���ַ���ʱ��Ҫ�жϵ�ǰ�ַ��Ƿ�
    // �ǵ��ʶ����
    pPrev = pData;
    pData += ( PFX_EC_IS_LEAD_BYTE( *pData ) ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// rFindReplace.ptFrom - �������λ��
// rFindReplace.ptTo   - �������λ��
BOOL CPfxEditView::SearchFindWholeWordDown( CPfxEditViewFindReplace& rFindReplace ) {
  LPCTSTR pDataStart;
  LPCTSTR pDataEnd;
  LPCTSTR pSearchStart;
  LPCTSTR pSearchEnd;
  LPCTSTR pFind;
  const CPfxEditLineInfo* pli;
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  if ( rFindReplace.bMatchCase ) {
    // ������ҷ�Χ��ͬһ����
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptFrom.x;
      pSearchEnd = pDataStart + ptTo.x;
      pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
    }
    // ���ҷ�Χ����ͬһ����
    else {
      // ������һ��
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptFrom.x;
      pSearchEnd = pDataStart + pli->nActLen;
      pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
      // �����м���
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetNextMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y >= ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pDataStart = pli->pData;
        pFind = SearchFindWholeWordStringCase( pDataStart, ( pDataStart + pli->nActLen ), pDataStart, pli->nActLen, pszFindWhat, cchFindWhat );
      }
      // ���������
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pDataStart = pli->pData;
        pFind = SearchFindWholeWordStringCase( pDataStart, ( pDataStart + pli->nActLen ), pDataStart, ptTo.x, pszFindWhat, cchFindWhat );
      }
    }
  }
  // ���Դ�Сд���²���
  else {
    // ������ҷ�Χ��ͬһ����
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptFrom.x;
      pSearchEnd = pDataStart + ptTo.x;
      pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
    }
    // ���ҷ�Χ����ͬһ����
    else {
      // ������һ��
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptFrom.x;
      pSearchEnd = pDataStart + pli->nActLen;
      pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
      // �����м���
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetNextMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y >= ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pDataStart = pli->pData;
        pFind = SearchFindWholeWordStringNoCase( pDataStart, ( pDataStart + pli->nActLen ), pDataStart, pli->nActLen, pszFindWhat, cchFindWhat );
      }
      // ���������
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pDataStart = pli->pData;
        pFind = SearchFindWholeWordStringNoCase( pDataStart, ( pDataStart + pli->nActLen ), pDataStart, ptTo.x, pszFindWhat, cchFindWhat );
      }
    }
  }
  if ( pFind != NULL ) {
    rFindReplace.ptFrom.y = ptFrom.y;
    rFindReplace.ptFrom.x = pFind - m_pdb->TextGetLineData( ptFrom.y );
    rFindReplace.ptTo.y = ptFrom.y;
    rFindReplace.ptTo.x = rFindReplace.ptFrom.x + cchFindWhat;
  }
  return ( pFind != NULL );
}
// rFindReplace.ptFrom - �������λ��
// rFindReplace.ptTo   - �������λ��
BOOL CPfxEditView::SearchFindWholeWordUp( CPfxEditViewFindReplace& rFindReplace ) {
  LPCTSTR pDataStart;
  LPCTSTR pDataEnd;
  LPCTSTR pSearchStart;
  LPCTSTR pSearchEnd;
  LPCTSTR pFind;
  const CPfxEditLineInfo* pli;
  CPoint ptFrom = rFindReplace.ptFrom;
  CPoint ptTo = rFindReplace.ptTo;
  LONG cchFindWhat = rFindReplace.cchFindWhat;
  LPCTSTR pszFindWhat = rFindReplace.pszFindWhat;
  // ������������ʱ�Ӻ������ָ��ǳ��������Բ�ȡ��һ�еĿ�ͷ��ʼ���ң�
  // Ȼ��Ƚ�λ�ã��������λ���Ѿ���������λ������ҽ���
  LPCTSTR pLastMatch = NULL;
  if ( rFindReplace.bMatchCase ) {
    // ������ҷ�Χ��ͬһ����
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptTo.x;
      pSearchEnd = pDataStart + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pSearchStart = pFind + cchFindWhat;
      }
    }
    // ���ҷ�Χ�������Ķ�����
    else {
      // ������һ��
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart;
      pSearchEnd = pDataStart + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pSearchStart = pFind + cchFindWhat;
      }
      // �����м���
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetPrevMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y < ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pDataStart = pli->pData;
        pDataEnd = pDataStart + pli->nActLen;
        pSearchStart = pDataStart;
        pSearchEnd = pDataStart + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pSearchStart = pFind + cchFindWhat;
        }
      }
      // ���������
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pDataStart = pli->pData;
        pDataEnd = pDataStart + pli->nActLen;
        pSearchStart = pDataStart + ptTo.x;
        pSearchEnd = pDataStart + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindWholeWordStringCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pSearchStart = pFind + cchFindWhat;
        }
      }
    }
  }
  // ���Դ�Сд���ϲ���
  else {
    // ������ҷ�Χ��ͬһ����
    if ( ptFrom.y == ptTo.y ) {
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart + ptTo.x;
      pSearchEnd = pDataStart + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pSearchStart = pFind + cchFindWhat;
      }
    }
    // ���ҷ�Χ�������Ķ�����
    else {
      // ������һ��
      pli = m_pdb->TextGetLineAt( ptFrom.y );
      pDataStart = pli->pData;
      pDataEnd = pDataStart + pli->nActLen;
      pSearchStart = pDataStart;
      pSearchEnd = pDataStart + ptFrom.x;
      for ( ;; ) {
        pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
        if ( ( pFind == NULL )
             || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
          pFind = pLastMatch;
          break;
        }
        pLastMatch = pFind;
        pSearchStart = pFind + cchFindWhat;
      }
      // �����м���
      while ( pFind == NULL ) {
        ptFrom.y = m_pdb->TextGetPrevMainLine( ptFrom.y );
        if ( ( ptFrom.y == PFX_EC_INVALID_LINE )
             || ( ptFrom.y < ptTo.y ) ) {
          break;
        }
        pli = m_pdb->TextGetLineAt( ptFrom.y );
        pDataStart = pli->pData;
        pDataEnd = pDataStart + pli->nActLen;
        pSearchStart = pDataStart;
        pSearchEnd = pDataStart + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pSearchStart = pFind + cchFindWhat;
        }
      }
      // ���������
      if ( pFind == NULL ) {
        ptFrom.y = ptTo.y;
        pli = m_pdb->TextGetLineAt( ptTo.y );
        pDataStart = pli->pData;
        pDataEnd = pDataStart + pli->nActLen;
        pSearchStart = pDataStart + ptTo.x;
        pSearchEnd = pDataStart + pli->nActLen;
        for ( ;; ) {
          pFind = SearchFindWholeWordStringNoCase( pDataStart, pDataEnd, pSearchStart, ( pSearchEnd - pSearchStart ), pszFindWhat, cchFindWhat );
          if ( ( pFind == NULL )
               || ( ( pFind + cchFindWhat ) > pSearchEnd ) ) {
            pFind = pLastMatch;
            break;
          }
          pLastMatch = pFind;
          pSearchStart = pFind + cchFindWhat;
        }
      }
    }
  }
  if ( pFind != NULL ) {
    rFindReplace.ptFrom.y = ptFrom.y;
    rFindReplace.ptFrom.x = pFind - m_pdb->TextGetLineData( ptFrom.y );
    rFindReplace.ptTo.y = ptFrom.y;
    rFindReplace.ptTo.x = rFindReplace.ptFrom.x + cchFindWhat;
  }
  return ( pFind != NULL );
}
// �﷨����...
// BOOL ParserIsMarkerString(LPCTSTR pStart,LPCTSTR pCur,LPCTSTR pEnd,LPCTSTR pString,LONG nStrLen,BOOL bCase) const
// �����һ���д�pCur��ʼ��pEnd�����Ƿ����pString���ַ�����ƥ������
// pCur��ʼ
// LPCTSTR pStart  - һ�����ݵĿ�ʼ��ַ
// LPCTSTR pCur    - �������ַ��ʼ�Ƚ�
// LPCTSTR pEnd    - һ�����ݵĽ�����ַ
// LPCTSTR pString - Ҫ���ҵ�Ŀ���ַ���
// LONG nStrLen    - ��Ŀ���ַ����ĳ���
// BOOL bCase      - �Ƿ�ƥ���Сд
// ���ƥ���򷵻�TRUE�����򷵻�FALSE
BOOL CPfxEditView::ParserIsMarkerString( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, LPCTSTR pString, LONG nStrLen, BOOL bCase ) const {
  ASSERT( pStart != NULL );
  ASSERT( pCur != NULL );
  ASSERT( pEnd != NULL );
  ASSERT( nStrLen > 0 );
  ASSERT( pString != NULL );
  if ( ( pEnd - pCur ) < nStrLen ) {
    return false;
  }
  // �����ǰ����ַ��Ƿ���ת���ַ�
  LONG nEscapes = 0;
  for ( LPCTSTR p1 = pCur; p1 > pStart; ) {
    LPCTSTR p2 = GetPrevCharPos( p1, pStart );
    if ( !ParserIsEscape( ( ( ( p1 - p2 ) >= sizeof( WCHAR ) ) ? *( LPCWSTR ) p2 : ( WCHAR ) * p2 ) ) ) {
      break;
    }
    p1 = p2;
    nEscapes++;
  }
  // �����Ҫ�����ַ�ǰ����������ת���ַ��������ת���ַ���Ч��Ҫ��
  // ����ַ����϶���ƥ��
  if ( ( nEscapes % 2 ) != 0 ) {
    return false;
  }
  // ������ж��Ѿ�����(pEnd - pCur)һ�����ܱ�nStrLenС
  return ( ( bCase ? _tcsncmp : _tcsnicmp ) ( pCur, pString, nStrLen ) == 0 );
}
// BOOL ParserIsMarkerStart(LPCTSTR pStart,LPCTSTR pCur,LPCTSTR pEnd,CPfxEditMarkerPair* pMarker) const
BOOL CPfxEditView::ParserIsMarkerStart( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, CPfxEditMarkerPair* pMarker ) const {
  if ( ( pStart == NULL )
       || ( pMarker == NULL )
       || ( pMarker->pszOn == NULL )
       || ( m_pti == NULL ) ) {
    return false;
  }
  return ParserIsMarkerString( pStart, pCur, pEnd, pMarker->pszOn, pMarker->cchOn, m_pti->IsMatchCase() );
}
// BOOL ParserIsMarkerClose(LPCTSTR pStart,LPCTSTR pCur,LPCTSTR pEnd,CPfxEditMarkerPair* pMarker) const
BOOL CPfxEditView::ParserIsMarkerClose( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, CPfxEditMarkerPair* pMarker ) const {
  if ( ( pStart == NULL )
       || ( pMarker == NULL )
       || ( pMarker->pszOff == NULL )
       || ( m_pti == NULL ) ) {
    return false;
  }
  return ParserIsMarkerString( pStart, pCur, pEnd, pMarker->pszOff, pMarker->cchOff, m_pti->IsMatchCase() );
}
// BOOL ParserIsEscapedLine(LPCTSTR pStart, LPCTSTR pEnd) const
// �жϸ������ַ��������һ���ַ��Ƿ���ת���ַ�
BOOL CPfxEditView::ParserIsEscapedLine( LPCTSTR pStart, LPCTSTR pEnd ) const {
  if ( ( pStart == NULL ) || ( pStart >= pEnd ) ) {
    return false;
  }
  // �����ǰ����ַ��Ƿ���ת���ַ�
  LONG nEscapes = 0;
  for ( LPCTSTR p1 = pEnd; p1 > pStart; ) {
    LPCTSTR p2 = GetPrevCharPos( p1, pStart );
    if ( !ParserIsEscape( ( ( ( p1 - p2 ) >= sizeof( WCHAR ) ) ? *( LPCWSTR ) p2 : ( WCHAR ) * p2 ) ) ) {
      break;
    }
    p1 = p2;
    nEscapes++;
  }
  // �������ĩ��ʼ������������ת���ַ��������ת���ַ���Ч����ĩ��ת
  // ���ַ�
  return ( ( nEscapes % 2 ) != 0 );
}
// BOOL ParserIsInsideBlock(CPoint pt) const
// �ж�һ����λ���Ƿ��ǿ��һ���֣�����ָ��ע�͡���ע�͡��ַ�����
// CPoint pt - �������λ��
// ����ǿ��һ���ַ���TRUE�����򷵻�FALSE
BOOL CPfxEditView::ParserIsInsideBlock( CPoint pt ) const {
  ASSERT( IsDrawHilighted() );
  if ( !m_pdb->TextIsValidChar( pt.y, pt.x ) ) {
    return false;
  }
  if ( pt.y > m_pdb->m_nLineLastParsed ) {
    ParserPreParse( pt.y, m_pdb->TextGetLineUpperBound(), m_pdb->m_nLineLastParsedButCanceled, m_pdb->m_nLineLastParsed, m_pdb->TextGetLineAt( 0 ) );
  }
  // ���ָ��������ע������Ҫ��һ���ж�
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nLen;
  const LPCTSTR pTargetPos = pStart + pt.x;
  LPCTSTR pCur = pStart;
  DWORD dwCookie = pli->dwFlags;
  CPfxEditMarkerPair* pMarker;
  if ( ( dwCookie & ( PFX_EC_LF_COOKIE_LINE_COMMENT1 | PFX_EC_LF_COOKIE_LINE_COMMENT2 ) ) != 0 ) {
    return true;
  } else if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT1 ) {
    pMarker = m_pti->GetMarkerPair( mpBlockComment1 );
  } else if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT2 ) {
    pMarker = m_pti->GetMarkerPair( mpBlockComment2 );
  } else if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING1 ) {
    pMarker = m_pti->GetMarkerPair( mpQuotedString1 );
  } else if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING2 ) {
    pMarker = m_pti->GetMarkerPair( mpQuotedString2 );
  } else {
    pMarker = NULL;
  }
  if ( pMarker != NULL ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, pMarker );
    if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
      return true;
    }
  }
  // ����������һ�������µ��ַ�
  // ÿ�η���һ���ַ���ʱ�򽫸��ַ�������ı�ʶ���ĵ�һ���ַ��Ƚϣ���
  // �����Ⱦ�����Ҫ�����Ƚϣ��������Լӿ�����ٶ�
  CPfxEditViewMarkerChar mc;
  ParserGetFirstCharOfMarkerStart( mc );
  while ( ( pCur < pEnd ) && ( pCur <= pTargetPos ) ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte
                   || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    if ( ( ( wc == mc.m_cLowerLineComment1 ) || ( wc == mc.m_cUpperLineComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment1 ) ) ) {
      return true;
    }
    if ( ( ( wc == mc.m_cLowerLineComment2 ) || ( wc == mc.m_cUpperLineComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment2 ) ) ) {
      return true;
    }
    if ( ( ( wc == mc.m_cLowerBlockComment1 ) || ( wc == mc.m_cUpperBlockComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment1 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment1 ) );
      if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
        return true;
      } else {
        continue;
      }
    }
    if ( ( ( wc == mc.m_cLowerBlockComment2 ) || ( wc == mc.m_cUpperBlockComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment2 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment2 ) );
      if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
        return true;
      } else {
        continue;
      }
    }
    if ( ( ( wc == mc.m_cLowerQuotedString1 ) || ( wc == mc.m_cUpperQuotedString1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString1 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString1 ) );
      if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
        return true;
      } else {
        continue;
      }
    }
    if ( ( ( wc == mc.m_cLowerQuotedString2 ) || ( wc == mc.m_cUpperQuotedString2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString2 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString2 ) );
      if ( ( pCur == NULL ) || ( pCur >= pTargetPos ) ) {
        return true;
      } else {
        continue;
      }
    }
    // ָ����һ���ַ�
    pCur += ( bLeadByte ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return false;
}
// void ParserGetFirstCharOfMarkerStart(CPfxEditViewMarkerChar& mc) const
void CPfxEditView::ParserGetFirstCharOfMarkerStart( CPfxEditViewMarkerChar& mc ) const {
  if ( m_pti == NULL ) {
    return ;
  }
  CPfxEditMarkerPair* pMarker;
  pMarker = m_pti->GetMarkerPair( mpQuotedString1 );
  mc.m_cLowerQuotedString1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperQuotedString1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpQuotedString2 );
  mc.m_cLowerQuotedString2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperQuotedString2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpLineComment1 );
  mc.m_cLowerLineComment1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperLineComment1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpLineComment2 );
  mc.m_cLowerLineComment2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperLineComment2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpBlockComment1 );
  mc.m_cLowerBlockComment1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperBlockComment1 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
  pMarker = m_pti->GetMarkerPair( mpBlockComment2 );
  mc.m_cLowerBlockComment2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wLowerOn );
  mc.m_cUpperBlockComment2 = ( ( pMarker == NULL ) ? ( WCHAR ) 0 : pMarker->wUpperOn );
}
// LPCTSTR ParserGetMarkerCloseEndPosition(LPCTSTR pStart,LPCTSTR pCur,LPCTSTR pEnd,BOOL bCurrentAddressPointToMarkerStart,CPfxEditMarkerPair* pMarker) const
// ���ҿ鶨�������־
// ���ҵ��ͷ��ظý�����־����һ���ַ��ĵ�ַ�����򷵻�NULL
LPCTSTR CPfxEditView::ParserGetMarkerCloseEndPosition( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, BOOL bCurrentAddressPointToMarkerStart, CPfxEditMarkerPair* pMarker ) const {
  if ( ( pMarker == NULL ) || ( pMarker->pszOff == NULL ) ) {
    return pStart;
  }
  // �����ǰָ��ָ��on����Ҫ�����on��
  if ( bCurrentAddressPointToMarkerStart ) {
    pCur += pMarker->cchOn;
  }
  WCHAR cLower = pMarker->wLowerOff;
  WCHAR cUpper = pMarker->wUpperOff;
  while ( pCur < pEnd ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte
                   || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    if ( ( ( wc == cLower ) || ( wc == cUpper ) )
         && ParserIsMarkerClose( pStart, pCur, pEnd, pMarker ) ) {
      return ( pCur + pMarker->cchOff );
    }
    pCur += ( bLeadByte ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return NULL;
}
// DWORD ParserParseLine(LPCTSTR pData, LONG nLen, DWORD dwCookie) const
// ����һ�е����ݣ����ط������
DWORD CPfxEditView::ParserParseLine( LPCTSTR pData, LONG nLen, DWORD dwCookie ) const {
  const LPCTSTR pStart = pData;
  const LPCTSTR pEnd = pStart + nLen;
  LPCTSTR pCur = pStart;
  // ��һ������һ�е�PFX_EC_LF_COOKIE_LINE_COMMENT1�������������һ�е����һ
  // ���ַ���ת���ַ�����ô��һ��Ҳ��PFX_EC_LF_COOKIE_LINE_COMMENT1������
  if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT1 ) {
    return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_LINE_COMMENT1 : PFX_EC_LF_COOKIE_NONE );
  }
  // ��һ������һ�е�PFX_EC_LF_COOKIE_LINE_COMMENT2�������������һ�е����һ
  // ���ַ���ת���ַ�����ô��һ��Ҳ��PFX_EC_LF_COOKIE_LINE_COMMENT2������
  else if ( dwCookie & PFX_EC_LF_COOKIE_LINE_COMMENT2 ) {
    return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_LINE_COMMENT2 : PFX_EC_LF_COOKIE_NONE );
  }
  // ��һ����PFX_EC_LF_COOKIE_BLOCK_COMMENT1�������������������һ�����ҵ���ע
  // �ͽ�����ʶ������һ��Ҳ��PFX_EC_LF_COOKIE_BLOCK_COMMENT1������������
  // PFX_EC_LF_COOKIE_BLOCK_COMMENT1��������һ������ֹ
  else if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT1 ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, m_pti->GetMarkerPair( mpBlockComment1 ) );
    // ����ҵ���ȡ��ԭ���ı�ʶ��������Ȼʹ�þ��б�ʶ
    if ( pCur == NULL ) {
      return PFX_EC_LF_COOKIE_BLOCK_COMMENT1;
    }
  }
  // ��һ����PFX_EC_LF_COOKIE_BLOCK_COMMENT2�������������������һ�����ҵ���ע
  // �ͽ�����ʶ������һ��Ҳ��PFX_EC_LF_COOKIE_BLOCK_COMMENT2������������
  // PFX_EC_LF_COOKIE_BLOCK_COMMENT2��������һ������ֹ
  else if ( dwCookie & PFX_EC_LF_COOKIE_BLOCK_COMMENT2 ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, m_pti->GetMarkerPair( mpBlockComment2 ) );
    // ����ҵ���ȡ��ԭ���ı�ʶ��������Ȼʹ�þ��б�ʶ
    if ( pCur == NULL ) {
      return PFX_EC_LF_COOKIE_BLOCK_COMMENT2;
    }
  }
  // ��һ������һ�е�PFX_EC_LF_COOKIE_QUOTED_STRING1�������������������������
  // ��PFX_EC_LF_COOKIE_QUOTED_STRING1�Ľ�����ʶ���������е����һ���ַ���ת��
  // �ַ�����ô��һ��Ҳ��PFX_EC_LF_COOKIE_QUOTED_STRING1������
  else if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING1 ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, m_pti->GetMarkerPair( mpQuotedString1 ) );
    if ( pCur == NULL ) {
      return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_QUOTED_STRING1 : PFX_EC_LF_COOKIE_NONE );
    }
  } else if ( dwCookie & PFX_EC_LF_COOKIE_QUOTED_STRING2 ) {
    pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, false, m_pti->GetMarkerPair( mpQuotedString2 ) );
    // ����ҵ���ȡ��ԭ���ı�ʶ��������Ȼʹ�þ��б�ʶ
    if ( pCur == NULL ) {
      return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_QUOTED_STRING2 : PFX_EC_LF_COOKIE_NONE );
    }
  }
  // ����������һ�������µ��ַ�
  // ÿ�η���һ���ַ���ʱ�򽫸��ַ�������ı�ʶ���ĵ�һ���ַ��Ƚϣ���
  // �����Ⱦ�����Ҫ�����Ƚϣ��������Լӿ�����ٶ�
  CPfxEditViewMarkerChar mc;
  ParserGetFirstCharOfMarkerStart( mc );
  while ( pCur < pEnd ) {
    BOOL bLeadByte = PFX_EC_IS_LEAD_BYTE( *pCur );
    WCHAR wc = ( ( !bLeadByte || ( ( pCur + PFX_EC_MAX_MB_LEN ) > pEnd ) ) ? ( ( WCHAR ) * pCur ) : ( *( LPWSTR ) pCur ) );
    if ( ( ( wc == mc.m_cLowerLineComment1 ) || ( wc == mc.m_cUpperLineComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment1 ) ) ) {
      return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_LINE_COMMENT1 : PFX_EC_LF_COOKIE_NONE );
    }
    if ( ( ( wc == mc.m_cLowerLineComment2 ) || ( wc == mc.m_cUpperLineComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpLineComment2 ) ) ) {
      return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_LINE_COMMENT2 : PFX_EC_LF_COOKIE_NONE );
    }
    if ( ( ( wc == mc.m_cLowerBlockComment1 ) || ( wc == mc.m_cUpperBlockComment1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment1 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment1 ) );
      // ���û���ҵ���������ַ������ڿ�ע��
      if ( pCur != NULL ) {
        continue;
      } else {
        return PFX_EC_LF_COOKIE_BLOCK_COMMENT1;
      }
    }
    if ( ( ( wc == mc.m_cLowerBlockComment2 ) || ( wc == mc.m_cUpperBlockComment2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpBlockComment2 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpBlockComment2 ) );
      // ���û���ҵ���������ַ������ڿ�ע��
      if ( pCur != NULL ) {
        continue;
      } else {
        return PFX_EC_LF_COOKIE_BLOCK_COMMENT2;
      }
    }
    if ( ( ( wc == mc.m_cLowerQuotedString1 ) || ( wc == mc.m_cUpperQuotedString1 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString1 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString1 ) );
      // ���û���ҵ���������ַ������ڳ����ַ�1
      if ( pCur != NULL ) {
        continue;
      } else {
        return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_QUOTED_STRING1 : PFX_EC_LF_COOKIE_NONE );
      }
    }
    if ( ( ( wc == mc.m_cLowerQuotedString2 ) || ( wc == mc.m_cUpperQuotedString2 ) )
         && ParserIsMarkerStart( pStart, pCur, pEnd, m_pti->GetMarkerPair( mpQuotedString2 ) ) ) {
      pCur = ParserGetMarkerCloseEndPosition( pStart, pCur, pEnd, true, m_pti->GetMarkerPair( mpQuotedString2 ) );
      // ���û���ҵ���������ַ������ڳ����ַ�1
      if ( pCur != NULL ) {
        continue;
      } else {
        return ( ( ( ( dwCookie & PFX_EC_LF_HARDRETURN ) == 0 ) || ParserIsEscapedLine( pStart, pEnd ) ) ? PFX_EC_LF_COOKIE_QUOTED_STRING2 : PFX_EC_LF_COOKIE_NONE );
      }
    }
    // ָ����һ���ַ�
    pCur += ( bLeadByte ? PFX_EC_MAX_MB_LEN : 1 );
  }
  return PFX_EC_LF_COOKIE_NONE;
}
// BOOL ParserPreParse(LONG nLineToEndParse,LONG nLineUpperBound,LONG& rnLineLastParsed,CPfxEditLineInfo* pliHead)
// ��rnLineLastParsed��ʼ����COOKIE��nLineToEndParse��
// LONG nLineToEndParse     - ��������һ����ֹ
// LONG nLineUpperBound     - pliHead�������ڵ�������
// LONG& rnLineLastParsed   - ��һ��������������
// CPfxEditLineInfo* pliHead - ����Ϣ�������׵�ַ
// ����TRUE��ʾҪˢ�� rnLineLastParsed����ʱ���м�����Ĳ��֣�����FALSE
// ��ʾ����Ҫ��һ������
BOOL CPfxEditView::ParserPreParse( LONG nLineToEndParse, LONG nLineUpperBound, LONG nLineLastParsedButCanceled, LONG& rnLineLastParsed, CPfxEditLineInfo* pliHead ) const {
  if ( ( pliHead == NULL ) || ( nLineUpperBound < 0 ) ) {
    return false;
  }
  // ��鲢ָ�����Ƿ�Ϸ���������Ϸ����������Ϸ���Χ
  nLineToEndParse = max( nLineToEndParse, 0 );
  LONG nLineEndParsing = min( nLineToEndParse, nLineUpperBound );
  LONG nLineLastParsed = rnLineLastParsed;
  // ���ԭ������Ϣ��Ч�����´�ͷ����
  if ( ( nLineLastParsed <= 0 ) || ( nLineLastParsed > nLineUpperBound ) ) {
    // ��һ�����ǵ���PFX_EC_LF_COOKIE_NONE
    pliHead->dwFlags &= ~PFX_EC_LF_COOKIE_ALL;
    nLineLastParsed = -1;
  }
  // ���δ���һ�������������е���һ�п�ʼ����
  LONG nStartParsingLine = nLineLastParsed + 1;
  // ���ָ�����Ѿ�����������ʼ�����������ĵ�������оͲ���Ҫ����
  if ( ( nStartParsingLine >= nLineEndParsing )
       || ( nStartParsingLine >= nLineUpperBound ) ) {
    return false;
  }
  // ĳ�еķ������������������һ���У����һ�в���Ҫ����
  CPfxEditLineInfo* pli = pliHead + nStartParsingLine;
  CPfxEditLineInfo* pliEnd = pliHead + nLineEndParsing;
  CPfxEditLineInfo* pliCanceled = pliHead + nLineLastParsedButCanceled;
  DWORD dwCookie = pli->dwFlags;
  BOOL bRefreshBelow = false;
  for ( ;; ) {
    dwCookie = ParserParseLine( pli->pData, pli->nLen, dwCookie );
    if ( ++pli > pliEnd ) {
      break;
    }
    nLineLastParsed++;
    // ���ԭ���п��ǣ���ע�͡���ע�͡��ַ�����ʶ�ȣ�����û�У�
    // ��ԭ��û�����ڱ���У���Ҫˢ���������������
    if ( ( pliCanceled >= pli )
         && ( ( pli->dwFlags & PFX_EC_LF_COOKIE_ALL ) != ( dwCookie & PFX_EC_LF_COOKIE_ALL ) ) ) {
      bRefreshBelow = true;
    }
    pli->dwFlags &= ~PFX_EC_LF_COOKIE_ALL;
    pli->dwFlags |= dwCookie;
    dwCookie |= pli->dwFlags;
  }
  rnLineLastParsed = nLineLastParsed;
  return bRefreshBelow;
}
// BOOL MatchBraceGetString(CPoint pt,BOOL bPrevStr,CPfxEditViewBraceHint& hint)
// ��ȡ��ǰ�༭λ�����ڵĵ���
// BOOL bPrevStr  - �Ƿ��ȡ��һ����
BOOL CPfxEditView::MatchBraceGetString( CPoint pt, BOOL bPrevStr, CPfxEditViewBraceHint& hint ) {
  if ( !m_pdb->TextIsValidChar( pt.y, pt.x ) ) {
    return false;
  }
  // ����Ե�����λ��ת��������Ե�����λ�ã�������Ϊ�������Զ�����ģ
  // ʽʱ���ܻὫһ����ǿ�жϿ�����ʹ�����з�ʽ���Ա�������������
  m_pdb->TextSubLineToMainLine( pt );
  // ��ȡ���ݵ�ַ
  const CPfxEditLineInfo* pli = m_pdb->TextGetLineAt( pt.y );
  const LPCTSTR pStart = pli->pData;
  const LPCTSTR pEnd = pStart + pli->nActLen;
  LPCTSTR pCur = pStart + min( pli->nActLen, pt.x );
  if ( bPrevStr ) {
    pCur = GetPrevCharPos( pCur, pStart );
  }
  // �յ��л��ߵ�ǰλ��ָ��һ�հ��ַ�
  if ( ( pCur >= pEnd ) || IsWhitespace( *pCur ) ) {
    return false;
  }
  // A) !bPrevStr
  // ��ǰλ�ÿ��ܻ�����������֮һ��
  //                              |
  // 1 ��ǰλ��ָ��һ���ʵ��м䣬���磺 v
  //                             "abcdefghijkl"
  // ������������Ϻ����²��Ҷ����������ȡ���������֮��Ĳ���
  //                              |
  // 2 ��ǰλ��ָ��һ���ʶ���������磺 v
  //                             "(abdefghijk)"
  // �������ֻ��ȡ���ʶ����
  // B) bPrevStr
  // ��ǰλ�ÿ��ܻ�����������֮һ��
  //                                         |
  // 1 ��ǰλ��ָ��һ���ʵ��м䣬���磺            v
  //                             "abcdefghijklmn"
  // ������������Ϻ����²��Ҷ����������ȡ���������֮��Ĳ���
  //                                          |
  // 2 ��ǰλ�õ���һ���ַ���һ���ʶ���������磺   v
  //                             "(abdefghijk)lmn"
  // �������ֻ��ȡ���ʶ����
  LONG nStrLen;
  LPCTSTR pString;
  if ( IsDelimiter( pCur, pEnd ) ) {
    pString = pCur;
    nStrLen = ( ( PFX_EC_IS_LEAD_BYTE( *pCur )
                  && ( ( pCur + PFX_EC_MAX_MB_LEN ) <= pEnd ) ) ? PFX_EC_MAX_MB_LEN : 1 );
  } else {
    pString = GetPrevDelimiterPos( pCur, pStart );
    nStrLen = GetNextDelimiterPos( pCur, pEnd ) - pString;
  }
  // ���ǵ���Щ�п��ܻ�ܳ�����CString�Ļ�����һ������512���ʹ��new
  // ����ȡ������������ʹ�þֲ�����أ����������������ж�Ҫ��ȡ���ַ�
  // ���ĳ����Ƿ������֪�����BRACE�ַ����ĳ��ȣ�����ǾͿ϶�����
  // ƥ�䣬���Է���FALSE
  if ( nStrLen > hint.m_nMaxBraceString ) {
    return false;
  }
  // ���������λ��ת�����������λ��
  hint.m_ptStrStart.y = pt.y;
  hint.m_ptStrStart.x = min( pli->nActLen, ( pString - pStart ) );
  hint.m_ptStrEnd.y = pt.y;
  hint.m_ptStrEnd.x = min( pli->nActLen, ( hint.m_ptStrStart.x + nStrLen ) );
  m_pdb->TextMainLineToSubLine( hint.m_ptStrStart );
  m_pdb->TextMainLineToSubLine( hint.m_ptStrEnd );
  // �ɹ���ȡһ���ʵ�λ�ú�Ҫ���õ������ڵ�λ�ã�����õ���λ�ڿ�
  // ע�͡���ע�͡��������ַ����ķ�Χ�ھ�����Ч
  if ( !ParserIsInsideBlock( hint.m_ptStrStart ) ) {
    LPTSTR lp = hint.m_strBrace.GetBufferSetLength( nStrLen );
    if ( lp != NULL ) {
      memcpy( lp, pString, nStrLen * sizeof( TCHAR ) );
    }
  }
  return !hint.m_strBrace.IsEmpty();
}
// BOOL MatchBraceGetHint(CPfxEditViewBraceHint& hint)
BOOL CPfxEditView::MatchBraceGetHint( CPfxEditViewBraceHint& hint ) {
  if ( ( m_pti == NULL ) || ( m_pti->GetMarkerPair( mpBraceStrings ) == NULL ) ) {
    return false;
  }
  LONG nMaxBraceLen = 0;
  BOOL arrBOOL[] = { false, true};
  BOOL bResult = false;
  BOOL bMatchCase = m_pti->IsMatchCase();
  // ������󳤶ȵ�BRACE�ַ�������ȡ���ĳ���
  for ( CPfxEditMarkerPair * pMarker = m_pti->GetMarkerPair( mpBraceStrings ); pMarker != NULL; pMarker = pMarker->pNext ) {
    nMaxBraceLen = max( nMaxBraceLen, pMarker->cchOn );
    nMaxBraceLen = max( nMaxBraceLen, pMarker->cchOff );
  }
  hint.m_nMaxBraceString = nMaxBraceLen;
  // ���Ȼ�ȡ��ǰλ�����ڵĵ��ʽ��бȽϣ������ƥ���ٻ�ȡ��ǰλ�õ���
  // һ�����ʽ��бȽ�
  for ( LONG i = 0; !bResult && ( i < PFX_EC_SIZEOF( arrBOOL ) ); i++ ) {
    if ( !MatchBraceGetString( m_ptEditPos, arrBOOL[ i ], hint ) ) {
      continue;
    }
    // �ɹ���ȡһ���ʺ�ͱ���������Ա�ʶ������������ON��OFF��
    // ʶ���Ƚϣ����ƥ���ֹͣ���Ҳ���
    for ( pMarker = m_pti->GetMarkerPair( mpBraceStrings ); pMarker != NULL; pMarker = pMarker->pNext ) {
      if ( ( pMarker->pszOn != NULL )
           && ( ( bMatchCase ? _tcscmp : _tcsicmp ) ( hint.m_strBrace, pMarker->pszOn ) == 0 ) ) {
        hint.m_strOn = pMarker->pszOn;
        hint.m_strOff = pMarker->pszOff;
        hint.m_bDirectDown = true;
        //  ���²�������Խ����ǰ����ַ���
        hint.m_ptFindStart = hint.m_ptStrEnd;
        bResult = true;
        break;
      }
      if ( ( pMarker->pszOff != NULL ) && ( ( bMatchCase ? _tcscmp : _tcsicmp ) ( hint.m_strBrace, pMarker->pszOff ) == 0 ) ) {
        hint.m_strOn = pMarker->pszOff;
        hint.m_strOff = pMarker->pszOn;
        hint.m_bDirectDown = false;
        //  ���ϲ�������Խ����ǰ����ַ���
        hint.m_ptFindStart = hint.m_ptStrStart;
        bResult = true;
        break;
      }
    }
  }
  if ( bResult ) {
    hint.m_bMatchCase = bMatchCase;
    if ( !hint.m_bMatchCase )           //      ת����Сд
    {
      _tcslwr( hint.m_strOn.GetBufferSetLength( hint.m_strOn.GetLength() ) );
      _tcslwr( hint.m_strOff.GetBufferSetLength( hint.m_strOff.GetLength() ) );
    }
    // �������λ��ת���ɾ�������λ��
    m_pdb->TextSubLineToMainLine( hint.m_ptFindStart );
  }
  return bResult;
}
// BOOL MatchBraceSearchUp(CPfxEditViewBraceHint& hint, CPoint& rpt)
// CPoint& rpt - �ҵ���ƥ�������������λ��
BOOL CPfxEditView::MatchBraceSearchUp( CPfxEditViewBraceHint& hint, CPoint& rpt ) {
  // �����㷨���£�
  // 1. �ӵ�ǰ��POINT_START�����ҵ���һ��OFF�ַ�����ȷ������λ��
  // 2. �����沽��1�ҵ���OFF�ַ�����β�����²��Ҳ�ͳ��ON�ַ���������
  //  ֱ������1�ĵ�ǰ��POINT_START
  // 3. ����ҵ���OFF�ַ�����ON�ַ�����Ŀ��ȾͿ�����Ϊ����1���ҵ���
  //  OFF�ַ��������ǵ�Ŀ���ַ��������ҿ��Խ��������򽫲���1���ҵ�
  //  ��OFF�ַ�������Ϊ��ǰ��POINT_START���Ӳ���1��ʼ���²��ң�ֱ��
  //  �ĵ��Ŀ�ͷ
  // ע���������ҵ�������
  // 1. �ҵ���ON�ַ�����OFF�ַ�������һ����
  // 2. �Ѿ����ҵ��ĵ��Ŀ�ͷ
  CPoint ptResultFrom;
  CPoint ptResultTo;
  CPoint ptReverseScanFrom;
  CPoint ptReverseScanTo = hint.m_ptFindStart;
  CPoint ptFindEnd = CPoint( 0, 0 );
  m_pdb->TextSubLineToMainLine( ptFindEnd );
  CPfxEditViewFindReplace FindReplace;
  for ( LONG nCountOn = 1, nCountOff = 0; nCountOn != nCountOff; ) {
    FindReplace.bMatchCase = hint.m_bMatchCase;
    FindReplace.pszFindWhat = hint.m_strOff;
    FindReplace.cchFindWhat = hint.m_strOff.GetLength();
    FindReplace.ptFrom = hint.m_ptFindStart;
    FindReplace.ptTo = ptFindEnd;
    FindReplace.bMatchWholeWord = true;
    FindReplace.bRegExp = false;
    FindReplace.bDirectDown = false;
    if ( !SearchDoFind( FindReplace ) )           //      ���ҵ��ĵ���ͷ��û���ҵ���
    {
      break;
    }
    ptResultFrom = FindReplace.ptFrom;
    ptResultTo = FindReplace.ptTo;
    hint.m_ptFindStart = FindReplace.ptFrom;
    // ����ҵ��ı�ʶ���Ƿ��ǿ��һ���֣����������Ч
    m_pdb->TextMainLineToSubLine( FindReplace.ptFrom );
    if ( ParserIsInsideBlock( FindReplace.ptFrom ) ) {
      continue;
    }
    nCountOff++;
    ptReverseScanFrom = ptResultTo;
    while ( true )           //      ���濪ʼ����ON�ַ�����ͳ������
    {
      FindReplace.bMatchCase = hint.m_bMatchCase;
      FindReplace.pszFindWhat = hint.m_strOn;
      FindReplace.cchFindWhat = hint.m_strOn.GetLength();
      FindReplace.ptFrom = ptReverseScanFrom;
      FindReplace.ptTo = ptReverseScanTo;
      FindReplace.bMatchWholeWord = true;
      FindReplace.bRegExp = false;
      FindReplace.bDirectDown = true;
      if ( !SearchDoFind( FindReplace )
           || ( FindReplace.ptFrom.y > ptReverseScanTo.y )
           || ( ( FindReplace.ptFrom.y == ptReverseScanTo.y )
                && ( FindReplace.ptFrom.x >= ptReverseScanTo.x ) ) ) {
        break;
      }
      ptReverseScanFrom = FindReplace.ptTo;
      // ����ҵ��ı�ʶ���Ƿ��ǿ��һ���֣����������Ч
      m_pdb->TextMainLineToSubLine( FindReplace.ptFrom );
      if ( !ParserIsInsideBlock( FindReplace.ptFrom ) ) {
        nCountOn++;
      }
    }
    ptReverseScanTo = ptResultFrom;
  }
  rpt = ptResultFrom;
  return ( nCountOn == nCountOff );
}
BOOL CPfxEditView::MatchBraceSearchDown( CPfxEditViewBraceHint& hint, CPoint& rpt ) {
  CPoint ptResultFrom;
  CPoint ptResultTo;
  CPoint ptReverseScanFrom = hint.m_ptFindStart;
  CPoint ptReverseScanTo;
  CPoint ptFindEnd;
  ptFindEnd.y = m_pdb->TextGetLineUpperBound();
  ptFindEnd.x = m_pdb->TextGetLineLength( ptFindEnd.y );
  m_pdb->TextSubLineToMainLine( ptFindEnd );
  CPfxEditViewFindReplace FindReplace;
  for ( LONG nCountOn = 1, nCountOff = 0; nCountOn != nCountOff; ) {
    FindReplace.bMatchCase = hint.m_bMatchCase;
    FindReplace.pszFindWhat = hint.m_strOff;
    FindReplace.cchFindWhat = hint.m_strOff.GetLength();
    FindReplace.ptFrom = hint.m_ptFindStart;
    FindReplace.ptTo = ptFindEnd;
    FindReplace.bMatchWholeWord = true;
    FindReplace.bRegExp = false;
    FindReplace.bDirectDown = true;
    if ( !SearchDoFind( FindReplace ) )           //      ���ҵ��ĵ���β��û���ҵ���
    {
      break;
    }
    ptResultFrom = FindReplace.ptFrom;
    ptResultTo = FindReplace.ptTo;
    hint.m_ptFindStart = FindReplace.ptTo;
    // ����ҵ��ı�ʶ���Ƿ��ǿ��һ����
    m_pdb->TextMainLineToSubLine( FindReplace.ptFrom );
    if ( ParserIsInsideBlock( FindReplace.ptFrom ) ) {
      continue;
    }
    nCountOff++;
    ptReverseScanTo = ptResultFrom;
    while ( true ) {
      FindReplace.bMatchCase = hint.m_bMatchCase;
      FindReplace.pszFindWhat = hint.m_strOn;
      FindReplace.cchFindWhat = hint.m_strOn.GetLength();
      FindReplace.ptFrom = ptReverseScanFrom;
      FindReplace.ptTo = ptReverseScanTo;
      FindReplace.bMatchWholeWord = true;
      FindReplace.bRegExp = false;
      FindReplace.bDirectDown = true;
      if ( !SearchDoFind( FindReplace )
           || ( FindReplace.ptFrom.y > ptReverseScanTo.y )
           || ( ( FindReplace.ptFrom.y == ptReverseScanTo.y )
                && ( FindReplace.ptFrom.x >= ptReverseScanTo.x ) ) ) {
        break;
      }
      ptReverseScanFrom = FindReplace.ptTo;
      // ����ҵ��ı�ʶ���Ƿ��ǿ��һ����
      m_pdb->TextMainLineToSubLine( FindReplace.ptFrom );
      if ( !ParserIsInsideBlock( FindReplace.ptFrom ) ) {
        nCountOn++;
      }
    }
    ptReverseScanFrom = ptResultTo;
  }
  rpt = ptResultFrom;
  return ( nCountOn == nCountOff );
}
