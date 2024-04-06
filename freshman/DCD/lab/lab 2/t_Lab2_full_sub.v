module t_Lab2_full_sub();
	wire	B,D;
	reg	x,y,z;

	Lab2_full_sub M1(x,y,z,B,D);

	initial begin
		x=1'b0; y=1'b0; z=1'b0;
		#50 z=1'b1;
		#50 y=1'b1; z=1'b0;
		#50 z=1'b1;
		#50 x=1'b1; y=1'b0; z=1'b0;
		#50 z=1'b1;
		#50 y=1'b1; z=1'b0;
		#50 z=1'b1;
	end
	initial #400 $finish;

	initial begin
		$dumpfile("t_Lab2_full_sub.vcd");
		$dumpvars;
	end
endmodule