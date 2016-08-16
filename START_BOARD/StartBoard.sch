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
LIBS:StartBoard-cache
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
P 5850 3000
F 0 "U?" H 7500 4950 60  0000 C CNN
F 1 "R5F100LxAFB" H 4450 1050 60  0000 C CNN
F 2 "QFP64" H 5850 4950 60  0000 C CNN
F 3 "" H 7450 4950 60  0000 C CNN
	1    5850 3000
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57AA5A71
P 3100 2650
F 0 "C?" H 3125 2750 50  0000 L CNN
F 1 "1uF" H 3125 2550 50  0000 L CNN
F 2 "" H 3138 2500 50  0000 C CNN
F 3 "" H 3100 2650 50  0000 C CNN
	1    3100 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 2400 3950 2400
$Comp
L GND #PWR?
U 1 1 57AA5C37
P 3100 2900
F 0 "#PWR?" H 3100 2650 50  0001 C CNN
F 1 "GND" H 3100 2750 50  0000 C CNN
F 2 "" H 3100 2900 50  0000 C CNN
F 3 "" H 3100 2900 50  0000 C CNN
	1    3100 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 2800 3100 2900
$Comp
L GND #PWR?
U 1 1 57AA5C61
P 3700 2900
F 0 "#PWR?" H 3700 2650 50  0001 C CNN
F 1 "GND" H 3700 2750 50  0000 C CNN
F 2 "" H 3700 2900 50  0000 C CNN
F 3 "" H 3700 2900 50  0000 C CNN
	1    3700 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2500 3700 2900
Wire Wire Line
	3700 2600 3950 2600
Wire Wire Line
	3700 2500 3950 2500
Connection ~ 3700 2600
$Comp
L +3.3V #PWR?
U 1 1 57AA5C8A
P 3500 2700
F 0 "#PWR?" H 3500 2550 50  0001 C CNN
F 1 "+3.3V" H 3500 2840 50  0000 C CNN
F 2 "" H 3500 2700 50  0000 C CNN
F 3 "" H 3500 2700 50  0000 C CNN
	1    3500 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3950 2700 3500 2700
Wire Wire Line
	3950 2800 3600 2800
Wire Wire Line
	3600 2800 3600 2700
Connection ~ 3600 2700
$Comp
L C C?
U 1 1 57AA5CE0
P 1850 2950
F 0 "C?" H 1875 3050 50  0000 L CNN
F 1 "0.1uF" H 1875 2850 50  0000 L CNN
F 2 "" H 1888 2800 50  0000 C CNN
F 3 "" H 1850 2950 50  0000 C CNN
	1    1850 2950
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57AA5D92
P 2250 2950
F 0 "C?" H 2275 3050 50  0000 L CNN
F 1 "0.1uF" H 2275 2850 50  0000 L CNN
F 2 "" H 2288 2800 50  0000 C CNN
F 3 "" H 2250 2950 50  0000 C CNN
	1    2250 2950
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 57AA5DDB
P 1850 2600
F 0 "#PWR?" H 1850 2450 50  0001 C CNN
F 1 "+3.3V" H 1850 2740 50  0000 C CNN
F 2 "" H 1850 2600 50  0000 C CNN
F 3 "" H 1850 2600 50  0000 C CNN
	1    1850 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57AA5DF7
P 2250 3300
F 0 "#PWR?" H 2250 3050 50  0001 C CNN
F 1 "GND" H 2250 3150 50  0000 C CNN
F 2 "" H 2250 3300 50  0000 C CNN
F 3 "" H 2250 3300 50  0000 C CNN
	1    2250 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 3300 2250 3100
Wire Wire Line
	1850 2600 1850 2800
Wire Wire Line
	1850 2700 2250 2700
Wire Wire Line
	2250 2700 2250 2800
Connection ~ 1850 2700
Wire Wire Line
	2250 3200 1850 3200
Wire Wire Line
	1850 3200 1850 3100
Connection ~ 2250 3200
Wire Wire Line
	2300 1300 2550 1300
Wire Wire Line
	2300 1400 2550 1400
Wire Wire Line
	2300 1500 2550 1500
