#
    This is a block comment!

    This program prints HelloWorld by calling the print function in main.
    The print function then calls output, which then prints out HelloWorld.
#

# This is a line comment! #

# Currently, nothing can be written on the same line as a function declaration. Place '{' below function declaration. #

# This program is overly complicated to demonstrate the current supported functionality of ICE #

# Declare a function called "print" with a return type of void, that takes 1 parameter called "text" with type "string" #

Function print : void ($text : string)
{
    $o : string; # Declare a variable called "o" with type "string" #
    o = text; # Set o equal to text. #
    $b : string; # Declare a variable called "b" with type "string" #
    b = o; # Set b equal to o. #
    output(0, b); # Call output (core function), passing in 0 (print to console) and o (print this value) #
}

# Declare a function called "main" with a return type of int, that takes 0 parameters. #

Function main : int (void)
{
    $msg : string; # declare a variable called "msg" with type "string" #
    msg = "HelloWorld!\n"; # set msg to the value "HelloWorld!" #
    print(msg); # call the print function, passing msg a an argument #
    return(0); # return 0 to the interpreter (Executed without errors) / Not implemented yet #
}