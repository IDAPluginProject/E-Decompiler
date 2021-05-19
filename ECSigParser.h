#pragma once
#include <pro.h>

class ECSigParser
{
public:
	//����������ģ�麯��������
	static int GenerateECSig(ea_t startAddr);

private:
	//����Ƿ�Ϊ�����Ա�׼����������Ϊ������ʼ��ַ
	static bool IsEStandardFunction(ea_t startAddr);
	//Ѱ�������Ա�׼����β��,����Ϊ������ʼ��ַ,�������һ��ָ��ĵ�ַ
	static ea_t SeachEFuncEnd(ea_t startAddr);
};