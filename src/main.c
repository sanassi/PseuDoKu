#include "processing/Processing.h"

#include "solver.h"
#include "solver_tools.h"
#include "file_handling.h"

int main(int argc, char *argv[])
{
        if (argc != 2)
        {
                return 1;
        }
        GetGrid(argv[1]);
	SolveGrid("grid");
        return 0;
}
