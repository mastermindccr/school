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
          ALUCtrl_o,
          Jr_o
          );
          
//I/O ports 
input      [6-1:0] funct_i;
input      [3-1:0] ALUOp_i;

output     [4-1:0] ALUCtrl_o;   
output reg         Jr_o; 
     
//Internal Signals
reg        [4-1:0] ALUCtrl_o;

//Parameter

       
//Select exact operation
always@(*) begin
    if(ALUOp_i == 3'b100) begin   // beq
        ALUCtrl_o = 4'd6;
        Jr_o = 0;
    end     
    else if(ALUOp_i == 3'b010) begin  // slti
        ALUCtrl_o = 4'd7; 
        Jr_o = 0;
    end
    else if(ALUOp_i == 3'b001) begin // addi, lw, sw, jump, jal => add
        ALUCtrl_o = 4'd2;
        Jr_o = 0;
    end
    else begin
	case(funct_i) // R-format
            8:  Jr_o = 1;
            32: begin
                ALUCtrl_o = 4'd2;
                Jr_o = 0;
            end
            34: begin
                ALUCtrl_o = 4'd6;
                Jr_o = 0;
            end
            36: begin
                ALUCtrl_o = 4'd0;
                Jr_o = 0;
            end
            37: begin
                ALUCtrl_o = 4'd1;
                Jr_o = 0;
            end
            42: begin
                ALUCtrl_o = 4'd7;
                Jr_o = 0;
            end
        endcase
    end	
end

endmodule     





                    
                    