#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
typedef struct image_res_t {
  int id;
  const char* filepath;
} image_res_t;

#define DISCARDABLE 0
#define ICON   1
#define BITMAP 2
#define CURSOR 3

#define IDR_MAINFRAME             "res\\BCGPControls.ico",
    
    //
    // Bitmap
    //
    
#define IDB_BTN1                 "res\\btn1.bmp"
#define IDB_BTN1_HOT             "res\\btn1_hot.bmp"  
#define IDB_GLOBE                "res\\globe.bmp"  
#define IDB_PLAY                 "res\\play.bmp"  
#define IDB_STOP                 "res\\stop.bmp"  
#define IDB_STOP_HOT             "res\\stop_hot.bmp"  
#define IDB_PLAY_HOT             "res\\play_hot.bmp"  
#define IDB_IMAGE                "res\\image.bmp"  
#define IDB_TEST_BITMAP          "res\\test_bit.bmp"  
#define IDR_TOOLBAR_MENU_IMAGES  "res\\toolbar1.bmp"  
#define IDB_ICONS                "res\\icons.bmp"  
#define IDB_CHECK                "res\\bitmap1.bmp"  
#define IDB_CHECKNO              "res\\checkno.bmp"  
#define IDB_RADIO_ON             "res\\radio_on.bmp"  
#define IDB_RADIO_OFF            "res\\radio_of.bmp"  
#define IDB_PROPICONS            "res\\propicons.bmp"  
#define IDB_BUTTONS              "res\\buttons.bmp"  
#define IDB_CHECK32              "res\\check32.bmp"  
#define IDB_CHECKNO32            "res\\checkno32.bmp"  
#define IDB_RADIO_OFF32          "res\\radio_of32.bmp"  
#define IDB_RADIO_ON32           "res\\radio_on32.bmp"  
#define IDB_BTN1_HOT_32          "res\\button32hot.bmp"  
#define IDB_BTN1_32              "res\\button32.bmp"  
#define IDB_ICONS32              "res\\icons32.bmp"  
    
    //
    // Cursor
    //
    
#define IDB_CURSOR                 "res\\Btn.cur"


int DIALOGimpl(ctrl_t* c, int id, int l, int t, int w, int h, int style, const char* caption, const char* fontinfo, ctrl_proc_t ctrlproc, void* obj, ctrl_t* listener, sys_t* sys) {
  if (listener!=c) {
    ctrl_set(ctrlproc, obj, c, listener, caption, sys);
    ctrl_addsub(listener, c);
  }
  c->id = id;
  ctrl_setrect(c, iRECT2(l, t, w, h));
  c->style = style;
  c->text = caption;
  if (fontinfo) {
    c->fo = sys_font_load_win32(c->sys, fontinfo);
  }
  return 0;
}

#define DIALOG(s, id, l, t, w, h, style, caption, fontinfo)  DIALOGimpl(*sub++ = (s)->c, id, l, t, w, h, style, caption, fontinfo, (s)->proc, s, listener, sys)

#define GROUPBOX(s, text, id, l, t, w, h)  DIALOG(s, id, l, t, w, h, 0, text, 0)
#define PUSHBUTTON(s, text, id, l, t, w, h, style)  DIALOG(s, id, l, t, w, h, style|BS_DEFPUSHBUTTON, text, 0)
#define LTEXT(s, text, id, l, t, w, h)  DIALOG(s, id, l, t, w, h, 0, text, 0)
#define EDITTEXT(s, id, l, t, w, h, style)  DIALOG(s, id, l, t, w, h, style, 0, 0)
#define COMBOBOX(s, id, l, t, w, h, style)  DIALOG(s, id, l, t, w, h, style, 0, 0)
#define CONTROL(s, text, id, cls, style, l, t, w, h)  DIALOG(s, id, l, t, w, h, style, text, 0)

