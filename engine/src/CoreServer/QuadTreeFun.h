#pragma once 

#include "CPos.h"
#include "CRectangle.h"


namespace sqr
{
	//�жϵ��Ƿ���quadnode��Χ����
	/*
		��Ϊ���ڴ��ڱ߽�Ķ��������������⴦������һ�����������һ���ڵ���
		��ô��������һ����(left, right] (top, bottom]֮��
	*/
	inline bool BeInQuadNodeBox(const CFPos& pos, const CFRect& rcBox)
	{
		
		if (pos.x > rcBox.left && pos.x <= rcBox.right &&
			pos.y < rcBox.top && pos.y >= rcBox.bottom)
		{
			return true;
		}
		
		
		return false;
	}

	//�ж����������Ƿ���ȫ�ص�
	inline bool BeCoincide(const CFRect& rcRect, const CFRect& rcBox)
	{
		if (rcRect.left >= rcBox.left && rcRect.right <= rcBox.right &&
				rcRect.top <= rcBox.top && rcRect.bottom >= rcBox.bottom)
		{
			return true;
		}
		return false;
	}

	//�жϾ����Ƿ���quadnode��Χ����
	inline bool BeInQuadNodeBox(const CFRect& rcRect, const CFRect& rcBox)
	{
		if (rcRect.left > rcBox.left && rcRect.right <= rcBox.right &&
			rcRect.top < rcBox.top && rcRect.bottom >= rcBox.bottom)
		{
			return true;
		}

		return false;
	}

	//�жϾ����Ƿ���quadnode��Χ�ص�
	inline bool BeOverlappedQuadNode(const CFRect& rcRect, const CFRect& rcBox)
	{
		if (BeCoincide(rcRect, rcBox))
		{
			return true;
		}

		if (BeCoincide(rcBox, rcRect))
		{
			return true;
		}

		//�����������������������û���κι�ϵ������ǰ���������жϵ�����£�
		if (rcRect.right <= rcBox.left || rcRect.left > rcBox.right ||
			rcRect.bottom >= rcBox.top || rcRect.top < rcBox.bottom)
		{
			return false;
		}

		return true;
	}

	//�õ����Ĳ�������
	CFRect GetQuadRect(uint32 uWidth, uint32 uHeight);


	//�õ���posΪ���ģ�fRadiusΪ�߰뾶�ľ�������
	inline void GetRadiusRect(CFRect& rect, const CFPos& pos, float fRadius, const CFRect& rcMaxBox)
	{
		float fLeft = pos.x - fRadius;
		rect.left = fLeft > rcMaxBox.left ? fLeft : rcMaxBox.left;

		float fRight = pos.x + fRadius;
		rect.right = fRight < rcMaxBox.right ? fRight : rcMaxBox.right;

		float fTop = pos.y + fRadius;
		rect.top = fTop < rcMaxBox.top ? fTop : rcMaxBox.top;

		float fBottom = pos.y - fRadius;
		rect.bottom = fBottom > rcMaxBox.bottom ? fBottom : rcMaxBox.bottom;

	}
}

