Function main : int (void)
{
    $a : int;
    $b : int;
    
    @START;

    output(0, "Enter 2 integers to compare:\n");
    input(0, a);
    input(0, b);
    
    if (a == b || b == 3 && a > b)
    {
        output(0, "TRUE\n");
    } else {
        output(0, "FALSE\n");
    }

    $response : string;
    output(0, "Would you like to run the program again?\n[y/n] ");
    input(0, response);
    output(0, "\n");

    if (response == "y")
    {
        goto("START");
    }
    else
    {
        output(0, "Thanks for running.\n")
    }

    return(0);
}