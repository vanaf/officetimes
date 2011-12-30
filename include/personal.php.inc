<?php
 class Personal {
  var $db, $id, $nick,$email, $groups;

  function Personal($id = 0, $parent=0, $type = 'EMP', $name = '', $pos = '', $lastzone = '', $lasttime='') {
    global $db;
    $this->db = $db;
    $this->id = $id;
    $this->parent = $parent;
    $this->type = $type;
    $this->name = $name;
    $this->pos = $pos;
    $this->lastzone = $lastzone;
    $this->lasttime = $lasttime;

    if ($this->id) {
     $this->getData();
    }

  }


  private function getData() {
   $sth = $this->db->prepare("select parent_id,type,name,pos,locationzone,locationact from personal where id=?",array('integer'));

   $r=$sth->execute(array($this->id));
   if($r){
    $row=$r->fetchRow();
    $this->parent = $row['parent_id'];
    $this->type = $row['type'];
    $this->name = $row['name'];
    $this->pos = $row['pos'];
    $this->lastzone = $row['locationzone'];
    $this->lasttime = $row['locationact'];
    return 1;
   }else{
    return 0;
   }    
  }


  function getDepsList($where='',$wherearrtype='',$wherearr='') {
   if(!$where){
	$sth = $this->db->prepare("select id,parent_id,name,type,pos,locationzone,locationact from personal where type='DEP' and status='AVAILABLE' order by name",array());
	$r=$sth->execute(array());
   }else{
        $sth = $this->db->prepare("select id,parent_id,name,type,pos,locationzone,locationact from personal where type='DEP' and status='AVAILABLE' $where order by name ",$wherearrtype);
        $r=$sth->execute($wherearr);
   }
   if($r){
    return $list=$r->fetchAll();
   }else{
    return array();
   }
   
  }

  function getEmpsList($where='',$wherearrtype='',$wherearr='',$orderby='name') {
   if(!$where){
        $sth = $this->db->prepare("select id,parent_id,name,type,pos,locationzone,locationact, unix_timestamp(locationact) as locationact_t from personal where type='EMP' and status='AVAILABLE' order by type,name",array());
        $r=$sth->execute(array());
   }else{
        $sth = $this->db->prepare("select id,parent_id,name,type,pos,locationzone,locationact, unix_timestamp(locationact) as locationact_t from personal where type='EMP' and status='AVAILABLE' $where order by $orderby ",$wherearrtype);
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
