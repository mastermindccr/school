//109550060
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
module Simple_Single_CPU(
        clk_i,
        rst_i
        );
		
//I/O port
input         clk_i;
input         rst_i;

//Internal Signles
wire [32-1:0] pc_i;
wire [32-1:0] pc_o;
wire [32-1:0] adder1_o;
wire [32-1:0] instr_o;
wire [2-1:0] RegDst_o;
wire [5-1:0] mux_write_reg_o;
wire reg_write_o;
wire [32-1:0] rsdata_o;
wire [32-1:0] rtdata_o;
wire [3-1:0] ALU_op_o;
wire ALUSrc_o;
wire Branch_o;
wire [2-1:0] Jump_o;
wire MemRead_o;
wire MemWrite_o;
wire [2-1:0] MemtoReg_o;
wire [4-1:0] ALUCtrl_o;
wire [32-1:0] sign_extend_o;
wire [32-1:0] mux_ALUsrc_o;
wire [32-1:0] ALUresult_o;
wire ALUzero_o;
wire [32-1:0] data_memory_o;
wire [32-1:0] mux_data_memory_o;
wire [28-1:0] shifter1_o;
wire [32-1:0] jump_address;
wire [32-1:0] shifter2_o;
wire [32-1:0] adder2_o;
wire branch_select;
wire [32-1:0] Mux_branch_o;
wire [32-1:0] adder3_o;

assign jump_address = {adder1_o[31:28], shifter1_o};
assign branch_select = Branch_o & ALUzero_o;


//Greate componentes
ProgramCounter PC(
        .clk_i(clk_i),      
        .rst_i (rst_i),     
        .pc_in_i(pc_i) ,   
        .pc_out_o(pc_o) 
        );
	
Adder Adder1(
        .src1_i(pc_o),     
        .src2_i(32'd4),     
        .sum_o(adder1_o)    
        );
	
Instr_Memory IM(
        .pc_addr_i(pc_o),  
        .instr_o(instr_o)    
        );

MUX_3to1 #(.size(5)) Mux_Write_Reg(
        .data0_i(instr_o[20:16]),
        .data1_i(instr_o[15:11]),
        .data2_i(5'b11111),
        .select_i(RegDst_o),
        .data_o(mux_write_reg_o)
        );	
		
Reg_File Registers(
        .clk_i(clk_i),      
	.rst_i(rst_i) ,     
        .RSaddr_i(instr_o[25:21]),  
        .RTaddr_i(instr_o[20:16]),  
        .RDaddr_i(mux_write_reg_o),  
        .RDdata_i(mux_data_memory_o), 
        .RegWrite_i (reg_write_o),
        .RSdata_o(rsdata_o),  
        .RTdata_o(rtdata_o)   
        );
	
Decoder Decoder(
        .instr_op_i(instr_o),
	.RegWrite_o(reg_write_o),
	.ALU_op_o(ALU_op_o),
	.ALUSrc_o(ALUSrc_o),
	.RegDst_o(RegDst_o),
	.Branch_o(Branch_o),
	.Jump_o(Jump_o),
	.MemRead_o(MemRead_o),
	.MemWrite_o(MemWrite_o),
	.MemtoReg_o(MemtoReg_o)
	);

ALU_Ctrl ALU_Ctrl(
        .funct_i(instr_o[5:0]),   
        .ALUOp_i(ALU_op_o),   
        .ALUCtrl_o(ALUCtrl_o) 
        );
	
Sign_Extend Sign_Extend(
        .data_i(instr_o[15:0]),
        .data_o(sign_extend_o)
        );

MUX_2to1 #(.size(32)) Mux_ALUSrc(
        .data0_i(rtdata_o),
        .data1_i(sign_extend_o),
        .select_i(ALUSrc_o),
        .data_o(mux_ALUsrc_o)
        );	
		
ALU ALU(
        .src1_i(rsdata_o),
        .src2_i(mux_ALUsrc_o),
        .ctrl_i(ALUCtrl_o),
        .result_o(ALUresult_o),
        .zero_o(ALUzero_o)
        );
	
Data_Memory Data_Memory(
	.clk_i(clk_i),
	.addr_i(ALUresult_o),
	.data_i(rtdata_o),
	.MemRead_i(MemRead_o),
	.MemWrite_i(MemWrite_o),
	.data_o(data_memory_o)
	);

MUX_3to1 #(.size(32)) Mux_data_memory(
        .data0_i(ALUresult_o),
        .data1_i(data_memory_o),
        .data2_i(adder1_o),
        .select_i(MemtoReg_o),
        .data_o(mux_data_memory_o)
        );	        

Shift_Left_Two_26 Shifter1(
        .data_i(instr_o[25:0]),
        .data_o(shifter1_o)
        ); 

Shift_Left_Two_32 Shifter2(
        .data_i(sign_extend_o),
        .data_o(shifter2_o)
        );

Adder Adder2(
        .src1_i(adder1_o),     
        .src2_i(shifter2_o),     
        .sum_o(adder2_o)      
        );
				
MUX_2to1 #(.size(32)) Mux_branch(
        .data0_i(adder1_o),
        .data1_i(adder2_o),
        .select_i(branch_select),
        .data_o(Mux_branch_o)
        );

MUX_3to1 #(.size(32)) Mux_jump(
        .data0_i(jump_address),
        .data1_i(Mux_branch_o),
        .data2_i(rsdata_o),
        .select_i(Jump_o),
        .data_o(pc_i)
        );

endmodule
		  


