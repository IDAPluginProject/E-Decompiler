#include "ECSigParser.h"
#include <funcs.hpp>
#include <bytes.hpp>
#include <segment.hpp>
#include <name.hpp>
#include <ua.hpp>
#include <allins.hpp>
#include <auto.hpp>
#include <fpro.h>
#include <diskio.hpp>
#include <search.hpp>
#include "SectionManager.h"
#include "ImportsParser.h"
#include "EDecompiler.h"
#include "common/public.h"

mid_KrnlJmp ECSigParser::m_KrnlJmp;
std::set<ea_t> ECSigParser::mHash_BasicFunc;
uint32 ECSigParser::m_UserResourceStartAddr = 0x00000000;
uint32 ECSigParser::m_UserResourceEndAddr = 0x0;
std::map<ea_t, qstring> ECSigParser::mSave_SubFunc;
bool ECSigParser::bFuzzySig = false;

const char* GetDataType(uint8 n)
{
	switch (n)
	{
	case 0:
		return "BYTE";
	case 1:
		return "WORD";
	case 2:
		return "DWORD";
	default:
		break;
	}
	return NULL;
}

const char* GetRegName(uint16 reg)
{
	switch (reg)
	{
	case 0:
		return "EAX";
	case 1:
		return "ECX";
	case 2:
		return "EDX";
	case 3:
		return "EBX";
	case 4:
		return "ESP";
	case 5:
		return "EBP";
	case 6:
		return "ESI";
	case 7:
		return "EDI";
	}

	msg("[GetRegName]To do...\n");
	return NULL;
}


qstring ECSigParser::GetInsPattern_Three(insn_t& ins)
{
	qstring ret;
	unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
	for (char n = 0; n < ins.ops[0].offb; ++n) {
		ret.append(UCharToStr(pData[n]));
	}
	if (IsUserResourceOffset(ins.ops[0].addr)) {
		for (char n = ins.ops[0].offb; n < ins.ops[1].offb; ++n) {
			ret.append("??");
		}
	}
	else {
		for (char n = ins.ops[0].offb; n < ins.ops[1].offb; ++n) {
			ret.append(UCharToStr(pData[n]));
		}
	}

	if (IsUserResourceImm(ins.ops[1].value)) {
		for (char n = ins.ops[1].offb; n < ins.size; ++n) {
			ret.append("??");
		}
	}
	else {
		for (char n = ins.ops[1].offb; n < ins.size; ++n) {
			ret.append(UCharToStr(pData[n]));
		}
	}

	return ret;
}

qstring ECSigParser::GetInsPattern_Two(insn_t& ins, char offset)
{
	qstring ret;
	unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);

	for (char n = 0; n < ins.size; ++n) {
		if (n < offset) {
			ret.append(UCharToStr(pData[n]));
		}
		else {
			ret.append("??");
		}
	}
	return ret;
}

qstring GetInsHex(insn_t& ins)
{
	qstring ret;
	unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
	for (unsigned int n = 0; n < ins.size; ++n) {
		ret.append(UCharToStr(pData[n]));
	}
	return ret;
}

bool ECSigParser::IsUserResourceOffset(uint32 offset)
{
	int32 distance = (uint32)offset;
	if (distance < 0) {
		offset = -offset;
	}
	return IsUserResourceImm(offset);
}

bool ECSigParser::IsUserResourceImm(uint32 imm)
{
	if (imm >= m_UserResourceStartAddr && imm < m_UserResourceEndAddr) {
		return true;
	}
	return false;
}

ea_t ECSigParser::SeachEFuncEnd(func_t* pFunc)
{
	ea_t ret = BADADDR;

	compiled_binpat_vec_t binPat;
	parse_binpat_str(&binPat, 0x0, "8B E5 5D", 16);
	segment_t* pSegment = getseg(pFunc->start_ea);
	if (!pSegment) {
		return BADADDR;
	}

	ea_t startAddr = pFunc->start_ea;

	while (true) {
		ret = bin_search2(startAddr, pSegment->end_ea, binPat, 0x0);
		if (ret == BADADDR) {
			return BADADDR;
		}
		ret = ret + 3;
		insn_t tmpIns;
		if (!decode_insn(&tmpIns, ret)) {
			return BADADDR;
		}
		if (ret + tmpIns.size >= pFunc->end_ea && tmpIns.itype == NN_retn) {
			break;
		}
		startAddr = ret;
	}
	return ret;
}

