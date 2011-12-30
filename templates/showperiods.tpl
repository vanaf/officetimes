<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<link rel="stylesheet" href="css/style.css" media="all"> 
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.5.1/jquery.min.js" type="text/javascript"></script> 
<script type="text/javascript">
{literal}
var startTime;
var timew;
var timed;
var timep;

function tick(){
	setTimeout(tick,1000);
	$('#firstw').text(actualizeTime(timew));
	$('#firstd').text(actualizeTime(timed));
	$('#firstp').text(actualizeTime(timep));	
}

function actualizeTime( time ){
	currTime = (new Date()).getTime();
	return secondsToTime( timeToSeconds(time) + Math.round((currTime-startTime)/1000));
}

function timeToSeconds( time ){
	parts = time.split(":");
	hours =  parts[0];
	minutes = parts[1];
	seconds = parts[2];
	return hours*3600 + minutes*60 + seconds*1;
}

function secondsToTime( seconds ){
        hours = Math.floor(seconds/3600);
        minutes = Math.floor(seconds%3600/60);
        seconds = seconds%60;
	hours = hours<10 ? "0" + hours : hours;
	minutes = minutes<10 ? "0" + minutes : minutes;
	seconds = seconds<10 ? "0" + seconds : seconds;
        return hours+":"+minutes+":"+seconds;
}




function beginCheck(){
	if($.trim($("#firste").text())==""){
		startTime = (new Date()).getTime();
//		alert(startTime);
		timew = $.trim($("#firstw").text());
		timed = $.trim($("#firstd").text());
		timep = $.trim($("#firstp").text());
		//actualizeTime(timew);



		setTimeout(tick,1000);
//		alert("Внутри");
	}
}


window.onload = beginCheck;
{/literal}
</script> 

</head>
<body>
<b>{$person}</b> <form style='display:inline; margin:0;'target='_parent'>
{if $logout eq null}
<input type='hidden' name='a' value='showperiods'>
<input type='hidden' name='id' value='{$perid}'>
{/if}
<input size='1' type='text' name='hours' value='{$hours}'>часов
<input type='submit' value='ok'></form> 
{$logout}
{literal}
<style type="text/css">
.week  td {
border-top-width: 3px !important;
}
.date td {
border-top-width: 2px !important;
}
</style>
{/literal}

<table border='1' style='border-collapse:collapse;' cellpadding="5">
<tr>
<th>
{if $period eq 'month'}
<a href='?a=showperiods&id={$perid}&hours={$req_hours}&period=week' title='Переключиться на недели'>
Месяц
</a>
{else}
<a href='?a=showperiods&id={$perid}&hours={$req_hours}&period=month' title='Переключиться на месяцы'>
Неделя
</a>
{/if}
</th><th>Дата</th><th>&#x2206;t</th><th>Вход</th><th>Выход</th>
</tr>

{assign var="idw" value=' id="firstw"'}
{assign var="idd" value=' id="firstd"'}
{assign var="idp" value=' id="firstp"'}
{assign var="ide" value=' id="firste"'}

{foreach from=$times item=time}

{if $time.ldate eq null}
<tr class='week'>
<td rowspan="{$time.kolvo}"{$idw}>
{if $idw ne ''}
{assign var="idw" value=''}
{/if}
{$time.sduration}
</td>
{assign var="datetr" value='0'}
{else}
{if $time.enter_time eq null}
{if $datetr eq '1'}
<tr class='date'>
{/if}
{assign var="datetr" value='1'}
<td rowspan="{$time.kolvo}">
{$time.ldate|date_format:"%e/%m, %A"}
{if $time.kolvo>1}
<br>&#x2211;=<span{$idd}>{$time.sduration}</span>
{/if}
{if $idd ne ''}
{assign var="idd" value=''}
{/if}
</td>
{assign var="pertr" value='0'}
{else}

{if $pertr eq '1'}
<tr>
{/if}
{assign var="pertr" value='1'}
<td{$idp}>
{if $idp ne ''}
{assign var="idp" value=''}
{/if}
{$time.sduration}
</td>
<td>
{$time.enter_time|date_format:"%k:%M"}
</td>
<td{$ide}>
{if $ide ne ''}
{assign var="ide" value=''}
{/if}
{if $time.exit_time|strlen <15}
{$time.exit_time|date_format:"%k:%M"}
{else}
{$time.exit_time}
{/if}
</td>
</tr>
{/if}
{/if}

{/foreach}

</table>

{if $secLeft> 0}

    <div id="wrapper">
      <div id="gradient">&nbsp;</div>
      <div id="container">
        <div id="canvas-content">
          <div id="countdown">
            <script type="text/javascript"> 
                var leftSeconds = {$secLeft};
            </script>
          </div>
          <script type="text/javascript" src="js/base.js"></script>
          <script type="text/javascript" src="js/box2d.js"></script>
          <script type="text/javascript" src="js/io.js"></script>
        </div>
      </div>
    </div>
{/if}


</body>
</html>
