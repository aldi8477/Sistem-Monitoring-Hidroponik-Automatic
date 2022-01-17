<?php
session_start();
if (empty($_SESSION['akun'])) {
    include 'login.php';
    die;
} else {
   include 'home.php';   
}
?>