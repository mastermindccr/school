//109550060
`timescale 1ns/1ps
module Forwarding(
    ID_EX_rs_i,
    ID_EX_rt_i,
    EX_MEM_rd_i,
    EX_MEM_RegWrite_i,
    MEM_WB_rd_i,
    MEM_WB_RegWrite_i,
    src1_i,
    src2_i
	);

//I/O ports
input  [5-1:0]   ID_EX_rs_i;
input  [5-1:0]	 ID_EX_rt_i;
input  [5-1:0]	 EX_MEM_rd_i;
input            EX_MEM_RegWrite_i;
input  [5-1:0]	 MEM_WB_rd_i;
input            MEM_WB_RegWrite_i;
output reg [2-1:0]	 src1_i;
output reg [2-1:0]	 src2_i;

always@(*) begin
    if(EX_MEM_RegWrite_i && EX_MEM_rd_i!=0 && EX_MEM_rd_i==ID_EX_rs_i) begin // EX_MEM forward A
        src1_i = 2'b1;
    end
    else if(MEM_WB_RegWrite_i && MEM_WB_rd_i!=0 && MEM_WB_rd_i==ID_EX_rs_i) begin // MEM_WB forward A
        src1_i = 2'b10;
    end
    else begin // no forwarding in A
        src1_i = 2'b0;
    end

    if(EX_MEM_RegWrite_i && EX_MEM_rd_i!=0 && EX_MEM_rd_i==ID_EX_rt_i) begin // EX_MEM forward B
        src2_i = 2'b1;
    end
    else if(MEM_WB_RegWrite_i && MEM_WB_rd_i!=0 && MEM_WB_rd_i==ID_EX_rt_i) begin // MEM_WB forward B
        src2_i = 2'b10;
    end
    else begin // no forwarding in A
        src2_i = 2'b0;
    end
end

endmodule