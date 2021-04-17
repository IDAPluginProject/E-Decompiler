#include "krnl_window.h"
#include <bytes.hpp>
#include "GuiParser.h"
#include "ControlInfoWidget.h"
#include "PropertyDelegate.h"
#include "public.h"
#include <QtWidgets/QComboBox>
#include <QVariant>

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

void krnl_window::�����л�����(unsigned char* lpControlData, QHash<QString, QVariant>& out_data)
{
	WinFrom_UnitData tmpData;
	tmpData.version = CDR_ReadUInt(lpControlData);

	if (tmpData.version <= 6) {
		tmpData.�߿� = CDR_ReadUInt(lpControlData);
		tmpData.��ͼ��ʽ = CDR_ReadUInt(lpControlData);
		tmpData.��ɫ = CDR_ReadUInt(lpControlData);
		tmpData.��󻯰�ť = CDR_ReadUInt(lpControlData);
		tmpData.��С����ť = CDR_ReadUInt(lpControlData);
		tmpData.���ư�ť = CDR_ReadUInt(lpControlData);
		tmpData.λ�� = CDR_ReadUInt(lpControlData);
		tmpData.�ɷ��ƶ� = CDR_ReadUInt(lpControlData);
		tmpData.���Ŵ��� = CDR_ReadUInt(lpControlData);
		tmpData.�س����ƽ��� = CDR_ReadUInt(lpControlData);
		tmpData.Esc���ر� = CDR_ReadUInt(lpControlData);
		tmpData.F1���򿪰��� = CDR_ReadUInt(lpControlData);
		tmpData.�������ֵ = CDR_ReadUInt(lpControlData);

		if (tmpData.version >= 2) {
			tmpData.������������ʾ = CDR_ReadUInt(lpControlData);
		}
		if (tmpData.version >= 3) {
			tmpData.�����ƶ� = CDR_ReadUInt(lpControlData);
			tmpData.���� = CDR_ReadUInt(lpControlData);
		}
		if (tmpData.version >= 4) {
			tmpData.������ǰ = CDR_ReadUInt(lpControlData);
		}
		if (tmpData.version >= 5) {
			tmpData.���ֱ��������� = CDR_ReadUInt(lpControlData);
		}
		if (tmpData.version == 6) {
			tmpData.�������� = CDR_ReadCString(lpControlData);
		}

		tmpData.��ͼ = CDR_ReadCFreqMem(lpControlData);
		tmpData.ͼ�� = CDR_ReadCFreqMem(lpControlData);
		tmpData.�������� = CDR_ReadCFreqMem(lpControlData);

		tmpData.���� = CDR_ReadCString(lpControlData);
		tmpData.�����ļ��� = CDR_ReadCString(lpControlData);
	}

	out_data[QStringLiteral("version")] = tmpData.version;
	out_data[QStringLiteral("�߿�")] = tmpData.�߿�;
	out_data[QStringLiteral("��ͼ��ʽ")] = tmpData.��ͼ��ʽ;
	out_data[QStringLiteral("��ɫ")] = tmpData.��ɫ;
	out_data[QStringLiteral("��󻯰�ť")] = tmpData.��С����ť;
	out_data[QStringLiteral("���ư�ť")] = tmpData.���ư�ť;
	out_data[QStringLiteral("λ��")] = tmpData.λ��;
	out_data[QStringLiteral("�ɷ��ƶ�")] = tmpData.�ɷ��ƶ�;
	out_data[QStringLiteral("���Ŵ���")] = tmpData.���Ŵ���;
	out_data[QStringLiteral("�س����ƽ���")] = tmpData.�س����ƽ���;
	out_data[QStringLiteral("Esc���ر�")] = tmpData.Esc���ر�;
	out_data[QStringLiteral("F1���򿪰���")] = tmpData.F1���򿪰���;
	out_data[QStringLiteral("�������ֵ")] = tmpData.�������ֵ;
	out_data[QStringLiteral("������������ʾ")] = tmpData.������������ʾ;
	out_data[QStringLiteral("�����ƶ�")] = tmpData.�����ƶ�;
	out_data[QStringLiteral("����")] = tmpData.����;
	out_data[QStringLiteral("������ǰ")] = tmpData.������ǰ;
	out_data[QStringLiteral("���ֱ���������")] = tmpData.���ֱ���������;
	out_data[QStringLiteral("��������")] = QString::fromLocal8Bit(tmpData.��������.c_str());
	
	if (tmpData.��ͼ.size()) {
		out_data[QStringLiteral("��ͼ")] = QByteArray((char*)&tmpData.��ͼ[0], tmpData.��ͼ.size());
	}
	if (tmpData.ͼ��.size()) {
		out_data[QStringLiteral("ͼ��")] = QByteArray((char*)&tmpData.ͼ��[0], tmpData.ͼ��.size());
	}
	if (tmpData.��������.size()) {
		out_data[QStringLiteral("��������")] = QByteArray((char*)&tmpData.��������[0], tmpData.��������.size());
	}
	
	out_data[QStringLiteral("����")] = QString::fromLocal8Bit(tmpData.����.c_str());
	out_data[QStringLiteral("�����ļ���")] = QString::fromLocal8Bit(tmpData.�����ļ���.c_str());
	return;
}

