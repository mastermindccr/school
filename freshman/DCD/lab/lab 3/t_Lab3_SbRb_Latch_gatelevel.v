module t_Lab3_SbRb_Latch_gatelevel();
    wire    Q,Qb;
    reg     Sb,Rb;

    Lab3_SbRb_Latch_gatelevel M1(Sb,Rb,Q,Qb);

    initial begin
            Sb = 1'b0;  Rb = 1'b1;
            #30 Sb = 1'b1;
            #30 Rb = 1'b0;
            #30 Rb = 1'b1;
            #30 Sb = 1'b0;  Rb = 1'b0;
            #30 Sb = 1'b1;  Rb = 1'b1;
            #30 Sb = 1'b0;
    end
    initial #210 $finish;

    initial begin
            $dumpfile("t_Lab3_SbRb_Latch_gatelevel");
            $dumpvars;
    end
endmodule
