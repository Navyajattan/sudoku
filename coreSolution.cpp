#include <iostream>
//#include <stdlib.h>
#include "coreSolution.h"
#include "Sudoku.h"
#include "basicDeff.h"

#ifdef _GENERATOR_V1
    #include <stdlib.h>
    #include <time.h>
#endif // _GENERATOR_V1

Sudoku Solver::SudokuSolution;
bool Solver::IsSolutionSet = false;
int Solver::Count = 0;
int Solver::GlobalPossibilities[9][9];
void Solver::initializeGP()
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			GlobalPossibilities[i][j] = POS_ALL;
}
void Solver::SetCurPuzzle(Sudoku P)
{
	int i, j;
	for (i = 0; i < 9; ++i)
		for (j = 0; j < 9; ++j)
			(*CurPuzzle.RetGrid())[i][j] = (*P.RetGrid())[i][j];
}


bool Solver::RecrussiveSolve()
{
	//----------------------------------------
	Solver::Count++;
	//---------------------------------------
	PriorityUnit Unit;
	Solver solve;
	#ifndef _GENERATOR_V1
        int temp_pos, temp;
	#else
        int temp_pos;
        int temp[9];
	#endif // _GENERATOR_V1
	int i;
	int size;
	CurPuzzle.reinitializepos();
	while (CurPuzzle.Solve());
	//-------------------------------------------------------------------------------
	if (Solver::Count % 10000000 == 0)
	{
		for (int i = 0; i<9; ++i)
		{
			for (int j = 0; j<9; ++j)
			{
				std::cout << (*CurPuzzle.RetGrid())[i][j] << "  ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
		//system("pause");
	}
	//-------------------------------------------------------------------------------
	if (CurPuzzle.FullPossibility())  return false;
	//else
		//std::cout << "got you!";
	CurPuzzle.GeneratePossibilityCount();
	CurPuzzle.GenerateInversePossibilityCount();
	CurPuzzle.SetPossibilityCount();
	Q = CurPuzzle.GenerateWaightValues();
	if (CurPuzzle.IsSolved())
	{
		if (CurPuzzle.IsLegal() )
		{
			Solver::SudokuSolution = CurPuzzle;
			Solver::IsSolutionSet = true;
			return true;
		}
		else
			return false;
	}

	solve.SetCurPuzzle(CurPuzzle);
	//while (!Q.empty())
	//{
		Unit = Q.top();
		temp_pos = (*CurPuzzle.RetPoss())[Unit.y][Unit.x];
		//temp_pos = temp_pos & Solver::GlobalPossibilities[Unit.y][Unit.x];
		size = Sudoku::NoOfElements(temp_pos);
			#ifdef _GENERATOR_V1
                int j, T;
                for(i=0; i<size; ++i)
                {
                    temp[i] = Sudoku::BitValue(temp_pos);
                    Sudoku::DeleteValue(temp[i], temp_pos);
                }
                srand(time(NULL));
                for(i=0; i<size; ++i)
                {
                    j = (rand() % size);
                    T = temp[j];
                    temp[j] = temp[i];
                    temp[i] = T;
                }
			#endif // _GENERATOR_V1
			for (i = 0; i < size; ++i)
			{
			    #ifndef _GENERATOR_V1
                    temp = (*solve.CurPuzzle.RetGrid())[Unit.y][Unit.x] = Sudoku::BitValue(temp_pos);
                    Sudoku::DeleteValue(temp, temp_pos);
				#else // _GENERATOR_V1
                        (*solve.CurPuzzle.RetGrid())[Unit.y][Unit.x] = temp[i];
				#endif
				if (solve.RecrussiveSolve())
					return true;
				// it is obvious that 'false' is returned. So this value should not (never ever ever!!!) be
				// checked again. Or else, we would be wasting a lot of time!!!!
				//Sudoku::DeleteValue(temp, Solver::GlobalPossibilities[Unit.y][Unit.x]);
				solve.SetCurPuzzle(CurPuzzle);
			}
		(*solve.CurPuzzle.RetGrid())[Unit.y][Unit.x] = 0;
		Q.pop();
	//}
	return false;
}
