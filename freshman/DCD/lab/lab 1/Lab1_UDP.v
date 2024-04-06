primitive Lab1_UDP(F,A,B,C,D);
	output	F;
	input	A,B,C,D;

	table
		0 0 0 0 : 0;
		0 0 0 1 : 0;
		0 0 1 0 : 0;
		0 0 1 1 : 0;
		0 1 0 0 : 0;
		0 1 0 1 : 1;
		0 1 1 0 : 1;
		0 1 1 1 : 1;
		1 0 0 0 : 0;
		1 0 0 1 : 1;
		1 0 1 0 : 1;
		1 0 1 1 : 1;
		1 1 0 0 : 0;
		1 1 0 1 : 1;
		1 1 1 0 : 1;
		1 1 1 1 : 1;
	endtable
endprimitive