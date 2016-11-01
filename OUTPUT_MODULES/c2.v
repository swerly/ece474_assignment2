`timescale 1ns / 1ps

module c2( Clk, rst, a, b, c, z, x );
    input Clk, rst;
    input signed [31:0] a, b, c;

    output signed [31:0] z, x;

    wire signed [31:0] d, e, f, g, h;
    wire signed dLTe, dEQe;
    wire signed [31:0] zwire, xwire;

    ADD #(32) add_1(a, b, d);
    ADD #(32) add_2(a, c, e);
    SUB #(32) sub_1(a, b, f);
    SCOMP #(32) scomp_1(d, e, , ,dEQe);
    SCOMP #(32) scomp_2(d, e, ,dLTe, );
    MUX2x1 #(32) mux_1(d, e, dLTe, g);
    MUX2x1 #(32) mux_2(g, f, dEQe, h);
    SHL #(32) shl_1(g, dLTe, xwire);
    SSHR #(32) sshr_1(h, dEQe, zwire);
    REG #(32) reg_1(xwire, Clk, rst, x);
    REG #(32) reg_2(zwire, Clk, rst, z);
endmodule