bool ECSigParser::IsEStandardFunction(ea_t startAddr)
{
	unsigned int head = get_dword(startAddr);
	if ((head & 0xFFFFFF) == 0xEC8B55) {
		return true;
	}
	return false;
}

qstring ECSigParser::GetSig_Nop(insn_t& ins)
{
	qstring ret;

	if (ins.ops[0].type == o_void) {
		ret = GetInsHex(ins);
		return ret;
	}
	

	msg("[GetSig_Nop]To do...\n");
	return ret;
}

qstring ECSigParser::GetSig_Call(insn_t& ins, qvector<qstring>& vec_saveSig)
{
	qstring ret;
	if (ins.ops[0].type == o_near) {
		//ϵͳ����֧�ֿ�
		if (ins.ops[0].addr == m_KrnlJmp.Jmp_MCallKrnlLibCmd) {
			insn_t LastIns;
			decode_prev_insn(&LastIns, ins.ip);
			//�ж���һ��ָ���ǲ���mov ebx,�����ַ
			if (LastIns.itype == NN_mov && LastIns.ops[0].reg == 3 && LastIns.ops[1].type == o_imm) {
				vec_saveSig[vec_saveSig.size() - 1] = "BB????????";
				if (bFuzzySig) {
					return getUTF8String("<δ֪����>");
				}
				qstring KrnlLibName = get_name(LastIns.ops[1].value);
				if (KrnlLibName.substr(0, 4) == "sub_") {
					msg("[GetSig_Call]Function not Scanned,%a", LastIns.ops[1].value);
					ret = getUTF8String("<δ֪����>");
					return ret;
				}
				ret.sprnt("<%s>", KrnlLibName.c_str());
				return ret;
			}
		}
		//�����ڴ�
		if (ins.ops[0].addr == m_KrnlJmp.Jmp_MMalloc) {
			ret = getUTF8String("<�����ڴ�>");
			return ret;
		}
		if (ins.ops[0].addr == m_KrnlJmp.Jmp_MRealloc) {
			ret = getUTF8String("<���·����ڴ�>");
			return ret;
		}
		//�ͷ��ڴ�
		if (ins.ops[0].addr == m_KrnlJmp.Jmp_MFree) {
			ret = getUTF8String("<�ͷ��ڴ�>");
			return ret;
		}
		//����DLL����
		if (ins.ops[0].addr == m_KrnlJmp.Jmp_MCallDllCmd) {
			insn_t LastIns;
			decode_prev_insn(&LastIns, ins.ip);
			//�ж���һ��ָ���ǲ���mov eax,DLL���
			if (LastIns.itype == NN_mov && LastIns.ops[0].reg == 0 && LastIns.ops[1].type == o_imm) {
				vec_saveSig[vec_saveSig.size() - 1] = "B8????????";
				qstring DLLFuncName = ImportsParser::mVec_ImportsApi[LastIns.ops[1].value].ApiName;
				ret.sprnt("[%s]", DLLFuncName.c_str());
				return ret;
			}
		}
		//����ص�����
		if (ins.ops[0].addr == m_KrnlJmp.Jmp_MReportError) {
			ea_t lastInsAddr = ins.ip;
			for (unsigned int n = 0; n < 3; ++n) {
				insn_t LastIns;
				lastInsAddr = decode_prev_insn(&LastIns, lastInsAddr);
				if (LastIns.itype == NN_push && LastIns.ops[0].type == o_imm) {
					vec_saveSig[vec_saveSig.size() - 1 - n] = "68????????";
				}
			}
			ret = getUTF8String("<����ص�>");
			return ret;
		}
		//��������
		if (ins.ops[0].addr == m_KrnlJmp.Jmp_MOtherHelp) {
			ret = getUTF8String("<��������>");
			return ret;
		}
		if (mHash_BasicFunc.count(ins.ops[0].addr)) {
			ret.sprnt("<%s>",get_name(ins.ops[0].addr).c_str());
			return ret;
		}

		//�û��Զ��庯��
		qstring subFuncName = mSave_SubFunc[ins.ops[0].addr];
		if (subFuncName.empty()) {
			//��ֹ�ݹ�ѭ��
			mSave_SubFunc[ins.ops[0].addr] = "<RecurFunc>";
			subFuncName = GetFunctionMD5(ins.ops[0].addr);
			if (subFuncName.empty()) {
				//��ʾ�ú�����ʵ���������
				ret = GetInsHex(ins);
				mSave_SubFunc[ins.ops[0].addr] = ret;
				return ret;
			}
			ret.sprnt("<%s>", subFuncName.c_str());
			mSave_SubFunc[ins.ops[0].addr] = ret;
			return ret;
		}
		ret.sprnt("%s", subFuncName.c_str());
		return ret;
	}
	
	msg("[GetSig_Call]To do...\n");
	return ret;
}

