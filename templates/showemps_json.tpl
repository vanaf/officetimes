[{if $emp_list}{foreach from=$emp_list item=dep key=num_key}{literal}{{/literal}"id": {$dep.id},"name": "{$dep.name}","parent": "{$dep.parent_id}","inside": {if $dep.locationzone}0{else}1{/if},"time": {$dep.locationact_t|default:0} {literal}}{/literal}{if count($emp_list)-1 != $num_key},{/if}{/foreach}{/if}]