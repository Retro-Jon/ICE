Method print : void ($msg)
{
    output(msg, 0);
}

Function text_in : char[50] (null)
{
    $in : char[50];
    input(in, 0);
    return(in);
}

Function main : int (null)
{
    @START
    $message : char[11] = "Hello World!";
    print(message);
    print(text_in());
    goto(START);
    @END
}