qstring ECSigParser::GetSig_FloatInstA(insn_t& ins)
{
	qstring ret;
	if (ins.ops[0].type == o_reg) {
		return GetInsHex(ins);
	}

	if (ins.ops[0].type == o_mem) {
		unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
		for (char n = 0; n < ins.size; ++n) {
			if (n >= ins.ops[0].offb) {
				ret.append("??");
			}
			else {
				ret.append(UCharToStr(pData[n]));
			}
		}
		return ret;
	}

	if (ins.ops[0].type == o_phrase) {
		return GetInsHex(ins);
	}

	//fadd [ebp-0xC]
	if (ins.ops[0].type == o_displ) {
		if (IsUserResourceOffset(ins.ops[0].addr)) {
			unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
			for (char n = 0; n < ins.size; ++n) {
				if (n >= ins.ops[0].offb) {
					ret.append("??");
				}
				else {
					ret.append(UCharToStr(pData[n]));
				}
			}
		}
		else {
			ret = GetInsHex(ins);
		}
		return ret;
	}
	msg("[GetSig_FloatInstA]To do...\n");
	return ret;
}

qstring ECSigParser::GetSig_FloatInstB(insn_t& ins)
{
	qstring ret;
	if (ins.ops[1].type == o_mem) {
		unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
		for (char n = 0; n < ins.size; ++n) {
			if (n >= ins.ops[1].offb) {
				ret.append("??");
			}
			else {
				ret.append(UCharToStr(pData[n]));
			}
		}
		return ret;
	}

	//fadd [ebp-0xC]
	if (ins.ops[1].type == o_displ) {
		if (IsUserResourceOffset(ins.ops[1].addr)) {
			unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
			for (char n = 0; n < ins.size; ++n) {
				if (n >= ins.ops[1].offb) {
					ret.append("??");
				}
				else {
					ret.append(UCharToStr(pData[n]));
				}
			}
		}
		else {
			ret = GetInsHex(ins);
		}
		return ret;
	}
	return GetInsHex(ins);
}


