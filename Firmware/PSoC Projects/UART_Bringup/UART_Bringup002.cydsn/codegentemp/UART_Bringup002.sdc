# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\github\Serial_Controlled_Motor_Driver\Firmware\PSoC Projects\UART_Bringup\UART_Bringup002.cydsn\UART_Bringup002.cyprj
# Date: Mon, 20 Jun 2016 15:08:12 GMT
#set_units -time ns
create_clock -name {Clock_1(FFB)} -period 291.66666666666663 -waveform {0 145.833333333333} [list [get_pins {ClockBlock/ff_div_8}] [get_pins {ClockBlock/ff_div_9}]]
create_clock -name {UART_1_SCBCLK(FFB)} -period 708.33333333333326 -waveform {0 354.166666666667} [list [get_pins {ClockBlock/ff_div_3}]]
create_clock -name {CyRouted1} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFCLK} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFCLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySYSCLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/hfclk}] -edges {1 7 13} -nominal_period 291.66666666666663 [list]
create_generated_clock -name {UART_1_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 17 35} -nominal_period 708.33333333333326 [list]


# Component constraints for C:\github\Serial_Controlled_Motor_Driver\Firmware\PSoC Projects\UART_Bringup\UART_Bringup002.cydsn\TopDesign\TopDesign.cysch
# Project: C:\github\Serial_Controlled_Motor_Driver\Firmware\PSoC Projects\UART_Bringup\UART_Bringup002.cydsn\UART_Bringup002.cyprj
# Date: Mon, 20 Jun 2016 15:08:10 GMT
