//---------------------------------------------------------------------------
#ifndef IDCGenH
#define IDCGenH

#include "Main.h"

typedef struct {
    int index;   //Index of name in names
    int counter; //Counter
} REPNAMEINFO, *PREPNAMEINFO;

class TIDCGen {
public:
    TIDCGen(FILE *FIdc, int splitSize);
    ~TIDCGen();
    void __fastcall NewIDCPart(FILE *FIdc);
    void __fastcall DeleteName(const int pos);
    int __fastcall MakeByte(const int pos);
    int __fastcall MakeWord(const int pos);
    int __fastcall MakeDword(const int pos);
    int __fastcall MakeQword(const int pos);
    int __fastcall MakeArray(const int pos, const int num);
    int __fastcall MakeShortString(int pos);
    int __fastcall MakeCString(int pos);
    void __fastcall MakeLString(const int pos);
    void __fastcall MakeWString(const int pos);
    void __fastcall MakeUString(const int pos);
    int __fastcall MakeCode(int pos);
    void __fastcall MakeFunction(DWord adr);
    void __fastcall MakeComment(const int pos, String text);
    int __fastcall OutputAttrData(int pos);
    void __fastcall OutputHeaderFull();
    void __fastcall OutputHeaderShort();
    int __fastcall OutputRTTIHeader(Byte kind, int pos);
    void __fastcall OutputRTTIInteger(Byte kind, int pos);
    void __fastcall OutputRTTIChar(Byte kind, int pos);
    void __fastcall OutputRTTIEnumeration(Byte kind, int pos, DWord adr);
    void __fastcall OutputRTTIFloat(Byte kind, int pos);
    void __fastcall OutputRTTIString(Byte kind, int pos);
    void __fastcall OutputRTTISet(Byte kind, int pos);
    void __fastcall OutputRTTIClass(Byte kind, int pos);
    void __fastcall OutputRTTIMethod(Byte kind, int pos);
    void __fastcall OutputRTTIWChar(Byte kind, int pos);
    void __fastcall OutputRTTILString(Byte kind, int pos);
    void __fastcall OutputRTTIWString(Byte kind, int pos);
    void __fastcall OutputRTTIVariant(Byte kind, int pos);
    void __fastcall OutputRTTIArray(Byte kind, int pos);
    void __fastcall OutputRTTIRecord(Byte kind, int pos);
    void __fastcall OutputRTTIInterface(Byte kind, int pos);
    void __fastcall OutputRTTIInt64(Byte kind, int pos);
    void __fastcall OutputRTTIDynArray(Byte kind, int pos);
    void __fastcall OutputRTTIUString(Byte kind, int pos);
    void __fastcall OutputRTTIClassRef(Byte kind, int pos);
    void __fastcall OutputRTTIPointer(Byte kind, int pos);
    void __fastcall OutputRTTIProcedure(Byte kind, int pos);
    void __fastcall OutputVMT(int pos, PInfoRec recN);
    int __fastcall OutputVMTHeader(int pos, String vmtName);
    void __fastcall OutputIntfTable(int pos);
    void __fastcall OutputIntfVTable(int pos, DWord stopAdr);
    void __fastcall OutputAutoTable(int pos);
    void __fastcall OutputAutoPTable(int pos);
    void __fastcall OutputInitTable(int pos);
    void __fastcall OutputFieldTable(int pos);
    void __fastcall OutputFieldTTable(int pos);
    void __fastcall OutputMethodTable(int pos);
    void __fastcall OutputVmtMethodEntry(int pos);
    int __fastcall OutputVmtMethodEntryTail(int pos);
    void __fastcall OutputDynamicTable(int pos);
    void __fastcall OutputResString(int pos, PInfoRec recN);
    int __fastcall OutputProc(int pos, PInfoRec recN, bool imp);
    void __fastcall OutputData(int pos, PInfoRec recN);
    PREPNAMEINFO __fastcall GetNameInfo(int idx);
    FILE *idcF;
    String unitName;
    String itemName;
    TStringList *names;
    TList *repeated;
    int SplitSize;     //Maximum output bytes if idc splitted
    int CurrentPartNo; //Current part number (filename looks like XXX_NN.idc)
    int CurrentBytes;  //Current part output bytes
};

class TSaveIDCDialog : public TSaveDialog {
public:
    __fastcall TSaveIDCDialog(TComponent *AOwner, char *TemplateName);

protected:
    virtual void __fastcall WndProc(Messages::TMessage &Message);
};

//---------------------------------------------------------------------------
#endif
