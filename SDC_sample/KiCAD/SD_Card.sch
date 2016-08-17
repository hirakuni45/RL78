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
LIBS:renesas
LIBS:akizuki
LIBS:SD_Card-cache
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
L DMG3415U Q?
U 1 1 57A9BCBD
P 8450 3200
F 0 "Q?" V 8550 3350 60  0000 C CNN
F 1 "DMG3415U" V 8600 3000 60  0000 C CNN
F 2 "" H 8450 3200 60  0000 C CNN
F 3 "" H 8450 3200 60  0000 C CNN
	1    8450 3200
	0    1    -1   0   
$EndComp
$Comp
L R FB?
U 1 1 57A9BE3C
P 9050 3200
F 0 "FB?" V 9150 3200 50  0000 C CNN
F 1 "BLM18PG471SN1D" V 8950 3200 50  0000 C CNN
F 2 "" V 8980 3200 50  0000 C CNN
F 3 "" H 9050 3200 50  0000 C CNN
	1    9050 3200
	0    1    1    0   
$EndComp
$Comp
L C-POL C?
U 1 1 57A9BE7B
P 9350 3500
F 0 "C?" H 9450 3600 60  0000 C CNN
F 1 "22uF" H 9250 3400 60  0000 C CNN
F 2 "" V 9350 3500 60  0000 C CNN
F 3 "" V 9350 3500 60  0000 C CNN
	1    9350 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 3200 8900 3200
Wire Wire Line
	9350 3350 9350 3200
Connection ~ 9350 3200
$Comp
L GND #PWR?
U 1 1 57A9C1B9
P 9350 3900
F 0 "#PWR?" H 9350 3650 50  0001 C CNN
F 1 "GND" H 9350 3750 50  0000 C CNN
F 2 "" H 9350 3900 50  0000 C CNN
F 3 "" H 9350 3900 50  0000 C CNN
	1    9350 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 3900 9350 3650
$Comp
L SD-CARD-SOCKET CN?
U 1 1 57A9C5C9
P 10550 3000
F 0 "CN?" H 10550 3750 60  0000 C CNN
F 1 "SD-CARD-SOCKET" H 10725 2250 60  0000 C CNN
F 2 "" H 10475 2925 60  0000 C CNN
F 3 "" H 10475 2925 60  0000 C CNN
	1    10550 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 2500 9600 3800
Wire Wire Line
	9600 3800 9350 3800
Connection ~ 9350 3800
Connection ~ 9600 3300
Connection ~ 9600 3000
$Comp
L R R?
U 1 1 57A9C62D
P 8050 3500
F 0 "R?" V 8150 3500 50  0000 C CNN
F 1 "47K" V 7950 3500 50  0000 C CNN
F 2 "" V 7980 3500 50  0000 C CNN
F 3 "" H 8050 3500 50  0000 C CNN
	1    8050 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 3650 8050 3800
Wire Wire Line
	8050 3800 8400 3800
Wire Wire Line
	8400 3450 8400 4150
Wire Wire Line
	8050 3200 8050 3350
Wire Wire Line
	8400 4150 8050 4150
Connection ~ 8400 3800
Text GLabel 8050 4150 0    60   Input ~ 0
/POWER
Text GLabel 8050 2600 0    60   Input ~ 0
/DETECT
Text GLabel 8050 4650 0    60   Input ~ 0
SCLK
Text GLabel 8050 4900 0    60   Input ~ 0
SO
Text GLabel 8050 4400 0    60   Input ~ 0
SI
Wire Wire Line
	8050 4400 9700 4400
Wire Wire Line
	9700 4400 9700 2900
Wire Wire Line
	8050 4650 9800 4650
Wire Wire Line
	9800 4650 9800 3100
Wire Wire Line
	9900 4900 8050 4900
Wire Wire Line
	9900 2300 9900 4900
Wire Wire Line
	9900 3400 10100 3400
