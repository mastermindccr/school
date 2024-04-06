module Lab2_encoder_5bit_dataflow(input [0:4] D, output [2:0] A, output V);
	assign	V = D[0]|D[1]|D[2]|D[3]|D[4];
	assign	A[0] = !D[0]&(D[1]|(!D[2]&D[3]));
	assign	A[1] = !D[0]&!D[1]&(D[2]|D[3]);
	assign	A[2] = !D[0]&!D[1]&!D[2]&!D[3]&D[4];
endmodule