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
	Jump_o,
	MemRead_o,
	MemWrite_o,
	MemtoReg_o
	);
     
//I/O ports
input  [32-1:0] instr_op_i;

output         RegWrite_o;
output [3-1:0] ALU_op_o;
output         ALUSrc_o;
output [2-1:0] RegDst_o;
output         Branch_o;
output [2-1:0] Jump_o;
output	       MemRead_o;
output	       MemWrite_o;
output [2-1:0] MemtoReg_o;
 
//Internal Signals
reg    [3-1:0] ALU_op_o;
reg            ALUSrc_o;
reg            RegWrite_o;
reg    [2-1:0] RegDst_o;
reg            Branch_o;
reg	   [2-1:0] Jump_o;
reg	           MemRead_o;
reg	           MemWrite_o;
reg	   [2-1:0] MemtoReg_o;

//Parameter

wire [6-1:0] op;
wire [5-1:0] rs;
wire [5-1:0] rt;
wire [5-1:0] rd;
wire [6-1:0] funct;

assign op = instr_op_i[31:26];
assign rs = instr_op_i[25:21];
assign rt = instr_op_i[20:16];
assign rd = instr_op_i[15:11];
assign funct = instr_op_i[5:0];

//Main function
always@(*) begin
	if(instr_op_i==32'b0)begin
		RegWrite_o = 1'b0;
		ALU_op_o = 3'b0;
		ALUSrc_o = 1'b0;
		RegDst_o = 2'b0;
		Branch_o = 1'b0;
		Jump_o = 2'b0;
		MemRead_o = 1'b0;
		MemWrite_o = 1'b0;
		MemtoReg_o = 2'b0;
	end
	case(op)
		8: begin
			RegWrite_o = 1'b1;
			ALU_op_o = 3'b1;
			ALUSrc_o = 1'b1;
			RegDst_o = 2'b0;
			Branch_o = 1'b0;
			Jump_o = 2'b1;
			MemRead_o = 1'b0;
			MemWrite_o = 1'b0;
			MemtoReg_o = 2'b0;
		end
		10: begin
			RegWrite_o = 1'b1;
			ALU_op_o = 3'b010;
			ALUSrc_o = 1'b1;
			RegDst_o = 2'b0;
			Branch_o = 1'b0;
			Jump_o = 2'b1;
			MemRead_o = 1'b0;
			MemWrite_o = 1'b0;
			MemtoReg_o = 2'b0;
		end
		4: begin
			RegWrite_o = 1'b0;
			ALU_op_o = 3'b100;
			ALUSrc_o = 1'b0;
			RegDst_o = 2'b1;
			Branch_o = 1'b1;
			Jump_o = 2'b1;
			MemRead_o = 1'b0;
			MemWrite_o = 1'b0;
			MemtoReg_o = 2'b0;
		end
		0: begin
			if(funct==8 && rd==0 && rt==0) begin // jr
				RegWrite_o = 1'b0;
				ALU_op_o = 3'b0;
				ALUSrc_o = 1'b0;
				RegDst_o = 2'b0;
				Branch_o = 1'b0;
				Jump_o = 2'b10;
				MemRead_o = 1'b0;
				MemWrite_o = 1'b0;
				MemtoReg_o = 2'b0;
			end
			else begin
				RegWrite_o = 1'b1;
				ALU_op_o = 3'b0;
				ALUSrc_o = 1'b0;
				RegDst_o = 2'b1;
				Branch_o = 1'b0;
				Jump_o = 2'b1;
				MemRead_o = 1'b0;
				MemWrite_o = 1'b0;
				MemtoReg_o = 2'b0;
			end
		end
		35: begin // lw
			RegWrite_o = 1'b1;
			ALU_op_o = 3'b001;
			ALUSrc_o = 1'b1;
			RegDst_o = 2'b0;
			Branch_o = 1'b0;
			Jump_o = 2'b1;
			MemRead_o = 1'b1;
			MemWrite_o = 1'b0;
			MemtoReg_o = 2'b1;
		end
		43: begin // sw
			RegWrite_o = 1'b0;
			ALU_op_o = 3'b001;
			ALUSrc_o = 1'b1;
			RegDst_o = 2'b0;
			Branch_o = 1'b0;
			Jump_o = 2'b1;
			MemRead_o = 1'b0;
			MemWrite_o = 2'b1;
		end
		2: begin // jump
			RegWrite_o = 1'b0;
			ALU_op_o = 3'b001;
			ALUSrc_o = 1'b0;
			RegDst_o = 2'b0;
			Branch_o = 1'b0;
			Jump_o = 2'b0;
			MemRead_o = 1'b0;
			MemWrite_o = 1'b0;
			MemtoReg_o = 2'b0;
		end
		3: begin //jal
			RegWrite_o = 1'b1;
			ALU_op_o = 3'b001;
			ALUSrc_o = 1'b1;
			RegDst_o = 2'b10;
			Branch_o = 1'b0;
			Jump_o = 2'b0;
			MemRead_o = 1'b1;
			MemWrite_o = 1'b0;
			MemtoReg_o = 2'b10;
		end
	endcase
end

endmodule





                    
                    