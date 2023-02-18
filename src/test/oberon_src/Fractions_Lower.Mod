(* from https://github.com/lboasso/oberon0 *)

module Fractions;  (*NW  10.10.07;  Tabulate fractions 1/n*)
  const Base = 10; N = 32;

  procedure Go;
    var i, j, m, n, r: integer;
      d: array N of integer;  (*digits*)
      x: array N of integer;  (*index*)
  begin ReadInt(n); i := 2;
      while i <= n do j := 0;
        while j < i do x[j] := 0; j := j+1 end ;
        m := 0; r := 1;
        while x[r] = 0 do
          x[r] := m; r := Base*r; d[m] := r div i; r := r mod i; m := m+1
        end ;
        WriteInt(i); WriteChar(9); WriteChar("."); j := 0;
        while j < x[r] do WriteChar(d[j] + 48); j := j+1 end ;
        WriteChar("'");
        while j < m do WriteChar(d[j] + 48); j := j+1 end ;
        WriteLn; i := i+1
      end
  end Go;

begin Go
end Fractions.