{include file="header.tpl"}


{if $parent}
<ul class="pager"><li class="previous"><a href='?a=showdeps&parent={$parent_parent}' xmlns="http://www.w3.org/1999/html" xmlns="http://www.w3.org/1999/html">На уровень выше</a></li></ul>

{/if}
<h1 id="parent">{$parent}</h1>

{assign var='lasttime' value='0'}

{if $denied}
{else}



<div id="wl">
{if $parent_id>=0}
{*<div id="showall"><a href='?a=showdeps&parent=-1'>Показать всех</a></div>*}
{/if}

<div id="deplist">{if $dep_list}
    <div id="showall"><a href='?a=showdeps&parent=-1'>Показать всех</a></div>
    <div id="linkstmar"> {foreach from=$dep_list item=dep}
    <div class="linkst"><a href='?a=showdeps&parent={$dep.id}'>{$dep.name}</a></div>
    {/foreach}</div>
</div>


{/if}
</div>

{if $emp_list}
        <table id="fruitbox">
        <thead>
<tr ><th>Имя</th><th>Где?</th><th>Когда?</th></tr>
        </thead>
      <tbody>
{foreach from=$emp_list item=dep}
    
<tr >
    <td class="firstcolumn">{$dep.name}</td>
    <td class="secondcolumn" id='{$dep.id}_location'>{if $dep.locationzone}<span class="label label-important">Снаружи</span>{else}<span class="label2 label-success"> Внутри </span>{/if}</td>
    <td class="thirdcolumn" id='{$dep.id}_time'><a href='?a=showperiods&id={$dep.id}' target='rightframe'>{$dep.locationact}</a></td>
</tr>
{math assign='lasttime' equation="max(x,y)" x=$lasttime y=$dep.locationact_t|default:0}
{/foreach}
        </tbody>
</table>



{/if}
</ul></div>

<script type="text/javascript">
    $(document).ready(function() 
    { 
        $("#fruitbox").tablesorter(); 
        DCH.wearchanger();
    } 
    ); 
        
        </script>


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

document.getElementById(val.id+'_location').innerHTML = val.inside? '<span class="label label-success">Внутри</span>' : '<span class="label label-important">Снаружи</span>';
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


