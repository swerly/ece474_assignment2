`timescale 1ns / 1ps

module m1( Clk, rst, a, b, c );
    input Clk, rst;
    input signed [7:0] a;
    input [7:0] b;

    output signed [7:0] c;

    wire signed [7:0] cwire;

    ADD #(8) add_1(a, b, cwire);
    REG #(8) reg_1(cwire, Clk, rst, c);
endmodule
