#include <vcl.h>
#pragma hdrstop

#ifdef IDR64

#include "Idr64Manager.h"
#include "Misc.h"
#include "Resources.h"
//---------------------------------------------------------------------------
//TBD: refactor any extern usage (delme)
// ideally: all these vars/dbs/etc must be kept private inside idr manager!
//
extern  int         dummy;
extern  DWord       EP;
//extern  String      IDR64Version;
extern  String      SelectedAsmItem;
//extern  char        StringBuf[MAXSTRBUFFER];
extern  DWord       ImageBase;
extern  DWord       ImageSize;
extern  DWord       TotalSize;
extern  DWord       CodeBase;
extern  DWord       CodeSize;

extern  Byte        *Image;
//extern  PInfoRec    *Infos;
extern  TStringList *BSSInfos;
//extern  MDisasm     Disasm;
extern  MKnowledgeBase  KnowledgeBase;
extern  Byte        *Code;
extern  TList       *SegmentList;
extern  TList       *OwnTypeList;
extern  TList       *VmtList;


extern DWord        HInstanceVarAdr;
extern DWord        LastTls;

//Units
extern int          UnitsNum;
extern TList        *Units;

//---------------------------------------------------------------------------

TCriticalSection *GetSyncObj() {
    static TCriticalSection *CrtSection;
    if (!CrtSection)
        CrtSection = new TCriticalSection();
    return CrtSection;
}
//---------------------------------------------------------------------------

MDisasm &GetDisasm() {
    static MDisasm Disasm(GetSyncObj()); // ������������ ��� ����������� ����
    return Disasm;
}
//---------------------------------------------------------------------------

//global instance (TBD: think about Singleton pattern & API like getIdr() )
Idr64Manager idr;
// DelphiVmt Vmt;
//---------------------------------------------------------------------------

Idr64Manager::Idr64Manager()
{
    IDR64Version = "2026-05-11";
    _ResInfo = new TResourceInfo;
};
//---------------------------------------------------------------------------

Idr64Manager::~Idr64Manager()
{
    //todo Phoenix singleton patter?  (idr object is destroyed earlier comparing to forms!)
    //delete _ResInfo;;
};
//---------------------------------------------------------------------------

PInfoRec Idr64Manager::AddToBSSInfos(DWord Adr, String AName, String ATypeName)
{
    PInfoRec recN;
    String _key = Val2Str8(Adr);
    int _idx = BSSInfos->IndexOf(_key);
    if (_idx == -1)
    {
        recN = new InfoRec(-1, ikData);
        recN->SetName(AName);
        recN->type = ATypeName;
        BSSInfos->AddObject(_key, (TObject*)recN);
    }
    else
    {
        recN = (PInfoRec)BSSInfos->Objects[_idx];
        if (recN->type == "")
        {
            recN->type = ATypeName;
        }
    }
    return recN;
}
//---------------------------------------------------------------------------

void Idr64Manager::BSSInfosAddObject(String _adr, PInfoRec recN)
{
    BSSInfos->AddObject(_adr, (TObject*)recN);
}
//---------------------------------------------------------------------------

PInfoRec Idr64Manager::GetBSSInfosRec(const String s)
{
    PInfoRec recN = 0;
    int _idx = BSSInfos->IndexOf(s);
    if (_idx != -1)
    {
        recN = GetBSSInfosObject(_idx);
    }
    return recN;
}

