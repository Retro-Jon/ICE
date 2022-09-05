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

Function fact : int ($n : int) # Can't go beyond !12 because it exceeds the 64 bit integer length #
{
    if (n != 0)
    {
        $new_n : int;
        new_n = n;
        new_n -= 1;
        $res : int;
        res = n;
        res *= fact(new_n);
        return(res);
    } else {
        return(1);
    }
}

Function tri : int ($x : int)
{
    $acc : int;
    
    if (x <= 1)
    {
        return(x);
    } else {
        $y : int;
        y = x;
        y -= 1;
        acc = tri(y);
        acc += x;
        return(acc);
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

Function fib : int ($n : int)
{
    $n_1 : int;
    $n_2 : int;

    @START;
    
}