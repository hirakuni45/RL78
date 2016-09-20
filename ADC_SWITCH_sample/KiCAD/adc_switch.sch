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
L 2P-SW SW?
U 1 1 57E0834F
P 3500 2250
F 0 "SW?" H 3550 2450 60  0000 C CNN
F 1 "RIGHT" H 3450 2050 60  0000 C CNN
F 2 "" H 3505 2250 60  0000 C CNN
F 3 "" H 3505 2250 60  0000 C CNN
	1    3500 2250
	1    0    0    -1  
$EndComp
$Comp
L 2P-SW SW?
U 1 1 57E0838E
P 3500 2850
F 0 "SW?" H 3550 3050 60  0000 C CNN
F 1 "UP" H 3450 2650 60  0000 C CNN
F 2 "" H 3505 2850 60  0000 C CNN
F 3 "" H 3505 2850 60  0000 C CNN
	1    3500 2850
	1    0    0    -1  
$EndComp
$Comp
L 2P-SW SW?
U 1 1 57E083AB
P 3500 3450
F 0 "SW?" H 3550 3650 60  0000 C CNN
F 1 "DOWN" H 3450 3250 60  0000 C CNN
F 2 "" H 3505 3450 60  0000 C CNN
F 3 "" H 3505 3450 60  0000 C CNN
	1    3500 3450
	1    0    0    -1  
$EndComp
$Comp
L 2P-SW SW?
U 1 1 57E083E2
P 3500 4050
F 0 "SW?" H 3550 4250 60  0000 C CNN
F 1 "LEFT" H 3450 3850 60  0000 C CNN
F 2 "" H 3505 4050 60  0000 C CNN
F 3 "" H 3505 4050 60  0000 C CNN
	1    3500 4050
	1    0    0    -1  
$EndComp
$Comp
L 2P-SW SW?
U 1 1 57E08401
P 7050 2250
F 0 "SW?" H 7100 2450 60  0000 C CNN
F 1 "A" H 7000 2050 60  0000 C CNN
F 2 "" H 7055 2250 60  0000 C CNN
F 3 "" H 7055 2250 60  0000 C CNN
	1    7050 2250
	1    0    0    -1  
$EndComp
$Comp
L 2P-SW SW?
U 1 1 57E08422
P 7050 3200
F 0 "SW?" H 7100 3400 60  0000 C CNN
F 1 "B" H 7000 3000 60  0000 C CNN
F 2 "" H 7055 3200 60  0000 C CNN
F 3 "" H 7055 3200 60  0000 C CNN
	1    7050 3200
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57E08443
P 3000 1950
F 0 "R?" V 3100 1950 50  0000 C CNN
F 1 "10K" V 2900 1950 50  0000 C CNN
F 2 "" V 2930 1950 50  0000 C CNN
F 3 "" H 3000 1950 50  0000 C CNN
	1    3000 1950
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57E084E1
P 3000 2550
F 0 "R?" V 3100 2550 50  0000 C CNN
F 1 "3.3K" V 2900 2550 50  0000 C CNN
F 2 "" V 2930 2550 50  0000 C CNN
F 3 "" H 3000 2550 50  0000 C CNN
	1    3000 2550
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57E08506
P 3000 3150
F 0 "R?" V 3100 3150 50  0000 C CNN
F 1 "6.8K" V 2900 3150 50  0000 C CNN
F 2 "" V 2930 3150 50  0000 C CNN
F 3 "" H 3000 3150 50  0000 C CNN
	1    3000 3150
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57E08551
P 3000 3750
F 0 "R?" V 3100 3750 50  0000 C CNN
F 1 "20K" V 2900 3750 50  0000 C CNN
F 2 "" V 2930 3750 50  0000 C CNN
F 3 "" H 3000 3750 50  0000 C CNN
	1    3000 3750
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57E0857A
P 6500 2550
F 0 "R?" V 6600 2550 50  0000 C CNN
F 1 "10K" V 6400 2550 50  0000 C CNN
F 2 "" V 6430 2550 50  0000 C CNN
F 3 "" H 6500 2550 50  0000 C CNN
	1    6500 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2300 3900 2300