int page1_init(CPage1* s, sys_t* sys, ctrl_t** sub) {
  ctrl_t* listener = s->c;
  ctrl_t** sub0 = sub;
  DIALOG(s, IDD_PAGE1,  0, 0, 309, 228, WS_CHILD | WS_DISABLED | WS_CAPTION, "Buttons", "MS Sans Serif|8");
  PUSHBUTTON(&s->m_Button, "CBCGB&utton",IDC_BUTTON,17,19,71,14, 0);
#if 1
  GROUPBOX(&s->m_gbox1, "Push button: ",IDC_STATIC,7,7,163,140);
  LTEXT(&s->m_text1, "Image &Location:",IDC_STATIC,17,54,52,8);
  COMBOBOX(&s->m_nImageLocation, IDC_IMAGE_LOCATION,79,53,83,83,CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP);
  LTEXT(&s->m_text2,           "Appearanc&e:",IDC_STATIC,17,72,42,8);
  COMBOBOX(&s->m_iImage,        IDC_IMAGE,79,71,83,83,CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP);
  LTEXT(&s->m_text3,           "&Border:",IDC_BORDER_LABEL,17,90,24,8);
  COMBOBOX(&s->m_iBorderStyle,        IDC_BORDER,79,89,83,74,CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP);
  LTEXT(&s->m_text4,           "&Cursor:",IDC_STATIC,17,108,23,8);
  COMBOBOX(&s->m_iCursor,        IDC_CURSOR,79,107,83,56,CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP);
  LTEXT          (&s->m_text5,"&Tooltip:",IDC_STATIC,17,126,24,8);
  EDITTEXT       (&s->m_strToolTip,IDC_TOOLTIP,79,125,49,14,ES_AUTOHSCROLL);
  PUSHBUTTON     (&s->m_btnSetTooltip,"&Set",IDC_SET_TOOLTIP,137,125,25,14, 0);
  GROUPBOX       (&s->m_gbox2,"Radio button: ",IDC_STATIC,180,7,122,101);
  CONTROL        (&s->m_btnRadio,"|Option 1|Option 2|Option 3|Option 4|",IDC_RADIO1,"Button",BS_AUTORADIOBUTTON | BS_PUSHLIKE,210,27,43,60);
  GROUPBOX       (&s->m_text6,"Menu button:",IDC_STATIC,7,151,163,70);
  PUSHBUTTON     (&s->m_btnMenu,"CBCG&MenuButton",IDC_BUTTON_MENU,17,166,90,14, 0);
  CONTROL        (&s->m_bRightArrow,"Ri&ght Arrow",IDC_RIGHT_ARROW,"Button",BS_AUTOCHECKBOX |                     WS_TABSTOP,17,188,53,10);
  CONTROL        (&s->m_bMenuStayPressed,"Stay &Pressed",IDC_PRESSED_ON_MENU,"Button",                    BS_AUTOCHECKBOX | WS_TABSTOP,79,188,57,10);
  CONTROL        (&s->m_bMenuDefaultClick,"&Default Click",IDC_MENU_DEFAULT_CLICK,"Button",                    BS_AUTOCHECKBOX | WS_TABSTOP,17,203,56,10);
  GROUPBOX       (&s->m_gbox3,"Check button:",IDC_STATIC,180,110,122,37);
  CONTROL        (&s->m_bCheck,"&On/Off",IDC_CHECK_BUTTON,"Button",BS_AUTOCHECKBOX |                     BS_PUSHLIKE | WS_TABSTOP,210,127,38,10);
  CONTROL        (&s->m_wndXPButtons,"Windows &XP-style Buttons",IDC_XP_BUTTONS,"Button",                    BS_AUTOCHECKBOX | WS_TABSTOP,203,211,99,10);
  GROUPBOX       (&s->m_gbox4,"Multi-line text button:",IDC_STATIC,180,151,122,54);
  PUSHBUTTON     (&s->m_btnMultiLine,"This is a multi-line button. \nYou can add some text lines here...",                  IDC_BUTTON_MULTLINE,191,166,96,32,BS_MULTILINE);
#endif
  return sub - sub0;
}

#if 0
/////////////////////////////////////////////////////////////////////////////
//
// Dialog
//