//---------------------------------------------------------------------------
//Return position of nearest up argument rcx (ecx, ch, cl) from position fromPos
int __fastcall Idr64Manager::GetNearestArgC(int fromPos)
{
    int         curPos = fromPos;

    for (curPos = fromPos - 1;;curPos--)
    {
        if (idr.IsFlagSet(cfInstruction, curPos))
        {
            if (IsFlagSet(cfProcStart, curPos)) break;
            if (IsFlagSet(cfSetC, curPos)) return curPos;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
//Return position of nearest up instruction with segment prefix fs:
int __fastcall Idr64Manager::GetNearestUpPrefixFs(int fromPos)
{
    int         _pos;
    DISINFO     _disInfo;

    assert(fromPos >= 0);
    for (_pos = fromPos - 1; _pos >= 0; _pos--)
    {
        if (IsFlagSet(cfInstruction, _pos))
        {
            GetDisasm().Disassemble(Code + _pos, Pos2Adr(_pos), &_disInfo, 0);
            if (_disInfo.SegPrefix == 4) return _pos;
        }
        if (IsFlagSet(cfProcStart, _pos)) break;
    }
    return -1;
}
//---------------------------------------------------------------------------
//Return position of nearest up instruction from position fromPos
int __fastcall Idr64Manager::GetNearestUpInstruction(int fromPos)
{
    assert(fromPos >= 0);
    for (int pos = fromPos - 1; pos >= 0; pos--)
    {
        if (IsFlagSet(cfInstruction, pos)) return pos;
        if (IsFlagSet(cfProcStart, pos)) break;
    }
    return -1;
}
//---------------------------------------------------------------------------
//Return position of N-th up instruction from position fromPos
int __fastcall Idr64Manager::GetNthUpInstruction(int fromPos, int N)
{
if (fromPos < 0)
return -1;
    assert(fromPos >= 0);
    for (int pos = fromPos - 1; pos >= 0; pos--)
    {
        if (IsFlagSet(cfInstruction, pos))
        {
            N--;
            if (!N) return pos;
        }
        if (IsFlagSet(cfProcStart, pos)) break;
    }
    return -1;
}
//---------------------------------------------------------------------------
//Return position of nearest up instruction from position fromPos
int __fastcall Idr64Manager::GetNearestUpInstruction(int fromPos, int toPos)
{
    assert(fromPos >= 0);
    for (int pos = fromPos - 1; pos >= toPos; pos--)
    {
        if (IsFlagSet(cfInstruction, pos)) return pos;
        if (IsFlagSet(cfProcStart, pos)) break;
    }
    return -1;
}
//---------------------------------------------------------------------------
int __fastcall Idr64Manager::GetNearestUpInstruction(int fromPos, int toPos, int no)
{
    assert(fromPos >= 0);
    for (int pos = fromPos - 1; pos >= toPos; pos--)
    {
        if (IsFlagSet(cfInstruction, pos))
        {
            no--;
            if (!no) return pos;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
//Return position of nearest up instruction from position fromPos
int __fastcall Idr64Manager::GetNearestUpInstruction1(int fromPos, int toPos, String Instruction)
{
    int         len = Instruction.Length();
    int         pos;
    DISINFO     DisInfo;

    assert(fromPos >= 0);
    for (pos = fromPos - 1; pos >= toPos; pos--)
    {
        if (IsFlagSet(cfInstruction, pos))
        {
            GetDisasm().Disassemble(Code + pos, Pos2Adr(pos), &DisInfo, 0);
            if (len && SameText(GetDisasm().GetMnemonic(DisInfo.MnemIdx), Instruction)) return pos;
        }
        if (IsFlagSet(cfProcStart, pos)) break;
    }
    return -1;
}
//---------------------------------------------------------------------------
//Return position of nearest up instruction from position fromPos
int __fastcall Idr64Manager::GetNearestUpInstruction2(int fromPos, int toPos, String Instruction1, String Instruction2)
{
    int         len1 = Instruction1.Length(), len2 = Instruction2.Length();
    int         pos;
    DISINFO     DisInfo;

    assert(fromPos >= 0);
    for (pos = fromPos - 1; pos >= toPos; pos--)
    {
        if (IsFlagSet(cfInstruction, pos))
        {
            GetDisasm().Disassemble(Code + pos, Pos2Adr(pos), &DisInfo, 0);
            if ((len1 && SameText(GetDisasm().GetMnemonic(DisInfo.MnemIdx), Instruction1)) ||
                (len2 && SameText(GetDisasm().GetMnemonic(DisInfo.MnemIdx), Instruction2))) return pos;
        }
        if (IsFlagSet(cfProcStart, pos)) break;
    }
    return -1;
}
//---------------------------------------------------------------------------
//Return position of nearest down instruction from position fromPos
int __fastcall Idr64Manager::GetNearestDownInstruction(int fromPos)
{
    int         instrLen;
    DISINFO     DisInfo;

    assert(fromPos >= 0);
    instrLen = GetDisasm().Disassemble(Code + fromPos, Pos2Adr(fromPos), &DisInfo, 0);
    if (!instrLen) return -1;
    return fromPos + instrLen;
}
//---------------------------------------------------------------------------
//Return position of nearest down "Instruction" from position fromPos
int __fastcall Idr64Manager::GetNearestDownInstruction(int fromPos, String Instruction)
{
    int         instrLen, len = Instruction.Length();
    int         curPos = fromPos;
    DISINFO     DisInfo;

    assert(fromPos >= 0);
    while (1)
    {
        instrLen = GetDisasm().Disassemble(Code + curPos, Pos2Adr(curPos), &DisInfo, 0);
        if (!instrLen)
        {
            curPos++;
            continue;
        }
        if (len && SameText(GetDisasm().GetMnemonic(DisInfo.MnemIdx), Instruction)) return curPos + instrLen;
        if (DisInfo.Ret) break;
        curPos += instrLen;
    }
    return -1;
}
//---------------------------------------------------------------------------
//-1 - error
//0 - simple if
//1 - jcc down
//2 - jcc up
//3 - jmp down
//4 - jump up
int __fastcall Idr64Manager::BranchGetPrevInstructionType(DWord fromAdr, DWord* jmpAdr, PLoopInfo loopInfo)
{
    int         _pos;
    DISINFO     _disInfo;

    *jmpAdr = 0;
    _pos = GetNearestUpInstruction(Adr2Pos(fromAdr));
    if (_pos == -1) return -1;
    GetDisasm().Disassemble(Code + _pos, Pos2Adr(_pos), &_disInfo, 0);
    if (_disInfo.Branch)
    {
        if (IsExit(_disInfo.Immediate)) return 0;
        if (_disInfo.Conditional)
        {
            if (_disInfo.Immediate > CodeBase + _pos)
            {
                if (loopInfo && loopInfo->BreakAdr == _disInfo.Immediate) return 0;
                return 1;
            }
            return 2;
        }
        if (_disInfo.Immediate > CodeBase + _pos)
        {
            *jmpAdr = _disInfo.Immediate;
            return 3;
        }
        return 4;
    }
    return 0;
}
//---------------------------------------------------------------------------
//toAdr:dec reg
int __fastcall Idr64Manager::IsInitStackViaLoop(DWord fromAdr, DWord toAdr)
{
    int         _mnemIdx, stackSize = 0;
    DWord       curAdr;
    int         instrLen;
    DISINFO     _disInfo;

    curAdr = fromAdr;
    while (curAdr <= toAdr)
    {
        instrLen = GetDisasm().Disassemble(Code + Adr2Pos(curAdr), curAdr, &_disInfo, 0);
        //if (!instrLen) return 0;
        if (!instrLen)
        {
            curAdr++;
            continue;
        }
        _mnemIdx = _disInfo.MnemIdx;
        //push ...
        if (_mnemIdx == IDX_PUSH)
        {
            stackSize += 8;
            curAdr += instrLen;
            continue;
        }
        //add rsp, ...
        if (_mnemIdx == IDX_ADD && _disInfo.OpType[0] == otREG && _disInfo.OpRegIdx[0] == REG_RSP && _disInfo.OpType[1] == otIMM)
        {
            if ((int)_disInfo.Immediate < 0) stackSize -= (int)_disInfo.Immediate;
            curAdr += instrLen;
            continue;
        }
        //sub rsp, ...
        if (_mnemIdx == IDX_SUB && _disInfo.OpType[0] == otREG && _disInfo.OpRegIdx[0] == REG_RSP && _disInfo.OpType[1] == otIMM)
        {
            if ((int)_disInfo.Immediate > 0) stackSize += (int)_disInfo.Immediate;
            curAdr += instrLen;
            continue;
        }
        //dec
        if (_mnemIdx == IDX_DEC)
        {
            curAdr += instrLen;
            if (curAdr == toAdr) return stackSize;
        }
        break;
    }
    return 0;
}

//---------------------------------------------------------------------------
//Check that fromAdr is BoundErr sequence
int __fastcall Idr64Manager::IsBoundErr(DWord fromAdr)
{
    int         _pos, _instrLen;
    DWord       _adr;
    PInfoRec    _recN;
    DISINFO     _disInfo;

    _pos = Adr2Pos(fromAdr); _adr = fromAdr;
    while (idr.IsFlagSet(cfSkip, _pos))
    {
        _instrLen = GetDisasm().Disassemble(Code + _pos, (__int64)_adr, &_disInfo, 0);
        _adr += _instrLen;
        if (_disInfo.Call && IsValidImageAdr(_disInfo.Immediate))
        {
            _recN = GetInfoRec(_disInfo.Immediate);
            if (_recN->SameName("@BoundErr")) return _adr - fromAdr;
        }
        _pos += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check that fromAdr points to Exit
bool __fastcall Idr64Manager::IsExit(DWord fromAdr)
{
    Byte        _op;
    int         _pos, _instrLen;
    DWord       _adr;
    DISINFO     _disInfo;

    if (!IsValidCodeAdr(fromAdr)) return 0;
    _pos = Adr2Pos(fromAdr); _adr = fromAdr;

    while (1)
    {
        _pos += 8; _adr += 8;
        _instrLen = GetDisasm().Disassemble(Code + _pos, (__int64)_adr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (_op == OP_PUSH || _op == OP_JMP)
        {
            _adr = _disInfo.Immediate;
            _pos = Adr2Pos(_adr);
        }
        else
        {
            return false;
        }
    }
    while (1)
    {
        _instrLen = GetDisasm().Disassemble(Code + _pos, (__int64)_adr, &_disInfo, 0);
        if (_disInfo.Ret) return true;
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (_op == OP_POP)
        {
            _pos += _instrLen;
            _adr += _instrLen;
            continue;
        }
        if (_op == OP_MOV && _disInfo.OpType[0] == otREG &&
           (IsSameRegister(_disInfo.OpRegIdx[0], 16) || IsSameRegister(_disInfo.OpRegIdx[0], 20)))
        {
            _pos += _instrLen;
            _adr += _instrLen;
            continue;
        }
        break;
    }
    return false;
}
//---------------------------------------------------------------------------
DWord __fastcall Idr64Manager::IsGeneralCase(DWord fromAdr, int retAdr)
{
    int         _regIdx = -1, _pos, _mnemIdx;
    DWord       _curAdr = fromAdr, _jmpAdr = 0;
    int         _curPos = Adr2Pos(fromAdr);
    int         _len, _num1 = 0;
    DISINFO     _disInfo;

    if (!IsValidCodeAdr(fromAdr)) return 0;

    while (1)
    {
        _len = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
        _mnemIdx = _disInfo.MnemIdx;
        //Switch at current address
        if (IsFlagSet(cfSwitch, _curPos))
        {
            GetDisasm().Disassemble(Code + _curPos + _len, (__int64)(_curAdr + _len), &_disInfo, 0);
            _mnemIdx = _disInfo.MnemIdx;
            if (_mnemIdx == IDX_JA)
            {
                if (IsValidCodeAdr(_disInfo.Immediate))
                    return _disInfo.Immediate;
                else
                    return 0;
            }
        }
        //Switch at next address
        if (IsFlagSet(cfSwitch, _curPos + _len))
        {
            _len += GetDisasm().Disassemble(Code + _curPos + _len, (__int64)(_curAdr + _len), &_disInfo, 0);
            GetDisasm().Disassemble(Code + _curPos + _len, (__int64)(_curAdr + _len), &_disInfo, 0);
            _mnemIdx = _disInfo.MnemIdx;
            if (_mnemIdx == IDX_JA)
            {
                if (IsValidCodeAdr(_disInfo.Immediate))
                    return _disInfo.Immediate;
                else
                    return 0;
            }
        }
        //cmp reg, imm
        if (_mnemIdx == IDX_CMP && _disInfo.OpType[0] == otREG && _disInfo.OpType[1] == otIMM)
        {
            _regIdx = _disInfo.OpRegIdx[0];
            _len += GetDisasm().Disassemble(Code + _curPos + _len, (__int64)(_curAdr + _len), &_disInfo, 0);
            _mnemIdx = _disInfo.MnemIdx;
            if (_mnemIdx == IDX_JB || _mnemIdx == IDX_JG || _mnemIdx == IDX_JGE)
            {
                if (IsGeneralCase(_disInfo.Immediate, retAdr))
                {
                    _curAdr += _len;
                    _curPos += _len;

                    _len = GetDisasm().Disassemble(Code + _curPos, (__int64)(_curAdr), &_disInfo, 0);
                    _mnemIdx = _disInfo.MnemIdx;
                    if (_mnemIdx == IDX_JE)
                    {
                        _curAdr += _len;
                        _curPos += _len;
                        //continue;
                    }
                    continue;
                }
                break;
            }
        }
        //sub reg, imm; dec reg
        if ((_mnemIdx == IDX_SUB && _disInfo.OpType[0] == otREG && _disInfo.OpType[1] == otIMM) ||
            (_mnemIdx == IDX_DEC && _disInfo.OpType[0] == otREG))
        {
            _num1++;
            if (_regIdx == -1)
                _regIdx = _disInfo.OpRegIdx[0];
            else if (!IsSameRegister(_regIdx, _disInfo.OpRegIdx[0]))
                break;

            _len += GetDisasm().Disassemble(Code + _curPos + _len, (__int64)(_curAdr + _len), &_disInfo, 0);
            _mnemIdx = _disInfo.MnemIdx;
            if (_mnemIdx == IDX_SUB && IsSameRegister(_regIdx, _disInfo.OpRegIdx[0]))
            {
                _len += GetDisasm().Disassemble(Code + _curPos + _len, (__int64)(_curAdr + _len), &_disInfo, 0);
                _mnemIdx = _disInfo.MnemIdx;
            }
            if (_mnemIdx == IDX_JB)
            {
                _curAdr += _len;
                _curPos += _len;
                _len = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
                _mnemIdx = _disInfo.MnemIdx;
                if (_mnemIdx == IDX_JE)
                {
                    _curAdr += _len;
                    _curPos += _len;
                }
                continue;
            }
            if (_mnemIdx == IDX_JE)
            {
                _pos = idr.GetNearestUpInstruction(Adr2Pos(_disInfo.Immediate));
                GetDisasm().Disassemble(Code + _pos, (__int64)Pos2Adr(_pos), &_disInfo, 0);
                _mnemIdx = _disInfo.MnemIdx;
                if (_mnemIdx == IDX_JMP)
                    _jmpAdr = _disInfo.Immediate;
                if (_disInfo.Ret)
                    _jmpAdr = Pos2Adr(GetLastLocPos(retAdr));
                _curAdr += _len;
                _curPos += _len;
                continue;
            }
            if (_mnemIdx == IDX_JNE)
            {
                if (!_jmpAdr)
                {
                    //if only one dec or sub then it is simple if...else construction
                    if (_num1 == 1) return 0;
                    return _disInfo.Immediate;
                }
                if (_disInfo.Immediate == _jmpAdr) return _jmpAdr;
            }
            break;
        }
        //add reg, imm; inc reg
        if ((_mnemIdx == IDX_ADD && _disInfo.OpType[0] == otREG && _disInfo.OpType[1] == otIMM) ||
            (_mnemIdx == IDX_INC && _disInfo.OpType[0] == otREG))
        {
            _num1++;
            if (_regIdx == -1)
                _regIdx = _disInfo.OpRegIdx[0];
            else if (!IsSameRegister(_regIdx, _disInfo.OpRegIdx[0]))
                break;

            _len += GetDisasm().Disassemble(Code + _curPos + _len, (__int64)(_curAdr + _len), &_disInfo, 0);
            _mnemIdx = _disInfo.MnemIdx;
            if (_mnemIdx == IDX_SUB)
            {
               _len += GetDisasm().Disassemble(Code + _curPos + _len, (__int64)(_curAdr + _len), &_disInfo, 0);
               _mnemIdx = _disInfo.MnemIdx;
               if (_mnemIdx == IDX_JB)
               {
                    _curAdr += _len;
                    _curPos += _len;
                    continue;
               }
            }
            if (_mnemIdx == IDX_JE)
            {
                _curAdr += _len;
                _curPos += _len;
                continue;
            }
            break;
        }
        if (_mnemIdx == IDX_JMP)
        {
            if (IsValidCodeAdr(_disInfo.Immediate))
                return _disInfo.Immediate;
            else
                return 0;
        }
        break;
    }
    return 0;
}
//---------------------------------------------------------------------------
//check
//xor reg, reg
//mov reg,...
bool __fastcall Idr64Manager::IsXorMayBeSkipped(DWord fromAdr)
{
    DWord       _curAdr = fromAdr;
    int         _instrlen, _regIdx, _mnemIdx;
    int         _curPos = Adr2Pos(fromAdr);
    DISINFO     _disInfo;

    _instrlen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
    _mnemIdx = _disInfo.MnemIdx;
    if (_mnemIdx == IDX_XOR && _disInfo.OpType[0] == otREG && _disInfo.OpType[1] == otREG && _disInfo.OpRegIdx[0] == _disInfo.OpRegIdx[1])
    {
        _regIdx = _disInfo.OpRegIdx[0];
        _curPos += _instrlen; _curAdr += _instrlen;
        GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
        _mnemIdx = _disInfo.MnemIdx;
        if (_mnemIdx == IDX_MOV && _disInfo.OpType[0] == otREG && IsSameRegister(_disInfo.OpRegIdx[0], _regIdx)) return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//Check construction (after cdq)
//xor rax, rdx
//sub rax, rdx
//return bytes to skip, if Abs, else return 0
int __fastcall Idr64Manager::IsAbs(DWord fromAdr)
{
    int         _curPos = Adr2Pos(fromAdr), _instrLen, _mnemIdx;
    DWord       _dd, _curAdr = fromAdr;
    DISINFO     _disInfo;

    _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
    _mnemIdx = _disInfo.MnemIdx;
    if (_mnemIdx == IDX_XOR &&
        _disInfo.OpType[0] == otREG &&
        _disInfo.OpType[1] == otREG &&
        _disInfo.OpRegIdx[0] == REG_RAX &&
        _disInfo.OpRegIdx[1] == REG_RDX)
    {
        _curPos += _instrLen; _curAdr += _instrLen;
        _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
        _mnemIdx = _disInfo.MnemIdx;
        if (_mnemIdx == IDX_SUB &&
            _disInfo.OpType[0] == otREG &&
            _disInfo.OpType[1] == otREG &&
            _disInfo.OpRegIdx[0] == REG_RAX &&
            _disInfo.OpRegIdx[1] == REG_RDX)
        {
            return (_curAdr + _instrLen) - fromAdr;
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction
//jxx @1
//call @IntOver
//@1:
//return bytes to skip, if @IntOver, else return 0
int __fastcall Idr64Manager::IsIntOver(DWord fromAdr)
{
    int         _instrLen, _curPos = Adr2Pos(fromAdr);
    DWord       _curAdr = fromAdr;
    PInfoRec    _recN;
    DISINFO     _disInfo;

    _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
    if (_disInfo.Branch && _disInfo.Conditional)
    {
        _curPos += _instrLen; _curAdr += _instrLen;
        _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
        if (_disInfo.Call)
        {
            if (IsValidCodeAdr(_disInfo.Immediate))
            {
                _recN = GetInfoRec(_disInfo.Immediate);
                if (_recN && _recN->SameName("@IntOver")) return (_curAdr + _instrLen) - fromAdr;
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction (test reg, reg)
//test reg, reg
//jz @1
//mov reg, [reg-8]
//or-------------------------------------------------------------------------
//test reg, reg
//jz @1
//sub reg, 8
//mov reg, [reg]
int __fastcall Idr64Manager::IsInlineLengthTest(DWord fromAdr)
{
    int         _curPos = Adr2Pos(fromAdr), _instrLen, _regIdx = -1, _mnemIdx;
    DWord       _adr = 0, _curAdr = fromAdr;
    DISINFO     _disInfo;

    _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
    _mnemIdx = _disInfo.MnemIdx;
    if (_mnemIdx == IDX_TEST &&
        _disInfo.OpType[0] == otREG &&
        _disInfo.OpType[1] == otREG &&
        _disInfo.OpRegIdx[0] == _disInfo.OpRegIdx[1])
    {
        _regIdx = _disInfo.OpRegIdx[0];
        _curPos += _instrLen; _curAdr += _instrLen;
        _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
        _mnemIdx = _disInfo.MnemIdx;
        if (_mnemIdx == IDX_JE)
        {
            _adr = _disInfo.Immediate;
            _curPos += _instrLen; _curAdr += _instrLen;
            _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
            _mnemIdx = _disInfo.MnemIdx;
            //mov reg, [reg-8]
            if (_mnemIdx == 'vom' &&
                _disInfo.OpType[0] == otREG &&
                _disInfo.OpType[1] == otMEM &&
                _disInfo.BaseReg == _regIdx &&
                _disInfo.IndxReg == -1 &&
                _disInfo.Offset == -8)
            {
                if (_adr == _curAdr + _instrLen) return (_curAdr + _instrLen) - fromAdr;
            }
            //sub reg, 8
            if (_mnemIdx == IDX_SUB &&
                _disInfo.OpType[0] == otREG &&
                _disInfo.OpType[1] == otIMM &&
                _disInfo.Immediate == 8)
            {
                _curPos += _instrLen; _curAdr += _instrLen;
                _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
                _mnemIdx = _disInfo.MnemIdx;
                //mov reg, [reg]
                if (_mnemIdx == IDX_MOV &&
                    _disInfo.OpType[0] == otREG &&
                    _disInfo.OpType[1] == otMEM &&
                    _disInfo.BaseReg == _regIdx &&
                    _disInfo.IndxReg == -1 &&
                    _disInfo.Offset == 0)
                {
                    if (_adr == _curAdr + _instrLen) return (_curAdr + _instrLen) - fromAdr;
                }
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
//cmp [lvar], 0
//jz @1
//mov reg, [lvar]
//sub reg, 8
//mov reg, [reg]
//mov [lvar], reg
int __fastcall Idr64Manager::IsInlineLengthCmp(DWord fromAdr)
{
    Byte        _op;
    int         _curPos = Adr2Pos(fromAdr), _instrLen, _regIdx = -1;
    int         _baseReg, _offset, _mnemIdx;
    DWord       _adr = 0, _curAdr = fromAdr;
    DISINFO     _disInfo;

    _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
    if (_op == OP_CMP &&
        _disInfo.OpType[0] == otMEM &&
        _disInfo.OpType[1] == otIMM &&
        _disInfo.Immediate == 0)
    {
        _baseReg = _disInfo.BaseReg;
        _offset = _disInfo.Offset;
        _curPos += _instrLen; _curAdr += _instrLen;
        _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
        _mnemIdx = _disInfo.MnemIdx;
        if (_mnemIdx == IDX_JE)
        {
            _adr = _disInfo.Immediate;
            _curPos += _instrLen; _curAdr += _instrLen;
            _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
            _op = GetDisasm().GetOp(_disInfo.MnemIdx);
            //mov reg, [lvar]
            if (_op == OP_MOV &&
                _disInfo.OpType[0] == otREG &&
                _disInfo.OpType[1] == otMEM &&
                _disInfo.BaseReg == _baseReg &&
                _disInfo.Offset == _offset)
            {
                _regIdx = _disInfo.OpRegIdx[0];
                _curPos += _instrLen; _curAdr += _instrLen;
                _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
                _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                //sub reg, 8
                if (_op == OP_SUB &&
                    _disInfo.OpType[0] == otREG &&
                    _disInfo.OpType[1] == otIMM &&
                    _disInfo.Immediate == 8)
                {
                    _curPos += _instrLen; _curAdr += _instrLen;
                    _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
                    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                    //mov reg, [reg]
                    if (_op == OP_MOV &&
                        _disInfo.OpType[0] == otREG &&
                        _disInfo.OpType[1] == otMEM &&
                        _disInfo.BaseReg == _regIdx &&
                        _disInfo.IndxReg == -1 &&
                        _disInfo.Offset == 0)
                    {
                        _curPos += _instrLen; _curAdr += _instrLen;
                        _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
                        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                        //mov [lvar], reg
                        if (_op == OP_MOV &&
                            _disInfo.OpType[0] == otMEM &&
                            _disInfo.OpType[1] == otREG &&
                            _disInfo.BaseReg == _baseReg &&
                            _disInfo.Offset == _offset)
                        {
                            if (_adr == _curAdr + _instrLen) return (_curAdr + _instrLen) - fromAdr;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
//test reg, reg
//jns @1
//add reg, (2^k - 1)
//sar reg, k
//@1
int __fastcall Idr64Manager::IsInlineDiv(DWord fromAdr, int* div)
{
    Byte        _op;
    int         _curPos = Adr2Pos(fromAdr), _instrLen, _regIdx = -1, _mnemIdx;
    DWord       _adr, _curAdr = fromAdr, _imm;
    DISINFO     _disInfo;

    _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
    if (_op == OP_TEST &&
        _disInfo.OpType[0] == otREG &&
        _disInfo.OpType[1] == otREG &&
        _disInfo.OpRegIdx[0] == _disInfo.OpRegIdx[1])
    {
        _regIdx = _disInfo.OpRegIdx[0];
        _curPos += _instrLen; _curAdr += _instrLen;
        _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
        _mnemIdx = _disInfo.MnemIdx;
        if (_mnemIdx == IDX_JNS)
        {
            _adr = _disInfo.Immediate;
            _curPos += _instrLen; _curAdr += _instrLen;
            _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
            _op = GetDisasm().GetOp(_disInfo.MnemIdx);
            if (_op == OP_ADD &&
                _disInfo.OpType[0] == otREG &&
                _disInfo.OpRegIdx[0] == _regIdx &&
                _disInfo.OpType[1] == otIMM)
            {
                _imm = _disInfo.Immediate + 1;
                _curPos += _instrLen; _curAdr += _instrLen;
                _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
                _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                if (_op == OP_SAR &&
                    _disInfo.OpType[0] == otREG &&
                    _disInfo.OpRegIdx[0] == _regIdx &&
                    _disInfo.OpType[1] == otIMM)
                {
                    if (((DWord)1 << _disInfo.Immediate) == _imm)
                    {
                        *div = _imm;
                        return (_curAdr + _instrLen) - fromAdr;
                    }
                }
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
//and reg, imm
//jns @1
//dec reg
//or reg, imm
//inc reg
//@1
int __fastcall Idr64Manager::IsInlineMod(DWord fromAdr, int* mod)
{
    Byte        _op;
    int         _curPos = Adr2Pos(fromAdr), _instrLen, _regIdx = -1, _mnemIdx;
    DWord       _adr = 0, _curAdr = fromAdr, _imm;
    DISINFO     _disInfo;

    _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
    if (_op == OP_AND &&
        _disInfo.OpType[0] == otREG &&
        _disInfo.OpType[1] == otIMM &&
        (_disInfo.Immediate & 0x80000000) != 0)
    {
        _regIdx = _disInfo.OpRegIdx[0];
        _imm = _disInfo.Immediate & 0x7FFFFFFF;
        _curPos += _instrLen; _curAdr += _instrLen;
        _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
        _mnemIdx = _disInfo.MnemIdx;
        if (_mnemIdx == IDX_JNS)
        {
            _adr = _disInfo.Immediate;
            _curPos += _instrLen; _curAdr += _instrLen;
            _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
            _op = GetDisasm().GetOp(_disInfo.MnemIdx);
            if (_op == OP_DEC &&
                _disInfo.OpType[0] == otREG &&
                _disInfo.OpRegIdx[0] == _regIdx)
            {
                _curPos += _instrLen; _curAdr += _instrLen;
                _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
                _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                if (_op == OP_OR &&
                    _disInfo.OpType[0] == otREG &&
                    _disInfo.OpType[1] == otIMM &&
                    _disInfo.OpRegIdx[0] == _regIdx &&
                    _disInfo.Immediate + _imm == 0xFFFFFFFF)
                {
                    _curPos += _instrLen; _curAdr += _instrLen;
                    _instrLen = GetDisasm().Disassemble(Code + _curPos, (__int64)_curAdr, &_disInfo, 0);
                    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                    if (_op == OP_INC &&
                        _disInfo.OpType[0] == otREG &&
                        _disInfo.OpRegIdx[0] == _regIdx)
                    {
                        if (_adr == _curAdr + _instrLen)
                        {
                            *mod = _imm + 1;
                            return (_curAdr + _instrLen) - fromAdr;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction equality ((Int64)val = XXX)
//cmp XXX,XXX -> set cfSkip (_skipAdr1 = address of this instruction)
//jne @1 -> set cfSkip (_skipAdr2 = address of this instruction)
//cmp XXX,XXX
//jne @1
//...
//@1:
int __fastcall Idr64Manager::ProcessInt64Equality(DWord fromAdr, DWord* maxAdr)
{
    Byte        _op, _b;
    int         _instrLen, n, _curPos;
    DWord       _curAdr, _adr1, _maxAdr;
    DWord       _skipAdr1, _skipAdr2;
    DISINFO     _disInfo;

    _curAdr = fromAdr; _curPos = Adr2Pos(_curAdr); _maxAdr = 0;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);

        _b = *(Code + _curPos);
        if (_b == 0xF) _b = *(Code + _curPos + 1);
        _b = (_b & 0xF) + 'A';

        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            _skipAdr1 = _curAdr;
        }
        else if (n == 2)//jne @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _b == 'F')) break;
            _skipAdr2 = _curAdr;
            _adr1 = _disInfo.Immediate;//@1
            if (_adr1 > _maxAdr) _maxAdr = _adr1;
        }
        else if (n == 3)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
        }
        else if (n == 4)//jne @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _b == 'F' && _disInfo.Immediate == _adr1)) break;
            *maxAdr = _maxAdr;
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr1));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr2));
            return _curAdr + _instrLen - fromAdr;
        }
        if (_disInfo.Ret) return 0;
        _curAdr += _instrLen; _curPos += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction not equality ((Int64)val <> XXX)
//cmp XXX,XXX -> set cfSkip (_skipAdr1 = address of this instruction)
//jne @1 -> set cfSkip (_skipAdr2 = address of this instruction)
//cmp XXX,XXX
//je @2
//@1:
int __fastcall Idr64Manager::ProcessInt64NotEquality(DWord fromAdr, DWord* maxAdr)
{
    Byte        _op, _b;
    int         _instrLen, n, _curPos;
    DWord       _curAdr, _adr1, _adr2, _maxAdr;
    DWord       _skipAdr1, _skipAdr2;
    DISINFO     _disInfo;

    _curAdr = fromAdr; _curPos = Adr2Pos(_curAdr); _maxAdr = 0;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);

        _b = *(Code + _curPos);
        if (_b == 0xF) _b = *(Code + _curPos + 1);
        _b = (_b & 0xF) + 'A';

        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            _skipAdr1 = _curAdr;
        }
        else if (n == 2)//jne @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _b == 'F')) break;
            _skipAdr2 = _curAdr;
            _adr1 = _disInfo.Immediate;//@1
            if (_adr1 > _maxAdr) _maxAdr = _adr1;
        }
        else if (n == 3)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
        }
        else if (n == 4)//je @2
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _b == 'E' && _curAdr + _instrLen == _adr1)) break;
            _adr2 = _disInfo.Immediate;//@2
            if (_adr2 > _maxAdr) _maxAdr = _adr2;
            *maxAdr = _maxAdr;
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr1));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr2));
            return _curAdr + _instrLen - fromAdr;
        }
        if (_disInfo.Ret) return 0;
        _curAdr += _instrLen; _curPos += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction comparison ((Int64)val >(<) XXX)
//cmp XXX,XXX -> set cfSkip (_skipAdr1 = address of this instruction)
//jxx @1 -> set cfSkip (_skipAdr2 = address of this instruction)
//cmp XXX,XXX
//jxx @@ -> set cfSkip (_skipAdr3 = address of this instruction)
//jmp @@ set cfSkip (_skipAdr4 = address of this instruction)
//@1:jxx @@
int __fastcall Idr64Manager::ProcessInt64Comparison(DWord fromAdr, DWord* maxAdr)
{
    Byte        _op;
    int         _instrLen, n;
    DWord       _curAdr, _adr, _adr1, _maxAdr;
    DWord       _skipAdr1, _skipAdr2, _skipAdr3, _skipAdr4;
    DISINFO     _disInfo;

    _curAdr = fromAdr; _maxAdr = 0;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            _skipAdr1 = _curAdr;
        }
        else if (n == 2)//jxx @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _skipAdr2 = _curAdr;
            _adr1 = _disInfo.Immediate;//@1
            if (_adr1 > _maxAdr) _maxAdr = _adr1;
        }
        else if (n == 3)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
        }
        else if (n == 4)//jxx @@
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _skipAdr3 = _curAdr;
            _adr = _disInfo.Immediate;//@@
            if (_adr > _maxAdr) _maxAdr = _adr;
        }
        else if (n == 5)//jmp @@
        {
            if (!(_disInfo.Branch && !_disInfo.Conditional)) break;
            _skipAdr4 = _curAdr;
            _adr = _disInfo.Immediate;//@@
            if (_adr > _maxAdr) _maxAdr = _adr;
        }
        else if (n == 6)////@1:jxx @@
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _curAdr == _adr1)) break;
            _adr = _disInfo.Immediate;//@@
            if (_adr > _maxAdr) _maxAdr = _adr;
            *maxAdr = _maxAdr;
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr1));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr2));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr3));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr4));
            return  _curAdr + _instrLen - fromAdr;
        }
        if (_disInfo.Ret) return 0;
        _curAdr += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction comparison ((Int64)val >(<) XXX)
//push reg
//push reg
//...
//cmp XXX,[esp+4] (m-th row) set cfSkip (_skipAdr1)
//jxx @1 ->set cfSkip (_skipAdr2)
//cmp XXX,[esp]
//@1:pop reg
//pop reg
//jxx @2
int __fastcall Idr64Manager::ProcessInt64ComparisonViaStack1(DWord fromAdr, DWord* maxAdr)
{
    Byte        _op;
    int         _instrLen, n, m, _skip, _pos;
    DWord       _curAdr, _adr1, _adr2, _adr3, _maxAdr, _pushAdr;
    DWord       _skipAdr1, _skipAdr2;
    DISINFO     _disInfo;

    _curAdr = fromAdr; m = -1; _maxAdr = 0;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//push reg
        {
            if (!(_op == OP_PUSH && _disInfo.OpType[0] == otREG)) break;
        }
        else if (n == 2)//push reg
        {
            if (!(_op == OP_PUSH && _disInfo.OpType[0] == otREG)) break;
            _pushAdr = _curAdr;
        }
        else if (n >= 3 && m == -1 && _op == OP_CMP && _disInfo.OpType[1] == otMEM && _disInfo.BaseReg == 20 && _disInfo.Offset == 4)//cmp XXX,[esp+4]
        {
            //Find nearest up instruction "push reg"
            _pos = Adr2Pos(_curAdr);
            while (1)
            {
                _pos--;
                if (_pos == fromAdr) break;
                if (IsFlagSet(cfInstruction, _pos))
                {
                    GetDisasm().Disassemble(Code + _pos, Pos2Adr(_pos), &_disInfo, 0);
                    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                    if (_op == OP_PUSH) break;
                }
            }
            if (Pos2Adr(_pos) != _pushAdr) return 0;
            m = n;
            _skipAdr1 = _curAdr;
        }
        else if (m != -1 && n == m + 1)//jxx @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _skipAdr2 = _curAdr;
            _adr1 = _disInfo.Immediate;//@1
            if (_adr1 > _maxAdr) _maxAdr = _adr1;
        }
        else if (m != -1 && n == m + 2)//cmp XXX,[esp]
        {
            if (!(_op == OP_CMP && _disInfo.OpType[1] == otMEM && _disInfo.BaseReg == 20 && _disInfo.Offset == 0)) break;
        }
        else if (m != -1 && n == m + 3)//@1:pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG && _curAdr == _adr1)) break;
        }
        else if (m != -1 && n == m + 4)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG)) break;
        }
        else if (m != -1 && n == m + 5)//jxx @2
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            *maxAdr = _maxAdr;
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr1));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr2));
            return  _curAdr + _instrLen - fromAdr;
        }
        if (m == -1 && (_disInfo.Ret || _disInfo.Branch)) return 0;
        _curAdr += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction comparison ((Int64)val >(<) XXX)
