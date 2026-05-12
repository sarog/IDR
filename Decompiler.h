//---------------------------------------------------------------------------
#ifndef DecompilerH
#define DecompilerH

#include "Main.h"
//---------------------------------------------------------------------------
//Precedence of operations
#define     PRECEDENCE_ATOM     24
#define     PRECEDENCE_UNARY    16
#define     PRECEDENCE_MULT     15  //*,/,div, mod,and,shl,shr,as
#define     PRECEDENCE_ADD      14  //+,-,or,xor
#define     PRECEDENCE_NOT      6   //@,not
#define     PRECEDENCE_CMP      9   //=,<>,<,>,<=,>=,in,is
#define     PRECEDENCE_NONE     0

#define     TAB_SIZE            2

#define     IF_ARG              1
#define     IF_VAR              2
#define     IF_STACK_PTR        4
#define     IF_CALL_RESULT      8
#define     IF_VMT_ADR          16
#define     IF_CYCLE_VAR        32
#define     IF_FIELD            64
#define     IF_ARRAY_PTR        128
#define     IF_INTVAL           256
#define     IF_INTERFACE        512
#define     IF_EXTERN_VAR       1024    //User for embedded procedures
#define     IF_RECORD_FOFS      2048    //Offset inside record

#define     CF_CONSTRUCTOR      1
#define     CF_DESTRUCTOR       2
#define     CF_FINALLY          4
#define     CF_EXCEPT           8
#define     CF_LOOP             16
#define     CF_BJL              32
#define     CF_ELSE             64

#define     CMP_FAILED          0
#define     CMP_BRANCH          1
#define     CMP_SET             2

//BJL
#define     MAXSEQNUM           1024

#define     BJL_USED            -1
#define     BJL_EMPTY           0
#define     BJL_BRANCH          1
#define     BJL_JUMP            2
#define     BJL_LOC	            3
#define     BJL_SKIP_BRANCH     4   //branches for IntOver, BoundErr,...

typedef struct {
    char state; //'U' not defined; 'B' branch; 'J' jump; '@' label; 'R' return; 'S' switch
    int bcnt;   //branches to... count
    DWord address;
    String dExpr; //condition of direct expression
    String iExpr; //condition of inverse expression
    String result;
} TBJLInfo;

typedef struct {
    bool branch;
    bool loc;
    int type;
    int address;
    int idx; //IDX in BJLseq
} TBJL;

//BJL

typedef struct {
    String L;
    char O;
    String R;
} CMPITEM, *PCMPITEM;

typedef struct {
    Byte Precedence;
    int Size;       //Size in bytes
    int Offset;     //Offset from beginning of type
    DWord IntValue; //For array element size calculation
    DWord Flags;
    String Value;
    String Value1; //For various purposes
    String Type;
    String Name;
} ITEM, *PITEM;

typedef struct {
    String Value;
    String Name;
} WHAT, *PWHAT;

#define     itUNK   0
#define     itREG   1
#define     itLVAR  2
#define     itGVAR  3

typedef struct {
    Byte IdxType;
    int IdxValue;
    String IdxStr;
} IDXINFO, *PIDXINFO;

class TForInfo {
public:
    bool NoVar;
    bool Down;   //downto (=true)
    int StopAdr; //instructions are ignored from this address and to end of cycle
    String From;
    String To;
    IDXINFO VarInfo;
    IDXINFO CntInfo;

public:
    TForInfo(bool ANoVar, bool ADown, int AStopAdr, String AFrom, String ATo, Byte AVarType, int AVarIdx,
                        Byte ACntType, int ACntIdx);
};

typedef TForInfo *PForInfo;

class TWhileInfo {
public:
    bool NoCondition; //No condition
public:
    TWhileInfo(bool ANoCond);
};

typedef TWhileInfo *PWhileInfo;

class TLoopInfo {
public:
    Byte Kind;      //'F'- for; 'W' - while; 'T' - while true; 'R' - repeat
    DWord ContAdr;  //Continue address
    DWord BreakAdr; //Break address
    DWord LastAdr;  //Last address for decompilation (skip some last instructions)
    PForInfo forInfo;
    PWhileInfo whileInfo;

public:
    TLoopInfo(Byte AKind, DWord AContAdr, DWord ABreakAdr, DWord ALastAdr);
    ~TLoopInfo();
};