Wire Wire Line
	3900 2300 3900 4450
Wire Wire Line
	3900 2900 3750 2900
Wire Wire Line
	3900 3500 3750 3500
Connection ~ 3900 2900
Wire Wire Line
	3900 4100 3750 4100
Connection ~ 3900 3500
Connection ~ 3900 4100
$Comp
L GND #PWR?
U 1 1 57E088C9
P 3900 4450
F 0 "#PWR?" H 3900 4200 50  0001 C CNN
F 1 "GND" H 3900 4300 50  0000 C CNN
F 2 "" H 3900 4450 50  0000 C CNN
F 3 "" H 3900 4450 50  0000 C CNN
	1    3900 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 2200 3250 2200
Wire Wire Line
	3000 2100 3000 2400
Connection ~ 3000 2200
Wire Wire Line
	3250 2800 3000 2800
Wire Wire Line
	3000 2700 3000 3000
Connection ~ 3000 2800
Wire Wire Line
	3250 3400 3000 3400
Wire Wire Line
	3000 3300 3000 3600
Connection ~ 3000 3400
Wire Wire Line
	3250 4000 3000 4000
Wire Wire Line
	3000 4000 3000 3900
Wire Wire Line
	3000 1800 3000 1500
$Comp
L VDD #PWR?
U 1 1 57E08A41
P 3000 1500
F 0 "#PWR?" H 3000 1350 50  0001 C CNN
F 1 "VDD" H 3000 1650 50  0000 C CNN
F 2 "" H 3000 1500 50  0000 C CNN
F 3 "" H 3000 1500 50  0000 C CNN
	1    3000 1500
	1    0    0    -1  
$EndComp
Text GLabel 2350 2200 0    60   Input ~ 0
P22/ANI2(54)
$Comp
L R R?
U 1 1 57E08C91
P 6500 3400
F 0 "R?" V 6600 3400 50  0000 C CNN
F 1 "5K" V 6400 3400 50  0000 C CNN
F 2 "" V 6430 3400 50  0000 C CNN
F 3 "" H 6500 3400 50  0000 C CNN
	1    6500 3400
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 57E08CCA
P 6500 1950
F 0 "R?" V 6600 1950 50  0000 C CNN
F 1 "5K" V 6400 1950 50  0000 C CNN
F 2 "" V 6430 1950 50  0000 C CNN
F 3 "" H 6500 1950 50  0000 C CNN
	1    6500 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2200 6800 2200
Wire Wire Line
	6500 2100 6500 2400
Connection ~ 6500 2200
Wire Wire Line
	6500 1800 6500 1450
$Comp
L VDD #PWR?
U 1 1 57E08F07
P 6500 1450
F 0 "#PWR?" H 6500 1300 50  0001 C CNN
F 1 "VDD" H 6500 1600 50  0000 C CNN
F 2 "" H 6500 1450 50  0000 C CNN
F 3 "" H 6500 1450 50  0000 C CNN
	1    6500 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2700 6500 3250
Wire Wire Line
	6500 2800 7450 2800
Wire Wire Line
	7450 2800 7450 2300
Wire Wire Line
	7450 2300 7300 2300
Wire Wire Line
	6500 3150 6800 3150
Connection ~ 6500 2800
Connection ~ 6500 3150
Wire Wire Line
	6500 3550 6500 3950
Wire Wire Line
	6500 3700 7450 3700
Wire Wire Line
	7450 3700 7450 3250
Wire Wire Line
	7450 3250 7300 3250
Connection ~ 6500 3700
$Comp
L GND #PWR?
U 1 1 57E0904E
P 6500 3950
F 0 "#PWR?" H 6500 3700 50  0001 C CNN
F 1 "GND" H 6500 3800 50  0000 C CNN
F 2 "" H 6500 3950 50  0000 C CNN
F 3 "" H 6500 3950 50  0000 C CNN
	1    6500 3950
	1    0    0    -1  
$EndComp
Text GLabel 5800 2200 0    60   Input ~ 0
P23/ANI3(55)
$EndSCHEMATC
