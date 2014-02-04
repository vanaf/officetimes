<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
</head>
<body>

<b>{$person}</b> {$logout}

<table border='1'>


{foreach from=$times item=time}

{assign var="color" value="red"}

{if $time.direction==2}
{assign var="color" value="green"}
{/if}

{if $time.ldate ne $olddate}
{if $olddate}

{/if}
<tr>
<td>{$time.ldate}<br>&#x2245;{$time.tdiff}</td><td><font color="{$color}">{$time.ltime}</font>
{else}
<br>
<font color="{$color}">{$time.ltime}</font>
{/if}
{assign var="olddate" value=$time.ldate}
{/foreach}

</table>
</body>
</html>
