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
    $in_a : int;
    $in_b : int;

    output(0, "Enter 2 Integers to add\nEnter the first integer: ");
    input(0, in_a);
    output(0, "Enter the second integer: ");
    input(0, in_b);

    r = add(in_a, in_b);

    output(0, "\n");
    output(0, r);
    output(0, "\n");
    return(0);
}