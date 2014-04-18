#pragma once
#include "CEventOwner.h"
#include "TSingleton.h"

namespace sqr
{
	class CLogicBrush;

	class CToolVertex: public Singleton<CToolVertex>
	{
	private:
		HWND m_hVertexView;

		CLogicBrush *m_pLogicBrush;
		DWORD	    m_dwColor;
		float       m_fVertexStrength;

		void OperatorVertexColor();
	public:

		CToolVertex();
		~CToolVertex();
		void OnEvent(const CMsg& msg);
		void Render();
		void Intialize(HWND hVertexView);

		void IncBrushSizeInX();
		void DecBrushSizeInX();

		void IncBrushSizeInZ();
		void DecBrushSizeInZ();

		void PickColor();
		void RevertColor();
		void DeleteVertexColor();

		void SetGridsEffectObject(const bool bEffectObject);
		void SetAllObjectsIsEffectByVertexColor(const bool bEffectObject);

		///bDiffuse = true ��ԭ���еĶ���diffuse��ɫ����ʼ��ɫ, bDiffuse = false��ԭ���еĶ���specular��ɫ����ʼ��ɫ
		void RevertVertexColorToIntial(bool bDiffuse);

		///�����ж�����ͬһɫ��index = 0 ��ʾ��diffuse�� index = 2��ʾ��specular
		void SetAllVertexToColor(DWORD color, int index = 0);

		inline void SetVertexColor(const DWORD dw)
		{
			m_dwColor = dw;
		}

		inline DWORD GetVertexColor() const
		{
			return m_dwColor;
		}

		inline void SetVertexStrength(const float f)
		{
			m_fVertexStrength = f;
		}

		inline float GetVertexStrength() const
		{
			return m_fVertexStrength;
		}
	};
}