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
L C C3
U 1 1 57AA5A71
P 3100 2650
F 0 "C3" H 3125 2750 50  0000 L CNN
F 1 "1uF" H 3125 2550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3138 2500 50  0001 C CNN
F 3 "" H 3100 2650 50  0000 C CNN
	1    3100 2650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR6
U 1 1 57AA5C37
P 3100 2900
F 0 "#PWR6" H 3100 2650 50  0001 C CNN
F 1 "GND" H 3100 2750 50  0000 C CNN
F 2 "" H 3100 2900 50  0000 C CNN
F 3 "" H 3100 2900 50  0000 C CNN
	1    3100 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR8
U 1 1 57AA5C61
P 3700 2900
F 0 "#PWR8" H 3700 2650 50  0001 C CNN
F 1 "GND" H 3700 2750 50  0000 C CNN
F 2 "" H 3700 2900 50  0000 C CNN
F 3 "" H 3700 2900 50  0000 C CNN
	1    3700 2900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR7
U 1 1 57AA5C8A
P 3500 2700
F 0 "#PWR7" H 3500 2550 50  0001 C CNN
F 1 "+3.3V" H 3500 2840 50  0000 C CNN
F 2 "" H 3500 2700 50  0000 C CNN
F 3 "" H 3500 2700 50  0000 C CNN
	1    3500 2700
	0    -1   -1   0   
$EndComp
$Comp
L C C1
U 1 1 57AA5CE0
P 1850 2950
F 0 "C1" H 1875 3050 50  0000 L CNN
F 1 "0.1uF" H 1875 2850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1888 2800 50  0001 C CNN
F 3 "" H 1850 2950 50  0000 C CNN
	1    1850 2950
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 57AA5D92
P 2250 2950
F 0 "C2" H 2275 3050 50  0000 L CNN
F 1 "0.1uF" H 2275 2850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2288 2800 50  0001 C CNN
F 3 "" H 2250 2950 50  0000 C CNN
	1    2250 2950
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR2
U 1 1 57AA5DDB
P 1850 2600
F 0 "#PWR2" H 1850 2450 50  0001 C CNN
F 1 "+3.3V" H 1850 2740 50  0000 C CNN
F 2 "" H 1850 2600 50  0000 C CNN
F 3 "" H 1850 2600 50  0000 C CNN
	1    1850 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR3
U 1 1 57AA5DF7
P 2250 3300
F 0 "#PWR3" H 2250 3050 50  0001 C CNN
F 1 "GND" H 2250 3150 50  0000 C CNN
F 2 "" H 2250 3300 50  0000 C CNN
F 3 "" H 2250 3300 50  0000 C CNN
	1    2250 3300
	1    0    0    -1  
$EndComp
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
L CONN_02X08 P1
U 1 1 57B27F81
P 2050 1650
F 0 "P1" H 2050 2100 50  0000 C CNN
F 1 "CONN_02X08" V 2050 1650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x08" H 2050 450 50  0001 C CNN
F 3 "" H 2050 450 50  0000 C CNN
	1    2050 1650
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X08 P2
U 1 1 57B27FB8
P 2050 4350
F 0 "P2" H 2050 4800 50  0000 C CNN
F 1 "CONN_02X08" V 2050 4350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x08" H 2050 3150 50  0001 C CNN
F 3 "" H 2050 3150 50  0000 C CNN
	1    2050 4350
	1    0    0    -1  
$EndComp
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
$Comp
L CONN_02X08 P3
U 1 1 57B2A5B7
P 9400 1650
F 0 "P3" H 9400 2100 50  0000 C CNN
F 1 "CONN_02X08" V 9400 1650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x08" H 9400 450 50  0001 C CNN
F 3 "" H 9400 450 50  0000 C CNN
	1    9400 1650
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X08 P4
U 1 1 57B2A5EC
P 9400 2650
F 0 "P4" H 9400 3100 50  0000 C CNN
F 1 "CONN_02X08" V 9400 2650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x08" H 9400 1450 50  0001 C CNN
F 3 "" H 9400 1450 50  0000 C CNN
	1    9400 2650
	1    0    0    -1  
$EndComp
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
	9950 2300 10050 2400
Entry Wire Line
	9950 2400 10050 2500
Entry Wire Line
	9950 2500 10050 2600
Entry Wire Line
	9950 2600 10050 2700
Entry Wire Line
	9950 2700 10050 2800
Entry Wire Line
	9950 2800 10050 2900
Entry Wire Line
	9950 2900 10050 3000
Entry Wire Line
	9950 3000 10050 3100
Entry Wire Line
	8750 2500 8850 2400
Entry Wire Line
	8750 2600 8850 2500
Entry Wire Line
	8750 2700 8850 2600
Entry Wire Line
	8750 2900 8850 2800
Entry Wire Line
	8750 2800 8850 2700
Entry Wire Line
	8750 3000 8850 2900