IDD_PAGE2 DIALOG DISCARDABLE  0, 0, 309, 227
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Color Controls"
FONT 8, "MS Sans Serif"
BEGIN
    PUSHBUTTON      "&Dialog...",IDC_COLOR_DLG,37,24,39,14
    GROUPBOX        "Advanced color dialog: ",IDC_STATIC,7,7,137,41
    PUSHBUTTON      "Picker",IDC_COLOR_PICKER,37,64,67,14
    CTEXT           "Static",IDC_RGB,39,82,67,8
    GROUPBOX        "Color picker: ",IDC_STATIC,7,52,137,42
    GROUPBOX        "Color bar: ",IDC_STATIC,7,98,137,85
    CONTROL         "",IDC_COLORBAR,"Static",SS_BLACKFRAME,17,112,65,64
    GROUPBOX        "Color bar with custom palette: ",IDC_STATIC,151,7,151,98
    CONTROL         "",IDC_COLORBAR2,"Static",SS_BLACKFRAME,159,20,136,64
    CTEXT           "Static",IDC_RGB_COLOR_BAR2,159,91,135,8
    CTEXT           "Static",IDC_RGB_COLOR_BAR,85,136,53,8
    CTEXT           "Static",IDC_RGB_DLG,77,27,59,8
    GROUPBOX        "Color palette: ",IDC_STATIC,151,109,151,74
    PUSHBUTTON      "Palette",IDC_COLOR_PALETTE,159,123,107,42
    CTEXT           "Static",IDC_RGB_COLOR_PALETTE,159,170,135,8
    PUSHBUTTON      "Lum",IDC_LUM,273,123,21,42
    GROUPBOX        "Color hue: ",IDC_STATIC,7,191,295,30
    PUSHBUTTON      "H",IDC_HUE,17,205,183,6
    CTEXT           "Static",IDC_RGB_COLOR_HUE,219,205,53,8
END

IDD_PAGE3 DIALOGEX 0, 0, 309, 217
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Misc. Controls"
FONT 8, "MS Sans Serif", 0, 0, 0x1
BEGIN
    GROUPBOX        "Font combobox: ",IDC_STATIC,7,7,148,67
    COMBOBOX        IDC_FONT,17,21,130,209,CBS_DROPDOWNLIST | 
                    CBS_OWNERDRAWFIXED | CBS_SORT | CBS_HASSTRINGS | 
                    WS_VSCROLL | WS_TABSTOP
    CONTROL         "&TrueType",IDC_TRUETYPE,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,17,40,47,10
    CONTROL         "&Raster",IDC_RASTER,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,65,40,37,10
    CONTROL         "&Device",IDC_DEVICE,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,105,40,39,10
    CONTROL         "Draw &using font",IDC_DRAW_USING_FONT,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,17,55,65,10
    GROUPBOX        "Link button: ",IDC_STATIC,164,7,138,49
    PUSHBUTTON      "CBCGURLLinkButton",IDC_BUTTON_URL,176,26,108,14
    GROUPBOX        "Animation control:",IDC_STATIC,7,78,148,68
    CONTROL         "",IDC_ANIMATION,"Static",SS_GRAYRECT,17,92,20,20
    PUSHBUTTON      "St&art",IDC_START_ANIMATION,63,92,38,14
    PUSHBUTTON      "Sto&p",IDC_STOP_ANIMATION,109,92,38,14,WS_DISABLED
    LTEXT           "&Frame Rate:",IDC_STATIC,17,118,40,8,0,
                    WS_EX_TRANSPARENT
    CONTROL         "Slider1",IDC_FRAME_RATE,"msctls_trackbar32",TBS_NOTICKS | 
                    WS_TABSTOP,63,118,84,16,WS_EX_STATICEDGE
    LTEXT           "CBCGEditListBo&x",IDC_EDIT_LIST,164,71,138,59,
                    WS_TABSTOP
    GROUPBOX        "Image editor:",IDC_STATIC,7,154,148,56
    CONTROL         "",IDC_IMAGE_AREA,"Static",SS_GRAYRECT | NOT WS_VISIBLE,
                    22,168,20,20
    PUSHBUTTON      "&Edit...",IDC_EDIT_IMAGE,63,168,38,14
    GROUPBOX        "Browse Edit Controls: ",IDC_STATIC,164,134,138,76
    EDITTEXT        IDC_FILE_EDIT,202,148,90,14,ES_AUTOHSCROLL
    LTEXT           "File:",IDC_STATIC,173,148,14,8
    EDITTEXT        IDC_FOLDER_EDIT,202,169,90,14,ES_AUTOHSCROLL
    LTEXT           "Folder: ",IDC_STATIC,173,169,24,8
    EDITTEXT        IDC_CUSTOM_EDIT,202,189,90,14,ES_AUTOHSCROLL
    LTEXT           "Custom: ",IDC_STATIC,173,189,28,8
END

