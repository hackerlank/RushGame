<?php
function ReadInfoFile($filename)
{
	$fp = fopen($filename, 'r');
	if (!$fp){
		echo "$filename �ļ�������";
		return;
	}
	$info = array();//code, title, stack, type
	$info[0] = "";
	$info[1] = "";
	$info[2] = "";
	$info[3] = "";		
	$info[4] = "";		
	$index = 0;
	while(!feof($fp)){
		$line = fgets($fp);
	
		$line = substr($line, 0, strlen($line) - 1);
		$key =  substr($line, 0, 4);
		if ($key == '���')
		{
			$code 	= mysql_escape_string($info[0]);
			$title 	= mysql_escape_string($info[1]);
			$stack 	= $info[2];
			$type 	= mysql_escape_string($info[3]);
			$extra  = mysql_escape_string($info[4]);
			InsertLogInfo($code, $title, $stack, $type, $extra);
			$info[0] = "";
			$info[1] = "";
			$info[2] = "";
			$info[3] = "";		
			$info[4] = "";		

			$index = 0;
			$info[$index] = $info[$index] . substr($line, 6);
		}
		else if ($key == '��ջ')
		{
			$index = 2;		
			$info[$index] = $info[$index] . substr($line, 6);
		}
		else if ($key == '��Ϣ')
		{
			$index = 1;		
			$info[$index] = $info[$index] . substr($line, 6);
		}
		else if ($key == '����')
		{
			$index = 3;		
			$info[$index] = $info[$index] . substr($line, 6);
		}
		else if ($key == '����')
		{
			$index = 4;
			$info[$index] = $info[$index] . substr($line, 10);
		}
		else
		{
			$info[$index] = $info[$index] . mysql_escape_string($line) . "\n";
		}
	}
	$code 	= mysql_escape_string($info[0]);
	$title 	= mysql_escape_string($info[1]);
	$stack 	= $info[2];
	$type 	= mysql_escape_string($info[3]);
	$extra  = mysql_escape_string($info[4]);
	if ($code != "")
		InsertLogInfo($code, $title, $stack, $type, $extra);
}

function process_all_info($path){
	foreach(glob("$path/*.info.log") as $filename)
	{
		ReadInfoFile($filename);
	}
}
?>