qstring ȡ��������_λ��(unsigned int index)
{
	qstring ret;
	switch (index)
	{
	case 0:
		ret = "ͨ��";
		break;
	case 1:
		ret = "����";
		break;
	case 2:
		ret = "��С��";
		break;
	case 3:
		ret = "���";
		break;
	default:
		ret = "δ֪λ��";
		break;
	}
	return ret;
}

qstring ȡ��������_����(unsigned int index)
{
	qstring ret;
	switch (index)
	{
	case 0:
		ret = "����";
		break;
	case 1:
		ret = "��Բ";
		break;
	case 2:
		ret = "Բ�Ǿ���";
		break;
	case 3:
		ret = "��";
		break;
	case 4:
		ret = "������";
		break;
	case 5:
		ret = "������";
		break;
	case 6:
		ret = "������";
		break;
	case 7:
		ret = "������";
		break;
	case 8:
		ret = "ƽ���ı���";
		break;
	case 9:
		ret = "�����";
		break;
	case 10:
		ret = "������";
		break;
	case 11:
		ret = "����";
		break;
	case 12:
		ret = "����";
		break;
	case 13:
		ret = "�����";
		break;
	case 14:
		ret = "ʮ����";
		break;
	case 15:
		ret = "������";
		break;
	case 16:
		ret = "��ը��1";
		break;
	case 17:
		ret = "��ը��2";
		break;
	case 18:
		ret = "��β";
		break;
	case 19:
		ret = "�۽Ǿ���";
		break;
	case 20:
		ret = "���ͷ";
		break;
	case 21:
		ret = "�Ҽ�ͷ";
		break;
	case 22:
		ret = "�ϼ�ͷ";
		break;
	case 23:
		ret = "�¼�ͷ";
		break;
	case 24:
		ret = "���Ҽ�ͷ";
		break;
	case 25:
		ret = "���¼�ͷ";
		break;
	case 26:
		ret = "ʮ�ּ�ͷ";
		break;
	case 27:
		ret = "���Ӽ�ͷ";
		break;
	case 28:
		ret = "��β��ͷ";
		break;
	case 29:
		ret = "����μ�ͷ";
		break;
	default:
		ret = "δ֪��ͷ";
		break;
	}
	return ret;

}



