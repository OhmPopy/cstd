// dear imgui, v1.50 WIP
// (headers)
// See imgui.cpp file for documentation.
// See ImGui_ShowTestWindow() in imgui_demo.cpp for demo code.
// Read 'Programmer guide' in imgui.cpp for notes on how to setup ImGui in your codebase.
// Get latest version at https://github.com/ocornut/imgui
#pragma once
#if !defined(IMGUI_DISABLE_INCLUDE_IMCONFIG_H) || defined(IMGUI_INCLUDE_IMCONFIG_H)
#include "imconfig.h" // User-editable configuration file
#endif
#include <float.h> // FLT_MAX
#include <stdarg.h> // va_list
#include <stddef.h> // ptrdiff_t, NULL
#include <string.h> // memset, memmove, memcpy, strlen, strchr, strcpy, strcmp
#define IMGUI_VERSION "1.50 WIP"
// Define attributes of all API symbols declarations, e.g. for DLL under Windows.
#ifndef IMGUI_API
#define IMGUI_API
#endif
// Define assertion handler.
#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR) assert(_EXPR)
#endif
// Some compilers support applying printf-style warnings to user functions.
#if defined(__clang__) || defined(__GNUC__)
#define IM_PRINTFARGS(FMT) __attribute__((format(printf, FMT, (FMT+1))))
#else
#define IM_PRINTFARGS(FMT)
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif
// Forward declarations
struct ImDrawChannel; // Temporary storage for outputting drawing commands out of order, used by ImDrawList::ChannelsSplit()
struct ImDrawCmd; // A single draw command within a parent ImDrawList (generally maps to 1 GPU draw call)
struct ImDrawData; // All draw command lists required to render the frame
struct ImDrawList; // A single draw command list (generally one per window)
struct ImDrawVert; // A single vertex (20 bytes by default, override layout with IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT)
struct ImFont; // Runtime data for a single font within a parent ImFontAtlas
struct ImFontAtlas; // Runtime data for multiple fonts, bake multiple fonts into a single texture, TTF font loader
struct ImFontConfig; // Configuration data when adding a font or merging fonts
struct ImColor; // Helper functions to create a color that can be converted to either u32 or float4
struct ImGuiIO; // Main configuration and I/O between your application and ImGui
struct ImGuiOnceUponAFrame; // Simple helper for running a block of code not more than once a frame, used by IMGUI_ONCE_UPON_A_FRAME macro
struct ImGuiStorage; // Simple custom key value storage
struct ImGuiStyle; // Runtime data for styling/colors
struct ImGuiTextFilter; // Parse and apply text filters. In format "aaaaa[,bbbb][,ccccc]"
struct ImGuiTextBuffer; // Text buffer for logging/accumulating text
struct ImGuiTextEditCallbackData; // Shared state of ImGui_InputText() when using custom ImGuiTextEditCallback (rare/advanced use)
struct ImGuiSizeConstraintCallbackData;// Structure used to constraint window size in custom ways when using custom ImGuiSizeConstraintCallback (rare/advanced use)
struct ImGuiListClipper; // Helper to manually clip large list of items
struct ImGuiContext; // ImGui context (opaque)

typedef unsigned char uchar;
typedef struct ImDrawChannel ImDrawChannel;
typedef struct ImDrawCmd ImDrawCmd;
typedef struct ImDrawData ImDrawData;
typedef struct ImDrawList ImDrawList;
typedef struct ImDrawVert ImDrawVert;
typedef struct ImFont ImFont;
typedef struct ImFontAtlas ImFontAtlas;
typedef struct ImFontConfig ImFontConfig;
typedef struct ImColor ImColor;
typedef struct ImGuiIO ImGuiIO;
typedef struct ImGuiOnceUponAFrame ImGuiOnceUponAFrame;
typedef struct ImGuiStorage ImGuiStorage;
typedef struct ImGuiStyle ImGuiStyle;
typedef struct ImGuiTextFilter ImGuiTextFilter;
typedef struct ImGuiTextBuffer ImGuiTextBuffer;
typedef struct ImGuiTextEditCallbackData ImGuiTextEditCallbackData;
typedef struct ImGuiSizeConstraintCallbackData ImGuiSizeConstraintCallbackData;
typedef struct ImGuiListClipper ImGuiListClipper;
typedef struct ImGuiContext ImGuiContext;

// Typedefs and Enumerations (declared as int for compatibility and to not pollute the top of this file)
typedef unsigned int ImU32; // 32-bit unsigned integer (typically used to store packed colors)
typedef unsigned int ImGuiID; // unique ID used by widgets (typically hashed from a stack of string)
typedef unsigned short ImWchar; // character for keyboard input/display
typedef void* ImTextureID; // user data to identify a texture (this is whatever to you want it to be! read the FAQ about ImTextureID in imgui.cpp)
typedef int ImGuiCol; // a color identifier for styling // enum ImGuiCol_
typedef int ImGuiStyleVar; // a variable identifier for styling // enum ImGuiStyleVar_
typedef int ImGuiKey; // a key identifier (ImGui-side enum) // enum ImGuiKey_
typedef int ImGuiColorEditMode; // color edit mode for ColorEdit*() // enum ImGuiColorEditMode_
typedef int ImGuiMouseCursor; // a mouse cursor identifier // enum ImGuiMouseCursor_
typedef int ImGuiWindowFlags; // window flags for Begin*() // enum ImGuiWindowFlags_
typedef int ImGuiSetCond; // condition flags for Set*() // enum ImGuiSetCond_
typedef int ImGuiInputTextFlags; // flags for InputText*() // enum ImGuiInputTextFlags_
typedef int ImGuiSelectableFlags; // flags for Selectable() // enum ImGuiSelectableFlags_
typedef int ImGuiTreeNodeFlags; // flags for TreeNode*(), Collapsing*() // enum ImGuiTreeNodeFlags_
typedef int (*ImGuiTextEditCallback)(ImGuiTextEditCallbackData* data);
typedef void (*ImGuiSizeConstraintCallback)(ImGuiSizeConstraintCallbackData* data);
// Others helpers at bottom of the file:
// class ImVector<> // Lightweight std::vector like class.
// IMGUI_ONCE_UPON_A_FRAME // Execute a block of code once per frame only (convenient for creating UI within deep-nested code that runs multiple times)

#undef DEFAULT
#ifdef __cplusplus
#define DEFAULT(x)
#else
typedef int bool;
#define DEFAULT(x)
#endif
#if 0
typedef unsigned int SSIZE;
#undef LOWORD
#undef HIWORD
#define LOWORD(l)           ((unsigned short)((SSIZE)(l) & 0xffff))
#define HIWORD(l)           ((unsigned short)((SSIZE)(l) >> 16))
#define GET_X(sz32)    ((short)LOWORD(sz32))
#define GET_Y(sz32)    ((short)HIWORD(sz32))
#define MAKE32(a, b)      ((SSIZE)(((unsigned short)((SSIZE)(a) & 0xffff)) | ((SSIZE)((unsigned short)((SSIZE)(b) & 0xffff))) << 16))
#endif

// ImGui end-user API
// In a namespace so that user can add extra functions in a separate file (e.g. Value() helpers for your vector or common types)
// namespace ImGui
// Main
IMGUI_API ImGuiIO* ImGui_GetIO();
IMGUI_API ImGuiStyle* ImGui_GetStyle();
IMGUI_API ImDrawData* ImGui_GetDrawData(); // same value as passed to your io.RenderDrawListsFn() function. valid after Render() and until the next call to NewFrame()
IMGUI_API void ImGui_NewFrame(); // start a new ImGui frame, you can submit any command from this point until NewFrame()/Render().
IMGUI_API void ImGui_Render(); // ends the ImGui frame, finalize rendering data, then call your io.RenderDrawListsFn() function if set.
IMGUI_API void ImGui_Shutdown();
IMGUI_API void ImGui_ShowUserGuide(); // help block
IMGUI_API void ImGui_ShowStyleEditor(ImGuiStyle* ref DEFAULT(NULL)); // style editor block. you can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it uses the default style)
IMGUI_API void ImGui_ShowTestWindow(bool* p_open DEFAULT(NULL)); // test window demonstrating ImGui features
IMGUI_API void ImGui_ShowMetricsWindow(bool* p_open DEFAULT(NULL)); // metrics window for debugging ImGui
// Window

