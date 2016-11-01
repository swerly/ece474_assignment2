`timescale 1ns / 1ps

module c1( Clk, rst, a, b, c, z, x );
    input Clk, rst;
    input signed [7:0] a, b, c;

    output signed [7:0] z;
    output signed [15:0] x;

    wire signed [7:0] d, e;
    wire signed [15:0] f, g, xwire;

    ADD #(8) add_1(a, b, d);
    ADD #(8) add_2(a, c, e);
    SCOMP #(8) scomp_1(d, e, g, , );
    SMUX2x1 #(8) smux_1(d, e, g, z);
    SMULT #(16) smult_1({{8{a[7]}},a[7:0]}, {{8{c[7]}},c[7:0]}, f);
    SUB #(16) sub__1(f, {{8'b0},d[7:0]}, xwire);
    SREG #(16) sreg_1(xwire, Clk, rst, x);
endmodule
