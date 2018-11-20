#include <iostream>
#include <vector>
#include <string>
using namespace std;
#define MATRIX_SIZE 5


vector< vector<char> > cipher_matrix(string key)
{
    const int LIST_SIZE = 25;
    vector<char> letter_list;
    vector<char> result_list;
    for(char letter = 'a'; letter <= 'z' ; letter++)
    {
        if(letter != 'j') letter_list.push_back(letter);
    }
    for(int i= 0; i <  key.length(); i++)
    {
        for(int j = 0; j < LIST_SIZE; j++)
        {
            if(key[i] == letter_list[j])
            {
                result_list.push_back(key[i]);
                letter_list[j] = '-';
                break;
            }
        }
    }
    for(int i = 0 ; i < LIST_SIZE ; i++)
    {
        if(letter_list[i] != '-') result_list.push_back(letter_list[i]);
    }

    vector< vector<char> > matrix;
    vector<char> temp_vector;
    for(int i = 0; i < result_list.size(); i++)
    {
        temp_vector.push_back(result_list[i]);
        if((i+1) % MATRIX_SIZE == 0)
        {
            matrix.push_back(temp_vector);
            temp_vector.clear();
            //         cout << temp_vector.size();
        }
    }
    return matrix;
}
void print_matrix(vector< vector<char> > matrix )
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for(int j = 0; j < matrix[i].size(); j++)
        {
            cout << matrix[i][j] << '\t';
        }
        cout << endl;
    }
}
int shift_left_up(int position)
{
    return (position + 4) % MATRIX_SIZE;
}

int shift_right_down(int position)
{
    return (position + 1) % MATRIX_SIZE;
}
string cipher (string input, vector< vector<char> > matrix )
{
    string output = "";
    string init = "";
    char first = input[0];
    char second = input[1];
    int first_row = 0, first_col = 0;
    int second_row = 0, second_col = 0;
    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        for(int j = 0; j < MATRIX_SIZE; j++)
        {
            if(first == matrix[i][j])
            {
                first_row = i;
                first_col = j;
            }
            if(second == matrix[i][j])
            {
                second_row = i;
                second_col = j;
            }
        }
    }
    if(first_row == second_row)
       output = init + matrix[first_row][shift_right_down(first_col)] + matrix[second_row][shift_right_down(second_col)];
    else if(first_col == second_col)
            output = init + matrix[shift_right_down(first_row)][first_col] + matrix[shift_right_down(second_row)][second_col];

    else
        output = init + matrix[first_row][second_col] + matrix[second_row][first_col];
    return output;
}

string decipher(string input,vector<vector<char>>matrix){
    string output = "";
    string init = "";
    char first = input[0];
    char second = input[1];
    int first_row = 0, first_col = 0;
    int second_row = 0, second_col = 0;
    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        for(int j = 0; j < MATRIX_SIZE; j++)
        {
            if(first == matrix[i][j])
            {
                first_row = i;
                first_col = j;
            }
            if(second == matrix[i][j])
            {
                second_row = i;
                second_col = j;
            }
        }
    }
    if(first_row == second_row)
       output = init + matrix[first_row][shift_left_up(first_col)] + matrix[second_row][shift_left_up(second_col)];

    else if(first_col == second_col)
           output = init + matrix[shift_left_up(first_row)][first_col] + matrix[shift_left_up(second_row)][second_col];
    else
        output = init + matrix[first_row][second_col] + matrix[second_row][first_col];
    return output;
}

string encode(string pstr,string key){
    string temp_str = "";
    string output="";
    for(int i = 0 ; i < pstr.length() ; i++)
    {
        if(pstr[i] != ' ')  temp_str +=  pstr[i];
    }
    pstr = temp_str;
    temp_str = "";
    for(int i = 0 ; i < key.length() ; i++)
    {
        if(key[i] != ' ')  temp_str +=  key[i];
    }
    key = temp_str;

    if((pstr.length() % 2) == 1) pstr += "x";
    vector< vector<char> > matrix = cipher_matrix(key);
    string process_pair = "";
    while(true)
    {
        if(pstr.length() == 1)
        {
            process_pair = pstr + "x";
        }
        else
        {
            process_pair = pstr.substr(0,2);
            if(process_pair[0] == process_pair[1])
            {
                pstr = process_pair.substr(1,1) + pstr;
                process_pair = process_pair.substr(0,1) + "x";

            }
        }
        process_pair = cipher(process_pair, matrix);
        output +=  process_pair;
        if(pstr.length() > 2)
        {
            pstr = pstr.substr(2);

        }
        else break;
    }
    return output;
}

string decode(string pstr,string key){
    string temp_str = "";
    string output="";
    for(int i = 0 ; i < pstr.length() ; i++)
    {
        if(pstr[i] != ' ')  temp_str +=  pstr[i];
    }
    pstr = temp_str;
    temp_str = "";
    for(int i = 0 ; i < key.length() ; i++)
    {
        if(key[i] != ' ')  temp_str +=  key[i];
    }
    key = temp_str;

    if((pstr.length() % 2) == 1) pstr += "x";
    vector< vector<char> > matrix = cipher_matrix(key);
    string process_pair = "";
    while(true)
    {
        if(pstr.length() == 1)
        {
            process_pair = pstr + "x";
        }
        else
        {
            process_pair = pstr.substr(0,2);
            if(process_pair[0] == process_pair[1])
            {
                pstr = process_pair.substr(1,1) + pstr;
                process_pair = process_pair.substr(0,1) + "x";

            }
        }
        process_pair = decipher(process_pair, matrix);
        output +=  process_pair;
        if(pstr.length() > 2)
        {
            pstr = pstr.substr(2);

        }
        else break;
    }
    return output;
}
