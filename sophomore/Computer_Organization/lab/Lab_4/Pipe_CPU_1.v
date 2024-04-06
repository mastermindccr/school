//109550060
`timescale 1ns / 1ps
//Subject:     CO project 4 - Pipe CPU 1
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------
module Pipe_CPU_1(
    clk_i,
    rst_i
    );
    
/****************************************
I/O ports
****************************************/
input clk_i;
input rst_i;

/****************************************
Internal signal
****************************************/
/**** IF stage ****/

wire [32-1:0] pc_src_mux_o;
wire [32-1:0] pc_o;

wire [32-1:0] adder1_o;
wire [32-1:0] instr_o;

wire [64-1:0] IF_i;

/**** ID stage ****/

wire [32-1:0] ID_adder1_o;
wire [32-1:0] ID_instr_o;

wire [64-1:0] ID_o;
wire [147-1:0]ID_i;

//control signal

wire [32-1:0] rs_data_o;
wire [32-1:0] rt_data_o;
wire [32-1:0] sign_extend_o;

wire          RegWrite_o; // WB
wire          MemtoReg_o; // WB
wire          Branch_o; // MEM
wire          MemRead_o; // MEM
wire          MemWrite_o; // MEM
wire [2-1:0]  ALU_op_o; // EX
wire          ALUSrc_o; // EX
wire          RegDst_o; // EX

/**** EX stage ****/

wire [147-1:0]EX_o;
wire [107-1:0]EX_i;

//control signal

wire [32-1:0] adder2_o;
wire [32-1:0] ALU_mux_o;
wire [32-1:0] shift_left_two_o;
wire          ALU_zero_o;
wire [32-1:0] ALU_result_o;
wire [4-1:0]  ALU_ctrl_o;
wire [5-1:0]  RegDst_mux_o;

/**** MEM stage ****/

wire [107-1:0]MEM_o;
wire [71-1:0] MEM_i;

//control signal

wire          branch_and_o;
wire [32-1:0] read_data_o;

/**** WB stage ****/

wire [71-1:0] WB_o;

//control signal

wire [32-1:0] MemtoReg_mux_o;

// pipe reg assignment

assign IF_i = {adder1_o, instr_o};
assign ID_i = {RegWrite_o, MemtoReg_o, Branch_o,
                MemRead_o, MemWrite_o, RegDst_o,
                ALU_op_o, ALUSrc_o, ID_o[63:32],
                rs_data_o, rt_data_o, sign_extend_o,
                ID_o[20:16], ID_o[15:11]};
assign EX_i = {EX_o[146:142], adder2_o, ALU_zero_o,
                ALU_result_o, EX_o[73:42], RegDst_mux_o};
assign MEM_i = {MEM_o[106:105], read_data_o, MEM_o[68:37], MEM_o[4:0]};

assign branch_and_o = MEM_o[104]&MEM_o[69];

/****************************************
Instantiate modules
****************************************/
//Instantiate the components in IF stage
MUX_2to1 #(.size(32)) Pcsrc_Mux(
    adder1_o,
    MEM_o[101:70],
    branch_and_o,
    pc_src_mux_o
);

ProgramCounter PC(
    clk_i,
    rst_i,
    pc_src_mux_o,
    pc_o
);

Instruction_Memory IM(
    pc_o,
    instr_o
);
			
Adder Add_pc(
    pc_o,
    32'd4,
    adder1_o
);

		
Pipe_Reg #(.size(64)) IF_ID(       //N is the total length of input/output
    clk_i,
    rst_i,
    IF_i,
    ID_o
);


//Instantiate the components in ID stage
Reg_File RF(
    clk_i,
    rst_i,
    ID_o[25:21],
    ID_o[20:16],
    WB_o[4:0],
    MemtoReg_mux_o,
    WB_o[70],
    rs_data_o,
    rt_data_o
);

Decoder Control(
    ID_o[31:0],
	RegWrite_o,
	ALU_op_o,
	ALUSrc_o,
	RegDst_o,
	Branch_o,
	MemRead_o,
	MemWrite_o,
	MemtoReg_o
);

Sign_Extend Sign_Extend(
    ID_o[15:0],
    sign_extend_o
);	

Pipe_Reg #(.size(147)) ID_EX(
    clk_i,
    rst_i,
    ID_i,
    EX_o
);


//Instantiate the components in EX stage	   
Shift_Left_Two_32 Shifter(
    EX_o[41:10],
    shift_left_two_o
);

ALU ALU(
    EX_o[105:74],
	ALU_mux_o,
	ALU_ctrl_o,
	ALU_result_o,
	ALU_zero_o
);
		
ALU_Ctrl ALU_Control(
    EX_o[15:10],
    EX_o[140:139],
    ALU_ctrl_o,
);

MUX_2to1 #(.size(32)) Mux1(
    EX_o[73:42],
    EX_o[41:10],
    EX_o[138],
    ALU_mux_o
);
		
MUX_2to1 #(.size(5)) Mux2(
    EX_o[9:5],
    EX_o[4:0],
    EX_o[141],
    RegDst_mux_o
);

Adder Add_pc_branch(
   EX_o[137:106],
   shift_left_two_o,
   adder2_o
);

Pipe_Reg #(.size(107)) EX_MEM(
    clk_i,
    rst_i,
    EX_i,
    MEM_o
);


//Instantiate the components in MEM stage
Data_Memory DM(
    clk_i,
    MEM_o[68:37],
    MEM_o[36:5],
    MEM_o[103],
    MEM_o[102],
    read_data_o
);

Pipe_Reg #(.size(71)) MEM_WB(
    clk_i,
    rst_i,
    MEM_i,
    WB_o
);


//Instantiate the components in WB stage
MUX_2to1 #(.size(32)) Mux3(
    WB_o[68:37],
    WB_o[36:5],
    WB_o[69],
    MemtoReg_mux_o
);

/****************************************
signal assignment
****************************************/

endmodule

