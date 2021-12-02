Function print : void ($f_out : string, $l_out : string)
{
    output(0, f_out);
    output(0, l_out);
}

Function main : int (null)
{
    $msg : string;
    msg = "HelloWorld";
    
    $msg2 : string;
    msg2 = msg;

    $msg3 : string;
    msg3 = "BYE"
    
    print(msg2, msg3);
    return(0);
}