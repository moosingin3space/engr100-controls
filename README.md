# Code for ENGR 100 Blimp

## 2015-04-07
Discovered problems with trimless servo controls. Current revision of
trimless control is in the branch `trimless` and is being debugged
independently.

## 2015-03-28
Implemented trimless elevator and throttle controls (merged from branch
`gains`) and successfully flew the blimp.

## 2015-03-27
Added working H-Bridge code (merged from branch `h-bridge`) and successfully
flew the blimp.

## 2015-03-26
Removed port reconfiguration for increased performance and vastly simpler
code.

## 2015-03-19

The Arduino no longer outputs debugging information, and as such has
faster ISRs.

## 2015-03-17

The Arduino now implements amplifier offsets for servo control. These
are configurable in the `localdefs.h` file as well.

## 2015-03-15

The Arduino now implements basic gains for servo control. All code is 
configurable using the `localdefs.h` file.

## 2015-03-11

The Arduino successfully reads signals from the receiver and successfully
controls servos. We have established the general form of all additional
code that will be required for driving.
