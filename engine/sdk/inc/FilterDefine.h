#ifndef _CFilter_Define_H_
#define _CFilter_Define_H_

//��Ⱦ��Ԫ�������ϵͳ �����ļ�
namespace sqr
{
#define MAX_RENDER_FILTER_NUM 255 //������������
//#define MAX_RENDER_GROUP_ID 0xFFFF
//#define INC_WINDOW_ID(ID) ID += 0x10000
//#define GET_WINDOW_ID(ID) ID&0xFFFF0000

//typedef DWORD GROUP_ID;

//������ ID
enum RENDER_BATCH_ID
{
	ID_SCENE_FILTER					= 0, //��������
	ID_NPC_FILTER					= 1, //NPC������
	ID_TERRAIN_FILTER				= 2, //���ι�����
	ID_WATER_FILTER					= 3, //ˮ�������
	ID_ALPHA_SCENE_FILTER			= 4, //��Alpha��ϵĳ���������
	ID_ALPHA_NPC_FILTER				= 5, //��Alpha��ϵ�NPC������
	ID_SHADOW_FILTER				= 6, //��Ӱ������
	ID_REFRACT_FILTER				= 7, //���������
	ID_NPC_EFFECT_FILTER			= 8, //NPC��Ч������
	ID_SCENE_EFFECT_FILTER			= 9, //������Ч������
	ID_DYNAMIC_VB					= 10,//��̬VB������
	ID_SCENE_SHADOWRECEIVET_FILTER	= 11,//NPC����������Ӱ������
	ID_NPC_SHADOWRECEIVET_FILTER	= 12,//������Ӱ������
	ID_TRANS_FILTER					= 13,//����������
	ID_SKYBOX_FILTER				= 14,//��պ�
	ID_GEOMETRY_INST_FILTER			= 15,//ʵ�������
	ID_GEOMETRY_INST_SR_FILTER		= 16,//������Ӱʵ�������
	ID_SHOW_ZFAILD_FILTER			= 17,//����Z����ʧ�ܵ�ģ��
	ID_ELEMENT_EX_1					= 18,//1����չ
	ID_ELEMENT_EX_2					= 19,//2����չ
	ID_ELEMENT_EX_3					= 20,//3����չ
	ID_ELEMENT_EX_4					= 21,//4����չ
	ID_ELEMENT_EX_5					= 22,//5����չ
	ID_ELEMENT_TOOLS				= 24,//������չ
	ID_MAX_FILTER_NUM,
	ID_END_FILTER					= 0xff,
	
};

//��Ӱ��������
enum eFilterFlag
{
	MFF_NONE_SHADOW		= 0,
	MFF_SCENE_SHADOW	= 1,
	MFF_NPC_SHADOW		= 2,
	MFF_ALL_SHADOW		= 3,
};

//��������Ⱦ˳������
struct RenderQueueFlag
{
	typedef char RENDER_INDEX;

	RENDER_INDEX	INDEX[MAX_RENDER_FILTER_NUM];

	RenderQueueFlag()
	{
		INDEX[0] = 0;
		INDEX[1] = RENDER_INDEX(ID_END_FILTER);
	}
	inline RenderQueueFlag&  operator	<< (RENDER_INDEX index)
	{
		INDEX[++INDEX[0]] = index;
		INDEX[INDEX[0]+1] = RENDER_INDEX(ID_END_FILTER);
		return *this;
	}

	inline RenderQueueFlag&  operator	>>(RENDER_INDEX index)
	{
		++INDEX[0];
		for (int i= INDEX[0];i>1;--i)
			INDEX[i] = INDEX[i-1];
		INDEX[1] = index;
		INDEX[INDEX[0]+1] = RENDER_INDEX(ID_END_FILTER);
		return *this;
	}

	inline bool InsertBack(RENDER_INDEX index1,RENDER_INDEX index2)
	{
		for (int i = 1;INDEX[i]!=RENDER_INDEX(ID_END_FILTER);++i)
			if (INDEX[i]==index1)
			{
				++i;
				++INDEX[0];
				for (int j= INDEX[0];j>i;--j)
					INDEX[j] = INDEX[j-1];
				INDEX[i] = index2;
				INDEX[INDEX[0]+1] = RENDER_INDEX(ID_END_FILTER);
				return true;
			}
		return false;
	}

	inline bool InsertBefore(RENDER_INDEX index1,RENDER_INDEX index2)
	{
		for (int i = 1;INDEX[i]!=RENDER_INDEX(ID_END_FILTER);++i)
			if (INDEX[i]==index1)
			{
				++INDEX[0];
				for (int j= INDEX[0];j>i;--j)
					INDEX[j] = INDEX[j-1];
				INDEX[i] = index2;
				INDEX[INDEX[0]+1] = RENDER_INDEX(ID_END_FILTER);
				return true;
			}
		return false;
	}

	inline bool Delete(RENDER_INDEX index)
	{
		for (int i = 1;INDEX[i]!=RENDER_INDEX(ID_END_FILTER);++i)
			if (INDEX[i]==index)
			{
				for (int j = i;INDEX[j]!=RENDER_INDEX(ID_END_FILTER);++j)
					INDEX[j] = INDEX[j+1];
				--INDEX[0];
				return true;
			}
		return false;
	}

	RenderQueueFlag& operator &= (const RenderQueueFlag& Flag) //�����Ƚ���
	{
		int IndexMap[MAX_RENDER_FILTER_NUM];
		int IndexQueue[MAX_RENDER_FILTER_NUM];
		for (int i = 1;Flag[i]!=RENDER_INDEX(ID_END_FILTER);++i)
		{
			IndexMap[i] = Find(Flag[i]);
			int j;
			for (j=1;j<i;++j)
			{
				if (IndexMap[IndexQueue[j]]>IndexMap[i])
				{
					for (int k=i;k>j;--k)
						IndexQueue[k] = IndexQueue[k-1];
					IndexQueue[j] = i;
					break;
				}
			}
			if (j==i)IndexQueue[i] = i;
		}

		for (int i = 1;Flag[i]!=RENDER_INDEX(ID_END_FILTER);++i)
		{
			if ( IndexQueue[i] == 0 )
				continue;
			INDEX[IndexMap[IndexQueue[i]]] = Flag[IndexQueue[i]];
		}
		return *this;
	}

	inline int Find(RENDER_INDEX index)
	{
		for (int i = 1;INDEX[i]!=RENDER_INDEX(ID_END_FILTER);++i)
			if (INDEX[i]==index)
				return i;
		return 0;
	}

	inline RenderQueueFlag& operator = (const RenderQueueFlag& flag)
	{
		memcpy(INDEX,flag.INDEX,sizeof(INDEX));
		return *this;
	}

	inline const RENDER_INDEX& operator [] (const int i) const
	{
		return INDEX[i];
	}
};
}
#endif