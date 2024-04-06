//109550060
//Subject:     CO project 2 - Decoder
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
module Decoder(
    instr_op_i,
	RegWrite_o,
	ALU_op_o,
	ALUSrc_o,
	RegDst_o,
	Branch_o,
	MemRead_o,
	MemWrite_o,
	MemtoReg_o
	);
     
//I/O ports
input  [32-1:0] instr_op_i;

output         RegWrite_o;
output [2-1:0] ALU_op_o;
output         ALUSrc_o;
output 	       RegDst_o;
output         Branch_o;
output	       MemRead_o;
output	       MemWrite_o;
output 		   MemtoReg_o;
 
//Internal Signals
reg    [2-1:0] ALU_op_o;
reg            ALUSrc_o;
reg            RegWrite_o;
reg    		   RegDst_o;
reg            Branch_o;
reg	           MemRead_o;
reg	           MemWrite_o;
reg	   		   MemtoReg_o;

//Main function
always@(*) begin
	if(instr_op_i == 32'b0) begin // nop
		RegWrite_o = 1'b0;
		ALU_op_o = 2'b0;
		ALUSrc_o = 1'b0;
		RegDst_o = 1'b0;
		Branch_o = 1'b0;
		MemRead_o = 1'b0;
		MemWrite_o = 1'b0;
		MemtoReg_o = 1'b0;
	end
	else begin
		case(instr_op_i[31:26])
			0: begin // R-type
				RegWrite_o = 1'b1;
				ALU_op_o = 2'b0;
				ALUSrc_o = 1'b0;
				RegDst_o = 1'b1;
				Branch_o = 1'b0;
				MemRead_o = 1'b0;
				MemWrite_o = 1'b0;
				MemtoReg_o = 1'b1;
			end
			8: begin // AddI
				RegWrite_o = 1'b1;
				ALU_op_o = 2'b1;
				ALUSrc_o = 1'b1;
				RegDst_o = 1'b0;
				Branch_o = 1'b0;
				MemRead_o = 1'b0;
				MemWrite_o = 1'b0;
				MemtoReg_o = 1'b1;
			end
			10: begin // SltI
				RegWrite_o = 1'b1;
				ALU_op_o = 2'b10;
				ALUSrc_o = 1'b1;
				RegDst_o = 1'b0;
				Branch_o = 1'b0;
				MemRead_o = 1'b0;
				MemWrite_o = 1'b0;
				MemtoReg_o = 1'b1;
			end
			4: begin // Beq
				RegWrite_o = 1'b0;
				ALU_op_o = 2'b11;
				ALUSrc_o = 1'b0;
				RegDst_o = 1'b0;
				Branch_o = 1'b1;
				MemRead_o = 1'b0;
				MemWrite_o = 1'b0;
				MemtoReg_o = 1'b1;
			end
			35: begin // lw
				RegWrite_o = 1'b1;
				ALU_op_o = 2'b1;
				ALUSrc_o = 1'b1;
				RegDst_o = 1'b0;
				Branch_o = 1'b0;
				MemRead_o = 1'b1;
				MemWrite_o = 1'b0;
				MemtoReg_o = 1'b0;
			end
			43: begin // sw
				RegWrite_o = 1'b0;
				ALU_op_o = 2'b1;
				ALUSrc_o = 1'b1;
				RegDst_o = 1'b0;
				Branch_o = 1'b0;
				MemRead_o = 1'b0;
				MemWrite_o = 1'b1;
			end
		endcase
	end
end

endmodule





                    
                    