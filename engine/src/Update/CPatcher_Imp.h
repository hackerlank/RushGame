#pragma once


namespace sqr
{
	class IAsyncUpdateHandler;

	class CPatcher_Imp
	{
	public:
		CPatcher_Imp();
		~CPatcher_Imp();

		int YbtxPatchExecute(IAsyncUpdateHandler* pAsyncUpdateHandler, const char *PatchFile); // �µ���������
		int decompress (void *in , void *out);

	private:
		// ���°��ļ�ͷ
		struct UpdateFileHead
		{
			DWORD flag; //��־ 'DTUD' ���DTBZ��ʾ��bzip2
			DWORD ver;  //�汾��
			DWORD decompress_len; //�������ݽ�ѹ��ĳ���
		};

		// ��Դ���½ڵ�(ĳһ��ADDF��MODI)
		struct UpdateSec
		{
			DWORD flag;
			DWORD file_len;
			DWORD uDiffLen;     // Diff����, ADDFģʽ��Ϊ0
			DWORD pathname_len;
		};

		// MODI�ڲ���ڵ�
		struct ModiChipNode
		{
			DWORD uSkip;
			DWORD uSize;
		};
	};

}