IMGUI_API bool ImGui_BeginEx(const char* name, bool* p_open, const ImVec2& size_on_first_use, float bg_alpha DEFAULT(-1.0f), ImGuiWindowFlags flags DEFAULT(0)); // OBSOLETE. this is the older/longer API. the extra parameters aren't very relevant. call SetNextWindowSize() instead if you want to set a window size. For regular windows, 'size_on_first_use' only applies to the first time EVER the window is created and probably not what you want! might obsolete this API eventually.
IMGUI_API bool ImGui_Begin(const char* name, bool* p_open DEFAULT(NULL), ImGuiWindowFlags flags DEFAULT(0)); // push window to the stack and start appending to it. see .cpp for details. return false when window is collapsed, so you can early out in your code. 'bool* p_open' creates a widget on the upper-right to close the window (which sets your bool to false).
IMGUI_API void ImGui_End(); // finish appending to current window, pop it off the window stack.
IMGUI_API bool ImGui_BeginChild(const char* str_id, const ImVec2& size DEFAULT(fVec2(0, 0)), bool border DEFAULT(false), ImGuiWindowFlags extra_flags DEFAULT(0)); // begin a scrolling region. size==0.0f: use remaining window size, size<0.0f: use remaining window size minus abs(size). size>0.0f: fixed size. each axis can use a different mode, e.g. ImVec2(0,400).
IMGUI_API bool ImGui_BeginChildID(ImGuiID id, const ImVec2* size DEFAULT(fVec2(0, 0)), bool border DEFAULT(false), ImGuiWindowFlags extra_flags DEFAULT(0)); // "
IMGUI_API void ImGui_EndChild();
IMGUI_API ImVec2 ImGui_GetContentRegionMax(); // current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates
IMGUI_API ImVec2 ImGui_GetContentRegionAvail(); // == GetContentRegionMax() - GetCursorPos()
IMGUI_API float ImGui_GetContentRegionAvailWidth(); //
IMGUI_API ImVec2 ImGui_GetWindowContentRegionMin(); // content boundaries min (roughly (0,0)-Scroll), in window coordinates
IMGUI_API ImVec2 ImGui_GetWindowContentRegionMax(); // content boundaries max (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates
IMGUI_API float ImGui_GetWindowContentRegionWidth(); //
IMGUI_API ImDrawList* ImGui_GetWindowDrawList(); // get rendering command-list if you want to append your own draw primitives
IMGUI_API ImVec2 ImGui_GetWindowPos(); // get current window position in screen space (useful if you want to do your own drawing via the DrawList api)
IMGUI_API ImVec2 ImGui_GetWindowSize(); // get current window size
IMGUI_API float ImGui_GetWindowWidth();
IMGUI_API float ImGui_GetWindowHeight();
IMGUI_API bool ImGui_IsWindowCollapsed();
IMGUI_API void ImGui_SetWindowFontScale(float scale); // per-window font scale. Adjust IO.FontGlobalScale if you want to scale all windows
IMGUI_API void ImGui_SetNextWindowPos(const ImVec2& pos, ImGuiSetCond cond DEFAULT(0)); // set next window position. call before Begin()
IMGUI_API void ImGui_SetNextWindowPosCenter(ImGuiSetCond cond DEFAULT(0)); // set next window position to be centered on screen. call before Begin()
IMGUI_API void ImGui_SetNextWindowSize(const ImVec2& size, ImGuiSetCond cond DEFAULT(0)); // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
IMGUI_API void ImGui_SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeConstraintCallback custom_callback DEFAULT(NULL), void* custom_callback_data DEFAULT(NULL)); // set next window size limits. use -1,-1 on either X/Y axis to preserve the current size. Use callback to apply non-trivial programmatic constraints.
IMGUI_API void ImGui_SetNextWindowContentSize(const ImVec2& size); // set next window content size (enforce the range of scrollbars). set axis to 0.0f to leave it automatic. call before Begin()
IMGUI_API void ImGui_SetNextWindowContentWidth(float width); // set next window content width (enforce the range of horizontal scrollbar). call before Begin()
IMGUI_API void ImGui_SetNextWindowCollapsed(bool collapsed, ImGuiSetCond cond = 0); // set next window collapsed state. call before Begin()
IMGUI_API void ImGui_SetNextWindowFocus(); // set next window to be focused / front-most. call before Begin()
IMGUI_API void ImGui_SetWindowPos(const ImVec2& pos, ImGuiSetCond cond = 0); // (not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
IMGUI_API void ImGui_SetWindowSize(const ImVec2& size, ImGuiSetCond cond = 0); // (not recommended) set current window size - call within Begin()/End(). set to ImVec2(0,0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.
IMGUI_API void ImGui_SetWindowCollapsed(bool collapsed, ImGuiSetCond cond = 0); // (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().
IMGUI_API void ImGui_SetWindowFocus(); // (not recommended) set current window to be focused / front-most. prefer using SetNextWindowFocus().
IMGUI_API void ImGui_SetWindowPos(const char* name, const ImVec2& pos, ImGuiSetCond cond = 0); // set named window position.
IMGUI_API void ImGui_SetWindowSize(const char* name, const ImVec2& size, ImGuiSetCond cond = 0); // set named window size. set axis to 0.0f to force an auto-fit on this axis.
IMGUI_API void ImGui_SetWindowCollapsed(const char* name, bool collapsed, ImGuiSetCond cond = 0); // set named window collapsed state
IMGUI_API void ImGui_SetWindowFocus(const char* name); // set named window to be focused / front-most. use NULL to remove focus.
IMGUI_API float ImGui_GetScrollX(); // get scrolling amount [0..GetScrollMaxX()]
IMGUI_API float ImGui_GetScrollY(); // get scrolling amount [0..GetScrollMaxY()]
IMGUI_API float ImGui_GetScrollMaxX(); // get maximum scrolling amount ~~ ContentSize.X - WindowSize.X
IMGUI_API float ImGui_GetScrollMaxY(); // get maximum scrolling amount ~~ ContentSize.Y - WindowSize.Y
IMGUI_API void ImGui_SetScrollX(float scroll_x); // set scrolling amount [0..GetScrollMaxX()]
IMGUI_API void ImGui_SetScrollY(float scroll_y); // set scrolling amount [0..GetScrollMaxY()]
IMGUI_API void ImGui_SetScrollHere(float center_y_ratio = 0.5f); // adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom.
IMGUI_API void ImGui_SetScrollFromPosY(float pos_y, float center_y_ratio = 0.5f); // adjust scrolling amount to make given position valid. use GetCursorPos() or GetCursorStartPos()+offset to get valid positions.
IMGUI_API void ImGui_SetKeyboardFocusHere(int offset = 0); // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use negative 'offset' to access previous widgets.
IMGUI_API void ImGui_SetStateStorage(ImGuiStorage* tree); // replace tree state storage with our own (if you want to manipulate it yourself, typically clear subsection of it)
IMGUI_API ImGuiStorage* ImGui_GetStateStorage();
// Parameters stacks (shared)
IMGUI_API void ImGui_PushFont(ImFont* font); // use NULL as a shortcut to push default font
IMGUI_API void ImGui_PopFont();
IMGUI_API void ImGui_PushStyleColor(ImGuiCol idx, const ImVec4& col);
IMGUI_API void ImGui_PopStyleColor(int count = 1);
IMGUI_API void ImGui_PushStyleVar(ImGuiStyleVar idx, float val);
IMGUI_API void ImGui_PushStyleVar2(ImGuiStyleVar idx, const ImVec2& val);
IMGUI_API void ImGui_PopStyleVar(int count = 1);
IMGUI_API ImFont* ImGui_GetFont(); // get current font
IMGUI_API float ImGui_GetFontSize(); // get current font size (= height in pixels) of current font with current scale applied
IMGUI_API ImVec2 ImGui_GetFontTexUvWhitePixel(); // get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API
IMGUI_API ImU32 ImGui_GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f); // retrieve given style color with style alpha applied and optional extra alpha multiplier
IMGUI_API ImU32 ImGui_GetColorU32(const ImVec4& col); // retrieve given color with style alpha applied
// Parameters stacks (current window)
IMGUI_API void ImGui_PushItemWidth(float item_width); // width of items for the common item+label case, pixels. 0.0f = default to ~2/3 of windows width, >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side)
IMGUI_API void ImGui_PopItemWidth();
IMGUI_API float ImGui_CalcItemWidth(); // width of item given pushed settings and current cursor position
IMGUI_API void ImGui_PushTextWrapPos(float wrap_pos_x = 0.0f); // word-wrapping for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
IMGUI_API void ImGui_PopTextWrapPos();
IMGUI_API void ImGui_PushAllowKeyboardFocus(bool v); // allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets
IMGUI_API void ImGui_PopAllowKeyboardFocus();
IMGUI_API void ImGui_PushButtonRepeat(bool repeat); // in 'repeat' mode, Button*() functions return repeated true in a typematic manner (uses io.KeyRepeatDelay/io.KeyRepeatRate for now). Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
IMGUI_API void ImGui_PopButtonRepeat();
// Cursor / Layout
IMGUI_API void ImGui_Separator(); // horizontal line
IMGUI_API void ImGui_SameLine(float pos_x = 0.0f, float spacing_w = -1.0f); // call between widgets or groups to layout them horizontally
IMGUI_API void ImGui_NewLine(); // undo a SameLine()
IMGUI_API void ImGui_Spacing(); // add vertical spacing
IMGUI_API void ImGui_Dummy(const ImVec2& size); // add a dummy item of given size
IMGUI_API void ImGui_Indent(float indent_w = 0.0f); // move content position toward the right, by style.IndentSpacing or indent_w if >0
IMGUI_API void ImGui_Unindent(float indent_w = 0.0f); // move content position back to the left, by style.IndentSpacing or indent_w if >0
IMGUI_API void ImGui_BeginGroup(); // lock horizontal starting position + capture group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
IMGUI_API void ImGui_EndGroup();
IMGUI_API ImVec2 ImGui_GetCursorPos(); // cursor position is relative to window position
IMGUI_API float ImGui_GetCursorPosX(); // "
IMGUI_API float ImGui_GetCursorPosY(); // "
IMGUI_API void ImGui_SetCursorPos(const ImVec2& local_pos); // "
IMGUI_API void ImGui_SetCursorPosX(float x); // "
IMGUI_API void ImGui_SetCursorPosY(float y); // "
IMGUI_API ImVec2 ImGui_GetCursorStartPos(); // initial cursor position
IMGUI_API ImVec2 ImGui_GetCursorScreenPos(); // cursor position in absolute screen coordinates [0..io.DisplaySize] (useful to work with ImDrawList API)
IMGUI_API void ImGui_SetCursorScreenPos(const ImVec2& pos); // cursor position in absolute screen coordinates [0..io.DisplaySize]
IMGUI_API void ImGui_AlignFirstTextHeightToWidgets(); // call once if the first item on the line is a Text() item and you want to vertically lower it to match subsequent (bigger) widgets
IMGUI_API float ImGui_GetTextLineHeight(); // height of font == GetWindowFontSize()
IMGUI_API float ImGui_GetTextLineHeightWithSpacing(); // distance (in pixels) between 2 consecutive lines of text == GetWindowFontSize() + GetStyle().ItemSpacing.y
IMGUI_API float ImGui_GetItemsLineHeightWithSpacing(); // distance (in pixels) between 2 consecutive lines of standard height widgets == GetWindowFontSize() + GetStyle().FramePadding.y*2 + GetStyle().ItemSpacing.y
// Columns
// You can also use SameLine(pos_x) for simplified columning. The columns API is still work-in-progress and rather lacking.
IMGUI_API void ImGui_Columns(int count DEFAULT(1), const char* id DEFAULT(NULL), bool border = true); // setup number of columns. use an identifier to distinguish multiple column sets. close with Columns(1).
IMGUI_API void ImGui_NextColumn(); // next column
IMGUI_API int ImGui_GetColumnIndex(); // get current column index
IMGUI_API float ImGui_GetColumnOffset(int column_index = -1); // get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetcolumnsCount() inclusive. column 0 is usually 0.0f and not resizable unless you call this
IMGUI_API void ImGui_SetColumnOffset(int column_index, float offset_x); // set position of column line (in pixels, from the left side of the contents region). pass -1 to use current column
IMGUI_API float ImGui_GetColumnWidth(int column_index = -1); // column width (== GetColumnOffset(GetColumnIndex()+1) - GetColumnOffset(GetColumnOffset())
IMGUI_API int ImGui_GetColumnsCount(); // number of columns (what was passed to Columns())
// ID scopes
// If you are creating widgets in a loop you most likely want to push a unique identifier so ImGui can differentiate them.
// You can also use the "##foobar" syntax within widget label to distinguish them from each others. Read "A primer on the use of labels/IDs" in the FAQ for more details.
IMGUI_API void ImGui_PushID(const char* str_id); // push identifier into the ID stack. IDs are hash of the *entire* stack!
IMGUI_API void ImGui_PushID(const char* str_id_begin, const char* str_id_end);
IMGUI_API void ImGui_PushID(const void* ptr_id);
IMGUI_API void ImGui_PushID(int int_id);
IMGUI_API void ImGui_PopID();
IMGUI_API ImGuiID ImGui_GetID(const char* str_id); // calculate unique ID (hash of whole ID stack + given parameter). useful if you want to query into ImGuiStorage yourself. otherwise rarely needed
IMGUI_API ImGuiID ImGui_GetID(const char* str_id_begin, const char* str_id_end);
IMGUI_API ImGuiID ImGui_GetID(const void* ptr_id);
// Widgets
IMGUI_API void ImGui_Text(const char* fmt, ...) IM_PRINTFARGS(1);
IMGUI_API void ImGui_TextV(const char* fmt, va_list args);
IMGUI_API void ImGui_TextColored(const ImVec4& col, const char* fmt, ...) IM_PRINTFARGS(2); // shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
IMGUI_API void ImGui_TextColoredV(const ImVec4& col, const char* fmt, va_list args);
IMGUI_API void ImGui_TextDisabled(const char* fmt, ...) IM_PRINTFARGS(1); // shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
IMGUI_API void ImGui_TextDisabledV(const char* fmt, va_list args);
IMGUI_API void ImGui_TextWrapped(const char* fmt, ...) IM_PRINTFARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
IMGUI_API void ImGui_TextWrappedV(const char* fmt, va_list args);
IMGUI_API void ImGui_TextUnformatted(const char* text, const char* text_end DEFAULT(NULL)); // doesn't require null terminated string if 'text_end' is specified. no copy done to any bounded stack buffer, recommended for long chunks of text
IMGUI_API void ImGui_LabelText(const char* label, const char* fmt, ...) IM_PRINTFARGS(2); // display text+label aligned the same way as value+label widgets
IMGUI_API void ImGui_LabelTextV(const char* label, const char* fmt, va_list args);
IMGUI_API void ImGui_Bullet(); // draw a small circle and keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses
IMGUI_API void ImGui_BulletText(const char* fmt, ...) IM_PRINTFARGS(1); // shortcut for Bullet()+Text()
IMGUI_API void ImGui_BulletTextV(const char* fmt, va_list args);
IMGUI_API bool ImGui_Button(const char* label, const ImVec2& size = fVec2(0, 0)); // button
IMGUI_API bool ImGui_SmallButton(const char* label); // button with FramePadding=(0,0)
IMGUI_API bool ImGui_InvisibleButton(const char* str_id, const ImVec2& size);
IMGUI_API void ImGui_Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = fVec2(0, 0), const ImVec2& uv1 = fVec2(1, 1), const ImVec4& tint_col = fVec4(1, 1, 1, 1), const ImVec4& border_col = fVec4(0, 0, 0, 0));
IMGUI_API bool ImGui_ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = fVec2(0, 0), const ImVec2& uv1 = fVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = fVec4(0, 0, 0, 0), const ImVec4& tint_col = fVec4(1, 1, 1, 1)); // <0 frame_padding uses default frame padding settings. 0 for no padding
IMGUI_API bool ImGui_Checkbox(const char* label, bool* v);
IMGUI_API bool ImGui_CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
IMGUI_API bool ImGui_RadioButton(const char* label, bool active);
IMGUI_API bool ImGui_RadioButton(const char* label, int* v, int v_button);
IMGUI_API bool ImGui_Combo(const char* label, int* current_item, const char** items, int items_count, int height_in_items = -1);
IMGUI_API bool ImGui_Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items = -1); // separate items with \0, end item-list with \0\0
IMGUI_API bool ImGui_Combo(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
IMGUI_API bool ImGui_ColorButton(const ImVec4& col, bool small_height = false, bool outline_border = true);
IMGUI_API bool ImGui_ColorEdit3(const char* label, float col[3]); // Hint: 'float col[3]' function argument is same as 'float* col'. You can pass address of first element out of a contiguous set, e.g. &myvector.x
IMGUI_API bool ImGui_ColorEdit4(const char* label, float col[4], bool show_alpha = true); // "
IMGUI_API void ImGui_ColorEditMode(ImGuiColorEditMode mode); // FIXME-OBSOLETE: This is inconsistent with most of the API and will be obsoleted/replaced.
IMGUI_API void ImGui_PlotLines(const char* label, const float* values, int values_count, int values_offset DEFAULT(0), const char* overlay_text DEFAULT(NULL), float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = fVec2(0, 0), int stride = sizeof(float));
IMGUI_API void ImGui_PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset DEFAULT(0), const char* overlay_text DEFAULT(NULL), float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = fVec2(0, 0));
IMGUI_API void ImGui_PlotHistogram(const char* label, const float* values, int values_count, int values_offset DEFAULT(0), const char* overlay_text DEFAULT(NULL), float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = fVec2(0, 0), int stride = sizeof(float));
IMGUI_API void ImGui_PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset DEFAULT(0), const char* overlay_text DEFAULT(NULL), float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = fVec2(0, 0));
IMGUI_API void ImGui_ProgressBar(float fraction, const ImVec2* psize_arg, const char* overlay DEFAULT(NULL));
// Widgets: Drags (tip: ctrl+click on a drag box to input with keyboard. manually input values aren't clamped, can go off-bounds)
// For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every functions, remember than a 'float v[3]' function argument is the same as 'float* v'. You can pass address of your first element out of a contiguous set, e.g. &myvector.x
IMGUI_API bool ImGui_DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f); // If v_min >= v_max we have no bound
IMGUI_API bool ImGui_DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
IMGUI_API bool ImGui_DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
IMGUI_API bool ImGui_DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
IMGUI_API bool ImGui_DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed DEFAULT(1.0f), float v_min DEFAULT(0.0f), float v_max DEFAULT(0.0f), const char* display_format DEFAULT("%.3f"), const char* display_format_max DEFAULT(NULL), float power = 1.0f);
IMGUI_API bool ImGui_DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f"); // If v_min >= v_max we have no bound
IMGUI_API bool ImGui_DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
IMGUI_API bool ImGui_DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
IMGUI_API bool ImGui_DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");
IMGUI_API bool ImGui_DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed DEFAULT(1.0f), int v_min DEFAULT(0), int v_max DEFAULT(0), const char* display_format DEFAULT("%.0f"), const char* display_format_max DEFAULT(NULL));

