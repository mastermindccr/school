`timescale 1ns/1ps

//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date:    15:15:11 08/18/2013
// Design Name:
// Module Name:    alu
// Project Name:
// Target Devices:
// Tool versions:
// Description:
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////

module alu(
           clk,           // system clock              (input)
           rst_n,         // negative reset            (input)
           src1,          // 32 bits source 1          (input)
           src2,          // 32 bits source 2          (input)
           ALU_control,   // 4 bits ALU control input  (input)
           result,        // 32 bits result            (output)
           zero,          // 1 bit when the output is 0, zero must be set (output)
           cout,          // 1 bit carry out           (output)
           overflow       // 1 bit overflow            (output)
           );

    input           clk;
    input           rst_n;
    input  [32-1:0] src1;
    input  [32-1:0] src2;
    input   [4-1:0] ALU_control;

    output [32-1:0] result;
    output          zero;
    output          cout;
    output          overflow;

    reg    [32-1:0] result;
    reg             zero;
    reg             cout;
    reg             overflow;

    reg    A_invert, B_invert, first_carry;
    reg    [2-1:0] operation;
	reg    [32-1:0] temp_src1;
	reg    [32-1:0] temp_src2;
    wire   [32-1:0] tmp;
    wire   [32-1:0] carry;
	
	always@(ALU_control) begin
        case(ALU_control)
            //AND
            4'b0000: begin
                A_invert = 1'b0;
                B_invert = 1'b0;
                first_carry = 1'b0;
                operation = 2'b00;
            end
            //OR
            4'b0001:
            begin
                A_invert = 1'b0;
                B_invert = 1'b0;
                first_carry = 1'b0;
                operation = 2'b01;
            end
            //ADD
            4'b0010: begin
                A_invert = 1'b0;
                B_invert = 1'b0;
                first_carry = 1'b0;
                operation = 2'b10;
            end
            //SUB
            4'b0110: begin
                A_invert = 1'b0;
                B_invert = 1'b1;
                first_carry = 1'b1; // complement requires +1 in the first bit
                operation = 2'b10;
            end
            //NOR = NOT AND NOT
            4'b1100: begin
                A_invert = 1'b1;
                B_invert = 1'b1;
                first_carry = 1'b0;
                operation = 2'b00;
            end
            //SLT
            4'b0111: begin
                A_invert = 1'b0;
                B_invert = 1'b1;
                first_carry = 1'b1; // same as subtraction
                operation = 2'b10;
            end
        endcase
    end
	
	generate
		alu_top Alu0(
			.src1(src1[0]),
			.src2(src2[0]),
			.less(0),
			.A_invert(A_invert),
			.B_invert(B_invert),
			.cin(first_carry),
			.operation(operation),
			.result(tmp[0]),
			.cout(carry[0])
		);
		for(genvar i = 1;i<32;i=i+1) begin
			alu_top Alu(
				.src1(src1[i]),
				.src2(src2[i]),
				.less(0),
				.A_invert(A_invert),
				.B_invert(B_invert),
				.cin(carry[i-1]),
				.operation(operation),
				.result(tmp[i]),
				.cout(carry[i])
			);
		end
	endgenerate
	
	integer i;
	
    always@( posedge clk or negedge rst_n )
        begin
            if(!rst_n) result = 32'b0;
            else begin
				result = tmp;
                cout = carry[31];
				if(src1[31]==src2[31] && ALU_control==4'b0010 && result[31]!=src1[31]) overflow = 1'b1;
				else if(src1[31]!=src2[31] && ALU_control==4'b0110 && result[31]==src2[31]) overflow = 1'b1;
				else overflow = 1'b0;
                if(ALU_control==4'b0111) begin
					cout = 1'b0;
					if(result[31]==1'b1) result = 32'b1;
					else result = 32'b0;
				end				
                if(result == 32'b0) zero = 1'b1;
				else zero = 1'b0;
            end
        end
	
endmodule
