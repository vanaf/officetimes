<?php
 require_once("PEAR.php");
 require_once("MDB2.php");
 global $config;

 $db = MDB2::connect($config['db']['connectString']);
 
if (PEAR::isError($db)) {
    /*
     * This is not what you would really want to do in
     * your program.  It merely demonstrates what kinds
     * of data you can get back from error objects.
     */
    echo 'Standard Message: ' . $db->getMessage() . "\n";
    echo 'Standard Code: ' . $db->getCode() . "\n";
echo 'debug: ' . $db->getDebugInfo() . "\n";
    exit;
}

$db->setOption('debug', 1);
 $db->setFetchMode(MDB2_FETCHMODE_ASSOC);
 $db->setErrorHandling(PEAR_ERROR_PRINT);

?>
