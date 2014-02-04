var DCH = {


 wearchanger: function(){ 
  var data0 = parseInt(new Date().getTime() / 1000);
  var day0 = new Date().getDay();
  var mess = '';
  var mn = ["Января","Февраля","Марта","Апреля","Мая","Июня","Июля","Августа","Сентября","Октября","Ноября","Декабря"];
  
  $('.thirdcolumn a').each(function () {
    var val = $(this).text();
    var month = new Date(val).getMonth();
    var day1 = new Date(val).getDay();
    var data1 = parseInt(new Date(val).getTime() / 1000);
    
    mess = '';
    
    if (data0 - data1 < 172800 && day0>day1) { 
        mess = 'Bчера,';         
    } else if (data0 - data1 < 86400 && day0==day1) {
        mess = ' ';        
    } else {
        mess = mn[month];
    }
    
    
    if (mess == 'Bчера,'||mess == ' ') {
        val = val.replace(val.substr(0, 10), mess);
        var val2 = val.replace(val.substr(-3), ' ');
        $(this).html(val2);
    } else {
        var val0 = val.substr(8, 2);
        val = val.replace(val.substr(0, 10), val0 + ' ' + mess +','+' ');
        var val3 = val.replace(val.substr(-3), ' ');
        $(this).html(val3);
    }
  });  

 }
 }

 

 $(document).ready(function(){
 });