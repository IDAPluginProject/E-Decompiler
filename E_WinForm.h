#pragma once
#include <pro.h>

struct WinFrom_UnitData
{
	unsigned int version;
	unsigned int �߿�;
	unsigned int ��ͼ��ʽ;
	unsigned int ��ɫ;
	unsigned int ��󻯰�ť;
	unsigned int ��С����ť;
	unsigned int ���ư�ť;
	unsigned int λ��;
	unsigned int �ɷ��ƶ�;
	unsigned int ���Ŵ���;
	unsigned int �س����ƽ���;
	unsigned int Esc���ر�;
	unsigned int F1���򿪰���;
	unsigned int �������ֵ;
	unsigned int ������������ʾ;
	unsigned int �����ƶ�;
	unsigned int ����;
	unsigned int ������ǰ;
	unsigned int ���ֱ���������;
	qstring ��������;
	qvector<unsigned char> ��ͼ;
	qvector<unsigned char> ͼ��;
	qvector<unsigned char> ��������;
	qstring ����;
	qstring �����ļ���;
};

void Unserialize_WinFormData(unsigned char* lpControlData, WinFrom_UnitData* out_Data);

void ȡ��������_Ĭ��(WinFrom_UnitData* out_Data);

qstring ȡ��������_����(unsigned int index);

qstring ȡ��������_λ��(unsigned int index);

qstring ȡ��������_���ָ��(unsigned int index);

qstring ȡ��������_�߿�(unsigned int index);

qstring ȡ��������_��ͼ��ʽ(unsigned int index);

qstring ȡ��������_���Ŵ���(unsigned int index);