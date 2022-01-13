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
    $in_a : int;
    $result : int;

    output(0, "Enter n integer to get the nth triangular number.\n");
    input(0, in_a);

    result = tri(in_a);
    print_result("answer", result);

    return(0);
}
