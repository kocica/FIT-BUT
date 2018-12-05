
function getButton() {
  return document.getElementsByClassName("loginButtons");
}

function wrongPass()  {
  var modal_register = document.getElementById("psw-repeat");
  modal_register.style.color = "Red";
}

function keyPress (e) {
    if(e.key === "Escape") {
        return true;
    }
}

/***************  Registration or login  **************/
// Get the modal
var modal_boss = document.getElementById('modalBoss');
var modal_keeper = document.getElementById('modalKeeper');
var modal_registration = document.getElementById('modalReg');

// Get buttons that opens the modal
var button_boss = document.getElementsByClassName("loginButtons")[0];//getButton();
var button_keeper = document.getElementsByClassName("loginButtons")[1];
var button_registration = document.getElementsByClassName("loginButtons")[2];//getButton();

// Get the <span> element that closes the modal
var span_boss = document.getElementsByClassName("close")[0]
var span_keeper = document.getElementsByClassName("close")[1]
var span_registration = document.getElementsByClassName("close")[2];

window.onload = function() {
  modal_boss.style.display = "none";
  modal_keeper.style.display = "none";
  modal_registration.style.display = "none";
}

// When the user clicks the button, open the modal
button_boss.onclick = function() {
    var modal_main = document.getElementsByClassName("modal")[0];
    modal_main.style.opacity = "1.0";
    modal_boss.style.display = "block";
    modal_keeper.style.display = "none";
    modal_registration.style.display = "none";
}
button_keeper.onclick = function() {
    var modal_main = document.getElementsByClassName("modal")[1];
    modal_main.style.opacity = "1.0";
    modal_keeper.style.display = "block";
    modal_boss.style.display = "none";
    modal_registration.style.display = "none";
}
button_registration.onclick = function() {
    var modal_main = document.getElementsByClassName("modal")[2];
    modal_main.style.opacity = "1.0";
    modal_registration.style.display = "block";
    modal_boss.style.display = "none";
    modal_keeper.style.display = "none";
}

// When the user clicks on <span> (x), close the modal
span_boss.onclick = function() {
    modal_boss.style.display = "none";
}
span_keeper.onclick = function() {
    modal_keeper.style.display = "none";
}
span_registration.onclick = function() {
    modal_registration.style.display = "none";
}

document.onkeydown = function(evt) {
    evt = evt || window.event;
    var isEscape = false;
    if ("key" in evt) {
        isEscape = (evt.key == "Escape" || evt.key == "Esc");
    } else {
        isEscape = (evt.keyCode == 27);
    }
    if (isEscape) {
      modal_boss.style.display = "none";
      modal_keeper.style.display = "none";
      modal_registration.style.display = "none";
    }
};

// When the user clicks anywhere outside of the modal, close it TODO
/*
window.onclick = function(event) {
    if (event.target == modal_boss) {
        modal_boss.style.display = "none";
    }
    else if (event.target == modal_keeper) {
        modal_keeper.style.display = "none";
    }
    else if (event.target == modal_registration) {
        modal_registration.style.display = "none";
    }
}
*/