//push reg
//push reg
//...
//cmp XXX,[esp+4] (m-th row) set cfSkip (_skipAdr1)
//jxx @1 ->set cfSkip (_skipAdr2)
//cmp XXX,[esp]
//pop reg ->set cfSkip (_skipAdr3)
//pop reg ->set cfSkip (_skipAdr4)
//jxx @@ ->set cfSkip (_skipAdr5)
//jmp @@ ->set cfSkip (_skipAdr6)
//@1:
//pop reg
//pop reg
//jxx @2
int __fastcall Idr64Manager::ProcessInt64ComparisonViaStack2(DWord fromAdr, DWord* maxAdr)
{
    Byte        _op;
    int         _instrLen, n, m, _skip, _pos;
    DWord       _curAdr, _adr, _adr1, _adr2, _maxAdr, _pushAdr;
    DWord       _skipAdr1, _skipAdr2, _skipAdr3, _skipAdr4, _skipAdr5, _skipAdr6;
    DISINFO     _disInfo;

    _curAdr = fromAdr; m = -1; _maxAdr = 0;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//push reg
        {
            if (!(_op == OP_PUSH && _disInfo.OpType[0] == otREG)) break;
        }
        else if (n == 2)//push reg
        {
            if (!(_op == OP_PUSH && _disInfo.OpType[0] == otREG)) break;
            _pushAdr = _curAdr;
        }
        else if (n >= 3 && m == -1 && _op == OP_CMP && _disInfo.OpType[1] == otMEM && _disInfo.BaseReg == 20 && _disInfo.Offset == 4)//cmp XXX,[esp+4]
        {
            //Find nearest up instruction "push reg"
            _pos = Adr2Pos(_curAdr);
            while (1)
            {
                _pos--;
                if (_pos == fromAdr) break;
                if (IsFlagSet(cfInstruction, _pos))
                {
                    GetDisasm().Disassemble(Code + _pos, Pos2Adr(_pos), &_disInfo, 0);
                    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                    if (_op == OP_PUSH) break;
                }
            }
            if (Pos2Adr(_pos) != _pushAdr) return 0;
            m = n;
            _skipAdr1 = _curAdr;
        }
        else if (m != -1 && n == m + 1)//jxx @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _skipAdr2 = _curAdr;
            _adr1 = _disInfo.Immediate;//@1
            if (_adr1 > _maxAdr) _maxAdr = _adr1;
        }
        else if (m != -1 && n == m + 2)//cmp XXX,[esp]
        {
            if (!(_op == OP_CMP && _disInfo.OpType[1] == otMEM && _disInfo.BaseReg == 20 && _disInfo.Offset == 0)) break;
        }
        else if (m != -1 && n == m + 3)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG)) break;
            _skipAdr3 = _curAdr;
        }
        else if (m != -1 && n == m + 4)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG)) break;
            _skipAdr4 = _curAdr;
        }
        else if (m != -1 && n == m + 5)//jxx @@
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _skipAdr5 = _curAdr;
            _adr = _disInfo.Immediate;//@3
            if (_adr > _maxAdr) _maxAdr = _adr;
        }
        else if (m != -1 && n == m + 6)//jmp @@
        {
            if (!(_disInfo.Branch && !_disInfo.Conditional)) break;
            _skipAdr6 = _curAdr;
            _adr = _disInfo.Immediate;//@2
            if (_adr > _maxAdr) _maxAdr = _adr;
        }
        else if (m != -1 && n == m + 7)//@1:pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG && _curAdr == _adr1)) break;
        }
        else if (m != -1 && n == m + 8)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG)) break;
        }
        else if (m != -1 && n == m + 9)//jxx @2
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _adr2 = _disInfo.Immediate;
            if (_adr2 > _maxAdr) _maxAdr = _adr2;
            *maxAdr = _maxAdr;
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr1));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr2));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr3));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr4));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr5));
            idr.SetFlag(cfSkip, Adr2Pos(_skipAdr6));
            return  _curAdr + _instrLen - fromAdr;
        }
        if (m == -1 && (_disInfo.Ret || _disInfo.Branch)) return 0;
        _curAdr += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction equality ((Int64)val = XXX)