Wire Wire Line
	9600 3300 10100 3300
Wire Wire Line
	10100 3200 9200 3200
Wire Wire Line
	9800 3100 10100 3100
Wire Wire Line
	9600 3000 10100 3000
Wire Wire Line
	9700 2900 10100 2900
Wire Wire Line
	9600 2500 10100 2500
Wire Wire Line
	8050 2600 10100 2600
Text GLabel 8050 5150 0    60   Input ~ 0
/SELECT
Wire Wire Line
	8050 5150 10000 5150
Wire Wire Line
	10000 5150 10000 3500
Wire Wire Line
	10000 3500 10100 3500
$Comp
L R R?
U 1 1 57A9CA02
P 9900 2150
F 0 "R?" V 10000 2150 50  0000 C CNN
F 1 "10K" V 9800 2150 50  0000 C CNN
F 2 "" V 9830 2150 50  0000 C CNN
F 3 "" H 9900 2150 50  0000 C CNN
	1    9900 2150
	1    0    0    -1  
$EndComp
Connection ~ 9900 3400
Connection ~ 8050 3200
Wire Wire Line
	9500 1400 9500 3200
Wire Wire Line
	9500 1900 9900 1900
Wire Wire Line
	9900 1900 9900 2000
Connection ~ 9500 3200
$Comp
L R5F100_REAL U?
U 1 1 57A9D03F
P 4400 3900
F 0 "U?" H 6000 5850 60  0000 C CNN
F 1 "R5F100LGAFB" H 3000 1950 60  0000 C CNN
F 2 "" H 6000 5850 60  0000 C CNN
F 3 "" H 6000 5850 60  0000 C CNN
	1    4400 3900
	1    0    0    -1  
$EndComp
Text GLabel 6750 4300 2    60   Input ~ 0
SCLK
Text GLabel 6750 4450 2    60   Input ~ 0
SI
Text GLabel 6750 4600 2    60   Input ~ 0
SO
Wire Wire Line
	6750 4300 6300 4300
Wire Wire Line
	6300 4400 6650 4400
Wire Wire Line
	6650 4400 6650 4450
Wire Wire Line
	6650 4450 6750 4450
Wire Wire Line
	6650 4500 6650 4600
Wire Wire Line
	6650 4600 6750 4600
Wire Wire Line
	6650 4500 6300 4500
$Comp
L C C?
U 1 1 57A9D7E0
P 1800 3100
F 0 "C?" H 1825 3200 50  0000 L CNN
F 1 "1uF" H 1825 3000 50  0000 L CNN
F 2 "" H 1838 2950 50  0000 C CNN
F 3 "" H 1800 3100 50  0000 C CNN
	1    1800 3100
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57A9D843
P 1800 3800
F 0 "C?" H 1825 3900 50  0000 L CNN
F 1 "0.1uF" H 1825 3700 50  0000 L CNN
F 2 "" H 1838 3650 50  0000 C CNN
F 3 "" H 1800 3800 50  0000 C CNN
	1    1800 3800
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57A9D9D3
P 1450 3800
F 0 "C?" H 1475 3900 50  0000 L CNN
F 1 "0.1uF" H 1500 3700 50  0000 L CNN
F 2 "" H 1488 3650 50  0000 C CNN
F 3 "" H 1450 3800 50  0000 C CNN
	1    1450 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3300 1800 3300
Wire Wire Line
	1800 3300 1800 3250
$Comp
L GND #PWR?
U 1 1 57A9DB17
P 1450 4150
F 0 "#PWR?" H 1450 3900 50  0001 C CNN
F 1 "GND" H 1450 4000 50  0000 C CNN
F 2 "" H 1450 4150 50  0000 C CNN
F 3 "" H 1450 4150 50  0000 C CNN
	1    1450 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57A9DB3F
