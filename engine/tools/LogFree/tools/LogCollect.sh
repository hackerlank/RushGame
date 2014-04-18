#!/bin/bash

#logԭʼ·��������ط�/home/uplog������������Ҫ���ļ��ƶ������Ǵ���ĵط�
/bin/date 
src_path=$1
if [ ! -d "$src_path" ]
	then
	echo "The Source Path is Not exist"
	exit 1
fi

database_name=$2
if [ -z "$database_name" ]
	then
	echo "The DataBase Name is nil"
	exit 1
fi

src_path=$(echo "$src_path" | sed 's/[\/\\\\]$//')

src_gas="$src_path/gas"
src_gac="$src_path/gac"

#gas��gac������������·��
input_path="$src_path"_in
input_gas="$input_path/gas"
input_gac="$input_path/gac"

mkdir -p $input_gas
mkdir -p $input_gac

outputbak_path="$src_path"_bak

mkdir -p $outputbak_path

mkdir -p "$outputbak_path/in"

cp -rf $src_gas "$outputbak_path/in"
cp -rf $src_gac "$outputbak_path/in"


src_gas_num=$(ls $src_gas/gas_err* 2>/dev/null | wc -l)
if [ "$src_gas_num" != 0 ];
	then
	mv $src_gas/gas_err* $input_gas
fi

src_gac_num=$(ls $src_gac/gac_err* 2>/dev/null | wc -l)
if [ "$src_gac_num" != 0 ];
	then
	mv $src_gac/gac_err* $input_gac
fi

#CatLog��������·��
catlog_path=/home/tangliu/ybtx/programmer/trunk/engine/tools/LogFree/tools/CatLog

#log server���ýű���·��
php_path=/home/tangliu/ybtx/programmer/trunk/engine/tools/LogFree/tools

output_path="$src_path"_out
output_gas="$output_path/gas"
output_gac="$output_path/gac"


#����mapfile symbol�ļ��ĵط���һ���Ǵ����ʽ������debug_11111_c.tar.gz
mapfile_ar=/var/www/html/debug

#������������mapfile�ĵط�������ļ������ж���ļ��У�ÿ���ļ����԰汾������
mapfile_path="$src_path"_mapfile

mkdir -p $mapfile_path

mkdir -p $output_gas
mkdir -p $output_gac

src_clientguid_num=$(ls $src_gas/ClientGuid* 2>/dev/null | wc -l)
if [ "$src_clientguid_num" != 0 ];
	then
	mv $src_gas/ClientGuid* $output_gas
fi

cd $catlog_path

#Cat gac log
find $input_gac -name "gac_err*.info" | while read file

do

version=$(echo $file | sed 's,.*v\([0-9][0-9]*\)\.info$,\1,')

#find .zip 
mapfile="$mapfile_ar/debug_"$version".zip"

if [ -f $mapfile ]
	then

	mapfile_dir="$mapfile_path/"$version
	if [ ! -d "$mapfile_dir" ]
		then

		mkdir -p $mapfile_dir
		unzip -q $mapfile -d $mapfile_dir
	fi
fi

#find .tar.gz 
mapfile="$mapfile_ar/debug_"$version"_c.tar.gz"

if [ -f $mapfile ]
	then

	mapfile_dir="$mapfile_path/"$version
	if [ ! -d "$mapfile_dir" ]
		then

		mkdir -p $mapfile_dir
		tar xzf $mapfile -C $mapfile_dir
	fi
fi

done

./CatLog $input_gac -path $mapfile_path -o $output_gac


#Cat gas log
./CatLog $input_gas -o $output_gas

#filt gas c++ symbol
cd $output_gas

find . -name "gas_err*.log" | while read file
do
cat $file | c++filt > "$file".bak
done

find . -name "gas_err*.log.bak" | while read file
do
NewFile=$(echo $file | sed 's,\.bak$,,')
cp -f $file $NewFile
done

rm -f *.log.bak

mkdir -p "$outputbak_path/out"

cp -rf $output_gas "$outputbak_path/out"
cp -rf $output_gac "$outputbak_path/out"

php $php_path/MainFunc.php $output_path "10.10.43.142" "root" "1" "$database_name" 

rm -rf $input_path
rm -rf $output_path
rm -rf $mapfile_path
