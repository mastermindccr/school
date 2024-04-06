module Lab3_Sequence_Recognizer_state_diagram (input x, clock, reset, output reg z);
    reg[2:0] state,next_state;
    initial state = 3'b000;
    parameter S0 = 3'b000, S1 = 3'b001, S2 = 3'b010, S3 = 3'b011, S4 = 3'b100, S5 = 3'b101;
    always@(posedge clock,negedge reset)
        if(reset == 0) state <= S0;
        else state <= next_state;

    always@(state,x)
        case(state)
            S0:if(x) next_state = S4; else next_state = S1;
            S1:if(x) next_state = S2; else next_state = S1;
            S2:if(x) next_state = S4; else next_state = S3;
            S3:if(x) next_state = S2; else next_state = S5;
            S4:if(x) next_state = S4; else next_state = S3;
            S5:if(x) next_state = S2; else next_state = S1;
        endcase

    always@(state,x)
        case(state)
            S2:if(~x) z = 1; else z = 0;
            S5:if(x) z = 1; else z = 0;
            S0,S1,S3,S4: z = 0;
        endcase
endmodule