Function main : int (void)
{
    $a : int;
    $b : int;
    
    output(0, "Enter 2 integers to compare:\n");
    input(0, a);
    input(0, b);
    
    if (a == b || b == 3 && a > b)
    {
        output(0, "TRUE\n");
    } else {
        output(0, "FALSE\n");
    }

    return(0);
}