Entry Wire Line
	8750 3100 8850 3000
Entry Wire Line
	8750 2400 8850 2300
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
P141
Text Label 9650 1400 0    60   ~ 0
P01
Text Label 9650 1500 0    60   ~ 0
P03
Text Label 9650 1600 0    60   ~ 0
P130
Text Label 9650 1700 0    60   ~ 0
P21
Text Label 9650 1800 0    60   ~ 0
P23
Text Label 9650 1900 0    60   ~ 0
P25
Text Label 9650 2000 0    60   ~ 0
P27
Text Label 8850 1300 0    60   ~ 0
P140
Text Label 8850 1400 0    60   ~ 0
P00
Text Label 8850 1500 0    60   ~ 0
P02
Text Label 8850 1600 0    60   ~ 0
P04
Text Label 8850 1700 0    60   ~ 0
P20
Text Label 8850 1800 0    60   ~ 0
P22
Text Label 8850 1900 0    60   ~ 0
P24
Text Label 8850 2000 0    60   ~ 0
P26
Text Label 9650 2300 0    60   ~ 0
P147
Text Label 9650 2400 0    60   ~ 0
P10
Text Label 9650 2500 0    60   ~ 0
P12
Text Label 9650 2600 0    60   ~ 0
P14
Text Label 9650 2700 0    60   ~ 0
P16
Text Label 9650 2800 0    60   ~ 0
P55
Text Label 9650 2900 0    60   ~ 0
P53
Text Label 9650 3000 0    60   ~ 0
P51
Text Label 8850 2300 0    60   ~ 0
P146
Text Label 8850 2400 0    60   ~ 0
P11
Text Label 8850 2500 0    60   ~ 0
P13
Text Label 8850 2600 0    60   ~ 0
P15
Text Label 8850 2700 0    60   ~ 0
P17
Text Label 8850 2800 0    60   ~ 0
P54
Text Label 8850 2900 0    60   ~ 0
P52
Text Label 8850 3000 0    60   ~ 0
P50
$Comp
L GND #PWR4
U 1 1 57B2C317
P 2550 2500
F 0 "#PWR4" H 2550 2250 50  0001 C CNN
F 1 "GND" H 2550 2350 50  0000 C CNN
F 2 "" H 2550 2500 50  0000 C CNN
F 3 "" H 2550 2500 50  0000 C CNN
	1    2550 2500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR5
U 1 1 57B2C33B
P 2850 2200
F 0 "#PWR5" H 2850 2050 50  0001 C CNN
F 1 "+3.3V" H 2850 2340 50  0000 C CNN
F 2 "" H 2850 2200 50  0000 C CNN
F 3 "" H 2850 2200 50  0000 C CNN
	1    2850 2200
	0    1    1    0   
$EndComp
$Comp
L R5F100_REAL U1
U 1 1 57BBABB9
P 5850 3000
F 0 "U1" H 7450 4950 60  0000 C CNN
F 1 "R5F100LxAFB" H 4450 1050 60  0000 C CNN
F 2 "Housings_QFP:LQFP-64_10x10mm_Pitch0.5mm" H 5200 4950 60  0000 C CNN
F 3 "" H 7450 4950 60  0000 C CNN
	1    5850 3000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P5
U 1 1 57BBDAFD
P 2000 5550
F 0 "P5" H 2000 5800 50  0000 C CNN
F 1 "I2C" V 2100 5550 50  0000 C CNN
F 2 "" H 2000 5550 50  0000 C CNN
F 3 "" H 2000 5550 50  0000 C CNN
	1    2000 5550
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P6
U 1 1 57BBE225
P 2000 6150
F 0 "P6" H 2000 6400 50  0000 C CNN
F 1 "I2C" V 2100 6150 50  0000 C CNN
F 2 "" H 2000 6150 50  0000 C CNN
F 3 "" H 2000 6150 50  0000 C CNN
	1    2000 6150
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X03 P7
U 1 1 57BBE25A
P 3050 6100
F 0 "P7" H 3050 6300 50  0000 C CNN
F 1 "TOOL" H 3050 5900 50  0000 C CNN
F 2 "" H 3050 4900 50  0000 C CNN
F 3 "" H 3050 4900 50  0000 C CNN
	1    3050 6100
	1    0    0    -1  
$EndComp
Entry Wire Line
	1450 5500 1550 5600
Entry Wire Line
	1450 5600 1550 5700
Entry Wire Line
	1450 6100 1550 6200
Entry Wire Line
	1450 6200 1550 6300
