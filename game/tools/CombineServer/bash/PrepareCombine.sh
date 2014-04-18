#!/bin/bash

source ./args

dumpfile=${1}
baseindex=${2}
database="ybtx_$baseindex"

if [ $baseindex -eq 1 ]
then 
	database="ybtx"
fi

echo "��$dumpfile����$database" >> "$CombineLogFile"
echo "drop database if exists ${database};" | mysql -uroot -p$MysqlPasswd >> "$CombineLogFile" 2>&1
echo "create database ${database};" | mysql -uroot -p$MysqlPasswd >> "$CombineLogFile" 2>&1
cat "$dumpfile" | mysql -uroot -p$MysqlPasswd ${database} >> "$CombineLogFile" 2>&1
errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi
echo "����$dumpfile���" >> "$CombineLogFile"


echo "ClearTable.sql" >> "$CombineLogFile" 2>&1
echo "$database���ĳЩ��" >> "$CombineLogFile" 2>&1
cat "$ExcelSqlDir"/ClearTable.sql | mysql -uroot -p$MysqlPasswd ${database} >> "$CombineLogFile" 2>&1
errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi

echo "SaveMaxId.sql" >> "$CombineLogFile" 2>&1
echo "����$database��������ID�����ֵ��Ϊ�������ݿ��ƫ����" >> "$CombineLogFile"
cat "$ExcelSqlDir"/SaveMaxId.sql | mysql -uroot -p$MysqlPasswd ${database} >> "$CombineLogFile" 2>&1
errstr=`grep "ERROR" "$CombineLogFile"`
if [ "$errstr" != "" ]
then
	cat "$CombineLogFile"
	exit 0
fi

