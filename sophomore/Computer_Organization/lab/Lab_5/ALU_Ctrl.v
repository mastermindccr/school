//109550060
//Subject:     CO project 2 - ALU Controller
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------
`timescale 1ns/1ps
module ALU_Ctrl(
          funct_i,
          ALUOp_i,
          ALUCtrl_o
          );
          
//I/O ports 
input      [6-1:0] funct_i;
input      [2-1:0] ALUOp_i;

output     [4-1:0] ALUCtrl_o;
     
//Internal Signals
reg        [4-1:0] ALUCtrl_o;

//Parameter

       
//Select exact operation
always@(*) begin
    if(ALUOp_i == 2'b0) begin   // R-type
        case(funct_i) 
            32: ALUCtrl_o = 4'd2;
            34: ALUCtrl_o = 4'd6;
            36: ALUCtrl_o = 4'd0;
            37: ALUCtrl_o = 4'd1;
            42: ALUCtrl_o = 4'd7;
            24: ALUCtrl_o = 4'd3;
        endcase
    end     
    else if(ALUOp_i == 2'b10) ALUCtrl_o = 4'd7; // SltI
    else if(ALUOp_i == 2'b01) ALUCtrl_o = 4'd2; // Add-regarding
    else if(ALUOp_i == 2'b11) ALUCtrl_o = 4'd6; // Branches
end

endmodule     





                    
                    