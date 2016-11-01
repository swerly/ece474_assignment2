`timescale 1ns / 1ps

module c5( Clk, rst, a, b, c, z, x );
    input Clk, rst;
    input signed [63:0] a, b, c;

    output signed [31:0] z, x;

    wire signed [63:0] d, e, f, g, h;
    wire signed dLTe, dEQe;
    wire signed [63:0] xrin, zrin;

    wire signed [63:0] greg, hreg;

    ADD #(64) add_1(a, b, d);
    ADD #(64) add_2(a, c, e);
    SUB #(64) sub_1(a, b, f);
    SCOMP #(64) scomp_1(d, e, , ,dEQe);
    SCOMP #(64) scomp_2(d, e, ,dLTe, );
    MUX2x1 #(64) mux_1(d, e, dLTe, g);
    MUX2x1 #(64) mux_2(g, f, dEQe, h);
    REG #(64) reg_1(g, Clk, rst, greg);
    REG #(64) reg_2(h, Clk, rst, hreg);
    SHL #(64) shl_1(hreg, dLTe, xrin);
    SSHR #(64) sshr_1(greg, dEQe, zrin);
    REG #(32) reg_3({{32{xrin[63]}},xrin[63:0]}, Clk, rst, x);
    REG #(32) reg_4({{32{zrin[63]}},zrin[63:0]}, Clk, rst, z);
endmodule
