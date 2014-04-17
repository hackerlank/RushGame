#pragma once
#include "CQuickRand.h"
#include "CMath.h"
#include "CMathMallocObject.h"

namespace sqr
{
	/// @brief ��ɫ�ṹ ARGB ��ʽ
	struct CColor : public CMathMallocObject
	{
		union
		{
			uint32	dwColor;	///< ARGB ��ʽ��ɫ
			uint8	byColor[4];	///< ��ǰ������BGRAȡ�ø���ɫ����
			struct
			{
				uint8 B;
				uint8 G;
				uint8 R;
				uint8 A;
			};
		};

		CColor();
		CColor( uint32 c );
		CColor( uint8 a, uint8 r, uint8 g, uint8 b );

		operator uint32() const;

		const CColor& operator= ( const CColor& arg );

		const CColor operator+ ( const CColor& arg ) const;
		const CColor operator- ( const CColor& arg ) const;
		const CColor operator* ( float arg ) const;
		const CColor operator/ (float arg) const;
		const CColor operator* (const CColor& rhs) const;

		CColor& operator+=(const CColor& rhs);
		CColor& operator-=(const CColor& rhs);
		CColor& operator*=(const CColor& rhs);
		CColor& operator*=(float ratio);
		CColor& operator/=(float ratio);


		CColor Modulate(const CColor& rhs);

		/// @brief ����ɫת��ΪABGR��ʽ
		uint32 ToABGR()const;
		/// @brief ����ɫת��ΪARGB��ʽ
		uint32 ToRGBA()const;
		/// @brief ����ɫת��ΪARGB��ʽ
		uint32 ToARGB()const;

		/// @brief ����ɫ�ķ�ɫ
		CColor Reverse()const;
		/// תΪ�Ҷ�ֵ
		uint8 ToGray() const;