P 1450 3300
F 0 "#PWR?" H 1450 3050 50  0001 C CNN
F 1 "GND" H 1450 3150 50  0000 C CNN
F 2 "" H 1450 3300 50  0000 C CNN
F 3 "" H 1450 3300 50  0000 C CNN
	1    1450 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 2900 1800 2900
Wire Wire Line
	1800 2900 1800 2950
Wire Wire Line
	1450 3300 1450 2900
Wire Wire Line
	2500 3400 1600 3400
Wire Wire Line
	1600 3400 1600 3200
Wire Wire Line
	1600 3200 1450 3200
Connection ~ 1450 3200
Wire Wire Line
	2500 3500 2400 3500
Wire Wire Line
	2400 3500 2400 3400
Connection ~ 2400 3400
Wire Wire Line
	1150 3600 2500 3600
Wire Wire Line
	1450 3600 1450 3650
Wire Wire Line
	2500 3700 2400 3700
Wire Wire Line
	2400 3700 2400 3600
Connection ~ 2400 3600
Wire Wire Line
	1800 3650 1800 3600
Connection ~ 1800 3600
Wire Wire Line
	1450 3950 1450 4150
Wire Wire Line
	1150 4050 1800 4050
Wire Wire Line
	1800 4050 1800 3950
Connection ~ 1450 4050
$Comp
L R R?
U 1 1 57A9DEA2
P 1800 2450
F 0 "R?" V 1900 2450 50  0000 C CNN
F 1 "10K" V 1700 2450 50  0000 C CNN
F 2 "" V 1730 2450 50  0000 C CNN
F 3 "" H 1800 2450 50  0000 C CNN
	1    1800 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 2700 1800 2700
Wire Wire Line
	1800 2700 1800 2600
Wire Wire Line
	1300 3600 1300 2200
Wire Wire Line
	1300 2200 1800 2200
Connection ~ 1450 3600
$Comp
L +3.3V #PWR?
U 1 1 57A9E023
P 1800 1300
F 0 "#PWR?" H 1800 1150 50  0001 C CNN
F 1 "+3.3V" H 1800 1440 50  0000 C CNN
F 2 "" H 1800 1300 50  0000 C CNN
F 3 "" H 1800 1300 50  0000 C CNN
	1    1800 1300
	1    0    0    -1  
$EndComp
Connection ~ 1800 2200
$Comp
L Led_Small D?
U 1 1 57A9E0FE
P 2150 2100
F 0 "D?" H 2100 2225 50  0000 L CNN
F 1 "RED" H 2100 2000 50  0000 L CNN
F 2 "" V 2150 2100 50  0000 C CNN
F 3 "" V 2150 2100 50  0000 C CNN
	1    2150 2100
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 57A9E2FF
P 2150 1650
F 0 "R?" V 2250 1650 50  0000 C CNN
F 1 "1.5K" V 2050 1650 50  0000 C CNN
F 2 "" V 2080 1650 50  0000 C CNN
F 3 "" H 2150 1650 50  0000 C CNN
	1    2150 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 2300 1800 1300
Wire Wire Line
	2150 1500 2150 1400
Wire Wire Line
	2150 1400 1800 1400
Connection ~ 1800 1400
Wire Wire Line
	2150 1800 2150 2000
Wire Wire Line
	2150 2200 2150 2300
Wire Wire Line
	2150 2300 2500 2300
Text GLabel 6750 2400 2    60   Input ~ 0
/SELECT
Text GLabel 6750 2600 2    60   Input ~ 0
/POWER
Text GLabel 6750 4150 2    60   Input ~ 0
/DETECT
Wire Wire Line
	6300 4200 6650 4200
Wire Wire Line
	6650 4200 6650 4150
Wire Wire Line
	6650 4150 6750 4150
Wire Wire Line
	6750 2400 6300 2400
Wire Wire Line
	6300 2500 6650 2500
Wire Wire Line
	6650 2500 6650 2600
Wire Wire Line
	6650 2600 6750 2600
