# create a dictionary of compatible VLNVs
source $::env(TAPASCO_HOME)/common/common_ip.tcl
dict set stdcomps   dualdma          vlnv   "esa.informatik.tu-darmstadt.de:user:dual_dma:1.12"
dict set stdcomps   system_ila       vlnv   "xilinx.com:ip:system_ila:1.1"
dict set stdcomps   axi_pcie3_0_usp  vlnv   "xilinx.com:ip:xdma:4.0"
dict set stdcomps   clk_wiz          vlnv   "xilinx.com:ip:clk_wiz:6.0"
dict set stdcomps   mig_core         vlnv   "xilinx.com:ip:mig_7series:4.1"
