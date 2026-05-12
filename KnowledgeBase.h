//---------------------------------------------------------------------------
#ifndef KnowledgeBaseH
#define KnowledgeBaseH
//---------------------------------------------------------------------------
#include <stdio.h>
//---------------------------------------------------------------------------
// Информация о смещениях имен и данных | Information about name and data offsets
typedef struct {
    DWord Offset;
    DWord Size;
    int ModId; // Modules
    int NamId; // Names
} OFFSETSINFO, *POFFSETSINFO;

//Fixup info
typedef struct {
    Byte Type; // 'A' - ADR, 'J' - JMP, 'D' - DAT
    DWord Ofs; // Смещение относительно начала дампа | Offset relative to the start of the dump
    char *Name;
} FIXUPINFO, *PFIXUPINFO;

/*
ModuleDataTable
---------------
//Состоит из ModuleCount записей вида
Word 	ID;
PSTR 	ModuleName;
PSTR	Filename;
Word 	UsesNum;
Word 	UsesID[UsesNum];	//Массив идентификаторов модулей
PSTR	UsesNames[UsesNum];	//Массив имен модулей

ConstDataTable
--------------
//Состоит из ModuleCount записей вида
Word 	ModuleID;
PSTR 	ConstName;
Byte 	Type; //'C'-ConstDecl, 'P'-PDecl (VMT), 'V'-VarCDecl
PSTR 	TypeDef; //Тип
PSTR 	Value; //Значение
DWord	DumpTotal;	//Общий размер дампа (дамп+релоки+фиксапы)
DWord 	DumpSize; //Размер бинарного дампа (RTTI)
DWord 	FixupNum; //Количество фиксапов дампа
Byte 	Dump[DumpSize]; //Бинарный дамп (RTTI)
Byte 	Relocs[DumpSize];
FIXUPINFO Fixups[FixupNum]; //Массив фиксапов

TypeDataTable
-------------
//Состоит из TypeCount записей вида
DWord   Size; //Size of Type
Word 	ModuleID;
PSTR 	TypeName;
Byte 	Kind; //drArrayDef,...,drVariantDef (см. начало)
DWord 	VMCnt; //Количество элементов VMT (начиная с 0)
PSTR 	Decl; //Декларация
DWord	DumpTotal;	//Общий размер дампа (дамп+релоки+фиксапы)
DWord 	DumpSize; //Размер бинарного дампа (RTTI)
DWord 	FixupNum; //Количество фиксапов дампа
Byte 	Dump[DumpSize]; //Бинарный дамп (RTTI)
Byte 	Relocs[DumpSize];
FIXUPINFO Fixups[FixupNum]; //Фиксапы
DWord	FieldsTotal;	//Общий размер данных полей
Word 	FieldsNum; //Количество полей (class, interface, record)
FIELDINFO Fields[FieldNum]; //Поля
DWord	PropsTotal;	//Общий размер данных свойств
Word 	PropsNum; //Количество свойств (class, interface)
PROPERTYINFO Props[PropNum]; //Свойства
DWord	MethodsTotal;	//Общий размер данных методов
Word 	MethodsNum; //Количество методов (class, interface)
METHODINFO Methods[MethodNum]; //Методы

VarDataTable
------------
//Состоит из VarCount записей вида
Word 	ModuleID;
PSTR 	VarName;
Byte 	Type; //'V'-Var;'A'-AbsVar;'S'-SpecVar;'T'-ThreadVar
PSTR 	TypeDef;
PSTR 	AbsName; //Для ключевого слова absolute

ResStrDataTable
---------------
//Состоит из ResStrCount записей вида
Word 	ModuleID;
PSTR 	ResStrName;
PSTR 	TypeDef;
PSTR	Context;

ProcDataTable
-------------
//Contains ProcCount structures:
Word 	ModuleID;
PSTR 	ProcName;
Byte 	Embedded; //Contains embedded procs
Byte 	DumpType; //'C' - code, 'D' - data
Byte 	MethodKind; //'M'-method,'P'-procedure,'F'-function,'C'-constructor,'D'-destructor
Byte 	CallKind; //1-'cdecl', 2-'pascal', 3-'stdcall', 4-'safecall'
int 	VProc; //Flag for "overload" (if Delphi version > verD3 and VProc&0x1000 != 0)
PSTR 	TypeDef; //Type of Result for function
DWord	DumpTotal;	//Total size of dump (dump+relocs+fixups)
DWord 	DumpSz; //Dump size
DWord 	FixupNum; //Dump fixups number
Byte 	Dump[DumpSz]; //Binary dump
Byte 	Relocs[DumpSize];
FIXUPINFO Fixups[FixupNum]; //Fixups
DWord	ArgsTotal;	//Total size of arguments
Word 	ArgsNum; //Arguments number
ARGINFO Args[ArgNum]; //Arguments
DWord	LocalsTotal;	//Total size of local vars
Word 	LocalsNum; //Local vars number
LOCALINFO Locals[LocalNum]; //Local vars
*/

