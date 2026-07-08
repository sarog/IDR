#ifndef IDR_IDR64MANAGER_H
#define IDR_IDR64MANAGER_H

#ifdef IDR64

#include <System.Classes.hpp>
#include <Vcl.Forms.hpp>

#include "Decompiler.h"
//---------------------------------------------------------------------------
class Idr64Manager {
public:
    Idr64Manager();
    ~Idr64Manager();

public:
    String GetVersion() const {return IDR64Version;}

    //Project API
    void CleanProject();
    void CreateDBs(DWord _TotalSize);

    //Flags API
    void __fastcall ClearFlag(DWord flag, int pos);
    void __fastcall ClearFlags(DWord flag, int pos, int num);
    bool __fastcall IsFlagSet(DWord flag, int pos);
    bool __fastcall IsFlagEmpty(int pos);
    void __fastcall SetFlag(DWord flag, int pos);
    void __fastcall SetFlags(DWord flag, int pos, int num);
    void __fastcall XorFlag(DWord Val, int pos);


    //TODO (future): hide vars into API (it is bad to have direct access to class members)
    DWord     *Flags;     //flags for used data

    String     WrkDir;

    //lookup instructions Up/Down API
    int __fastcall GetNearestArgC(int fromPos);
    int __fastcall GetNearestDownInstruction(int fromPos);
    int __fastcall GetNearestDownInstruction(int fromPos, String Instruction);
    int __fastcall GetNearestUpPrefixFs(int fromPos);
    int __fastcall GetNearestUpInstruction(int fromPos);
    int __fastcall GetNthUpInstruction(int fromPos, int N);
    int __fastcall GetNearestUpInstruction(int fromPos, int toPos);
    int __fastcall GetNearestUpInstruction(int fromPos, int toPos, int no);
    int __fastcall GetNearestUpInstruction1(int fromPos, int toPos, String Instruction);
    int __fastcall GetNearestUpInstruction2(int fromPos, int toPos, String Instruction1, String Instruction2);
    int __fastcall BranchGetPrevInstructionType(DWord fromAdr, DWord* jmpAdr, PLoopInfo loopInfo);

    //Resource API
    TResourceInfo* ResInfo() const {return _ResInfo;}

    //Infos
    PInfoRec GetInfos(DWord classAdr);
    PInfoRec GetInfosAt(int Pos);
    void SetInfosAt(int Pos, PInfoRec rec);
    bool HasInfosAt(int Pos);

    //BSS
    PInfoRec AddToBSSInfos(DWord Adr, String AName, String ATypeName);
    void BSSInfosAddObject(String _adr, PInfoRec recN);
    int GetBSSInfosCount() const {return BSSInfos->Count;}
    PInfoRec GetBSSInfosObject(int n) const {return (PInfoRec)BSSInfos->Objects[n];}
    String GetBSSInfosString(int n) const {return BSSInfos->Strings[n];}
    //int GetBSSInfosIndexOf(const String s) const {return BSSInfos->IndexOf(s);}
    PInfoRec GetBSSInfosRec(const String s);


public:
    //Decompiler helpers
    //TODO: maybe put into Decompiler module?
    int __fastcall IsValidCode(DWord fromAdr);
    int  __fastcall IsBoundErr(DWord fromAdr);
    int __fastcall IsInitStackViaLoop(DWord fromAdr, DWord toAdr);
    bool __fastcall IsConnected(DWord fromAdr, DWord toAdr);
    DWord __fastcall IsGeneralCase(DWord fromAdr, int retAdr);
    bool __fastcall IsExit(DWord fromAdr);
    bool __fastcall IsXorMayBeSkipped(DWord fromAdr);

    int __fastcall IsAbs(DWord fromAdr);
    int __fastcall IsIntOver(DWord fromAdr);
    int __fastcall IsInlineLengthCmp(DWord fromAdr);
    int __fastcall IsInlineLengthTest(DWord fromAdr);
    int __fastcall IsInlineDiv(DWord fromAdr, int* div);
    int __fastcall IsInlineMod(DWord fromAdr, int* mod);
    int __fastcall ProcessInt64Equality(DWord fromAdr, DWord* maxAdr);
    int __fastcall ProcessInt64NotEquality(DWord fromAdr, DWord* maxAdr);
    int __fastcall ProcessInt64Comparison(DWord fromAdr, DWord* maxAdr);
    int __fastcall ProcessInt64ComparisonViaStack1(DWord fromAdr, DWord* maxAdr);
    int __fastcall ProcessInt64ComparisonViaStack2(DWord fromAdr, DWord* maxAdr);
    int __fastcall IsInt64Equality(DWord fromAdr, int* skip1, int* skip2, bool *immVal, __int64* Val);
    int __fastcall IsInt64NotEquality(DWord fromAdr, int* skip1, int* skip2, bool *immVal, __int64* Val);
    int __fastcall IsInt64Comparison(DWord fromAdr, int* skip1, int* skip2, bool *immVal, __int64* Val);
    int __fastcall IsInt64ComparisonViaStack1(DWord fromAdr, int* skip1, DWord* simEnd);
    int __fastcall IsInt64ComparisonViaStack2(DWord fromAdr, int* skip1, int* skip2, DWord* simEnd);
    int __fastcall IsInt64Shr(DWord fromAdr);
    int __fastcall IsInt64Shl(DWord fromAdr);

    String __fastcall AnalyzeArguments(DWord fromAdr);
    String __fastcall AnalyzeCall(DWord parentAdr, int callPos, DWord callAdr, PRINFO registers);
    DWord  __fastcall AnalyzeProcInitial(DWord fromAdr);
    void   __fastcall AnalyzeProc1(DWord fromAdr, char xrefType, DWord xrefAdr, int xrefOfs, bool maybeEmb);
    bool   __fastcall AnalyzeProc2(DWord fromAdr, bool addArg, bool AnalyzeRetType, TList *sctx);
    void   __fastcall AnalyzeProc2(DWord fromAdr, bool addArg, bool AnalyzeRetType);


private:
    String  IDR64Version;

    TResourceInfo* _ResInfo;   //Information about forms
    PInfoRec       *Infos;	   //Array of pointers to store items data
    TStringList    *BSSInfos;  //Data from BSS
};

//global instance (TBD: think about Singleton pattern & API like getIdr() )
extern Idr64Manager idr;

//global instance for Vmt
extern DelphiVmt Vmt;

#endif

#endif // IDR_IDR64MANAGER_H
