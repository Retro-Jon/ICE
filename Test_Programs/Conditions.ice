Function main : int (void)
{
    if (false)
    {
        output(0, "True\n");
    }
    else
    {
        if (true)
        {
            output(0, "False\n");
        }
    }
    if (false)
    {
        output(0, "SECOND\n");
    }
    output(0, "BREAK\n");
    return(0);
}