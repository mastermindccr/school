`timescale 1ns / 1ps

module Full_Subtractor(
    In_A, In_B, Borrow_in, Difference, Borrow_out
    );
    input In_A, In_B, Borrow_in;
    output Difference, Borrow_out;
    wire Second_A, Second_B, Second_C;
    
    // implement full subtractor circuit, your code starts from here.
    // use half subtractor in this module, fulfill I/O ports connection.
    Half_Subtractor HSUB1 (
        .In_A(In_A), 
        .In_B(In_B), 
        .Difference(Second_A), 
        .Borrow_out(Second_B)
    );
    Half_Subtractor HSUB2 (
        .In_A(Second_A),
        .In_B(Borrow_in),
        .Difference(Difference),
        .Borrow_out(Second_C)
    );
    or(Borrow_out,Second_B,Second_C);

endmodule
