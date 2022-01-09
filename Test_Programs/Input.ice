Function main : int(void)
{
    $value1 : int;
    $value2 : int;

    output(0, "Enter 2 Integers to subtract\n");

    output(0, "Enter First Integer: ");
    input(0, value1); # set value1 to console input #
    output(0, "Enter Second Interger: ");
    input(0, value2); # set value2 to console input #

    $result : int;
    result = value1; # set result to value1 #
    result -= value2; # subtract value2 from result #

    output(0, result); # print result to console #
    return(0);
}