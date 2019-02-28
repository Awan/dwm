#!/bin/sh


$HOME/.fehbg 
xbacklight = 5 
pactl set-source-mute auto_null.monitor true 
xset dpms 180  
unclutter -root -reset -idle 1 -noevents
xss-lock -- slock
