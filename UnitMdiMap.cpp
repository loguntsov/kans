//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMdiMap.h"
#include "Main.h"
#include "math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mapcmp"
#pragma link "mapviewa"
#pragma link "mapwina"
#pragma link "DlgSel"
#pragma link "mapcompa"
#pragma link "MapDialg"
#pragma link "MapMtr3D"
#pragma link "maction"
#pragma link "mactpnt"
#pragma link "MapSect"
#pragma link "mapobj"
#pragma link "mapobja"
#pragma link "mappoint"
#pragma resource "*.dfm"

TMdiMap *MdiMap;
//---------------------------------------------------------------------------
__fastcall TMdiMap::TMdiMap(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMdiMap::N1Click(TObject *Sender)
{
    if (OpenMapDialog->Execute())
    {
        Map->MapVisible=true;
        Map->Visible=true;
        AnsiString nameMTW = "podolsk.mtw";
        Map->MapMtrs->Append(nameMTW);
        //mapSetMtrView(hMap,1,0);
    }
}
//---------------------------------------------------------------------------


void __fastcall TMdiMap::ZoomPlusClick(TObject *Sender)
{
    Map->ViewScale /=2;    
}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::ZoomMinusClick(TObject *Sender)
{
    Map->ViewScale *=2;        
}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::mnuMapViewObjClick(TObject *Sender)
{
     // Активизировать диалог состава отображения
    if(MapSelectDialog->Execute(Map->ViewSelect,false) == true)
    {
        // Если в диалоге нажата кнопка УСТАНОВИТЬ - перерисовать документ с новым
        // объектовым составом
        Map->Invalidate();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::MapMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    MapPoint->PlaceInp = PP_PICTURE;
    MapPoint->PlaceOut = PP_PLANE;

    MapPoint->X=Map->MapLeft+X;
    MapPoint->Y=Map->MapTop+Y;
    double h = mapGetHeightValue(Map->MapHandle,MapPoint->X,MapPoint->Y);
    MainFormKansProgram->StatusBar->Panels->Items[1]->Text="X : "+IntToStr(int(MapPoint->X))+"   Y : "+IntToStr(int(MapPoint->Y))+"   h : "+FloatToStr(h);
    Xa=X;Ya=Y;
}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;        
}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::popupAddPointClick(TObject *Sender)
{
    MapPoint->PlaceInp = PP_PICTURE;
    MapPoint->PlaceOut = PP_PLANE;

    MapPoint->X=Xa+Map->MapLeft;
    MapPoint->Y=Ya+Map->MapTop;
    KANSPoint a;
    a.X=MapPoint->X;
    a.Y=MapPoint->Y;
    a.Q=0;
    a.h=mapGetHeightValue(Map->MapHandle,MapPoint->X,MapPoint->Y);
    KANSPoints.push_back(a);
    Map->Refresh();
    //PaintObj();
}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::FormCreate(TObject *Sender)
{
    BeginPoint=NULL;
    PointIsMove=false;    
}
//---------------------------------------------------------------------------


void TMdiMap::PaintObj(void)
{
    MapPoint->PlaceInp = PP_PLANE;
    MapPoint->PlaceOut = PP_PICTURE;
    MapCanvas->Pen->Color=clRed;
    MapCanvas->Pen->Style=psSolid;
    MapCanvas->Brush->Color=clRed;
    MapCanvas->Brush->Style=bsSolid;
    TRect MapBorder=MapCanvas->ClipRect;
    int MapLeft=MapBorder.Left;
    int MapTop=MapBorder.Top; 
    for (KANSPOINT::iterator i=KANSPoints.begin();i!=KANSPoints.end();i++)
    {
        MapPoint->X=(*i).X;
        MapPoint->Y=(*i).Y;
        int x0=MapPoint->X;//-MapLeft;
        int y0=MapPoint->Y;//-MapTop;
        MapCanvas->Ellipse(x0-5,y0-5,x0+5,y0+5);
    }
    if (BeginPoint!=NULL)
    {
        MapPoint->X=(*BeginPoint).X;
        MapPoint->Y=(*BeginPoint).Y;
        int x0=MapPoint->X;//-Map->MapLeft;
        int y0=MapPoint->Y;//-Map->MapTop;
        MapCanvas->Ellipse(x0-10,y0-10,x0+10,y0+10);
    }

    for (KANSTRUBA::iterator i=KANSTruby.begin();i!=KANSTruby.end();i++)
    {
        (*i).View(MapCanvas,MapPoint);
    }

}
void __fastcall TMdiMap::MapMapPaintObject(TObject *Sender,
      TCanvas *aCanvas, TRect &aMapPaintRect, int aLeftPosition,
      int aTopPosition, TMapPaintEventType aMapPaintEventType)
{
    //MapCanvas=aCanvas;


}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::MapMapPaint(TObject *Sender, TCanvas *aCanvas,
      TRect &MapPaintRect)
{
    Map->UpdatePictureBorderForAll();
    MapCanvas=aCanvas;
    PaintObj();

}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::N6Click(TObject *Sender)
{
    KANSPOINT::iterator deleted;
    deleted=FindPoint(Xa,Ya);
    if (deleted!=NULL)
    {
            KANSPoints.erase(deleted);
            Map->Refresh();
    }
}
//---------------------------------------------------------------------------


KANSPOINT::iterator TMdiMap::FindPoint(int X, int Y)
{
    MapPoint->PlaceInp = PP_PLANE;
    MapPoint->PlaceOut = PP_PICTURE;

    for (KANSPOINT::iterator i=KANSPoints.begin();i!=KANSPoints.end();i++)
    {
        MapPoint->X=(*i).X;
        MapPoint->Y=(*i).Y;
        int x0=MapPoint->X-Map->MapLeft;
        int y0=MapPoint->Y-Map->MapTop;

        if (abs(x0-Xa)<5 && abs(y0-Ya)<5)
        {
            return i;
        }
    }
    return NULL;
}
void __fastcall TMdiMap::popupBeginPointClick(TObject *Sender)
{
    KANSPOINT::iterator selected;
    selected=FindPoint(Xa,Ya);
    if (selected!=NULL)
    {
        BeginPoint=selected;
        Map->Refresh();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::popupPaintSegmentClick(TObject *Sender)
{
    if (BeginPoint!=NULL && !PointIsMove)
    {
        KANSPOINT::iterator selected;
        selected=FindPoint(Xa,Ya);
        if (selected!=NULL && selected!=BeginPoint)
        {
            KANSTruba a;
            a.BegPoint = BeginPoint;
            a.EndPoint = selected;
            KANSTruby.push_back(a);
            BeginPoint=NULL;
            Map->Refresh();//PaintObj();
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMdiMap::mnuMapRefreshClick(TObject *Sender)
{
    Map->Refresh();    
}
//---------------------------------------------------------------------------
//
// Class KANSTruby
//
//---------------------------------------------------------------------------

void KANSTruba::View(TCanvas *MapCanvas,TMapPoint *MapPoint)
{
    MapPoint->PlaceInp = PP_PLANE;
    MapPoint->PlaceOut = PP_PICTURE;


    MapCanvas->Pen->Color=((*EndPoint).h>(*BegPoint).h)?clRed:clBlack;
    MapCanvas->Pen->Style=psSolid;
    MapCanvas->Pen->Width=2;

    MapPoint->X=(*BegPoint).X;
    MapPoint->Y=(*BegPoint).Y;
    int x0=MapPoint->X;//-Map->MapLeft;
    int y0=MapPoint->Y;//-Map->MapTop;

    MapPoint->X=(*EndPoint).X;
    MapPoint->Y=(*EndPoint).Y;
    int x1=MapPoint->X;//-Map->MapLeft;
    int y1=MapPoint->Y;//-Map->MapTop;

    MapCanvas->MoveTo(x0,y0);
    MapCanvas->LineTo(x1,y1);
    float xs=float(x0+x1)/2;
    float ys=float(y0+y1)/2;
    double alfa=atan2(y1-y0,x1-x0);
    const double dalfa=20.0*M_PI/180.0;
    const double r=20;
    MapCanvas->MoveTo(xs,ys);
    MapCanvas->LineTo(xs-r*cos(alfa+dalfa),ys-r*sin(alfa+dalfa));

    MapCanvas->MoveTo(xs,ys);
    MapCanvas->LineTo(xs-r*cos(alfa-dalfa),ys-r*sin(alfa-dalfa));

}

void __fastcall TMdiMap::MapDblClick(TObject *Sender)
{

    if (BeginPoint==NULL && !PointIsMove)
    {
        KANSPOINT::iterator selected;
        selected=FindPoint(Xa,Ya);
        if (selected!=NULL)
        {
            BeginPoint=selected;
            PointIsMove=true;
            Map->Refresh();
        }
    } else
    if (BeginPoint!=NULL && PointIsMove)
    {
        KANSPOINT::iterator selected;
        MapPoint->PlaceInp = PP_PICTURE;
        MapPoint->PlaceOut = PP_PLANE;

        MapPoint->X=Xa+Map->MapLeft;
        MapPoint->Y=Ya+Map->MapTop;
        (*BeginPoint).X=MapPoint->X;
        (*BeginPoint).Y=MapPoint->Y;
        (*BeginPoint).h=mapGetHeightValue(Map->MapHandle,MapPoint->X,MapPoint->Y);
        PointIsMove=false;
        BeginPoint=NULL;
        Map->Refresh();
    }

}
//---------------------------------------------------------------------------



void __fastcall TMdiMap::MapClick(TObject *Sender)
{
    if (BeginPoint==NULL && !PointIsMove)
    {
        popupBeginPointClick(Sender);
    } else
    {
        popupPaintSegmentClick(Sender);
    }
}
//---------------------------------------------------------------------------

