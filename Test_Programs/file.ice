ICE Fetch Libs/io.ice;

Function main : int (void)
{
    $path : string;
    $contents : string;

    println("Where would you like to save your file?");
    path = get_console_input();
    println("Enter your message to save.");
    contents = get_console_input();

    save_string(path, contents);
    return(0);
}