#undef DEFAULT
#ifdef __cplusplus
#define DEFAULT(x) =x
#else
typedef int bool;
#define DEFAULT(x)
#endif
// Widgets: Input with Keyboard
IMGUI_API bool ImGui_InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback DEFAULT(NULL), void* user_data DEFAULT(NULL));
IMGUI_API bool ImGui_InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = fVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback DEFAULT(NULL), void* user_data DEFAULT(NULL));
IMGUI_API bool ImGui_InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
IMGUI_API bool ImGui_InputFloat2(const char* label, float v[2], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
IMGUI_API bool ImGui_InputFloat3(const char* label, float v[3], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
IMGUI_API bool ImGui_InputFloat4(const char* label, float v[4], int decimal_precision = -1, ImGuiInputTextFlags extra_flags = 0);
IMGUI_API bool ImGui_InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags extra_flags = 0);
IMGUI_API bool ImGui_InputInt2(const char* label, int v[2], ImGuiInputTextFlags extra_flags = 0);
IMGUI_API bool ImGui_InputInt3(const char* label, int v[3], ImGuiInputTextFlags extra_flags = 0);
IMGUI_API bool ImGui_InputInt4(const char* label, int v[4], ImGuiInputTextFlags extra_flags = 0);
// Widgets: Sliders (tip: ctrl+click on a slider to input with keyboard. manually input values aren't clamped, can go off-bounds)
IMGUI_API bool ImGui_SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f); // adjust display_format to decorate the value with a prefix or a suffix. Use power!=1.0 for logarithmic sliders
IMGUI_API bool ImGui_SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
IMGUI_API bool ImGui_SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
IMGUI_API bool ImGui_SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
IMGUI_API bool ImGui_SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f);
IMGUI_API bool ImGui_SliderInt(const char* label, int* v, int v_min, int v_max, const char* display_format = "%.0f");
IMGUI_API bool ImGui_SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* display_format = "%.0f");
IMGUI_API bool ImGui_SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* display_format = "%.0f");
IMGUI_API bool ImGui_SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* display_format = "%.0f");
IMGUI_API bool ImGui_VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f);
IMGUI_API bool ImGui_VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* display_format = "%.0f");
// Widgets: Trees
IMGUI_API bool ImGui_TreeNode(const char* label); // if returning 'true' the node is open and the tree id is pushed into the id stack. user is responsible for calling TreePop().
IMGUI_API bool ImGui_TreeNode(const char* str_id, const char* fmt, ...) IM_PRINTFARGS(2); // read the FAQ about why and how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().
IMGUI_API bool ImGui_TreeNode(const void* ptr_id, const char* fmt, ...) IM_PRINTFARGS(2); // "
IMGUI_API bool ImGui_TreeNodeV(const char* str_id, const char* fmt, va_list args); // "
IMGUI_API bool ImGui_TreeNodeV(const void* ptr_id, const char* fmt, va_list args); // "
IMGUI_API bool ImGui_TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
IMGUI_API bool ImGui_TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_PRINTFARGS(3);
IMGUI_API bool ImGui_TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_PRINTFARGS(3);
IMGUI_API bool ImGui_TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
IMGUI_API bool ImGui_TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args);
IMGUI_API void ImGui_TreePush(const char* str_id DEFAULT(NULL)); // ~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can call Push/Pop yourself for layout purpose
IMGUI_API void ImGui_TreePush(const void* ptr_id DEFAULT(NULL)); // "
IMGUI_API void ImGui_TreePop(); // ~ Unindent()+PopId()
IMGUI_API void ImGui_TreeAdvanceToLabelPos(); // advance cursor x position by GetTreeNodeToLabelSpacing()
IMGUI_API float ImGui_GetTreeNodeToLabelSpacing(); // horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode
IMGUI_API void ImGui_SetNextTreeNodeOpen(bool is_open, ImGuiSetCond cond = 0); // set next TreeNode/CollapsingHeader open state.
IMGUI_API bool ImGui_CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0); // if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().
IMGUI_API bool ImGui_CollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags = 0); // when 'p_open' isn't NULL, display an additional small close button on upper right of the header
// Widgets: Selectable / Lists
IMGUI_API bool ImGui_Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = fVec2(0, 0)); // size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
IMGUI_API bool ImGui_Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = fVec2(0, 0));
IMGUI_API bool ImGui_ListBox(const char* label, int* current_item, const char** items, int items_count, int height_in_items = -1);
IMGUI_API bool ImGui_ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
IMGUI_API bool ImGui_ListBoxHeader(const char* label, const ImVec2& size = fVec2(0, 0)); // use if you want to reimplement ListBox() will custom data or interactions. make sure to call ListBoxFooter() afterwards.
IMGUI_API bool ImGui_ListBoxHeader(const char* label, int items_count, int height_in_items = -1); // "
IMGUI_API void ImGui_ListBoxFooter(); // terminate the scrolling region
// Widgets: Value() Helpers. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
IMGUI_API void ImGui_Value(const char* prefix, bool b);
IMGUI_API void ImGui_Value(const char* prefix, int v);
IMGUI_API void ImGui_Value(const char* prefix, unsigned int v);
IMGUI_API void ImGui_Value(const char* prefix, float v, const char* float_format DEFAULT(NULL));
IMGUI_API void ImGui_ValueColor(const char* prefix, const ImVec4& v);
IMGUI_API void ImGui_ValueColor(const char* prefix, ImU32 v);
// Tooltips
IMGUI_API void ImGui_SetTooltip(const char* fmt, ...) IM_PRINTFARGS(1); // set tooltip under mouse-cursor, typically use with ImGui_IsHovered(). last call wins
IMGUI_API void ImGui_SetTooltipV(const char* fmt, va_list args);
IMGUI_API void ImGui_BeginTooltip(); // use to create full-featured tooltip windows that aren't just text
IMGUI_API void ImGui_EndTooltip();
// Menus
IMGUI_API bool ImGui_BeginMainMenuBar(); // create and append to a full screen menu-bar. only call EndMainMenuBar() if this returns true!
IMGUI_API void ImGui_EndMainMenuBar();
IMGUI_API bool ImGui_BeginMenuBar(); // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set). only call EndMenuBar() if this returns true!
IMGUI_API void ImGui_EndMenuBar();
IMGUI_API bool ImGui_BeginMenu(const char* label, bool enabled = true); // create a sub-menu entry. only call EndMenu() if this returns true!
IMGUI_API void ImGui_EndMenu();
IMGUI_API bool ImGui_MenuItem(const char* label, const char* shortcut DEFAULT(NULL), bool selected = false, bool enabled = true); // return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment
IMGUI_API bool ImGui_MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true); // return true when activated + toggle (*p_selected) if p_selected !DEFAULT(NULL)
// Popups
IMGUI_API void ImGui_OpenPopup(const char* str_id); // mark popup as open. popups are closed when user click outside, or activate a pressable item, or CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block. popup identifiers are relative to the current ID-stack (so OpenPopup and BeginPopup needs to be at the same level).
IMGUI_API bool ImGui_BeginPopup(const char* str_id); // return true if the popup is open, and you can start outputting to it. only call EndPopup() if BeginPopup() returned true!
IMGUI_API bool ImGui_BeginPopupModal(const char* name, bool* p_open DEFAULT(NULL), ImGuiWindowFlags extra_flags = 0); // modal dialog (can't close them by clicking outside)
IMGUI_API bool ImGui_BeginPopupContextItem(const char* str_id, int mouse_button = 1); // helper to open and begin popup when clicked on last item. read comments in .cpp!
IMGUI_API bool ImGui_BeginPopupContextWindow(bool also_over_items = true, const char* str_id DEFAULT(NULL), int mouse_button = 1); // helper to open and begin popup when clicked on current window.
IMGUI_API bool ImGui_BeginPopupContextVoid(const char* str_id DEFAULT(NULL), int mouse_button = 1); // helper to open and begin popup when clicked in void (no window).
IMGUI_API void ImGui_EndPopup();
IMGUI_API void ImGui_CloseCurrentPopup(); // close the popup we have begin-ed into. clicking on a MenuItem or Selectable automatically close the current popup.
// Logging: all text output from interface is redirected to tty/file/clipboard. By default, tree nodes are automatically opened during logging.
IMGUI_API void ImGui_LogToTTY(int max_depth = -1); // start logging to tty
IMGUI_API void ImGui_LogToFile(int max_depth = -1, const char* filename DEFAULT(NULL)); // start logging to file
IMGUI_API void ImGui_LogToClipboard(int max_depth = -1); // start logging to OS clipboard
IMGUI_API void ImGui_LogFinish(); // stop logging (close file, etc.)
IMGUI_API void ImGui_LogButtons(); // helper to display buttons for logging to tty/file/clipboard
IMGUI_API void ImGui_LogText(const char* fmt, ...) IM_PRINTFARGS(1); // pass text data straight to log (without being displayed)
// Clipping
IMGUI_API void ImGui_PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
IMGUI_API void ImGui_PopClipRect();
// Utilities
IMGUI_API bool ImGui_IsItemHovered(); // was the last item hovered by mouse?
IMGUI_API bool ImGui_IsItemHoveredRect(); // was the last item hovered by mouse? even if another item is active or window is blocked by popup while we are hovering this
IMGUI_API bool ImGui_IsItemActive(); // was the last item active? (e.g. button being held, text field being edited- items that don't interact will always return false)
IMGUI_API bool ImGui_IsItemClicked(int mouse_button = 0); // was the last item clicked? (e.g. button/node just clicked on)
IMGUI_API bool ImGui_IsItemVisible(); // was the last item visible? (aka not out of sight due to clipping/scrolling.)
IMGUI_API bool ImGui_IsAnyItemHovered();
IMGUI_API bool ImGui_IsAnyItemActive();
IMGUI_API ImVec2 ImGui_GetItemRectMin(); // get bounding rect of last item in screen space
IMGUI_API ImVec2 ImGui_GetItemRectMax(); // "
IMGUI_API ImVec2 ImGui_GetItemRectSize(); // "
IMGUI_API void ImGui_SetItemAllowOverlap(); // allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.
IMGUI_API bool ImGui_IsWindowHovered(); // is current window hovered and hoverable (not blocked by a popup) (differentiate child windows from each others)
IMGUI_API bool ImGui_IsWindowFocused(); // is current window focused
IMGUI_API bool ImGui_IsRootWindowFocused(); // is current root window focused (root = top-most parent of a child, otherwise self)
IMGUI_API bool ImGui_IsRootWindowOrAnyChildFocused(); // is current root window or any of its child (including current window) focused
IMGUI_API bool ImGui_IsRootWindowOrAnyChildHovered(); // is current root window or any of its child (including current window) hovered and hoverable (not blocked by a popup)
IMGUI_API bool ImGui_IsRectVisible(const ImVec2& size); // test if rectangle (of given size, starting from cursor position) is visible / not clipped.
IMGUI_API bool ImGui_IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max); // test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.
IMGUI_API bool ImGui_IsPosHoveringAnyWindow(const ImVec2& pos); // is given position hovering any active imgui window
IMGUI_API float ImGui_GetTime();
IMGUI_API int ImGui_GetFrameCount();
IMGUI_API const char* ImGui_GetStyleColName(ImGuiCol idx);
IMGUI_API ImVec2 ImGui_CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge = false, float outward = +0.0f); // utility to find the closest point the last item bounding rectangle edge. useful to visually link items
IMGUI_API ImVec2 ImGui_CalcTextSize(const char* text, const char* text_end DEFAULT(NULL), bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
IMGUI_API void ImGui_CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end); // calculate coarse clipping for large list of evenly sized items. Prefer using the ImGuiListClipper higher-level helper if you can.
IMGUI_API bool ImGui_BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags = 0); // helper to create a child window / scrolling region that looks like a normal widget frame
IMGUI_API void ImGui_EndChildFrame();
IMGUI_API ImVec4 ImGui_ColorConvertU32ToFloat4(ImU32 in);
IMGUI_API ImU32 ImGui_ColorConvertFloat4ToU32(const ImVec4& in);
IMGUI_API void ImGui_ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
IMGUI_API void ImGui_ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);
// Inputs
IMGUI_API int ImGui_GetKeyIndex(ImGuiKey key); // map ImGuiKey_* values into user's key index. == io.KeyMap[key]
IMGUI_API bool ImGui_IsKeyDown(int key_index); // key_index into the keys_down[] array, imgui doesn't know the semantic of each entry, uses your own indices!
IMGUI_API bool ImGui_IsKeyPressed(int key_index, bool repeat = true); // uses user's key indices as stored in the keys_down[] array. if repeat=true. uses io.KeyRepeatDelay / KeyRepeatRate
IMGUI_API bool ImGui_IsKeyReleased(int key_index); // "
IMGUI_API bool ImGui_IsMouseDown(int button); // is mouse button held
IMGUI_API bool ImGui_IsMouseClicked(int button, bool repeat = false); // did mouse button clicked (went from !Down to Down)
IMGUI_API bool ImGui_IsMouseDoubleClicked(int button); // did mouse button double-clicked. a double-click returns false in IsMouseClicked(). uses io.MouseDoubleClickTime.
IMGUI_API bool ImGui_IsMouseReleased(int button); // did mouse button released (went from Down to !Down)
IMGUI_API bool ImGui_IsMouseHoveringWindow(); // is mouse hovering current window ("window" in API names always refer to current window). disregarding of any consideration of being blocked by a popup. (unlike IsWindowHovered() this will return true even if the window is blocked because of a popup)
IMGUI_API bool ImGui_IsMouseHoveringAnyWindow(); // is mouse hovering any visible window
IMGUI_API bool ImGui_IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true); // is mouse hovering given bounding rect (in screen space). clipped by current clipping settings. disregarding of consideration of focus/window ordering/blocked by a popup.
IMGUI_API bool ImGui_IsMouseDragging(int button = 0, float lock_threshold = -1.0f); // is mouse dragging. if lock_threshold < -1.0f uses io.MouseDraggingThreshold
IMGUI_API ImVec2 ImGui_GetMousePos(); // shortcut to ImGui_GetIO().MousePos provided by user, to be consistent with other calls
IMGUI_API ImVec2 ImGui_GetMousePosOnOpeningCurrentPopup(); // retrieve backup of mouse positioning at the time of opening popup we have BeginPopup() into
IMGUI_API ImVec2 ImGui_GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f); // dragging amount since clicking. if lock_threshold < -1.0f uses io.MouseDraggingThreshold
IMGUI_API void ImGui_ResetMouseDragDelta(int button = 0); //
IMGUI_API ImGuiMouseCursor ImGui_GetMouseCursor(); // get desired cursor type, reset in ImGui_NewFrame(), this updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
IMGUI_API void ImGui_SetMouseCursor(ImGuiMouseCursor type); // set desired cursor type
IMGUI_API void ImGui_CaptureKeyboardFromApp(bool capture = true); // manually override io.WantCaptureKeyboard flag next frame (said flag is entirely left for your application handle). e.g. force capture keyboard when your widget is being hovered.
IMGUI_API void ImGui_CaptureMouseFromApp(bool capture = true); // manually override io.WantCaptureMouse flag next frame (said flag is entirely left for your application handle).
// Helpers functions to access functions pointers in ImGui_GetIO()
IMGUI_API void* ImGui_MemAlloc(size_t sz);
IMGUI_API void ImGui_MemFree(void* ptr);
IMGUI_API const char* ImGui_GetClipboardText();
IMGUI_API void ImGui_SetClipboardText(const char* text);
// Internal context access - if you want to use multiple context, share context between modules (e.g. DLL). There is a default context created and active by default.
// All contexts share a same ImFontAtlas by default. If you want different font atlas, you can new() them and overwrite the GetIO().Fonts variable of an ImGui context.
IMGUI_API const char* ImGui_GetVersion();
IMGUI_API ImGuiContext* ImGui_CreateContext(void * (*malloc_fn)(size_t) DEFAULT(NULL), void (*free_fn)(void*) DEFAULT(NULL));
IMGUI_API void ImGui_DestroyContext(ImGuiContext* ctx);
IMGUI_API ImGuiContext* ImGui_GetCurrentContext();
IMGUI_API void ImGui_SetCurrentContext(ImGuiContext* ctx);
// Obsolete (will be removed)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
static inline bool ImGui_CollapsingHeader(const char* label, const char* str_id, bool framed = true, bool default_open = false)
{
  (void)str_id; // OBSOLETE 1.49+
  (void)framed;
  ImGuiTreeNodeFlags default_open_flags = 1 << 5;
  return ImGui_CollapsingHeader(label, (default_open ? default_open_flags : 0));
}
static inline ImFont* ImGui_GetWindowFont()
{
  return ImGui_GetFont(); // OBSOLETE 1.48+
}
static inline float ImGui_GetWindowFontSize()
{
  return ImGui_GetFontSize(); // OBSOLETE 1.48+
}
static inline void ImGui_SetScrollPosHere()
{
  ImGui_SetScrollHere(); // OBSOLETE 1.42+
}
static inline bool ImGui_GetWindowCollapsed()
{
  return ImGui_IsWindowCollapsed(); // OBSOLETE 1.39+
}
static inline bool ImGui_IsRectClipped(const ImVec2& size)
{
  return !ImGui_IsRectVisible(size); // OBSOLETE 1.39+
}
#endif
// namespace ImGui
// Flags for ImGui_Begin()
enum ImGuiWindowFlags_ {
  // Default: 0
  ImGuiWindowFlags_NoTitleBar = 1 << 0, // Disable title-bar
  ImGuiWindowFlags_NoResize = 1 << 1, // Disable user resizing with the lower-right grip
  ImGuiWindowFlags_NoMove = 1 << 2, // Disable user moving the window
  ImGuiWindowFlags_NoScrollbar = 1 << 3, // Disable scrollbars (window can still scroll with mouse or programatically)
  ImGuiWindowFlags_NoScrollWithMouse = 1 << 4, // Disable user vertically scrolling with mouse wheel
  ImGuiWindowFlags_NoCollapse = 1 << 5, // Disable user collapsing window by double-clicking on it
  ImGuiWindowFlags_AlwaysAutoResize = 1 << 6, // Resize every window to its content every frame
  ImGuiWindowFlags_ShowBorders = 1 << 7, // Show borders around windows and items
  ImGuiWindowFlags_NoSavedSettings = 1 << 8, // Never load/save settings in .ini file
  ImGuiWindowFlags_NoInputs = 1 << 9, // Disable catching mouse or keyboard inputs
  ImGuiWindowFlags_MenuBar = 1 << 10, // Has a menu-bar
  ImGuiWindowFlags_HorizontalScrollbar = 1 << 11, // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
  ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12, // Disable taking focus when transitioning from hidden to visible state
  ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13, // Disable bringing window to front when taking focus (e.g. clicking on it or programatically giving it focus)
  ImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14, // Always show vertical scrollbar (even if ContentSize.y < Size.y)
  ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15, // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
  ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16, // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
  // [Internal]
  ImGuiWindowFlags_ChildWindow = 1 << 20, // Don't use! For internal use by BeginChild()
  ImGuiWindowFlags_ChildWindowAutoFitX = 1 << 21, // Don't use! For internal use by BeginChild()
  ImGuiWindowFlags_ChildWindowAutoFitY = 1 << 22, // Don't use! For internal use by BeginChild()
  ImGuiWindowFlags_ComboBox = 1 << 23, // Don't use! For internal use by ComboBox()
  ImGuiWindowFlags_Tooltip = 1 << 24, // Don't use! For internal use by BeginTooltip()
  ImGuiWindowFlags_Popup = 1 << 25, // Don't use! For internal use by BeginPopup()
  ImGuiWindowFlags_Modal = 1 << 26, // Don't use! For internal use by BeginPopupModal()
  ImGuiWindowFlags_ChildMenu = 1 << 27 // Don't use! For internal use by BeginMenu()
};
// Flags for ImGui_InputText()
enum ImGuiInputTextFlags_ {
  // Default: 0
  ImGuiInputTextFlags_CharsDecimal = 1 << 0, // Allow 0123456789.+-*/
  ImGuiInputTextFlags_CharsHexadecimal = 1 << 1, // Allow 0123456789ABCDEFabcdef
  ImGuiInputTextFlags_CharsUppercase = 1 << 2, // Turn a..z into A..Z
  ImGuiInputTextFlags_CharsNoBlank = 1 << 3, // Filter out spaces, tabs
  ImGuiInputTextFlags_AutoSelectAll = 1 << 4, // Select entire text when first taking mouse focus
  ImGuiInputTextFlags_EnterReturnsTrue = 1 << 5, // Return 'true' when Enter is pressed (as opposed to when the value was modified)
  ImGuiInputTextFlags_CallbackCompletion = 1 << 6, // Call user function on pressing TAB (for completion handling)
  ImGuiInputTextFlags_CallbackHistory = 1 << 7, // Call user function on pressing Up/Down arrows (for history handling)
  ImGuiInputTextFlags_CallbackAlways = 1 << 8, // Call user function every time. User code may query cursor position, modify text buffer.
  ImGuiInputTextFlags_CallbackCharFilter = 1 << 9, // Call user function to filter character. Modify data->EventChar to replace/filter input, or return 1 to discard character.
  ImGuiInputTextFlags_AllowTabInput = 1 << 10, // Pressing TAB input a '\t' character into the text field
  ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11, // In multi-line mode, allow exiting edition by pressing Enter. Ctrl+Enter to add new line (by default adds new lines with Enter).
  ImGuiInputTextFlags_NoHorizontalScroll = 1 << 12, // Disable following the cursor horizontally
  ImGuiInputTextFlags_AlwaysInsertMode = 1 << 13, // Insert mode
  ImGuiInputTextFlags_ReadOnly = 1 << 14, // Read-only mode
  ImGuiInputTextFlags_Password = 1 << 15, // Password mode, display all characters as '*'
  // [Internal]
  ImGuiInputTextFlags_Multiline = 1 << 20 // For internal use by InputTextMultiline()
};
// Flags for ImGui_TreeNodeEx(), ImGui_CollapsingHeader*()
enum ImGuiTreeNodeFlags_ {
  ImGuiTreeNodeFlags_Selected = 1 << 0, // Draw as selected
  ImGuiTreeNodeFlags_Framed = 1 << 1, // Full colored frame (e.g. for CollapsingHeader)
  ImGuiTreeNodeFlags_AllowOverlapMode = 1 << 2, // Hit testing to allow subsequent widgets to overlap this one
  ImGuiTreeNodeFlags_NoTreePushOnOpen = 1 << 3, // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
  ImGuiTreeNodeFlags_NoAutoOpenOnLog = 1 << 4, // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
  ImGuiTreeNodeFlags_DefaultOpen = 1 << 5, // Default node to be open
  ImGuiTreeNodeFlags_OpenOnDoubleClick = 1 << 6, // Need double-click to open node
  ImGuiTreeNodeFlags_OpenOnArrow = 1 << 7, // Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
  ImGuiTreeNodeFlags_Leaf = 1 << 8, // No collapsing, no arrow (use as a convenience for leaf nodes).
  ImGuiTreeNodeFlags_Bullet = 1 << 9, // Display a bullet instead of arrow
  //ImGuITreeNodeFlags_SpanAllAvailWidth = 1 << 10, // FIXME: TODO: Extend hit box horizontally even if not framed
  //ImGuiTreeNodeFlags_NoScrollOnOpen = 1 << 11, // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
  ImGuiTreeNodeFlags_CollapsingHeader = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog
};
// Flags for ImGui_Selectable()
enum ImGuiSelectableFlags_ {
  // Default: 0
  ImGuiSelectableFlags_DontClosePopups = 1 << 0, // Clicking this don't close parent popup window
  ImGuiSelectableFlags_SpanAllColumns = 1 << 1, // Selectable frame can span all columns (text will still fit in current column)
  ImGuiSelectableFlags_AllowDoubleClick = 1 << 2 // Generate press events on double clicks too
};
// User fill ImGuiIO.KeyMap[] array with indices into the ImGuiIO.KeysDown[512] array
enum ImGuiKey_ {
  ImGuiKey_Tab, // for tabbing through fields
  ImGuiKey_LeftArrow, // for text edit
  ImGuiKey_RightArrow,// for text edit
  ImGuiKey_UpArrow, // for text edit
  ImGuiKey_DownArrow, // for text edit
  ImGuiKey_PageUp,
  ImGuiKey_PageDown,
  ImGuiKey_Home, // for text edit
  ImGuiKey_End, // for text edit
  ImGuiKey_Delete, // for text edit
  ImGuiKey_Backspace, // for text edit
  ImGuiKey_Enter, // for text edit
  ImGuiKey_Escape, // for text edit
  ImGuiKey_A, // for text edit CTRL+A: select all
  ImGuiKey_C, // for text edit CTRL+C: copy
  ImGuiKey_V, // for text edit CTRL+V: paste
  ImGuiKey_X, // for text edit CTRL+X: cut
  ImGuiKey_Y, // for text edit CTRL+Y: redo
  ImGuiKey_Z, // for text edit CTRL+Z: undo
  ImGuiKey_COUNT
};
// Enumeration for PushStyleColor() / PopStyleColor()
enum ImGuiCol_ {
  ImGuiCol_Text,
  ImGuiCol_TextDisabled,
  ImGuiCol_WindowBg, // Background of normal windows
  ImGuiCol_ChildWindowBg, // Background of child windows
  ImGuiCol_PopupBg, // Background of popups, menus, tooltips windows
  ImGuiCol_Border,
  ImGuiCol_BorderShadow,
  ImGuiCol_FrameBg, // Background of checkbox, radio button, plot, slider, text input
  ImGuiCol_FrameBgHovered,
  ImGuiCol_FrameBgActive,
  ImGuiCol_TitleBg,
  ImGuiCol_TitleBgCollapsed,
  ImGuiCol_TitleBgActive,
  ImGuiCol_MenuBarBg,
  ImGuiCol_ScrollbarBg,
  ImGuiCol_ScrollbarGrab,
  ImGuiCol_ScrollbarGrabHovered,
  ImGuiCol_ScrollbarGrabActive,
  ImGuiCol_ComboBg,
  ImGuiCol_CheckMark,
  ImGuiCol_SliderGrab,
  ImGuiCol_SliderGrabActive,
  ImGuiCol_Button,
  ImGuiCol_ButtonHovered,
  ImGuiCol_ButtonActive,
  ImGuiCol_Header,
  ImGuiCol_HeaderHovered,
  ImGuiCol_HeaderActive,
  ImGuiCol_Column,
  ImGuiCol_ColumnHovered,
  ImGuiCol_ColumnActive,
  ImGuiCol_ResizeGrip,
  ImGuiCol_ResizeGripHovered,
  ImGuiCol_ResizeGripActive,
  ImGuiCol_CloseButton,
  ImGuiCol_CloseButtonHovered,
  ImGuiCol_CloseButtonActive,
  ImGuiCol_PlotLines,
  ImGuiCol_PlotLinesHovered,
  ImGuiCol_PlotHistogram,
  ImGuiCol_PlotHistogramHovered,
  ImGuiCol_TextSelectedBg,
  ImGuiCol_ModalWindowDarkening, // darken entire screen when a modal window is active
  ImGuiCol_COUNT
};
// Enumeration for PushStyleVar() / PopStyleVar()
// NB: the enum only refers to fields of ImGuiStyle() which makes sense to be pushed/poped in UI code. Feel free to add others.
enum ImGuiStyleVar_ {
  ImGuiStyleVar_Alpha, // float
  ImGuiStyleVar_WindowPadding, // ImVec2
  ImGuiStyleVar_WindowRounding, // float
  ImGuiStyleVar_WindowMinSize, // ImVec2
  ImGuiStyleVar_ChildWindowRounding, // float
  ImGuiStyleVar_FramePadding, // ImVec2
  ImGuiStyleVar_FrameRounding, // float
  ImGuiStyleVar_ItemSpacing, // ImVec2
  ImGuiStyleVar_ItemInnerSpacing, // ImVec2
  ImGuiStyleVar_IndentSpacing, // float
  ImGuiStyleVar_GrabMinSize, // float
  ImGuiStyleVar_ButtonTextAlign, // flags ImGuiAlign_*
  ImGuiStyleVar_Count_
};
// Enumeration for ColorEditMode()
// FIXME-OBSOLETE: Will be replaced by future color/picker api
enum ImGuiColorEditMode_ {
  ImGuiColorEditMode_UserSelect = -2,
  ImGuiColorEditMode_UserSelectShowButton = -1,
  ImGuiColorEditMode_RGB = 0,
  ImGuiColorEditMode_HSV = 1,
  ImGuiColorEditMode_HEX = 2
};
// Enumeration for GetMouseCursor()
enum ImGuiMouseCursor_ {
  ImGuiMouseCursor_Arrow = 0,
  ImGuiMouseCursor_TextInput, // When hovering over InputText, etc.
  ImGuiMouseCursor_Move, // Unused
  ImGuiMouseCursor_ResizeNS, // Unused
  ImGuiMouseCursor_ResizeEW, // When hovering over a column
  ImGuiMouseCursor_ResizeNESW, // Unused
  ImGuiMouseCursor_ResizeNWSE, // When hovering over the bottom-right corner of a window
  ImGuiMouseCursor_Count_
};
// Condition flags for ImGui_SetWindow***(), SetNextWindow***(), SetNextTreeNode***() functions
// All those functions treat 0 as a shortcut to ImGuiSetCond_Always
enum ImGuiSetCond_ {
  ImGuiSetCond_Always = 1 << 0, // Set the variable
  ImGuiSetCond_Once = 1 << 1, // Set the variable once per runtime session (only the first call with succeed)
  ImGuiSetCond_FirstUseEver = 1 << 2, // Set the variable if the window has no saved data (if doesn't exist in the .ini file)
  ImGuiSetCond_Appearing = 1 << 3 // Set the variable if the window is appearing after being hidden/inactive (or the first time)
};
struct ImGuiStyle {
  float Alpha; // Global alpha applies to everything in ImGui
  ImVec2 WindowPadding; // Padding within a window
  ImVec2 WindowMinSize; // Minimum window size
  float WindowRounding; // Radius of window corners rounding. Set to 0.0f to have rectangular windows
  ImVec2 WindowTitleAlign; // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
  float ChildWindowRounding; // Radius of child window corners rounding. Set to 0.0f to have rectangular windows
  ImVec2 FramePadding; // Padding within a framed rectangle (used by most widgets)
  float FrameRounding; // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
  ImVec2 ItemSpacing; // Horizontal and vertical spacing between widgets/lines
  ImVec2 ItemInnerSpacing; // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
  ImVec2 TouchExtraPadding; // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
  float IndentSpacing; // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
  float ColumnsMinSpacing; // Minimum horizontal spacing between two columns
  float ScrollbarSize; // Width of the vertical scrollbar, Height of the horizontal scrollbar
  float ScrollbarRounding; // Radius of grab corners for scrollbar
  float GrabMinSize; // Minimum width/height of a grab box for slider/scrollbar.
  float GrabRounding; // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
  ImVec2 ButtonTextAlign; // Alignment of button text when button is larger than text. Defaults to (0.5f,0.5f) for horizontally+vertically centered.
  ImVec2 DisplayWindowPadding; // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
  ImVec2 DisplaySafeAreaPadding; // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
  bool AntiAliasedLines; // Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.
  bool AntiAliasedShapes; // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
  float CurveTessellationTol; // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
  ImVec4 Colors[ImGuiCol_COUNT];
  IMGUI_API ImGuiStyle();
};
//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------
// Lightweight std::vector<> like class to avoid dragging dependencies (also: windows implementation of STL with debug enabled is absurdly slow, so let's bypass it so our code runs fast in debug).
// Our implementation does NOT call c++ constructors because we don't use them in ImGui. Don't use this class as a straight std::vector replacement in your code!
template<typename T>
class ImVector
{
public:
  int Size;
  int Capacity;
  T* Data;
  typedef T value_type;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  ImVector() {
    Size = Capacity = 0;
    Data = NULL;
  }
  ~ImVector() {
    if (Data) {
      ImGui_MemFree(Data);
    }
  }
  inline bool empty() const {
    return Size == 0;
  }
  inline int size() const {
    return Size;
  }
  inline int capacity() const {
    return Capacity;
  }
  inline value_type& operator[](int i) {
    IM_ASSERT(i < Size);
    return Data[i];
  }
  inline const value_type& operator[](int i) const {
    IM_ASSERT(i < Size);
    return Data[i];
  }
  inline void clear() {
    if (Data) {
      Size = Capacity = 0;
      ImGui_MemFree(Data);
      Data = NULL;
    }
  }
  inline iterator begin() {
    return Data;
  }
  inline const_iterator begin() const {
    return Data;
  }
  inline iterator end() {
    return Data + Size;
  }
  inline const_iterator end() const {
    return Data + Size;
  }
  inline value_type& front() {
    IM_ASSERT(Size > 0);
    return Data[0];
  }
  inline const value_type& front() const {
    IM_ASSERT(Size > 0);
    return Data[0];
  }
  inline value_type& back() {
    IM_ASSERT(Size > 0);
    return Data[Size - 1];
  }
  inline const value_type& back() const {
    IM_ASSERT(Size > 0);
    return Data[Size - 1];
  }
  inline void swap(ImVector<T>& rhs) {
    int rhs_size = rhs.Size;
    rhs.Size = Size;
    Size = rhs_size;
    int rhs_cap = rhs.Capacity;
    rhs.Capacity = Capacity;
    Capacity = rhs_cap;
    value_type* rhs_data = rhs.Data;
    rhs.Data = Data;
    Data = rhs_data;
  }
  inline int _grow_capacity(int new_size) {
    int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8;
    return new_capacity > new_size ? new_capacity : new_size;
  }
  inline void resize(int new_size) {
    if (new_size > Capacity) {
      reserve(_grow_capacity(new_size));
    }

    Size = new_size;
  }
  inline void reserve(int new_capacity) {
    if (new_capacity <= Capacity) {
      return;
    }

    T* new_data = (value_type*)ImGui_MemAlloc((size_t)new_capacity * sizeof(value_type));

    if (Data) {
      memcpy(new_data, Data, (size_t)Size * sizeof(value_type));
    }

    ImGui_MemFree(Data);
    Data = new_data;
    Capacity = new_capacity;
  }
  inline void push_back(const value_type& v) {
    if (Size == Capacity) {
      reserve(_grow_capacity(Size + 1));
    }

    Data[Size++] = v;
  }
  inline void pop_back() {
    IM_ASSERT(Size > 0);
    Size--;
  }
  inline iterator erase(const_iterator it) {
    IM_ASSERT(it >= Data && it < Data + Size);
    const ptrdiff_t off = it - Data;
    memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(value_type));
    Size--;
    return Data + off;
  }
  inline iterator insert(const_iterator it, const value_type& v) {
    IM_ASSERT(it >= Data && it <= Data + Size);
    const ptrdiff_t off = it - Data;

    if (Size == Capacity) {
      reserve(Capacity ? Capacity * 2 : 4);
    }

    if (off < (int)Size) {
      memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(value_type));
    }

    Data[off] = v;
    Size++;
    return Data + off;
  }
};
// Helper: execute a block of code at maximum once a frame
// Convenient if you want to quickly create an UI within deep-nested code that runs multiple times every frame.
// Usage:
// IMGUI_ONCE_UPON_A_FRAME
// {
// // code block will be executed one per frame
// }
// Attention! the macro expands into 2 statement so make sure you don't use it within e.g. an if() statement without curly braces.
#define IMGUI_ONCE_UPON_A_FRAME static ImGuiOnceUponAFrame imgui_oaf##__LINE__; if (imgui_oaf##__LINE__)
struct ImGuiOnceUponAFrame {
  ImGuiOnceUponAFrame() {
    RefFrame = -1;
  }
  mutable int RefFrame;
  operator bool() const {
    int current_frame = ImGui_GetFrameCount();

    if (RefFrame == current_frame) {
      return false;
    }

    RefFrame = current_frame;
    return true;
  }
};
#include <stdlib.h>
#define IMGUI_REALLOC(ptr, size)  ImGui_ReAlloc(ptr, size, addsz)  (ImGui_ReAlloc(&ptr, sizeof(*ptr)*(size += addsz)), (ptr + size - addsz) )
#define IMGUI_FREE(ptr, size)     if (ptr) { free(ptr); ptr = NULL; } size=0;
static void* ImGui_ReAlloc(void* ptr, int size)
{
  void** pptr = (void**)ptr;
  *pptr = realloc(*pptr, size);
  return *pptr;
}
struct TextRange {
  const char* b;
  const char* e;
  TextRange() {
    b = e = NULL;
  }
  TextRange(const char* _b, const char* _e) {
    b = _b;
    e = _e;
  }
  const char* begin() const {
    return b;
  }
  const char* end() const {
    return e;
  }
  bool empty() const {
    return b == e;
  }
  char front() const {
    return *b;
  }
  static bool is_blank(char c) {
    return c == ' ' || c == '\t';
  }
  void trim_blanks() {
    while (b < e && is_blank(*b)) {
      b++;
    }

    while (e > b && is_blank(*(e - 1))) {
      e--;
    }
  }
  IMGUI_API void split(char separator, ImVector<TextRange>& out);
};
// Helper: Parse and apply text filters. In format "aaaaa[,bbbb][,ccccc]"
struct ImGuiTextFilter {
  char InputBuf[256];
  ImVector<TextRange> Filters;
  int CountGrep;
  ImGuiTextFilter(const char* default_filter = "");
  ~ImGuiTextFilter() {}
  void Clear() {
    InputBuf[0] = 0;
    Build();
  }
  bool Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f); // Helper calling InputText+Build
  bool PassFilter(const char* text, const char* text_end = NULL) const;
  bool IsActive() const {
    return !Filters.empty();
  }
  IMGUI_API void Build();
};
// Helper: Text buffer for logging/accumulating text
struct ImGuiTextBuffer {
  ImVector<char> Buf;
  ImGuiTextBuffer() {
    Buf.push_back(0);
  }
  inline char operator[](int i) {
    return Buf.Data[i];
  }
  const char* begin() const {
    return &Buf.front();
  }
  const char* end() const {
    return &Buf.back(); // Buf is zero-terminated, so end() will point on the zero-terminator
  }
  int size() const {
    return Buf.Size - 1;
  }
  bool empty() {
    return Buf.Size <= 1;
  }
  void clear() {
    Buf.clear();
    Buf.push_back(0);
  }
  const char* c_str() const {
    return Buf.Data;
  }
  IMGUI_API void append(const char* fmt, ...) IM_PRINTFARGS(2);
  IMGUI_API void appendv(const char* fmt, va_list args);
};
// Helper: Simple Key->value storage
// - Store collapse state for a tree (Int 0/1)
// - Store color edit options (Int using values in ImGuiColorEditMode enum).
// - Custom user storage for temporary values.
// Typically you don't have to worry about this since a storage is held within each Window.
// Declare your own storage if:
// - You want to manipulate the open/close state of a particular sub-tree in your interface (tree node uses Int 0/1 to store their state).
// - You want to store custom debug data easily without adding or editing structures in your code.
// Types are NOT stored, so it is up to you to make sure your Key don't collide with different types.
struct ImGuiStorage {
  struct Pair {
    ImGuiID key;
    union {
      int val_i;
      float val_f;
      void* val_p;
    };
    Pair(ImGuiID _key, int _val_i) {
      key = _key;
      val_i = _val_i;
    }
    Pair(ImGuiID _key, float _val_f) {
      key = _key;
      val_f = _val_f;
    }
    Pair(ImGuiID _key, void* _val_p) {
      key = _key;
      val_p = _val_p;
    }
  };
  ImVector<Pair> Data;
  // - Get***() functions find pair, never add/allocate. Pairs are sorted so a query is O(log N)
  // - Set***() functions find pair, insertion on demand if missing.
  // - Sorted insertion is costly, paid once. A typical frame shouldn't need to insert any new pair.
  IMGUI_API void Clear();
  IMGUI_API int GetInt(ImGuiID key, int default_val = 0) const;
  IMGUI_API void SetInt(ImGuiID key, int val);
  IMGUI_API bool GetBool(ImGuiID key, bool default_val = false) const;
  IMGUI_API void SetBool(ImGuiID key, bool val);
  IMGUI_API float GetFloat(ImGuiID key, float default_val = 0.0f) const;
  IMGUI_API void SetFloat(ImGuiID key, float val);
  IMGUI_API void* GetVoidPtr(ImGuiID key) const; // default_val is NULL
  IMGUI_API void SetVoidPtr(ImGuiID key, void* val);
  // - Get***Ref() functions finds pair, insert on demand if missing, return pointer. Useful if you intend to do Get+Set.
  // - References are only valid until a new value is added to the storage. Calling a Set***() function or a Get***Ref() function invalidates the pointer.
  // - A typical use case where this is convenient:
  // float* pvar = ImGui_GetFloatRef(key); ImGui_SliderFloat("var", pvar, 0, 100.0f); some_var += *pvar;
  // - You can also use this to quickly create temporary editable values during a session of using Edit&Continue, without restarting your application.
  IMGUI_API int* GetIntRef(ImGuiID key, int default_val = 0);
  IMGUI_API bool* GetBoolRef(ImGuiID key, bool default_val = false);
  IMGUI_API float* GetFloatRef(ImGuiID key, float default_val = 0.0f);
  IMGUI_API void** GetVoidPtrRef(ImGuiID key, void* default_val = NULL);
  // Use on your own storage if you know only integer are being stored (open/close all tree nodes)
  IMGUI_API void SetAllInt(int val);
};
// Shared state of InputText(), passed to callback when a ImGuiInputTextFlags_Callback* flag is used and the corresponding callback is triggered.
struct ImGuiTextEditCallbackData {
  ImGuiInputTextFlags EventFlag; // One of ImGuiInputTextFlags_Callback* // Read-only
  ImGuiInputTextFlags Flags; // What user passed to InputText() // Read-only
  void* UserData; // What user passed to InputText() // Read-only
  bool ReadOnly; // Read-only mode // Read-only
  // CharFilter event:
  ImWchar EventChar; // Character input // Read-write (replace character or set to zero)
  // Completion,History,Always events:
  // If you modify the buffer contents make sure you update 'BufTextLen' and set 'BufDirty' to true.
  ImGuiKey EventKey; // Key pressed (Up/Down/TAB) // Read-only
  char* Buf; // Current text buffer // Read-write (pointed data only, can't replace the actual pointer)
  int BufTextLen; // Current text length in bytes // Read-write
  int BufSize; // Maximum text length in bytes // Read-only
  bool BufDirty; // Set if you modify Buf/BufTextLen!! // Write
  int CursorPos; // // Read-write
  int SelectionStart; // // Read-write (== to SelectionEnd when no selection)
  int SelectionEnd; // // Read-write
  // NB: Helper functions for text manipulation. Calling those function loses selection.
  void DeleteChars(int pos, int bytes_count);
  void InsertChars(int pos, const char* text, const char* text_end = NULL);
  bool HasSelection() const {
    return SelectionStart != SelectionEnd;
  }
};
// Resizing callback data to apply custom constraint. As enabled by SetNextWindowSizeConstraints(). Callback is called during the next Begin().
// NB: For basic min/max size constraint on each axis you don't need to use the callback! The SetNextWindowSizeConstraints() parameters are enough.
struct ImGuiSizeConstraintCallbackData {
  void* UserData; // Read-only. What user passed to SetNextWindowSizeConstraints()
  ImVec2 Pos; // Read-only. Window position, for reference.
  ImVec2 CurrentSize; // Read-only. Current window size.
  ImVec2 DesiredSize; // Read-write. Desired size, based on user's mouse position. Write to this field to restrain resizing.
};
// ImColor() helper to implicity converts colors to either ImU32 (packed 4x1 byte) or ImVec4 (4x1 float)
// Prefer using IM_COL32() macros if you want a guaranteed compile-time ImU32 for usage with ImDrawList API.
// Avoid storing ImColor! Store either u32 of ImVec4. This is not a full-featured color class.
// None of the ImGui API are using ImColor directly but you can use it as a convenience to pass colors in either ImU32 or ImVec4 formats.
#if 1
ImVec4 ImColor(int r, int g, int b, int a = 255)
{
  ImVec4 Value;
  float sc = 1.0f / 255.0f;
  Value.x = (float)r * sc;
  Value.y = (float)g * sc;
  Value.z = (float)b * sc;
  Value.w = (float)a * sc;
  return Value;
}
ImVec4 ImColor(ImU32 rgba)
{
  ImVec4 Value;
  float sc = 1.0f / 255.0f;
  Value.x = (float)(rgba & 0xFF) * sc;
  Value.y = (float)((rgba >> 8) & 0xFF) * sc;
  Value.z = (float)((rgba >> 16) & 0xFF) * sc;
  Value.w = (float)(rgba >> 24) * sc;
  return Value;
}
ImVec4 ImColor(float r, float g, float b, float a = 1.0f)
{
  ImVec4 Value;
  Value.x = r;
  Value.y = g;
  Value.z = b;
  Value.w = a;
  return Value;
}
#define ImColorU32RGB(r, g, b)  ImColorU32RGBA(r, g, b, 255)
ImU32 ImColorU32RGBA(int r, int g, int b, int a)
{
  ImU32 out;
  out = ((ImU32)(r));
  out |= ((ImU32)(g)) << 8;
  out |= ((ImU32)(b)) << 16;
  out |= ((ImU32)(a)) << 24;
  return out;
}
//inline operator ImU32 ImVec4ToU32(const ImVec4& Value) {  return ImGui_ColorConvertFloat4ToU32(Value);}
ImVec4 ImColorHSV(float h, float s, float v, float a = 1.0f)
{
  ImVec4 Value;
  ImGui_ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z);
  Value.w = a;
  return Value;
}
ImVec4 HSV____(float h, float s, float v, float a = 1.0f)
{
  float r, g, b;
  ImGui_ColorConvertHSVtoRGB(h, s, v, r, g, b);
  return ImColor(r, g, b, a);
}
#else
struct ImColor {
  ImVec4 Value;
  ImColor() {
    Value.x = Value.y = Value.z = Value.w = 0.0f;
  }
  ImColor(int r, int g, int b, int a = 255) {
    float sc = 1.0f / 255.0f;
    Value.x = (float)r * sc;
    Value.y = (float)g * sc;
    Value.z = (float)b * sc;
    Value.w = (float)a * sc;
  }
  ImColor(ImU32 rgba) {
    float sc = 1.0f / 255.0f;
    Value.x = (float)(rgba & 0xFF) * sc;
    Value.y = (float)((rgba >> 8) & 0xFF) * sc;
    Value.z = (float)((rgba >> 16) & 0xFF) * sc;
    Value.w = (float)(rgba >> 24) * sc;
  }
  ImColor(float r, float g, float b, float a = 1.0f) {
    Value.x = r;
    Value.y = g;
    Value.z = b;
    Value.w = a;
  }
  ImColor(const ImVec4& col) {
    Value = col;
  }
  inline operator ImU32() const {
    return ImGui_ColorConvertFloat4ToU32(Value);
  }
  inline operator ImVec4() const {
    return Value;
  }
  inline void SetHSV(float h, float s, float v, float a = 1.0f) {
    ImGui_ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z);
    Value.w = a;
  }
  static ImColor HSV(float h, float s, float v, float a = 1.0f) {
    float r, g, b;
    ImGui_ColorConvertHSVtoRGB(h, s, v, r, g, b);
    return ImColor(r, g, b, a);
  }
};
#endif
// Helper: Manually clip large list of items.
// If you are submitting lots of evenly spaced items and you have a random access to the list, you can perform coarse clipping based on visibility to save yourself from processing those items at all.
// The clipper calculates the range of visible items and advance the cursor to compensate for the non-visible items we have skipped.
// ImGui already clip items based on their bounds but it needs to measure text size to do so. Coarse clipping before submission makes this cost and your own data fetching/submission cost null.
// Usage:
// ImGuiListClipper clipper(1000); // we have 1000 elements, evenly spaced.
// while (clipper.Step())
// for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
// ImGui_Text("line number %d", i);
// - Step 0: the clipper let you process the first element, regardless of it being visible or not, so we can measure the element height (step skipped if we passed a known height as second arg to constructor).
// - Step 1: the clipper infer height from first element, calculate the actual range of elements to display, and position the cursor before the first element.
// - (Step 2: dummy step only required if an explicit items_height was passed to constructor or Begin() and user call Step(). Does nothing and switch to Step 3.)
// - Step 3: the clipper validate that we have reached the expected Y position (corresponding to element DisplayEnd), advance the cursor to the end of the list and then returns 'false' to end the loop.
typedef struct ImGuiListClipper {
  float StartPosY;
  float ItemsHeight;
  int ItemsCount, StepNo, DisplayStart, DisplayEnd;
  // items_count: Use -1 to ignore (you can call Begin later). Use INT_MAX if you don't know how many items you have (in which case the cursor won't be advanced in the final step).
  // items_height: Use -1.0f to be calculated automatically on first step. Otherwise pass in the distance between your items, typically GetTextLineHeightWithSpacing() or GetItemsLineHeightWithSpacing().
  // If you don't specify an items_height, you NEED to call Step(). If you specify items_height you may call the old Begin()/End() api directly, but prefer calling Step().
  //ImGuiListClipper(int items_count = -1, float items_height = -1.0f) Begin(items_count, items_height); // NB: Begin() initialize every fields (as we allow user to call Begin/End multiple times on a same instance if they want).
  //~ImGuiListClipper()     IM_ASSERT(ItemsCount == -1); // Assert if user forgot to call End() or Step() until false.
} ImGuiListClipper;

