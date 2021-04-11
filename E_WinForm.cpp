#include "E_WinForm.h"
#include "public.h"

void Unserialize_WinFormData(unsigned char* lpControlData, WinFrom_UnitData* out_Data)
{
	out_Data->version = CDR_ReadUInt(lpControlData);

	if (out_Data->version <= 6) {
		out_Data->�߿� = CDR_ReadUInt(lpControlData);
		out_Data->��ͼ��ʽ = CDR_ReadUInt(lpControlData);
		out_Data->��ɫ = CDR_ReadUInt(lpControlData);

		out_Data->��󻯰�ť = CDR_ReadUInt(lpControlData);
		out_Data->��С����ť = CDR_ReadUInt(lpControlData);
		out_Data->���ư�ť = CDR_ReadUInt(lpControlData);
		out_Data->λ�� = CDR_ReadUInt(lpControlData);
		out_Data->�ɷ��ƶ� = CDR_ReadUInt(lpControlData);
		out_Data->���Ŵ��� = CDR_ReadUInt(lpControlData);
		out_Data->�س����ƽ��� = CDR_ReadUInt(lpControlData);
		out_Data->Esc���ر� = CDR_ReadUInt(lpControlData);
		out_Data->F1���򿪰��� = CDR_ReadUInt(lpControlData);
		out_Data->�������ֵ = CDR_ReadUInt(lpControlData);

		if (out_Data->version >= 2) {
			out_Data->������������ʾ = CDR_ReadUInt(lpControlData);
		}
		if (out_Data->version >= 3) {
			out_Data->�����ƶ� = CDR_ReadUInt(lpControlData);
			out_Data->���� = CDR_ReadUInt(lpControlData);
		}
		if (out_Data->version >= 4) {
			out_Data->������ǰ = CDR_ReadUInt(lpControlData);
		}
		if (out_Data->version >= 5) {
			out_Data->���ֱ��������� = CDR_ReadUInt(lpControlData);
		}
		if (out_Data->version == 6) {
			out_Data->�������� = CDR_ReadCString(lpControlData);
		}

		out_Data->��ͼ = CDR_ReadCFreqMem(lpControlData);
		out_Data->ͼ�� = CDR_ReadCFreqMem(lpControlData);
		out_Data->�������� = CDR_ReadCFreqMem(lpControlData);

		out_Data->���� = CDR_ReadCString(lpControlData);
		out_Data->�����ļ��� = CDR_ReadCString(lpControlData);
	}
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

qstring ȡ��������_���ָ��(unsigned int index)
{
	qstring ret;
	switch (index)
	{
	case 0x0:
		ret = "Ĭ����";
		break;
	case 0x00007F00:
		ret = "��׼��ͷ��";
		break;
	case 0x00007F03:
		ret = "ʮ����";
		break;
	case 0x00007F01:
		ret = "�ı��༭��";
		break;
	case 0x00007F02:
		ret = "ɳ©��";
		break;
	case 0x00007F8B:
		ret = "��ͷ���ʺ���";
		break;
	case 0x00007F8A:
		ret = "��ͷ��ɳ©��";
		break;
	case 0x00007F88:
		ret = "��ֹ����";
		break;
	case 0x00007F86:
		ret = "�����ͷ��";
		break;
	case 0x00007F83:
		ret = "��<->����ͷ��";
		break;
	case 0x00007F85:
		ret = "��<->�ϼ�ͷ��";
		break;
	case 0x00007F82:
		ret = "��<->����ͷ��";
		break;
	case 0x00007F84:
		ret = "��<->����ͷ��";
		break;
	case 0x00007F04:
		ret = "���ϼ�ͷ��";
		break;
	case 0x00007F89:
		ret = "����";
		break;
	case 0xFFFFFFFF:
		ret = "�Զ�����";
		break;
	default:
		ret = "��������";
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

void ȡ��������_Ĭ��(WinFrom_UnitData* out_Data)
{
	out_Data->version = 6;
	out_Data->�߿� = 2;
	out_Data->��ɫ = 0xFF000000;
	out_Data->��ͼ��ʽ = 1;
	out_Data->���Ŵ��� = 0;
	out_Data->���ư�ť = 1;
	out_Data->��󻯰�ť = 0;
	out_Data->��С����ť = 1;
	out_Data->λ�� = 1;
	out_Data->�ɷ��ƶ� = 1;
	out_Data->�س����ƽ��� = 0;
	out_Data->Esc���ر� = 1;
	out_Data->F1���򿪰��� = 0;
	out_Data->������������ʾ = 1;
	out_Data->�����ƶ� = 0;
	out_Data->���� = 0;
	out_Data->������ǰ = 0;
	out_Data->���ֱ��������� = 0;
}

qstring ȡ�����¼�����(int eventIndex)
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