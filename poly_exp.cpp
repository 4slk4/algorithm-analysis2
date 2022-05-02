///////////////////////////////////////////////////////////////////////////////
// poly_exp.cpp
//
// Definitions for two algorithms:
//
// soccer_exhaustive
// soccer_dyn_prog
//
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <stdexcept>
#include "poly_exp.hpp"

int algorithms::soccer_exhaustive(const std::vector<std::string> field) {
    
    /* Start checking for exception */
    if (field.empty())
        throw std::invalid_argument("Row cannot be empty");

    if (field[0].empty())
        throw std::invalid_argument("Column cannot be empty");
    
    int row_len = field.size();
    int col_len = field[0].size();
    int len = row_len + col_len - 2;

    if (row_len > 31 || col_len > 31)
        throw std::invalid_argument("Test is too long");

    for (auto e : field){
        if (e.size() != col_len)
            throw std::invalid_argument("Wrong column size");
    }

    for (auto row : field){
        for (auto col : row){
            if (col != '.' && col != 'X')
                throw std::invalid_argument("Invalid character");
        }
    }
    /* End checking for exception */
    
    int counter = 0;
    //Check if the field has only one spot
    if (len == 0){
        if (field[0] == ".")
            counter++;
    }
    if (len > 0){
        for (size_t bits = 0; bits <= pow(2,len) - 1; bits++){
            //Generate a candidate
            std::vector<std::string> candidate = {"start"};           
            for (size_t k = 0; k <= len - 1; k++){
                size_t bit = (bits >> k) & 1;
                
                if (bit == 1)
                    candidate.push_back("r"); //move right
                else
                    candidate.push_back("d"); //move down
            }

            //Check if the candidate is valid
            int i = 0;
            int j = 0;
            for (size_t s = 1; s < candidate.size(); s++){
                if (candidate[s] == "d"){
                    if (i < row_len-1)
                        i++;
                    else
                        break;
                }
                if (candidate[s] == "r"){
                    if (j < col_len-1)
                        j++;
                    else
                        break;
                }
                if (field[i][j] == 'X')
                    break;
            }
            
            if (i == row_len-1 && j == col_len-1){
                if (field[i][j] != 'X')
                    counter++;
            }           
        }
    }
    return counter;
}
 
int algorithms::soccer_dyn_prog(const std::vector<std::string> field) {
    /* Start checking for exception */
    if (field.empty())
        throw std::invalid_argument("Row cannot be empty");

    if (field[0].empty())
        throw std::invalid_argument("Column cannot be empty");
    
    int row_len = field.size();
    int col_len = field[0].size();

    for (auto e : field){
        if (e.size() != col_len)
            throw std::invalid_argument("Wrong column size");
    }

    for (auto row : field){
        for (auto col : row){
            if (col != '.' && col != 'X')
                throw std::invalid_argument("Invalid character");
        }
    }
    /* End checking for exception */

    //Corner case: initial cell is impassible
    if (field[0][0] == 'X')
        return 0;
    
    //Create memoization
    int A[row_len][col_len];

    //General cases
    for (size_t i = 0; i < row_len; i++){
        for (size_t j = 0; j < col_len; j++){
            if (field[i][j] == 'X'){
                A[i][j] = 0;
                continue;
            }

            int above = 0, left = 0;

            if (i>0 && field[i-1][j] == '.')
                above = A[i-1][j];
            if (j>0 && field[i][j-1] == '.')
                left = A[i][j-1];
            if (i == 0 && j == 0)
                A[i][j] = 1; //Base case
            else
                A[i][j] = above + left;  
        }
    }
    
    return A[row_len-1][col_len-1];
}
