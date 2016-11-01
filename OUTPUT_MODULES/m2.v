`timescale 1ns / 1ps

module m2( Clk, rst, a, b, c );
    input Clk, rst;
    input signed [7:0] a;
    input [15:0] b;

    output signed [31:0] c;

    wire signed [31:0] cwire;

    ADD #(32) add_1({{24'b0},a[7:0]}, {{16'b0}},b[15:0]}, cwire);
    SREG #(32) sreg_1(cwire, Clk, rst, c);
endmodule
