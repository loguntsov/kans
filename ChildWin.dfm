�
 TMDICHILD 0
  TPF0	TMDIChildMDIChildTagLeft� Top� Width�HeightqCaption	MDI ChildColor	clBtnFace
ParentFont		FormStyle
fsMDIChildOldCreateOrderPosition	poDefaultVisible	WindowStatewsMaximizedOnClose	FormCloseOnCreate
FormCreatePixelsPerInch`
TextHeight TPageControlPageControl1Left Top Width�HeightT
ActivePage	TabSheet3AlignalClient	MultiLine	TabIndexTabOrder  	TTabSheet	TabSheet1Caption(   Ввод начальных данных TPanelPanel1Left Top#Width�HeightAlignalBottom
BevelOuterbvNoneTabOrder  TPanelPanel3LeftgTop WidthkHeightAlignalRight
BevelOuter	bvLoweredTabOrder  TSpeedButtonSpeedButton1Left Top WidthmHeightCaption   Далее Flat	
Glyph.Data
*  &  BM&      6  (               �                        �  �   �� �   � � ��  ��� ��� �Ȥ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 ��� ��� ���   �  �   �� �   � � ��  ��� LayoutblGlyphRightOnClickSpeedButton1Click    
TScrollBox
ScrollBox1Left Top Width�Height#HorzScrollBar.ButtonSizeHorzScrollBar.ParentColorHorzScrollBar.Size
HorzScrollBar.Style
ssHotTrackHorzScrollBar.Tracking	VertScrollBar.ButtonSizeVertScrollBar.IncrementVertScrollBar.Size
VertScrollBar.Style
ssHotTrackVertScrollBar.Tracking	AlignalClientBorderStylebsNoneTabOrder TLabelLabel1LeftTopWidthnHeightCaption&   Количество участков:  TLabelLabel4LeftTophWidth0HeightCaptione   Минимальное заглубление трубопровода по верху трубы ,м  TLabelLabel5LeftTop� Width.HeightCaptiond   Максимальное заглубление трубопровода по низу трубы,м  TLabelLabel7LeftTop� Width+HeightCaption�   Приращение к макисмальному заглублению при котором оптимизируется положение насосоной станции,%WordWrap	  TLabelLabel2LeftTop8Width� HeightCaption2   Материал труб по умолчанию:  TLabelLabel10LeftTop� Width� HeightCaptionS   Максимальное суточное количество осадков, мм  TLabelLabel6LeftTop� WidthHeightCaption   4;O  TLabelLabel8LeftTop$Width0HeightCaption   Тип сети:  TEditNumberSegmentLeftxTopWidthQHeight	MaxLengthTabOrder Text1OnChangeNumberSegmentChange  	TComboBoxSortamentBoxLeft TopHWidth�HeightStylecsDropDownListBiDiModebdLeftToRight
ItemHeight ParentBiDiModeTabOrder  TEditHminEditLeft@TopdWidth5HeightTabOrderText2  TEditHmaxEditLeft@Top|Width5HeightTabOrderText8  TEdit	dHmaxEditLeft@Top� Width5HeightTabOrderText0  TUpDownUpDownLeft� TopWidthHeight	AssociateNumberSegmentMinMax�PositionTabOrderWrap
OnChangingUpDownChanging  TEditMdEditLeft@Top� Width5HeightTabOrderText99  	TComboBox
MdComboBoxLeft Top� WidthUHeightStylecsDropDownList
ItemHeight Sorted	TabOrderOnChangeMdComboBoxChange  	TComboBox	ComboTypeLeft8Top Width� HeightStylecsDropDownList
ItemHeightTabOrderItems.Strings"   Внутриквартальная   Уличная     	TTabSheet	TabSheet2Caption"   Ввод отметок земли TStringGridGeodezicCellsLeft Top Width�Height8AlignalClientColCountRowCountOptionsgoFixedVertLinegoFixedHorzLine
goVertLine
goHorzLinegoRangeSelectgoColSizing	goEditing TabOrder 	ColWidths!� �     	TTabSheet	TabSheet3Caption$   Ввод топологии сети TStringGridSegmentCellsLeft Top Width�HeightAlignalClientColCountRowCountOptionsgoFixedVertLinegoFixedHorzLine
goVertLine
goHorzLinegoRangeSelectgoColSizing	goEditing TabOrder OnSelectCellSegmentCellsSelectCellOnSetEditTextSegmentCellsSetEditText	ColWidthsCHFOL   
TScrollBox
ScrollBox2Left TopWidth�Height0HorzScrollBar.ButtonSizeHorzScrollBar.Size
HorzScrollBar.Tracking	VertScrollBar.VisibleAlignalBottomBorderStylebsNoneTabOrder TLabelLabel3Left�TopWidth:HeightHint   Диаметеры трубCaption   Cортамент:  TLabelLabel9LeftTTopWidthIHeight!AutoSizeCaption&   Путевой расход м3/сутWordWrap	  TStaticTextStaticText1Left TopWidthHeightHint(   Номер начальной точкиCaption   7TabOrder   	TComboBoxBegGeoPointBoxLeftTopWidth-HeightHint(   Номер начальной точкиDropDownCount
ItemHeightTabOrderOnChangeBegGeoPointBoxChangeItems.Strings12   	TComboBoxEndGeoPointBoxLeftHTopWidth1HeightHint&   Номер конечной точкиDropDownCount
ItemHeightTabOrderOnChangeBegGeoPointBoxChangeItems.Strings12   TStaticTextStaticText2Left<TopWidth
HeightHint&   Номер конечной точкиCaption   2TabOrder  TStaticTextStaticText3LeftxTopWidth(HeightHint   Длина участкаCaption   Длина:TabOrder  TEdit
SegmentLenLeft� TopWidthAHeightHint   Длина участкаTabOrderOnChangeBegGeoPointBoxChange  TStaticTextStaticText4Left� TopWidthHHeightHintI   Минимальное заглубление начала участкаCaption   Заглубление:TabOrder  TEditSegmentHBegLeft$TopWidth-HeightHintI   Минимальное заглубление начала участкаTabOrderOnChangeBegGeoPointBoxChange  	TComboBoxSegmentSortamentBoxLeftTopWidthQHeightHint   Диаметры трубStylecsDropDownList
ItemHeightTabOrderOnChangeBegGeoPointBoxChangeItems.Strings   (По умолчанию)   TEditSegmentQLeft�TopWidth5HeightTabOrder	    	TTabSheet	TabSheet4Caption    0AG5B
ImageIndex TSpeedButtonSpeedButton2LeftTopWidth� Height!Caption   !G8B05<OnClickSpeedButton2Click    TSaveDialog
SaveDialog
DefaultExtkansFilter5   Канализационные сети *.KANS|*.kansTitle2   Сохранение исходных данныхLeft�Top4  
TPopupMenupopmenuImageLeftPTop�  	TMenuItemaddOtmetCaption*   Добавить отметку земли    