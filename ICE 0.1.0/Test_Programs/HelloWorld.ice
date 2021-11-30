Function print : void ($f_out : string, $l_out : string)
{
    output(0, f_out);
    output(0, l_out);
}

Function main : int (null)
{
    print("HelloWorld!", "Bye");
    return(0);
}