Wire Wire Line
	2300 1600 2550 1600
Wire Wire Line
	2300 1700 2550 1700
Wire Wire Line
	1550 1300 1800 1300
Wire Wire Line
	1550 1400 1800 1400
Wire Wire Line
	1550 1500 1800 1500
Wire Wire Line
	1550 1600 1800 1600
Wire Wire Line
	1550 1700 1800 1700
Wire Wire Line
	1550 1800 1800 1800
Wire Wire Line
	2300 4000 2550 4000
Wire Wire Line
	2300 4100 2550 4100
Wire Wire Line
	2300 4200 2550 4200
Wire Wire Line
	2300 4300 2550 4300
Wire Wire Line
	2300 4400 2550 4400
Wire Wire Line
	2300 4500 2550 4500
Wire Wire Line
	2300 4600 2550 4600
Wire Wire Line
	2300 4700 2550 4700
Wire Wire Line
	1550 4000 1800 4000
Wire Wire Line
	1550 4100 1800 4100
Wire Wire Line
	1550 4200 1800 4200
Wire Wire Line
	1550 4300 1800 4300
Wire Wire Line
	1550 4400 1800 4400
Wire Wire Line
	1550 4500 1800 4500
Wire Wire Line
	1550 4600 1800 4600
Wire Wire Line
	1550 4700 1800 4700
Wire Wire Line
	3950 1300 3550 1300
Wire Wire Line
	3950 1400 3550 1400
Wire Wire Line
	3950 1500 3550 1500
Wire Wire Line
	3950 1600 3550 1600
Wire Wire Line
	3950 1700 3550 1700
Wire Wire Line
	3950 1800 3550 1800
Wire Wire Line
	3950 1900 3550 1900
Wire Wire Line
	3950 2000 3550 2000
Wire Wire Line
	3950 2100 3550 2100
Wire Wire Line
	3950 2200 3550 2200
Wire Wire Line
	3950 2300 3550 2300
Text Label 3600 1300 0    60   ~ 0
P120
Text Label 3600 1400 0    60   ~ 0
P43
Text Label 3600 1500 0    60   ~ 0
P42
Text Label 3600 1600 0    60   ~ 0
P41
Text Label 3600 1700 0    60   ~ 0
P40
Text Label 3600 1800 0    60   ~ 0
/RESET
Text Label 3600 1900 0    60   ~ 0
P124
Text Label 3600 2000 0    60   ~ 0
P123
Text Label 3600 2100 0    60   ~ 0
P137
Text Label 3600 2200 0    60   ~ 0
P122
Text Label 3600 2300 0    60   ~ 0
P121
Entry Wire Line
	3450 1200 3550 1300
Entry Wire Line
	3450 1300 3550 1400
Entry Wire Line
	3450 1400 3550 1500
Entry Wire Line
	3450 1500 3550 1600
Entry Wire Line
	3450 1600 3550 1700
Entry Wire Line
	3450 1700 3550 1800
Entry Wire Line
	3450 1800 3550 1900
Entry Wire Line
	3450 1900 3550 2000
Entry Wire Line
	3450 2000 3550 2100
Entry Wire Line
	3450 2100 3550 2200
Entry Wire Line
	3450 2200 3550 2300
Entry Wire Line
	1450 1200 1550 1300
Entry Wire Line
	1450 1300 1550 1400
Entry Wire Line
	1450 1400 1550 1500
Entry Wire Line
	1450 1500 1550 1600
Entry Wire Line
	1450 1600 1550 1700
Entry Wire Line
	1450 1700 1550 1800
Entry Wire Line
	2550 1300 2650 1200
Entry Wire Line
	2550 1400 2650 1300
Entry Wire Line
	2550 1500 2650 1400
Entry Wire Line
	2550 1600 2650 1500
Entry Wire Line
	2550 1700 2650 1600
Entry Wire Line
	3450 3300 3550 3200
Wire Bus Line
	3450 2200 3450 1000
Wire Bus Line
	3450 1000 3350 900 
Wire Bus Line
	3350 900  1550 900 
Wire Bus Line
	1550 900  1450 1000
