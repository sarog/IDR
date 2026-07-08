//---------------------------------------------------------------------------
#ifndef ActiveProcessesH
#define ActiveProcessesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <TlHelp32.h>
#include <Psapi.h>
//---------------------------------------------------------------------------
class TFActiveProcesses : public TForm {
__published: // IDE-managed Components
    TButton * btnDump;
    TButton *btnCancel;
    TListView *lvProcesses;
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnDumpClick(TObject *Sender);
    void __fastcall lvProcessesClick(TObject *Sender);

private: // User declarations
    HINSTANCE InstKernel32;
    HINSTANCE InstPSAPI;
    DWord ProcessesNum;
    DWord ModulesNum;
    DWord ProcessIds[1024];
    HMODULE ModuleHandles[1024];

    typedef HANDLE __stdcall(*TCreateToolhelp32Snapshot)(DWord dwFlags, DWord th32ProcessID);
    typedef BOOL __stdcall(*TProcess32First)(HANDLE hSnapshot, PROCESSENTRY32 *lppe);
    typedef BOOL __stdcall(*TProcess32Next)(HANDLE hSnapshot, PROCESSENTRY32 *lppe);
    typedef BOOL __stdcall(*TModule32First)(HANDLE hSnapshot, MODULEENTRY32 *lpme);
    typedef BOOL __stdcall(*TModule32Next)(HANDLE hSnapshot, MODULEENTRY32 *lpme);

    TCreateToolhelp32Snapshot lpCreateToolhelp32Snapshot;
    TProcess32First lpProcess32First;
    TProcess32Next lpProcess32Next;
    TModule32First lpModule32First;
    TModule32Next lpModule32Next;

    typedef BOOL __stdcall(*TEnumProcesses)(HANDLE lpidProcess, DWord cb, PDWORD cbNeeded);
    typedef BOOL __stdcall(*TEnumProcessModules)(HANDLE hProcess, HMODULE lphModule, DWord cb, PDWORD lpcbNeeded);
    typedef DWord __stdcall(*TGetModuleFileNameEx)(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename, DWord nSize);
    typedef BOOL __stdcall(*TGetModuleInformation)(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWord cb);

    TEnumProcesses lpEnumProcesses;
    TEnumProcessModules lpEnumProcessModules;
    TGetModuleFileNameEx lpGetModuleFileNameEx;
    TGetModuleInformation lpGetModuleInformation;

    void ShowProcesses95();
    void ShowProcessesNT();

public: // User declarations
    __fastcall TFActiveProcesses(TComponent *Owner);
    ~TFActiveProcesses();

    void ShowProcesses();
    void __fastcall EnumSections(HANDLE HProcess, Byte *PProcessBase, IMAGE_SECTION_HEADER *Buffer, DWord *Secnum);
    void __fastcall DumpProcess(DWord PID, TMemoryStream *MemStream, DWord *BoC, DWord *PoC, DWord *ImB);
};

//---------------------------------------------------------------------------
extern PACKAGE TFActiveProcesses *FActiveProcesses;
//---------------------------------------------------------------------------
#endif
