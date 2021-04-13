#include "ControlInfoWidget.h"
#include "GuiParser.h"
#include <bytes.hpp>
#include "public.h"
#include "krnl_window.h"
#include "krnl_Label.h"
#include "PropertyDelegate.h"


#define COLUMN_PropertyName  0
#define COLUMN_PropertyValue 1


QString Control_GetBoolStr(unsigned int value)
{
	QString ret = QStringLiteral("��");
	if (!value) {
		ret = QStringLiteral("��");
	}
	return ret;
}

void ControlInfoWidget::InitKernelControl_Window(unsigned int propertyAddr, int propertySize)
{	
	qvector<unsigned char> tmpBuf;
	tmpBuf.resize(propertySize);
	get_bytes(&tmpBuf[0], propertySize, propertyAddr);
	unsigned char* lpControlInfo = &tmpBuf[0];

	ui.ControlTable->setRowCount(33);
	//������ʵ��û�����ֵ�
	//qstring controlName;
	//controlName.sprnt("����_%08a", pControl->m_controlId);
	ui.ControlTable->setItem(0, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	ui.ControlTable->setItem(0, COLUMN_PropertyValue, new QTableWidgetItem(ui_LineEditor_Disabled));

	{
		uint32 dwControlTypeId = ReadUInt(lpControlInfo);
		lpControlInfo += 4;
		//�̶���20�����ֽ�,����ʹ��?
		lpControlInfo += 20;
		//����
		ReadStr(lpControlInfo);
		lpControlInfo += qstrlen(lpControlInfo) + 1;
		//�����ַ���2?
		ReadStr(lpControlInfo);
		lpControlInfo += qstrlen(lpControlInfo) + 1;
		//�洢����?
		ReadUInt(lpControlInfo);
		lpControlInfo += 4;
	}

	int left = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(1, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(1, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(left)));


	int top = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(2, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	ui.ControlTable->setItem(2, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(top)));

	int width = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(3, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(3, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(width)));

	int height = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(4, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("�߶�")));
	ui.ControlTable->setItem(4, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(height)));

	//δֵ֪
	lpControlInfo += 8;

	//һ������?
	unsigned int index1 = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	//���ָ��
	unsigned int cursorFlag = ReadUInt(lpControlInfo + 4);

	ui.ControlTable->setItem(8, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���ָ��")));
	ui.ControlTable->setItem(8, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(ȡ��������_���ָ��(cursorFlag).c_str())));

	//����ƫ��?
	unsigned int offset2 = ReadUInt(lpControlInfo + (index1 * 4));
	lpControlInfo += index1 * 4;
	lpControlInfo += offset2 + 4;
	
	//���
	qstring m_tag = ReadStr(lpControlInfo);
	lpControlInfo += qstrlen(lpControlInfo) + 1;
	ui.ControlTable->setItem(5, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(5, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(m_tag.c_str())));

	unsigned int unKnowValueA = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	unsigned int flags = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	ui.ControlTable->setItem(6, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	if (flags & 0x1) {
		ui.ControlTable->setItem(6, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}
	else {
		ui.ControlTable->setItem(6, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}

	ui.ControlTable->setItem(7, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("��ֹ")));
	if (flags & 0x2) {
		ui.ControlTable->setItem(7, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}
	else {
		ui.ControlTable->setItem(7, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}

	unsigned int unKnowValueB = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	unsigned int index2 = ReadUInt(lpControlInfo);
	lpControlInfo += 4;
	lpControlInfo += (index2 * 8) + 0x14;

	//����������������������������ʣ�µ�����ΪUnitData����������������������������
	int UnitDataSize = (&tmpBuf[0] - lpControlInfo) + propertySize;

	WinFrom_UnitData WinData = {};
	if (!UnitDataSize) {
		ȡ��������_Ĭ��(&WinData);
	}
	else {
		Unserialize_WinFormData(lpControlInfo, &WinData);
	}
	
	ui.ControlTable->setItem(9, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	if (WinData.����.size()) {
		ui.ControlTable->setItem(9, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(WinData.����.c_str())));
	}

	ui.ControlTable->setItem(10, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("�߿�")));
	ui.ControlTable->setItem(10, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(ȡ��������_�߿�(WinData.�߿�).c_str())));

	ui.ControlTable->setItem(11, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("��ɫ")));
	if (WinData.��ɫ == 0xFF000000) {
		uint32 color = GetSysColor(COLOR_BTNFACE);
		ui.ControlTable->setItem(11, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("Ĭ�ϵ�ɫ"), ui_ColorDialog));
		ui.ControlTable->item(11, COLUMN_PropertyValue)->setBackgroundColor(QColor(GetRValue(color), GetGValue(color), GetBValue(color)));
	}
	else {
		ui.ControlTable->setItem(11, COLUMN_PropertyValue, new QTableWidgetItem("", ui_ColorDialog));
		ui.ControlTable->item(11, COLUMN_PropertyValue)->setBackgroundColor(QColor(GetRValue(WinData.��ɫ), GetGValue(WinData.��ɫ), GetBValue(WinData.��ɫ)));
	}

	ui.ControlTable->setItem(12, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("��ͼ")));
	if (WinData.��ͼ.size()) {
		ui.ControlTable->setItem(12, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("������")));
		//������ͼ��...
	}

	ui.ControlTable->setItem(13, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("    ��ͼ��ʽ")));
	ui.ControlTable->setItem(13, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(ȡ��������_��ͼ��ʽ(WinData.��ͼ��ʽ).c_str())));

	ui.ControlTable->setItem(14, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("��������")));
	if (WinData.��������.size()) {
		ui.ControlTable->setItem(14, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("������")));
	}

	ui.ControlTable->setItem(15, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("    ���Ŵ���")));
	ui.ControlTable->setItem(15, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(ȡ��������_���Ŵ���(WinData.���Ŵ���).c_str())));

	ui.ControlTable->setItem(16, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���ư�ť")));
	ui.ControlTable->setItem(16, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.���ư�ť)));

	ui.ControlTable->setItem(17, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("    ��󻯰�ť")));
	ui.ControlTable->setItem(17, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.��󻯰�ť)));

	ui.ControlTable->setItem(18, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("    ��С����ť")));
	ui.ControlTable->setItem(18, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.��С����ť)));

	ui.ControlTable->setItem(19, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("λ��")));
	ui.ControlTable->setItem(19, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(ȡ��������_λ��(WinData.λ��).c_str())));
	
	ui.ControlTable->setItem(20, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("�ɷ��ƶ�")));
	ui.ControlTable->setItem(20, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.�ɷ��ƶ�)));

	ui.ControlTable->setItem(21, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("ͼ��")));
	if (WinData.ͼ��.size()) {
		ui.ControlTable->setItem(21, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("������")));
	}

	ui.ControlTable->setItem(22, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("�س����ƽ���")));
	ui.ControlTable->setItem(22, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.�س����ƽ���)));

	ui.ControlTable->setItem(23, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("Esc���ر�")));
	ui.ControlTable->setItem(23, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.Esc���ر�)));

	ui.ControlTable->setItem(24, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("F1���򿪰���")));
	ui.ControlTable->setItem(24, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.F1���򿪰���)));

	ui.ControlTable->setItem(25, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("    �����ļ���")));
	ui.ControlTable->setItem(25, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(WinData.�����ļ���.c_str())));

	ui.ControlTable->setItem(26, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("    �������ֵ")));
	ui.ControlTable->setItem(26, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(WinData.�������ֵ)));

	ui.ControlTable->setItem(27, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("������������ʾ")));
	ui.ControlTable->setItem(27, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.������������ʾ)));
	
	ui.ControlTable->setItem(28, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("�����ƶ�")));
	ui.ControlTable->setItem(28, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.�����ƶ�)));
	
	ui.ControlTable->setItem(29, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	ui.ControlTable->setItem(29, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(ȡ��������_����(WinData.����).c_str())));

	ui.ControlTable->setItem(30, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("������ǰ")));
	ui.ControlTable->setItem(30, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.������ǰ)));

	ui.ControlTable->setItem(31, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���ֱ���������")));
	ui.ControlTable->setItem(31, COLUMN_PropertyValue, new QTableWidgetItem(Control_GetBoolStr(WinData.���ֱ���������)));

	ui.ControlTable->setItem(32, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("��������")));
	ui.ControlTable->setItem(32, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(WinData.��������.c_str())));
	return;
}