IDD_PAGE4 DIALOG DISCARDABLE  0, 0, 309, 190
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Masked Edit"
FONT 8, "MS Sans Serif"
BEGIN
    EDITTEXT        IDC_MASKEDIT1,18,22,114,14,ES_AUTOHSCROLL
    LTEXT           "Mask 1: phone number",IDC_STATIC,18,13,74,8
    EDITTEXT        IDC_MASKEDIT2,18,50,114,14,ES_AUTOHSCROLL
    LTEXT           "Mask 2: State, Zip Code",IDC_STATIC,18,41,78,8
    EDITTEXT        IDC_MASKEDIT3,18,78,114,14,ES_AUTOHSCROLL
    LTEXT           "Mask 3: serial code",IDC_STATIC,18,69,62,8
    EDITTEXT        IDC_MASKEDIT4,18,106,114,14,ES_AUTOHSCROLL
    LTEXT           "Mask 4: 0xFFFF",IDC_STATIC,18,97,52,8
    EDITTEXT        IDC_MASKEDIT5,18,134,114,14,ES_AUTOHSCROLL
    LTEXT           "Mask 5: digits only",IDC_STATIC,18,125,59,8
    PUSHBUTTON      "Get values",IDC_BUTTON1,249,23,40,14
    EDITTEXT        IDC_VALUE1,142,23,94,14,ES_AUTOHSCROLL | ES_READONLY
    EDITTEXT        IDC_VALUE2,142,50,94,14,ES_AUTOHSCROLL | ES_READONLY
    EDITTEXT        IDC_VALUE3,142,78,94,14,ES_AUTOHSCROLL | ES_READONLY
    EDITTEXT        IDC_VALUE4,142,105,94,14,ES_AUTOHSCROLL | ES_READONLY
    EDITTEXT        IDC_VALUE5,142,134,94,14,ES_AUTOHSCROLL | ES_READONLY
END

IDD_PAGE5 DIALOG DISCARDABLE  0, 0, 309, 243
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "List Control && Property Grid"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "CBCGPListCtrl:",IDC_STATIC,7,7,48,8
    CONTROL         "List1",IDC_LIST1,"SysListView32",LVS_REPORT | WS_BORDER | 
                    WS_TABSTOP,7,20,132,140
    CONTROL         "&Mark sorted column",IDC_MARK_SORTED_COLUMN,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,7,167,78,10
    CONTROL         "&Colored rows",IDC_COLOR_ROWS,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,7,181,57,10
    CONTROL         "Modify &Font",IDC_MODIFY_FONT,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,7,194,53,10
    LTEXT           "CBCGPPropList:",IDC_STATIC,157,7,52,8
    CONTROL         "",IDC_PROPLIST_LOCATION,"Static",SS_BLACKFRAME | NOT 
                    WS_VISIBLE,157,20,145,140
    PUSHBUTTON      "Expand &All",IDC_PROPLIST_EXPAND_ALL,252,6,50,12
    CONTROL         "&Header",IDC_HEADER,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,157,167,39,10
    CONTROL         "VS.NET look",IDC_DOT_NET_LOOK,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,157,179,57,10
    CONTROL         "&Description",IDC_DESCRIPTION_AREA,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,157,191,51,10
    CONTROL         "Mar&k Changed",IDC_MARK_CHANGED,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,157,203,63,10
    CONTROL         "Hide Fon&t Prop.",IDC_HIDE_FONT_PROPS,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,157,215,65,10
    CONTROL         "Show Comma&nds",IDC_SHOW_COMMANDS,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,157,226,71,10
    CONTROL         "C&ustom Colors",IDC_CUSTOM_COLORS,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,224,167,61,10
    CONTROL         "Categori&zed",IDC_PROPLIST_CATEGORIZED,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,224,181,53,10
    CONTROL         "Show D&rag Context",IDC_PROPLIST_CATEGORIZED2,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,224,194,77,10
    PUSHBUTTON      "R&eset Values",IDC_RESET_VALUES,237,207,65,14
    PUSHBUTTON      "Commit Chan&ges",IDC_COMMIT_CHANGES,237,222,65,14
END

