module t_Lab3_PE_Dff_gatelevel();
    wire Q,Qb;
    reg D,clock;

    Lab3_PE_Dff_gatelevel M1(D,clock,Q,Qb);

    initial begin clock = 0; forever #20 clock = ~clock; end
    initial fork
        D = 1;
        #25 D = 0;
        #65 D = 1;
        #88 D = 0;
        #122 D = 1;
        #195 D = 0;
    join
    initial #250 $finish;
    initial begin
            $dumpfile("t_Lab3_PE_Dff_gatelevel");
            $dumpvars;
    end
endmodule     