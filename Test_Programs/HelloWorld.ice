Function TestB : void (null)
{
    @TEST_B;
}

Function Test : void (null)
{
    @Test_Start;
    TestB();
    @Test_End;
    TestB();
}

Function main : int (null)
{
    Test();
}