Wire Bus Line
	2650 1000 2750 900 
Text Label 1550 1300 0    60   ~ 0
P120
Text Label 2300 1300 0    60   ~ 0
P43
Text Label 1550 1400 0    60   ~ 0
P42
Text Label 2300 1400 0    60   ~ 0
P41
Text Label 1550 1500 0    60   ~ 0
P40
Text Label 2300 1500 0    60   ~ 0
/RESET
Text Label 1550 1600 0    60   ~ 0
P124
Text Label 2300 1600 0    60   ~ 0
P123
Text Label 1550 1700 0    60   ~ 0
P137
Text Label 2300 1700 0    60   ~ 0
P122
Text Label 1550 1800 0    60   ~ 0
P121
$Comp
L CONN_02X08 P?
U 1 1 57B27F81
P 2050 1650
F 0 "P?" H 2050 2100 50  0000 C CNN
F 1 "CONN_02X08" V 2050 1650 50  0000 C CNN
F 2 "" H 2050 450 50  0000 C CNN
F 3 "" H 2050 450 50  0000 C CNN
	1    2050 1650
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X08 P?
U 1 1 57B27FB8
P 2050 4350
F 0 "P?" H 2050 4800 50  0000 C CNN
F 1 "CONN_02X08" V 2050 4350 50  0000 C CNN
F 2 "" H 2050 3150 50  0000 C CNN
F 3 "" H 2050 3150 50  0000 C CNN
	1    2050 4350
	1    0    0    -1  
$EndComp
Wire Bus Line
	1450 1000 1450 1700
Wire Bus Line
	2650 1600 2650 1000
Wire Wire Line
	3100 2400 3100 2500
Wire Wire Line
	3950 3200 3550 3200
Wire Wire Line
	3950 3300 3550 3300
Wire Wire Line
	3950 3400 3550 3400
Wire Wire Line
	3950 3500 3550 3500
Wire Wire Line
	3950 3600 3550 3600
Wire Wire Line
	3950 3700 3550 3700
Wire Wire Line
	3950 3800 3550 3800
Wire Wire Line
	3950 3900 3550 3900
Wire Wire Line
	3950 4000 3550 4000
Wire Wire Line
	3950 4100 3550 4100
Wire Wire Line
	3950 4200 3550 4200
Wire Wire Line
	3950 4300 3550 4300
Wire Wire Line
	3950 4400 3550 4400
Wire Wire Line
	3950 4500 3550 4500
Wire Wire Line
	3950 4600 3550 4600
Wire Wire Line
	3950 4700 3550 4700
Text Label 3600 3200 0    60   ~ 0
P60
Text Label 3600 3300 0    60   ~ 0
P61
Text Label 3600 3400 0    60   ~ 0
P62
Text Label 3600 3500 0    60   ~ 0
P63
Text Label 3600 3600 0    60   ~ 0
P31
Text Label 3600 3700 0    60   ~ 0
P77
Text Label 3600 3800 0    60   ~ 0
P76
Text Label 3600 3900 0    60   ~ 0
P75
Text Label 3600 4000 0    60   ~ 0
P74
Text Label 3600 4100 0    60   ~ 0
P73
Text Label 3600 4200 0    60   ~ 0
P72
Text Label 3600 4300 0    60   ~ 0
P71
Text Label 3600 4400 0    60   ~ 0
P70
Text Label 3600 4500 0    60   ~ 0
P06
Text Label 3600 4600 0    60   ~ 0
P05
Text Label 3600 4700 0    60   ~ 0
P30
Entry Wire Line
	3450 3400 3550 3300
Entry Wire Line
	3450 3500 3550 3400
Entry Wire Line
	3450 3600 3550 3500
Entry Wire Line
	3450 3700 3550 3600
Entry Wire Line
	3450 3800 3550 3700
Entry Wire Line
	3450 4000 3550 3900
Entry Wire Line
	3450 4100 3550 4000
Entry Wire Line
	3450 4200 3550 4100
Entry Wire Line
	3450 3900 3550 3800
Entry Wire Line
	3450 4500 3550 4400
