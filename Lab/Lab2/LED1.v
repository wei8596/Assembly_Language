`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2017/10/06 11:28:28
// Design Name: 
// Module Name: LED1
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module LED1(
    data,k,reset,q
 );
 
 input data,k,reset;
 output q;
 reg q;
 
 always @ ( posedge k)
 if (~reset) begin
    q <= 1'b0;
 end  else begin
    q <= data;
 end
  
 endmodule