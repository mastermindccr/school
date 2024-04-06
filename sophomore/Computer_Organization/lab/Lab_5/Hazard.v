//109550060
`timescale 1ns/1ps
module Hazard(
    IF_rs_i,
    IF_rt_i,
    ID_rt_i,
    EX_mem_read_i,
    pc_write_o,
    IF_ID_Write_o,
    ID_flush_o
	);
     
//I/O ports
input  [5-1:0]  IF_rs_i;
input  [5-1:0]	IF_rt_i;
input  [5-1:0]  ID_rt_i;
input           EX_mem_read_i;
output reg      pc_write_o;
output reg [1:0]IF_ID_Write_o;
output reg      ID_flush_o;

always@(*) begin
    if(EX_mem_read_i && (IF_rs_i==ID_rt_i || IF_rt_i==ID_rt_i)) begin // load use data hazard
        pc_write_o = 0;
        IF_ID_Write_o = 2'b0; // no update
        ID_flush_o = 1;
    end
    else begin // no load use data hazard
        pc_write_o = 1;
        IF_ID_Write_o = 2'b10; // update
        ID_flush_o = 0;
    end
end

endmodule