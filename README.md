# mustang-raider

Allow Fender Mustang series guitar amplifiers to be controlled by command line and USB footpedals

# Introduction

Mustang-raider is a fork of Mustang-bridge with the midi part stripped down and changed to accept parameters via command line or work in deamon mode with USB footpedals 

Special thanks to (similar to mustang-bridge project!):

  + The original developer and contributors to 'PLUG' who blazed the
  path with reverse-engineering of Fender's communication scheme.

  + Robert Fransson (Codesmart on VGuitar Forums) for feedback and
  encouragement - and for being a tireless programming wizard :-).

  + Robert Heitman, author of the Android 'Remuda' application for
  Mustang amp control, who provided valuable insights into various
  subtleties to snhirsch

 + snhirsch, for the work with mustang-bridge that i barely stole

# For everyone

Stay tuned for the first application!

# Status

 Need hand configuration, only working command are tuner on and off, program change, work only with one foot USB controller. See below TODO

# Build
```
$ make
```

# Configure

 TO BE DONE

# Install

 TO BE DONE

# Run

 TO BE DONE

# In case of difficulty

Drop some line on my blog! :)

# TODO

- clean and organize output
- move preset dumps to a folder
- dump preset restore
- single parameter control via command line
- support for 3 pedal USB (only need to buy one to test, in fact)
- support for 2 different usb pedal devices ( 1+1 single pedal, 1 single + 1 triple, 2 triple pedal)
- installation guide (will buy a new raspberry with 3.5 display very soon)
- support for configuration file
- fully configurable footswitch behavior via configuration file or command line
- graphical fronted for a 3.5' display resolution
- support for preset editing via touch display
- increase responsivness of preset change


