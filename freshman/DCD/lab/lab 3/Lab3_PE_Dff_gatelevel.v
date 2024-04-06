module Lab3_PE_Dff_gatelevel(input D, clock, output Q, Qb);
    wire w1,w2,w3,w4;

    nand #(5)(w4,w3,D);
    nand #(5)(w1,w2,w4);
    nand #(5)(w2,w1,clock);
    nand #(5)(w3,w2,clock,w4);
    Lab3_SbRb_Latch_gatelevel SbRb(w2,w3,Q,Qb);
endmodule