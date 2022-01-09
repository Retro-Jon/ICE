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

Function print_result : void ($opperation : string, $value : int)
{
    output(0, "- ");
    output(0, opperation);
    output(0, ": ");
    output(0, value);
    output(0, "\n");
}