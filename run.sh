#! /bin/bash
# Switch off automation
v4l2-ctl -c exposure_auto_priority=0
v4l2-ctl -c exposure_auto=1
v4l2-ctl -c backlight_compensation=0
v4l2-ctl -c white_balance_temperature_auto=0
v4l2-ctl -c power_line_frequency=0

#Set shutter to 1 second
v4l2-ctl -c exposure_absolute=10000
# Amplify as much as possible
v4l2-ctl -c gain=255
v4l2-ctl -c contrast=255
v4l2-ctl -c brightness=242

v4l2-ctl --set-fmt-video=width=1280,height=960,pixelformat=0
#v4l2-ctl -c --set-param=1
v4l2-ctl -c white_balance_temperature_auto=0
# Non-critical
v4l2-ctl -c saturation=32
v4l2-ctl -c white_balance_temperature=6500

./Trigger
#timeout 1800 ./Trigger