Entry Wire Line
	3450 4600 3550 4500
Entry Wire Line
	3450 4800 3550 4700
Entry Wire Line
	2550 4100 2650 4200
Entry Wire Line
	2550 4200 2650 4300
Entry Wire Line
	2550 4000 2650 4100
Entry Wire Line
	3450 4700 3550 4600
Entry Wire Line
	3450 4400 3550 4300
Entry Wire Line
	3450 4300 3550 4200
Entry Wire Line
	2550 4300 2650 4400
Entry Wire Line
	2550 4400 2650 4500
Entry Wire Line
	2550 4500 2650 4600
Entry Wire Line
	2550 4600 2650 4700
Entry Wire Line
	2550 4700 2650 4800
Entry Wire Line
	1450 4100 1550 4000
Entry Wire Line
	1450 4200 1550 4100
Entry Wire Line
	1450 4300 1550 4200
Entry Wire Line
	1450 4400 1550 4300
Entry Wire Line
	1450 4500 1550 4400
Entry Wire Line
	1450 4600 1550 4500
Entry Wire Line
	1450 4700 1550 4600
Entry Wire Line
	1450 4800 1550 4700
Text Label 1550 4000 0    60   ~ 0
P60
Text Label 1550 4100 0    60   ~ 0
P62
Text Label 1550 4200 0    60   ~ 0
P31
Text Label 1550 4300 0    60   ~ 0
P76
Text Label 1550 4400 0    60   ~ 0
P74
Text Label 1550 4500 0    60   ~ 0
P72
Text Label 1550 4600 0    60   ~ 0
P70
Text Label 1550 4700 0    60   ~ 0
P05
Text Label 2300 4000 0    60   ~ 0
P61
Text Label 2300 4100 0    60   ~ 0
P63
Text Label 2300 4200 0    60   ~ 0
P77
Text Label 2300 4300 0    60   ~ 0
P75
Text Label 2300 4400 0    60   ~ 0
P73
Text Label 2300 4500 0    60   ~ 0
P71
Text Label 2300 4600 0    60   ~ 0
P06
Text Label 2300 4700 0    60   ~ 0
P30
Wire Bus Line
	3450 3300 3450 5100
Wire Bus Line
	3450 5100 3400 5150
Wire Bus Line
	3400 5150 1500 5150
Wire Bus Line
	1500 5150 1450 5100
Wire Bus Line
	2650 5100 2700 5150
Wire Wire Line
	7750 1300 8150 1300
Wire Wire Line
	7750 1400 8150 1400
Wire Wire Line
	7750 1500 8150 1500
Wire Wire Line
	7750 1600 8150 1600
Wire Wire Line
	7750 1700 8150 1700
Wire Wire Line
	7750 1800 8150 1800
Wire Wire Line
	7750 1900 8150 1900
Wire Wire Line
	7750 2000 8150 2000
Wire Wire Line
	7750 2100 8150 2100
Wire Wire Line
	7750 2200 8150 2200
Wire Wire Line
	7750 2300 8150 2300
Wire Wire Line
	7750 2400 8150 2400
Wire Wire Line
	7750 2500 8150 2500
Wire Wire Line
	7750 2600 8150 2600
Wire Wire Line
	7750 2800 8150 2800
Wire Wire Line
	7750 3200 8150 3200
Wire Wire Line
	7750 3300 8150 3300
Wire Wire Line
	7750 3400 8150 3400
Wire Wire Line
	7750 3500 8150 3500
Wire Wire Line
	7750 3600 8150 3600
Wire Wire Line
	7750 3700 8150 3700
Wire Wire Line
	7750 3800 8150 3800
Wire Wire Line
	7750 3900 8150 3900
Wire Wire Line
	7750 4000 8150 4000
Wire Wire Line
	7750 4100 8150 4100
Wire Wire Line
	7750 4200 8150 4200
Wire Wire Line
	7750 4300 8150 4300
Wire Wire Line
	7750 4400 8150 4400
Wire Wire Line
	7750 4500 8150 4500
Wire Wire Line
	7750 4600 8150 4600
