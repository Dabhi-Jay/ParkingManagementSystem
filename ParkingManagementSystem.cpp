// PARKING MANAGEMENT SYSTEM

// INPUT: CHECKIN GJ03DF-2708 08:15am NR
// OUTPUT:A-2
// INPUT: CHECKIN GJT-3293 09:00am R
// OUTPUT:A-1
// INPUT:CHECKOUT GJ03DF-2708 11:45am
// OUTPUT:80
// INPUT:CHECKOUT GJT-3293 12:00pm
// OUTPUR:80
// INPUT:CHECKIN GJ09DF-9687 12:15pm NR
// OUTPUT:A-2
// INPUT:CHECKOUT GJ09DF-9687 12:45pm
// OUTPUT:50
// INPUT:GENERATE REPORT
// OUTPUT:

// PARKING SLOT, CAR NO, CHECK IN TIME, CHECK OUT TIME, CHARGES, CATEGORY
// A-1, GJT-3293, 09:00am, 12:00pm 80, R
// A-2, GJ03DF-2708, 08:15am, 11:45am, 80, NR
// A-2, GJ09DF-9687, 12:15pm, 12:45pm, 50, NR
#include <bits/stdc++.h>
using namespace std;
bool checkin(string &s, vector<vector<int>> &matrix, vector<string> &slot,int &z)
{
    int count = 0;
    int x = -1, y = -1;
    bool flag = true;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (matrix[i][j] == 0 && s == "NR")
            {
                matrix[i][j] = 2;
                x = i;
                y = j;
                flag = false;
                break;
            }
            else if (matrix[i][j] == 1 && s == "R")
            {
                matrix[i][j] = 3;
                x = i;
                y = j;
                flag = false;
                break;
            }
            if (matrix[i][j] == 3 && s == "R")
            {
                count++;
            }
            if (count == z)
            {
                i = 0;
                j = -1;
                s = "NR";
                count = 0;
                cout << "Since all the reserved slots are full we will park this vehicle to Non-reserved slot\n";
            }
        }
        if (!flag)
        {
            break;
        }
    }
    if (x == -1 && y == -1)
    {
        cout << "PARKING FULL\n";
        return false;
    }
    char ch = x + 65;
    cout << ch << "-" << y + 1 << endl;
    string temp2 = to_string(y + 1);
    string temp;
    temp += ch;
    temp += "-";
    temp += temp2;
    slot.push_back(temp);
    return true;
}
vector<string> splitByDelimeter(const string &s,char ch)
{
    vector<string> ans;
    string temp;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] != ch)
        {
            temp += s[i];
        }
        else
        {
            ans.push_back(temp);
            temp = "";
        }
    }
    ans.push_back(temp);
    return ans;
}
int main()
{
    vector<string> ans;
    int n = 5;
    int x = 10;
    vector<string> reservedSlots = {"A-1", "A-10", "B-2", "B-5", "C-1", "C-8", "D-2", "D-4", "E-5", "E-10"};
    int z=reservedSlots.size();
    vector<vector<int>> matrix;

    vector<string> slot;
    vector<string> number;
    vector<string> timeIn;
    vector<string> timeOut;
    vector<int> charge;
    vector<string> category;

    vector<int> temp(x);
    for (int i = 0; i < n; i++)
    {
        vector<int> temp(x);
        for (int j = 0; j < x; j++)
        {
            temp[j] = 0;
        }
        matrix.push_back(temp);
    }
    for (int i = 0; i < reservedSlots.size(); i++)
    {
        int x = (reservedSlots[i][0] - 'A');
        string str = reservedSlots[i].substr(2);
        int y = stoi(str) - 1;
        matrix[x][y] = 1;
    }

    while (true)
    {
        string input;
        getline(cin, input);
        vector<string> inp = splitByDelimeter(input,' ');
        if (inp[0] == "CHECKIN")
        {
            if (checkin(inp[3], matrix, slot,z))
            {
                number.push_back(inp[1]);
                timeIn.push_back(inp[2]);
                category.push_back(inp[3]);
                timeOut.push_back("");
                charge.push_back(0);
            }
        }
        else if (inp[0] == "CHECKOUT")
        {
            if (number.size() == 0)
            {
                cout << "There is no vehicle in parking slot\n";
            }
            else
            {
                bool flag = false;
                int index;
                for (int i = number.size(); i >=0; i--)
                {
                    if (inp[1] == number[i])
                    {
                        index = i;
                        flag = true;
                        timeOut[i] = inp[2];
                        int minutes = 0;
                        if (timeOut[i].substr(5) != timeIn[i].substr(5))
                        {
                            minutes = 60 - stoi(timeIn[i].substr(3, 2)) + ((12 - (stoi(timeIn[i].substr(0, 2)) + 1)) * 60);
                            if (stoi(timeOut[i].substr(0, 2)) == 12)
                            {
                                minutes += stoi(timeOut[i].substr(3, 2));
                            }
                            else
                            {
                                minutes += stoi(timeOut[i].substr(0, 2)) * 60;
                                minutes += stoi(timeOut[i].substr(3, 2));
                            }
                        }
                        else
                        {
                            if ((stoi(timeOut[i].substr(0, 2)) - stoi(timeIn[i].substr(0, 2))) < 0)
                            {
                                minutes = 250;
                            }
                            else if (timeOut[i].substr(0, 2) == timeIn[i].substr(0, 2))
                            {
                                if ((stoi(timeOut[i].substr(3, 2)) - stoi(timeIn[i].substr(3, 2))) < 0)
                                {
                                    minutes = 250;
                                }
                                else
                                {
                                    minutes += (stoi(timeOut[i].substr(3, 2)) - stoi(timeIn[i].substr(3, 2)));
                                }
                            }
                            else
                            {
                                int l = (60 - stoi(timeIn[i].substr(3, 2)));
                                int m = stoi(timeOut[i].substr(0, 2));
                                int p = stoi(timeIn[i].substr(0, 2)) + 1;
                                m = ((m - p) * 60);
                                int o = (stoi(timeOut[i].substr(3, 2)));
                                minutes += l + m + o;
                            }
                        }
                        if (minutes <= 120)
                        {
                            charge[i] = 50;
                        }
                        else if (minutes > 120 && minutes <= 240)
                        {
                            charge[i] = 80;
                        }
                        else
                        {
                            charge[i] = 100;
                        }
                        cout << charge[i] << "\n";
                        break;
                    }
                }
                if (flag == false)
                {
                    cout << "Vehicle not found\n";
                }
                else
                {
                    int idx1 = slot[index][0] - 'A';
                    int idx2 = (stoi(slot[index].substr(2))) - 1;
                    if (matrix[idx1][idx2] == 2)
                    {
                        matrix[idx1][idx2] = 0;
                    }
                    else if (matrix[idx1][idx2] == 3)
                    {
                        matrix[idx1][idx2] = 1;
                    }
                }
            }
        }

        if (input == "GENERATE REPORT")
        {
            break;
        }
    }
    cout << "PARKING SLOT, CAR NO, CHECK IN TIME, CHECK OUT TIME, CHARGES, CATEGORY\n";
    string temporary = "";
    for (int index = 0; index < timeIn.size(); index++)
    {
        if (charge[index] != 0)
        {
            temporary += slot[index] + "," + timeIn[index] + "," + number[index] + "," + timeOut[index] + "," + to_string(charge[index]) + "," + category[index];
            ans.push_back(temporary);
            temporary = "";
        }
        
    }
    sort(ans.begin(), ans.end());
    for (int i = 0; i < ans.size(); i++)
    {
        vector<string>ans2=splitByDelimeter(ans[i],',');
        cout<<ans2[0]<<", "<<ans2[2]<<", "<<ans2[1]<<", "<<ans2[3]<<", "<<ans2[4]<<", "<<ans2[5]<<"\n";
    }
    return 0;
}
