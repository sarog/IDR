//---------------------------------------------------------------------------
#ifndef MiscH
#define MiscH
//---------------------------------------------------------------------------
#include "Decompiler.h"
//---------------------------------------------------------------------------
//Float Type
#define     FT_SINGLE       1
#define     FT_DOUBLE       2
#define     FT_EXTENDED     3
#define     FT_REAL         4
#define     FT_COMP         5
#define     FT_CURRENCY     6

//---------------------------------------------------------------------------
typedef struct {
    int Offset;
    int Size;
    String Name;
    String Type;
} FIELD_INFO, *PFIELD_INFO;

//---------------------------------------------------------------------------
//global API
void __fastcall ScaleForm(TForm * AForm);
int __fastcall Adr2Pos(DWord adr);
String __fastcall Val2Str0(DWord Val);
String __fastcall Val2Str1(DWord Val);
String __fastcall Val2Str2(DWord Val);
String __fastcall Val2Str4(DWord Val);
String __fastcall Val2Str5(DWord Val);
String __fastcall Val2Str8(DWord Val);
DWord __fastcall Pos2Adr(int Pos);
PInfoRec __fastcall AddToBSSInfos(DWord Adr, String AName, String ATypeName);
void __fastcall AddClassAdr(DWord Adr, const String &AName);
void __fastcall AddFieldXref(PFIELDINFO fInfo, DWord ProcAdr, int ProcOfs, char type);
void __fastcall AddPicode(int Pos, Byte Op, String Name, int Ofs);
int __fastcall BranchGetPrevInstructionType(DWord fromAdr, DWord *jmpAdr, PLoopInfo loopInfo);
bool __fastcall CanReplace(const String &fromName, const String &toName);
void __fastcall ClearFlag(DWord flag, int pos);
void __fastcall ClearFlags(DWord flag, int pos, int num);
void __fastcall Copy2Clipboard(TStrings *items, int leftMargin, bool asmCode);
String __fastcall ExtractClassName(const String &AName);
String __fastcall ExtractProcName(const String &AName);
String __fastcall ExtractName(const String &AName);
String __fastcall ExtractType(const String &AName);
void __fastcall FillArgInfo(int k, Byte callkind, PARGINFO argInfo, Byte **p, int *s);
DWord __fastcall FindClassAdrByName(const String &AName);
int __fastcall FloatNameToFloatType(String AName);
String __fastcall GetArrayElementType(String arrType);
int __fastcall GetArrayElementTypeSize(String arrType);
bool __fastcall GetArrayIndexes(String arrType, int ADim, int *LowIdx, int *HighIdx);
int __fastcall GetArraySize(String arrType);
DWord __fastcall GetChildAdr(DWord Adr);
DWord __fastcall GetClassAdr(const String &AName);
int __fastcall GetClassSize(DWord adr);
String __fastcall GetClsName(DWord adr);
String __fastcall GetDefaultProcName(DWord adr);
String __fastcall GetDynaInfo(DWord adr, Word id, DWord *dynAdr);
String __fastcall GetDynArrayTypeName(DWord adr);
String __fastcall GetEnumerationString(String TypeName, Variant Val);
String __fastcall GetImmString(int Val);
String __fastcall GetImmString(String TypeName, int Val);
PInfoRec __fastcall GetInfoRec(DWord adr);
int __fastcall GetLastLocPos(int fromAdr);
String __fastcall GetModuleVersion(const String &module);
int __fastcall GetNearestArgA(int fromPos);
int __fastcall GetNearestDownInstruction(int fromPos);
int __fastcall GetNearestDownInstruction(int fromPos, const char *Instruction);
int __fastcall GetNearestUpPrefixFs(int fromPos);
int __fastcall GetNearestUpInstruction(int fromPos);
int __fastcall GetNthUpInstruction(int fromPos, int N);
int __fastcall GetNearestUpInstruction(int fromPos, int toPos);
int __fastcall GetNearestUpInstruction(int fromPos, int toPos, int no);
int __fastcall GetNearestUpInstruction1(int fromPos, int toPos, const char *Instruction);
int __fastcall GetNearestUpInstruction2(int fromPos, int toPos, const char *Instruction1, const char *Instruction2);
DWord __fastcall GetParentAdr(DWord Adr);
String __fastcall GetParentName(DWord adr);
String __fastcall GetParentName(const String &ClassName);
int __fastcall GetParentSize(DWord Adr);
int __fastcall GetProcRetBytes(MProcInfo * pInfo);
int __fastcall GetProcSize(DWord fromAdr);
int __fastcall StrGetRecordSize(String str);
int __fastcall StrGetRecordFieldOffset(String str);
String __fastcall StrGetRecordFieldName(String str);
String __fastcall StrGetRecordFieldType(String str);
int __fastcall GetRecordSize(String AName);
String __fastcall GetRecordFields(int AOfs, String AType);
String __fastcall GetAsmRegisterName(int Idx);
String __fastcall GetDecompilerRegisterName(int Idx);
String __fastcall GetSetString(String TypeName, Byte *ValAdr);
DWord __fastcall GetStopAt(DWord vmtAdr);
DWord __fastcall GetOwnTypeAdr(String AName);
PTypeRec __fastcall GetOwnTypeByName(String AName);
String __fastcall GetTypeDeref(String ATypeName);
Byte __fastcall GetTypeKind(DWord Adr);
Byte __fastcall GetTypeKind(String AName, int *size);
int __fastcall GetRTTIRecordSize(DWord adr);
int __fastcall GetPackedTypeSize(String AName);
String __fastcall GetTypeName(DWord TypeAdr);
int __fastcall GetTypeSize(String AName);
bool __fastcall IsADC(int Idx);
int __fastcall IsBoundErr(DWord fromAdr);
bool __fastcall IsConnected(DWord fromAdr, DWord toAdr);
bool __fastcall IsBplByExport(const char *bpl);
bool __fastcall IsDefaultName(String AName);
DWord __fastcall IsGeneralCase(DWord fromAdr, int retAdr);
bool __fastcall IsExit(DWord fromAdr);
bool __fastcall IsInheritsByAdr(const DWord Adr1, const DWord Adr2);
bool __fastcall IsInheritsByClassName(const String &Name1, const String &Name2);
bool __fastcall IsInheritsByProcName(const String &Name1, const String &Name2);
int __fastcall IsInitStackViaLoop(DWord fromAdr, DWord toAdr);
bool __fastcall IsSameRegister(int Idx1, int Idx2);
int __fastcall IsCopyDynArrayToStack(DWord fromAdr);
bool __fastcall IsValidCodeAdr(DWord Adr);
bool __fastcall IsValidCString(int pos);
bool __fastcall IsValidImageAdr(DWord Adr);
bool __fastcall IsValidModuleName(int len, int pos);
bool __fastcall IsValidName(int len, int pos);
bool __fastcall IsValidString(int len, int pos);
bool __fastcall IsXorMayBeSkipped(DWord fromAdr);
void __fastcall MakeGvar(PInfoRec recN, DWord adr, DWord xrefAdr);
String __fastcall MakeGvarName(DWord adr);
void __fastcall OutputDecompilerHeader(FILE * f);
bool __fastcall IsFlagSet(DWord flag, int pos);
void __fastcall SetFlag(DWord flag, int pos);
void __fastcall SetFlags(DWord flag, int pos, int num);
int __fastcall SortUnitsByAdr(void *item1, void *item2);
int __fastcall SortUnitsByNam(void *item1, void *item2);
int __fastcall SortUnitsByOrd(void *item1, void *item2);
bool __fastcall LineContainsShadowName(String line);
int __fastcall GetAdrOfsFromShadowName(String name);
String __fastcall SanitizeName(String name);
String __fastcall TransformShadowName(String name, Byte typeKind, DWord typeAdr);
String __fastcall TransformString(char *str, int len);
String __fastcall TransformUString(Word codePage, const wchar_t *data, int len);
String __fastcall TrimTypeName(const String &TypeName);
String __fastcall TypeKind2Name(Byte kind);
//String __fastcall UnmangleName(String Name);
//Decompiler
int __fastcall IsAbs(DWord fromAdr);
int __fastcall IsIntOver(DWord fromAdr);
int __fastcall IsInlineLengthCmp(DWord fromAdr);
int __fastcall IsInlineLengthTest(DWord fromAdr);
int __fastcall IsInlineDiv(DWord fromAdr, int *div);
int __fastcall IsInlineMod(DWord fromAdr, int *mod);
int __fastcall ProcessInt64Equality(DWord fromAdr, DWord *maxAdr);
int __fastcall ProcessInt64NotEquality(DWord fromAdr, DWord *maxAdr);
int __fastcall ProcessInt64Comparison(DWord fromAdr, DWord *maxAdr);
int __fastcall ProcessInt64ComparisonViaStack1(DWord fromAdr, DWord *maxAdr);
int __fastcall ProcessInt64ComparisonViaStack2(DWord fromAdr, DWord *maxAdr);
int __fastcall IsInt64Equality(DWord fromAdr, int *skip1, int *skip2, bool *immVal, __int64 *Val);
int __fastcall IsInt64NotEquality(DWord fromAdr, int *skip1, int *skip2, bool *immVal, __int64 *Val);
int __fastcall IsInt64Comparison(DWord fromAdr, int *skip1, int *skip2, bool *immVal, __int64 *Val);
int __fastcall IsInt64ComparisonViaStack1(DWord fromAdr, int *skip1, DWord *simEnd);
int __fastcall IsInt64ComparisonViaStack2(DWord fromAdr, int *skip1, int *skip2, DWord *simEnd);
int __fastcall IsInt64Shr(DWord fromAdr);
int __fastcall IsInt64Shl(DWord fromAdr);
String __fastcall InputDialogExec(String caption, String labelText, String text);
String __fastcall ManualInput(DWord procAdr, DWord curAdr, String caption, String labelText);
bool __fastcall MatchCode(Byte * code, MProcInfo * pInfo);
void __fastcall SaveCanvas(TCanvas * ACanvas);
void __fastcall RestoreCanvas(TCanvas * ACanvas);
void __fastcall DrawOneItem(String AItem, TCanvas *ACanvas, TRect &ARect, TColor AColor, int flags);
int __fastcall IsTryBegin(DWord fromAdr, DWord *endAdr);
int __fastcall IsTryBegin0(DWord fromAdr, DWord *endAdr);
int __fastcall IsTryEndPush(DWord fromAdr, DWord *endAdr);
int __fastcall IsTryEndJump(DWord fromAdr, DWord *endAdr);

PFIELDINFO __fastcall GetClassField(String TypeName, int Offset);
int __fastcall GetRecordField(String ARecType, int AOfs, String &name, String &type);
int __fastcall GetField(String TypeName, int Offset, String &name, String &type);

int __fastcall ArgsCmpFunction(void *item1, void *item2);
int __fastcall ExportsCmpFunction(void *item1, void *item2);
int __fastcall FieldInfoCmpFunction(void *item1, void *item2);
int __fastcall FieldsCmpFunction(void *item1, void *item2);
int __fastcall ImportsCmpFunction(void *item1, void *item2);
int __fastcall LocalsCmpFunction(void *item1, void *item2);
int __fastcall MethodsCmpFunction(void *item1, void *item2);
//---------------------------------------------------------------------------
#endif