void ControlInfoWidget::InitKernelControl_Label(unsigned int propertyAddr, int propertySize)
{
	qvector<unsigned char> tmpBuf;
	tmpBuf.resize(propertySize);
	get_bytes(&tmpBuf[0], propertySize, propertyAddr);
	unsigned char* lpControlInfo = &tmpBuf[0];

	ui.ControlTable->setRowCount(33);

	ui.ControlTable->setItem(0, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	//ui.ControlTable->setItem(0, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(pControl->m_controlName.c_str()), ui_LineEditor_ReadOnly));

	{
		uint32 dwControlTypeId = ReadUInt(lpControlInfo);
		lpControlInfo += 4;
		//�̶���20�����ֽ�,����ʹ��?
		lpControlInfo += 20;
		//����
		qstring ���� = ReadStr(lpControlInfo);
		lpControlInfo += qstrlen(lpControlInfo) + 1;
		//�����ַ���2?
		ReadStr(lpControlInfo);
		lpControlInfo += qstrlen(lpControlInfo) + 1;
		//�洢����?
		ReadUInt(lpControlInfo);
		lpControlInfo += 4;
	}

	int left = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(1, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(1, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(left)));

	int top = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(2, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	ui.ControlTable->setItem(2, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(top)));

	int width = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(3, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(3, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(width)));

	int height = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(4, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("�߶�")));
	ui.ControlTable->setItem(4, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(height)));

	//δֵ֪
	lpControlInfo += 8;

	//һ������?
	unsigned int index1 = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	//���ָ��
	unsigned int cursorFlag = ReadUInt(lpControlInfo + 4);
	ui.ControlTable->setItem(8, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���ָ��")));
	ui.ControlTable->setItem(8, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(ȡ��������_���ָ��(cursorFlag).c_str())));

	//����ƫ��?
	unsigned int offset2 = ReadUInt(lpControlInfo + (index1 * 4));
	lpControlInfo += index1 * 4;
	lpControlInfo += offset2 + 4;

	//���
	qstring m_tag = ReadStr(lpControlInfo);
	lpControlInfo += qstrlen(lpControlInfo) + 1;
	ui.ControlTable->setItem(5, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(5, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(m_tag.c_str())));

	unsigned int unKnowValueA = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	unsigned int flags = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	ui.ControlTable->setItem(6, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	if (flags & 0x1) {
		ui.ControlTable->setItem(6, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}
	else {
		ui.ControlTable->setItem(6, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}

	ui.ControlTable->setItem(7, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("��ֹ")));
	if (flags & 0x2) {
		ui.ControlTable->setItem(7, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}
	else {
		ui.ControlTable->setItem(7, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}

	unsigned int unKnowValueB = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	unsigned int index2 = ReadUInt(lpControlInfo);
	lpControlInfo += 4;
	lpControlInfo += (index2 * 8) + 0x14;

	//����������������������������ʣ�µ�����ΪUnitData����������������������������
	int UnitDataSize = (&tmpBuf[0] - lpControlInfo) + propertySize;

	Label_UnitData LabelData = {};
	if (!UnitDataSize) {
		
	}
	else {
		Unserialize_LabelData(lpControlInfo, &LabelData);
	}

	return;
}



ControlInfoWidget::ControlInfoWidget()
{
	ui.setupUi(this);

	ui.ControlTable->setColumnCount(2);
	ui.ControlTable->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.ControlTable->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui.ControlTable->setItemDelegateForColumn(COLUMN_PropertyValue, new PropertyDelegate(ui.ControlTable));

	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, [&](QTreeWidgetItem* item, int column) {
		on_controlClicked(item, column);
	});
}

ControlInfoWidget::~ControlInfoWidget()
{

}

void ControlInfoWidget::on_controlClicked(QTreeWidgetItem* item, int column)
{
	ui.ControlTable->setRowCount(0);
	ui.ControlTable->clearContents();

	QTreeWidgetItem* parentItem = item->parent();
	if (!item->parent()) {
		//����������ϲ�ڵ�
		ui.groupBox->setTitle(QStringLiteral("����"));
	}
	else {
		unsigned int controlId = item->data(0, Qt::UserRole).toUInt();

		GuiParser::mid_ControlInfo currentControl;
		if (GuiParser::GetControlInfo(controlId, currentControl)) {

			ui.groupBox->setTitle(QString::fromLocal8Bit(currentControl.m_controlTypeName.c_str()));
			ControlType_t controlType = GuiParser::GetControlType(currentControl.m_controlTypeId);
			ea_t propertyAddr = currentControl.m_propertyAddr;
			int propertySize = currentControl.m_propertySize;
			switch (controlType)
			{
			case krnl_window:
				InitKernelControl_Window(propertyAddr, propertySize);
				break;
			case krnl_Label:
				InitKernelControl_Label(propertyAddr, propertySize);
				break;
			default:
				break;
			}
		}

	}

}