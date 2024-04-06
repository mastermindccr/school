module Lab3_SbRb_Latch_gatelevel(input Sb, Rb, output Q, Qb);
    nand#(5) (Qb,Q,Rb);
    nand#(5) (Q,Qb,Sb);
endmodule