Wire Wire Line
	7750 4700 8150 4700
Wire Wire Line
	7750 2700 8150 2700
$Comp
L CONN_02X08 P?
U 1 1 57B2A5B7
P 9400 1650
F 0 "P?" H 9400 2100 50  0000 C CNN
F 1 "CONN_02X08" V 9400 1650 50  0000 C CNN
F 2 "" H 9400 450 50  0000 C CNN
F 3 "" H 9400 450 50  0000 C CNN
	1    9400 1650
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X08 P?
U 1 1 57B2A5EC
P 9400 4350
F 0 "P?" H 9400 4800 50  0000 C CNN
F 1 "CONN_02X08" V 9400 4350 50  0000 C CNN
F 2 "" H 9400 3150 50  0000 C CNN
F 3 "" H 9400 3150 50  0000 C CNN
	1    9400 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 1300 8850 1300
Wire Wire Line
	9150 1400 8850 1400
Wire Wire Line
	9150 1500 8850 1500
Wire Wire Line
	9150 1600 8850 1600
Wire Wire Line
	9150 1700 8850 1700
Wire Wire Line
	9150 1800 8850 1800
Wire Wire Line
	9150 1900 8850 1900
Wire Wire Line
	9150 2000 8850 2000
Wire Wire Line
	9950 1300 9650 1300
Wire Wire Line
	9950 1400 9650 1400
Wire Wire Line
	9950 1500 9650 1500
Wire Wire Line
	9950 1600 9650 1600
Wire Wire Line
	9950 1700 9650 1700
Wire Wire Line
	9950 1800 9650 1800
Wire Wire Line
	9950 1900 9650 1900
Wire Wire Line
	9950 2000 9650 2000
Wire Wire Line
	9150 4000 8850 4000
Wire Wire Line
	9150 4100 8850 4100
Wire Wire Line
	9150 4200 8850 4200
Wire Wire Line
	9150 4300 8850 4300
Wire Wire Line
	9150 4400 8850 4400
Wire Wire Line
	9150 4500 8850 4500
Wire Wire Line
	9150 4600 8850 4600
Wire Wire Line
	9150 4700 8850 4700
Wire Wire Line
	9950 4000 9650 4000
Wire Wire Line
	9950 4100 9650 4100
Wire Wire Line
	9950 4200 9650 4200
Wire Wire Line
	9950 4300 9650 4300
Wire Wire Line
	9950 4400 9650 4400
Wire Wire Line
	9950 4500 9650 4500
Wire Wire Line
	9950 4600 9650 4600
Wire Wire Line
	9950 4700 9650 4700
Entry Wire Line
	9950 1300 10050 1200
Entry Wire Line
	9950 1400 10050 1300
Entry Wire Line
	9950 1500 10050 1400
Entry Wire Line
	9950 1600 10050 1500
Entry Wire Line
	9950 1700 10050 1600
Entry Wire Line
	9950 1800 10050 1700
Entry Wire Line
	9950 1900 10050 1800
Entry Wire Line
	9950 2000 10050 1900
Entry Wire Line
	8150 1300 8250 1200
Entry Wire Line
	8150 1400 8250 1300
Entry Wire Line
	8150 1500 8250 1400
Entry Wire Line
	8150 1600 8250 1500
Entry Wire Line
	8150 1700 8250 1600
Entry Wire Line
	8150 1800 8250 1700
Entry Wire Line
	8150 1900 8250 1800
Entry Wire Line
	8150 2000 8250 1900
Entry Wire Line
	8150 2100 8250 2000
Entry Wire Line
	8150 2200 8250 2100
Entry Wire Line
	8150 2300 8250 2200
Entry Wire Line
	8150 2400 8250 2300
Entry Wire Line
	8150 2500 8250 2400
Entry Wire Line
	8150 2600 8250 2500
Entry Wire Line
	8150 2700 8250 2600
Entry Wire Line
	8150 2800 8250 2700
Entry Wire Line
	8750 1200 8850 1300
Entry Wire Line
	8750 1300 8850 1400
Entry Wire Line
	8750 1400 8850 1500
Entry Wire Line
	8750 1500 8850 1600
