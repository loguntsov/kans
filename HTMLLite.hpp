// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'HTMLLite.pas' rev: 6.00

#ifndef HTMLLiteHPP
#define HTMLLiteHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Menus.hpp>	// Pascal unit
#include <LiteSubs.hpp>	// Pascal unit
#include <LitePars.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <LiteUn2.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Htmllite
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum THTMLBorderStyle { htFocused, htNone, htSingle };
#pragma option pop

class DELPHICLASS TRightClickParameters;
class PASCALIMPLEMENTATION TRightClickParameters : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	AnsiString URL;
	AnsiString Target;
	Litesubs::TImageObj* Image;
	int ImageX;
	int ImageY;
	AnsiString ClickWord;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRightClickParameters(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRightClickParameters(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TRightClickEvent)(System::TObject* Sender, TRightClickParameters* Parameters);

typedef void __fastcall (__closure *THotSpotEvent)(System::TObject* Sender, const AnsiString SRC);

typedef void __fastcall (__closure *THotSpotClickEvent)(System::TObject* Sender, const AnsiString SRC, bool &Handled);

typedef void __fastcall (__closure *TProcessingEvent)(System::TObject* Sender, bool ProcessingOn);

typedef void __fastcall (__closure *TImageClickEvent)(System::TObject* Sender, System::TObject* Obj, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);

typedef void __fastcall (__closure *TImageOverEvent)(System::TObject* Sender, System::TObject* Obj, Classes::TShiftState Shift, int X, int Y);

typedef void __fastcall (__closure *TMetaRefreshType)(System::TObject* Sender, int Delay, const AnsiString URL);

#pragma option push -b-
enum htOptionEnum { htOverLinksActive, htNoLinkUnderline, htShowDummyCaret, htShowVScroll };
#pragma option pop

typedef Set<htOptionEnum, htOverLinksActive, htShowVScroll>  ThtmlViewerOptions;

class DELPHICLASS TPaintPanel;
class PASCALIMPLEMENTATION TPaintPanel : public Extctrls::TCustomPanel 
{
	typedef Extctrls::TCustomPanel inherited;
	
private:
	Classes::TNotifyEvent FOnPaint;
	Classes::TComponent* FViewer;
	Graphics::TCanvas* Canvas2;
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TWMEraseBkgnd &Message);
	HIDESBASE MESSAGE void __fastcall WMLButtonDblClk(Messages::TWMMouse &Message);
	void __fastcall DoBackground(Graphics::TCanvas* ACanvas, bool WmErase);
	__fastcall TPaintPanel(Classes::TComponent* AOwner, Classes::TComponent* Viewer);
	__property Classes::TNotifyEvent OnPaint = {read=FOnPaint, write=FOnPaint};
	
public:
	virtual void __fastcall Paint(void);
public:
	#pragma option push -w-inl
	/* TCustomPanel.Create */ inline __fastcall virtual TPaintPanel(Classes::TComponent* AOwner) : Extctrls::TCustomPanel(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TPaintPanel(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TPaintPanel(HWND ParentWindow) : Extctrls::TCustomPanel(ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS T32ScrollBar;
class PASCALIMPLEMENTATION T32ScrollBar : public Stdctrls::TScrollBar 
{
	typedef Stdctrls::TScrollBar inherited;
	
private:
	int FPosition;
	int FMin;
	int FMax;
	int FPage;
	HIDESBASE void __fastcall SetPosition(int Value);
	HIDESBASE void __fastcall SetMin(int Value);
	HIDESBASE void __fastcall SetMax(int Value);
	HIDESBASE MESSAGE void __fastcall CNVScroll(Messages::TWMScroll &Message);
	
public:
	__property int Position = {read=FPosition, write=SetPosition, nodefault};
	__property int Min = {read=FMin, write=SetMin, nodefault};
	__property int Max = {read=FMax, write=SetMax, nodefault};
	HIDESBASE void __fastcall SetParams(int APosition, int APage, int AMin, int AMax);
public:
	#pragma option push -w-inl
	/* TScrollBar.Create */ inline __fastcall virtual T32ScrollBar(Classes::TComponent* AOwner) : Stdctrls::TScrollBar(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall T32ScrollBar(HWND ParentWindow) : Stdctrls::TScrollBar(ParentWindow) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TWinControl.Destroy */ inline __fastcall virtual ~T32ScrollBar(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum ThtmlFileType { HTMLType, TextType, ImgType, OtherType };
#pragma option pop

class DELPHICLASS ThtmlLite;
class PASCALIMPLEMENTATION ThtmlLite : public Controls::TWinControl 
{
	typedef Controls::TWinControl inherited;
	
protected:
	bool KeepCanvas;
	Controls::TDragDropEvent FOnDragDrop;
	Controls::TDragOverEvent FOnDragOver;
	Controls::TDragDropEvent __fastcall GetDragDrop();
	Controls::TDragOverEvent __fastcall GetDragOver();
	void __fastcall SetDragDrop(const Controls::TDragDropEvent Value);
	void __fastcall SetDragOver(const Controls::TDragOverEvent Value);
	void __fastcall HTMLDragDrop(System::TObject* Sender, System::TObject* Source, int X, int Y);
	void __fastcall HTMLDragOver(System::TObject* Sender, System::TObject* Source, int X, int Y, Controls::TDragState State, bool &Accept);
	bool DontDraw;
	AnsiString *FTitle;
	AnsiString *FURL;
	AnsiString *FTarget;
	AnsiString *FBase;
	AnsiString *FBaseEx;
	AnsiString *FBaseTarget;
	AnsiString *FCurrentFile;
	Classes::TStringList* FNameList;
	ThtmlFileType FCurrentFileType;
	THotSpotEvent FOnHotSpotCovered;
	THotSpotClickEvent FOnHotSpotClick;
	Litesubs::TGetImageEvent FOnImageRequest;
	Liteun2::TScriptEvent FOnScript;
	Litesubs::TFormSubmitEvent FOnFormSubmit;
	Classes::TNotifyEvent FOnHistoryChange;
	TProcessingEvent FOnProcessing;
	Litepars::TIncludeType FOnInclude;
	Litepars::TSoundType FOnSoundRequest;
	Litepars::TMetaType FOnMeta;
	TMetaRefreshType FOnMetaRefresh;
	AnsiString FRefreshURL;
	int FRefreshDelay;
	TRightClickEvent FOnRightClick;
	TImageClickEvent FOnImageClick;
	TImageOverEvent FOnImageOver;
	Litesubs::TObjectClickEvent FOnObjectClick;
	Classes::TStrings* FHistory;
	Classes::TStrings* FTitleHistory;
	Liteun2::TFreeList* FPositionHistory;
	int FHistoryIndex;
	int FHistoryMaxCount;
	AnsiString *FFontName;
	AnsiString *FPreFontName;
	Graphics::TColor FFontColor;
	Graphics::TColor FHotSpotColor;
	Graphics::TColor FVisitedColor;
	Graphics::TColor FOverColor;
	int FVisitedMaxCount;
	Graphics::TColor FBackGround;
	int FFontSize;
	bool FProcessing;
	AnsiString FAction;
	AnsiString FFormTarget;
	AnsiString FEncType;
	AnsiString FMethod;
	Classes::TStringList* FStringList;
	int FImageCacheCount;
	bool FNoSelect;
	Stdctrls::TScrollStyle FScrollBars;
	THTMLBorderStyle FBorderStyle;
	bool FDither;
	int FCaretPos;
	ThtmlViewerOptions FOptions;
	int sbWidth;
	int ScrollWidth;
	int MaxVertical;
	bool MouseScrolling;
	bool LeftButtonDown;
	bool MiddleScrollOn;
	int MiddleY;
	bool Hiliting;
	Graphics::TFontCharset FCharset;
	int FPage;
	Controls::TMouseEvent FOnMouseDouble;
	bool HotSpotAction;
	int FMarginHeight;
	int FMarginWidth;
	AnsiString FServerRoot;
	Litesubs::TSectionList* FSectionList;
	Classes::TMemoryStream* FImageStream;
	Litesubs::TExpandNameEvent FOnExpandName;
	Extctrls::TTimer* HTMLTimer;
	HIDESBASE MESSAGE void __fastcall WMSize(Messages::TWMSize &Message);
	void __fastcall ScrollTo(int Y);
	void __fastcall Scroll(System::TObject* Sender, Stdctrls::TScrollCode ScrollCode, int &ScrollPos);
	void __fastcall Layout(void);
	void __fastcall SetViewImages(bool Value);
	bool __fastcall GetViewImages(void);
	HIDESBASE void __fastcall SetColor(Graphics::TColor Value);
	AnsiString __fastcall GetBase();
	void __fastcall SetBase(AnsiString Value);
	AnsiString __fastcall GetBaseTarget();
	AnsiString __fastcall GetFURL();
	AnsiString __fastcall GetTitle();
	AnsiString __fastcall GetCurrentFile();
	void __fastcall SetBorderStyle(THTMLBorderStyle Value);
	int __fastcall GetPosition(void);
	void __fastcall SetPosition(int Value);
	int __fastcall GetScrollPos(void);
	void __fastcall SetScrollPos(int Value);
	int __fastcall GetScrollBarRange(void);
	void __fastcall SetHistoryIndex(int Value);
	AnsiString __fastcall GetFontName();
	void __fastcall SetFontName(AnsiString Value);
	AnsiString __fastcall GetPreFontName();
	void __fastcall SetPreFontName(AnsiString Value);
	void __fastcall SetFontSize(int Value);
	void __fastcall SetFontColor(Graphics::TColor Value);
	void __fastcall SetHotSpotColor(Graphics::TColor Value);
	void __fastcall SetActiveColor(Graphics::TColor Value);
	void __fastcall SetVisitedColor(Graphics::TColor Value);
	void __fastcall SetVisitedMaxCount(int Value);
	void __fastcall SetOnImageRequest(Litesubs::TGetImageEvent Handler);
	void __fastcall SetOnScript(Liteun2::TScriptEvent Handler);
	void __fastcall SetOnFormSubmit(Litesubs::TFormSubmitEvent Handler);
	HPALETTE __fastcall GetOurPalette(void);
	void __fastcall SetOurPalette(HPALETTE Value);
	void __fastcall SetDither(bool Value);
	void __fastcall SetCaretPos(int Value);
	MESSAGE void __fastcall WMGetDlgCode(Messages::TMessage &Message);
	void __fastcall BackgroundChange(System::TObject* Sender);
	void __fastcall SubmitForm(System::TObject* Sender, const AnsiString Action, const AnsiString Target, const AnsiString EncType, const AnsiString Method, Classes::TStringList* Results);
	void __fastcall SetImageCacheCount(int Value);
	MESSAGE void __fastcall WMFormSubmit(Messages::TMessage &Message);
	MESSAGE void __fastcall WMMouseScroll(Messages::TMessage &Message);
	void __fastcall SetSelLength(int Value);
	void __fastcall SetSelStart(int Value);
	int __fastcall GetSelLength(void);
	AnsiString __fastcall GetSelText();
	void __fastcall SetNoSelect(bool Value);
	void __fastcall SetHistoryMaxCount(int Value);
	void __fastcall DrawBorder(void);
	void __fastcall DoHilite(int X, int Y);
	void __fastcall SetScrollBars(Stdctrls::TScrollStyle Value);
	void __fastcall SetProcessing(bool Value);
	AnsiString __fastcall GetTarget();
	void __fastcall SetCharset(Graphics::TFontCharset Value);
	Classes::TList* __fastcall GetFormControlList(void);
	Classes::TStringList* __fastcall GetNameList(void);
	Classes::TList* __fastcall GetLinkList(void);
	void __fastcall SetMarginWidth(int Value);
	void __fastcall SetMarginHeight(int Value);
	void __fastcall SetServerRoot(AnsiString Value);
	void __fastcall SetOnObjectClick(Litesubs::TObjectClickEvent Handler);
	void __fastcall FormControlEnterEvent(System::TObject* Sender);
	void __fastcall HandleMeta(System::TObject* Sender, const AnsiString HttpEq, const AnsiString Name, const AnsiString Content);
	void __fastcall SetOptions(ThtmlViewerOptions Value);
	void __fastcall DoImage(System::TObject* Sender, const AnsiString SRC, Classes::TMemoryStream* &Stream);
	void __fastcall SetOnExpandName(Litesubs::TExpandNameEvent Handler);
	bool __fastcall GetWordAtCursor(int X, int Y, int &St, int &En, AnsiString &AWord);
	void __fastcall HTMLTimerTimer(System::TObject* Sender);
	void __fastcall InitLoad(void);
	TPaintPanel* PaintPanel;
	Extctrls::TPanel* BorderPanel;
	T32ScrollBar* VScrollBar;
	Stdctrls::TScrollBar* HScrollBar;
	int Sel1;
	Classes::TStringList* Visited;
	void __fastcall DoLogic(void);
	void __fastcall DoScrollBars(void);
	void __fastcall SetupAndLogic(void);
	bool __fastcall GetURL(int X, int Y, Liteun2::TUrlTarget* &UrlTarg, Litesubs::TImageFormControlObj* &FormControl);
	DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
	DYNAMIC HPALETTE __fastcall GetPalette(void);
	virtual void __fastcall HTMLPaint(System::TObject* Sender);
	virtual void __fastcall HTMLMouseDown(System::TObject* Sender, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	void __fastcall HTMLMouseWheel(System::TObject* Sender, Classes::TShiftState Shift, int WheelDelta, const Types::TPoint &MousePos, bool &Handled);
	virtual void __fastcall HTMLMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall HTMLMouseUp(System::TObject* Sender, Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	void __fastcall HTMLMouseDblClk(const Messages::TWMMouse &Message);
	virtual void __fastcall URLAction(void);
	DYNAMIC bool __fastcall HotSpotClickHandled(void);
	void __fastcall LoadFile(const AnsiString FileName, ThtmlFileType ft);
	virtual void __fastcall PaintWindow(HDC DC);
	void __fastcall UpdateImageCache(void);
	void __fastcall LoadTheStrings(Classes::TStrings* Strings, ThtmlFileType ft);
	void __fastcall AddVisitedLink(const AnsiString S);
	void __fastcall CheckVisitedLinks(void);
	
public:
	System::TObject* FrameOwner;
	int FMarginHeightX;
	int FMarginWidthX;
	__fastcall virtual ThtmlLite(Classes::TComponent* AOwner);
	__fastcall virtual ~ThtmlLite(void);
	virtual AnsiString __fastcall HTMLExpandFilename(const AnsiString Filename);
	void __fastcall LoadFromFile(const AnsiString FileName);
	void __fastcall LoadTextFile(const AnsiString FileName);
	void __fastcall LoadImageFile(const AnsiString FileName);
	void __fastcall LoadStrings(Classes::TStrings* Strings);
	void __fastcall LoadTextStrings(Classes::TStrings* Strings);
	void __fastcall LoadFromBuffer(char * Buffer, int BufSize);
	void __fastcall LoadFromString(const AnsiString S);
	void __fastcall LoadFromStream(Classes::TStream* AStream);
	void __fastcall LoadStream(const AnsiString URL, Classes::TMemoryStream* AStream, ThtmlFileType ft);
	bool __fastcall PositionTo(AnsiString Dest);
	bool __fastcall Find(const AnsiString S, bool MatchCase);
	virtual void __fastcall Clear(void);
	void __fastcall CopyToClipboard(void);
	void __fastcall SelectAll(void);
	void __fastcall ClearHistory(void);
	void __fastcall Reload(void);
	void __fastcall BumpHistory(const AnsiString FileName, const AnsiString Title, int OldPos, ThtmlFileType ft);
	int __fastcall GetSelTextBuf(char * Buffer, int BufSize);
	bool __fastcall InsertImage(const AnsiString Src, Classes::TMemoryStream* Stream);
	DYNAMIC void __fastcall DoEnter(void);
	DYNAMIC void __fastcall DoExit(void);
	virtual void __fastcall Repaint(void);
	int __fastcall FindSourcePos(int DisplayPos);
	int __fastcall FindDisplayPos(int SourcePos, bool Prev);
	bool __fastcall DisplayPosToXy(int DisplayPos, int &X, int &Y);
	bool __fastcall PtInObject(int X, int Y, System::TObject* &Obj);
	__property AnsiString DocumentTitle = {read=GetTitle};
	__property AnsiString URL = {read=GetFURL};
	__property AnsiString Base = {read=GetBase, write=SetBase};
	__property AnsiString BaseTarget = {read=GetBaseTarget};
	__property int Position = {read=GetPosition, write=SetPosition, nodefault};
	__property int VScrollBarPosition = {read=GetScrollPos, write=SetScrollPos, nodefault};
	__property int VScrollBarRange = {read=GetScrollBarRange, nodefault};
	__property AnsiString CurrentFile = {read=GetCurrentFile};
	__property Classes::TStrings* History = {read=FHistory};
	__property Classes::TStrings* TitleHistory = {read=FTitleHistory};
	__property int HistoryIndex = {read=FHistoryIndex, write=SetHistoryIndex, nodefault};
	__property bool Processing = {read=FProcessing, nodefault};
	__property int SelStart = {read=FCaretPos, write=SetSelStart, nodefault};
	__property int SelLength = {read=GetSelLength, write=SetSelLength, nodefault};
	__property AnsiString SelText = {read=GetSelText};
	__property AnsiString Target = {read=GetTarget};
	__property HPALETTE Palette = {read=GetOurPalette, write=SetOurPalette, nodefault};
	__property bool Dither = {read=FDither, write=SetDither, default=1};
	__property int CaretPos = {read=FCaretPos, write=SetCaretPos, nodefault};
	__property Classes::TList* FormControlList = {read=GetFormControlList};
	__property Classes::TStringList* NameList = {read=GetNameList};
	__property Classes::TList* LinkList = {read=GetLinkList};
	__property Litesubs::TSectionList* SectionList = {read=FSectionList};
	__property Litesubs::TExpandNameEvent OnExpandName = {read=FOnExpandName, write=SetOnExpandName};
	
__published:
	__property THotSpotEvent OnHotSpotCovered = {read=FOnHotSpotCovered, write=FOnHotSpotCovered};
	__property THotSpotClickEvent OnHotSpotClick = {read=FOnHotSpotClick, write=FOnHotSpotClick};
	__property Litesubs::TGetImageEvent OnImageRequest = {read=FOnImageRequest, write=SetOnImageRequest};
	__property Liteun2::TScriptEvent OnScript = {read=FOnScript, write=SetOnScript};
	__property Litesubs::TFormSubmitEvent OnFormSubmit = {read=FOnFormSubmit, write=SetOnFormSubmit};
	__property Classes::TNotifyEvent OnHistoryChange = {read=FOnHistoryChange, write=FOnHistoryChange};
	__property bool ViewImages = {read=GetViewImages, write=SetViewImages, default=1};
	__property Enabled  = {default=1};
	__property TabStop  = {default=0};
	__property TabOrder  = {default=-1};
	__property Align  = {default=0};
	__property Name ;
	__property Tag  = {default=0};
	__property PopupMenu ;
	__property ShowHint ;
	__property Height  = {default=150};
	__property Width  = {default=150};
	__property Graphics::TColor DefBackground = {read=FBackGround, write=SetColor, default=-2147483633};
	__property THTMLBorderStyle BorderStyle = {read=FBorderStyle, write=SetBorderStyle, nodefault};
	__property Visible  = {default=1};
	__property int HistoryMaxCount = {read=FHistoryMaxCount, write=SetHistoryMaxCount, nodefault};
	__property AnsiString DefFontName = {read=GetFontName, write=SetFontName};
	__property AnsiString DefPreFontName = {read=GetPreFontName, write=SetPreFontName};
	__property int DefFontSize = {read=FFontSize, write=SetFontSize, default=12};
	__property Graphics::TColor DefFontColor = {read=FFontColor, write=SetFontColor, default=-2147483630};
	__property Graphics::TColor DefHotSpotColor = {read=FHotSpotColor, write=SetHotSpotColor, default=16711680};
	__property Graphics::TColor DefVisitedLinkColor = {read=FVisitedColor, write=SetVisitedColor, default=8388736};
	__property Graphics::TColor DefOverLinkColor = {read=FOverColor, write=SetActiveColor, default=16711680};
	__property int VisitedMaxCount = {read=FVisitedMaxCount, write=SetVisitedMaxCount, default=50};
	__property int ImageCacheCount = {read=FImageCacheCount, write=SetImageCacheCount, default=5};
	__property bool NoSelect = {read=FNoSelect, write=SetNoSelect, nodefault};
	__property Stdctrls::TScrollStyle ScrollBars = {read=FScrollBars, write=SetScrollBars, default=3};
	__property Graphics::TFontCharset CharSet = {read=FCharset, write=SetCharset, nodefault};
	__property int MarginHeight = {read=FMarginHeight, write=SetMarginHeight, default=5};
	__property int MarginWidth = {read=FMarginWidth, write=SetMarginWidth, default=10};
	__property AnsiString ServerRoot = {read=FServerRoot, write=SetServerRoot};
	__property ThtmlViewerOptions htOptions = {read=FOptions, write=SetOptions, nodefault};
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnMouseDown ;
	__property OnKeyDown ;
	__property OnKeyUp ;
	__property OnKeyPress ;
	__property OnEnter ;
	__property TProcessingEvent OnProcessing = {read=FOnProcessing, write=FOnProcessing};
	__property Litepars::TIncludeType OnInclude = {read=FOnInclude, write=FOnInclude};
	__property Litepars::TSoundType OnSoundRequest = {read=FOnSoundRequest, write=FOnSoundRequest};
	__property Litepars::TMetaType OnMeta = {read=FOnMeta, write=FOnMeta};
	__property TMetaRefreshType OnMetaRefresh = {read=FOnMetaRefresh, write=FOnMetaRefresh};
	__property TImageClickEvent OnImageClick = {read=FOnImageClick, write=FOnImageClick};
	__property TImageOverEvent OnImageOver = {read=FOnImageOver, write=FOnImageOver};
	__property Litesubs::TObjectClickEvent OnObjectClick = {read=FOnObjectClick, write=SetOnObjectClick};
	__property TRightClickEvent OnRightClick = {read=FOnRightClick, write=FOnRightClick};
	__property Controls::TMouseEvent OnMouseDouble = {read=FOnMouseDouble, write=FOnMouseDouble};
	__property Controls::TDragDropEvent OnDragDrop = {read=GetDragDrop, write=SetDragDrop};
	__property Controls::TDragOverEvent OnDragOver = {read=GetDragOver, write=SetDragOver};
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall ThtmlLite(HWND ParentWindow) : Controls::TWinControl(ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const Word wm_FormSubmit = 0x464;
static const Word wm_MouseScroll = 0x466;
extern PACKAGE void __fastcall Register(void);

}	/* namespace Htmllite */
using namespace Htmllite;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// HTMLLite
