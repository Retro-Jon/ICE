# ICE
An interpreted programming language

# Basics

$(variable_name) - declare variable
@(label_name) - define a label
(variable) : (type) - set the datatype of a variable
(function) : (type) - set the return type of a function (use void to return nothing)
; - end a line

# Function Declaration

Function main : int (null) # define a function called main with a return type of int that takes 0 (null) arguments
{
  return(0); # return 0
}

# Function Calls

Function print : void ($text)
{
  output(0, text); # print text to console (0)
}

Function main : int (null)
{
  print("HelloWorld!"); # run print function and pass "HelloWorld!" into text
  return(0);
}
