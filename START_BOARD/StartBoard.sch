EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:akizuki
LIBS:renesas
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L R5F100_REAL U?
U 1 1 57AA58FF
P 5900 3700
F 0 "U?" H 7550 5650 60  0000 C CNN
F 1 "R5F100LxAFB" H 4500 1750 60  0000 C CNN
F 2 "QFP64" H 5900 5650 60  0000 C CNN
F 3 "" H 7500 5650 60  0000 C CNN
	1    5900 3700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57AA5A71
P 3900 5750
F 0 "C?" H 3925 5850 50  0000 L CNN
F 1 "1uF" H 3925 5650 50  0000 L CNN
F 2 "" H 3938 5600 50  0000 C CNN
F 3 "" H 3900 5750 50  0000 C CNN
	1    3900 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 5600 3900 3100
Wire Wire Line
	3900 3100 4000 3100
$Comp
L GND #PWR?
U 1 1 57AA5C37
P 3900 6000
F 0 "#PWR?" H 3900 5750 50  0001 C CNN
F 1 "GND" H 3900 5850 50  0000 C CNN
F 2 "" H 3900 6000 50  0000 C CNN
F 3 "" H 3900 6000 50  0000 C CNN
	1    3900 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 5900 3900 6000
$Comp
L GND #PWR?
U 1 1 57AA5C61
P 3750 3600
F 0 "#PWR?" H 3750 3350 50  0001 C CNN
F 1 "GND" H 3750 3450 50  0000 C CNN
F 2 "" H 3750 3600 50  0000 C CNN
F 3 "" H 3750 3600 50  0000 C CNN
	1    3750 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3200 3750 3600
Wire Wire Line
	3750 3300 4000 3300
Wire Wire Line
	3750 3200 4000 3200
Connection ~ 3750 3300
$Comp
L +3.3V #PWR?
U 1 1 57AA5C8A
P 3550 3400
F 0 "#PWR?" H 3550 3250 50  0001 C CNN
F 1 "+3.3V" H 3550 3540 50  0000 C CNN
F 2 "" H 3550 3400 50  0000 C CNN
F 3 "" H 3550 3400 50  0000 C CNN
	1    3550 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4000 3400 3550 3400
Wire Wire Line
	4000 3500 3650 3500
Wire Wire Line
	3650 3500 3650 3400
Connection ~ 3650 3400
$Comp
L C C?
U 1 1 57AA5CE0
P 4200 1150
F 0 "C?" H 4225 1250 50  0000 L CNN
F 1 "0.1uF" H 4225 1050 50  0000 L CNN
F 2 "" H 4238 1000 50  0000 C CNN
F 3 "" H 4200 1150 50  0000 C CNN
	1    4200 1150
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57AA5D92
P 4600 1150
F 0 "C?" H 4625 1250 50  0000 L CNN
F 1 "0.1uF" H 4625 1050 50  0000 L CNN
F 2 "" H 4638 1000 50  0000 C CNN
F 3 "" H 4600 1150 50  0000 C CNN
	1    4600 1150
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 57AA5DDB
P 4200 800
F 0 "#PWR?" H 4200 650 50  0001 C CNN
F 1 "+3.3V" H 4200 940 50  0000 C CNN
F 2 "" H 4200 800 50  0000 C CNN
F 3 "" H 4200 800 50  0000 C CNN
	1    4200 800 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57AA5DF7
P 4600 1500
F 0 "#PWR?" H 4600 1250 50  0001 C CNN
F 1 "GND" H 4600 1350 50  0000 C CNN
F 2 "" H 4600 1500 50  0000 C CNN
F 3 "" H 4600 1500 50  0000 C CNN
	1    4600 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 1500 4600 1300
Wire Wire Line
	4200 800  4200 1000
Wire Wire Line
	4200 900  4600 900 
Wire Wire Line
	4600 900  4600 1000
Connection ~ 4200 900 
Wire Wire Line
	4600 1400 4200 1400
Wire Wire Line
	4200 1400 4200 1300
Connection ~ 4600 1400
$EndSCHEMATC
