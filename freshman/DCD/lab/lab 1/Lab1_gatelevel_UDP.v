module Lab1_gatelevel_UDP(F,A,B,C,D);
	output	F;
	input	A,B,C,D;
	wire	w1,w2,w3,w4,w5;

	Lab1_UDP M0(w2,A,B,w1,D);
	not	 M1(w1,C);
	or	 M2(w3,A,C);
	not	 M3(w4,B);
	and	 M4(w5,w3,w4);
	or	 M5(F,w2,w5);
endmodule