Text Label 1550 5600 0    60   ~ 0
P60
Text Label 1550 6200 0    60   ~ 0
P60
Text Label 1550 5700 0    60   ~ 0
P61
Text Label 1550 6300 0    60   ~ 0
P61
$Comp
L R R1
U 1 1 57BBE746
P 2050 3700
F 0 "R1" V 2150 3700 50  0000 C CNN
F 1 "1.5K" V 1950 3700 50  0000 C CNN
F 2 "" V 1980 3700 50  0000 C CNN
F 3 "" H 2050 3700 50  0000 C CNN
	1    2050 3700
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 57BBE7E9
P 2750 3700
F 0 "R2" V 2850 3700 50  0000 C CNN
F 1 "1.5K" V 2650 3700 50  0000 C CNN
F 2 "" V 2680 3700 50  0000 C CNN
F 3 "" H 2750 3700 50  0000 C CNN
	1    2750 3700
	0    1    1    0   
$EndComp
Entry Wire Line
	1450 3800 1550 3700
Entry Wire Line
	2650 4100 2750 4000
$Comp
L +3.3V #PWR1
U 1 1 57BBEE97
P 2600 3300
F 0 "#PWR1" H 2600 3150 50  0001 C CNN
F 1 "+3.3V" H 2600 3440 50  0000 C CNN
F 2 "" H 2600 3300 50  0000 C CNN
F 3 "" H 2600 3300 50  0000 C CNN
	1    2600 3300
	1    0    0    -1  
$EndComp
$Comp
L C-POL C4
U 1 1 57BBEFC3
P 1500 2950
F 0 "C4" H 1600 3050 60  0000 C CNN
F 1 "10uF" H 1400 2850 60  0000 C CNN
F 2 "" V 1500 2950 60  0000 C CNN
F 3 "" V 1500 2950 60  0000 C CNN
	1    1500 2950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57BBF5EC
P 1750 6450
F 0 "#PWR?" H 1750 6200 50  0001 C CNN
F 1 "GND" H 1750 6300 50  0000 C CNN
F 2 "" H 1750 6450 50  0000 C CNN
F 3 "" H 1750 6450 50  0000 C CNN
	1    1750 6450
	1    0    0    -1  
$EndComp
$Comp
L SD-CARD-SOCKET P?
U 1 1 57BD4534
P 6550 6750
F 0 "P?" H 6550 7500 60  0000 C CNN
F 1 "SD" H 6350 6000 60  0000 C CNN
F 2 "" H 6475 6675 60  0000 C CNN
F 3 "" H 6475 6675 60  0000 C CNN
	1    6550 6750
	1    0    0    -1  
$EndComp
$Comp
L 2N7002K Q?
U 1 1 57BD4611
P 1950 7050
F 0 "Q?" H 2050 7200 60  0000 C CNN
F 1 "2N7002K" H 1650 6900 60  0000 C CNN
F 2 "" H 1950 7050 60  0000 C CNN
F 3 "" H 1950 7050 60  0000 C CNN
	1    1950 7050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 57BD464E
P 3400 5800
F 0 "#PWR?" H 3400 5650 50  0001 C CNN
F 1 "+3.3V" H 3400 5940 50  0000 C CNN
F 2 "" H 3400 5800 50  0000 C CNN
F 3 "" H 3400 5800 50  0000 C CNN
	1    3400 5800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57BD4742
P 2700 6400
F 0 "#PWR?" H 2700 6150 50  0001 C CNN
F 1 "GND" H 2700 6250 50  0000 C CNN
F 2 "" H 2700 6400 50  0000 C CNN
F 3 "" H 2700 6400 50  0000 C CNN
	1    2700 6400
	1    0    0    -1  
$EndComp
$Comp
L 6P-SW SW?
U 1 1 57BD4B52
P 3100 7000
F 0 "SW?" H 3100 7250 60  0000 C CNN
F 1 "6P-SW" H 3000 6750 60  0000 C CNN
F 2 "" H 3035 7000 60  0000 C CNN
F 3 "" H 3035 7000 60  0000 C CNN
	1    3100 7000
	1    0    0    -1  
$EndComp
$Comp
L SD-CARD-SOCKET P?
U 1 1 57BD56FF
P 5250 6750
F 0 "P?" H 5250 7500 60  0000 C CNN
F 1 "MicroSD" H 5200 6000 60  0000 C CNN
F 2 "" H 5175 6675 60  0000 C CNN
F 3 "" H 5175 6675 60  0000 C CNN
	1    5250 6750
	1    0    0    -1  
$EndComp
Entry Wire Line
	5650 6050 5750 6150
Entry Wire Line
	5650 6250 5750 6350
Entry Wire Line
	5650 6450 5750 6550
Entry Wire Line
	5650 6550 5750 6650
Entry Wire Line
	5650 6750 5750 6850
Entry Wire Line
	5650 7050 5750 7150
Entry Wire Line
	5650 7150 5750 7250
Entry Wire Line
	5650 7250 5750 7350
$Comp
L GND #PWR?
U 1 1 57BD6B00
P 6000 7550
F 0 "#PWR?" H 6000 7300 50  0001 C CNN
F 1 "GND" H 6000 7400 50  0000 C CNN
F 2 "" H 6000 7550 50  0000 C CNN
F 3 "" H 6000 7550 50  0000 C CNN
	1    6000 7550
	1    0    0    -1  
