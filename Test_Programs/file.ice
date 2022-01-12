Function save_string : void ($path : string, $data : string)
{
    output(1, data, path);
}

Function load_string : string ($path : string)
{
    $result : string;
    input(1, result, path);
    return result;
}

Function main : int (void)
{
    #save_string("test.txt", "IT WORKS!!!");#
    $contents : string;
    contents = load_string("test.txt");
    output(0, contents);
    return(0);
}