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
wire [64-1:0] ID_o;
wire [119-1:0]ID_i;

//control signal

wire [32-1:0] rs_data_o;
wire [32-1:0] rt_data_o;
wire [32-1:0] sign_extend_o;
wire [32-1:0] shift_left_two_o;
wire [32-1:0] adder2_o;
wire          branch_decision_o;
wire [11-1:0] control_mux_o;

wire          RegWrite_o; // WB
wire          MemtoReg_o; // WB
wire          Branch_o;
wire [2-1:0]  BranchType_o;
wire          MemRead_o; // MEM
wire          MemWrite_o; // MEM
wire [2-1:0]  ALU_op_o; // EX
wire          ALUSrc_o; // EX
wire          RegDst_o; // EX

/**** EX stage ****/

wire [119-1:0]EX_o;
wire [73-1:0] EX_i;

//control signal

wire [32-1:0] src1_mux_o;
wire [32-1:0] src2_mux_o;
wire [32-1:0] ALU_src_mux_o;
wire [32-1:0] ALU_result_o;
wire [4-1:0]  ALU_ctrl_o;
wire [5-1:0]  RegDst_mux_o;

/**** MEM stage ****/

wire [73-1:0] MEM_o;
wire [71-1:0] MEM_i;

//control signal

wire [32-1:0] read_data_o;

/**** WB stage ****/

wire [71-1:0] WB_o;

//control signal

wire [32-1:0] MemtoReg_mux_o;

/**** Hazard Detection Unit ****/

wire         pc_write_o;
wire [2-1:0] IF_ID_Write_o;
wire         ID_flush_o;

/**** Forwarding Unit ****/

wire [2-1:0] forward_A_o;
wire [2-1:0] forward_B_o;

// pipe reg assignment

assign IF_i = {adder1_o, instr_o};
assign ID_i = (~load_branch_hazard) ? 
		{control_mux_o[10:9], control_mux_o[5:0],
                rs_data_o, rt_data_o, sign_extend_o,
                ID_o[25:21], ID_o[20:16], ID_o[15:11]} : 
		{8'b0,
                rs_data_o, rt_data_o, sign_extend_o,
                ID_o[25:21], ID_o[20:16], ID_o[15:11]};
assign EX_i = {EX_o[118:115], ALU_result_o, src2_mux_o, RegDst_mux_o};
assign MEM_i = {MEM_o[72:71], read_data_o, MEM_o[68:37], MEM_o[4:0]};

/**** Other Signals ****/

wire load_branch_hazard;
wire stall;
wire [1:0] IF_ID_control;
wire pc_write_control;

assign load_branch_hazard = (control_mux_o[8] && (MEM_o[70] | MEM_o[72]) && (ID_o[25:21]==MEM_o[4:0] || ID_o[20:16]==MEM_o[4:0]))
			   |(control_mux_o[8] && EX_o[118] && (ID_o[25:21]==EX_o[9:5] || ID_o[20:16]==MEM_o[9:5]));
assign stall = load_branch_hazard | branch_decision_o;
assign IF_ID_control = (IF_ID_Write_o==2'b0) ? 2'b0 :
		       (load_branch_hazard)  ? 2'b0 :
		       (branch_decision_o)   ? 2'b1 : 2'b10;
assign pc_write_control = (pc_write_o==0) ? 0 :
			  (load_branch_hazard) ? 0 :
			  (branch_decision_o) ? 1 : 1;

/****************************************
Instantiate modules
****************************************/
//Instantiate the components in IF stage
MUX_2to1 #(.size(32)) Pcsrc_Mux(
    adder1_o,
    adder2_o,
    branch_decision_o,
    pc_src_mux_o
);

ProgramCounter PC(
    clk_i,
    rst_i,
    pc_write_control,
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

		
Pipe_Reg_IF_ID #(.size(64)) IF_ID(       //N is the total length of input/output
    clk_i,
    rst_i,
    IF_ID_control,
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
    BranchType_o,
	MemRead_o,
	MemWrite_o,
	MemtoReg_o
);

MUX_2to1 #(.size(11)) Decoder_mux(
    {RegWrite_o, MemtoReg_o, Branch_o, BranchType_o, MemRead_o, MemWrite_o, RegDst_o, ALU_op_o, ALUSrc_o},
    11'b0,
    ID_flush_o,
    control_mux_o
);

Branch_Decision Branch_Decision(
    rs_data_o,
	rt_data_o,
    control_mux_o[7:6],
    control_mux_o[8],
	branch_decision_o
);

Sign_Extend Sign_Extend(
    ID_o[15:0],
    sign_extend_o
);	

Shift_Left_Two_32 Shifter(
    sign_extend_o,
    shift_left_two_o
);

Adder Adder2(
   ID_o[63:32],
   shift_left_two_o,
   adder2_o
);

Pipe_Reg #(.size(119)) ID_EX(
    clk_i,
    rst_i,
    ID_i,
    EX_o
);


//Instantiate the components in EX stage	   

MUX_3to1 #(.size(32)) Src1_mux(
    EX_o[110:79],
    MEM_o[68:37],
    MemtoReg_mux_o,
    forward_A_o,
    src1_mux_o
);

MUX_3to1 #(.size(32)) Src2_mux(
    EX_o[78:47],
    MEM_o[68:37],
    MemtoReg_mux_o,
    forward_B_o,
    src2_mux_o
);

ALU ALU(
    src1_mux_o,
	ALU_src_mux_o,
	ALU_ctrl_o,
	ALU_result_o
);
		
ALU_Ctrl ALU_Control(
    EX_o[20:15],
    EX_o[113:112],
    ALU_ctrl_o
);

MUX_2to1 #(.size(32)) ALU_mux(
    src2_mux_o,
    EX_o[46:15],
    EX_o[111],
    ALU_src_mux_o
);
		
MUX_2to1 #(.size(5)) RegDst_mux(
    EX_o[9:5],
    EX_o[4:0],
    EX_o[114],
    RegDst_mux_o
);

Pipe_Reg #(.size(73)) EX_MEM(
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
    MEM_o[70],
    MEM_o[69],
    read_data_o
);

Pipe_Reg #(.size(71)) MEM_WB(
    clk_i,
    rst_i,
    MEM_i,
    WB_o
);


//Instantiate the components in WB stage
MUX_2to1 #(.size(32)) MemtoReg_mux(
    WB_o[68:37],
    WB_o[36:5],
    WB_o[69],
    MemtoReg_mux_o
);

// Hazard Detection Unit
Hazard Hazard(
    ID_o[25:21],
    ID_o[20:16],
    EX_o[9:5],
    EX_o[116],
    pc_write_o,
    IF_ID_Write_o,
    ID_flush_o
);

Forwarding Forwarding(
    EX_o[14:10],
    EX_o[9:5],
    MEM_o[4:0],
    MEM_o[72],
    WB_o[4:0],
    WB_o[70],
    forward_A_o,
    forward_B_o
);


endmodule