IMGUI_API bool ImGuiListClipper_Step(ImGuiListClipper* lc); // Call until it returns false. The DisplayStart/DisplayEnd fields will be set and you can process/draw those items.
IMGUI_API void ImGuiListClipper_Begin(ImGuiListClipper* lc, int items_count, float items_height = -1.0f); // Automatically called by constructor if you passed 'items_count' or by Step() in Step 1.
IMGUI_API void ImGuiListClipper_End(ImGuiListClipper* lc); // Automatically called on the last call of Step() that returns false.

//-----------------------------------------------------------------------------
// Draw List
// Hold a series of drawing commands. The user provides a renderer for ImDrawData which essentially contains an array of ImDrawList.
//-----------------------------------------------------------------------------
// Helpers macros to generate 32-bits encoded colors
#define IM_COL32(R,G,B,A) (((ImU32)(A)<<24) | ((ImU32)(B)<<16) | ((ImU32)(G)<<8) | ((ImU32)(R)))
#define IM_COL32_WHITE (0xFFFFFFFF)
#define IM_COL32_BLACK (0xFF000000)
#define IM_COL32_BLACK_TRANS (0x00000000) // Transparent black
// Draw callbacks for advanced uses.
// NB- You most likely do NOT need to use draw callbacks just to create your own widget or customized UI rendering (you can poke into the draw list for that)
// Draw callback may be useful for example, A) Change your GPU render state, B) render a complex 3D scene inside a UI element (without an intermediate texture/render target), etc.
// The expected behavior from your rendering function is 'if (cmd.UserCallback != NULL) cmd.UserCallback(parent_list, cmd); else RenderTriangles()'
typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);
// Typically, 1 command = 1 gpu draw call (unless command is a callback)
typedef struct ImDrawCmd {
  unsigned int ElemCount; // Number of indices (multiple of 3) to be rendered as triangles. Vertices are stored in the callee ImDrawList's vtx_buffer[] array, indices in idx_buffer[].
  ImVec4 ClipRect; // Clipping rectangle (x1, y1, x2, y2)
  ImTextureID TextureId; // User-provided texture ID. Set by user in ImfontAtlas::SetTexID() for fonts or passed to Image*() functions. Ignore if never using images or multiple fonts atlas.
  ImDrawCallback UserCallback; // If != NULL, call the function instead of rendering the vertices. clip_rect and texture_id will be set normally.
  void* UserCallbackData; // The draw callback code can access this.
} ImDrawCmd;
static void ImDrawCmd_init(ImDrawCmd* cmd)
{
  cmd->ElemCount = 0;
  cmd->ClipRect.x = cmd->ClipRect.y = -8192.0f;
  cmd->ClipRect.z = cmd->ClipRect.w = +8192.0f;
  cmd->TextureId = NULL;
  cmd->UserCallback = NULL;
  cmd->UserCallbackData = NULL;
}
// Vertex index (override with '#define ImDrawIdx unsigned int' inside in imconfig.h)
#ifndef ImDrawIdx
typedef unsigned short ImDrawIdx;
#endif
// Vertex layout
#ifndef IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct ImDrawVert {
  ImVec2 pos;
  ImVec2 uv;
  ImU32 col;
};
#else
// You can override the vertex format layout by defining IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT in imconfig.h
// The code expect ImVec2 pos (8 bytes), ImVec2 uv (8 bytes), ImU32 col (4 bytes), but you can re-order them or add other fields as needed to simplify integration in your engine.
// The type has to be described within the macro (you can either declare the struct or use a typedef)
IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif
// Draw channels are used by the Columns API to "split" the render list into different channels while building, so items of each column can be batched together.
// You can also use them to simulate drawing layers and submit primitives in a different order than how they will be rendered.
struct ImDrawChannel {
  ImVector<ImDrawCmd> CmdBuffer;
  ImVector<ImDrawIdx> IdxBuffer;
};
typedef struct ImDrawList ImDrawList;

