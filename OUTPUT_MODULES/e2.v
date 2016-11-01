`timescale 1ns / 1ps

module e2( Clk, rst, a, b, c, z, x );
    input Clk, rst;
    input signed [7:0] a, b, c;

    output signed [7:0] z;
    output signed [15:0] x;

    wire signed [7:0] e, //, missing, wire;
    wire signed [15:0] f, g, xwire;

//DATA OUT FOR MODULE COULD NOT BE LOCATED
    ADD #(8) add_1(a, c, e);
//DATA IN FOR MODULE COULD NOT BE LOCATED
//DATA IN FOR MODULE COULD NOT BE LOCATED
    SMULT #(16) smult_1({{8{a[7]}},a[7:0]}, {{8{c[7]}},c[7:0]}, f);
//DATA IN FOR MODULE COULD NOT BE LOCATED
    SREG #(16) sreg_1(xwire, Clk, rst, x);
endmodule