$EndComp
Entry Wire Line
	4350 6050 4450 6150
Entry Wire Line
	4350 6250 4450 6350
Entry Wire Line
	4350 6450 4450 6550
Entry Wire Line
	4350 6550 4450 6650
Entry Wire Line
	4350 6750 4450 6850
Entry Wire Line
	4350 7050 4450 7150
Entry Wire Line
	4350 7150 4450 7250
Entry Wire Line
	4350 7250 4450 7350
$Comp
L GND #PWR?
U 1 1 57BD6DB4
P 4700 7550
F 0 "#PWR?" H 4700 7300 50  0001 C CNN
F 1 "GND" H 4700 7400 50  0000 C CNN
F 2 "" H 4700 7550 50  0000 C CNN
F 3 "" H 4700 7550 50  0000 C CNN
	1    4700 7550
	1    0    0    -1  
$EndComp
Text Label 5750 6150 0    60   ~ 0
/WP
Text Label 4450 6150 0    60   ~ 0
/WP
Text Label 5750 6350 0    60   ~ 0
/CD
Text Label 4450 6350 0    60   ~ 0
/CD
Text Label 5750 6550 0    60   ~ 0
DAT1
Text Label 4450 6550 0    60   ~ 0
DAT1
Text Label 5750 6650 0    60   ~ 0
DO
Text Label 4450 6650 0    60   ~ 0
DO
Text Label 4450 6850 0    60   ~ 0
CLK
Text Label 5750 6850 0    60   ~ 0
CLK
Text Label 4450 7150 0    60   ~ 0
DI
Text Label 5750 7150 0    60   ~ 0
DI
Text Label 4450 7250 0    60   ~ 0
/CS
Text Label 5750 7250 0    60   ~ 0
/CS
Text Label 4450 7350 0    60   ~ 0
DAT2
Text Label 5750 7350 0    60   ~ 0
DAT2
Entry Wire Line
	8150 5100 8250 5000
Entry Wire Line
	8150 5200 8250 5100
Entry Wire Line
	7150 5300 7250 5200
Entry Wire Line
	8150 5300 8250 5200
Entry Wire Line
	7150 5400 7250 5300
Entry Wire Line
	7150 5200 7250 5100
Text Label 7250 5100 0    60   ~ 0
CLK
Text Label 8000 5100 0    60   ~ 0
P10
Text Label 7250 5200 0    60   ~ 0
DO
Text Label 8000 5200 0    60   ~ 0
P11
Text Label 7250 5300 0    60   ~ 0
DI
Text Label 8000 5300 0    60   ~ 0
P12
Entry Wire Line
	7150 5500 7250 5400
Entry Wire Line
	8150 5400 8250 5300
Text Label 7250 5400 0    60   ~ 0
/CD
Text Label 7950 5400 0    60   ~ 0
P146
Entry Wire Line
	8250 5500 8350 5400
Entry Wire Line
	7150 5600 7250 5500
Text Label 7250 5500 0    60   ~ 0
/CS
Text Label 8000 5500 0    60   ~ 0
P00
Entry Wire Line
	8350 5400 8450 5500
$Comp
L DMG3415U Q?
U 1 1 57BE8F4F
P 9300 4600
F 0 "Q?" V 9400 4750 60  0000 C CNN
F 1 "DMG3415U" V 9100 4800 60  0000 C CNN
F 2 "" H 9300 4600 60  0000 C CNN
F 3 "" H 9300 4600 60  0000 C CNN
	1    9300 4600
	0    1    -1   0   
$EndComp
$Comp
L R R?
U 1 1 57BE9145
P 8900 4800
F 0 "R?" V 9000 4800 50  0000 C CNN
F 1 "47K" V 8800 4800 50  0000 C CNN
F 2 "" V 8830 4800 50  0000 C CNN
F 3 "" H 8900 4800 50  0000 C CNN
	1    8900 4800
	1    0    0    -1  
$EndComp
Text Label 8450 5500 0    60   ~ 0
P01
$Comp
L R FB?
U 1 1 57BE95D6
P 8900 4400
F 0 "FB?" V 9000 4400 50  0000 C CNN
F 1 "FB" V 8800 4400 50  0000 C CNN
F 2 "" V 8830 4400 50  0000 C CNN
F 3 "" H 8900 4400 50  0000 C CNN
	1    8900 4400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 57BE99A1
P 9250 3800
F 0 "#PWR?" H 9250 3650 50  0001 C CNN
F 1 "+3.3V" H 9250 3940 50  0000 C CNN
F 2 "" H 9250 3800 50  0000 C CNN
F 3 "" H 9250 3800 50  0000 C CNN
	1    9250 3800
	1    0    0    -1  