//cmp XXX,XXX
//jne @1 (_br1Adr = address of this instruction)
//cmp XXX,XXX ->skip1 up to this instruction
//jne @1 -> skip2 up to this instruction, Result = skip2
//...
//@1:... -> delete 1 xRef to this instruction (address = _adr1)
int __fastcall Idr64Manager::IsInt64Equality(DWord fromAdr, int* skip1, int* skip2, bool *immVal, __int64* Val)
{
    bool        _imm;
    Byte        _op, _b;
    int         _instrLen, n, _curPos, _skip;
    DWord       _curAdr, _adr1, _br1Adr;
    DISINFO     _disInfo;
    __int64     _val1, _val2;
    //PInfoRec    _recN;

    _curAdr = fromAdr; _curPos = Adr2Pos(_curAdr); _imm = false;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);

        _b = *(Code + _curPos);
        if (_b == 0xF) _b = *(Code + _curPos + 1);
        _b = (_b & 0xF) + 'A';

        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            if (_disInfo.OpType[1] == otIMM)
            {
                _imm = true;
                _val1 = _disInfo.Immediate;
            }
        }
        else if (n == 2)//jne @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _b == 'F')) break;
            _br1Adr = _curAdr;
            _adr1 = _disInfo.Immediate;//@1
        }
        else if (n == 3)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            *skip1 = _curAdr - fromAdr;
            if (_disInfo.OpType[1] == otIMM)
            {
                _imm = true;
                _val2 = _disInfo.Immediate;
            }
        }
        else if (n == 4)//jne @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _b == 'F' && _disInfo.Immediate == _adr1)) break;
            _skip = _curAdr - fromAdr;
            *skip2 = _skip;
            *immVal = _imm;
            if (_imm) *Val = (_val1 << 32) | _val2;
            return _skip;
        }
        if (_disInfo.Ret) return 0;
        _curAdr += _instrLen; _curPos += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction not equality ((Int64)val <> XXX)
