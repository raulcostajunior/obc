MODULE NumLiterals;

        CONST InvalidRealNoIntPart = .2E+4;
        CONST InvalidRealNoScaleSign = 1.2E4;
        CONST ValidRealNoDecimalScale = 23.E+2; (* Real numbers can have no decimal digit before scale *)
        CONST ValidRealNoDecimalNoScale = 23.;
        CONST ValidHexInt = 87AH;
        CONST InvalidInt = 87A;
        CONST ValidSingleCharStr = 2AX;
        CONST ValidInt = 1783;
        CONST ValidRealNoScale = 1.234;
        CONST InvalidRealHexNoScale = 1A.234H;
        CONST ValidRealScale = 1.234E+2;
        CONST ValidRealScaleNeg = 1.234E-10;
        CONST ValidRealNoDecPart = 23.E+2;

END NumLiterals.