`timescale 1ns / 1ps

module u1( Clk, rst, a, b, c );
    input Clk, rst;
    input [7:0] a, b;

    output [7:0] c;

    wire [7:0] cwire;

    ADD #(8) add_1(a, b, cwire);
    REG #(8) reg_1(cwire, Clk, rst, c);
endmodule