//cmp XXX,XXX
//jne @1 (_br1Adr = address of this instruction)
//cmp XXX,XXX ->skip1 up to this instruction
//je @2 -> skip2 up to this instruction, Result = skip2
//@1:... -> delete 1 xRef to this instruction (address = _adr1)
int __fastcall Idr64Manager::IsInt64NotEquality(DWord fromAdr, int* skip1, int* skip2, bool *immVal, __int64* Val)
{
    bool        _imm;
    Byte        _op, _b;
    int         _instrLen, n, _curPos, _skip;
    DWord       _curAdr, _adr1, _adr2, _br1Adr;
    DISINFO     _disInfo;
    __int64     _val1, _val2;
    //PInfoRec    _recN;

    _curAdr = fromAdr; _curPos = Adr2Pos(_curAdr); _imm = false;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);

        _b = *(Code + _curPos);
        if (_b == 0xF) _b = *(Code + _curPos + 1);
        _b = (_b & 0xF) + 'A';

        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            if (_disInfo.OpType[1] == otIMM)
            {
                _imm = true;
                _val1 = _disInfo.Immediate;
            }
        }
        else if (n == 2)//jne @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _b == 'F')) break;
            _br1Adr = _curAdr;
            _adr1 = _disInfo.Immediate;//@1
        }
        else if (n == 3)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            *skip1 = _curAdr - fromAdr;
            if (_disInfo.OpType[1] == otIMM)
            {
                _imm = true;
                _val2 = _disInfo.Immediate;
            }
        }
        else if (n == 4)//je @2
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _b == 'E' && _curAdr + _instrLen == _adr1)) break;
            _skip = _curAdr - fromAdr;
            *skip2 = _skip;
            *immVal = _imm;
            if (_imm) *Val = (_val1 << 32) | _val2;
            return _skip;
        }
        if (_disInfo.Ret) return 0;
        _curAdr += _instrLen; _curPos += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction comparison ((Int64)val >(<) XXX)
