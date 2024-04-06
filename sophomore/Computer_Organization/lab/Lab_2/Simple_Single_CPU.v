//Subject:     CO project 2 - Simple Single CPU
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
module Simple_Single_CPU(
        clk_i,
		rst_i
		);
		
//I/O port
input         clk_i;
input         rst_i;

//Internal Signles
wire [31:0]pc_out_o;
wire [31:0]sum_o_1;
wire [31:0]instr_o;
wire RegDst_o;
wire [2:0]ALU_op_o;
wire ALUSrc_o;
wire Branch_o;
wire RegWrite_o;;
wire [31:0]Sign_Extend_o;
wire [3:0]ALUCtrl_o;
wire [31:0]RSdata_o;
wire [31:0]RTdata_o;
wire [31:0]ALU_result;
wire ALU_zero;

wire [4:0]WriteReg_o;

wire [31:0]RTMux_o;
wire [31:0]shift_o;
wire [31:0]sum_o_2;
wire [31:0]AddMux_o;
wire AddMux_select;

assign AddMux_select = Branch_o&ALU_zero;

//Greate componentes
ProgramCounter PC(
        .clk_i(clk_i),      
	    .rst_i (rst_i),     
	    .pc_in_i(AddMux_o) ,   
	    .pc_out_o(pc_out_o) 
	    );
	
Adder Adder1(
        .src1_i(32'd4),     
	    .src2_i(pc_out_o),     
	    .sum_o(sum_o_1)    
	    );
	
Instr_Memory IM(
        .pc_addr_i(pc_out_o),  
	    .instr_o(instr_o)    
	    );

MUX_2to1 #(.size(5)) Mux_Write_Reg(
        .data0_i(instr_o[20:16]),
        .data1_i(instr_o[15:11]),
        .select_i(RegDst_o),
        .data_o(WriteReg_o)
        );	
		
Reg_File RF(
        .clk_i(clk_i),      
	.rst_i(rst_i) ,     
        .RSaddr_i(instr_o[25:21]) ,  
        .RTaddr_i(instr_o[20:16]) ,  
        .RDaddr_i(WriteReg_o) ,  
        .RDdata_i(ALU_result)  , 
        .RegWrite_i (RegWrite_o),
        .RSdata_o(RSdata_o) ,  
        .RTdata_o(RTdata_o)   
        );
	
Decoder Decoder(
        .instr_op_i(instr_o[31:26]), 
	    .RegWrite_o(RegWrite_o), 
	    .ALU_op_o(ALU_op_o),   
	    .ALUSrc_o(ALUSrc_o),   
	    .RegDst_o(RegDst_o),   
		.Branch_o(Branch_o)   
	    );

ALU_Ctrl AC(
        .funct_i(instr_o[5:0]),   
        .ALUOp_i(ALU_op_o),   
        .ALUCtrl_o(ALUCtrl_o) 
        );
	
Sign_Extend SE(
        .data_i(instr_o[15:0]),
        .data_o(Sign_Extend_o)
        );

MUX_2to1 #(.size(32)) Mux_ALUSrc(
        .data0_i(RTdata_o),
        .data1_i(Sign_Extend_o),
        .select_i(ALUSrc_o),
        .data_o(RTMux_o)
        );	
		
ALU ALU(
        .src1_i(RSdata_o),
	    .src2_i(RTMux_o),
	    .ctrl_i(ALUCtrl_o),
	    .result_o(ALU_result),
		.zero_o(ALU_zero)
	    );
		
Adder Adder2(
        .src1_i(sum_o_1),     
	    .src2_i(shift_o),     
	    .sum_o(sum_o_2)      
	    );
		
Shift_Left_Two_32 Shifter(
        .data_i(Sign_Extend_o),
        .data_o(shift_o)
        ); 		
		
MUX_2to1 #(.size(32)) Mux_PC_Source(
        .data0_i(sum_o_1),
        .data1_i(sum_o_2),
        .select_i(AddMux_select),
        .data_o(AddMux_o)
        );	

endmodule
		  


