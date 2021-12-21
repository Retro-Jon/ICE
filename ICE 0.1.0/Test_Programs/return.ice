Function add : int ($a : int, $b : int)
{
    $result : int;
    result = a;
    result += b;
    return(result);
}

Function main : int (void)
{
    $r : int;
    $a : int;
    $b : int;

    output(0, "Enter 2 Integers to add\nEnter the first integer: ");
    input(0, a);
    output(0, "Enter the second integer: ");
    input(0, b);

    r = add(a, b);

    output(0, "\n");
    output(0, r);
    output(0, "\n");
    return(1);
}