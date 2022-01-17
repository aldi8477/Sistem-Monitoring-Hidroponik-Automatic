<?php
include 'include/koneksi.php';
if (isset($_POST['login'])) {
    $username = $_POST['username'];
    $password = $_POST['password'];
    
    $cek = $koneksi->query("SELECT * FROM tb_user WHERE username = '$username'")->fetch_assoc();
    
    if ($cek) {
        if (password_verify($password, $cek['password'])) {
            session_start();
            $_SESSION['akun'] = $cek;
            echo 1;
        } else {
            echo 2;
        }
    } else {
        echo 2;
    }
    
    die;
}
?>
<!DOCTYPE html>
<html lang="en">

<head>

<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
<meta name="description" content="">
<meta name="author" content="">

<title>Login</title>

<link href="https://fonts.googleapis.com/css?family=Nunito:200,200i,300,300i,400,400i,600,600i,700,700i,800,800i,900,900i"
rel="stylesheet">

<link rel="stylesheet" href="/assets/css/style.css">
<link href="/assets/css/bootstrap/bootstrap.min.css" rel="stylesheet">

</head>

<body>
<div class="container">

<div class="row justify-content-center">

<div class="col-md-5">

<div class="card o-hidden border-0 shadow-lg my-5">
<div class="card-body p-0">
<div class="row">
<div class="col-lg-12">
<div class="p-5">
<img src="/assets/img/logo.png">
<div class="text-center">
<h1 class="h4 text-gray-900 mb-4">Hidroponik Automatic</h1>
</div>
<div class="user">
<div class="form-group">
<input type="text" class="form-control form-control-user" id="username" name="username" placeholder="Username">
</div>
<div class="form-group">
<input type="password" class="form-control form-control-user" id="password" name="password" placeholder="Password">
</div>
<button id="login" class="btn btn-success btn-user btn-block">Login</button>
</div>
<hr>
</div>
</div>
</div>
</div>
</div>

</div>

</div>

</div>

<script src="/assets/vendor/jquery/dist/jquery.min.js"></script>
<script src="/assets/vendor/sweetalert2/dist/sweetalert2.all.min.js"></script>
<script>
$(document).ready(function () {
    $("#login").on('click', function() {
        let username = $('#username').val().trim();
        let password = $('#password').val().trim();
        
        if (username == '' || password == '') {
            Swal.fire('Ooops!','Form login harap diisi!','error')
        } else {
            $.ajax({
                url: './login.php',
                type: "POST",
                data: {username: username, password: password, login: 'login'},
                success: function (respon) {
                    let timerInterval
                    Swal.fire({
                        title: 'Harap tunggu',
                        html: 'Silahkan tunggu beberapa detik.',
                        timer: 2000,
                        timerProgressBar: true,
                        didOpen: () => {
                            Swal.showLoading()
                        },
                        willClose: () => {
                            clearInterval(timerInterval)
                            response(respon)
                        }
                    }).then((result) => {
                        if (result.dismiss === Swal.DismissReason.timer) {
                            response(respon)
                        }
                    })
                    
                }
            })
        }
    })
})

function response(params) {
    if (params == 1) {
        location.href = './';
    } else {
        Swal.fire('Ooops!','Akun anda tidak terdaftar!','error')
    }
}
</script>

</body>
</html>