$EndComp
$Comp
L C-POL C?
U 1 1 57BE99DF
P 9250 4050
F 0 "C?" H 9350 4150 60  0000 C CNN
F 1 "10uF" H 9150 3950 60  0000 C CNN
F 2 "" V 9250 4050 60  0000 C CNN
F 3 "" V 9250 4050 60  0000 C CNN
	1    9250 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57BE9CB0
P 9250 4300
F 0 "#PWR?" H 9250 4050 50  0001 C CNN
F 1 "GND" H 9250 4150 50  0000 C CNN
F 2 "" H 9250 4300 50  0000 C CNN
F 3 "" H 9250 4300 50  0000 C CNN
	1    9250 4300
	1    0    0    -1  
$EndComp
$Comp
L C-POL C?
U 1 1 57BE9FE7
P 9850 4850
F 0 "C?" H 9950 4950 60  0000 C CNN
F 1 "22uF" H 9750 4750 60  0000 C CNN
F 2 "" V 9850 4850 60  0000 C CNN
F 3 "" V 9850 4850 60  0000 C CNN
	1    9850 4850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57BEA27C
P 9850 5100
F 0 "#PWR?" H 9850 4850 50  0001 C CNN
F 1 "GND" H 9850 4950 50  0000 C CNN
F 2 "" H 9850 5100 50  0000 C CNN
F 3 "" H 9850 5100 50  0000 C CNN
	1    9850 5100
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 57BEA8A0
P 10000 4600
F 0 "#PWR?" H 10000 4450 50  0001 C CNN
F 1 "VCC" H 10000 4750 50  0000 C CNN
F 2 "" H 10000 4600 50  0000 C CNN
F 3 "" H 10000 4600 50  0000 C CNN
	1    10000 4600
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR?
U 1 1 57BEA8E2
P 5900 6950
F 0 "#PWR?" H 5900 6800 50  0001 C CNN
F 1 "VCC" H 5900 7100 50  0000 C CNN
F 2 "" H 5900 6950 50  0000 C CNN
F 3 "" H 5900 6950 50  0000 C CNN
	1    5900 6950
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR?
U 1 1 57BEA924
P 4600 6950
F 0 "#PWR?" H 4600 6800 50  0001 C CNN
F 1 "VCC" H 4600 7100 50  0000 C CNN
F 2 "" H 4600 6950 50  0000 C CNN
F 3 "" H 4600 6950 50  0000 C CNN
	1    4600 6950
	0    -1   -1   0   
$EndComp
$Comp
L LED D?
U 1 1 57BEABBA
P 9850 4300
F 0 "D?" H 9850 4400 50  0000 C CNN
F 1 "RED" H 9850 4200 50  0000 C CNN
F 2 "" H 9850 4300 50  0000 C CNN
F 3 "" H 9850 4300 50  0000 C CNN
	1    9850 4300
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57BEAC1F
P 9850 3850
F 0 "R?" V 9950 3850 50  0000 C CNN
F 1 "1.5K" V 9750 3850 50  0000 C CNN
F 2 "" V 9780 3850 50  0000 C CNN
F 3 "" H 9850 3850 50  0000 C CNN
	1    9850 3850
	1    0    0    -1  
$EndComp
$Comp
L GS2 GS?
U 1 1 57BECD87
P 8900 5500
F 0 "GS?" H 9000 5650 50  0000 C CNN
F 1 "GS2" H 9000 5351 50  0000 C CNN
F 2 "GS2" V 8974 5500 50  0000 C CNN
F 3 "" H 8900 5500 50  0000 C CNN
	1    8900 5500
	0    1    1    0   
$EndComp
Entry Wire Line
	8250 5600 8350 5500
Entry Wire Line
	8250 5700 8350 5600
Text Label 8000 5600 0    60   ~ 0
P02
Text Label 8000 5700 0    60   ~ 0
P03
$Comp
L CONN_01X03 P?
U 1 1 57BEE9FC
P 10900 2500
F 0 "P?" H 10900 2700 50  0000 C CNN
F 1 "T01" V 11000 2500 50  0000 C CNN
F 2 "" H 10900 2500 50  0000 C CNN
F 3 "" H 10900 2500 50  0000 C CNN
	1    10900 2500
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P?
U 1 1 57BEEA53
P 10900 3100
F 0 "P?" H 10900 3300 50  0000 C CNN
F 1 "T02" V 11000 3100 50  0000 C CNN
F 2 "" H 10900 3100 50  0000 C CNN
F 3 "" H 10900 3100 50  0000 C CNN
	1    10900 3100
	1    0    0    -1  
$EndComp
Entry Wire Line
	10050 3000 10150 3100
Entry Wire Line
	10050 2400 10150 2500
