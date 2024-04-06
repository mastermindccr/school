module Lab2_4_bit_RBS(input [3:0] X,Y,input Bin,output Bout,output [3:0] Diff);
	wire C1,C2,C3;

	Lab2_full_sub FS0(X[0],Y[0],Bin,C1,Diff[0]);
	Lab2_full_sub FS1(X[1],Y[1],C1,C2,Diff[1]);
	Lab2_full_sub FS2(X[2],Y[2],C2,C3,Diff[2]);
	Lab2_full_sub FS3(X[3],Y[3],C3,Bout,Diff[3]);
endmodule