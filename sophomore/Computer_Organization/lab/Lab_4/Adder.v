//109550060
//Subject:     CO project 2 - Adder
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      Luke
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------
`timescale 1ns/1ps
module Adder(
    src1_i,
	src2_i,
	sum_o
	);
     
//I/O ports
input  [32-1:0]  src1_i;
input  [32-1:0]	 src2_i;
output [32-1:0]	 sum_o;

//Internal Signals
wire    [32-1:0] s_o;

//Parameter
//Main function
assign sum_o[0] = src1_i[0]^src2_i[0];
assign s_o[0] = src1_i[0]&src2_i[0];

for(genvar i = 1;i<32;i=i+1) begin
	assign sum_o[i] = s_o[i-1] ^ src1_i[i] ^ src2_i[i];
	assign s_o[i] = (s_o[i-1]&src1_i[i]) | (s_o[i-1]&src2_i[i]) | (src1_i[i]&src2_i[i]);
end
endmodule





                    
                    