#pragma once
#include "CTrMirrorBuffer.h"

namespace sqr
{
	/*
		����Ƴ����Ĳ�����ʱ�����ǰ����е��Ĳ���������һ��triple buffer��ͬʱ��
		bufferҲ�����Ĳ������tree��ΪʲôҪ��ô��������Ϊ���ڳ����Ĳ�����˵��
		�Ĳ���������Ĳ�����һ��ģ������������buffer�ֱ������ô������������

		1���Ĳ�������
		2���Ĳ������󴴽�  

		��Ϊһ��buffer��Ӧ����һ��patch������������ϵ��Ĳ���buffer��patch��Ȼ������
		�Ĳ��������patch����������Ĵ���û��ʲô���⣬�����������

		1���Ĳ�������ɾ��
		2���Ĳ���ɾ��

		��ô�ͻ������patch��ʱ�����Ĳ���ɾ����Ȼ�����Ĳ�������ɾ��������ܻ�������
	*/

	class CMirrorBuffer;

	class CTrMirrorQuadBuffer : public CTrMirrorBuffer
	{
	public:
		static CTrMirrorBuffer* CreateBufferMgr(uint32 uDataNum);
		void Release();

	private:
		CTrMirrorQuadBuffer(uint32 uDataSize, uint32 uDataNum);
		~CTrMirrorQuadBuffer();
	};

}