Entry Wire Line
	8750 1600 8850 1700
Entry Wire Line
	8750 1700 8850 1800
Entry Wire Line
	8750 1800 8850 1900
Entry Wire Line
	8750 1900 8850 2000
Wire Bus Line
	10050 1900 10050 900 
Wire Bus Line
	10050 900  9950 800 
Wire Bus Line
	9950 800  8350 800 
Wire Bus Line
	8350 800  8250 900 
Wire Bus Line
	8250 900  8250 2700
Wire Bus Line
	8750 1900 8750 900 
Wire Bus Line
	8750 900  8650 800 
Entry Wire Line
	8150 3200 8250 3300
Entry Wire Line
	8150 3300 8250 3400
Entry Wire Line
	8150 3400 8250 3500
Entry Wire Line
	8150 3500 8250 3600
Entry Wire Line
	8150 3600 8250 3700
Entry Wire Line
	8150 3700 8250 3800
Entry Wire Line
	8150 3800 8250 3900
Entry Wire Line
	8150 3900 8250 4000
Entry Wire Line
	8150 4000 8250 4100
Entry Wire Line
	8150 4100 8250 4200
Entry Wire Line
	8150 4200 8250 4300
Entry Wire Line
	8150 4300 8250 4400
Entry Wire Line
	8150 4400 8250 4500
Entry Wire Line
	8150 4500 8250 4600
Entry Wire Line
	8150 4600 8250 4700
Entry Wire Line
	8150 4700 8250 4800
Entry Wire Line
	9950 4000 10050 4100
Entry Wire Line
	9950 4100 10050 4200
Entry Wire Line
	9950 4200 10050 4300
Entry Wire Line
	9950 4300 10050 4400
Entry Wire Line
	9950 4400 10050 4500
Entry Wire Line
	9950 4500 10050 4600
Entry Wire Line
	9950 4600 10050 4700
Entry Wire Line
	9950 4700 10050 4800
Entry Wire Line
	8750 4200 8850 4100
Entry Wire Line
	8750 4300 8850 4200
Entry Wire Line
	8750 4400 8850 4300
Entry Wire Line
	8750 4600 8850 4500
Entry Wire Line
	8750 4500 8850 4400
Entry Wire Line
	8750 4700 8850 4600
Entry Wire Line
	8750 4800 8850 4700
Entry Wire Line
	8750 4100 8850 4000
Wire Bus Line
	10050 4100 10050 5100
Wire Bus Line
	10050 5100 9950 5200
Wire Bus Line
	9950 5200 8350 5200
Wire Bus Line
	8350 5200 8250 5100
Wire Bus Line
	8250 5100 8250 3300
Wire Bus Line
	8750 4100 8750 5100
Wire Bus Line
	8750 5100 8650 5200
