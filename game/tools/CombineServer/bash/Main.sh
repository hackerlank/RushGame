#!/bin/bash

source ./args

if [ -e $CombineLogFile ]
then
	rm -f "$CombineLogFile"
fi

declare -a dumpfilelist=(${*})
dumpfilecount=${#}


#������������������Ҫ����Ҫ�ϲ������ݿ�dump�ļ�
if [ $dumpfilecount -lt 2 ]
then
	echo "usage:bash Main.sh dumpfile1 dumfile2 dumpfile3"
	echo "��dumpfile2,dumpfile3���ݺϲ���dumpfile1��ȥ���������ݿ���Ϊybtx"
	echo "ERROR:������������������Ҫ����Ҫ�ϲ������ݿ�dump�ļ�" > "$CombineLogFile"
	cat "$CombineLogFile"
	exit 0
fi

#�������Ƿ�Ϊ�ļ����Ƿ����
num=1
while [ $num -le $dumpfilecount ]
do
	dumpfile=${dumpfilelist[${num}-1]}
	if [ ! -e "$dumpfile" ]
	then
		echo "ERROR:Ҫ�ϲ������ݿ��dump�ļ�������"
		cat "$CombineLogFile"
		exit 0
	fi
	let num=num+1
done

echo "Main.sh"
date
date > "$CombineLogFile"

#������ʱ�н����ݿ�medi_database���н��
echo "������ʱ�н����ݿ�medi_database���н��"
echo "������ʱ�н����ݿ�medi_database���н��" >> "$CombineLogFile"
cat "$ExcelSqlDir"/CreateMediTbl.sql | mysql -uroot -p$MysqlPasswd >> "$CombineLogFile" 2>&1

errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi

#��ʼ���н����ݿ��е�ĳЩ�н��
echo "��ʼ���н����ݿ��е�ĳЩ�н��"
echo "��ʼ���н����ݿ��е�ĳЩ�н��" >> "$CombineLogFile"
num=1
while [ $num -le $dumpfilecount ]
do
	database="ybtx_$num"
	if [ $num -eq 1 ]
	then
		database="ybtx"
	fi
	
	#��ʼ��tbl_id_add_count��
	echo "insert into tbl_id_add_count(ian_sBaseName) values(\"$database\");" | mysql -uroot -p$MysqlPasswd medi_database >> "$CombineLogFile" 2>&1

	#��ʼ��tbl_base_Id��
	echo "insert into tbl_base_Id(bi_sBaseName,bi_uId) values(\"$database\",$num)" | mysql -uroot -p$MysqlPasswd medi_database > "$CombineLogFile" 2>&1

	let num=num+1
done

errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
        cat "$CombineLogFile"
        exit 0
fi

#�ϲ�ǰ��׼��
echo "PrepareCombine.sh"
echo "  ����һ��dump�ļ������������ݿ�ybtx������ķֱ��뵽ybtx_2,ybtx_3...."
echo "  ��պϷ�����Ҫ���ݵı�"
echo "  �������ݿ�������id�����ֵ����Ϊ�������ݿ�ƫ�Ƶ�ƫ����"
num=1
while [ $num -le $dumpfilecount ]
do
	dumpfile=${dumpfilelist[${num}-1]}
	bash PrepareCombine.sh "$dumpfile" "$num" >> "$CombineLogFile" 2>&1 &
	let num=num+1
done
wait

errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
        exit 0
fi

#ĳЩ������Ҫ���⴦��
echo "SpecialProcess.sh"
echo "  �����ݿ�ybtx_2,ybtx_3...��������id��ƫ��"
echo "  ���û���ĳЩ���ݿⱣ�浽medi_database"
echo "  �޸�ybtx_2,ybtx_3...����������idΪybtx��������id"
num=1
while [ $num -le $dumpfilecount ]
do
	bash SpecialProcess.sh "$num" >> "$CombineLogFile" 2>&1 &
	let num=num+1
done
wait

errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
        exit 0
fi

#�������ݿ�ϲ����Ѻ�������ݿⶼ�ϲ�����һ�����ݿ���ȥ
echo "Combine.sh"
echo "  �������ݿ�ϲ����Ѻ�������ݿⶼ�ϲ�����һ�����ݿ���ȥ"
echo "  ��ybtx_2,ybtx_3...�и�ybtx������Ӷ�������ͽ�ɫ������������"
echo "�������ݿ�ϲ����Ѻ�������ݿⶼ�ϲ�����һ�����ݿ���ȥ" >> "$CombineLogFile"
num=2
while [ $num -le $dumpfilecount ]
do
	bash Combine.sh "$num" >> "$CombineLogFile"  2>&1 &
	let num=num+1
done
wait

errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
        exit 0
fi

#�ϲ����һЩ����
echo "SpecProcAfterCombine.sh"
echo "  �ϲ������������ybtx���û�����"
echo "  ���ý�ɫ��¼��ɾ��ʱ��"
bash SpecProcAfterCombine.sh >> "$CombineLogFile" 2>&1

errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
        exit 0
fi

echo "���ݿ�ϲ���ϣ�"
echo "���ݿ�ϲ���ϣ�" >> "$CombineLogFile"
date
date >> "$CombineLogFile"

