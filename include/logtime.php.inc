<?php
 class LogTime {
  var $db, $id, $nick,$email, $groups;

  function LogTime($id = 0, $logtime=0, $emp = 0, $direction=0) {
    global $db;
    $this->db = $db;
    $this->id = $id;
    $this->logtime = $logtime;
    $this->emp = $emp;
    $this->direction = $direction;

    if ($this->id) {
     $this->getData();
    }

  }


  private function getData() {
   $sth = $this->db->prepare("select logtime,emphint,SUBSTRING( HEX(  `LOGDATA` ) , 10, 1 ) as direction from tc-db-log.logs where id=?",array('integer'));

   $r=$sth->execute(array($this->id));
   if($r){
    $row=$r->fetchRow();
    $this->logtime = $row['logtime'];
    $this->emphint = $row['emphint'];
    $this->direction = $row['direction'];
    return 1;
   }else{
    return 0;
   }    
  }


  function getTimesList($where='',$wherearrtype='',$wherearr='') {
   if(!$where){
	$sth = $this->db->prepare("select id,logtime,emphint,date(logtime) as ldate,time(logtime) as ltime, SUBSTRING( HEX(  `LOGDATA` ) , 10, 1 ) as direction from `tc-db-log`.logs order by logtime desc limit 300",array());
	$r=$sth->execute(array());
   }else{

        $sth = $this->db->prepare("
SELECT id, logtime,  `tc-db-log`.logs.emphint, DATE( logtime ) AS ldate, TIME( logtime ) AS ltime, SUBSTRING( HEX(  `LOGDATA` ) , 10, 1 ) as direction , tdiff
FROM  `tc-db-log`.logs, (

SELECT if(count(logtime)<=1 and date(logtime)=curdate(),TIMEDIFF( now() , MIN( logtime ) ), TIMEDIFF( MAX( logtime )  , MIN( logtime ) )) AS tdiff, DATE( logtime ) AS ldate2, emphint
FROM  `tc-db-log`.logs
WHERE $where
GROUP BY DATE( logtime ) , emphint
) AS diffs
WHERE DATE( logtime ) = ldate2
AND `tc-db-log`.logs.EMPHINT = diffs.emphint
AND $where
ORDER BY logtime DESC 
LIMIT 300",$wherearrtype);
        $r=$sth->execute($wherearr);
   }
   if($r){
    return $list=$r->fetchAll();
   }else{
    return array();
   }
   
  }

  function getPeriodsList($where='',$wherearrtype='',$wherearr='',$groupfunc='YEARWEEK( enter_time )') {
   if(!$where){
        $sth = $this->db->prepare("
SELECT emphint, lweek, ldate, sduration, kolvo, time(enter_time) as enter_time, if(date(exit_time)=ldate,time(exit_time),exit_time) AS exit_time
FROM (

SELECT emphint,$groupfunc as lweek , DATE( enter_time ) as ldate , SEC_TO_TIME( SUM( TIME_TO_SEC( duration ) ) ) as sduration, count(enter_time) as kolvo, enter_time, exit_time
FROM (

SELECT emphint, TIMEDIFF( if(direction>0, NULL, 
if(min_exit_time is null and date(enter_time)=curdate(), now(), min_exit_time )) , enter_time ) AS duration, enter_time, if(direction>0, NULL,min_exit_time) as exit_time 
FROM (

SELECT enters.EMPHINT as emphint,enter_time,MIN( exit_time) AS min_exit_time, (select count(*) from `tc-db-log`.logs where LOGTIME=MIN( exit_time ) and emphint=enters.EMPHINT and SUBSTRING( HEX(  `LOGDATA` ) , 10, 1 ) =  '2' ) as direction FROM (

SELECT  EMPHINT,`LOGTIME` AS enter_time
FROM `tc-db-log`.LOGS WHERE  `LOGTIME`>CURDATE() - INTERVAL 2 MONTH
AND SUBSTRING( HEX(  `LOGDATA` ) , 10, 1 ) =  '2'
ORDER BY logtime DESC 
LIMIT 100
) AS enters
LEFT OUTER JOIN (

SELECT EMPHINT,`LOGTIME` AS exit_time
FROM `tc-db-log`.LOGS WHERE `LOGTIME`>CURDATE() - INTERVAL 2 MONTH
ORDER BY logtime DESC 
LIMIT 200
) AS exits ON enters.enter_time < exits.exit_time and enters.EMPHINT=exits.EMPHINT
GROUP BY enter_time DESC, enters.EMPHINT
) AS periods
) AS periods2
GROUP BY $groupfunc desc , DATE( enter_time ) desc ,enter_time DESC 
WITH rollup
HAVING lweek is not null

) as fororder

ORDER BY lweek desc, ifnull(ldate,'2099-01-01') desc, ifnull(enter_time,'2099-01-01') desc
",array());
        $r=$sth->execute(array());
   }else{

        $sth = $this->db->prepare("
SELECT lweek, ldate, sduration,secduration, kolvo, time(enter_time) as enter_time, unix_timestamp(enter_time) as enter_time_t, if(date(exit_time)=ldate,time(exit_time),exit_time) AS exit_time, unix_timestamp(exit_time) as exit_time_t 
FROM (

SELECT $groupfunc as lweek , DATE( enter_time ) as ldate , SEC_TO_TIME( SUM( TIME_TO_SEC( duration ) ) ) as sduration,SUM( TIME_TO_SEC( duration ) ) as secduration, count(enter_time) as kolvo,enter_time, exit_time
FROM (

SELECT TIMEDIFF( if(direction>0, NULL, 
if(min_exit_time is null and date(enter_time)=curdate(), now(), min_exit_time )) , enter_time ) AS duration, enter_time, if(direction>0, NULL,min_exit_time) as exit_time
FROM (
SELECT enter_time,MIN( exit_time) AS min_exit_time, (select count(*) from `tc-db-log`.logs where LOGTIME=MIN( exit_time ) and $where and SUBSTRING( HEX(  `LOGDATA` ) , 10, 1 ) =  '2' ) as direction FROM (

SELECT  `LOGTIME` AS enter_time
FROM `tc-db-log`.LOGS WHERE  `LOGTIME`>CURDATE() - INTERVAL 2 MONTH AND $where
AND SUBSTRING( HEX(  `LOGDATA` ) , 10, 1 ) =  '2'
ORDER BY logtime DESC 
LIMIT 200
) AS enters
LEFT OUTER JOIN (

SELECT `LOGTIME` AS exit_time
FROM `tc-db-log`.LOGS WHERE `LOGTIME`>CURDATE() - INTERVAL 2 MONTH AND $where
ORDER BY logtime DESC 
LIMIT 400
) AS exits ON enters.enter_time < exits.exit_time
GROUP BY enter_time DESC
) AS periods
) AS periods2
GROUP BY $groupfunc desc , DATE( enter_time ) desc, enter_time DESC 
WITH rollup
HAVING lweek is not null

) as fororder

ORDER BY lweek desc, ifnull(ldate,'2099-01-01') desc, ifnull(enter_time,'2099-01-01') desc
",$wherearrtype);
        $r=$sth->execute($wherearr);
   }
   if($r){
    return $list=$r->fetchAll();
   }else{
    return array();
   }

  }


 }
?>