qstring ECSigParser::GetSig_FlexDoubleInst(insn_t& ins)
{
	qstring ret;
	ret.reserve(ins.size * 2 + 1);

	if (ins.ops[0].type == o_reg) {
		//mov eax,ebx
		//mov eax,[eax+ebx]
		if (ins.ops[1].type == o_reg || ins.ops[1].type == o_phrase) {
			ret = GetInsHex(ins);
			return ret;
		}

		// mov eax,[0x401000]
		if (ins.ops[1].type == o_mem) {
			unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
			for (char n = 0; n < ins.size; ++n) {
				if (n >= ins.ops[1].offb) {
					ret.append("??");
				}
				else {
					ret.append(UCharToStr(pData[n]));
				}
			}
			return ret;
		}

		//mov eax,[eax+ebx-0x401000]
		if (ins.ops[1].type == o_displ) {
			if (IsUserResourceOffset(ins.ops[1].addr)) {
				unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
				for (char n = 0; n < ins.size; ++n) {
					if (n >= ins.ops[1].offb) {
						ret.append("??");
					}
					else {
						ret.append(UCharToStr(pData[n]));
					}
				}
			}
			else {
				ret = GetInsHex(ins);
			}
			return ret;
		}

		//mov eax,0x401000
		if (ins.ops[1].type == o_imm) {
			goto label_HEX_PAT;
		}
	}

	if (ins.ops[0].type == o_mem) {
		//mov [0x401000],eax
		if (ins.ops[1].type == o_reg) {
			unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
			for (char n = 0; n < ins.size; ++n) {
				if (n >= ins.ops[0].offb) {
					ret.append("??");
				}
				else {
					ret.append(UCharToStr(pData[n]));
				}
			}
			return ret;
		}
		//mov [0x401000],0x501000
		if (ins.ops[1].type == o_imm) {
			unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
			if (IsUserResourceImm(ins.ops[1].value)) {
				for (char n = 0; n < ins.size; ++n) {
					if (n >= ins.ops[0].offb) {
						ret.append("??");
					}
					else {
						ret.append(UCharToStr(pData[n]));
					}
				}
			}
			else {
				for (char n = 0; n < ins.size; ++n) {
					if (n >= ins.ops[0].offb && n < ins.ops[1].offb) {
						ret.append("??");
					}
					else {
						ret.append(UCharToStr(pData[n]));
					}
				}
			}
			return ret;
		}
	}

	if (ins.ops[0].type == o_phrase) {
		//mov [eax+ebx],eax
		if (ins.ops[1].type == o_reg) {
			return GetInsHex(ins);
		}
		//mov [eax+ebx],0x401000
		if (ins.ops[1].type == o_imm) {
			goto label_HEX_PAT;
		}
	}
	
	if (ins.ops[0].type == o_displ) {

		//mov [ebp+ebx+0x401000],eax 
		if (ins.ops[1].type == o_reg) {
			if(IsUserResourceOffset(ins.ops[0].addr)) {
				unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
				for (char n = 0; n < ins.size; ++n) {
					if (n >= ins.ops[0].offb && n < ins.ops[1].offb) {
						ret.append("??");
					}
					else {
						ret.append(UCharToStr(pData[n]));
					}
				}
			}
			else {
				ret = GetInsHex(ins);
			}
			return ret;
		}

		//mov [eax+ebx-0x401000],0x401000
		if (ins.ops[1].type == o_imm) {
			unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
			for (char n = 0; n < ins.ops[0].offb; ++n) {
				ret.append(UCharToStr(pData[n]));
			}
			if (IsUserResourceOffset(ins.ops[0].addr)) {
				for (char n = ins.ops[0].offb; n < ins.ops[1].offb; ++n) {
					ret.append("??");
				}
			}
			else {
				for (char n = ins.ops[0].offb; n < ins.ops[1].offb; ++n) {
					ret.append(UCharToStr(pData[n]));
				}
			}
			if (IsUserResourceImm(ins.ops[1].value)) {
				for (char n = ins.ops[1].offb; n < ins.size; ++n) {
					ret.append("??");
				}
			}
			else {
				for (char n = ins.ops[1].offb; n < ins.size; ++n) {
					ret.append(UCharToStr(pData[n]));
				}
			}
			return ret;
		}
	}

	msg("[GetSig_FlexDoubleInst]To do...\n");
	return ret;

label_HEX_PAT:
	if (IsUserResourceImm(ins.ops[1].value)) {
		unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
		for (char n = 0; n < ins.size; ++n) {
			if (n < ins.ops[1].offb) {
				ret.append(UCharToStr(pData[n]));
			}
			else {
				ret.append("??");
			}
		}
	}
	else {
		ret = GetInsHex(ins);
	}
	return ret;
}

