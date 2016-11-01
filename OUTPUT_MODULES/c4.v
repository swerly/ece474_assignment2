`timescale 1ns / 1ps

module c4( Clk, rst, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, final );
    input Clk, rst;
    input signed [7:0] a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;

    output signed [31:0] final;

    wire signed [31:0] t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;

    ADD #(32) add_1({{24'b0},a[7:0]}, {{24'b0}},b[7:0]}, t1);
    ADD #(32) add_2(t1, {{24'b0},c[7:0]}, t2);
    ADD #(32) add_3(t2, {{24'b0},d[7:0]}, t3);
    ADD #(32) add_4(t3, {{24'b0},e[7:0]}, t4);
    ADD #(32) add_5(t4, {{24'b0},f[7:0]}, t5);
    ADD #(32) add_6(t5, {{24'b0},g[7:0]}, t6);
    ADD #(32) add_7(t6, {{24'b0},h[7:0]}, t7);
    ADD #(32) add_8(t7, {{24'b0},i[7:0]}, t8);
    ADD #(32) add_9(t8, {{24'b0},j[7:0]}, t9);
    ADD #(32) add_10(t9, {{24'b0},l[7:0]}, t10);
    ADD #(32) add_11(t10, {{24'b0},m[7:0]}, t11);
    ADD #(32) add_12(t11, {{24'b0},n[7:0]}, t12);
    ADD #(32) add_13(t12, {{24'b0},o[7:0]}, t13);
    ADD #(32) add_14(t13, {{24'b0},p[7:0]}, t14);
    SREG #(32) sreg_1(t14, Clk, rst, final);
endmodule
