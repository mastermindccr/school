module t_Lab3_Sequence_Recognizer();
    wire z1,z2;
    reg clock,reset,x;

    Lab3_Sequence_Recognizer_state_diagram state_diagram(x,clock,reset,z1);
    Lab3_Sequence_Recognizer_structure structure(x,clock,reset,z2);
    initial #160 $finish;

    initial begin
        clock = 1;
        reset = 0;
        #3 reset = 1;
        forever
            #5 clock = ~clock;
    end

    initial begin
        #10 x = 0;
        #10 x = 0;
        #10 x = 1;
        #10 x = 1;
        #10 x = 1;
        #10 x = 0;
        #10 x = 0;
        #10 x = 0;
        #10 x = 1;
        #10 x = 0;
        #10 x = 0;
        #10 x = 1;
        #10 x = 0;
        #10 x = 1;
    end
    initial begin
            $dumpfile("t_Lab3_Sequence_Recognizer");
            $dumpvars;
    end
endmodule