	public:
		/// @brief ���������ɫ
		static CColor Rand();
		/// @brief �����������ɫ
		static CColor RandDark();
		/// �������ֵ(ÿ�����������ֵ)
		static const CColor Max(const CColor& color1, const CColor& color2);
		/// ������Сֵ(ÿ����������Сֵ)
		static const CColor Min(const CColor& color1, const CColor& color2);
	public:
		/// @brief ���õ���ɫֵ������ɫ���ο� http://www.ttpw.com/rgb.htm
#ifdef _WIN32
#pragma region ConstantColors
#endif
		static const uint32 LightPink			= 0xFFFFB6C1;	///< ǳ�ۺ�
		static const uint32 Pink				= 0xFFFFC0CB;	///< �ۺ�
		static const uint32 Crimson				= 0xFFDC143C;	///< ���/�ɺ�
		static const uint32 LavenderBlush		= 0xFFFFF0F5;	///< ���Ϻ�
		static const uint32 PaleVioletRed		= 0xFFDB7093;	///< ����������
		static const uint32 HotPink				= 0xFFFF69B4;	///< ����ķۺ�
		static const uint32 DeepPink			= 0xFFFF1493;	///< ��ۺ�
		static const uint32 MediumVioletRed 	= 0xFFC71585;	///< ����������
		static const uint32 Orchid				= 0xFFDA70D6;	///< ����ɫ/������
		static const uint32 Thistle				= 0xFFD8BFD8;	///< ��ɫ
		static const uint32 Plum				= 0xFFDDA0DD;	///< ����ɫ/������
		static const uint32 Violet				= 0xFFEE82EE;	///< ������
		static const uint32 Magenta				= 0xFFFF00FF;	///< ���/õ���
		static const uint32 Fuchsia				= 0xFFFF00FF;	///< �Ϻ�/��������
		static const uint32 DarkMagenta			= 0xFF8B008B;	///< �����
		static const uint32 Purple				= 0xFF800080;	///< ��ɫ
		static const uint32 MediumOrchid		= 0xFFBA55D3;	///< ��������
		static const uint32 DarkViolet			= 0xFF9400D3;	///< ��������
		static const uint32 DarkOrchid			= 0xFF9932CC;	///< ��������
		static const uint32 Indigo				= 0xFF4B0082;	///< ����/����ɫ
		static const uint32 BlueViolet			= 0xFF8A2BE2;	///< ��������
		static const uint32 MediumPurple		= 0xFF9370DB;	///< ����ɫ
		static const uint32 MediumSlateBlue 	= 0xFF7B68EE;	///< �а���ɫ/�а�����
		static const uint32 SlateBlue			= 0xFF6A5ACD;	///< ʯ��ɫ/������
		static const uint32 DarkSlateBlue		= 0xFF483D8B;	///< ������ɫ/��������
		static const uint32 Lavender			= 0xFFE6E6FA;	///< ����ɫ/Ѭ�²ݵ���
		static const uint32 GhostWhite			= 0xFFF8F8FF;	///< �����
		static const uint32 Blue				= 0xFF0000FF;	///< ����
		static const uint32 MediumBlue			= 0xFF0000CD;	///< ����ɫ
		static const uint32 MidnightBlue		= 0xFF191970;	///< ��ҹ��
		static const uint32 DarkBlue			= 0xFF00008B;	///< ����ɫ
		static const uint32 Navy				= 0xFF000080;	///< ������
		static const uint32 RoyalBlue			= 0xFF4169E1;	///< �ʼ���/����
		static const uint32 CornflowerBlue		= 0xFF6495ED;	///< ʸ������
		static const uint32 LightSteelBlue		= 0xFFB0C4DE;	///< ������
		static const uint32 LightSlateGray		= 0xFF778899;	///< ������/��ʯ���
		static const uint32 SlateGray			= 0xFF708090;	///< ��ʯɫ/ʯ���
		static const uint32 DodgerBlue			= 0xFF1E90FF;	///< ����ɫ/������
		static const uint32 AliceBlue			= 0xFFF0F8FF;	///< ����˿��
		static const uint32 SteelBlue			= 0xFF4682B4;	///< ����/����
		static const uint32 LightSkyBlue		= 0xFF87CEFA;	///< ������ɫ
		static const uint32 SkyBlue				= 0xFF87CEEB;	///< ����ɫ
		static const uint32 DeepSkyBlue			= 0xFF00BFFF;	///< ������
		static const uint32 LightBlue			= 0xFFADD8E6;	///< ����
		static const uint32 PowderBlue			= 0xFFB0E0E6;	///< ����ɫ/��ҩ��
		static const uint32 CadetBlue			= 0xFF5F9EA0;	///< ����ɫ/������
		static const uint32 Azure				= 0xFFF0FFFF;	///< ε��ɫ
		static const uint32 LightCyan			= 0xFFE0FFFF;	///< ����ɫ
		static const uint32 PaleTurquoise		= 0xFFAFEEEE;	///< ���̱�ʯ
		static const uint32 Cyan				= 0xFF00FFFF;	///< ��ɫ
		static const uint32 Aqua				= 0xFF00FFFF;	///< ǳ��ɫ/ˮɫ
		static const uint32 DarkTurquoise		= 0xFF00CED1;	///< ���̱�ʯ
		static const uint32 DarkSlateGray		= 0xFF2F4F4F;	///< ���߻�ɫ/��ʯ���
		static const uint32 DarkCyan			= 0xFF008B8B;	///< ����ɫ
		static const uint32 Teal				= 0xFF008080;	///< ˮѼɫ
		static const uint32 MediumTurquoise 	= 0xFF48D1CC;	///< ���̱�ʯ
		static const uint32 LightSeaGreen		= 0xFF20B2AA;	///< ǳ������
		static const uint32 Turquoise			= 0xFF40E0D0;	///< �̱�ʯ
		static const uint32 Aquamarine			= 0xFF7FFFD4;	///< ��ʯ����
		static const uint32 MediumAquamarine 	= 0xFF66CDAA;	///< �б�ʯ����
		static const uint32 MediumSpringGreen 	= 0xFF00FA9A;	///< �д���ɫ
		static const uint32 MintCream			= 0xFFF5FFFA;	///< ��������
		static const uint32 SpringGreen			= 0xFF00FF7F;	///< ����ɫ
		static const uint32 MediumSeaGreen		= 0xFF3CB371;	///< �к�����
		static const uint32 SeaGreen			= 0xFF2E8B57;	///< ������
		static const uint32 Honeydew			= 0xFFF0FFF0;	///< ��ɫ/�۹�ɫ
		static const uint32 LightGreen			= 0xFF90EE90;	///< ����ɫ
		static const uint32 PaleGreen			= 0xFF98FB98;	///< ����ɫ
		static const uint32 DarkSeaGreen		= 0xFF8FBC8F;	///< ��������
		static const uint32 LimeGreen			= 0xFF32CD32;	///< ��������
		static const uint32 Lime				= 0xFF00FF00;	///< ������
		static const uint32 ForestGreen			= 0xFF228B22;	///< ɭ����
		static const uint32 Green				= 0xFF008000;	///< ����
		static const uint32 DarkGreen			= 0xFF006400;	///< ����ɫ
		static const uint32 Chartreuse			= 0xFF7FFF00;	///< ����ɫ/���ؾ���
		static const uint32 LawnGreen			= 0xFF7CFC00;	///< ����ɫ/��ƺ��
		static const uint32 GreenYellow			= 0xFFADFF2F;	///< �̻�ɫ
		static const uint32 DarkOliveGreen		= 0xFF556B2F;	///< �������
		static const uint32 YellowGreen			= 0xFF9ACD32;	///< ����ɫ
		static const uint32 OliveDrab			= 0xFF6B8E23;	///< ��魺�ɫ
		static const uint32 Beige				= 0xFFF5F5DC;	///< ��ɫ/����ɫ
		static const uint32 LightGoldenrodYellow= 0xFFFAFAD2;	///< ���ջ�
		static const uint32 Ivory				= 0xFFFFFFF0;	///< ����ɫ
		static const uint32 LightYellow			= 0xFFFFFFE0;	///< ǳ��ɫ
		static const uint32 Yellow				= 0xFFFFFF00;	///< ����
		static const uint32 Olive				= 0xFF808000;	///< ���
		static const uint32 DarkKhaki			= 0xFFBDB76B;	///< ���ƺ�ɫ/�ߴ��
		static const uint32 LemonChiffon		= 0xFFFFFACD;	///< ���ʳ�
		static const uint32 PaleGoldenrod		= 0xFFEEE8AA;	///< �Ҿջ�/������ɫ
		static const uint32 Khaki				= 0xFFF0E68C;	///< �ƺ�ɫ/��ߴ��
		static const uint32 Gold				= 0xFFFFD700;	///< ��ɫ
		static const uint32 Cornsilk			= 0xFFFFF8DC;	///< ����˿ɫ
		static const uint32 Goldenrod			= 0xFFDAA520;	///< ��ջ�
		static const uint32 DarkGoldenrod		= 0xFFB8860B;	///< ����ջ�
		static const uint32 FloralWhite			= 0xFFFFFAF0;	///< ���İ�ɫ
		static const uint32 OldLace				= 0xFFFDF5E6;	///< �ϻ�ɫ/����˿
		static const uint32 Wheat				= 0xFFF5DEB3;	///< ǳ��ɫ/С��ɫ
		static const uint32 Moccasin			= 0xFFFFE4B5;	///< ¹Ƥɫ/¹Ƥѥ
		static const uint32 Orange				= 0xFFFFA500;	///< ��ɫ
		static const uint32 PapayaWhip			= 0xFFFFEFD5;	///< ��ľɫ/��ľ��
		static const uint32 BlanchedAlmond		= 0xFFFFEBCD;	///< ����ɫ
		static const uint32 NavajoWhite			= 0xFFFFDEAD;	///< ���߰�/������
		static const uint32 AntiqueWhite		= 0xFFFAEBD7;	///< �Ŷ���
		static const uint32 Tan					= 0xFFD2B48C;	///< ��ɫ
		static const uint32 BurlyWood			= 0xFFDEB887;	///< Ӳľɫ
		static const uint32 Bisque				= 0xFFFFE4C4;	///< ������
		static const uint32 DarkOrange			= 0xFFFF8C00;	///< ���ɫ
		static const uint32 Linen				= 0xFFFAF0E6;	///< ���鲼
		static const uint32 Peru				= 0xFFCD853F;	///< ��³ɫ
		static const uint32 PeachPuff			= 0xFFFFDAB9;	///< ����ɫ
		static const uint32 SandyBrown			= 0xFFF4A460;	///< ɳ��ɫ
		static const uint32 Chocolate			= 0xFFD2691E;	///< �ɿ���ɫ
		static const uint32 SaddleBrown			= 0xFF8B4513;	///< �غ�ɫ/����ɫ
		static const uint32 Seashell			= 0xFFFFF5EE;	///< ������
		static const uint32 Sienna				= 0xFFA0522D;	///< ������ɫ
		static const uint32 LightSalmon			= 0xFFFFA07A;	///< ǳ������ɫ
		static const uint32 Coral				= 0xFFFF7F50;	///< ɺ��
		static const uint32 OrangeRed			= 0xFFFF4500;	///< �Ⱥ�ɫ
		static const uint32 DarkSalmon			= 0xFFE9967A;	///< ������/����ɫ
		static const uint32 Tomato				= 0xFFFF6347;	///< ���Ѻ�
		static const uint32 MistyRose			= 0xFFFFE4E1;	///< ǳõ��ɫ/����õ��
		static const uint32 Salmon				= 0xFFFA8072;	///< ����/����ɫ
		static const uint32 Snow				= 0xFFFFFAFA;	///< ѩ��ɫ
		static const uint32 LightCoral			= 0xFFF08080;	///< ��ɺ��ɫ
		static const uint32 RosyBrown			= 0xFFBC8F8F;	///< õ����ɫ
		static const uint32 IndianRed			= 0xFFCD5C5C;	///< ӡ�Ⱥ�
		static const uint32 Red					= 0xFFFF0000;	///< ����
		static const uint32 Brown				= 0xFFA52A2A;	///< ��ɫ
		static const uint32 FireBrick			= 0xFFB22222;	///< ��שɫ/�ͻ�ש
		static const uint32 DarkRed				= 0xFF8B0000;	///< ���ɫ
		static const uint32 Maroon				= 0xFF800000;	///< ��ɫ
		static const uint32 White				= 0xFFFFFFFF;	///< ����
		static const uint32 WhiteSmoke			= 0xFFF5F5F5;	///< ����
		static const uint32 Gainsboro			= 0xFFDCDCDC;	///< ����ɫ
		static const uint32 LightGrey			= 0xFFD3D3D3;	///< ǳ��ɫ
		static const uint32 Silver				= 0xFFC0C0C0;	///< ����ɫ
		static const uint32 DarkGray			= 0xFFA9A9A9;	///< ���ɫ
		static const uint32 Gray				= 0xFF808080;	///< ��ɫ
		static const uint32 DimGray				= 0xFF696969;	///< ������
		static const uint32 Black				= 0xFF000000;	///< ����
#ifdef _WIN32
#pragma endregion ConstantColors
#endif
	};



}// namespace sqr
