module Lab2_half_sub_gatelevel(input x, y, output B, D);
	wire	w1;

	xor	#(4) G1(D,x,y);
	not	     G2(w1,x);
	and	#(2) G3(B,w1,y);
endmodule