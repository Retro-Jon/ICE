ICE Fetch Libs/io.ice;

# Declare a function called "main" with a return type of int, that takes 0 parameters. #

Function main : int (void)
{
    $msg : string; # declare a variable called "msg" with type "string" #
    msg = "HelloWorld!"; # set msg to the value "HelloWorld!" #
    println(msg); # call the print function, passing msg an argument #
    return(0); # return 0 to the interpreter (Executed without errors)#
}