// Draw command list
// This is the low-level list of polygons that ImGui functions are filling. At the end of the frame, all command lists are passed to your ImGuiIO::RenderDrawListFn function for rendering.
// At the moment, each ImGui window contains its own ImDrawList but they could potentially be merged in the future.
// If you want to add custom rendering within a window, you can use ImGui_GetWindowDrawList() to access the current draw list and add your own primitives.
// You can interleave normal ImGui_ calls and adding primitives to the current draw list.
// All positions are in screen coordinates (0,0=top-left, 1 pixel per unit). Primitives are always added to the list and not culled (culling is done at render time and at a higher-level by ImGui_ functions).
struct ImDrawList {
  // This is what you have to render
  ImVector<ImDrawCmd> CmdBuffer; // Commands. Typically 1 command = 1 gpu draw call.
  ImVector<ImDrawIdx> IdxBuffer; // Index buffer. Each command consume ImDrawCmd::ElemCount of those
  ImVector<ImDrawVert> VtxBuffer; // Vertex buffer.
  // [Internal, used while building lists]
  const char* _OwnerName; // Pointer to owner window's name (if any) for debugging
  unsigned int _VtxCurrentIdx; // [Internal] == VtxBuffer.Size
  ImDrawVert* _VtxWritePtr; // [Internal] point within VtxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
  ImDrawIdx* _IdxWritePtr; // [Internal] point within IdxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
  ImVector<ImVec4> _ClipRectStack; // [Internal]
  ImVector<ImTextureID> _TextureIdStack; // [Internal]
  ImVector<ImVec2> _Path; // [Internal] current path building
  int _ChannelsCurrent; // [Internal] current channel number (0)
  int _ChannelsCount; // [Internal] number of active channels (1+)
  ImVector<ImDrawChannel> _Channels; // [Internal] draw channels for columns API (not resized down so _ChannelsCount may be smaller than _Channels.Size)
};