Text Label 10150 2500 0    60   ~ 0
P16
Text Label 10150 3100 0    60   ~ 0
P17
$Comp
L GND #PWR?
U 1 1 57BEF11C
P 10600 3300
F 0 "#PWR?" H 10600 3050 50  0001 C CNN
F 1 "GND" H 10600 3150 50  0000 C CNN
F 2 "" H 10600 3300 50  0000 C CNN
F 3 "" H 10600 3300 50  0000 C CNN
	1    10600 3300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 57BEF3E9
P 10500 800
F 0 "#PWR?" H 10500 650 50  0001 C CNN
F 1 "+3.3V" H 10500 940 50  0000 C CNN
F 2 "" H 10500 800 50  0000 C CNN
F 3 "" H 10500 800 50  0000 C CNN
	1    10500 800 
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P?
U 1 1 57BEF4E7
P 10900 1000
F 0 "P?" H 10900 1200 50  0000 C CNN
F 1 "AN2" V 11000 1000 50  0000 C CNN
F 2 "" H 10900 1000 50  0000 C CNN
F 3 "" H 10900 1000 50  0000 C CNN
	1    10900 1000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P?
U 1 1 57BEF53C
P 10900 1550
F 0 "P?" H 10900 1750 50  0000 C CNN
F 1 "AN3" V 11000 1550 50  0000 C CNN
F 2 "" H 10900 1550 50  0000 C CNN
F 3 "" H 10900 1550 50  0000 C CNN
	1    10900 1550
	1    0    0    -1  
$EndComp
Entry Wire Line
	10050 1450 10150 1550
Entry Wire Line
	10050 900  10150 1000
Text Label 10150 1000 0    60   ~ 0
P22
Text Label 10150 1550 0    60   ~ 0
P23
$Comp
L R R?
U 1 1 57FF7CEA
P 1350 7350
F 0 "R?" V 1450 7350 50  0000 C CNN
F 1 "47K" V 1250 7350 50  0000 C CNN
F 2 "" V 1280 7350 50  0000 C CNN
F 3 "" H 1350 7350 50  0000 C CNN
	1    1350 7350
	1    0    0    -1  
$EndComp
Entry Wire Line
	2150 800  2250 900 
Text Label 1850 700  0    60   ~ 0
/RESET
$Comp
L R R?
U 1 1 57FF9881
P 800 1050
F 0 "R?" V 900 1050 50  0000 C CNN
F 1 "10K" V 700 1050 50  0000 C CNN
F 2 "" V 730 1050 50  0000 C CNN
F 3 "" H 800 1050 50  0000 C CNN
	1    800  1050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 57FF9E55
P 800 750
F 0 "#PWR?" H 800 600 50  0001 C CNN
F 1 "+3.3V" H 800 890 50  0000 C CNN
F 2 "" H 800 750 50  0000 C CNN
F 3 "" H 800 750 50  0000 C CNN
	1    800  750 
	1    0    0    -1  
$EndComp
$Comp
L Shotkey D?
U 1 1 57FFBADF
P 3500 7400
F 0 "D?" H 3600 7450 60  0000 C CNN
F 1 "Shotkey" H 3350 7350 60  0000 C CNN
F 2 "" V 3495 7365 60  0000 C CNN
F 3 "" V 3495 7365 60  0000 C CNN
	1    3500 7400
	1    0    0    -1  
$EndComp
Text Label 1550 3700 0    60   ~ 0
P60
Text Label 2750 4000 0    60   ~ 0
P61
Wire Wire Line
	3100 2400 3950 2400
Wire Wire Line
	3100 2800 3100 2900
Wire Wire Line
	3700 2500 3700 2900
Wire Wire Line
	3700 2600 3950 2600
Wire Wire Line
	3700 2500 3950 2500
Connection ~ 3700 2600
Wire Wire Line
	3950 2700 3500 2700
Wire Wire Line
	3950 2800 3600 2800
Wire Wire Line
	3600 2800 3600 2700
Connection ~ 3600 2700
Wire Wire Line
	2250 3300 2250 3100
Wire Wire Line
	1850 2600 1850 2800
Wire Wire Line
	1500 2700 2250 2700
Wire Wire Line
	2250 2700 2250 2800
Connection ~ 1850 2700
Wire Wire Line
	1500 3200 2250 3200
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
	9150 2300 8850 2300
Wire Wire Line
	9150 2400 8850 2400
Wire Wire Line
	9150 2500 8850 2500
Wire Wire Line
	9150 2600 8850 2600
Wire Wire Line
	9150 2700 8850 2700
Wire Wire Line
	9150 2800 8850 2800
Wire Wire Line
	9150 2900 8850 2900
Wire Wire Line
	9150 3000 8850 3000
Wire Wire Line
	9950 2300 9650 2300
Wire Wire Line
	9950 2400 9650 2400
Wire Wire Line
	9950 2500 9650 2500
Wire Wire Line
	9950 2600 9650 2600
Wire Wire Line
	9950 2700 9650 2700
Wire Wire Line
	9950 2800 9650 2800
Wire Wire Line
	9950 2900 9650 2900