typedef TLoopInfo *PLoopInfo;

//cmpStack Format: "XYYYYYYY^ZZZZ" (== YYYYYY X ZZZZ)
//'A'-JO;'B'-JNO;'C'-JB;'D'-'JNB';'E'-JZ;'F'-JNZ;'G'-JBE;'H'-JA;
//'I'-'JS';'J'-JNS;'K'-JP;'L'-JNP;'M'-JL;'N'-JGE;'O'-JLE;'P'-JG

//Only registers eax, ecx, edx, ebx, esp, ebp, esi, edi 
typedef ITEM REGS[8];

class TNamer {
public:
    int MaxIdx;
    TStringList *Names;
    TNamer();
    ~TNamer();
    String __fastcall MakeName(String shablon);
};

struct TCaseTreeNode;

struct TCaseTreeNode {
    TCaseTreeNode *LNode;
    TCaseTreeNode *RNode;
    DWord ZProc;
    int FromVal;
    int ToVal;
};

//structure for saving context of all registers
typedef struct {
    DWord adr;
    REGS gregs;  //general registers
    REGS fregs;  //float point registers
    REGS fregsd; //float point registers (copy)
} DCONTEXT, *PDCONTEXT;

class TDecompiler;

class TDecompileEnv {
public:
    String ProcName; //Name of decompiled procedure
    DWord StartAdr;  //Start of decompilation area
    int Size;        //Size of decompilation area
    int Indent;      //For output source code
    bool Alarm;
    bool BpBased;
    int LocBase;
    DWord StackSize;
    PITEM Stack;
    DWord ErrAdr;
    String LastResString;
    TStringList *Body;
    ITEM RegInfo[8];
    ITEM FStack[8]; //Floating registers stack
    TNamer *Namer;
    int BJLnum;
    int BJLmaxbcnt;
    TList *SavedContext;
    TList *BJLseq;  //TBJLInfo
    TList *bjllist; //TBJL
    TList *CmpStack;
    bool Embedded;             //Is proc embedded
    TStringList *EmbeddedList; //List of embedded procedures addresses

    TDecompileEnv(DWord AStartAdr, int ASize, PInfoRec recN);
    ~TDecompileEnv();
    String __fastcall GetFieldName(PFIELDINFO fInfo);
    String __fastcall GetArgName(PARGINFO argInfo);
    String __fastcall GetGvarName(DWord adr);
    String __fastcall GetLvarName(int Ofs, String Type);
    void __fastcall AssignItem(PITEM DstItem, PITEM SrcItem);
    void __fastcall AddToBody(String src);
    void __fastcall AddToBody(TStringList *src);
    bool __fastcall IsExitAtBodyEnd();

    void __fastcall OutputSourceCodeLine(String line);
    void __fastcall OutputSourceCode();
    void __fastcall MakePrototype();
    void __fastcall DecompileProc();
    //BJL
    bool __fastcall GetBJLRange(DWord fromAdr, DWord *bodyBegAdr, DWord *bodyEndAdr, DWord *jmpAdr, PLoopInfo loopInfo);
    void __fastcall CreateBJLSequence(DWord fromAdr, DWord bodyBegAdr, DWord bodyEndAdr);
    void __fastcall UpdateBJLList();
    void __fastcall BJLAnalyze();
    bool __fastcall BJLGetIdx(int *idx, int from, int num);
    bool __fastcall BJLCheckPattern1(const char *t, int from);
    bool __fastcall BJLCheckPattern2(const char *t, int from);
    int __fastcall BJLFindLabel(int address, int *no);
    void __fastcall BJLSeqSetStateU(int *idx, int num);
    void __fastcall BJLListSetUsed(int from, int num);
    char __fastcall ExprGetOperation(String s);
    void __fastcall ExprMerge(String &dst, String src, char op); //dst = dst op src, op = '|' or '&'
    String __fastcall PrintBJL();
    PDCONTEXT __fastcall GetContext(DWord Adr);
    void __fastcall SaveContext(DWord Adr);
    void __fastcall RestoreContext(DWord Adr);
};

