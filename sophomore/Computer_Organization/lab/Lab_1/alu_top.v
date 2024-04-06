`timescale 1ns/1ps

//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:58:01 10/10/2013
// Design Name: 
// Module Name:    alu_top 
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

module alu_top(
               src1,       //1 bit source 1 (input)
               src2,       //1 bit source 2 (input)
               less,       //1 bit less     (input)
               A_invert,   //1 bit A_invert (input)
               B_invert,   //1 bit B_invert (input)
               cin,        //1 bit carry in (input)
               operation,  //operation      (input)
               result,     //1 bit result   (output)
               cout,       //1 bit carry out(output)
               );

	input         src1;
	input         src2;
	input         less;
	input         A_invert;
	input         B_invert;
	input         cin;
	input [2-1:0] operation;

	output reg     result;
	output reg     cout;
	wire temp_src1, temp_src2;
	assign temp_src1 = A_invert ^ src1;
	assign temp_src2 = B_invert ^ src2;
	always@(temp_src1 or temp_src2 or operation) begin
		case(operation)
			2'b00: begin
				assign result = temp_src1&temp_src2;
				assign cout = 0;
			end
			2'b01: begin
				assign result = temp_src1|temp_src2;
				assign cout = 0;
			end	
			2'b10: begin 
				assign result = temp_src1 ^ temp_src2 ^ cin;
				assign cout = (temp_src1 & temp_src2) | (temp_src1 & cin) | (temp_src2 & cin);
			end
			2'b11: begin
				assign result = less;
				assign cout = (temp_src1 & temp_src2) | (temp_src1 & cin) | (temp_src2 & cin);
			end
		endcase
	end
endmodule