Wire Wire Line
	9950 3000 9650 3000
Wire Bus Line
	10050 1900 10050 900 
Wire Bus Line
	10050 900  9950 800 
Wire Bus Line
	9950 800  8350 800 
Wire Bus Line
	8350 800  8250 900 
Wire Bus Line
	8250 900  8250 3000
Wire Bus Line
	8750 1900 8750 900 
Wire Bus Line
	8750 900  8650 800 
Wire Bus Line
	1450 3800 1450 6200
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
Wire Wire Line
	1550 5600 1800 5600
Wire Wire Line
	1550 5700 1800 5700
Wire Wire Line
	1800 6200 1550 6200
Wire Wire Line
	1550 6300 1800 6300
Wire Wire Line
	1550 3700 1900 3700
Wire Wire Line
	2750 4000 3100 4000
Wire Wire Line
	1500 3200 1500 3100
Connection ~ 1850 3200
Wire Wire Line
	1500 2700 1500 2800
Wire Wire Line
	1300 6000 1800 6000
Wire Wire Line
	1300 3500 1300 6000
Wire Wire Line
	1800 5500 1750 5500
Wire Wire Line
	1750 5500 1750 6450
Wire Wire Line
	1750 6100 1800 6100
Connection ~ 1750 6100
Wire Wire Line
	2700 6400 2700 6100
Wire Wire Line
	2400 6100 2800 6100
Wire Wire Line
	3300 6100 3400 6100
Wire Wire Line
	3400 6100 3400 5800
Wire Bus Line
	8250 4900 8300 4950
Wire Bus Line
	8300 4950 8400 4950
Wire Bus Line
	8400 4950 8450 4900
Wire Bus Line
	8450 4900 8450 3450
Wire Bus Line
	8450 3450 8500 3400
Wire Bus Line
	8500 3400 10000 3400
Wire Bus Line
	10000 3400 10050 3350
Wire Bus Line
	10050 3350 10050 2400
Wire Bus Line
	8750 2400 8750 3350
Wire Bus Line
	8750 3350 8700 3400
Wire Wire Line
	6100 6150 5750 6150
Wire Wire Line
	6100 6350 5750 6350
Wire Wire Line
	6100 6550 5750 6550
Wire Wire Line
	6100 6650 5750 6650
Wire Wire Line
	6100 6850 5750 6850
Wire Wire Line
	6100 7150 5750 7150
Wire Wire Line
	6100 7250 5750 7250
Wire Wire Line
	6100 7350 5750 7350
Wire Wire Line
	6100 6750 6000 6750
Wire Wire Line
	6000 6250 6000 7550
Wire Wire Line
	6000 7050 6100 7050
Connection ~ 6000 7050
Wire Wire Line
	4800 6150 4450 6150
Wire Wire Line
	4800 6350 4450 6350
Wire Wire Line
	4800 6550 4450 6550
Wire Wire Line
	4800 6650 4450 6650
Wire Wire Line
	4800 6850 4450 6850
Wire Wire Line
	4800 7150 4450 7150
Wire Wire Line
	4800 7250 4450 7250
Wire Wire Line
	4800 7350 4450 7350
Wire Wire Line
	4800 6750 4700 6750
Wire Wire Line
	4700 6250 4700 7550
Wire Wire Line
	4700 7050 4800 7050
Connection ~ 4700 7050
Wire Wire Line
	6100 6250 6000 6250
Connection ~ 6000 6750
Wire Wire Line
	4800 6250 4700 6250
Connection ~ 4700 6750
Wire Wire Line
	6100 6950 5900 6950
Wire Wire Line
	4800 6950 4600 6950
Wire Bus Line
	4350 5950 4350 7250
Wire Bus Line
	4350 5950 4400 5900
Wire Bus Line
	4400 5900 6900 5900
Wire Bus Line
	5600 5900 5650 5950
Wire Bus Line
	5650 5950 5650 7250
Wire Bus Line
	6900 5900 6950 5950
Wire Bus Line
	6950 5950 6950 6450
Wire Bus Line
	6950 6450 7000 6500
Wire Bus Line
	7000 6500 7100 6500
Wire Bus Line
	7100 6500 7150 6450
Wire Bus Line
	7150 6450 7150 5200
Wire Wire Line
	8150 5100 7250 5100
Wire Wire Line
	7250 5200 8150 5200
Wire Wire Line
	8150 5300 7250 5300
Wire Wire Line
	8150 5400 7250 5400
Wire Bus Line
	8250 3300 8250 5300
Wire Bus Line
	8250 3000 8350 3100
Wire Wire Line
	7250 5500 8250 5500
Wire Bus Line
	8350 3100 8350 5600
Wire Wire Line
	8900 4550 8900 4650
Wire Wire Line
	8900 4600 9050 4600
Connection ~ 8900 4600
Wire Wire Line
	9250 3900 9250 3800