$Comp
L +3.3V #PWR?
U 1 1 57A9EF42
P 7450 3200
F 0 "#PWR?" H 7450 3050 50  0001 C CNN
F 1 "+3.3V" H 7450 3340 50  0000 C CNN
F 2 "" H 7450 3200 50  0000 C CNN
F 3 "" H 7450 3200 50  0000 C CNN
	1    7450 3200
	0    -1   -1   0   
$EndComp
$Comp
L Led_Small D?
U 1 1 57A9F813
P 9050 2100
F 0 "D?" H 9000 2225 50  0000 L CNN
F 1 "RED" H 9000 2000 50  0000 L CNN
F 2 "" V 9050 2100 50  0000 C CNN
F 3 "" V 9050 2100 50  0000 C CNN
	1    9050 2100
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 57A9F819
P 9050 1650
F 0 "R?" V 9150 1650 50  0000 C CNN
F 1 "1.5K" V 8950 1650 50  0000 C CNN
F 2 "" V 8980 1650 50  0000 C CNN
F 3 "" H 9050 1650 50  0000 C CNN
	1    9050 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 1800 9050 2000
Wire Wire Line
	9500 1400 9050 1400
Wire Wire Line
	9050 1400 9050 1500
Connection ~ 9500 1900
$Comp
L GND #PWR?
U 1 1 57A9F926
P 9050 2300
F 0 "#PWR?" H 9050 2050 50  0001 C CNN
F 1 "GND" H 9050 2150 50  0000 C CNN
F 2 "" H 9050 2300 50  0000 C CNN
F 3 "" H 9050 2300 50  0000 C CNN
	1    9050 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 2300 9050 2200
Text GLabel 1350 4900 0    60   Input ~ 0
/RESET
Text GLabel 1350 5100 0    60   Input ~ 0
TOOL0
Wire Wire Line
	1350 4900 2100 4900
Wire Wire Line
	2100 4900 2100 2700
Connection ~ 2100 2700
Wire Wire Line
	2500 2600 2200 2600
Wire Wire Line
	2200 2600 2200 5100
Wire Wire Line
	2200 5100 1350 5100
Text GLabel 6100 1150 0    60   Input ~ 0
RXD
Text GLabel 6100 1350 0    60   Input ~ 0
TXD
Wire Wire Line
	6300 2700 6400 2700
Wire Wire Line
	6400 2700 6400 1350
Wire Wire Line
	6400 1350 6100 1350
Wire Wire Line
	6300 2600 6500 2600
Wire Wire Line
	6500 2600 6500 1150
Wire Wire Line
	6500 1150 6100 1150
$Comp
L C-POL C?
U 1 1 57B4E28C
P 7750 3500
F 0 "C?" H 7850 3600 60  0000 C CNN
F 1 "10uF" H 7650 3400 60  0000 C CNN
F 2 "" V 7750 3500 60  0000 C CNN
F 3 "" V 7750 3500 60  0000 C CNN
	1    7750 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 3200 7450 3200
Wire Wire Line
	7750 3350 7750 3200
Connection ~ 7750 3200
$Comp
L GND #PWR?
U 1 1 57B4E3BB
P 7750 3800
F 0 "#PWR?" H 7750 3550 50  0001 C CNN
F 1 "GND" H 7750 3650 50  0000 C CNN
F 2 "" H 7750 3800 50  0000 C CNN
F 3 "" H 7750 3800 50  0000 C CNN
	1    7750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 3800 7750 3650
$Comp
L C-POL C?
U 1 1 57B4EA51
P 1150 3800
F 0 "C?" H 1250 3900 60  0000 C CNN
F 1 "10uF" H 1050 3700 60  0000 C CNN
F 2 "" V 1150 3800 60  0000 C CNN
F 3 "" V 1150 3800 60  0000 C CNN
	1    1150 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 3950 1150 4050
Wire Wire Line
	1150 3600 1150 3650
Connection ~ 1300 3600
$EndSCHEMATC
