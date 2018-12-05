/*  Checks if password and confirm_password matche */
var check = function() {
  var pass = document.getElementsByClassName('password')[0];
  var passrep = document.getElementsByClassName('confirm_password')[0];

/*  Checks password min length*/
  if (pass.value == passrep.value) {
    document.getElementById('message').style.color = 'green';
    document.getElementById('message').innerHTML = 'OK';
    document.getElementById('regButton').disabled = false;
    document.getElementById('regButton').style.opacity = 1;

    if (pass.value.length < 5)  {
      document.getElementById('messageShort').style.color = 'red';
      document.getElementById('messageShort').innerHTML = 'Heslo je príliš krátke';
      document.getElementById('regButton').disabled = true;
      document.getElementById('regButton').style.opacity = 0.5;
      document.getElementById('message').innerHTML = '';
    }
    else  {
      document.getElementById('messageShort').innerHTML = '';
    }
  }
   else {
     if (pass.value.length < 5)  {
       document.getElementById('messageShort').style.color = 'red';
       document.getElementById('messageShort').innerHTML = 'Heslo je príliš krátke';
       document.getElementById('regButton').disabled = true;
       document.getElementById('regButton').style.opacity = 0.5;
       document.getElementById('message').innerHTML = '';
     }
     else {
       document.getElementById('messageShort').innerHTML = '';
     }
    document.getElementById('message').style.color = 'red';
    document.getElementById('message').innerHTML = 'Heslá sa nezhodujú';
    document.getElementById('regButton').disabled = true;
    document.getElementById('regButton').style.opacity = 0.5;
  }
}

var loginControl = function()  {
  var logincheck = document.getElementsByClassName('loginCheck')[0];
  if(logincheck.value.length < 5) {
    document.getElementById('messageLoginShort').style.color = 'red';
    document.getElementById('messageLoginShort').innerHTML = 'Login je príliš krátky';
    document.getElementById('regButton').disabled = true;
    document.getElementById('regButton').style.opacity = 0.5;
  }
  else {
    document.getElementById('messageLoginShort').innerHTML = '';
  }
}
