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
LIBS:ftdi
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
L CONN_01X28 P2
U 1 1 57D2E25B
P 8500 3600
F 0 "P2" H 8500 5050 50  0000 C CNN
F 1 "64128" V 8600 3600 50  0000 C CNN
F 2 "" H 8500 3600 50  0000 C CNN
F 3 "" H 8500 3600 50  0000 C CNN
	1    8500 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 2250 8300 2250
Wire Wire Line
	6100 2350 8300 2350
Wire Wire Line
	6000 2450 8300 2450
Wire Wire Line
	8300 2550 7800 2550
Wire Wire Line
	8300 2650 7800 2650
Wire Wire Line
	8300 2750 7800 2750
Wire Wire Line
	8300 2850 7800 2850
Wire Wire Line
	8300 2950 7800 2950
Wire Wire Line
	8300 3050 7800 3050
Wire Wire Line
	8300 3150 7800 3150
Wire Wire Line
	8300 3250 7800 3250
Wire Wire Line
	6500 4350 8300 4350
Wire Wire Line
	6750 4450 8300 4450
Wire Wire Line
	7000 4550 8300 4550
Wire Wire Line
	7250 4650 8300 4650
Wire Wire Line
	7500 4750 8300 4750
Text Label 8000 2250 0    60   ~ 0
/CS
Text Label 8000 2350 0    60   ~ 0
/RES
Text Label 8000 2450 0    60   ~ 0
A0
Text Label 8000 2550 0    60   ~ 0
/WR
Text Label 8000 2650 0    60   ~ 0
/RD
Text Label 8000 2750 0    60   ~ 0
D0
Text Label 8000 2850 0    60   ~ 0
D1
Text Label 8000 2950 0    60   ~ 0
D2
Text Label 8000 3050 0    60   ~ 0
D3
Text Label 8000 3150 0    60   ~ 0
D4
Text Label 8000 3250 0    60   ~ 0
D5
Text Label 8000 3350 0    60   ~ 0
D6
Text Label 8000 3450 0    60   ~ 0
D7
Text Label 8000 3550 0    60   ~ 0
VDD
Text Label 8000 3650 0    60   ~ 0
VSS
Text Label 8000 3750 0    60   ~ 0
VOUT
Text Label 8000 3850 0    60   ~ 0
CAP3P
Text Label 8000 3950 0    60   ~ 0
CAP1N
Text Label 8000 4050 0    60   ~ 0
CAP1P
Text Label 8000 4150 0    60   ~ 0
CAP2P
Text Label 8000 4250 0    60   ~ 0
CAP2N
Text Label 8000 4350 0    60   ~ 0
V4
Text Label 8000 4450 0    60   ~ 0
V3
Text Label 8000 4550 0    60   ~ 0
V2
Text Label 8000 4650 0    60   ~ 0
V1
Text Label 8000 4750 0    60   ~ 0
V0
Text Label 8000 4850 0    60   ~ 0
C86
Text Label 8000 4950 0    60   ~ 0
P/S
$Comp
L C C9
U 1 1 57D2E41A
P 7500 5000
F 0 "C9" H 7525 5100 50  0000 L CNN
F 1 "1uF" H 7525 4900 50  0000 L CNN
F 2 "" H 7538 4850 50  0000 C CNN
F 3 "" H 7500 5000 50  0000 C CNN
	1    7500 5000
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 57D2E475
P 7250 5000
F 0 "C8" H 7275 5100 50  0000 L CNN
F 1 "1uF" H 7275 4900 50  0000 L CNN
F 2 "" H 7288 4850 50  0000 C CNN
F 3 "" H 7250 5000 50  0000 C CNN
	1    7250 5000
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 57D2E494
P 7000 5000
F 0 "C6" H 7025 5100 50  0000 L CNN
F 1 "1uF" H 7025 4900 50  0000 L CNN
F 2 "" H 7038 4850 50  0000 C CNN
F 3 "" H 7000 5000 50  0000 C CNN
	1    7000 5000
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 57D2E4C3
P 6750 5000
F 0 "C4" H 6775 5100 50  0000 L CNN
F 1 "1uF" H 6775 4900 50  0000 L CNN
F 2 "" H 6788 4850 50  0000 C CNN
F 3 "" H 6750 5000 50  0000 C CNN
	1    6750 5000
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 57D2E4F2
P 6500 5000
F 0 "C2" H 6525 5100 50  0000 L CNN
F 1 "1uF" H 6525 4900 50  0000 L CNN
F 2 "" H 6538 4850 50  0000 C CNN
F 3 "" H 6500 5000 50  0000 C CNN
	1    6500 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5150 6500 5250
