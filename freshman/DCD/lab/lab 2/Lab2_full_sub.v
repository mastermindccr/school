module Lab2_full_sub(input x,y,z,output B,D);

	Lab2_half_sub_gatelevel	M0(x,y,B1,D1);
	Lab2_half_sub_gatelevel M1(D1,z,B2,D);
	or	#(2) M2(B,B1,B2);
endmodule
	
	