qstring ECSigParser::GetSig_Imul(insn_t& ins)
{
	qstring ret;

	if (ins.ops[0].type == o_reg) {
		if (ins.ops[1].type == o_phrase) {
			//imul eax,[eax+ebx]
			if (ins.ops[2].type == o_void) {
				return GetInsHex(ins);
			}
		}
	}

	msg("[GetSig_Imul]To do...\n");
	return ret;
}

qstring ECSigParser::GetSig_LogicInst(insn_t& ins)
{
	qstring ret;

	if (ins.ops[0].type == o_reg) {
		if (ins.ops[1].type == o_reg) {
			ret = GetInsHex(ins);
			return ret;
		}

		//test eax,0x401000
		if (ins.ops[1].type == o_imm) {
			ret = GetInsHex(ins);
			return ret;
		}
	}


	msg("[GetSig_LogicInst]To do...\n");
	return ret;
}


qstring ECSigParser::GetSig_FlexSingleInst(insn_t& ins)
{
	qstring ret;

	//push reg
	if (ins.ops[0].type == o_reg) {
		ret = GetInsHex(ins);
		return ret;
	}

	if (ins.ops[0].type == o_mem) {
		goto label_PAT;
	}

	//push [eax]
	if (ins.ops[0].type == o_phrase) {
		ret = GetInsHex(ins);
		return ret;
	}


	//push [eax+ebx+0x401000]
	if (ins.ops[0].type == o_displ) {
		if (IsUserResourceOffset(ins.ops[0].addr)) {
			goto label_PAT;
		}
		else {
			ret = GetInsHex(ins);
		}
		return ret;
	}
	
	if (ins.ops[0].type == o_imm) {
		if (IsUserResourceImm(ins.ops[0].value)) {
			goto label_PAT;
		}
		else {
			ret = GetInsHex(ins);
		}
		return ret;
	}

	msg("[GetSig_FlexSingleInst]To do...\n");
	return ret;
label_PAT:
	unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(ins.ip);
	for (char n = 0; n < ins.size; ++n) {
		if (n >= ins.ops[0].offb) {
			ret.append("??");
		}
		else {
			ret.append(UCharToStr(pData[n]));
		}
	}
	return ret;
}

void ECSigParser::InitECSigResource(uint32 startAddr, uint32 endAddr)
{
	m_UserResourceStartAddr = startAddr;
	m_UserResourceEndAddr = endAddr;
}

void ECSigParser::InitECSigBasciFunc(std::set<ea_t>& mhash)
{
	mHash_BasicFunc.clear();
	mHash_BasicFunc = mhash;
}

void ECSigParser::InitECSigKrnl(mid_KrnlJmp& inFunc)
{
	m_KrnlJmp = inFunc;
}

void ECSigParser::ScanMSig(const char* lpsigPath, ea_t rangeStart, ea_t rangeEnd)
{
	qstring str_filePath;
	acp_utf8(&str_filePath, lpsigPath);

	FILE* hFile = fopenRB(str_filePath.c_str());
	if (!hFile)
	{
		return;
	}

	std::multimap<qstring, qstring> map_MSig;

	qstring str_Line;
	while (-1 != qgetline(&str_Line, hFile)) {
		size_t spitIndex = str_Line.find(":");
		if (spitIndex == qstring::npos) {
			continue;
		}
		qstring funcName = str_Line.substr(0, spitIndex);
		qstring funcSig = str_Line.substr(spitIndex + 1);
		if (funcSig.last() == '\r') {
			funcSig.remove_last();
		}
		if (funcSig.length() != 32) {
			continue;
		}
		map_MSig.insert(std::make_pair(funcSig, funcName));
	}

	size_t funcCount = get_func_qty();
	for (unsigned int idx = 0; idx < funcCount; ++idx)
	{
		func_t* pFunc = getn_func(idx);

		if (pFunc->start_ea < rangeStart || pFunc->start_ea >= rangeEnd) {
			continue;
		}

		if (pFunc->start_ea == 0x40163F) {
			int a = 0;
		}

		bFuzzySig = false;
		qstring goodMD5 = GetFunctionMD5(pFunc->start_ea);
		auto funcCount = map_MSig.count(goodMD5);

		if (funcCount == 1) {
			auto it = map_MSig.find(goodMD5);
			setFuncName(pFunc->start_ea, it->second.c_str());
			msg("%s%a--%s\n", getUTF8String("ʶ��ģ�麯��").c_str(), pFunc->start_ea, getUTF8String(it->second.c_str()).c_str());
			continue;
		}
		else if (funcCount != 0) {

		}
		

		bFuzzySig = true;
		qstring badMD5 = GetFunctionMD5(pFunc->start_ea);
		funcCount = map_MSig.count(badMD5);
		if (funcCount) {
			auto it = map_MSig.find(badMD5);
			setFuncName(pFunc->start_ea, it->second.c_str());
			continue;
		}
		if (funcCount != 0) {

		}
	}

	qfclose(hFile);
	return;
}