//cmp XXX,XXX
//jxx @1 (_br1Adr = address of this instruction)
//cmp XXX,XXX ->skip1 up to this instruction
//jxx @@ (_br3Adr = address of this instruction)
//jmp @@ (_br2Adr = address of this instruction)
//@1:jxx @@ (skip2 up to this instruction, Result = skip2)
int __fastcall Idr64Manager::IsInt64Comparison(DWord fromAdr, int* skip1, int* skip2, bool* immVal, __int64* Val)
{
    bool        _imm;
    Byte        _op;
    int         _instrLen, n, m, _skip;
    __int64     _val1, _val2;
    DWord       _curAdr, _adr1, _br1Adr, _br2Adr, _br3Adr;
    DISINFO     _disInfo;
    //PInfoRec    _recN;

    _curAdr = fromAdr; _imm = false;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            if (_disInfo.OpType[1] == otIMM)
            {
                _imm = true;
                _val1 = _disInfo.Immediate;
            }
        }
        else if (n == 2)//jxx @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _br1Adr = _curAdr;
            _adr1 = _disInfo.Immediate;//@1
        }
        else if (n == 3)//cmp XXX,XXX
        {
            if (!(_op == OP_CMP)) break;
            *skip1 = _curAdr - fromAdr;
            if (_disInfo.OpType[1] == otIMM)
            {
                _imm = true;
                _val2 = _disInfo.Immediate;
            }
        }
        else if (n == 4)//jxx @@
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _br3Adr = _curAdr;
        }
        else if (n == 5)//jmp @@
        {
            if (!(_disInfo.Branch && !_disInfo.Conditional)) break;
            _br2Adr = _curAdr;
        }
        else if (n == 6)////@1:jxx @@
        {
            if (!(_disInfo.Branch && _disInfo.Conditional && _curAdr == _adr1)) break;
            _skip = _curAdr - fromAdr;
            *skip2 = _skip;
            *immVal = _imm;
            if (_imm) *Val = (_val1 << 32) | _val2;
            return _skip;
        }
        if (_disInfo.Ret) return 0;
        _curAdr += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction comparison ((Int64)val >(<) N)
