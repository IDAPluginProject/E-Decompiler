#pragma once
#include <pro.h>

//������Դ��������
class UserResourceParser
{
public:
	enum BinType_t
	{
		e_UnknownValue = 0,
		e_NullBin,
		e_BinValue,
		e_NullStr,
		e_StringValue,
		e_ArrayHead,
		e_FloatValue,
		e_ClassTable,
		e_SwitchTable,         //switch case��
	};
	struct mid_BinSource
	{
		BinType_t itype;
		ea_t address;
		unsigned int extraData;	//��������
	};
public:
	static bool ParseUserResource(ea_t lpStringStart, uint32 StringSize);
	static int MenuHandle_ShowUserResource();
public:
	
};