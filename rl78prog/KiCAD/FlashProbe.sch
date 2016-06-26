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
L Shotkey D?
U 1 1 576EE636
P 2700 2950
F 0 "D?" H 2800 3000 60  0000 C CNN
F 1 "RB751S-40" H 2350 2900 60  0000 C CNN
F 2 "" V 2695 2915 60  0000 C CNN
F 3 "" V 2695 2915 60  0000 C CNN
	1    2700 2950
	-1   0    0    1   
$EndComp
$Comp
L 2N7002K Q?
U 1 1 576EE6C7
P 3450 4200
F 0 "Q?" H 3550 4350 60  0000 C CNN
F 1 "BSS138" H 3150 4050 60  0000 C CNN
F 2 "" H 3450 4200 60  0000 C CNN
F 3 "" H 3450 4200 60  0000 C CNN
	1    3450 4200
	1    0    0    -1  
$EndComp
Text GLabel 2300 3500 0    60   Input ~ 0
TXD
Text GLabel 2300 2700 0    60   Input ~ 0
RXD
Text GLabel 2300 4250 0    60   Input ~ 0
RTS
$Comp
L R 5K
U 1 1 576EE8DD
P 2700 2450
F 0 "5K" V 2780 2450 50  0000 C CNN
F 1 "R" V 2700 2450 50  0000 C CNN
F 2 "" V 2630 2450 50  0000 C CNN
F 3 "" H 2700 2450 50  0000 C CNN
	1    2700 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 576EE997
P 3450 4850
F 0 "#PWR?" H 3450 4600 50  0001 C CNN
F 1 "GND" H 3450 4700 50  0000 C CNN
F 2 "" H 3450 4850 50  0000 C CNN
F 3 "" H 3450 4850 50  0000 C CNN
	1    3450 4850
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 576EE9B9
P 2700 2100
F 0 "#PWR?" H 2700 1950 50  0001 C CNN
F 1 "VCC" H 2700 2250 50  0000 C CNN
F 2 "" H 2700 2100 50  0000 C CNN
F 3 "" H 2700 2100 50  0000 C CNN
	1    2700 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 2100 2700 2300
Wire Wire Line
	2300 3500 2700 3500
Wire Wire Line
	2700 3500 2700 3100
Wire Wire Line
	2700 2800 2700 2600
Connection ~ 2700 2700
Text GLabel 3700 2700 2    60   BiDi ~ 0
TOOL0
Wire Wire Line
	2300 4250 3200 4250
$Comp
L R 47K
U 1 1 576EEBD4
P 2800 4550
F 0 "47K" V 2880 4550 50  0000 C CNN
F 1 "R" V 2800 4550 50  0000 C CNN
F 2 "" V 2730 4550 50  0000 C CNN
F 3 "" H 2800 4550 50  0000 C CNN
	1    2800 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 4450 3450 4850
Wire Wire Line
	3450 4800 2800 4800
Wire Wire Line
	2800 4800 2800 4700
Connection ~ 3450 4800
Wire Wire Line
	2800 4400 2800 4250
Connection ~ 2800 4250
Text GLabel 3750 3700 2    60   Output ~ 0
/RESET
Wire Wire Line
	3750 3700 3450 3700
Wire Wire Line
	3450 3700 3450 3950
Wire Wire Line
	2300 2700 3700 2700
$EndSCHEMATC
