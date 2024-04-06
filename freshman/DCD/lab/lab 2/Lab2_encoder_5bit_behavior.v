module Lab2_encoder_5bit_behavior(input [0:4] D, output [2:0] A, output V);
	reg A,V;
	always@(D)
	begin
		V = 1;
		if(D[0]==1) A=3'b000;
		else if(D[1]==1) A=3'b001;
		else if(D[2]==1) A=3'b010;
		else if(D[3]==1) A=3'b011;
		else if(D[4]==1) A=3'b100;
		else		 V=1'b0;
	end
endmodule