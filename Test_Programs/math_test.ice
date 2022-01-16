ICE Fetch Libs/math.ice;

Function main : int (void)
{
    $in_a : int;
    $in_b : int;
    $result : int;

    output(0, "Enter 2 integers to do math on.\n");
    input(0, in_a);
    input(0, in_b);

    result = add(in_a, in_b);
    print_result("addition", result);

    result = sub(in_a, in_b);
    print_result("subtraction", result);
    
    result = mul(in_a, in_b);
    print_result("multiplication", result);
    
    result = div(in_a, in_b);
    print_result("division", result);

    result = fact(in_a);
    print_result("factorial", result);

    result = tri(in_a);
    print_result("triangular", result);

    return(0);
}
