Function get_value : float ($a : int, $b : int)
{
    $result : int;
    result = a;
    result -= b;
    return(result);
}

Function main : int (void)
{
    $r : int;
    $a : int;
    a = 3;

    r = get_value(a, 5);
    output(0, r);
    output(0, "\n");
    return(1);
}