class TDecompiler {
public:
    bool WasRet;  //Was ret instruction
    char CmpOp;   //Compare operation
    DWord CmpAdr; //Compare dest address
    int _ESP_;    //Stack pointer
    int _TOP_;    //Top of FStack
    DISINFO DisInfo;
    CMPITEM CmpInfo;
    TDecompileEnv *Env;
    Byte *DeFlags;
    PITEM Stack;

    TDecompiler(TDecompileEnv *AEnv);
    ~TDecompiler();
    bool __fastcall CheckPrototype(PInfoRec ARec);
    void __fastcall ClearStop(DWord Adr);
    DWord __fastcall Decompile(DWord fromAdr, DWord flags, PLoopInfo loopInfo);
    DWord __fastcall DecompileCaseEnum(DWord fromAdr, int N, PLoopInfo loopInfo);
    DWord __fastcall DecompileGeneralCase(DWord fromAdr, DWord markAdr, PLoopInfo loopInfo, int N);
    DWord __fastcall DecompileTry(DWord fromAdr, DWord flags, PLoopInfo loopInfo);
    PITEM __fastcall FGet(int idx);
    PITEM __fastcall FPop();
    void __fastcall FPush(PITEM val);
    void __fastcall FSet(int idx, PITEM val);
    void __fastcall FXch(int idx1, int idx2);
    int __fastcall GetArrayFieldOffset(String ATypeName, int AFromOfs, int AScale, String &_name, String &_type);
    int __fastcall GetCmpInfo(DWord fromAdr);
    String __fastcall GetCycleFrom();
    void __fastcall GetCycleIdx(PIDXINFO IdxInfo, DISINFO *ADisInfo);
    String __fastcall GetCycleTo();
    void __fastcall GetFloatItemFromStack(int Esp, PITEM Dst, int FloatType);
    void __fastcall GetInt64ItemFromStack(int Esp, PITEM Dst);
    String __fastcall GetStringArgument(PITEM item);
    PLoopInfo __fastcall GetLoopInfo(int fromAdr);
    void __fastcall GetMemItem(int CurAdr, PITEM Dst, Byte Op);
    void __fastcall GetRegItem(int Idx, PITEM Dst);
    String __fastcall GetRegType(int Idx);
    String __fastcall GetSysCallAlias(String AName);
    bool __fastcall Init(DWord fromAdr);
    void __fastcall InitFlags();
    void __fastcall MarkCaseEnum(DWord fromAdr);
    void __fastcall MarkGeneralCase(DWord fromAdr);
    PITEM __fastcall Pop();
    void __fastcall Push(PITEM item);
    void __fastcall SetStackPointers(TDecompiler *ASrc);
    void __fastcall SetDeFlags(Byte *ASrc);
    void __fastcall SetRegItem(int Idx, PITEM Val);
    void __fastcall SetStop(DWord Adr);
    bool __fastcall SimulateCall(DWord curAdr, DWord callAdr, int instrLen, PMethodRec recM, DWord AClassAdr);
    void __fastcall SimulateFloatInstruction(DWord curAdr);
    void __fastcall SimulateFormatCall();
    void __fastcall SimulateInherited(DWord procAdr);
    void __fastcall SimulateInstr1(DWord curAdr, Byte Op);
    void __fastcall SimulateInstr2(DWord curAdr, Byte Op);
    void __fastcall SimulateInstr2RegImm(DWord curAdr, Byte Op);
    void __fastcall SimulateInstr2RegMem(DWord curAdr, Byte Op);
    void __fastcall SimulateInstr2RegReg(DWord curAdr, Byte Op);
    void __fastcall SimulateInstr2MemImm(DWord curAdr, Byte Op);
    void __fastcall SimulateInstr2MemReg(DWord curAdr, Byte Op);
    void __fastcall SimulateInstr3(DWord curAdr, Byte Op);
    void __fastcall SimulatePop(DWord curAdr);
    void __fastcall SimulatePush(DWord curAdr, bool bShowComment);
    bool __fastcall SimulateSysCall(String name, DWord procAdr, int instrLen);
    int __fastcall AnalyzeConditions(int brType, DWord curAdr, DWord sAdr, DWord jAdr, PLoopInfo loopInfo, BOOL bFloat);
};

//---------------------------------------------------------------------------
#endif