#define SCOPE_TMP   32  // Temp struct FIELDINFO, to be deleted

typedef struct FIELDINFO {
    FIELDINFO() : xrefs(0) {}
    ~FIELDINFO();
    Byte Scope;   // 9-private, 10-protected, 11-public, 12-published
    int Offset;   // Offset in class instance
    int Case;     // Case for record (in other cases 0xFFFFFFFF)
    String Name;  // Field Name
    String Type;  // Field Type
    TList *xrefs; // Xrefs from code
} FIELDINFO, *PFIELDINFO;

typedef struct {
    Byte Scope;        // 9-private, 10-protected, 11-public, 12-published
    int Index;         // readonly, writeonly в зависимости от установки бит 1 и 2 | depending on the setting of bits 1 and 2
    int DispID;        // ???
    String Name;       // Имя свойства | Property name
    String TypeDef;    // Тип свойства | Property type
    String ReadName;   // Процедура для чтения свойства или соответствующее поле
    String WriteName;  // Процедура для записи свойства или соответствующее поле
    String StoredName; // Процедура для проверки свойства или соответствующее значение
} PROPINFO, *PPROPINFO;

typedef struct {
    Byte Scope;       // 9-private, 10-protected, 11-public, 12-published
    Byte MethodKind;  // 'M'-method, 'P'-procedure, 'F'-function, 'C'-constructor, 'D'-destructor
    String Prototype; // Prototype full name
} METHODINFO, *PMETHODINFO;

typedef struct {
    Byte Tag;       // 0x21-"val", 0x22-"var"
    bool Register;  // If true - argument is in register, else - in stack
    int Ndx;        // Register number and offset (XX-number, XXXXXX-offset) (0-EAX, 1-ECX, 2-EDX)
    int Size;       // Argument Size
    String Name;    // Argument Name
    String TypeDef; // Argument Type
} ARGINFO, *PARGINFO;

typedef struct {
    int Ofs;        // Offset of local var (from ebp or EP)
    int Size;       // Size of local var
    String Name;    // Local var Name
    String TypeDef; // Local var Type
} LOCALINFO, *PLOCALINFO;

typedef struct {
    char type;  // 'C'-call; 'J'-jmp; 'D'-data
    DWord adr;  // address of procedure
    int offset; // offset in procedure
} XrefRec, *PXrefRec;

//Флажки для заполнения членов классов
#define INFO_DUMP       1
#define INFO_ARGS       2
#define INFO_LOCALS     4
#define INFO_FIELDS     8
#define INFO_PROPS      16
#define INFO_METHODS    32
#define INFO_ABSNAME    64

class MConstInfo {
public:
    MConstInfo();
    ~MConstInfo();

public:
    Word ModuleID;
    String ConstName;
    Byte Type;      // 'C'-ConstDecl, 'P'-PDecl (VMT), 'V'-VarCDecl
    String TypeDef; // Тип | Type
    String Value;   // Значение
    DWord DumpSz;   // Размер бинарного дампа | Binary dump size
    DWord FixupNum; // Количество фиксапов дампа | Number of dump fixups
    Byte *Dump;     // Бинарный дамп | Binary dump
};

