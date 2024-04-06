module t_Lab2_encoder_5bit();
	wire	[2:0] A1;
	wire	[2:0] A2;
	wire	[2:0] A3;
	wire	V1,V2,V3;
	reg	[0:4] D;

	Lab2_encoder_5bit_gate_level M0(D,A1,V1);
	Lab2_encoder_5bit_dataflow M1(D,A2,V2);
	Lab2_encoder_5bit_behavior M2(D,A3,V3);

	initial begin
		D=5'b00000;
		#50 D=5'b00001;
		#50 D=5'b00011;
		#50 D=5'b00110;
		#50 D=5'b01001;
		#50 D=5'b01101;
		#50 D=5'b10110;
		#50 D=5'b11010;
	end
	initial	#400 $finish;

	initial begin
		$dumpfile("t_Lab2_encoder_5bit.vcd");
		$dumpvars;
	end
endmodule