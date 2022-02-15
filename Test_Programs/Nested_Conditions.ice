ICE Fetch Libs/io.ice;

Function main : int (void)
{
    $msg : string;
    msg = get_console_input();

    if (msg == "go")
    {
        println("Where do you want to go?");
        msg = get_console_input();
        if (msg == "n")
        {
            println("You go north");
        } elif (msg == "s")
        {
            println("You go south");
        } elif (msg == "e")
        {
            println("You go east");
        } elif (msg == "w")
        {
            println("You go west");
        } else {
            println("That's not a direction.");
        }
    } elif (msg == "say")
    {
        println("What do you want to say?");
        msg = get_console_input();
    } else {
        println("Must've been the wind.");
    }
}