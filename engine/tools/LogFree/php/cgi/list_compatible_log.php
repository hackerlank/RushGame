#!/usr/bin/php
<html>
<title>������Դlog�б�</title>
<head>
<script type="text/javascript" src="/js/pager.js"></script>
<script type="text/javascript" src="/js/WriteStatus.js"></script>
<link href="/css/logfree.css" type="text/css" rel="stylesheet"></link>
</head>
<body align='center'>
<h3>��ӭ����LogFreeϵͳ</h3>
<p>
      <ul>
         <li class="item other item1 "><a href='list_serv_log.php?page=1&order=1'>�������˳���log</a></li>
         <li class="item other item2 "><a href='list_client_log.php?page=1&order=1'>�ͻ��˳���log</a></li>
         <li class="item other item3 "><a href='list_art_log.php?page=1&order=1'>������Դlog</a></li>
         <li class="item other item4 "><a href='list_designer_log_c.php?order=1&page=1'>�ͻ������ñ�log</a></li>
         <li class="item other item5 "><a href='list_designer_log_s.php?order=1&page=1'>���������ñ�log</a></li>
	 <li class="item current"><a href='list_compatible_log.php?order=1&page=1'>�����Դ���log</a></li>
	 <li class="item other item7 "><a href='list_user_log.php?order=1&page=1' target='_blank'>�û��ύlog</a></li>
	 <li class="item other item1 "><a href='list_statistic.php'>�鿴ͳ��</a></li>
      </ul>
</p>
<br />
<p><b>���м�����log����</b></p>
<table border=1>
<?php
require_once("../../conf/LogFree/config.php");
require_once("../../lib/LogFree/Lib.php");
$total = 0;
$page = 0;
$size=20;
$view_size = 5;
$order = 1;

function ViewLog()
{
	global $db_host, $db_user, $db_pass, $db_name;	
	global $total, $page, $size, $view_size;
	global $server_count_info, $server_from_info, $server_select_info, $server_where_info, $server_order_info;
	$page = $_GET["page"];
	$order = intval($_GET["order"]);

	$Conn = mysql_connect($db_host, $db_user, $db_pass);
	mysql_query("SET NAMES 'GBK'");
	mysql_select_db($db_name, $Conn);

	$qs = "select count(distinct clc.error_code) from tbl_client_log_common clc, tbl_log_info li, tbl_guid_info gi where li.error_code = clc.error_code and gi.guid = clc.client_guid and li.error_type like binary '������%'";
	$result = mysql_query($qs);
	$row = mysql_fetch_row($result);
	$total = $row[0];

	$b = $page * $size - $size;
	$e = $size;

	$order_str = " order by first desc ";
	if ($order == 1)
	{
		$order_str = " order by last desc ";
	}
	
	$qs = "select li.error_code, li.error_title, max(clc.happened_time) as last, min(clc.happened_time) as first, count(clc.happened_time), li.error_owner, li.error_status, max(clc.version) , sum(distinct network_flag) as cnf from tbl_client_log_common clc, tbl_log_info li, tbl_guid_info gi where li.error_code = clc.error_code and gi.guid = clc.client_guid and li.error_type like binary '������%' and is_special_client = 0 group by li.error_code $order_str limit $b, $e";
	$result = mysql_query($qs);
	
	$str = sprintf("<tr bgcolor='#9acd32'>\n")
		.sprintf("<th width=400>log����(����鿴��ջ)</th><th width=100>������</th><th width=125><a href='list_art_log.php?order=1&page=1'>�����ʱ��</a></th><th width=125><a href='list_art_log.php?order=2&page=1'>�״η���ʱ��</a></th><th width=75>����(����鿴�б�)</th><th width='100'>����λ��</th><th width=75>ָ�ɸ�</th><th width=75>����״̬</th><th width=75>���汾</th>\n")
		.sprintf("</tr>\n");
	$str_title = "<a href='detail_c.php?code=%s' target='_blank'>%s</a>";

	$i=1;
	while($row=mysql_fetch_row($result))
	{
		$code = $row[0];
		$title= $row[1];
		$last = $row[2];
		$first= $row[3];
		$times= $row[4];
		$owner= $row[5];
		$status=$row[6];
		$version = $row[7];
		$color = "#ffffff";

                $cnf = intval($row[8]);
                $network_flag = GetNetFlag($cnf);

		if ($i % 2)
			$color = "#d8d8d8";
		$str = $str.sprintf("<tr bgcolor='%s'>\n", $color)
			.sprintf("<td align='left'>&nbsp;").sprintf($str_title, urlencode($code), str_replace('%', '%%', $title)).sprintf("</td>\n")
			.sprintf("<td align='center'>%s</td>", $code)
			.sprintf("<td align='center'>%s</td>", $last)
			.sprintf("<td align='center'>%s</td>", $first)
			.sprintf("<td align='center'><a href='list_by_code_c.php?page=1&code=%s' target='_blank'>%s</a></td>", $code, $times)
                        .sprintf("<td align='center'>%s</td>", $network_flag)
                        .sprintf("<td align='center'>%s</td>", WriteOwner($owner))
                        .sprintf("<td align='center'><script>WriteStatus(%d)</script></td>", $status)
			.sprintf("<td align='center'>%s</td>", $version)
			.sprintf("</tr>");
		$i++;
	}

	printf($str);
	
}
ViewLog();
?>
</table>
<div id="pager"></div>
<script>
<!--
<?php
	printf("var url='list_compatible_log.php?order=$order&';\n");
	printf("CreatePage($page, $total, $size, \"pager\", $view_size, url);\n");	
?>
-->
</script>
</body>
</html>
