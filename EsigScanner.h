#pragma once
#include <pro.h>

class ESigScanner
{
public:
	//ɨ�貢ʶ��֧�ֿ⺯��
	static bool ScanLibFunction(ea_t lpLibStartAddr, uint32 dwLibCount);
};
