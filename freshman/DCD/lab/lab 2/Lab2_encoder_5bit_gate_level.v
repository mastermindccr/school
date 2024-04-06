module Lab2_encoder_5bit_gate_level (input [0:4] D, output [2:0] A, output V);
	wire	w1,w2,w3;
	or	(V,D[0],D[1],D[2],D[3],D[4]);
	not	(w1,D[0]);
	and	(w2,w1,D[1]);
	not	(w3,D[1]);
	not	(w4,D[2]);
	and	(w5,w3,w4,D[3]);
	or	(A[0],w2,w5);
	or	(w6,D[2],D[3]);
	and	(A[1],w1,w3,w6);
	not	(w7,D[3]);
	and	(A[2],w1,w3,w4,w7,D[4]);
endmodule
	