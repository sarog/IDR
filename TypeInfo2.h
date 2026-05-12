//---------------------------------------------------------------------------
#ifndef TypeInfoH
#define TypeInfoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.StdCtrls.hpp>
#include "KnowledgeBase.h"
//---------------------------------------------------------------------------
String __fastcall Guid2String(Byte * Guid);

//---------------------------------------------------------------------------
class TFTypeInfo_11011981 : public TForm {
    __published: // IDE-managed Components
    TMemo * memDescription;
    TPanel *Panel1;
    TButton *bSave;
    void __fastcall FormKeyDown(TObject *Sender, Word &Key, TShiftState Shift);
    void __fastcall bSaveClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);

private: // User declarations
    int RTTIKind;
    DWord RTTIAdr;
    String RTTIName;

public: // User declarations
    __fastcall TFTypeInfo_11011981(TComponent *Owner);
    void __fastcall ShowKbInfo(MTypeInfo *tInfo);
    String __fastcall GetRTTI(DWord adr);
    String __fastcall GetCppTypeInfo(DWord adr, int *size, int action);
    void __fastcall ShowRTTI(DWord adr);
};

//---------------------------------------------------------------------------
extern PACKAGE TFTypeInfo_11011981 *FTypeInfo_11011981;
//---------------------------------------------------------------------------
#endif
