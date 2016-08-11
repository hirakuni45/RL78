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
L VS1053 U?
U 1 1 57ACA381
P 6450 4000
F 0 "U?" H 7000 5750 60  0000 C CNN
F 1 "VS1063a" H 6000 2250 60  0000 C CNN
F 2 "" H 6450 3950 60  0000 C CNN
F 3 "" H 6450 3950 60  0000 C CNN
	1    6450 4000
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X05 P?
U 1 1 57ACA3DB
P 3600 3100
F 0 "P?" H 3600 3400 50  0000 C CNN
F 1 "CONN_02X05" H 3600 2800 50  0000 C CNN
F 2 "" H 3600 1900 50  0000 C CNN
F 3 "" H 3600 1900 50  0000 C CNN
	1    3600 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 2900 3850 2900
Wire Wire Line
	3350 2900 3200 2900
Wire Wire Line
	3200 2900 3200 2600
Wire Wire Line
	3200 2600 3950 2600
Wire Wire Line
	3950 2600 3950 2800
Wire Wire Line
	3950 2800 5650 2800
Wire Wire Line
	3850 3000 4050 3000
$Comp
L +3.3V #PWR?
U 1 1 57ACA542
P 4050 2200
F 0 "#PWR?" H 4050 2050 50  0001 C CNN
F 1 "+3.3V" H 4050 2340 50  0000 C CNN
F 2 "" H 4050 2200 50  0000 C CNN
F 3 "" H 4050 2200 50  0000 C CNN
	1    4050 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3000 4050 2200
Wire Wire Line
	5650 2600 5550 2600
Wire Wire Line
	5550 2600 5550 2200
$Comp
L +3.3V #PWR?
U 1 1 57ACA574
P 5550 2200
F 0 "#PWR?" H 5550 2050 50  0001 C CNN
F 1 "+3.3V" H 5550 2340 50  0000 C CNN
F 2 "" H 5550 2200 50  0000 C CNN
F 3 "" H 5550 2200 50  0000 C CNN
	1    5550 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 2400 5550 2400
Connection ~ 5550 2400
Wire Wire Line
	5650 2500 5550 2500
Connection ~ 5550 2500
$Comp
L GND #PWR?
U 1 1 57ACA5AC
P 2900 3550
F 0 "#PWR?" H 2900 3300 50  0001 C CNN
F 1 "GND" H 2900 3400 50  0000 C CNN
F 2 "" H 2900 3550 50  0000 C CNN
F 3 "" H 2900 3550 50  0000 C CNN
	1    2900 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3000 2900 3550
Wire Wire Line
	3850 3100 5650 3100
Wire Wire Line
	5650 3000 4200 3000
Wire Wire Line
	4200 3000 4200 2500
Wire Wire Line
	4200 2500 3100 2500
Wire Wire Line
	3100 2500 3100 3100
Wire Wire Line
	3100 3100 3350 3100
Wire Wire Line
	5650 3400 4050 3400
Wire Wire Line
	4050 3400 4050 3300
Wire Wire Line
	4050 3300 3850 3300
Wire Wire Line
	5650 3200 3850 3200
Wire Wire Line
	5650 3300 4300 3300
Wire Wire Line
	4300 3300 4300 2400
Wire Wire Line
	4300 2400 3000 2400
Wire Wire Line
	3000 2400 3000 3200
Wire Wire Line
	3000 3200 3350 3200
Text Label 3850 2900 0    60   ~ 0
SI
Text Label 3250 2900 0    60   ~ 0
SO
Text Label 3150 3100 0    60   ~ 0
SCLK
Text Label 3850 3100 0    60   ~ 0
~CS
Text Label 3850 3200 0    60   ~ 0
~DCS
Text Label 3150 3200 0    60   ~ 0
DREQ
Text Label 3850 3300 0    60   ~ 0
~RESET
Wire Wire Line
	3350 3000 2900 3000
$EndSCHEMATC
