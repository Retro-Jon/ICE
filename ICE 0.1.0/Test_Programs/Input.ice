Function main : int(void)
{
    output(0, "HelloWorld!\n");
    $in : string;
    input(0, in);
    $result : string;
    result = "> ";
    result += in;
    output(0, result);
    return(0);
}