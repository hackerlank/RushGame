#pragma once
#include <vector>

#define COUT(s)  //cout << #s << endl;		

#define OUT					//����ע��Ϊ���


const double DIR_TO_RADIAN_RATE = 2 * 3.14159265358 / 256.0; //��Ϸ������뻡�ȵı���,  ���� = ���� * DIR_TO_RADIAN_RATE
//const int OFFSET_8DIR[8][2] =  { {-1,-1},{ 0,-1},{ 1,-1},    //8��ƫ������
//								 {-1, 0},	     { 1, 0},
//								 {-1, 1},{ 0, 1},{ 1, 1},
//								};
//const int OFFSET_4DIR[4][2] =   {     { 0,-1},				//4��ƫ������
//								{-1, 0},	  { 1, 0},
//									   { 0, 1}
//								};

const int OFFSET_8DIR[8][2] =  { {-1,-1},{ 0,-1},{ 1,-1},{ 1, 0},{ 1, 1},{ 0, 1},{-1, 1},{-1, 0}};   //8��ƫ������

const int OFFSET_4DIR[4][2] =   { { 0,-1},{ 1, 0},{ 0, 1},{-1, 0} };				//4��ƫ������
	 


//#define GRID_PIXEL_W				8
#define MIN_DIS						0.2f   //������������С����
#define COLONY_AREA					12.0f	//Ⱥ���Ա������npc�ľ�����, ��ֹճ����ʱ�����̫Զ

#define GAME_VIEW_TOP_DIS			16.5f
#define GAME_VIEW_TOP_LENGTH		39.0f
#define GAME_VIEW_BOTTOM_DIS		9.5f
#define GAME_VIEW_BOTTOM_LENGTH		20.0f
#define GAME_VIEW_ANGLE				0.785398163397   // 45 ��


#define GRID_PRECISION			3			//Ѱ·���ӵ�ϸ�ִ���, ���Ӿ���

#define  PAINT_EVENT_ID			1			//�ػ�Timer�¼�ID
#define	 PAINT_ELAPSE			35			//�ػ�Timer���ʱ��
#define  PAUSE_TIMES			3			//��ͣ���ƴ���


#define BLOCK_SIZE				4			//npc,obj,trap �洢��Ĵ�С,���ڴ�ֵ�ﵽ�����ٶ�	


//#define SELECT_RECT_LINE_SIZE   1			//��ѡ���ߵĴ�ϸ

#define SELECT_PRECISION		4			//���ѡ��ľ���

#define SELECT_MARK_SIZE		18			//��ѡ������ǿ��С
//#define SELECT_MARK_LINE_SIZE	1			//��ѡ������ǿ�Ĵ�ϸ

#define NPC_IMAGE_SIZE			9			//NPC��ͼ��ǵ����ش�С
#define OBJ_IMAGE_SIZE			9			//OBJ��ͼ��ǵ����ش�С
#define TRAP_IMAGE_SIZE			9			//TRAP��ͼ��ǵ����ش�С
#define PATH_KEY_POS_SIZE		7			//Ѳ��·���ؼ����С
#define PATH_LINE_SIZE			1			//Ѳ��·�ߴ�ϸ
#define DIR_ARROW_SIZE			25			//�����ͷ����
#define DIR_ARROW_LINE_SIZE		2			//�����ͷ�ߴ�ϸ
#define GAME_VIEW_LIEN_SIZE		1
#define ROTATE_ROUND_SIZE		3
#define ROTATE_ROUND_CENTER_RADIUS 9
#define MEASURE_LINE_SIZE		2

#define SELECT_RECT_COLOR			RGB(0,255,0)
#define SELECT_MARK_COLOR			RGB(0,255,0)

#define NPC_IMAGE_COLOR				RGB(255,0,0)		//NPC��ͼ��ǵ���ɫ
#define BOSS_IMAGE_COLOR			RGB(255,255,255)
#define SERVANT_IMAGE_COLOR			RGB(255,128,0)
#define SINGLE_ATTACK_IMAGE_COLOR	RGB(0,255,255)
#define MULTI_ATTACK_IMAGE_COLOR	RGB(255,0,255)
#define NPC_IMAGE_FRAME_COLOR		RGB(0,0,1)
#define OBJ_IMAGE_COLOR				RGB(0,255,0)		//OBJ��ͼ��ǵ���ɫ
#define BARRIER_OBJ_IMAGE_COLOR		RGB(0,0,0)
#define TRAP_IMAGE_COLOR			RGB(0,0,255)		//TRAP��ͼ��ǵ���ɫ
#define PATH_KEY_POS_COLOR			RGB(255,255,0)
#define PATH_START_COLOR			RGB(255,0,255)
#define PATH_LINE_COLOR				RGB(0,128,255)
#define PATH_VIRTUAL_LINE_COLOR		RGB(128,80,0)
#define PATH_LINE_FOCUS_COLOR		RGB(0,255,80)
#define DIR_ARROW_COLOR				RGB(255,0,0)
#define DIR_ARROW_FOCUS_COLOR		RGB(255,255,0)
#define GAME_VIEW_COLOR				RGB(255,255,255)
#define PEN_RECT_COLOR				RGB(255,255,255)
#define BLOCK_COLOR_LV1				RGB(0,0,255)
#define BLOCK_COLOR_LV2				RGB(255,100,100)
#define BLOCK_COLOR_LV3				RGB(255,0,0)
#define GRID_LINE_COLOR				RGB(0,80,0)
#define ROTATE_ROUND_COLOR			RGB(250,55,217)
#define ROTATE_ROUND_FOCUS_COLOR	RGB(255,255,255)
#define MOVE_ROUND_FOCUS_COLOR		RGB(255,255,255)
#define ROTATE_ROUND_CENTER_COLOR	RGB(255,255,255)
#define MEASURE_LINE_COLOR			RGB(255,255,255)
#define MEASURE_BACK_COLOR			RGB(255,255,255)
#define MEASURE_TEXT_COLOR			RGB(0,0,0)

#define MIN_SCALING				0.15		//��ͼ��С���ű���
#define MAX_SCALING				6.0			//��ͼ������ű���

#define CHAT_BUFFER_SIZE		1024	//�ַ����������С,����Save��, ��С���ž���,�����ٸ�





#define  eRegionSpan			16  //��������ֻ��Ϊ��ʹ����Ϸ�����������һ��
