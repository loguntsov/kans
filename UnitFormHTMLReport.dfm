object MDIhtml: TMDIhtml
  Tag = 3
  Left = 201
  Top = 112
  Width = 554
  Height = 394
  Caption = 'MDIhtml'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object WebBrowser: TCppWebBrowser
    Left = 0
    Top = 0
    Width = 546
    Height = 365
    Align = alClient
    TabOrder = 0
    ControlData = {
      4C0000006E380000B92500000000000000000000000000000000000000000000
      000000004C000000000000000000000001000000E0D057007335CF11AE690800
      2B2E126208000000000000004C0000000114020000000000C000000000000046
      8000000000000000000000000000000000000000000000000000000000000000
      00000000000000000100000000000000000000000000000000000000}
  end
  object Memo1: TMemo
    Left = 8
    Top = 12
    Width = 185
    Height = 89
    Lines.Strings = (
      'Memo1')
    TabOrder = 1
    Visible = False
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'htm'
    Filter = 'HTML '#1092#1072#1081#1083#1099'|*.htm; *.html'
    Left = 256
    Top = 160
  end
end
