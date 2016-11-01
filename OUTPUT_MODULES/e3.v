`timescale 1ns / 1ps

module e3( Clk, rst, a, b, //missing, input, z, x );
    input Clk, rst;
    input signed [7:0] a, b, //missing, input;

    output signed [7:0] z;
    output signed [15:0] x;

    wire signed [7:0] d, e;
    wire signed [15:0] f, g, xwire;

    ADD #(8) add_1(a, b, d);
//DATA IN FOR MODULE COULD NOT BE LOCATED
    SCOMP #(8) scomp_1(d, e, g, , );
    SMUX2x1 #(8) smux_1(d, e, g, z);
//DATA IN FOR MODULE COULD NOT BE LOCATED
    SUB #(16) sub__1(f, {{8'b0},d[7:0]}, xwire);
    SREG #(16) sreg_1(xwire, Clk, rst, x);
endmodule
