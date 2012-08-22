//---------------------------------------------------------------------------

#ifndef UnitMdiMapH
#define UnitMdiMapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "mapcmp.hpp"
#include "mapviewa.hpp"
#include "mapwina.hpp"
#include <Menus.hpp>
#include "DlgSel.hpp"
#include "mapcompa.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include "MapDialg.hpp"
#include "MapMtr3D.hpp"
#include "maction.hpp"
#include "mactpnt.hpp"
#include "MapSect.hpp"
#include "mapobj.hpp"
#include "mapobja.hpp"
#include "mappoint.hpp"
#include <list>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
#define KANSTRUBA list <KANSTruba>
#define KANSPOINT list <KANSPoint>

class KANSPoint
{
public:
    double X;
    double Y;
    double h;
    double Q;
};
class KANSTruba
{
public:
    KANSPOINT::iterator BegPoint;
    KANSPOINT::iterator EndPoint;
    double Len;
    double Q;
    void View(TCanvas *aCanvas,TMapPoint *aPoint);
};
class TMdiMap : public TForm
{
__published:	// IDE-managed Components
    TMapView *Map;
    TMainMenu *MainMenu;
    TMenuItem *mnuMap;
    TMenuItem *N1;
    TMapSelectDialog *MapSelectDialog;
    TControlBar *ControlBar1;
    TToolBar *ToolBar1;
    TToolButton *ZoomPlus;
    TToolButton *ZoomMinus;
    TOpenMapDialog *OpenMapDialog;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *mnuMapViewObj;
    TMapPoint *MapPoint;
    TPopupMenu *PopupMenu;
    TMenuItem *popupAddPoint;
    TMenuItem *N6;
    TMenuItem *popupBeginPoint;
    TMenuItem *popupPaintSegment;
    TMenuItem *mnuMapRefresh;
    void __fastcall N1Click(TObject *Sender);
    void __fastcall ZoomPlusClick(TObject *Sender);
    void __fastcall ZoomMinusClick(TObject *Sender);
    void __fastcall mnuMapViewObjClick(TObject *Sender);
    void __fastcall MapMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall popupAddPointClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall MapMapPaintObject(TObject *Sender, TCanvas *aCanvas,
          TRect &aMapPaintRect, int aLeftPosition, int aTopPosition,
          TMapPaintEventType aMapPaintEventType);
    void __fastcall MapMapPaint(TObject *Sender, TCanvas *aCanvas,
          TRect &MapPaintRect);
    void __fastcall N6Click(TObject *Sender);
    void __fastcall popupBeginPointClick(TObject *Sender);
    void __fastcall popupPaintSegmentClick(TObject *Sender);
    void __fastcall mnuMapRefreshClick(TObject *Sender);
    void __fastcall MapDblClick(TObject *Sender);
    void __fastcall MapClick(TObject *Sender);
private:	// User declarations
    TCanvas *MapCanvas;
    int Xa,Ya;    
public:		// User declarations
    __fastcall TMdiMap(TComponent* Owner);
    KANSPOINT KANSPoints;
    KANSTRUBA KANSTruby;
    void PaintObj(void);
    KANSPOINT::iterator FindPoint(int X, int Y);
    KANSPOINT::iterator BeginPoint;
    bool PointIsMove;
};


//---------------------------------------------------------------------------
extern PACKAGE TMdiMap *MdiMap;
//---------------------------------------------------------------------------
#endif
