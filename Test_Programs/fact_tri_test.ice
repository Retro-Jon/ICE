ICE Fetch Libs/math.ice;

Function tri : int ($x : int)
{
    $res : int;
    $y : int;
    $acc : int;
    if ( x == 0 )
    {
    } else {
        y = x;
        y -= 1;
        acc += x;
        res = tri(y);
    }

    return(acc);
}

Function main : int (void)
{
    $lp_c : int;
    lp_c = 0;

    output(0, "\nFactorials\n\n");

    @FACT_START;
    if (lp_c <= 20)
    {
        $r : int;
        r = fact(lp_c);
        output(0, lp_c);
        output(0, " = ");
        output(0, r);
        output(0, "\n");
        lp_c += 1;
        goto("FACT_START");
    }

    lp_c = 0;

    output(0, "\ntri_sequence\n\n");

    @TRI_START;
    if (lp_c <= 20)
    {
        $r : int;
        r = tri(lp_c);
        output(0, lp_c);
        output(0, " = ");
        output(0, r);
        output(0, "\n");
        lp_c += 1;
        goto("TRI_START");
    }

    return(0);
}