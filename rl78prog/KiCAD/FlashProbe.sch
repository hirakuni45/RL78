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
L Shotkey D?
U 1 1 576EE636
P 3900 2350
F 0 "D?" H 4000 2400 60  0000 C CNN
F 1 "RB751S-40" H 3550 2300 60  0000 C CNN
F 2 "" V 3895 2315 60  0000 C CNN
F 3 "" V 3895 2315 60  0000 C CNN
	1    3900 2350
	-1   0    0    1   
$EndComp
$Comp
L 2N7002K Q?
U 1 1 576EE6C7
P 3450 4250
F 0 "Q?" H 3550 4400 60  0000 C CNN
F 1 "BSS138" H 3150 4100 60  0000 C CNN
F 2 "" H 3450 4250 60  0000 C CNN
F 3 "" H 3450 4250 60  0000 C CNN
	1    3450 4250
	1    0    0    -1  
$EndComp
Text GLabel 2300 3500 0    60   Input ~ 0
TXD
Text GLabel 2300 2700 0    60   Input ~ 0
RXD
Text GLabel 2300 4300 0    60   Input ~ 0
RTS
$Comp
L R R?
U 1 1 576EE8DD
P 3900 1650
F 0 "R?" V 3980 1650 50  0000 C CNN
F 1 "5K" V 3900 1650 50  0000 C CNN
F 2 "" V 3830 1650 50  0000 C CNN
F 3 "" H 3900 1650 50  0000 C CNN
	1    3900 1650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 576EE997
P 3450 6150
F 0 "#PWR?" H 3450 5900 50  0001 C CNN
F 1 "GND" H 3450 6000 50  0000 C CNN
F 2 "" H 3450 6150 50  0000 C CNN
F 3 "" H 3450 6150 50  0000 C CNN
	1    3450 6150
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 576EE9B9
P 3900 1300
F 0 "#PWR?" H 3900 1150 50  0001 C CNN
F 1 "VCC" H 3900 1450 50  0000 C CNN
F 2 "" H 3900 1300 50  0000 C CNN
F 3 "" H 3900 1300 50  0000 C CNN
	1    3900 1300
	1    0    0    -1  
$EndComp
Text GLabel 5850 2100 2    60   BiDi ~ 0
TOOL0(5)
Wire Wire Line
	2300 4300 3200 4300
$Comp
L R R?
U 1 1 576EEBD4
P 2800 4600
F 0 "R?" V 2880 4600 50  0000 C CNN
F 1 "47K" V 2800 4600 50  0000 C CNN
F 2 "" V 2730 4600 50  0000 C CNN
F 3 "" H 2800 4600 50  0000 C CNN
	1    2800 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 4850 2800 4750
Wire Wire Line
	2800 4450 2800 4300
Connection ~ 2800 4300
Text GLabel 5850 3500 2    60   Output ~ 0
/RESET(6)
$Comp
L 6P-SW SW?
U 1 1 578DC4AC
P 4500 3500
F 0 "SW?" H 4500 3800 60  0000 C CNN
F 1 "6P-SW" H 4500 3200 60  0000 C CNN
F 2 "" H 4500 3500 60  0000 C CNN
F 3 "" H 4500 3500 60  0000 C CNN
	1    4500 3500
	1    0    0    -1  
$EndComp
Text GLabel 5850 5000 2    60   BiDi ~ 0
P12/TxD0(44)
Text GLabel 5850 5300 2    60   BiDi ~ 0
P11/RxD0(45)
Wire Wire Line
	5850 5000 5600 5000
Wire Wire Line
	5600 2700 2300 2700
Wire Wire Line
	2300 3500 4000 3500
Wire Wire Line
	4000 3650 3900 3650
Wire Wire Line
	3900 3650 3900 5300
Wire Wire Line
	3900 5300 5850 5300
Wire Wire Line
	5850 3500 5000 3500
Wire Wire Line
	3450 4000 3450 3000
Wire Wire Line
	3450 3000 5100 3000
Wire Wire Line
	5100 3000 5100 3350
Wire Wire Line
	5100 3350 5000 3350
$Comp
L 2P-SW SW?
U 1 1 578DC795
P 4550 5850
F 0 "SW?" H 4600 6050 60  0000 C CNN
F 1 "RESET" H 4500 5650 60  0000 C CNN
F 2 "" H 4555 5850 60  0000 C CNN
F 3 "" H 4555 5850 60  0000 C CNN
	1    4550 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3650 5100 3650
Wire Wire Line
	5100 3650 5100 5800
Wire Wire Line
	5100 5800 4800 5800
Wire Wire Line
	3450 4500 3450 6150
Wire Wire Line
	2800 4850 3450 4850
Connection ~ 3450 4850
Wire Wire Line
	4300 5800 3450 5800
Connection ~ 3450 5800
Wire Wire Line
	3900 2500 3900 3350
Wire Wire Line
	3900 3350 4000 3350
Wire Wire Line
	3900 1800 3900 2200
Wire Wire Line
	3450 2100 5850 2100
Wire Wire Line
	3900 1300 3900 1500
Connection ~ 3900 2100
Wire Wire Line
	3450 2700 3450 2100
Connection ~ 3450 2700
Wire Wire Line
	5600 5000 5600 2700
$Comp
L R R?
U 1 1 578DD23F
P 5350 1650
F 0 "R?" V 5430 1650 50  0000 C CNN
F 1 "10K" V 5350 1650 50  0000 C CNN
F 2 "" V 5280 1650 50  0000 C CNN
F 3 "" H 5350 1650 50  0000 C CNN
	1    5350 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 1800 5350 3500
Connection ~ 5350 3500
Wire Wire Line
	3900 1400 5350 1400
Wire Wire Line
	5350 1400 5350 1500
Connection ~ 3900 1400
$EndSCHEMATC
