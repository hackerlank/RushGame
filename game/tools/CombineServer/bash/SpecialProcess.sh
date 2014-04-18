#!/bin/bash

source ./args

#ִ�кϲ�
baseindex=${1}
database=ybtx_$baseindex

if [ $baseindex -eq 1 ]
then
	database="ybtx"
fi

if [ $baseindex != 1 ]
then
	echo "UpdateAutoIncreID.sql" >> "$CombineLogFile" 2>&1
	echo "��$database������ID����ƫ��" >> "$CombineLogFile"
	#��ǰһ�����ݿ�����ID���ϱ����ݿ�����ID��ֵ��ƫ��ֵ�����ı䱾���ݿ�Ķ�ӦID
	cat "$ExcelSqlDir"/UpdateAutoIncreID.sql | mysql -uroot -p$MysqlPasswd $database >> "$CombineLogFile" 2>&1
	errstr=`grep "ERROR" "$CombineLogFile"`
	if [ "$errstr" != "" ]
	then
		cat "$CombineLogFile"
		exit 0
	fi
fi


echo "SaveUserDataToMediTbl.sql" >> "$CombineLogFile" 2>&1
echo "$database�û���ĳЩ���ݱ��浽MediaTbl" >> "$CombineLogFile"
cat "$ExcelSqlDir"/SaveUserDataToMediTbl.sql | mysql -uroot -p$MysqlPasswd $database >> "$CombineLogFile" 2>&1
errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi

if [ $baseindex != 1 ]
then
	echo "ResetSceneID.sql" >> "$CombineLogFile" 2>&1
	echo "�޸�$database������������IDΪ��һ�����ݿ��������ID" >> "$CombineLogFile"
	cat "$ExcelSqlDir"/ResetSceneID.sql | mysql -uroot -p$MysqlPasswd $database >> "$CombineLogFile" 2>&1
	errstr=`grep "ERROR" "$CombineLogFile"`
	if [ "$errstr" != "" ]
	then
		cat "$CombineLogFile"
		exit 0
	fi
fi
