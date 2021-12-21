Function main : int(void)
{
    $value1 : int;
    $value2 : int;

    output(0, "Enter 2 Integers to subtract\n");

    output(0, "Enter First Integer: ");
    input(0, value1);
    output(0, "Enter Second Interger: ");
    input(0, value2);

    $result : int;
    result = value1;
    result -= value2;

    output(0, result);
    return(0);
}