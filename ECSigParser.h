#pragma once
#include <pro.h>

class insn_t;
class mid_KrnlJmp;

class ECSigParser
{
public:
	//����������ģ�麯��������
	static int GenerateECSig(ea_t startAddr);
	static void InitECSigParser(mid_KrnlJmp& inFunc);
private:
	static qstring GetSig_Mov(insn_t& ins);
	static qstring GetSig_Push(insn_t& ins);
	static qstring GetSig_Sub(insn_t& ins);
	static qstring GetSig_Call(insn_t& ins);
	//����Ƿ�Ϊ�����Ա�׼����������Ϊ������ʼ��ַ
	static bool IsEStandardFunction(ea_t startAddr);
	//Ѱ�������Ա�׼����β��,����Ϊ������ʼ��ַ,�������һ��ָ��ĵ�ַ
	static ea_t SeachEFuncEnd(ea_t startAddr);

private:
	static mid_KrnlJmp m_KrnlJmp;
};