Wire Wire Line
	9250 4200 9250 4300
Wire Wire Line
	9250 3850 8900 3850
Wire Wire Line
	8900 3850 8900 4250
Connection ~ 9250 3850
Wire Wire Line
	9550 4600 10000 4600
Wire Wire Line
	9850 4500 9850 4700
Connection ~ 9850 4600
Wire Wire Line
	9850 5000 9850 5100
Wire Wire Line
	9250 4250 9550 4250
Wire Wire Line
	9550 4250 9550 3650
Wire Wire Line
	9550 3650 9850 3650
Wire Wire Line
	9850 3650 9850 3700
Connection ~ 9250 4250
Wire Wire Line
	9850 4000 9850 4100
Wire Wire Line
	8900 4950 8900 5000
Wire Wire Line
	8900 5000 9250 5000
Wire Wire Line
	9250 4850 9250 5500
Wire Wire Line
	8700 5500 8450 5500
Wire Wire Line
	9250 5500 9100 5500
Connection ~ 9250 5000
Wire Wire Line
	3700 5600 8250 5600
Wire Wire Line
	8250 5700 3800 5700
Wire Wire Line
	10700 2500 10150 2500
Wire Wire Line
	10700 3100 10150 3100
Wire Wire Line
	10700 2600 10600 2600
Wire Wire Line
	10600 1100 10600 3300
Wire Wire Line
	10600 3200 10700 3200
Connection ~ 10600 3200
Wire Wire Line
	10500 3000 10700 3000
Wire Wire Line
	10500 800  10500 3000
Wire Wire Line
	10500 2400 10700 2400
Connection ~ 10500 2400
Wire Wire Line
	10700 1000 10150 1000
Wire Wire Line
	10700 1550 10150 1550
Wire Wire Line
	10700 1100 10600 1100
Connection ~ 10600 2600
Wire Wire Line
	10700 900  10500 900 
Connection ~ 10500 900 
Wire Wire Line
	10700 1450 10500 1450
Connection ~ 10500 1450
Wire Wire Line
	10700 1650 10600 1650
Connection ~ 10600 1650
Wire Wire Line
	1350 6700 1350 7200
Wire Wire Line
	1350 7100 1700 7100
Wire Wire Line
	1350 6700 2300 6700
Wire Wire Line
	2300 6700 2300 6200
Wire Wire Line
	2300 6200 2800 6200
Connection ~ 1350 7100
Wire Wire Line
	1350 7500 1350 7600
Wire Wire Line
	1350 7600 2400 7600
Wire Wire Line
	1950 7600 1950 7300
Wire Wire Line
	2400 7600 2400 6100
Connection ~ 2700 6100
Connection ~ 1950 7600
Wire Wire Line
	1950 6800 1950 6750
Wire Wire Line
	1950 6750 1150 6750
Wire Wire Line
	1150 6750 1150 700 
Wire Wire Line
	1150 700  2150 700 
Wire Wire Line
	2150 700  2150 800 
Wire Wire Line
	800  1200 800  1300
Wire Wire Line
	800  1300 1150 1300
Connection ~ 1150 1300
Wire Wire Line
	800  900  800  750 
Wire Wire Line
	2800 6000 2550 6000
Wire Wire Line
	2550 6000 2550 7000
Wire Wire Line
	2550 7000 2850 7000
Wire Wire Line
	3300 6000 3500 6000
Wire Wire Line
	3350 7000 3500 7000
Wire Wire Line
	3500 7000 3500 6000
Wire Wire Line
	3800 5700 3800 6850
Wire Wire Line
	3800 6850 3350 6850
Wire Wire Line
	2850 6850 2750 6850
Wire Wire Line
	2750 6850 2750 6650
Wire Wire Line
	2750 6650 3700 6650
Wire Wire Line
	3700 6650 3700 5600
Wire Wire Line
	3500 7250 3500 7150
Wire Wire Line
	3500 7150 3350 7150
Wire Wire Line
	2850 7150 2750 7150
Wire Wire Line
	2750 7150 2750 7700
Wire Wire Line
	2750 7600 3500 7600
Wire Wire Line
	3500 7600 3500 7550
Wire Wire Line
	2900 3700 3100 3700
Wire Wire Line
	3100 3700 3100 4000
Wire Wire Line
	2600 3700 2200 3700
Wire Wire Line
	2400 3700 2400 3500
Wire Wire Line
	1300 3500 2600 3500
Wire Wire Line
	2600 3500 2600 3300
Connection ~ 2400 3700
Connection ~ 2400 3500
Wire Wire Line
	2750 7700 1050 7700
Wire Wire Line
	1050 7700 1050 1800
Connection ~ 2750 7600
Entry Wire Line
	1350 1800 1450 1700
Wire Wire Line
	1050 1800 1350 1800
Text Label 1200 1800 0    60   ~ 0
P40
$EndSCHEMATC
