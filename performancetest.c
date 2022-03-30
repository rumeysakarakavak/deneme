extern int getopt_musl(int argc, char * const argv[], const char *optstring);

static double GetoptFunctionEfficiencyExec(int (GetoptFunc) (int, const char **, const char *), int argc,
                                           char * const argv[], const char *optstring, int loopcnt)
{
    uint32_t count = 0;
    double timeSpent = 0.0;
    clock_t begin;
    clock_t end;
 
    optind = 1;
    begin = clock();
    while (count < ITERATION_COUNT) {
        if (count % loopcnt == 0) {
            optind = 1;
        }

        GetoptFunc(argc, argv, optstring);
        count++;
    }
 
    end = clock();
    timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

    return timeSpent;
}

static void GetoptPerformanceTestRun(int argc, char * const argv[], const char *optstring, int loopcnt)
{
    double elibcScore;
    double muslScore;
 
    elibcScore = GetoptFunctionEfficiencyExec(getopt, argc, argv, optstring, loopcnt);
    muslScore  = GetoptFunctionEfficiencyExec(getopt_musl, argc, argv, optstring, loopcnt);
    printf("|===================|====================|===================|\n");
    printf("|  Rate: %+.6f  |  Elibc: %+.6f  |  Musl: %+.6f  |\n", (muslScore - elibcScore) / muslScore,
           elibcScore, muslScore);
    printf("|===================|====================|===================|\n\n");
 
    DT_ASSERT_TRUE(muslScore > 0.0f);
    DT_ASSERT_TRUE(elibcScore > 0.0f);
    DT_ASSERT_TRUE(elibcScore < muslScore);
}

void UT_Misc_Test019(void)
{
    int argc = 6;
    int loopcnt = 5;
    char *opstring = "if:lrx";
    char *argv[] = { "./a.out", "-i", "-f", "file.txt", "-lr", "-x" };

    GetoptPerformanceTestRun(argc, argv, opstring, loopcnt);
}

void UT_Misc_Test020(void)
{
    int argc = 13;
    int loopcnt = 11;
    char *opstring = "-:a:b:X";
    char *argv[] = { "./a.out", "-t", "x", "-a", "foo", "-M", "y", "-b", "bar", "z", "-X", "w", "-b" };

    GetoptPerformanceTestRun(argc, argv, opstring, loopcnt);
}