qstring ȡ��������_�߿�(unsigned int index)
{
	qstring ret;
	switch (index)
	{
	case 0x0:
		ret = "�ޱ߿�";
		break;
	case 1:
		ret = "��ͨ�ɵ��߿�";
		break;
	case 2:
		ret = "��ͨ�̶��߿�";
		break;
	case 3:
		ret = "խ����ɵ��߿�";
		break;
	case 4:
		ret = "խ����̶��߿�";
		break;
	case 5:
		ret = "����ʽ�ɵ��߿�";
		break;
	case 6:
		ret = "����ʽ�̶��߿�";
		break;
	default:
		ret = "δ֪�߿�";
		break;
	}

	return ret;
}


qstring ȡ��������_��ͼ��ʽ(unsigned int index)
{
	qstring ret;
	switch (index)
	{
	case 0:
		ret = "ͼƬ������";
		break;
	case 1:
		ret = "ͼƬƽ��";
		break;
	case 2:
		ret = "ͼƬ����";
		break;
	case 3:
		ret = "����ͼƬ";
		break;
	default:
		ret = "δ֪��ʽ";
		break;
	}
	return ret;
}

qstring ȡ��������_���Ŵ���(unsigned int index)
{
	qstring ret;
	switch (index)
	{
	case 0:
		ret = "ѭ������";
		break;
	case 1:
		ret = "������һ��";
		break;
	case 2:
		ret = "������";
		break;
	default:
		ret = "δ֪����";
		break;
	}
	return ret;
}
void krnl_window::��ӵ�ɫ�ؼ�(ControlInfoWidget* pWindow, unsigned int ProperyValue)
{
	int insertRow = pWindow->ui.ControlTable->rowCount();
	pWindow->ui.ControlTable->insertRow(insertRow);

	pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("��ɫ")));


	if (ProperyValue == 0xFF000000) {
		uint32 color = GetSysColor(COLOR_BTNFACE);
		pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("Ĭ�ϵ�ɫ"), ui_ColorDialog));
		pWindow->ui.ControlTable->item(insertRow, COLUMN_PropertyValue)->setBackgroundColor(QColor(GetRValue(color), GetGValue(color), GetBValue(color)));
	}
	else {
		pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyValue, new QTableWidgetItem("", ui_ColorDialog));
		pWindow->ui.ControlTable->item(insertRow, COLUMN_PropertyValue)->setBackgroundColor(QColor(GetRValue(ProperyValue), GetGValue(ProperyValue), GetBValue(ProperyValue)));
	}
}

void krnl_window::��ӱ߿�ؼ�(ControlInfoWidget* pWindow, unsigned int ProperyValue)
{
	int insertRow = pWindow->ui.ControlTable->rowCount();
	pWindow->ui.ControlTable->insertRow(insertRow);

	pWindow->ui.ControlTable->setItem(insertRow, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("�߿�")));
	QComboBox* pComboBox = new QComboBox();
	pComboBox->setEditable(false);

	QStringList Items = {
		QStringLiteral("�ޱ߿�"),
		QStringLiteral("��ͨ�ɵ��߿�"),
		QStringLiteral("��ͨ�̶��߿�"),
		QStringLiteral("խ����ɵ��߿�"),
		QStringLiteral("խ����̶��߿�"),
		QStringLiteral("����ʽ�ɵ��߿�"),
		QStringLiteral("����ʽ�̶��߿�"),
	};
	pComboBox->addItems(Items);
	pComboBox->setCurrentIndex(ProperyValue);
	pWindow->ui.ControlTable->setCellWidget(insertRow, COLUMN_PropertyValue, pComboBox);
}