// Значения байта Kind информации о типе
// Byte type kind information values
#define drArrayDef          0x4C    //'L'
#define drClassDef          0x46    //'F'
#define drFileDef           0x4F    //'O'
#define drFloatDef          0x49    //'I'
#define drInterfaceDef      0x54    //'T'
#define drObjVMTDef         0x47    //'G'
#define drProcTypeDef       0x48    //'H'
#define drPtrDef            0x45    //'E'
#define drRangeDef          0x44    //'D'
#define drRecDef            0x4D    //'M'
#define drSetDef            0x4A    //'J'
#define drShortStrDef       0x4B    //'K'
#define drStringDef         0x52    //'R'
#define drTextDef           0x50    //'P'
#define drVariantDef        0x53    //'S'
#define drAliasDef          0x41    //'Z'

class MTypeInfo {
public:
    MTypeInfo();
    ~MTypeInfo();

public:
    DWord Size;
    Word ModuleID;
    String TypeName;
    Byte Kind;      //drArrayDef,...,drVariantDef
    Word VMCnt;     //VMT elements number (from 0)
    String Decl;    //Declaration
    DWord DumpSz;   //Binary dump size
    DWord FixupNum; //Binary dump fixup number
    Byte *Dump;     //Binary dump
    Word FieldsNum; //Fields number (class, interface, record)
    Byte *Fields;
    Word PropsNum; //Properties number (class, interface)
    Byte *Props;
    Word MethodsNum; //Methods number (class, interface)
    Byte *Methods;
};

//Var Type field
#define VT_VAR          'V'
#define VT_ABSVAR       'A'
#define VT_SPECVAR      'S'
#define VT_THREADVAR    'T'

class MVarInfo {
public:
    MVarInfo();
    ~MVarInfo();

public:
    Word ModuleID;
    String VarName;
    Byte Type; //'V'-Var;'A'-AbsVar;'S'-SpecVar;'T'-ThreadVar
    String TypeDef;
    String AbsName; //Для ключевого слова absolute
};

class MResStrInfo {
public:
    MResStrInfo();
    ~MResStrInfo();

public:
    Word ModuleID;
    String ResStrName;
    String TypeDef;
    //String      Context;
};

class MProcInfo {
public:
    MProcInfo();
    ~MProcInfo();

public:
    Word ModuleID;
    String ProcName;
    bool Embedded;   //true-содержит вложенные процедуры
    char DumpType;   //'C' - код, 'D' - данные
    char MethodKind; //'M'-method,'P'-procedure,'F'-function,'C'-constructor,'D'-destructor
    Byte CallKind;   //1-'cdecl', 2-'pascal', 3-'stdcall', 4-'safecall'
    int VProc;       //флажок для "overload" (если версия Дельфи > verD3 и VProc&0x1000 != 0)
    String TypeDef;  //Тип
    DWord DumpSz;    //Размер бинарного дампа
    DWord FixupNum;  //Количество фиксапов дампа
    Byte *Dump;      //Бинарный дамп (включает в себя собственно дамп, релоки и фиксапы)
    Word ArgsNum;    //Количество аргументов процедуры
    Byte *Args;      //Список аргументов
    //Word 	    LocalsNum;  //Количество локальных переменных процедуры
    //Byte        *Locals;    //Список локальных переменных
};

//Секции базы знаний
#define     KB_NO_SECTION       0
#define     KB_CONST_SECTION    1
#define     KB_TYPE_SECTION     2
#define     KB_VAR_SECTION      4
#define     KB_RESSTR_SECTION   8
#define     KB_PROC_SECTION     16

class MKnowledgeBase {
public:
    MKnowledgeBase();
    ~MKnowledgeBase();

    bool __fastcall Open(char *filename);
    void __fastcall Close();

