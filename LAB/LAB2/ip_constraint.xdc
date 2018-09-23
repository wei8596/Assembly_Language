set_property LOC M15 [get_ports {data}]
set_property LOC H17 [get_ports {reset}]
set_property LOC H18 [get_ports {k}]

set_property LOC W22 [get_ports {q}]

set_property IOSTANDARD LVCMOS33 [get_ports {q}]
set_property IOSTANDARD LVCMOS18 [get_ports {data}]
set_property IOSTANDARD LVCMOS18 [get_ports {reset}]
set_property IOSTANDARD LVCMOS18 [get_ports {k}]

set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {k}]