void krnl_window::ȡĬ������(QHash<QString, QVariant>& out_data)
{
	out_data[QStringLiteral("version")] = 6;
	out_data[QStringLiteral("�߿�")] = 2;
	out_data[QStringLiteral("��ɫ")] = 0xFF000000;
	out_data[QStringLiteral("��ͼ��ʽ")] = 1;	
	out_data[QStringLiteral("���Ŵ���")] = 0;
	out_data[QStringLiteral("���ư�ť")] = 1;
	out_data[QStringLiteral("��󻯰�ť")] = 0;
	out_data[QStringLiteral("��С����ť")] = 1;
	out_data[QStringLiteral("λ��")] = 1;
	out_data[QStringLiteral("�ɷ��ƶ�")] = 1;
	out_data[QStringLiteral("�س����ƽ���")] = 0;
	out_data[QStringLiteral("Esc���ر�")] = 1;
	out_data[QStringLiteral("F1���򿪰���")] = 0;
	out_data[QStringLiteral("������������ʾ")] = 1;
	out_data[QStringLiteral("�����ƶ�")] = 0;
	out_data[QStringLiteral("����")] = 0;
	out_data[QStringLiteral("������ǰ")] = 0;
	out_data[QStringLiteral("���ֱ���������")] = 0;
}

void krnl_window::��ʾ�ؼ���Ϣ(unsigned int propertyAddr, int propertySize)
{
	QHash<QString, QVariant> map_ControlData;

	qvector<unsigned char> tmpBuf;
	tmpBuf.resize(propertySize);
	get_bytes(&tmpBuf[0], propertySize, propertyAddr);
	unsigned char* lpControlInfo = &tmpBuf[0];

	map_ControlData["�ؼ�ID"]= ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	//�̶���20�����ֽ�,����ʹ��?
	lpControlInfo += 20;

	map_ControlData[QStringLiteral("����")] = QString::fromLocal8Bit(ReadStr(lpControlInfo).c_str());
	lpControlInfo += qstrlen(lpControlInfo) + 1;

	//�����ַ���?
	ReadStr(lpControlInfo);
	lpControlInfo += qstrlen(lpControlInfo) + 1;
	
	//�洢����?
	ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	map_ControlData[QStringLiteral("���")] = ReadInt(lpControlInfo);
	lpControlInfo += 4;

	map_ControlData[QStringLiteral("����")] = ReadInt(lpControlInfo);
	lpControlInfo += 4;

	map_ControlData[QStringLiteral("���")] = ReadInt(lpControlInfo);
	lpControlInfo += 4;

	map_ControlData[QStringLiteral("�߶�")] = ReadInt(lpControlInfo);
	lpControlInfo += 4;

	//ֵΪ0,�����洢LoadCursorA���صľ��ֵ��
	lpControlInfo += 4;

	//���ؼ�ID
	map_ControlData[QStringLiteral("���ؼ�ID")] = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	//�ӿؼ���Ŀ
	unsigned int childControlCount = ReadUInt(lpControlInfo);
	lpControlInfo += 4;
	for (unsigned int n = 0; n < childControlCount; ++n) {
		unsigned int tmpChildControlId = ReadUInt(lpControlInfo);
		lpControlInfo += 4;
	}

	//���ָ���С
	unsigned int CursorSize = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	map_ControlData[QStringLiteral("���ָ��")] = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	if (CursorSize > 4) {
		map_ControlData[QStringLiteral("���ָ������")] = QByteArray((char*)lpControlInfo, CursorSize);
		lpControlInfo += CursorSize - 4;
	}

	map_ControlData[QStringLiteral("���")] = QString::fromLocal8Bit(ReadStr(lpControlInfo).c_str());
	lpControlInfo += qstrlen(lpControlInfo) + 1;

	//δ֪��ֵ
	unsigned int unKnowValueA = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	unsigned int windowFlags = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	map_ControlData[QStringLiteral("����")] = windowFlags & 0x1;
	map_ControlData[QStringLiteral("��ֹ")] = windowFlags & 0x2;



	unsigned int unKnowValueB = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	unsigned int index2 = ReadUInt(lpControlInfo);
	lpControlInfo += 4;
	lpControlInfo += (index2 * 8) + 0x14;

	//����������������������������ʣ�µ�����ΪUnitData����������������������������
	int UnitDataSize = (&tmpBuf[0] - lpControlInfo) + propertySize;

	if (!UnitDataSize) {
		ȡĬ������(map_ControlData);
	}
	else {
		�����л�����(lpControlInfo, map_ControlData);
	}

	ControlInfoWidget* pWindow = GuiParser::g_ControlInfoWindow;
	EAppControl::����ı��ؼ�(pWindow, QStringLiteral("����"), map_ControlData[QStringLiteral("����")].toString());
	EAppControl::����ı��ؼ�(pWindow, QStringLiteral("���"), map_ControlData[QStringLiteral("���")].toString());
	EAppControl::����ı��ؼ�(pWindow, QStringLiteral("����"), map_ControlData[QStringLiteral("����")].toString());
	EAppControl::����ı��ؼ�(pWindow, QStringLiteral("���"), map_ControlData[QStringLiteral("���")].toString());
	EAppControl::����ı��ؼ�(pWindow, QStringLiteral("�߶�"), map_ControlData[QStringLiteral("�߶�")].toString());
	EAppControl::����ı��ؼ�(pWindow, QStringLiteral("���"), map_ControlData[QStringLiteral("���")].toString());
	EAppControl::��Ӳ����ؼ�(pWindow, QStringLiteral("����"), map_ControlData[QStringLiteral("����")].toBool());
	EAppControl::��Ӳ����ؼ�(pWindow, QStringLiteral("��ֹ"), map_ControlData[QStringLiteral("��ֹ")].toBool());
	EAppControl::������ؼ�(pWindow, map_ControlData[QStringLiteral("���ָ��")].toUInt());
	EAppControl::����ı��ؼ�(pWindow, QStringLiteral("����"), map_ControlData[QStringLiteral("����")].toString());
	krnl_window::��ӱ߿�ؼ�(pWindow, map_ControlData[QStringLiteral("�߿�")].toUInt());
	krnl_window::��ӵ�ɫ�ؼ�(pWindow, map_ControlData[QStringLiteral("��ɫ")].toUInt());
	EAppControl::���ͼƬ�ؼ�(pWindow, QStringLiteral("��ͼ"), map_ControlData[QStringLiteral("��ͼ")].toByteArray());
	return;
}


