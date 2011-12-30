{include file="header.tpl"}

<h3>OpenSoft: 
{if $parent}
<a href='?a=showdeps&parent={$parent_parent}'>></a>

{/if}
{$parent}</h3>
{assign var='lasttime' value='0'}

{if $denied}
<h4>Ты сюда не ходи, ты <a href='?a=showdeps&parent=17'>туда</a> ходи, а то мало ли чего нехорошего может случиться...</h4>
{else}

{if $parent_id>=0}
<a href='?a=showdeps&parent=-1'>Показать всех</a>
{/if}

{if $dep_list}
<h4>Отделы</h4>

<table border=1>
<tr><th>Название</th></tr>


{foreach from=$dep_list item=dep}
<tr>
<td><a href='?a=showdeps&parent={$dep.id}'>{$dep.name}</a> </td>
</tr>
{/foreach}

</table>
{/if}

{if $emp_list}
<h4>Персоны</h4>

<table border=1>
<tr><th>Имя</th><th>Где?</th><th>Когда?</th></tr>

{foreach from=$emp_list item=dep}
<tr>
<td>{$dep.name}</td>
<td id='{$dep.id}_location'>{if $dep.locationzone}<font color='red'>Снаружи</font>{else}<font color='green'>Внутри</font>{/if}</td>
<td id='{$dep.id}_time'><a href='?a=showperiods&id={$dep.id}' target='rightframe'>{$dep.locationact}</a></td>
</tr>
{math assign='lasttime' equation="max(x,y)" x=$lasttime y=$dep.locationact_t|default:0}
{/foreach}

</table>

{/if}

<script type="text/javascript">
var lasttime={$lasttime};
{literal}

document.body.addEventListener('click', function() {
    window.webkitNotifications.requestPermission();
  }, true);


function zerofill( a ){
   return a<10 ? "0" + a : a;
}

function updatePersons(){
{/literal}


$.getJSON('?a=showemps_json&parent={$parent_id}&time='+lasttime, function(data) {literal} {
  $.each(data, function(key, val) {
lasttime=val.time;

document.getElementById(val.id+'_location').innerHTML = val.inside? '<font color="green">Внутри</font>' : '<font color="red">Снаружи</font>';
dat=new Date(val.time*1000);
document.getElementById(val.id+'_time').getElementsByTagName('a')[0].innerHTML = dat.getFullYear() + "-" + (dat.getMonth()+1) + "-" + dat.getDate() + " " +  zerofill(dat.getHours()) + ":" + zerofill(dat.getMinutes()) + ":" + zerofill(dat.getSeconds());
  if (window.webkitNotifications.checkPermission() == 0&&data.length<5) { // 0 is PERMISSION_ALLOWED
    // function defined in step 2
    var action = val.inside ? 'вошел(а)' : 'вышел(а)';
    var pic_url = val.inside ? 'img/enter.gif' : 'img/exit.jpg';

    var notification = window.webkitNotifications.createNotification(pic_url,new Date(val.time*1000),val.name + " " + action);
    notification.show();
    setTimeout(function(){
            notification.cancel();
        }, 7000);
  }

  });
});        
}

setInterval( "updatePersons()", 2000 );
{/literal}
</script>


{/if}
{include file="footer.tpl"}
