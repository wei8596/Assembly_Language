`timescale 1ns / 1ps

module draw_line_tb();
reg             clk, rst_n, wea;
reg      [31:0] x, y, dir, len;
reg      [31:0] index_rd;
wire     [31:0] line_x,line_y;
wire     finish;
integer i;

draw_line draw_line_0(.clk(clk),
          .rst_n(rst_n),
          .wea(wea),
          .x(x),
          .y(y),
          .dir(dir),
          .len(len),
          .index_rd(index_rd),
          .line_x(line_x),
          .line_y(line_y),
          .finish(finish)
          );

initial begin
    clk = 1'b0;
    forever #5 clk = ~clk ;
end

reg [1:0] status;


initial begin
  i=0;
  rst_n = 1'b0;
  wea = 1'b0;
  index_rd = 32'dx;
  x = 32'd10;
  y = 32'd10;
  dir = 32'd0;
  len = 32'd5;

  #25
  rst_n = 1'b1;
  wea = 1'b1;

  #80
  for(i=0;i<len;i=i+1)
  begin
    #10 
    wea = 1'b0;
    index_rd <= i ;
  end

//----------------------

  #40
  rst_n = 1'b0;
  wea = 1'b0;
  index_rd = 32'd0;
  x = 32'd10;
  y = 32'd10;
  dir = 32'd1;
  len = 32'd5;

  #10
  rst_n = 1'b1;
  wea = 1'b1;

  #80
  for(i=0;i<len;i=i+1)
  begin
    #10 
    wea = 1'b0;
    index_rd <= i ;
  end

//----------------------

  #40
  rst_n = 1'b0;
  wea = 1'b0;
  index_rd = 32'd0;
  x = 32'd10;
  y = 32'd10;
  dir = 32'd2;
  len = 32'd5;

  #10
  rst_n = 1'b1;
  wea = 1'b1;

  #80;

    for(i=0;i<len;i=i+1)
    begin
        #10 
        wea = 1'b0;
        index_rd <= i ;
    end
   

//----------------------

#40
rst_n = 1'b0;
wea = 1'b0;
index_rd = 32'd0;
x = 32'd10;
y = 32'd10;
dir = 32'd3;
len = 32'd5;

#10
rst_n = 1'b1;
wea = 1'b1;

#80
for(i=0;i<len;i=i+1)
begin
#10 
  wea = 1'b0;
  index_rd <= i ;
end

//----------------------

#300 $finish;
end


endmodule