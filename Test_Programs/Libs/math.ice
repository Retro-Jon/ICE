Function add : int ($a : int, $b : int)
{
    $result : int;
    result = a;
    result += b;
    return(result);
}

Function sub : int ($a : int, $b : int)
{
    $result : int;
    result = a;
    result -= b;
    return(result);
}

Function mul : int ($a : int, $b : int)
{
    $result : int;
    result = a;
    result *= b;
    return(result);
}

Function div : int ($a : int, $b : int)
{
    $result : int;
    result = a;
    result /= b;
    return(result);
}

Function fact : int ($n : int)
{
    if (n == 0)
    {
        return(1);
    } else {
        $new_n : int;
        new_n = n;
        new_n -= 1;
        $res : int;
        res = n;
        res *= fact(new_n);
        return(res);
    }
}

Function print_result : void ($opperation : string, $value : int)
{
    output(0, "- ");
    output(0, opperation);
    output(0, ": ");
    output(0, value);
    output(0, "\n");
}
