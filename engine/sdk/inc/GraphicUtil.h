#pragma once

#include "CRenderStyle.h"
namespace sqr
{
	//Ĭ����Ⱦ����1
	struct DEFAULT1RS : public STATIC_RS
	{
		DEFAULT1RS();
	};

	//Ĭ�ϵ�ȡ����Ⱦ����
	struct PONITSAMPLERS : public DEFAULT1RS
	{
		PONITSAMPLERS();
	};

	//������Ⱦ״̬
	struct FONTRS : public STATIC_RS
	{
		FONTRS();
	};

	//GUI״̬
	struct GUIRS : public STATIC_RS
	{
		GUIRS();
	};

	//������Ⱦ״̬
	struct GUIMIRRORRS : public STATIC_RS
	{
		GUIMIRRORRS();
	};

	//������Ⱦ״̬
	struct GUI_COLORADDRS : public STATIC_RS
	{
		GUI_COLORADDRS();
	};

	//�����ͼ��Ⱦ״̬
	struct MAP1LRS : public STATIC_RS
	{
		MAP1LRS();
	};

	//2���ͼ��Ⱦ״̬
	struct MAP2LRS : public STATIC_RS
	{
		MAP2LRS();
	};

	//4��
	struct MAP4LRS : public STATIC_RS
	{
		MAP4LRS();
	};

	//��Ӱ��Ⱦ״̬
	struct SHADOWRS : public STATIC_RS
	{
		SHADOWRS();
	};

	//��͸�����Ҷ�
	struct ALPHA2GRAYRS : public STATIC_RS
	{
		ALPHA2GRAYRS();
	};

	//ColorKey
	struct NONETEXTALPHA : public GUIRS
	{
		NONETEXTALPHA();
	};

	//ColorKey
	struct NONETEXTALPHALINEAR : public NONETEXTALPHA
	{
		NONETEXTALPHALINEAR();
	};

	//������Ⱦ״̬
	struct GUISHADOWRS : public GUIRS
	{
		GUISHADOWRS();
	};


	//GUIר��
	struct GUIALPHA : public GUIRS
	{
		GUIALPHA();
	};

	struct GUIBIGMAP: public GUIRS
	{
		GUIBIGMAP();
	};

	struct EMTRS: public STATIC_RS
	{
		EMTRS();
	};

	struct LINERS : public STATIC_RS
	{
		LINERS();
	};

	struct GUIALPHABLEND : public GUIRS
	{
		GUIALPHABLEND();
	};

	//���建��������
	struct TRIAGLE_INDEX
	{
		TRIAGLE_INDEX();
		GVector<WORD>		m_IndexBuf;

		operator const WORD*()
		{
			return &( m_IndexBuf[0] );
		}
	};
}