qstring ECSigParser::GetFunctionMD5(ea_t FuncStartAddr)
{
	qstring ret_MD5;

	func_t* pFunc = get_func(FuncStartAddr);
	if (!pFunc) {
		return ret_MD5;
	}

	if (!IsEStandardFunction(pFunc->start_ea)) {
		return ret_MD5;
	}

	ea_t endAddr = SeachEFuncEnd(pFunc);
	if (endAddr == BADADDR) {
		return ret_MD5;
	}

	ea_t startAddr = pFunc->start_ea;
	qvector<qstring> vec_SaveSig;
	do
	{
		qstring tmpSig;
		insn_t CurrentIns;
		int insLen = decode_insn(&CurrentIns, startAddr);
		
		if (!insLen) {
			ea_t nextCodeAddr = find_code(startAddr, SEARCH_DOWN | SEARCH_NOSHOW);
			if (nextCodeAddr >= endAddr) {
				nextCodeAddr = endAddr;
			}
			unsigned char* pData = SectionManager::LinearAddrToVirtualAddr(startAddr);
			for (unsigned int n = 0; n < nextCodeAddr - startAddr; ++n) {
				tmpSig.append(UCharToStr(pData[n]));
			}
			vec_SaveSig.push_back(tmpSig);
			startAddr = nextCodeAddr;
			continue;
		}

		if (CurrentIns.ip == 0x0040204B) {
			int a = 0;
		}

		switch (CurrentIns.itype)
		{
		case NN_ja:
		case NN_jae:
		case NN_jb:
		case NN_jbe:
		case NN_jc:
		case NN_jcxz:
		case NN_jecxz:
		case NN_jrcxz:
		case NN_je:
		case NN_jg:
		case NN_jge:
		case NN_jl:
		case NN_jle:
		case NN_jna:
		case NN_jnae:
		case NN_jnb:
		case NN_jnbe:
		case NN_jnc:
		case NN_jne:
		case NN_jng:
		case NN_jnge:
		case NN_jnl:
		case NN_jnle:
		case NN_jno:
		case NN_jnp:
		case NN_jns:
		case NN_jnz:
		case NN_jo:
		case NN_jp:
		case NN_jpe:
		case NN_jpo:
		case NN_js:
		case NN_jz:
		case NN_jmp:
		case NN_jmpfi:
		case NN_jmpni:
		case NN_jmpshort:
			tmpSig = GetInsHex(CurrentIns);
			break;
		case NN_add:
		case NN_cmp:
		case NN_mov:
		case NN_sub:
			tmpSig = GetSig_FlexDoubleInst(CurrentIns);
			break;
		case NN_fldcw:
		case NN_fnstcw:
		case NN_fstsw:
			tmpSig = GetSig_FloatInstA(CurrentIns);
			break;
		case NN_fld:
		case NN_fstp:
		case NN_fild:
		case NN_fadd:
		case NN_fsub:
		case NN_fmul:
		case NN_fdiv:
		case NN_fcomp:
		case NN_fistp:
		case NN_fstcw:
			tmpSig = GetSig_FloatInstB(CurrentIns);
			break;
		case NN_inc:
		case NN_dec:
		case NN_push:
			tmpSig = GetSig_FlexSingleInst(CurrentIns);
			break;
		case NN_call:
			tmpSig = GetSig_Call(CurrentIns, vec_SaveSig);
			break;
		//nopָ��Ĵ����Ǹ�����,�����Ԥ���������ԵĻ�ָ���
		case NN_nop:
			tmpSig = "";
			break;
		//��������ָ��(�۲���)
		case NN_lea:
		case NN_setnz:
		case NN_setz:
		case NN_imul:
		case NN_and:
		case NN_or:
		case NN_shl:
		case NN_shr:
		case NN_xor:
		case NN_test:
		case NN_loop:
		case NN_loopd:
		case NN_fninit:
		case NN_cld:
		case NN_sbb:
		case NN_not:
		case NN_mul:
		case NN_ftst:
		case NN_fnstsw:
		case NN_fchs:
		case NN_cwde:
		case NN_movs:
		case NN_pop:
		case NN_retf:
			tmpSig = GetInsHex(CurrentIns);
			break;
			//��������ָ��(�߶�ȷ��)
		case NN_aaa:
		case NN_aad:
		case NN_aam:
		case NN_aas:
		case NN_adc:
		case NN_arpl:
		case NN_bound:
		case NN_bsf:
		case NN_bsr:
		case NN_bt:
		case NN_btc:
		case NN_btr:
		case NN_bts:
		case NN_cbw:
		case NN_cdqe:
		case NN_clc:
		case NN_cli:
		case NN_clts:
		case NN_cmc:
		case NN_daa:
		case NN_das:
		case NN_wait:
		case NN_cmova:
		case NN_cmovnz:
		case NN_cpuid:
		case NN_leave:
		case NN_xgetbv:
		case NN_vxorpd:
		case NN_vpcmpeqb:
		case NN_vpmovmskb:
		case NN_movdqa:
		case NN_pxor:
		case NN_movq:
		case NN_pcmpeqb:
		case NN_pmovmskb:
		case NN_movzx:
		case NN_vmovdqu:
			tmpSig = GetInsHex(CurrentIns);
			break;
		//���������ָ��
		case NN_lods:
		case NN_stos:
		case NN_retn:
			tmpSig = GetInsHex(CurrentIns);
			break;
		default:
			//msg("UnHandled Instruction--%a\n", CurrentIns.ip);
			tmpSig = GetInsHex(CurrentIns);
			break;
		}

		vec_SaveSig.push_back(tmpSig);
		if (tmpSig.empty() && CurrentIns.itype != NN_nop) {
			msg("%s--%a\n", getUTF8String("��ȡ����ʧ��").c_str(), startAddr);
		}
		startAddr = startAddr + CurrentIns.size;

	} while (startAddr <= endAddr);

	qstring STRING_RESULT;
	for (unsigned int n = 0; n < vec_SaveSig.size(); ++n) {
		STRING_RESULT.append(vec_SaveSig[n]);
	}

	ret_MD5 = CalculateMD5(STRING_RESULT);


#ifdef _DEBUG
	if (!bFuzzySig) {
		msg("[%a]:%s\n", FuncStartAddr, STRING_RESULT.c_str());
	}
#endif // _DEBUG
	
	return ret_MD5;
}

int ECSigParser::GenerateECSig(ea_t addr)
{
	mSave_SubFunc.clear();

	if (!auto_is_ok()) {
		info(getUTF8String("��ȴ�IDA�������...").c_str());
		return false;
	}

	func_t* pFunc = get_func(addr);
	if (!pFunc) {
		return false;
	}

	//����ȡ��ϸ
	bFuzzySig = false;
	qstring GoodMd5 = GetFunctionMD5(pFunc->start_ea);
	
	mSave_SubFunc.clear();
	bFuzzySig = true;
	qstring BadMd5 = GetFunctionMD5(pFunc->start_ea);

	msg("[%s%a]:%s\n", getUTF8String("��ȷ����").c_str(), pFunc->start_ea, GoodMd5.c_str());
	msg("[%s%a]:%s\n", getUTF8String("ģ������").c_str(), pFunc->start_ea, BadMd5.c_str());

	return true;
}