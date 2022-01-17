<?php

if (isset($_GET['page'])) {
    switch ($_GET['page']) {
        case 'devices':
        include 'pages/devices.php';
        break;
        
        default:
        include 'pages/dashboard.php';
        break;
    }
} else {
    include 'pages/dashboard.php';
}