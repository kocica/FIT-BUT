/***************  Menu popouts  **************/
//modals
var modal_cleaning = document.getElementById("modalCleaning");
var modal_feeding = document.getElementById("modalFeeding");
var modal_animal = document.getElementById("modalAnimal");
var modal_fence = document.getElementById("modalFence");

//button/text field which opened the modal
var cleaning_opener = document.getElementsByClassName("cleaningMenu")[0];
var cleaning_opener2 = document.getElementsByClassName("cleaningMenu")[1]

var feeding_opener = document.getElementsByClassName("feedingMenu")[0];
var feeding_opener2 = document.getElementsByClassName("feedingMenu")[1];

var animal_opener = document.getElementsByClassName("animalMenu")[0];
var animal_opener2 = document.getElementsByClassName("animalMenu")[1];

var fence_opener = document.getElementsByClassName("fenceMenu")[0];
var fence_opener2 = document.getElementsByClassName("fenceMenu")[1];

//Get the <span> element that closes the modal
var span_cleaning_menu = document.getElementsByClassName("close")[0];
var span_feeding_menu = document.getElementsByClassName("close")[1];
var span_animal_menu = document.getElementsByClassName("close")[2];
var span_fence_menu = document.getElementsByClassName("close")[3];


window.onload = function() {
  modal_cleaning.style.display = "none";
  modal_feeding.style.display = "none";
  modal_animal.style.display = "none";
  modal_fence.style.display = "none";
  var modal_main = document.getElementsByClassName("modal")[0];
  modal_main.style.opacity = "0.0";
}
/*  cleaning  */
cleaning_opener.onclick = function()  {
  var modal_main = document.getElementsByClassName("modal")[0];
  modal_main.style.opacity = "1.0";
  modal_cleaning.style.display = "block";
  modal_feeding.style.display = "none";
  modal_animal.style.display = "none";
  modal_fence.style.display = "none";
}
cleaning_opener2.onclick = function()  {
  var modal_main = document.getElementsByClassName("modal")[0];
  modal_main.style.opacity = "1.0";
  modal_cleaning.style.display = "block";
  modal_feeding.style.display = "none";
  modal_animal.style.display = "none";
  modal_fence.style.display = "none";
}
span_cleaning_menu.onclick = function() {
  modal_cleaning.style.display = "none";
}
/*  feeding */
feeding_opener.onclick = function()  {
  var modal_main = document.getElementsByClassName("modal")[1];
  modal_main.style.opacity = "1.0";
  modal_feeding.style.display = "block";
  modal_cleaning.style.display = "none";
  modal_animal.style.display = "none";
  modal_fence.style.display = "none";
}
feeding_opener2.onclick = function()  {
  var modal_main = document.getElementsByClassName("modal")[1];
  modal_main.style.opacity = "1.0";
  modal_feeding.style.display = "block";
  modal_cleaning.style.display = "none";
  modal_animal.style.display = "none";
  modal_fence.style.display = "none";
}
span_feeding_menu.onclick = function() {
  modal_feeding.style.display = "none";
}
/*  animals */
animal_opener.onclick = function()  {
  var modal_main = document.getElementsByClassName("modal")[2];
  modal_main.style.opacity = "1.0";
  modal_animal.style.display = "block";
  modal_cleaning.style.display = "none";
  modal_feeding.style.display = "none";
  modal_fence.style.display = "none";
}
animal_opener2.onclick = function()  {
  var modal_main = document.getElementsByClassName("modal")[2];
  modal_main.style.opacity = "1.0";
  modal_animal.style.display = "block";
  modal_cleaning.style.display = "none";
  modal_feeding.style.display = "none";
  modal_fence.style.display = "none";
}
span_animal_menu.onclick = function() {
  modal_animal.style.display = "none";
}
/*  fences  */
fence_opener.onclick = function()  {
  var modal_main = document.getElementsByClassName("modal")[3];
  modal_main.style.opacity = "1.0";
  modal_fence.style.display = "block";
  modal_cleaning.style.display = "none";
  modal_feeding.style.display = "none";
  modal_animal.style.display = "none";
}
fence_opener2.onclick = function()  {
  var modal_main = document.getElementsByClassName("modal")[3];
  modal_main.style.opacity = "1.0";
  modal_fence.style.display = "block";
  modal_cleaning.style.display = "none";
  modal_feeding.style.display = "none";
  modal_animal.style.display = "none";
}
span_fence_menu.onclick = function() {
  modal_fence.style.display = "none";
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
      modal_cleaning.style.display = "none";
      modal_feeding.style.display = "none";
      modal_animal.style.display = "none";
      modal_fence.style.display = "none";
    }
};
