//109550060
`timescale 1ns / 1ps
//Subject:     CO project 4 - Pipe Register
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------
module Pipe_Reg_IF_ID(
    clk_i,
    rst_i,
    IF_ID_Write_i,
    data_i,
    data_o
    );
					
parameter size = 0;

input   clk_i;		  
input   rst_i;
input   [2-1:0]    IF_ID_Write_i;
input   [size-1:0] data_i;
output reg  [size-1:0] data_o;
	  
always@(posedge clk_i) begin
    if(IF_ID_Write_i==2'b1) begin // nop
		data_o <= 0;
    end
    else if(IF_ID_Write_i==2'b10) begin // update
	if(~rst_i)
    		data_o <= 0;
        else
            	data_o <= data_i;
    end
    else begin
	if(~rst_i)
		data_o <= 0;
    end 
end

endmodule	