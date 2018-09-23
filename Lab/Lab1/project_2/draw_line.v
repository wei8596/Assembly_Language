`timescale 1ns / 1ps

module draw_line(clk, rst_n, wea, x, y, m, b, dir, range, index_rd, line_x, line_y, finish);
input             clk, rst_n;
input             wea;
input      [31:0] x, y, m, b, dir, range;
input      [31:0] index_rd;
output     [31:0] line_x, line_y;
output     reg finish;
reg        [31:0] index_rd_reg;
reg        [31:0] line_x_mem [30:0] ;
reg        [31:0] line_y_mem [30:0] ;
wire       [31:0] new_x,new_y;
reg        [31:0] cnt,cnt_upd;

//counter
always@ (posedge clk, negedge rst_n) begin
    if(!rst_n) begin
        cnt       <= 32'd0;
        cnt_upd <= 32'd0;
    end
    else begin
        cnt       <= cnt_upd + 1'b1;
    end
end

always@ (posedge clk) begin
    if(!rst_n) begin
        finish    <= 1'b0;
    end
    else if(cnt < range) begin
        cnt_upd <= cnt;
    end
    else begin
        finish   <=1'b1;
    end
end

//new x,y
assign new_x = (dir==32'd0)? x - cnt_upd :
               (dir==32'd1)? x + cnt_upd : x;

assign new_y = m * new_x + b;

// load data
assign line_x = line_x_mem[index_rd_reg]; 
assign line_y = line_y_mem[index_rd_reg];
	
// store data		   
always@(posedge clk) 
begin
  if (wea) begin
    line_x_mem [cnt_upd] = new_x;
    line_y_mem [cnt_upd] = new_y;
  end
  else 
    index_rd_reg <= index_rd;
end
 
endmodule