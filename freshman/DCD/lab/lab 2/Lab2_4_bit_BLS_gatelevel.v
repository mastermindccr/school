module Lab2_4_bit_BLS_gatelevel(input[3:0] X, Y, input Bin, output Bout, output[3:0] Diff);
	wire B1,P0,Pp0,G0,Pp0C0,NOT0,B2,P1,Pp1,G1,Pp1G0,NOT1,Pp1Pp0C0,B3,P2,Pp2,G2,Pp2G1,Pp2Pp1G0,Pp2Pp1Pp0C0,P3,Pp3,G3,Pp3G2,Pp3Pp2G1,Pp3Pp2Pp1G0,Pp3Pp2Pp1Pp0C0,NOT2,NOT3;
	
	xor	#(4)(P0,X[0],Y[0]);
	not	(Pp0,P0);
	xor	#(4)(Diff[0],P0,Bin);
	not	(NOT0,X[0]);
	and	#(2)(G0,NOT0,Y[0]);
	and	#(2)(Pp0C0,Bin,Pp0);
	or	#(2)(B1,Pp0C0,G0);

	xor	#(4)(P1,X[1],Y[1]);
	not	(Pp1,P1);
	xor	#(4)(Diff[1],P1,B1);
	not	(NOT1,X[1]);
	and	#(2)(G1,NOT1,Y[1]);
	and	#(2)(Pp1G0,G0,Pp1);
	and	#(2)(Pp1Pp0C0,Pp1,Pp0,Bin);
	or	#(2)(B2,G1,Pp1G0,Pp1Pp0C0);

	xor	#(4)(P2,X[2],Y[2]);
	not	(Pp2,P2);
	xor	#(4)(Diff[2],P2,B2);
	not	(NOT2,X[2]);
	and	#(2)(G2,NOT2,Y[2]);
	and	#(2)(Pp2G1,Pp2,G1);
	and	#(2)(Pp2Pp1G0,Pp2,Pp1,G0);
	and	#(2)(Pp2Pp1Pp0C0,Pp2,Pp1,Pp0,Bin);
	or	#(2)(B3,G2,Pp2G1,Pp2Pp1G0,Pp2Pp1Pp0C0);
	
	xor	#(4)(P3,X[3],Y[3]);
	not	(Pp3,P3);
	xor	#(4)(Diff[3],P3,B3);
	not	(NOT3,X[3]);
	and	#(2)(G3,NOT3,Y[3]);
	and	#(2)(Pp3G2,Pp3,G2);
	and	#(2)(Pp3Pp2G1,Pp3,Pp2,G1);
	and	#(2)(Pp3Pp2Pp1G0,Pp3,Pp2,Pp1,G0);
	and	#(2)(Pp3Pp2Pp1Pp0C0,Pp3,Pp2,Pp1,Pp0,Bin);
	or	#(2)(Bout,G3,Pp3G2,Pp3Pp2G1,Pp3Pp2Pp1G0,Pp3Pp2Pp1Pp0C0);
endmodule