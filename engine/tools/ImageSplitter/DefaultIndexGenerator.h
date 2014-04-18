#pragma once
#include "IIndexGenerator.h"

// ---- �ඨ�� --------------------------------------------------------------------------------
		
	/*
	
	���ɹ���:
	
	m = m_uWidth - 1 
	n = m_uHeiht - 1 
	
	M_0_n M_1_n M_2_n  . . .  M_m_n
 	.	 .	   .	  . . .  .
	.	 .	   .	  . . .  .
	.	 .	   .	  . . .  .
	M_0_2 M_1_2 M_2_2  . . .  M_m_2
	M_0_1 M_1_1 M_2_1  . . .  M_m_1
	M_0_0 M_1_0 M_2_0  . . .  M_m_0
	
	*/

	class CDefaultIndexGenerator : public IIndexGenerator
	{
		uint32			m_uWidth;
		uint32			m_uHeight;
		uint32			m_uIndexCount;
		uint32			m_uCurrIndex;

	public:
		CDefaultIndexGenerator();

		// ---- ���������Ⱥ͸߶ȣ��Դ��������з�������Ŀ
		virtual void	SetRaginInfo( uint32 uWidth, uint32 uHeight );

		// ---- �����и������η����и������Լ��зֽ��ͼƬ���� 
		//		��������������������ϣ��򷵻�false
		virtual bool	GetNextIndex( IndexStruct *sIndex );

		// ---- �ͷ�������
		virtual void	Release();
	};