Wire Wire Line
	6500 5250 7800 5250
Wire Wire Line
	6750 5250 6750 5150
Wire Wire Line
	7000 5150 7000 5350
Connection ~ 6750 5250
Wire Wire Line
	7250 5250 7250 5150
Connection ~ 7000 5250
Wire Wire Line
	7500 5250 7500 5150
Connection ~ 7250 5250
Wire Wire Line
	8300 4850 7800 4850
Wire Wire Line
	7800 4850 7800 5250
Connection ~ 7500 5250
Wire Wire Line
	8300 4950 7800 4950
Connection ~ 7800 4950
$Comp
L GND #PWR5
U 1 1 57D2E687
P 7000 5350
F 0 "#PWR5" H 7000 5100 50  0001 C CNN
F 1 "GND" H 7000 5200 50  0000 C CNN
F 2 "" H 7000 5350 50  0000 C CNN
F 3 "" H 7000 5350 50  0000 C CNN
	1    7000 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 4850 7500 4750
Wire Wire Line
	7250 4850 7250 4650
Wire Wire Line
	7000 4850 7000 4550
Wire Wire Line
	6750 4850 6750 4450
Wire Wire Line
	6500 4350 6500 4850
$Comp
L C C1
U 1 1 57D2E851
P 6150 4200
F 0 "C1" H 6175 4300 50  0000 L CNN
F 1 "1uF" H 6175 4100 50  0000 L CNN
F 2 "" H 6188 4050 50  0000 C CNN
F 3 "" H 6150 4200 50  0000 C CNN
	1    6150 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 4250 8300 4250
Wire Wire Line
	6350 4150 8300 4150
$Comp
L C C7
U 1 1 57D2EAA8
P 7200 3900
F 0 "C7" H 7225 4000 50  0000 L CNN
F 1 "1uF" H 7225 3800 50  0000 L CNN
F 2 "" H 7238 3750 50  0000 C CNN
F 3 "" H 7200 3900 50  0000 C CNN
	1    7200 3900
	-1   0    0    1   
$EndComp
$Comp
L C C5
U 1 1 57D2EADD
P 6850 3900
F 0 "C5" H 6875 4000 50  0000 L CNN
F 1 "1uF" H 6875 3800 50  0000 L CNN
F 2 "" H 6888 3750 50  0000 C CNN
F 3 "" H 6850 3900 50  0000 C CNN
	1    6850 3900
	-1   0    0    1   
$EndComp
$Comp
L C C3
U 1 1 57D2EB21
P 6600 3900
F 0 "C3" H 6625 4000 50  0000 L CNN
F 1 "1uF" H 6625 3800 50  0000 L CNN
F 2 "" H 6638 3750 50  0000 C CNN
F 3 "" H 6600 3900 50  0000 C CNN
	1    6600 3900
	-1   0    0    1   
$EndComp
Wire Wire Line
	7350 3950 8300 3950
Wire Wire Line
	7450 3850 8300 3850
Wire Wire Line
	7550 3750 8300 3750
$Comp
L GND #PWR4
U 1 1 57D2F4DA
P 6200 3800
F 0 "#PWR4" H 6200 3550 50  0001 C CNN
F 1 "GND" H 6200 3650 50  0000 C CNN
F 2 "" H 6200 3800 50  0000 C CNN
F 3 "" H 6200 3800 50  0000 C CNN
	1    6200 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3550 6200 3450
$Comp
L +3.3V #PWR3
U 1 1 57D2F6E6
P 6200 3450
F 0 "#PWR3" H 6200 3300 50  0001 C CNN
F 1 "+3.3V" H 6200 3590 50  0000 C CNN
F 2 "" H 6200 3450 50  0000 C CNN
F 3 "" H 6200 3450 50  0000 C CNN
	1    6200 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 4050 7200 4050
Wire Wire Line
	7350 3950 7350 3750
Wire Wire Line
	7350 3750 7000 3750
Wire Wire Line
	7000 3750 7000 4050
Wire Wire Line
	7000 4050 6850 4050
Wire Wire Line
	7450 3850 7450 3700
Wire Wire Line
	7450 3700 6850 3700
Wire Wire Line
	6850 3700 6850 3750
Wire Wire Line
	7550 3750 7550 3650
Wire Wire Line
	7550 3650 6600 3650
Wire Wire Line
	6600 3650 6600 3750
Wire Wire Line
	6600 4050 6400 4050
Wire Wire Line
	6350 4150 6350 4050
Wire Wire Line
	6350 4050 6150 4050
