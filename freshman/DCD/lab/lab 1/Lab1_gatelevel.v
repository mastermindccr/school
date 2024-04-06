module Lab1_gatelevel(F,A,B,C,D);
	output 	F;
	input	A,B,C,D,E;
	wire	w1,w2,w3,w4,w5,w6,w7,w8;

	or	G1(w1,A,B);
	not	G2(w2,C);
	or	G3(w3,w2,D);
	and	G4(w4,w1,w3);
	or	G5(w5,A,C);
	not	G6(w6,B);
	and	G7(w7,w5,w6);
	or	G8(F,w4,w7);
endmodule
