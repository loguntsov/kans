//---------------------------------------------------------------------------

#ifndef TableReportH
#define TableReportH
//---------------------------------------------------------------------------
enum eTableReportType {tbtText, tbtRTF, tbtHTML};

class CTableReport
{
private:
    TStream *Str;
    //eTabelReportType type;
public:
    CTableReport(eTableReportType Type,TStream *OutPutStream);
    ~CTableReport();
    bool BegTable(void);
    bool EndTable(void);
    bool Cells(AnsiString Text);
    bool NextCells(void);
    bool NextRows();
    bool Text();
    bool SetFont(TFont Name);
    bool SetAlligment(TAlignment Align);
    bool AddTag(AnsiString Tag);
    bool BegParagraph(void);
    bool EndParagraph(void);
    bool Text(AnsiString Text);
}

#endif
