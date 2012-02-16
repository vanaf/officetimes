<?php
ini_set('session.auto_start','1');
 session_start();
 require_once 'require.php.inc';

setlocale(LC_ALL, 'ru_RU.utf8');

 $params = $_SERVER['REQUEST_METHOD'] == 'POST' ? $_POST : $_GET;

 $action=strtolower($params['a']);

$defact= $_SESSION['userId']?'showperiods':'auth';

 $action = $action ? $action : $defact;

 $smarty = new Smarty;

 
 $content=array();
 $acts = new Actions();
 $template=$action;

 if (method_exists($acts, $action)) {
   $content=$acts->$action();
   $smarty->assign($content);
   $template=$content['template']?$content['template']:$template;
   if(!$smarty->templateExists($template.'.tpl')){
     $template='default';
   }

 }else{
   if(!$smarty->templateExists($template.'.tpl')){
     $template='noAction';
   }
 }




 $smarty->display($template.'.tpl');  


?>