//push reg
//push reg
//...
//cmp XXX,[esp+4] (m-th row) ->Simulate upto this address
//jxx @1
//cmp XXX,[esp] ->skip1=this position
//@1:pop reg
//pop reg
//jxx @@ ->Result
int __fastcall Idr64Manager::IsInt64ComparisonViaStack1(DWord fromAdr, int* skip1, DWord* simEnd)
{
    Byte        _op;
    int         _instrLen, n, m, _pos;
    DWord       _curAdr = fromAdr, _adr1, _pushAdr;
    DISINFO     _disInfo;

    _curAdr = fromAdr; *skip1 = 0; *simEnd = 0; m = -1;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//push reg
        {
            if (!(_op == OP_PUSH && _disInfo.OpType[0] == otREG)) break;
        }
        else if (n == 2)//push reg
        {
            if (!(_op == OP_PUSH && _disInfo.OpType[0] == otREG)) break;
            _pushAdr = _curAdr;
        }
        else if (n >= 3 && m == -1 && _op == OP_CMP && _disInfo.OpType[1] == otMEM && _disInfo.BaseReg == 20 && _disInfo.Offset == 4)//cmp XXX,[esp+4]
        {
            //Find nearest up instruction "push reg"
            _pos = Adr2Pos(_curAdr);
            while (1)
            {
                _pos--;
                if (_pos == fromAdr) break;
                if (IsFlagSet(cfInstruction, _pos))
                {
                    GetDisasm().Disassemble(Code + _pos, Pos2Adr(_pos), &_disInfo, 0);
                    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                    if (_op == OP_PUSH) break;
                }
            }
            if (Pos2Adr(_pos) != _pushAdr) return 0;
            m = n;
            *simEnd = _curAdr;
        }
        else if (m != -1 && n == m + 1)//jxx @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _adr1 = _disInfo.Immediate;//@1
        }
        else if (m != -1 && n == m + 2)//cmp XXX,[esp]
        {
            if (!(_op == OP_CMP && _disInfo.OpType[1] == otMEM && _disInfo.BaseReg == 20 && _disInfo.Offset == 0)) break;
            *skip1 = _curAdr - fromAdr;
        }
        else if (m != -1 && n == m + 3)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG && _curAdr == _adr1)) break;
        }
        else if (m != -1 && n == m + 4)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG)) break;
        }
        else if (m != -1 && n == m + 5)//jxx @@
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            return  _curAdr - fromAdr;
        }
        if (m == -1 && (_disInfo.Ret || _disInfo.Branch)) return 0;
        _curAdr += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction comparison ((Int64)val >(<) XXX)
//push reg
//push reg
//...
//cmp XXX,[esp+4] (m-th row) ->Simulate upto this address
//jxx @1
//cmp XXX,[esp] ->skip1=this position
//pop reg
//pop reg
//jxx @@ ->skip2
//jmp @@
//@1:
//pop reg
//pop reg
//jxx @@ ->Result
int __fastcall Idr64Manager::IsInt64ComparisonViaStack2(DWord fromAdr, int* skip1, int* skip2, DWord* simEnd)
{
    Byte        _op;
    int         _instrLen, n, m, _pos;
    DWord       _curAdr = fromAdr, _adr1, _pushAdr;
    DISINFO     _disInfo;

    _curAdr = fromAdr; *simEnd = 0; m = -1;
    for (n = 1; n <= 1024; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)//push reg
        {
            if (!(_op == OP_PUSH && _disInfo.OpType[0] == otREG)) break;
        }
        else if (n == 2)//push reg
        {
            if (!(_op == OP_PUSH && _disInfo.OpType[0] == otREG)) break;
            _pushAdr = _curAdr;
        }
        else if (n >= 3 && m == -1 && _op == OP_CMP && _disInfo.OpType[1] == otMEM && _disInfo.BaseReg == 20 && _disInfo.Offset == 4)//cmp XXX,[esp+4]
        {
            //Find nearest up instruction "push reg"
            _pos = Adr2Pos(_curAdr);
            while (1)
            {
                _pos--;
                if (_pos == fromAdr) break;
                if (IsFlagSet(cfInstruction, _pos))
                {
                    GetDisasm().Disassemble(Code + _pos, Pos2Adr(_pos), &_disInfo, 0);
                    _op = GetDisasm().GetOp(_disInfo.MnemIdx);
                    if (_op == OP_PUSH) break;
                }
            }
            if (Pos2Adr(_pos) != _pushAdr) return 0;
            m = n;
            *simEnd = _curAdr;
        }
        else if (m != -1 && n == m + 1)//jxx @1
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            _adr1 = _disInfo.Immediate;//@1
        }
        else if (m != -1 && n == m + 2)//cmp XXX,[esp]
        {
            if (!(_op == OP_CMP && _disInfo.OpType[1] == otMEM && _disInfo.BaseReg == 20 && _disInfo.Offset == 0)) break;
            *skip1 = _curAdr - fromAdr;
        }
        else if (m != -1 && n == m + 3)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG)) break;
        }
        else if (m != -1 && n == m + 4)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG)) break;
        }
        else if (m != -1 && n == m + 5)//jxx @@
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            *skip2 = _curAdr - fromAdr;
        }
        else if (m != -1 && n == m + 6)//jmp @@
        {
            if (!(_disInfo.Branch && !_disInfo.Conditional)) break;
        }
        else if (m != -1 && n == m + 7)//@1:pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG && _curAdr == _adr1)) break;
        }
        else if (m != -1 && n == m + 8)//pop reg
        {
            if (!(_op == OP_POP && _disInfo.OpType[0] == otREG)) break;
        }
        else if (m != -1 && n == m + 9)//jxx @@
        {
            if (!(_disInfo.Branch && _disInfo.Conditional)) break;
            return  _curAdr - fromAdr;
        }
        if (m == -1 && (_disInfo.Ret || _disInfo.Branch)) return 0;
        _curAdr += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction
