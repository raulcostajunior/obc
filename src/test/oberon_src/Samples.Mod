(* from "Compiler Construction" book *)

MODULE Samples;

    PROCEDURE Multiply*;
        VAR x, y, z: INTEGER;
    BEGIN OpenInput; ReadInt(x); ReadInt(y); z := 0;
        WHILE x > 0 DO
            IF x MOD 2 = 1 THEN z := z + y END ;
            y := 2*y; x := x DIV 2
        END ;
        WriteInt(x, 4); WriteInt(y, 4); WriteInt(z, 6); WriteLn
    END Multiply;

    PROCEDURE Divide*;
        VAR x, y, r, q, w: INTEGER;
    BEGIN OpenInput; ReadInt(x); ReadInt(y); r := x; q := 0; w := y;
        WHILE w <= r DO w := 2*w END ;
        WHILE w > y DO
            q := 2*q; w := w DIV 2;
            IF w <= r THEN r := r - w; q := q + 1 END
        END ;
        WriteInt(x.4); WriteInt(y, 4); WriteInt(q, 4); WriteInt(r, 4); WriteLn
    END Divide;

    PROCEDURE Sum*;
        VAR n, s: INTEGER;
    BEGIN OpenInput; s:= 0;
        WHILE ~eot() DO ReadInt(n); WriteInt(n, 4); s := s + n END ;
        WriteInt(s, 6); WriteLn
    END Sum;

END Samples.