    const Byte * __fastcall GetKBCachePtr(DWord Offset, DWord Size);
    Word __fastcall GetModuleID(const char *ModuleName);
    String __fastcall GetModuleName(Word ModuleID);
    void __fastcall GetModuleIdsByProcName(const char *ProcName);
    int __fastcall GetItemSection(Word *ModuleIDs, char *ItemName);
    int __fastcall GetConstIdx(Word *ModuleID, const char *ConstName);
    int __fastcall GetConstIdxs(const char *ConstName, int *ConstIdx);
    int __fastcall GetTypeIdxByModuleIds(Word *ModuleIDs, const char *TypeName);
    int __fastcall GetTypeIdxsByName(const char *TypeName, int *TypeIdx);
    // int __fastcall GetTypeIdxByUID(char *UID); // unused
    int __fastcall GetVarIdx(Word *ModuleIDs, const char *VarName);
    int __fastcall GetResStrIdx(int from, const char *ResStrContext);
    int __fastcall GetResStrIdx(Word ModuleID, const char *ResStrContext);
    int __fastcall GetResStrIdx(Word *ModuleIDs, const char *ResStrName);
    int __fastcall GetProcIdx(Word ModuleID, const char *ProcName);
    int __fastcall GetProcIdx(Word ModuleID, const char *ProcName, Byte *code);
    int __fastcall GetProcIdx(Word *ModuleIDs, const char *ProcName, Byte *code);
    bool __fastcall GetProcIdxs(Word ModuleID, int *FirstProcIdx, int *LastProcIdx);
    bool __fastcall GetProcIdxs(Word ModuleID, int *FirstProcIdx, int *LastProcIdx, int *DumpSize);
    MConstInfo * __fastcall GetConstInfo(int AConstIdx, DWord AFlags, MConstInfo *cInfo);
    MProcInfo * __fastcall GetProcInfo(const char *ProcName, DWord AFlags, MProcInfo *pInfo, int *procIdx);
    MProcInfo * __fastcall GetProcInfo(int AProcIdx, DWord AFlags, MProcInfo *pInfo);
    MTypeInfo * __fastcall GetTypeInfo(int ATypeIdx, DWord AFlags, MTypeInfo *tInfo);
    MVarInfo * __fastcall GetVarInfo(int AVarIdx, DWord AFlags, MVarInfo *vInfo);
    MResStrInfo * __fastcall GetResStrInfo(int AResStrIdx, DWord AFlags, MResStrInfo *rsInfo);
    int __fastcall ScanCode(Byte *code, DWord *CodeFlags, DWord CodeSz, MProcInfo *pInfo);
    Word * __fastcall GetModuleUses(Word ModuleID);
    int __fastcall GetProcUses(const char *ProcName, Word *uses);
    Word * __fastcall GetTypeUses(const char *TypeName);
    Word * __fastcall GetConstUses(const char *ConstName);
    String __fastcall GetProcPrototype(int ProcIdx);
    String __fastcall GetProcPrototype(MProcInfo *pInfo);
    bool __fastcall IsUsedProc(int AIdx);
    void __fastcall SetUsedProc(int AIdx);
    bool __fastcall GetKBProcInfo(String typeName, MProcInfo *procInfo, int *procIdx);
    bool __fastcall GetKBTypeInfo(String typeName, MTypeInfo *typeInfo);
    bool __fastcall GetKBPropertyInfo(String className, String propName, MTypeInfo *typeInfo);
    String __fastcall IsPropFunction(String className, String procName);
    DWord Version;
    int ModuleCount;
    OFFSETSINFO *ModuleOffsets;
    Word *Mods;
    Byte *UsedProcs;
    const OFFSETSINFO *ConstOffsets;
    const OFFSETSINFO *TypeOffsets;
    const OFFSETSINFO *VarOffsets;
    const OFFSETSINFO *ResStrOffsets;
    const OFFSETSINFO *ProcOffsets;

private:
    bool Inited;
    FILE *Handle;
    bool __fastcall CheckKBFile();

    long SectionsOffset;
    //Modules
    int MaxModuleDataSize;
    //Consts
    int ConstCount;
    int MaxConstDataSize;
    //Types
    int TypeCount;
    int MaxTypeDataSize;
    //Vars
    int VarCount;
    int MaxVarDataSize;
    //ResStr
    int ResStrCount;
    int MaxResStrDataSize;
    //Procs
    int MaxProcDataSize;
    int ProcCount;

    //as temp test (global KB file cache in mem)
    const Byte *KBCache;
    long SizeKBFile;
    String NameKBFile;
};

//---------------------------------------------------------------------------
#endif