Text Label 7750 1300 0    60   ~ 0
P140
Text Label 7750 1400 0    60   ~ 0
P141
Text Label 7750 1500 0    60   ~ 0
P00
Text Label 7750 1600 0    60   ~ 0
P01
Text Label 7750 1700 0    60   ~ 0
P02
Text Label 7750 1800 0    60   ~ 0
P03
Text Label 7750 1900 0    60   ~ 0
P04
Text Label 7750 2000 0    60   ~ 0
P130
Text Label 7750 2100 0    60   ~ 0
P20
Text Label 7750 2200 0    60   ~ 0
P21
Text Label 7750 2300 0    60   ~ 0
P22
Text Label 7750 2400 0    60   ~ 0
P23
Text Label 7750 2500 0    60   ~ 0
P24
Text Label 7750 2600 0    60   ~ 0
P25
Text Label 7750 2700 0    60   ~ 0
P26
Text Label 7750 2800 0    60   ~ 0
P27
Text Label 7750 3200 0    60   ~ 0
P147
Text Label 7750 3300 0    60   ~ 0
P146
Text Label 7750 3400 0    60   ~ 0
P10
Text Label 7750 3500 0    60   ~ 0
P11
Text Label 7750 3600 0    60   ~ 0
P12
Text Label 7750 3700 0    60   ~ 0
P13
Text Label 7750 3800 0    60   ~ 0
P14
Text Label 7750 3900 0    60   ~ 0
P15
Text Label 7750 4000 0    60   ~ 0
P16
Text Label 7750 4100 0    60   ~ 0
P17
Text Label 7750 4200 0    60   ~ 0
P55
Text Label 7750 4300 0    60   ~ 0
P54
Text Label 7750 4400 0    60   ~ 0
P53
Text Label 7750 4500 0    60   ~ 0
P52
Text Label 7750 4600 0    60   ~ 0
P51
Text Label 7750 4700 0    60   ~ 0
P50
Text Label 9650 1300 0    60   ~ 0
P140
Text Label 9650 1400 0    60   ~ 0
P00
Text Label 9650 1500 0    60   ~ 0
P02
Text Label 9650 1600 0    60   ~ 0
P04
Text Label 9650 1700 0    60   ~ 0
P20
Text Label 9650 1800 0    60   ~ 0
P22
Text Label 9650 1900 0    60   ~ 0
P24
Text Label 9650 2000 0    60   ~ 0
P26
Text Label 8850 1300 0    60   ~ 0
P141
Text Label 8850 1400 0    60   ~ 0
P01
Text Label 8850 1500 0    60   ~ 0
P03
Text Label 8850 1600 0    60   ~ 0
P130
Text Label 8850 1700 0    60   ~ 0
P21
Text Label 8850 1800 0    60   ~ 0
P23
Text Label 8850 1900 0    60   ~ 0
P25
Text Label 8850 2000 0    60   ~ 0
P27
Text Label 9650 4000 0    60   ~ 0
P147
Text Label 9650 4100 0    60   ~ 0
P10
Text Label 9650 4200 0    60   ~ 0
P12
Text Label 9650 4300 0    60   ~ 0
P14
Text Label 9650 4400 0    60   ~ 0
P16
Text Label 9650 4500 0    60   ~ 0
P55
Text Label 9650 4600 0    60   ~ 0
P53
Text Label 9650 4700 0    60   ~ 0
P51
Text Label 8850 4000 0    60   ~ 0
P146
Text Label 8850 4100 0    60   ~ 0
P11
Text Label 8850 4200 0    60   ~ 0
P13
Text Label 8850 4300 0    60   ~ 0
P15
Text Label 8850 4400 0    60   ~ 0
P17
Text Label 8850 4500 0    60   ~ 0
P54
Text Label 8850 4600 0    60   ~ 0
P52
Text Label 8850 4700 0    60   ~ 0
P50
Wire Bus Line
	1450 5100 1450 4100
Wire Bus Line
	2650 5100 2650 4100
Wire Wire Line
	1800 2000 1550 2000
Wire Wire Line
	1550 2000 1550 2200
Wire Wire Line
	1550 2200 2850 2200
Wire Wire Line
	2550 2200 2550 2000
Wire Wire Line
	2550 2000 2300 2000
Wire Wire Line
	1800 1900 1450 1900
Wire Wire Line
	1450 1900 1450 2300
Wire Wire Line
	1450 2300 2650 2300
Wire Wire Line
	2650 2300 2650 1900
Wire Wire Line
	2650 1900 2300 1900
Connection ~ 2550 2200
Wire Wire Line
	2550 2300 2550 2500
Connection ~ 2550 2300
$Comp
L GND #PWR?
U 1 1 57B2C317
P 2550 2500
F 0 "#PWR?" H 2550 2250 50  0001 C CNN
F 1 "GND" H 2550 2350 50  0000 C CNN
F 2 "" H 2550 2500 50  0000 C CNN
F 3 "" H 2550 2500 50  0000 C CNN
	1    2550 2500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 57B2C33B
P 2850 2200
F 0 "#PWR?" H 2850 2050 50  0001 C CNN
F 1 "+3.3V" H 2850 2340 50  0000 C CNN
F 2 "" H 2850 2200 50  0000 C CNN
F 3 "" H 2850 2200 50  0000 C CNN
	1    2850 2200
	0    1    1    0   
$EndComp
$EndSCHEMATC
