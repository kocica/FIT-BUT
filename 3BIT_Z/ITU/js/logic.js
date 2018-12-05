var id = document.getElementById("buttons");
var items = id.getElementsByTagName("div");

var current = 0;

function animate(){
  $(items[current]).addClass("animate");
  
  if (current == items.length){
    
  }else{
    current++
    setTimeout(function(){ 
      animate();
  }, 300);
  }
}

  setTimeout(function(){ 
      animate();
  }, 800);

var resizeTimer;

$(window).on('resize', function(e) {
  
  $(items).removeClass("animate");
  
  clearTimeout(resizeTimer);
  resizeTimer = setTimeout(function() {
    
    current = 0;
    animate();

  }, 250);

});

function changeIcon()
{
  if (document.getElementById("playStop").src.includes("play.png"))
  {
    document.getElementById("playStop").src="../img/stop.png";
    document.getElementById("currentlyPlaying").textContent = "Právě hraje: Antonín Dvořák – Symfonie č. 9";
    playSound();
  }
  else
  {
    document.getElementById("playStop").src="../img/play.png";
    document.getElementById("currentlyPlaying").textContent = "Přehrávání vypnuto";
    stopSound();
  }
}

function changeIconMusicTest()
{
  if (document.getElementById("playStop").src.includes("play.png"))
  {
    document.getElementById("playStop").src="../img/stop.png";
    playSound();
  }
  else
  {
    document.getElementById("playStop").src="../img/play.png";
    stopSound();
  }
}

var audioElement = null;
function playSound()
{
  if (!audioElement)
  {
    audioElement = document.createElement('audio');
    audioElement.setAttribute('src', '../audio/DvorakSymphonyNo.9.mp3');
    audioElement.setAttribute('autoplay', 'autoplay');
    audioElement.load();
  }
  audioElement.play();
}

function stopSound()
{
  audioElement.pause();
}

function changeMusic()
{
  document.getElementById("playStop").src="../img/stop.png";
  document.getElementById("currentlyPlaying").textContent = "Právě hraje: Antonín Dvořák – Symfonie č. 9";
  if (audioElement)
  {
    audioElement.pause();
    audioElement = null;
  }
  playSound();
}