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
input      [3-1:0] ALUOp_i;

output     [4-1:0] ALUCtrl_o;    
     
//Internal Signals
reg        [4-1:0] ALUCtrl_o;

//Parameter

       
//Select exact operation
always@(*) begin
    if(ALUOp_i == 3'b100) ALUCtrl_o = 4'd6;
    else if(ALUOp_i == 3'b010) ALUCtrl_o = 4'd7;
    else if(ALUOp_i == 3'b001) ALUCtrl_o = 4'd2;
    else begin
	case(funct_i)
            32: ALUCtrl_o = 4'd2;
            34: ALUCtrl_o = 4'd6;
            36: ALUCtrl_o = 4'd0;
            37: ALUCtrl_o = 4'd1;
            42: ALUCtrl_o = 4'd7;
        endcase
    end	
end

endmodule     





                    
                    