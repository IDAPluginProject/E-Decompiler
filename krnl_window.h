#pragma once
#include <pro.h>
#include <QHash>
#include <QVariant>

class ControlInfoWidget;
class krnl_window
{
public:
	static qstring ȡ�¼�����(int eventIndex);
	static void ȡ�ؼ�����(ea_t propertyAddr, int propertySize, QHash<QString, QVariant>* out_Data);
	static void ��ʾ�ؼ���Ϣ(unsigned int propertyAddr, int propertySize);
private:
	static void ȡĬ������(QHash<QString, QVariant>& out_data);
	static void �����л�����(unsigned char* lpControlData, QHash<QString, QVariant>& out_data);
	static void ��ӱ߿�ؼ�(ControlInfoWidget* pWindow, unsigned int ProperyValue);
	static void ��ӵ�ɫ�ؼ�(ControlInfoWidget* pWindow, unsigned int ProperyValue);
};


qstring ȡ��������_����(unsigned int index);

qstring ȡ��������_λ��(unsigned int index);

qstring ȡ��������_���ָ��(unsigned int index);

qstring ȡ��������_�߿�(unsigned int index);

qstring ȡ��������_��ͼ��ʽ(unsigned int index);

qstring ȡ��������_���Ŵ���(unsigned int index);

qstring ȡ�����¼�����(int eventIndex);