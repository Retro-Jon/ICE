# ICE

An interpreted programming language

# Compiling the Interpreter

Uses Mingw-w64
Should work with other compilers if they support the following libraries:
1. vector

# Basics

$(variable_name) - declare variable

@(label_name) - define a label

(variable) : (type) - set the datatype of a variable

(function) : (type) - set the return type of a function (use void to return nothing)

; - end a line

'# Comment #' - Comment

# Function Declaration

Function main : int (null)
{
  return(0); # return 0 #
}

# Function Calls

Function print : void ($text)
{
  output(0, text);
}

Function main : int (null)
{
  print("HelloWorld!");
  return(0);
}
