object MdiMap: TMdiMap
  Left = 240
  Top = 120
  Width = 696
  Height = 480
  Caption = 'MdiMap'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  Menu = MainMenu
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Map: TMapView
    Left = 0
    Top = 30
    Width = 688
    Height = 401
    Rendering = True
    BackStretch = False
    BackTile = False
    BackColor = clSilver
    PrintScale = 0
    PlaceOut = PP_PLANE
    OnMapPaint = MapMapPaint
    OnMapPaintObject = MapMapPaintObject
    Align = alClient
    PopupMenu = PopupMenu
    TabOrder = 0
    OnDblClick = MapDblClick
    OnMouseMove = MapMouseMove
    OnClick = MapClick
    MapView = False
    ViewScale = 0
    MapBright = 0
    MapContrast = 0
    MapContour = False
    MapWinAPI = False
    PropertyFont.Charset = DEFAULT_CHARSET
    PropertyFont.Color = clWindowText
    PropertyFont.Height = -11
    PropertyFont.Name = 'MS Sans Serif'
    PropertyFont.Style = []
    GroupEnable = False
    MapMtrs.MapMtr = ()
    ErrorHeight = -11111111
    MapRsts.MapRst = ()
    MapSites.MapSite = ()
    PrecisionHeight = False
  end
  object ControlBar1: TControlBar
    Left = 0
    Top = 0
    Width = 688
    Height = 30
    Align = alTop
    AutoSize = True
    TabOrder = 1
    object ToolBar1: TToolBar
      Left = 11
      Top = 2
      Width = 102
      Height = 22
      Caption = 'ToolBar1'
      Flat = True
      Images = MainFormKansProgram.ImageList1
      TabOrder = 0
      object ZoomPlus: TToolButton
        Left = 0
        Top = 0
        Caption = 'ZoomPlus'
        ImageIndex = 22
        OnClick = ZoomPlusClick
      end
      object ZoomMinus: TToolButton
        Left = 23
        Top = 0
        Caption = 'ZoomMinus'
        ImageIndex = 23
        OnClick = ZoomMinusClick
      end
    end
  end
  object MainMenu: TMainMenu
    Images = MainFormKansProgram.ImageList1
    Left = 100
    Top = 44
    object mnuMap: TMenuItem
      Caption = #1050#1072#1088#1090#1072
      GroupIndex = 1
      object N3: TMenuItem
        Caption = #1052#1072#1089#1096#1090#1072#1073
        object N4: TMenuItem
          Caption = #1059#1074#1077#1083#1080#1095#1080#1090#1100
          ImageIndex = 22
          OnClick = ZoomPlusClick
        end
        object N5: TMenuItem
          Caption = #1059#1084#1077#1085#1100#1096#1080#1090#1100
          ImageIndex = 23
          OnClick = ZoomMinusClick
        end
      end
      object mnuMapViewObj: TMenuItem
        Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1086#1073#1098#1077#1082#1090#1099' ...'
        OnClick = mnuMapViewObjClick
      end
      object mnuMapRefresh: TMenuItem
        Caption = #1054#1073#1085#1086#1074#1080#1090#1100
        OnClick = mnuMapRefreshClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object N1: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' ...'
        OnClick = N1Click
      end
    end
  end
  object MapSelectDialog: TMapSelectDialog
    MapView = Map
    Left = 36
    Top = 40
  end
  object OpenMapDialog: TOpenMapDialog
    MapView = Map
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Caption = #1054#1090#1082#1088#1099#1090#1100' ...'
    Preview = True
    Left = 20
    Top = 48
  end
  object MapPoint: TMapPoint
    MapView = Map
    PlaceInp = PP_PICTURE
    PlaceOut = PP_PLANE
    Left = 72
    Top = 44
    FX = 0
    FY = 0
  end
  object PopupMenu: TPopupMenu
    Left = 128
    Top = 40
    object popupAddPoint: TMenuItem
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1090#1086#1095#1082#1091
      OnClick = popupAddPointClick
    end
    object N6: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1090#1086#1095#1082#1091
      OnClick = N6Click
    end
    object popupBeginPoint: TMenuItem
      Caption = #1054#1090#1084#1077#1090#1080#1090#1100' '#1082#1072#1082' '#1085#1072#1095#1072#1083#1100#1085#1091#1102' '#1090#1086#1095#1082#1091' '#1091#1095#1072#1089#1090#1082#1072
      OnClick = popupBeginPointClick
    end
    object popupPaintSegment: TMenuItem
      Caption = #1053#1072#1088#1080#1089#1086#1074#1072#1090#1100' '#1091#1095#1072#1089#1090#1086#1082
      OnClick = popupPaintSegmentClick
    end
  end
end
