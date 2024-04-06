module Lab3_Sequence_Recognizer_structure (input x, clock, reset, output z);
    wire A,B,C,DA,DB,DC;
    assign DA = (x&~C) | (~x&B&C);
    assign DB = (x&C) | (~x&A&~C) | (~x&B&~C);
    assign DC = ~x;
    assign z = (~x&B&~C) | (x&A&C);
    D_ff_AR _A(A,DA,clock,reset);
    D_ff_AR _B(B,DB,clock,reset);
    D_ff_AR _C(C,DC,clock,reset);
endmodule