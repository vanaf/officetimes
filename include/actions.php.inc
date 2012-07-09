<?php
class Actions {
  function Actions() {
  }

  function showdeps() {
   global $params;
   global $smarty;

   $voidPers = new Personal();
   $parent=$params['parent']?$params['parent']:0;

//   if($parent==7||$parent==9){
//$denied=1;
//   }
   if($parent>=0){
     $parent_obj=new Personal($parent);
     $parent_parent=$parent_obj->parent;
     $parentname=$parent_obj->name;
   }else{
     $parent_parent=0;
     $parentname='Все';
   }
   $depList= $voidPers->getDepsList('and parent_id=? ',array('integer'),array($parent));
   if($parent>=0){
     $empList= $voidPers->getEmpsList('and parent_id=? ',array('integer'),array($parent));
   }else{
     $empList= $voidPers->getEmpsList();

   }

   return array('denied' => $denied, 'dep_list' => $depList, 'emp_list' => $empList, 'parent_id' => $parent, 'parent' => $parentname,'parent_parent' => $parent_parent, 'title' => "Opensoft");
  }


  function showemps_json() {
   global $params;
   global $smarty;

   $voidPers = new Personal();
   $parent=$params['parent']?$params['parent']:0;
   $perid=$params['id'];
   $time=$params['time'];

   $where='';
   $wheretypearr=array();
   $wherearr=array();
   if($parent>=0){
	$where.='and parent_id=? ';
	$wheretypearr[]='integer';
	$wherearr[]=$parent;
        $parent_parent=$parent_obj->parent;
        $parentname=$parent_obj->name;
   }
   if($perid){
        $where.='and id=? ';
        $wheretypearr[]='integer';
        $wherearr[]=$perid;
   }
   if($time){
        $where.='and locationact>from_unixtime(?) ';
        $wheretypearr[]='integer';
        $wherearr[]=$time;
   }
   $empList= $voidPers->getEmpsList($where,$wheretypearr,$wherearr,'locationact');

   return array('emp_list' => $empList, 'parent' => $parentname,'parent_parent' => $parent_parent, 'title' => "Opensoft");
  }

  function showtimes() {
   global $params;
   global $smarty;

   $voidPers = new LogTime();
   $defuid=$_SESSION['userId']?$_SESSION['userId']:0;
   $logout=$params['id']?'':'<a href="?a=auth&key=0&logout=1" target="_top">Выйти</a>';
   $perid=$params['id']?$params['id']:$defuid;
   $person=new Personal($perid);
   $personname=$person->name;
   $timeList= $voidPers->getTimesList('`tc-db-log`.logs.EMPHINT=? ',array('integer','integer'),array($perid,$perid));

   return array('template' => 'showtimes','times' => $timeList, 'person' => $personname, 'title' => "Времена", 'logout' => $logout);
  }

  function showperiods() {
   global $params;
   global $smarty;

   $voidPers = new LogTime();
   $defuid=$_SESSION['userId']?$_SESSION['userId']:0;
   $logout=$params['id']?'':'<a href="?a=auth&key=0&logout=1" target="_top">Выйти</a>';
   $perid=$params['id']?$params['id']:$defuid;
   $person=new Personal($perid);
   $personname=$person->name;
   $hours=$params['hours']>0?$params['hours']:40;
   $defperiod=$_SESSION['group']?$_SESSION['group']:'week';
   $period=$params['period']?$params['period']:$defperiod;
   if($period=='month'){
     $timeList= $voidPers->getPeriodsList('EMPHINT=? ',array('integer','integer','integer'),array($perid,$perid,$perid), 'DATE(enter_time) - INTERVAL DAYOFMONTH(enter_time)-1 day');
   }else{
     $timeList= $voidPers->getPeriodsList('EMPHINT=? ',array('integer','integer','integer'),array($perid,$perid,$perid)); 
   }
$secleft=$params['hours']?$params['hours']*3600-$timeList[0]['secduration']:0;


   return array('template' => 'showperiods','times' => $timeList, 'secLeft' => $secleft, 'perid' => $perid,  'person' => $personname, 'title' => "Времена", 'logout' => $logout,'hours' => $hours,'period' => $period, 'req_hours' => $params['hours'] );
  }


  function auth() {
   global $params;
   global $smarty;

   if($params['submit']){

    $voidPers = new Personal();
    $key=$params['key']?$params['key']:0;

    $empList= $voidPers->getEmpsList('and SUBSTRING( HEX(  `CODEKEY` ) , 5, 4 ) = LPAD(HEX(?),4,"0") ',array('integer'),array($key));
    if($empList&&$empList[0]['id']){
     $_SESSION['userId']=$empList[0]['id'];
     session_write_close();
     return $this->showperiods();
    }
    return array('content' => "Ошибка входа. Пользователя с таким кодом не существует. Возможно, Вы опечатались. Проверьте, не включена ли у Вас русская раскладка клавиатуры или режим CapsLock. Если эта ошибка повторится еще 2 раза, обратитесь в службу поддержки.", 'title' => "Неудачный вход в систему"); 
   }

  }


}
?>