IDD_PAGE6 DIALOG DISCARDABLE  0, 0, 309, 217
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Time Controls"
FONT 8, "MS Sans Serif"
BEGIN
    GROUPBOX        "CBCGPDateTimeCtrl: ",IDC_STATIC,4,7,146,157
    PUSHBUTTON      "DateTime",IDC_DATETIMEPICKER1,11,20,119,12
    GROUPBOX        "Display parts: ",IDC_STATIC,11,39,120,40
    CONTROL         "D&ate and Time",IDC_DATETIME_PARTS,"Button",
                    BS_AUTORADIOBUTTON | WS_GROUP,20,50,63,10
    CONTROL         "&Date only",IDC_DATETIME_PARTS2,"Button",
                    BS_AUTORADIOBUTTON,20,64,46,10
    CONTROL         "&Time only",IDC_DATETIME_PARTS3,"Button",
                    BS_AUTORADIOBUTTON,71,64,46,10
    CONTROL         "Show &Check Box",IDC_CAL_CHKBOX,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,20,84,71,10
    CONTROL         "Show D&rop Calendar Button",IDC_CAL_DROPBTN,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,20,94,104,10
    CONTROL         "Show &Spin Button",IDC_CAL_SPIN,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,20,105,73,10
    GROUPBOX        "Time Format: ",IDC_STATIC,11,119,120,40
    CONTROL         "Defined by System &Locale",IDC_TIME_FORMAT,"Button",
                    BS_AUTORADIOBUTTON | WS_GROUP,20,131,98,10
    CONTROL         "&24 Hours",IDC_TIME_FORMAT2,"Button",BS_AUTORADIOBUTTON,
                    20,144,45,10
    CONTROL         "&12 Hours",IDC_TIME_FORMAT3,"Button",BS_AUTORADIOBUTTON,
                    71,144,45,10
    GROUPBOX        "CBCGPDurationCtrl: ",IDC_STATIC,4,165,146,43
    PUSHBUTTON      "Duration",IDC_DURATION,11,177,85,12
    CONTROL         "Show S&pin Button",IDC_CHECKSPIN,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,11,195,73,10
    CONTROL         "Da&ys",IDC_CHECKDAYS,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,102,171,29,10
    CONTROL         "&H/M",IDC_CHECKHOURS,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,102,183,31,10
    CONTROL         "S&econds",IDC_CHECKSECONDS,"Button",BS_AUTOCHECKBOX | 
                    WS_TABSTOP,102,195,44,10
    GROUPBOX        "CBCGPCalendar: ",IDC_STATIC,156,7,146,201
    CONTROL         "",IDC_CALENDARLOCATION,"Static",SS_GRAYFRAME | NOT 
                    WS_VISIBLE,165,20,126,105
    LTEXT           "&First Day of Week: ",IDC_STATIC,165,142,62,8
    COMBOBOX        IDC_WEEKSTART,228,140,63,85,CBS_DROPDOWNLIST | 
                    WS_VSCROLL | WS_TABSTOP
    CONTROL         "Multiple Day Selection",IDC_CALENDARBAR_MULTY,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,183,161,86,10
    CONTROL         "Display Week Numbers",IDC_CALENDARBAR_WEEKNOBND,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,183,176,90,10
    CONTROL         "Display ""Today"" Button",IDC_CALENDARBAR_TODAY_BUTTON,
                    "Button",BS_AUTOCHECKBOX | WS_TABSTOP,183,191,90,10
END

IDD_PAGE7 DIALOG DISCARDABLE  0, 0, 309, 217
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Shell Controls"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "CBCGPShellTree:",IDC_STATIC,6,7,57,8
    CONTROL         "Tree1",IDC_TREE1,"SysTreeView32",TVS_HASBUTTONS | 
                    TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | 
                    WS_BORDER | WS_TABSTOP,6,19,153,77
    GROUPBOX        "Folder selector: ",IDC_STATIC,173,7,129,89
    PUSHBUTTON      "Select &Folder...",IDC_SELECT_FOLDER,184,20,60,14
    LTEXT           "CBCGPShellList:",IDC_STATIC,6,102,53,8
    CONTROL         "List1",IDC_LIST1,"SysListView32",LVS_REPORT | 
                    LVS_SHOWSELALWAYS | WS_BORDER | WS_TABSTOP,6,113,296,95
    LTEXT           "Static",IDC_SELECTED_FOLDER,184,40,111,48
END

IDD_PAGE8 DIALOG DISCARDABLE  0, 0, 309, 217
STYLE WS_CHILD | WS_DISABLED | WS_CAPTION
CAPTION "Calculator"
FONT 8, "MS Sans Serif"
BEGIN
    LTEXT           "CBCGPCalculator:",IDC_STATIC,6,7,58,8
    CONTROL         "",IDC_CALCULATOR,"Static",SS_BLACKFRAME,6,19,135,128
    GROUPBOX        "CBCGPEdit with calculator: ",IDC_STATIC,148,7,154,140
    EDITTEXT        IDC_EDIT,167,31,114,14,ES_AUTOHSCROLL
    CONTROL         "&User-defined commands",IDC_USER_COMMANDS,"Button",
                    BS_AUTOCHECKBOX | WS_TABSTOP,6,154,92,10
