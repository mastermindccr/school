//109550060
//Subject:      CO project 2 - Shift_Left_Two_26
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Description: 
//--------------------------------------------------------------------------------
`timescale 1ns/1ps
module Shift_Left_Two_26(
    data_i,
    data_o
    );

//I/O ports                    
input [26-1:0] data_i;
output [28-1:0] data_o;

//shift left 2
assign data_o = {data_i, 2'b0};
endmodule