Wire Wire Line
	6350 4250 6350 4350
Wire Wire Line
	6350 4350 6150 4350
Connection ~ 7200 3750
Wire Wire Line
	8300 3650 7650 3650
Wire Wire Line
	7650 3650 7650 3600
Wire Wire Line
	7650 3600 6200 3600
Wire Wire Line
	6400 4050 6400 3600
Connection ~ 6400 3600
Wire Wire Line
	6200 3600 6200 3800
Wire Wire Line
	8300 3550 6200 3550
$Comp
L CONN_01X07 P1
U 1 1 57D2FCBE
P 5350 2700
F 0 "P1" H 5350 3100 50  0000 C CNN
F 1 "LCD I/F" V 5450 2700 50  0000 C CNN
F 2 "" H 5350 2700 50  0000 C CNN
F 3 "" H 5350 2700 50  0000 C CNN
	1    5350 2700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5550 2400 5700 2400
Wire Wire Line
	5700 2400 5700 2250
$Comp
L +3.3V #PWR1
U 1 1 57D2FDA6
P 5700 2250
F 0 "#PWR1" H 5700 2100 50  0001 C CNN
F 1 "+3.3V" H 5700 2390 50  0000 C CNN
F 2 "" H 5700 2250 50  0000 C CNN
F 3 "" H 5700 2250 50  0000 C CNN
	1    5700 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3000 5700 3000
Wire Wire Line
	5700 3000 5700 3200
$Comp
L GND #PWR2
U 1 1 57D2FE21
P 5700 3200
F 0 "#PWR2" H 5700 2950 50  0001 C CNN
F 1 "GND" H 5700 3050 50  0000 C CNN
F 2 "" H 5700 3200 50  0000 C CNN
F 3 "" H 5700 3200 50  0000 C CNN
	1    5700 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2500 5900 2500
Wire Wire Line
	5900 2500 5900 2250
Wire Wire Line
	5550 2600 6000 2600
Wire Wire Line
	6000 2600 6000 2450
Wire Wire Line
	8300 3450 6400 3450
Wire Wire Line
	6400 3450 6400 2700
Wire Wire Line
	6400 2700 5550 2700
Wire Wire Line
	8300 3350 6500 3350
Wire Wire Line
	6500 3350 6500 2800
Wire Wire Line
	6500 2800 5550 2800
Wire Wire Line
	5550 2900 6100 2900
Wire Wire Line
	6100 2900 6100 2350
Text Label 5550 2500 0    60   ~ 0
/CS
Text Label 5550 2600 0    60   ~ 0
A0
Text Label 5550 2700 0    60   ~ 0
D7/SDA
Text Label 5550 2800 0    60   ~ 0
D6/SCK
Text Label 5550 2900 0    60   ~ 0
/RES
Wire Wire Line
	7800 2650 7800 2000
$Comp
L +3.3V #PWR6
U 1 1 57D3017A
P 7800 2000
F 0 "#PWR6" H 7800 1850 50  0001 C CNN
F 1 "+3.3V" H 7800 2140 50  0000 C CNN
F 2 "" H 7800 2000 50  0000 C CNN
F 3 "" H 7800 2000 50  0000 C CNN
	1    7800 2000
	1    0    0    -1  
$EndComp
Connection ~ 7800 2550
NoConn ~ 7800 2750
NoConn ~ 7800 2850
NoConn ~ 7800 2950
NoConn ~ 7800 3050
NoConn ~ 7800 3150
NoConn ~ 7800 3250
$Comp
L C C?
U 1 1 57D30280
P 5700 3900
F 0 "C?" H 5725 4000 50  0000 L CNN
F 1 "1uF" H 5725 3800 50  0000 L CNN
F 2 "" H 5738 3750 50  0000 C CNN
F 3 "" H 5700 3900 50  0000 C CNN
	1    5700 3900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 57D302B7
P 5700 3650
F 0 "#PWR?" H 5700 3500 50  0001 C CNN
F 1 "+3.3V" H 5700 3790 50  0000 C CNN
F 2 "" H 5700 3650 50  0000 C CNN
F 3 "" H 5700 3650 50  0000 C CNN
	1    5700 3650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57D302E3
P 5700 4150
F 0 "#PWR?" H 5700 3900 50  0001 C CNN
F 1 "GND" H 5700 4000 50  0000 C CNN
F 2 "" H 5700 4150 50  0000 C CNN
F 3 "" H 5700 4150 50  0000 C CNN
	1    5700 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 3750 5700 3650
Wire Wire Line
	5700 4150 5700 4050
$EndSCHEMATC