END


#ifndef _MAC
/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
 FILEVERSION 1,0,0,1
 PRODUCTVERSION 1,0,0,1
 FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x4L
 FILETYPE 0x1L
 FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "040904B0"
        BEGIN
            VALUE "CompanyName", "\0"
            VALUE "FileDescription", "BCGControls MFC Application\0"
            VALUE "FileVersion", "1, 0, 0, 1\0"
            VALUE "InternalName", "BCGControls\0"
            VALUE "LegalCopyright", "Copyright (C) 2000\0"
            VALUE "LegalTrademarks", "\0"
            VALUE "OriginalFilename", "BCGControls.EXE\0"
            VALUE "ProductName", "BCGControls Application\0"
            VALUE "ProductVersion", "1, 0, 0, 1\0"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x409, 1200
    END
END

#endif    // !_MAC


/////////////////////////////////////////////////////////////////////////////
//
// Menu
//

IDR_MENU1 MENU DISCARDABLE 
BEGIN
    POPUP "Dummy"
    BEGIN
        MENUITEM "Item &1",                     ID_ITEM_1
        MENUITEM "Item &2",                     ID_ITEM_2
        MENUITEM "Item &3",                     ID_ITEM_3
        MENUITEM "Item &4",                     ID_ITEM_4
    END
END

IDR_DIALOG_MENU MENU DISCARDABLE 
BEGIN
    POPUP "Dummy"
    BEGIN
        MENUITEM "Item &1",                     ID_ITEM_1
        MENUITEM "Item &2",                     ID_ITEM_2
        MENUITEM SEPARATOR
        MENUITEM "&About...",                   ID_DIALOG_ABOUT
    END
END


/////////////////////////////////////////////////////////////////////////////
//
// Dialog Info
//

IDD_PAGE1 DLGINIT
BEGIN
    IDC_IMAGE_LOCATION, 0x403, 24, 0
0x6d49, 0x6761, 0x2065, 0x6e6f, 0x6c20, 0x6665, 0x2074, 0x6428, 0x6665, 
0x7561, 0x746c, 0x0029, 
    IDC_IMAGE_LOCATION, 0x403, 15, 0
0x6d49, 0x6761, 0x2065, 0x6e6f, 0x7220, 0x6769, 0x7468, "\000" 
    IDC_IMAGE_LOCATION, 0x403, 13, 0
0x6d49, 0x6761, 0x2065, 0x6e6f, 0x7420, 0x706f, "\000" 
    IDC_IMAGE, 0x403, 6, 0
0x6d49, 0x6761, 0x0065, 
    IDC_IMAGE, 0x403, 5, 0
0x6554, 0x7478, "\000" 
    IDC_IMAGE, 0x403, 15, 0
0x6d49, 0x6761, 0x2065, 0x6e61, 0x2064, 0x6554, 0x7478, "\000" 
    IDC_BORDER, 0x403, 5, 0
0x6c46, 0x7461, "\000" 
    IDC_BORDER, 0x403, 10, 0
0x6553, 0x696d, 0x462d, 0x616c, 0x0074, 
    IDC_BORDER, 0x403, 4, 0
0x2d33, 0x0044, 
    IDC_CURSOR, 0x403, 9, 0
0x7453, 0x6e61, 0x6164, 0x6472, "\000" 
    IDC_CURSOR, 0x403, 5, 0
0x6148, 0x646e, "\000" 
    IDC_CURSOR, 0x403, 13, 0
0x7355, 0x7265, 0x642d, 0x6665, 0x6e69, 0x6465, "\000" 
    0
END


/////////////////////////////////////////////////////////////////////////////
//
// Toolbar
//

IDR_TOOLBAR_MENU_IMAGES TOOLBAR DISCARDABLE  16, 15
BEGIN
    BUTTON      ID_DIALOG_ABOUT
    BUTTON      ID_ITEM_1
END


/////////////////////////////////////////////////////////////////////////////
//
// String Table
//

STRINGTABLE DISCARDABLE 
BEGIN
    IDS_ABOUTBOX            "&About BCGControls..."
    IDS_CAPTION             "BCGControls"
END

STRINGTABLE DISCARDABLE 
BEGIN
    AFX_IDS_APP_TITLE       "BCGPControls Example"
END

/////////////////////////////////////////////////////////////////////////////


#endif
