`timescale 1ns / 1ps

module m3( Clk, rst, a, b, c );
    input Clk, rst;
    input signed [31:0] a;
    input [15:0] b;

    output signed [7:0] c;

    wire signed [7:0] cwire;

    ADD #(8) add_1({{24'b0},a[31:0]}, {{8'b0},b[15:0]}, cwire);
    REG #(8) reg_1(cwire, Clk, rst, c);
endmodule
