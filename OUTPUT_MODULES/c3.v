`timescale 1ns / 1ps

module c3( Clk, rst, a, b, c, d, e, f, g, h, sa, avg );
    input Clk, rst;
    input signed [15:0] a, b, c, d, e, f, g, h;
    input signed [7:0] sa;

    output signed [15:0] avg;

    wire signed [31:0] l00, l01, l02, l03, l10, l11, l2, l2div2, l2div4, l2div8;

    ADD #(32) add_1({{16'b0},a[15:0]}, {{16'b0}},b[15:0]}, l00);
    ADD #(32) add_2({{16'b0},c[15:0]}, {{16'b0}},d[15:0]}, l01);
    ADD #(32) add_3({{16'b0},e[15:0]}, {{16'b0}},f[15:0]}, l02);
    ADD #(32) add_4({{16'b0},g[15:0]}, {{16'b0}},h[15:0]}, l03);
    ADD #(32) add_5(l00, l01, l10);
    ADD #(32) add_6(l02, l03, l11);
    ADD #(32) add_7(l10, l11, l2);
    SSHR #(32) sshr_1(l2, sa, l2div2);
    SSHR #(32) sshr_2(l2div2, sa, l2div4);
    SSHR #(32) sshr_3(l2div4, sa, l2div8);
    SREG #(16) sreg_1({{16{l2div8[31]}},l2div8[31:0]}, Clk, rst, avg);
endmodule
