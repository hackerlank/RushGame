Ŀ¼˵��

bin		������Ŀ���п�ִ���ļ��Ĵ��λ��
engine		�����ֻ����������
game		�����ֻ�����Ϸ�߼��Ĵ���
implib		��ű���Ŀ�õ��ĵ��������ͷ�ļ������ļ�����װ�ļ����ϴ�ĵ������⣬����Direct3d,fmod�Ȳ����������
etc		�������ʱ��̬�����ļ����ļ�������������ʱ�޸�
var		�������ʱ���޸ĵ��ļ�������log�ļ����û������ļ��ȡ���̬�����ļ���Ҫ��������Ŀ¼��
tools ����Զ�������


MakefileTemplate.include �ļ����������Linux��������ı������

Debian�£�
  ��Ҫ cp MakefileTemplate.include Makefile.include
  33��������Ҫ�޸�
	GCC:=gcc $(COMMON_COMPILE_FLAG)  =>  GCC:=gcc-4.1 $(COMMON_COMPILE_FLAG)
	CC:=g++ $(COMMON_COMPILE_FLAG)  =>  CC:=g++-4.1 $(COMMON_COMPILE_FLAG)
	LIB_MYSQL:=-L/usr/local/lib/mysql -lmysqlclient -lz  =>  LIB_MYSQL:=-lmysqlclient -lz 
