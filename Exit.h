//----------------------------------------------------------------------------
#ifndef ExitH
#define ExitH
//----------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Buttons.hpp>

//----------------------------------------------------------------------------
class TFExit_11011981 : public TForm {
    __published:
    TButton * OKBtn;
    TButton *CancelBtn;
    TBevel *Bevel1;
    TCheckBox *cbDontSaveProject;
    void __fastcall OKBtnClick(TObject *Sender);
    void __fastcall CancelBtnClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);

private:
public:
    virtual __fastcall TFExit_11011981(TComponent *AOwner);
};

//----------------------------------------------------------------------------
extern PACKAGE TFExit_11011981 *FExit_11011981;
//----------------------------------------------------------------------------
#endif
