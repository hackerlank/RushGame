#!/usr/bin/php
<html>
<title>��ӭ���� <?php require_once("../../conf/LogFree/config.php"); echo "$log_free_name"; ?> LogFree</title>
<head>
<link href="/css/logfree.css" type="text/css" rel="stylesheet"></link>
</head>
<body>
<h3>��ӭ���� <?php require_once("../../conf/LogFree/config.php"); echo "$log_free_name"; ?> LogFree</h3>
</head>
<body align=center>
<div align=center>
      <ul>
         <li class="item other item1"><a href='list_serv_log.php?page=1&order=1'>�������˳���log</a></li>
         <li class="item other item2"><a href='list_client_log.php?page=1&order=1'>�ͻ��˳���log</a></li>
         <li class="item other item3"><a href='list_art_log.php?page=1&order=1'>������Դlog</a></li>
         <li class="item other item4"><a href='list_designer_log_c.php?order=1&page=1'>�ͻ������ñ�log</a></li>
         <li class="item other item5"><a href='list_designer_log_s.php?order=1&page=1'>���������ñ�log</a></li>
         <li class="item other item6"><a href='list_compatible_log.php?order=1&page=1'>�����Դ���log</a></li>
	 <li class="item other item7 "><a href='list_user_log.php?order=1&page=1' target='_blank'>�û��ύlog</a></li>
	 <li class="item other item4 "><a href='list_statistic.php'>�鿴ͳ��</a></li>
      </ul>
</div>
</body>
</html>
