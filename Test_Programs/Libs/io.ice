Function print : void ($message : string)
{
    output(0, message);
}

Function println : void ($message : string)
{
    output(0, message);
    output(0, "\n");
}

Function get_console_input : string (void)
{
    $result : string;
    input(0, result);
    return(result);
}

Function save_string : void ($path : string, $data : string)
{
    output(1, data, path);
}

Function load_string : string ($path : string)
{
    $result : string;
    input(1, result, path);
    return(result);
}
