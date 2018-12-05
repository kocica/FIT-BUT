<?php

    function isInSession($str)
    {
        if (isset($_SESSION[$str]))
        {
            $jmenoVybehu = $_SESSION[$str];
            unset($_SESSION[$str]);
            return $jmenoVybehu;
        }
    }

?>