module Lab2_4_bit_BLS_behavioral(input[3:0] X,Y,input Bin,output reg Bout,output reg[3:0]Diff);
	
	wire B1,B2,B3,P0,P1,P2,P3,G0,G1,G2,G3;
	assign G0 = !X[0]&Y[0];
	assign G1 = !X[1]&Y[1];
	assign G2 = !X[2]&Y[2];
	assign G3 = !X[3]&Y[3];
	assign P0 = !(X[0]^Y[0]);
	assign P1 = !(X[1]^Y[1]);
	assign P2 = !(X[2]^Y[2]);
	assign P3 = !(X[3]^Y[3]);
	assign B1 = G0|(Bin&P0);
	assign B2 = G1|(P1&G0)|(P1&P0&Bin);
	assign B3 = G2|(P2&G1)|(P2&P1&G0)|(P2&P1&P0&Bin);
	
	always@(X[0] or Y[0] or Bin)
		Diff[0] = X[0]^Y[0]^Bin;
	always@(X[1] or Y[1] or B1)
		Diff[1] = X[1]^Y[1]^B1;
	always@(X[2] or Y[2] or B2)
		Diff[2] = X[2]^Y[2]^B2;
	always@(X[3] or Y[3] or B3)
		Diff[3] = X[3]^Y[3]^B3;
	always@(G3 or P3 or B3)
		Bout = G3|(P3&B3);
endmodule