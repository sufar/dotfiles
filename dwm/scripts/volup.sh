#!/bin/bash
#amixer -M set Master 5%+ unmute

#amixer sset Master 5%+ unmute
#amixer -c 0 set PCM 5%+ unmute


sink=$(pactl info | grep 'Default Sink' | awk '{print $3}')
vol=$(pactl list sinks | grep $sink -A 7 | sed -n '8p' | awk '{printf int($5)}')
mod=$((vol % 5))

target="+$((5 - mod))%"

pactl set-sink-volume @DEFAULT_SINK@ $target