void ImDrawList_Init(ImDrawList* g);
void ImDrawList_Clear(ImDrawList* g);
void ImDrawList_ClearFreeMemory(ImDrawList* g);
void ImDrawList_PushClipRect(ImDrawList* g, ImVec2 cr_min, ImVec2 cr_max, bool intersect_with_current_clip_rect);
void ImDrawList_PushClipRectFullScreen(ImDrawList* g);
void ImDrawList_PopClipRect(ImDrawList* g);
void ImDrawList_PushTextureID(ImDrawList* g, const ImTextureID& texture_id);
void ImDrawList_PopTextureID(ImDrawList* g);
void ImDrawList_ChannelsSplit(ImDrawList* g, int channels_count);
void ImDrawList_ChannelsSetCurrent(ImDrawList* g, int idx);
void ImDrawList_ChannelsMerge(ImDrawList* g);
void ImDrawList_PrimRect(ImDrawList* g, const ImVec2& a, const ImVec2& c, ImU32 col);


//IMGUI_API void PushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect = false); // Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui_PushClipRect() to affect logic (hit-testing and widget culling)
//IMGUI_API void PushClipRectFullScreen();
//IMGUI_API void PopClipRect();
//IMGUI_API void PushTextureID(const ImTextureID& texture_id);
//IMGUI_API void PopTextureID();
// Primitives
IMGUI_API void ImDrawList_AddLine(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.0f);
IMGUI_API void ImDrawList_AddRect(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners = 0x0F, float thickness = 1.0f); // a: upper-left, b: lower-right
IMGUI_API void ImDrawList_AddRectFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners = 0x0F); // a: upper-left, b: lower-right
IMGUI_API void ImDrawList_AddRectFilledMultiColor(ImDrawList* g, const ImVec2& a, const ImVec2& b, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
IMGUI_API void ImDrawList_AddQuad(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness = 1.0f);
IMGUI_API void ImDrawList_AddQuadFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col);
IMGUI_API void ImDrawList_AddTriangle(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness = 1.0f);
IMGUI_API void ImDrawList_AddTriangleFilled(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col);
IMGUI_API void ImDrawList_AddCircle(ImDrawList* g, const ImVec2& centre, float radius, ImU32 col, int num_segments = 12, float thickness = 1.0f);
IMGUI_API void ImDrawList_AddCircleFilled(ImDrawList* g, const ImVec2& centre, float radius, ImU32 col, int num_segments = 12);
IMGUI_API void ImDrawList_AddText(ImDrawList* g, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
IMGUI_API void ImDrawList_AddText(ImDrawList* g, const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
IMGUI_API void ImDrawList_AddImage(ImDrawList* g, ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv0 = fVec2(0, 0), const ImVec2& uv1 = fVec2(1, 1), ImU32 col = 0xFFFFFFFF);
IMGUI_API void ImDrawList_AddPolyline(ImDrawList* g, const ImVec2* points, const int num_points, ImU32 col, bool closed, float thickness, bool anti_aliased);
IMGUI_API void ImDrawList_AddConvexPolyFilled(ImDrawList* g, const ImVec2* points, const int num_points, ImU32 col, bool anti_aliased);
IMGUI_API void ImDrawList_AddBezierCurve(ImDrawList* g, const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments = 0);
// Stateful path API, add points then finish with PathFill() or PathStroke()
inline void ImDrawList_PathClear(ImDrawList* g)
{
  g->_Path.resize(0);
}
inline void ImDrawList_PathLineTo(ImDrawList* g, const ImVec2& pos)
{
  g->_Path.push_back(pos);
}
inline void ImDrawList_PathLineToMergeDuplicate(ImDrawList* g, const ImVec2& pos)
{
  if (g->_Path.Size == 0 || memcmp(&g->_Path[g->_Path.Size - 1], &pos, 8) != 0) {
    g->_Path.push_back(pos);
  }
}
inline void ImDrawList_PathFill(ImDrawList* g, ImU32 col)
{
  ImDrawList_AddConvexPolyFilled(g, g->_Path.Data, g->_Path.Size, col, true);
  ImDrawList_PathClear(g);
}
inline void ImDrawList_PathStroke(ImDrawList* g, ImU32 col, bool closed, float thickness = 1.0f)
{
  ImDrawList_AddPolyline(g, g->_Path.Data, g->_Path.Size, col, closed, thickness, true);
  ImDrawList_PathClear(g);
}
IMGUI_API void ImDrawList_PathArcTo(ImDrawList* g, const ImVec2& centre, float radius, float a_min, float a_max, int num_segments = 10);
IMGUI_API void ImDrawList_PathArcToFast(ImDrawList* g, const ImVec2& centre, float radius, int a_min_of_12, int a_max_of_12); // Use precomputed angles for a 12 steps circle
IMGUI_API void ImDrawList_PathBezierCurveTo(ImDrawList* g, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, int num_segments = 0);
IMGUI_API void ImDrawList_PathRect(ImDrawList* g, const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, int rounding_corners = 0x0F);
// Channels
// - Use to simulate layers. By switching channels to can render out-of-order (e.g. submit foreground primitives before background primitives)
// - Use to minimize draw calls (e.g. if going back-and-forth between multiple non-overlapping clipping rectangles, prefer to append into separate channels then merge at the end)
//IMGUI_API void ChannelsSplit(int channels_count);
//IMGUI_API void ChannelsMerge();
//IMGUI_API void ChannelsSetCurrent(int channel_index);
// Advanced
IMGUI_API void ImDrawList_AddCallback(ImDrawList* g, ImDrawCallback callback, void* callback_data); // Your rendering function must check for 'UserCallback' in ImDrawCmd and call the function instead of rendering triangles.
//IMGUI_API void AddDrawCmd(); // This is useful if you need to forcefully create a new draw call (to allow for dependent rendering / blending). Otherwise primitives are merged into the same draw-call as much as possible
// Internal helpers
// NB: all primitives needs to be reserved via PrimReserve() beforehand!
//IMGUI_API void PrimReserve(int idx_count, int vtx_count);
//IMGUI_API void PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col); // Axis aligned rectangle (composed of two triangles)
IMGUI_API void ImDrawList_PrimRectUV(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col);
IMGUI_API void ImDrawList_PrimQuadUV(ImDrawList* g, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col);
inline void ImDrawList_PrimWriteVtx(ImDrawList* g, const ImVec2& pos, const ImVec2& uv, ImU32 col)
{
  g->_VtxWritePtr->pos = pos;
  g->_VtxWritePtr->uv = uv;
  g->_VtxWritePtr->col = col;
  g->_VtxWritePtr++;
  g->_VtxCurrentIdx++;
}
inline void ImDrawList_PrimWriteIdx(ImDrawList* g, ImDrawIdx idx)
{
  *g->_IdxWritePtr = idx;
  g->_IdxWritePtr++;
}
inline void ImDrawList_PrimVtx(ImDrawList* g, const ImVec2& pos, const ImVec2& uv, ImU32 col)
{
  ImDrawList_PrimWriteIdx(g, (ImDrawIdx)g->_VtxCurrentIdx);
  ImDrawList_PrimWriteVtx(g, pos, uv, col);
}
//IMGUI_API void UpdateClipRect();
//IMGUI_API void UpdateTextureID();


// All draw data to render an ImGui frame
struct ImDrawData {
  bool Valid; // Only valid after Render() is called and before the next NewFrame() is called.
  ImDrawList** CmdLists;
  int CmdListsCount;
  int TotalVtxCount; // For convenience, sum of all cmd_lists vtx_buffer.Size
  int TotalIdxCount; // For convenience, sum of all cmd_lists idx_buffer.Size
  // Functions
};
IMGUI_API void ImDrawData_Init(ImDrawData* g);
IMGUI_API void ImDrawData_DeIndexAllBuffers(ImDrawData* g); // For backward compatibility: convert all buffers from indexed to de-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
IMGUI_API void ImDrawData_ScaleClipRects(ImDrawData* g, const ImVec2& sc); // Helper to scale the ClipRect field of each ImDrawCmd. Use if your final output buffer is at a different scale than ImGui expects, or if there is a difference between your window resolution and framebuffer resolution.

struct ImFontConfig {
  void* FontData; // // TTF data
  int FontDataSize; // // TTF data size
  bool FontDataOwnedByAtlas; // true // TTF data ownership taken by the container ImFontAtlas (will delete memory itself). Set to true
  int FontNo; // 0 // Index of font within TTF file
  float SizePixels; // // Size in pixels for rasterizer
  int OversampleH, OversampleV; // 3, 1 // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
  bool PixelSnapH; // false // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
  ImVec2 GlyphExtraSpacing; // 0, 0 // Extra spacing (in pixels) between glyphs
  const ImWchar* GlyphRanges; // // Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
  bool MergeMode; // false // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs).
  bool MergeGlyphCenterV; // false // When merging (multiple ImFontInput for one ImFont), vertically center new glyphs instead of aligning their baseline
  // [Internal]
  char Name[32]; // Name (strictly for debugging)
  ImFont* DstFont;
};
void ImFontConfig_Init(ImFontConfig* g);
// Load and rasterize multiple TTF fonts into a same texture.
// Sharing a texture for multiple fonts allows us to reduce the number of draw calls during rendering.
// We also add custom graphic data into the texture that serves for ImGui.
// 1. (Optional) Call AddFont*** functions. If you don't call any, the default font will be loaded for you.
// 2. Call GetTexDataAsAlpha8() or GetTexDataAsRGBA32() to build and retrieve pixels data.
// 3. Upload the pixels data into a texture within your graphics system.
// 4. Call SetTexID(my_tex_id); and pass the pointer/identifier to your texture. This value will be passed back to you during rendering to identify the texture.
// 5. Call ClearTexData() to free textures memory on the heap.
// NB: If you use a 'glyph_ranges' array you need to make sure that your array persist up until the ImFont is cleared. We only copy the pointer, not the data.
struct ImFontAtlas {
  // Members
  // (Access texture data via GetTexData*() calls which will setup a default font for you.)
  void* TexID; // User data to refer to the texture once it has been uploaded to user's graphic systems. It ia passed back to you during rendering.
  unsigned char* TexPixelsAlpha8; // 1 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight
  unsigned int* TexPixelsRGBA32; // 4 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight * 4
  int TexWidth; // Texture width calculated during Build().
  int TexHeight; // Texture height calculated during Build().
  int TexDesiredWidth; // Texture width desired by user before Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.
  ImVec2 TexUvWhitePixel; // Texture coordinates to a white pixel
  ImVector<ImFont*> Fonts; // Hold all the fonts returned by AddFont*. Fonts[0] is the default font upon calling ImGui_NewFrame(), use ImGui_PushFont()/PopFont() to change the current font.
  // Private
  ImVector<ImFontConfig> ConfigData; // Internal data
};
IMGUI_API void ImFontAtlas_Init(ImFontAtlas* g);
IMGUI_API ImFont* ImFontAtlas_AddFont(ImFontAtlas* g, const ImFontConfig* font_cfg);
IMGUI_API ImFont* ImFontAtlas_AddFontDefault(ImFontAtlas* g, const ImFontConfig* font_cfg = NULL);
IMGUI_API ImFont* ImFontAtlas_AddFontFromFileTTF(ImFontAtlas* g, const char* filename, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
IMGUI_API ImFont* ImFontAtlas_AddFontFromMemoryTTF(ImFontAtlas* g, void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // Transfer ownership of 'ttf_data' to ImFontAtlas, will be deleted after Build()
IMGUI_API ImFont* ImFontAtlas_AddFontFromMemoryCompressedTTF(ImFontAtlas* g, const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // 'compressed_ttf_data' still owned by caller. Compress with binary_to_compressed_c.cpp
IMGUI_API ImFont* ImFontAtlas_AddFontFromMemoryCompressedBase85TTF(ImFontAtlas* g, const char* compressed_ttf_data_base85, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // 'compressed_ttf_data_base85' still owned by caller. Compress with binary_to_compressed_c.cpp with -base85 paramaeter
IMGUI_API void ImFontAtlas_ClearTexData(ImFontAtlas* g); // Clear the CPU-side texture data. Saves RAM once the texture has been copied to graphics memory.
IMGUI_API void ImFontAtlas_ClearInputData(ImFontAtlas* g); // Clear the input TTF data (inc sizes, glyph ranges)
IMGUI_API void ImFontAtlas_ClearFonts(ImFontAtlas* g); // Clear the ImGui-side font data (glyphs storage, UV coordinates)
IMGUI_API void ImFontAtlas_Clear(ImFontAtlas* g); // Clear all
// Retrieve texture data
// User is in charge of copying the pixels into graphics memory, then call SetTextureUserID()
// After loading the texture into your graphic system, store your texture handle in 'TexID' (ignore if you aren't using multiple fonts nor images)
// RGBA32 format is provided for convenience and high compatibility, but note that all RGB pixels are white, so 75% of the memory is wasted.
// Pitch = Width * BytesPerPixels
IMGUI_API void ImFontAtlas_GetTexDataAsAlpha8(ImFontAtlas* g, unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL); // 1 byte per-pixel
IMGUI_API void ImFontAtlas_GetTexDataAsRGBA32(ImFontAtlas* g, unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL); // 4 bytes-per-pixel
// Helpers to retrieve list of common Unicode ranges (2 value per range, values are inclusive, zero-terminated list)
// NB: Make sure that your string are UTF-8 and NOT in your local code page. See FAQ for details.
IMGUI_API const ImWchar* ImFontAtlas_GetGlyphRangesDefault(ImFontAtlas* g); // Basic Latin, Extended Latin
IMGUI_API const ImWchar* ImFontAtlas_GetGlyphRangesKorean(ImFontAtlas* g); // Default + Korean characters
IMGUI_API const ImWchar* ImFontAtlas_GetGlyphRangesJapanese(ImFontAtlas* g); // Default + Hiragana, Katakana, Half-Width, Selection of 1946 Ideographs
IMGUI_API const ImWchar* ImFontAtlas_GetGlyphRangesChinese(ImFontAtlas* g); // Japanese + full set of about 21000 CJK Unified Ideographs
IMGUI_API const ImWchar* ImFontAtlas_GetGlyphRangesCyrillic(ImFontAtlas* g); // Default + about 400 Cyrillic characters
IMGUI_API const ImWchar* ImFontAtlas_GetGlyphRangesThai(ImFontAtlas* g); // Default + Thai characters
IMGUI_API bool ImFontAtlas_Build(ImFontAtlas* g); // Build pixels data. This is automatically for you by the GetTexData*** functions.
IMGUI_API void ImFontAtlas_RenderCustomTexData(ImFontAtlas* g, int pass, void* rects);
typedef struct ImFontGlyph {
  ImWchar Codepoint;
  float XAdvance;
  float X0, Y0, X1, Y1;
  float U0, V0, U1, V1; // Texture coordinates
} ImFontGlyph;
// Font runtime data and rendering
// ImFontAtlas automatically loads a default embedded font for you when you call GetTexDataAsAlpha8() or GetTexDataAsRGBA32().
struct ImFont {
  // Members: Hot ~62/78 bytes
  float FontSize; // <user set> // Height of characters, set during loading (don't change after loading)
  float Scale; // = 1.f // Base font scale, multiplied by the per-window font scale which you can adjust with SetFontScale()
  ImVec2 DisplayOffset; // = (0.f,1.f) // Offset font rendering by xx pixels
  ImVector<ImFontGlyph> Glyphs; // // All glyphs.
  ImVector<float> IndexXAdvance; // // Sparse. Glyphs->XAdvance in a directly indexable way (more cache-friendly, for CalcTextSize functions which are often bottleneck in large UI).
  ImVector<unsigned short> IndexLookup; // // Sparse. Index glyphs by Unicode code-point.
  const ImFontGlyph* FallbackGlyph; // == FindGlyph(FontFallbackChar)
  float FallbackXAdvance; // == FallbackGlyph->XAdvance
  ImWchar FallbackChar; // = '?' // Replacement glyph if one isn't found. Only set via SetFallbackChar()
  // Members: Cold ~18/26 bytes
  short ConfigDataCount; // ~ 1 // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
  ImFontConfig* ConfigData; // // Pointer within ContainerAtlas->ConfigData
  ImFontAtlas* ContainerAtlas; // // What we has been loaded into
  float Ascent, Descent; // // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
};

// Methods
IMGUI_API void ImFont_Init(ImFont* g);
IMGUI_API void ImFont_Clear(ImFont* g);
IMGUI_API void ImFont_BuildLookupTable(ImFont* g);
IMGUI_API const ImFontGlyph* ImFont_FindGlyph(const ImFont* g, ImWchar c);
IMGUI_API void ImFont_SetFallbackChar(ImFont* g, ImWchar c);
float ImFont_GetCharAdvance(ImFont* g, ImWchar c)
{
  return ((int)c < g->IndexXAdvance.Size) ? g->IndexXAdvance[(int)c] : g->FallbackXAdvance;
}
bool ImFont_IsLoaded(ImFont* g)
{
  return g->ContainerAtlas != NULL;
}
// 'max_width' stops rendering after a certain width (could be turned into a 2d size). FLT_MAX to disable.
// 'wrap_width' enable automatic word-wrapping across multiple lines to fit into given width. 0.0f to disable.
IMGUI_API ImVec2 ImFont_CalcTextSizeA(const ImFont* g, float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL); // utf8
IMGUI_API const char* ImFont_CalcWordWrapPositionA(const ImFont* g, float scale, const char* text, const char* text_end, float wrap_width);
IMGUI_API void ImFont_RenderChar(const ImFont* g, ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, unsigned short c);
IMGUI_API void ImFont_RenderText(const ImFont* g, ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false);
// Private
IMGUI_API void ImFont_GrowIndex(ImFont* g, int new_size);
IMGUI_API void ImFont_AddRemapChar(ImFont* g, ImWchar dst, ImWchar src, bool overwrite_dst = true); // Makes 'dst' character/glyph points to 'src' character/glyph. Currently needs to be called AFTER fonts have been built.

// This is where your app communicate with ImGui. Access via ImGui_GetIO().
// Read 'Programmer guide' section in .cpp file for general usage.
struct ImGuiIO {
  //------------------------------------------------------------------
  // Settings (fill once) // Default value:
  //------------------------------------------------------------------
  ImVec2 DisplaySize; // <unset> // Display size, in pixels. For clamping windows positions.
  float DeltaTime; // = 1.0f/60.0f // Time elapsed since last frame, in seconds.
  float IniSavingRate; // = 5.0f // Maximum time between saving positions/sizes to .ini file, in seconds.
  const char* IniFilename; // = "imgui.ini" // Path to .ini file. NULL to disable .ini saving.
  const char* LogFilename; // = "imgui_log.txt" // Path to .log file (default parameter to ImGui_LogToFile when no file is specified).
  float MouseDoubleClickTime; // = 0.30f // Time for a double-click, in seconds.
  float MouseDoubleClickMaxDist; // = 6.0f // Distance threshold to stay in to validate a double-click, in pixels.
  float MouseDragThreshold; // = 6.0f // Distance threshold before considering we are dragging
  int KeyMap[ImGuiKey_COUNT]; // <unset> // Map of indices into the KeysDown[512] entries array
  float KeyRepeatDelay; // = 0.250f // When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).
  float KeyRepeatRate; // = 0.020f // When holding a key/button, rate at which it repeats, in seconds.
  void* UserData; // = NULL // Store your own data for retrieval by callbacks.
  ImFontAtlas Fonts[1]; // <auto> // Load and assemble one or more fonts into a single tightly packed texture. Output to Fonts array.
  float FontGlobalScale; // = 1.0f // Global scale all fonts
  bool FontAllowUserScaling; // = false // Allow user scaling text of individual window with CTRL+Wheel.
  ImVec2 DisplayFramebufferScale; // = (1.0f,1.0f) // For retina display or other situations where window coordinates are different from framebuffer coordinates. User storage only, presently not used by ImGui.
  ImVec2 DisplayVisibleMin; // <unset> (0.0f,0.0f) // If you use DisplaySize as a virtual space larger than your screen, set DisplayVisibleMin/Max to the visible area.
  ImVec2 DisplayVisibleMax; // <unset> (0.0f,0.0f) // If the values are the same, we defaults to Min=(0.0f) and Max=DisplaySize
  // Advanced/subtle behaviors
  bool OSXBehaviors; // = defined(__APPLE__) // OS X style: Text editing cursor movement using Alt instead of Ctrl, Shortcuts using Cmd/Super instead of Ctrl, Line/Text Start and End using Cmd+Arrows instead of Home/End, Double click selects by word instead of selecting whole text, Multi-selection in lists uses Cmd/Super instead of Ctrl
  //------------------------------------------------------------------
  // User Functions
  //------------------------------------------------------------------
  // Rendering function, will be called in Render().
  // Alternatively you can keep this to NULL and call GetDrawData() after Render() to get the same pointer.
  // See example applications if you are unsure of how to implement this.
  void (*RenderDrawListsFn)(ImDrawData* data);
  // Optional: access OS clipboard
  // (default to use native Win32 clipboard on Windows, otherwise uses a private clipboard. Override to access OS clipboard on other architectures)
  const char* (*GetClipboardTextFn)();
  void (*SetClipboardTextFn)(const char* text);
  // Optional: override memory allocations. MemFreeFn() may be called with a NULL pointer.
  // (default to posix malloc/free)
  void* (*MemAllocFn)(size_t sz);
  void (*MemFreeFn)(void* ptr);
  // Optional: notify OS Input Method Editor of the screen position of your cursor for text input position (e.g. when using Japanese/Chinese IME in Windows)
  // (default to use native imm32 api on Windows)
  void (*ImeSetInputScreenPosFn)(int x, int y);
  void* ImeWindowHandle; // (Windows) Set this to your HWND to get automatic IME cursor positioning.
  //------------------------------------------------------------------
  // Input - Fill before calling NewFrame()
  //------------------------------------------------------------------
  ImVec2 MousePos; // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
  bool MouseDown[5]; // Mouse buttons: left, right, middle + extras. ImGui itself mostly only uses left button (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.
  float MouseWheel; // Mouse wheel: 1 unit scrolls about 5 lines text.
  bool MouseDrawCursor; // Request ImGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor).
  bool KeyCtrl; // Keyboard modifier pressed: Control
  bool KeyShift; // Keyboard modifier pressed: Shift
  bool KeyAlt; // Keyboard modifier pressed: Alt
  bool KeySuper; // Keyboard modifier pressed: Cmd/Super/Windows
  bool KeysDown[512]; // Keyboard keys that are pressed (in whatever storage order you naturally have access to keyboard data)
  ImWchar InputCharacters[16 + 1]; // List of characters input (translated by user from keypress+keyboard state). Fill using AddInputCharacter() helper.
  // Functions
  IMGUI_API void AddInputCharacter(ImWchar c); // Helper to add a new character into InputCharacters[]
  IMGUI_API void AddInputCharactersUTF8(const char* utf8_chars); // Helper to add new characters into InputCharacters[] from an UTF-8 string
  inline void ClearInputCharacters() {
    InputCharacters[0] = 0; // Helper to clear the text input buffer
  }
  //------------------------------------------------------------------
  // Output - Retrieve after calling NewFrame(), you can use them to discard inputs or hide them from the rest of your application
  //------------------------------------------------------------------
  bool WantCaptureMouse; // Mouse is hovering a window or widget is active (= ImGui will use your mouse input)
  bool WantCaptureKeyboard; // Widget is active (= ImGui will use your keyboard input)
  bool WantTextInput; // Some text input widget is active, which will read input characters from the InputCharacters array.
  float Framerate; // Framerate estimation, in frame per second. Rolling average estimation based on IO.DeltaTime over 120 frames
  int MetricsAllocs; // Number of active memory allocations
  int MetricsRenderVertices; // Vertices output during last call to Render()
  int MetricsRenderIndices; // Indices output during last call to Render() = number of triangles * 3
  int MetricsActiveWindows; // Number of visible windows (exclude child windows)
  //------------------------------------------------------------------
  // [Internal] ImGui will maintain those fields for you
  //------------------------------------------------------------------
  ImVec2 MousePosPrev; // Previous mouse position
  ImVec2 MouseDelta; // Mouse delta. Note that this is zero if either current or previous position are negative to allow mouse enabling/disabling.
  bool MouseClicked[5]; // Mouse button went from !Down to Down
  ImVec2 MouseClickedPos[5]; // Position at time of clicking
  float MouseClickedTime[5]; // Time of last click (used to figure out double-click)
  bool MouseDoubleClicked[5]; // Has mouse button been double-clicked?
  bool MouseReleased[5]; // Mouse button went from Down to !Down
  bool MouseDownOwned[5]; // Track if button was clicked inside a window. We don't request mouse capture from the application if click started outside ImGui bounds.
  float MouseDownDuration[5]; // Duration the mouse button has been down (0.0f == just clicked)
  float MouseDownDurationPrev[5]; // Previous time the mouse button has been down
  float MouseDragMaxDistanceSqr[5]; // Squared maximum distance of how much mouse has traveled from the click point
  float KeysDownDuration[512]; // Duration the keyboard key has been down (0.0f == just pressed)
  float KeysDownDurationPrev[512]; // Previous duration the key has been down
  IMGUI_API ImGuiIO();
};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
//---- Include imgui_user.h at the end of imgui.h
//---- So you can include code that extends ImGui using any of the types declared above.
//---- (also convenient for user to only explicitly include vanilla imgui.h)
#ifdef IMGUI_INCLUDE_IMGUI_USER_H
#include "imgui_user.h"
#endif

