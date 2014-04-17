

#define  STRING_LEN        256
#define  STRING_POOL_SIZE  2000000
#define  MAX_BUFFER_COUNT  5000


// ��ͨģʽ�µĽڵ�
typedef struct tagBufferData {
	int   level;        // ��ǰջ����
	char* FileDefined;  // �����ļ�
	char* FunctionName; // ������
	long  LineDefined;  // ����������
	long  CurrentLine;  // ��ǰ������
	double LocalTime;    // ����GetSeconds���ȡʱ�䷵��ֵ��������ʱ��
	double TotalTime;    // ͬ��
} BufferData;


// �ýṹ�洢���ļ�֮����Ҫ����ı���
typedef struct tagGlobalState {
	int   CurrentLine;  // ��¼��ǰ����������
	double FuncCallTime; // Lua������������ʱ��
	int   HookMark;     // �Ƿ������¼�������ñ�ʶ
	char  auxs[STRING_LEN]; // ��¼log�ļ���

	int BufCount;
	BufferData* Buffer;
} GlobalState;


#ifdef DEBUG
#define ASSERT(e, msg)                           \
	if (!e) {                                    \
		printf("Assertion failed: %s\n", msg);   \
		exit(1);                                 \
	}

#define AstFile(e, msg)                          \
	if (!e) {                                    \
		printf("Can't open file: %s\n", msg);    \
		exit(1);                                 \
	}

#else
#define ASSERT(e, msg)
#define AstFile(e, msg)

#endif