qstring krnl_window::ȡ�¼�����(int eventIndex)
{
	qstring ret;
	switch (eventIndex)
	{
	case -1:
		ret = "������������";
		break;
	case -2:
		ret = "���������ſ�";
		break;
	case -3:
		ret = "��˫��";
		break;
	case -4:
		ret = "����Ҽ�������";
		break;
	case -5:
		ret = "����Ҽ����ſ�";
		break;
	case -6:
		ret = "���λ�ñ��ƶ�";
		break;
	case -7:
		ret = "��ý���";
		break;
	case -8:
		ret = "ʧȥ����";
		break;
	case -9:
		ret = "����ĳ��";
		break;
	case -10:
		ret = "�ſ�ĳ��";
		break;
	case -11:
		ret = "�ַ�����";
		break;
	case -12:
		ret = "���ֱ�����";
		break;
	case 0:
		ret = "�������";
		break;
	case 1:
		ret = "�ɷ񱻹ر�";
		break;
	case 2:
		ret = "��������";
		break;
	case 3:
		ret = "λ�ñ��ı�";
		break;
	case 4:
		ret = "�ߴ类�ı�";
		break;
	case 5:
		ret = "������";
		break;
	case 6:
		ret = "��ȡ������";
		break;
	case 7:
		ret = "����";
		break;
	case 8:
		ret = "�״μ���";
		break;
	case 9:
		ret = "�����¼�";
		break;
	case 10:
		ret = "����ʾ";
		break;
	case 11:
		ret = "������";
		break;
	case 12:
		ret = "���ڿɷ񱻹ر�";
		break;
	default:
		ret = "δ֪�¼�";
		break;
	}
	return ret;
}