//shrd reg1,reg2,N
//shr reg2,N
int __fastcall Idr64Manager::IsInt64Shr(DWord fromAdr)
{
    Byte        _op;
    int         _instrLen, n, _idx, _val;
    DWord       _curAdr = fromAdr;
    DISINFO     _disInfo;

    for (n = 1; n <= 2; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)
        {
            if (!(_op == OP_SHR && _disInfo.OpNum == 3 && _disInfo.OpType[0] == otREG && _disInfo.OpType[1] == otREG && _disInfo.OpType[2] == otIMM)) break;
            _idx = _disInfo.OpRegIdx[1];
            _val = _disInfo.Immediate;
        }
        else if (n == 2)
        {
            if (!(_op == OP_SHR && _disInfo.OpNum == 2 && _disInfo.OpType[0] == otREG && _disInfo.OpType[1] == otIMM && _disInfo.OpRegIdx[0] == _idx && _disInfo.Immediate == _val)) break;
            return _curAdr + _instrLen - fromAdr;
        }
        if (_disInfo.Ret) return 0;
        _curAdr += _instrLen;
    }
    return 0;
}
//---------------------------------------------------------------------------
//Check construction
//shld reg1,reg2,N
//shl reg2,N
int __fastcall Idr64Manager::IsInt64Shl(DWord fromAdr)
{
    Byte        _op;
    int         _instrLen, n, _idx, _val;
    DWord       _curAdr = fromAdr;
    DISINFO     _disInfo;

    for (n = 1; n <= 2; n++)
    {
        _instrLen = GetDisasm().Disassemble(Code + Adr2Pos(_curAdr), _curAdr, &_disInfo, 0);
        _op = GetDisasm().GetOp(_disInfo.MnemIdx);
        if (n == 1)
        {
            if (!(_op == OP_SHL && _disInfo.OpNum == 3 && _disInfo.OpType[0] == otREG && _disInfo.OpType[1] == otREG && _disInfo.OpType[2] == otIMM)) break;
            _idx = _disInfo.OpRegIdx[1];
            _val = _disInfo.Immediate;
        }
        else if (n == 2)
        {
            if (!(_op == OP_SHL && _disInfo.OpNum == 2 && _disInfo.OpType[0] == otREG && _disInfo.OpType[2] == otIMM && _disInfo.OpRegIdx[0] == _idx &&_disInfo.Immediate == _val)) break;
            return _curAdr + _instrLen - fromAdr;
        }
        if (_disInfo.Ret) return 0;
        _curAdr += _instrLen;
    }
    return 0;
}

//---------------------------------------------------------------------------
void Idr64Manager::CleanProject()
{
    int         n;
    PInfoRec    recN;

    if (Image)
    {
        delete[] Image;
        Image = 0;
    }

    if (Flags)
    {
        delete[] Flags;
        Flags = 0;
    }

    if (Infos)
    {
        for (n = 0; n < TotalSize; n++)
        {
            recN = GetInfoRec(Pos2Adr(n));
            if (recN) delete recN;
        }
        delete[] Infos;
        Infos = 0;
    }
    if (BSSInfos)
    {
        for (n = 0; n < BSSInfos->Count; n++)
        {
            recN = (PInfoRec)BSSInfos->Objects[n];
            if (recN) delete recN;
        }

        delete BSSInfos;
        BSSInfos = 0;
    }

    if (SegmentList)
    {
        for (n = 0; n < SegmentList->Count; n++)
        {
            PSegmentInfo segInfo = (PSegmentInfo)SegmentList->Items[n];
            delete segInfo;
        }
        SegmentList->Clear();
    }

    if (VmtList)
        VmtList->Clear();

    if (Units)
    {
        for (n = 0; n < UnitsNum; n++)
        {
            PUnitRec recU = (PUnitRec)Units->Items[n];
            delete recU;
        }
        Units->Clear();
    }
    UnitsNum = 0;

    if (OwnTypeList)
    {
        for (n = 0; n < OwnTypeList->Count; n++)
        {
            PTypeRec recT = (PTypeRec)OwnTypeList->Items[n];
            delete recT;
        }
        OwnTypeList->Clear();
    }
}


//---------------------------------------------------------------------------
bool __fastcall Idr64Manager::IsFlagSet(DWord flag, int pos)
{
//!!!
if (pos < 0 || pos >= TotalSize)
{
  dummy = 1;
  return false;
}
    assert(pos >= 0 && pos < TotalSize);
    return (Flags[pos] & flag);
}
//---------------------------------------------------------------------------
void __fastcall Idr64Manager::SetFlag(DWord flag, int pos)
{
//!!!
if (pos < 0 || pos >= TotalSize)
{
  dummy = 1;
  return;
}
    assert(pos >= 0 && pos < TotalSize);
    Flags[pos] |= flag;
}
//---------------------------------------------------------------------------
void __fastcall Idr64Manager::SetFlags(DWord flag, int pos, int num)
{
//!!!
if (pos < 0 || pos + num >= TotalSize)
{
dummy = 1;
return;
}
    assert(pos >= 0 && (pos + num < TotalSize));
    for (int i = pos; i < pos + num; i++)
    {
        Flags[i] |= flag;
    }
}
//---------------------------------------------------------------------------
void __fastcall Idr64Manager::ClearFlag(DWord flag, int pos)
{
//!!!
if (pos < 0 || pos >= TotalSize)
{
  dummy = 1;
  return;
}
    assert(pos >= 0 && pos < TotalSize);
    Flags[pos] &= ~flag;
}
//---------------------------------------------------------------------------
void __fastcall Idr64Manager::ClearFlags(DWord flag, int pos, int num)
{
if (pos < 0 || pos + num > TotalSize)
{
dummy = 1;
return;
}
    assert(pos >= 0 && (pos + num < TotalSize));
    for (int i = pos; i < pos + num; i++)
    {
        Flags[i] &= ~flag;
    }
}

bool __fastcall Idr64Manager::IsFlagEmpty(int pos)
{
    assert(pos >= 0 && pos < TotalSize);
    return (!Flags[pos]);
}

void __fastcall Idr64Manager::XorFlag(DWord Val, int pos)
{
    assert(pos >= 0 && pos < TotalSize);
    Flags[pos] ^= Val;
}

PInfoRec Idr64Manager::GetInfos(DWord classAdr)
{
    PInfoRec        recN;
    recN = (IsValidImageAdr(classAdr)) ? Infos[Adr2Pos(classAdr)] : 0;
    return recN;
}
PInfoRec Idr64Manager::GetInfosAt(int Pos)
{
    assert(Pos >= 0); //? && < maxsize
    return Infos[Pos];
}

bool Idr64Manager::HasInfosAt(int Pos)
{
    return (Infos[Pos] != 0);
}
void Idr64Manager::SetInfosAt(int Pos, PInfoRec rec)
{
    if (HasInfosAt(Pos))
    {
        //as: if we here - memory leak then!
        ++stat_InfosOverride;
    }

    Infos[Pos] = rec;
}

void Idr64Manager::CreateDBs(DWord _TotalSize)
{
    Flags = new DWord[_TotalSize];
    memset(Flags, cfUndef, sizeof(DWord) * _TotalSize);

    Infos = new PInfoRec[_TotalSize];
    memset(Infos, 0, sizeof(PInfoRec) * _TotalSize);

    BSSInfos = new TStringList;
    BSSInfos->Sorted = true;
}
#endif
