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
module Branch_Decision(
    src1_i,
    src2_i,
    branch_type_i,
    branch_i,
	result_o
	);
     
//I/O ports
input  [32-1:0]  src1_i;
input  [32-1:0]	 src2_i;
input  [2-1:0]   branch_type_i;
input            branch_i;
output           result_o;

//Internal Signals
reg              result_o;

always@(*) begin
    if(branch_i) begin
        case(branch_type_i)
            0: begin
                if(src1_i==src2_i) result_o = 1;
                else result_o = 0;
            end
            1: begin
                if(src1_i>src2_i) result_o = 1;
                else result_o = 0;
            end
            2: begin
                if(src1_i>=src2_i) result_o = 1;
                else result_o = 0;
            end
            3: begin
                if(src1_i!=src2_i) result_o = 1;
                else result_o = 0;
            end
         endcase
    end
    else result_o = 0;
end
endmodule               